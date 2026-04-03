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
 * $Revision$
 * $Date$
 *
 * Purpose : Definition of TIME API
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) IEEE 1588
 *
 */

#ifndef __RTK_TIME_H__
#define __RTK_TIME_H__

/*
 * Include Files
 */
#include <common/rt_type.h>


/*
 * Symbol Definition
 */

typedef enum rtk_time_clkOutMode_e
{
    PTP_CLK_OUT_REPEAT = 0,
    PTP_CLK_OUT_PULSE = 1,
    PTP_CLK_OUT_END
} rtk_time_clkOutMode_t;

typedef enum rtk_time_outSigSel_e
{
    PTP_OUT_SIG_SEL_CLOCK = 0,
    PTP_OUT_SIG_SEL_1PPS = 1,
    PTP_OUT_SIG_SEL_DISABLE = 2,
    PTP_OUT_SIG_SEL_END
} rtk_time_outSigSel_t;

typedef enum rtk_time_portRole_e
{
    PTP_PORT_ROLE_DISABLE = 0,
    PTP_PORT_ROLE_BC = 1,
    PTP_PORT_ROLE_E2E_TC = 2,
    PTP_PORT_ROLE_P2P_TC = 3,
    PTP_PORT_ROLE_END
} rtk_time_portRole_t;

typedef enum rtk_time_dir_e
{
    PTP_DIR_IGR = 0,
    PTP_DIR_EGR = 1,
    PTP_DIR_END
} rtk_time_dir_t;

typedef enum rtk_time_interrupt_e
{
    PTP_INTR_1PPS_TIME_LATCH = 0,
    PTP_INTR_TOD_DONE = 1,
    PTP_INTR_TX_TIME_FIFO = 2,
    PTP_INTR_END
} rtk_time_interrupt_t;




/* TIME transmission callback function prototype */
typedef int32 (rtk_time_ptpTime_cb_f)(
    uint32                      unit,
    rtk_port_t                  port,
    rtk_time_ptpIdentifier_t    identifier,
    rtk_time_timeStamp_t        time);


typedef struct rtk_time_txTimeEntry_s
{
    uint8 valid;
    rtk_port_t port;
    rtk_time_ptpMsgType_t msg_type;
    uint32 seqId;
    rtk_time_timeStamp_t txTime;
} rtk_time_txTimeEntry_t;

typedef struct rtk_time_clkOutput_s
{
    rtk_time_clkOutMode_t mode;
    rtk_time_timeStamp_t startTime;
    uint32 halfPeriodNsec;
    uint32 halfPeriodFractionalNsec;
    rtk_enable_t enable;
    uint8 runing; //Only valid for get API
} rtk_time_clkOutput_t;

typedef struct rtk_time_todData_s
{
    uint32 word[8];
} rtk_time_todData_t;


/*
 * Data Declaration
 */

/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */

/* Module Name : TIME */

/* Function Name:
 *      rtk_time_init
 * Description:
 *      Initialize Time module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8380, 8390, 9310, 9311B
 * Note:
 *      Must initialize Time module before calling any Time APIs.
 * Changes:
 *      None
 */
extern int32
rtk_time_init(uint32 unit);

/* Function Name:
 *      rtk_time_portPtpTxTimestampCallback_register
 * Description:
 *      Register PTP transmission callback function of the PTP identifier on the dedicated port to the specified device.
 * Input:
 *      unit       - unit id
 *      port       - port id
 *      identifier - indentifier of PTP packet
 *      fCb        - callback function
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390
 * Note:
 *      None
 * Changes:
 *      None
 */
extern int32
rtk_time_portPtpTxTimestampCallback_register(
    uint32                      unit,
    rtk_port_t                  port,
    rtk_time_ptpIdentifier_t    identifier,
    rtk_time_ptpTime_cb_f       *fCb);


/* Function Name:
 *      rtk_time_portPtpRole_get
 * Description:
 *      Get PTP port role of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pPortRole - port role
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT         - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      None
 */
extern int32
rtk_time_portPtpRole_get(uint32 unit, rtk_port_t port, rtk_time_portRole_t *pPortRole);

/* Function Name:
 *      rtk_time_portPtpRole_set
 * Description:
 *      Set PTP port role of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      portRole - port role
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT     - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      None
 */
extern int32
rtk_time_portPtpRole_set(uint32 unit, rtk_port_t port, rtk_time_portRole_t portRole);

/* Function Name:
 *      rtk_time_portPtpLantecyCmpsn_get
 * Description:
 *      Get PTP ingress/egress latency compensation of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      dir     - ingress or egress
 * Output:
 *      pVal    - compensation time, unit: 2^-6 nsec
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT         - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      None
 */
extern int32
rtk_time_portPtpLantecyCmpsn_get(uint32 unit, rtk_port_t port, rtk_time_dir_t dir, uint32 *pVal);

