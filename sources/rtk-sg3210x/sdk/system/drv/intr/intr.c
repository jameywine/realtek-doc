/* Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : Definition of Interrupt control API
 *
 * Feature : The file includes the following modules
 *           (1) Interrupt
 *
 */


 /*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/debug/rt_log.h>
#include <osal/isr.h>
#include <drv/intr/intr.h>
#include <private/drv/intr/intr_mapper.h>
#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>
#include <dev_config.h>
#include <hwp/hw_profile.h>

/*
 * Symbol Definition
 */



/*
 * Data Declaration
 */
uint32      drv_intr_imrExtGpio0Data[RTK_MAX_NUM_OF_UNIT] = { 0 };
uint32      drv_intr_imrExtGpio1Data[RTK_MAX_NUM_OF_UNIT] = { 0 };


/*
 * Macro Declaration
 */



/*
 * Function Declaration
 */

/* Function Name:
 *      drv_intr_init
 * Description:
 *      Init the interrupt module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_intr_init(uint32 unit)
{
    RT_INIT_MSG("    Intr init (unit %u)\n", unit);
    RT_PARAM_CHK(((unit > RTK_MAX_UNIT_ID) || (unit != RTK_SOC_UNIT_ID)), RT_ERR_UNIT_ID);

    return INTR_CTRL(unit).init(unit);
}

/* Function Name:
 *      drv_intr_exit
 * Description:
 *      Deinit the interrupt module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310, 9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.2]
 */
int32
drv_intr_exit(uint32 unit)
{
    RT_INIT_MSG("    Intr exit (unit %u)\n", unit);
    RT_PARAM_CHK(((unit > RTK_MAX_UNIT_ID) || (unit != RTK_SOC_UNIT_ID)), RT_ERR_UNIT_ID);

    return INTR_CTRL(unit).exit(unit);
}

/* Function Name:
 *      drv_intr_swcore_init
 * Description:
 *      Init the swcore interrupt
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_intr_swcore_init(uint32 unit)
{
    RT_INIT_MSG("    Intr swcore init (unit %u)\n", unit);
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    return INTR_CTRL(unit).swcore_init(unit);
}

/* Function Name:
 *      drv_intr_swcore_exit
 * Description:
 *      Init the swcore interrupt
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310, 9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.2]
 */
int32
drv_intr_swcore_exit(uint32 unit)
{
    RT_INIT_MSG("    Intr swcore exit (unit %u)\n", unit);
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    return INTR_CTRL(unit).swcore_exit(unit);
}

/* Function Name:
 *      drv_intr_isrSts_get
 * Description:
 *      Get swcore Global ISR status
 * Input:
 *      unit - unit
 * Output:
 *      pisr_sts - ISR status bitmap. The ID is defined in drv_intr_isr_t.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          structure change: pisr_sts change from uint32 to drv_intr_isrBitmap_t.
 *      [SDK_3.7.0]
 *          New added function.
 */
int32
drv_intr_isrSts_get(uint32 unit, drv_intr_isrBitmap_t *pisr_sts)
{
    if (!INTR_CHK(unit) || (INTR_CTRL(unit).swcoreIsrSts_get == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    return INTR_CTRL(unit).swcoreIsrSts_get(unit, pisr_sts);
}

/* Function Name:
 *      drv_intr_swcoreImrExtGpioPinEnable_set
 * Description:
 *      SWCORE externel GPIO pin enable set
 * Input:
 *      unit - unit
 *      pin - externel GPIO pin number
 *      enable - enable or disable IMR
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.7.0]
 *          New added function.
 */
int32
drv_intr_swcoreImrExtGpioPinEnable_set(uint32 unit, uint32 pin, rtk_enable_t enable)
{
    int32   bit;
    uint32  *pData;

    if (pin < 32)
    {
        pData = &drv_intr_imrExtGpio0Data[unit];
        bit = pin;
    }
    else
    {
        pData = &drv_intr_imrExtGpio1Data[unit];
        bit = pin - 32;
    }

    if (enable == ENABLED)
    {
        *pData |= (0x1 << bit);
    }
    else
    {
        *pData &= ~(0x1 << bit);
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      drv_intr_imrEnable_set
 * Description:
 *      Enable/Disable a ISR
 * Input:
 *      unit - unit
 *      isrId - ISR function ID
 *      enable - enable or disable IMR
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.2]
 *          New added function.
 */
int32
drv_intr_imrEnable_set(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t enable)
{

    if (!INTR_CHK(unit) || (INTR_CTRL(unit).swcoreImrEnable_set == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    return INTR_CTRL(unit).swcoreImrEnable_set(unit, isrId, enable);
}

/* Function Name:
 *      drv_intr_swcoreSts_get
 * Description:
 *      Get ISR status by ISR function ID
 * Input:
 *      unit - unit
 *      isrId - ISR function ID
 * Output:
 *      pFunc_sts - ISR status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.2]
 *          New added function.
 */
int32
drv_intr_swcoreSts_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    if (!INTR_CHK(unit) || (INTR_CTRL(unit).swcoreSts_get == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    return INTR_CTRL(unit).swcoreSts_get(unit, isrId, pFunc_sts);
}


