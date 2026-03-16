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
 * $Revision: 81323 $
 * $Date: 2017-08-16 15:31:30 +0800 (Wed, 16 Aug 2017) $
 *
 * Purpose : chip symbol and data type definition in the SDK.
 *
 * Feature : chip symbol and data type definition
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/debug/rt_log.h>
#include <osal/print.h>
#include <ioal/ioal_init.h>
#include <ioal/mem32.h>
#if defined(CONFIG_SDK_RTL8390)
#include <private/drv/swcore/swcore_rtl8390.h>
#endif
#if defined(CONFIG_SDK_RTL8380)
#include <private/drv/swcore/swcore_rtl8380.h>
#endif
#if defined(CONFIG_SDK_RTL9300)
#include <private/drv/swcore/swcore_rtl9300.h>
#endif
#if defined(CONFIG_SDK_RTL9310)
#include <private/drv/swcore/swcore_rtl9310.h>
#endif
#if defined(RTK_UNIVERSAL_BSP)
#if defined(CONFIG_RTL8390_SERIES)
#include <private/drv/swcore/swcore_rtl8390.h>
#endif
#if defined(CONFIG_RTL8380_SERIES)
#include <private/drv/swcore/swcore_rtl8380.h>
#endif
#if defined(CONFIG_RTL9300_SERIES)
#include <private/drv/swcore/swcore_rtl9300.h>
#endif
#if defined(CONFIG_RTL9310_SERIES)
#include <private/drv/swcore/swcore_rtl9310.h>
#endif
#endif
#if defined(RTK_UNIVERSAL_BSP)
#include "swcore.h"
#else
#include <private/drv/swcore/swcore.h>
#endif
#include <hwp/hw_profile.h>
#include <private/drv/watchdog/watchdog_common.h>
#include <private/drv/watchdog/watchdog_mapper.h>
#include <private/drv/uart/uart_common.h>
#include <private/drv/uart/uart_mapper.h>
#include <private/drv/tc/tc_common.h>
#include <private/drv/tc/tc_mapper.h>




/*
 * Symbol Definition
 */

#if defined(RTK_UNIVERSAL_BSP)
#define SWCORE_VIRT_BASE    0xBB000000
#endif
/*
 * Data Declaration
 */
typedef int32 (*hal_get_chip_id_f)(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);


#if defined(RTK_UNIVERSAL_BSP)
#if defined(CONFIG_RTL8380_SERIES)
static int32 _bsp_drv_swcore_cid8380_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif
#if defined(CONFIG_RTL8390_SERIES)
static int32 _bsp_drv_swcore_cid8390_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif

#if defined(CONFIG_RTL9300_SERIES)
static int32 _bsp_drv_swcore_cid9300_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif
#if defined(CONFIG_RTL9310_SERIES)
static int32 _bsp_drv_swcore_cid9310_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif

static hal_get_chip_id_f func[] =
{
#if defined(CONFIG_RTL8380_SERIES)
    _bsp_drv_swcore_cid8380_get,
#endif
#if defined(CONFIG_RTL8390_SERIES)
    _bsp_drv_swcore_cid8390_get,
#endif
#if defined(CONFIG_RTL9300_SERIES)
    _bsp_drv_swcore_cid9300_get,
#endif
#if defined(CONFIG_RTL9310_SERIES)
    _bsp_drv_swcore_cid9310_get,
#endif
};


#else  /*SDK part*/
#if defined(CONFIG_SDK_RTL8380)
static int32 _drv_swcore_cid8380_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif
#if defined(CONFIG_SDK_RTL8390)
static int32 _drv_swcore_cid8390_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif
#if defined(CONFIG_SDK_RTL9300)
static int32 _drv_swcore_cid9300_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif
#if defined(CONFIG_SDK_RTL9310)
static int32 _drv_swcore_cid9310_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id);
#endif


#define CHIP_ID_INVALID (0xFFFFFFFF)

static hal_get_chip_id_f func[] =
{
#if defined(CONFIG_SDK_RTL8380)
        _drv_swcore_cid8380_get,
#endif
#if defined(CONFIG_SDK_RTL8390)
        _drv_swcore_cid8390_get,
#endif
#if defined(CONFIG_SDK_RTL9300)
        _drv_swcore_cid9300_get,
#endif
#if defined(CONFIG_SDK_RTL9310)
        _drv_swcore_cid9310_get,
#endif
};


#endif
/*
 * Macro Definition
 */
