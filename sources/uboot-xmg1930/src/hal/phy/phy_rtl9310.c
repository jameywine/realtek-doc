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
 * $Revision: 94760 $
 * $Date: 2019-01-10 19:43:37 +0800 (Thu, 10 Jan 2019) $
 *
 * Purpose : PHY 9310 intra serdes Driver APIs.
 *
 * Feature : PHY 9310 intra serdes Driver APIs
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>

#include <hal/common/halctrl.h>
#include <hal/mac/miim_common_drv.h>
#include <hal/phy/phydef.h>
#include <hal/phy/phy_common.h>
#include <hal/phy/phy_rtl9310.h>
#include <soc/type.h>

#include <hal/mac/reg.h>
#include <hal/mac/serdes.h>
#include <hal/mac/drv/drv_rtl9310.h>
#include <hal/chipdef/mango/rtk_mango_reg_struct.h>

#include <dal/dal_construct.h>
#include <rtk/customer/param/conf_rtl9310_sds_cust.c>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
static uint32               phy_rtl9310_init_flag[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};
static phy_osal_mutex_t     phy_rtl9310_sem[RTK_MAX_NUM_OF_UNIT];
static rtk_port_10gMedia_t  port10gMedia[RTK_MAX_NUM_OF_UNIT][RTK_MAX_NUM_OF_PORTS] = {{0}};
static rt_serdesMode_t      sdsMode[RTK_MAX_NUM_OF_UNIT][RTK_MAX_SDS_PER_UNIT] = {{RTK_MII_NONE}};
static rtk_bitmap_t         sdsAdminEnable[RTK_MAX_NUM_OF_UNIT][RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_SERDES_LIST] = {{0}};
static rtk_bitmap_t         waMonRxCalied[RTK_MAX_NUM_OF_UNIT][RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_SERDES_LIST] = {{0}};
static uint32               phy_rtl9310_10g_tx[RTK_MAX_NUM_OF_UNIT][RTK_MAX_SDS_PER_UNIT] = {{0}};

/* calibrate */
static rtk_bitmap_t         sdsCaliEn[RTK_MAX_NUM_OF_UNIT][RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_SERDES_LIST] = {{0}};
static rtk_enable_t         sdsCaliDbg = DISABLED;
static rtk_sds_rxCaliConf_t rxCaliConf[RTK_MAX_NUM_OF_UNIT][RTK_MAX_SDS_PER_UNIT];

rt_phyInfo_t phy_rtl9310sds_info =
{
    .phy_num    = PORT_NUM_IN_9310SDS,
    .eth_type   = HWP_XGE,
    .isComboPhy = {0, 0, 0, 0, 0, 0, 0, 0},
};


/*
 * Macro Definition
 */
#define PHY_RTL9310_SEM_LOCK(unit)    \
do {\
    if (INIT_COMPLETED == phy_rtl9310_init_flag[unit])   \
    {\
        if (phy_osal_sem_mutex_take(phy_rtl9310_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_HAL|MOD_SDS), "semaphore lock failed");\
            return RT_ERR_SEM_LOCK_FAILED;\
        }\
    }\
} while(0)

#define PHY_RTL9310_SEM_UNLOCK(unit)   \
do {\
    if (INIT_COMPLETED == phy_rtl9310_init_flag[unit])   \
    {\
        if (phy_osal_sem_mutex_give(phy_rtl9310_sem[unit]) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_HAL|MOD_SDS), "semaphore unlock failed");\
            return RT_ERR_SEM_UNLOCK_FAILED;\
        }\
    }\
} while(0)

#define PHY_SDS_DBG(fmt, args...)                           \
do {                                                        \
    if (DISABLED == sdsCaliDbg) {                           \
        RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), fmt, ##args);  \
    } else {                                                \
        osal_printf(fmt, ##args);                           \
    }                                                       \
} while(0)

/*
 * Function Declaration
 */

int32 _phy_rtl9310_10gMedia_get(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t *pMedia);

/* Function Name:
 *      _phy_rtl9310_linkSts_get
 * Description:
 *      Get the SerDes link status.
 * Input:
 *      unit      - unit id
 *      sds       - user SerDes id
 * Output:
 *      info      - link status information
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
_phy_rtl9310_linkSts_get(uint32 unit, uint32 sds, rtk_sds_linkSts_t *info)
{
    int32   ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK(!HWP_SDS_EXIST(unit, sds), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(NULL == info, RT_ERR_NULL_POINTER);

    /* function body */
    if (RTK_MII_XSGMII == HWP_SDS_MODE(unit, sds))
    {
        uint32  xsg_sdsid_0, xsg_sdsid_1;

        RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &xsg_sdsid_0), ret);
        xsg_sdsid_1 = xsg_sdsid_0 + 1;

        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_0, 0x1, 29, 8, 0, &info->sts), ret);
        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_1, 0x1, 29, 8, 0, &info->sts1), ret);
        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_0, 0x1, 30, 8, 0, &info->latch_sts), ret);
        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_1, 0x1, 30, 8, 0, &info->latch_sts1), ret);
    }
    else
    {
        uint32  asds, dsds;

        RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);
        RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x5, 0, 12, 12, &info->sts), ret);
        RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x4, 1, 2, 2, &info->latch_sts), ret);

        RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dsds), ret);
        RT_ERR_CHK(SDS_FIELD_R(unit, dsds, 0x2, 1, 2, 2, &info->latch_sts1), ret);
        RT_ERR_CHK(SDS_FIELD_R(unit, dsds, 0x2, 1, 2, 2, &info->sts1), ret);
    }

    return ret;
}   /* end of _phy_rtl9310_linkSts_get */

/* Function Name:
 *      phy_rtl9310_linkSts_get
 * Description:
 *      Get the SerDes link status.
 * Input:
 *      unit      - unit id
 *      sds       - user SerDes id
 * Output:
 *      info      - link status information
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_rtl9310_linkSts_get(uint32 unit, uint32 sds, rtk_sds_linkSts_t *info)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_linkSts_get(unit, sds, info), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_linkSts_get */

int32
_phy_rtl9310_rx_rst(uint32 unit, uint32 sds)
{
    return hal_mac_serdes_rst(unit, sds);
}   /* end of _phy_rtl9310_rx_rst */

/* Function Name:
 *      phy_rtl9310_rx_rst
 * Description:
 *      Reset Serdes Rx and original patch are kept.
 * Input:
 *      unit    - unit id
 *      sds     - user serdes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_OUT_OF_RANGE - Serdes index is not support.
 * Note:
 *      None
 */
int32
phy_rtl9310_rx_rst(uint32  unit, uint32 sds)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_rx_rst(unit, sds), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_rx_rst */

int32
_phy_rtl9310_sds_rst(uint32 unit, uint32 sds)
{
    uint32  oriVal = 0, val = 0, ori = 0;
    int32   ret;

    RT_ERR_CHK(reg_read(unit, MANGO_PS_SERDES_OFF_MODE_CTRLr, &ori), ret);
    val = ori | (1 << sds);
    RT_ERR_CHK(reg_write(unit, MANGO_PS_SERDES_OFF_MODE_CTRLr, &val), ret);

    RT_ERR_CHK(reg_array_field_read(unit, MANGO_SERDES_MODE_CTRLr,
            REG_ARRAY_INDEX_NONE, sds, MANGO_SERDES_IFf, &oriVal), ret);

    val = (1 << 7) | 0x1F;
    RT_ERR_CHK(reg_array_field_write(unit, MANGO_SERDES_MODE_CTRLr,
            REG_ARRAY_INDEX_NONE, sds, MANGO_SERDES_IFf, &val), ret);

    RT_ERR_CHK(reg_array_field_write(unit, MANGO_SERDES_MODE_CTRLr,
            REG_ARRAY_INDEX_NONE, sds, MANGO_SERDES_IFf, &oriVal), ret);

    RT_ERR_CHK(reg_write(unit, MANGO_PS_SERDES_OFF_MODE_CTRLr, &ori), ret);

    return RT_ERR_OK;
}   /* end of _phy_rtl9310_sds_rst */

/* Function Name:
 *      phy_rtl9310_sds_rst
 * Description:
 *      Reset Serdes.
 * Input:
 *      unit    - unit id
 *      sds     - user serdes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_OUT_OF_RANGE - Serdes index is not support.
 * Note:
 *      None
 */
int32
phy_rtl9310_sds_rst(uint32  unit, uint32 sds)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_sds_rst(unit, sds), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sds_rst */

/* Function Name:
 *      _phy_rtl9310_xsgmii_symErr_get
 * Description:
 *      Get the SerDes XSGMII symbol error count.
 * Input:
 *      unit      - unit id
 *      sds       - user SerDes id
 * Output:
 *      info      - symbol error count information
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
_phy_rtl9310_xsgmii_symErr_get(uint32 unit, uint32 sds, rtk_sds_symErr_t *info)
{
    uint32  xsg_sdsid_0, xsg_sdsid_1;
    uint32  val, i;
    int32   ret;

    RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &xsg_sdsid_0), ret);
    xsg_sdsid_1 = xsg_sdsid_0 + 1;

    RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_0, 0x1, 1, 15, 8, &val), ret);
    info->all[0] = val << 16;
    RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_0, 0x1, 0, 15, 0, &val), ret);
    info->all[0] |= val;

    RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_1, 0x1, 1, 15, 8, &val), ret);
    info->all[1] = val << 16;
    RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_1, 0x1, 0, 15, 0, &val), ret);
    info->all[1] |= val;

    for (i = 0; i < 4; ++i)
    {
        RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_0, 0x1, 24, 2, 0, i), ret);

        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_0, 0x1, 3, 15, 8, &val), ret);
        info->ch[i] = val << 16;
        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_0, 0x1, 2, 15, 0, &val), ret);
        info->ch[i] |= val;
    }

    for (i = 0; i < 4; ++i)
    {
        RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_1, 0x1, 24, 2, 0, i), ret);

        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_1, 0x1, 3, 15, 8, &val), ret);
        info->ch[i + 4] = val << 16;
        RT_ERR_CHK(SDS_FIELD_R(unit, xsg_sdsid_1, 0x1, 2, 15, 0, &val), ret);
        info->ch[i + 4] |= val;
    }

    return ret;
}   /* end of _phy_rtl9310_xsgmii_symErr_get */

/* Function Name:
 *      _phy_rtl9310_10gr_symErr_get
 * Description:
 *      Get the SerDes 10G-R symbol error count.
 * Input:
 *      unit      - unit id
 *      sds       - user SerDes id
 * Output:
 *      info      - symbol error count information
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
_phy_rtl9310_10gr_symErr_get(uint32 unit, uint32 sds, rtk_sds_symErr_t *info)
{
    uint32  aSds, val;
    int32   ret;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &aSds), ret);
    RT_ERR_CHK(SDS_FIELD_R(unit, aSds, 0x5, 1, 7, 0, &val), ret);
    info->ch[0] = val;

    return ret;
}   /* end of _phy_rtl9310_10gr_symErr_get */

