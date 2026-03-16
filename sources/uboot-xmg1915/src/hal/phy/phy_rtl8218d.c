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

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <osal/time.h>
#include <osal/lib.h>
#include <osal/memory.h>
#include <soc/type.h>
#include <hal/common/halctrl.h>
#include <hal/phy/phydef.h>
#include <hal/phy/phy_common.h>
#include <hal/phy/phy_rtl8218d.h>
#include <hal/phy/phy_rtl8218d_patch.h>
#include <hal/mac/miim_common_drv.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */

typedef enum phy_8218d_regPtp_e
{
    REG_8218D_PTP_BASE              = 0x1600,
    REG_8218D_PTP_TIME_NSEC_L       = 0x1600,
    REG_8218D_PTP_TIME_NSEC_H       = 0x1601,
    REG_8218D_PTP_TIME_SEC_L        = 0x1602,
    REG_8218D_PTP_TIME_SEC_M        = 0x1603,
    REG_8218D_PTP_TIME_CFG_0        = 0x1604,
    REG_8218D_PTP_MAC_ADDR_L        = 0x1605,
    REG_8218D_PTP_MAC_ADDR_M        = 0x1606,
    REG_8218D_PTP_MAC_ADDR_H        = 0x1607,
    REG_8218D_PTP_OTAG_TPID         = 0x1608,
    REG_8218D_PTP_ITAG_TPID         = 0x1609,
    REG_8218D_PTP_TIME_NSEC_L_RO    = 0x160A,
    REG_8218D_PTP_TIME_NSEC_H_RO    = 0x160B,
    REG_8218D_PTP_TIME_SEC_L_RO     = 0x160C,
    REG_8218D_PTP_TIME_SEC_M_RO     = 0x160D,
    REG_8218D_PTP_TIME_CFG_1        = 0x160E,
    REG_8218D_PTP_TIME_INT_STS_P    = 0x160F,

    /* per-port register addr-offset: 0x10 */
    REG_8218D_PTP_TIME_TX_SID_PN    = 0x1610,
    REG_8218D_PTP_TIME_RX_SID_PN    = 0x1614,
    REG_8218D_PTP_TIME_NSEC_L_PN    = 0x1618,
    REG_8218D_PTP_TIME_NSEC_H_PN    = 0x1619,
    REG_8218D_PTP_TIME_SEC_L_PN     = 0x161A,
    REG_8218D_PTP_TIME_SEC_M_PN     = 0x161B,
    REG_8218D_PTP_TIME_CTRL_PN      = 0x161C,
    REG_8218D_PTP_TIME_SEC_H_PN      = 0x161D,

    REG_8218D_PTP_FREQ_LSB      = 0x16D4,
    REG_8218D_PTP_FREQ_MSB      = 0x16D5,
    REG_8218D_PTP_TIME_SEC_H        = 0x16D6,
    REG_8218D_PTP_TIME_SEC_H_RO  = 0x16D7,
    REG_8218D_PTP_OTAG_TPID_1      = 0x16D8,
    REG_8218D_PTP_ITAG_TPID_1       = 0x16D9,
    REG_8218D_PTP_OTAG_TPID_2      = 0x16DA,
    REG_8218D_PTP_ITAG_TPID_2       = 0x16DB,
    REG_8218D_PTP_OTAG_TPID_3      = 0x16DC,
    REG_8218D_PTP_ITAG_TPID_3       = 0x16DD,
} phy_8218d_regPtp_t;

typedef enum phy_8218d_regPtpField_e
{
    REG_FIELD_8218D_PTP_PHY_ETH_EN              = (0x1 << 8),
    REG_FIELD_8218D_PTP_PHY_UDP_EN              = (0x1 << 9),

    REG_FIELD_8218D_PTP_CMD_EXEC            = (0x1 << 15),
    REG_FIELD_8218D_PTP_MAC_RNG_MSK          = (0x3FF << 6),
    REG_FIELD_8218D_PTP_CMD_OP_MSK          = (0x3 << 4),
    REG_FIELD_8218D_PTP_CMD_OP_READ         = (0x0 << 4),
    REG_FIELD_8218D_PTP_CMD_OP_WRITE        = (0x1 << 4),
    REG_FIELD_8218D_PTP_CMD_OP_ADJ_INC      = (0x2 << 4),
    REG_FIELD_8218D_PTP_CMD_OP_ADJ_DEC      = (0x3 << 4),
    REG_FIELD_8218D_PTP_TIME_NSEC_H         = (0x3FFF << 0),

    REG_FIELD_8218D_PTP_CFG_TIMER_EN_FRC    = (0x1 << 2),
    REG_FIELD_8218D_PTP_CFG_TIMER_1588_EN   = (0x1 << 1),

} phy_8218d_regPtpField_t;



#define REG_8218D_PTP_OFFSET_PORT(port) ((port % 8) * 0x10)
#define REG_8218D_PTP_OFFSET_MSGTYPE(type) (type % 4)


rt_phyInfo_t phy_8218D_info =
{
    .phy_num    = PORT_NUM_IN_8218D,
    .eth_type   = HWP_GE,
    .isComboPhy = {0, 0, 0, 0, 0, 0, 0, 0},
};
/*
 * Function Declaration
 */
int32
phy_8218d_greenEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      phy_8218d_init
 * Description:
 *      Initialize PHY 8218D.
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
phy_8218d_init(uint32 unit, rtk_port_t port)
{
    uint32  base_port = 0;
    int32 ret;

    if ((base_port = HWP_PHY_BASE_MACID(unit, port)) == HWP_NONE)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "unit=%u,port=%u get base port ID fail", unit, port);
        return RT_ERR_PORT_ID;
    }

    if ((ret = hal_miim_write(unit, base_port, 0, 30, 0x8)) != RT_ERR_OK)
        return ret;

     /*Disable Serdes Nway*/
    if ((ret = hal_miim_write(unit, base_port, 0x400, 0x10, 0x9703)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_write(unit, base_port, 0x500, 0x10, 0x9403)) != RT_ERR_OK)
        return ret;

     /* Set default value for phyESD checking in waMon */
    if ((ret = hal_miim_write(unit, base_port, 0x327, 0x10, 0x8218)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_write(unit, base_port, 0, 30, 0x0)) != RT_ERR_OK)
        return ret;

    phy_8218d_greenEnable_set(unit, port, DISABLED);

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_greenEnable_get
 * Description:
 *      Get the status of link-up green feature of the specific port in the specific unit
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pEnable - pointer to status of link-up green feature
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port link-up green feature.
 */
int32
phy_8218d_greenEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32   ret;
    uint32  phyData, fixed_page;

    fixed_page = 0;

    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* get value from CHIP*/
    if ((ret = hal_miim_write(unit, port, fixed_page, 27, 0x8011)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_read(unit, port, fixed_page, 28, &phyData)) != RT_ERR_OK)
        return ret;

    if ((phyData >> 15) & 0x1)
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_greenEnable_get */

/* Function Name:
 *      phy_8218d_greenEnable_set
 * Description:
 *      Set the status of link-up green feature of the specific port in the specific unit
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - status of link-up  green feature
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port link-up green feature.
 */
int32
phy_8218d_greenEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  value, fixed_page;
    int32   ret = RT_ERR_FAILED;

    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    fixed_page = 0;

    /* get value from CHIP*/
    if ((ret = hal_miim_write(unit, port, fixed_page, 27, 0x8011)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_read(unit, port, fixed_page, 28, &value)) != RT_ERR_OK)
        return ret;

    if (ENABLED == enable)
    {
        value |= (0x1 << 15);
    }
    else
    {
        value &= ~(0x1 << 15);
    }

    if ((ret = hal_miim_write(unit, port, fixed_page, 27, 0x8011)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_write(unit, port, fixed_page, 28, value)) != RT_ERR_OK)
        return ret;

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_greenEnable_set */


#if !defined(__BOOTLOADER__)

/* Function Name:
 *      phy_8218d_media_get
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
 *      The media type is return PORT_MEDIA_COPPER.
 */
int32
phy_8218d_media_get(uint32 unit, rtk_port_t port, rtk_port_media_t *pMedia)
{
    *pMedia = PORT_MEDIA_COPPER;
    return RT_ERR_OK;
} /* end of phy_8218d_media_get */