int rtk_chip_type[RTK_MAX_NUM_OF_UNIT]={0};
#define BSP_CHIP_TYPE_ASSIGN(unit, type)\
    do {\
        rtk_chip_type[unit] = type;\
    } while(0)


#if defined(RTK_UNIVERSAL_BSP)

#define RT_ERR_OK 0
#define RT_ERR_FAILED -1

#define ioal_mem32_read bsp_mem32_read
#define ioal_mem32_write bsp_mem32_write


/*
 * Function Declaration
 */

/* Function Name:
 *      bsp_mem32_read
 * Description:
 *      Get the value from register.
 * Input:
 *      unit - unit id
 *      addr - register address
 * Output:
 *      pVal - pointer buffer of the register value
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. Support single unit right now and ignore unit
 *      2. When we support the multiple chip in future, we will check the input unit
 */
int32
bsp_mem32_read(uint32 unit, uint32 addr, uint32 *pVal)
{
    int32 ret = RT_ERR_OK;

    /* Upper layer have check the unit, and don't need to check again */

    *pVal = MEM32_READ(SWCORE_VIRT_BASE | addr);

    return ret;
} /* end of bsp_mem32_read */


/* Function Name:
 *      bsp_mem32_write
 * Description:
 *      Set the value to register.
 * Input:
 *      unit - unit id
 *      addr - register address
 *      val  - the value to write register
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. Support single unit right now and ignore unit
 *      2. When we support the multiple chip in future, we will check the input unit
 */
int32 bsp_mem32_write(uint32 unit, uint32 addr, uint32 val)
{
    int32 ret = RT_ERR_OK;

    MEM32_WRITE(SWCORE_VIRT_BASE | addr, val);

    return ret;
} /* end of bsp_mem32_write */
#endif

/*
 * Function Declaration
 */
