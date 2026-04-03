/*
 * Copyright (C) 2009-2022 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * Purpose : Definition those public SER APIs and its data type in the SDK.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) SER (Soft Error Recovery)
 */

#ifndef __RTK_SER_H__
#define __RTK_SER_H__


/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/rt_type.h>
#include <common/rt_error.h>

/*
 * Symbol Definition
 */

typedef enum rtk_ser_errorType_e
{
    SER_ERROR_TYPE_PARITY,
    SER_ERROR_TYPE_ECC_1BIT,
    SER_ERROR_TYPE_ECC_2BIT,
    SER_ERROR_TYPE_END
} rtk_ser_errorType_t;

typedef enum rtk_ser_recoverStatus_e
{
    SER_RECOVER_HW_DONE,  // ECC 1bit error auto recovery
    SER_RECOVER_SW_DONE,  // Recover from software table shadow
    SER_RECOVER_RST_DONE, // Reset
    SER_RECOVER_FAIL,     // Failed to recover
    SER_RECOVER_NO_NEED,  // Temperarily error
    SER_RECOVER_END
} rtk_ser_recoverStatus_t;

typedef enum rtk_ser_infoFlags_e
{
    SER_INFO_RECOVERED,  // mem is recovered
    SER_INFO_INTERNAL,   // mem can not be accessed by CPU or register is private
    SER_INFO_END
} rtk_ser_infoFlag_t;

typedef struct rtk_ser_memInfo_s
{
    uint32 mem_id;       // mem enum defined in chip view table list
    uint32 entry_index;  // error entry index
    rtk_ser_errorType_t error_type;
    rtk_bitmap_t flags[BITMAP_ARRAY_CNT(SER_INFO_END)]; /* bit 0 SER_INFO_RECOVERED:     mem is recovered
                                                         * bit 1 SER_INFO_INTERNAL:      mem can not be accessed by CPU */
    rtk_ser_recoverStatus_t status;
} rtk_ser_memInfo_t;

typedef struct rtk_ser_regInfo_s
{
    uint32 reg_addr;     // reg address
    rtk_ser_errorType_t error_type;
    rtk_bitmap_t flags[BITMAP_ARRAY_CNT(SER_INFO_END)]; /* bit 0 SER_INFO_RECOVERED:     mem is recovered
                                                         * bit 1 SER_INFO_INTERNAL:      register is private */
    rtk_ser_recoverStatus_t status;
} rtk_ser_regInfo_t;

typedef int32 (*rtk_ser_memError_callback_t)(uint32 unit, rtk_ser_memInfo_t info);
typedef int32 (*rtk_ser_regError_callback_t)(uint32 unit, rtk_ser_regInfo_t info);

/*
 * Macro Declaration
 */
#define RTK_MAX_NUM_OF_SER_CALLBACK      (4)

#define RTK_SER_INVALID_ENTRY_INDEX      (0xffffffff)

/*
 * Data Declaration
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      rtk_ser_init
 * Description:
 *      Initialize SER module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      Must initialize SER module before calling any SER APIs.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_init(uint32 unit);

/* Function Name:
 *      rtk_ser_exit
 * Description:
 *      Exit SER module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_exit(uint32 unit);

/* Function Name:
 *      rtk_ser_memErrorTest_set
 * Description:
 *      Set ser memory error test info and trigger test operation.
 * Input:
 *      unit        - unit id
 *      pInfo       - error info, .error_type is required, others is optional
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_ENTRY_INDEX  - invalid entry index
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_memErrorTest_set(uint32 unit, rtk_ser_memInfo_t *pInfo);

/* Function Name:
 *      rtk_ser_regErrorTest_set
 * Description:
 *      Set ser register error test info and trigger test operation.
 * Input:
 *      unit        - unit id
 *      pInfo       - error info, .error_type is required, others is optional
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_regErrorTest_set(uint32 unit, rtk_ser_regInfo_t *pInfo);

/* Function Name:
 *      rtk_ser_memErrorTestEnable_set
 * Description:
 *      Set ser mem error to test mode.
 * Input:
 *      unit        - unit id
 *      enable      - state enable
 *      error_type  - error type
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      Recommended test flow:
 *          1. set test mode ENABLED
 *          2. write, read table OR send packets to switch
 *          3. set test mode DISABLED
 *      After test mode is disabled, error will be handled.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_memErrorTestEnable_set(uint32 unit, rtk_enable_t enable, rtk_ser_errorType_t error_type);

/* Function Name:
 *      rtk_ser_regErrorTestEnable_set
 * Description:
 *      Set ser register error to test mode.
 * Input:
 *      unit        - unit id
 *      enable      - state enable
 *      error_type  - error type
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      Recommended test flow:
 *          1. set test mode ENABLED
 *          2. write, read table OR send packets to switch
 *          3. set test mode DISABLED
 *      After test mode is disabled, error will be handled.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_regErrorTestEnable_set(uint32 unit, rtk_enable_t enable, rtk_ser_errorType_t error_type);

/* Function Name:
 *      rtk_ser_monitorEnable_set
 * Description:
 *      Set ser monitor state.
 * Input:
 *      unit        - unit id
 *      enable      - state enable
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_monitorEnable_set(uint32 unit, rtk_enable_t enable);

/* Function Name:
 *      rtk_ser_monitorEnable_get
 * Description:
 *      Get ser monitor state.
 * Input:
 *      unit        - unit id
 * Output:
 *      enable      - state enable
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_monitorEnable_get(uint32 unit, rtk_enable_t *enable);

/* Function Name:
 *      rtk_ser_memErrorHandle_register
 * Description:
 *      Register mem error handle callback
 * Input:
 *      unit          - unit id
 *      callback_func - callback function
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_memErrorHandle_register(
    rtk_ser_memError_callback_t callback_func);

/* Function Name:
 *      rtk_ser_memErrorHandle_unregister
 * Description:
 *      Unregister mem error handle callback
 * Input:
 *      unit          - unit id
 *      callback_func - callback function
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_memErrorHandle_unregister(
    rtk_ser_memError_callback_t callback_func);

/* Function Name:
 *      rtk_ser_regErrorHandle_register
 * Description:
 *      Register register error handle callback
 * Input:
 *      unit          - unit id
 *      callback_func - callback function
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_regErrorHandle_register(
    rtk_ser_regError_callback_t callback_func);

/* Function Name:
 *      rtk_ser_regErrorHandle_unregister
 * Description:
 *      Unregister register error handle callback
 * Input:
 *      unit          - unit id
 *      callback_func - callback function
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      9311B
 * Note:
 *      None.
 * Changes:
 *      [SDK_4.0.3]
 *          New added function.
 */
extern int32
rtk_ser_regErrorHandle_unregister(
    rtk_ser_regError_callback_t callback_func);

#endif

