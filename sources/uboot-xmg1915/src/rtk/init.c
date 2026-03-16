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
 * $Revision: 73408 $
 * $Date: 2016-11-17 16:23:05 +0800 (Thu, 17 Nov 2016) $
 *
 * Purpose : Definition of Init API
 *
 * Feature : Initialize All Layers of RTK Module
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/debug/rt_log.h>
#include <hal/common/halctrl.h>
#include <dal/dal_mgmt.h>
#if defined(CONFIG_SDK_DRIVER_RTK_CUSTOMER)
#include <rtk/customer_hook.h>
#endif
#include <rtk/init.h>
#include <rtk/stack.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
 static uint32   rtk_init_status = INIT_NOT_COMPLETED;

/*
 * Function Declaration
 */

/* Function Name:
 *      rtk_init
 * Description:
 *      Initialize the specified device
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      INIT must be initialized before using all of APIs in each modules
 * Changes:
 *      None
 */
int32
rtk_init(void)
{
    int32 ret = RT_ERR_FAILED;

    RT_INIT_REENTRY_CHK(rtk_init_status);
    RT_INIT_MSG("RTK Driver Module Initialize\n");

    RT_LOG(LOG_EVENT, MOD_INIT, "rtk_init Start!!\n");

    /* Initialize the hal layer */
    if ((ret = hal_init()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_INIT, "hal_init failed!!\n");
        return ret;
    }
    RT_LOG(LOG_EVENT, MOD_INIT, "hal_init Completed!!\n");

    /* Initialize the dal layer */
    if ((ret = dal_mgmt_init()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_INIT, "dal_mgmt_init failed!!\n");
        return ret;
    }
    RT_LOG(LOG_EVENT, MOD_INIT, "dal_mgmt_init Completed!!\n");

    if ((ret = dal_mgmt_initDevice()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_INIT, "dal_mgmt_initDevice failed!!\n");
        return ret;
    }

    RT_LOG(LOG_EVENT, MOD_INIT, "dal_mgmt_initDevice Completed!!\n");

    /* Initialize the dal layer misc items */
#ifndef __BOOTLOADER__
    if ((ret = dal_mgmt_misc_init()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_INIT, "dal_mgmt_misc_init failed!!\n");
        return ret;
    }
    RT_LOG(LOG_EVENT, MOD_INIT, "dal_mgmt_misc_init Completed!!\n");

  #if defined(CONFIG_SDK_DRIVER_RTK_CUSTOMER)
    /*Customer RTK API initial process will be executed over here*/
    if ((ret = rtk_customer_api_init()) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_INIT, "rtk_customer_api_init failed!!\n");
        return ret;
    }
  #endif
#endif

    rtk_init_status = INIT_COMPLETED;

    return ret;

} /* end of rtk_init */

