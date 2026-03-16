/*
 * Copyright (C) 2009 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 74379 $
 * $Date: 2016-12-19 11:39:00 +0800 (Mon, 19 Dec 2016) $
 *
 * Purpose : PHY 8295 Driver APIs.
 *
 * Feature : PHY 8295 Driver APIs
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <osal/print.h>
#include <osal/time.h>
#include <osal/lib.h>
#include <soc/type.h>
#include <hal/common/halctrl.h>
#include <hal/mac/miim_common_drv.h>
#include <hal/phy/phy_rtl8218d.h>
#include <hal/phy/phy_construct.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
uint32 rtl8218d_0819_efuseOcpAddr[] =
    {
        0xBCDC,
        0xBCDE,
        0xBCE0,
        0xBCE2,
        0xBCAC,
        0xBCFC,
    };

/*
 * Macro Declaration
 */
#define UINT32_BITS_MASK(mBit, lBit)        (uint32)((((uint64)0x1 << (mBit - lBit + 1)) - 1) << lBit)
#define PHY_8218D_EFAD2PORT_LE49(_ef)       ((_ef - 2) / (sizeof(rtl8218d_0819_efuseOcpAddr)/sizeof(uint32)))
#define PHY_8218D_EFAD2OcpIdx_LE49(_ef)     ((_ef - 2) % (sizeof(rtl8218d_0819_efuseOcpAddr)/sizeof(uint32)))
#define PHY_8218D_OCP2PAGE_LE49(_d)         REG32_FIELD_GET(_d, 4, 0xFFF0)
#define PHY_8218D_OCP2REG_LE49(_d)          ((REG32_FIELD_GET(_d, 0, 0x000F) / 2) + 16)
#define PHY_8218D_OCP2PAGE_GE50(_d)         REG32_FIELD_GET(_d, 3, 0xFFF8)
#define PHY_8218D_OCP2REG_GE50(_d)          (REG32_FIELD_GET(_d, 0, 0x0007) + 16)


/*
 * Function Declaration
 */


int32
_phy_8218d_efuseSw_load_phy(uint32 unit, rtk_port_t  port, uint32 page, uint32 phy_reg, uint32 data)
{
    int32       ret;

    ret = hal_miim_park_write(unit, port, page, 0x1F, phy_reg, data);

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SWITCH), "U%u P%u 0x%x reg 0x%x data 0x%x\n", unit, port, page, phy_reg, data);
    return ret;
}


/* Function Name:
 *      _phy_8218d_efuse_read
 * Description:
 *      Read from eFUSE
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 *      efAddr    - address to read
 * Output:
 *      pData     - pointer buffer of read data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
_phy_8218d_efuse_read(uint32 unit, rtk_port_t basePort, uint32 efAddr, uint32 *pData)
{
    int32           ret;
    uint32          data = 0, dataReg30;
    uint32          rCnt = 0;
    uint32          maxPage;

    maxPage = HAL_MIIM_PAGE_ID_MAX(unit);

    /* keep reg.30 data */
    if ((ret = hal_miim_park_read(unit, basePort, 0, 0x1F, 30, &dataReg30)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_park_write(unit, basePort, maxPage, 0x1F, 30, 0x8)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_park_write(unit, basePort, maxPage, 0x1F, 31, 0x26E)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_park_write(unit, basePort, maxPage, 0x1F, 17, efAddr)) != RT_ERR_OK)
        return ret;

    if ((ret = hal_miim_park_write(unit, basePort, maxPage, 0x1F, 16, 0)) != RT_ERR_OK)
        return ret;


    while (1)
    {
        rCnt++;
        ret = hal_miim_park_read(unit, basePort, maxPage, 0x1F, 16, &data);
        if (ret != RT_ERR_OK)
        {
            RT_ERR(ret, (MOD_HAL), "unit %u port %u Read16", unit, basePort);
            return ret;
        }

        if (!(data & 0x0004))
        {
            break;
        }

        if (rCnt >= 10000)
        {
            ret = RT_ERR_TIMEOUT;
            RT_ERR(ret, (MOD_HAL), "unit %u port %u ReadTimeOut", unit, basePort);
            *pData = 0;
            goto ERR_RET;
        }
    }

    ret = hal_miim_park_read(unit, basePort, maxPage, 0x1F, 19, pData);
    if (ret != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_HAL), "unit %u port %u", unit, basePort);
    }

  ERR_RET:
    /* recover reg 30, park page */
    hal_miim_park_write(unit, basePort, maxPage, 0, 30, dataReg30);

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SWITCH), "U%u P%u efE%u data 0x%04x\n", unit, basePort, efAddr, *pData);

    return ret;
}


