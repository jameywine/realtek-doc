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
 * $Revision: 71994 $
 * $Date: 2016-09-30 11:03:29 +0800 (Fri, 30 Sep 2016) $
 *
 * Purpose : Definition those NIC command and APIs in the SDK diagnostic shell.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) NIC
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
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>

#define TRACE_RX    (1)
#define TRACE_TX    (2)

#define START_OF_TX_RING   (0)
#define END_OF_TX_RING     (1)

#define START_OF_RX_RING   (0)

#ifdef CMD_NIC_RESET_DUMP_COUNTER
/*
 * nic ( reset | dump ) counter
 */
cparser_result_t cparser_cmd_nic_reset_dump_counter(cparser_context_t *context)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if ('r' == TOKEN_CHAR(1,0))
    {
        DIAG_UTIL_ERR_CHK(drv_nic_cntr_clear(unit), ret);
    }
    else if ('d' == TOKEN_CHAR(1,0))
    {
        DIAG_UTIL_ERR_CHK(drv_nic_cntr_dump(unit), ret);
    }
    else
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
} /* end of cparser_cmd_nic_reset_dump_counter */
#endif

#ifdef CMD_NIC_DUMP_BUFFER_USAGE
/*
 * nic dump buffer-usage
 */
cparser_result_t cparser_cmd_nic_dump_buffer_usage(cparser_context_t *context)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_ERR_CHK(drv_nic_ringbuf_dump(unit), ret);

    return CPARSER_OK;
} /* end of cparser_cmd_nic_dump_buffer_usage */
#endif

#ifdef CMD_NIC_DUMP_PKTHDR_MBUF_RAW_DATA
/*
 * nic dump pkthdr-mbuf { raw-data }
 */
cparser_result_t cparser_cmd_nic_dump_pkthdr_mbuf_raw_data(cparser_context_t *context)
{
    uint32      unit = 0;
    uint32      flag_rawdata = FALSE;
    int32       ret = RT_ERR_FAILED;
    uint32      end = 0;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if (3 == TOKEN_NUM)
    {
        flag_rawdata = FALSE;
    }
    else if ('r' == context->parser->tokens[3].buf[0])
    {
        flag_rawdata = TRUE;
    }
    else
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    if (DIAG_OM_GET_FAMILYID(RTL9300_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL9310_FAMILY_ID))
        end = 31;
    else
        end = 7;
    DIAG_UTIL_ERR_CHK(drv_nic_pktHdrMBuf_dump(unit, NIC_PKTHDR_MBUF_MODE_RX, START_OF_RX_RING, end, flag_rawdata), ret);
    DIAG_UTIL_ERR_CHK(drv_nic_pktHdrMBuf_dump(unit, NIC_PKTHDR_MBUF_MODE_TX, START_OF_TX_RING, END_OF_TX_RING, flag_rawdata), ret);

    return CPARSER_OK;
} /* end of cparser_cmd_nic_dump_pkthdr_mbuf_raw_data */
#endif

#ifdef CMD_NIC_DUMP_PKTHDR_MBUF_TX_RING_IDX_RAW_DATA
/*
 * nic dump pkthdr-mbuf tx { <UINT:ring_idx> } { raw-data }
 */
