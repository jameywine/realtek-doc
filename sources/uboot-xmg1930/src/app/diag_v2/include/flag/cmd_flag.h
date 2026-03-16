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
 * $Revision: 75125 $
 * $Date: 2017-01-09 15:13:30 +0800 (Mon, 09 Jan 2017) $
 *
 * Purpose : include command flag file
 *
 * Feature : None
 *
 */

#ifndef __CMD_FLAG_H__
#define __CMD_FLAG_H__


/*
 * Include Files
 */
#include "cmd_flag_common.h"

#ifdef CONFIG_SDK_RTL8390
#ifndef CONFIG_SDK_FPGA_PLATFORM
#include "cmd_flag_rtl8390.h"
#endif
#endif  /* CONFIG_SDK_RTL8390 */

#ifdef CONFIG_SDK_RTL8380
#include "cmd_flag_rtl8380.h"
#endif  /* CONFIG_SDK_RTL8380 */

#ifdef CONFIG_SDK_RTL9310
#include "cmd_flag_rtl9310.h"
#endif  /* CONFIG_SDK_RTL9310 */

#ifdef CONFIG_SDK_RTL9300
#include "cmd_flag_rtl9300.h"
#endif  /* CONFIG_SDK_RTL9300 */

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */


/*
 * Macro Declaration
 */


/*
 * Function Declaration
 */

/* Module Name : diag */

#endif /* __CMD_FLAG_H__ */

