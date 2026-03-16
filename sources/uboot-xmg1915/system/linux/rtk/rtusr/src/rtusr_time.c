/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) Clock/Time
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rtk/time.h>
#include <rtusr_util.h>
#include <rtdrv/rtdrv_netfilter.h>


int32 rtk_time_portPtpEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    time_cfg.port = port;
    GETSOCKOPT(RTDRV_TIME_PORT_PTP_ENABLE_GET, &time_cfg, rtdrv_timeCfg_t, 1);
    *pEnable = time_cfg.enable;

    return RT_ERR_OK;
}

int32 rtk_time_portPtpEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit   = unit;
    time_cfg.port   = port;
    time_cfg.enable = enable;
    SETSOCKOPT(RTDRV_TIME_PORT_PTP_ENABLE_SET, &time_cfg, rtdrv_timeCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_time_portPtpRxTimestamp_get(uint32 unit, rtk_port_t port, rtk_time_ptpIdentifier_t identifier, rtk_time_timeStamp_t *pTimeStamp)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    time_cfg.port = port;
    time_cfg.identifier = identifier;
    GETSOCKOPT(RTDRV_TIME_PORT_PTP_RX_TIME_GET, &time_cfg, rtdrv_timeCfg_t, 1);
    *pTimeStamp = time_cfg.timeStamp;

    return RT_ERR_OK;
}

int32 rtk_time_portPtpTxTimestamp_get(uint32 unit, rtk_port_t port, rtk_time_ptpIdentifier_t identifier, rtk_time_timeStamp_t *pTimeStamp)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    time_cfg.port = port;
    time_cfg.identifier = identifier;
    GETSOCKOPT(RTDRV_TIME_PORT_PTP_TX_TIME_GET, &time_cfg, rtdrv_timeCfg_t, 1);
    *pTimeStamp = time_cfg.timeStamp;

    return RT_ERR_OK;
}

int32 rtk_time_refTime_get(uint32 unit, rtk_time_timeStamp_t *pTimeStamp)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    GETSOCKOPT(RTDRV_TIME_REF_TIME_GET, &time_cfg, rtdrv_timeCfg_t, 1);
    *pTimeStamp = time_cfg.timeStamp;

    return RT_ERR_OK;
}

int32 rtk_time_refTime_set(uint32 unit, rtk_time_timeStamp_t timeStamp)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    time_cfg.timeStamp = timeStamp;
    SETSOCKOPT(RTDRV_TIME_REF_TIME_SET, &time_cfg, rtdrv_timeCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_time_refTimeAdjust_set(uint32 unit, uint32 sign, rtk_time_timeStamp_t timeStamp)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    time_cfg.sign = sign;
    time_cfg.timeStamp = timeStamp;
    SETSOCKOPT(RTDRV_TIME_REF_TIME_ADJUST_SET, &time_cfg, rtdrv_timeCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_time_refTimeEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    GETSOCKOPT(RTDRV_TIME_REF_TIME_ENABLE_GET, &time_cfg, rtdrv_timeCfg_t, 1);
    *pEnable = time_cfg.enable;

    return RT_ERR_OK;
}

int32 rtk_time_refTimeEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_timeCfg_t time_cfg;

    osal_memset(&time_cfg, 0, sizeof(time_cfg));

    time_cfg.unit = unit;
    time_cfg.enable = enable;
    SETSOCKOPT(RTDRV_TIME_REF_TIME_ENABLE_SET, &time_cfg, rtdrv_timeCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_time_refTimeFreq_get(uint32 unit, uint32 *pFreq)
{
    rtdrv_timeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.unit = unit;
    GETSOCKOPT(RTDRV_TIME_REF_TIME_FREQ_GET, &cfg, rtdrv_timeCfg_t, 1);
    *pFreq = cfg.freq;

    return RT_ERR_OK;
}   /* end of rtk_time_refTimeFreq_get */


int32
rtk_time_refTimeFreq_set(uint32 unit, uint32 freq)
{
    rtdrv_timeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.unit = unit;
    cfg.freq = freq;
    SETSOCKOPT(RTDRV_TIME_REF_TIME_FREQ_SET, &cfg, rtdrv_timeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_time_refTimeFreq_set */

/* Function Name:
 *      rtk_time_correctionFieldTransparentValue_get
 * Description:
 *      Get the transparent value of correction field for transparent clock.
 * Input:
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
 * Note:
 *      This API calculate the value of transparent correction field by original correction field - rxTimeStamp.
 *      Software should replace the correction field in packet with the transparent correction field and send the packet.
 *      Hardware would replace the correction field by the result of TX TimeStamp - RX TimeStamp + original correction field.
 */
int32
rtk_time_correctionFieldTransparentValue_get(uint32 unit, int64 oriCf, rtk_time_timeStamp_t rxTimeStamp, int64 *pTransCf)
{
    rtdrv_timeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pTransCf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.unit = unit;
    cfg.oriCf = oriCf;
    osal_memcpy(&cfg.rxTimeStamp, &rxTimeStamp, sizeof(rtk_time_timeStamp_t));

    GETSOCKOPT(RTDRV_TIME_CORRECTION_FIELD_TRANSPARENT_VALUE_GET, &cfg, rtdrv_timeCfg_t, 1);
    *pTransCf = cfg.transCf;

    return RT_ERR_OK;
}   /* end of rtk_time_correctionFieldTransparentValue_get */

