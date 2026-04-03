/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : IOAL Layer Init Module
 *
 * Feature : IOAL Init Functions
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/debug/rt_log.h>
#include <ioal/ioal_init.h>
#include <osal/memory.h>
#include <osal/lib.h>
#include <osal/spl.h>
#include <osal/cpu.h>
#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>
#include <hwp/hw_profile.h>
#include <ioal/mem32.h>

#include <virtualmac/vmac_target.h>

#if defined(CONFIG_SDK_DRIVER_EXTC_PCI)
#include <rtcore/rtcore_pci.h>
#endif

#ifndef __BOOTLOADER__
#include <linux/mm.h>
#include <asm/page.h>
#include <common/util/rt_util_time.h>
#include <rtcore/rtcore.h>
#endif


/*
 * Symbol Definition
 */
#define VIRT_SWCORE_REG_SIZE    (0x1000000 - 16*64*1024)     /*15M */
#define VIRTUAL_SWCORE_TBL_SIZE	(4 * 1024*1024)             /* 4M */

/*
 * Data Declaration
 */
int8 virtualSwReg[VIRT_SWCORE_REG_SIZE];
int8 virtualSwTable[VIRTUAL_SWCORE_TBL_SIZE];
int8 *pVirtualSWReg = (int8 *)0;
int8 *pVirtualSWTable = (int8 *)0;
uintptr swcore_base[RTK_MAX_NUM_OF_UNIT];

ioal_db_t ioal_db[RTK_MAX_NUM_OF_UNIT];
uint32 ioal_init_status[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED}; /* won't check, since ioal may init multiple times */

ioal_phy_base_addr_t rtk_chip_phy_base[] =
{
    [RTK_CHIP_CPU_MIPS] =
    {
        .phy_soc_base = SOC_PHYS_BASE,
        .phy_sram_base = SRAM_PHYS_BASE,
        .phy_swcore_base = SWCORE_PHYS_BASE,
        .phy_swcore_size = SWCORE_MEM_SIZE,
    },
    [RTK_CHIP_CPU_ARM64] =
    {
        .phy_soc_base = SOC_ARM_PHYS_BASE,
        .phy_sram_base = SRAM_ARM_PHYS_BASE,
        .phy_swcore_base = SWCORE_ARM_PHYS_BASE,
        .phy_swcore_size = SWCORE_ARM_MEM_SIZE,
    },
};

#if defined(CONFIG_TARGET_ARCH_ARM)
uint32 ioal_init_cpu_type = RTK_CHIP_CPU_ARM64;
#elif defined(CONFIG_TARGET_ARCH_MIPS)
uint32 ioal_init_cpu_type = RTK_CHIP_CPU_MIPS;
#elif defined(CONFIG_TARGET_ARCH_X86)
uint32 ioal_init_cpu_type = RTK_CHIP_CPU_MIPS;
#else
uint32 ioal_init_cpu_type = RTK_CHIP_CPU_MIPS;
#endif

osal_spinlock_t pci_spl_lock = 0;
uint32 ioal_avoidSimultaneousMulticoreAcc = 0;


/*
 * Macro Declaration
 */


/*
 * Function Declaration
 */

/* Function Name:
 *      ioal_init_memRegion_get
 * Description:
 *      Get memory base address
 * Input:
 *      unit      - unit id
 *      mem       - memory region
 * Output:
 *      pBaseAddr - pointer to the base address of memory region
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
ioal_init_memRegion_get(uint32 unit, ioal_memRegion_t mem, uintptr *pBaseAddr)
{
    enum IC_TYPE ictype;

    vmac_getTarget(&ictype);

    switch(mem)
    {
        case IOAL_MEM_SWCORE:
            *pBaseAddr = ioal_db[unit].swcore_base;
            break;

        case IOAL_MEM_SOC:
            *pBaseAddr = ioal_db[unit].soc_base;
            break;

        case IOAL_MEM_SRAM:
            *pBaseAddr = ioal_db[unit].sram_base;
            break;
#ifdef CONFIG_SDK_DRIVER_NIC_USER_MODE
        case IOAL_MEM_DMA:
            *pBaseAddr = ioal_db[unit].pkt_base;
            break;

        case IOAL_MEM_L2NTFY_RING:
            *pBaseAddr = ioal_db[unit].l2ntfy_ring_base;
            break;

        case IOAL_MEM_L2NTFY_BUF:
            *pBaseAddr = ioal_db[unit].l2ntfy_buf_base;
            break;

        case IOAL_MEM_L2NTFY_USR:
            *pBaseAddr = ioal_db[unit].l2ntfy_usr_base;
            break;

        case IOAL_MEM_DESC:
            *pBaseAddr = ioal_db[unit].desc_base;
            break;
#endif

        case IOAL_MEM_REG_SIZE:
            *pBaseAddr = ioal_db[unit].swcore_size;
            break;
        default:
            return RT_ERR_FAILED;
    }

    switch (ictype)
    {
        case IC_TYPE_MODEL:
            if (IOAL_MEM_SWCORE == mem)
                *pBaseAddr = swcore_base[unit];
            else
                return RT_ERR_FAILED;
            break;
        default:
            break;
    }

    return RT_ERR_OK;
} /* end of ioal_init_memRegion_get */

