/*
 * Copyright (C) 2016-2022 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 81483 $
 * $Date: 2017-08-18 11:55:41 +0800 (Fri, 18 Aug 2017) $
 *
 * Purpose : PHY 9300 intra serdes Driver APIs.
 *
 * Feature : PHY 9300 intra serdes Driver APIs
 *
 */



/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <osal/time.h>
#include <hal/common/halctrl.h>
#include <hal/phy/phydef.h>
#include <hal/phy/phy_common.h>
#include <hal/phy/phy_rtl9300.h>
#include <hal/chipdef/longan/rtk_longan_reg_struct.h>
#include <dal/dal_construct.h>
#include <hal/mac/serdes.h>
#include <hal/mac/drv/drv_rtl9300.h>

/*
 * Symbol Definition
 */
#define PHY_PAGE_2              (2)
#define PHY_PAGE_3              (3)
#define PHY_PAGE_4              (4)


/*
 * Data Declaration
 */
static rtk_port_10gMedia_t  port10gMedia[RTK_MAX_NUM_OF_UNIT][RTK_MAX_NUM_OF_PORTS] = {{0}};
static rtk_enable_t  portAdminEnable[RTK_MAX_NUM_OF_UNIT][RTK_MAX_NUM_OF_PORTS] = {{0}};

rt_phyInfo_t phy_9300sds_info =
{
    .phy_num    = PORT_NUM_IN_9300SDS,
    .eth_type   = HWP_XGE,
    .isComboPhy = {0, 0, 0, 0, 0, 0, 0, 0},
};

sds_config _9300_dT_sds_10g_1000Bx_lane0[] =
{
    {0x21, 0x02, 0x03C0},{0x21, 0x05, 0x40B0},{0x21, 0x07, 0xF09F},{0x21, 0x08, 0x0000},
    {0x21, 0x0D, 0x0009},{0x21, 0x0F, 0x0008},{0x24, 0x00, 0x0668},{0x24, 0x01, 0x2088},
    {0x24, 0x02, 0xD020},{0x24, 0x09, 0xF000},{0x24, 0x0F, 0xFFDF},{0x24, 0x12, 0x03C4},
    {0x24, 0x13, 0x027F},{0x24, 0x14, 0x1311},{0x24, 0x16, 0x00C9},{0x24, 0x17, 0xA100},
    {0x24, 0x18, 0xBE48},{0x24, 0x1E, 0x07F8},{0x25, 0x02, 0x1017},{0x25, 0x05, 0x7F7C},
    {0x25, 0x07, 0x8100},{0x25, 0x08, 0x0001},{0x25, 0x0A, 0x7C2F},{0x25, 0x0F, 0x0121},
    {0x25, 0x11, 0x8840},{0x25, 0x13, 0x0050},{0x25, 0x16, 0x4000},{0x25, 0x18, 0x8E88},
    {0x25, 0x19, 0x4902},{0x25, 0x1A, 0xA02B},{0x25, 0x1C, 0x7109},
};

sds_config _9300_dT_sds_10g_1000Bx_lane1[] =
{
    {0x21, 0x02, 0x03C0},{0x21, 0x05, 0x40B0},{0x21, 0x07, 0xF09F},{0x21, 0x08, 0x0000},
    {0x21, 0x0A, 0x0003},{0x21, 0x0B, 0x0005},{0x21, 0x0D, 0x0009},{0x21, 0x0F, 0x0008},
    {0x24, 0x00, 0x0668},{0x24, 0x01, 0x2088},{0x24, 0x02, 0xD020},{0x24, 0x09, 0xF000},
    {0x24, 0x0F, 0xFFDF},{0x24, 0x12, 0x03C4},{0x24, 0x13, 0x027F},{0x24, 0x14, 0x1311},
    {0x24, 0x16, 0x00C9},{0x24, 0x17, 0xA100},{0x24, 0x18, 0xBE48},{0x24, 0x1E, 0x07F8},
    {0x25, 0x02, 0x1017},{0x25, 0x05, 0x7F7C},{0x25, 0x07, 0x8100},{0x25, 0x08, 0x0001},
    {0x25, 0x0A, 0x7C2F},{0x25, 0x0F, 0x0121},{0x25, 0x11, 0x8840},{0x25, 0x16, 0x4000},
    {0x25, 0x17, 0x4105},{0x25, 0x18, 0x8E88},{0x25, 0x19, 0x4902},{0x25, 0x1A, 0xA02B},
    {0x25, 0x1B, 0x0003},{0x25, 0x1C, 0x7109},{0x25, 0x1D, 0x2501},{0x25, 0x1E, 0x5030},
    {0x25, 0x1F, 0x2624},
};

sds_config _9300_dT_sds_10gr[] =
{
    {0x06, 0x0D, 0x0F00},{0x21, 0x02, 0x03C0},{0x21, 0x05, 0x40B0},{0x21, 0x07, 0xF09F},
    {0x21, 0x08, 0x0000},{0x21, 0x0D, 0x0009},{0x21, 0x0F, 0x0008},{0x2E, 0x00, 0x8668},
    {0x2E, 0x01, 0x2088},{0x2E, 0x02, 0xD020},{0x2E, 0x09, 0xF000},{0x2E, 0x0F, 0xFFDF},
    {0x2E, 0x12, 0x2034},{0x2E, 0x13, 0x027F},{0x2E, 0x14, 0x1311},{0x2E, 0x16, 0x00CF},
    {0x2E, 0x17, 0xA100},{0x2E, 0x18, 0xBE48},{0x2E, 0x1E, 0x07FA},{0x2F, 0x02, 0x1017},
    {0x2F, 0x05, 0x7F7C},{0x2F, 0x07, 0x8100},{0x2F, 0x08, 0x0001},{0x2F, 0x0A, 0x7C2F},
    {0x2F, 0x0F, 0x0121},{0x2F, 0x11, 0x8840},{0x2F, 0x13, 0x0050},{0x2F, 0x16, 0x4000},
    {0x2F, 0x17, 0x4108},{0x2F, 0x18, 0x8E88},{0x2F, 0x19, 0x4902},{0x2F, 0x1A, 0xA12B},
    {0x2F, 0x1C, 0x7109},
};

