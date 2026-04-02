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
 * $Revision: 75479 $
 * $Date: 2017-01-20 15:17:16 +0800 (Fri, 20 Jan 2017) $
 *
 * Purpose : PHY 8218D Driver APIs.
 *
 * Feature : PHY 8218D Driver APIs
 *
 */

#define PLP_SHORTFIN_SUPPORT
#define BCM_PLP_BASE_T_PHY

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <common/debug/mem.h>
#include <rtk/init.h>
#include <ioal/ioal_init.h>
#include <ioal/mem32.h>
#include <hal/chipdef/mango/rtk_mango_reg_struct.h>
#include <hal/mac/reg.h>
#include <soc/type.h>
#include <hal/common/halctrl.h>
#include <hal/mac/serdes.h>
#include <hal/mac/miim_common_drv.h>
#include <hal/mac/drv/drv_rtl9310.h>
#include <hal/phy/phy_cust1.h>
#include <hal/phy/phy_bcm_shortfin.h>
#include <phymod_custom_config.h>
#include <bcm_pm_if.h>
#include <phymod/phymod.h>
#include <epdm.h>
#include <osal/time.h>
#include <hwp/hw_profiles/hwp_senao_non_rtl_phy_info.c>
/*
 * Symbol Definition
 */
#undef PHY_CUST_BOOTLOAD_ENABLE
#undef PROV_TABLE
/*#define PHY_CUST_PHY_IMAGE_FILE             "/lib/modules/PHY_image"
#define PHY_CUST_PHY_PROV_TABLE_FILE        "/lib/modules/PHY_prov_table"
#define PHY_CUST_BCM_PHY_PORT_MAX            4*/

extern int p_ctxt_phy_bcm84898;

/*
 * Macro Declaration
 */
#define PHY_BCM_LOG(fmt, args...)      RT_LOG(LOG_INFO, (MOD_HAL|MOD_PHY), "%s:%d:"fmt, __FUNCTION__, __LINE__, ##args)
#define PHY_BCM_ERR(fmt, args...)      RT_LOG(LOG_WARNING, (MOD_HAL|MOD_PHY), "WARN.%s:%d:"fmt, __FUNCTION__, __LINE__, ##args)


/*
 * Function Declaration
 */
int32 phy_rtl9313_bcm84898_8XGE_4FibXGE_init(uint32 unit, rtk_port_t port);
int32 phy_rtl9313_bcm84898_8XGE_4FibXGE_eye_diagram_init(uint32 unit);

extern phyp_senao_phy_info_t *senao_non_rtl_phy_info;

/* Function Name:
 *      phy_rtl9313_bcm84898_8xge_4fibxgedrv_identify
 * Description:
 *      Identify the port is CUST1  PHY or not?
 * Input:
 *      unit     - unit id
 *      port     - port id
 *      model_id - model id
 *      rev_id   - revision id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - is intra serdes PHY
 *      RT_ERR_FAILED           - access failure or others
 *      RT_ERR_PHY_NOT_MATCH    - is not intra serdes PHY
 * Note:
 *      None
 */
int32
phy_rtl9313_bcm84898_8xge_4fibxgedrv_identify(uint32 unit, rtk_port_t port, uint32 model_id, uint32 rev_id)
{
    int32                   ret = 0;
    uint32                  reg1d2 = 0;
    uint32                  reg1d3 = 0;
    hal_control_t          *pHalCtrl = NULL;
    uint32                  bcm_phy_id;
    drv_smi_mdxProtoSel_t   mdxProto;

    if(phy_board_type != RTL9313_BCM84898_BOARD_TYPE_1)
        return RT_ERR_PHY_NOT_MATCH;

    PHY_BCM_LOG("U%u P%u", unit, port);
    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_PHY_NOT_MATCH;

    /*For dentify fail*/
    if(bcm_phy_id == 0)
       /*phy_osal_time_mdelay(2000);*/ /* delay 2000ms*/
       osal_time_mdelay(2000);

    RT_PARAM_CHK((NULL == (pHalCtrl = hal_ctrlInfo_get(unit))), RT_ERR_FAILED);

    if (hal_miim_portSmiMdxProto_get(unit, bcm_phy_id, &mdxProto) != RT_ERR_OK)
    {
        return RT_ERR_PHY_NOT_MATCH;
    }

    if (mdxProto != DRV_SMI_MDX_PROTO_C45)
    {
        PHY_BCM_LOG("U%u P%u change SMI to C45", unit, port);
        /* change to C45 for probing BCM PHY */
        if (hal_miim_portSmiMdxProto_set(unit, bcm_phy_id, DRV_SMI_MDX_PROTO_C45) != RT_ERR_OK)
        {
            PHY_BCM_LOG("U%u P%u change SMI to C45 fail", unit, port);
            /* PHY can only be accessed through C45 MDC/MDIO format. if SMI is not able to using C45, then return there is no match. */
            return RT_ERR_PHY_NOT_MATCH;
        }
    }

    /* PMA/PMD device identifier (Registers 1.2) */
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, bcm_phy_id, 1, 2, &reg1d2)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1 reg0x2 read fail", unit, port);
        return ret;
    }
    /* PMA/PMD device identifier (Registers 1.3) */
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, bcm_phy_id, 1, 3, &reg1d3)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1 reg0x3 read fail", unit, port);
        return ret;
    }


    /* ID1 Mapping for SHORTFIN
     * Part       A0     B0
     * BCM84898   0x5000 0x5001
     * BCM84896   0x5004 0x5005
     * BCM54998S  0x5008 0x5009
     * BCM54998ES 0x500C 0x500D
     * BCM54998   0x5010 0x5011
     * BCM54998E  0x5014 0x5015
     */
    if ((reg1d2 != 0x3590) || ((reg1d3 != 0x5000) && (reg1d3 != 0x5001)))
    {
        PHY_BCM_LOG("U%u P%u id0%x id1%x PHY NOT MATCH", unit, port, reg1d2, reg1d3);
        return RT_ERR_PHY_NOT_MATCH;
    }

