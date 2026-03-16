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
 * $Revision: 77323 $
 * $Date: 2017-04-10 11:46:18 +0800 (Mon, 10 Apr 2017) $
 *
 * Purpose : Definition those extension command and APIs in the SDK diagnostic shell.
 *
 */

/*
 * Include Files
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/debug/rt_log.h>
#include <common/rt_error.h>
#include <common/rt_type.h>
#include <rtk/rtusr/include/rtusr_util.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>

#include <parser/cparser_priv.h>
#include <hal/common/halctrl.h>

#include <hal/mac/reg.h>
#include <ioal/mem32.h>
#include <diag_debug.h>

#include <ctype.h>
#include <stdlib.h>

#include <rtdrv/ext/rtdrv_netfilter_ext_9300.h>
#include <osal/memory.h>
#include <rtk/stat.h>


#include <ioal/ioal_init.h>

#define PKT_GEN             1

#define DIAG_EXT_PKTGEN_STREAM_MAX      2
#define DIAG_EXT_PKTGEN_STREAM_RANDOM_OFFSET_MAX    31

#define MAX_DEBUG_FIELD_LEN 4    // Maximum 128 bits for IPv6 address
#define MAX_DEBUG_ENTRY_LEN (256-62)    // Maximum 8192 bits
#define MAX_TABLE_FIELD_NAME_LEN 128
#define MAX_VALUE_STRING 128


const char text_mac_mode[RTDRV_EXT_MAC_MODE_RONDOM+1][32] =
{
    /* Fixed */          "Fixed",
    /* Increametal */           "Increametal",
    /* Rondom */           "Rondom",
};

static void ipv6_byte2int(uint32 *value, uint8 *ipv6_addr)
{
    if ((value != NULL) && (ipv6_addr != NULL)) {
        value[0] = (ipv6_addr[0] << 24) | (ipv6_addr[1] << 16) | (ipv6_addr[2] << 8) | ipv6_addr[3];
        value[1] = (ipv6_addr[4] << 24) | (ipv6_addr[5] << 16) | (ipv6_addr[6] << 8) | ipv6_addr[7];
        value[2] = (ipv6_addr[8] << 24) | (ipv6_addr[9] << 16) | (ipv6_addr[10] << 8) | ipv6_addr[11];
        value[3] = (ipv6_addr[12] << 24) | (ipv6_addr[13] << 16) | (ipv6_addr[14] << 8) | ipv6_addr[15];
    }

    return;
}

static char *strcpy_lower(char *strDest, const char *strSrc)
{
    char *address = strDest;

    while(*strSrc != '\0' ) {
        *strDest = tolower(*strSrc);
        strSrc++;
        strDest++;
    }

    return (address);
}

/*
 * debug get chip <UINT:chip_index> table <STRING:table_name> <UINT:table_address> field ( normal | mac | ipv4 | ipv6 ) <STRING:field_name>
 */
cparser_result_t cparser_cmd_debug_get_chip_chip_index_table_table_name_table_address_field_normal_mac_ipv4_ipv6_field_name(cparser_context_t *context,
    uint32_t *chip_index_ptr,
    char **table_name_ptr,
    uint32_t *table_address_ptr,
    char **field_name_ptr)
{
    int32 ret = RT_ERR_FAILED;
    rtk_diag_debug_t cfg;
    char value_string[MAX_VALUE_STRING];
    uint32 temp;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(*chip_index_ptr);
    DIAG_UTIL_OUTPUT_INIT();

    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.type = ENTRY_TABLE;
    cfg.hash_l2 = 0;
    cfg.hash_type = 0;
    strcpy_lower(cfg.table_reg_name, *table_name_ptr);
    cfg.table_reg_offset = *table_address_ptr;
    strcpy_lower(cfg.field_name, *field_name_ptr);
    if (0 == osal_strcmp(TOKEN_STR(8), "mac")) {
        cfg.value_type = VALUE_MAC;
    }

    DIAG_UTIL_ERR_CHK(rtk_diag_table_reg_field_get(*chip_index_ptr, &cfg), ret);

    if (0 == osal_strcmp(TOKEN_STR(8), "ipv4")) {
        diag_util_ip2str(value_string, cfg.value[0]);
        diag_util_printf("%s\n\n", value_string);
    } else if (0 == osal_strcmp(TOKEN_STR(8), "mac")) {
        diag_util_mac2str(value_string, (uint8 *)&cfg.value[0]);
        diag_util_printf("%s\n\n", value_string);
    } else if (0 == osal_strcmp(TOKEN_STR(8), "ipv6")) {
        temp = cfg.value[0];    cfg.value[0] = cfg.value[3];    cfg.value[3] = temp;
        temp = cfg.value[1];    cfg.value[1] = cfg.value[2];    cfg.value[2] = temp;
        diag_util_ipv62str(value_string, (uint8 *)&cfg.value[0]);
        diag_util_printf("%s\n\n", value_string);
    } else {
        diag_util_mprintf("0x%x\n\n", cfg.value[0]);
    }

    return CPARSER_OK;
}



/*
 * debug set chip <UINT:chip_index> table <STRING:table_name> <UINT:table_address> field normal < STRING:field_name> <UINT:value>
 */
