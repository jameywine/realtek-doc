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
 * $Revision: 89577 $
 * $Date: 2018-06-21 16:15:53 +0800 (Thu, 21 Jun 2018) $
 *
 * Purpose : PHY Common Driver APIs.
 *
 * Feature : PHY Common Driver APIs
 *
 */

#ifndef __HAL_PHY_PHY_GPY2xx_H__
#define __HAL_PHY_PHY_GPY2xx_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <rtk/port.h>


/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
extern rt_phyInfo_t phy_gpy2xx_info;



/*
 * Macro Declaration
 */


/* Function Name:
 *      _phy_gpy2xx_struct_gpy211_device_per_port
 * Description:
 *      Init gpy211_device with different port number.
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
_phy_gpy2xx_struct_gpy211_device_per_port(uint32 unit, rtk_port_t port);

/* Function Name:
 *      _phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index
 * Description:
 *      Transfert rtk unit/port to gpy2xx phy port and get array index
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pgpy2xx_phy_id - gpy2xx port id
 *      table_index - array index
 * Return:
 *      None
 * Note:
 *      None
 */
extern int32
_phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index(uint32 unit, rtk_port_t port, uint32 *pgpy2xx_phy_id, uint32 *table_index);

/* Function Name:
 *      _phy_gpy2xx_rtkPort_to_gpy2xxPort
 * Description:
 *      Transfert rtk unit/port to gpy2xx phy port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pgpy2xx_phy_id - gpy2xx port id
 * Return:
 *      None
 * Note:
 *      None
 */
extern int32
_phy_gpy2xx_rtkPort_to_gpy2xxPort(uint32 unit, rtk_port_t port, uint32 *pgpy2xx_phy_id);

/* Function Name:
 *      _phy_gpy2xx_gpy2xxPort_to_rtkPort
 * Description:
 *      Transfert gpy2xx phy port to rtk unit/port
 * Input:
 *      pgpy2xx_phy_id - gpy2xx port id
 * Output:
 *      unit - unit id
 *      port - port id
 * Return:
 *      None
 * Note:
 *      None
 */
extern int32
_phy_gpy2xx_gpy2xxPort_to_rtkPort(uint32 *unit, rtk_port_t *port, uint32 pgpy2xx_phy_id);