#if 0
    /* recover SMI setting */
    if (mdxProto != DRV_SMI_MDX_PROTO_C45)
    {
        PHY_BCM_LOG("U%u P%u restore SMI to C22", unit, port);
        hal_miim_portSmiMdxProto_set(unit, bcm_phy_id, mdxProto);
    }
#endif
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_rtl9313_bcm84898_8XGE_4FibXGE_speed_set
 * Description:
 *      Set speed mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      speed         - link speed status 100
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK                 - OK
 *      RT_ERR_FAILED             - invalid parameter
 * Note:
 *      None
 */
int32
phy_rtl9313_bcm84898_8XGE_4FibXGE_speed_set(uint32 unit, rtk_port_t port, uint32 speed)
{
    PHY_BCM_LOG("U%u P%u speed:%u set api", unit, port, speed);

    char                               *chipname = "shortfin";
    uint32                              bcm_phy_id;
    uint32                              table_index;
    int32                               pre_speed = 0;
    int32                               bcm_speed = 0;
    int32                               intf_type = 0;
    int32                               r_clk = 0;
    int32                               if_mode = 0;
    int32                               ret = 0;
    int32                               rand_t = 0;
    static int                          randomizer;
    unsigned int                        or_shadow = 0;
    unsigned int                        shadow = 0;
    unsigned int                        or_data = 0;
    unsigned int                        data = 0;
    hal_control_t                       *pHalCtrl;
    bcm_plp_base_t_device_aux_modes_t       pre_aux_mode;
    bcm_plp_base_t_device_aux_modes_t       aux_mode;

    srand((unsigned int)time(NULL)+randomizer++); /* seed */

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort_with_table_index(unit, port, &bcm_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 0)
    {
         PHY_BCM_ERR("Speed cannot be set if auto-negotiation is enabled");
         return RT_ERR_FAILED;
    }

    pre_aux_mode.ctrl_select = bcmplpBaseTCtrlDuplex;

    bcm_plp_base_t_ability_t  abi;
    abi.speed_full_duplex = abi.speed_half_duplex = abi.pause = abi.eee = 0;

    switch(speed)
    {
      case PORT_SPEED_100M:
        /*bcm_speed = bcmplpBaseTSpeed100;*/
        bcm_speed = 100;
        break;
      case PORT_SPEED_1000M:
        abi.speed_full_duplex |= bcmplpBaseTSpeed1000;
        break;
      case PORT_SPEED_2_5G:
        abi.speed_full_duplex |= bcmplpBaseTSpeed2500;
        break;
      case PORT_SPEED_5G:
        abi.speed_full_duplex |= bcmplpBaseTSpeed5000;
        break;
      case PORT_SPEED_10G:
        abi.speed_full_duplex |= bcmplpBaseTSpeed10000;
        break;
      default:
        bcm_speed = 100;
        speed = PORT_SPEED_100M;
        /*PHY_BCM_ERR("U%u P%u speed:%u set invalid input", unit, port, speed);*/
        break;
        /*return RT_ERR_INPUT;*/
    }

    if(speed == PORT_SPEED_100M)
    {
        /*Get*/
        if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),
                                    &pre_speed, &intf_type, &r_clk,
                                    &if_mode, (void *) &pre_aux_mode) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed set - get fail", unit, port);
             return RT_ERR_FAILED;
        }

        PHY_BCM_LOG("S%d T%d C%d M% speed set - get", speed, intf_type, r_clk, if_mode);

        /*Set*/
        /*aux_mode.ctrl_select = bcmplpBaseTCtrlMdix;
        aux_mode.ctrl_value = pre_aux_mode.ctrl_value;*/
        aux_mode.ctrl_select = bcmplpBaseTCtrlDuplex;
        aux_mode.ctrl_value = TRUE;
        if (bcm_plp_mode_config_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}), bcm_speed, 0, 0, 0, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed:%u set - set fail", unit, port, speed);
             return RT_ERR_FAILED;
        }

        if (bcm_plp_base_t_autoneg_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}), DISABLED) != BCM_PM_IF_SUCCESS)
        {
              PHY_BCM_ERR("U%u P%u speed:%u set - autoNego set fail", unit, port, speed);
              return RT_ERR_FAILED;
        }

        if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        {
            PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, port);
            return ret;
        }
        /*Enable Force Auto MDIX*/
        /*Read Shadow*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x7, 0xfff8, &or_shadow)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, bcm_phy_id);
            return ret;
        }
        /*Write Shadow*/
        shadow = or_shadow | 0x7007;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0xfff8, shadow)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0xfff8 write fail", unit, bcm_phy_id);
            return ret;
        }
        /*Read Data*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x7, 0xfff8, &or_data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0xfff8 read fail", unit, bcm_phy_id);
            return ret;
        }
        /*Write Data*/
        data = or_data | 0x8200;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0xfff8, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0xfff8 write fail", unit, bcm_phy_id);
            return ret;
        }
    }
    else
    {
        if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        {
            PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, bcm_phy_id);
            return ret;
        }
        /*Disable Force Auto MDIX*/
        /*Read Shadow*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x7, 0xfff8, &or_shadow)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0xfff8 read fail", unit, bcm_phy_id);
            return ret;
        }
        /*Write Shadow*/
        shadow = or_shadow | 0x7007;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0xfff8, shadow)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0xfff8 write fail", unit, bcm_phy_id);
            return ret;
        }
        /*Read Data*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x7, 0xfff8, &or_data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0xfff8 read fail", unit, bcm_phy_id);
            return ret;
        }
        /*Write Data*/
        data = or_data & 0x7DFF;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0xfff8, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0xfff8 write fail", unit, bcm_phy_id);
            return ret;
        }


        if (bcm_plp_base_t_autoneg_ability_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}), &abi ) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed:%u set - AutoNego ability set fail", unit, port, speed);
             return RT_ERR_FAILED;
        }
        if (bcm_plp_base_t_autoneg_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}), ENABLED) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed:%u set - autoNego set fail", unit, port, speed);
             return RT_ERR_FAILED;
    	}
    }