cparser_result_t cparser_cmd_debug_set_chip_chip_index_table_table_name_table_address_field_normal_field_name_value(cparser_context_t *context,
    uint32_t *chip_index_ptr,
    char **table_name_ptr,
    uint32_t *table_address_ptr,
    char **field_name_ptr,
    uint32_t *value_ptr)
{
    int32 ret = RT_ERR_FAILED;
    rtk_diag_debug_t cfg;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(*chip_index_ptr);
    DIAG_UTIL_OUTPUT_INIT();

    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.type = ENTRY_TABLE;
    cfg.hash_l2 = 0;
    cfg.hash_type = 0;
    strcpy_lower(cfg.table_reg_name, *table_name_ptr);
    cfg.table_reg_offset = *table_address_ptr;
    strcpy_lower(cfg.field_name, *field_name_ptr);
    cfg.value[0] = *value_ptr;

    DIAG_UTIL_ERR_CHK(rtk_diag_table_reg_field_set(*chip_index_ptr, &cfg), ret);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}

/*
 * debug set chip <UINT:chip_index> table <STRING:table_name> <UINT:table_address> field mac < STRING:field_name> <MACADDR:mac>
*/
cparser_result_t cparser_cmd_debug_set_chip_chip_index_table_table_name_table_address_field_mac_field_name_mac(cparser_context_t *context,
    uint32_t *chip_index_ptr,
    char **table_name_ptr,
    uint32_t *table_address_ptr,
    char **field_name_ptr,
    cparser_macaddr_t *mac_ptr)
{
    int32 ret = RT_ERR_FAILED;
    rtk_diag_debug_t cfg;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(*chip_index_ptr);
    DIAG_UTIL_OUTPUT_INIT();

    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.type = ENTRY_TABLE;
    cfg.hash_l2 = 0;
    cfg.hash_type = 0;
    strcpy_lower(cfg.table_reg_name, *table_name_ptr);
    cfg.table_reg_offset = *table_address_ptr;
    strcpy_lower(cfg.field_name, *field_name_ptr);
    cfg.value_type = VALUE_MAC;
    osal_memcpy(cfg.value, mac_ptr->octet, sizeof(cparser_macaddr_t));

    DIAG_UTIL_ERR_CHK(rtk_diag_table_reg_field_set(*chip_index_ptr, &cfg), ret);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}

/*
 * debug set chip <UINT:chip_index> table <STRING:table_name> <UINT:table_address> field ipv6 <STRING:field_name> <STRING:ipv6>
 */
cparser_result_t cparser_cmd_debug_set_chip_chip_index_table_table_name_table_address_field_ipv6_field_name_ipv6(cparser_context_t *context,
    uint32_t *chip_index_ptr,
    char **table_name_ptr,
    uint32_t *table_address_ptr,
    char **field_name_ptr,
    char **ipv6_ptr)
{
    int32 ret = RT_ERR_FAILED;
    rtk_diag_debug_t cfg;
    rtk_ipv6_addr_t address;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(*chip_index_ptr);
    DIAG_UTIL_OUTPUT_INIT();

    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.type = ENTRY_TABLE;
    cfg.hash_l2 = 0;
    cfg.hash_type = 0;
    strcpy_lower(cfg.table_reg_name, *table_name_ptr);
    cfg.table_reg_offset = *table_address_ptr;
    strcpy_lower(cfg.field_name, *field_name_ptr);
    cfg.value_type = VALUE_IPV6;
    DIAG_UTIL_ERR_CHK(diag_util_str2ipv6(address.octet, *ipv6_ptr), ret);
    ipv6_byte2int(cfg.value, address.octet);

    DIAG_UTIL_ERR_CHK(rtk_diag_table_reg_field_set(*chip_index_ptr, &cfg), ret);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}

/*
 * debug set chip <UINT:chip_index> table <STRING:table_name> <UINT:table_address> field ipv4 <STRING:field_name> <IPV4ADDR:ipv4>
 */
cparser_result_t cparser_cmd_debug_set_chip_chip_index_table_table_name_table_address_field_ipv4_field_name_ipv4(cparser_context_t *context,
    uint32_t *chip_index_ptr,
    char **table_name_ptr,
    uint32_t *table_address_ptr,
    char **field_name_ptr,
    uint32_t *ipv4_ptr)
{
    int32 ret = RT_ERR_FAILED;
    rtk_diag_debug_t cfg;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(*chip_index_ptr);
    DIAG_UTIL_OUTPUT_INIT();

    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.type = ENTRY_TABLE;
    cfg.hash_l2 = 0;
    cfg.hash_type = 0;
    strcpy_lower(cfg.table_reg_name, *table_name_ptr);
    cfg.table_reg_offset = *table_address_ptr;
    strcpy_lower(cfg.field_name, *field_name_ptr);
    cfg.value[0] = *ipv4_ptr;

    DIAG_UTIL_ERR_CHK(rtk_diag_table_reg_field_set(*chip_index_ptr, &cfg), ret);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}


/******************************************************/
/***************PKT GEN  Start**************************/

#if PKT_GEN
/*
 * pktgen get tx-cmd
 */
