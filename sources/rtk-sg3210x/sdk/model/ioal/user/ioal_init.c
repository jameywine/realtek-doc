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

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <common/debug/rt_log.h>
#include <ioal/ioal_init.h>
#include <ioal/mem32.h>
#include <virtualmac/vmac_target.h>
#include <osal/memory.h>
#include <osal/lib.h>
#include <osal/cpu.h>
#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>
#include <hwp/hw_profile.h>
#include <ioal/mem32.h>
#include <rtcore/user/rtcore_drv_usr.h>

#if defined(CONFIG_SDK_DRIVER_EXTC_PCI)
#include <common/rtcore/rtcore_init.h>
#include <rtcore/rtcore_pci.h>
#endif

#include <common/rtcore/rtcore_init.h>

#if (defined(CONFIG_SDK_RTL8380) || defined(CONFIG_RTL8380_SERIES))
    #include <private/drv/swcore/swcore_rtl8380.h>
#endif
#if (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_RTL8390_SERIES))
    #include <private/drv/swcore/swcore_rtl8390.h>
#endif
#if (defined(CONFIG_SDK_RTL9300) || defined(CONFIG_RTL9300_SERIES))
    #include <private/drv/swcore/swcore_rtl9300.h>
#endif
#if (defined(CONFIG_SDK_RTL9310) || defined(CONFIG_RTL9310_SERIES))
    #include <private/drv/swcore/swcore_rtl9310.h>
#endif
#if (defined(CONFIG_SDK_RTL9311B) || defined(CONFIG_RTL9311B_SERIES))
    #include <private/drv/swcore/swcore_rtl9311b.h>
#endif



/*
 * Symbol Definition
 */
#define VIRT_SWCORE_REG_SIZE    (0x1000000 - 16*64*1024)     /*15M */
#define VIRTUAL_SWCORE_TBL_SIZE	(4 * 1024*1024)             /* 4M */

/*
 * Data Declaration
 */
ioal_db_t ioal_db[RTK_MAX_NUM_OF_UNIT];
uint32 ioal_init_status[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED}; /* won't check, since ioal may init multiple times */
#if defined(CONFIG_SDK_DRIVER_NIC_USER_MODE) || (defined(CONFIG_SDK_DRIVER_EXTC_NIC) && defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && (defined(CONFIG_SDK_RTL9311B) || defined(CONFIG_SDK_RTL9330)))
uintptr ioal_lowMem_size, ioal_highMem_size, ioal_dma_size;
#endif

uint32 ioal_avoidSimultaneousMulticoreAcc = 0;

int8 virtualSwReg[VIRT_SWCORE_REG_SIZE];
int8 virtualSwTable[VIRTUAL_SWCORE_TBL_SIZE];
int8 *pVirtualSWReg = (int8 *)0;
int8 *pVirtualSWTable = (int8 *)0;
uint32 swcore_base[RTK_MAX_NUM_OF_UNIT];

/*
 * Macro Declaration
 */


/*
 * Function Declaration
 */
#if defined(CONFIG_SDK_DRIVER_EXTC_PCI)
int32
rtcore_pci_info_get(uint8 bus_num, rt_pcidevID_t pci_dev_id, rtcore_pci_dev_t *pci_info)
{
    rtcore_ioctl_t dio;
    rtcore_pci_dev_t *p;

    if (rtcore_dev_fd < 0)
        return RT_ERR_FAILED;

    osal_memset(&dio, 0, sizeof(rtcore_ioctl_t));

    dio.data[0] = bus_num;
    dio.data[1] = pci_dev_id;
    ioctl(rtcore_dev_fd, RTCORE_PCI_INFO_GET, &dio);
    if (dio.ret == RT_ERR_OK)
    {
        p = (rtcore_pci_dev_t *)&dio.data[2];
        osal_memcpy(pci_info, p, sizeof(rtcore_pci_dev_t));
    }

    return dio.ret;
}
#endif /* CONFIG_SDK_DRIVER_EXTC_PCI */