#if 1  /*10G-PHY84898, 100M workaround*/

    rand_t = rand() % 100 + 50;
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);

    /* 100M workaround */
    if (port==0)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 2, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 2, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }else if (port==8)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 3, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 3, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }else if (port==16)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 6, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 6, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }else if (port==24)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 7, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 7, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }else if (port==32)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 10, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 10, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }else if (port==40)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 11, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 11, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }else if (port==48)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 14, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 14, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }else if (port==50)
    {
        /*Disable serdes 2 AN*/
        if((hal_serdes_reg_set(unit, 15, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
        /*phy_osal_time_mdelay(rand_t);*/
        osal_time_mdelay(rand_t);
        if((hal_serdes_reg_set(unit, 15, 7, 17, 0x0540)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }

#endif

    senao_non_rtl_phy_info->port_mapping[bcm_phy_id].speed = speed;
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm84898_100MportWorkaround_set
 * Description:
 *      Set PHY interface status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      enable        - admin configuration of PHY interface
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_bcm84898_100MportWorkaround_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{

    PHY_BCM_LOG("U%u P%u enable:%d set api", unit, port, enable);
    char                               *chipname = "shortfin";
    uint32                              bcm_phy_id;
    int32                               speed = 0;
    int32                               intf_type = 0;
    int32                               r_clk = 0;
    int32                               if_mode = 0;
    int32                               rand_t = 0;
    static int                          randomizer;
    bcm_plp_base_t_device_aux_modes_t       pre_aux_mode;

    srand((unsigned int)time(NULL)+randomizer++); /* seed */

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (enable == ENABLED) /* link up */
    {
        if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),
                                    &speed, &intf_type, &r_clk,
                                    &if_mode, (void *) &pre_aux_mode) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed set - get fail", unit, port);
             return RT_ERR_FAILED;
        }

        rand_t = rand() % 100 + 50;
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);

        /* 100M workaround */
        if (port==0)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 2, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 2, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }else if (port==8)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 3, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 3, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }else if (port==16)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 6, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 6, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }else if (port==24)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 7, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 7, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }else if (port==32)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 10, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 10, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }else if (port==40)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 11, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 11, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }else if (port==48)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 14, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 14, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }else if (port==50)
        {
            /*Disable serdes 2 AN*/
            if((hal_serdes_reg_set(unit, 15, 7, 17, 0x054F)) != RT_ERR_OK)
                return RT_ERR_FAILED;
            /*phy_osal_time_mdelay(rand_t);*/
            osal_time_mdelay(rand_t);
            if((hal_serdes_reg_set(unit, 15, 7, 17, 0x0540)) != RT_ERR_OK)
                return RT_ERR_FAILED;
        }
    }
    return RT_ERR_OK;
}




