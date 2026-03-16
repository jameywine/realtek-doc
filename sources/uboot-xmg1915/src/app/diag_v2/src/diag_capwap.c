/*
 * Copyright (C) 2015 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Define diag shell functions for CAPWAP.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) CAPWAP diag shell.
 */
#include <common/debug/rt_log.h>
#include <common/rt_error.h>
#include <common/rt_type.h>
#include <common/util/rt_util.h>
#include <rtk/capwap.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>

static const char text_trap_target[RTK_TRAP_END][8] =
{
    "Local",
    "Master",
};

static const char text_otag_pri_src[CAPWAP_OTAG_PRISRC_END][32] =
{
    "Force",
    "Original",
    "Inner priority remark",
};

static const char text_itag_pri_src[CAPWAP_ITAG_PRISRC_END][32] =
{
    "Force",
    "Original",
    "Inner priority remark",
};

static const char text_ovid_cmd[CAPWAP_OVID_CMD_END][32] =
{
    "untag and pri tag",
    "untag",
    "all",
};

static const char text_ivid_cmd[CAPWAP_IVID_CMD_END][32] =
{
    "untag and pri tag",
    "untag",
    "all",
};

static const char text_vlan_type[VLAN_TYPE_END][16] =
{
    "inner vlan",
    "outer vlan",
};

#define DUMP_QOS_PROFILE(__i, __q)  \
do{                                                                                 \
    DIAG_UTIL_MPRINTF("capwap qos profile index %u:\n", __i);                       \
    DIAG_UTIL_MPRINTF("\totag pri source: %s\n", text_otag_pri_src[__q.otag_src]);  \
    DIAG_UTIL_MPRINTF("\totag pri: %u\n", __q.opri);                                \
    DIAG_UTIL_MPRINTF("\titag pri source: %s\n", text_itag_pri_src[__q.itag_src]);  \
    DIAG_UTIL_MPRINTF("\titag pri: %u\n", __q.ipri);                                \
}while(0);

#define DUMP_BSSID(__b)    \
do{                             \
    uint8 __i = 0;              \
    char __ms[32];              \
    osal_memset(&__ms, 0, sizeof(__ms));        \
    diag_util_mac2str(__ms, __b.bssid.octet); \
    DIAG_UTIL_MPRINTF("bssid entry:\n");        \
    DIAG_UTIL_MPRINTF("\tfields used when tunnel termination\n");   \
    DIAG_UTIL_MPRINTF("\t\tmac address: %s\n", __ms);               \
    DIAG_UTIL_MPRINTF("\t\tflags: %x\n", __b.flags);                \
    DIAG_UTIL_MPRINTF("\t\tovid cmd: %s\n", text_ovid_cmd[__b.ovid_cmd]);   \
    DIAG_UTIL_MPRINTF("\t\tovid: %u\n", __b.ovid);                          \
    DIAG_UTIL_MPRINTF("\t\tivid cmd: %s\n", text_ivid_cmd[__b.ivid_cmd]);   \
    DIAG_UTIL_MPRINTF("\t\tivid: %u\n", __b.ivid);                          \
    DIAG_UTIL_MPRINTF("\t\tfwd_vlan: %s\n", text_vlan_type[__b.fwd_vlan]);  \
    DIAG_UTIL_MPRINTF("\t\tinternal priority: %u\n", __b.int_pri);          \
    DIAG_UTIL_MPRINTF("\t\tindex to priority selection group: %u\n", __b.priGrp_idx);   \
    DIAG_UTIL_MPRINTF("\n\tfields used when tunnel start\n");   \
    DIAG_UTIL_MPRINTF("\t\tinterface id: %u\n", __b.intf_id);   \
    DIAG_UTIL_MPRINTF("\t\tqosPro_idx: %u\n", __b.qosPro_idx);  \
    DIAG_UTIL_MPRINTF("\t\tradio_id: %u\n", __b.radio_id);      \
    DIAG_UTIL_MPRINTF("\t\ttid_encap: %s\n", text_state[__b.tid_encap]);    \
    DIAG_UTIL_MPRINTF("\t\twl_info=0x");                        \
    for (__i = 0; __i < RTK_CAPWAP_WL_INFO_LEN_MAX ; ++__i)     \
        printf("%02x", __b.wl_info[__i]);         \
    printf("\n");         \
    DIAG_UTIL_MPRINTF("\t\tentry_idx: %u\n", __b.entry_idx);    \
}while(0);

