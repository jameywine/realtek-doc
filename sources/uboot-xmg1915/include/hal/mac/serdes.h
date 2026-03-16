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

#ifndef __HAL_MAC_SERDES_H__
#define __HAL_MAC_SERDES_H__


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
extern int32
hal_serdes_reg_get(uint32 unit, uint32 sdsId, uint32 page, uint32 reg, uint32 *pData);

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
extern int32
hal_serdes_reg_set(uint32 unit, uint32 sdsId, uint32 page, uint32 reg, uint32 data);


#endif /* __HAL_MAC_SERDES_H__ */