/* Function Name:
 *      _phy_8218d_efuseSw_load_le49
 * Description:
 *      software load eFUSE patch to PHY
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 *      entStart  - start address
 *      entEnd    - end address
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
_phy_8218d_efuseSw_load_le49(uint32 unit, rtk_port_t basePort, uint32 entStart, uint32 entEnd)
{
    uint32          ent;
    uint32          pmsk, data, phyPort;
    uint32          phyPage, phyReg;
    rtk_port_t      port;

    if (entStart == 0)
    {
        _phy_8218d_efuse_read(unit, basePort, 0, &data);
        pmsk = data & 0xFF00; /* bit15: port0, bit14: port1, ... bit8: port7,  */
    }
    else
    {
        pmsk = 0xFF00; /* set all ports */
    }

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SWITCH), "U%u P%u pmsk:%x\n", unit, basePort, pmsk);

    /* sel GPHY block */
    for (phyPort = 0; phyPort < HWP_PHY_BASE_PHY_MAX(unit, basePort); phyPort++)
    {
        hal_miim_park_write(unit, (basePort + phyPort), 0, 0x1F, 30, 0x1);
    }


    for (ent = ((entStart < 2) ? 2 : entStart);
         ent <= ((entEnd > 49) ? 49 : entEnd);
         ent++)
    {
        phyPort = PHY_8218D_EFAD2PORT_LE49(ent);
        if (!((0x8000 >> phyPort) & pmsk))
            continue;

        data = rtl8218d_0819_efuseOcpAddr[PHY_8218D_EFAD2OcpIdx_LE49(ent)];
        phyPage = PHY_8218D_OCP2PAGE_LE49(data);
        phyReg =  PHY_8218D_OCP2REG_LE49(data);
        port = basePort + phyPort;

        _phy_8218d_efuse_read(unit, basePort, ent, &data);

        _phy_8218d_efuseSw_load_phy(unit, port, phyPage, phyReg, data);
    }/* end for */

    return;
}


/* Function Name:
 *      _phy_8218d_efuseSw_load_a
 * Description:
 *      software load eFUSE patch to PHY
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 *      *pEnt     - current address
 * Output:
 *      *pEnt     - address to tne next
 * Return:
 *      None
 * Note:
 *      None
 */
void
_phy_8218d_efuseSw_load_a(uint32 unit, rtk_port_t basePort, rtk_port_t port, uint32 *pEnt, uint32 pageReg)
{
    uint32      data;
    uint32      phyPage, phyReg;

    phyPage = PHY_8218D_OCP2PAGE_GE50(pageReg);
    phyReg =  PHY_8218D_OCP2REG_GE50(pageReg);

    *pEnt = *pEnt + 1;
    _phy_8218d_efuse_read(unit, basePort, *pEnt, &data);

    _phy_8218d_efuseSw_load_phy(unit, port, phyPage, phyReg, data);

}


/* Function Name:
 *      _phy_8218d_efuseSw_load_reg30
 * Description:
 *      software load eFUSE patch to PHY
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 *      *pEnt     - current address
 * Output:
 *      *pEnt     - address to tne next
 * Return:
 *      None
 * Note:
 *      None
 */
void
_phy_8218d_efuseSw_load_reg30(uint32 unit, rtk_port_t basePort, uint32 *pEnt, rtk_port_t *pPort)
{
    uint32      data;
    uint32      phyPort;

    *pEnt = *pEnt + 1;
    _phy_8218d_efuse_read(unit, basePort, *pEnt, &data);

    if ((data & 0x000F) == 0x8) /* [3:0] */
    {
        *pPort = basePort;
        data = 0x8;
    }
    else
    {
        phyPort = REG32_FIELD_GET(data, 4, 0x00F0); /* [7:4] */
        *pPort = basePort + phyPort;
        data = REG32_FIELD_GET(data, 0, 0x000F); /* [3:0] */
    }

    /* sel block */
    _phy_8218d_efuseSw_load_phy(unit, *pPort, 0, 30, data);
}


