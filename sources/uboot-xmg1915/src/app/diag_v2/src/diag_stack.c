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
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Definition those TRUNK command and APIs in the SDK diagnostic shell.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) trunk configuration
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
#include <rtk/switch.h>
#include <rtk/stack.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>

#ifdef CMD_STACK_GET_PORT
/*
 * stack get port
 */
cparser_result_t
cparser_cmd_stack_get_port(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_portmask_t stkPorts;
    char portList[DIAG_UTIL_PORT_MASK_STRING_LEN];

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(rtk_stack_port_get(unit,  &stkPorts), ret);
    diag_util_lPortMask2str(portList, &stkPorts);

    if(osal_strcmp(portList, "") == 0)
        osal_strcpy(portList, "None");

    diag_util_mprintf("stacking ports : %s\n", portList);


    return CPARSER_OK;
}   /* end of cparser_cmd_stack_get_port */
#endif

#ifdef CMD_STACK_SET_PORT_PORTS_NONE
/*
 * stack set port ( <PORT_LIST:ports> | none )
 */
cparser_result_t
cparser_cmd_stack_set_port_ports_none(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32   ret = RT_ERR_FAILED;
    rtk_portmask_t stkPorts;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    osal_memset(&stkPorts, 0, sizeof(rtk_portmask_t));

     if ('n' != TOKEN_CHAR(3,0))
        diag_util_str2LPortMask(*ports_ptr, &stkPorts);

    DIAG_UTIL_ERR_CHK(rtk_stack_port_set(unit, &stkPorts), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_port_ports_none */
#endif

#ifdef CMD_STACK_GET_UNIT
/*
 * stack get unit
 */
cparser_result_t
cparser_cmd_stack_get_unit(
    cparser_context_t *context)
{
    uint32  unit, localUnitId;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(rtk_stack_unit_get(unit, &localUnitId), ret);

    diag_util_mprintf("switch local unit ID : %d\n", localUnitId);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_get_unit */
#endif

#ifdef CMD_STACK_SET_UNIT_UNIT_ID
/*
 * stack set unit <UINT:unit_id>
 */
cparser_result_t
cparser_cmd_stack_set_unit_unit_id(
    cparser_context_t *context,
    uint32_t *unit_id_ptr)
{
    uint32  unit;
    int32           ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_ERR_CHK(rtk_stack_unit_set(unit, *unit_id_ptr), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_unit_unit_id */
#endif

#ifdef CMD_STACK_GET_MASTER_UNIT
/*
 * stack get master-unit
 */
cparser_result_t
cparser_cmd_stack_get_master_unit(
    cparser_context_t *context)
{
    uint32  unit, masterUnitId;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(rtk_stack_masterUnit_get(unit, &masterUnitId), ret);

    diag_util_mprintf("switch master unit ID : %d\n", masterUnitId);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_get_master_unit */
#endif

#ifdef CMD_STACK_SET_MASTER_UNIT_UNIT_ID
/*
 * stack set master-unit <UINT:unit_id>
 */
cparser_result_t
cparser_cmd_stack_set_master_unit_unit_id(
    cparser_context_t *context,
    uint32_t *unit_id_ptr)
{
    uint32  unit;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_ERR_CHK(rtk_stack_masterUnit_set(unit, *unit_id_ptr), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_master_unit_unit_id */
#endif

#ifdef CMD_STACK_GET_LOOP_GUARD_STATE
/*
 * stack get loop-guard state
 */
cparser_result_t
cparser_cmd_stack_get_loop_guard_state(
    cparser_context_t *context)
{
    uint32  unit;
    int32   ret = RT_ERR_FAILED;
    rtk_enable_t enable;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(rtk_stack_loopGuard_get(unit, &enable), ret);

    diag_util_mprintf("Stacking Loop Guard: ");

    if(ENABLED == enable)
    {
        diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
    }
    else
    {
        diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_get_loop_guard_state */
#endif

#ifdef CMD_STACK_SET_LOOP_GUARD_STATE_DISABLE_ENABLE
/*
 * stack set loop-guard state ( disable | enable )
 */
cparser_result_t
cparser_cmd_stack_set_loop_guard_state_disable_enable(
    cparser_context_t *context)
{
    uint32  unit;
    int32    ret = RT_ERR_FAILED;
    rtk_enable_t enable;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if ('e' == TOKEN_CHAR(4, 0))
        enable = ENABLED;
    else
        enable = DISABLED;

    DIAG_UTIL_ERR_CHK(rtk_stack_loopGuard_set(unit, enable), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_loop_guard_state_disable_enable */
#endif

#ifdef CMD_STACK_GET_UNIT_PORT_MAP_UNIT_UNITS_ALL
/*
 * stack get unit-port-map unit ( <MASK_LIST:units> | all )
 */
cparser_result_t
cparser_cmd_stack_get_unit_port_map_unit_units_all(
    cparser_context_t *context,
    char **units_ptr)
{
    uint32  unit, idx;
    int32 ret = RT_ERR_FAILED;
    diag_mask_t unitMask;
    rtk_portmask_t stkPorts;
    char portList[DIAG_UTIL_PORT_MASK_STRING_LEN];

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_MASK(unitMask, 4, DIAG_MASKTYPE_UNIT), ret);

    DIAG_UTIL_MASK_SCAN(unitMask, idx)
    {
        DIAG_UTIL_ERR_CHK(rtk_stack_unitPortMap_get(unit,  idx, &stkPorts), ret);
        diag_util_lPortMask2str(portList, &stkPorts);

        if(osal_strcmp(portList, "") == 0)
            osal_strcpy(portList, "None");

        diag_util_mprintf("destination unit : %d, egress stacking ports: %s\n", idx, portList);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_get_unit_port_map_unit_units_all */
#endif

#ifdef CMD_STACK_SET_UNIT_PORT_MAP_UNIT_UNITS_ALL_PORT_PORTS_NONE
/*
 * stack set unit-port-map unit ( <MASK_LIST:units> | all ) port ( <PORT_LIST:ports> | none )
 */
cparser_result_t
cparser_cmd_stack_set_unit_port_map_unit_units_all_port_ports_none(
    cparser_context_t *context,
    char **units_ptr,
    char **ports_ptr)
{
    uint32  unit, idx;
    int32 ret = RT_ERR_FAILED;
    diag_mask_t unitMask;
    rtk_portmask_t stkPorts;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_MASK(unitMask, 4, DIAG_MASKTYPE_UNIT), ret);

    osal_memset(&stkPorts, 0, sizeof(stkPorts));

    if ('n' != TOKEN_CHAR(6,0))
        diag_util_str2LPortMask(*ports_ptr, &stkPorts);

    DIAG_UTIL_MASK_SCAN(unitMask, idx)
    {
        DIAG_UTIL_ERR_CHK(rtk_stack_unitPortMap_set(unit,  idx, &stkPorts), ret);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_unit_port_map_unit_units_all_port_ports_none */
#endif

#ifdef CMD_STACK_GET_NON_UNICAST_BLOCK_SRC_UNIT_UNIT_ID_PORT
/*
 * stack get non-unicast-block src-unit <UINT:unit_id> port
 */
cparser_result_t
cparser_cmd_stack_get_non_unicast_block_src_unit_unit_id_port(
    cparser_context_t *context,
    uint32_t *unit_id_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_portmask_t stkPorts;
    char portList[DIAG_UTIL_PORT_MASK_STRING_LEN];

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(rtk_stack_nonUcastBlockPort_get(unit,  *unit_id_ptr, &stkPorts), ret);
    diag_util_lPortMask2str(portList, &stkPorts);

    if(osal_strcmp(portList, "") == 0)
        osal_strcpy(portList, "None");

    diag_util_mprintf("stacking source unit %d non-unicast block ports : %s\n", *unit_id_ptr, portList);


    return CPARSER_OK;
}   /* end of cparser_cmd_stack_get_non_unicast_block_port */
#endif

#ifdef CMD_STACK_SET_NON_UNICAST_BLOCK_SRC_UNIT_UNIT_ID_PORT_PORTS_NONE
/*
 * stack set non-unicast-block src-unit <UINT:unit_id> port ( <PORT_LIST:ports> | none )
 */
cparser_result_t
cparser_cmd_stack_set_non_unicast_block_src_unit_unit_id_port_ports_none(
    cparser_context_t *context,
    uint32_t *unit_id_ptr,
    char **ports_ptr)
{
    uint32  unit;
    int32   ret = RT_ERR_FAILED;
    rtk_portmask_t stkPorts;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    osal_memset(&stkPorts, 0, sizeof(rtk_portmask_t));

     if ('n' != TOKEN_CHAR(6,0))
        diag_util_str2LPortMask(*ports_ptr, &stkPorts);

    DIAG_UTIL_ERR_CHK(rtk_stack_nonUcastBlockPort_set(unit, *unit_id_ptr, &stkPorts), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_non_unicast_block_port_ports_none */
#endif

#ifdef CMD_STACK_SET_REMOTE_INTERRUPT_NOTIFICATION_TX_STATE_DISABLE_ENABLE
/*
 * stack set remote-interrupt-notification tx state ( disable | enable )
 */
cparser_result_t
cparser_cmd_stack_set_remote_interrupt_notification_tx_state_disable_enable(
    cparser_context_t *context)
{
    uint32  unit;
    int32   ret = RT_ERR_FAILED;
    rtk_enable_t enable;

    enable = ('e' == TOKEN_CHAR(5, 0))? ENABLED : DISABLED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrTxEnable_set(unit, enable), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_remote_interrupt_notification_tx_state_disable_enable */
#endif

#ifdef CMD_STACK_SET_REMOTE_INTERRUPT_NOTIFICATION_TX_TRIGGER
/*
 * stack set remote-interrupt-notification tx trigger
 */
cparser_result_t
cparser_cmd_stack_set_remote_interrupt_notification_tx_trigger(
    cparser_context_t *context)
{
    uint32  unit;
    int32   ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrTxTriggerEnable_set(unit, ENABLED), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_remote_interrupt_notification_tx_trigger */
#endif

#ifdef CMD_STACK_SET_REMOTE_INTERRUPT_NOTIFICATION_RX_SEQUENCE_COMPARE_MARGIN_MARGIN
/*
 * stack set remote-interrupt-notification rx sequence-compare-margin <UINT:margin>
 */
cparser_result_t
cparser_cmd_stack_set_remote_interrupt_notification_rx_sequence_compare_margin_margin(
    cparser_context_t *context,
    uint32_t *margin_ptr)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrRxSeqCmpMargin_set(unit, *margin_ptr), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_remote_interrupt_notification_rx_sequence_compare_margin_margin */
#endif

#ifdef CMD_STACK_SET_REMOTE_INTERRUPT_NOTIFICATION_RX_FORCE_UPDATE_STATE_DISABLE_ENABLE
/*
 * stack set remote-interrupt-notification rx force-update-state ( disable | enable )
 */
cparser_result_t
cparser_cmd_stack_set_remote_interrupt_notification_rx_force_update_state_disable_enable(
    cparser_context_t *context)
{
    uint32  unit;
    int32   ret = RT_ERR_FAILED;
    rtk_enable_t enable;

    enable = ('e' == TOKEN_CHAR(5, 0))? ENABLED : DISABLED;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrRxForceUpdateEnable_set(unit, enable), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_set_remote_interrupt_notification_rx_force_update_state_disable_enable */
#endif

#ifdef CMD_STACK_GET_REMOTE_INTERRUPT_NOTIFICATION_INFO
/*
 * stack get remote-interrupt-notification info
 */
cparser_result_t
cparser_cmd_stack_get_remote_interrupt_notification_info(
    cparser_context_t *context)
{
    uint32 unit;
    int32 ret = RT_ERR_FAILED;
    rtk_enable_t txEn, txTrigger, rxEn;
    int32 rxSeqMargin;
    rtk_stack_rmtIntrInfo_t info;
    uint32 unit_id;
    rtk_switch_devInfo_t devInfo;
    diag_portlist_t  portlist;
    rtk_port_t port;

    DIAG_UTIL_FUNC_INIT(unit);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrTxEnable_get(unit, &txEn), ret);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrTxTriggerEnable_get(unit, &txTrigger), ret);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrRxForceUpdateEnable_get(unit, &rxEn), ret);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrRxSeqCmpMargin_get(unit, &rxSeqMargin), ret);
    DIAG_UTIL_ERR_CHK(rtk_stack_rmtIntrInfo_get(unit, &info), ret);

    DIAG_UTIL_MPRINTF(" Remote Interrupt Notification Information \n");
    DIAG_UTIL_MPRINTF("----------------------------------------------------\n");
    DIAG_UTIL_MPRINTF("              Tx State : %s\n", text_state[txEn]);
    DIAG_UTIL_MPRINTF("      Tx Trigger State : %s\n", text_state[txTrigger]);
    DIAG_UTIL_MPRINTF(" Rx Force Update State : %s\n", text_state[rxEn]);
    DIAG_UTIL_MPRINTF("    Rx Sequence Margin : %d\n", rxSeqMargin);

    for (unit_id=0; unit_id<RTK_MAX_NUM_OF_UNIT; unit_id++)
    {
        osal_memset(&devInfo, 0, sizeof(rtk_switch_devInfo_t));
        if ((ret = diag_om_get_deviceInfo(unit_id, &devInfo)) != RT_ERR_OK)
            continue;  /* skip */

        diag_util_port_min_max_get(&(devInfo.ether.portmask), &portlist.min, &portlist.max);
        osal_memcpy(&portlist.portmask, &(devInfo.ether.portmask), sizeof(rtk_portmask_t));

        /* Display for each unit */
        DIAG_UTIL_MPRINTF("\n");
        DIAG_UTIL_MPRINTF(" Unit %02d - Last-Rx-Seq: %3d, Rx-Seq: %3d, CCM: 0x%04X\n", \
            unit_id, info.unit[unit_id].last_rx_seq_id, info.unit[unit_id].rx_seq_id, \
            info.unit[unit_id].isr_ccm);

        DIAG_UTIL_PORTMASK_SCAN(portlist, port)
        {
            DIAG_UTIL_MPRINTF("\tPort %02d : %s\n", port, \
                RTK_PORTMASK_IS_PORT_SET(info.unit[unit_id].port_link_sts, port)? "Up" : "Down");

        }
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_stack_get_remote_interrupt_notification_info */
#endif


