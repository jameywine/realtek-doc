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
#include <hal/phy/phy_cust1.h>
#include <hal/phy/phy_bcm_shortfin.h>
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
/*#define PHY_CUST_PHY_IMAGE_FILE             "/lib/modules/PHY_image"
#define PHY_CUST_PHY_PROV_TABLE_FILE        "/lib/modules/PHY_prov_table"
#define PHY_CUST_BCM_PHY_PORT_MAX            4*/

extern int p_ctxt_phy_bcm50998;

/*
 * Macro Declaration
 */
#define PHY_BCM_LOG(fmt, args...)      RT_LOG(LOG_INFO, (MOD_HAL|MOD_PHY), "%s:%d:"fmt, __FUNCTION__, __LINE__, ##args)
#define PHY_BCM_ERR(fmt, args...)      RT_LOG(LOG_WARNING, (MOD_HAL|MOD_PHY), "WARN.%s:%d:"fmt, __FUNCTION__, __LINE__, ##args)


/*
 * Function Declaration
 */
int32 phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_init(uint32 unit, rtk_port_t port);
int32 phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_eye_diagram_init(uint32 unit);

#if 0
typedef struct phyp_senao_port_force_mode_s
{
    uint32     force_auto;
    int32      speed;
} phyp_senao_port_force_mode_t;

static phyp_senao_port_force_mode_t phyp_senao_port_force_mode[] = {
        [0] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [1] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [2] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [3] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [4] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [5] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [6] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [7] = { .force_auto = 0,       .speed = PORT_SPEED_100M},
        [8] = { .force_auto = HWP_END, .speed = HWP_END,   }
};
#endif
extern phyp_senao_phy_info_t *senao_non_rtl_phy_info;

/* Function Name:
 *      phy_rtl9302c_bcm50998_8x2_5G_4fibxgedrv_identify
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
phy_rtl9302c_bcm50998_8x2_5G_4fibxgedrv_identify(uint32 unit, rtk_port_t port, uint32 model_id, uint32 rev_id)
{
    int32                   ret = 0;
    uint32                  reg1d2 = 0;
    uint32                  reg1d3 = 0;
    hal_control_t          *pHalCtrl = NULL;
    uint32                  bcm_phy_id;
    drv_smi_mdxProtoSel_t   mdxProto;

    if(phy_board_type != RTL9302C_BCM50998_BOARD_TYPE_1)
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
    if ((reg1d2 != 0x3590) || ((reg1d3 != 0x5014) && (reg1d3 != 0x5015)))
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
 *      phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_speed_set
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
phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_speed_set(uint32 unit, rtk_port_t port, uint32 speed)
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
    unsigned int                        or_shadow = 0;
    unsigned int                        shadow = 0;        
    unsigned int                        or_data = 0;
    unsigned int                        data = 0;    
    hal_control_t                       *pHalCtrl;
    bcm_plp_base_t_device_aux_modes_t       pre_aux_mode;
    bcm_plp_base_t_device_aux_modes_t       aux_mode;	
	
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
        if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}),
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
        if (bcm_plp_mode_config_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), bcm_speed, 0, 0, 0, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed:%u set - set fail", unit, port, speed);
             return RT_ERR_FAILED;
        }

        if (bcm_plp_base_t_autoneg_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), DISABLED) != BCM_PM_IF_SUCCESS)
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
     
        if (bcm_plp_base_t_autoneg_ability_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), &abi ) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed:%u set - AutoNego ability set fail", unit, port, speed);
             return RT_ERR_FAILED;
        }
        if (bcm_plp_base_t_autoneg_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), ENABLED) != BCM_PM_IF_SUCCESS)
        {
             PHY_BCM_ERR("U%u P%u speed:%u set - autoNego set fail", unit, port, speed);
             return RT_ERR_FAILED;
        }
    }
    senao_non_rtl_phy_info->port_mapping[bcm_phy_id].speed = speed;
    return RT_ERR_OK;
}

#define BCM_PHY_UPGRADE 1
#if BCM_PHY_UPGRADE
extern char  *plp_shortfin_xgbaset_firmware_ver;
extern unsigned int plp_shortfin_xgbaset_firmware_ver_id;
#endif
/* Function Name:
 *      phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_init
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
phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_init(uint32 unit, rtk_port_t port)
{
    PHY_BCM_LOG("U%u P%u init api", unit, port);

    char                    *chipname = "shortfin";
    uint32                  bcm_phy_id;
    int32                   options = 0x0; /*load none*/
    /*int32                   bcm_phy_id_counter = 0;*/
    /*bcm_plp_base_t_device_aux_modes_t  aux_mode;*/
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

    if (bcm_plp_init(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), 
	                   shortfin_api_mdio_read, shortfin_api_mdio_write, options) != BCM_PM_IF_SUCCESS)
    {
        PHY_BCM_ERR("U%u P%u init fail", unit, port);
        return RT_ERR_FAILED;	
    }

    /*Disable copper port mac polling phy*/
    /*hal_miim_pollingEnable_set(unit, port, DISABLED);*/
	