#define DUMP_UCAST(__u)         \
do{                             \
    char __ms[32];              \
    osal_memset(&__ms, 0, sizeof(__ms));            \
    diag_util_mac2str(__ms, __u.mac.octet);         \
    DIAG_UTIL_MPRINTF("capwap unicast entry:\n");   \
    DIAG_UTIL_MPRINTF("\tmac  :%s\n", __ms);        \
    DIAG_UTIL_MPRINTF("\tvlan :%u\n", __u.vid);     \
    osal_memset(&__ms, 0, sizeof(__ms));            \
    diag_util_mac2str(__ms, __u.bssid.octet);       \
    DIAG_UTIL_MPRINTF("\tbssid:%s\n", __ms);        \
}while(0);

#define DUMP_MCAST(__u)         \
do{                             \
    char portStr[DIAG_UTIL_PORT_MASK_STRING_LEN];   \
    char __ms[32];                                  \
    osal_memset(&__ms, 0, sizeof(__ms));            \
    diag_util_mac2str(__ms, __u.mac.octet);         \
    DIAG_UTIL_MPRINTF("capwap multicast entry:\n");   \
    DIAG_UTIL_MPRINTF("\tmac  :%s\n", __ms);        \
    DIAG_UTIL_MPRINTF("\tvlan :%u\n", __u.vid);     \
    DIAG_UTIL_MPRINTF("\twire_fwd: %s\n", text_state[__u.wire_fwd]);    \
    osal_memset(&portStr, 0, sizeof(portStr));      \
    diag_util_lPortMask2str(portStr, &__u.portmask);\
    DIAG_UTIL_MPRINTF("\tportmask: %s\n", portStr);   \
    DIAG_UTIL_MPRINTF("\tfwdIndex:%d\n", __u.fwdIndex); \
}while(0);

#ifdef CMD_CAPWAP_GET_STATE
/*
 * capwap get state
 */