#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL8380_SERIES))
static int32
_bsp_drv_swcore_cid8380_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL8380))
static int32
_drv_swcore_cid8380_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL8380_SERIES)) || (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL8380))
{
    uint32 original_data_intRd;
    uint32 chipId;
    uint32 chipVersion;
    uint32 sub_version = 1;

    uint32 val = 0;

    /* parameter check */
    RT_INTERNAL_PARAM_CHK((NULL == pChip_id), RT_ERR_NULL_POINTER);
    RT_INTERNAL_PARAM_CHK((NULL == pChip_rev_id), RT_ERR_NULL_POINTER);

    if (ioal_mem32_read(unit, RTL8380_MODEL_NAME_INFO_ADDR, &chipId) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((((chipId>>16)&0xFFFF) != 0x8330) && (((chipId>>16)&0xFFFF) != 0x8332) &&
        (((chipId>>16)&0xFFFF) != 0x8380) && (((chipId>>16)&0xFFFF) != 0x8382))
        return RT_ERR_FAILED;

    ioal_mem32_read(unit, RTL8380_INT_RW_CTRL_ADDR, &val);
    original_data_intRd = val;
    val |= (0x3);
    ioal_mem32_write(unit, RTL8380_INT_RW_CTRL_ADDR, val);

    ioal_mem32_read(unit, RTL8380_MODEL_NAME_INFO_ADDR, &chipId);
    ioal_mem32_read(unit, RTL8380_EXT_VERSION_ADDR, &chipVersion);
    chipVersion &= 0x1f;

    /* Check sub-version */
    ioal_mem32_read(unit, RTL8380_MODE_DEFINE_CTL_ADDR, &sub_version);
    sub_version = sub_version & (1UL<<23);

    ioal_mem32_write(unit, RTL8380_INT_RW_CTRL_ADDR, original_data_intRd);

    switch (chipId & 0xfffff800)
    {
        case RTL8330M_CHIP_ID & 0xfffff800:
            *pChip_id = RTL8330M_CHIP_ID;
            *pChip_rev_id = chipVersion == 2 ? CHIP_REV_ID_B : CHIP_REV_ID_C;
            break;
        case RTL8332M_CHIP_ID & 0xfffff800:
            *pChip_id = RTL8332M_CHIP_ID;
            *pChip_rev_id = chipVersion == 2 ? CHIP_REV_ID_B : CHIP_REV_ID_C;
            break;
        case RTL8380M_CHIP_ID & 0xfffff800:
            if(sub_version == 0)
            {
                *pChip_id = RTL8381M_CHIP_ID;
                *pChip_rev_id = chipVersion == 2 ? CHIP_REV_ID_B  : CHIP_REV_ID_C;
            }
            else
            {
                *pChip_id = RTL8380M_CHIP_ID;
                *pChip_rev_id = chipVersion == 2 ? CHIP_REV_ID_B : CHIP_REV_ID_C;
            }
            break;
        case RTL8382M_CHIP_ID & 0xfffff800:
            *pChip_id = RTL8382M_CHIP_ID;
            *pChip_rev_id = chipVersion == 2 ? CHIP_REV_ID_B : CHIP_REV_ID_C;
            break;
        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
} /* end of _drv_swcore_cid8380_get */
#endif


#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL8390_SERIES))
static int32
_bsp_drv_swcore_cid8390_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL8390))
static int32
_drv_swcore_cid8390_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL8390_SERIES)) || (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL8390))
{
    uint32 model_info = 0;
#if !defined(__MODEL_USER__)
    uint32 tmp = 0;
#endif

    /* parameter check */
    RT_INTERNAL_PARAM_CHK((NULL == pChip_id), RT_ERR_NULL_POINTER);
    RT_INTERNAL_PARAM_CHK((NULL == pChip_rev_id), RT_ERR_NULL_POINTER);

#if defined(__MODEL_USER__)
    *pChip_id = model_info = RTL8393M_CHIP_ID;
    *pChip_rev_id = CHIP_REV_ID_A;
#else
    ioal_mem32_read(unit, RTL8390_MODEL_NAME_INFO_ADDR, &model_info);
    tmp = (model_info & 0x3E) >> 1;
    model_info &= 0xFFFFFFC0;

    switch (model_info)
    {
        case RTL8391M_CHIP_ID:
            *pChip_id = RTL8391M_CHIP_ID;
            break;

        case RTL8392M_CHIP_ID:
            if (tmp == 0)
            {
                *pChip_id = RTL8392MES_CHIP_ID;
                BSP_CHIP_TYPE_ASSIGN(unit,CHIP_TYPE_1);
            }
            else
            {
                *pChip_id = RTL8392M_CHIP_ID;
            }
            break;
        case RTL8393M_CHIP_ID:
            if (tmp == 0)
            {
                *pChip_id = RTL8393MES_CHIP_ID;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            else
            {
                *pChip_id = RTL8393M_CHIP_ID;
            }
            break;
        case RTL8396M_CHIP_ID:
            if (tmp == 0)
            {
                *pChip_id = RTL8396MES_CHIP_ID;
                BSP_CHIP_TYPE_ASSIGN(unit,CHIP_TYPE_1);
            }
            else
            {
                *pChip_id = RTL8396M_CHIP_ID;
            }
            break;
        case RTL8351M_CHIP_ID:
            *pChip_id = RTL8351M_CHIP_ID;
            break;
        case RTL8352M_CHIP_ID:
            if (tmp == 0)
            {
                *pChip_id = RTL8352MES_CHIP_ID;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            else
            {
                *pChip_id = RTL8352M_CHIP_ID;
            }
            break;
        case RTL8353M_CHIP_ID:
            if (tmp == 0)
            {
                *pChip_id = RTL8353MES_CHIP_ID;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            else
            {
                *pChip_id = RTL8353M_CHIP_ID;
            }
            break;
        default:
            return RT_ERR_FAILED;
    }

    *pChip_rev_id = tmp;
#endif

    return RT_ERR_OK;
} /* end of _drv_swcore_cid8390_get */
#endif


#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL9300_SERIES))
static int32
_bsp_drv_swcore_cid9300_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL9300))
static int32
_drv_swcore_cid9300_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL9300_SERIES)) || (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL9300))
{
    uint32 model_info = 0;
    uint32 temp = 0;
    uint32 model_char_3rd = 0; /* 1: testchip; 0: formal chip */

    ioal_mem32_read(unit, RTL9300_MODEL_NAME_INFO_ADDR, &temp);
    model_info = temp & 0xFFFFFFC0;
    /* bit 6:4 */
    model_char_3rd = (temp & RTL9300_MODEL_NAME_INFO_MODEL_CHAR_3RD_MASK) >> RTL9300_MODEL_NAME_INFO_MODEL_CHAR_3RD_OFFSET;
    /* bit 3:0 */
    *pChip_rev_id = (temp & RTL9300_MODEL_NAME_INFO_RTL_VID_MASK) >> RTL9300_MODEL_NAME_INFO_RTL_VID_OFFSET;

    switch (model_info)
    {
        case RTL9301_CHIP_ID & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9301_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9301_CHIP_ID_24G;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9301_CHIP_ID_24G & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9301_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9301_CHIP_ID_24G;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9303_CHIP_ID & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9303_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9303_CHIP_ID_8XG;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9303_CHIP_ID_8XG & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9303_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9303_CHIP_ID_8XG;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9301H_CHIP_ID & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9301H_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9301H_CHIP_ID_4X2_5G;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9302A_CHIP_ID & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9302A_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9302A_CHIP_ID_12X2_5G;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9302B_CHIP_ID & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9302B_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9302B_CHIP_ID_8X2_5G;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9302C_CHIP_ID & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9302C_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9302C_CHIP_ID_16X2_5G;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;
        case RTL9302D_CHIP_ID & 0xFFFFFFC0:
            if(0 == model_char_3rd)
            {
                *pChip_id = RTL9302D_CHIP_ID;
                if( *pChip_rev_id > RTL9300_CHIP_ID_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_LASTEST_REV;
            }
            else
            {
                *pChip_id = RTL9302D_CHIP_ID_24X2_5G;
                if( *pChip_rev_id > RTL9300_CHIP_ID_ES_LASTEST_REV)
                    *pChip_rev_id = RTL9300_CHIP_ID_ES_LASTEST_REV;
                BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
            }
            break;

        default:
            osal_printf("swcore probe, in %s line %d, %x\n", __FILE__, __LINE__, model_info);
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
} /* end of _drv_swcore_cid9300_get */
#endif


#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL9310_SERIES))
static int32
_bsp_drv_swcore_cid9310_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL9310))
static int32
_drv_swcore_cid9310_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
#endif
#if (defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_RTL9310_SERIES)) || (!defined(RTK_UNIVERSAL_BSP) && defined(CONFIG_SDK_RTL9310))
{
    uint32 model_info = 0;

#if defined(__MODEL_USER__)
    *pChip_id = model_info = RTL9310_CHIP_ID;
    *pChip_rev_id = CHIP_REV_ID_A;
#else
    ioal_mem32_read(unit, RTL9310_MODEL_NAME_INFO_ADDR, &model_info);


    if ((model_info >> 4) & 0x1)
    {
        BSP_CHIP_TYPE_ASSIGN(unit, CHIP_TYPE_1);
    }

    *pChip_rev_id = model_info & 0xF;

    model_info &= 0xFFFF0000;
    switch (model_info)
    {
        case RTL9310_CHIP_ID & 0xffff0000:
            *pChip_id = RTL9310_CHIP_ID;
            break;
        case RTL9311_CHIP_ID & 0xffff0000:
            *pChip_id = RTL9311_CHIP_ID;
            break;
        case RTL9313_CHIP_ID & 0xffff0000:
            *pChip_id = RTL9313_CHIP_ID;
            break;
        default:
            osal_printf("%s %x\n", __func__, model_info);
            return RT_ERR_FAILED;
    }
#endif

    return RT_ERR_OK;
} /* end of _bsp_drv_swcore_cid9310_get */
#endif