/*
 * Function Declaration
 */
static int32 _phy_9300_serdes_init_10g_fiber(uint32 unit, uint8 sdsId);
static int32 _phy_9300_serdes_init_1g_fiber(uint32 unit, uint8 sdsId);
static int32 _phy_9300_serdes_init_100m_fiber(uint32 unit, uint8 sdsId);
static void _9300_serdes_index_to_physical(uint32 unit, uint8 page, uint32 logicIdx, uint32 *phyIdx0, uint32 *phyIdx1);
static void _9300_serdes_reg_write(uint32 unit, uint8 index, uint8 page, uint8 reg, uint16 data);
static void _9300_serdes_patch_set(uint32 unit, uint32 sdsId, sds_config *cfg, uint32 size);

static void _9300_serdes_index_to_physical(uint32 unit, uint8 page, uint32 logicIdx, uint32 *phyIdx0,uint32 *phyIdx1)
{
    if((page >= 0x20) || ((2 != logicIdx) && (3 != logicIdx)))
    {
        *phyIdx0 = logicIdx;
        *phyIdx1 = logicIdx;
    }
    else if(2 == logicIdx)
    {
        if((HWP_SDS_MODE(unit, logicIdx) == RTK_MII_XSGMII) ||
            (HWP_SDS_MODE(unit, logicIdx) == RTK_MII_RS8MII))
        {
            *phyIdx0 = 2;
            *phyIdx1 = 3;
        }
        else
        {
            *phyIdx0 = logicIdx;
            *phyIdx1 = logicIdx;
        }
    }
    else /*logic sdsid is 3*/
    {
        if((HWP_SDS_MODE(unit, 2) == RTK_MII_10GR) ||
            (HWP_SDS_MODE(unit, 2) == RTK_MII_RXAUI_LITE) ||
            (HWP_SDS_MODE(unit, 2) == RTK_MII_HISGMII) ||
            (HWP_SDS_MODE(unit, 2) == RTK_MII_SGMII) ||
            (HWP_SDS_MODE(unit, 2) == RTK_MII_1000BX_FIBER) ||
            (HWP_SDS_MODE(unit, 2) == RTK_MII_100BX_FIBER) ||
            (HWP_SDS_MODE(unit, 2) == RTK_MII_1000BX100BX_AUTO))
        {
            if((HWP_SDS_MODE(unit, logicIdx) == RTK_MII_HISGMII) ||
                (HWP_SDS_MODE(unit, logicIdx) == RTK_MII_SGMII) ||
                (HWP_SDS_MODE(unit, logicIdx) == RTK_MII_1000BX_FIBER) ||
                (HWP_SDS_MODE(unit, logicIdx) == RTK_MII_100BX_FIBER) ||
                (HWP_SDS_MODE(unit, logicIdx) == RTK_MII_1000BX100BX_AUTO))
            {
                *phyIdx0 = 10;
                *phyIdx1 = 10;
            }
            else
            {
                *phyIdx0 = logicIdx;
                *phyIdx1 = logicIdx;
            }
        }
        else if((HWP_SDS_MODE(unit, 2) == RTK_MII_XSGMII) &&
                    (HWP_SDS_MODE(unit, 3) == RTK_MII_XSGMII))
        {
            *phyIdx0 = 10;
            *phyIdx1 = 11;
        }
        else if((HWP_SDS_MODE(unit, 2) == RTK_MII_RS8MII) &&
                    (HWP_SDS_MODE(unit, 3) == RTK_MII_RS8MII))
        {
            *phyIdx0 = 10;
            *phyIdx1 = 11;
        }
        else if((HWP_SDS_MODE(unit, 2) == RTK_MII_QSGMII) &&
                    (HWP_SDS_MODE(unit, 3) == RTK_MII_XSGMII))
        {
            *phyIdx0 = 10;
            *phyIdx1 = 11;
        }
        else if((HWP_SDS_MODE(unit, 2) == RTK_MII_XSGMII) &&
                    (HWP_SDS_MODE(unit, 3) == RTK_MII_QSGMII))
        {
            *phyIdx0 = 10;
            *phyIdx1 = 10;
        }
        else if((HWP_SDS_MODE(unit, 2) == RTK_MII_QHSGMII) &&
                    (HWP_SDS_MODE(unit, 3) == RTK_MII_QSGMII))
        {
            *phyIdx0 = 10;
            *phyIdx1 = 10;
        }
        else if((HWP_SDS_MODE(unit, 2) == RTK_MII_QSGMII) &&
                    (HWP_SDS_MODE(unit, 3) == RTK_MII_QSGMII))
        {
            *phyIdx0 = 10;
            *phyIdx1 = 10;
        }
        else
        {
            *phyIdx0 = logicIdx;
            *phyIdx1 = logicIdx;
        }
    }

    return;
}

static void _9300_serdes_reg_write(uint32 unit, uint8 index, uint8 page, uint8 reg, uint16 data)
{
    uint32 cmddata;
    uint32 flag;
    uint32 i;
    uint32 value;

    value = 0;
    value = data;
    reg_field_write(unit, LONGAN_SDS_INDACS_DATAr, LONGAN_SDS_DATAf, &value);

    cmddata = 0;
    cmddata = (index << 2) + (page << 7) + (reg << 13) + 3;
    reg_write(unit, LONGAN_SDS_INDACS_CMDr, &cmddata);

    for (i = 0; i < 100; i++)
    {
        osal_time_udelay(10);
        reg_field_read(unit, LONGAN_SDS_INDACS_CMDr, LONGAN_SDS_CMDf,&flag);
        if (0 == flag)
        {
            break;
        }
    }

    if (i >= 100)
    {
        CNSTRT_PRINT("%s,%d:sds register access fail!\n", __FUNCTION__, __LINE__);
        return;
    }

    return;
}

