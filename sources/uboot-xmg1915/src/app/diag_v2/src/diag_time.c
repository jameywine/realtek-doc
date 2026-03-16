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
 * $Revision: 76404 $
 * $Date: 2017-03-10 15:36:12 +0800 (Fri, 10 Mar 2017) $
 *
 * Purpose : Definition those TIME command and APIs in the SDK diagnostic shell.
 *
 * Feature : The file have include the following module and sub-modules
 *
 */

/*
 * Include Files
 */
#include <common/debug/rt_log.h>
#include <common/rt_error.h>
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/util/rt_util.h>
#include <rtk/time.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>


#ifdef CMD_TIME_GET_PTP_PORT_PORTS_ALL_STATE
/*
 * time get ptp port ( <PORT_LIST:ports> | all ) state
 */
cparser_result_t cparser_cmd_time_get_ptp_port_ports_all_state(cparser_context_t *context,
    char **ports_ptr)
{
    uint32          unit = 0;
    int32           ret = RT_ERR_FAILED;
    rtk_port_t      port;
    diag_portlist_t portlist;
    rtk_enable_t    enable;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    diag_util_mprintf("State of PTP of Ports \n");
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        if ((ret = rtk_time_portPtpEnable_get(unit, port, &enable)) != RT_ERR_OK)
        {
            if (RT_ERR_PORT_ID == ret)
            {
                diag_util_mprintf("\tPort %2d State : %s\n", port, "(Invalid Port)");
                continue;
            }

            DIAG_ERR_PRINT(ret);
            return CPARSER_NOT_OK;
        }

        diag_util_mprintf("\tPort %2d State : %s\n", port, (enable == ENABLED)? DIAG_STR_ENABLE : DIAG_STR_DISABLE);
    }

    return CPARSER_OK;
}
#endif

#ifdef CMD_TIME_SET_PTP_PORT_PORTS_ALL_STATE_ENABLE_DISABLE
/*
 * time set ptp port ( <PORT_LIST:ports> | all ) state ( enable | disable )
 */
cparser_result_t cparser_cmd_time_set_ptp_port_ports_all_state_enable_disable(cparser_context_t *context,
    char **ports_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    rtk_port_t  port;
    diag_portlist_t  portlist;
    rtk_enable_t    enable;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    enable = ('e' == TOKEN_CHAR(6, 0))? ENABLED : DISABLED;

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        if ((ret = rtk_time_portPtpEnable_set(unit, port, enable)) != RT_ERR_OK)
        {
            if (RT_ERR_PORT_ID == ret)
            {
                diag_util_mprintf("\tInvalid Port: %2d (Not Supported)\n", port);
                continue;
            }
            DIAG_ERR_PRINT(ret);
            return CPARSER_NOT_OK;
        }
    }

    return CPARSER_OK;
}
#endif


#ifdef CMD_TIME_GET_PTP_PORT_PORTS_ALL_TIME_STAMP_RX_MESSAGE_TYPE_DELAY_REQUEST_PEER_DELAY_REQUEST_PEER_DELAY_RESPONSE_SYNC_SEQUENCE_ID_SEQUENCE_ID
/*
 * time get ptp port ( <PORT_LIST:ports> | all ) time-stamp rx message-type ( delay-request | peer-delay-request | peer-delay-response | sync ) sequence-id <UINT:sequence_id>
 */