/* Function Name:
 *      phy_8218d_autoNegoAbility_get
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
phy_8218d_autoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    int32   ret;
    uint32  phyData4;
    uint32  phyData9;
    rtk_enable_t     enable;

    phy_common_autoNegoEnable_get(unit, port, &enable);

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_AN_ADVERTISEMENT_REG, &phyData4)) != RT_ERR_OK)
        return ret;

    pAbility->FC = (phyData4 & Pause_R4_MASK) >> Pause_R4_OFFSET;
    pAbility->AsyFC = (phyData4 & AsymmetricPause_R4_MASK) >> AsymmetricPause_R4_OFFSET;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_1000_BASET_CONTROL_REG, &phyData9)) != RT_ERR_OK)
        return ret;

    pAbility->Full_100= (phyData4 & _100Base_TX_FD_R4_MASK) >> _100Base_TX_FD_R4_OFFSET;
    pAbility->Half_100= (phyData4 & _100Base_TX_R4_MASK) >> _100Base_TX_R4_OFFSET;
    pAbility->Full_10= (phyData4 & _10Base_T_FD_R4_MASK) >> _10Base_T_FD_R4_OFFSET;
    pAbility->Half_10= (phyData4 & _10Base_T_R4_MASK) >> _10Base_T_R4_OFFSET;
    pAbility->Half_1000 = (phyData9 & _1000Base_THalfDuplex_MASK) >> _1000Base_THalfDuplex_OFFSET;
    pAbility->Full_1000 = (phyData9 & _1000Base_TFullDuplex_MASK) >> _1000Base_TFullDuplex_OFFSET;

    return ret;
} /* end of phy_8218d_autoNegoAbility_get */

/* Function Name:
 *      phy_8218d_autoNegoAbility_set
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
phy_8218d_autoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    int32   ret;
    uint32  phyData0;
    uint32  phyData4;
    uint32  phyData9;
    rtk_enable_t     enable;

    phy_common_autoNegoEnable_get(unit, port, &enable);

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_AN_ADVERTISEMENT_REG, &phyData4)) != RT_ERR_OK)
        return ret;

    phyData4 = phyData4 & ~(Pause_R4_MASK | AsymmetricPause_R4_MASK);
    phyData4 = phyData4
            | (pAbility->FC << Pause_R4_OFFSET)
            | (pAbility->AsyFC << AsymmetricPause_R4_OFFSET);

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_1000_BASET_CONTROL_REG, &phyData9)) != RT_ERR_OK)
        return ret;

    phyData4 = phyData4 &
            ~(_100Base_TX_FD_R4_MASK | _100Base_TX_R4_MASK | _10Base_T_FD_R4_MASK | _10Base_T_R4_MASK);
    phyData4 = phyData4
            | (pAbility->Full_100 << _100Base_TX_FD_R4_OFFSET)
            | (pAbility->Half_100 << _100Base_TX_R4_OFFSET)
            | (pAbility->Full_10 << _10Base_T_FD_R4_OFFSET)
            | (pAbility->Half_10 << _10Base_T_R4_OFFSET);

    phyData9 = phyData9 & ~(_1000Base_TFullDuplex_MASK | _1000Base_THalfDuplex_MASK);
    phyData9 = phyData9 | (pAbility->Full_1000 << _1000Base_TFullDuplex_OFFSET)
               | (pAbility->Half_1000 << _1000Base_THalfDuplex_OFFSET);

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_AN_ADVERTISEMENT_REG, phyData4)) != RT_ERR_OK)
        return ret;


    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_1000_BASET_CONTROL_REG, phyData9)) != RT_ERR_OK)
        return ret;

    /* Force re-autonegotiation if AN is on*/
    if (ENABLED == enable)
    {
        if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
            return ret;

        phyData0 = phyData0 & ~(RestartAutoNegotiation_MASK);
        phyData0 = phyData0 | (enable << RestartAutoNegotiation_OFFSET);

        if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData0)) != RT_ERR_OK)
            return ret;
    }

    return ret;
} /* end of phy_8218d_autoNegoAbility_set */

/* Function Name:
 *      phy_8218d_speed_get
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
phy_8218d_speed_get(uint32 unit, rtk_port_t port, uint32 *pSpeed)
{
    int32   ret;
    uint32  val;
    uint32  phyData0;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_STATUS_REG, &val)) != RT_ERR_OK)
        return ret;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    *pSpeed = ((phyData0 & SpeedSelection1_MASK) >> (SpeedSelection1_OFFSET -1))
              | ((phyData0 & SpeedSelection0_MASK) >> SpeedSelection0_OFFSET);

    return ret;
} /* end of phy_8218d_speed_get */

/* Function Name:
 *      phy_8218d_speed_set
 * Description:
 *      Set speed mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      speed         - link speed status 10/100/1000
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK                 - OK
 *      RT_ERR_FAILED             - invalid parameter
 *      RT_ERR_CHIP_NOT_SUPPORTED - copper media chip is not supported Force-1000
 * Note:
 *      None
 */
int32
phy_8218d_speed_set(uint32 unit, rtk_port_t port, uint32 speed)
{
    int32   ret;
    uint32  phyData0;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    phyData0 = phyData0 & ~(SpeedSelection1_MASK | SpeedSelection0_MASK);
    phyData0 = phyData0 | (((speed & 2) << (SpeedSelection1_OFFSET - 1)) | ((speed & 1) << SpeedSelection0_OFFSET));

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData0)) != RT_ERR_OK)
        return ret;

    return ret;
} /* end of phy_8218d_speed_set */



/* Function Name:
 *      phy_8218d_eeeEnable_get
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
phy_8218d_eeeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32   ret;
    uint32  phyData;

    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, 0xa43, 25, &phyData)) != RT_ERR_OK)
        return ret;

    if (((phyData >> 5) & 0x1) == 0x1)
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_eeeEnable_get */

/* Function Name:
 *      phy_8218_eeeEnable_set
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
phy_8218d_eeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  phyData;
    int32   ret = RT_ERR_FAILED;
    rtk_enable_t     an_enable;


    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    phy_common_autoNegoEnable_get(unit, port, &an_enable);

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, 0xa43, 25, &phyData)) != RT_ERR_OK)
            return ret;

    #if 0 /* always configure to MAC mode EEE, the mode configuration could be aparted to another API in the future */
    if (ENABLED == enable)
    {
        phyData |= (1 << 5);
        if ((ret = hal_miim_write(unit, port, 0xa43, 25, phyData)) != RT_ERR_OK)
            return ret;
    }
    else
    #else
    {
        phyData &= ~(1 << 5);
        if ((ret = hal_miim_write(unit, port, 0xa43, 25, phyData)) != RT_ERR_OK)
            return ret;
    }
    #endif

    if(ENABLED == enable)
        phyData = 0x6; /*enable 100M/1000M EEE ability*/
    else
        phyData = 0x0; /*disable 100M/1000M EEE ability*/

    ret = hal_miim_mmd_write(unit, port, 7, 60, phyData);
    if ((ret != RT_ERR_OK)&&(ret != RT_ERR_CHIP_NOT_SUPPORTED))
    {
        return ret;
    }

    if ((ret = hal_miim_read(unit, port, 0xa42, 20, &phyData)) != RT_ERR_OK)
    {
        return ret;
    }

    if(ENABLED == enable)
        phyData |= 0x1 << 7; /*enable 500M EEE ability*/
    else
        phyData &= ~(0x1 << 7); /*disable 500M EEE ability*/

    if ((ret = hal_miim_write(unit, port, 0xa42, 20, phyData)) != RT_ERR_OK)
    {
        return ret;
    }

    /* Force re-autonegotiation if AN is on*/
    if (ENABLED == an_enable)
    {
        if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
            return ret;

        phyData = phyData & ~(RestartAutoNegotiation_MASK);
        phyData = phyData | (an_enable << RestartAutoNegotiation_OFFSET);

        if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
            return ret;
    }

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_eeeEnable_set */