static void _9300_serdes_patch_set(uint32 unit, uint32 sdsId, sds_config *cfg, uint32 size)
{
    uint32 phySdsId0;
    uint32 phySdsId1;

    if(NULL == cfg)
        return;

    while(size)
    {
        _9300_serdes_index_to_physical(unit, cfg->page, sdsId, &phySdsId0, &phySdsId1);
        if(phySdsId0 == phySdsId1)
        {
            _9300_serdes_reg_write(unit, phySdsId0, cfg->page, cfg->reg, cfg->data);
        }
        else
        {
            _9300_serdes_reg_write(unit, phySdsId0, cfg->page, cfg->reg, cfg->data);
            _9300_serdes_reg_write(unit, phySdsId1, cfg->page, cfg->reg, cfg->data);
        }
        cfg++;
        size--;
    }

    return;
}


int32  _9300_serdes_mode_get(uint32 unit, uint32 sdsId, rt_serdesMode_t* pMode)
{
    uint32  value;
    uint32 reg;
    uint32 field;
    reg_field_t sds_mode_sel[] =
    {
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS0_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS1_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS2_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS3_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS4_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS5_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS6_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS7_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_2r, LONGAN_SDS8_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_2r, LONGAN_SDS9_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_3r, LONGAN_SDS10_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_3r, LONGAN_SDS11_MODE_SELf},
    };

    reg = sds_mode_sel[sdsId].reg;
    field = sds_mode_sel[sdsId].field;

    reg_field_read(unit, reg, field, &value);
    switch(value)
    {
        case 0x4:
            *pMode = RTK_MII_1000BX_FIBER;
            break;
        case 0x5:
            *pMode = RTK_MII_100BX_FIBER;
            break;
        case 0x6:
            *pMode = RTK_MII_QSGMII;
            break;
        case 0x10:
            *pMode = RTK_MII_XSGMII;
            break;
        case 0x1a:
            *pMode = RTK_MII_10GR;
            break;
        case 0x1f:
            *pMode = RTK_MII_DISABLE;
            break;
        default:
            return RT_ERR_INPUT;
    }

    return RT_ERR_OK;

}

