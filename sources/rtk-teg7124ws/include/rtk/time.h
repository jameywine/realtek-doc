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

#define RTK_TIME_NSEC_MAX 999999999

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
 *      8380, 8390, 9310
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
 *      rtk_time_portRefTime_get
 * Description:
 *      Get the reference time of the specified port.
 * Input:
 *      unit       - unit id
 *      port       - port id, it should be base port of PHY
 * Output:
 *      pTimeStamp - pointer buffer of the reference time
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [3.2.0]
 *          Change name from rtk_time_refTime_get.
 *          Add port parameter.
 */
extern int32
rtk_time_portRefTime_get(uint32 unit, rtk_port_t port, rtk_time_timeStamp_t *pTimeStamp);

/* Function Name:
 *      rtk_time_portRefTime_set
 * Description:
 *      Set the reference time of the specified portmask.
 * Input:
 *      unit      - unit id
 *      portmask    - portmask, it should be base ports of PHYs
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
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [3.2.0]
 *          Change name from rtk_time_refTime_set.
 *          Add portmask parameter.
 */
extern int32
rtk_time_portRefTime_set(uint32 unit, rtk_portmask_t portmask, rtk_time_timeStamp_t timeStamp, uint32 exec);

/* Function Name:
 *      rtk_time_portRefTimeAdjust_set
 * Description:
 *      Adjust the reference time of portmask.
 * Input:
 *      unit      - unit id
 *      portmask    - portmask, it should be base ports of PHYs
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
 *      8380, 8390, 9310
 * Note:
 *      sign=0 for positive adjustment, sign=1 for negative adjustment.
 * Changes:
 *      [3.2.0]
 *          Change name from rtk_time_refTimeAdjust_set.
 *          Add portmask parameter.
 */
extern int32
rtk_time_portRefTimeAdjust_set(uint32 unit, rtk_portmask_t portmask, uint32 sign, rtk_time_timeStamp_t timeStamp, uint32 exec);

/* Function Name:
 *      rtk_time_portRefTimeEnable_get
 * Description:
 *      Get the enable state of reference time of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id, it should be base port of PHY
 * Output:
 *      pEnable - status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [3.2.0]
 *          Change name from rtk_time_refTimeEnable_get.
 *          Add port parameter.
 */
extern int32
rtk_time_portRefTimeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_time_portRefTimeEnable_set
 * Description:
 *      Set the enable state of reference time of the specified portmask.
 * Input:
 *      unit   - unit id
 *      portmask    - portmask, it should be base ports of PHYs
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
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [3.2.0]
 *          Change name from rtk_time_refTimeEnable_set.
 *          Add portmask parameter.
 */
extern int32
rtk_time_portRefTimeEnable_set(uint32 unit, rtk_portmask_t portmask, rtk_enable_t enable);

/* Function Name:
 *      rtk_time_portRefTimeFreq_get
 * Description:
 *      Get the frequency of PTP reference time of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id, it should be base port of PHY
 * Output:
 *      pFreq  - pointer to reference time frequency
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      The frequency configuration decides the reference time tick frequency.
 *      The default value is 0x80000000.
 *      If it is configured to 0x40000000, the tick frequency would be half of default.
 *      If it is configured to 0xC0000000, the tick frequency would be one and half times of default.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 *      [3.2.0]
 *          Change name from rtk_time_refTimeFreq_get.
 *          Add port parameter.
 */
extern int32
rtk_time_portRefTimeFreq_get(uint32 unit, rtk_port_t port, uint32 *pFreq);


/* Function Name:
 *      rtk_time_portRefTimeFreq_set
 * Description:
 *      Set the frequency of PTP reference time of the specified portmask.
 * Input:
 *      unit   - unit id
 *      portmask    - portmask, it should be base ports of PHYs
 *      freq   - reference time frequency
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      The frequency configuration decides the reference time tick frequency.
 *      The default value is 0x80000000.
 *      If it is configured to 0x40000000, the tick frequence would be half of default.
 *      If it is configured to 0xC0000000, the tick frequence would be one and half times of default.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 *      [3.2.0]
 *          Change name from rtk_time_refTimeFreq_set.
 *          Add portmask parameter.
 */
extern int32
rtk_time_portRefTimeFreq_set(uint32 unit, rtk_portmask_t portmask, uint32 freq);

