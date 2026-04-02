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
#include <rtk/init.h>
#include <soc/type.h>
#include <hal/common/halctrl.h>
#include <hal/mac/serdes.h>
#include <hal/mac/miim_common_drv.h>
/*#include <hal/phy/phy_cust1.h>*/
#include <phymod_custom_config.h>
#include <bcm_pm_if.h>
#include <phymod/phymod.h>
#include <epdm.h>
#include <ioal/mem32.h>
#include <osal/time.h>
#include <hwp/hw_profiles/hwp_senao_non_rtl_phy_info.c>
/*
 * Symbol Definition
 */
#undef PHY_CUST_BOOTLOAD_ENABLE
#undef PROV_TABLE

/*
 * Data Declaration
 */
#define PORT_NUM_IN_BCM50998                8
rt_phyInfo_t phy_bcm50998_info =
{
    .phy_num    = PORT_NUM_IN_BCM50998,
    .eth_type   = HWP_2_5GE_ID,
    .isComboPhy = {0, 0, 0, 0, 0, 0, 0, 0},
    .flags      = (RTK_PHYINFO_FLAG_NO_RES_REG),    /* PHY attribute flags. Set corrisponding flags for the PHY */
};

#define PORT_NUM_IN_BCM84898                8
rt_phyInfo_t phy_bcm84898_info =
{
    .phy_num    = PORT_NUM_IN_BCM84898,
    .eth_type   = HWP_XGE,
    .isComboPhy = {0, 0, 0, 0, 0, 0, 0, 0},
    .flags      = (RTK_PHYINFO_FLAG_NO_RES_REG),    /* PHY attribute flags. Set corrisponding flags for the PHY */
};


int p_ctxt_phy_bcm_shortfin = NULL;

/*
 * Macro Declaration
 */
#define PHY_BCM_LOG(fmt, args...)      RT_LOG(LOG_INFO, (MOD_HAL|MOD_PHY), "%s:%d:"fmt, __FUNCTION__, __LINE__, ##args)
#define PHY_BCM_ERR(fmt, args...)      RT_LOG(LOG_WARNING, (MOD_HAL|MOD_PHY), "WARN.%s:%d:"fmt, __FUNCTION__, __LINE__, ##args)
int p_ctxt_phy_bcm50998 = NULL;
int p_ctxt_phy_bcm84898 = NULL;

/*
 * Function Declaration
 */
extern int32 phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_init(uint32 unit, rtk_port_t port);
extern int32 phy_rtl9313_bcm84898_8XGE_4FibXGE_init(uint32 unit, rtk_port_t port);
extern int32 phy_rtl9313_4x8218d_2xbcm50998e_4XGE_init(uint32 unit, rtk_port_t port);
extern int32 phy_rtl9302b_2x8218d_bcm50998e_4XGE_init(uint32 unit, rtk_port_t port);
int32 _phy_bcm_shortfin_rtkPort_to_bcmPort(uint32 unit, rtk_port_t port, uint32 *pbcm_phy_id);
int32 _phy_bcm_shortfin_rtkPort_to_bcmPort_with_table_index(uint32 unit, rtk_port_t port, uint32 *pbcm_phy_id, uint32 *table_index);
int32 _phy_bcm_shortfin_bcmPort_to_rtkPort(uint32 *unit, rtk_port_t *port, uint32 pbcm_phy_id);

extern phyp_senao_phy_info_t *senao_non_rtl_phy_info;

