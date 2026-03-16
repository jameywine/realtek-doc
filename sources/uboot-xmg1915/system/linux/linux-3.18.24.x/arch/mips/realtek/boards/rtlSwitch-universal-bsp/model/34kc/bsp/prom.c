/*
 * Realtek Semiconductor Corp.
 *
 * bsp/prom.c
 *     bsp early initialization code
 *
 * Copyright (C) 2006-2012 Tony Wu (tonywu@realtek.com)
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/bootinfo.h>
#include <asm/addrspace.h>
#include <asm/page.h>
#include <asm/cpu.h>
#include <asm/prom.h>
#include <linux/ioport.h>
#include "bspcpu.h"
#include "bspchip.h"
#include "swcore.h"
#include <int_setup.h>

#include <asm/mips-cm.h>
#include <asm/mips-cpc.h>


unsigned int bsp_chip_id, bsp_chip_rev_id;
unsigned int bsp_chip_family_id, bsp_chip_type;

unsigned int bsp_Interrupt_srcID_Mapping_IRQ[GIC_NUM_INTRS];
unsigned int bsp_Interrupt_IRQ_Mapping_IP[ICTL_MAX_IP];
extern interrupt_mapping_conf_t rtl839X8X_intr_mapping[];
extern interrupt_mapping_conf_t rtl9300_intr_mapping[];
extern interrupt_mapping_conf_t rtl9310_intr_mapping[];
extern struct gic_intr_map rtl9310_gic_intr_map[];


extern int bsp_drv_swcore_cid_get(unsigned int unit, unsigned int *pCid, unsigned int *pCrevid);

void bsp_interrupt_irq_mapping_setup(void)
{
    int	index;
    int	irq_index;
    unsigned int	src_index;
    int int_type;
    interrupt_mapping_conf_t *intr_mapping_table;
    struct gic_intr_map *gic_mapping_table = NULL;

    if((bsp_chip_family_id == RTL8390_FAMILY_ID)||(bsp_chip_family_id == RTL8350_FAMILY_ID)||(bsp_chip_family_id == RTL8380_FAMILY_ID)||(bsp_chip_family_id == RTL8330_FAMILY_ID))
    {
        intr_mapping_table = rtl839X8X_intr_mapping;
    }
    else if((bsp_chip_family_id == RTL9300_FAMILY_ID))
    {
        intr_mapping_table = rtl9300_intr_mapping;
    }
    else if((bsp_chip_family_id == RTL9310_FAMILY_ID))
    {
        intr_mapping_table = rtl9310_intr_mapping;
        gic_mapping_table = rtl9310_gic_intr_map;
    }else{
        printk("\n Interrupt Mapping failed !!! \n");
        return;
    }

    for(index = 0; index < ICTL_MAX_IP; index++)
    {
        src_index = ((interrupt_mapping_conf_t *)(intr_mapping_table + index)->intr_src_id);
        bsp_Interrupt_IRQ_Mapping_IP[index] = src_index;
        if((bsp_chip_family_id == RTL9310_FAMILY_ID) && (src_index < GIC_NUM_INTRS) && (gic_mapping_table != NULL))
        {
            ((struct gic_intr_map *)(gic_mapping_table + src_index))->cpunum = (unsigned int)((interrupt_mapping_conf_t *)(intr_mapping_table + index)->target_cpu_id);
            ((struct gic_intr_map *)(gic_mapping_table + src_index))->pin = (unsigned int)((unsigned int)((interrupt_mapping_conf_t *)(intr_mapping_table + index)->cpu_ip_id) - 2);
            int_type = (int)((interrupt_mapping_conf_t *)(intr_mapping_table + index)->intr_type);
            if(((interrupt_mapping_conf_t *)(intr_mapping_table + index)->intr_type) != INTR_TYPE_DEFAULT)
            {
                switch(int_type)
                {
                    case INTR_TYPE_HIGH_LEVEL:
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->polarity = GIC_POL_POS;
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->trigtype = GIC_TRIG_LEVEL;
                        break;
                    case INTR_TYPE_LOW_LEVEL:
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->polarity = GIC_POL_NEG;
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->trigtype = GIC_TRIG_LEVEL;
                        break;
                    case INTR_TYPE_FALLING_EDGE:
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->polarity = GIC_POL_NEG;
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->trigtype = GIC_TRIG_EDGE;
                        break;
                    case INTR_TYPE_RISING_EDGE:
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->polarity = GIC_POL_POS;
                        ((struct gic_intr_map *)(gic_mapping_table + src_index))->trigtype = GIC_TRIG_EDGE;
                        break;
                     default:
                        break;
                }
            }
        }
    }

    if((bsp_chip_family_id == RTL9310_FAMILY_ID))
    {
        for(src_index = 0; src_index < GIC_NUM_INTRS; src_index++)
        {
            for(irq_index = 0; irq_index < ICTL_MAX_IP; irq_index++){
                if(bsp_Interrupt_IRQ_Mapping_IP[irq_index] == src_index) /*interrupt src index*/
                    bsp_Interrupt_srcID_Mapping_IRQ[src_index] = irq_index;
            }
        }
    }
}