/* Function Name:
 *      phy_8218d_crossOverMode_get
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
phy_8218d_crossOverMode_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t *pMode)
{
    int32   ret = RT_ERR_FAILED;
    uint32  phyData, force_mode, mdi;

    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, 0xa43, 24, &phyData)) != RT_ERR_OK)
        return ret;

    if((phyData & (1 << 9)) != 0)
    {
        force_mode = 1;
        if((phyData & (1 << 8)) != 0)
            mdi = 1;
        else
            mdi = 0;
    }else{
        force_mode = 0;
    }

    if (force_mode)
    {
        if (mdi)
            *pMode = PORT_CROSSOVER_MODE_MDI;
        else
            *pMode = PORT_CROSSOVER_MODE_MDIX;
    }
    else
        *pMode = PORT_CROSSOVER_MODE_AUTO;

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_crossOverMode_get */

/* Function Name:
 *      phy_8218d_crossOverMode_set
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
phy_8218d_crossOverMode_set(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t mode)
{
    int32   ret = RT_ERR_FAILED;
    uint32  phyData;


    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, 0xa43, 24, &phyData)) != RT_ERR_OK)
        return ret;

    switch (mode)
    {
        case PORT_CROSSOVER_MODE_AUTO:
            phyData &= ~(1 << 9);
            break;
        case PORT_CROSSOVER_MODE_MDI:
            phyData |= (1 << 9);
            phyData |= (1 << 8);
            break;
        case PORT_CROSSOVER_MODE_MDIX:
            phyData |= (1 << 9);
            phyData &= ~(1 << 8);
            break;
        default:
            return RT_ERR_INPUT;
    }

    if ((ret = hal_miim_write(unit, port, 0xa43, 24, phyData)) != RT_ERR_OK)
        return ret;

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218_crossOverMode_set */

/* Function Name:
 *      phy_8218d_crossOverStatus_get
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
phy_8218d_crossOverStatus_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_status_t *pStatus)
{
    int32   ret = RT_ERR_FAILED;
    uint32  phyData;

    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, 0xa43, 26, &phyData)) != RT_ERR_OK)
        return ret;

    if((phyData & (1 << 1)) != 0)
    {
        *pStatus = PORT_CROSSOVER_STATUS_MDI;
    }else{
        *pStatus = PORT_CROSSOVER_STATUS_MDIX;
    }

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_crossOverStatus_get */

/* Function Name:
 *      phy_8218d_linkDownPowerSavingEnable_get
 * Description:
 *      Get the status of link-down power saving of the specific port in the specific unit
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - pointer to status of link-down power saving
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port link-down power saving
 */
int32
phy_8218d_linkDownPowerSavingEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  value;
    int32   ret = RT_ERR_FAILED;

    /* For Link-Down Power Saving (per-port) */
    if ((ret = hal_miim_read(unit, port, 0xa43, 24, &value)) != RT_ERR_OK)
        return ret;

    if (((value >> 2) & 0x1) == 0x1)
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;
    return RT_ERR_OK;
} /* end of phy_8218d_linkDownPowerSavingEnable_get */

/* Function Name:
 *      phy_8218d_linkDownPowerSavingEnable_set
 * Description:
 *      Set the status of link-down power saving of the specific port in the specific unit
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - status of link-down power saving
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port link-down power saving
 */
int32
phy_8218d_linkDownPowerSavingEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  value;
    int32   ret = RT_ERR_FAILED;

    /* For Link-Down Power Saving (per-port) */
    if ((ret = hal_miim_read(unit, port, 0xa43, 24, &value)) != RT_ERR_OK)
        return ret;

    value &= ~(0x1 << 2);
    if (ENABLED == enable)
    {
        value |= (0x1 << 2);
    }
    if ((ret = hal_miim_write(unit, port, 0xa43, 24, value)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_linkDownPowerSavingEnable_set */

/* Function Name:
 *      phy_8218d_broadcastEnable_set
 * Description:
 *      Set enable status of broadcast mode
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      enable        - broadcast enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_8218d_broadcastEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32   ret;
    uint32  base_port = 0;
    uint32  phyData;

    if ((base_port = HWP_PHY_BASE_MACID(unit, port)) == HWP_NONE)
    {
        return RT_ERR_PORT_ID;
    }

    /* get value from CHIP*/
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 30, 0x0008)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 31, 0x0266)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_read(unit, base_port, 0xa42, 22, &phyData)) != RT_ERR_OK)
        return ret;
    if (enable)
        phyData |= (0x1 << (port-base_port+8));
    else
        phyData &= ~(0x1 << (port-base_port+8));
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 22, phyData)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return ret;
} /* end of phy_8218d_broadcastEnable_set */