#if 0	
    if(bcm_phy_id == 7)
    {
        for(bcm_phy_id_counter=0; bcm_phy_id_counter <=7; bcm_phy_id_counter++)
        {
            /*aux_mode.ctrl_select = bcmplpBaseTCtrlSerdesAutoNeg;*/
            /*aux_mode.ctrl_value = TRUE;*/
            aux_mode.ctrl_select = bcmplpBaseTCtrlNone;
            if (bcm_plp_mode_config_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id_counter, 1, 0x1}),
                                    2500, bcm_pm_InterfaceUSXGMII, bcm_pm_RefClk156Mhz, bcmpmUsxgmiiModeQuad, (void *) &aux_mode) != BCM_PM_IF_SUCCESS)
            {
                PHY_BCM_ERR("U%u P%u init fail", unit, port);
                return RT_ERR_FAILED;
            }
        }
    }
#endif
    if(bcm_phy_id < 7)
        return RT_ERR_OK;

    hal_control_t   *pHalCtrl;
    int32           ret = 0;
    unsigned int    mdio_addr = 0;
    unsigned int    or_data = 0;	
    unsigned int    data = 0;

    /***************************************************************/
    /*1. GET 0x401c register first to check bit 8
      2. SET 1e.0x401c bit8=1 to all 8 ports
      3. AND wait until 1e.0x400e=0
      4. executed CMD:
         set the following to all 8 ports
         set 1e.0x4038=1         <-- related USXGMII
         set 1e.0x4039=0         <-- related AN
         set 1e.0x403a=0x100     <-- related speed mode
         set 1e.0x403b=0x2       <-- 20K Alignment Marker
         set 1e.0x403c=0
         set 1e.0x4005=0x8026
         wait until 1e.0x4037=4  <--last command has been executed
      5. SET 1e.0x401c bit8=0 to all 8 ports
      6. AND wait until 1e.0x400e=0*/
    /***************************************************************/
	
	
    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }
	
    for(mdio_addr = 0; mdio_addr <= 7; mdio_addr++)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }

        data = or_data | 0x100;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x401c, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
            return ret;
        }

        data = 1;
        while (data != 0)
        {
	    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x400e, &data)) != RT_ERR_OK)
            {
                PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
                return ret;
            }
        }

		/*DATA1*/
        data = 0x1;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4038, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
            return ret;
        }	

		/*DATA2*/
        data = 0x0;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4039, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
            return ret;
        }

		/*DATA3*/
        data = 0x100;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x403a, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
            return ret;
        }	

		/*DATA4*/
        data = 0x2;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x403b, data)) != RT_ERR_OK)
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
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x403c, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
            return ret;
        }	

		/*CMD*/
        data = 0x8026;
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4005, data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
            return ret;
        }
	
        data = 0;
        while (data != 4)
        {
	    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x4037, &data)) != RT_ERR_OK)
            {
                PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
                return ret;
            }
        }

        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
            return ret;
        }

        data = 1;
        while (data != 0)
        {
	    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x400e, &data)) != RT_ERR_OK)
            {
                PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
                return ret;
            }
        }

        /*Init for force speed 100M*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1, 0x0, 0x2000)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1 addr0x0 write fail", unit, mdio_addr);
            return ret;
        }

    }

    if(phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_eye_diagram_init(unit)!= RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }	
	
	/*20K Alignment Marker*/
    // if((hal_serdes_reg_set(unit, 2, 6, 18, 0xc8)) != RT_ERR_OK)
    //     return RT_ERR_FAILED;
    // if((hal_serdes_reg_set(unit, 3, 6, 18, 0xc8)) != RT_ERR_OK)
    //     return RT_ERR_FAILED;

    /*128.6K Alignment Marker, for B0-v02-02-03*/
    if((hal_serdes_reg_set(unit, 2, 6, 18, 0x506)) != RT_ERR_OK)
        return RT_ERR_FAILED;
    if((hal_serdes_reg_set(unit, 3, 6, 18, 0x506)) != RT_ERR_OK)
        return RT_ERR_FAILED;
			
    /*Disable serdes 2 AN*/
    if((hal_serdes_reg_set(unit, 2, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*Disable serdes 3 AN*/
    if((hal_serdes_reg_set(unit, 3, 7, 17, 0x0540)) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*---------------------------------------------*/
    /* Register default value for MAC's flow-control */
    /* MANGO_FC_PORT_ACT_CTRLr [0xBB00D804] */
    /* Bits[12] : ACT ; Bits[11:0] : ALLOW_PAGE_CNT */
    /* For copper ports */
    ioal_mem32_write(unit, 0xD804, 0x0000107f);
    ioal_mem32_write(unit, 0xD808, 0x0000107f);
    ioal_mem32_write(unit, 0xD80C, 0x0000107f);
    ioal_mem32_write(unit, 0xD810, 0x0000107f);
    ioal_mem32_write(unit, 0xD824, 0x0000107f);
    ioal_mem32_write(unit, 0xD828, 0x0000107f);
    ioal_mem32_write(unit, 0xD82C, 0x0000107f);
    ioal_mem32_write(unit, 0xD830, 0x0000107f);

    /* For Fiber ports */
    ioal_mem32_write(unit, 0xD864, 0x0000107f);
    ioal_mem32_write(unit, 0xD868, 0x0000107f);
    ioal_mem32_write(unit, 0xD86C, 0x0000107f);
    ioal_mem32_write(unit, 0xD870, 0x0000107f);
    /*---------------------------------------------*/

    return RT_ERR_OK;
}

int32
phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_eye_diagram_init(uint32 unit)
{
    hal_control_t   *pHalCtrl;
    int32           ret = 0;
    unsigned int    mdio_addr = 0;
    unsigned int    or_data = 0;
    unsigned int    data = 0;

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

    /*Serdes 2*/
    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x5000;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
	
    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x802c;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);

    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /************************************************************************************************************************/
    /*Serdes 3*/
    mdio_addr = 5;
    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, mdio_addr);
        return ret;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x401c, &or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
        return ret;
    }

    data = or_data | 0x100;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x401c, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x400e, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x400e read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    /*DATA1*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4038, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA2*/
    data = 0x4500;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4039, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*DATA3*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x403a, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
	
    /*DATA4*/
    data = 0x0;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x403b, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    /*CMD*/
    data = 0x802c;
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x4005, data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }
    /*phy_osal_time_mdelay(100);*/
    osal_time_mdelay(100);

    data = 0;
    while (data != 4)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x4037, &data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c read fail", unit, mdio_addr);
            return ret;
        }
        /*phy_osal_time_mdelay(100);*/
        osal_time_mdelay(100);
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, mdio_addr, 0x1e, 0x401c, or_data)) != RT_ERR_OK)
    {
        PHY_BCM_ERR("U%u P%u mmd0x1e addr0x401c write fail", unit, mdio_addr);
        return ret;
    }

    data = 1;
    while (data != 0)
    {
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, mdio_addr, 0x1e, 0x400e, &data)) != RT_ERR_OK)
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
 *      phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_reset_set
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
phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_reset_set(uint32 unit, rtk_port_t port)
{

    PHY_BCM_LOG("U%u P%u reset set api", unit, port);

    /*int32       ret;*/
    char        *chipname = "shortfin";
    uint32      bcm_phy_id;	
    uint32      reset = 1; /*0:Hard, 1:Soft*/

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    /*ret = bcm_plp_reset_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), reset, 0);*/
    if (bcm_plp_reset_set(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), reset, 0) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u reset set fail", unit, port);
         return RT_ERR_FAILED;
    }
	
    return RT_ERR_OK;
}