int32
phy_bcm84898_init_senao_workaround(uint32 unit, rtk_port_t port)
{
    hal_control_t   *pHalCtrl;
    int32           ret = 0;
    unsigned int    mdio_addr = 0;
    unsigned int    or_data = 0;
    unsigned int    data = 0;

    #if 0
    /*Force to 1G*/
    ioal_mem32_write(unit, 0xdcc, 0x32a0F);
    ioal_mem32_write(unit, 0xdec, 0x32a0F);
    ioal_mem32_write(unit, 0xe0c, 0x32a0F);
    ioal_mem32_write(unit, 0xe2c, 0x32a0F);
    ioal_mem32_write(unit, 0xe4c, 0x32a0F);
    ioal_mem32_write(unit, 0xe6c, 0x32a0F);
    ioal_mem32_write(unit, 0xe8c, 0x32a0F);
    ioal_mem32_write(unit, 0xe94, 0x32a0F);
    #endif

    /***************************************************************/
    /*1. GET 0x401c register first to check bit 8
      2. SET 1e.0x401c bit8=1 to all 8 ports
      3. GET 0x401c register first to check bit 8
      4. AND wait until 1e.0x400e=0
      5. executed CMD:
         set the following to all 8 ports
         set 1e.0x4038=1         <-- related USXGMII
         set 1e.0x4039=0         <-- related AN
         set 1e.0x403a=0x4       <-- related speed mode
         set 1e.0x403b=0
         set 1e.0x403c=0
         set 1e.0x4005=0x8026
         wait until 1e.0x4037=4  <--last command has been executed

      6. SET 1e.0x401c bit8=0 to all 8 ports
      7. AND wait until 1e.0x400e=0*/
    /***************************************************************/

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x4;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA5*/
    // data = 0x0;
    /*(B0-v02-02-03)Please make sure in SET_USXGMII command, keep data4=2(Realtek switch) and data5=1 (frequency unlocked)*/
    /*Compatibility Mode
      Enable    : 0x0
      Disable   : 0x1
     */
    data = 0x1;/*compatibility mode*/
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x8026;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);

    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*Init for force speed 100M*/
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1, 0x0, 0x2000)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1 addr0x0 write fail", unit, mdio_addr);
        return ret;
    }

    /************************************************************************************************************************/
    phy_bcm_shortfin_enable_set(unit, port, DISABLED);

    /*phy_osal_time_mdelay(600);*/
    osal_time_mdelay(600);

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x1;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x4;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA5*/
    // data = 0x0;
    /*(B0-v02-02-03)Please make sure in SET_USXGMII command, keep data4=2(Realtek switch) and data5=1 (frequency unlocked)*/
    /*Compatibility Mode
      Enable    : 0x0
      Disable   : 0x1
     */
    data = 0x1;/*compatibility mode*/
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x8026;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);
 
    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);
    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*Init for force speed 100M*/
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1, 0x0, 0x2000)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1 addr0x0 write fail", unit, mdio_addr);
        return ret;
    }

    phy_bcm_shortfin_enable_set(unit, port, ENABLED);

    return RT_ERR_OK;
}

#define BCM_PHY_UPGRADE 1
#if BCM_PHY_UPGRADE
extern char  *plp_shortfin_xgbaset_firmware_ver;
extern unsigned int plp_shortfin_xgbaset_firmware_ver_id;
#endif
/* Function Name:
 *      phy_rtl9313_bcm84898_8XGE_4FibXGE_init
 * Description:
 *      Initialize PHY.
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_rtl9313_bcm84898_8XGE_4FibXGE_init(uint32 unit, rtk_port_t port)
{
    PHY_BCM_LOG("U%u P%u init api", unit, port);

    char                    *chipname = "shortfin";
    uint32                  bcm_phy_id;
    int32                   options = 0x0; /*load none*/
    //int32                   bcm_phy_id_counter = 0;
    //bcm_plp_base_t_device_aux_modes_t  aux_mode;
    uint32                  regval_37=0, regval_38=0, regval_39=0,regval_3a=0, regval_3b=0, regval_3c=0;
    uint32                  addr_reg0 = 0, addr_reg9 = 0, addr_reg10 = 0;
    int32                   ret;
