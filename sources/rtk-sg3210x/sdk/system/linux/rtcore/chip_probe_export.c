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
 * $Revision: 83481 $
 * $Date: 2017-11-15 15:45:28 +0800 (Wed, 15 Nov 2017) $
 *
 * Purpose : Realtek Switch SDK Core Module.
 *
 * Feature : Realtek Switch SDK Core Module
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <private/drv/swcore/chip_probe.h>


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

/* BSP functions */
EXPORT_SYMBOL(drv_swcore_ioalCB_register);
EXPORT_SYMBOL(drv_swcore_earlyInit_start);
EXPORT_SYMBOL(drv_swcore_earlyInit_end);
EXPORT_SYMBOL(drv_swcore_chipFamilyId_get);
EXPORT_SYMBOL(drv_swcore_cid_get);
EXPORT_SYMBOL(drv_swcore_cid_cmp);
EXPORT_SYMBOL(rtk_chip_type);
EXPORT_SYMBOL(rtk_chip_probe_info);
EXPORT_SYMBOL(drv_swcore_chip_probe);