cparser_result_t cparser_cmd_nic_dump_pkthdr_mbuf_tx_ring_idx_raw_data(cparser_context_t *context,
    uint32_t *ring_idx_ptr)
{
    uint32      unit = 0;
    uint32      flag_rawdata = FALSE;
    uint32      start = 0;
    uint32      end = 0;
    int32       ret = RT_ERR_FAILED;

    /* Don't check the (NULL == ring_idx_ptr) due to it is optional token */
    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if (6 == TOKEN_NUM)
    {
        DIAG_UTIL_ERR_CHK(diag_util_str2ul(&start, TOKEN_STR(4)), ret);
        end = start;
        flag_rawdata = TRUE;
    }
    else if (5 == TOKEN_NUM)
    {
        if ('r' == context->parser->tokens[4].buf[0])
        {
            start = START_OF_TX_RING;
            end = END_OF_TX_RING;
            flag_rawdata = TRUE;
        }
        else
        {
            DIAG_UTIL_ERR_CHK(diag_util_str2ul(&start, TOKEN_STR(4)), ret);
            end = start;
            flag_rawdata = FALSE;
        }
    }
    else
    {
        start = START_OF_TX_RING;
        end = END_OF_TX_RING;
        flag_rawdata = FALSE;
    }

    if (end > END_OF_TX_RING)
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_ERR_CHK(drv_nic_pktHdrMBuf_dump(unit, NIC_PKTHDR_MBUF_MODE_TX, start, end, flag_rawdata), ret);

    return CPARSER_OK;
} /* end of cparser_cmd_nic_dump_pkthdr_mbuf_tx_ring_idx_raw_data */
#endif

#ifdef CMD_NIC_DUMP_PKTHDR_MBUF_RX_RING_IDX_RAW_DATA
/*
 * nic dump pkthdr-mbuf rx { <UINT:ring_idx> } { raw-data }
 */
cparser_result_t cparser_cmd_nic_dump_pkthdr_mbuf_rx_ring_idx_raw_data(cparser_context_t *context,
    uint32_t *ring_idx_ptr)
{
    uint32      unit = 0;
    uint32      flag_rawdata = FALSE;
    uint32      start = 0;
    uint32      end = 0;
    int32       ret = RT_ERR_FAILED;

    /* Don't check the (NULL == ring_idx_ptr) due to it is optional token */
    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if (6 == TOKEN_NUM)
    {
        DIAG_UTIL_ERR_CHK(diag_util_str2ul(&start, TOKEN_STR(4)), ret);
        end = start;
        flag_rawdata = TRUE;
    }
    else if (5 == TOKEN_NUM)
    {
        if ('r' == context->parser->tokens[4].buf[0])
        {
            start = START_OF_RX_RING;
            if (DIAG_OM_GET_FAMILYID(RTL9300_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL9310_FAMILY_ID))
                end = 31;
            else
                end = 7;
            flag_rawdata = TRUE;
        }
        else
        {
            DIAG_UTIL_ERR_CHK(diag_util_str2ul(&start, TOKEN_STR(4)), ret);
            end = start;
            flag_rawdata = FALSE;
        }
    }
    else
    {
        start = START_OF_RX_RING;
        if (DIAG_OM_GET_FAMILYID(RTL9300_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL9310_FAMILY_ID))
            end = 31;
        else
            end = 7;
        flag_rawdata = FALSE;
    }

    if (DIAG_OM_GET_FAMILYID(RTL9300_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL9310_FAMILY_ID))
    {
        if (end > 31)
        {
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
        }
    }
    else
    {
        if (end > 7)
        {
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
        }
    }

    DIAG_UTIL_ERR_CHK(drv_nic_pktHdrMBuf_dump(unit, NIC_PKTHDR_MBUF_MODE_RX, start, end, flag_rawdata), ret);

    return CPARSER_OK;
} /* end of cparser_cmd_nic_dump_pkthdr_mbuf_rx_ring_idx_raw_data */
#endif

#ifdef CMD_NIC_SET_RX_STATE_DISABLE_ENABLE
/*
 * nic set rx state ( disable | enable )
 */
cparser_result_t cparser_cmd_nic_set_rx_state_disable_enable(cparser_context_t *context)
{
    uint32  unit = 0;
    int32   ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if ('e' == context->parser->tokens[4].buf[0])
    {
        DIAG_UTIL_ERR_CHK(drv_nic_rx_start(unit), ret);
    }
    else
    {
        DIAG_UTIL_ERR_CHK(drv_nic_rx_stop(unit), ret);
    }

    return CPARSER_OK;
} /* end of cparser_cmd_nic_set_rx_state_disable_enable */
#endif