/* Function Name:
 *      _phy_bcm_shortfin_rtkPort_to_bcmPort
 * Description:
 *      Transfert rtk unit/port to bcm phy port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pbcm_phy_id - bcm port id
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_bcm_shortfin_rtkPort_to_bcmPort(uint32 unit, rtk_port_t port, uint32 *pbcm_phy_id)
{
    uint32 index = 0;

    if(senao_non_rtl_phy_info==NULL)
        return RT_ERR_FAILED;

    for(index = 0; senao_non_rtl_phy_info->port_mapping[index].unit != HWP_END; index++)
    {
        if(unit == senao_non_rtl_phy_info->port_mapping[index].unit && port == senao_non_rtl_phy_info->port_mapping[index].mac_id)
        {
            *pbcm_phy_id = senao_non_rtl_phy_info->port_mapping[index].phy_id;
            return RT_ERR_OK;
        }
    }
    PHY_BCM_ERR("U%u P%u port mapping fail", unit, port);
    return RT_ERR_FAILED;
}

/* Function Name:
 *      _phy_bcm_shortfin_rtkPort_to_bcmPort_with_table_index
 * Description:
 *      Transfert rtk unit/port to bcm phy port and get array index
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pbcm_phy_id - bcm port id
 *      table_index - array index
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_bcm_shortfin_rtkPort_to_bcmPort_with_table_index(uint32 unit, rtk_port_t port, uint32 *pbcm_phy_id, uint32 *table_index)
{
    uint32 index = 0;

    if(senao_non_rtl_phy_info==NULL)
        return RT_ERR_FAILED;

    for(index = 0; senao_non_rtl_phy_info->port_mapping[index].unit != HWP_END; index++)
    {
        if(unit == senao_non_rtl_phy_info->port_mapping[index].unit && port == senao_non_rtl_phy_info->port_mapping[index].mac_id)
        {
            *pbcm_phy_id = senao_non_rtl_phy_info->port_mapping[index].phy_id;
            *table_index = index;
            return RT_ERR_OK;
        }
    }
    PHY_BCM_ERR("U%u P%u port mapping/get table index fail", unit, port);
    return RT_ERR_FAILED;
}

/* Function Name:
 *      _phy_bcm_shortfin_bcmPort_to_rtkPort
 * Description:
 *      Transfert bcm phy port to rtk unit/port
 * Input:
 *      pbcm_phy_id - bcm port id
 * Output:
 *      unit - unit id
 *      port - port id
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_bcm_shortfin_bcmPort_to_rtkPort(uint32 *unit, rtk_port_t *port, uint32 pbcm_phy_id)
{
    uint32 index = 0;

    if(senao_non_rtl_phy_info==NULL)
        return RT_ERR_FAILED;

    for(index = 0; senao_non_rtl_phy_info->port_mapping[index].unit != HWP_END; index++)
    {
        if(pbcm_phy_id == senao_non_rtl_phy_info->port_mapping[index].phy_id)		
        {
            *unit = senao_non_rtl_phy_info->port_mapping[index].unit;
            *port = senao_non_rtl_phy_info->port_mapping[index].mac_id;
            return RT_ERR_OK;
        }
    }
    PHY_BCM_ERR("P%d port mapping fail", pbcm_phy_id);
    return RT_ERR_FAILED;
}

#if 0
/* Function Name:
 *      _phy_bcm_shortfin_bcmSysIntfMode_to_rtkSdsMode
 * Description:
 *      Transfert BCM System Interface Mode to RTK SerDes Mode
 * Input:
 *      bcmSysIntfOpMode - BCM System Interface Mode
 * Output:
 *      prtkSerdesMode  - rtk serdes mode
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_bcm_shortfin_bcmSysIntfMode_to_rtkSdsMode(BCM_API_SysIntfOpMode bcmSysIntfOpMode, rt_serdesMode_t *prtkSerdesMode)
{
    switch (bcmSysIntfOpMode)
    {
      case BCM_API_SIOM_USXGMII:
        *prtkSerdesMode = RTK_MII_USXGMII_10GQXGMII;
        break;
      case BCM_API_SIOM_XFI:
        *prtkSerdesMode = RTK_MII_10GR;
        break;
      case BCM_API_SIOM_Other:
        *prtkSerdesMode = RTK_MII_2500Base_X;
        break;
      case BCM_API_SIOM_SGMII:
        *prtkSerdesMode = RTK_MII_SGMII;
        break;
      case BCM_API_SIOM_OFF:
        *prtkSerdesMode = RTK_MII_DISABLE;
        break;
      default:
        *prtkSerdesMode = RTK_MII_DISABLE;
        return RT_ERR_FAILED;
    }


    return RT_ERR_OK;

}
#endif


/* Function Name:
 *      phy_bcm_shortfin_media_get
 * Description:
 *      Get PHY media type.
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pMedia - pointer buffer of phy media type
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - invalid parameter
 *      RT_ERR_NULL_POINTER - input parameter is null pointer
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_media_get(uint32 unit, rtk_port_t port, rtk_port_media_t *pMedia)
{
    PHY_BCM_LOG("U%u P%u media get api", unit, port);

    *pMedia = PORT_MEDIA_COPPER;
    return RT_ERR_OK;
}


/* Function Name:
 *      phy_bcm_shortfin_autoNegoEnable_get
 * Description:
 *      Get autonegotiation enable status of the specific port
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
phy_bcm_shortfin_autoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    PHY_BCM_LOG("U%u P%u autoNego get api", unit, port);

    char           *chipname = "shortfin";
    uint32          bcm_phy_id;
    uint32          table_index;
    int32           aneg=0;
    int32           andn=0;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort_with_table_index(unit, port, &bcm_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 1)
    {
        *pEnable = DISABLED;
        return RT_ERR_OK;
    }

    if (bcm_plp_base_t_autoneg_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &aneg, &andn) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u autoNego get fail", unit, port);
         return 0;
    }
	
    switch (aneg)
    {
      case ENABLED:
        *pEnable = ENABLED;
        break;
      case DISABLED:
        *pEnable = DISABLED;
        break;
      default:
        PHY_BCM_ERR("U%u P%u autoNego get fail", unit, port);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_autoNegoEnable_set
 * Description:
 *      Set autonegotiation enable status of the specific port
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
phy_bcm_shortfin_autoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{

    PHY_BCM_LOG("U%u P%u autoNego set api", unit, port);

    char           *chipname = "shortfin";
    uint32          bcm_phy_id;
    uint32          table_index;
    int32           aneg=0;
    int32           ret = 0;
    unsigned int    or_shadow = 0;
    unsigned int    shadow = 0;
    unsigned int    or_data = 0;
    unsigned int    data = 0;
    hal_control_t  *pHalCtrl;

    switch (enable)
    {
      case ENABLED:
        aneg = ENABLED;
        break;
      case DISABLED:
        aneg = DISABLED;
        break;
      default:
        PHY_BCM_ERR("U%u P%u autoNego set invalid input enable:%d", unit, port, enable);
        return RT_ERR_FAILED;
    }

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort_with_table_index(unit, port, &bcm_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 1 && aneg == DISABLED)
        return RT_ERR_OK;

    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 1 && aneg == ENABLED)
    {
        if (bcm_plp_base_t_autoneg_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), DISABLED) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u autoNego set fail", unit, port);
             return RT_ERR_FAILED;
        }
    }

    if(aneg == ENABLED)
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
    }

    if (bcm_plp_base_t_autoneg_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), aneg) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u autoNego set fail", unit, port);
         return RT_ERR_FAILED;
    }

    if(aneg == DISABLED)
         senao_non_rtl_phy_info->port_mapping[table_index].force_auto = 1;
    else
         senao_non_rtl_phy_info->port_mapping[table_index].force_auto = 0;

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_autoNegoAbility_get
 * Description:
 *      Get ability advertisement for auto negotiation of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pAbility - pointer to PHY auto negotiation ability
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_autoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    PHY_BCM_LOG("U%u P%u autoNego Ability get api", unit, port);
  
    char                     *chipname = "shortfin";    	
    uint32                    bcm_phy_id;
    int32                     ret = 0;
    unsigned int              data = 0;
    hal_control_t            *pHalCtrl;
    bcm_plp_base_t_ability_t  abi;
    abi.speed_full_duplex = abi.speed_half_duplex = abi.pause = abi.eee = 0;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_base_t_autoneg_ability_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &abi ) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u autoNego Ability get fail", unit, port);
         return RT_ERR_FAILED;
    }

    if(abi.speed_full_duplex & bcmplpBaseTSpeed100)
        pAbility->Full_100 = 1;
    else
        pAbility->Full_100 = 0;
    if(abi.speed_full_duplex & bcmplpBaseTSpeed1000)
        pAbility->Full_1000 = 1;
    else
        pAbility->Full_1000 = 0;
    if(abi.speed_full_duplex & bcmplpBaseTSpeed2500)
        pAbility->adv_2_5G = 1;
    else
        pAbility->adv_2_5G = 0;
    /*10G*/
    if(abi.speed_full_duplex & bcmplpBaseTSpeed10000)
        pAbility->adv_10GBase_T = 1;
    else
        pAbility->adv_10GBase_T = 0;
    /*5G*/
    if(abi.speed_full_duplex & bcmplpBaseTSpeed5000)
        pAbility->adv_5G = 1;
    else
        pAbility->adv_5G = 0;

    if(abi.speed_half_duplex & bcmplpBaseTSpeed100)
        pAbility->Half_100 = 1;
    else
        pAbility->Half_100 = 0;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, bcm_phy_id);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x7, 0xffe4, &data)) != RT_ERR_OK)
    {
         PHY_BCM_ERR("U%u P%u mmd0x7 addr0xffe4 read fail", unit, bcm_phy_id);
         return ret;
    }

    if(data & 0x400)
        pAbility->FC = 1;
    else
        pAbility->FC = 0;
    if(data & 0x800)
        pAbility->AsyFC = 1;
    else
        pAbility->AsyFC = 0;
		
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_autoNegoAbility_set
 * Description:
 *      Set ability advertisement for auto negotiation of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 *      pAbility  - auto negotiation ability that is going to set to PHY
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_autoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    PHY_BCM_LOG("U%u P%u autoNego Ability set api", unit, port);
	
    char                     *chipname = "shortfin";    	
    uint32                    bcm_phy_id;
    hal_control_t            *pHalCtrl;
    int32                     ret = 0;
    int32                     sdsId;
    unsigned int              data = 0;
    rtk_enable_t              an_status;
    bcm_plp_base_t_ability_t  abi;
    abi.speed_full_duplex = abi.speed_half_duplex = abi.pause = abi.eee = 0;
	
    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(pAbility->Full_100)
        abi.speed_full_duplex |= bcmplpBaseTSpeed100;
    if(pAbility->Full_1000)
        abi.speed_full_duplex |= bcmplpBaseTSpeed1000;
    if(pAbility->adv_2_5G)
        abi.speed_full_duplex |= bcmplpBaseTSpeed2500;
    /*10G*/
    if(pAbility->adv_10GBase_T)
        abi.speed_full_duplex |= bcmplpBaseTSpeed10000;
    /*5G*/
    if(pAbility->adv_5G)
        abi.speed_full_duplex |= bcmplpBaseTSpeed5000;
    if((sdsId = HWP_PORT_SDSID(unit, port)) != HWP_NONE)
    {
        if((HWP_SDS_MODE(unit,sdsId) != RTK_MII_USXGMII_10GSXGMII) && 
            (HWP_SDS_MODE(unit,sdsId) != RTK_MII_USXGMII_10GQXGMII))
        {
            if(pAbility->Half_100)
                abi.speed_half_duplex |= bcmplpBaseTSpeed100;           
        }
    }

    if (bcm_plp_base_t_autoneg_ability_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &abi ) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u autoNego Ability set fail", unit, port);
         return RT_ERR_FAILED;
    }

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, bcm_phy_id);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x7, 0xffe4, &data)) != RT_ERR_OK)
    {
         PHY_BCM_ERR("U%u P%u mmd0x7 addr0xffe4 read fail", unit, bcm_phy_id);
         return ret;
    }

    if(pAbility->FC)
        data = data | 0x400;
    if(pAbility->AsyFC)
        data = data | 0x800;

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0xffe4, data)) != RT_ERR_OK)
    {
         PHY_BCM_ERR("U%u P%u mmd0x7 addr0xffe4 write fail", unit, bcm_phy_id);
         return ret;
    }
    
    if(phy_bcm_shortfin_autoNegoEnable_get(unit, port, &an_status) == RT_ERR_OK)
    {
        if(an_status == ENABLED)
        {
            if (bcm_plp_base_t_autoneg_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), ENABLED) != BCM_PM_IF_SUCCESS)
            {
                PHY_BCM_ERR("U%u P%u autoNego set fail", unit, port);
                return RT_ERR_FAILED;
            }
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_duplex_get
 * Description:
 *      Set duplex mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      duplex        - duplex mode of the port, full or half
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_duplex_get(uint32 unit, rtk_port_t port, uint32 *pDuplex)
{
    PHY_BCM_LOG("U%u P%u duplex get api", unit, port);
    	
    char                               *chipname = "shortfin";
    uint32                              bcm_phy_id;
    int32                               speed = 0;
    int32                               intf_type = 0;
    int32                               r_clk = 0;
    int32                               if_mode = 0;
	bcm_plp_base_t_device_aux_modes_t   aux_mode;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    aux_mode.ctrl_select = bcmplpBaseTCtrlDuplex;
  
    if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                 &speed, &intf_type, &r_clk, &if_mode, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u duplex get fail", unit, port);
         return RT_ERR_FAILED;
    }

    PHY_BCM_LOG("S%d T%d C%d M% duplex get", speed, intf_type, r_clk, if_mode);

    *pDuplex = aux_mode.ctrl_value;

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_duplex_set
 * Description:
 *      Set duplex mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      duplex        - duplex mode of the port, full or half
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_duplex_set(uint32 unit, rtk_port_t port, uint32 duplex)
{
    PHY_BCM_LOG("U%u P%u duplex set api", unit, port);

    char                               *chipname = "shortfin";
    uint32                              bcm_phy_id;
    int32                               speed = 0;
    int32                               intf_type = 0;
    int32                               r_clk = 0;
    int32                               if_mode = 0;
    bcm_plp_base_t_device_aux_modes_t   pre_aux_mode;
    bcm_plp_base_t_device_aux_modes_t   aux_mode;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Get*/
    pre_aux_mode.ctrl_select = bcmplpBaseTCtrlDuplex;
  
    if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                 &speed, &intf_type, &r_clk, &if_mode, (void *) &pre_aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u duplex set - get fail", unit, port);
         return RT_ERR_FAILED;
    }

    PHY_BCM_LOG("S%d T%d C%d M% duplex set - get", speed, intf_type, r_clk, if_mode);

    /*Set*/
    if(pre_aux_mode.ctrl_value != duplex)
    {
        aux_mode.ctrl_select = bcmplpBaseTCtrlDuplex;
        aux_mode.ctrl_value = duplex;
        if (bcm_plp_mode_config_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), speed, 0, 0, 0, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u duplex set - set fail", unit, port);
             return RT_ERR_FAILED;
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_speed_get
 * Description:
 *      Get link speed status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pSpeed - pointer to PHY link speed
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_speed_get(uint32 unit, rtk_port_t port, uint32 *pSpeed)
{
    PHY_BCM_LOG("U%u P%u get api", unit, port);

    char                               *chipname = "shortfin";
    uint32                              bcm_phy_id;
    uint32                              table_index;
    int32                               speed = 0;
    int32                               intf_type = 0;
    int32                               r_clk = 0;
    int32                               if_mode = 0;
    bcm_plp_base_t_device_aux_modes_t   aux_mode;
	
    if(_phy_bcm_shortfin_rtkPort_to_bcmPort_with_table_index(unit, port, &bcm_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if( senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 1)
    {
        *pSpeed = senao_non_rtl_phy_info->port_mapping[table_index].speed;
        return RT_ERR_OK;
    }

    aux_mode.ctrl_select = bcmplpBaseTCtrlMdix;

    if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                    &speed, &intf_type, &r_clk,
                                    &if_mode, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u speed get fail", unit, port);
         return RT_ERR_FAILED;
    }

    PHY_BCM_LOG("S%d T%d C%d M% speed get", speed, intf_type, r_clk, if_mode);
    switch (speed)
    {
      case 10000:
        *pSpeed = PORT_SPEED_10G;
        return RT_ERR_OK;
      case 5000:
        *pSpeed = PORT_SPEED_5G;
        return RT_ERR_OK;
      case 2500:
        *pSpeed = PORT_SPEED_2_5G;
        return RT_ERR_OK;
      case 1000:
        *pSpeed = PORT_SPEED_1000M;
        return RT_ERR_OK;
      case 100:
        *pSpeed = PORT_SPEED_100M;
        return RT_ERR_OK;
      default:
        /*PHY_BCM_ERR("U%u P%u speed get fail", unit, port);*/
        break;
    }	

    return RT_ERR_FAILED;
}


/* Function Name:
 *      phy_bcm_shortfin_enable_set
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
phy_bcm_shortfin_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{

    PHY_BCM_LOG("U%u P%u enable:%d set api", unit, port, enable);

    char        *chipname = "shortfin";
    uint32       bcm_phy_id;
    uint32       runtime_init = INIT_NOT_COMPLETED;
    int32        power=0;

    runtime_rtnic_init_state_get(&runtime_init);

    if (runtime_init == INIT_COMPLETED)
    {
        switch (enable)
        {
          case ENABLED:
            power = ENABLED;
            break;
          case DISABLED:
            power = DISABLED;
            break;
          default:
            PHY_BCM_ERR("U%u P%u enable set invalid input enable:%d", unit, port, enable);
            return RT_ERR_FAILED;
        }
    }

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_power_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), power, power) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u enable:%d set fail", unit, port, enable);
         return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_bcm_shortfin_eeeEnable_get
 * Description:
 *      Get enable status of EEE function in the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - pointer to enable status of EEE
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_eeeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{

    PHY_BCM_LOG("U%u P%u eee get api", unit, port);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;
    bcm_plp_base_t_eee_t     eee_cfg;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    eee_cfg.mode = bcmplpBaseTEeeModeGetStats;
    if (bcm_plp_base_t_eee_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &eee_cfg) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u eee get fail", unit, port);
         return RT_ERR_FAILED;
    }

    switch (eee_cfg.mode)
    {
      case bcmplpBaseTEeeModeAuto:
        *pEnable = ENABLED;
        break;
      case bcmplpBaseTEeeModeNone:
        *pEnable = DISABLED;
        break;
      default:
        PHY_BCM_ERR("U%u P%u eee get fail", unit, port);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_bcm_shortfin_eeeEnable_set
 * Description:
 *      Set enable status of EEE function in the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - enable status of EEE
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_eeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{

    PHY_BCM_LOG("U%u P%u eee set api", unit, port);

    char                     *chipname = "shortfin";
    uint32                    bcm_phy_id;
    uint32                    runtime_init = INIT_NOT_COMPLETED;
    int32                     aneg=0;
    int32                     andn=0;
    bcm_plp_base_t_eee_t      eee_cfg;
    bcm_plp_base_t_ability_t  abi;
    abi.speed_full_duplex = abi.speed_half_duplex = abi.pause = abi.eee = 0;	

    runtime_rtnic_init_state_get(&runtime_init);

    switch (enable)
    {
      case ENABLED:
        eee_cfg.mode = bcmplpBaseTEeeModeNative;
        break;
      case DISABLED:
        eee_cfg.mode = bcmplpBaseTEeeModeNone;
        break;
      default:
        PHY_BCM_ERR("U%u P%u invalid input enable:%d", unit, port, enable);
        return RT_ERR_INPUT;
    }

    eee_cfg.latency = bcmplpBaseTEeeLatencyNoChange;
    eee_cfg.latency_value= 0x047e;    /* default latency value  */
    eee_cfg.idle_threshold= 0x3d09;   /* default idle threshold */

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_base_t_autoneg_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &aneg, &andn) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u autoNego get fail", unit, port);
         return 0;
    }

    if(aneg == ENABLED)
    {
        if (bcm_plp_power_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), DISABLED, DISABLED) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u disable set fail", unit, port);
             return RT_ERR_FAILED;
        }
    }

    if (bcm_plp_base_t_eee_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &eee_cfg) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u enable:%d eee set fail", unit, port, enable);
         return RT_ERR_FAILED;
    }

    if (bcm_plp_base_t_autoneg_ability_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &abi ) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u enable:%d eee set: autoNego Ability get fail", unit, port, enable);
         return RT_ERR_FAILED;
    }	

    switch (enable)
    {
      case ENABLED:
        abi.eee |= (bcmplpBaseTEee100mBaseTX | bcmplpBaseTEee1gBaseT | bcmplpBaseTEee10gBaseT);
        break;
      case DISABLED:
        abi.eee &= ((~bcmplpBaseTEee100mBaseTX) & (~bcmplpBaseTEee1gBaseT) & (~bcmplpBaseTEee10gBaseT));
        break;
      default:
        PHY_BCM_ERR("U%u P%u invalid input enable:%d", unit, port, enable);
        return RT_ERR_INPUT;
    }

    if (bcm_plp_base_t_autoneg_ability_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &abi ) != BCM_PM_IF_SUCCESS)
    {
		 PHY_BCM_ERR("U%u P%u enable:%d eee set: autoNego Ability set fail", unit, port, enable);
         return RT_ERR_FAILED;
    }	

    if(aneg == ENABLED && runtime_init == INIT_COMPLETED)
    {
        if (bcm_plp_power_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), ENABLED, ENABLED) != BCM_PM_IF_SUCCESS)
        { 
             PHY_BCM_ERR("U%u P%u enable set fail", unit, port);
             return RT_ERR_FAILED;
        }
    }
    return RT_ERR_OK;	
	
}