int32
phy_bcm50998_swMacPollPhyStatus_get(uint32 unit, rtk_port_t port, rtk_port_swMacPollPhyStatus_t *pphyStatus)
{
    char            *chipname = "shortfin";
    uint32          bcm_phy_id;
    uint32          reslStatus = 0, speed5_4, speed10_9, tx_pause, rx_pause, duplex;
    uint32          isConnected;
    uint32          link_sts = 0;
    int32           speed;
    int32           ret = 0;
    int32           intf_type = 0;
    int32           r_clk = 0;
    int32           if_mode = 0;
    unsigned int    lo_data = 0;
    unsigned int    lp_data = 0;
    hal_control_t   *pHalCtrl;

    bcm_plp_base_t_device_aux_modes_t       aux_mode;
    /*rtk_port_masterSlave_t                  masterSlaveCfg;*/
    /*rtk_port_masterSlave_t                  masterSlaveActual;*/

    if(_phy_bcm_shortfin_rtkPort_to_bcmPort(unit, port, &bcm_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;
    
    if (bcm_plp_link_status_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}), &link_sts) != BCM_PM_IF_SUCCESS)
    {
         PHY_BCM_ERR("U%u P%u linkStatus get fail", unit, port);
         return RT_ERR_FAILED;
    }

    if(!link_sts)
	{
        isConnected = FALSE;
		duplex = 1;
        speed5_4 = 0x3;
        speed10_9 = 0x3;
	}
    else
    {
		aux_mode.ctrl_select = bcmplpBaseTCtrlDuplex;
        if (bcm_plp_mode_config_get(chipname, ((bcm_plp_access_t){&p_ctxt_phy_bcm50998, bcm_phy_id, 0, 0x1}),
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
             isConnected = TRUE;
             speed5_4 = 0x0;
             speed10_9 = 0x1;
             break;
          case 5000:
             isConnected = TRUE;
             speed5_4 = 0x2;
             speed10_9 = 0x1;
             break;
          case 2500:
             isConnected = TRUE;
             speed5_4 = 0x1;
             speed10_9 = 0x1;
             break;
          case 1000:
             isConnected = TRUE;
             speed5_4 = 0x2;
             speed10_9 = 0x0;
             break;
          case 100:
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
        duplex = aux_mode.ctrl_value;
    }

    /*phy_bcm50998_duplex_get(unit, port, &duplex);*/

    /*phy_bcm50998_masterSlave_get(unit, port, &masterSlaveCfg, &masterSlaveActual);*/

#if 0
    //if (connStatus.rxLinkUp == True)
    if (isConnected == TRUE)
    {
        _phy_cust1_swMacPollPhyStatusTxRxPause_get(unit, port, &aq_port, &tx_pause, &rx_pause);
        duplex = (connStatus.duplex == True) ? 1 : 0;
    }
    else
    {
        duplex = 1;
        tx_pause = 0;
        rx_pause = 0;
    }
#endif

    tx_pause = 0;
    rx_pause = 0;

    if (isConnected == TRUE)
    {
        if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        {
            PHY_BCM_ERR("U%u P%u pHalCtrl get fail", unit, bcm_phy_id);
            return ret;
        }

        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, bcm_phy_id, 0x7, 0xffe4, &lo_data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x7 addr0xffe4 read fail", unit, bcm_phy_id);
            return ret;
        }

        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, bcm_phy_id, 0x7, 0xffe5, &lp_data)) != RT_ERR_OK)
        {
            PHY_BCM_ERR("U%u P%u mmd0x7 addr0xffe4 read fail", unit, bcm_phy_id);
            return ret;
        }

        if ((~(lo_data & 0x400)) &&
            (lo_data & 0x800) &&
            (lp_data & 0x400) &&
            (lp_data & 0x800))
        {
            tx_pause = 1;
        }
        else if ((lo_data & 0x400) &&
                 ~(lo_data & 0x800) &&
                 (lp_data & 0x400))
        {
            tx_pause = 1;
            rx_pause = 1;
        }
        else if ((lo_data & 0x400) &&
                 (lo_data & 0x800) &&
                 ~(lp_data & 0x400) &&
                 (lp_data & 0x800))
        {
            rx_pause = 1;
        }
        else if ((lo_data & 0x400) &&
                 (lo_data & 0x800) &&
                 (lp_data & 0x400))
        {
            tx_pause = 1;
            rx_pause = 1;
        }
    }

	
    reslStatus = REG32_FIELD_SET(reslStatus, 0, PHY_RESL_REG_MDI_OFFSET, PHY_RESL_REG_MDI_MASK);
    /*reslStatus = REG32_FIELD_SET(reslStatus, ((connStatus.rxLinkUp == True) ? 1 : 0), PHY_RESL_REG_LINK_OFFSET, PHY_RESL_REG_LINK_MASK);*/
    reslStatus = REG32_FIELD_SET(reslStatus, ((isConnected == TRUE) ? 1 : 0), PHY_RESL_REG_LINK_OFFSET, PHY_RESL_REG_LINK_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, duplex, PHY_RESL_REG_DUPLEX_OFFSET, PHY_RESL_REG_DUPLEX_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, speed5_4, PHY_RESL_REG_SPEED0_OFFSET, PHY_RESL_REG_SPEED0_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, ((tx_pause) ? 1 : 0), PHY_RESL_REG_TX_PAUSE_OFFSET, PHY_RESL_REG_TX_PAUSE_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, ((rx_pause) ? 1 : 0), PHY_RESL_REG_RX_PAUSE_OFFSET, PHY_RESL_REG_RX_PAUSE_MASK);
    reslStatus = REG32_FIELD_SET(reslStatus, speed10_9, PHY_RESL_REG_SPEED1_OFFSET, PHY_RESL_REG_SPEED1_MASK);
    /*reslStatus = REG32_FIELD_SET(reslStatus, ((masterSlaveActual == PORT_MASTER_MODE) ? 1 : 0), PHY_RESL_REG_MASTER_SLAVE_OFFSET, PHY_RESL_REG_MASTER_SLAVE_MASK);*/

    pphyStatus->reslStatus = reslStatus;
    pphyStatus->media = PORT_MEDIA_COPPER;
    /*For SNOS performance*/
    /*phy_osal_time_usleep(50 * 1000); *//* sleep 50mS */
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_cust1drv_mapperInit
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
phy_rtl9302c_bcm50998_8x2_5G_4fibxgedrv_mapperInit(rt_phydrv_t *pPhydrv)
{
    pPhydrv->phydrv_index = RT_PHYDRV_CUST1;
    pPhydrv->fPhydrv_init = phy_bcm_shortfin_init;
    pPhydrv->fPhydrv_media_get = phy_bcm_shortfin_media_get;
    pPhydrv->fPhydrv_autoNegoEnable_get = phy_bcm_shortfin_autoNegoEnable_get;
    pPhydrv->fPhydrv_autoNegoEnable_set = phy_bcm_shortfin_autoNegoEnable_set;
    pPhydrv->fPhydrv_autoNegoAbility_get = phy_bcm_shortfin_autoNegoAbility_get;
    pPhydrv->fPhydrv_autoNegoAbility_set = phy_bcm_shortfin_autoNegoAbility_set;
    pPhydrv->fPhydrv_duplex_get = phy_bcm_shortfin_duplex_get;
    pPhydrv->fPhydrv_duplex_set = phy_bcm_shortfin_duplex_set;
    pPhydrv->fPhydrv_speed_get = phy_bcm_shortfin_speed_get;
    pPhydrv->fPhydrv_speed_set = phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_speed_set;
    pPhydrv->fPhydrv_enable_set = phy_bcm_shortfin_enable_set;

    pPhydrv->fPhydrv_reg_mmd_get = phy_bcm_shortfin_reg_mmd_get;
    pPhydrv->fPhydrv_reg_mmd_set = phy_bcm_shortfin_reg_mmd_set;
    /*pPhydrv->fPhydrv_portimageFlash_load = phy_cust1_imageFlash_load;*/
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
    /*pPhydrv->fPhydrv_macIntfSerdesMode_get = phy_cust1_macIntfSerdesMode_get;*/
    pPhydrv->fPhydrv_linkStatus_get = phy_bcm_shortfin_linkStatus_get;
    pPhydrv->fPhydrv_reset_set = phy_rtl9302c_bcm50998_8x2_5G_4FibXGE_reset_set;
}
