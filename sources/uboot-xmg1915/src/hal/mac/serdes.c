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
 * $Revision: 71708 $
 * $Date: 2016-09-19 11:31:17 +0800 (Mon, 19 Sep 2016) $
 *
 * Purpose : SerDes indirect access APIs in the SDK.
 *
 * Feature : SerDes indirect access APIs
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <hwp/hw_profile.h>
#include <hal/chipdef/driver.h>
#include <hal/common/halctrl.h>
#include <hal/mac/serdes.h>

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
 *      hal_serdes_reg_get
 * Description:
 *      Get SerDes register through indirect access
 * Input:
 *      unit    - unit id
 *      sdsId   - SerDes index
 *      page    - page
 *      reg     - register index/address
 * Output:
 *      pData   - register value
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 *      RT_ERR_CHIP_NOT_SUPPORTED - chip not support
 * Note:
 *      None
 */
int32
hal_serdes_reg_get(uint32 unit, uint32 sdsId, uint32 page, uint32 reg, uint32 *pData)
{
    hal_control_t   *pHalCtrl;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_CHIP_NOT_FOUND;

    if (MACDRV(pHalCtrl)->fMdrv_mac_serdes_read == NULL)
        return RT_ERR_CHIP_NOT_SUPPORTED;

    return MACDRV(pHalCtrl)->fMdrv_mac_serdes_read(unit, sdsId, page, reg, pData);
}


/* Function Name:
 *      hal_serdes_reg_set
 * Description:
 *      Set SerDes register through indirect access
 * Input:
 *      unit    - unit id
 *      sdsId   - SerDes index
 *      page    - page
 *      reg     - register index/address
 *      data    - register value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 *      RT_ERR_CHIP_NOT_SUPPORTED - chip not support
 * Note:
 *      None
 */
int32
hal_serdes_reg_set(uint32 unit, uint32 sdsId, uint32 page, uint32 reg, uint32 data)
{
    hal_control_t   *pHalCtrl;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_CHIP_NOT_FOUND;

    if (MACDRV(pHalCtrl)->fMdrv_mac_serdes_write == NULL)
        return RT_ERR_CHIP_NOT_SUPPORTED;

    return MACDRV(pHalCtrl)->fMdrv_mac_serdes_write(unit, sdsId, page, reg, data);
}