/* Function Name:
 *      rtk_time_portPtpLantecyCmpsn_set
 * Description:
 *      Set PTP ingress/egress latency compensation of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      dir    - ingress or egress
 *      val   - compensation time, unit: 2^-6 nsec
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT     - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      None
 */
extern int32
rtk_time_portPtpLantecyCmpsn_set(uint32 unit, rtk_port_t port, rtk_time_dir_t dir, uint32 val);


/* Function Name:
 *      rtk_time_refTime_get
 * Description:
 *      Get the reference time of the device.
 * Input:
 *      unit       - unit id
 * Output:
 *      pTimeStamp - pointer buffer of the reference time
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_refTime_get(uint32 unit, rtk_time_timeStamp_t *pTimeStamp);

/* Function Name:
 *      rtk_time_refTime_set
 * Description:
 *      Set the reference time of the device.
 * Input:
 *      unit      - unit id
 *      timeStamp - reference timestamp value
 *      exec      - 0 : do not execute, 1: execute
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_refTime_set(uint32 unit, rtk_time_timeStamp_t timeStamp, uint32 exec);

/* Function Name:
 *      rtk_time_refTimeAdjust_set
 * Description:
 *      Adjust the reference time of the device.
 * Input:
 *      unit      - unit id
 *      sign      - significant
 *      timeStamp - reference timestamp value
 *      exec      - 0 : do not execute, 1: execute
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9330
 * Note:
 *      sign=0 for positive adjustment, sign=1 for negative adjustment.
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_refTimeAdjust_set(uint32 unit, uint32 sign, rtk_time_timeStamp_t timeStamp, uint32 exec);

/* Function Name:
 *      rtk_time_refTimeEnable_get
 * Description:
 *      Get the enable state of reference time of the device.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_refTimeEnable_get(uint32 unit, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_time_refTimeEnable_set
 * Description:
 *      Set the enable state of reference time of the device.
 * Input:
 *      unit   - unit id
 *      enable - status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_refTimeEnable_set(uint32 unit, rtk_enable_t enable);

/* Function Name:
 *      rtk_time_ptpOper_get
 * Description:
 *      Get the PTP time operation configuration of the device.
 * Input:
 *      unit    - unit id
 * Output:
 *      pOperCfg  - pointer to PTP time operation configuraton
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpOper_get(uint32 unit, rtk_time_operCfg_t *pOperCfg);

/* Function Name:
 *      rtk_time_ptpOper_set
 * Description:
 *      Set the PTP time operation configuration of the device.
 * Input:
 *      unit    - unit id
 *      pOperCfg  - pointer to PTP time operation configuraton
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpOper_set(uint32 unit, rtk_time_operCfg_t *pOperCfg);

/* Function Name:
 *      rtk_time_ptpLatchTime_get
 * Description:
 *      Get the PTP latched time of the device.
 * Input:
 *      unit    - unit id
 * Output:
 *      pOperCfg  - pointer to PTP time operation configuraton
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpLatchTime_get(uint32 unit, rtk_time_timeStamp_t *pLatchTime);

/* Function Name:
 *      rtk_time_ptpRefTimeFreqCfg_get
 * Description:
 *      Get the frequency of reference time of the device.
 * Input:
 *      unit        - unit id
 * Output:
 *      pFreqCfg    - pointer to configured reference time frequency
 *      pFreqCur    - pointer to current reference time frequency
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpRefTimeFreqCfg_get(uint32 unit, uint32 *pFreqCfg, uint32 *pFreqCur);

/* Function Name:
 *      rtk_time_ptpRefTimeFreqCfg_set
 * Description:
 *      Set the frequency of reference time of the device.
 * Input:
 *      unit        - unit id
 *      freq        - reference time frequency
 *      apply       - if the frequency is applied immediately
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpRefTimeFreqCfg_set(uint32 unit, uint32 freq, uint32 apply);

/* Function Name:
 *      rtk_time_ptpTxInterruptStatus_get
 * Description:
 *      Get the PTP related interrupt status of the device.
 * Input:
 *      unit        - unit id
 *      type        - interrupt type
 * Output:
 *      pIntrSts    - interrupt status of RX/TX PTP frame types
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpInterruptStatus_get(uint32 unit, rtk_time_interrupt_t type, uint32 *pIntrSts);

/* Function Name:
 *      rtk_time_ptpInterruptEnable_get
 * Description:
 *      Get the PTP related interrupt enable status of the device.
 * Input:
 *      unit    - unit id
 *      type    - interrupt type
 * Output:
 *      pEnable - status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpInterruptEnable_get(uint32 unit, rtk_time_interrupt_t type, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_time_ptpInterruptEnable_set
 * Description:
 *      Set the PTP TX timestamp FIFO non-empty interrupt enable status of the device.
 * Input:
 *      unit    - unit id
 *      type    - interrupt type
 *      enable  - status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpInterruptEnable_set(uint32 unit, rtk_time_interrupt_t type, rtk_enable_t enable);

/* Function Name:
 *      rtk_time_ptpTxTimestampFifo_get
 * Description:
 *      Get the top entry from PTP Tx timstamp FIFO on the device
 * Input:
 *      unit        - unit id
 * Output:
 *      pTimeEntry  - pointer buffer of TIME timestamp entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpTxTimestampFifo_get(uint32 unit, rtk_time_txTimeEntry_t *pTimeEntry);

/* Function Name:
 *      rtk_time_ptp1PPSOutput_get
 * Description:
 *      Get the 1 PPS output configuration of the device.
 * Input:
 *      unit        - unit id
 * Output:
 *      pPulseWidth - pointer to 1 PPS pulse width
 *      pEnable     - pointer to 1 PPS output enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptp1PPSOutput_get(uint32 unit, uint32 *pPulseWidth, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_time_ptp1PPSOutput_set
 * Description:
 *      Set the 1 PPS output configuration of the device.
 * Input:
 *      unit        - unit id
 *      pulseWidth  - pointer to 1 PPS pulse width
 *      enable      - enable 1 PPS output
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptp1PPSOutput_set(uint32 unit, uint32 pulseWidth, rtk_enable_t enable);

/* Function Name:
 *      rtk_time_ptpClockOutput_get
 * Description:
 *      Get the clock output configuration of the device.
 * Input:
 *      unit        - unit id
 * Output:
 *      pClkOutput  - pointer to clock output configuration
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpClockOutput_get(uint32 unit, rtk_time_clkOutput_t *pClkOutput);

/* Function Name:
 *      rtk_time_ptpClockOutput_set
 * Description:
 *      Set the clock output configuration of the device.
 * Input:
 *      unit        - unit id
 *      pClkOutput  - pointer to clock output configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpClockOutput_set(uint32 unit, rtk_time_clkOutput_t *pClkOutput);

/* Function Name:
 *      rtk_time_ptpOutputSigSel_get
 * Description:
 *      Get the output pin signal selection configuration of the device.
 * Input:
 *      unit        - unit id
 * Output:
 *      pOutSigSel  - pointer to output pin signal selection configuration
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpOutputSigSel_get(uint32 unit, rtk_time_outSigSel_t *pOutSigSel);

/* Function Name:
 *      rtk_time_ptpOutputSigSel_set
 * Description:
 *      Set the output pin signal selection configuration of the device.
 * Input:
 *      unit        - unit id
 *      outSigSel   - output pin signal selection configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpOutputSigSel_set(uint32 unit, rtk_time_outSigSel_t outSigSel);

/* Function Name:
 *      rtk_time_ptpTodCfg_get
 * Description:
 *      Get the TOD output configuration of the device.
 * Input:
 *      unit        - unit id
 * Output:
 *      pIsManual   - pointer to if TOD output is manual mode
 *      pDelay      - pointer to TOD send delay after 1PPS sent configuration
 *      pBaudRate   - pointer to TOD send baud rate
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpTodCfg_get(uint32 unit, uint8 *pIsManual, uint32 *pDelay, uint32 *pBaudRate);

/* Function Name:
 *      rtk_time_ptpTodCfg_set
 * Description:
 *      Set the TOD output configuration of the device.
 * Input:
 *      unit        - unit id
 *      isManual    - if TOD output is manual mode
 *      delay       - TOD send delay after 1PPS sent configuration
 *      baudRate    - TOD send baud rate
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpTodCfg_set(uint32 unit, uint8 isManual, uint32 delay, uint32 baudRate);

/* Function Name:
 *      rtk_time_ptpTodData_get
 * Description:
 *      Get the TOD output data configuration of the device.
 * Input:
 *      unit      - unit id
 * Output:
 *      pLen      - pointer to TOD output data length (byte)
 *      pData     - pointer to TOD output data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpTodData_get(uint32 unit, uint32 *pLen, rtk_time_todData_t *pData);

/* Function Name:
 *      rtk_time_ptpTodData_set
 * Description:
 *      Set the TOD output data configuration of the device.
 * Input:
 *      unit        - unit id
 *      len         - TOD output data length (byte)
 *      data        - TOD output data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpTodData_set(uint32 unit, uint32 len, rtk_time_todData_t data);

/* Function Name:
 *      rtk_time_ptpTodReferencePoint_get
 * Description:
 *      Get the TOD output auto mode reference point to assist ToD generation for the device.
 * Input:
 *      unit      - unit id
 * Output:
 *      pUtcSec   - pointer to reference point time in UTC sec (0 means 1970.1.1)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpTodReferencePoint_get(uint32 unit, uint32 *pUtcSec);

/* Function Name:
 *      rtk_time_ptpTodReferencePoint_set
 * Description:
 *      Set the TOD output auto mode reference point to assist ToD generation for the device.
 * Input:
 *      unit       - unit id
 *      utcSec     - reference point time in UTC sec (0 means 1970.1.1)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New API for switch side PTP function
 */
extern int32
rtk_time_ptpTodReferencePoint_set(uint32 unit, uint32 utcSec);


#endif /* __RTK_TIME_H__ */