/* Function Name:
 *      phy_bcm_shortfin_crossOverMode_get
 * Description:
 *      Get cross over mode in the specified port.
 * Input:
 *      unit  - unit id
 *      port  - port id
 * Output:
 *      pMode - pointer to cross over mode
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      Following value is valid
 *      - PORT_CROSSOVER_MODE_AUTO
 *      - PORT_CROSSOVER_MODE_MDI
 *      - PORT_CROSSOVER_MODE_MDIX
 */
int32
phy_bcm_shortfin_crossOverMode_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t *pMode)
{
    PHY_BCM_LOG("U%u P%u crossOver Mode get api", unit, port);

    char                          *chipname = "shortfin";
    uint32                         bcm_phy_id;
    int32                          speed = 0;
    int32                          intf_type = 0;
    int32                          r_clk = 0;
    int32                          if_mode = 0;
    bcm_plp_base_t_device_aux_modes_t  aux_mode;

    aux_mode.ctrl_select = bcmplpBaseTCtrlMdix;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                    &speed, &intf_type, &r_clk,
                                    &if_mode, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u crossOver Mode get fail", unit, port);
         PHY_BCM_ERR("S%d T%d C%d M% crossOver Mode get fail", speed, intf_type, r_clk, if_mode);
         return RT_ERR_FAILED;
    }

    PHY_BCM_LOG("S%d T%d C%d M% crossOver Mode get", speed, intf_type, r_clk, if_mode);

    if(aux_mode.ctrl_value == bcmplpBaseTMdixAuto || aux_mode.ctrl_value == bcmplpBaseTMdixForceAuto)
        *pMode = PORT_CROSSOVER_MODE_AUTO;
    else if(aux_mode.ctrl_value == bcmplpBaseTMdixStraight)
        *pMode = PORT_CROSSOVER_MODE_MDI;
    else if(aux_mode.ctrl_value == bcmplpBaseTMdixCrossover)
        *pMode = PORT_CROSSOVER_MODE_MDIX;
    else
    {
        PHY_BCM_ERR("U%u P%u crossOver Mode get fail", unit, port);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;

}