/* Function Name:
 *      drv_swcore_cid_get
 * Description:
 *      Get chip id and chip revision id.
 * Input:
 *      unit           - unit id
 * Output:
 *      pChip_id       - pointer buffer of chip id
 *      pChip_rev_id   - pointer buffer of chip revision id
 * Return:
 *      RT_ERR_OK      - OK
 *      RT_ERR_UNIT_ID - invalid unit id
 *      RT_ERR_NULL_POINTER
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_swcore_cid_get(uint32 unit, uint32 *pChip_id, uint32 *pChip_rev_id)
{
    uint32  i;
    hal_get_chip_id_f f;

    /* parameter check */
    RT_PARAM_CHK((NULL == pChip_id), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pChip_rev_id), RT_ERR_NULL_POINTER);

    for (i = 0; i < (sizeof(func)/sizeof(hal_get_chip_id_f)); i++)
    {
        f = (hal_get_chip_id_f) func[i];
        if (RT_ERR_OK == ((f)(unit, pChip_id, pChip_rev_id)))
            return RT_ERR_OK;
    }

    return RT_ERR_FAILED;

} /* end of drv_swcore_cid_get */


/* Function Name:
 *      drv_swcore_cid_cmp
 * Description:
 *      Compare cmp_id with the chip id of unit
 * Input:
 *      unit           - unit id
 *      cmp_id         - chip id for compare
 * Output:
 *      None
 * Return:
 *      0              - identical
 *      RT_ERR_FAILED  - not identical
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_swcore_cid_cmp(uint32 unit, uint32 cmp_id)
{
    uint32 cid = 0, crid = 0;

    if (RT_ERR_OK != drv_swcore_cid_get(unit, &cid, &crid))
        return RT_ERR_FAILED;

    if (cmp_id == cid)
        return 0;
    else
        return RT_ERR_FAILED;

} /* end of drv_swcore_cid_cmp */

