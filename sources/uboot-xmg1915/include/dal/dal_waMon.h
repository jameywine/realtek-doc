/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 77038 $
 * $Date: 2017-03-31 10:49:34 +0800 (Fri, 31 Mar 2017) $
 *
 * Purpose : Monitor workaround status of each port
 *
 * Feature :
 *
 */
#ifndef __DAL_WA_MON_H__
#define __DAL_WA_MON_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>


/*
 * Symbol Definition
 */
#ifndef __BOOTLOADER__
    #define CONFIG_SDK_WA_FIBER_RX_WATCHDOG 1
  #if defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL9300)
    #define CONFIG_SDK_WA_PHY_WATCHDOG 1
    #define CONFIG_SDK_WA_SERDES_WATCHDOG 1
  #endif

  #if defined(CONFIG_SDK_RTL8390)
    #define CONFIG_SDK_WA_SERDES_FIBER_LINKDOWN_WATCHDOG 1
  #endif

  #if defined(CONFIG_SDK_RTL8380)  || defined(CONFIG_SDK_RTL9300)
    #define CONFIG_SDK_WA_PKTBUF_WATCHDOG 1
  #endif
  #if defined(CONFIG_SDK_RTL8380)
    #define CONFIG_SDK_WA_COMBO_FLOWCONTROL 1
  #endif
#endif

/*
 * Data Declaration
 */


/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Module Name : */

/* Function Name:
 *      dal_waMon_init
 * Description:
 *      Initial Workaround Monitor component
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED - initialize fail
 *      RT_ERR_OK     - initialize success
 * Note:
 *      None
 */
extern int32
dal_waMon_init(void);

/* Function Name:
 *      dal_waMon_enable
 * Description:
 *      Enable workaround monitor thread
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - scan interval is too small
 * Note:
 *      When enable workaround monitor thread
 *
 */
int32 dal_waMon_enable(void);

/* Function Name:
 *      dal_waMon_disable
 * Description:
 *      Disable workaround scan thread
 * Input:
 *      None.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      When disable workaround monitor thread
 */
extern int32
dal_waMon_disable(void);

/* Function Name:
 *      dal_waMon_phyReconfig_register
 * Description:
 *      Register callback function for PHY need to reconfigure notification
 * Input:
 *      phyReconfig_callback    - callback function for link change
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *
 */
extern int32
dal_waMon_phyReconfig_register(rtk_port_phyReconfig_callback_t phyReconfig_callback);


/* Function Name:
 *      dal_waMon_phyReconfig_unregister
 * Description:
 *      UnRegister callback function for PHY need to reconfigure notification
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *
 */
extern int32
dal_waMon_phyReconfig_unregister(void);

#endif /* __DAL_WA_MON_H__ */