int32  _9300_serdes_mode_set(uint32 unit, uint32 sdsId, rt_serdesMode_t mode)
{
    uint32  value;
    uint32 reg;
    uint32 field;
    reg_field_t sds_mode_sel[] =
    {
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS0_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS1_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS2_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_0r, LONGAN_SDS3_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS4_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS5_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS6_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_1r, LONGAN_SDS7_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_2r, LONGAN_SDS8_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_2r, LONGAN_SDS9_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_3r, LONGAN_SDS10_MODE_SELf},
        {LONGAN_SDS_MODE_SEL_3r, LONGAN_SDS11_MODE_SELf},
    };

    reg = sds_mode_sel[sdsId].reg;
    field = sds_mode_sel[sdsId].field;

    switch (mode)
    {
        case RTK_MII_DISABLE:
            /* serdes off */
            value = 0x1f;
            reg_field_write(unit, reg, field, &value);
            break;
        case RTK_MII_QSGMII:
            /* serdes off */
            value = 0x1f;
            reg_field_write(unit, reg, field, &value);
            /* serdes mode QSGMII */
            value = 0x6;
            reg_field_write(unit, reg, field, &value);
            break;
        case RTK_MII_10GR:
            /* serdes off */
            value = 0x1f;
            reg_field_write(unit, reg, field, &value);
            /* serdes mode 10G Base-R */
            value = 0x1a;
            reg_field_write(unit, reg, field, &value);
            break;
       case RTK_MII_XSGMII:
            /* serdes off */
            value = 0x1f;
            reg_field_write(unit, reg, field, &value);
            /* serdes mode XSGMII */
            value = 0x10;
            reg_field_write(unit, reg, field, &value);
            break;
        case RTK_MII_1000BX_FIBER:
            /* serdes off */
            value = 0x1f;
            reg_field_write(unit, reg, field, &value);
            /* serdes mode FIBER1G */
            value = 0x4;
            reg_field_write(unit, reg, field, &value);
            break;
        case RTK_MII_100BX_FIBER:
            /* serdes off */
            value = 0x1f;
            reg_field_write(unit, reg, field, &value);
            /* serdes mode FIBER1G */
            value = 0x5;
            reg_field_write(unit, reg, field, &value);
            break;
        default:
            return RT_ERR_INPUT;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_media_get
 * Description:
 *      Get 9300 serdes PHY media type.
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
phy_9300_media_get(uint32 unit, rtk_port_t port, rtk_port_media_t *pMedia)
{
    RT_PARAM_CHK((NULL == pMedia), RT_ERR_NULL_POINTER);

    *pMedia = PORT_MEDIA_FIBER;

    return RT_ERR_OK;
} /* end of phy_9300_media_get */

/* Function Name:
 *      phy_9300_10gmedia_get
 * Description:
 *      Get 9300 10 serdes PHY media type.
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
phy_9300_10gmedia_get(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t *pMedia)
{
    /* parameter check */
    RT_PARAM_CHK(!HWP_10GE_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK((NULL == pMedia), RT_ERR_NULL_POINTER);

    *pMedia = port10gMedia[unit][port];

    return RT_ERR_OK;
} /* end of phy_9300_10gmedia_get */

/* Function Name:
 *      phy_9300_10gMedia_set
 * Description:
 *
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
phy_9300_10gMedia_set(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t media)
{
    int    ret = RT_ERR_OK;
    uint8  sdsId;

    /* parameter check */
    RT_PARAM_CHK(!HWP_10GE_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK((PORT_10GMEDIA_END <= media), RT_ERR_INPUT);

    /* function body */
    if(DISABLED == portAdminEnable[unit][port])
    {
        port10gMedia[unit][port] = media;
        return RT_ERR_OK;
    }

    sdsId = HWP_PORT_SDSID(unit, port);
    switch (media)
    {
        case PORT_10GMEDIA_NONE:
            ret = _9300_serdes_mode_set(unit,sdsId,RTK_MII_DISABLE);
            break;
        case PORT_10GMEDIA_FIBER_10G:
        case PORT_10GMEDIA_DAC_50CM ... PORT_10GMEDIA_DAC_END:
            ret = _phy_9300_serdes_init_10g_fiber(unit, sdsId);
            break;
        case PORT_10GMEDIA_FIBER_1G:
            ret = _phy_9300_serdes_init_1g_fiber(unit, sdsId);
            break;
        case PORT_10GMEDIA_FIBER_100M:
            ret = _phy_9300_serdes_init_100m_fiber(unit, sdsId);
            break;
        default:
            ret = RT_ERR_INPUT;
            return ret;
    }

    port10gMedia[unit][port] = media;

    return ret;
}   /* end of phy_9300_10gMedia_set */

/* Function Name:
 *      phy_9300_speed_get
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
phy_9300_speed_get(uint32 unit, rtk_port_t port, uint32 *pSpeed)
{
    /* parameter check */
    RT_PARAM_CHK(!HWP_10GE_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK((NULL == pSpeed), RT_ERR_NULL_POINTER);

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    /* function body */
    switch(port10gMedia[unit][port])
    {
        case PORT_10GMEDIA_FIBER_100M:
            *pSpeed = PORT_SPEED_100M;
            break;
        case PORT_10GMEDIA_FIBER_1G:
            *pSpeed = PORT_SPEED_1000M;
            break;
        case PORT_10GMEDIA_FIBER_10G:
        case PORT_10GMEDIA_DAC_50CM ... PORT_10GMEDIA_DAC_END:
            *pSpeed = PORT_SPEED_10G;
            break;
        default:
            *pSpeed = PORT_SPEED_10G;
    }

    return RT_ERR_OK;
} /* end of phy_9300_speed_get */

/* Function Name:
 *      phy_9300_speed_set
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
phy_9300_speed_set(uint32 unit, rtk_port_t port, uint32 speed)
{
    int32               ret = RT_ERR_OK;

    if (HWP_10GE_PORT(unit, port))
    {
        switch (speed)
        {
            case PORT_SPEED_10G:
                ret = phy_9300_10gMedia_set(unit, port, PORT_10GMEDIA_FIBER_10G);
                break;
            case PORT_SPEED_1000M:
                ret = phy_9300_10gMedia_set(unit, port, PORT_10GMEDIA_FIBER_1G);
                break;
            case PORT_SPEED_100M:
                ret = phy_9300_10gMedia_set(unit, port, PORT_10GMEDIA_FIBER_100M);
                break;
            default:
                return RT_ERR_CHIP_NOT_SUPPORTED;
        }
    }

    return ret;
} /* end of phy_9300_speed_set */

/* Function Name:
 *      phy_9300_duplex_get
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
phy_9300_duplex_get(uint32 unit, rtk_port_t port, uint32 *pDuplex)
{
    uint32 data,speed;
    uint32 sdsId;
    int32   ret;

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    if ((ret = phy_9300_speed_get(unit, port, &speed)) != RT_ERR_OK)
        return ret;

    if (PORT_SPEED_10G == speed)
    {
        *pDuplex = PORT_FULL_DUPLEX;
        return RT_ERR_OK;
    }

    sdsId = HWP_PORT_SDSID(unit, port);

    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,&data)) != RT_ERR_OK)
        return ret;

    data  = REG32_FIELD_GET(data,DuplexMode_OFFSET,DuplexMode_MASK);

    if(1 == data)
        *pDuplex  = PORT_FULL_DUPLEX;
    else
        *pDuplex = PORT_HALF_DUPLEX;

    return RT_ERR_OK;
} /* end of phy_9300_duplex_get */

/* Function Name:
 *      phy_9300_duplex_set
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
phy_9300_duplex_set(uint32 unit, rtk_port_t port, uint32 duplex)
{
    uint32 data,speed;
    uint32 sdsId;
    int32 ret;

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    if ((ret = phy_9300_speed_get(unit, port, &speed)) != RT_ERR_OK)
        return ret;

    if (PORT_SPEED_10G == speed)
    {
        if (PORT_FULL_DUPLEX != duplex)
            return RT_ERR_FAILED;

        return RT_ERR_OK;
    }

    sdsId = HWP_PORT_SDSID(unit, port);

    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,&data)) != RT_ERR_OK)
        return ret;
    if (PORT_HALF_DUPLEX == duplex)
        data = REG32_FIELD_SET(data, 0, DuplexMode_OFFSET, DuplexMode_MASK);
    else
        data = REG32_FIELD_SET(data, 1, DuplexMode_OFFSET, DuplexMode_MASK);

    if((ret = hal_serdes_reg_set(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,data)) != RT_ERR_OK)
        return ret;


    return RT_ERR_OK;
} /* end of phy_9300_duplex_set */

/* Function Name:
 *      phy_9300_enable_set
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
phy_9300_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32 data;
    uint32 sdsId;
    int32   ret;

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_OK;

    sdsId = HWP_PORT_SDSID(unit, port);

    /* 1G reg */
    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,&data)) != RT_ERR_OK)
        return ret;
    if (enable == ENABLED)
        data = REG32_FIELD_SET(data, 0, PowerDown_OFFSET, PowerDown_MASK);
    else
        data = REG32_FIELD_SET(data, 1, PowerDown_OFFSET, PowerDown_MASK);

    if((ret = hal_serdes_reg_set(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,data)) != RT_ERR_OK)
        return ret;

    /* 10G reg */
    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_4, PHY_CONTROL_REG,&data)) != RT_ERR_OK)
        return ret;
    if (enable == ENABLED)
        data = REG32_FIELD_SET(data, 0, PowerDown_OFFSET, PowerDown_MASK);
    else
        data = REG32_FIELD_SET(data, 1, PowerDown_OFFSET, PowerDown_MASK);

    if((ret = hal_serdes_reg_set(unit, sdsId, PHY_PAGE_4, PHY_CONTROL_REG,data)) != RT_ERR_OK)
        return ret;

    portAdminEnable[unit][port] = enable;

    if (enable == ENABLED)
        phy_9300_10gMedia_set(unit, port, port10gMedia[unit][port]);
    else
        _9300_serdes_mode_set(unit, sdsId, RTK_MII_DISABLE);

    return RT_ERR_OK;
} /* end of phy_9300_enable_set */