/* Function Name:
 *      rtk_time_correctionFieldTransparentValue_get
 * Description:
 *      Get the transparent value of correction field for transparent clock.
 * Input:
 *      unit   - unit id
 *      oriCf   - original correction field value
 *      rxTimeStamp   - RX time stamp of the packet
 * Output:
 *      pTransCf - pointer to transparent correction field value
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      This API calculate the value of transparent correction field by original correction field - rxTimeStamp.
 *      Software should replace the correction field in packet with the transparent correction field and send the packet.
 *      Hardware would replace the correction field by the result of TX TimeStamp - RX TimeStamp + original correction field.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_time_correctionFieldTransparentValue_get(uint32 unit, int64 oriCf, rtk_time_timeStamp_t rxTimeStamp, int64 *pTransCf);

/* Function Name:
 *      rtk_time_portPtpMacAddr_get
 * Description:
 *      Get the PTP MAC address of specific port.
 * Input:
 *      unit    - unit id
 *      port   - port ID
 * Output:
 *      pMac  - pointer to MAC adress
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpMacAddr_get(uint32 unit, rtk_port_t port, rtk_mac_t *pMac);

/* Function Name:
 *      rtk_time_portPtpMacAddr_set
 * Description:
 *      Set the PTP MAC address of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port ID
 *      pMac  - pointer to MAC adress
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpMacAddr_set(uint32 unit, rtk_port_t port, rtk_mac_t *pMac);

/* Function Name:
 *      rtk_time_portPtpMacAddrRange_get
 * Description:
 *      Get the PTP MAC address of specific port.
 * Input:
 *      unit    - unit id
 *      port   - port ID
 * Output:
 *      pMacRange  - pointer to MAC adress range
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpMacAddrRange_get(uint32 unit, rtk_port_t port, uint32 *pRange);

/* Function Name:
 *      rtk_time_portPtpMacAddrRange_set
 * Description:
 *      Set the PTP MAC address range of the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port ID
 *      range  - pointer to MAC adress range
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpMacAddrRange_set(uint32 unit, rtk_port_t port, uint32 range);

/* Function Name:
 *      rtk_time_portPtpVlanTpid_get
 * Description:
 *      Get the VLAN TPID of specific port.
 * Input:
 *      unit    - unit id
 *      port   - port ID
 *      type   - outer or inner VLAN
 *      idx    - TPID entry index
 * Output:
 *      pTpid  - pointer to TPID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8390, 8380, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpVlanTpid_get(uint32 unit, rtk_port_t port, rtk_vlanType_t type, uint32 idx, uint32 *pTpid);

/* Function Name:
 *      rtk_time_portPtpVlanTpid_set
 * Description:
 *      Set the VLAN TPID of specific port.
 * Input:
 *      unit    - unit id
 *      port   - port ID
 *      type   - outer or inner VLAN
 *      idx     - TPID entry index
 *      tpid    - VLAN TPID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      8380, 8390, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpVlanTpid_set(uint32 unit, rtk_port_t port, rtk_vlanType_t type, uint32 idx, uint32 tpid);

/* Function Name:
 *      rtk_time_portPtpOper_get
 * Description:
 *      Get the PTP time operation configuration of specific port.
 * Input:
 *      unit    - unit id
 *      port   - port ID
 * Output:
 *      pOperCfg  - pointer to PTP time operation configuraton
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8390, 8380, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpOper_get(uint32 unit, rtk_port_t port, rtk_time_operCfg_t *pOperCfg);

/* Function Name:
 *      rtk_time_portPtpOper_set
 * Description:
 *      Set the PTP time operation configuration of specific port.
 * Input:
 *      unit    - unit id
 *      port   - port ID
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
 *      8390, 8380, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpOper_set(uint32 unit, rtk_port_t port, rtk_time_operCfg_t *pOperCfg);

/* Function Name:
 *      rtk_time_portPtpLatchTime_get
 * Description:
 *      Get the PTP latched time of specific port.
 * Input:
 *      unit    - unit id
 *      port   - port ID
 * Output:
 *      pOperCfg  - pointer to PTP time operation configuraton
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8390, 8380, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.0]
 *          New added function.
 */
extern int32
rtk_time_portPtpLatchTime_get(uint32 unit, rtk_port_t port, rtk_time_timeStamp_t *pLatchTime);