/* Function Name:
 *      phy_8218d_broadcastID_set
 * Description:
 *      Set broadcast ID
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      broadcastID   - broadcast ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_8218d_broadcastID_set(uint32 unit, rtk_port_t port, uint32 broadcastID)
{
    int32   ret;
    uint32  base_port = 0;
    uint32  phyData;

    if ((base_port = HWP_PHY_BASE_MACID(unit, port)) == HWP_NONE)
    {
        return RT_ERR_PORT_ID;
    }

    /* get value from CHIP*/
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 30, 0x0008)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 31, 0x0266)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_read(unit, base_port, 0xa42, 22, &phyData)) != RT_ERR_OK)
        return ret;
    phyData &= ~(0x1f);
    phyData |= (broadcastID & 0x1f);
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 22, phyData)) != RT_ERR_OK)
        return ret;
    if ((ret = hal_miim_write(unit, base_port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return ret;
} /* end of phy_8218d_broadcastID_set */

/* Function Name:
 *      phy_8218d_gigaLiteEnable_get
 * Description:
 *      Get the status of Giga Lite of the specific port in the specific unit
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - pointer to status of Giga Lite
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port Giga Lite feature.
 */
int32
phy_8218d_gigaLiteEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  value;
    int32   ret = RT_ERR_FAILED;

    /* Configure the GPHY page to copper */
   if ((ret = hal_miim_write(unit, port, GIGA_LITE_CTRL_REG, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* For Giga Lite (per-port) */
   if ((ret = hal_miim_read(unit, port, GIGA_LITE_CTRL_REG, 20, &value)) != RT_ERR_OK)
       return ret;

   if (((value >> 9) & 0x1) == 0x1)
       *pEnable = ENABLED;
   else
       *pEnable = DISABLED;

    /* Configure the GPHY page to auto */
   if ((ret = hal_miim_write(unit, port, GIGA_LITE_CTRL_REG, 30, 0x0000)) != RT_ERR_OK)
       return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_gigaLiteEnable_get */

/* Function Name:
 *      phy_8218d_gigaLiteEnable_set
 * Description:
 *      Set the status of Giga Lite of the specific port in the specific unit
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - status of Giga Lite
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port Giga Lite feature.
 */
int32
phy_8218d_gigaLiteEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  value;
    int32   ret = RT_ERR_FAILED;


    /* Configure the GPHY page to copper */
    if ((ret = hal_miim_write(unit, port, GIGA_LITE_CTRL_REG, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* bit[2]: configure 2-pair auto-downspeed */
    /* bit[9]: configure retry speed down to 500M */
    if ((ret = hal_miim_read(unit, port, 0xa44, 17, &value)) != RT_ERR_OK)
        return ret;

    value &= ~(0x1 << 9);
    value &= ~(0x1 << 2);
    if (ENABLED == enable)
    {
        value |= (0x1 << 9);
        value |= (0x1 << 2);
    }
    if ((ret = hal_miim_write(unit, port, 0xa44, 17, value)) != RT_ERR_OK)
        return ret;


    /* For Giga Lite (per-port) */
    if ((ret = hal_miim_read(unit, port, GIGA_LITE_CTRL_REG, 20, &value)) != RT_ERR_OK)
        return ret;

    value &= ~(0x1 << 9);
    if (ENABLED == enable)
    {
        value |= (0x1 << 9);
    }

    if ((ret = hal_miim_write(unit, port, GIGA_LITE_CTRL_REG, 20, value)) != RT_ERR_OK)
        return ret;

    /* Do the restart N-WAY */
    if ((ret = hal_miim_read(unit, port, 0, 0, &value)) != RT_ERR_OK)
        return ret;
    value |= (0x1UL<<9);
    if ((ret = hal_miim_write(unit, port, 0, 0, value)) != RT_ERR_OK)
        return ret;

    /* Configure the GPHY page to auto */
    if ((ret = hal_miim_write(unit, port, GIGA_LITE_CTRL_REG, 30, 0x0000)) != RT_ERR_OK)
        return ret;


    return RT_ERR_OK;
} /* end of phy_8218d_gigaLiteEnable_set */

/* Function Name:
 *      phy_8218d_downSpeedEnable_get
 * Description:
 *      Get UTP down speed 1000M --> 100M status of the specific port
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - pointer to status of down speed
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID - invalid port id
 * Note:
 *      None
 */
int32
phy_8218d_downSpeedEnable_get(uint32 unit, rtk_port_t port,
    rtk_enable_t *pEnable)
{
    uint32  phyData;
    int32   ret;

    if ((ret = hal_miim_write(unit, port, 0, 30, 1)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_read(unit, port, 0xa44, 17, &phyData)) != RT_ERR_OK)
        goto ERR;

    if (phyData & (1 << 3))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    if ((ret = hal_miim_write(unit, port, 0, 30, 0)) != RT_ERR_OK)
        return ret;
    return ret;
ERR:
    hal_miim_write(unit, port, 0, 30, 0);
    return ret;
}

/* Function Name:
 *      phy_8218d_downSpeedEnable_set
 * Description:
 *      Set UTP down speed 1000M --> 100M status of the specific port
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      enable  - status of down speed
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID - invalid port id
 * Note:
 *      None
 */
int32
phy_8218d_downSpeedEnable_set(uint32 unit, rtk_port_t port,
    rtk_enable_t enable)
{
    uint32  phyData;
    int32   ret;

    if ((ret = hal_miim_write(unit, port, 0, 30, 1)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_read(unit, port, 0xa44, 17, &phyData)) != RT_ERR_OK)
        goto ERR;

    /* DownSpeed to 100M*/
    phyData &= ~(1 << 5);

    if (ENABLED == enable)
        phyData |= (1 << 3);
    else
        phyData &= ~(1 << 3);

    if ((ret = hal_miim_write(unit, port, 0xa44, 17, phyData)) != RT_ERR_OK)
        goto ERR;

    if ((ret = hal_miim_write(unit, port, 0, 30, 0)) != RT_ERR_OK)
        return ret;

    return ret;
ERR:
    hal_miim_write(unit, port, 0, 30, 0);
    return ret;
}

/* Function Name:
 *      _phy_8218d_ptpReg_get
 * Description:
 *      Get PTP register data of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      reg_addr            - reg address
 * Output:
 *      pData               - pointer to the PHY reg data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID       - invalid port id
 *      RT_ERR_NULL_POINTER  - input parameter may be null pointer
 * Note:
 *      None
 */
static int32
_phy_8218d_ptpReg_get(uint32 unit, rtk_port_t port, uint32 reg_addr, uint32 *pData)
{
    int32 ret;
    rtk_port_t port_base;
    uint32 page, reg;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_PORT), "unit=%d, port=%d, reg_addr=0x%x, reg=0x%x",
           unit, port, reg_addr);

    /* parameter check */
    RT_PARAM_CHK((NULL == pData), RT_ERR_NULL_POINTER);

    if ((port_base = HWP_PHY_BASE_MACID(unit, port)) == HWP_NONE)
    {
        return RT_ERR_PORT_ID;
    }

    page = (reg_addr - (reg_addr % 8)) / 8;
    reg = ((reg_addr - REG_8218D_PTP_BASE) % 8) + 16;

    if ((ret = hal_miim_write(unit, port_base, 0, 30, 8)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = hal_miim_read(unit, port_base, page, reg, pData)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = hal_miim_write(unit, port_base, 0, 30, 0)) != RT_ERR_OK)
    {
        return ret;
    }

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_PORT), "pData=0x%x", *pData);

    return RT_ERR_OK;
}

/* Function Name:
 *      _phy_8218d_ptpReg_set
 * Description:
 *      Set PTP register data of the specific port
 * Input:
 *      unit               - unit id
 *      port               - port id
 *      reg_addr           - reg address
 *      reg_val            - reg value
 *      reg_msk            - reg value mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
int32
_phy_8218d_ptpReg_set(uint32 unit, uint32 port, uint32 reg_addr, uint32 reg_val, uint32 reg_msk)
{
    int32 ret;
    rtk_port_t port_base;
    uint32 page, reg, ori_val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_TIME), "unit=%d, port=%d, reg_addr=0x%x, reg_val=0x%x, reg_msk=0x%x",
            unit, port, reg_addr, reg_val, reg_msk);

    /* parameter check */
    RT_PARAM_CHK(!HWP_PORT_EXIST(unit, port), RT_ERR_PORT_ID);

    if ((port_base = HWP_PHY_BASE_MACID(unit, port)) == HWP_NONE)
    {
        return RT_ERR_PORT_ID;
    }

    page = (reg_addr - (reg_addr % 8)) / 8;
    reg = ((reg_addr - REG_8218D_PTP_BASE) % 8) + 16;

    if ((ret = hal_miim_write(unit, port_base, 0, 30, 8)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((reg_msk & 0xFFFF) != 0xFFFF)
    {
        if ((ret = hal_miim_read(unit, port_base, page, reg, &ori_val)) != RT_ERR_OK)
        {
            return ret;
        }

        reg_val = (ori_val & ~reg_msk) | (reg_val & reg_msk);
    }
    if ((ret = hal_miim_write(unit, port_base, page, reg, reg_val)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = hal_miim_write(unit, port_base, 0, 30, 0)) != RT_ERR_OK)
    {
        return ret;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_8218d_ptpSwitchMacAddr_get
 * Description:
 *      Get the Switch MAC address setting of PHY of the specified port.
 * Input:
 *      unit           - unit id
 *      port           - port id
 *      pSwitchMacAddr - point to the Switch MAC Address
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
phy_8218d_ptpSwitchMacAddr_get(uint32 unit, rtk_port_t port, rtk_mac_t *pSwitchMacAddr)
{
    int32 ret;
    uint32 mac_h, mac_m, mac_l;

    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_MAC_ADDR_H, &mac_h)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_MAC_ADDR_M, &mac_m)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_MAC_ADDR_L, &mac_l)) != RT_ERR_OK)
    {
        return ret;
    }

    pSwitchMacAddr->octet[0] = (uint8)(mac_h >> 8);
    pSwitchMacAddr->octet[1] = (uint8)(mac_h & 0xFF);
    pSwitchMacAddr->octet[2] = (uint8)(mac_m >> 8);
    pSwitchMacAddr->octet[3] = (uint8)(mac_m & 0xFF);
    pSwitchMacAddr->octet[4] = (uint8)(mac_l >> 8);
    pSwitchMacAddr->octet[5] = (uint8)(mac_l & 0xFF);

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpSwitchMacAddr_set
 * Description:
 *      Set the Switch MAC address setting of PHY of the specified port.
 * Input:
 *      unit           - unit id
 *      port           - port id
 *      pSwitchMacAddr - point to the Switch MAC Address
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
phy_8218d_ptpSwitchMacAddr_set(uint32 unit, rtk_port_t port, rtk_mac_t *pSwitchMacAddr)
{
    int32 ret;
    uint32 mac_h, mac_m, mac_l;

    mac_h = (pSwitchMacAddr->octet[0] << 8) | (pSwitchMacAddr->octet[1]);
    mac_m = (pSwitchMacAddr->octet[2] << 8) | (pSwitchMacAddr->octet[3]);
    mac_l = (pSwitchMacAddr->octet[4] << 8) | (pSwitchMacAddr->octet[5]);

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_MAC_ADDR_H, mac_h, 0xFFFF)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_MAC_ADDR_M, mac_m, 0xFFFF)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_MAC_ADDR_L, mac_l, 0xFFFF)) != RT_ERR_OK)
    {
        return ret;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpRefTime_get
 * Description:
 *      Get the reference time of PHY of the specified port.
 * Input:
 *      unit       - unit id
 * Output:
 *      pTimeStamp - pointer buffer of the reference time
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpRefTime_get(uint32 unit, rtk_port_t port,
    rtk_time_timeStamp_t *pTimeStamp)
{
    int32 ret;
    uint32 sec_l, sec_m, sec_h, nsec_l, nsec_h;
    uint32 reg_val, reg_msk;

    /* execute reading command, [15] = 0x1 executing, [13:12] = 0x0 read */
    reg_val = REG_FIELD_8218D_PTP_CMD_OP_READ;
    reg_msk = REG_FIELD_8218D_PTP_CMD_OP_MSK;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_CFG_0, reg_val, reg_msk)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    reg_val = REG_FIELD_8218D_PTP_CMD_EXEC;
    reg_msk = REG_FIELD_8218D_PTP_CMD_EXEC;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_NSEC_H, reg_val, reg_msk)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    do {
        if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_NSEC_H, &reg_val)) != RT_ERR_OK)
        {
            RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
            return ret;
        }
    } while (reg_val & REG_FIELD_8218D_PTP_CMD_EXEC); /* busy watting */

    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_NSEC_L_RO, &nsec_l)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_NSEC_H_RO, &nsec_h)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_SEC_L_RO, &sec_l)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_SEC_M_RO, &sec_m)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_SEC_H_RO, &sec_h)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    pTimeStamp->sec = ((uint64)sec_h << 32) | ((uint64)sec_m << 16) | ((uint64)sec_l & 0xFFFF);
    pTimeStamp->nsec = (((nsec_h & 0x3FFF) << 16) | (nsec_l & 0xFFFF));

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpRefTime_set
 * Description:
 *      Set the reference time of PHY of the specified port.
 * Input:
 *      unit      - unit id
 *      timeStamp - reference timestamp value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_8218d_ptpRefTime_set(uint32 unit, rtk_port_t port, rtk_time_timeStamp_t timeStamp)
{
    int32 ret;
    uint32 sec_l, sec_m, sec_h, nsec_l, nsec_h;
    uint32 reg_val, reg_msk;

    /* adjust Timer of PHY */
    sec_l = (timeStamp.sec) & 0xFFFF;
    sec_m = ((timeStamp.sec) >> 16) & 0xFFFF;
    sec_h = ((timeStamp.sec) >> 32) & 0xFFFF;
    /* convert nsec to 8nsec */
    nsec_l = timeStamp.nsec & 0xFFFF;
    nsec_h = (timeStamp.nsec >> 16) & 0x3FFF;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_SEC_L, sec_l, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_SEC_M, sec_m, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_SEC_H, sec_h, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_NSEC_L, nsec_l, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_NSEC_H, nsec_h, 0x3FFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    reg_val = REG_FIELD_8218D_PTP_CMD_OP_WRITE;
    reg_msk = REG_FIELD_8218D_PTP_CMD_OP_MSK;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_CFG_0, reg_val, reg_msk)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    reg_val = REG_FIELD_8218D_PTP_CMD_EXEC;
    reg_msk = REG_FIELD_8218D_PTP_CMD_EXEC;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_NSEC_H, reg_val, reg_msk)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    /* busy watting */
    do {
        if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_NSEC_H, &reg_val)) != RT_ERR_OK)
        {
            RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
            return ret;
        }
    } while (reg_val & REG_FIELD_8218D_PTP_CMD_EXEC);

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpRefTimeAdjust_set
 * Description:
 *      Adjust the reference time of PHY of the specified port.
 * Input:
 *      unit      - unit id
 *      port    - port id
 *      sign      - significant
 *      timeStamp - reference timestamp value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      sign=0 for positive adjustment, sign=1 for negative adjustment.
 */