void bsp_console_init(void)
{
    unsigned int value = 0;
    /* 8 bits, 1 stop bit, no parity. */
    REG32(UART0_LCR) = ((CHAR_LEN_8 | ONE_STOP | PARITY_DISABLE) << 24);
    /* Reset/Enable the FIFO */
    REG32(UART0_FCR) = ((FCR_EN | RXRST | TXRST | CHAR_TRIGGER_14) << 24);
    /* Disable All Interrupts */
    REG32(UART0_IER) = 0x00000000;
    /* Enable Divisor Latch */
    REG32(UART0_LCR) |= (DLAB << 24);
    /* Set Divisor */
   if((bsp_chip_family_id == RTL9310_FAMILY_ID) || (bsp_chip_family_id == RTL8390_FAMILY_ID) || (bsp_chip_family_id == RTL8350_FAMILY_ID)  || (bsp_chip_family_id == RTL8380_FAMILY_ID) || (bsp_chip_family_id == RTL8330_FAMILY_ID))
   {
        value = (SYSCLK / (BAUDRATE * 16) - 1) & 0x00FF;
        REG32(UART0_DLL) = (value << 24);

        value = (((SYSCLK / (BAUDRATE * 16) - 1) & 0xFF00) >> 8);
        REG32(UART0_DLM) = (value << 24);
   }else{

        value = (SYSCLK_9300_MP / (BAUDRATE * 16) - 1) & 0x00FF;
        REG32(UART0_DLL) = (value << 24);

        value = (((SYSCLK_9300_MP / (BAUDRATE * 16) - 1) & 0xFF00) >> 8);
        REG32(UART0_DLM) = (value << 24);
   }

    /* Disable Divisor Latch */
    REG32(UART0_LCR) &= ((~DLAB) << 24);

}
#define  DOR2          0xB8001720
#define  DMAR2         0xB8001724
#define  ZONE2_BASE    0x20000000
#define  ZONE2_SIZE    ((256<<20))       //MB
#define  ZONE2_PHY     0x10000000
#define ZONE2_OFF     (0x80000000 + ZONE2_PHY - ZONE2_BASE)
#define ZONE2_MAX     (ZONE2_OFF | 0xfffffff)

void __init prom_mem_zone2(void){
  REG32(DOR2) = ZONE2_OFF;
  REG32(DMAR2) = ZONE2_MAX;    //1G
  __sync();
  PROM_DEBUG_PRINT("mem zone2: OFF=0x%x,MAX=0x%x\n",REG32(DOR2) ,  REG32(DMAR2));
}
#define SRAMSAR0 0xB8004000
#define  UMSAR0  0xB8001300
void __init prom_sram_disable(void){
  REG32(UMSAR0) = 0;
  REG32(SRAMSAR0) = 0;
}

void __init prom_meminit(void)
{
    char *ptr;
    unsigned int memsize;

    /* Check the command line first for a memsize directive */
    ptr = strstr(arcs_cmdline, "mem=");

    if (ptr)
       memsize = memparse(ptr + 4, &ptr);
    else
       memsize = 0x02000000;  /* Default to 32MB */

    /*
     * call <add_memory_region> to register boot_mem_map
     * add_memory_region(base, size, type);
     * type: BOOT_MEM_RAM, BOOT_MEM_ROM_DATA or BOOT_MEM_RESERVED
     */
//         prom_sram_disable();
    if(bsp_chip_family_id == RTL9300_FAMILY_ID)
    {
        prom_mem_zone2();
    }
    add_memory_region(0, 256<<20, BOOT_MEM_RAM);
    PROM_DEBUG_PRINT("mem zone2: Base=0x%x, size=%d(B)\n",ZONE2_BASE, ZONE2_SIZE);
    add_memory_region(ZONE2_BASE, ZONE2_SIZE, BOOT_MEM_RAM);
}


#ifdef CONFIG_EARLY_PRINTK
static int promcons_output __initdata = 0;

void unregister_prom_console(void)
{
    if (promcons_output)
        promcons_output = 0;
}

void disable_early_printk(void)
    __attribute__ ((alias("unregister_prom_console")));

void prom_putchar(char c)
{
}

