/*
 * Realtek Semiconductor Corp.
 *
 * bsp/setup.c
 *     bsp interrult initialization and handler code
 *
 * Copyright (C) 2006-2012 Tony Wu (tonywu@realtek.com)
 */
#include <linux/console.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>

#include <asm/addrspace.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <asm/bootinfo.h>
#include <asm/time.h>
#include <asm/reboot.h>

#include <asm/smp-ops.h>
#include <asm/mips-cm.h>
#include <asm/mips-cpc.h>
#include "bspchip.h"
#include "bspcpu.h"
// #include "cpc.h"

#include <swcore.h>

void (* hook_restart_func)(void) = NULL;

void rtk_hook_restart_function(void (*func)(void))
{
    hook_restart_func = func;
    return;
}


/*
 * Reset whole chip
 */
void
rtk_chip_reset(char *command)
{
#if defined(CONFIG_RTL9310_SERIES)
    if (bsp_chip_family_id == RTL9310_FAMILY_ID)
    {
        /*To be continued*/

    }
    else
#endif
#if defined(CONFIG_RTL9300_SERIES)
    if (bsp_chip_family_id == RTL9300_FAMILY_ID)
    {
        /*To be continued*/
        REG32(0xBB00000C) = 0x1;
    }
    else
#endif
#if defined(CONFIG_RTL8390_SERIES)
    if ((bsp_chip_family_id == RTL8390_FAMILY_ID) || (bsp_chip_family_id == RTL8350_FAMILY_ID))
    {
        REG32(0xBB000014) = 0xFFFFFFFF;
    }
    else
#endif
#if defined(CONFIG_RTL8380_SERIES)
    if ((bsp_chip_family_id == RTL8380_FAMILY_ID) || (bsp_chip_family_id == RTL8330_FAMILY_ID))
    {
        uint32 tmp = 0;
        OTTO838x_FLASH_DISABLE_4BYTE_ADDR_MODE();
        REG32(0xBB000040) = 0x1;    /* Reset Global Control Register */
    }
    else
#endif
    {

    }
}

static void bsp_machine_restart(char *command)
{
    if(hook_restart_func != NULL)
    {
        hook_restart_func();
    }


    printk("System restart.\n");
#if 1
    rtk_chip_reset(command);   /* Reset whole chip */
#else
       smp_call_function_single(0, bsp_cpc_reset, NULL,0);
#endif

}

static void bsp_machine_halt(void)
{

    printk("System halted.\n");
    while(1);


}

#ifdef CONFIG_MIPS_MT_SMTC
extern struct plat_smp_ops bsp_smtc_smp_ops;
#endif

extern int bsp_serial_init(void);
/* callback function */
void __init plat_mem_setup(void)
{
    /* define io/mem region */
// 	set_io_port_base((unsigned long) KSEG1);
//  	ioport_resource.start = 0x10000000;
//  	ioport_resource.end = 0x1ffffff;
//
//  	iomem_resource.start = 0x10000000;
//  	iomem_resource.end = 0x1fffffff;

         _machine_restart = bsp_machine_restart;
         _machine_halt = bsp_machine_halt;

    bsp_serial_init();

}

EXPORT_SYMBOL(rtk_hook_restart_function);