/* Function Name:
 *      phy_bcm_shortfin_crossOverMode_set
 * Description:
 *      Set cross over mode in the specified port.
 * Input:
 *      unit - unit id
 *      port - port id
 *      mode - cross over mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 *      RT_ERR_PORT_ID - invalid port id
 *      RT_ERR_INPUT   - invalid input parameter
 * Note:
 *      Following value is valid
 *      - PORT_CROSSOVER_MODE_AUTO
 *      - PORT_CROSSOVER_MODE_MDI
 *      - PORT_CROSSOVER_MODE_MDIX
 */
int32
phy_bcm_shortfin_crossOverMode_set(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t mode)
{
    PHY_BCM_LOG("U%u P%u Mode:%d crossOver Mode set api", unit, port, mode);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;
    int32                    speed = 0;

    bcm_plp_base_t_device_aux_modes_t  aux_mode;
    aux_mode.ctrl_select = bcmplpBaseTCtrlMdix;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(mode == PORT_CROSSOVER_MODE_AUTO)
        aux_mode.ctrl_value = bcmplpBaseTMdixAuto;
    else if(mode == PORT_CROSSOVER_MODE_MDI)
        aux_mode.ctrl_value = bcmplpBaseTMdixStraight;
    else if(mode == PORT_CROSSOVER_MODE_MDIX)
        aux_mode.ctrl_value = bcmplpBaseTMdixCrossover;
    else
    {
        PHY_BCM_ERR("U%u P%u Mode:%d crossOver Mode set fail", unit, port, mode);
        return RT_ERR_FAILED;
    }

    if (bcm_plp_mode_config_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                    speed, 0, 0, 0, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u Mode:%d crossOver Mode set fail", unit, port, mode);
         return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_crossOverStatus_get
 * Description:
 *      Get cross over status in the specified port.
 * Input:
 *      unit  - unit id
 *      port  - port id
 * Output:
 *      pMode - pointer to cross over mode status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 *      RT_ERR_PHY_FIBER_LINKUP - This feature is not supported in this mode
 * Note:
 *      Following value is valid
 *      - PORT_CROSSOVER_STATUS_MDI
 *      - PORT_CROSSOVER_STATUS_MDIX
 */
int32
phy_bcm_shortfin_crossOverStatus_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_status_t *pStatus)
{

    PHY_BCM_LOG("U%u P%u crossOver Status get api", unit, port);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;
    int32                    speed = 0;
    int32                    intf_type = 0;
    int32                    r_clk = 0;
    int32                    if_mode = 0;

    bcm_plp_base_t_device_aux_modes_t  aux_mode;
    aux_mode.ctrl_select = bcmplpBaseTCtrlMdixStatus;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                &speed, &intf_type, &r_clk,
                                &if_mode, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u crossOver Status get fail", unit, port);
         return RT_ERR_FAILED;
    }

    PHY_BCM_LOG("S%d T%d C%d M% crossOver Status get", speed, intf_type, r_clk, if_mode);

    if(aux_mode.ctrl_value == bcmplpBaseTMdixStraight)
        *pStatus = PORT_CROSSOVER_STATUS_MDI;
    else if(aux_mode.ctrl_value == bcmplpBaseTMdixCrossover)
        *pStatus = PORT_CROSSOVER_STATUS_MDIX;
    else
    {
        PHY_BCM_ERR("U%u P%u crossOver Status get fail", unit, port);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_bcm_shortfin_reg_mmd_get
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mmdAddr - mmd device address
 *      mmdReg  - mmd reg id
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_reg_mmd_get(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 *pData)
{

    PHY_BCM_LOG("U%u P%u mmd 0x%X reg 0x%X get api", unit, port, mmdAddr, mmdReg);

    char       *chipname = "shortfin";
    uint32      bcm_phy_id;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_reg_value_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), mmdAddr, mmdReg, pData) != BCM_PM_IF_SUCCESS)
    {
        PHY_BCM_ERR("U%u P%u mmd 0x%X reg 0x%X get fail", unit, port, mmdAddr, mmdReg);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_reg_mmd_set
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mmdAddr - mmd device address
 *      mmdReg  - mmd reg id
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_reg_mmd_set(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 data)
{

    PHY_BCM_LOG("U%u P%u mmd 0x%X reg 0x%X set api", unit, port, mmdAddr, mmdReg);

    char       *chipname = "shortfin";
    uint32      bcm_phy_id;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_reg_value_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), mmdAddr, mmdReg, data) != BCM_PM_IF_SUCCESS)
    {
        PHY_BCM_ERR("U%u P%u mmd 0x%X reg 0x%X set fail", unit, port, mmdAddr, mmdReg);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_bcm_shortfin_masterSlave_get
 * Description:
 *      Get PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      pMasterSlaveCfg     - pointer to the PHY master slave configuration
 *      pMasterSlaveActual  - pointer to the PHY master slave actual link status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      This function only works on giga/ 10g port to get its master/slave mode configuration.
 */
int32
phy_bcm_shortfin_masterSlave_get(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   *pMasterSlaveCfg,
    rtk_port_masterSlave_t   *pMasterSlaveActual)
{
/*No pMasterSlaveActual*/
    PHY_BCM_LOG("U%u P%u masterSlave get api", unit, port);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;
    int32                    speed = 0;
    int32                    intf_type = 0;
    int32                    r_clk = 0;
    int32                    if_mode = 0;

    bcm_plp_base_t_device_aux_modes_t  aux_mode;
    aux_mode.ctrl_select = bcmplpBaseTCtrlMasterSlave;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                &speed, &intf_type, &r_clk,
                                &if_mode, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u masterSlave get fail", unit, port);
         return RT_ERR_FAILED;
    }

    PHY_BCM_LOG("S%d T%d C%d M% masterSlave get", speed, intf_type, r_clk, if_mode);

    if(aux_mode.ctrl_value == bcmplpBaseTMsModeAuto)
        *pMasterSlaveCfg = PORT_AUTO_MODE;
    else if(aux_mode.ctrl_value == bcmplpBaseTMsModeMaster)
        *pMasterSlaveCfg = PORT_MASTER_MODE;
    else if(aux_mode.ctrl_value == bcmplpBaseTMsModeSlave)
        *pMasterSlaveCfg = PORT_SLAVE_MODE;
    else
    {
        PHY_BCM_ERR("U%u P%u masterSlave get fail", unit, port);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_masterSlave_set
 * Description:
 *      Set PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      masterSlave         - PHY master slave configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_masterSlave_set(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   masterSlave)
{
    PHY_BCM_LOG("U%u P%u masterSlave:%d masterSlave set api", unit, port, masterSlave);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;
    int32                    speed = 0;

    bcm_plp_base_t_device_aux_modes_t  aux_mode;
    aux_mode.ctrl_select = bcmplpBaseTCtrlMasterSlave;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(masterSlave == PORT_AUTO_MODE)
        aux_mode.ctrl_value = bcmplpBaseTMsModeAuto;
    else if(masterSlave == PORT_MASTER_MODE)
        aux_mode.ctrl_value = bcmplpBaseTMsModeMaster;
    else if(masterSlave == PORT_SLAVE_MODE)
        aux_mode.ctrl_value = bcmplpBaseTMsModeSlave;
    else
    {
        PHY_BCM_ERR("U%u P%u masterSlave:%d masterSlave set fail", unit, port, masterSlave);
        return RT_ERR_FAILED;
    }

    if (bcm_plp_mode_config_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                    speed, 0, 0, 0, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
        PHY_BCM_ERR("U%u P%u masterSlave:%d masterSlave set fail", unit, port, masterSlave);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_loopback_get
 * Description:
 *      Get PHY Loopback mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      pEnable           -loopback mode status;
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_loopback_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{

    PHY_BCM_LOG("U%u P%u lookback get api", unit, port);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;
    uint32                   lb_sts = 0;
    /*uint32                   lb_mode = 0;*/

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_loopback_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), /*&lb_mode*/0, &lb_sts) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u lookback get fail", unit, port);
         return RT_ERR_FAILED;
    }

    switch (lb_sts)
    {
      case ENABLED:
        *pEnable = ENABLED;
        break;
      case DISABLED:
        *pEnable = DISABLED;
        break;
      default:
        PHY_BCM_ERR("U%u P%u lookback get fail", unit, port);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;

}

/* Function Name:
 *      phy_bcm_shortfin_loopback_set
 * Description:
 *      Set PHY Loopback mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      enable              - ENABLED: Enable loopback;
 *                            DISABLED: Disable loopback. PHY back to normal operation.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_loopback_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{

    PHY_BCM_LOG("U%u P%u enable:%d lookback set api", unit, port, enable);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;
    uint32                   lb_sts = 0;
    uint32                   lb_mode = 2; /*Remote mode*/
	
    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    switch (enable)
    {
      case ENABLED:
        lb_sts = ENABLED;
        break;
      case DISABLED:
        lb_sts = DISABLED;
        break;
      default:
        PHY_BCM_ERR("U%u P%u invalid input enable:%d", unit, port, enable);
        return RT_ERR_INPUT;
    }

    if (bcm_plp_loopback_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), lb_mode, lb_sts) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u enable:%d lookback set fail", unit, port, enable);
         return RT_ERR_FAILED;
    }

    return RT_ERR_OK;

}