#if !defined(RTK_UNIVERSAL_BSP)
#if defined(CONFIG_SDK_RTL8390)
static int32
_drv_swcore_jtag_intf_8390_set(uint32 unit, drv_swcore_jtag_intf_t intf_sel)
{
    ioal_mem32_write(unit, RTL8390_MAC_IF_CTRL_ADDR, ((intf_sel) & RTL8390_MAC_IF_CTRL_JTAG_SEL_MASK));
    return RT_ERR_OK;
}
#endif

#if defined(CONFIG_SDK_RTL8380)
static int32
_drv_swcore_jtag_intf_8380_set(uint32 unit, drv_swcore_jtag_intf_t intf_sel)
{
    ioal_mem32_write(unit, RTL8380_GMII_INTF_SEL_ADDR, ((intf_sel) & RTL8380_GMII_INTF_SEL_GMII_IF_SEL_MASK));
    return RT_ERR_OK;
}
#endif

#if defined(CONFIG_SDK_RTL9300)
static int32
_drv_swcore_jtag_intf_9300_set(uint32 unit, drv_swcore_jtag_intf_t intf_sel)
{
    ioal_mem32_write(unit, RTL9300_JTAG_SEL_CTRL_ADDR, ((intf_sel) & RTL9300_JTAG_SEL_CTRL_JTAG_SEL_MASK));
    return RT_ERR_OK;
}
#endif

/* Function Name:
 *      drv_swcore_jtag_intf_set
 * Description:
 *      Set the usage of JTAG interface.
 * Input:
 *      unit           - unit id
 *      intf_sel       - interface type
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK      - OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_swcore_jtag_intf_set(uint32 unit, drv_swcore_jtag_intf_t intf_sel)
{

    switch(HWP_CHIP_FAMILY_ID(unit))
    {
   #if defined(CONFIG_SDK_RTL8390)
    case RTL8390_FAMILY_ID:
    case RTL8350_FAMILY_ID:
        _drv_swcore_jtag_intf_8390_set(unit, intf_sel);
        break;
   #endif
   #if defined(CONFIG_SDK_RTL8380)
    case RTL8380_FAMILY_ID:
    case RTL8330_FAMILY_ID:
        _drv_swcore_jtag_intf_8380_set(unit, intf_sel);
        break;
   #endif
   #if defined(CONFIG_SDK_RTL9300)
    case RTL9300_FAMILY_ID:
        _drv_swcore_jtag_intf_9300_set(unit, intf_sel);
        break;
   #endif
    default:
        return RT_ERR_FAILED;
    }
    return RT_ERR_OK;

} /* end of drv_swcore_jtag_intf_set */

#if defined(CONFIG_SDK_RTL8390)
static int32
_drv_swcore_jtag_intf_8390_get(uint32 unit, drv_swcore_jtag_intf_t * pIntf_sel)
{
    uint32 reg_value;
    ioal_mem32_read(unit, RTL8390_MAC_IF_CTRL_ADDR, &reg_value);
    *pIntf_sel = ((reg_value) & RTL8390_MAC_IF_CTRL_JTAG_SEL_MASK);
    return RT_ERR_OK;
}
#endif

#if defined(CONFIG_SDK_RTL8380)
static int32
_drv_swcore_jtag_intf_8380_get(uint32 unit, drv_swcore_jtag_intf_t * pIntf_sel)
{
    uint32 reg_value;
    ioal_mem32_read(unit, RTL8380_GMII_INTF_SEL_ADDR, &reg_value);
    *pIntf_sel = ((reg_value) & RTL8380_GMII_INTF_SEL_GMII_IF_SEL_MASK);
    return RT_ERR_OK;
}
#endif

#if defined(CONFIG_SDK_RTL9300)
static int32
_drv_swcore_jtag_intf_9300_get(uint32 unit, drv_swcore_jtag_intf_t * pIntf_sel)
{
    uint32 reg_value;
    ioal_mem32_read(unit, RTL9300_JTAG_SEL_CTRL_ADDR, &reg_value);
    *pIntf_sel = ((reg_value) & RTL9300_JTAG_SEL_CTRL_JTAG_SEL_MASK);
    return RT_ERR_OK;
}
#endif