int32
_phy_rtl9310_symErr_get(uint32 unit, uint32 sds, rtk_sds_symErr_t *info)
{
    int32   ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK(!HWP_SDS_EXIST(unit, sds), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(NULL == info, RT_ERR_NULL_POINTER);

    /* function body */
    switch (HWP_SDS_MODE(unit, sds))
    {
        case RTK_MII_DISABLE:
            break;
        case RTK_MII_XSGMII:
            RT_ERR_CHK(_phy_rtl9310_xsgmii_symErr_get(unit, sds, info), ret);
            break;
        default:
            RT_ERR_CHK(_phy_rtl9310_10gr_symErr_get(unit, sds, info), ret);
    }

    return ret;
}   /* end of _phy_rtl9310_symErr_get */

/* Function Name:
 *      phy_rtl9310_symErr_get
 * Description:
 *      Get the SerDes symbol error count.
 * Input:
 *      unit      - unit id
 *      sds       - user SerDes id
 * Output:
 *      info      - symbol error count information
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_rtl9310_symErr_get(uint32 unit, uint32 sds, rtk_sds_symErr_t *info)
{
    int32   ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK(!HWP_SDS_EXIST(unit, sds), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(NULL == info, RT_ERR_NULL_POINTER);

    /* function body */
    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_symErr_get(unit, sds, info), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_symErr_get */

int32
_phy_rtl9310_symErr_clear(uint32 unit, uint32 sds)
{
    rtk_sds_symErr_t    info;
    uint32              i;
    uint32              xsg_sdsid_0, xsg_sdsid_1;
    int32               ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK(!HWP_SDS_EXIST(unit, sds), RT_ERR_OUT_OF_RANGE);

    /* function body */
    switch (HWP_SDS_MODE(unit, sds))
    {
        case RTK_MII_DISABLE:
            break;
        case RTK_MII_XSGMII:
            RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &xsg_sdsid_0), ret);
            xsg_sdsid_1 = xsg_sdsid_0 + 1;

            for (i = 0; i < 4; ++i)
            {
                RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_0, 0x1, 24, 2, 0, i), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_0, 0x1, 3, 15, 8, 0x0), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_0, 0x1, 2, 15, 0, 0x0), ret);
            }

            for (i = 0; i < 4; ++i)
            {
                RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_1, 0x1, 24, 2, 0, i), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_1, 0x1, 3, 15, 8, 0x0), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_1, 0x1, 2, 15, 0, 0x0), ret);
            }

            RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_0, 0x1, 0, 15, 0, 0x0), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_0, 0x1, 1, 15, 8, 0x0), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_1, 0x1, 0, 15, 0, 0x0), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, xsg_sdsid_1, 0x1, 1, 15, 8, 0x0), ret);
            break;
        default:
            RT_ERR_CHK(_phy_rtl9310_10gr_symErr_get(unit, sds, &info), ret);
            break;
    }

    return ret;
}   /* end of _phy_rtl9310_symErr_clear */

/* Function Name:
 *      phy_rtl9310_symErr_clear
 * Description:
 *      Clear the SerDes symbol error count.
 * Input:
 *      unit      - unit id
 *      sds       - user SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_rtl9310_symErr_clear(uint32 unit, uint32 sds)
{
    int32   ret;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK(!HWP_SDS_EXIST(unit, sds), RT_ERR_OUT_OF_RANGE);

    /* function body */
    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_symErr_clear(unit, sds), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_symErr_clear */

/* Function Name:
 *      _phy_rtl9310_sds_mode_get
 * Description:
 *      Get 9310 serdes current mode.
 * Input:
 *      unit     - unit id
 *      sdsId   - serdes id
 *      mode   - serdes mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_INPUT
 *      RT_ERR_CHIP_NOT_SUPPORTED
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32 _phy_rtl9310_sds_mode_get(uint32 unit, uint32 sdsId, rt_serdesMode_t *mode)
{
    *mode = sdsMode[unit][sdsId];

    return RT_ERR_OK;
}   /* end of _phy_rtl9310_sds_mode_get */

int32
_phy_rtl9310_sds_mii_mode_set(uint32 unit, uint32 sds, rt_serdesMode_t mode)
{
    uint32  val;
    int32   ret;

    switch (mode)
    {
        case RTK_MII_DISABLE:
            /* serdes off */
            val = 0x1f;
            break;
        case RTK_MII_QSGMII:
            /* serdes mode QSGMII */
            val = 0x6;
            break;
        case RTK_MII_XSGMII:
            /* serdes mode XSGMII */
            val = 0x10;
            break;
        //case RTK_MII_USXGMII:
        case RTK_MII_USXGMII_10GSXGMII:
        case RTK_MII_USXGMII_10GDXGMII:
        case RTK_MII_USXGMII_10GQXGMII:
        case RTK_MII_USXGMII_5GSXGMII:
        case RTK_MII_USXGMII_5GDXGMII:
        case RTK_MII_USXGMII_2_5GSXGMII:
            val = 0xD;
            break;
        case RTK_MII_HISGMII:
            val = 0x12;
            break;
        case RTK_MII_XSMII:
            val = 0x9;
            break;
        case RTK_MII_SGMII:
            val = 0x2;
            break;
        default:
            return RT_ERR_INPUT;
    }

    val |= (1 << 7);
    RT_ERR_CHK(reg_array_field_write(unit, MANGO_SERDES_MODE_CTRLr, \
            REG_ARRAY_INDEX_NONE, sds, MANGO_SERDES_IFf, &val), ret);

    return ret;
}   /* end of _phy_rtl9310_sds_mii_mode_set */

int32 _phy_rtl9310_sds_fiber_mode_set(uint32 unit, uint32 sds, rt_serdesMode_t mode)
{
    uint32  val, asds;
    int32   ret;

    /* clear symbol error count before change mode */
    RT_ERR_CHK(_phy_rtl9310_symErr_clear(unit, sds), ret);

    val = 0x9F;
    RT_ERR_CHK(reg_array_field_write(unit, MANGO_SERDES_MODE_CTRLr, \
            REG_ARRAY_INDEX_NONE, sds, MANGO_SERDES_IFf, &val), ret);

    switch (mode)
    {
        case RTK_MII_SGMII:
            val = 0x5;
            break;
        case RTK_MII_1000BX_FIBER:
            /* serdes mode FIBER1G */
            val = 0x9;
            break;
        case RTK_MII_10GR:
            /* serdes mode 10G Base-R */
            val = 0x35;
            break;
        case RTK_MII_10GR1000BX_AUTO:
            val = 0x39;
            break;
        case RTK_MII_DISABLE:
            /* serdes off */
            val = 0x3F;
            break;
        default:
            return RT_ERR_INPUT;
    }

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);
    SDS_FIELD_W(unit, asds, 0x1F, 0x9, 11, 6, val);

    return ret;
}   /* end of _phy_rtl9310_sds_fiber_mode_set */

int32
_phy_rtl9310_sds_mode_set(uint32 unit, uint32 sds, rt_serdesMode_t mode)
{
    int32   ret;

    if (RTK_MII_XSGMII == HWP_SDS_MODE(unit, sds))
    {
        RT_ERR_CHK(_phy_rtl9310_sds_mii_mode_set(unit, sds, mode), ret);
    }
    else
    {
        RT_ERR_CHK(_phy_rtl9310_sds_fiber_mode_set(unit, sds, mode), ret);
    }

    if (RTK_MII_DISABLE != mode)
        sdsMode[unit][sds] = mode;

    return ret;
}   /* end of _phy_rtl9310_sds_mode_set */

/* Function Name:
 *      phy_rtl9310_sds_mode_set
 * Description:
 *      Set 9310 serdes mode.
 * Input:
 *      unit     - unit id
 *      sdsId   - serdes id
 *      mode   - serdes mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_INPUT
 *      RT_ERR_CHIP_NOT_SUPPORTED
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_sds_mode_set(uint32 unit, uint32 sdsId, rt_serdesMode_t mode)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_sds_mode_set(unit, sdsId, mode), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sds_mode_set */

/* Function Name:
 *      _phy_rtl9310_dbg_set
 * Description:
 *      Set SerDes debug
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 * Output:
 *      leq - LEQ value
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      No semaphore
 */
static int32
_phy_rtl9310_dbg_set(uint32 unit, uint32 sds, uint32 dbg_sel)
{
    uint32  asds, dbg_sel_rx = 75;
    uint32  evenSds = sds - (sds % 2);
    int32   ret;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, evenSds, &asds), ret);

    dbg_sel_rx += (sds % 2);
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x1f, 0x02, dbg_sel_rx), ret);

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x21, 0x0, 2, 2, 1), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x15, 11, 10, dbg_sel), ret);

    return RT_ERR_OK;
}   /* end of _phy_rtl9310_dbg_set */

int32
_phy_rtl9310_leq_get(uint32 unit, uint32 sds, uint32 *leq)
{
    uint32  asds;
    uint32  gray;
    int32   ret;

    RT_ERR_CHK(_phy_rtl9310_dbg_set(unit, sds, 0x1), ret);

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x1f, 0x14, 7, 3, &gray), ret);

    *leq = rt_util_grayToBinary(gray);

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "SDS%u leq %u\n", sds, *leq);

    return ret;
}   /* end of _phy_rtl9310_leq_get */

/* Function Name:
 *      phy_rtl9310_leq_get
 * Description:
 *      Get SerDes LEQ
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 * Output:
 *      leq - LEQ value
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      No semaphore
 */
int32
phy_rtl9310_leq_get(uint32 unit, uint32 sds, uint32 *leq)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_leq_get(unit, sds, leq), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_leq_get */

int32
_phy_rtl9310_leq_set(uint32 unit, uint32 sds, uint32 leq)
{
    uint32  asds;
    int32   ret;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x0d, 6, 2, leq), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x0d, 7, 7, 1), ret);

    return ret;
}   /* end of _phy_rtl9310_leq_set */

/* Function Name:
 *      phy_rtl9310_leq_set
 * Description:
 *      Set SerDes LEQ
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 *      leq  - LEQ value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      No semaphore
 */
int32
phy_rtl9310_leq_set(uint32 unit, uint32 sds, uint32 leq)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_leq_set(unit, sds, leq), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_leq_set */

int32
_phy_rtl9310_dfe_dis(uint32 unit, uint32 asds)
{
    int32   ret;

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2A, 0xf, 12, 6, 0x7f), ret);

    return ret;
}   /* end of _phy_rtl9310_dfe_dis */

/* Function Name:
 *      _phy_rtl9310_leq_adapt
 * Description:
 *      Set SerDes LEQ adapt
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
static int32
_phy_rtl9310_leq_adapt(uint32 unit, uint32 sds)
{
    uint32  asds;
    int32   ret;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK((!HWP_SDS_EXIST(unit, sds)), RT_ERR_OUT_OF_RANGE);

    /* function body */
    if (sds < 2)
        return RT_ERR_FAILED;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 6, 0, 0x0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 13, 13, 0x0), ret);
    _phy_rtl9310_dfe_dis(unit, asds);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 1), ret);
    RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);

    phy_osal_time_mdelay(10);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 0), ret);

    phy_osal_time_mdelay(100);

    return ret;
}   /* end of _phy_rtl9310_leq_adapt */

/* Function Name:
 *      _phy_rtl9310_dfe_dump
 * Description:
 *      Dump SerDes DFE
 * Input:
 *      unit - unit id
 *      asds - analog SerDes id
 *      dfe  - dfe info
 * Output:
 *      dfe  - def dump info
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
_phy_rtl9310_dfe_dump(uint32 unit, uint32 asds, phy_rtl9310_dfe_t *dfe)
{
    uint32  val;
    int32   ret;

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x14, 10, 5, dfe->coefNum), ret);
    RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x1f, 0x14, dfe->endBit, dfe->startBit, &val), ret);

    dfe->val = val;

    if (dfe->signBit == 32)
    {
        PHY_SDS_DBG(" %s = 0x%x\n", dfe->name, dfe->val);
    }
    else
    {
        PHY_SDS_DBG(" %s = ", dfe->name);

        val = val & ~(1 << dfe->signBit);
        if (1 == (dfe->val >> dfe->signBit))
        {
            PHY_SDS_DBG(" -");
        }
        PHY_SDS_DBG("%d\n", val);
    }

    return ret;
}   /* end of _phy_rtl9310_dfe_dump */