/* Function Name:
 *      phy_bcm_shortfin_rtctResult_get
 * Description:
 *      Get test result of RTCT.
 * Input:
 *      unit        - unit id
 *      port        - the port for retriving RTCT test result
 * Output:
 *      pRtctResult - RTCT result
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_PHY_RTCT_NOT_FINISH   - RTCT not finish. Need to wait a while.
 *      RT_ERR_TIMEOUT      - RTCT test timeout in this port.
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      The result unit is cm
 */
int32
phy_bcm_shortfin_rtctResult_get(uint32 unit, rtk_port_t port, rtk_rtctResult_t *pRtctResult)
{

    PHY_BCM_LOG("U%u P%u rtctResult get api", unit, port);
    char                                   *chipname = "shortfin";
    char                                    chip_name[16] = {0};
    uint32                                  bcm_phy_id;
    uint32                                  link_sts = 0;
    int32                                   speed;
    int32                                   intf_type = 0;
    int32                                   r_clk = 0;
    int32                                   if_mode = 0;
    unsigned short                          api_ver = 0;
    unsigned short                          en_ver = 0;
    bcm_plp_base_t_device_aux_modes_t       aux_mode;
    bcm_plp_base_t_cable_diag_t             cdiag;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(bcm_plp_driver_version_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                  chip_name, &api_ver, &en_ver) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u driver version get fail", unit, port);
         return RT_ERR_FAILED;
    }

    if(api_ver==2 && en_ver==4) /*SFIN2.4*/
    {
        cdiag.ecd_unit      = 0;    /* 0:centimeter  1:meter  */
        cdiag.cable_type    = 1;    /* 0:CAT-6a      1:CAT=5e */
        cdiag.partner_power = 0;    /* 0:default              */
    }
    else if(api_ver==2 && en_ver==5) /*SFIN2.5*/
    {
        cdiag.ecd_unit      = 0;    /* 0:centimeter  1:meter  */
        cdiag.cable_type    = 3;    /* 0 = cat6a cable and 10G link partner
                                       1 = cat5e cable and 10G link partner
                                       2 = cat6 cable and 10G link partner
                                       3 = cat6a cable and Gphy link partner
                                       4 = cat5e and Gphy link partner*/
        cdiag.partner_power = 0;    /* 0:default              */
    }
    else
    {
        cdiag.ecd_unit      = 0;    /* 0:centimeter  1:meter  */
        cdiag.cable_type    = 1;    /* 0:CAT-6a      1:CAT=5e */
        cdiag.partner_power = 0;    /* 0:default              */
    }

    if (bcm_plp_link_status_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &link_sts) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u linkStatus get fail", unit, port);
         return RT_ERR_FAILED;
    }

    aux_mode.ctrl_select = bcmplpBaseTCtrlDuplex;
    if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}),
                                &speed, &intf_type, &r_clk,
                                &if_mode, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u speed get fail", unit, port);
         return RT_ERR_FAILED;
    }		

    if (bcm_plp_base_t_cable_diag(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &cdiag) != BCM_PM_IF_SUCCESS)
    {
	     PHY_BCM_ERR("U%u P%u rtctResult get fail", unit, port);
         return RT_ERR_FAILED;
    }

    if(!link_sts)
        pRtctResult->linkType = PORT_SPEED_1000M;
    else if(speed == 100)
    /*100M and 1G are no different in BCM Cable Diagnostics*/
        pRtctResult->linkType = PORT_SPEED_1000M;
    else if(speed == 1000)
        pRtctResult->linkType = PORT_SPEED_1000M;
    else if(speed == 2500)
        pRtctResult->linkType = PORT_SPEED_2_5G;
    else if(speed == 5000)
        pRtctResult->linkType = PORT_SPEED_5G;
    else if(speed == 10000)
        pRtctResult->linkType = PORT_SPEED_10G;		

    /*pair_state 1 - open
      pair_state 2 - short*/
 
    /*PairA State*/    
    if(cdiag.pair_state[0] == 1)
        pRtctResult->un.ge_result.channelAOpen = 1;
    else if(cdiag.pair_state[0] == 2)
        pRtctResult->un.ge_result.channelAShort = 1;

    /*PairB State*/
    if(cdiag.pair_state[1] == 1)
        pRtctResult->un.ge_result.channelBOpen = 1;
    else if(cdiag.pair_state[1] == 2)
        pRtctResult->un.ge_result.channelBShort = 1;

    /*PairC State*/
    if(cdiag.pair_state[2] == 1)
        pRtctResult->un.ge_result.channelCOpen = 1;
    else if(cdiag.pair_state[2] == 2)
        pRtctResult->un.ge_result.channelCShort = 1;

    /*PairD State*/
    if(cdiag.pair_state[3] == 1)
        pRtctResult->un.ge_result.channelDOpen = 1;
    else if(cdiag.pair_state[3] == 2)
        pRtctResult->un.ge_result.channelDShort = 1;

    /*PairA Length*/
    pRtctResult->un.ge_result.channelALen = (uint32) cdiag.pair_len[0];
    /*PairB Length*/
    pRtctResult->un.ge_result.channelBLen = (uint32) cdiag.pair_len[1];
    /*PairC Length*/
    pRtctResult->un.ge_result.channelCLen = (uint32) cdiag.pair_len[2];
    /*PairD Length*/
    pRtctResult->un.ge_result.channelDLen = (uint32) cdiag.pair_len[3];

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_bcm_shortfin_rtct_start
 * Description:
 *      Start PHY interface RTCT test of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_CHIP_NOT_SUPPORTED - chip not supported
 * Note:
 *      None
 */