/* Function Name:
 *      phy_gpy2xx_identify
 * Description:
 *      Identify the port is GPY2xx PHY or not?
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
extern int32
phy_gpy2xx_identify(uint32 unit, rtk_port_t port, uint32 model_id, uint32 rev_id);

/* Function Name:
 *      phy_gpy2xx_init
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
extern int32
phy_gpy2xx_init(uint32 unit, rtk_port_t port);

extern int32
phy_gpy2xx_eye_diagram_init(uint32 unit, rtk_port_t port);

/* Function Name:
 *      phy_gpy2xx_reset_set
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
extern int32
phy_gpy2xx_reset_set(uint32 unit, rtk_port_t port);

/* Function Name:
 *      phy_gpy2xx_swMacPollPhyStatus_get
 * Description:
 *      Get PHY status
 * Input:
 *      unit    - unit ID
 *      port    - port ID
 * Output:
 *      pphyStatus  - PHY status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
phy_gpy2xx_swMacPollPhyStatus_get(uint32 unit, uint32 port, rtk_port_swMacPollPhyStatus_t *pphyStatus);

/* Function Name:
 *      phy_gpy2xx_media_get
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
extern int32
phy_gpy2xx_media_get(uint32 unit, rtk_port_t port, rtk_port_media_t *pMedia);

/* Function Name:
 *      phy_gpy2xx_autoNegoEnable_get
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
extern int32
phy_gpy2xx_autoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable);

/* Function Name:
 *      phy_gpy2xx_autoNegoEnable_set
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
extern int32
phy_gpy2xx_autoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      phy_gpy2xx_autoNegoAbility_get
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
extern int32
phy_gpy2xx_autoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility);

/* Function Name:
 *      phy_gpy2xx_autoNegoAbility_set
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
extern int32
phy_gpy2xx_autoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility);

/* Function Name:
 *      phy_gpy2xx_duplex_get
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
extern int32
phy_gpy2xx_duplex_get(uint32 unit, rtk_port_t port, uint32 *pDuplex);

/* Function Name:
 *      phy_gpy2xx_duplex_set
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
extern int32
phy_gpy2xx_duplex_set(uint32 unit, rtk_port_t port, uint32 duplex);

/* Function Name:
 *      phy_gpy2xx_speed_get
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
extern int32
phy_gpy2xx_speed_get(uint32 unit, rtk_port_t port, uint32 *pSpeed);

/* Function Name:
 *      phy_gpy2xx_speed_set
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
extern int32
phy_gpy2xx_speed_set(uint32 unit, rtk_port_t port, uint32 speed);

/* Function Name:
 *      phy_gpy2xx_enable_set
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
extern int32
phy_gpy2xx_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      phy_gpy2xx_eeeEnable_get
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
extern int32
phy_gpy2xx_eeeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable);

/* Function Name:
 *      phy_gpy2xx_eeeEnable_set
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
extern int32
phy_gpy2xx_eeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      phy_gpy2xx_crossOverMode_get
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
extern int32
phy_gpy2xx_crossOverMode_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t *pMode);

/* Function Name:
 *      phy_gpy2xx_crossOverMode_set
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
extern int32
phy_gpy2xx_crossOverMode_set(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t mode);

/* Function Name:
 *      phy_gpy2xx_crossOverStatus_get
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
extern int32
phy_gpy2xx_crossOverStatus_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_status_t *pStatus);

/* Function Name:
 *      phy_gpy2xx_reg_mmd_get
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
extern int32
phy_gpy2xx_reg_mmd_get(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 *pData);

/* Function Name:
 *      phy_gpy2xx_reg_mmd_set
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
extern int32
phy_gpy2xx_reg_mmd_set(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 data);


/* Function Name:
 *      phy_gpy2xx_masterSlave_get
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
extern int32
phy_gpy2xx_masterSlave_get(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   *pMasterSlaveCfg,
    rtk_port_masterSlave_t   *pMasterSlaveActual);

/* Function Name:
 *      phy_gpy2xx_masterSlave_set
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
extern int32
phy_gpy2xx_masterSlave_set(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   masterSlave);

/* Function Name:
 *      phy_gpy2xx_loopback_get
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
extern int32
phy_gpy2xx_loopback_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable);

/* Function Name:
 *      phy_gpy2xx_loopback_set
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
extern int32
phy_gpy2xx_loopback_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      phy_gpy2xx_rtctResult_get
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
extern int32
phy_gpy2xx_rtctResult_get(uint32 unit, rtk_port_t port, rtk_rtctResult_t *pRtctResult);

/* Function Name:
 *      phy_gpy2xx_rtct_start
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
extern int32
phy_gpy2xx_rtct_start(uint32 unit, rtk_port_t port);

/* Function Name:
 *      phy_gpy2xx_ieeeTestMode_set
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
extern int32
phy_gpy2xx_ieeeTestMode_set(uint32 unit, rtk_port_t port, rtk_port_phyTestMode_t *pTestMode);

extern int32
phy_gpy2xx_portEyeMonitor_start(uint32 unit, rtk_port_t port, uint32 sdsId, uint32 frameNum);

/* Function Name:
*      phy_gpy2xx_linkStatus_get
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
extern int32
phy_gpy2xx_linkStatus_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pStatus);

/* Function Name:
*      phy_gpy2xx_usxgmii_linkStatus_get
* Description:
*      Get Maxlinear GPY PHY's MAC side USXGMII serdes link status
* Input:
*      unit    - unit id
*      port    - port id
* Output:
*      None
* Return:
*      RT_ERR_OK
*      RT_ERR_FAILED
* Note:
*        None
*/
extern int32
phy_gpy2xx_usxgmii_linkStatus_get(uint32 unit, rtk_port_t port);

extern int32 phy_gpy2xx_usxgmii_vr_reset(uint32 unit, rtk_port_t port);

extern int32 phy_gpy2xx_usxgmii_aneg_reset(uint32 unit, rtk_port_t port);

extern int32 phy_gpy2xx_usxgmii_reach_get(uint32 unit, rtk_port_t port, uint32 *pBuff);

extern int32 phy_gpy2xx_usxgmii_reach_set(uint32 unit, rtk_port_t port, uint32 *pBuff);

extern int32 phy_gpy2xx_usxgmii_loopback_get(uint32 unit, rtk_port_t port, uint32 *pmode);

extern int32 phy_gpy2xx_usxgmii_loopback_cfg(uint32 unit, rtk_port_t port, uint32 mode);

extern int32 phy_gpy2xx_usxgmii_alignmentmarker_get(uint32 unit, rtk_port_t port, uint32 *pam);

extern int32 phy_gpy2xx_usxgmii_alignmentmarker_set(uint32 unit, rtk_port_t port, uint32 am);

/* Function Name:
 *      phy_gpy2xx_mapperInit
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
extern void
phy_gpy2xx_mapperInit(rt_phydrv_t *pPhydrv);


#endif /* __HAL_PHY_PHY_GPY2xx_H__ */