/* Function Name:
 *      phy_9300_autoNegoEnable_get
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
phy_9300_autoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32 data,speed;
    uint32 sdsId;
    int32   ret;

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    if ((ret = phy_9300_speed_get(unit, port, &speed)) != RT_ERR_OK)
        return ret;

    if (PORT_SPEED_10G == speed)
    {
        *pEnable = DISABLED;
        return RT_ERR_OK;
    }

    sdsId = HWP_PORT_SDSID(unit, port);

    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,&data)) != RT_ERR_OK)
        return ret;

    *pEnable  = REG32_FIELD_GET(data,AutoNegotiationEnable_OFFSET,AutoNegotiationEnable_MASK);

    return RT_ERR_OK;
} /* end of phy_9300_autoNegoEnable_get */

/* Function Name:
 *      phy_9300_autoNegoEnable_set
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
phy_9300_autoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32 data,speed;
    uint32 sdsId;
    int32 ret;

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    if ((ret = phy_9300_speed_get(unit, port, &speed)) != RT_ERR_OK)
        return ret;

    if (PORT_SPEED_10G == speed)
    {
        if(ENABLED == enable)
            return RT_ERR_FAILED;

        return RT_ERR_OK;
    }

    sdsId = HWP_PORT_SDSID(unit, port);

    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,&data)) != RT_ERR_OK)
        return ret;
    if (enable == ENABLED)
        data = REG32_FIELD_SET(data, 1, AutoNegotiationEnable_OFFSET, AutoNegotiationEnable_MASK);
    else
        data = REG32_FIELD_SET(data, 0, AutoNegotiationEnable_OFFSET, AutoNegotiationEnable_MASK);

    if((ret = hal_serdes_reg_set(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,data)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_9300_autoNegoEnable_set */

/* Function Name:
 *      phy_9300_autoNegoAbility_get
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
phy_9300_autoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    uint32 data,speed;
    uint32 sdsId;
    int32 ret;

    RT_PARAM_CHK((NULL == pAbility), RT_ERR_NULL_POINTER);

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    if ((ret = phy_9300_speed_get(unit, port, &speed)) != RT_ERR_OK)
        return ret;

    sdsId = HWP_PORT_SDSID(unit, port);
    osal_memset(pAbility, 0, sizeof(rtk_port_phy_ability_t));

    if (PORT_SPEED_10G == speed)
    {
        /* get value from CHIP*/
        if((ret = hal_serdes_reg_get(unit, sdsId, 0x1f, 11,&data)) != RT_ERR_OK)
            return ret;

        pAbility->FC = (REG32_FIELD_GET(data, 2, (0x3 << 2))) ? ENABLED : DISABLED;
        pAbility->AsyFC = (REG32_FIELD_GET(data, 2, (0x3 << 2))) ? ENABLED : DISABLED;

        return RT_ERR_OK;
    }

    /* get value from CHIP*/
    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_AN_ADVERTISEMENT_REG,&data)) != RT_ERR_OK)
        return ret;

    pAbility->FC = (data & _1000BaseX_Pause_R4_MASK) >> _1000BaseX_Pause_R4_OFFSET;
    pAbility->AsyFC = (data & _1000BaseX_AsymmetricPause_R4_MASK) >> _1000BaseX_AsymmetricPause_R4_OFFSET;
    pAbility->Half_10 = 0;
    pAbility->Full_10 = 0;
    pAbility->Half_100 = 0;
    pAbility->Full_100 = 0;
    pAbility->Half_1000 = (data & _1000BaseX_HalfDuplex_R4_MASK) >> _1000BaseX_HalfDuplex_R4_OFFSET;
    pAbility->Full_1000 = (data & _1000BaseX_FullDuplex_R4_MASK) >> _1000BaseX_FullDuplex_R4_OFFSET;

    return RT_ERR_OK;
} /* end of phy_9300_autoNegoAbility_get */