/* Function Name:
 *      ioal_init_memBase_set
 * Description:
 *      Set memory base address
 * Input:
 *      unit      - unit id
 *      mem       - memory region
 * Output:
 *      baseAddr - the base address of memory region
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
ioal_init_memRegion_set(uint32 unit, ioal_memRegion_t mem, uintptr baseAddr)
{
    switch(mem)
    {
        case IOAL_MEM_SWCORE:
            ioal_db[unit].swcore_base = baseAddr;
            break;

        case IOAL_MEM_SOC:
            ioal_db[unit].soc_base = baseAddr;
            break;

        case IOAL_MEM_SRAM:
            ioal_db[unit].sram_base = baseAddr;
            break;

        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
} /* end of ioal_init_memRegion_set */

int32
_ioal_init_memmap(uint32 unit)
{
    ioal_db[unit].soc_base      = osal_memory_baseAddr_p2v(rtk_chip_phy_base[ioal_init_cpu_type].phy_soc_base);
    ioal_db[unit].sram_base     = osal_memory_baseAddr_p2v(rtk_chip_phy_base[ioal_init_cpu_type].phy_sram_base);
    ioal_db[unit].swcore_base   = osal_memory_baseAddr_p2v(rtk_chip_phy_base[ioal_init_cpu_type].phy_swcore_base);
    ioal_db[unit].swcore_size   = rtk_chip_phy_base[ioal_init_cpu_type].phy_swcore_size;

    RT_LOG(LOG_DEBUG, MOD_INIT, "[%s] %d soc_base    = 0x%X -> 0x%X\n"   , __FUNCTION__, unit, rtk_chip_phy_base[ioal_init_cpu_type].phy_soc_base, (uint32)ioal_db[unit].soc_base);
    RT_LOG(LOG_DEBUG, MOD_INIT, "[%s] %d sram_base   = 0x%X -> 0x%X\n", __FUNCTION__, unit, rtk_chip_phy_base[ioal_init_cpu_type].phy_sram_base, (uint32)ioal_db[unit].sram_base );
    RT_LOG(LOG_DEBUG, MOD_INIT, "[%s] %d swcore_base = 0x%X -> 0x%X\n", __FUNCTION__, unit, rtk_chip_phy_base[ioal_init_cpu_type].phy_swcore_base, (uint32)ioal_db[unit].swcore_base );
    RT_LOG(LOG_DEBUG, MOD_INIT, "[%s] %d swcore_size = 0x%X\n", __FUNCTION__, unit, ioal_db[unit].swcore_size);

    return RT_ERR_OK;
}

#if defined(CONFIG_SDK_DRIVER_EXTC_PCI)
int32
_ioal_init_pcie(uint32 unit)
{
    int32   ret = RT_ERR_OK;
    uint32  num_cores = 0;
    rtcore_pci_dev_t    p;
    rtcore_pci_info_get(HWP_SWCORE_PCIE_BUS(unit), RT_PCIDEV_SWCORE, &p);
    ioal_db[unit].swcore_base   = p.ioaddr;
    ioal_db[unit].swcore_size   = p.mem_size;


    RT_ERR_CHK_EHDL(osal_cpu_num_cores_get(&num_cores), ret,
                    { RT_INIT_ERR(ret, (MOD_INIT), "_ioal_init_pcie fail"); });

    if ((num_cores > 1) && (HWP_CPU_EMBEDDED())
            && (RTK_CHIP_ID(RTK_SOC_UNIT_ID) == RTL9311BM_CHIP_ID)
            && (RTK_CHIP_REV(RTK_SOC_UNIT_ID) == CHIP_REV_ID_A))
    {
        if (0 == pci_spl_lock)
        {
            pci_spl_lock = osal_spl_spin_lock_create();
            if (0 == pci_spl_lock)
            {
                RT_ERR(RT_ERR_FAILED, (MOD_INIT), "spin-lock create failed");
                return RT_ERR_FAILED;
            }
        }

        ioal_avoidSimultaneousMulticoreAcc = 1;
    }

    return RT_ERR_OK;
}
#endif