int32
phy_bcm_shortfin_rtct_start(uint32 unit, rtk_port_t port)
{
    PHY_BCM_LOG("U%u P%u rtct start api", unit, port);

    uint32                    bcm_phy_id;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_bcm_shortfin_ieeeTestMode_set
 * Description:
 *      Set test mode for PHY transmitter test
 * Input:
 *      unit  - unit id
 *      port  - port id
 *      pTestMode->mode  - test mode RTK_PORT_PHY_1G_TEST_MODEx which is defined in IEEE 40.6.1.1.2 for 1G.
 *                         RTK_PORT_PHY_10G_TEST_MODEx is for 10G test.
 *                         RTK_PORT_PHY_100M_TEST_MODE_xx is for 100M test.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_NOT_ALLOWED - The operation is not allowed
 *      RT_ERR_PORT_NOT_SUPPORTED - test mode is not supported
 * Note:
 *      RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS is not supported
 */
int32
phy_bcm_shortfin_ieeeTestMode_set(uint32 unit, rtk_port_t port, rtk_port_phyTestMode_t *pTestMode)
{

    PHY_BCM_LOG("U%u P%u ieeeTestMode set api", unit, port);

    uint32                    bcm_phy_id;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;
/*
plp_shortfin_BCM848x_set_IEEE_TestMode(uint32_t prtad, uint8_t testmode_number, uint8_t freq_index, uint8_t speed)
plp_shortfin_BCM848x_set_super_isolate(uint32_t prtad, uint8_t si_ena)
*/

    return RT_ERR_OK;
}

int32
phy_bcm_shortfin_portEyeMonitor_start(uint32 unit, rtk_port_t port, uint32 sdsId, uint32 frameNum)
{
    PHY_BCM_LOG("U%u P%u portEyeMonitor start api", unit, port);

    char                    *chipname = "shortfin";
    uint32                   bcm_phy_id;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_display_eye_scan(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1})) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u portEyeMonitor start fail", unit, port);
         return RT_ERR_FAILED;
    }
    return RT_ERR_OK;
}

