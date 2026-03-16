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
 * $Revision: 79749 $
 * $Date: 2017-06-16 17:10:28 +0800 (Fri, 16 Jun 2017) $
 *
 * Purpose : PHY probe and init service APIs in the SDK.
 *
 * Feature : PHY probe and init service APIs
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <hal/chipdef/driver.h>
#include <hal/chipdef/chipdef.h>
#include <hal/common/halctrl.h>
#include <hal/mac/mac_probe.h>
#include <hal/phy/identify.h>
#include <hal/phy/phy_probe.h>
#include <hal/phy/phy_common.h>
#include <hwp/hw_profile.h>
#include <hwp/hwp_util.h>
#include <osal/memory.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
 static uint32 phy_init_status = INIT_NOT_COMPLETED;


/*
 * Function Declaration
 */


/* Static Function Body */

#if !(defined(__MODEL_USER__) || defined(CONFIG_VIRTUAL_ARRAY_ONLY))

/* Function Name:
 *      phy_driver_hook
 * Description:
 *      Probe the external PHY chip in the specified chip.
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
rt_phyctrl_t *
phy_driver_hook(uint32 unit, rtk_port_t port)
{
    rt_phyctrl_t    *pPhyctrl = NULL;

    if (HWP_USEDEFHWP())
    {
        pPhyctrl = phy_identify_driver_find_blindly(unit,port);
    }
    else
    {
        if (!HWP_PHY_EXIST(unit, port) && !HWP_SERDES_PORT(unit, port))
        {
            hal_ctrl[unit].pPhy_ctrl[port] = NULL;
            RT_LOG(LOG_TRACE, MOD_HAL, "unit %u port %d HWP does not specify PHY nor SerDes-port.", unit, port);
            return NULL;
        }

        /* find PHY control */
        if (NULL == (pPhyctrl = phy_identify_find(unit, port)))
        {
            /* check upon all phy drivers to see if we can find one driver */
            pPhyctrl = phy_identify_driver_find_blindly(unit, port);
            if (pPhyctrl == NULL)
            {
                hal_ctrl[unit].pPhy_ctrl[port] = NULL;
                RT_ERR(RT_ERR_FAILED, MOD_HAL, "unit %u port %u PHY type %d PHY probe fail!\n",
                   unit, port, HWP_PHY_MODEL_BY_PORT(unit, port));
                return NULL;
            }
        }
    }

    hal_ctrl[unit].pPhy_ctrl[port] = pPhyctrl;
    RT_LOG(LOG_TRACE, MOD_HAL, "PHY driver probed on unit %u port %d", unit, port);

    return pPhyctrl;
} /* end of phy_driver_hook */
#endif

/* Function Name:
 *      _phy_probe_defaultProfilePort_update
 * Description:
 *      Updates defaul hardware profile according to probed PHY type.
 * Input:
 *      defaultHwp - default hardware profile
 *      unit - unit id
 *      port - port id
 *      phyIndex - for updating phy_idx of hardware profile port description
 *      phyBasePort - for updating mac_id of hardware profile phy description
 *      phyCtrl - probed PHY control block
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
void
_phy_probe_defaultProfilePort_update(hwp_hwProfile_t *defaultHwp, uint32 unit, uint32 port, int32 phyIndex, int32 phyBasePort, rt_phyctrl_t *phyCtrl)
{
    uint8       attr, medi, eth;

    attr = HWP_ETH;
    eth  = (uint8)phyCtrl->pPhyInfo->eth_type;

    switch (phyCtrl->phyType)
    {
      case RTK_PHYTYPE_RTL8212B:
      case RTK_PHYTYPE_RTL8214FB:
      case RTK_PHYTYPE_RTL8214FC:
      case RTK_PHYTYPE_RTL8218FB:
        if (phyCtrl->pPhyInfo->isComboPhy[(port-phyBasePort)])
            medi = HWP_COMBO;
        else
            medi = HWP_COPPER;
        break;
      case RTK_PHYTYPE_RTL8295R_C22:
      case RTK_PHYTYPE_RTL8295R:
      case RTK_PHYTYPE_RTL8214QF_NC5:
      case RTK_PHYTYPE_RTL8214QF:
        medi = HWP_FIBER;
        break;
      case RTK_PHYTYPE_SERDES:
        medi = HWP_SERDES;
        break;
      case RTK_PHYTYPE_RTL8214B:
      case RTK_PHYTYPE_RTL8214C:
      case RTK_PHYTYPE_RTL8218B:
      case RTK_PHYTYPE_RTL8218D_NMP:
      case RTK_PHYTYPE_RTL8218D:
      case RTK_PHYTYPE_RTL8208D:
      case RTK_PHYTYPE_RTL8208G:
      case RTK_PHYTYPE_RTL8208L:
      case RTK_PHYTYPE_RTL8208L_INT:
        medi = HWP_COPPER;
        break;
      default:
        return;
    }

    hwp_defaultProfilePort_update(hwp_myHwProfile, (uint8)port, attr, medi, eth, phyIndex);
}


/* Public Function Body */