void
_ioal_swcore_acc_method_init(uint32 unit)
{
    if (HWP_NOT_INIT_YET())
    {
        ioal_db[unit].acc_method =  IOAL_SWCORE_ACC_MEM;
    }
    else
    {
        switch (HWP_SWCORE_ACC_METHOD(unit))
        {
            case HWP_SW_ACC_MEM:
                ioal_db[unit].acc_method = IOAL_SWCORE_ACC_MEM;
                break;
            case HWP_SW_ACC_SPI:
                ioal_db[unit].acc_method = IOAL_SWCORE_ACC_SPI;
                break;
            case HWP_SW_ACC_PCIe:
                ioal_db[unit].acc_method = IOAL_SWCORE_ACC_PCIE;
                break;
            case HWP_SW_ACC_I2C:
                ioal_db[unit].acc_method = IOAL_SWCORE_ACC_I2C;
                break;
            case HWP_SW_ACC_SIM:
                ioal_db[unit].acc_method = IOAL_SWCORE_ACC_SIM;
                break;
            default:
                RT_LOG(LOG_WARNING, MOD_INIT, "[%s] u:%d bad swcore_access_method in hwp: %d\n", __FUNCTION__, unit, HWP_SWCORE_ACC_METHOD(unit));
                ioal_db[unit].acc_method = IOAL_SWCORE_ACC_NONE;
                break;
        }
    }
}

int32
ioal_init(uint32 unit)
{
    int32 ret = RT_ERR_OK;

    RT_INIT_MSG("    IOAL init (unit %d)\n", unit);

    pVirtualSWReg = virtualSwReg;
    swcore_base[unit] = (uintptr)pVirtualSWReg;
    pVirtualSWTable = virtualSwTable;

    if (INIT_COMPLETED == ioal_init_status[unit])
    {
        return  RT_ERR_OK;
    }

    drv_swcore_ioalCB_register(ioal_mem32_read, ioal_mem32_write);

    _ioal_swcore_acc_method_init(unit);

    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_MEM)
    {
        ret = _ioal_init_memmap(unit);
    }
#if defined(CONFIG_SDK_DRIVER_EXTC_PCI)
    else if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE)
    {
        ret = _ioal_init_pcie(unit);
    }
#endif
    else if ((ioal_db[unit].acc_method == IOAL_SWCORE_ACC_HRA) ||
             (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_I2C) ||
             (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_SPI))
    {
        ioal_db[unit].swcore_base   = 0;
        ioal_db[unit].swcore_size   = 0;
    }
    else if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_SIM)
    {
        ioal_db[unit].swcore_base   = 0;
        ioal_db[unit].swcore_size   = 0;
    }
    else
    {
        ioal_db[unit].swcore_base   = 0;
        ioal_db[unit].swcore_size   = 0;
        ioal_init_status[unit] = INIT_NOT_COMPLETED;
        return ret;
    }

    ioal_init_status[unit] = INIT_COMPLETED;
    return ret;
}

int32
ioal_exit(uint32 unit)
{
    ioal_init_status[unit] = INIT_NOT_COMPLETED;
    osal_memset((void *)&(ioal_db[unit]), 0, sizeof(ioal_db_t));

    return RT_ERR_OK;
}


/* Function Name:
 *      ioal_init_unitID_change
 * Description:
 *      Change a unit ID from fromID to toID, and delete fromID.
 * Input:
 *      fromID        - original unit ID
 *      toID          - new unit ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
ioal_init_unitID_change(uint32 fromID, uint32 toID)
{

    if (ioal_db[toID].initialized != 0)
        return RT_ERR_FAILED;

    if (ioal_db[fromID].initialized == 0)
        return RT_ERR_FAILED;


    ioal_db[toID] = ioal_db[fromID];
    osal_memset((void *)&(ioal_db[fromID]), 0, sizeof(ioal_db_t));

    return RT_ERR_OK;
}


int32
ioal_debug_show(void)
{
    int i;

    for(i=0;i<RTK_MAX_NUM_OF_UNIT;i++){

        osal_printf("ioal_db[%d].initialized     = %d\n",   i, ioal_db[i].initialized     );
        osal_printf("ioal_db[%d].swcore_base     = 0x%x\n", i, (uint32)ioal_db[i].swcore_base);
        osal_printf("ioal_db[%d].soc_base        = 0x%x\n", i, (uint32)ioal_db[i].soc_base  );
        //osal_printf("ioal_db[%d].sram_base       = 0x%x\n", i, ioal_db[i].sram_base       );
        //osal_printf("ioal_db[%d].pkt_base        = 0x%x\n", i, ioal_db[i].pkt_base        );
        //osal_printf("ioal_db[%d].l2ntfy_ring_base= 0x%x\n", i, ioal_db[i].l2ntfy_ring_base);
        //osal_printf("ioal_db[%d].l2ntfy_buf_base = 0x%x\n", i, ioal_db[i].l2ntfy_buf_base );
        //osal_printf("ioal_db[%d].l2ntfy_usr_base = 0x%x\n", i, ioal_db[i].l2ntfy_usr_base );
        //osal_printf("ioal_db[%d].desc_base       = 0x%x\n", i, ioal_db[i].desc_base       );
    }

    return RT_ERR_OK;
}


