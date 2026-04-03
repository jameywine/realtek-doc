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
 * $Revision: 93380 $
 * $Date: 2018-11-08 17:49:37 +0800 (Thu, 08 Nov 2018) $
 *
 * Purpose : DRV APIs definition.
 *
 * Feature : GPIO relative API
 *
 */



/*
 * Include Files
 */
#include <common/rt_type.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <common/error.h>


#include <common/util/rt_util_time.h>
#include <rtcore/rtcore.h>
#include <private/drv/gpio/gpio.h>
#include <hwp/hw_profile.h>
#include <common/rtcore/rtcore_init.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      drv_gpio_isrStsShadow_get
 * Description:
 *      Get ISR status from shadow database
 * Input:
 *      unit - unit ID
 *      pin - internal GPIO pin ID
 * Output:
 *      pData - ISR status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
drv_gpio_isrStsShadow_get(uint32 unit, GPIO_INTERNAL_PIN_t pin, uint32 *pData)
{
    rtcore_ioctl_t dio;

    if (rtcore_dev_fd < 0)
        return RT_ERR_FAILED;

    dio.data[0] = unit;
    dio.data[1] = pin;

    if (ioctl(rtcore_dev_fd, RTCORE_GPIO_ISR_STS_SHADOW_GET, &dio) < 0)
    {
        return RT_ERR_FAILED;
    }

    *pData = dio.data[2];

    return dio.ret;
}