/* Function Name:
 *      phy_probe
 * Description:
 *      Probe the PHY chip in the specified chip.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
phy_probe(void)
{
#if !(defined(__MODEL_USER__) || defined(CONFIG_VIRTUAL_ARRAY_ONLY))
    uint32  port=0, unit=0;
    //int32   ret = RT_ERR_FAILED;
    uint32          phyHwpUpCnt = 0;
    rtk_portmask_t  phyProbeFailPmsk;
    rtk_portmask_t  phyUpdatePmsk;
    rt_phyctrl_t    *phyCtrl;
    uint32 phy_total_port=0, TBD_basePort=0, TBD_phyIdx=0;

    /* ini identify database */
    phy_identify_init();

    /* Probe PHY */
    HWP_UNIT_TRAVS_LOCAL(unit)
    {
        if(!HWP_SWCORE_SUPPORT(unit))
            continue;

        RT_INIT_MSG("  PHY probe (unit %u)\n",unit);

        RTK_PORTMASK_RESET(phyProbeFailPmsk);
        RTK_PORTMASK_RESET(phyUpdatePmsk);
        HWP_PORT_TRAVS_EXCEPT_CPU(unit,port)
        {
            if ((phyCtrl =  phy_driver_hook(unit, port)) == NULL)
            {
                if (!HWP_USEDEFHWP())
                {
                    RTK_PORTMASK_PORT_SET(phyProbeFailPmsk, port);
                }
            }
            else
            {
                if (HWP_USEDEFHWP())
                {
                    if (phy_total_port == 0)
                    {
                        TBD_basePort = port;
                    }

                    phy_total_port++;

                    _phy_probe_defaultProfilePort_update(hwp_myHwProfile, unit, port, TBD_phyIdx, TBD_basePort, phyCtrl);
                    hwp_defaultProfilePhy_build(hwp_myHwProfile, port, phyCtrl, TBD_basePort, TBD_phyIdx);

                    if (phy_total_port >= phyCtrl->pPhyInfo->phy_num)
                    {
                        TBD_phyIdx++;
                        phy_total_port=0;
                    }
                }
                else
                {
                    if ((HWP_PHY_MODEL_BY_PORT(unit, port) != phyCtrl->phyType) && (HWP_PHY_BASE_MACID(unit, port) == port))
                    {
                        RTK_PORTMASK_PORT_SET(phyUpdatePmsk, port);
                    }
                }
            }
        }/* end HWP_PORT_TRAVS_EXCEPT_CPU */


        if (!RTK_PORTMASK_IS_ALL_ZERO(phyUpdatePmsk))
        {
            HWP_PORT_TRAVS_EXCEPT_CPU(unit, port)
            {
                if (RTK_PORTMASK_IS_PORT_SET(phyUpdatePmsk, port))
                {
                    RT_LOG(LOG_INFO, MOD_HAL, "unit %u port %u update PHY type to HWP", unit, port);
                    phyHwpUpCnt += hwp_profilePhy_update(unit, port, hal_ctrl[unit].pPhy_ctrl[port]->phyType);
                }
            }

        }

        if (!RTK_PORTMASK_IS_ALL_ZERO(phyProbeFailPmsk))
        {
            RT_LOG(LOG_INFO, MOD_HAL, "unit %u port %u delete PHY to HWP", unit, port);
            phyHwpUpCnt += hwp_profilePhy_del(unit, &phyProbeFailPmsk);
        }
    }/* end of HWP_UNIT_TRAVS_LOCAL */

    if (HWP_USEDEFHWP() || (phyHwpUpCnt > 0))
    {
        hwp_parsedInfo_buildup(&myParsedInfo, hwp_myHwProfile);
    }

#endif
    return RT_ERR_OK;

} /* end of phy_probe */

/* Function Name:
 *      phy_init
 * Description:
 *      Init the PHY chip in the specified chip.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
phy_init(void)
{
    uint32          port,unit=0;
    rt_phyctrl_t    *pPhyctrl;

    RT_INIT_REENTRY_CHK(phy_init_status);

    HWP_UNIT_TRAVS_LOCAL(unit)
    {
        if(!HWP_SWCORE_SUPPORT(unit))
            continue;

        RT_INIT_MSG("  PHY init (unit %u)\n",unit);
        HWP_PORT_TRAVS(unit,port)
        {
            if ((pPhyctrl = hal_ctrl[unit].pPhy_ctrl[port]) == NULL)
                continue;

            /* init */
            if (NULL != pPhyctrl->pPhydrv->fPhydrv_init)
            {
                pPhyctrl->pPhydrv->fPhydrv_init(unit, port);
                RT_LOG(LOG_INFO, MOD_HAL, "PHY driver init on unit %u port %u", unit, port);
            }
        }
    }

    phy_init_status = INIT_COMPLETED;

    return RT_ERR_OK;
} /* end of phy_init */