cparser_result_t
cparser_cmd_pktgen_get_tx_cmd(
    cparser_context_t *context)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;

    const char text_tx_mode[4][32] =
    {
        "None",
        "Start",
        "Stop and reset counter",
        "Stop and hold counter",
    };

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    GETSOCKOPT(RTDRV_EXT_PKTGEN_TX_CMD_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    diag_util_mprintf("\tPacket generation TX command : ");
    diag_util_mprintf("%s \n", text_tx_mode[cfg.value]);

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_tx_cmd */

/*
 * pktgen set tx-cmd ( start | stop-and-reset-counter | stop-and-hold-counter )
 */
cparser_result_t
cparser_cmd_pktgen_set_tx_cmd_start_stop_and_reset_counter_stop_and_hold_counter(
    cparser_context_t *context)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;

    if ('a' == TOKEN_CHAR(3, 2))
        cfg.value = 1;
    else if ('r' == TOKEN_CHAR(3, 9))
        cfg.value = 2;
    else
        cfg.value = 3;

    SETSOCKOPT(RTDRV_EXT_PKTGEN_TX_CMD_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_tx_cmd_start_stop_and_reset_counter_stop_and_hold_counter */

/*
 * pktgen get state
 */
cparser_result_t
cparser_cmd_pktgen_get_state(
    cparser_context_t *context)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    GETSOCKOPT(RTDRV_EXT_PKTGEN_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    diag_util_mprintf("\tPacket generation status : ");
    diag_util_mprintf("%s\n", text_state[cfg.enable]);

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_state */

/*
 * pktgen set state ( enable | disable )
 */
cparser_result_t
cparser_cmd_pktgen_set_state_enable_disable(
    cparser_context_t *context)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    switch(TOKEN_CHAR(3,0))
    {
        case 'd': /*disable*/
            cfg.enable = DISABLED;
            break;
        case 'e': /*enable*/
            cfg.enable = ENABLED;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }
    SETSOCKOPT(RTDRV_EXT_PKTGEN_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_state_enable_disable */

/*
 * pktgen get debug-access state
 */
cparser_result_t
cparser_cmd_pktgen_get_debug_access_state(
    cparser_context_t *context)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    GETSOCKOPT(RTDRV_EXT_PKTGEN_DBG_ACC_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    diag_util_mprintf("\tPacket generation debug-access status : ");
    diag_util_mprintf("%s\n", text_state[cfg.enable]);

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_debug_access_state */

/*
 * pktgen set debug-access state ( enable | disable )
 */
cparser_result_t
cparser_cmd_pktgen_set_debug_access_state_enable_disable(
    cparser_context_t *context)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    switch(TOKEN_CHAR(4,0))
    {
        case 'd': /*disable*/
            cfg.enable = DISABLED;
            break;
        case 'e': /*enable*/
            cfg.enable = ENABLED;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }
    SETSOCKOPT(RTDRV_EXT_PKTGEN_DBG_ACC_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_debug_access_state_enable_disable */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) state
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_state(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32 ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STATE_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d : \n", port);
        diag_util_mprintf("\tTX state : ");
        diag_util_mprintf("%s\n", text_state[cfg.enable]);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_state */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) state ( enable | disable )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_state_enable_disable(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    switch(TOKEN_CHAR(5,0))
    {
        case 'd': /*disable*/
            cfg.enable = DISABLED;
            break;
        case 'e': /*enable*/
            cfg.enable = ENABLED;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STATE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_state_enable_disable */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) tx-done state
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_tx_done_state(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX_DONE_STATE_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d : \n", port);
        diag_util_mprintf("\tTX done state : ");
        if (0 == cfg.value)
            diag_util_mprintf("Normal\n");
        else if (1 == cfg.value)
            diag_util_mprintf("Finished\n");
        else
            diag_util_mprintf("\n");
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_tx_done_state */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) ipg-len
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_ipg_len(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_IPG_LEN_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d : \n", port);
        diag_util_mprintf("\tInter-Packet Gap length : ");
        diag_util_mprintf("%d\n", cfg.value);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_ipg_len */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) ipg-len <UINT:len>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_ipg_len_len(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *ipglen_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.value = *ipglen_ptr;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_IPG_LEN_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_ipg_len_ipglen */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) tx-pkt-cnt
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_tx_pkt_cnt(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;
    uint64      txPktCnt = 0;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX_PKT_CNT_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        txPktCnt = ((uint64)(cfg.pktlen_end & 0xFFFFFFFF) << 32) | cfg.pktlen_start;
        diag_util_mprintf("Port %d TX packet counter: %llu \n", port, txPktCnt);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_tx_pkt_cnt */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) tx-pkt-cnt <UINT64:pktcnt>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_tx_pkt_cnt_pktcnt(
    cparser_context_t *context,
    char **ports_ptr,
    uint64_t *pktcnt_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    cfg.pktlen_end = ((*pktcnt_ptr >> 32) & 0xFFFFFFFF);
    cfg.pktlen_start = *pktcnt_ptr & 0xFFFFFFFF;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX_PKT_CNT_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_tx_pkt_cnt_pktcnt */


/*
 * pktgen get port ( <PORT_LIST:ports> | all ) pkt-cnt
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_pkt_cnt(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;
    uint64      txPktCnt = 0;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_PKT_CNT_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        txPktCnt = ((uint64)(cfg.pktlen_end & 0xFFFFFFFF) << 32) | cfg.pktlen_start;
        diag_util_mprintf("Port %2d packet counter: %llu \n", port, txPktCnt);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_pkt_cnt */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> bad-crc state
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_bad_crc_state(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_BADCRC_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d TX bad CRC state: ", port, *stream_idx_ptr);
        diag_util_mprintf("%s\n", text_state[cfg.enable]);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_bad_crc_state */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> bad-crc state ( enable | disable )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_bad_crc_state_enable_disable(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(8,0))
    {
        case 'd': /*disable*/
            cfg.enable = DISABLED;
            break;
        case 'e': /*enable*/
            cfg.enable = ENABLED;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_BADCRC_STATE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_bad_crc_state_enable_disable */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> da-inc mode
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_da_inc_mode(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_DA_INC_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d DA increament mode: ",
                port, *stream_idx_ptr);
        diag_util_mprintf("%s\n", text_mac_mode[cfg.value]);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_da_inc_state */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> da-inc mode ( fixed | increamental | random )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_da_inc_mode_fixed_increamental_random(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(8,1))
    {
        case 'i':
            cfg.value = RTDRV_EXT_MAC_MODE_FIXED;
            break;
        case 'n':
            cfg.value = RTDRV_EXT_MAC_MODE_INCR;
            break;
        case 'r':
            cfg.value = RTDRV_EXT_MAC_MODE_RONDOM;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_DA_INC_STATE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_da_inc_state_enable_disable */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> sa-inc mode
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_sa_inc_mode(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_SA_INC_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d SA increament mode: ",
                port, *stream_idx_ptr);
        diag_util_mprintf("%s\n", text_mac_mode[cfg.value]);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_sa_inc_state */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> sa-inc mode ( fixed | increamental | random )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_sa_inc_mode_fixed_increamental_random(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(8,1))
    {
        case 'i':
            cfg.value = RTDRV_EXT_MAC_MODE_FIXED;
            break;
        case 'n':
            cfg.value = RTDRV_EXT_MAC_MODE_INCR;
            break;
        case 'r':
            cfg.value = RTDRV_EXT_MAC_MODE_RONDOM;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_SA_INC_STATE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_sa_inc_state_enable_disable */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> len-type
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_len_type(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_LENTYPE_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d packet length type: ",port, *stream_idx_ptr);

        if (0 == cfg.value)
            diag_util_mprintf("Fixed\n");
        else if (1 == cfg.value)
            diag_util_mprintf("Random\n");
        else if (2 == cfg.value)
            diag_util_mprintf("Increamental\n");
        else
            diag_util_mprintf("\n");
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_len_type */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> len-type ( fixed | random | increamental )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_len_type_fixed_random_increamental(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(7,0))
    {
        case 'f':
            cfg.value = 0;
            break;
        case 'r':
            cfg.value = 1;
            break;
        case 'i':
            cfg.value = 2;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_LENTYPE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_len_type_fixed_random_increamental */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> random-offset
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_random_offset(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_OFFSET_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d random content offset: ", port, *stream_idx_ptr);
        diag_util_mprintf("%d\n", cfg.value);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_random_offset */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> random-offset <UINT:offset>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_random_offset_offset(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *offset_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    if (*offset_ptr > DIAG_EXT_PKTGEN_STREAM_RANDOM_OFFSET_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.value = *offset_ptr;
    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_OFFSET_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_random_offset_offset */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> random-content mode
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_random_content_mode(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_CONTENT_MODE_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d random content mode: ", port, *stream_idx_ptr);
        if (0 == cfg.value)
            diag_util_mprintf("Disable\n");
        else if (1 == cfg.value)
            diag_util_mprintf("Random\n");
        else if (2 == cfg.value)
            diag_util_mprintf("Repeating\n");
        else
            diag_util_mprintf("\n");
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_random_content_mode */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> random-content mode ( none | random | repeating )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_random_content_mode_none_random_repeating(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(8,1))
    {
        case 'o':
            cfg.value = 0;
            break;
        case 'a':
            cfg.value = 1;
            break;
        case 'e':
            cfg.value = 2;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_CONTENT_MODE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_random_content_mode_none_random_repeating */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> repeat-content
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_repeat_content(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_REPEAT_CONTENT_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d random content repeat counter: %d\n",port, *stream_idx_ptr, cfg.value);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_repeat_content */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> repeat-content <UINT:content>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_repeat_content_content(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *content_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.value = *content_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_REPEAT_CONTENT_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_repeat_content_content */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> tx-pkt-cnt
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_tx_pkt_cnt(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_TX_PKT_CNT_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d TX packet counter: %d\n",port, *stream_idx_ptr, cfg.value);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_tx_pkt_cnt */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> tx-pkt-cnt <UINT:pktcnt>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_tx_pkt_cnt_pktcnt(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *pktcnt_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.value = *pktcnt_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_TX_PKT_CNT_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_tx_pkt_cnt_pktcnt */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> pkt-len
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_pkt_len(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    int32                   ret;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_PKT_LEN_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d TX packet length start: %d end: %d\n",
                port, *stream_idx_ptr, cfg.pktlen_start, cfg.pktlen_end);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_pkt_len */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> pkt-len start <UINT:start_val> end <UINT:end_val>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_pkt_len_start_start_val_end_end_val(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *start_val_ptr,
    uint32_t *end_val_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.pktlen_start = *start_val_ptr;
    cfg.pktlen_end = *end_val_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_PKT_LEN_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_pkt_len_start_start_val_end_end_val */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> sa-repeat-cnt
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_sa_repeat_cnt(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    int32                   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_SA_REPEAT_CNT_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d SA repeat counter: %d\n", port, *stream_idx_ptr, cfg.value);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_sa_repeat_cnt */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> sa-repeat-cnt <UINT:repeat_cnt>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_sa_repeat_cnt_repeat_cnt(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *repeat_cnt_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.value= *repeat_cnt_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_SA_REPEAT_CNT_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }
    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_sa_repeat_cnt_repeat_cnt */

/*
 * pktgen get port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> da-repeat-cnt
 */
cparser_result_t
cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_da_repeat_cnt(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    int32                   ret;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_DA_REPEAT_CNT_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("Port %2d stream %d DA repeat counter: %d\n",
                port, *stream_idx_ptr, cfg.value);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_port_ports_all_stream_stream_idx_da_repeat_cnt */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> da-repeat-cnt <UINT:repeat_cnt>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_da_repeat_cnt_repeat_cnt(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *repeat_cnt_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.value = *repeat_cnt_ptr;
    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_DA_REPEAT_CNT_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_da_repeat_cnt_repeat_cnt */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field sa <MACADDR:mac>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_sa_mac(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    cparser_macaddr_t *mac_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    osal_memcpy(&cfg.sa.octet, mac_ptr->octet, ETHER_ADDR_LEN);

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_SA_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
    }
    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_sa_mac */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field da <MACADDR:mac>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_da_mac(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    cparser_macaddr_t *mac_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    osal_memcpy(&cfg.da.octet, mac_ptr->octet, ETHER_ADDR_LEN);

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_DA_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_da_mac */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field ether-type <UINT:ether_type>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_ether_type_ether_type(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *ether_type_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.etherType = (uint16)*ether_type_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ETHTYPE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_ether_type_ether_type */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field ether-header state ( enable | disable )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_ether_header_state_enable_disable(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(9,0))
    {
        case 'd': /*disable*/
            cfg.enable = DISABLED;
            break;
        case 'e': /*enable*/
            cfg.enable = ENABLED;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ETHTYPE_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_ether_header_state_enable_disable */


/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field svlan-header <UINT:vlan_header>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_svlan_header_vlan_header(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *vlan_header_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.vlanHdr = *vlan_header_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_OTAG_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_svlan_header_vlan_header */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field svlan-header state ( enable | disable )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_svlan_header_state_enable_disable(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(9,0))
    {
        case 'd': /*disable*/
            cfg.enable = DISABLED;
            break;
        case 'e': /*enable*/
            cfg.enable = ENABLED;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_OTAG_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }
    return CPARSER_OK;
}

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field vlan-header <UINT:vlan_header>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_vlan_header_vlan_header(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *vlan_header_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.vlanHdr = *vlan_header_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ITAG_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }
    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_vlan_header_vlan_header */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field vlan-header state ( enable | disable )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_vlan_header_state_enable_disable(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    switch(TOKEN_CHAR(9,0))
    {
        case 'd': /*disable*/
            cfg.enable = DISABLED;
            break;
        case 'e': /*enable*/
            cfg.enable = ENABLED;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_ITAG_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }
    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_vlan_header_state_enable_disable */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field payload-type ( fix-pattern | incr | repeat-pattern | zero )
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_payload_type_fix_pattern_incr_repeat_pattern_zero(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.pattern = 0;
    switch(TOKEN_CHAR(8,0))
    {
        case 'z': /*zero*/
            cfg.patternType = RTDRV_EXT_SPG_PAYLOAD_ZERO;
            break;
        case 'i': /*increment*/
            cfg.patternType = RTDRV_EXT_SPG_PAYLOAD_INCR;
            break;
        case 'f': /*fix*/
            cfg.patternType = RTDRV_EXT_SPG_PAYLOAD_FIX;
            break;
        case 'r': /*repeat*/
            cfg.patternType = RTDRV_EXT_SPG_PAYLOAD_REPEAT;
            break;
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_PAYLOAD_TYPE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_payload_type_fix_pattern_incr_repeat_pattern_zero */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> field payload-pattern <UINT:pattern>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_payload_pattern_pattern(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *pattern_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.pattern = *pattern_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_PAYLOAD_PATTERN_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_field_payload_pattern_pattern */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> offset <UINT:offset_idx> pattern <UINT:pattern>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern_pattern(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *offset_idx_ptr,
    uint32_t *pattern_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.pattern = *pattern_ptr;
    cfg.len = *offset_idx_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern_pattern */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> offset <UINT:offset_idx> pattern8byte <UINT64:pattern>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern8byte_pattern(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *offset_idx_ptr,
    uint64_t *pattern_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.pattern8Btye[0] = *pattern_ptr;
    cfg.len = *offset_idx_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD8B_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern8byte_pattern */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> offset <UINT:offset_idx> pattern16byte <UINT64:pattern0> <UINT64:pattern1>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern16byte_pattern0_pattern1(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *offset_idx_ptr,
    uint64_t *pattern0_ptr,
    uint64_t *pattern1_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.pattern8Btye[0] = *pattern0_ptr;
    cfg.pattern8Btye[1] = *pattern1_ptr;
    cfg.len = *offset_idx_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD16B_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern16byte_pattern_pattern */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> offset <UINT:offset_idx> pattern32byte <UINT64:pattern0> <UINT64:pattern1> <UINT64:pattern2> <UINT64:pattern3>
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern32byte_pattern0_pattern1_pattern2_pattern3(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *offset_idx_ptr,
    uint64_t *pattern0_ptr,
    uint64_t *pattern1_ptr,
    uint64_t *pattern2_ptr,
    uint64_t *pattern3_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    if (*stream_idx_ptr >= DIAG_EXT_PKTGEN_STREAM_MAX)
    {
        diag_util_printf("User stream config: Error!\n");
        return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.stream_idx = *stream_idx_ptr;
    cfg.pattern8Btye[0] = *pattern0_ptr;
    cfg.pattern8Btye[1] = *pattern1_ptr;
    cfg.pattern8Btye[2] = *pattern2_ptr;
    cfg.pattern8Btye[3] = *pattern3_ptr;

    cfg.len = *offset_idx_ptr;

    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_OFFSET_PAYLOAD32B_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_stream_stream_idx_offset_offset_idx_pattern32byte_pattern0_pattern1_pattern2_pattern3 */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) tx
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_tx(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_tx */

/*
 * pktgen set port ( <PORT_LIST:ports> | all ) tx dying-gasp { <UINT:len> }
 */
cparser_result_t
cparser_cmd_pktgen_set_port_ports_all_tx_dying_gasp_len(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *len_ptr)
{
    uint32  unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;
    uint32                  length;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    if (6 == TOKEN_NUM)
        length = 1518;
    else
        length = *len_ptr;

    if (length < 64 || length > 12288)
        return CPARSER_NOT_OK;

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        cfg.value = length;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX_DYING_GASP, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_set_port_ports_all_tx_dying_gasp_len */

/*
 * pktgen dump port ( <PORT_LIST:ports> | all )
 */
cparser_result_t
cparser_cmd_pktgen_dump_port_ports_all(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32 i;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtk_port_t              port;
    diag_portlist_t         portlist;

    DIAG_UTIL_FUNC_INIT(unit);

    cfg.unit = unit;
    if (DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3) != RT_ERR_OK)
    {
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.port = port;
        diag_util_mprintf("Port %2d :\n", port);

        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("\tTX state : ");
        diag_util_mprintf("%s\n", text_state[cfg.enable]);

        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX_PKT_CNT_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("\tTX packet counter: %llu\n", ((uint64)(cfg.pktlen_end & 0xFFFFFFFF) << 32 | cfg.pktlen_start));

        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_IPG_LEN_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        diag_util_mprintf("\tTX packet ipg-length: %d\n\n", cfg.value);

        for (i = 0; i < DIAG_EXT_PKTGEN_STREAM_MAX; i++)
        {
            cfg.stream_idx = i;
            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_BADCRC_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d TX bad CRC state: ", i);
            diag_util_mprintf("%s\n", text_state[cfg.enable]);

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_LENTYPE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d packet length type: ", i);

            if (0 == cfg.value)
                diag_util_mprintf("Fixed\n");
            else if (1 == cfg.value)
                diag_util_mprintf("Random\n");
            else if (2 == cfg.value)
                diag_util_mprintf("Increamental\n");
            else
                diag_util_mprintf("\n");

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_CONTENT_MODE_GET, &cfg,rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d random content state: ", i);

            if (0 == cfg.value)
                diag_util_mprintf("Disable\n");
            else if (1 == cfg.value)
                diag_util_mprintf("Random\n");
            else if (2 == cfg.value)
                diag_util_mprintf("Repeating\n");
            else
                diag_util_mprintf("\n");

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_RANDOM_OFFSET_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d random content offset: %d\n", i, cfg.value);

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_SA_INC_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d SA increament mode: ", i);
            diag_util_mprintf("%s\n", text_mac_mode[cfg.value]);

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_DA_INC_STATE_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d DA increament mode: ", i);
            diag_util_mprintf("%s\n", text_mac_mode[cfg.value]);

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_TX_PKT_CNT_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d TX packet counter: %d\n", i, cfg.value);

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_PKT_LEN_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d TX packet length start: %d end: %d\n", i, cfg.pktlen_start, cfg.pktlen_end);

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_SA_REPEAT_CNT_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d SA repeat counter: %d\n", i, cfg.value);

            GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_DA_REPEAT_CNT_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
            diag_util_mprintf("\tstream %d DA repeat counter: %d\n", i, cfg.value);

            diag_util_mprintf("\n");
        }
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_dump_port_ports_all */

/*
 * pktgen dump port ( <PORT_LIST:ports> | all ) stream <UINT:stream_idx> pkt-len <UINT:len>
 */
cparser_result_t
cparser_cmd_pktgen_dump_port_ports_all_stream_stream_idx_pkt_len_len(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *stream_idx_ptr,
    uint32_t *len_ptr)
{
    uint32  unit;
    int32   ret;
    rtdrv_ext_pktGenCfg_t   cfg;
    diag_portlist_t portlist;
    rtk_port_t port;

    DIAG_UTIL_FUNC_INIT(unit);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);

    if (*len_ptr > PKTGEN_MAX_LEN)
        return RT_ERR_OUT_OF_RANGE;

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = *stream_idx_ptr;
        cfg.len = *len_ptr;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_STREAM_GET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_dump_port_ports_all_stream_stream_idx_pkt_len_len */

/*
 * port ( <PORT_LIST:ports> | all ) link
 */
cparser_result_t
cparser_cmd_port_ports_all_link(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32   ret;
    diag_portlist_t portlist;
    rtk_port_t port;
    rtk_port_linkStatus_t link_status;
    rtk_port_speed_t    speed;
    rtk_port_duplex_t   duplex;


    DIAG_UTIL_FUNC_INIT(unit);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 1), ret);

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        speed = PORT_SPEED_10M;
        DIAG_UTIL_ERR_CHK(rtk_port_link_get(unit, port, &link_status), ret);
        if (PORT_LINKUP == link_status)
        {
            DIAG_UTIL_ERR_CHK(rtk_port_speedDuplex_get(unit, port, &speed, &duplex), ret);
        }
        diag_util_printf("%d", link_status*PORT_SPEED_END + speed);
    }
    diag_util_printf("\n");

    return CPARSER_OK;
}   /* end of cparser_cmd_port_ports_all_link */

/*
 * pktgen get counter port ( <PORT_LIST:ports> | all )
 * note: get mib counter
 * RX/TX Octes, RX/TX Pkts, RX/TX Pause, Undersize/Oversize, RX Drops, RX Errors
 */

cparser_result_t
cparser_cmd_pktgen_get_counter_port_ports_all(
    cparser_context_t *context,
    char **ports_ptr)
{
    uint32  unit;
    int32   ret;
    diag_portlist_t portlist;
    rtk_port_t  port;
    rtk_stat_port_cntr_t    portCnt;

    DIAG_UTIL_FUNC_INIT(unit);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        DIAG_UTIL_ERR_CHK(rtk_stat_port_getAll(unit, port, &portCnt), ret);
        diag_util_printf("%llu %llu %llu %llu %u %u %u %u %u %u %u\n",
                                    portCnt.ifHCInOctets,
                                    portCnt.ifHCOutOctets,
                                    (portCnt.ifHCInUcastPkts + portCnt.ifHCInMulticastPkts + portCnt.ifHCInBroadcastPkts),
                                    (portCnt.ifHCOutUcastPkts + portCnt.ifHCOutMulticastPkts + portCnt.ifHCOutBrocastPkts),
                                    portCnt.dot3InPauseFrames,
                                    portCnt.dot3OutPauseFrames,
                                    portCnt.etherStatsUndersizePkts,
                                    portCnt.etherStatsOversizePkts,
                                    portCnt.rxMacDiscards,
                                    (portCnt.etherStatsFragments + portCnt.etherStatsCRCAlignErrors + portCnt.etherStatsJabbers),
                                    portCnt.etherStatsCRCAlignErrors);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_counter_port_ports_all */

/*
 * pktgen get info portlist
 */
cparser_result_t
cparser_cmd_pktgen_get_info_portlist(
    cparser_context_t *context)
{
    uint32  unit;
    int32   ret;
    diag_portlist_t portlist;
    rtk_port_t  port;

    DIAG_UTIL_FUNC_INIT(unit);

    DIAG_UTIL_ERR_CHK(diag_util_extract_portlist(unit, "all", &(portlist)), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        diag_util_printf("%d,", port);
    }
    diag_util_printf("\n");

    return CPARSER_OK;
}   /* end of cparser_cmd_pktgen_get_info_portlist */

/*
 * qc get <UINT:address>
 */
cparser_result_t
cparser_cmd_qc_get_address(
    cparser_context_t *context,
    uint32_t *address_ptr)
{
    uint32  unit;
    uint32  reg, value;
    int32   ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);

    reg = *address_ptr;
    if (0 != (reg % 4))
    {
        diag_util_printf("\n\rWarning! The address must be a multiple of 4.\n\r\n\r");
        return CPARSER_NOT_OK;
    }

    if (reg >= SWCORE_MEM_SIZE)
    {
        DIAG_ERR_PRINT(RT_ERR_OUT_OF_RANGE);
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, reg, &value), ret);
    diag_util_mprintf("0x%08x\n", value);

    return CPARSER_OK;
}   /* end of cparser_cmd_qc_get_address */

/*
 * qc set <UINT:address> <UINT:value>
 */
cparser_result_t
cparser_cmd_qc_set_address_value(
    cparser_context_t *context,
    uint32_t *address_ptr,
    uint32_t *value_ptr)
{
    uint32  unit;
    uint32  reg, value;
    int32   ret = RT_ERR_FAILED;

    DIAG_UTIL_FUNC_INIT(unit);

    reg = *address_ptr;
    value = *value_ptr;

    if (0 != (reg % 4))
    {
        diag_util_printf("\n\rWarning! The address must be a multiple of 4.\n\r\n\r");
        return CPARSER_NOT_OK;
    }

    if (reg >= SWCORE_MEM_SIZE)
    {
        DIAG_ERR_PRINT(RT_ERR_OUT_OF_RANGE);
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, reg, value), ret);

    return CPARSER_OK;
}   /* end of cparser_cmd_qc_set_address_value */

/*
 * qc test port ( <PORT_LIST:ports> | all ) topology ring { pktlen <UINT:len> }
 */
cparser_result_t
cparser_cmd_qc_test_port_ports_all_topology_ring_pktlen_len(
    cparser_context_t *context,
    char **ports_ptr,
    uint32_t *len_ptr)
{
    uint32  unit;
    int32   ret = RT_ERR_FAILED;
    diag_portlist_t portlist;
    rtk_port_t  port;
    rtk_mac_t  macAddr;
    //rtk_vlan_t  vlan = 1;
    //rtk_l2_ucastAddr_t  l2Addr;
    rtdrv_ext_pktGenCfg_t   cfg;
    uint32 pktlen;
    uint8   macSufOctet[32];
    uint8   portIdArry[32];
    int32   i = 0;
    uint32 isFirstInit = FALSE, portMin=0, portMax=63;

    DIAG_UTIL_FUNC_INIT(unit);

    if (TOKEN_NUM > 6)
    {
        pktlen = *len_ptr;
        if (pktlen >= 12288)
        {
            DIAG_ERR_PRINT(RT_ERR_OUT_OF_RANGE);
            return CPARSER_NOT_OK;
        }
    }
    else
        pktlen = 64;

    osal_memset(&macAddr, 0, sizeof(macAddr));
    osal_memset(&macSufOctet, 0, sizeof(macSufOctet));

    //Flush L2 address
    //DIAG_UTIL_ERR_CHK(rtk_l2_addr_delAll(unit, FALSE), ret);

    //init packet enable
    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.unit = unit;
    cfg.enable = ENABLED;
    SETSOCKOPT(RTDRV_EXT_PKTGEN_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 3), ret);
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        portMax = port;
        if (FALSE == isFirstInit)
        {
            portMin = port;
            isFirstInit = TRUE;
        }
        macSufOctet[port] = port + 1;
        portIdArry[i++] = (uint8)port;
    }

    i = 0;
    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        i++;
        macAddr.octet[5] = macSufOctet[port];
        #if 0
        DIAG_UTIL_ERR_CHK(rtk_l2_addr_init(unit, vlan, &macAddr, &l2Addr), ret);
        l2Addr.isAged = FALSE;
        l2Addr.unit_id = unit;
        l2Addr.port = port;
        DIAG_UTIL_ERR_CHK(rtk_l2_addr_add(unit, &l2Addr),ret);
        #endif
        //enable port pktgen tx
        osal_memset(&cfg, 0, sizeof(cfg));
        cfg.unit = unit;
        cfg.port = port;
        cfg.enable = ENABLED;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STATE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);

        //disable stream 1
        osal_memset(&cfg, 0, sizeof(cfg));
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 1;
        cfg.value = 0;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_TX_PKT_CNT_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);

        //enable stream 0
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 0;
        cfg.value = 1;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_TX_PKT_CNT_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);

        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 0;
        cfg.value = 0;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_LENTYPE_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);

        //DA/SA mode fixed
        osal_memset(&cfg, 0, sizeof(cfg));
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 0;
        cfg.value = RTDRV_EXT_MAC_MODE_FIXED;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_DA_INC_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 0;
        cfg.value = RTDRV_EXT_MAC_MODE_FIXED;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_SA_INC_STATE_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

        //DA/SA field
        osal_memset(&cfg, 0, sizeof(cfg));
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 0;
        osal_memcpy(&cfg.sa.octet, macAddr.octet, ETHER_ADDR_LEN);
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_SA_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);

        macAddr.octet[5] = macSufOctet[portIdArry[i]];
        if (port == portMax)
        {
            macAddr.octet[5] = macSufOctet[portMin];
        }
        osal_memset(&cfg, 0, sizeof(cfg));
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 0;
        osal_memcpy(&cfg.da.octet, macAddr.octet, ETHER_ADDR_LEN);
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_FIELD_DA_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);

        //set port tx packet len
        cfg.unit = unit;
        cfg.port = port;
        cfg.stream_idx = 0;
        cfg.pktlen_start = pktlen;
        cfg.pktlen_end = 1518;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_STREAM_PKT_LEN_SET, &cfg,rtdrv_ext_pktGenCfg_t, 1);

        //set port tx continue
        osal_memset(&cfg, 0, sizeof(cfg));
        cfg.unit = unit;
        cfg.port = port;
        cfg.pktlen_end = 0;
        cfg.pktlen_start = 0;
        GETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX_PKT_CNT_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

        //set DA/SA
        osal_memset(&cfg, 0, sizeof(cfg));
        cfg.unit = unit;
        cfg.port = port;
        SETSOCKOPT(RTDRV_EXT_PKTGEN_PORT_TX, &cfg, rtdrv_ext_pktGenCfg_t, 1);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_qc_test_port_ports_all_topology_ring */


/*
 * qc test packet-buffer ( write0 | write1 | read-only | read-check )
 */
cparser_result_t
cparser_cmd_qc_test_packet_buffer_write0_write1_read_only_read_check(
    cparser_context_t *context)
{
    uint32 unit;
    rtdrv_ext_pktGenCfg_t   cfg;
    rtdrv_ext_bufferTest_t    testItem;

    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&cfg, 0, sizeof(cfg));

    switch(TOKEN_CHAR(3,0))
    {
        case 'w': /*write*/
        {
            if ('0' == TOKEN_CHAR(3,5))
                testItem = RTDRV_EXT_BUF_TEST_WR0;
            else if ('1' == TOKEN_CHAR(3,5))
                testItem = RTDRV_EXT_BUF_TEST_WR1;
            else
            {
                diag_util_printf("User config: Error!\n");
                return CPARSER_NOT_OK;
            }
            break;
        }
        case 'r': /*read*/
        {
            if ('o' == TOKEN_CHAR(3,5))
                testItem = RTDRV_EXT_BUF_TEST_READONLY;
            else if ('c' == TOKEN_CHAR(3,5))
                testItem = RTDRV_EXT_BUF_TEST_READCHECK;
            else
            {
                diag_util_printf("User config: Error!\n");
                return CPARSER_NOT_OK;
            }
            break;
        }
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    cfg.unit = unit;
    cfg.value = testItem;
    SETSOCKOPT(RTDRV_EXT_PKTGEN_PACKET_BUFFER_TEST, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    return CPARSER_OK;
}   /* end of cparser_cmd_qc_test_packet_buffer_write0_write1_read_only_read_check */


/*
 * qc test set buffer-section ( first2k | last2k )  */
cparser_result_t
cparser_cmd_qc_test_set_buffer_section_first2k_last2k(
    cparser_context_t *context)
{
    uint32 unit;
    rtdrv_ext_pktGenCfg_t   cfg;


    DIAG_UTIL_FUNC_INIT(unit);
    osal_memset(&cfg, 0, sizeof(cfg));
    cfg.unit = unit;

    switch(TOKEN_CHAR(4,0))
    {
        case 'f':
        {
            cfg.value = 0;
            break;
        }
        case 'l':
        {
            cfg.value = 1;
            break;
        }
        default:
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
    }

    SETSOCKOPT(RTDRV_EXT_PKTGEN_PACKET_BUFFER_SECT_SET, &cfg, rtdrv_ext_pktGenCfg_t, 1);

    return CPARSER_OK;
}   /* end of cparser_cmd_qc_test_set_buffer_section_first2k_last2k */


#endif /*PKT_GEN*/

/***************PKT GEN  End****************************/
/******************************************************/

