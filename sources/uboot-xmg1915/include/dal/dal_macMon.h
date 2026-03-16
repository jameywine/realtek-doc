/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 71708 $
 * $Date: 2016-09-19 11:31:17 +0800 (Mon, 19 Sep 2016) $
 *
 * Purpose : Monitor mac constraint workaround status of each port
 *
 * Feature :
 *
 */
#ifndef __DAL_MAC_MON_H__
#define __DAL_MAC_MON_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>

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

/* Module Name : */

/* Function Name:
 *      dal_macMon_init
 * Description:
 *      Initial Mac Constraint Workaround Monitor component
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
dal_macMon_init(void);

/* Function Name:
 *      dal_macMon_enable
 * Description:
 *      Enable mac constraint workaround monitor thread
 * Input:
 *      scan_interval_us        - scan interval in us.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - scan interval is too small
 * Note:
 *      When enable mac constraint workaround monitor thread
 *
 */
extern int32
dal_macMon_enable(uint32 scan_interval_us);

/* Function Name:
 *      dal_macMon_disable
 * Description:
 *      Disable mac constraint workaround scan thread
 * Input:
 *      None.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      When disable mac constraint workaround monitor thread
 */
extern int32
dal_macMon_disable(void);

#endif /* __DAL_MAC_MON_H__ */