#if BCM_PHY_UPGRADE
    int32                   rv = 0;
    uint32                  fw_version = 0;
    static uint32           upgrade_once_flag = 0;
    hal_control_t           *pHalCtrlx = NULL;
#endif

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

#if BCM_PHY_UPGRADE
    if (upgrade_once_flag == 0)
    {
        pHalCtrlx = hal_ctrlInfo_get(unit);
        if (pHalCtrlx != NULL)
        {
            if ((rv = MACDRV(pHalCtrlx)->fMdrv_miim_mmd_read(unit, bcm_phy_id, 30, 0x400f, &fw_version)) != RT_ERR_OK)
                PHY_BCM_ERR("U%u P%u Read Internal PHY firmware failed(%d)", unit, port, rv);
            else
                osal_printf("Internal PHY firmware version: 0x%04X\n", fw_version);
            osal_printf("Image PHY firmware version %s(0x%04X)\n", plp_shortfin_xgbaset_firmware_ver, plp_shortfin_xgbaset_firmware_ver_id);
            if (rv == 0 && fw_version != plp_shortfin_xgbaset_firmware_ver_id)
            {
                osal_printf("Upgrade PHY firmware from 0x%04X to 0x%04X\n", fw_version, plp_shortfin_xgbaset_firmware_ver_id);
                /*force upgrade phy firmware to EEPROM*/
                options = bcmpmFirmwareLoadMethodProgEEPROM;
            }
        }
        upgrade_once_flag++;
    }