int32
_phy_rtl9310_dfe_set(uint32 unit, uint32 sds, phy_rtl9310_dfeType_t type, int32 val)
{
    int32   ret;

    switch (type)
    {
        case PHY_RTL9310_DFE_VTH:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2f, 0x12, 11, 4, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 12, 12, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP0:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x1c, 5, 5, 0), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x1c, 4, 0, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 6, 6, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP1EVEN:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x1d, 5, 0, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 7, 7, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP1ODD:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x1d, 11, 6, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 7, 7, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP2EVEN:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x1f, 5, 0, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 8, 8, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP2ODD:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x1f, 11, 6, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 8, 8, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP3EVEN:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2f, 0x0, 5, 0, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 9, 9, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP3ODD:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2f, 0x0, 11, 6, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 9, 9, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP4EVEN:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2f, 0x1, 5, 0, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 10, 10, 1), ret);
            break;
        case PHY_RTL9310_DFE_TAP4ODD:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2f, 0x1, 11, 6, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0xf, 10, 10, 1), ret);
            break;
        case PHY_RTL9310_DFE_FGCAL_OFST:
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x19, 14, 7, val), ret);
            RT_ERR_CHK(SDS_FIELD_W(unit, sds, 0x2e, 0x19, 6, 6, 1), ret);
            break;
        default:
            break;
    }

    return RT_ERR_OK;
}   /* end of _phy_rtl9310_dfe_set */

/* Function Name:
 *      phy_rtl9310_dfe_set
 * Description:
 *      Set SerDes DFE
 * Input:
 *      unit - unit id
 *      sds  - analog SerDes id
 *      type - DFE type
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
phy_rtl9310_dfe_set(uint32 unit, uint32 sds, phy_rtl9310_dfeType_t type, int32 val)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_dfe_set(unit, sds, type, val), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_dfe_set */

static int32
_phy_rtl9310_init_leq_dfe(uint32 unit, uint32 sds)
{
    uint32  asds;
    int32   ret;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 6, 0, 0x0), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x1c, 5, 0, 0x1E), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x1d, 11, 0, 0x00), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x1f, 11, 0, 0x00), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2f, 0x0, 11, 0, 0x00), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2f, 0x1, 11, 0, 0x00), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 12, 6, 0x7F), ret);
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x2f, 0x12, 0xaaa), ret);

    return ret;
}   /* end of _phy_rtl9310_init_leq_dfe */

/* Function Name:
 *      _phy_rtl9310_dfe_leq_adapt
 * Description:
 *      Set SerDes DFE LEQ adapt
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
static int32
_phy_rtl9310_dfe_leq_adapt(uint32 unit, uint32 sds, rtk_port_10gMedia_t media)
{
    rtk_sds_symErr_t    info;
    phy_rtl9310_dfe_t   sds_dfe[] =
        {
            {0x0f, 7, 0, 32, 0, PHY_RTL9310_DFE_END, "FGCAL_OFST_BIN"},
        };
    phy_rtl9310_dfe_t   sds_dfe2[] =
        {
            {0x00, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP0, "TAP0"},
            {0x0c, 7, 0, 32, 0, PHY_RTL9310_DFE_VTH, "VTH_BIN"},
        };
    phy_rtl9310_dfe_t   sds_dfe3[] =
        {
            {0x01, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP1EVEN, "TAP1_EVEN"},
            {0x06, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP1ODD, "TAP1_ODD"},
            {0x02, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP2EVEN, "TAP2_EVEN"},
            {0x07, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP2ODD, "TAP2_ODD"},
            {0x03, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP3EVEN, "TAP3_EVEN"},
            {0x08, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP3ODD, "TAP3_ODD"},
            {0x04, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP4EVEN, "TAP4_EVEN"},
            {0x09, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP4ODD, "TAP4_ODD"},
        };
    uint32  asds;
    uint32  i, leq;
    int32   ret;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK((!HWP_SDS_EXIST(unit, sds)), RT_ERR_OUT_OF_RANGE);

    /* function body */
    if (sds < 2)
        return RT_ERR_FAILED;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    PHY_SDS_DBG("SDS%d RX calibration...\n", sds);

    RT_ERR_CHK(_phy_rtl9310_init_leq_dfe(unit, sds), ret);

    RT_ERR_CHK(_phy_rtl9310_dbg_set(unit, sds, 0x2), ret);

    for (i = 0; i < sizeof(sds_dfe)/sizeof(phy_rtl9310_dfe_t); ++i)
    {
        RT_ERR_CHK(_phy_rtl9310_dfe_dump(unit, asds, &sds_dfe[i]), ret);
    }

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 13, 13, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 1), ret);

    RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);
    phy_osal_time_mdelay(10);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 0), ret);
    phy_osal_time_mdelay(100);

    RT_ERR_CHK(_phy_rtl9310_leq_get(unit, sds, &i), ret);
    PHY_SDS_DBG("SDS%u leq %u\n", sds, i);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 6, 6, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 12, 12, 0), ret);

    RT_ERR_CHK(_phy_rtl9310_dbg_set(unit, sds, 0x2), ret);

    for (i = 0; i < sizeof(sds_dfe2)/sizeof(phy_rtl9310_dfe_t); ++i)
    {
        RT_ERR_CHK(_phy_rtl9310_dfe_dump(unit, asds, &sds_dfe2[i]), ret);
    }

    for (i = 0; i < sizeof(sds_dfe2)/sizeof(phy_rtl9310_dfe_t); ++i)
    {
        if (sds_dfe2[i].type != PHY_RTL9310_DFE_END)
        {
            if (PHY_RTL9310_DFE_TAP0 == sds_dfe2[i].type)
                sds_dfe2[i].val = 31;

            RT_ERR_CHK(_phy_rtl9310_dfe_set(unit, asds, sds_dfe2[i].type, sds_dfe2[i].val), ret);
        }
    }

    if (DISABLED == rxCaliConf[unit][sds].leqAuto)
    {
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 1), ret);

        RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);
        phy_osal_time_mdelay(10);
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 0), ret);
        phy_osal_time_mdelay(100);

        RT_ERR_CHK(_phy_rtl9310_leq_get(unit, sds, &leq), ret);

        if (media >= PORT_10GMEDIA_DAC_300CM)
        {
            leq += rxCaliConf[unit][sds].ofst;
        }

        RT_ERR_CHK(_phy_rtl9310_leq_set(unit, sds, leq), ret);
    }

    RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);

    if (ENABLED == rxCaliConf[unit][sds].dfeTap1_4Enable)
    {
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 7, 7, 0), ret);
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 8, 8, 0), ret);
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 9, 9, 0), ret);
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 10, 10, 0), ret);
    }

    if (DISABLED == rxCaliConf[unit][sds].dfeTap1_4Enable ||
            DISABLED == rxCaliConf[unit][sds].dfeAuto)
    {
        for (i = 0; i < sizeof(sds_dfe3)/sizeof(phy_rtl9310_dfe_t); ++i)
        {
            RT_ERR_CHK(_phy_rtl9310_dfe_dump(unit, asds, &sds_dfe3[i]), ret);
        }

        for (i = 0; i < sizeof(sds_dfe3)/sizeof(phy_rtl9310_dfe_t); ++i)
        {
            if (sds_dfe3[i].type != PHY_RTL9310_DFE_END)
            {
                RT_ERR_CHK(_phy_rtl9310_dfe_set(unit, asds, sds_dfe3[i].type, sds_dfe3[i].val), ret);
            }
        }
    }

    RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);

    for (i = 0; i < 20; ++i)
    {
        RT_ERR_CHK(_phy_rtl9310_symErr_clear(unit, sds), ret);
        phy_osal_time_mdelay(150);
        RT_ERR_CHK(_phy_rtl9310_10gr_symErr_get(unit, sds, &info), ret);
        PHY_SDS_DBG("SDS %d symErr 0x%x\n", sds, info.ch[0]);

        if (0 == info.ch[0])
            break;
    }

    if (0 != info.ch[0])
    {
        return RT_ERR_FAILED;
    }

    return ret;
}   /* end of _phy_rtl9310_dfe_leq_adapt */

/* Function Name:
 *      _phy_rtl9310_fiber_adapt
 * Description:
 *      Set SerDes fiber adapt
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
static int32
_phy_rtl9310_fiber_adapt(uint32 unit, uint32 sds)
{
    rtk_sds_symErr_t    info;
    phy_rtl9310_dfe_t   sds_dfe[] =
        {
            {0x0f, 7, 0, 32, 0, PHY_RTL9310_DFE_END, "FGCAL_OFST_BIN"},
            {0x00, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP0, "TAP0"},
            {0x0c, 7, 0, 32, 0, PHY_RTL9310_DFE_VTH, "VTH_BIN"},
        };
    phy_rtl9310_dfe_t   sds_dfe2[] =
        {
            {0x01, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP1EVEN, "TAP1_EVEN"},
            {0x06, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP1ODD, "TAP1_ODD"},
            {0x02, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP2EVEN, "TAP2_EVEN"},
            {0x07, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP2ODD, "TAP2_ODD"},
            {0x03, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP3EVEN, "TAP3_EVEN"},
            {0x08, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP3ODD, "TAP3_ODD"},
            {0x04, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP4EVEN, "TAP4_EVEN"},
            {0x09, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP4ODD, "TAP4_ODD"},
        };
    uint32  asds;
    uint32  i;
    int32   ret;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK((!HWP_SDS_EXIST(unit, sds)), RT_ERR_OUT_OF_RANGE);

    /* function body */
    if (sds < 2)
        return RT_ERR_FAILED;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xc, 14, 10, 0), ret);

    PHY_SDS_DBG("SDS%d RX calibration...\n", sds);

    RT_ERR_CHK(_phy_rtl9310_init_leq_dfe(unit, sds), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 6, 6, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 12, 12, 0), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 7, 7, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 8, 8, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 9, 9, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 10, 10, 0), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x3, 3, 2, 0x3), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x3, 5, 4, 0x3), ret);

    RT_ERR_CHK(_phy_rtl9310_dbg_set(unit, sds, 0x2), ret);

    for (i = 0; i < sizeof(sds_dfe)/sizeof(phy_rtl9310_dfe_t); ++i)
    {
        RT_ERR_CHK(_phy_rtl9310_dfe_dump(unit, asds, &sds_dfe[i]), ret);
    }

    for (i = 0; i < sizeof(sds_dfe)/sizeof(phy_rtl9310_dfe_t); ++i)
    {
        if (sds_dfe[i].type != PHY_RTL9310_DFE_END)
        {
            if (PHY_RTL9310_DFE_TAP0 == sds_dfe[i].type)
                sds_dfe[i].val = 31;

            RT_ERR_CHK(_phy_rtl9310_dfe_set(unit, asds, sds_dfe[i].type, sds_dfe[i].val), ret);
        }
    }

    RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);

    if (DISABLED == rxCaliConf[unit][sds].dfeAuto)
    {
        for (i = 0; i < sizeof(sds_dfe2)/sizeof(phy_rtl9310_dfe_t); ++i)
        {
            RT_ERR_CHK(_phy_rtl9310_dfe_dump(unit, asds, &sds_dfe2[i]), ret);
        }

        for (i = 0; i < sizeof(sds_dfe2)/sizeof(phy_rtl9310_dfe_t); ++i)
        {
            if (sds_dfe2[i].type != PHY_RTL9310_DFE_END)
            {
                RT_ERR_CHK(_phy_rtl9310_dfe_set(unit, asds, sds_dfe2[i].type, sds_dfe2[i].val), ret);
            }
        }

        RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);
    }

    RT_ERR_CHK(_phy_rtl9310_symErr_clear(unit, sds), ret);
    RT_ERR_CHK(_phy_rtl9310_10gr_symErr_get(unit, sds, &info), ret);
    PHY_SDS_DBG("SDS %d symErr 0x%x\n", sds, info.ch[0]);

    if (0 != info.ch[0])
    {
        return RT_ERR_FAILED;
    }

    return ret;
}   /* end of _phy_rtl9310_fiber_adapt */