cparser_result_t
cparser_cmd_capwap_get_state(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_enable_t state = RTK_ENABLE_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_enable_get(unit, &state), ret);
    DIAG_UTIL_MPRINTF("capwap state: %s\n", text_state[state]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_state */
#endif

#ifdef CMD_CAPWAP_SET_STATE_DISABLE_ENABLE
/*
 * capwap set state ( disable | enable )
 */
cparser_result_t
cparser_cmd_capwap_set_state_disable_enable(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_enable_t state = RTK_ENABLE_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_PARSE_STATE(3, state);
    DIAG_UTIL_ERR_CHK(rtk_capwap_enable_set(unit, state), ret);
    DIAG_UTIL_MPRINTF("capwap state: %s\n", text_state[state]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_state_disable_enable */
#endif

#ifdef CMD_CAPWAP_GET_UDP_PORT
/*
 * capwap get udp-port
 */
cparser_result_t
cparser_cmd_capwap_get_udp_port(
    cparser_context_t *context)
{
    uint32 unit;
    uint32 ctrlPort = 0;
    uint32 dataPort = 0;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_udpPort_get(unit, &ctrlPort, &dataPort), ret);
    DIAG_UTIL_MPRINTF("capwap control port: %u\n", ctrlPort);
    DIAG_UTIL_MPRINTF("capwap data port: %u\n", dataPort);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_udp_port */
#endif

#ifdef CMD_CAPWAP_SET_UDP_PORT_CONTROL_DATA_PORT
/*
 * capwap set udp-port ( control | data) <UINT:port>
 */
cparser_result_t
cparser_cmd_capwap_set_udp_port_control_data_port(
    cparser_context_t *context,
    uint32_t *port_ptr)
{
    uint32 unit;
    uint32 ctrlPort = 0;
    uint32 dataPort = 0;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_udpPort_get(unit, &ctrlPort, &dataPort), ret);
    if ('c' == TOKEN_CHAR(3, 0))
        ctrlPort = *port_ptr;
    else if ('d' == TOKEN_CHAR(3, 0))
        dataPort = *port_ptr;
    DIAG_UTIL_ERR_CHK(rtk_capwap_udpPort_set(unit, ctrlPort, dataPort), ret);
    DIAG_UTIL_MPRINTF("capwap control port: %u\n", ctrlPort);
    DIAG_UTIL_MPRINTF("capwap data port: %u\n", dataPort);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_udp_port_control_data_port */
#endif

#ifdef CMD_CAPWAP_GET_CONTROL_FRAME_ACTION
/*
 * capwap get control-frame action
 */
cparser_result_t
cparser_cmd_capwap_get_control_frame_action(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_action_t act = ACTION_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_fwdAction_get(unit, CAPWAP_FWDPKTTYPE_CTRL_FRAME, &act), ret);
    DIAG_UTIL_MPRINTF("capwap control frame action: %s\n", text_action[act]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_control_frame_action */
#endif

#ifdef CMD_CAPWAP_SET_CONTROL_FRAME_ACTION_FORWARD_TRAP_TO_CPU
/*
 * capwap set control-frame action ( forward | trap-to-cpu )
 */
cparser_result_t
cparser_cmd_capwap_set_control_frame_action_forward_trap_to_cpu(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_action_t act = ACTION_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_PARSE_ACTION(4, act)
    DIAG_UTIL_ERR_CHK(rtk_capwap_fwdAction_set(unit, CAPWAP_FWDPKTTYPE_CTRL_FRAME, act), ret);
    DIAG_UTIL_MPRINTF("capwap control frame action: %s\n", text_action[act]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_control_frame_action_forward_trap_to_cpu */
#endif

#ifdef CMD_CAPWAP_GET_INVALID_HEADER_ACTION
/*
 * capwap get invalid-header action
 */
cparser_result_t
cparser_cmd_capwap_get_invalid_header_action(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_action_t act = ACTION_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_fwdAction_get(unit, CAPWAP_FWDPKTTYPE_INVALID_HDR, &act), ret);
    DIAG_UTIL_MPRINTF("capwap invalid-header frame action: %s\n", text_action[act]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_invalid_header_action */
#endif

#ifdef CMD_CAPWAP_SET_INVALID_HEADER_ACTION_DROP_TRAP_TO_CPU
/*
 * capwap set invalid-header action ( drop | trap-to-cpu )
 */
cparser_result_t
cparser_cmd_capwap_set_invalid_header_action_drop_trap_to_cpu(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_action_t act = ACTION_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_PARSE_ACTION(4, act)
    DIAG_UTIL_ERR_CHK(rtk_capwap_fwdAction_set(unit, CAPWAP_FWDPKTTYPE_INVALID_HDR, act), ret);
    DIAG_UTIL_MPRINTF("capwap invalid-header frame action: %s\n", text_action[act]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_invalid_header_action_drop_trap_to_cpu */
#endif

#ifdef CMD_CAPWAP_GET_UNKNOWN_SA_ACTION
/*
 * capwap get unknown-sa action
 */
cparser_result_t
cparser_cmd_capwap_get_unknown_sa_action(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_action_t act = ACTION_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_fwdAction_get(unit, CAPWAP_FWDPKTTYPE_SA_MISS, &act), ret);
    DIAG_UTIL_MPRINTF("capwap unknown-sa frame action: %s\n", text_action[act]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_unknown_sa_action */
#endif

#ifdef CMD_CAPWAP_SET_UNKNOWN_SA_ACTION_FORWARD_DROP_TRAP_TO_CPU
/*
 * capwap set unknown-sa action ( forward | drop | trap-to-cpu )
 */
cparser_result_t
cparser_cmd_capwap_set_unknown_sa_action_forward_drop_trap_to_cpu(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_action_t act = ACTION_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_PARSE_ACTION(4, act)
    DIAG_UTIL_ERR_CHK(rtk_capwap_fwdAction_set(unit, CAPWAP_FWDPKTTYPE_INVALID_HDR, act), ret);
    DIAG_UTIL_MPRINTF("capwap unknown-sa frame action: %s\n", text_action[act]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_unknown_sa_action_forward_drop_trap_to_cpu */
#endif

#ifdef CMD_CAPWAP_GET_TRAP_TARGET
/*
 * capwap get trap-target
 */
cparser_result_t
cparser_cmd_capwap_get_trap_target(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_trapTarget_t target = RTK_TRAP_END;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_trapTarget_get(unit, &target), ret);
    DIAG_UTIL_MPRINTF("capwap trap taget: %s\n", text_trap_target[target]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_trap_target */
#endif

#ifdef CMD_CAPWAP_SET_TRAP_TARGET_LOCAL_MASTER
/*
 * capwap set trap-target ( local | master )
 */
cparser_result_t
cparser_cmd_capwap_set_trap_target_local_master(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_trapTarget_t target = RTK_TRAP_END;

    DIAG_UTIL_FUNC_INIT(unit);
    if ('l' == TOKEN_CHAR(3, 0))
        target = RTK_TRAP_LOCAL;
    else if ('m' == TOKEN_CHAR(3, 0))
        target = RTK_TRAP_MASTER;
    DIAG_UTIL_ERR_CHK(rtk_capwap_trapTarget_set(unit, target), ret);
    DIAG_UTIL_MPRINTF("capwap trap taget: %s\n", text_trap_target[target]);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_trap_target_local_master */
#endif

#ifdef CMD_CAPWAP_GET_BSSID_HASH_ALGO
/*
 * capwap get bssid-hash-algo
 */
cparser_result_t
cparser_cmd_capwap_get_bssid_hash_algo(
    cparser_context_t *context)
{
    uint32 unit;
    uint32 algo = 0;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidTblHashAlgo_get(unit, &algo), ret);
    DIAG_UTIL_MPRINTF("capwap bssid table hash algorithm: %u\n", algo);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_bssid_hash_algo */
#endif

#ifdef CMD_CAPWAP_SET_BSSID_HASH_ALGO_ALGO_ID
/*
 * capwap set bssid-hash-algo <UINT:algo_id>
 */
cparser_result_t
cparser_cmd_capwap_set_bssid_hash_algo_algo_id(
    cparser_context_t *context,
    uint32_t *algo_id_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidTblHashAlgo_set(unit, *algo_id_ptr), ret);
    DIAG_UTIL_MPRINTF("capwap bssid table hash algorithm: %u\n", *algo_id_ptr);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_bssid_hash_algo_algo_id */
#endif

#ifdef CMD_CAPWAP_GET_QOS_PROFILE_IDX
/*
 * capwap get qos-profile <UINT:idx>
 */
cparser_result_t
cparser_cmd_capwap_get_qos_profile_idx(
    cparser_context_t *context,
    uint32_t *idx_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_qosProfile_t qosp;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&qosp, 0, sizeof(qosp));
    DIAG_UTIL_ERR_CHK(rtk_capwap_qosProfile_get(unit, *idx_ptr, &qosp), ret);
    DUMP_QOS_PROFILE(*idx_ptr, qosp);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_qos_profile_idx */
#endif

#ifdef CMD_CAPWAP_SET_QOS_PROFILE_IDX_INNER_TAG_OUTER_TAG_ORIGINAL_REMARK
/*
 * capwap set qos-profile <UINT:idx> ( inner-tag | outer-tag ) ( original | remark )
 */
cparser_result_t
cparser_cmd_capwap_set_qos_profile_idx_inner_tag_outer_tag_original_remark(
    cparser_context_t *context,
    uint32_t *idx_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_qosProfile_t qosp;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&qosp, 0, sizeof(qosp));
    DIAG_UTIL_ERR_CHK(rtk_capwap_qosProfile_get(unit, *idx_ptr, &qosp), ret);

    if ('i' == TOKEN_CHAR(4,0))
    {
        if ('o' == TOKEN_CHAR(5,0))
            qosp.itag_src = CAPWAP_ITAG_PRISRC_ORIGINAL;
        else if ('r' == TOKEN_CHAR(5,0))
            qosp.itag_src = CAPWAP_ITAG_PRISRC_INTPRI_REMARK;
    }
    else if ('o' == TOKEN_CHAR(4,0))
    {
        if ('o' == TOKEN_CHAR(5,0))
            qosp.itag_src = CAPWAP_OTAG_PRISRC_ORIGINAL;
        else if ('r' == TOKEN_CHAR(5,0))
            qosp.itag_src = CAPWAP_OTAG_PRISRC_INTPRI_REMARK;
    }

    DIAG_UTIL_ERR_CHK(rtk_capwap_qosProfile_set(unit, *idx_ptr, qosp), ret);
    //DUMP_QOS_PROFILE(*idx_ptr, qosp);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_qos_profile_idx_inner_tag_outer_tag_original_remark */
#endif

#ifdef CMD_CAPWAP_SET_QOS_PROFILE_IDX_INNER_TAG_OUTER_TAG_FORCE_PRI
/*
 * capwap set qos-profile <UINT:idx> ( inner-tag | outer-tag ) force <UINT:pri>
 */
cparser_result_t
cparser_cmd_capwap_set_qos_profile_idx_inner_tag_outer_tag_force_pri(
    cparser_context_t *context,
    uint32_t *idx_ptr,
    uint32_t *pri_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_qosProfile_t qosp;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&qosp, 0, sizeof(qosp));
    DIAG_UTIL_ERR_CHK(rtk_capwap_qosProfile_get(unit, *idx_ptr, &qosp), ret);

    if ('i' == TOKEN_CHAR(4,0))
    {
        qosp.itag_src = CAPWAP_ITAG_PRISRC_FORCE;
        qosp.ipri = *pri_ptr;
    }
    else if ('o' == TOKEN_CHAR(4,0))
    {
        qosp.otag_src = CAPWAP_OTAG_PRISRC_FORCE;
        qosp.opri = *pri_ptr;
    }

    DIAG_UTIL_ERR_CHK(rtk_capwap_qosProfile_set(unit, *idx_ptr, qosp), ret);
    //DUMP_QOS_PROFILE(*idx_ptr, qosp);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_qos_profile_idx_inner_tag_outer_tag_force_pri */
#endif

#ifdef CMD_CAPWAP_GET_TID_REMARK
/*
 * capwap get tid-remark
 */
cparser_result_t
cparser_cmd_capwap_get_tid_remark(
    cparser_context_t *context)
{
    uint32 unit;
    uint8 dp = 0;
    uint8 ip = 0;
    uint32 tid = 0;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);

    for (dp = 0; dp <= 2; dp++)
    {
        DIAG_UTIL_MPRINTF("drop precedence %u:\n", dp);
        for (ip = 0; ip <= 7; ip++)
        {
            DIAG_UTIL_ERR_CHK(rtk_capwap_tidRemarking_get(unit, dp, ip, &tid), ret);
            DIAG_UTIL_MPRINTF("\tinternal prioirty %u tid remark: %u\n", ip, tid);
        }
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_tid_remark */
#endif

#ifdef CMD_CAPWAP_SET_TID_REMARK_DP_INT_PRI_TID
/*
 * capwap set tid-remark <UINT:dp> <UINT:int_pri> <UINT:tid>
 */
cparser_result_t
cparser_cmd_capwap_set_tid_remark_dp_int_pri_tid(
    cparser_context_t *context,
    uint32_t *dp_ptr,
    uint32_t *int_pri_ptr,
    uint32_t *tid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_capwap_tidRemarking_set(unit, *dp_ptr, *int_pri_ptr, *tid_ptr), ret);
    DIAG_UTIL_MPRINTF("drop precedence %u:\n", *dp_ptr);
    DIAG_UTIL_MPRINTF("\tinternal prioirty %u tid remark: %u\n", *int_pri_ptr, *tid_ptr);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_set_tid_remark_dp_int_pri_tid */
#endif

#ifdef CMD_CAPWAP_GET_BSSID_ENTRY_BSSID
/*
 * capwap get bssid-entry <MACADDR:bssid>
 */
cparser_result_t
cparser_cmd_capwap_get_bssid_entry_bssid(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_get(unit, &bssid), ret);
    DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_bssid_entry_bssid */
#endif

#ifdef CMD_CAPWAP_DUMP_BSSID_ENTRY
/*
 * capwap dump bssid-entry
 */
cparser_result_t
cparser_cmd_capwap_dump_bssid_entry(
    cparser_context_t *context)
{
    uint32                  unit;
    int32                   i = -1;
    int32                   ret;
    uint32                  total_entry = 0;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);

    do
    {
        osal_memset(&bssid, 0 , sizeof(bssid));
        if ((ret = rtk_capwap_bssidEntryNextValid_get(unit, &i, &bssid)) != RT_ERR_OK)
            break;

        total_entry++;
        DUMP_BSSID(bssid);
    } while (i != -1);

    diag_util_mprintf("\nTotal Number Of Entries : %d\n", total_entry);
    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_dump_bssid_entry */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_INNER_TAG_OUTER_TAG_UNTAG_UNTAG_PRI_ALL_VID
/*
 * capwap add bssid-entry <MACADDR:bssid> ( inner-tag | outer-tag ) ( untag | untag-pri | all ) <UINT:vid>
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_inner_tag_outer_tag_untag_untag_pri_all_vid(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr,
    uint32_t *vid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);

    if ('i' == TOKEN_CHAR(4,0))
    {
        bssid.ivid = *vid_ptr;
        if ('a' == TOKEN_CHAR(5,0))
            bssid.ivid_cmd = CAPWAP_IVID_CMD_ALL;
        else
        {
            if (osal_strlen(TOKEN_STR(5)) <= 5)
                bssid.ivid_cmd = CAPWAP_IVID_CMD_UNTAG;
            else
                bssid.ivid_cmd = CAPWAP_IVID_CMD_UNTAG_PRITAG;
        }
    }
    else if ('o' == TOKEN_CHAR(4,0))
    {
        bssid.ovid = *vid_ptr;
        if ('a' == TOKEN_CHAR(5,0))
            bssid.ovid_cmd = CAPWAP_OVID_CMD_ALL;
        else
        {
            if (osal_strlen(TOKEN_STR(5)) <= 5)
                bssid.ovid_cmd = CAPWAP_OVID_CMD_UNTAG;
            else
                bssid.ovid_cmd = CAPWAP_OVID_CMD_UNTAG_PRITAG;
        }
    }

    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_inner_tag_outer_tag_untag_untag_pri_all_vid */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_FORWARD_VLAN_INNER_OUTER
/*
 * capwap add bssid-entry <MACADDR:bssid> forward-vlan ( inner | outer )
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_forward_vlan_inner_outer(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);

    if ('i' == TOKEN_CHAR(5,0))
        bssid.fwd_vlan = INNER_VLAN;
    else if ('o' == TOKEN_CHAR(5,0))
        bssid.fwd_vlan = OUTER_VLAN;

    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_forward_vlan_inner_outer */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_INTERNAL_PRIORITY_PRI
/*
 * capwap add bssid-entry <MACADDR:bssid> internal-priority <UINT:pri>
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_internal_priority_pri(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr,
    uint32_t *pri_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    bssid.int_pri = *pri_ptr;
    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_internal_priority_pri */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_PRIORITY_GROUP_GRP
/*
 * capwap add bssid-entry <MACADDR:bssid> priority-group <UINT:grp>
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_priority_group_grp(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr,
    uint32_t *grp_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    bssid.priGrp_idx = *grp_ptr;
    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_priority_group_grp */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_INTERFACE_IFID
/*
 * capwap add bssid-entry <MACADDR:bssid> interface <UINT:ifid>
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_interface_ifid(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr,
    uint32_t *ifid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    bssid.intf_id = *ifid_ptr;
    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_interface_ifid */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_QOS_PROFILE_IDX
/*
 * capwap add bssid-entry <MACADDR:bssid> qos-profile <UINT:idx>
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_qos_profile_idx(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr,
    uint32_t *idx_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    bssid.qosPro_idx = *idx_ptr;
    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_qos_profile_idx */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_RADIO_ID_RID
/*
 * capwap add bssid-entry <MACADDR:bssid> radio-id <UINT:rid>
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_radio_id_rid(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr,
    uint32_t *rid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    bssid.radio_id = *rid_ptr;
    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_radio_id_rid */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_TID_ENCAP_STATE_DISABLE_ENABLE
/*
 * capwap add bssid-entry <MACADDR:bssid> tid-encap state ( disable | enable )
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_tid_encap_state_disable_enable(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    DIAG_UTIL_PARSE_STATE(6, bssid.tid_encap);
    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_tid_encap_state_disable_enable */
#endif

#ifdef CMD_CAPWAP_ADD_BSSID_ENTRY_BSSID_WIRELESS_INFO_DATA
/*
 * capwap add bssid-entry <MACADDR:bssid> wireless-info <STRING:data>
 */
cparser_result_t
cparser_cmd_capwap_add_bssid_entry_bssid_wireless_info_data(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr,
    char **data_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    bssid.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;

    if (diag_util_str2IntArray(bssid.wl_info, *data_ptr, RTK_CAPWAP_WL_INFO_LEN_MAX) != RT_ERR_OK)
    {
        DIAG_UTIL_MPRINTF("wireless specific info!\n");
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_add(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_bssid_entry_bssid_wireless_info_data */
#endif

#ifdef CMD_CAPWAP_DEL_BSSID_ENTRY_BSSID
/*
 * capwap del bssid-entry <MACADDR:bssid>
 */
cparser_result_t
cparser_cmd_capwap_del_bssid_entry_bssid(
    cparser_context_t *context,
    cparser_macaddr_t *bssid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_bssidEntry_t bssid;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&bssid, 0, sizeof(bssid));
    osal_memcpy((void*)&bssid.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    DIAG_UTIL_ERR_CHK(rtk_capwap_bssidEntry_del(unit, &bssid), ret);
    //DUMP_BSSID(bssid);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_del_bssid_entry_bssid */
#endif

#ifdef CMD_CAPWAP_GET_UNICAST_VID_MAC
/*
 * capwap get unicast <UINT:vid> <MACADDR:mac>
 */
cparser_result_t
cparser_cmd_capwap_get_unicast_vid_mac(
    cparser_context_t *context,
    uint32_t *vid_ptr,
    cparser_macaddr_t *mac_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_ucast_t ucast;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&ucast, 0, sizeof(ucast));
    ucast.vid = *vid_ptr;
    osal_memcpy((void*)&ucast.mac, (void*)mac_ptr, ETHER_ADDR_LEN);
    DIAG_UTIL_ERR_CHK(rtk_capwap_ucast_get(unit, &ucast), ret);
    DUMP_UCAST(ucast);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_unicast_vid_mac */
#endif

#ifdef CMD_CAPWAP_DUMP_UNICAST
/*
 * capwap dump unicast
 */
cparser_result_t
cparser_cmd_capwap_dump_unicast(
    cparser_context_t *context)
{
    uint32              unit;
    int32               i = -1;
    int32               ret;
    uint32              total_entry = 0;
    rtk_capwap_ucast_t  ucast;

    DIAG_UTIL_FUNC_INIT(unit);

    do
    {
        osal_memset(&ucast, 0 , sizeof(ucast));
        if ((ret = rtk_capwap_ucastNextValid_get(unit, &i, &ucast)) != RT_ERR_OK)
            break;

        total_entry++;
        DUMP_UCAST(ucast);
    } while (i != -1);

    diag_util_mprintf("\nTotal Number Of Entries : %d\n", total_entry);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_dump_unicast */
#endif

#ifdef CMD_CAPWAP_ADD_UNICAST_VID_MAC_BSSID
/*
 * capwap add unicast <UINT:vid> <MACADDR:mac> <MACADDR:bssid>
 */
cparser_result_t
cparser_cmd_capwap_add_unicast_vid_mac_bssid(
    cparser_context_t *context,
    uint32_t *vid_ptr,
    cparser_macaddr_t *mac_ptr,
    cparser_macaddr_t *bssid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_ucast_t ucast;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&ucast, 0, sizeof(ucast));
    ucast.vid = *vid_ptr;
    osal_memcpy((void*)&ucast.mac, (void*)mac_ptr, ETHER_ADDR_LEN);
    osal_memcpy((void*)&ucast.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    ucast.flags |= RTK_CAPWAP_FLAG_ENTRY_REPLACE;
    DIAG_UTIL_ERR_CHK(rtk_capwap_ucast_add(unit, &ucast), ret);
    //DUMP_UCAST(ucast);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_unicast_vid_mac_bssid */
#endif

#ifdef CMD_CAPWAP_DEL_UNICAST_VID_MAC
/*
 * capwap del unicast <UINT:vid> <MACADDR:mac>
 */
cparser_result_t
cparser_cmd_capwap_del_unicast_vid_mac(
    cparser_context_t *context,
    uint32_t *vid_ptr,
    cparser_macaddr_t *mac_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_ucast_t ucast;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&ucast, 0, sizeof(ucast));
    ucast.vid = *vid_ptr;
    osal_memcpy((void*)&ucast.mac, (void*)mac_ptr, ETHER_ADDR_LEN);
    DIAG_UTIL_ERR_CHK(rtk_capwap_ucast_del(unit, &ucast), ret);
    //DUMP_UCAST(ucast);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_del_unicast_vid_mac */
#endif

#ifdef CMD_CAPWAP_GET_MULTICAST_VID_MAC
/*
 * capwap get multicast <UINT:vid> <MACADDR:mac>
 */
cparser_result_t
cparser_cmd_capwap_get_multicast_vid_mac(
    cparser_context_t *context,
    uint32_t *vid_ptr,
    cparser_macaddr_t *mac_ptr)
{
    char macStr[32];
    uint32 unit;
    uint32 cnt = 0;
    uint32 i = 0;
    int32 ret = RT_ERR_FAILED;
    rtk_mac_t bssid[CAPWAP_MCAST_BSSID_ARRAY_SIZE_MAX];
    rtk_capwap_mcast_t mcast;
    rtk_capwap_mcastBssid_t mcastb;

    DIAG_UTIL_FUNC_INIT(unit);

    /* Dump multicast bssid list */
    osal_memset(&mcast, 0, sizeof(mcast));
    osal_memset(&bssid, 0, sizeof(bssid));
    mcastb.vid = *vid_ptr;
    osal_memcpy(mcastb.mac.octet, mac_ptr->octet, ETHER_ADDR_LEN);
    DIAG_UTIL_ERR_CHK(rtk_capwap_mcastBssid_get(unit, &mcastb, CAPWAP_MCAST_BSSID_ARRAY_SIZE_MAX, bssid, &cnt), ret);
    for (i = 0; i < cnt ; ++i)
    {
        osal_memset(&macStr, 0, sizeof(macStr));
        diag_util_mac2str(macStr, bssid[i].octet);
        DIAG_UTIL_MPRINTF("\tbssid :%s\n", macStr);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_get_multicast_vid_mac */
#endif

#ifdef CMD_CAPWAP_DUMP_MULTICAST
/*
 * capwap dump multicast
 */
cparser_result_t
cparser_cmd_capwap_dump_multicast(
    cparser_context_t *context)
{
    char macStr[32];
    uint32 unit;
    uint32 cnt = 0;
    int32 i = -1;
    uint32 j = 0;
    int32 ret = RT_ERR_FAILED;
    uint32                  total_entry = 0;
    rtk_capwap_mcast_t mcast;
    rtk_capwap_mcastBssid_t mcastb;
    rtk_mac_t bssid[CAPWAP_MCAST_BSSID_ARRAY_SIZE_MAX];

    DIAG_UTIL_FUNC_INIT(unit);

    diag_util_mprintf(" Index | VID  | MAC address       | BSSID-List Index | BSSID \n");
    diag_util_mprintf("-------+------+-------------------+------------------+------------------\n");

    do
    {
        osal_memset(&mcast, 0 , sizeof(mcast));
        if ((ret = rtk_capwap_mcastNextValid_get(unit, &i, &mcast)) != RT_ERR_OK)
            break;
        //DUMP_MCAST(mcast);

        diag_util_mprintf("%6d | %4d | %02X:%02X:%02X:%02X:%02X:%02X | %16d | ",
                        i, mcast.vid,
                        mcast.mac.octet[0], mcast.mac.octet[1], mcast.mac.octet[2],
                        mcast.mac.octet[3], mcast.mac.octet[4], mcast.mac.octet[5], mcast.fwdIndex);

        /* Dump multicast bssid list */
        osal_memset(&bssid, 0, sizeof(bssid));
        mcastb.vid = mcast.vid;
        osal_memcpy(mcastb.mac.octet, mcast.mac.octet, ETHER_ADDR_LEN);
        if ((ret = rtk_capwap_mcastBssid_get(unit, &mcastb, CAPWAP_MCAST_BSSID_ARRAY_SIZE_MAX, bssid, &cnt)) != RT_ERR_OK)
            break;


        diag_util_mac2str(macStr, bssid[0].octet);
        diag_util_mprintf("%s\n", macStr);
        for (j = 1; j < cnt ; ++j)
        {
            osal_memset(&macStr, 0, sizeof(macStr));
            diag_util_mac2str(macStr, bssid[j].octet);
            DIAG_UTIL_MPRINTF("       |      |                   |                  | %s\n", macStr);
        }

        total_entry++;
    } while (i != -1);

    diag_util_mprintf("\nTotal Number Of Entries : %d\n", total_entry);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_dump_multicast */
#endif

#ifdef CMD_CAPWAP_ADD_MULTICAST_VID_MAC_BSSID_BSSID
/*
 * capwap add multicast <UINT:vid> <MACADDR:mac> bssid <MACADDR:bssid>
 */
cparser_result_t
cparser_cmd_capwap_add_multicast_vid_mac_bssid_bssid(
    cparser_context_t *context,
    uint32_t *vid_ptr,
    cparser_macaddr_t *mac_ptr,
    cparser_macaddr_t *bssid_ptr)
{
//    char macStr[32];
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_mcastBssid_t mcastb;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&mcastb, 0, sizeof(mcastb));
    mcastb.vid = *vid_ptr;
    osal_memcpy((void*)&mcastb.mac, (void*)mac_ptr, ETHER_ADDR_LEN);
    osal_memcpy((void*)&mcastb.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    DIAG_UTIL_ERR_CHK(rtk_capwap_mcastBssid_add(unit, &mcastb), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_add_multicast_vid_mac_bssid_bssid */
#endif

#ifdef CMD_CAPWAP_DEL_MULTICAST_VID_MAC_BSSID_BSSID
/*
 * capwap del multicast <UINT:vid> <MACADDR:mac> bssid <MACADDR:bssid>
 */
cparser_result_t
cparser_cmd_capwap_del_multicast_vid_mac_bssid_bssid(
    cparser_context_t *context,
    uint32_t *vid_ptr,
    cparser_macaddr_t *mac_ptr,
    cparser_macaddr_t *bssid_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_capwap_mcastBssid_t mcastb;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&mcastb, 0, sizeof(mcastb));
    mcastb.vid = *vid_ptr;
    osal_memcpy((void*)&mcastb.mac, (void*)mac_ptr, ETHER_ADDR_LEN);
    osal_memcpy((void*)&mcastb.bssid, (void*)bssid_ptr, ETHER_ADDR_LEN);
    DIAG_UTIL_ERR_CHK(rtk_capwap_mcastBssid_del(unit, &mcastb), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_del_multicast_vid_mac_bssid_bssid */
#endif

#ifdef CMD_CAPWAP_DUMP_BSSID_LIST
/*
 * capwap dump bssid-list
 */
cparser_result_t
cparser_cmd_capwap_dump_bssid_list(
    cparser_context_t *context)
{
    uint32                  unit;
    int32                   i = -1;
    int32                   ret;
    uint32                  total_entry = 0;
    rtk_capwap_bssidList_t  blist;

    DIAG_UTIL_FUNC_INIT(unit);

    DIAG_UTIL_MPRINTF("Index | BSSID index | Next index\n");
    DIAG_UTIL_MPRINTF("------+-------------+------------\n");

    do
    {
        osal_memset(&blist, 0 , sizeof(blist));
        if ((ret = rtk_capwap_bssidListNextValid_get(unit, &i, &blist)) != RT_ERR_OK)
            break;

        total_entry++;
        DIAG_UTIL_MPRINTF("%5d | %11d | %9d \n", i, blist.bssid_idx, blist.bssid_next_idex);
    } while (i != -1);

    diag_util_mprintf("\nTotal Number Of Entries : %d\n", total_entry);
    return CPARSER_OK;
}   /* end of cparser_cmd_capwap_dump_bssid_list */
#endif