#endif

    if (bcm_plp_init(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),
                       shortfin_api_mdio_read, shortfin_api_mdio_write, options) != BCM_PM_IF_SUCCESS)
    {
        PHY_BCM_ERR("U%u P%u init fail", unit, port);
        return RT_ERR_FAILED;
    }

    #if 0
    /*Disable copper port mac polling phy*/
    hal_miim_pollingEnable_set(unit, port, ENABLED);

    rtl9310_miim_pollingEnable_set(unit, port, ENABLED);
    #endif

    /*phy_osal_time_mdelay(1000);*/
    /*osal_time_mdelay(1000);*/

    #if 0
    if(bcm_phy_id == 7)
    {
        for(bcm_phy_id_counter=0; bcm_phy_id_counter <=7; bcm_phy_id_counter++)
        {
            /*aux_mode.ctrl_select = bcmplpBaseTCtrlSerdesAutoNeg;*/
            /*aux_mode.ctrl_value = TRUE;*/
            aux_mode.ctrl_select = bcmplpBaseTCtrlNone;
            if (bcm_plp_mode_config_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id_counter, 1, 0x1}),
                                        10000,
                                        bcm_pm_InterfaceUSXGMII,
                                        bcm_pm_RefClk156Mhz,
                                        bcmpmUsxgmiiModeSingle, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
            {
                PHY_BCM_ERR("%u P%u init fail", unit, port);
                return RT_ERR_FAILED;
            }
        }
    }
    #else
    /*Disable serdes 2 AN*/
    if((hal_serdes_reg_set(unit, 2, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 3 AN*/
    if((hal_serdes_reg_set(unit, 3, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 4 AN*/
    if((hal_serdes_reg_set(unit, 6, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 5 AN*/
    if((hal_serdes_reg_set(unit, 7, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 6 AN*/
    if((hal_serdes_reg_set(unit, 10, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 7 AN*/
    if((hal_serdes_reg_set(unit, 11, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 8 AN*/
    if((hal_serdes_reg_set(unit, 14, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 9 AN*/
    if((hal_serdes_reg_set(unit, 15, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

     /*phy_osal_time_mdelay(100);*/
     osal_time_mdelay(100);

    /************************************************************************************************************************/
    /* PAIR SWAP*/
    /************************************************************************************************************************/
    /*GET:PAIR-SWAP*/
    bcm_plp_reg_value_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4005, 0x8000);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4037, &regval_37);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4038, &regval_38);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4039, &regval_39);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x403A, &regval_3a);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x403B, &regval_3b);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x403C, &regval_3c);

    /*phy_osal_time_mdelay(250);*/
    osal_time_mdelay(250);

    /*SET:PAIR-SWAP*/
    if ((port==8) || (port==24) || (port==40) || (port==50))
    {
        bcm_plp_reg_value_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4039, 0x001B);
        bcm_plp_reg_value_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4005, 0x8001);
        bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4037, &regval_37);
        bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4039, &regval_39);
    }
    /************************************************************************************************************************/

    /* register address at 10G phy */
    if(port==0)
    {
        addr_reg0   = 0x107FFE0;
        addr_reg9   = 0x127FFE9;
        addr_reg10  = 0x167FFEA;

        if ((ret = reg_write(unit, MANGO_SMI_10GPHY_POLLING_SEL2r, &addr_reg0)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u ret%u set MANGO_SMI_10GPHY_POLLING_SEL2r fail", unit, port, ret);
        }

        if ((ret = reg_write(unit, MANGO_SMI_10GPHY_POLLING_SEL3r, &addr_reg9)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u ret%u set MANGO_SMI_10GPHY_POLLING_SEL3r fail", unit, port, ret);
        }

        if ((ret = reg_write(unit, MANGO_SMI_10GPHY_POLLING_SEL4r, &addr_reg10)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u ret%u set MANGO_SMI_10GPHY_POLLING_SEL4r fail", unit, port, ret);
        }
        /*register set 0xcc0 0x10550*/
        ioal_mem32_write(unit, 0xcc0, 0x10550);

    }

    /*---------------------------------------------*/
    /* Register default value for MAC's flow-control */
    /* MANGO_FC_PORT_ACT_CTRLr [0xBB00504C] */
    /* Bits[24] : ACT ; Bits[12:0] : ALLOW_PAGE_CNT */
    /* For copper ports */
    ioal_mem32_write(unit, 0x504C, 0x0100007f);
    ioal_mem32_write(unit, 0x506C, 0x0100007f);
    ioal_mem32_write(unit, 0x508C, 0x0100007f);
    ioal_mem32_write(unit, 0x50AC, 0x0100007f);
    ioal_mem32_write(unit, 0x50CC, 0x0100007f);
    ioal_mem32_write(unit, 0x50EC, 0x0100007f);
    ioal_mem32_write(unit, 0x510C, 0x0100007f);
    ioal_mem32_write(unit, 0x5114, 0x0100007f);

    /* For Fiber ports */
    ioal_mem32_write(unit, 0x511C, 0x0100007f);
    ioal_mem32_write(unit, 0x5120, 0x0100007f);
    ioal_mem32_write(unit, 0x5124, 0x0100007f);
    ioal_mem32_write(unit, 0x5128, 0x0100007f);
    /*---------------------------------------------*/

    /* eye-diagram for copper port */
    if(phy_rtl9313_bcm84898_8XGE_4FibXGE_eye_diagram_init(unit)!= RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }

    if(phy_bcm84898_init_senao_workaround(unit, port)!= RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }

    #endif

    return RT_ERR_OK;
}

int32
phy_rtl9313_bcm84898_8XGE_4FibXGE_eye_diagram_init(uint32 unit)
{
    hal_control_t   *pHalCtrl;
    int32           ret = 0;
    unsigned int    mdio_addr = 0;
    unsigned int    or_data = 0;
    unsigned int    data = 0;
    rtk_port_t      port=0;

    /***************************************************************/
    /*1. GET 0x401c register first to check bit 8
      2. SET 1e.0x401c bit8=1
      3. AND wait until 1e.0x400e=0
      4. executed CMD:
         set 1e.0x4038=0x0       <-- Speed
         set 1e.0x4039=0x5000    <-- Pre tap + Main tap
         set 1e.0x403a=0x0       <-- Post1 tap + Post2 tap
         set 1e.0x403b=0x0       <-- Post3 tap + tx_hpf
         set 1e.0x4005=0x802c
         wait until 1e.0x4037=4  <--last command has been executed
      5. SET 1e.0x401c bit8=0
      6. AND wait until 1e.0x400e=0*/
    /***************************************************************/

    /* Serdes eye-diag tuning.
    port: 0   0
    port: 8 * 1
    port:16   2
    port:24 * 3
    port:32 * 4
    port:40   5
    port:48 * 6
    port:50   7
    */

    /*Serdes 8*/
    //mdio_addr = 1;
    port=8;
    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x3700;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    
    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x802c;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
    }

    /************************************************************************************************************************/
    /*Serdes 24*/
    //mdio_addr = 3;
    port=24;
    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x3700;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    
    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x802c;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);

    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /************************************************************************************************************************/
    /*Serdes 32*/
    //mdio_addr = 4;
    port=32;
    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x3700;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    
    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x802c;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100); 

    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /************************************************************************************************************************/
    /*Serdes 48*/
    //mdio_addr = 6;
    port=48;
    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x3700;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    
    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x802c;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);

    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_rtl9313_bcm84898_8XGE_4FibXGE_reset_set
 * Description:
 *      Set PHY standard register Reset bit (0.15).
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9313_bcm84898_8XGE_4FibXGE_reset_set(uint32 unit, rtk_port_t port)
{

    PHY_BCM_LOG("U%u P%u reset set api", unit, port);

    /*int32       ret;*/
    char        *chipname = "shortfin";
    uint32      bcm_phy_id;
    uint32      reset = 1; /*0:Hard, 1:Soft*/
    uint32      regval_37=0, regval_38=0, regval_39=0,regval_3a=0, regval_3b=0, regval_3c=0;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*ret = bcm_plp_reset_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}), reset, 0);*/
    if (bcm_plp_reset_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}), reset, 0) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u reset set fail", unit, port);
         return RT_ERR_FAILED;
    }

    /************************************************************************************************************************/
    /* PAIR SWAP*/
    /************************************************************************************************************************/
    /*GET:PAIR-SWAP*/
    bcm_plp_reg_value_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4005, 0x8000);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4037, &regval_37);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4038, &regval_38);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4039, &regval_39);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x403A, &regval_3a);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x403B, &regval_3b);
    bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x403C, &regval_3c);

    /*phy_osal_time_mdelay(250);*/
    osal_time_mdelay(250);

    /*SET:PAIR-SWAP*/
    if ((port==8) || (port==24) || (port==40) || (port==50))
    {
        bcm_plp_reg_value_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4039, 0x001B);
        bcm_plp_reg_value_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4005, 0x8001);
        bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4037, &regval_37);
        bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm84898, bcm_phy_id, 0, 0x1}),  0x1e, 0x4039, &regval_39);
    }
    /************************************************************************************************************************/

    if(phy_bcm84898_init_senao_workaround(unit, port)!= RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

int32
phy_bcm84898_swMacPollPhyStatus_get(uint32 unit, rtk_port_t port, rtk_port_swMacPollPhyStatus_t *pphyStatus)
{

    uint32      bcm_phy_id;
    uint32      reslStatus = 0, speed5_4, speed10_9, /*tx_pause, rx_pause, */duplex;
    uint32      isConnected;
    uint32      speed;
    uint32      linkStatus;

    rtk_port_masterSlave_t   masterSlaveCfg;
    rtk_port_masterSlave_t   masterSlaveActual;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    phy_bcm_shortfin_speed_get(unit, port, &speed);

    phy_bcm_shortfin_linkStatus_get(unit, port, &linkStatus);

    switch (speed)
    {
      case PORT_SPEED_10G:
         isConnected = TRUE;
         speed5_4 = 0x0;
         speed10_9 = 0x1;
         break;
      case PORT_SPEED_5G:
         isConnected = TRUE;
         speed5_4 = 0x2;
         speed10_9 = 0x1;
         break;
      case PORT_SPEED_2_5G:
         isConnected = TRUE;
         speed5_4 = 0x1;
         speed10_9 = 0x1;
         break;
      case PORT_SPEED_1000M:
         isConnected = TRUE;
         speed5_4 = 0x2;
         speed10_9 = 0x0;
         break;
      case PORT_SPEED_100M:
         isConnected = TRUE;
         speed5_4 = 0x1;
         speed10_9 = 0x0;
         break;
      default:
         isConnected = FALSE;
#if 0
         /* link is down or port is inactive... */
         if (connStatus.rxLinkUp == True)
         {
            PHY_AQR_LOG("U%u P%u link unknown", unit, port);
         }
#endif
         speed5_4 = 0x3;
         speed10_9 = 0x3;
         break;
    }

    phy_bcm_shortfin_duplex_get(unit, port, &duplex);

    phy_bcm_shortfin_masterSlave_get(unit, port, &masterSlaveCfg, &masterSlaveActual);

#if 0
    //if (connStatus.rxLinkUp == True)
    if (isConnected == TRUE)
    {
        _phy_cust2_swMacPollPhyStatusTxRxPause_get(unit, port, &aq_port, &tx_pause, &rx_pause);
        duplex = (connStatus.duplex == True) ? 1 : 0;
    }
    else
    {
        duplex = 1;
        tx_pause = 0;
        rx_pause = 0;
    }
#endif

    reslStatus = REG32_FIELD_SET(reslStatus, 0, PHY_RESL_REG_MDI_OFFSET, PHY_RESL_REG_MDI_MASK);
    //reslStatus = REG32_FIELD_SET(reslStatus, ((connStatus.rxLinkUp == True) ? 1 : 0), PHY_RESL_REG_LINK_OFFSET, PHY_RESL_REG_LINK_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, ((isConnected == TRUE) ? 1 : 0), PHY_RESL_REG_LINK_OFFSET, PHY_RESL_REG_LINK_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, duplex, PHY_RESL_REG_DUPLEX_OFFSET, PHY_RESL_REG_DUPLEX_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, speed5_4, PHY_RESL_REG_SPEED0_OFFSET, PHY_RESL_REG_SPEED0_MASK);
    /*reslStatus = REG32_FIELD_SET(reslStatus, ((tx_pause) ? 1 : 0), PHY_RESL_REG_TX_PAUSE_OFFSET, PHY_RESL_REG_TX_PAUSE_MASK);*/
    /*reslStatus = REG32_FIELD_SET(reslStatus, ((rx_pause) ? 1 : 0), PHY_RESL_REG_RX_PAUSE_OFFSET, PHY_RESL_REG_RX_PAUSE_MASK);*/
    reslStatus = REG32_FIELD_SET(reslStatus, speed10_9, PHY_RESL_REG_SPEED1_OFFSET, PHY_RESL_REG_SPEED1_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, ((masterSlaveActual == PORT_MASTER_MODE) ? 1 : 0), PHY_RESL_REG_MASTER_SLAVE_OFFSET, PHY_RESL_REG_MASTER_SLAVE_MASK);

    pphyStatus->reslStatus = reslStatus;
    pphyStatus->media = PORT_MEDIA_COPPER;
    /*For SNOS performance*/
    //phy_osal_time_usleep(50 * 1000); /* sleep 50mS */
    return RT_ERR_OK;
}


/* Function Name:
 *      phy_bcm84898drv_mapperInit
 * Description:
 *      Initialize PHY driver.
 * Input:
 *      pPhydrv - pointer of phy driver
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
phy_rtl9313_bcm84898_8xge_4fibxgedrv_mapperInit(rt_phydrv_t *pPhydrv)
{
    pPhydrv->phydrv_index = RT_PHYDRV_CUST2;
    pPhydrv->fPhydrv_init = phy_bcm_shortfin_init;
    pPhydrv->fPhydrv_media_get = phy_bcm_shortfin_media_get;
    pPhydrv->fPhydrv_autoNegoEnable_get = phy_bcm_shortfin_autoNegoEnable_get;
    pPhydrv->fPhydrv_autoNegoEnable_set = phy_bcm_shortfin_autoNegoEnable_set;
    pPhydrv->fPhydrv_autoNegoAbility_get = phy_bcm_shortfin_autoNegoAbility_get;
    pPhydrv->fPhydrv_autoNegoAbility_set = phy_bcm_shortfin_autoNegoAbility_set;
    pPhydrv->fPhydrv_duplex_get = phy_bcm_shortfin_duplex_get;
    pPhydrv->fPhydrv_duplex_set = phy_bcm_shortfin_duplex_set;
    pPhydrv->fPhydrv_speed_get = phy_bcm_shortfin_speed_get;
    pPhydrv->fPhydrv_speed_set = phy_rtl9313_bcm84898_8XGE_4FibXGE_speed_set;
    pPhydrv->fPhydrv_enable_set = phy_bcm_shortfin_enable_set;
    pPhydrv->fPhydrv_100MportWorkaround_set = phy_bcm84898_100MportWorkaround_set;

    pPhydrv->fPhydrv_reg_mmd_get = phy_bcm_shortfin_reg_mmd_get;
    pPhydrv->fPhydrv_reg_mmd_set = phy_bcm_shortfin_reg_mmd_set;
    //pPhydrv->fPhydrv_portimageFlash_load = phy_cust2_imageFlash_load;*/
    pPhydrv->fPhydrv_eeeEnable_get = phy_bcm_shortfin_eeeEnable_get;
    pPhydrv->fPhydrv_eeeEnable_set = phy_bcm_shortfin_eeeEnable_set;
    pPhydrv->fPhydrv_crossOverMode_get = phy_bcm_shortfin_crossOverMode_get;
    pPhydrv->fPhydrv_crossOverMode_set = phy_bcm_shortfin_crossOverMode_set;
    pPhydrv->fPhydrv_crossOverStatus_get = phy_bcm_shortfin_crossOverStatus_get;
    pPhydrv->fPhydrv_masterSlave_get = phy_bcm_shortfin_masterSlave_get;
    pPhydrv->fPhydrv_masterSlave_set = phy_bcm_shortfin_masterSlave_set;
    pPhydrv->fPhydrv_loopback_get = phy_bcm_shortfin_loopback_get;
    pPhydrv->fPhydrv_loopback_set = phy_bcm_shortfin_loopback_set;
    pPhydrv->fPhydrv_rtctResult_get = phy_bcm_shortfin_rtctResult_get;
    pPhydrv->fPhydrv_rtct_start =  phy_bcm_shortfin_rtct_start;
    pPhydrv->fPhydrv_ieeeTestMode_set = phy_bcm_shortfin_ieeeTestMode_set;
    pPhydrv->fPhydrv_portEyeMonitor_start = phy_bcm_shortfin_portEyeMonitor_start;
    /*pPhydrv->fPhydrv_polar_get = phy_bcm_shortfin_polar_get;*/
    /*pPhydrv->fPhydrv_macIntfSerdesMode_get = phy_cust2_macIntfSerdesMode_get;*/
    pPhydrv->fPhydrv_linkStatus_get = phy_bcm_shortfin_linkStatus_get;
    pPhydrv->fPhydrv_reset_set = phy_rtl9313_bcm84898_8XGE_4FibXGE_reset_set;
}