int32
_phy_rtl9310_10gMedia_get(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t *pMedia)
{
    /* parameter check */
    RT_PARAM_CHK(!HWP_10GE_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK((NULL == pMedia), RT_ERR_NULL_POINTER);

    *pMedia = port10gMedia[unit][port];

    return RT_ERR_OK;
} /* end of _phy_rtl9310_10gMedia_get */

/* Function Name:
 *      phy_rtl9310_10gmedia_get
 * Description:
 *      Get 9310 10G serdes PHY media type.
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
phy_rtl9310_10gmedia_get(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t *pMedia)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_10gMedia_get(unit, port, pMedia), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_10gmedia_get */

/* Function Name:
 *      _phy_rtl9310_sdsTxParam_get
 * Description:
 *      Get SerDes Tx parameter
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 * Output:
 *      pParam - eye parameter
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
_phy_rtl9310_sdsTxParam_get(uint32 unit, uint32 sds, rtk_sds_eyeParam_t *pParam)
{
    uint32  asds, evenSds;
    int32   ret = RT_ERR_FAILED;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK(!HWP_SDS_EXIST(unit, sds), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK((NULL == pParam), RT_ERR_NULL_POINTER);

    osal_memset(pParam, 0x0, sizeof(rtk_sds_eyeParam_t));

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);
    evenSds = asds - (asds % 2);

    RT_ERR_CHK(SDS_FIELD_R(unit, evenSds, 0x2E, 0x8, 10, 7, &pParam->impedance), ret);
    RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x2E, 0x1, 4, 0, &pParam->pre_amp), ret);
    RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x2E, 0x1, 9, 5, &pParam->main_amp), ret);
    RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x2E, 0x1, 14, 10, &pParam->post_amp), ret);
    RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x2E, 0x0, 1, 1, &pParam->pre_en), ret);
    RT_ERR_CHK(SDS_FIELD_R(unit, asds, 0x2E, 0x0, 0, 0, &pParam->post_en), ret);

    return ret;
}   /* end of _phy_rtl9310_sdsTxParam_get */

/* Function Name:
 *      phy_rtl9310_sdsTxParam_get
 * Description:
 *      Get SerDes Tx parameter
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 * Output:
 *      pParam - eye parameter
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsTxParam_get(uint32 unit, uint32 sds, rtk_sds_eyeParam_t *pParam)
{
    int ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_sdsTxParam_get(unit, sds, pParam), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sdsTxParam_get */

/* Function Name:
 *      _phy_rtl9310_sdsTxParam_set
 * Description:
 *      Set SerDes Tx parameters
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 *      param  - eye parameter value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
_phy_rtl9310_sdsTxParam_set(uint32 unit, uint32 sds, rtk_sds_eyeParam_t param)
{
    uint32  asds, evenSds;
    int32   ret = RT_ERR_FAILED;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    RT_PARAM_CHK(!HWP_SDS_EXIST(unit, sds), RT_ERR_OUT_OF_RANGE);

    /* function body */
    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    evenSds = asds - (asds % 2);

    RT_ERR_CHK(SDS_FIELD_W(unit, evenSds, 0x2E, 0x8, 10, 7, param.impedance), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2E, 0x1, 4, 0, param.pre_amp), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2E, 0x1, 9, 5, param.main_amp), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2E, 0x1, 14, 10, param.post_amp), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2E, 0x0, 1, 1, param.pre_en), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2E, 0x0, 0, 0, param.post_en), ret);

    return ret;
}   /* end of _phy_rtl9310_sdsTxParam_set */

/* Function Name:
 *      phy_rtl9310_sdsTxParam_set
 * Description:
 *      Set SerDes Tx parameters
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 *      param  - eye parameter value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsTxParam_set(uint32 unit, uint32 sds, rtk_sds_eyeParam_t param)
{
    int ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_sdsTxParam_set(unit, sds, param), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sdsTxParam_set */

int32
_phy_rtl9310_sdsRxCaliConf_set(uint32 unit, uint32 sds, rtk_sds_rxCaliConf_t *conf)
{
    osal_memcpy(&rxCaliConf[unit][sds], conf, sizeof(rtk_sds_rxCaliConf_t));

    return RT_ERR_OK;
}   /* end of _phy_rtl9310_sdsRxCaliConf_set */

int32
_phy_rtl9310_sdsCustParam_set(uint32 unit, uint32 sds)
{
    rtk_port_10gMedia_t     media;
    rtk_port_t              port;
    sds_conf_t              *pSds_cust_conf = NULL;
    sds_conf_t              **ppSds_conf;
    sds_tx_param_t          *pTxParam;
    sds_config              *pParam, *pTxConfig;
    rtk_sds_rxCaliConf_t    *pRxConfig;
    int32                   ret = RT_ERR_OK;

    /* Search if there is customer's serdes config for current board  */
    for (ppSds_conf = conf_rtl9310_cust_board_list; *ppSds_conf != NULL; ppSds_conf++)
    {
        if ((*ppSds_conf)->hwp_id == HWP_IDENTIFIER_ID())
        {
            pSds_cust_conf = *ppSds_conf;
            break;
        }
    }

    if (pSds_cust_conf == NULL)
    {
        return RT_ERR_OK;
    }

    if (pSds_cust_conf->sds_conf_per_unit[unit] == NULL)
    {
        return RT_ERR_OK;
    }

    if (pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds] == NULL)
        return RT_ERR_OK;

    /* common */
    pParam = pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds]->sds_param;
    if (pParam == NULL)
        return RT_ERR_OK;

    while (pParam->page != RTK_SDS_PAGEREGVAL_END)
    {
        RT_ERR_CHK(hal_serdes_reg_set(unit, sds, pParam->page, pParam->reg, pParam->data), ret);
        pParam++;
    }

    if (!HWP_SERDES_PORT(unit, sds))
        return RT_ERR_OK;

    /* Rx */
    pRxConfig = pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds]->rxCaliConf;
    if (pRxConfig)
    {
        RT_ERR_CHK(_phy_rtl9310_sdsRxCaliConf_set(unit, sds, pRxConfig), ret);
    }

    /* Tx */
    port = HWP_SDS_ID2MACID(unit, sds);
    RT_ERR_CHK(_phy_rtl9310_10gMedia_get(unit, port, &media), ret);

    switch (media)
    {
        case PORT_10GMEDIA_DAC_50CM:
            pTxParam = pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds]->dac50cm_tx_param;
            break;
        case PORT_10GMEDIA_DAC_100CM:
            pTxParam = pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds]->dac100cm_tx_param;
            break;
        case PORT_10GMEDIA_DAC_300CM:
            pTxParam = pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds]->dac300cm_tx_param;
            break;
        case PORT_10GMEDIA_DAC_500CM:
            pTxParam = pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds]->dac500cm_tx_param;
            break;
        case PORT_10GMEDIA_FIBER_10G:
        case PORT_10GMEDIA_FIBER_10G_1G_AUTO:
            pTxParam = pSds_cust_conf->sds_conf_per_unit[unit]->sds_cfg[sds]->fiber10g_8431_param;
            break;
        default:
            return RT_ERR_OK;
    }

    if (pTxParam && TRUE == pTxParam->eyeParam_valid)
    {
        RT_ERR_CHK(_phy_rtl9310_sdsTxParam_set(unit, sds, pTxParam->eyeParam), ret);
        pTxConfig = pTxParam->tx_param_misc;

        if (pTxConfig != NULL)
        {
            while (pTxConfig->page != RTK_SDS_PAGEREGVAL_END)
            {
                hal_serdes_reg_set(unit, sds, pTxConfig->page, pTxConfig->reg, pTxConfig->data);
                pTxConfig++;
            }
        }
    }

    return ret;
}   /* end of _phy_rtl9310_sdsCustParam_set */

/* Function Name:
 *      phy_rtl9310_sdsCustConfig_init
 * Description:
 *      Serdes Customer Configuration
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsCustConfig_init(uint32 unit, uint32 sds)
{
    int32   ret;

    /* function body */
    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_sdsCustParam_set(unit, sds), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sdsCustConfig_init */

int32
_phy_rtl9310_10gMedia_set(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t media)
{
    rt_serdesMode_t     mode, old_mode;
    uint32              sds, asds;
    uint32              even_asds;
    uint32              ori, val;
    int32               ret = RT_ERR_OK;

    /* parameter check */
    RT_PARAM_CHK(!HWP_10GE_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK((PORT_10GMEDIA_END <= media), RT_ERR_INPUT);

    /* function body */
    sds = HWP_PORT_SDSID(unit, port);

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    RT_ERR_CHK(_phy_rtl9310_init_leq_dfe(unit, sds), ret);

    /* media none behavior */
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x2e, 0x12, 0x2740), ret);
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x2f, 0x0, 0x0), ret);
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x2f, 0x2, 0x2010), ret);
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x20, 0x0, 0xcd1), ret);

    RT_ERR_CHK(reg_array_read(unit, MANGO_MAC_FORCE_MODE_CTRLr, port,
            REG_ARRAY_INDEX_NONE, &ori), ret);

    val = 0x30809;
    RT_ERR_CHK(reg_array_write(unit, MANGO_MAC_FORCE_MODE_CTRLr, port,
            REG_ARRAY_INDEX_NONE, &val), ret);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2A, 0x12, 7, 6, 0x1), ret);
    RT_ERR_CHK(_phy_rtl9310_sds_mode_set(unit, sds, RTK_MII_DISABLE), ret);

    RT_ERR_CHK(reg_array_write(unit, MANGO_MAC_FORCE_MODE_CTRLr, port,
            REG_ARRAY_INDEX_NONE, &ori), ret);

    RT_ERR_CHK(_phy_rtl9310_sds_mode_get(unit, sds, &old_mode), ret);
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x21, 0x19, 0xF0F0), ret);

    even_asds = asds - (asds % 2);
    RT_ERR_CHK(hal_serdes_reg_set(unit, even_asds, 0x2E, 0x8, 0x0294), ret);

    switch (media)
    {
        case PORT_10GMEDIA_NONE:
            return ret;
        case PORT_10GMEDIA_DAC_50CM:
        case PORT_10GMEDIA_DAC_100CM:
            mode = RTK_MII_10GR;

            if (mode != old_mode)
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x0), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2A, 0x7, 15, 15, 0x1), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x3), ret);

            RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x1, 15, 0, 0x1340), ret);
            RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x21, 0x19, 0xF0A5), ret);
            RT_ERR_CHK(hal_serdes_reg_set(unit, even_asds, 0x2E, 0x8, 0x02A0), ret);
            break;
        case PORT_10GMEDIA_DAC_300CM ... PORT_10GMEDIA_DAC_END:
            mode = RTK_MII_10GR;

            if (mode != old_mode)
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x0), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2A, 0x7, 15, 15, 0x1), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x3), ret);

            RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0x1, 15, 0, 0x5200), ret);
            RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x21, 0x19, 0xF0A5), ret);
            RT_ERR_CHK(hal_serdes_reg_set(unit, even_asds, 0x2E, 0x8, 0x02A0), ret);
            break;
        case PORT_10GMEDIA_FIBER_10G:
        case PORT_10GMEDIA_FIBER_10G_1G_AUTO:
            mode = RTK_MII_10GR;

            if (mode != old_mode)
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x0), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2A, 0x7, 15, 15, 0x1), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x3), ret);

            hal_serdes_reg_set(unit, asds, 0x2E, 0x1, phy_rtl9310_10g_tx[unit][sds]);
            break;
        case PORT_10GMEDIA_FIBER_1G:
            mode = RTK_MII_1000BX_FIBER;

            if (mode != old_mode)
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x0), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2A, 0x7, 15, 15, 0x0), ret);
                RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x20, 0x0, 11, 10, 0x3), ret);

            break;
        default:
            ret = RT_ERR_INPUT;
            return ret;
    }

    RT_ERR_CHK(_phy_rtl9310_sdsCustParam_set(unit, sds), ret);

    RT_ERR_CHK(_phy_rtl9310_sds_mode_set(unit, sds, mode), ret);

    if (RTK_MII_10GR == mode)
    {
        RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x2e, 0x12, 0x27c0), ret);
        RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x2f, 0x0, 0xc000), ret);
        RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x2f, 0x2, 0x6010), ret);
    }
    RT_ERR_CHK(hal_serdes_reg_set(unit, asds, 0x20, 0x0, 0xc30), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2A, 0x12, 7, 6, 0x3), ret);

    RT_ERR_CHK(_phy_rtl9310_sds_rst(unit, sds), ret);

    BITMAP_CLEAR(waMonRxCalied[unit], sds);

    return ret;
}   /* end of _phy_rtl9310_10gMedia_set */