/* Function Name:
 *      drv_swcore_jtag_intf_get
 * Description:
 *      Get the configuration of JTAG interface.
 * Input:
 *      unit           - unit id
 *      intf_sel       - interface type
 * Output:
 *      intf_sel       - interface type
 * Return:
 *      RT_ERR_OK      - OK
 *      RT_ERR_FAILED
 *      RT_ERR_NULL_POINTER
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_swcore_jtag_intf_get(uint32 unit, drv_swcore_jtag_intf_t * pIntf_sel)
{
    drv_swcore_jtag_intf_t intf_conf;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf_sel), RT_ERR_NULL_POINTER);

    switch(HWP_CHIP_FAMILY_ID(unit))
    {
   #if defined(CONFIG_SDK_RTL8390)
    case RTL8390_FAMILY_ID:
    case RTL8350_FAMILY_ID:
        _drv_swcore_jtag_intf_8390_get(unit, &intf_conf);
        break;
   #endif
   #if defined(CONFIG_SDK_RTL8380)
    case RTL8380_FAMILY_ID:
    case RTL8330_FAMILY_ID:
        _drv_swcore_jtag_intf_8380_get(unit, &intf_conf);
        break;
   #endif
   #if defined(CONFIG_SDK_RTL9300)
    case RTL9300_FAMILY_ID:
        _drv_swcore_jtag_intf_9300_get(unit, &intf_conf);
        break;
   #endif
    default:
        return RT_ERR_FAILED;
    }
    *pIntf_sel = intf_conf;
    return RT_ERR_OK;

} /* end of drv_swcore_jtag_intf_get */

/* Function Name:
 *      drv_swcore_family_cid_get
 * Description:
 *      Get Chip Family ID
 * Input:
 *      unit            - unit id
 * Output:
 *      pFamily_id      - Family_id
 * Return:
 *      RT_ERR_OK       - OK
 *      RT_ERR_FAILED   - Failed
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_swcore_family_cid_get(uint32 unit, uint32 * pFamily_id)
{
#if defined(CONFIG_SDK_RTL9310)
    if(HWP_9310_FAMILY_ID(unit))
    {
        *pFamily_id = RTL9310_FAMILY_ID;
    }
    else
#endif
#if defined(CONFIG_SDK_RTL9300)
    if(HWP_9300_FAMILY_ID(unit))
    {
        *pFamily_id = RTL9300_FAMILY_ID;
    }
    else
#endif
#if defined(CONFIG_SDK_RTL8390)
    if(HWP_8390_FAMILY_ID(unit))
    {
        *pFamily_id = RTL8390_FAMILY_ID;
    }
    else
    if(HWP_8350_FAMILY_ID(unit))
    {
        *pFamily_id = RTL8350_FAMILY_ID;
    }
    else
#endif
#if defined(CONFIG_SDK_RTL8380)
    if(HWP_8380_FAMILY_ID(unit))
    {
        *pFamily_id = RTL8380_FAMILY_ID;
    }
    else
    if(HWP_8330_FAMILY_ID(unit))
    {
        *pFamily_id = RTL8330_FAMILY_ID;
    }
    else
#endif
    {
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;

} /* end of drv_swcore_family_cid_get */


int32
drv_swcore_CPU_freq_get_8390(uint32 unit, uint32 * pCPU_freq)
{
    uint32 pll_value0 = 0, pll_value1 = 0;

    ioal_mem32_read(unit, RTL839x5x_CPU_PLL0, &pll_value0);
    ioal_mem32_read(unit, RTL839x5x_CPU_PLL1, &pll_value1);
    *pCPU_freq = 0;

    if((pll_value0 == RTL839x5x_CPU_750M_PLL0) && (pll_value1 == RTL839x5x_CPU_750M_PLL1))
        *pCPU_freq = 750;
    if((pll_value0 == RTL839x5x_CPU_700M_PLL0) && (pll_value1 == RTL839x5x_CPU_700M_PLL1))
        *pCPU_freq = 700;
    if((pll_value0 == RTL839x5x_CPU_650M_PLL0) && (pll_value1 == RTL839x5x_CPU_650M_PLL1))
        *pCPU_freq = 650;

    if(*pCPU_freq != 0)
        return RT_ERR_OK;

    return RT_ERR_FAILED;
}