/* Function Name:
 *      phy_9300_autoNegoAbility_set
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
phy_9300_autoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    rtk_enable_t    enable;
    uint32             data,speed;
    uint32             sdsId;
    int32               ret;

    RT_PARAM_CHK((NULL == pAbility), RT_ERR_NULL_POINTER);

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    if ((ret = phy_9300_speed_get(unit, port, &speed)) != RT_ERR_OK)
        return ret;

    sdsId = HWP_PORT_SDSID(unit, port);

    if (PORT_SPEED_10G == speed)
    {
        /* get value from CHIP*/
        if((ret = hal_serdes_reg_get(unit, sdsId, 0x1f, 11,&data)) != RT_ERR_OK)
            return ret;

        if(pAbility->FC == ENABLED)
            data = REG32_FIELD_SET(data, 0x3, 2, 0x3 <<2);
        else
            data = REG32_FIELD_SET(data, 0x0, 2, 0x3 <<2);

        if((ret = hal_serdes_reg_set(unit, sdsId, 0x1f, 11,data)) != RT_ERR_OK)
            return ret;

        return RT_ERR_OK;
    }

    /* get value from CHIP*/
    if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_AN_ADVERTISEMENT_REG,&data)) != RT_ERR_OK)
        return ret;

    data = data & ~(_1000BaseX_Pause_R4_MASK | _1000BaseX_AsymmetricPause_R4_MASK);
    data = data
            | (pAbility->FC << _1000BaseX_Pause_R4_OFFSET)
            | (pAbility->AsyFC << _1000BaseX_AsymmetricPause_R4_OFFSET);
    data = data & ~(_1000BaseX_HalfDuplex_R4_MASK | _1000BaseX_FullDuplex_R4_MASK);
    data = data | (pAbility->Half_1000 << _1000BaseX_HalfDuplex_R4_OFFSET)
            | (pAbility->Full_1000 << _1000BaseX_FullDuplex_R4_OFFSET);

    if ((ret = phy_9300_autoNegoEnable_get(unit, port, &enable)) != RT_ERR_OK)
        return ret;

    if((ret = hal_serdes_reg_set(unit, sdsId, PHY_PAGE_2, PHY_AN_ADVERTISEMENT_REG,data)) != RT_ERR_OK)
        return ret;

    /* Force re-autonegotiation if AN is on*/
    if(ENABLED == enable)
    {
        if((ret = hal_serdes_reg_get(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG,&data)) != RT_ERR_OK)
            return ret;
        data = REG32_FIELD_SET(data, 0, RestartAutoNegotiation_OFFSET, RestartAutoNegotiation_MASK);

        if((ret = hal_serdes_reg_set(unit, sdsId, PHY_PAGE_2, PHY_CONTROL_REG, data)) != RT_ERR_OK)
            return ret;
    }

    return RT_ERR_OK;
} /* end of phy_9300_autoNegoAbility_set */

/* Function Name:
 *      phy_9300_init
 * Description:
 *      Initialize 9300 MAC internal serdes PHY.
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
phy_9300_init(uint32 unit, rtk_port_t port)
{
    uint32 sdsId;

    if (!HWP_10GE_PORT(unit, port))
        return RT_ERR_PORT_ID;

    sdsId = HWP_PORT_SDSID(unit, port);

    if(RTK_MII_10GR == HWP_SDS_MODE(unit, sdsId))
        port10gMedia[unit][port] = PORT_10GMEDIA_FIBER_10G;
    else
        port10gMedia[unit][port] = PORT_10GMEDIA_FIBER_1G;

    return RT_ERR_OK;
} /* end of phy_9300_init */

static int32 _phy_9300_serdes_init_100m_fiber(uint32 unit, uint8 sdsId)
{
    int32  ret;

    /* serdes mode 10G Base-R */
    if((ret = _9300_serdes_mode_set(unit, sdsId, RTK_MII_100BX_FIBER)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
}

/* Function Name:
 *      _phy_9300_serdes_init_1g_fiber
 * Description:
 *
 * Input:
 *      unit - unit id
 *      sdsId - serdes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_CHIP_NOT_SUPPORTED
 *      RT_ERR_TIMEOUT
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
_phy_9300_serdes_init_1g_fiber(uint32 unit, uint8 sdsId)
{
    int32  ret;
    uint32 size;

    /* serdes mode 1G Fiber */
    if((ret = _9300_serdes_mode_set(unit, sdsId, RTK_MII_1000BX_FIBER)) != RT_ERR_OK)
        return ret;

    if(CHIP_REV_ID_D <= HWP_CHIP_REV(unit))
    {
        if(sdsId%2)
        {
            size =  sizeof(_9300_dT_sds_10g_1000Bx_lane1)/sizeof(sds_config);
            _9300_serdes_patch_set(unit, sdsId, _9300_dT_sds_10g_1000Bx_lane1, size);
        }
        else
        {
            size =  sizeof(_9300_dT_sds_10g_1000Bx_lane0)/sizeof(sds_config);
            _9300_serdes_patch_set(unit, sdsId, _9300_dT_sds_10g_1000Bx_lane0, size);
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      _phy_9300_serdes_init_10g_fiber
 * Description:
 *
 * Input:
 *      unit - unit id
 *      sdsId - serdes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_CHIP_NOT_SUPPORTED
 *      RT_ERR_TIMEOUT
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
_phy_9300_serdes_init_10g_fiber(uint32 unit, uint8 sdsId)
{
    int32  ret;
    uint32 size;

    /* serdes mode 10G Base-R */
    if((ret = _9300_serdes_mode_set(unit, sdsId, RTK_MII_10GR)) != RT_ERR_OK)
        return ret;

    if(CHIP_REV_ID_D <= HWP_CHIP_REV(unit))
    {
        size =  sizeof(_9300_dT_sds_10gr)/sizeof(sds_config);
        _9300_serdes_patch_set(unit, sdsId, _9300_dT_sds_10gr, size);
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_serdes_loopback_get
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
phy_9300_serdes_loopback_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32         data, val;
    int32           ret;

    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 0, 0, &data)) != RT_ERR_OK)
        return ret;

    *pEnable = (REG32_FIELD_GET(data, 4, (0x1 << 4))) ? ENABLED : DISABLED; /* SP_CFG_DIG_LPK */

    /* verbose check on the other reg */
    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 6, 1, &data)) != RT_ERR_OK)
        return ret;

    val = REG32_FIELD_GET(data, 2, (0x1 << 2)); /* FP_TGR1_CFG_AFE_LPK */
    if ( ((val == 0) && (*pEnable != DISABLED))
        ||((val != 0) && (*pEnable != ENABLED)))
    {
        RT_LOG(LOG_WARNING, (MOD_HAL|MOD_PORT), "unit=%u,port=%u,loopback: regs inconsisten!", unit, port);
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_serdes_loopback_set
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
phy_9300_serdes_loopback_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rt_serdesMode_t mode;
    uint32 data,val;
    int32 ret;

    val = (ENABLED == enable) ? 1 : 0;
    /*Fiber1G*/
    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 0, 0, &data)) != RT_ERR_OK)
        return ret;

    data = REG32_FIELD_SET(data, val, 4, (0x1 << 4));   /* SP_CFG_DIG_LPK */
    if((ret = hal_serdes_reg_set(unit, HWP_PORT_SDSID(unit, port), 0, 0, data)) != RT_ERR_OK)
        return ret;

    /*10GR*/
    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 6,0x1 , &data)) != RT_ERR_OK)
        return ret;

    data = REG32_FIELD_SET(data, val, 2, (0x1 << 2));   /* FP_TGR1_CFG_AFE_LPK */
    if((ret = hal_serdes_reg_set(unit, HWP_PORT_SDSID(unit, port), 6,0x1 , data)) != RT_ERR_OK)
        return ret;

    _9300_serdes_mode_get( unit, HWP_PORT_SDSID(unit, port), &mode);
    _9300_serdes_mode_set( unit, HWP_PORT_SDSID(unit, port), mode);
    osal_time_mdelay(1);

    return RT_ERR_OK;
}