/* Function Name:
 *      phy_rtl9310_10gMedia_set
 * Description:
 *      Set 9310 10G serdes PHY media type.
 * Input:
 *      unit - unit id
 *      port - port id
 *      media
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_INPUT
 *      RT_ERR_CHIP_NOT_SUPPORTED
 *      RT_ERR_TIMEOUT
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_10gMedia_set(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t media)
{
    uint32  sds;
    int32   ret = RT_ERR_OK;

    /* parameter check */
    RT_PARAM_CHK(!HWP_10GE_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK((PORT_10GMEDIA_END <= media), RT_ERR_INPUT);

    /* function body */
    sds = HWP_PORT_SDSID(unit, port);

    PHY_RTL9310_SEM_LOCK(unit);

    if (BITMAP_IS_SET(sdsAdminEnable[unit], sds) || PORT_10GMEDIA_NONE == media)
    {
        RT_ERR_HDL(_phy_rtl9310_10gMedia_set(unit, port, media), ERR, ret);
    }

    port10gMedia[unit][port] = media;

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_10gMedia_set */

int32
_phy_rtl9310_rxCali(uint32 unit, uint32 sds)
{
    rtk_port_10gMedia_t media;
    rtk_port_t          port;
    uint32              val = 0, ori = 0, asds = 0;
    int32               ret = RT_ERR_OK;

    if (BITMAP_IS_CLEAR(sdsCaliEn[unit], sds))
        return RT_ERR_OK;

    port = HWP_SDS_ID2MACID(unit, sds);

    RT_ERR_CHK(reg_read(unit, MANGO_PS_SERDES_OFF_MODE_CTRLr, &ori), ret);
    val = ori | (1 << sds);
    RT_ERR_CHK(reg_write(unit, MANGO_PS_SERDES_OFF_MODE_CTRLr, &val), ret);

    RT_ERR_HDL(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ERR, ret);
    RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x2e, 0xe, 13, 11, 2), ERR, ret);

    if (HWP_PORT_PHY_IDX(unit, port) != HWP_NONE)
        RT_ERR_HDL(_phy_rtl9310_leq_adapt(unit, sds), ERR, ret);
    else if (HWP_SERDES_PORT(unit, port))
    {
        RT_ERR_HDL(_phy_rtl9310_10gMedia_get(unit, port, &media), ERR, ret);
        switch(media)
        {
            case PORT_10GMEDIA_FIBER_10G:
            case PORT_10GMEDIA_FIBER_10G_1G_AUTO:
            //case PORT_10GMEDIA_FIBER_1G:
                RT_ERR_HDL(_phy_rtl9310_fiber_adapt(unit, sds), ERR, ret);
                break;
            case PORT_10GMEDIA_DAC_50CM ... PORT_10GMEDIA_DAC_END:
                RT_ERR_HDL(_phy_rtl9310_dfe_leq_adapt(unit, sds, media), ERR, ret);
                break;
            default:
                break;
        }
    }

    BITMAP_SET(waMonRxCalied[unit], sds);

ERR:
    RT_ERR_CHK(reg_write(unit, MANGO_PS_SERDES_OFF_MODE_CTRLr, &ori), ret);

    return ret;
}   /* end of _phy_rtl9310_rxCali */

/* Function Name:
 *      phy_rtl9310_rxCali
 * Description:
 *      SerDes Rx calibration
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
phy_rtl9310_rxCali(uint32 unit, uint32 sds)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    PHY_SDS_DBG("%s %d calibration trigger\n", __func__, __LINE__);
    RT_ERR_HDL(_phy_rtl9310_rxCali(unit, sds), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_rxCali */

int32
_phy_rtl9310_linkUp_process(uint32 unit, rtk_port_t port)
{
    uint32  sds, val;
    int32   ret;

    sds = HWP_PORT_SDSID(unit, port);

    if (BITMAP_IS_CLEAR(waMonRxCalied[unit], sds))
    {
        PHY_SDS_DBG("%s %d calibration \n", __func__, __LINE__);
        RT_ERR_CHK(_phy_rtl9310_rxCali(unit, sds), ret);
    }

    val = (1 << (sds - 2));
    RT_ERR_CHK(reg_field_write(unit, MANGO_ISR_SERDES_RXIDLEr,
            MANGO_ISR_SERDES_RXIDLEf, &val), ret);

    RT_ERR_CHK(drv_rtl9310_portMacForceLink_set(unit, port, DISABLED,
            PORT_LINKUP), ret);

    return ret;
}   /* end of _phy_rtl9310_linkUp_process */

int32
_phy_rtl9310_linkChangeUp_process(uint32 unit, rtk_port_t port)
{
    int32   ret;

    /* link change up process */
    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%u,port=%u UP", unit, port);

    PHY_SDS_DBG("%s %d port %u calibration trigger\n", __func__, __LINE__, port);
    RT_ERR_CHK(_phy_rtl9310_linkUp_process(unit, port), ret);

    return ret;
}   /* end of _phy_rtl9310_linkChangeUp_process */

int32
_phy_rtl9310_linkChangeDown_process(uint32 unit, rtk_port_t port)
{
    uint32  sds, val, asds;
    int32   ret;
#ifndef __BOOTLOADER__
    RT_ERR_CHK(drv_rtl9310_portMacForceLink_set(unit, port, ENABLED,
            PORT_LINKDOWN), ret);
#endif  /* __BOOTLOADER__ */
    /* clear status */
    sds = HWP_PORT_SDSID(unit, port);
    val = (1 << (sds - 2));
    RT_ERR_CHK(reg_field_write(unit, MANGO_ISR_SERDES_RXIDLEr,
            MANGO_ISR_SERDES_RXIDLEf, &val), ret);

    BITMAP_CLEAR(waMonRxCalied[unit], sds);

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xe, 13, 11, 0), ret);
    RT_ERR_CHK(hal_mac_serdes_rst(unit, sds), ret);

    RT_ERR_CHK(_phy_rtl9310_init_leq_dfe(unit, sds), ret);

    return RT_ERR_OK;
}   /* end of _phy_rtl9310_linkChangeDown_process */

/* Function Name:
 *      phy_rtl9310_linkChange_process
 * Description:
 *      For 10G SerDes link change process
 * Input:
 *      unit    - unit id
 *      port    - which port occurs link change
 *      linkSts - link status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_rtl9310_linkChange_process(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t linkSts)
{
    int32   ret = RT_ERR_OK;

    PHY_SDS_DBG("unit=%u,port=%u,linkSts=%s\n",
            unit, port, ((linkSts==PORT_LINKUP)?"UP":"DOWN"));
    PHY_RTL9310_SEM_LOCK(unit);

    if (PORT_LINKDOWN == linkSts)
    {
        /* link change down process */
        RT_ERR_HDL(_phy_rtl9310_linkChangeDown_process(unit, port), ERR, ret);
    }

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_linkChange_process */

/* Function Name:
 *      phy_rtl9310_init
 * Description:
 *      Initialize 9310 MAC internal serdes PHY.
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
phy_rtl9310_init(uint32 unit, rtk_port_t port)
{
    uint32  sds, asds;
    int32   ret;

    if (INIT_NOT_COMPLETED == phy_rtl9310_init_flag[unit])
    {
        /* create semaphore */
        phy_rtl9310_sem[unit] = phy_osal_sem_mutex_create();
        if (0 == phy_rtl9310_sem[unit])
        {
            RT_ERR(RT_ERR_FAILED, (MOD_HAL|MOD_SDS), "semaphore create failed");
            return RT_ERR_FAILED;
        }

        phy_rtl9310_init_flag[unit] = INIT_COMPLETED;
    }
    //RT_INIT_REENTRY_CHK(phy_rtl9310_init_flag[unit]);
    //phy_rtl9310_init_flag[unit] = INIT_NOT_COMPLETED;

    sds = HWP_PORT_SDSID(unit, port);
    sdsMode[unit][sds] = HWP_SDS_MODE(unit, sds);

    BITMAP_SET(sdsCaliEn[unit], sds);

    rxCaliConf[unit][sds].dfeAuto = ENABLED;
    rxCaliConf[unit][sds].leqAuto = ENABLED;
    rxCaliConf[unit][sds].dfeTap1_4Enable = ENABLED;
    rxCaliConf[unit][sds].ofst = 3;

    if (HWP_UNIT_VALID_LOCAL(unit))
    {
        RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);
        RT_ERR_CHK(hal_serdes_reg_get(unit, asds, 0x2E, 0x1, &phy_rtl9310_10g_tx[unit][sds]), ret);

        RT_ERR_CHK(phy_rtl9310_10gMedia_set(unit, port, PORT_10GMEDIA_NONE), ret);

        RT_ERR_CHK(_phy_rtl9310_linkChangeDown_process(unit, port), ret);
    }


    return RT_ERR_OK;
} /* end of phy_rtl9310_init */

/* Function Name:
 *      phy_rtl9310_media_get
 * Description:
 *      Get 9310 serdes PHY media type.
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
 *      1. media type is PORT_MEDIA_FIBER
 */