int32
phy_8218d_ptpRefTimeAdjust_set(uint32 unit, rtk_port_t port, uint32 sign, rtk_time_timeStamp_t timeStamp)
{
    int32 ret;
    uint32 sec_l, sec_m, sec_h, nsec_l, nsec_h;
    uint32 reg_val, reg_msk;

    /* adjust Timer of PHY */
    sec_l = (timeStamp.sec) & 0xFFFF;
    sec_m = ((timeStamp.sec) >> 16) & 0xFFFF;
    sec_h = ((timeStamp.sec) >> 32) & 0xFFFF;
    /* convert nsec to 8nsec */
    nsec_l = (timeStamp.nsec) & 0xFFFF;
    nsec_h = ((timeStamp.nsec) >> 16) & 0x07FF;

    nsec_h = nsec_h | REG_FIELD_8218D_PTP_CMD_EXEC;

    if (sign == 0)
    {
        reg_val = REG_FIELD_8218D_PTP_CMD_OP_ADJ_INC;
    } else {
        reg_val = REG_FIELD_8218D_PTP_CMD_OP_ADJ_DEC;
    }

    reg_msk = REG_FIELD_8218D_PTP_CMD_OP_MSK;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_CFG_0, reg_val, reg_msk)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_SEC_L, sec_l, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_SEC_M, sec_m, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_SEC_H, sec_h, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_NSEC_L, nsec_l, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_set(unit, port, REG_8218D_PTP_TIME_NSEC_H, nsec_h, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    /* busy watting */
    do {
        if ((ret = _phy_8218d_ptpReg_get(unit, port, REG_8218D_PTP_TIME_NSEC_H, &reg_val)) != RT_ERR_OK)
        {
            RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
            return ret;
        }
    } while (reg_val & REG_FIELD_8218D_PTP_CMD_EXEC);

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpRefTimeEnable_get
 * Description:
 *      Get the enable state of reference time of PHY of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpRefTimeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32 ret;
    uint32 reg_addr, reg_val, reg_msk;

    reg_addr = REG_8218D_PTP_TIME_CFG_0;
    reg_msk = (REG_FIELD_8218D_PTP_CFG_TIMER_EN_FRC | REG_FIELD_8218D_PTP_CFG_TIMER_1588_EN);

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if (reg_val & reg_msk)
    {
        *pEnable = ENABLED;
    }
    else
    {
        *pEnable = DISABLED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpRefTimeEnable_set
 * Description:
 *      Set the enable state of reference time of PHY of the specified port.
 * Input:
 *      unit   - unit id
 *      port    - port id
 *      enable - status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_8218d_ptpRefTimeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32 ret;
    uint32 reg_addr, reg_val, reg_msk;

    reg_addr = REG_8218D_PTP_TIME_CFG_0;
    reg_val = (enable == ENABLED)? \
        (REG_FIELD_8218D_PTP_CFG_TIMER_EN_FRC | REG_FIELD_8218D_PTP_CFG_TIMER_1588_EN) : 0;
    reg_msk = (REG_FIELD_8218D_PTP_CFG_TIMER_EN_FRC | REG_FIELD_8218D_PTP_CFG_TIMER_1588_EN);

    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, reg_msk)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpRefTimeFreq_get
 * Description:
 *      Get the frequency of reference time of PHY of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pFreq  - pointer to reference time frequency
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      The frequency configuration decides the reference time tick frequency.
 *      The default value is 0x8000000.
 *      If it is configured to 0x4000000, the tick frequency would be half of default.
 *      If it is configured to 0xC000000, the tick frequency would be one and half times of default.
 */
int32
phy_8218d_ptpRefTimeFreq_get(uint32 unit, rtk_port_t port, uint32 *pFreq)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_FREQ_LSB;

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    *pFreq = reg_val;

    reg_addr = REG_8218D_PTP_FREQ_MSB;

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    *pFreq |= reg_val << 16;

    return RT_ERR_OK;
} /* end of phy_8218d_ptpRefTimeFreq_get */

/* Function Name:
 *      phy_8218d_ptpRefTimeFreq_set
 * Description:
 *      Set the frequency of reference time of PHY of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      freq   - reference time frequency
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      The frequency configuration decides the reference time tick frequency.
 *      The default value is 0x8000000.
 *      If it is configured to 0x4000000, the tick frequency would be half of default.
 *      If it is configured to 0xC000000, the tick frequency would be one and half times of default.
 */
int32
phy_8218d_ptpRefTimeFreq_set(uint32 unit, rtk_port_t port, uint32 freq)
{
    int32 ret;
    uint32 reg_addr;

    reg_addr = REG_8218D_PTP_FREQ_LSB;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, freq, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    reg_addr = REG_8218D_PTP_FREQ_MSB;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, (freq>>16), 0x0FFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    return RT_ERR_OK;
} /* end of phy_8218d_ptpRefTimeFreq_set */