/*
int32
phy_bcm_shortfin_polar_get(uint32 unit, rtk_port_t port, rtk_port_phyPolarCtrl_t *pPolarCtrl)
{
    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}
*/

int32 shortfin_api_mdio_write (
  void *user_acc,
  unsigned int mdio_addr,
  unsigned int reg_addr,
  unsigned int data
)
{

    if (!user_acc)
        return -1;

    /*PHY_BCM_LOG("mmd 0x%X reg 0x%X mdio write api", mdio_addr, reg_addr);*/

    hal_control_t   *pHalCtrl;
    int32            ret = 0;
    uint32           unit = 0;
    rtk_port_t       port;
    unsigned int     mmd;
    unsigned int     addr;

    mmd = (reg_addr >> 16) & 0xffff;
    addr = reg_addr & 0xffff;
	
    _phy_bcm_shortfin_bcmPort_to_rtkPort(&unit, &port, mdio_addr);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, mmd, addr, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x%X addr0x%X write fail", unit, port, mmd, addr);
        return ret;
    }

    return ret;
}

int32 shortfin_api_mdio_read (
  void *user_acc,
  unsigned int mdio_addr,
  unsigned int reg_addr,
  unsigned int *data
)
{

    if (!user_acc)
        return -1;

    /*PHY_BCM_LOG("mmd 0x%X reg 0x%X mdio read api", mdio_addr, reg_addr);*/

    hal_control_t   *pHalCtrl;
    int32            ret = 0;
    uint32           unit = 0;
    rtk_port_t       port;
    unsigned int     mmd;
    unsigned int     addr;
	
    mmd = (reg_addr >> 16) & 0xffff;
    addr = reg_addr & 0xffff;

    _phy_bcm_shortfin_bcmPort_to_rtkPort(&unit, &port, mdio_addr);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, mmd, addr, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x%X addr0x%X read fail", unit, mdio_addr, mmd, addr);
        return ret;
    }

    return ret;
}