char prom_getchar(void)
{
    return '\0';
}
#endif

const char *get_system_type(void)
{
    if(bsp_chip_family_id == RTL8390_FAMILY_ID)
        return "RTL839xM";
    if(bsp_chip_family_id == RTL8350_FAMILY_ID)
        return "RTL835xM";
    if(bsp_chip_family_id == RTL8380_FAMILY_ID)
        return "RTL838xM";
    if(bsp_chip_family_id == RTL8330_FAMILY_ID)
        return "RTL833xM";
    if(bsp_chip_family_id == RTL9300_FAMILY_ID)
        return "RTL9300";
    if(bsp_chip_family_id == RTL9310_FAMILY_ID)
        return "RTL9310";

    return "CHIP recognization error";
}

void __init prom_free_prom_memory(void)
{
    return;
}

/* Do basic initialization */
phys_addr_t mips_cpc_default_phys_base(void)
{
    return CPC_BASE_ADDR;
}
/**************************************************
 * For L2 Cache initialization
**************************************************/
#ifdef CONFIG_L2_CACHE_PRCOESS
extern void init_l2_cache(void);
void __init bsp_setup_scache(void)
{
    unsigned long config2;
    unsigned int tmp;
    config2 = read_c0_config2();
    tmp = (config2 >> 4) & 0x0f;
    /*if enable l2_bypass mode, linesize will be 0       */
    /*if arch not implement L2cache, linesize will be 0  */
    if (0 < tmp && tmp <= 7){ //Scache linesize >0 and <=256 (B)
        init_l2_cache();
    }
}
#endif
void __init prom_init(void)
{
    int i, ret;
    int argc = fw_arg0;
    char **arg = (char **)fw_arg1;

    extern void plat_smp_init(void);
    prom_meminit();
//	mips_set_machine_name(get_system_type());

    bsp_chip_id = 0;
    bsp_chip_rev_id = 0;
    bsp_chip_family_id = 0;
    bsp_chip_type = 0;
    ret = bsp_drv_swcore_cid_get((unsigned int)0, (unsigned int *)&bsp_chip_id, (unsigned int *)&bsp_chip_rev_id);
    if(ret == -1)
    {
        printk("\nbsp_init(), RTK Switch chip is not found!!!\n");
    }else
    {
#if defined(CONFIG_RTL8390_SERIES)
    if((bsp_chip_id & FAMILY_ID_MASK) == RTL8390_FAMILY_ID)
        bsp_chip_family_id = RTL8390_FAMILY_ID;
    if((bsp_chip_id & FAMILY_ID_MASK) == RTL8350_FAMILY_ID)
        bsp_chip_family_id = RTL8350_FAMILY_ID;
#endif
#if defined(CONFIG_RTL8380_SERIES)
    if((bsp_chip_id & FAMILY_ID_MASK) == RTL8380_FAMILY_ID)
        bsp_chip_family_id = RTL8380_FAMILY_ID;
    if((bsp_chip_id & FAMILY_ID_MASK) == RTL8330_FAMILY_ID)
        bsp_chip_family_id = RTL8330_FAMILY_ID;
#endif
#if defined(CONFIG_RTL9300_SERIES)
    if((bsp_chip_id & FAMILY_ID_MASK) == RTL9300_FAMILY_ID)
        bsp_chip_family_id = RTL9300_FAMILY_ID;
#endif
#if defined(CONFIG_RTL9310_SERIES)
    if((bsp_chip_id & FAMILY_ID_MASK) == RTL9310_FAMILY_ID)
        bsp_chip_family_id = RTL9310_FAMILY_ID;
#endif

    }

    mips_set_machine_name(get_system_type());
    bsp_interrupt_irq_mapping_setup();

    if((bsp_chip_family_id == RTL9310_FAMILY_ID))
    {
        /* vv-Early detection of CMP support (For GIC init)*/
 	    mips_cm_probe();
 	    mips_cpc_probe();
        /* ^^-Early detection of CMP support */
    }
#ifdef CONFIG_L2_CACHE_PRCOESS
// 	bsp_setup_scache();
#endif
#ifdef CONFIG_SMP
        plat_smp_init();
#endif

    /* if user passes kernel args, ignore the default one */
    if (argc > 1)
       arcs_cmdline[0] = '\0';

    /* arg[0] is "g", the rest is boot parameters */
    for (i = 1; i < argc; i++) {
        if (strlen(arcs_cmdline) + strlen(arg[i] + 1)
            >= sizeof(arcs_cmdline))
            break;
        strcat(arcs_cmdline, arg[i]);
        strcat(arcs_cmdline, " ");
    }
}