/* Function Name:
 *      _phy_9300_serdesFiber1g_handeler
 * Description:
 *      Handle serdes fiber 1G and reset action
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
_phy_9300_serdesFiber1g_handeler(uint32 unit, uint32 port, uint32 *pStatus)
{
    uint32  chk_loop, chk_error;
    uint32  reg_data;
    uint32  sdsId;
    int32 ret;

    chk_error = 0;
    *pStatus = PHY_FIBER_RX_STATUS_0;

    sdsId = HWP_PORT_SDSID(unit, port);

    /*chanel 0*/
    if((ret = hal_serdes_reg_get(unit, sdsId, 1, 24, &reg_data)) != RT_ERR_OK)
        return ret;

    reg_data &= (~0x7);
    if((ret = hal_serdes_reg_set(unit, sdsId, 1, 24, reg_data)) != RT_ERR_OK)
        return ret;
    /* clear counter*/
    if((ret = hal_serdes_reg_set(unit, sdsId, 1, 2, 0)) != RT_ERR_OK)
        return ret;

    for(chk_loop = 0; chk_loop < 3; ++chk_loop)
    {
        /*check error counter*/
        if((ret = hal_serdes_reg_get(unit, sdsId, 1, 2, &reg_data)) != RT_ERR_OK)
            return ret;

        if (0xff <= reg_data)
        {
            chk_error++;
        }
        osal_time_usleep(200);
    }

    if (chk_error >= 2)
    {
        *pStatus = PHY_FIBER_RX_STATUS_1;
    }
    else
    {
        chk_error = 0;

        for(chk_loop = 0; chk_loop < 3; ++chk_loop)
        {
            if((ret = hal_serdes_reg_get(unit, sdsId, 1, 29, &reg_data)) != RT_ERR_OK)
                return ret;

             if (0x100 == (reg_data & 0x100))
             {
                if (0x10 != (reg_data & 0x10))
                {
                    ++chk_error;
                }
            }
        }

        if (3 == chk_error)
        {
            *pStatus = PHY_FIBER_RX_STATUS_2;
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_serdesFiberRx_check
 * Description:
 *      Handle serdes fiber 1G
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
phy_9300_serdesFiberRx_check(uint32 unit, uint32 port, uint32 *pStatus)
{
    uint32 speed;
    int32  ret;

    if (HWP_10GE_PORT(unit, port))
    {
        /* only work for giga */
        if((ret = phy_9300_speed_get(unit, port, &speed)) != RT_ERR_OK)
            return ret;

        if (PORT_SPEED_1000M == speed)
        {
            _phy_9300_serdesFiber1g_handeler(unit, port, pStatus);
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_serdesFiberRx_reset
 * Description:
 *      Handle serdes fiber 1G
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
phy_9300_serdesFiberRx_reset(uint32 unit, uint32 port)
{
    uint32 value;
    uint32 reg_data;
    int32   ret;

    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 0, 0, &reg_data)) != RT_ERR_OK)
        return ret;

    value = reg_data & (~0x3);
    if((ret = hal_serdes_reg_set(unit, HWP_PORT_SDSID(unit, port), 0, 0, value)) != RT_ERR_OK)
        return ret;

    value |= 0x3;
    if((ret = hal_serdes_reg_set(unit, HWP_PORT_SDSID(unit, port), 0, 0, value)) != RT_ERR_OK)
        return ret;

    if((ret = hal_serdes_reg_set(unit, HWP_PORT_SDSID(unit, port), 0, 0, reg_data)) != RT_ERR_OK)
        return ret;

    return  RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_polar_get
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
phy_9300_polar_get(uint32 unit, rtk_port_t port, rtk_port_phyPolarCtrl_t *pPolarCtrl)
{
    uint32         data, val;
    int32           ret;

    osal_memset(pPolarCtrl, 0, sizeof(rtk_port_phyPolarCtrl_t));

    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 6, 2, &data)) != RT_ERR_OK)
        return ret;

    pPolarCtrl->phy_polar_rx = (REG32_FIELD_GET(data, 13, (0x1 << 13))) ? PHY_POLARITY_INVERSE : PHY_POLARITY_NORMAL; /* FP_TGR2_CFG_INV_HSI */
    pPolarCtrl->phy_polar_tx = (REG32_FIELD_GET(data, 14, (0x1 << 14))) ? PHY_POLARITY_INVERSE : PHY_POLARITY_NORMAL; /* FP_TGR2_CFG_INV_HSO */

    /* verbose check on the other reg */
    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 0, 0, &data)) != RT_ERR_OK)
        return ret;

    val = REG32_FIELD_GET(data, 9, (0x1 << 9)); /* SP_INV_HSI */
    if (  ((val == 0) && (pPolarCtrl->phy_polar_rx != PHY_POLARITY_NORMAL))
        ||((val != 0) && (pPolarCtrl->phy_polar_rx != PHY_POLARITY_INVERSE)))
    {
        RT_LOG(LOG_WARNING, (MOD_HAL|MOD_PORT), "unit=%u,port=%u,polar: rx regs inconsisten!", unit, port);
    }

    val = REG32_FIELD_GET(data, 8, (0x1 << 8)); /* SP_INV_HSO */
    if (  ((val == 0) && (pPolarCtrl->phy_polar_tx != PHY_POLARITY_NORMAL))
        ||((val != 0) && (pPolarCtrl->phy_polar_tx != PHY_POLARITY_INVERSE)))
    {
        RT_LOG(LOG_WARNING, (MOD_HAL|MOD_PORT), "unit=%u,port=%u,polar: tx regs inconsisten!", unit, port);
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_polar_set
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
phy_9300_polar_set(uint32 unit, rtk_port_t port, rtk_port_phyPolarCtrl_t *pPolarCtrl)
{
    uint32         data, rxVal, txVal;
    int32           ret;

    rxVal = (pPolarCtrl->phy_polar_rx == PHY_POLARITY_INVERSE) ? 1 : 0;
    txVal = (pPolarCtrl->phy_polar_tx == PHY_POLARITY_INVERSE) ? 1 : 0;

    /*10GR*/
    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 6, 2, &data)) != RT_ERR_OK)
        return ret;

    data = REG32_FIELD_SET(data, rxVal, 13, (0x1 << 13));   /* FP_TGR2_CFG_INV_HSI */
    data = REG32_FIELD_SET(data, txVal, 14, (0x1 << 14));   /* FP_TGR2_CFG_INV_HSO */
    if((ret = hal_serdes_reg_set(unit, HWP_PORT_SDSID(unit, port), 6, 2, data)) != RT_ERR_OK)
        return ret;

    /*Fiber1G*/
    if((ret = hal_serdes_reg_get(unit, HWP_PORT_SDSID(unit, port), 0, 0, &data)) != RT_ERR_OK)
        return ret;

    data = REG32_FIELD_SET(data, rxVal, 9, (0x1 << 9));   /* SP_INV_HSI */
    data = REG32_FIELD_SET(data, txVal, 8, (0x1 << 8));   /* SP_INV_HSO */
    if((ret = hal_serdes_reg_set(unit, HWP_PORT_SDSID(unit, port), 0, 0, data)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_portEyeMonitor_start
 * Description:
 *      port eye monitor start.
 * Input:
 *      port   - port id
 *      frameNum - frame number
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
int32
phy_9300_portEyeMonitor_start(uint32 unit, rtk_port_t port, uint32 frameNum)
{

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_9300_serdes_mapperInit
 * Description:
 *      Initialize PHY 9300 serdes driver.
 * Input:
 *      pPhydrv - pointer of phy driver
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void phy_9300_serdes_mapperInit(rt_phydrv_t *pPhydrv)
{
    pPhydrv->phydrv_index = RT_PHYDRV_RTL9300_SERDES;
    pPhydrv->fPhydrv_init = phy_9300_init;
    pPhydrv->fPhydrv_media_get = phy_9300_media_get;
    pPhydrv->fPhydrv_10gMedia_get = phy_9300_10gmedia_get;
    pPhydrv->fPhydrv_10gMedia_set = phy_9300_10gMedia_set;
    pPhydrv->fPhydrv_autoNegoEnable_get = phy_9300_autoNegoEnable_get;
    pPhydrv->fPhydrv_autoNegoEnable_set = phy_9300_autoNegoEnable_set;
    pPhydrv->fPhydrv_autoNegoAbility_get = phy_9300_autoNegoAbility_get;
    pPhydrv->fPhydrv_autoNegoAbility_set = phy_9300_autoNegoAbility_set;
    pPhydrv->fPhydrv_duplex_get = phy_9300_duplex_get;
    pPhydrv->fPhydrv_duplex_set = phy_9300_duplex_set;
    pPhydrv->fPhydrv_speed_get = phy_9300_speed_get;
    pPhydrv->fPhydrv_speed_set = phy_9300_speed_set;
    pPhydrv->fPhydrv_enable_set = phy_9300_enable_set;
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
    pPhydrv->fPhydrv_serdesFiberRx_check = phy_9300_serdesFiberRx_check;
    pPhydrv->fPhydrv_serdesFiberRx_reset = phy_9300_serdesFiberRx_reset;
    pPhydrv->fPhydrv_loopback_get = phy_9300_serdes_loopback_get;
    pPhydrv->fPhydrv_loopback_set = phy_9300_serdes_loopback_set;

    pPhydrv->fPhydrv_polar_get = phy_9300_polar_get;
    pPhydrv->fPhydrv_polar_set = phy_9300_polar_set;

    pPhydrv->fPhydrv_portEyeMonitor_start = phy_9300_portEyeMonitor_start;

} /* end of phy_9300_serdes_mapperInit*/