int32
phy_rtl9310_media_get(uint32 unit, rtk_port_t port, rtk_port_media_t *pMedia)
{
    /* parameter check */
    RT_PARAM_CHK(!HWP_10GE_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK((NULL == pMedia), RT_ERR_NULL_POINTER);

    *pMedia = PORT_MEDIA_FIBER;

    return RT_ERR_OK;
} /* end of phy_rtl9310_media_get */

/* Function Name:
 *      phy_rtl9310_speed_get
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
phy_rtl9310_speed_get(uint32 unit, rtk_port_t port, rtk_port_speed_t *pSpeed)
{
    rt_serdesMode_t mode;
    uint32          val, sds;
    int32           ret;

    RT_PARAM_CHK((NULL == pSpeed), RT_ERR_NULL_POINTER);

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(reg_array_field_read(unit, MANGO_MAC_LINK_STSr,
            port, REG_ARRAY_INDEX_NONE, MANGO_LINK_STSf, &val), ERR, ret);

    if (0 == val)
    {
        sds = HWP_PORT_SDSID(unit, port);
        RT_ERR_HDL(_phy_rtl9310_sds_mode_get(unit, sds, &mode), ERR, ret);

        switch (mode)
        {
            case RTK_MII_DISABLE:
            case RTK_MII_10GR:
            case RTK_MII_USXGMII_10GSXGMII:
            case RTK_MII_USXGMII_10GDXGMII:
            case RTK_MII_USXGMII_10GQXGMII:
            case RTK_MII_10GR1000BX_AUTO:
                *pSpeed = PORT_SPEED_10G;
                break;
           case RTK_MII_1000BX_FIBER:
           case RTK_MII_1000BX100BX_AUTO:
                 /* serdes mode FIBER1G */
                *pSpeed = PORT_SPEED_1000M;
                break;
            case RTK_MII_100BX_FIBER:
                /* serdes mode FIBER1G */
                *pSpeed = PORT_SPEED_100M;
                break;
            //case RTK_MII_USXGMII:
            case RTK_MII_USXGMII_5GSXGMII:
            case RTK_MII_USXGMII_5GDXGMII:
                *pSpeed = PORT_SPEED_5G;
                break;
            case RTK_MII_USXGMII_2_5GSXGMII:
            case RTK_MII_2500Base_X:
                *pSpeed = PORT_SPEED_2_5G;
                break;
            default:
                PHY_RTL9310_SEM_UNLOCK(unit);
                return RT_ERR_FAILED;
        }
    }
    else
    {
        RT_ERR_HDL(reg_array_field_read(unit, MANGO_MAC_LINK_SPD_STSr,
                port, REG_ARRAY_INDEX_NONE, MANGO_SPD_STSf, &val), ERR, ret);

        switch (val)
        {
            case 0:
                *pSpeed = PORT_SPEED_10M;
                break;
            case 1:
                *pSpeed = PORT_SPEED_100M;
                break;
            case 2:
                *pSpeed = PORT_SPEED_1000M;
                break;
            case 3:
                *pSpeed = PORT_SPEED_500M;
                break;
            case 4:
                *pSpeed = PORT_SPEED_10G;
                break;
            case 5:
                *pSpeed = PORT_SPEED_2_5G;
                break;
            case 6:
                *pSpeed = PORT_SPEED_5G;
                break;
            default:
                PHY_RTL9310_SEM_UNLOCK(unit);
                return RT_ERR_FAILED;
        }
    }

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_speed_get */

/* Function Name:
 *      phy_rtl9310_speed_set
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
phy_rtl9310_speed_set(uint32 unit, rtk_port_t port, rtk_port_speed_t speed)
{
    rt_serdesMode_t mode;
    uint32          sds;
    int32           ret;

    PHY_RTL9310_SEM_LOCK(unit);

    switch (speed)
    {
        case PORT_SPEED_100M:
            mode = RTK_MII_100BX_FIBER;
            break;
        case PORT_SPEED_1000M:
            mode = RTK_MII_1000BX_FIBER;
            break;
        case PORT_SPEED_10G:
            mode = RTK_MII_10GR;
            break;
        default:
            PHY_RTL9310_SEM_UNLOCK(unit);
            return RT_ERR_FAILED;
    }

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_HDL(_phy_rtl9310_sds_mode_set(unit, sds, mode), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_speed_set */

/* Function Name:
 *      phy_rtl9310_duplex_get
 * Description:
 *      Get duplex mode status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pDuplex - pointer to PHY duplex mode status
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_rtl9310_duplex_get(uint32 unit, rtk_port_t port, rtk_port_duplex_t *pDuplex)
{
    uint32  sds, dSds;
    uint32  speed;
    uint32  val;
    int32   ret;

    RT_ERR_CHK(phy_rtl9310_speed_get(unit, port, &speed), ret);

    PHY_RTL9310_SEM_LOCK(unit);

    if (PORT_SPEED_10G == speed)
    {
        *pDuplex = PORT_FULL_DUPLEX;
        PHY_RTL9310_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ret);

    RT_ERR_HDL(SDS_FIELD_R(unit, dSds, 0x2, 0x0, DuplexMode_OFFSET,
            DuplexMode_OFFSET, &val), ERR, ret);

    if (1 == val)
        *pDuplex  = PORT_FULL_DUPLEX;
    else
        *pDuplex = PORT_HALF_DUPLEX;

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_duplex_get */

/* Function Name:
 *      phy_rtl9310_duplex_set
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
phy_rtl9310_duplex_set(uint32 unit, rtk_port_t port, rtk_port_duplex_t duplex)
{
    uint32  sds, dSds;
    uint32  speed;
    uint32  val;
    int32   ret;

    RT_ERR_CHK(phy_rtl9310_speed_get(unit, port, &speed), ret);

    PHY_RTL9310_SEM_LOCK(unit);

    if (PORT_SPEED_10G == speed)
    {
        if (PORT_FULL_DUPLEX != duplex)
        {
            PHY_RTL9310_SEM_UNLOCK(unit);
            return RT_ERR_FAILED;
        }

        PHY_RTL9310_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_HDL(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ERR, ret);

    if (PORT_HALF_DUPLEX == duplex)
        val = 0;
    else
        val = 1;

    RT_ERR_HDL(SDS_FIELD_W(unit, dSds, 0x2, 0x0, DuplexMode_OFFSET,
            DuplexMode_OFFSET, val), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_duplex_set */
int32
_phy_rtl9310_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  sds;
    int32   ret;

    sds = HWP_PORT_SDSID(unit, port);

    if (ENABLED == enable)
    {
        RT_ERR_CHK(_phy_rtl9310_10gMedia_set(unit, port, port10gMedia[unit][port]), ret);
        BITMAP_SET(sdsAdminEnable[unit], sds);
    }
    else
    {
        RT_ERR_CHK(_phy_rtl9310_10gMedia_set(unit, port, PORT_10GMEDIA_NONE), ret);
        BITMAP_CLEAR(sdsAdminEnable[unit], sds);
    }

    BITMAP_CLEAR(waMonRxCalied[unit], sds);

    return ret;
} /* end of _phy_rtl9310_enable_set */

/* Function Name:
 *      phy_rtl9310_enable_set
 * Description:
 *      Set interface status of the specific port
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
phy_rtl9310_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_enable_set(unit, port, enable), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_enable_set */

/* Function Name:
 *      phy_rtl9310_autoNegoEnable_get
 * Description:
 *      Get autonegotiation enable status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pEnable -   auto negotiation status
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_rtl9310_autoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  sds, dSds;
    uint32  speed;
    uint32  val;
    int32   ret;

    RT_ERR_CHK(phy_rtl9310_speed_get(unit, port, &speed), ret);
    if (PORT_SPEED_10G == speed)
    {
        *pEnable = DISABLED;
        return RT_ERR_OK;
    }

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ret);

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(SDS_FIELD_R(unit, dSds, 0x2, 0x0, AutoNegotiationEnable_OFFSET,
            AutoNegotiationEnable_OFFSET, &val), ERR, ret);

    if (1 == val)
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_autoNegoEnable_get */

/* Function Name:
 *      phy_rtl9310_autoNegoEnable_set
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
phy_rtl9310_autoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  sds, dSds;
    uint32  speed;
    uint32  val;
    int32   ret;

    RT_ERR_CHK(phy_rtl9310_speed_get(unit, port, &speed), ret);
    if (PORT_SPEED_10G == speed)
    {
        if(ENABLED == enable)
            return RT_ERR_PORT_NOT_SUPPORTED;

        return RT_ERR_OK;
    }

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ret);

    if (ENABLED == enable)
        val = 1;
    else
        val = 0;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(SDS_FIELD_W(unit, dSds, 0x2, 0x0, AutoNegotiationEnable_OFFSET,
            AutoNegotiationEnable_OFFSET, val), ERR, ret);

    RT_ERR_HDL(SDS_FIELD_W(unit, dSds, 0x2, 0x0, RestartAutoNegotiation_OFFSET,
            RestartAutoNegotiation_OFFSET, 1), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_autoNegoEnable_set */

/* Function Name:
 *      phy_rtl9310_autoNegoAbility_get
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
phy_rtl9310_autoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    rtk_enable_t    en;
    uint32          speed, val;
    uint32          phyData4;
    uint32          sds, dSds;
    int32           ret;

    RT_ERR_CHK(phy_rtl9310_speed_get(unit, port, &speed), ret);
    osal_memset(pAbility, 0, sizeof(rtk_port_phy_ability_t));

    if (PORT_SPEED_10G == speed)
    {
        PHY_RTL9310_SEM_LOCK(unit);

        RT_ERR_HDL(SDS_FIELD_R(unit, dSds, 0x1f, 11, 3, 2, &val), ERR, ret);

        if (0x3 == val)
            pAbility->FC = ENABLED;
        else
            pAbility->FC = DISABLED;

        PHY_RTL9310_SEM_UNLOCK(unit);

        return RT_ERR_OK;
    }

    RT_ERR_CHK(phy_rtl9310_autoNegoEnable_get(unit, port, &en), ret);

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_HDL(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ERR, ret);

    RT_ERR_HDL(hal_serdes_reg_get(unit, dSds, 0x2, 0x4, &phyData4), ERR, ret);

    pAbility->FC = (phyData4 & _1000BaseX_Pause_R4_MASK) >> _1000BaseX_Pause_R4_OFFSET;
    pAbility->AsyFC = (phyData4 & _1000BaseX_AsymmetricPause_R4_MASK) >> _1000BaseX_AsymmetricPause_R4_OFFSET;

    pAbility->Half_1000 = (phyData4 & _1000BaseX_HalfDuplex_R4_MASK) >> _1000BaseX_HalfDuplex_R4_OFFSET;
    pAbility->Full_1000 = (phyData4 & _1000BaseX_FullDuplex_R4_MASK) >> _1000BaseX_FullDuplex_R4_OFFSET;

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_autoNegoAbility_get */

/* Function Name:
 *      phy_rtl9310_autoNegoAbility_set
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
phy_rtl9310_autoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    rtk_enable_t    en;
    uint32          speed;
    uint32          phyData4;
    uint32          sds, dSds;
    int32           ret;

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_CHK(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ret);
    RT_ERR_CHK(phy_rtl9310_speed_get(unit, port, &speed), ret);

    if (PORT_SPEED_10G == speed)
    {
        PHY_RTL9310_SEM_LOCK(unit);

        if (pAbility->FC == ENABLED)
            RT_ERR_HDL(SDS_FIELD_W(unit, dSds, 0x1f, 11, 3, 2, 0x3), ERR, ret);
        else
            RT_ERR_HDL(SDS_FIELD_W(unit, dSds, 0x1f, 11, 3, 2, 0x0), ERR, ret);

        PHY_RTL9310_SEM_UNLOCK(unit);

        return RT_ERR_OK;
    }

    RT_ERR_CHK(phy_rtl9310_autoNegoEnable_get(unit, port, &en), ret);

    PHY_RTL9310_SEM_LOCK(unit);
    RT_ERR_HDL(hal_serdes_reg_get(unit, dSds, 0x2, 0x4, &phyData4), ERR, ret);

    phyData4 = phyData4 & ~(_1000BaseX_Pause_R4_OFFSET | _1000BaseX_AsymmetricPause_R4_MASK);
    phyData4 = phyData4 | (pAbility->FC << _1000BaseX_Pause_R4_OFFSET) |
            (pAbility->AsyFC << _1000BaseX_AsymmetricPause_R4_OFFSET);

    phyData4 = phyData4 & ~(_1000BaseX_HalfDuplex_R4_MASK | _1000BaseX_FullDuplex_R4_MASK);
    phyData4 = phyData4 | (pAbility->Half_1000 << _1000BaseX_HalfDuplex_R4_OFFSET)
            | (pAbility->Full_1000 << _1000BaseX_FullDuplex_R4_OFFSET);

    RT_ERR_HDL(hal_serdes_reg_set(unit, dSds, 0x2, 0x4, phyData4), ERR, ret);

    PHY_RTL9310_SEM_UNLOCK(unit);

    /* Force re-autonegotiation if AN is on*/
    if (ENABLED == en)
    {
        return phy_rtl9310_autoNegoEnable_set(unit, port, en);
    }

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
} /* end of phy_rtl9310_autoNegoAbility_set */