/* Function Name:
 *      phy_8218d_ptpEnable_get
 * Description:
 *      Get PTP status of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT         - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_TIME_CTRL_PN + REG_8218D_PTP_OFFSET_PORT(port);

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if (reg_val & REG_FIELD_8218D_PTP_PHY_UDP_EN)
    {
        *pEnable = ENABLED;
    } else {
        *pEnable = DISABLED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpEnable_set
 * Description:
 *      Set PTP status of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT     - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_8218d_ptpEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_TIME_CTRL_PN + REG_8218D_PTP_OFFSET_PORT(port);
    reg_val = (enable == ENABLED)? REG_FIELD_8218D_PTP_PHY_UDP_EN |REG_FIELD_8218D_PTP_PHY_ETH_EN  : 0;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, REG_FIELD_8218D_PTP_PHY_UDP_EN | REG_FIELD_8218D_PTP_PHY_ETH_EN)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpInterruptStatus_get
 * Description:
 *      Get PTP interrupt status of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pIsPortIntr - port interrupt triggered status
 *      pIntrSts - interrupt status of RX/TX PTP frame types
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT         - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpInterruptStatus_get(uint32 unit, rtk_port_t port, uint32 *pIsPortIntr, uint32 *pIntrSts)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_TIME_INT_STS_P;

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if (reg_val & (1 << (port % 8)))
    {
        *pIsPortIntr = ENABLED;
    } else {
        *pIsPortIntr = DISABLED;
    }

    reg_addr = REG_8218D_PTP_TIME_CTRL_PN + REG_8218D_PTP_OFFSET_PORT(port);

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    *pIntrSts = reg_val & 0xFF;

    /*clear the interrupt*/
    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, 0xFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpInterruptEnable_get
 * Description:
 *      Get PTP interrupt enable status of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT         - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpInterruptEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_TIME_CFG_1;

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    if (reg_val & 0xFF)
    {
        *pEnable = ENABLED;
    } else {
        *pEnable = DISABLED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpInterruptEnable_set
 * Description:
 *      Set PTP interrupt enable status of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT     - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_8218d_ptpInterruptEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_TIME_CFG_1;
    reg_val = (enable == ENABLED)? 0xFF  : 0;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, 0xFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpRxTimestamp_get
 * Description:
 *      Get PTP Rx timstamp according to the PTP identifier on the dedicated port from the specified device.
 * Input:
 *      unit       - unit id
 *      port       - port id
 *      identifier - indentifier of PTP packet
 * Output:
 *      pTimeStamp - pointer buffer of TIME timestamp
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpRxTimestamp_get(uint32 unit, rtk_port_t port,
    rtk_time_ptpIdentifier_t identifier, rtk_time_timeStamp_t *pTimeStamp)
{
    int32 ret;
    uint32 reg_addr, reg_val;
    uint32 sec_l, sec_m, sec_h, nsec_l, nsec_h;

    reg_addr = REG_8218D_PTP_TIME_RX_SID_PN + \
               REG_8218D_PTP_OFFSET_PORT(port) + \
               REG_8218D_PTP_OFFSET_MSGTYPE(identifier.msgType);

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        return ret;
    }

    /* Input sequence ID NEED match currently sequence ID of chip */
    if (reg_val != identifier.sequenceId)
    {
        reg_addr = REG_8218D_PTP_TIME_CTRL_PN + REG_8218D_PTP_OFFSET_PORT(port);
        reg_val = 0x1 << (4 + REG_8218D_PTP_OFFSET_MSGTYPE(identifier.msgType));
        if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, reg_val)) != RT_ERR_OK)
        {
            RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
            return ret;
        }

        return RT_ERR_INPUT;
    }


    /* Get Rx Timestamp */
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_SEC_L_PN + REG_8218D_PTP_OFFSET_PORT(port), &sec_l)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_SEC_M_PN + REG_8218D_PTP_OFFSET_PORT(port), &sec_m)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_SEC_H_PN + REG_8218D_PTP_OFFSET_PORT(port), &sec_h)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_NSEC_L_PN + REG_8218D_PTP_OFFSET_PORT(port), &nsec_l)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_NSEC_H_PN + REG_8218D_PTP_OFFSET_PORT(port), &nsec_h)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    /* Clear the pending status */
    reg_addr = REG_8218D_PTP_TIME_CTRL_PN + REG_8218D_PTP_OFFSET_PORT(port);
    reg_val = 0x1 << (4 + REG_8218D_PTP_OFFSET_MSGTYPE(identifier.msgType));
    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    pTimeStamp->sec = ((uint64)sec_h << 32) | ((uint64)sec_m << 16) | ((uint64)sec_l & 0xFFFF);
    pTimeStamp->nsec = ((nsec_h & 0x7FF) << 16) | (nsec_l & 0xFFFF);

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpTxTimestamp_get
 * Description:
 *      Get PTP Tx timstamp according to the PTP identifier on the dedicated port from the specified device.
 * Input:
 *      unit       - unit id
 *      port       - port id
 *      identifier - indentifier of PTP packet
 * Output:
 *      pTimeStamp - pointer buffer of TIME timestamp
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpTxTimestamp_get(uint32 unit, rtk_port_t port,
    rtk_time_ptpIdentifier_t identifier, rtk_time_timeStamp_t *pTimeStamp)
{
    int32 ret;
    uint32 reg_addr, reg_val;
    uint32 sec_l, sec_m, sec_h, nsec_l, nsec_h;

    reg_addr = REG_8218D_PTP_TIME_TX_SID_PN + \
               REG_8218D_PTP_OFFSET_PORT(port) + \
               REG_8218D_PTP_OFFSET_MSGTYPE(identifier.msgType);

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        return ret;
    }

    /* Input sequence ID NEED match currently sequence ID of chip */
    if (reg_val != identifier.sequenceId)
    {
        /* Clear the pending status */
        reg_addr = REG_8218D_PTP_TIME_CTRL_PN + REG_8218D_PTP_OFFSET_PORT(port);
        reg_val = 0x1 << (REG_8218D_PTP_OFFSET_MSGTYPE(identifier.msgType));
        if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, reg_val)) != RT_ERR_OK)
        {
            RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
            return ret;
        }

        return RT_ERR_NOT_FINISH;
    }

    /* Get Tx Timestamp */
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_SEC_L_PN + REG_8218D_PTP_OFFSET_PORT(port), &sec_l)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_SEC_M_PN + REG_8218D_PTP_OFFSET_PORT(port), &sec_m)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_SEC_H_PN + REG_8218D_PTP_OFFSET_PORT(port), &sec_h)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_NSEC_L_PN + REG_8218D_PTP_OFFSET_PORT(port), &nsec_l)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }
    if ((ret = _phy_8218d_ptpReg_get(unit, port, \
        REG_8218D_PTP_TIME_NSEC_H_PN + REG_8218D_PTP_OFFSET_PORT(port), &nsec_h)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    /* Clear the pending status */
    reg_addr = REG_8218D_PTP_TIME_CTRL_PN + REG_8218D_PTP_OFFSET_PORT(port);
    reg_val = 0x1 << (REG_8218D_PTP_OFFSET_MSGTYPE(identifier.msgType));
    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    pTimeStamp->sec = ((uint64)sec_h << 32) | ((uint64)sec_m << 16) | ((uint64)sec_l & 0xFFFF);
    pTimeStamp->nsec = ((nsec_h & 0x7FF) << 16) | (nsec_l & 0xFFFF);

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpIgrTpid_get
 * Description:
 *      Get inner/outer TPID of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      type  -  vlan type
 *      tpid_idx - TPID index
 * Output:
 *      pTpid   - TPID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT         - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
extern int32
phy_8218d_ptpIgrTpid_get(uint32 unit, rtk_port_t port, rtk_vlanType_t type, uint32 tpid_idx, uint32 *pTpid)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    if (INNER_VLAN == type)
    {
        switch(tpid_idx)
        {
            case 0:
                reg_addr = REG_8218D_PTP_ITAG_TPID;
                break;
            case 1:
                reg_addr = REG_8218D_PTP_ITAG_TPID_1;
                break;
            case 2:
                reg_addr = REG_8218D_PTP_ITAG_TPID_2;
                break;
            case 3:
                reg_addr = REG_8218D_PTP_ITAG_TPID_3;
                break;
           default:
                reg_addr = REG_8218D_PTP_ITAG_TPID;
                break;
        }
    }
    else
    {
        switch(tpid_idx)
        {
            case 0:
                reg_addr = REG_8218D_PTP_OTAG_TPID;
                break;
            case 1:
                reg_addr = REG_8218D_PTP_OTAG_TPID_1;
                break;
            case 2:
                reg_addr = REG_8218D_PTP_OTAG_TPID_2;
                break;
            case 3:
                reg_addr = REG_8218D_PTP_OTAG_TPID_3;
                break;
           default:
                reg_addr = REG_8218D_PTP_OTAG_TPID;
                break;
        }
    }

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    *pTpid = reg_val;

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpIgrTpid_set
 * Description:
 *      Set inner/outer TPID of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      type  -  vlan type
 *      tpid_idx - TPID index
 *      tpid   - TPID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT     - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
extern int32
phy_8218d_ptpIgrTpid_set(uint32 unit, rtk_port_t port, rtk_vlanType_t type, uint32 tpid_idx, uint32 tpid)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    if (INNER_VLAN == type)
    {
        switch(tpid_idx)
        {
            case 0:
                reg_addr = REG_8218D_PTP_ITAG_TPID;
                break;
            case 1:
                reg_addr = REG_8218D_PTP_ITAG_TPID_1;
                break;
            case 2:
                reg_addr = REG_8218D_PTP_ITAG_TPID_2;
                break;
            case 3:
                reg_addr = REG_8218D_PTP_ITAG_TPID_3;
                break;
           default:
                reg_addr = REG_8218D_PTP_ITAG_TPID;
                break;
        }
    }
    else
    {
        switch(tpid_idx)
        {
            case 0:
                reg_addr = REG_8218D_PTP_OTAG_TPID;
                break;
            case 1:
                reg_addr = REG_8218D_PTP_OTAG_TPID_1;
                break;
            case 2:
                reg_addr = REG_8218D_PTP_OTAG_TPID_2;
                break;
            case 3:
                reg_addr = REG_8218D_PTP_OTAG_TPID_3;
                break;
           default:
                reg_addr = REG_8218D_PTP_OTAG_TPID;
                break;
        }
    }

    reg_val = tpid;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, 0xFFFF)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpSwitchMacRange_get
 * Description:
 *      Get MAC address range of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pRange - pointer to MAC address range
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT         - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_8218d_ptpSwitchMacRange_get(uint32 unit, rtk_port_t port, uint32 *pRange)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_TIME_CFG_0;

    if ((ret = _phy_8218d_ptpReg_get(unit, port, reg_addr, &reg_val)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }

    *pRange =  (reg_val >> 6) & 0x3FF;

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_ptpSwitchMacRange_set
 * Description:
 *      Set MAC address range of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      pMac  -  pointer to MAC address
 *      range - MAC address range
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT     - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_8218d_ptpSwitchMacRange_set(uint32 unit, rtk_port_t port, uint32 range)
{
    int32 ret;
    uint32 reg_addr, reg_val;

    reg_addr = REG_8218D_PTP_TIME_CFG_0;

    reg_val =  range << 6;

    if ((ret = _phy_8218d_ptpReg_set(unit, port, reg_addr, reg_val, REG_FIELD_8218D_PTP_MAC_RNG_MSK)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, (MOD_HAL), "");
        return ret;
    }


    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218d_patch_set
 * Description:
 *      Set patch to PHY.
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_8218d_patch_set(uint32 unit, rtk_port_t port)
{
    int32 ret;
    ret = sub_phy_8218d_patch_set(unit, port);
    return ret;
} /* end of phy_8218d_patch_set */

#endif /* !defined(__BOOTLOADER__) */

int32 _phy_8218d_powerSavingMode_leave(uint32 unit, rtk_port_t port)
{
    int32  ret;
    uint32 phyData;
    uint32 org_phyData;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &org_phyData)) != RT_ERR_OK)
        return ret;

    phyData = org_phyData | (Reset_MASK);

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, org_phyData)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
}
/* Function Name:
 *      phy_8218d_enable_set
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
phy_8218d_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32   ret;

    if (ENABLED == enable)
    {
        if ((ret = _phy_8218d_powerSavingMode_leave(unit, port)) != RT_ERR_OK)
              return ret;
    }

    if ((ret = phy_common_enable_set(unit, port, enable)) != RT_ERR_OK)
        return ret;


    return RT_ERR_OK;
} /* end of phy_common_enable_set */