#ifdef CMD_NIC_SET_RX_TX_TRACE_START_RAW_DATA_CPU_TAG
/*
 * nic set ( rx | tx ) trace start { raw-data } { cpu-tag }
 */
cparser_result_t cparser_cmd_nic_set_rx_tx_trace_start_raw_data_cpu_tag(cparser_context_t *context)
{
    uint32      unit = 0;
    uint32      flags = 0;
    uint32      trace = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if ('r' == TOKEN_CHAR(2,0))
    {
        trace = TRACE_RX;
    }
    else if ('t' == TOKEN_CHAR(2,0))
    {
        trace = TRACE_TX;
    }
    else
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_ERR_CHK(drv_nic_dbg_get(unit, &flags), ret);
    if (TRACE_RX == trace)
    {
        if (7 == TOKEN_NUM)
        {
            flags = flags | DEBUG_RX_RAW_LEN_BIT | DEBUG_RX_CPU_TAG_BIT;
        }
        else if (6 == TOKEN_NUM)
        {
            if ('r' == TOKEN_CHAR(5,0))
                flags = flags | DEBUG_RX_RAW_LEN_BIT;
            if ('c' == TOKEN_CHAR(5,0))
                flags = flags | DEBUG_RX_CPU_TAG_BIT;
        }
    }
    else if (TRACE_TX == trace)
    {
        if (7 == TOKEN_NUM)
        {
            flags = flags | DEBUG_TX_RAW_LEN_BIT | DEBUG_TX_CPU_TAG_BIT;
        }
        else if (6 == TOKEN_NUM)
        {
            if ('r' == TOKEN_CHAR(5,0))
                flags = flags | DEBUG_TX_RAW_LEN_BIT;
            if ('c' == TOKEN_CHAR(5,0))
                flags = flags | DEBUG_TX_CPU_TAG_BIT;
        }
    }

    DIAG_UTIL_ERR_CHK(drv_nic_dbg_set(unit, flags), ret);

    return CPARSER_OK;
} /* end of cparser_cmd_nic_set_rx_tx_trace_start_raw_data_cpu_tag */
#endif

#ifdef CMD_NIC_SET_RX_TX_TRACE_STOP
/*
 * nic set ( rx | tx ) trace stop
 */
cparser_result_t cparser_cmd_nic_set_rx_tx_trace_stop(cparser_context_t *context)
{
    uint32      unit = 0;
    uint32      flags = 0;
    uint32      trace = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if ('r' == TOKEN_CHAR(2,0))
    {
        trace = TRACE_RX;
    }
    else if ('t' == TOKEN_CHAR(2,0))
    {
        trace = TRACE_TX;
    }
    else
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_ERR_CHK(drv_nic_dbg_get(unit, &flags), ret);
    if (TRACE_RX == trace)
    {
        flags = flags & (~ DEBUG_RX_RAW_LEN_BIT);
        flags = flags & (~ DEBUG_RX_CPU_TAG_BIT);
    }
    else if (TRACE_TX == trace)
    {
        flags = flags & (~ DEBUG_TX_RAW_LEN_BIT);
        flags = flags & (~ DEBUG_TX_CPU_TAG_BIT);
    }
    else
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    DIAG_UTIL_ERR_CHK(drv_nic_dbg_set(unit, flags), ret);

    return CPARSER_OK;
} /* end of cparser_cmd_nic_set_rx_tx_trace_stop */
#endif

#ifdef CMD_NIC_GET
/*
 * nic get
 */