cparser_result_t cparser_cmd_time_get_ptp_port_ports_all_time_stamp_rx_message_type_delay_request_peer_delay_request_peer_delay_response_sync_sequence_id_sequence_id(cparser_context_t *context,
    char **ports_ptr,
    uint32_t *sequence_id_ptr)
{
    uint32          unit = 0;
    int32           ret = RT_ERR_FAILED;
    rtk_port_t      port;
    diag_portlist_t portlist;
    rtk_time_ptpIdentifier_t identifier;
    rtk_time_timeStamp_t timeStamp;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    identifier.sequenceId = *sequence_id_ptr;
    if ('s' == TOKEN_CHAR(8, 0))
        identifier.msgType = PTP_MSG_TYPE_SYNC;
    else if ('d' == TOKEN_CHAR(8, 0))
        identifier.msgType = PTP_MSG_TYPE_DELAY_REQ;
    else {
        if ('q' == TOKEN_CHAR(8, 13))
            identifier.msgType = PTP_MSG_TYPE_PDELAY_REQ;
        else
            identifier.msgType = PTP_MSG_TYPE_PDELAY_RESP;
    }

    diag_util_mprintf("RX time stamp of ports \n");
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        if ((ret = rtk_time_portPtpRxTimestamp_get(unit, port, identifier, &timeStamp)) != RT_ERR_OK)
        {
            DIAG_ERR_PRINT(ret);
            return CPARSER_NOT_OK;
        }
        if (PTP_MSG_TYPE_SYNC == identifier.msgType)
            diag_util_mprintf("\tPort %2d Sync : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
        else if (PTP_MSG_TYPE_DELAY_REQ == identifier.msgType)
            diag_util_mprintf("\tPort %2d Delay-Request : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
        else if (PTP_MSG_TYPE_PDELAY_REQ == identifier.msgType)
            diag_util_mprintf("\tPort %2d Peer-delay-Request : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
        else
            diag_util_mprintf("\tPort %2d Peer-delay-Response : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
    }

    return CPARSER_OK;
}
#endif

#ifdef CMD_TIME_GET_PTP_PORT_PORTS_ALL_TIME_STAMP_TX_MESSAGE_TYPE_DELAY_REQUEST_PEER_DELAY_REQUEST_PEER_DELAY_RESPONSE_SYNC_SEQUENCE_ID_SEQUENCE_ID
/*
 * time get ptp port ( <PORT_LIST:ports> | all ) time-stamp tx message-type ( delay-request | peer-delay-request | peer-delay-response | sync ) sequence-id <UINT:sequence_id>
 */
cparser_result_t cparser_cmd_time_get_ptp_port_ports_all_time_stamp_tx_message_type_delay_request_peer_delay_request_peer_delay_response_sync_sequence_id_sequence_id(cparser_context_t *context,
    char **ports_ptr,
    uint32_t *sequence_id_ptr)
{
    uint32          unit = 0;
    int32           ret = RT_ERR_FAILED;
    rtk_port_t      port;
    diag_portlist_t portlist;
    rtk_time_ptpIdentifier_t identifier;
    rtk_time_timeStamp_t timeStamp;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    identifier.sequenceId = *sequence_id_ptr;
    if ('s' == TOKEN_CHAR(8, 0))
        identifier.msgType = PTP_MSG_TYPE_SYNC;
    else if ('d' == TOKEN_CHAR(8, 0))
        identifier.msgType = PTP_MSG_TYPE_DELAY_REQ;
    else {
        if ('q' == TOKEN_CHAR(8, 13))
            identifier.msgType = PTP_MSG_TYPE_PDELAY_REQ;
        else
            identifier.msgType = PTP_MSG_TYPE_PDELAY_RESP;
    }

    diag_util_mprintf("TX time stamp of ports \n");
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        if ((ret = rtk_time_portPtpTxTimestamp_get(unit, port, identifier, &timeStamp)) != RT_ERR_OK)
        {
            DIAG_ERR_PRINT(ret);
            return CPARSER_NOT_OK;
        }
        if (PTP_MSG_TYPE_SYNC == identifier.msgType)
            diag_util_mprintf("\tPort %2d Sync : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
        else if (PTP_MSG_TYPE_DELAY_REQ == identifier.msgType)
            diag_util_mprintf("\tPort %2d Delay-Request : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
        else if (PTP_MSG_TYPE_PDELAY_REQ == identifier.msgType)
            diag_util_mprintf("\tPort %2d Peer-delay-Request : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
        else
            diag_util_mprintf("\tPort %2d Peer-delay-Response : %10llu.%09u\n", port, timeStamp.sec, timeStamp.nsec);
    }

    return CPARSER_OK;
}
#endif

#ifdef CMD_TIME_GET_REFERENCE_TIME
/*
 * time get reference-time
 */
cparser_result_t cparser_cmd_time_get_reference_time(cparser_context_t *context)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    rtk_time_timeStamp_t    timeStamp;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    diag_util_mprintf("The time-stamp of reference time \n");
    if ((ret = rtk_time_refTime_get(unit, &timeStamp)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }
    diag_util_mprintf("\tRef-Time : %llu.%09u\n", timeStamp.sec, timeStamp.nsec);

    return CPARSER_OK;
}
#endif

#ifdef CMD_TIME_SET_REFERENCE_TIME_SECOND_SECOND_NANOSECOND_NANOSECOND
/*
 * time set reference-time second <UINT64:second> nanosecond <UINT:nanosecond>
 */
cparser_result_t cparser_cmd_time_set_reference_time_second_second_nanosecond_nanosecond(cparser_context_t *context,
    uint64_t *second_ptr,
    uint32_t *nanosecond_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    rtk_time_timeStamp_t    timeStamp;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    timeStamp.sec = *second_ptr;
    timeStamp.nsec = *nanosecond_ptr;
    if ((ret = rtk_time_refTime_set(unit, timeStamp)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
}
#endif

#ifdef CMD_TIME_SET_REFERENCE_TIME_INCREASE_DECREASE_SECOND_SECOND_NANOSECOND_NANOSECOND
/*
 * time set reference-time ( increase | decrease ) second <UINT64:second> nanosecond <UINT:nanosecond>
 */
cparser_result_t cparser_cmd_time_set_reference_time_increase_decrease_second_second_nanosecond_nanosecond(cparser_context_t *context,
    uint64_t *second_ptr,
    uint32_t *nanosecond_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    uint32      sign;
    rtk_time_timeStamp_t    timeStamp;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    sign = ('i' == TOKEN_CHAR(3, 0))? 0 : 1;
    timeStamp.sec = *second_ptr;
    timeStamp.nsec = *nanosecond_ptr;
    if ((ret = rtk_time_refTimeAdjust_set(unit, sign, timeStamp)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
}
#endif

#ifdef CMD_TIME_GET_REFERENCE_TIME_STATE
/*
 * time get reference-time state
 */
cparser_result_t cparser_cmd_time_get_reference_time_state(cparser_context_t *context)
{
    uint32          unit = 0;
    int32           ret = RT_ERR_FAILED;
    rtk_enable_t    enable;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if ((ret = rtk_time_refTimeEnable_get(unit, &enable)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }

    diag_util_mprintf("\tRef-Time Clock : %s\n", (enable == ENABLED)? DIAG_STR_ENABLE : DIAG_STR_DISABLE);

    return CPARSER_OK;

}
#endif

#ifdef CMD_TIME_SET_REFERENCE_TIME_STATE_ENABLE_DISABLE
/*
 * time set reference-time state ( enable | disable )
 */
cparser_result_t cparser_cmd_time_set_reference_time_state_enable_disable(cparser_context_t *context)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    rtk_enable_t    enable;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    enable = ('e' == TOKEN_CHAR(4, 0))? ENABLED : DISABLED;

    if ((ret = rtk_time_refTimeEnable_set(unit, enable)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
}
#endif

#ifdef CMD_TIME_GET_REFERENCE_TIME_FREQUENCY
/*
 * time get reference-time frequency
 */
cparser_result_t
cparser_cmd_time_get_reference_time_frequency(
    cparser_context_t *context)
{
    uint32      unit = 0;
    int32        ret = RT_ERR_FAILED;
    uint32      freq;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    diag_util_mprintf("The frequency of reference time \n");
    if ((ret = rtk_time_refTimeFreq_get(unit, &freq)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }
    diag_util_mprintf("\tFrequency Of Ref-Time : %x\n", freq);

    return CPARSER_OK;
}   /* end of cparser_cmd_time_get_reference_time_frequency */
#endif

#ifdef CMD_TIME_SET_REFERENCE_TIME_FREQUENCY_FREQUENCYVAL
/*
 * time set reference-time frequency <UINT:frequencyVal>
 */
cparser_result_t
cparser_cmd_time_set_reference_time_frequency_frequencyVal(
    cparser_context_t *context,
    uint32_t *frequencyVal_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if ((ret = rtk_time_refTimeFreq_set(unit, *frequencyVal_ptr)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_time_set_reference_time_frequency_frequencyVal */
#endif

#ifdef CMD_TIME_GET_CORRECT_FIELD_TRANS_VAL_CORRECT_FIELD_ORI_CF_SECOND_SECOND_NANOSECOND_NANOSECOND
/*
 * time get correct-field-trans-val correct-field <UINT64:ori_cf> second <UINT64:second> nanosecond <UINT:nanosecond>
 */
cparser_result_t
cparser_cmd_time_get_correct_field_trans_val_correct_field_ori_cf_second_second_nanosecond_nanosecond(
    cparser_context_t *context,
    uint64_t *ori_cf_ptr,
    uint64_t *second_ptr,
    uint32_t *nanosecond_ptr)
{
    uint32      unit = 0;
    int32        ret = RT_ERR_FAILED;
    int64        transCf;
    rtk_time_timeStamp_t rxTimeStamp;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    rxTimeStamp.sec = *second_ptr;
    rxTimeStamp.nsec = *nanosecond_ptr;

    if ((ret = rtk_time_correctionFieldTransparentValue_get(unit, *ori_cf_ptr, rxTimeStamp, &transCf)) != RT_ERR_OK)
    {
        DIAG_ERR_PRINT(ret);
        return CPARSER_NOT_OK;
    }
    diag_util_mprintf("Transparent Correction Field : %llx\n", transCf);

    return CPARSER_OK;
}   /* end of cparser_cmd_time_get_correct_field_trans_val_correct_field_64ori_cf_second_64second_nanosecond_nanosecond */
#endif