void
_phy_8218d_ieeeTestModeAllPhyPort_disable(uint32 unit, rtk_port_t phyBasePort, uint32 phyPortCnt)
{
    uint32      phyPort;

    for (phyPort = 0; phyPort < phyPortCnt; phyPort++)
    {
        hal_miim_write(unit, phyBasePort+phyPort,0xA41,9,0x0E00);  // Disable Test mode
    }
}

/* Function Name:
 *      phy_8218d_ieeeTestMode_set
 * Description:
 *      Set test mode for Giga PHY transmitter test
 * Input:
 *      unit  - unit id
 *      port  - port id
 *      pTestMode->mode  - test mode 1 ~ 4 which is defined in IEEE 40.6.1.1.2
 *      pTestMode->channel  - Channel A, B, C, D, or none
 *      pTestMode->flags -
 *          RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS -
 *              apply the test on all ports of the PHY.
 *              To use this feature, the "port" parameter shall set to the first port of the PHY.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_NOT_ALLOWED - The operation is not allowed
 *      RT_ERR_PORT_NOT_SUPPORTED - test mode is not supported
 * Note:
 *      RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS only support in mode 4
 */
int32
phy_8218d_ieeeTestMode_set(uint32 unit, rtk_port_t port, rtk_port_phyTestMode_t *pTestMode)
{
    uint32      phyBasePort = HWP_PHY_BASE_MACID(unit, port);
    uint32      phyPortCnt = 0;
    uint32      val, chn;

    osal_printf("RTL8218D Test Mode (PHYID: %u)\n", port);

    if ((pTestMode->flags & RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS) && (pTestMode->mode != 4))
    {
        osal_printf("mode %u not support all-phy-port operation.\n", pTestMode->mode);
        return RT_ERR_NOT_ALLOWED;
    }

    switch (HWP_PHY_MODEL_BY_PORT(unit, port))
    {
      case RTK_PHYTYPE_RTL8218D:
      case RTK_PHYTYPE_RTL8218D_NMP:
        phyPortCnt = PORT_NUM_IN_8218D;
        break;
      default:
        osal_printf("port type not support.\n");
        return RT_ERR_PORT_NOT_SUPPORTED;
    }

    switch (pTestMode->mode)
    {
      case 1:
        {
            _phy_8218d_ieeeTestModeAllPhyPort_disable(unit, phyBasePort, phyPortCnt);

            hal_miim_write(unit, port,0xA41,9,0x2E00); // Test mode 1
            return RT_ERR_OK;
        }
        break;

      case 2:
        {
            _phy_8218d_ieeeTestModeAllPhyPort_disable(unit, phyBasePort, phyPortCnt);

            hal_miim_write(unit, port,0xA41,9,0x4E00);// Test mode 2
            return RT_ERR_OK;
        }
        break;

    case 4:
        {
            uint32      phyPort;

            //0=A,1=B,2=C,3=D
            chn = pTestMode->channel - PHY_TESTMODE_CHANNEL_A;
            if (chn > 3)
            {
                osal_printf(" Error test channel: %u\n", chn);
                return RT_ERR_INPUT;
            }

            osal_printf(" test channel: %u\n", chn);

            if (!(pTestMode->flags & RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS))
            {
                /* single port test */
                for (phyPort = 0; phyPort < phyPortCnt; phyPort++)
                {
                    hal_miim_write(unit, phyBasePort+phyPort,0xBC0,18,0x0000);
                    hal_miim_write(unit, phyBasePort+phyPort,0xBC0,19,0x01C0);
                    hal_miim_write(unit, phyBasePort+phyPort,0xA41,9,0x0E00);// Disable Test mode
                }

                val = 0x1 << chn;
                hal_miim_write(unit, port,0xBC0,18, val);
                hal_miim_write(unit, port,0xBC0,19,0x01D0);/*Enable get power control*/
                hal_miim_write(unit, port,0xA41,9,0x8E00);/*Enable Test Mode 4*/

                return RT_ERR_OK;
            }
            else
            {
                osal_printf("All phy is not be suppoted yet.\n");
                return RT_ERR_PORT_NOT_SUPPORTED;
            }
        }
        break;

    default:
        osal_printf("The mode (%u) is not be suppoted yet.\n", pTestMode->mode);
        return RT_ERR_PORT_NOT_SUPPORTED;
    }

}