/* Function Name:
 *      phy_rtl9310_fiberUnidirEnable_set
 * Description:
 *      Set fiber unidirection enable status of the specific port
 * Input:
 *      unit           - unit id
 *      port           - port id
 *      enable         - enable status of fiber unidirection
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_rtl9310_fiberUnidirEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  xgeVal, geVal;
    uint32  sds, dSds;
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);

    RT_ERR_HDL(reg_field_read(unit, MANGO_SERDES_UNIDIR_CTRLr,
            MANGO_SDS_UNIDIR_TX_ABLEf, &geVal), ERR, ret);

    RT_ERR_HDL(reg_field_read(unit, MANGO_SERDES_UNIDIR_CTRLr,
            MANGO_CFG_UNIDIR_EN_TGXRf, &xgeVal), ERR, ret);

    if (ENABLED == enable)
    {
        geVal |= (1 << sds);
        if (sds >= 2)
            xgeVal |= (1 << (sds - 2));
    }
    else
    {
        geVal &= ~(1 << sds);
        if (sds >= 2)
            xgeVal &= ~(1 << (sds - 2));
    }

    RT_ERR_HDL(reg_field_write(unit, MANGO_SERDES_UNIDIR_CTRLr,
            MANGO_SDS_UNIDIR_TX_ABLEf, &geVal), ERR, ret);

    RT_ERR_HDL(reg_field_write(unit, MANGO_SERDES_UNIDIR_CTRLr,
            MANGO_CFG_UNIDIR_EN_TGXRf, &xgeVal), ERR, ret);

    RT_ERR_HDL(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ERR, ret);
    RT_ERR_HDL(SDS_FIELD_W(unit, dSds, 0x6, 30, 1, 1, 0x0), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_fiberUnidirEnable_set */

int32
_phy_rtl9310_dac_chk(uint32 unit, uint32 port, uint32 sds)
{
    rtk_port_10gMedia_t media;
    uint32              val;
    int32               ret;

    RT_ERR_CHK(reg_field_read(unit, MANGO_ISR_SERDES_RXIDLEr,
            MANGO_ISR_SERDES_RXIDLEf, &val), ret);

    if (0 == (val & (1 << (sds - 2))))
    {
        return RT_ERR_OK;
    }

    RT_ERR_CHK(_phy_rtl9310_10gMedia_get(unit, port, &media), ret);

    switch(media)
    {
        case PORT_10GMEDIA_DAC_50CM ... PORT_10GMEDIA_DAC_END:
            if (BITMAP_IS_SET(waMonRxCalied[unit], sds))
            {
                val = (1 << (sds - 2));
                RT_ERR_CHK(reg_field_write(unit, MANGO_ISR_SERDES_RXIDLEr,
                        MANGO_ISR_SERDES_RXIDLEf, &val), ret);

                return ret;
            }
            else
            {
                PHY_SDS_DBG("%s %d port %u calibration trigger\n", __func__, __LINE__, port);
                RT_ERR_CHK(_phy_rtl9310_linkUp_process(unit, port), ret);
            }
            break;
        default:
            break;
    }

    return ret;
}   /* end of _phy_rtl9310_dac_chk */

/* Function Name:
 *      phy_rtl9310_sdsFiberRx_check
 * Description:
 *      Check SerDes fiber Rx status
 * Input:
 *      unit - unit id
 *      port - port which is to be handle
 * Output:
 *      pStatus -Fiber RX status
 * Return:
 *      RT_ERR_FAILED - initialize fail
 *      RT_ERR_OK     - initialize success
 * Note:
 *
 */
int32
phy_rtl9310_sdsFiberRx_check(uint32 unit, uint32 port, uint32 *pStatus)
{
    rtk_port_10gMedia_t media;
    rtk_sds_symErr_t    info;
    rtk_sds_linkSts_t   linkSts;
    uint32              sds;
    uint32              loop, chkErrCnt = 0;
    int32               ret;

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);

    RT_ERR_HDL(_phy_rtl9310_10gMedia_get(unit, port, &media), ERR, ret);

    if (PORT_10GMEDIA_NONE == media || BITMAP_IS_CLEAR(sdsAdminEnable[unit], sds))
    {
        PHY_RTL9310_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }

    RT_ERR_HDL(_phy_rtl9310_dac_chk(unit, port, sds), ERR, ret);

    RT_ERR_HDL(_phy_rtl9310_linkSts_get(unit, sds, &linkSts), ERR, ret);
    if (0 == linkSts.sts && 0 == linkSts.sts1)
        goto ERR;

    if (BITMAP_IS_CLEAR(waMonRxCalied[unit], sds))
    {
        PHY_SDS_DBG("%s %d port %u calibration trigger\n", __func__, __LINE__, port);
        RT_ERR_HDL(_phy_rtl9310_linkUp_process(unit, port), ERR, ret);
    }

    phy_osal_memset(&info, 0, sizeof(rtk_sds_symErr_t));

    for (loop = 0; loop < 3; ++loop)
    {
        RT_ERR_HDL(_phy_rtl9310_symErr_get(unit, sds, &info), ERR, ret);
        if (info.ch[0] != 0)
        {
            ++chkErrCnt;
            PHY_SDS_DBG("SDS %d errCnt %d\n", sds, info.ch[0]);
        }
    }

    if (chkErrCnt >= 2)
    {
        *pStatus = PHY_FIBER_RX_STATUS_1;
    }

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sdsFiberRx_check */

/* Function Name:
 *      phy_rtl9310_sdsFiberRx_reset
 * Description:
 *      Reset Rx
 * Input:
 *      unit - unit id
 *      port - port which is to be handle
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED - initialize fail
 *      RT_ERR_OK     - initialize success
 * Note:
 *
 */
int32
phy_rtl9310_sdsFiberRx_reset(uint32 unit, uint32 port)
{
    uint32  sds;
    int32   ret;

    sds = HWP_PORT_SDSID(unit, port);

    PHY_RTL9310_SEM_LOCK(unit);

    PHY_SDS_DBG("%s %d port %u calibration trigger\n", __func__, __LINE__, port);
    RT_ERR_HDL(_phy_rtl9310_rxCali(unit, sds), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sdsFiberRx_reset */

/* Function Name:
 *      phy_rtl9310_sds_loopback_get
 * Description:
 *      Get serdes Loopback status of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      pEnable           -loopback mode status;
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_sds_loopback_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  sds, dSds, val;
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_HDL(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ERR, ret);

    RT_ERR_HDL(SDS_FIELD_R(unit, dSds, 0x2, 0x0, Loopback_OFFSET,
            Loopback_OFFSET, &val), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sds_loopback_get */

/* Function Name:
 *      phy_rtl9310_sds_loopback_set
 * Description:
 *      Get serdes Loopback status of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      enable           -loopback mode status;
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_sds_loopback_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  sds, asds, dSds, val;
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    val = (ENABLED == enable) ? 1 : 0;

    sds = HWP_PORT_SDSID(unit, port);

    /* 10GR */
    RT_ERR_HDL(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ERR, ret);

    RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x6, 0x1, 2, 2, val), ERR, ret);

    /* 1G */
    RT_ERR_HDL(drv_rtl9310_sds2XsgmSds_get(unit, sds, &dSds), ERR, ret);

    RT_ERR_HDL(SDS_FIELD_W(unit, dSds, 0x2, 0x0, Loopback_OFFSET,
            Loopback_OFFSET, val), ERR, ret);

    RT_ERR_HDL(_phy_rtl9310_enable_set(unit, port, DISABLED), ERR, ret);

    RT_ERR_HDL(_phy_rtl9310_enable_set(unit, port, ENABLED), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sds_loopback_set */

/* Function Name:
 *      phy_rtl9310_polar_get
 * Description:
 *      Get port polarity configure
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pPolarCtrl - polarity configuration
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_polar_get(uint32 unit, rtk_port_t port, rtk_port_phyPolarCtrl_t *pPolarCtrl)
{
    uint32  sds, asds, val;
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);

    RT_ERR_HDL(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ERR, ret);

    RT_ERR_HDL(SDS_FIELD_R(unit, asds, 0x0, 0x0, 9, 8, &val), ERR, ret);

    pPolarCtrl->phy_polar_rx = (val >> 1) ? PHY_POLARITY_INVERSE : PHY_POLARITY_NORMAL;
    pPolarCtrl->phy_polar_tx = (val & 0x1) ? PHY_POLARITY_INVERSE : PHY_POLARITY_NORMAL;

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_polar_get */

/* Function Name:
 *      phy_rtl9310_polar_set
 * Description:
 *      Configure 10GE port polarity
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      polarCtrl - polarity configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_polar_set(uint32 unit, rtk_port_t port, rtk_port_phyPolarCtrl_t *pPolarCtrl)
{
    uint32  sds, rxVal, txVal, val, asds;
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);

    rxVal = (pPolarCtrl->phy_polar_rx == PHY_POLARITY_INVERSE) ? 1 : 0;
    txVal = (pPolarCtrl->phy_polar_tx == PHY_POLARITY_INVERSE) ? 1 : 0;

    RT_ERR_HDL(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ERR, ret);

    /* 10GR */
    val = (txVal << 1) | rxVal;
    RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x6, 0x2, 14, 13, val), ERR, ret);

    /* 1G */
    val = (rxVal << 1) | txVal;
    RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x0, 0x0, 9, 8, val), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_9310_polar_set */

/* Function Name:
 *      phy_rtl9310_fiberRxEnable_get
 * Description:
 *      Get fiber Rx enable status of the specific port
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pEnable - fiber Rx enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
int32
phy_rtl9310_fiberRxEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  sds, asds, val;
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);

    RT_ERR_HDL(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ERR, ret);
    RT_ERR_HDL(SDS_FIELD_R(unit, asds, 0x20, 2, 12, 12, &val), ERR, ret);
    if (val == 1)
        *pEnable = DISABLED;
    else
        *pEnable = ENABLED;

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_fiberRxEnable_get */

/* Function Name:
 *      phy_rtl9310_fiberRxEnable_set
 * Description:
 *      Set fiber Rx enable status of the specific port
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - fiber Rx enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
int32
phy_rtl9310_fiberRxEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  sds, asds;
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    sds = HWP_PORT_SDSID(unit, port);
    RT_ERR_HDL(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ERR, ret);

    if (DISABLED == enable)
    {
        RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x21, 0, 15, 15, 1), ERR, ret);
        RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x20, 2, 12, 12, 1), ERR, ret);
        RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x21, 0, 15, 15, 0), ERR, ret);
    }
    else
    {
        RT_ERR_HDL(SDS_FIELD_W(unit, asds, 0x20, 2, 12, 12, 0), ERR, ret);
    }

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_fiberRxEnable_set */