/* Function Name:
 *      phy_bcm_shortfin_init
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
phy_bcm_shortfin_init(uint32 unit, rtk_port_t port)
{
    int32           ret = 0;

    if(phy_board_type == RTL9302C_BCM50998_BOARD_TYPE_1)
    {
       if((ret = phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_init(unit, port)) != RT_ERR_OK)
       {
            return RT_ERR_FAILED;
       }
    }
    else if(phy_board_type == RTL9313_BCM84898_BOARD_TYPE_1)
    {
       if((ret = phy_rtl9313_bcm84898_8XGE_4FibXGE_init(unit, port)) != RT_ERR_OK)
       {
            return RT_ERR_FAILED;
       }
    }
    else if(phy_board_type == RTL9313_BCM50998_BOARD_TYPE_1)
    {
       if((ret = phy_rtl9313_4x8218d_2xbcm50998e_4XGE_init(unit, port)) != RT_ERR_OK)
       {
            return RT_ERR_FAILED;
       }
    }
    else if(phy_board_type == RTL9302B_BCM50998_BOARD_TYPE_1)
    {
       if((ret = phy_rtl9302b_2x8218d_bcm50998e_4XGE_init(unit, port)) != RT_ERR_OK)
       {
            return RT_ERR_FAILED;
       }
    }
    else
    {
        return RT_ERR_FAILED;
    }
	return RT_ERR_OK;
}

/* Function Name:
*      phy_bcm_shortfin_linkStatus_get
* Description:
*      Get PHY link status from standard register (1.2).
* Input:
*      unit    - unit id
*      port    - port id
* Output:
*      pStatus - pointer to the link status
* Return:
*      RT_ERR_OK
*      RT_ERR_FAILED
* Note:
*      The Link Status bit (Status Register 1.2) has LL (Latching Low) attribute
*      for link failure. Please refer IEEE 802.3 for detailed.
*/
int32
phy_bcm_shortfin_linkStatus_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pStatus)
{
    PHY_BCM_LOG("U%u P%u linkStatus get api", unit, port);

    char        *chipname = "shortfin";
    uint32       bcm_phy_id;
    uint32       link_sts = 0;

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (bcm_plp_link_status_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm_shortfin, bcm_phy_id, 0, 0x1}), &link_sts) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u linkStatus get fail", unit, port);
         return RT_ERR_FAILED;
    }

    *pStatus = (link_sts) ? PORT_LINKUP: PORT_LINKDOWN;

    return RT_ERR_OK;
}