/* Function Name:
 *      _phy_8218d_efuseSw_load_ge50
 * Description:
 *      software load eFUSE patch to PHY
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 *      entStart  - start address
 *      entEnd    - end address
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
void
_phy_8218d_efuseSw_load_ge50(uint32 unit, rtk_port_t basePort, uint32 entStart, uint32 entEnd)
{
    uint32      ent, data;
    uint32      port = basePort;

    for (ent = ((entStart <= 50) ? 50 : entStart);
         ent <= entEnd;
         ent++)
    {
        _phy_8218d_efuse_read(unit, basePort, ent, &data);

        if (data == 0xFFFF)
        {
            continue;
        }
        else if (data == 0x0000)
        {
            break;
        }
        else if (data == 0xFE00)
        {
            _phy_8218d_efuseSw_load_reg30(unit, basePort, &ent, &port);
        }
        else
        {
            _phy_8218d_efuseSw_load_a(unit, basePort, port, &ent, data);
        }
    }/* end for */

    return;
}

/* Function Name:
 *      _phy_8218d_efuseSw_load
 * Description:
 *      software load eFUSE patch to PHY
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
void
_phy_8218d_efuseSw_load(uint32 unit, rtk_port_t basePort)
{
    uint32      data, phyPort;
    uint32      entStart, entEnd;
    int32       ret;

    if ((ret = _phy_8218d_efuse_read(unit, basePort, 253, &data)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_HAL), "unit %u port %u", unit, basePort);
        return;
    }
    entStart = REG32_FIELD_GET(data, 8, 0xFF00);    /* 253[15:8] */
    entEnd = REG32_FIELD_GET(data, 0, 0x00FF);      /* 253[7:0] */

    RT_LOG(LOG_DEBUG, (MOD_HAL|MOD_SWITCH), "U%u P%u LoadPhy start:%u end:%u\n", unit, basePort, entStart, entEnd);
    if (entStart <= 49)
    {
        _phy_8218d_efuseSw_load_le49(unit, basePort, entStart, entEnd);
    }

    if (entEnd >= 50)
    {
        _phy_8218d_efuseSw_load_ge50(unit, basePort, entStart, entEnd);
    }

    /* Back to GPHY block, page 0 */
    for (phyPort = 0; phyPort < HWP_PHY_BASE_PHY_MAX(unit, basePort); phyPort++)
    {
        hal_miim_park_write(unit, (basePort + phyPort), 0, 0, 30, 0x1);
    }
}


/* Function Name:
 *      phy_8218d_efuseSwLoadEnable_get
 * Description:
 *      Read from eFUSE
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 * Output:
 *      enable    - ENABLED: swfotware load should be performed. DISABLED: no need for swfotware load
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_8218d_efuseSwLoadEnable_get(uint32 unit, rtk_port_t basePort, rtk_enable_t *enable)
{
    uint32      data, bit12, bit13, bit14, bit15;
    int32       ret;

    *enable = DISABLED;
    if ((ret = _phy_8218d_efuse_read(unit, basePort, 252, &data)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_HAL), "unit %u port %u", unit, basePort);
        return ret;
    }

    bit12 = REG32_FIELD_GET(data, 12, (0x1 << 12));
    bit13 = REG32_FIELD_GET(data, 13, (0x1 << 13));
    bit14 = REG32_FIELD_GET(data, 14, (0x1 << 14));
    bit15 = REG32_FIELD_GET(data, 15, (0x1 << 15));

    *enable = bit15 ^ bit14 ^ bit13 ^ bit12;

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_8218d_efuseSw_load
 * Description:
 *      software load eFUSE patch to PHY
 * Input:
 *      unit      - unit id
 *      basePort  - base MAC ID of the 8218D
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_8218d_efuseSw_load(uint32 unit, rtk_port_t basePort)
{

    _phy_8218d_efuseSw_load(unit, basePort);

    return RT_ERR_OK;
}

/* Function Name:
 *      sub_phy_8218d_patch_set
 * Description:
 *      Patch the PHY:8218B.
 * Input:
 *      unit   - unit id
 *      port   - PHY base port id
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
sub_phy_8218d_patch_set(uint32 unit, rtk_port_t port)
{
    hal_control_t   *pHalCtrl;
    int32   ret = RT_ERR_FAILED;
    int32   i;

    RT_PARAM_CHK(((port % PORT_NUM_IN_8218D) != 0), RT_ERR_PORT_ID);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    /*Re-Patch 8218d*/
    rtl8218d_config(unit, port);

    for (i = 0; i < PORT_NUM_IN_8218D; ++i)
        hal_ctrl[unit].pPhy_ctrl[port]->pPhydrv->fPhydrv_init(unit, port + i);

    ret = RT_ERR_OK;
    return ret;
} /* end of sub_phy_8218d_patch_set */