/* Function Name:
 *      _phy_rtl9310_pcb_adapt
 * Description:
 *      Set SerDes PCB adapt
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
static int32
_phy_rtl9310_pcb_adapt(uint32 unit, uint32 sds)
{
    rtk_sds_symErr_t    info;
    rtk_port_t          port;
    phy_rtl9310_dfe_t   sds_dfe[] =
        {
            {0x00, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP0, "TAP0"},
            {0x0c, 7, 0, 32, 0, PHY_RTL9310_DFE_VTH, "VTH_BIN"},
        };
    phy_rtl9310_dfe_t   sds_dfe2[] =
        {
            {0x01, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP1EVEN, "TAP1_EVEN"},
            {0x06, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP1ODD, "TAP1_ODD"},
            {0x02, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP2EVEN, "TAP2_EVEN"},
            {0x07, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP2ODD, "TAP2_ODD"},
            {0x03, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP3EVEN, "TAP3_EVEN"},
            {0x08, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP3ODD, "TAP3_ODD"},
            {0x04, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP4EVEN, "TAP4_EVEN"},
            {0x09, 5, 0, 5, 0, PHY_RTL9310_DFE_TAP4ODD, "TAP4_ODD"},
        };
    uint32  asds;
    uint32  i;
    int32   ret;

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SDS), "unit=%d,sds=%d", unit, sds);

    /* parameter check */
    RT_PARAM_CHK((!HWP_SDS_EXIST(unit, sds)), RT_ERR_OUT_OF_RANGE);

    /* function body */
    if (sds < 2)
        return RT_ERR_FAILED;

    port = HWP_SDS_ID2MACID(unit, sds);

    if (HWP_PORT_PHY_IDX(unit, port) == HWP_NONE)
        return RT_ERR_FAILED;

    RT_ERR_CHK(drv_rtl9310_sds2AnaSds_get(unit, sds, &asds), ret);

    //RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xc, 14, 10, rxCaliConf[unit][sds].ofst), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 6, 0, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 13, 13, 0), ret);
    RT_ERR_CHK(_phy_rtl9310_dfe_dis(unit, asds), ret);

	RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 1), ret);
	RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);
	phy_osal_time_mdelay(10);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xd, 7, 7, 0), ret);
	phy_osal_time_mdelay(100);

    PHY_SDS_DBG("SDS%d RX calibration...\n", sds);

    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 6, 6, 0), ret);
    RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 12, 12, 0), ret);

    RT_ERR_CHK(_phy_rtl9310_dbg_set(unit, sds, 0x2), ret);

    for (i = 0; i < sizeof(sds_dfe)/sizeof(phy_rtl9310_dfe_t); ++i)
    {
        RT_ERR_CHK(_phy_rtl9310_dfe_dump(unit, asds, &sds_dfe[i]), ret);
    }

    for (i = 0; i < sizeof(sds_dfe)/sizeof(phy_rtl9310_dfe_t); ++i)
    {
        if (sds_dfe[i].type != PHY_RTL9310_DFE_END)
        {
            if (PHY_RTL9310_DFE_TAP0 == sds_dfe[i].type)
                sds_dfe[i].val = 31;

            RT_ERR_CHK(_phy_rtl9310_dfe_set(unit, asds, sds_dfe[i].type, sds_dfe[i].val), ret);
        }
    }

    RT_ERR_CHK(_phy_rtl9310_rx_rst(unit, sds), ret);

    if (ENABLED == rxCaliConf[unit][sds].dfeTap1_4Enable)
    {
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 7, 7, 0), ret);
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 8, 8, 0), ret);
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 9, 9, 0), ret);
        RT_ERR_CHK(SDS_FIELD_W(unit, asds, 0x2e, 0xf, 10, 10, 0), ret);
    }

    if (DISABLED == rxCaliConf[unit][sds].dfeTap1_4Enable ||
            DISABLED == rxCaliConf[unit][sds].dfeAuto)
    {
        for (i = 0; i < sizeof(sds_dfe2)/sizeof(phy_rtl9310_dfe_t); ++i)
        {
            RT_ERR_CHK(_phy_rtl9310_dfe_dump(unit, asds, &sds_dfe2[i]), ret);
        }

        for (i = 0; i < sizeof(sds_dfe2)/sizeof(phy_rtl9310_dfe_t); ++i)
        {
            if (sds_dfe2[i].type != PHY_RTL9310_DFE_END)
            {
                RT_ERR_CHK(_phy_rtl9310_dfe_set(unit, asds, sds_dfe2[i].type, sds_dfe2[i].val), ret);
            }
        }
    }

    for (i = 0; i < 20; ++i)
    {
        RT_ERR_CHK(_phy_rtl9310_symErr_clear(unit, sds), ret);
        phy_osal_time_mdelay(150);
        RT_ERR_CHK(_phy_rtl9310_10gr_symErr_get(unit, sds, &info), ret);
        PHY_SDS_DBG("SDS %d symErr 0x%x\n", sds, info.ch[0]);

        if (0 == info.ch[0])
            break;
    }

    if (0 != info.ch[0])
    {
        PHY_SDS_DBG("SDS %d symErr\n", sds);
        return RT_ERR_FAILED;
    }

    return ret;
}   /* end of _phy_rtl9310_pcb_adapt */

/* Function Name:
 *      phy_rtl9310_pcb_adapt
 * Description:
 *      SerDes PCB adapt
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      None
 */
int32
phy_rtl9310_pcb_adapt(uint32 unit, uint32 sds)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_pcb_adapt(unit, sds), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_pcb_adapt */

/* Function Name:
 *      phy_rtl9310_sdsRxCaliEnable_get
 * Description:
 *      Get the SerDes rx cali enable status.
 * Input:
 *      unit      - unit id
 *      sds       - SerDes id
 * Output:
 *      pEnable      - pointer to the sds enable status of rx calibration
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsRxCaliEnable_get(uint32 unit, uint32 sds, rtk_enable_t *pEnable)
{
    RT_PARAM_CHK(NULL == pEnable, RT_ERR_NULL_POINTER);

    PHY_RTL9310_SEM_LOCK(unit);

    if (BITMAP_IS_SET(sdsCaliEn[unit], sds))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    PHY_RTL9310_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of phy_rtl9310_sdsRxCaliEnable_get */

/* Function Name:
 *      phy_rtl9310_sdsRxCaliEnable_set
 * Description:
 *      Set the SerDes rx cali status.
 * Input:
 *      unit      - unit id
 *      sds       - SerDes id
 *      enable    - status of rx calibration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsRxCaliEnable_set(uint32 unit, uint32 sds, rtk_enable_t enable)
{
    PHY_RTL9310_SEM_LOCK(unit);

    if (ENABLED == enable)
        BITMAP_SET(sdsCaliEn[unit], sds);
    else
        BITMAP_CLEAR(sdsCaliEn[unit], sds);

    PHY_RTL9310_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of phy_rtl9310_sdsRxCaliEnable_set */

/* Function Name:
 *      phy_rtl9310_sdsRxCaliDbgEnable_set
 * Description:
 *      Set debug msg status for SerDes rx calibration
 * Input:
 *      enable      - enable print debug msg
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsRxCaliDbgEnable_set(rtk_enable_t enable)
{
    sdsCaliDbg = enable;

    return RT_ERR_OK;
}   /* end of phy_rtl9310_sdsRxCaliDbgEnable_set */

/* Function Name:
 *      phy_rtl9310_sdsRxCaliConf_get
 * Description:
 *      Get the SerDes rx calibration configration.
 * Input:
 *      unit      - unit id
 *      sds       - SerDes id
 * Output:
 *      pConf      - pointer to the sds rx calibration configuration
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsRxCaliConf_get(uint32 unit, uint32 sds, rtk_sds_rxCaliConf_t *pConf)
{
    PHY_RTL9310_SEM_LOCK(unit);

    osal_memcpy(pConf, &rxCaliConf[unit][sds], sizeof(rtk_sds_rxCaliConf_t));

    PHY_RTL9310_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of phy_rtl9310_sdsRxCaliConf_get */

/* Function Name:
 *      phy_rtl9310_sdsRxCaliConf_set
 * Description:
 *      Config the SerDes rx calibration.
 * Input:
 *      unit      - unit id
 *      sds       - SerDes id
 *      conf      - rx calibration conf
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 * Note:
 *      None
 */
int32
phy_rtl9310_sdsRxCaliConf_set(uint32 unit, uint32 sds, rtk_sds_rxCaliConf_t conf)
{
    int32   ret;

    PHY_RTL9310_SEM_LOCK(unit);

    RT_ERR_HDL(_phy_rtl9310_sdsRxCaliConf_set(unit, sds, &conf), ERR, ret);

ERR:
    PHY_RTL9310_SEM_UNLOCK(unit);

    return ret;
}   /* end of phy_rtl9310_sdsRxCaliConf_set */

/* Function Name:
 *      phy_rtl9310_serdes_mapperInit
 * Description:
 *      Initialize PHY 9310 serdes driver.
 * Input:
 *      pPhydrv - pointer of phy driver
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void phy_rtl9310_serdes_mapperInit(rt_phydrv_t *pPhydrv)
{
    pPhydrv->phydrv_index = RT_PHYDRV_RTL9310_SERDES;
    pPhydrv->fPhydrv_init = phy_rtl9310_init;

    pPhydrv->fPhydrv_media_get = phy_rtl9310_media_get;
    pPhydrv->fPhydrv_10gMedia_get = phy_rtl9310_10gmedia_get;
    pPhydrv->fPhydrv_10gMedia_set = phy_rtl9310_10gMedia_set;
    pPhydrv->fPhydrv_autoNegoEnable_get = phy_rtl9310_autoNegoEnable_get;
    pPhydrv->fPhydrv_autoNegoEnable_set = phy_rtl9310_autoNegoEnable_set;
    pPhydrv->fPhydrv_autoNegoAbility_get = phy_rtl9310_autoNegoAbility_get;
    pPhydrv->fPhydrv_autoNegoAbility_set = phy_rtl9310_autoNegoAbility_set;
    pPhydrv->fPhydrv_duplex_get = phy_rtl9310_duplex_get;
    pPhydrv->fPhydrv_duplex_set = phy_rtl9310_duplex_set;
    pPhydrv->fPhydrv_speed_get = phy_rtl9310_speed_get;
    pPhydrv->fPhydrv_speed_set = phy_rtl9310_speed_set;
    pPhydrv->fPhydrv_enable_set = phy_rtl9310_enable_set;

    pPhydrv->fPhydrv_fiberUnidirEnable_set = phy_rtl9310_fiberUnidirEnable_set;
    pPhydrv->fPhydrv_loopback_get = phy_rtl9310_sds_loopback_get;
    pPhydrv->fPhydrv_loopback_set = phy_rtl9310_sds_loopback_set;
    pPhydrv->fPhydrv_polar_get = phy_rtl9310_polar_get;
    pPhydrv->fPhydrv_polar_set = phy_rtl9310_polar_set;
    pPhydrv->fPhydrv_fiberRxEnable_get = phy_rtl9310_fiberRxEnable_get;
    pPhydrv->fPhydrv_fiberRxEnable_set = phy_rtl9310_fiberRxEnable_set;

    pPhydrv->fPhydrv_serdesFiberRx_check = phy_rtl9310_sdsFiberRx_check;
    pPhydrv->fPhydrv_serdesFiberRx_reset = phy_rtl9310_sdsFiberRx_reset;
} /* end of phy_rtl9310_serdes_mapperInit*/
