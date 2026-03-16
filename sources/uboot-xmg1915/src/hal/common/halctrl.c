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
 * $Revision: 78914 $
 * $Date: 2017-05-19 09:55:17 +0800 (Fri, 19 May 2017) $
 *
 * Purpose : Hardware Abstraction Layer (HAL) control structure and definition in the SDK.
 *
 * Feature : HAL control structure and definition
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <common/util/rt_bitop.h>
#include <common/util/rt_util.h>
#include <osal/lib.h>
#include <hal/common/halctrl.h>
#include <hal/mac/mac_probe.h>
#include <hal/phy/phy_probe.h>
#include <dal/dal_construct.h>
#include <hwp/hw_profile.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
hal_control_t hal_ctrl[RTK_MAX_NUM_OF_UNIT];
static uint32 hal_init_status = INIT_NOT_COMPLETED;


/*
 * Function Declaration
 */
void hal_show_portmask(rtk_portmask_t  *portmask)
{
    int i,j;
    uint32 tmp;
    for(i=RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST-1;i>=0;i--)
    {
        osal_printf("      [%d]:",i);
        tmp = portmask->bits[i];
        for(j=BITMAP_WIDTH-1;j>=0;j--)
        {
            osal_printf("%d",(tmp&(0x1<<j))?1:0);
        }
        osal_printf("\n");
    }
}
void hal_debug_show_info(uint32 unit, uint32 sequenceNo)
{
    int i,port;

    osal_printf("+------(%u)---halctrl ctrl(unit %u)-----------------\n",sequenceNo,unit);
    osal_printf("|    chip_flags=0x%08X\n",       hal_ctrl[unit].chip_flags);
    osal_printf("|    semaphore=0x%08X\n",        hal_ctrl[unit].semaphore);
    osal_printf("|    *pChip_driver=0x%08X\n",    (uint32)hal_ctrl[unit].pChip_driver);
    osal_printf("|        type=%d\n",             hal_ctrl[unit].pChip_driver->type);
    osal_printf("|        driver_id=0x%08X\n",    hal_ctrl[unit].pChip_driver->driver_id);
    osal_printf("|        driver_rev_id=0x%08X\n",hal_ctrl[unit].pChip_driver->driver_rev_id);
    osal_printf("|        pReg_list=0x%08X\n",    (uint32)hal_ctrl[unit].pChip_driver->pReg_list);
    osal_printf("|        pPortmask_list=\n");
    hal_show_portmask(hal_ctrl[unit].pChip_driver->pPortmask_list);
    for(i=RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST-1;i>=0;i--)

    osal_printf("|    *pDev_info=0x%08X\n",    (uint32)hal_ctrl[unit].pDev_info);
    osal_printf("|        chip_id=0x%08X\n",hal_ctrl[unit].pDev_info->chip_id);
    osal_printf("|        chip_rev_id=0x%08X\n",hal_ctrl[unit].pDev_info->chip_rev_id);
    osal_printf("|        driver_id=0x%08X\n",hal_ctrl[unit].pDev_info->driver_id);
    osal_printf("|        driver_rev_id=0x%08X\n",hal_ctrl[unit].pDev_info->driver_rev_id);
    osal_printf("|        chip_aflags=0x%08X\n",hal_ctrl[unit].pDev_info->chip_aflags);
    osal_printf("|        chip_family_id=0x%08X\n",hal_ctrl[unit].pDev_info->chip_family_id);

    HWP_PORT_TRAVS(unit,port)
        osal_printf("    *pPhy_ctrl[%d]=0x%08X\n", port, (uint32)hal_ctrl[unit].pPhy_ctrl[port]);
    HWP_PORT_TRAVS_EXCEPT_CPU(unit, port)
    {
        if (HWP_PHY_EXIST(unit, port))
        {
            osal_printf("    phydrv_index(%d)   %d\n", port, (uint32)hal_ctrl[unit].pPhy_ctrl[port]->pPhydrv->phydrv_index);
        }
    }
    osal_printf("+---------------------------------------------------\n");

}


/* Function Name:
 *      hal_init
 * Description:
 *      Initialize the hal layer API.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - OK
 *      RT_ERR_FAILED           - General Error
 *      RT_ERR_CHIP_NOT_FOUND   - The chip can not found
 *      RT_ERR_DRIVER_NOT_FOUND - The driver can not found
 * Note:
 *      Initialize the hal layer API, include get the chip id and chip revision
 *      id, get its driver id and driver revision id, then bind to its major
 *      driver. Also initialize its hal_ctrl structure for this specified unit.
 *      Before calling the function, bsps should already scan HW interface, like
 *      PCI device in all buses, or physical Lextra, and the total chip numbers,
 *      its chip id and chip revision id is known and store in database in lower
 *      layer.
 */
int32
hal_init(void)
{
    int32   ret = RT_ERR_FAILED,i;
    RT_INIT_REENTRY_CHK(hal_init_status);

    for(i=0;i<RTK_MAX_NUM_OF_UNIT;i++)
        osal_memset((uint8 *)&hal_ctrl[i], 0, sizeof(hal_control_t));

    /* Probe MAC */
    if ((ret = mac_probe()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_HAL, "mac_probe failed!!\n");
        return ret;
    }

    /* Init MAC */
    if ((ret = mac_init()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_HAL, "mac_init failed!!\n");
        return ret;
    }

    /* Probe PHY */
    if ((ret = phy_probe()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_HAL, "phy_probe failed!!\n");
        return ret;
    }

    /* Construct PHY/MAC-Serdes */
    if ((ret = chip_construct()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_HAL, "chip_construct() failed!!\n");
        return ret;
    }

    /* Init PHY */
    if ((ret = phy_init()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_HAL, "phy_init failed!!\n");
        return ret;
    }
    hal_init_status = INIT_COMPLETED;

    return RT_ERR_OK;
} /* end of hal_init */

/* Function Name:
 *      hal_ctrlInfo_get
 * Description:
 *      Find the hal control information structure for this specified unit.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      NULL      - Not found
 *      Otherwise - Pointer of hal control information structure that found
 * Note:
 *      The function have found the exactly hal control information structure.
 */
hal_control_t *
hal_ctrlInfo_get(uint32 unit)
{
    if (0 == hal_ctrl[unit].chip_flags)
    {
        return NULL;
    }
    else
    {
        return &hal_ctrl[unit];
    }
} /* end of hal_ctrlInfo_get */

/* Function Name:
 *      hal_portMaxBandwidth_ret
 * Description:
 *      Get the max bandwith of port.
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      max bandwidth value
 * Note:
 *      The return value is different in FE/GE/10GE port.
 */
uint32
hal_portMaxBandwidth_ret(uint32 unit, rtk_port_t port)
{
    if (HWP_FE_PORT(unit, port))
        return (hal_ctrl[unit].pDev_info->pCapacityInfo->rate_of_bandwidth_max_fe_port);
    else if (HWP_GE_PORT(unit, port))
        return (hal_ctrl[unit].pDev_info->pCapacityInfo->rate_of_bandwidth_max_ge_port);
    else if (HWP_10GE_PORT(unit, port))
        return (hal_ctrl[unit].pDev_info->pCapacityInfo->rate_of_bandwidth_max_10ge_port);
    else
        return (hal_ctrl[unit].pDev_info->pCapacityInfo->rate_of_bandwidth_max);
} /* end of hal_portMaxBandwidth_ret */