int32
ioal_init_memRegion_info_get(ioal_memRegion_t memRegion, ioal_memRegion_info_t *pMem_info)
{
    switch (memRegion)
    {
        case IOAL_MEM_SWCORE:
            if (HWP_CPU_ARCH() == HWP_CPU_ARCH_MIPS)
            {
                pMem_info->phys_base_addr = SWCORE_PHYS_BASE;
                pMem_info->mem_size = SWCORE_MEM_SIZE;
            }
            else if (HWP_CPU_ARCH() == HWP_CPU_ARCH_ARM)
            {
                pMem_info->phys_base_addr = SWCORE_ARM_PHYS_BASE;
                pMem_info->mem_size = SWCORE_ARM_MEM_SIZE;
            }
            else
                return RT_ERR_FAILED;
            break;

        case IOAL_MEM_SOC:
            if (HWP_CPU_ARCH() == HWP_CPU_ARCH_MIPS)
            {
                pMem_info->phys_base_addr = SOC_PHYS_BASE;
                pMem_info->mem_size = SOC_MEM_SIZE;
            }
            else if (HWP_CPU_ARCH() == HWP_CPU_ARCH_ARM)
            {
                pMem_info->phys_base_addr = SOC_ARM_PHYS_BASE;
                pMem_info->mem_size = SOC_ARM_MEM_SIZE;
            }
            else
                return RT_ERR_FAILED;
            break;

        case IOAL_MEM_SRAM:
            if (HWP_CPU_ARCH() == HWP_CPU_ARCH_MIPS)
            {
                pMem_info->phys_base_addr = SRAM_PHYS_BASE;
                pMem_info->mem_size = SRAM_MEM_SIZE_128M;
            }
            else if (HWP_CPU_ARCH() == HWP_CPU_ARCH_ARM)
            {
                pMem_info->phys_base_addr = SRAM_ARM_PHYS_BASE;
                pMem_info->mem_size = SRAM_MEM_SIZE_128M;
            }
            else
                return RT_ERR_FAILED;
            break;

        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


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

    switch (ictype)
    {
        case IC_TYPE_MODEL:
            if (IOAL_MEM_SWCORE == mem)
                *pBaseAddr = swcore_base[unit];
            else
                return RT_ERR_FAILED;
            break;
        case IC_TYPE_REAL:
            if (IOAL_MEM_SWCORE == mem)
                *pBaseAddr = SWCORE_VIRT_BASE;
            else if (IOAL_MEM_SOC == mem)
                *pBaseAddr = SOC_VIRT_BASE;
            else
                return RT_ERR_FAILED;
            break;
        default:
            break;
    }

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

        case IOAL_MEM_NIC:
            *pBaseAddr = ioal_db[unit].nic_base;
            break;

#ifdef CONFIG_SDK_DRIVER_NIC_USER_MODE

        case IOAL_MEM_L2NTFY_PHY:
            *pBaseAddr = ioal_db[unit].l2ntfy_phy_base;
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

        case IOAL_MEM_DMA:
            *pBaseAddr = ioal_db[unit].pkt_base;
            break;

        case IOAL_MEM_DMA_PHY:
            *pBaseAddr = ioal_db[unit].pkt_phy_base;
            break;

        case IOAL_MEM_DESC:
            *pBaseAddr = ioal_db[unit].desc_base;
            break;

        case IOAL_MEM_DESC_PHY:
            *pBaseAddr = ioal_db[unit].desc_phy_base;
            break;
#endif
        default:
            return RT_ERR_FAILED;
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

        case IOAL_MEM_TABLE_SIZE:
            ioal_db[unit].swcore_tbl_size = baseAddr;
            break;

        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
} /* end of ioal_init_memRegion_set */

/* Function Name:
 *      _ioal_sim_init
 * Description:
 *      Init simulated ioal base for linux user space usage
 * Input:
 *      unit    - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
_ioal_sim_init(uint32 unit)
{
    int32 ret;
    uint32 chip_id;

    if ((uintptr)NULL == ioal_db[unit].swcore_base)
    {
        uintptr *pSimMem = (uintptr*)osal_alloc((SIM_SWCORE_REG_SIZE + SIM_SWCORE_TBL_SIZE + SIM_SOC_SIZE + SIM_SRAM_SIZE) * sizeof(uintptr));
        if (NULL == pSimMem)
            return RT_ERR_FAILED;

        ioal_db[unit].swcore_base   = (uintptr)(pSimMem);
        ioal_db[unit].soc_base      = (uintptr)(pSimMem + (SIM_SWCORE_REG_SIZE + SIM_SWCORE_TBL_SIZE));
        ioal_db[unit].sram_base     = (uintptr)(pSimMem + (SIM_SWCORE_REG_SIZE + SIM_SWCORE_TBL_SIZE + SIM_SOC_SIZE));
        ioal_db[unit].swcore_size   = SIM_SWCORE_REG_SIZE + SIM_SWCORE_TBL_SIZE;
    }

    ioal_db[unit].initialized = 1;

    chip_id = RTK_CHIP_ID(unit);
    switch(chip_id)
    {
#if (defined(CONFIG_SDK_RTL8380) || defined(CONFIG_RTL8380_SERIES))
        case RTL8382M_CHIP_ID:
            if ((ret = ioal_mem32_write(unit, RTL8380_MODEL_NAME_INFO_ADDR, (chip_id & 0xFFFFF800))) != RT_ERR_OK)
                return ret;
            if ((ret = ioal_mem32_write(unit, RTL8380_EXT_VERSION_ADDR, 2)) != RT_ERR_OK)
                return ret;
            break;
#endif
#if (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_RTL8390_SERIES))
        case RTL8393M_CHIP_ID:
        {
            uint32 chip_rev_id = CHIP_REV_ID_A;
            uint32 model_info = (chip_id & 0xFFFFFF00);
            model_info |= ((chip_rev_id & 0x3E) << 1);
            if ((ret = ioal_mem32_write(unit, RTL8390_MODEL_NAME_INFO_ADDR, model_info)) != RT_ERR_OK)
                return ret;
            break;
        }
#endif
#if (defined(CONFIG_SDK_RTL9300) || defined(CONFIG_RTL9300_SERIES))
        case RTL9303_CHIP_ID:
        {
            uint32 chip_rev_id = CHIP_REV_ID_B;
            uint32 model_info = (chip_id & 0xFFFFFFC0);
            model_info |= (chip_rev_id & 0xF);
            if ((ret = ioal_mem32_write(unit, RTL9300_MODEL_NAME_INFO_ADDR, model_info)) != RT_ERR_OK)
                return ret;
            break;
        }
#endif
#if (defined(CONFIG_SDK_RTL9310) || defined(CONFIG_RTL9310_SERIES))
        case RTL9313_CHIP_ID:
        {
            uint32 chip_rev_id = CHIP_REV_ID_A;
            uint32 model_info = (chip_id & 0xFFFFFF00);
            model_info |= (chip_rev_id & 0xF);
            if ((ret = ioal_mem32_write(unit, RTL9310_MODEL_NAME_INFO_ADDR, model_info)) != RT_ERR_OK)
                return ret;
            break;
        }
#endif
#if (defined(CONFIG_SDK_RTL9311B) || defined(CONFIG_RTL9311B_SERIES))
        case RTL9311BM_CHIP_ID:
        {
            uint32 chip_rev_id = CHIP_REV_ID_B;
            uint32 model_info = (chip_id & 0xFFFFFFC0);
            model_info |= (chip_rev_id << RTL9311B_MODEL_NAME_INFO_RTL_VID_OFFSET) & RTL9311B_MODEL_NAME_INFO_RTL_VID_MASK;
            if ((ret = ioal_mem32_write(unit, RTL9311B_MODEL_NAME_INFO_ADDR, model_info)) != RT_ERR_OK)
                return ret;
            break;
        }
#endif
        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


#if defined(CONFIG_SDK_DRIVER_EXTC_PCI)
/* Function Name:
 *      _ioal_init_pcie
 * Description:
 *      Init ioal base for linux user space usage in external CPU with PCI interface environment
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
_ioal_init_pcie(uint32 unit)
{
    int32   ret = RT_ERR_OK;
    uint32  num_cores = 0;
    void    *pUserReg;
    rtcore_pci_dev_t pci_info;
    int pageSize = 0;
#if defined(CONFIG_SDK_RTL9311B) || defined(CONFIG_SDK_RTL9330)
#if defined(CONFIG_SDK_DRIVER_L2NTFY)
    uint32 chip_id, chip_rev_id;
#endif
#endif

    /* swcore map memory */
    rtcore_pci_info_get(HWP_SWCORE_PCIE_BUS(unit), RT_PCIDEV_SWCORE, &pci_info);
    pUserReg = osal_mmap(MEM_DEV_NAME, pci_info.physical_base_addr, pci_info.mem_size);
    if (RT_ERR_FAILED == (uint32)((uintptr)pUserReg) )
    {
        osal_printf("%s(%d): swcore pci mmap failed (phy add=0x%x)\n", __FUNCTION__, __LINE__, (uint32)(uintptr)pci_info.physical_base_addr);
        return RT_ERR_FAILED;
    }
    ioal_db[unit].swcore_base = (uintptr)pUserReg;
    ioal_db[unit].swcore_size = pci_info.mem_size;


    /* NIC map memory */
    rtcore_pci_info_get(HWP_SWCORE_PCIE_BUS(unit), RT_PCIDEV_NIC, &pci_info);
    pUserReg = osal_mmap(MEM_DEV_NAME, pci_info.physical_base_addr, pci_info.mem_size);
    if (RT_ERR_FAILED == (uint32)((uintptr)pUserReg) )
    {
        osal_printf("%s(%d): nic pci mmap failed\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }
    ioal_db[unit].nic_base = (uintptr)pUserReg;


    /* NIC DMA map memory */
#if defined(CONFIG_SDK_RTL9311B) || defined(CONFIG_SDK_RTL9330)
    rtcore_system_memsize_info(&ioal_lowMem_size, &ioal_highMem_size, &ioal_dma_size);
    rtcore_nic_dmaMem_info_get(unit, &ioal_db[unit].desc_phy_base,
                               &ioal_db[unit].pkt_phy_base);

    pageSize = getpagesize();
    pUserReg = osal_mmap(RTCORE_DEV_NAME, RTCORE_MMAP_NIC_DESC_PCI * pageSize, ioal_dma_size);

    if (RT_ERR_FAILED == (uint32)((uintptr)pUserReg) )
    {
        osal_printf("%s(%d): nic dma mmap failed\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }
    ioal_db[unit].desc_base = (uintptr)pUserReg;

    pUserReg = osal_mmap(RTCORE_DEV_NAME, RTCORE_MMAP_NIC_BUF * pageSize, ioal_dma_size);

    if (RT_ERR_FAILED == (uint32)((uintptr)pUserReg) )
    {
        osal_printf("%s(%d): nic dma mmap failed\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }
    ioal_db[unit].pkt_base = (uintptr)pUserReg;

#if defined(CONFIG_SDK_DRIVER_L2NTFY)
    if( RT_ERR_OK != drv_swcore_cid_get(unit, &chip_id, &chip_rev_id) )
        return RT_ERR_FAILED;

    if(FAMILY_ID(chip_id) == RTL9311B_FAMILY_ID)
    {
        ioal_db[unit].l2ntfy_phy_base   = ioal_db[unit].desc_phy_base + RTL9311B_DESC_MEM_SIZE;
        ioal_db[unit].l2ntfy_ring_base  = ioal_db[unit].desc_base + RTL9311B_DESC_MEM_SIZE;
        ioal_db[unit].l2ntfy_buf_base   = ioal_db[unit].pkt_base + RTL9311B_PKT_MEM_SIZE;
        ioal_db[unit].l2ntfy_usr_base   = ioal_db[unit].l2ntfy_buf_base + RTL9311B_L2NTFY_BUF_MEM_SIZE;
    }
#endif

#else

    rtcore_pci_info_get(HWP_SWCORE_PCIE_BUS(unit), RT_PCIDEV_NIC_DMA, &pci_info);
    /* Get memory page size. */
    pageSize = getpagesize();
    pUserReg = osal_mmap(RTCORE_DEV_NAME, RTCORE_MMAP_NIC_DESC_PCI * pageSize, pci_info.mem_size);
    if (RT_ERR_FAILED == (uint32)((uintptr)pUserReg) )
    {
        osal_printf("%s(%d): nic dma mmap failed\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }
    ioal_db[unit].desc_base = (uintptr)pUserReg;
    ioal_db[unit].desc_phy_base = pci_info.physical_base_addr;

#endif

    ioal_db[unit].initialized = 1;


    RT_ERR_CHK_EHDL(osal_cpu_num_cores_get(&num_cores), ret,
                    { RT_INIT_ERR(ret, (MOD_INIT), "_ioal_init_pcie fail"); });

    if ((num_cores > 1) && (HWP_CPU_EMBEDDED())
            && (RTK_CHIP_ID(RTK_SOC_UNIT_ID) == RTL9311BM_CHIP_ID)
            && (RTK_CHIP_REV(RTK_SOC_UNIT_ID) == CHIP_REV_ID_A))
    {
        ioal_avoidSimultaneousMulticoreAcc = 1;
    }

    return RT_ERR_OK;
}

#endif /* CONFIG_SDK_DRIVER_EXTC_PCI */

/* Function Name:
 *      _ioal_init
 * Description:
 *      Init ioal base for linux user space usage with internal CPU or external CPU with SPI/I2C interface environment
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
_ioal_init(uint32 unit)
{
    uint32 chip_id, chip_rev_id;
    void    *pUserReg;
    ioal_memRegion_info_t mem_info;
#ifdef CONFIG_SDK_DRIVER_NIC_USER_MODE
    uint32  used_dma_size;
    int pageSize = 0;
    uintptr phy_base = 0;
#endif

#ifdef CONFIG_SDK_DRIVER_NIC_USER_MODE
    rtcore_system_memsize_info(&ioal_lowMem_size, &ioal_highMem_size, &ioal_dma_size);
    /* check memory size definition */
    used_dma_size = (RTL8380_DESC_MEM_SIZE + RTL8380_PKT_MEM_SIZE + \
                     RTL8380_L2NTFY_RING_MEM_SIZE + RTL8380_L2NTFY_BUF_MEM_SIZE + RTL8380_L2NTFY_USR_MEM_SIZE);

    if ( ioal_dma_size < used_dma_size)
    {
        return RT_ERR_FAILED;
    }
#endif

    osal_memset(&mem_info, 0, sizeof(ioal_memRegion_info_t));

    /* map memory */
    ioal_init_memRegion_info_get(IOAL_MEM_SWCORE, &mem_info);
    pUserReg = osal_mmap(MEM_DEV_NAME, mem_info.phys_base_addr, mem_info.mem_size);
    if (RT_ERR_FAILED == (uint32)(uintptr)pUserReg)
        return RT_ERR_FAILED;
    ioal_db[unit].swcore_base = (uintptr)pUserReg;

    ioal_init_memRegion_info_get(IOAL_MEM_SRAM, &mem_info);
    pUserReg = osal_mmap(MEM_DEV_NAME, mem_info.phys_base_addr, mem_info.mem_size);
    if (RT_ERR_FAILED == (uint32)(uintptr)pUserReg)
        return RT_ERR_FAILED;
    ioal_db[unit].sram_base = (uintptr)pUserReg;

#ifdef CONFIG_SDK_DRIVER_NIC_USER_MODE
#if defined(CONFIG_SDK_RTL9311B) || defined(CONFIG_SDK_RTL9330)
    rtcore_nic_dmaMem_info_get(unit, &ioal_db[unit].desc_phy_base,
                               &ioal_db[unit].pkt_phy_base);
    pageSize = getpagesize();
    pUserReg = osal_mmap(RTCORE_DEV_NAME, RTCORE_MMAP_NIC_DESC * pageSize, ioal_dma_size);
    if (RT_ERR_FAILED == (uint32)(uintptr)pUserReg)
        return RT_ERR_FAILED;
    ioal_db[unit].desc_base = (uintptr)pUserReg;
    pUserReg = osal_mmap(RTCORE_DEV_NAME, RTCORE_MMAP_NIC_BUF * pageSize, ioal_dma_size);
    if (RT_ERR_FAILED == (uint32)(uintptr)pUserReg)
        return RT_ERR_FAILED;
    ioal_db[unit].pkt_base = (uintptr)pUserReg;
    ioal_lowMem_size = ioal_db[unit].desc_phy_base;
#else
    pUserReg = osal_mmap(MEM_DEV_NAME, DMA_PHYS_BASE, DMA_MEM_SIZE);
    if (RT_ERR_FAILED == (uint32)(uintptr)pUserReg)
    return RT_ERR_FAILED;
    ioal_db[unit].desc_base = (uintptr)pUserReg;
#endif
#endif

    if( RT_ERR_OK != drv_swcore_cid_get(RTK_SOC_UNIT_ID, &chip_id, &chip_rev_id) )
        return RT_ERR_FAILED;

#ifdef CONFIG_SDK_DRIVER_NIC_USER_MODE
    switch(FAMILY_ID(chip_id))
    {
        case RTL8390_FAMILY_ID:
        case RTL8350_FAMILY_ID:
            ioal_db[unit].pkt_base          = ioal_db[unit].desc_base + RTL8390_DESC_MEM_SIZE;
            ioal_db[unit].l2ntfy_ring_base  = ioal_db[unit].pkt_base  + RTL8390_PKT_MEM_SIZE;
            ioal_db[unit].l2ntfy_buf_base   = ioal_db[unit].l2ntfy_ring_base + RTL8390_L2NTFY_RING_MEM_SIZE;
            ioal_db[unit].l2ntfy_usr_base   = ioal_db[unit].l2ntfy_buf_base + RTL8390_L2NTFY_BUF_MEM_SIZE;
            ioal_db[unit].swcore_size       = SWCORE_MEM_SIZE;
            break;
        case RTL8380_FAMILY_ID:
        case RTL8330_FAMILY_ID:
            ioal_db[unit].pkt_base          = ioal_db[unit].desc_base + RTL8380_DESC_MEM_SIZE;
            ioal_db[unit].l2ntfy_ring_base  = ioal_db[unit].pkt_base  + RTL8380_PKT_MEM_SIZE;
            ioal_db[unit].l2ntfy_buf_base   = ioal_db[unit].l2ntfy_ring_base + RTL8380_L2NTFY_RING_MEM_SIZE;
            ioal_db[unit].l2ntfy_usr_base   = ioal_db[unit].l2ntfy_buf_base + RTL8380_L2NTFY_BUF_MEM_SIZE;
            ioal_db[unit].swcore_size       = SWCORE_MEM_SIZE;
            break;
        case RTL9310_FAMILY_ID:
            ioal_db[unit].pkt_base          = ioal_db[unit].desc_base + RTL9310_DESC_MEM_SIZE;
            ioal_db[unit].l2ntfy_ring_base  = ioal_db[unit].pkt_base  + RTL9310_PKT_MEM_SIZE;
            ioal_db[unit].l2ntfy_buf_base   = ioal_db[unit].l2ntfy_ring_base + RTL9310_L2NTFY_RING_MEM_SIZE;
            ioal_db[unit].l2ntfy_usr_base   = ioal_db[unit].l2ntfy_buf_base + RTL9310_L2NTFY_BUF_MEM_SIZE;
            ioal_db[unit].swcore_size       = SWCORE_MEM_SIZE;
            break;
        case RTL9300_FAMILY_ID:
            ioal_db[unit].pkt_base          = ioal_db[unit].desc_base + RTL9300_DESC_MEM_SIZE;
            ioal_db[unit].l2ntfy_ring_base  = ioal_db[unit].pkt_base  + RTL9300_PKT_MEM_SIZE;
            ioal_db[unit].l2ntfy_buf_base   = ioal_db[unit].l2ntfy_ring_base + RTL9300_L2NTFY_RING_MEM_SIZE;
            ioal_db[unit].l2ntfy_usr_base   = ioal_db[unit].l2ntfy_buf_base + RTL9300_L2NTFY_BUF_MEM_SIZE;
            ioal_db[unit].swcore_size       = SWCORE_MEM_SIZE;
            break;
        case RTL9311B_FAMILY_ID:
#if defined(CONFIG_SDK_DRIVER_L2NTFY)
            ioal_db[unit].l2ntfy_ring_base  = ioal_db[unit].desc_base + RTL9311B_DESC_MEM_SIZE;
            ioal_db[unit].l2ntfy_phy_base   = ioal_db[unit].desc_phy_base + RTL9311B_DESC_MEM_SIZE;
            ioal_db[unit].l2ntfy_buf_base   = ioal_db[unit].pkt_base + RTL9311B_PKT_MEM_SIZE;
            ioal_db[unit].l2ntfy_usr_base   = ioal_db[unit].l2ntfy_buf_base + RTL9311B_L2NTFY_BUF_MEM_SIZE;
#endif
            ioal_db[unit].swcore_size       = SWCORE_ARM_MEM_SIZE;
            break;

        default:
            return RT_ERR_FAILED;
    }
#endif

    switch(FAMILY_ID(chip_id))
    {
        case RTL8390_FAMILY_ID:
        case RTL8350_FAMILY_ID:
        case RTL8380_FAMILY_ID:
        case RTL8330_FAMILY_ID:
        case RTL9310_FAMILY_ID:
        case RTL9300_FAMILY_ID:
        case RTL9311B_FAMILY_ID:
            ioal_init_memRegion_info_get(IOAL_MEM_SOC, &mem_info);
            pUserReg = osal_mmap(MEM_DEV_NAME, mem_info.phys_base_addr, mem_info.mem_size);
            if (RT_ERR_FAILED == (uint32)(uintptr)pUserReg)
                return RT_ERR_FAILED;
            ioal_db[unit].soc_base = (uintptr)pUserReg;
            break;
        default:
            return RT_ERR_FAILED;
    }

    ioal_db[unit].initialized = 1;
    return RT_ERR_OK;
}

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

/* Function Name:
 *      ioal_init
 * Description:
 *      Init SDK IOAL Layer
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
ioal_init(uint32 unit)
{
    int32 ret = 0;

    RT_INIT_MSG("    IOAL init\n");

    pVirtualSWReg = virtualSwReg;
    swcore_base[unit] = (uint32)pVirtualSWReg;
    pVirtualSWTable = virtualSwTable;

    drv_swcore_ioalCB_register(ioal_mem32_read,ioal_mem32_write);

    if (INIT_COMPLETED == ioal_init_status[unit])
    {
        return  RT_ERR_OK;
    }

    drv_swcore_ioalCB_register(ioal_mem32_read, ioal_mem32_write);

    _ioal_swcore_acc_method_init(unit);

    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_MEM)
    {
        ret = _ioal_init(unit);
    }
#if defined(CONFIG_SDK_DRIVER_EXTC_PCI)
    else if ((ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE))
    {
        ret = _ioal_init_pcie(unit);
    }
#endif
    else if ((ioal_db[unit].acc_method == IOAL_SWCORE_ACC_HRA) ||
             (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_I2C) ||
             (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_SPI))
    {
        ret = _ioal_init(unit);
    }
    else if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_SIM)
    {
        ret = _ioal_sim_init(unit);
    }
    else
    {
        ioal_db[unit].swcore_base   = 0;
        ioal_db[unit].swcore_size   = 0;
        ret = RT_ERR_FAILED;
    }


    if (ret == RT_ERR_OK)
    {
        ioal_init_status[unit] = INIT_COMPLETED;
    }

    return ret;
} /* end of ioal_init */

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
    return RT_ERR_OK;
}