/* Function Name:
 *      rtk_time_portPtpRefTimeFreqCfg_get
 * Description:
 *      Get the frequency of reference time of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 * Output:
 *      pFreqCfg    - pointer to configured reference time frequency
 *      pFreqCur    - pointer to current reference time frequency
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpRefTimeFreqCfg_get(uint32 unit, rtk_port_t port, uint32 *pFreqCfg, uint32 *pFreqCur);

/* Function Name:
 *      rtk_time_portPtpRefTimeFreqCfg_set
 * Description:
 *      Set the frequency of reference time of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
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
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpRefTimeFreqCfg_set(uint32 unit, rtk_port_t port, uint32 freq, uint32 apply);

/* Function Name:
 *      rtk_time_portPtpTxInterruptStatus_get
 * Description:
 *      Get the TX timestamp FIFO non-empty interrupt status of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 *
 * Output:
 *      pIntrSts    - interrupt status of RX/TX PTP frame types
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpTxInterruptStatus_get(uint32 unit, rtk_port_t port, uint32 *pIntrSts);

/* Function Name:
 *      rtk_time_portPtpInterruptEnable_get
 * Description:
 *      Get the PTP TX timestamp FIFO non-empty interrupt enable status of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpInterruptEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_time_portPtpInterruptEnable_set
 * Description:
 *      Set the PTP TX timestamp FIFO non-empty interrupt enable status of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      enable  - status
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
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpInterruptEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      rtk_time_portPtpTxTimestampFifo_get
 * Description:
 *      Get the top entry from PTP Tx timstamp FIFO on the dedicated port from the specified device. of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 * Output:
 *      pTimeEntry  - pointer buffer of TIME timestamp entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpTxTimestampFifo_get(uint32 unit, rtk_port_t port, rtk_time_txTimeEntry_t *pTimeEntry);

/* Function Name:
 *      rtk_time_portPtp1PPSOutput_get
 * Description:
 *      Get the 1 PPS output configuration of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 * Output:
 *      pPulseWidth - pointer to 1 PPS pulse width
 *      pEnable     - pointer to 1 PPS output enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtp1PPSOutput_get(uint32 unit, rtk_port_t port, uint32 *pPulseWidth, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_time_portPtp1PPSOutput_set
 * Description:
 *      Set the 1 PPS output configuration of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 *      pulseWidth  - pointer to 1 PPS pulse width
 *      enable      - enable 1 PPS output
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
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtp1PPSOutput_set(uint32 unit, rtk_port_t port, uint32 pulseWidth, rtk_enable_t enable);

/* Function Name:
 *      rtk_time_portPtpClockOutput_get
 * Description:
 *      Get the clock output configuration of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 * Output:
 *      pClkOutput  - pointer to clock output configuration
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpClockOutput_get(uint32 unit, rtk_port_t port, rtk_time_clkOutput_t *pClkOutput);

/* Function Name:
 *      rtk_time_portPtpClockOutput_set
 * Description:
 *      Set the clock output configuration of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 *      pClkOutput  - pointer to clock output configuration
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
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpClockOutput_set(uint32 unit, rtk_port_t port, rtk_time_clkOutput_t *pClkOutput);

/* Function Name:
 *      rtk_time_portPtpOutputSigSel_get
 * Description:
 *      Get the output pin signal selection configuration of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 * Output:
 *      pOutSigSel  - pointer to output pin signal selection configuration
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpOutputSigSel_get(uint32 unit, rtk_port_t port, rtk_time_outSigSel_t *pOutSigSel);

/* Function Name:
 *      rtk_time_portPtpOutputSigSel_set
 * Description:
 *      Set the output pin signal selection configuration of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 *      outSigSel   - output pin signal selection configuration
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
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpOutputSigSel_set(uint32 unit, rtk_port_t port, rtk_time_outSigSel_t outSigSel);

/* Function Name:
 *      rtk_time_portPtpTransEnable_get
 * Description:
 *      Get the enable status for PTP transparent clock of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 * Output:
 *      pEnable     - pointer to PTP transparent clock enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpTransEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_time_portPtpTransEnable_set
 * Description:
 *      Set the enable status for PTP transparent clock of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 *      enable      - PTP transparent clock enable status
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
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpTransEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      rtk_time_portPtpLinkDelay_get
 * Description:
 *      Get the link delay for PTP p2p transparent clock of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 * Output:
 *      pLinkDelay  - pointer to link delay (unit: nsec)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpLinkDelay_get(uint32 unit, rtk_port_t port, uint32 *pLinkDelay);

/* Function Name:
 *      rtk_time_portPtpLinkDelay_set
 * Description:
 *      Set the link delay for PTP p2p transparent clock of the specified port.
 * Input:
 *      unit        - unit id
 *      port        - port id
 *      linkDelay   - link delay (unit: nsec)
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
 *      8218E
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.3]
 *          New added function.
 */
extern int32
rtk_time_portPtpLinkDelay_set(uint32 unit, rtk_port_t port, uint32 linkDelay);

#endif /* __RTK_TIME_H__ */