int32
drv_swcore_CPU_freq_get_8380(uint32 unit, uint32 * pCPU_freq)
{
    uint32  pll_value0 = 0, pll_value1 = 0;
    uint32  enable_value = 0, pll_value2 = 0;

    enable_value = 0x3;
    ioal_mem32_write(unit, RTL838x3x_CPU_READ_CTL, enable_value);
    ioal_mem32_read(unit, RTL838x3x_CPU_CTL0, &pll_value0);
    ioal_mem32_read(unit, RTL838x3x_CPU_CTL1, &pll_value1);
    ioal_mem32_read(unit, RTL838x3x_CPU_MISC, &pll_value2);
    enable_value = 0x0;
    ioal_mem32_write(unit, RTL838x3x_CPU_READ_CTL, enable_value);
    *pCPU_freq = 0;
    if((pll_value0 == RTL838x3x_CPU_500M_CTL0) && (pll_value1 == RTL838x3x_CPU_500M_CTL1)  && (pll_value2 ==RTL838x3x_CPU_500M_MISC))
        *pCPU_freq = 500;
    if((pll_value0 == RTL838x3x_CPU_300M_CTL0) && (pll_value1 == RTL838x3x_CPU_300M_CTL1)  && (pll_value2 ==RTL838x3x_CPU_300M_MISC))
        *pCPU_freq = 300;

    if(*pCPU_freq == 0)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

int32
drv_swcore_CPU_freq_get_9310(uint32 unit, uint32 * pCPU_freq)
{
    return RT_ERR_OK;
}

int32
drv_swcore_CPU_freq_get_9300(uint32 unit, uint32 * pCPU_freq)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      drv_swcore_CPU_freq_get
 * Description:
 *      Get Chip CPU Frequency
 * Input:
 *      unit           - unit id
 * Output:
 *      pCPU_freq       - CPU_freq
 * Return:
 *      RT_ERR_OK        - OK
 *      RT_ERR_FAILED   - Failed
 *      RT_ERR_CHIP_NOT_SUPPORTED - Not support by this API
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_swcore_CPU_freq_get(uint32 unit, uint32 * pCPU_freq)
{

#if defined(CONFIG_SDK_RTL8390)
    if(HWP_8390_50_FAMILY(unit))
    {
        return( drv_swcore_CPU_freq_get_8390(unit,pCPU_freq));
    }
#endif


#if defined(CONFIG_SDK_RTL8380)
    if(HWP_8380_30_FAMILY(unit))
    {
        return( drv_swcore_CPU_freq_get_8380(unit,pCPU_freq));
    }
#endif

#if defined(CONFIG_SDK_RTL9310)

#endif

#if defined(CONFIG_SDK_RTL9300)

#endif

    return RT_ERR_CHIP_NOT_SUPPORTED;

} /* end of drv_swcore_CPU_freq_get */


int32
_drv_swcore_uart0RegBase_get(uint32 unit, uint32 *pReg_base_offset, uint32 *pReg_num)
{
    if(HWP_8380_30_FAMILY(unit))
    {
        *pReg_base_offset = RTL8380_UART0_RX_TX_DIV_LSB_ADDR;
        *pReg_num     = 64;
    }
    else if(HWP_8390_50_FAMILY(unit))
    {
        *pReg_base_offset = RTL8390_UART0_RX_TX_DIV_L_ADDR;
        *pReg_num     = 64;
    }
    else if(HWP_9300_FAMILY_ID(unit))
    {
        *pReg_base_offset = RTL9300_UART0_PBR_THR_DLL_ADDR;
        *pReg_num     = 7;
    }
    else
    {
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

int32
_drv_swcore_gpioRegBase_get(uint32 unit, uint32 *pReg_base_offset, uint32 *pReg_num)
{
    if(HWP_8380_30_FAMILY(unit))
    {
        *pReg_base_offset = RTL8380_GPIO_AB_CTRL_ADDR;
        *pReg_num     = 64;
    }
    else if(HWP_8390_50_FAMILY(unit))
    {
        *pReg_base_offset = RTL8390_PABC_CNR_ADDR;
        *pReg_num     = 64;
    }
    else if(HWP_9300_FAMILY_ID(unit))
    {
        *pReg_base_offset = RTL9300_GPIO_NO_USE_0004_ADDR;
        *pReg_num     = 7;
    }
    else
    {
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

#define MAX_REG_IDX 64

#ifndef RTK_USERMODE_LKM
/* Function Name:
 *      drv_swcore_register_dump
 * Description:
 *      Dump Chip peripher registers
 * Input:
 *      unit           - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK        - OK
 *      RT_ERR_FAILED   - Failed
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_swcore_register_dump(uint32 unit)
{
    uint32 index, offset, reg_value;
    uint32 base_offset, reg_num;

    if(!HWP_IS_CPU_UNIT(unit))
    {
        osal_printf("The SoC registers of this chip are not memory mapped.\n");
        return RT_ERR_OK;
    }

    osal_printf("\nUART0 register dump:\n");
    if (_drv_swcore_uart0RegBase_get(unit, &base_offset, &reg_num) != RT_ERR_OK)
    {
        osal_printf("unable to obtain register base offset\n");
    }
    else
    {
        for(index = 0; index < reg_num; index++)
        {
            offset = (index*4) + base_offset;
            ioal_soc_mem32_read(unit, offset, &reg_value);

            osal_printf("(%08x) : %08x",(offset),reg_value);
            if((index%3)==2)
                osal_printf("\n");
            else
                osal_printf(", ");
        }
    }

#if defined(CONFIG_SDK_UART1)
    osal_printf("\nUART1 register dump:\n");
    for(index = 0; index < 64; index++)
    {
        if (UART_REG(unit).uart1_rx_tx_div_lsb_addr.reg_addr)
        {
            offset = (index*4) +  UART_REG(unit).uart1_rx_tx_div_lsb_addr.reg_addr;
            ioal_soc_mem32_read(unit, offset, &reg_value);

            osal_printf("(%08x) : %08x",(offset),reg_value);
            if((index%3)==2)
                osal_printf("\n");
            else
                osal_printf(", ");
        }
    }
#endif

#if defined(CONFIG_SDK_DRIVER_WATCHDOG)
    osal_printf("\nInterrupt register dump:\n");
    for(index = 0; index < 64; index++)
    {
        if (WDG_REG(unit).glbl_intr_msk.reg_addr)
        {
            offset = (index*4) + WDG_REG(unit).glbl_intr_msk.reg_addr;
            ioal_soc_mem32_read(unit, offset, &reg_value);

            osal_printf("(%08x) : %08x",(offset),reg_value);
            if((index%3)==2)
                osal_printf("\n");
            else
                osal_printf(", ");
        }
    }
#endif

#if defined(CONFIG_SDK_TC_DRV)
    osal_printf("\nTimer/Counter register dump:\n");
    for(index = 0; index < 64; index++)
    {
        if(tc_reg_DATA(unit, 0))
        {
            offset = (index*4) + tc_reg_DATA(unit, 0);
            ioal_soc_mem32_read(unit, offset, &reg_value);

            osal_printf("(%08x) : %08x",(offset),reg_value);
            if((index%3)==2)
                osal_printf("\n");
            else
                osal_printf(", ");
        }
    }
#endif

    osal_printf("\nGPIO register dump:\n");
    if (_drv_swcore_gpioRegBase_get(unit, &base_offset, &reg_num) != RT_ERR_OK)
    {
        osal_printf("unable to obtain register base offset\n");
    }
    else
    {
        for(index = 0; index < reg_num; index++)
        {
            offset = (index*4) + base_offset;
            ioal_soc_mem32_read(unit, offset, &reg_value);

            osal_printf("(%08x) : %08x",(offset),reg_value);
            if((index%3)==2)
                osal_printf("\n");
            else
                osal_printf(", ");
        }
    }
    osal_printf("\n");
    return RT_ERR_OK;

} /* end of drv_swcore_register_dump */
#endif //RTK_USERMODE_LKM
#endif

#if defined(RTK_UNIVERSAL_BSP)
/* Function Name:
 *      bsp_drv_swcore_cid_get
 * Description:
 *      Get chip id and chip revision id.
 * Input:
 *      unit           - unit id
 * Output:
 *      pChip_id       - pointer buffer of chip id
 *      pChip_rev_id   - pointer buffer of chip revision id
 * Return:
 *      RT_ERR_OK      - OK
 *      RT_ERR_UNIT_ID - invalid unit id
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
int
bsp_drv_swcore_cid_get(unsigned int unit, unsigned int *pCid, unsigned int *pCrevid)
{
    int ret;

    ret = (int)drv_swcore_cid_get((uint32)0, (uint32 *)pCid, (uint32 *)pCrevid);

    return ret;

} /* end of bsp_drv_swcore_cid_get */
#endif