/* Function Name:
 *      phy_8218d_chipRevId_get
 * Description:
 *      Get chip revision ID
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pChip_rev_id - chip revision ID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_8218d_chipRevId_get(uint32 unit, rtk_port_t port, uint16 *chip_rev_id)
{
    uint32              basePort, blk;
    int32               ret = RT_ERR_OK;
    uint32              data;

    basePort = HWP_PHY_BASE_MACID(unit, port);
    if ((ret = hal_miim_read(unit, basePort, 0, 30, &blk)) != RT_ERR_OK)
    {
        return ret;
    }

    hal_miim_write(unit, basePort, 0, 30, 8);
    hal_miim_read(unit, basePort, 0x327, 0x15, &data);
    hal_miim_write(unit, basePort, 0, 30, blk);

    *chip_rev_id = (uint16) REG32_FIELD_GET(data, 0, 0x3F);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8218Ddrv_ge_mapperInit
 * Description:
 *      Initialize PHY 8218D driver.
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
phy_8218Ddrv_ge_mapperInit(rt_phydrv_t *pPhydrv)
{
    pPhydrv->phydrv_index = RT_PHYDRV_RTL8218D;
    pPhydrv->fPhydrv_init = phy_8218d_init;
    pPhydrv->fPhydrv_enable_set = phy_8218d_enable_set;
    pPhydrv->fPhydrv_chipRevId_get = phy_8218d_chipRevId_get;
    pPhydrv->fPhydrv_ieeeTestMode_set = phy_8218d_ieeeTestMode_set;
#if !defined(__BOOTLOADER__)
    pPhydrv->fPhydrv_media_get = phy_8218d_media_get;
    pPhydrv->fPhydrv_autoNegoEnable_get = phy_common_autoNegoEnable_get;
    pPhydrv->fPhydrv_autoNegoEnable_set = phy_common_autoNegoEnable_set;
    pPhydrv->fPhydrv_autoNegoAbility_get = phy_8218d_autoNegoAbility_get;
    pPhydrv->fPhydrv_autoNegoAbility_set = phy_8218d_autoNegoAbility_set;
    pPhydrv->fPhydrv_duplex_get = phy_common_duplex_get;
    pPhydrv->fPhydrv_duplex_set = phy_common_duplex_set;
    pPhydrv->fPhydrv_speed_get = phy_8218d_speed_get;
    pPhydrv->fPhydrv_speed_set = phy_8218d_speed_set;
    pPhydrv->fPhydrv_greenEnable_get = phy_8218d_greenEnable_get;
    pPhydrv->fPhydrv_greenEnable_set = phy_8218d_greenEnable_set;
    pPhydrv->fPhydrv_eeeEnable_get = phy_8218d_eeeEnable_get;
    pPhydrv->fPhydrv_eeeEnable_set = phy_8218d_eeeEnable_set;
    pPhydrv->fPhydrv_crossOverMode_get = phy_8218d_crossOverMode_get;
    pPhydrv->fPhydrv_crossOverMode_set = phy_8218d_crossOverMode_set;
    pPhydrv->fPhydrv_crossOverStatus_get = phy_8218d_crossOverStatus_get;
    pPhydrv->fPhydrv_linkDownPowerSavingEnable_get = phy_8218d_linkDownPowerSavingEnable_get;
    pPhydrv->fPhydrv_linkDownPowerSavingEnable_set = phy_8218d_linkDownPowerSavingEnable_set;
    pPhydrv->fPhydrv_broadcastEnable_set = phy_8218d_broadcastEnable_set;
    pPhydrv->fPhydrv_broadcastID_set = phy_8218d_broadcastID_set;
    pPhydrv->fPhydrv_gigaLiteEnable_get = phy_8218d_gigaLiteEnable_get;
    pPhydrv->fPhydrv_gigaLiteEnable_set = phy_8218d_gigaLiteEnable_set;
    pPhydrv->fPhydrv_patch_set = phy_8218d_patch_set;
    pPhydrv->fPhydrv_downSpeedEnable_get = phy_8218d_downSpeedEnable_get;
    pPhydrv->fPhydrv_downSpeedEnable_set = phy_8218d_downSpeedEnable_set;
    pPhydrv->fPhydrv_ptpSwitchMacAddr_get = phy_8218d_ptpSwitchMacAddr_get;
    pPhydrv->fPhydrv_ptpSwitchMacAddr_set = phy_8218d_ptpSwitchMacAddr_set;
    pPhydrv->fPhydrv_ptpRefTime_get = phy_8218d_ptpRefTime_get;
    pPhydrv->fPhydrv_ptpRefTime_set = phy_8218d_ptpRefTime_set;
    pPhydrv->fPhydrv_ptpRefTimeAdjust_set = phy_8218d_ptpRefTimeAdjust_set;
    pPhydrv->fPhydrv_ptpRefTimeEnable_get = phy_8218d_ptpRefTimeEnable_get;
    pPhydrv->fPhydrv_ptpRefTimeEnable_set = phy_8218d_ptpRefTimeEnable_set;
    pPhydrv->fPhydrv_ptpRefTimeFreq_get = phy_8218d_ptpRefTimeFreq_get;
    pPhydrv->fPhydrv_ptpRefTimeFreq_set = phy_8218d_ptpRefTimeFreq_set;
    pPhydrv->fPhydrv_ptpEnable_get = phy_8218d_ptpEnable_get;
    pPhydrv->fPhydrv_ptpEnable_set = phy_8218d_ptpEnable_set;
    pPhydrv->fPhydrv_ptpInterruptStatus_get = phy_8218d_ptpInterruptStatus_get;
    pPhydrv->fPhydrv_ptpInterruptEnable_get = phy_8218d_ptpInterruptEnable_get;
    pPhydrv->fPhydrv_ptpInterruptEnable_set = phy_8218d_ptpInterruptEnable_set;
    pPhydrv->fPhydrv_ptpIgrTpid_get = phy_8218d_ptpIgrTpid_get;
    pPhydrv->fPhydrv_ptpIgrTpid_set = phy_8218d_ptpIgrTpid_set;
    pPhydrv->fPhydrv_ptpSwitchMacRange_get = phy_8218d_ptpSwitchMacRange_get;
    pPhydrv->fPhydrv_ptpSwitchMacRange_set = phy_8218d_ptpSwitchMacRange_set;
    pPhydrv->fPhydrv_ptpRxTimestamp_get = phy_8218d_ptpRxTimestamp_get;
    pPhydrv->fPhydrv_ptpTxTimestamp_get = phy_8218d_ptpTxTimestamp_get;
    pPhydrv->fPhydrv_reg_get = phy_common_reg_get;
    pPhydrv->fPhydrv_reg_set = phy_common_reg_set;
    pPhydrv->fPhydrv_reg_park_get = phy_common_reg_park_get;
    pPhydrv->fPhydrv_reg_park_set = phy_common_reg_park_set;
    pPhydrv->fPhydrv_reg_mmd_get = phy_common_reg_mmd_get;
    pPhydrv->fPhydrv_reg_mmd_set = phy_common_reg_mmd_set;
    pPhydrv->fPhydrv_reg_mmd_portmask_set = phy_common_reg_mmd_portmask_set;
    pPhydrv->fPhydrv_reg_extParkPage_get = phy_common_reg_extParkPage_get;
    pPhydrv->fPhydrv_reg_extParkPage_set = phy_common_reg_extParkPage_set;
    pPhydrv->fPhydrv_reg_extParkPage_portmask_set = phy_common_reg_extParkPage_portmask_set;
    pPhydrv->fPhydrv_masterSlave_get = phy_common_masterSlave_get;
    pPhydrv->fPhydrv_masterSlave_set = phy_common_masterSlave_set;
    pPhydrv->fPhydrv_loopback_get = phy_common_loopback_get;
    pPhydrv->fPhydrv_loopback_set = phy_common_loopback_set;
#endif
} /* end of phy_8218Bdrv_ge_mapperInit*/