cparser_result_t cparser_cmd_nic_get(cparser_context_t *context)
{
    uint32      unit = 0;
    uint32      flags = 0, rx_status = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_nic_dbg_get(unit, &flags), ret);
    DIAG_UTIL_ERR_CHK(drv_nic_rx_status_get(unit, &rx_status), ret);

    if (rx_status)
    {
        diag_util_mprintf("Rx status : Enabled\n");
    }
    else
    {
        diag_util_mprintf("Rx status : Disabled\n");
    }

    diag_util_mprintf("Rx debug flags:\n");
    diag_util_printf("    +raw-data : ");
    if (flags & DEBUG_RX_RAW_LEN_BIT)
    {
        diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
    }
    else
    {
        diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
    }
    diag_util_printf("    +cpu-tag : ");
    if (flags & DEBUG_RX_CPU_TAG_BIT)
    {
        diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
    }
    else
    {
        diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
    }

    diag_util_mprintf("Tx debug flags:\n");
    diag_util_printf("    +raw-data : ");
    if (flags & DEBUG_TX_RAW_LEN_BIT)
    {
        diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
    }
    else
    {
        diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
    }
    diag_util_printf("    +cpu-tag : ");
    if (flags & DEBUG_TX_CPU_TAG_BIT)
    {
        diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
    }
    else
    {
        diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
    }

    return CPARSER_OK;
} /* end of cparser_cmd_nic_get */
#endif

#ifdef CMD_NIC_GET_RX_TX
/*
 * nic get ( rx | tx )
 */
cparser_result_t cparser_cmd_nic_get_rx_tx(cparser_context_t *context)
{
    uint32      unit = 0;
    uint32      flags = 0, rx_status = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();
    DIAG_UTIL_ERR_CHK(drv_nic_dbg_get(unit, &flags), ret);

    if ('r' == TOKEN_CHAR(2,0))
    {
        DIAG_UTIL_ERR_CHK(drv_nic_rx_status_get(unit, &rx_status), ret);
        if (rx_status)
        {
            diag_util_mprintf("Rx status : Enabled\n");
        }
        else
        {
            diag_util_mprintf("Rx status : Disabled\n");
        }

        diag_util_mprintf("Rx debug flags:\n");
        diag_util_printf("    +raw-data : ");
        if (flags & DEBUG_RX_RAW_LEN_BIT)
        {
            diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
        }
        else
        {
            diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
        }
        diag_util_printf("    +cpu-tag : ");
        if (flags & DEBUG_RX_CPU_TAG_BIT)
        {
            diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
        }
        else
        {
            diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
        }
    }

    if ('t' == TOKEN_CHAR(2,0))
    {
        diag_util_mprintf("Tx debug flags:\n");
        diag_util_printf("    +raw-data : ");
        if (flags & DEBUG_TX_RAW_LEN_BIT)
        {
            diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
        }
        else
        {
            diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
        }
        diag_util_printf("    +cpu-tag : ");
        if (flags & DEBUG_TX_CPU_TAG_BIT)
        {
            diag_util_mprintf("%s\n",DIAG_STR_ENABLE);
        }
        else
        {
            diag_util_mprintf("%s\n", DIAG_STR_DISABLE);
        }
    }

    return CPARSER_OK;
} /* end of cparser_cmd_nic_get_rx_tx */
#endif

#ifdef CMD_NIC_SET_DEBUG_FLAG
/*
 * nic set debug <UINT:flag>
 */
cparser_result_t cparser_cmd_nic_set_debug_flag(cparser_context_t *context, uint32_t *flag_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);


    DIAG_UTIL_ERR_CHK(drv_nic_dbg_set(unit, *flag_ptr), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_NIC_GET_DEBUG_FLAG
/*
 * nic get debug flag
 */
cparser_result_t cparser_cmd_nic_get_debug_flag(cparser_context_t *context)
{
    uint32      unit = 0;
    uint32      flags;
    int32       ret = RT_ERR_FAILED;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);


    DIAG_UTIL_ERR_CHK(drv_nic_dbg_get(unit, &flags), ret);
    diag_util_mprintf("debug flag : %d\n", flags);

    return CPARSER_OK;
}
#endif

