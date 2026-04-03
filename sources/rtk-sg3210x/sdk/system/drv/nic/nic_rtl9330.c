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
 * $Revision$
 * $Date$
 *
 * Purpose : Definition those public NIC(Network Interface Controller) APIs and
 *           its data type in the SDK.
 *
 * Feature : The file have include the following module and sub-modules
 *            1) CPU tag
 *            2) NIC tx
 *            3) NIC rx
 *
 */

/*
 * Include Files
 */
#include <common/error.h>
#include <ioal/mem32.h>
#include <private/drv/nic/nic_rtl9330.h>
#include <private/drv/swcore/swcore_rtl9330.h>
#include <drv/nic/nic.h>
#include <osal/lib.h>
#include <common/debug/rt_log.h>
#include <common/rt_type.h>
#include <osal/time.h>
#include <osal/memory.h>

/*
 * Symbol Definition
 */
#define PORT_CPU				65
#define NIC_9330_RXRING_NUM     (12)
#define NIC_9330_TXRING_NUM     (4)
#define NIC_9330_QUE_NUM        48
#define ADDR_LO                 0
#define ADDR_HI                 1

#define NIC_9330_RX_PREFETCH        0x3
#define NIC_9330_TX_PREFETCH        0x3
#define NIC_9330_RX_PREFETCH_THR    0xC
#define NIC_9330_TX_PREFETCH_THR    0x10
#define NIC_9330_RX_CLOSE_THR       0x3
#define NIC_9330_RX_CLOSE_TIMEOUT   0x80
#define NIC_9330_RX_RING_SURP       0x0



/*
 * Data Type Definition
 */


/*
 * Data Declaration
 */
static uint32 		reasonTbl[][2] =
{
    {0,                                         },
    {NIC_RX_REASON_CPU2CPU,                     0, },
    {NIC_RX_REASON_CCM_INVALID,                 0, },
    {NIC_RX_REASON_CCM_UNKWN,                   0, },
    {NIC_RX_REASON_CCM_UNSUPPORT,               0, },
    {NIC_RX_REASON_CCM_XCON,                    0, },
    {NIC_RX_REASON_CCM_ERR,                     0, },
    {NIC_RX_REASON_CFM_PF_CCM,                  NIC_RX_REASON_CFM, },
    {NIC_RX_REASON_CFM_PF_LT,                   NIC_RX_REASON_CFM, },
    {NIC_RX_REASON_CFM_PF_LB,                   NIC_RX_REASON_CFM, },
    {NIC_RX_REASON_CFM_PF_LM,                   NIC_RX_REASON_CFM, },   /* 10 */
    {NIC_RX_REASON_CFM_PF_DM,                   NIC_RX_REASON_CFM, },
    {NIC_RX_REASON_CFM_PF_OTHER,                NIC_RX_REASON_CFM, },
    {NIC_RX_REASON_BPE_ECID_EQ_PCID,            0, },
    {NIC_RX_REASON_BPE_ECID_LOOKUP_MISS,        0, },
    {NIC_RX_REASON_MPLS_LBL_UNKN,               NIC_RX_REASON_MPLS_EXCPT, },
    {NIC_RX_REASON_MPLS_TTL_FAIL,               NIC_RX_REASON_MPLS_EXCPT, },
    {NIC_RX_REASON_MPLS_LBL_OVER,               NIC_RX_REASON_MPLS_EXCPT, },
    {NIC_RX_REASON_MPLS_LBL_RAL,                NIC_RX_REASON_MPLS_EXCPT, },
    {NIC_RX_REASON_MPLS_LBL_ENTROPY_ERR,        NIC_RX_REASON_MPLS_EXCPT, },
    {NIC_RX_REASON_MPLS_SEQ_CHK_FAIL,           NIC_RX_REASON_MPLS_EXCPT, },    /* 20 */
    {NIC_RX_REASON_MPLS_PAYLOAD_INVALID,        NIC_RX_REASON_MPLS_EXCPT, },
    {NIC_RX_REASON_PARSE_EXCPT,                 0, },
    {NIC_RX_REASON_MALFORM,                     0, },
    {NIC_RX_REASON_L2_UC_MC_LUMIS,              0, },
    {NIC_RX_REASON_L2_DST_IF_CTRL,              0, },
    {NIC_RX_REASON_L2_NTFY,                     0, },
    {NIC_RX_REASON_VXC_L2_LU_MIS,               0, },
    {NIC_RX_REASON_RMA_USR_DEF,                 NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA_LLDP,                    NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA_BPDU,                    NIC_RX_REASON_RMA, },   /* 30 */
    {NIC_RX_REASON_OAM,                         0, },
    {NIC_RX_REASON_RMA_LACP,                    NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA_PTP,                     NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_EAPOL,                       NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA,                         0, },
    {NIC_RX_REASON_SEC_IP_MAC_BIND,             0, },
    {NIC_RX_REASON_ACH_UNKWN,                   0, },
    {NIC_RX_REASON_ACH_BFD,                     0, },
    {NIC_RX_REASON_ACH_SBFD,                    0, },
    {NIC_RX_REASON_ACH_DLM,                     0, },
    {NIC_RX_REASON_ACH_ILM,                     0, },                   /* 40 */
    {NIC_RX_REASON_ACH_DM,                      0, },
    {NIC_RX_REASON_ACH_DLM_DM,                  0, },
    {NIC_RX_REASON_ACH_ILM_DM,                  0, },
    {NIC_RX_REASON_ACH_IPV4,                    0, },
    {NIC_RX_REASON_ACH_MPLSTP_CC,               0, },
    {NIC_RX_REASON_ACH_MPLSTP_CV,               0, },
    {NIC_RX_REASON_ACH_LSP_PING,                0, },
    {NIC_RX_REASON_ACH_IPV6,                    0, },
    {NIC_RX_REASON_ACH_OAM,                     0, },
    {NIC_RX_REASON_IVC,                         0, },                   /* 50 */
    {NIC_RX_REASON_BFD_VALID,                   0, },
    {NIC_RX_REASON_TNL_PAYLOAD_PROTO_FAIL,      0, },
    {NIC_RX_REASON_TNL_AUX_EXCEPT,              0, },
    {NIC_RX_REASON_TNL_DECAP_LU_MIS,            0, },
    {NIC_RX_REASON_MY_MAC_TCAM,                 NIC_RX_REASON_MY_MAC, },
    {NIC_RX_REASON_L3UC_NON_IP,                 0, },
    {NIC_RX_REASON_L3_HDR_ERR,                  0, },
    {NIC_RX_REASON_IP4_6_ROUTE_LUMIS,           0, },
    {NIC_RX_REASON_IPUC_RPF,                    0, },
    {NIC_RX_REASON_L3_DST_IF,                   0, },                   /* 60 */
    {NIC_RX_REASON_IPP_L3_TTL_CHK_FAIL,         0, },
    {NIC_RX_REASON_IPP_L3_MTU_CHK_FAIL,         0, },
    {NIC_RX_REASON_IP4_6_ICMP_REDIR,            0, },
    {NIC_RX_REASON_ACL_FLEXKEY,                 0, },
    {NIC_RX_REASON_EPP_L3_TTL_CHK_FAIL,         0, },
    {NIC_RX_REASON_EPP_L3_MTU_CHK_FAIL,         0, },
    {NIC_RX_REASON_VLAN_EVC_LU_MIS,             0, },
    {NIC_RX_REASON_TNL_VXLAN_GPE_OAM_PKT,       0, },
    {NIC_RX_REASON_TNL_OL_UNKNOWN_PKT,          0, },
    {NIC_RX_REASON_MGMT_ERR,                    0, },
    {NIC_RX_REASON_SRH_TLV,                     0, },
    {NIC_RX_REASON_SRH_EXCEPT,                  0, },
    {NIC_RX_REASON_SR6_NH_HDL,                  0, },
    {NIC_RX_REASON_SR6_NH_HDL,                  0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_DUMMY,                  		0, },
    {NIC_RX_REASON_NORMAL_FWD,                  0, },
};


/*QueueId:                             0                       8         */
static uint32   rxRingIdSize_9330[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43};
static uint32   txRingIdSize_9330[] = {32, 32, 32, 32};
static uint8    nic_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};


/*
 * Macro Definition
 */


/*
 * Function Declaration
 */

static int32 _nic_9330_rx_reason_translate(drv_nic_pkt_t *pPacket, nic_9330_cpuTag_t *pCpuTag)
{
#define RTL9330_MAX_REASON_NM  127
    uint8 reason = pCpuTag->un.rx.REASON;

    if (pCpuTag->un.rx.SFLOW_HIT && (NIC_9330_SFLOW_RX == pCpuTag->un.rx.SFLOW_DIR))
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_RX);
    else if (pCpuTag->un.rx.SFLOW_HIT && (NIC_9330_SFLOW_TX == pCpuTag->un.rx.SFLOW_DIR))
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_TX);
    if (pCpuTag->un.rx.FLOW_SAMPLE)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_FLOW_SAMPLE);
    if (pCpuTag->un.rx.DOS)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_ATTACK);
    if (pCpuTag->un.rx.MAC_CST)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_MAC_CONSTRAINT);
    if (pCpuTag->un.rx.NEW_SA)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_NEW_SA);
    if (pCpuTag->un.rx.PMV_FBD)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_PMV_FBD);
    if (pCpuTag->un.rx.L2_STTC_PMV)
    {
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_STC_L2_PMV);
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_PMV);
    }
    if (pCpuTag->un.rx.L2_DYN_PMV)
    {
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_DYN_L2_PMV);
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_PMV);
    }
    if (pCpuTag->un.rx.L2_HASH_FULL)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_HASH_FULL);


    if (reason == RTL9330_MAX_REASON_NM)
    {
        NIC_REASON_MASK_SET(*pPacket, RTL9330_MAX_REASON_NM);
    }
    else    /* reason < RTL9330_MAX_REASON_NM */
    {
        if (reason <= 127)
        {
        NIC_REASON_MASK_SET(*pPacket, reasonTbl[reason][0]);
        NIC_REASON_MASK_SET(*pPacket, reasonTbl[reason][1]);
        }
        else
            osal_printf("%s():%d  fatal error!  reason:%d\n", __FUNCTION__, __LINE__, reason);
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      r9330_init
 * Description:
 *      Initialize nic module of the specified device.
 * Input:
 *      unit     - unit id
 *      pInitCfg - pointer to initial config struct of NIC
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      Must initialize nic module before calling any nic APIs.
 */
int32
r9330_init(uint32 unit, drv_nic_initCfg_t *pInitCfg)
{
    uint32  i;


    RT_INIT_REENTRY_CHK(nic_init[unit]);
    
    ioal_mem32_write(unit, RTL9330_PDMA_PHYSICAL_ADDR_SHIFT_ADDR(0), 0x10000000);
    ioal_mem32_write(unit, RTL9330_PDMA_PHYSICAL_ADDR_SHIFT_ADDR(1), 0x0);

    // Set prefetch size
    ioal_mem32_field_write(unit, RTL9330_PDMA_PREFETCH_CTRL_ADDR, RTL9330_PDMA_PREFETCH_CTRL_RX_PREFETCH_SIZE_OFFSET, RTL9330_PDMA_PREFETCH_CTRL_RX_PREFETCH_SIZE_MASK, NIC_9330_RX_PREFETCH);
    ioal_mem32_field_write(unit, RTL9330_PDMA_PREFETCH_CTRL_ADDR, RTL9330_PDMA_PREFETCH_CTRL_TX_PREFETCH_SIZE_OFFSET, RTL9330_PDMA_PREFETCH_CTRL_TX_PREFETCH_SIZE_MASK, NIC_9330_TX_PREFETCH);

    // Set prefetch threshold
    ioal_mem32_field_write(unit, RTL9330_PDMA_PREFETCH_CTRL_ADDR, RTL9330_PDMA_PREFETCH_CTRL_RX_PREFETCH_THR_OFFSET, RTL9330_PDMA_PREFETCH_CTRL_RX_PREFETCH_THR_MASK, NIC_9330_RX_PREFETCH_THR);
    ioal_mem32_field_write(unit, RTL9330_PDMA_PREFETCH_CTRL_ADDR, RTL9330_PDMA_PREFETCH_CTRL_TX_PREFETCH_THR_OFFSET, RTL9330_PDMA_PREFETCH_CTRL_TX_PREFETCH_THR_MASK, NIC_9330_TX_PREFETCH_THR);

#if 0
    do
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "Wait ... ");
        ioal_mem32_field_read(unit, RTL9330_PDMA_LOCAL_RING_STS_ADDR, RTL9330_PDMA_LOCAL_RING_STS_LOCAL_RING_EMPTY_OFFSET, RTL9330_PDMA_LOCAL_RING_STS_LOCAL_RING_EMPTY_MASK, &temp);
        cnt++;
    } while (temp != 0 && cnt < 10000);

    if (temp != 0)
    {
        osal_printf("%s():%d  NIC local ring init failed!\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }
#endif


    // Set RX close threshold
    ioal_mem32_field_write(unit, RTL9330_PDMA_PREFETCH_CTRL_ADDR, RTL9330_PDMA_PREFETCH_CTRL_RX_CLOSE_THR_OFFSET, RTL9330_PDMA_PREFETCH_CTRL_RX_CLOSE_THR_MASK, NIC_9330_RX_CLOSE_THR);
    for (i = 0; i < NIC_9330_RXRING_NUM; i++)
    {
        // Set RX close timeout
        ioal_mem32_field_write(unit, RTL9330_PDMA_CLOSE_TIMEOUT_ADDR(i), RTL9330_PDMA_CLOSE_TIMEOUT_TIMEOUT_OFFSET(i), RTL9330_PDMA_CLOSE_TIMEOUT_TIMEOUT_MASK(i), NIC_9330_RX_CLOSE_TIMEOUT);
        // HOL
        ioal_mem32_field_write(unit, RTL9330_PDMA_HOL_RX_RING_SUPR_AMOUNT_ADDR(i), RTL9330_PDMA_HOL_RX_RING_SUPR_AMOUNT_AMOUNT_OFFSET(i), RTL9330_PDMA_HOL_RX_RING_SUPR_AMOUNT_AMOUNT_MASK(i), NIC_9330_RX_RING_SURP);
        // RX ring size
        ioal_mem32_field_write(unit, RTL9330_PDMA_RX_RING_SIZE_ADDR(i), RTL9330_PDMA_RX_RING_SIZE_SIZE_OFFSET(i), RTL9330_PDMA_RX_RING_SIZE_SIZE_MASK(i), rxRingIdSize_9330[i]);
    }
    for (i = 0; i < NIC_9330_TXRING_NUM; i++)
    {
        // TX ring size
        ioal_mem32_field_write(unit, RTL9330_PDMA_TX_RING_SIZE_ADDR(i), RTL9330_PDMA_TX_RING_SIZE_SIZE_OFFSET(i), RTL9330_PDMA_TX_RING_SIZE_SIZE_MASK(i), txRingIdSize_9330[i]);
    }


    nic_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}



/* Function Name:
 *      r9330_rx_start
 * Description:
 *      Start the rx action of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      None
 */
int32
r9330_rx_start(uint32 unit)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9330_rx_stop
 * Description:
 *      Stop the rx action of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      None
 */
int32
r9330_rx_stop(uint32 unit)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9330_rx_register
 * Description:
 *      Register to receive callbacks for received packets of the specified device.
 * Input:
 *      unit     - unit id
 *      priority - Relative priority of the callback
 *      fRxCb    - pointer to a handler of received packets
 *      pCookie  - application data returned with callback (can be null)
 *      flags    - optional flags for reserved
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
r9330_rx_register(uint32 unit, uint8 priority, drv_nic_rx_cb_f fRxCb, void *pCookie, uint32 flags)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9330_rx_unregister
 * Description:
 *      Unregister to receive callbacks for received packets of the specified device.
 * Input:
 *      unit     - unit id
 *      priority - Relative priority of the callback
 *      fRxCb    - pointer to a handler of received packets (can be null)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
r9330_rx_unregister(uint32 unit, uint8 priority, drv_nic_rx_cb_f fRxCb)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9330_pkt_alloc
 * Description:
 *      Packet allocate API in the specified device.
 * Input:
 *      unit     - unit id
 *      size     - packet size
 *      flags    - flags
 * Output:
 *      ppPacket - pointer buffer of packet
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
r9330_pkt_alloc(uint32 unit, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9330_pkt_free
 * Description:
 *      Packet free API in the specified device.
 * Input:
 *      unit     - unit id
 *      pPacket  - pointer buffer of packet
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
r9330_pkt_free(uint32 unit, drv_nic_pkt_t *pPacket)
{
    return RT_ERR_OK;
}

/* NIC Tx/Rx debug */
/* Function Name:
 *      r9330_debug_set
 * Description:
 *      Set NIC debug flags of the specified device.
 * Input:
 *      unit  - unit id
 *      flags - NIC debug flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      There are 4 BIT flags can be selected as following:
 *      - DEBUG_RX_RAW_LEN_BIT
 *      - DEBUG_RX_CPU_TAG_BIT
 *      - DEBUG_TX_RAW_LEN_BIT
 *      - DEBUG_TX_CPU_TAG_BIT
 */
int32
r9330_debug_set(uint32 unit, uint32 flags)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9330_debug_get
 * Description:
 *      Get NIC debug flags of the specified device.
 * Input:
 *      unit   - unit id
 * Output:
 *      pFlags - NIC debug flags
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      There are 4 BIT flags as following:
 *      - DEBUG_RX_RAW_LEN_BIT
 *      - DEBUG_RX_CPU_TAG_BIT
 *      - DEBUG_TX_RAW_LEN_BIT
 *      - DEBUG_TX_CPU_TAG_BIT
 */
int32
r9330_debug_get(uint32 unit, uint32 *pFlags)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9330_counter_dump
 * Description:
 *      Dump NIC debug counter information of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      There are 4 debug counters be dump as following:
 *      - nic_tx_success_cntr
 *      - nic_tx_failed_cntr
 *      - nic_rx_success_cntr
 *      - nic_rx_failed_cntr
 */
int32
r9330_counter_dump(uint32 unit)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9330_counter_clear
 * Description:
 *      Clear NIC debug counter information of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      Clear following NIC debug counters
 *      - nic_tx_success_cntr
 *      - nic_tx_failed_cntr
 *      - nic_rx_success_cntr
 *      - nic_rx_failed_cntr
 */
int32
r9330_counter_clear(uint32 unit)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9330_bufStatus_dump
 * Description:
 *      Dump NIC buffer status of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      Following message are dump
 *      1) From SW View
 *      - Rx Ring Packet Header (FDPBase, CDPIdx, RDPIdx)
 *      - Tx Ring Packet Header (FDPBase, CDPIdx, RDPIdx)
 *      2) From HW View
 *      - Rx Ring Packet Header(CDPIdx)
 *      - Tx Ring Packet Header(CDPIdx)
 *      3) Register Information
 *      - CPUIIMR (CPU Interface Interrupt Mask Register)
 *      - CPUIISR (CPU Interface Interrupt Status Register)
 *      - CPUICR  (CPU Interface Control Register)
 */
int32
r9330_bufStatus_dump(uint32 unit)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9330_pkthdrMbuf_dump
 * Description:
 *      Dump NIC packet header and mbuf detail information of the specified device.
 * Input:
 *      unit  - unit id
 *      mode  - tx/rx mode
 *      start - start ring id
 *      end   - end ring id
 *      flags - dump flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1) valid 'mode' value:
 *      - NIC_PKTHDR_MBUF_MODE_RX
 *      - NIC_PKTHDR_MBUF_MODE_TX
 *      2) valid ring id (start .. end)
 *      - Rx (0 .. 7)
 *      - Tx (0 .. 1)
 *      3) valid 'flags' value:
 *      - TRUE: include packet raw data
 *      - FALSE: exclude packet raw data
 */
int32
r9330_pkthdrMbuf_dump(uint32 unit, uint32 mode, uint32 start, uint32 end, uint32 flags)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9330_rxStatus_get
 * Description:
 *      Get NIC rx status of the specified device.
 * Input:
 *      unit    - unit id
 * Output:
 *      pStatus - rx status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      None
 */
int32
r9330_rxStatus_get(uint32 unit, uint32 *pStatus)
{
    return RT_ERR_OK;
}

int32 r9330_local_ringSts_get(uint32 unit, uint32 *pSts)
{
    ioal_mem32_field_read(unit, RTL9330_PDMA_LOCAL_RING_STS_ADDR, RTL9330_PDMA_LOCAL_RING_STS_LOCAL_RING_EMPTY_OFFSET, RTL9330_PDMA_LOCAL_RING_STS_LOCAL_RING_EMPTY_MASK, pSts);

    return RT_ERR_OK;
}

int32 r9330_ringInfo_get(uint32 unit, nic_dir_t type, uint32 **ringIdSizeList, uint32 *pRingNum)
{
    if (type == NIC_DIR_RX)
    {
        *ringIdSizeList = rxRingIdSize_9330;
        *pRingNum = NIC_9330_RXRING_NUM;
    }
    else
    {
        *ringIdSizeList = txRingIdSize_9330;
        *pRingNum = NIC_9330_TXRING_NUM;
    }

    return RT_ERR_OK;
}

int32 r9330_cpuPortTxRxEnable_set(uint32 unit, rtk_enable_t enable)
{
    ioal_mem32_field_write(unit, RTL9330_MAC_L2_PORT_CTRL_ADDR(PORT_CPU), RTL9330_MAC_L2_PORT_CTRL_TX_EN_OFFSET, RTL9330_MAC_L2_PORT_CTRL_TX_EN_MASK, enable);
    ioal_mem32_field_write(unit, RTL9330_MAC_L2_PORT_CTRL_ADDR(PORT_CPU), RTL9330_MAC_L2_PORT_CTRL_RX_EN_OFFSET, RTL9330_MAC_L2_PORT_CTRL_RX_EN_MASK, enable);

    ioal_mem32_field_write(unit, RTL9330_VLAN_PORT_IGR_CTRL_ADDR(PORT_CPU), RTL9330_VLAN_PORT_IGR_CTRL_IGR_FLTR_EN_OFFSET, RTL9330_VLAN_PORT_IGR_CTRL_IGR_FLTR_EN_MASK, 0);

    return RT_ERR_OK;
}

int32 r9330_intrMask_get(uint32 unit, nic_intr_type_t type, uint32 *pMask)
{
    if (type == NIC_RX_DONE)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_RX_DONE_MSK_ADDR, RTL9330_PDMA_INTR_RX_DONE_MSK_RX_DONE_OFFSET, RTL9330_PDMA_INTR_RX_DONE_MSK_RX_DONE_MASK, pMask);
    else if (type == NIC_RX_RUNOUT)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_RX_RUNOUT_MSK_ADDR, RTL9330_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_OFFSET, RTL9330_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_MASK, pMask);
    else if (type == NIC_TX_DONE)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_DONE_OFFSET, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_DONE_MASK, pMask);
    else if (type == NIC_TX_ALLDONE)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_OFFSET, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_MASK, pMask);
    else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
        ioal_mem32_field_read(unit, RTL9330_L2_NTFY_INTR_MSK_ADDR, RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK, pMask);

    return RT_ERR_OK;
}

int32 r9330_intrMask_set(uint32 unit, nic_intr_type_t type, uint32 mask)
{
    if (type == NIC_RX_DONE)
        ioal_mem32_field_write(unit, RTL9330_PDMA_INTR_RX_DONE_MSK_ADDR, RTL9330_PDMA_INTR_RX_DONE_MSK_RX_DONE_OFFSET, RTL9330_PDMA_INTR_RX_DONE_MSK_RX_DONE_MASK, mask);
    else if (type == NIC_RX_RUNOUT)
        ioal_mem32_field_write(unit, RTL9330_PDMA_INTR_RX_RUNOUT_MSK_ADDR, RTL9330_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_OFFSET, RTL9330_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_MASK, mask);
    else if (type == NIC_TX_DONE)
        ioal_mem32_field_write(unit, RTL9330_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_DONE_OFFSET, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_DONE_MASK, mask);
    else if (type == NIC_TX_ALLDONE)
        ioal_mem32_field_write(unit, RTL9330_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_OFFSET, RTL9330_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_MASK, mask);
    else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
        ioal_mem32_field_write(unit, RTL9330_L2_NTFY_INTR_MSK_ADDR, RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK, mask);

    return RT_ERR_OK;
}

int32 r9330_intrSts_get(uint32 unit, nic_intr_type_t type, uint32 *pVal)
{
    if (type == NIC_RX_DONE)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_STS_ADDR, RTL9330_PDMA_INTR_STS_RX_DONE_OFFSET, RTL9330_PDMA_INTR_STS_RX_DONE_MASK, pVal);
    else if (type == NIC_RX_RUNOUT)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_STS_ADDR, RTL9330_PDMA_INTR_STS_RX_RUN_OUT_OFFSET, RTL9330_PDMA_INTR_STS_RX_DONE_MASK, pVal);
    else if (type == NIC_TX_DONE)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_STS_ADDR, RTL9330_PDMA_INTR_STS_TX_DONE_OFFSET, RTL9330_PDMA_INTR_STS_TX_DONE_MASK, pVal);
    else if (type == NIC_TX_ALLDONE)
        ioal_mem32_field_read(unit, RTL9330_PDMA_INTR_STS_ADDR, RTL9330_PDMA_INTR_STS_TX_ALL_DONE_OFFSET, RTL9330_PDMA_INTR_STS_TX_ALL_DONE_MASK, pVal);
    else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
        ioal_mem32_field_read(unit, RTL9330_L2_NTFY_INTR_STS_ADDR, RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK, pVal);

    return RT_ERR_OK;
}

int32 r9330_intrSts_set(uint32 unit, nic_intr_type_t type, uint32 val)
{
    if (type == NIC_RX_DONE)
        ioal_mem32_write(unit, RTL9330_PDMA_INTR_STS_ADDR, (val << RTL9330_PDMA_INTR_STS_RX_DONE_OFFSET) & RTL9330_PDMA_INTR_STS_RX_DONE_MASK);
    else if (type == NIC_RX_RUNOUT)
        ioal_mem32_write(unit, RTL9330_PDMA_INTR_STS_ADDR, (val << RTL9330_PDMA_INTR_STS_RX_RUN_OUT_OFFSET) & RTL9330_PDMA_INTR_STS_RX_RUN_OUT_MASK);
    else if (type == NIC_TX_DONE)
        ioal_mem32_write(unit, RTL9330_PDMA_INTR_STS_ADDR, (val << RTL9330_PDMA_INTR_STS_TX_DONE_OFFSET) & RTL9330_PDMA_INTR_STS_TX_DONE_MASK);
    else if (type == NIC_TX_ALLDONE)
        ioal_mem32_write(unit, RTL9330_PDMA_INTR_STS_ADDR, (val << RTL9330_PDMA_INTR_STS_TX_ALL_DONE_OFFSET) & RTL9330_PDMA_INTR_STS_TX_ALL_DONE_MASK);
    else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
        ioal_mem32_write(unit, RTL9330_L2_NTFY_INTR_STS_ADDR, (val << RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET) & RTL9330_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK);

    return RT_ERR_OK;
}

int32 r9330_swNicRst_get(uint32 unit, uint32 *pStatus)
{
    ioal_mem32_field_read(unit, RTL9330_RST_GLB_CTRL_0_ADDR, RTL9330_RST_GLB_CTRL_0_SW_NIC_RST_OFFSET, RTL9330_RST_GLB_CTRL_0_SW_NIC_RST_MASK, pStatus);
    return RT_ERR_OK;
}

int32 r9330_swNicRst_set(uint32 unit)
{
    RT_LOG(LOG_DEBUG, MOD_NIC, "Reset NIC (R9330)... ");
    ioal_mem32_field_write(unit, RTL9330_RST_GLB_CTRL_0_ADDR, RTL9330_RST_GLB_CTRL_0_SW_NIC_RST_OFFSET, RTL9330_RST_GLB_CTRL_0_SW_NIC_RST_MASK, 1);
    return RT_ERR_OK;
}

int32 r9330_swQueRst_get(uint32 unit, uint32 *pStatus)
{
    ioal_mem32_field_read(unit, RTL9330_RST_GLB_CTRL_0_ADDR, RTL9330_RST_GLB_CTRL_0_SW_Q_RST_OFFSET, RTL9330_RST_GLB_CTRL_0_SW_Q_RST_MASK, pStatus);

    return RT_ERR_OK;
}

int32 r9330_swQueRst_set(uint32 unit)
{
    ioal_mem32_field_write(unit, RTL9330_RST_GLB_CTRL_0_ADDR, RTL9330_RST_GLB_CTRL_0_SW_Q_RST_OFFSET, RTL9330_RST_GLB_CTRL_0_SW_Q_RST_MASK, 1);
    osal_time_mdelay(50); /* delay 50mS */

    return RT_ERR_OK;
}

int32 r9330_cpuL2FloodMask_add(uint32 unit)
{
    uint32 i, data[4];

    ioal_mem32_write(unit, RTL9330_TBL_ACCESS_CTRL_5_ADDR, 0x86000000);
    for (i = 0; i < 4; i++)
        ioal_mem32_read(unit, RTL9330_TBL_ACCESS_DATA_5_ADDR(i), &data[i]);

    data[0] |= 0x20000000;
    ioal_mem32_write(unit, RTL9330_TBL_ACCESS_DATA_5_ADDR(0), data[0]);
    ioal_mem32_write(unit, RTL9330_TBL_ACCESS_CTRL_5_ADDR, 0xC6000000);

    return RT_ERR_OK;
}

int32 r9330_cpuL2FloodMask_remove(uint32 unit)
{
    uint32 i, data[4];

    ioal_mem32_write(unit, RTL9330_TBL_ACCESS_CTRL_5_ADDR, 0x86000000);
    for (i = 0; i < 4; i++)
        ioal_mem32_read(unit, RTL9330_TBL_ACCESS_DATA_5_ADDR(i), &data[i]);

    data[0] &= ~(1 << 29);
    ioal_mem32_write(unit, RTL9330_TBL_ACCESS_DATA_5_ADDR(0), data[0]);
    ioal_mem32_write(unit, RTL9330_TBL_ACCESS_CTRL_5_ADDR, 0xC6000000);

    return RT_ERR_OK;
}

int32 r9330_cpuForceLinkupEnable_set(uint32 unit, rtk_enable_t enable)
{
    uint32      val, i;

    /* Force link */
    val = (enable == ENABLED) ? 1 : 0;
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_LINK_EN_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_LINK_EN_MASK, val);
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_LINK_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_LINK_MASK, val);

    /* Force duplex: Full duplex */
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_DUP_EN_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_DUP_EN_MASK, 1);
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_DUP_SEL_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_DUP_SEL_MASK, 1);

    /* Force speed: 10G */
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_SPD_EN_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_SPD_EN_MASK, 1);
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_SPD_SEL_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_SPD_SEL_MASK, 4);

    /* Force flow control: disable */
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_FC_EN_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_FORCE_FC_EN_MASK, 1);
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_RX_PAUSE_EN_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_RX_PAUSE_EN_MASK, 0);
    ioal_mem32_field_write(unit, RTL9330_PORT_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9330_PORT_FORCE_MODE_CTRL_SMI_TX_PAUSE_EN_OFFSET, RTL9330_PORT_FORCE_MODE_CTRL_SMI_TX_PAUSE_EN_MASK, 0);

    /* Egress Queue Drop */
    for (i = 0; i < NIC_9330_QUE_NUM; i++)
        ioal_mem32_field_write(unit, RTL9330_FC_CPU_FORCE_DROP_CTRL_ADDR(i), \
            RTL9330_FC_CPU_FORCE_DROP_CTRL_EN_OFFSET(i), RTL9330_FC_CPU_FORCE_DROP_CTRL_EN_MASK(i), 1);

    return RT_ERR_OK;
}

int32 r9330_holRingSize_set(uint32 unit, uint32 ring, uint32 val)
{
    ioal_mem32_field_write(unit, RTL9330_PDMA_HOL_RX_RING_SUPR_AMOUNT_ADDR(ring), RTL9330_PDMA_HOL_RX_RING_SUPR_AMOUNT_AMOUNT_OFFSET(ring),
                                RTL9330_PDMA_HOL_RX_RING_SUPR_AMOUNT_AMOUNT_MASK(ring), val);
    return RT_ERR_OK;
}

int32 r9330_holRingCnt_get(uint32 unit, uint32 ring, uint32 *pVal)
{
#if 0
    uint32 tmpVal;

    ioal_mem32_read(unit, RTL9330_DMA_IF_RX_RING_CNTR_ADDR(ring), &tmpVal);
    *pVal = (tmpVal & RTL9330_DMA_IF_RX_RING_CNTR_CNTR_MASK(ring)) >> RTL9330_DMA_IF_RX_RING_CNTR_CNTR_OFFSET(ring);
#endif

    return RT_ERR_OK;
}

int32 r9330_holRingCnt_set(uint32 unit, uint32 ring, uint32 val)
{
#if 0
    ioal_mem32_write(unit, RTL9330_DMA_IF_RX_RING_CNTR_ADDR(ring), ((val << RTL9330_DMA_IF_RX_RING_CNTR_CNTR_OFFSET(ring)) & RTL9330_DMA_IF_RX_RING_CNTR_CNTR_MASK(ring)));
#endif

    return RT_ERR_OK;
}

int32 r9330_ntfyBaseAddr_get(uint32 unit, uintptr *pVal)
{
    return RT_ERR_OK;
}

int32 r9330_ntfyBaseAddr_set(uint32 unit, uintptr val)
{
    return RT_ERR_OK;
}

int32 r9330_ringBaseAddr_get(uint32 unit, nic_dir_t dir, uint32 ring, uintptr *pVal)
{
#ifdef CONFIG_SDK_WORDSIZE_64
    uint32 val0, val1;
#endif


    if (dir == NIC_DIR_RX)
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9330_PDMA_RX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9330_PDMA_RX_BASE_DESC_ADDR_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uint64)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9330_PDMA_RX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    else
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9330_PDMA_TX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9330_PDMA_TX_BASE_DESC_ADDR_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uint64)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9330_PDMA_TX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    return RT_ERR_OK;
}

int32 r9330_ringBaseAddr_set(uint32 unit, nic_dir_t dir, uint32 ring, uintptr val)
{
    if (dir == NIC_DIR_RX)
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_write(unit, RTL9330_PDMA_RX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), val & 0xffffffff);
        ioal_mem32_write(unit, RTL9330_PDMA_RX_BASE_DESC_ADDR_ADDR(ring, ADDR_HI), val >> 32);
#else
        ioal_mem32_write(unit, RTL9330_PDMA_RX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), val);
#endif
    }
    else
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_write(unit, RTL9330_PDMA_TX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), val & 0xffffffff);
        ioal_mem32_write(unit, RTL9330_PDMA_TX_BASE_DESC_ADDR_ADDR(ring, ADDR_HI), val >> 32);
#else
        ioal_mem32_write(unit, RTL9330_PDMA_TX_BASE_DESC_ADDR_ADDR(ring, ADDR_LO), val);
#endif
    }
    return RT_ERR_OK;
}

int32 r9330_ringCurAddr_get(uint32 unit, nic_dir_t dir, uint32 ring, uintptr *pVal)
{
#ifdef CONFIG_SDK_WORDSIZE_64
    uint32 val0, val1;
#endif


    if (dir == NIC_DIR_RX)
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9330_PDMA_RX_CUR_DESC_ADDR_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9330_PDMA_RX_CUR_DESC_ADDR_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uint64)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9330_PDMA_RX_CUR_DESC_ADDR_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    else
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9330_PDMA_TX_CUR_DESC_ADDR_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9330_PDMA_TX_CUR_DESC_ADDR_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uint64)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9330_PDMA_TX_CUR_DESC_ADDR_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    return RT_ERR_OK;
}

int32 r9330_cpuPtr_get(uint32 unit, nic_dir_t dir, uint32 ring, uint32 *pVal)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_read(unit, RTL9330_PDMA_TX_CPU_PTR_ADDR(ring), pVal);
    else
        ioal_mem32_read(unit, RTL9330_PDMA_RX_CPU_PTR_ADDR(ring), pVal);

    return RT_ERR_OK;
}

int32 r9330_cpuPtr_set(uint32 unit, nic_dir_t dir, uint32 ring, uint32 val)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_write(unit, RTL9330_PDMA_TX_CPU_PTR_ADDR(ring), val);
    else
        ioal_mem32_write(unit, RTL9330_PDMA_RX_CPU_PTR_ADDR(ring), val);

    return RT_ERR_OK;
}

int32 r9330_hwPtr_get(uint32 unit, nic_dir_t dir, uint32 ring, uint32 *pVal)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_read(unit, RTL9330_PDMA_TX_HW_PTR_ADDR(ring), pVal);
    else
        ioal_mem32_read(unit, RTL9330_PDMA_RX_HW_PTR_ADDR(ring), pVal);

    return RT_ERR_OK;
}

int32 r9330_hwPtr_set(uint32 unit, nic_dir_t dir, uint32 ring, uint32 val)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_write(unit, RTL9330_PDMA_TX_HW_PTR_ADDR(ring), val);
    else
        ioal_mem32_write(unit, RTL9330_PDMA_RX_HW_PTR_ADDR(ring), val);

    return RT_ERR_OK;
}

int32 r9330_rxTruncateEnable_get(uint32 unit, uint32 *pVal)
{
    ioal_mem32_field_read(unit, RTL9330_MAC_L2_CPU_TRUNCATE_ADDR, RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_EN_OFFSET,
                                                          RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_EN_MASK, pVal);
    return RT_ERR_OK;
}

int32 r9330_rxTruncateEnable_set(uint32 unit, uint32 val)
{
    ioal_mem32_field_write(unit, RTL9330_MAC_L2_CPU_TRUNCATE_ADDR, RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_EN_OFFSET,
                                                          RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_EN_MASK, val);
    return RT_ERR_OK;
}

int32 r9330_rxTruncateLength_get(uint32 unit, uint32 *pVal)
{
    ioal_mem32_field_read(unit, RTL9330_MAC_L2_CPU_TRUNCATE_ADDR, RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_LEN_OFFSET,
                                                          RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_LEN_MASK, pVal);
    return RT_ERR_OK;
}

int32 r9330_rxTruncateLength_set(uint32 unit, uint32 val)
{
    ioal_mem32_field_write(unit, RTL9330_MAC_L2_CPU_TRUNCATE_ADDR, RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_LEN_OFFSET,
                                                          RTL9330_MAC_L2_CPU_TRUNCATE_CPU_RX_TRUNCATE_LEN_MASK, val);
    return RT_ERR_OK;
}

int32 r9330_nicEnable_get(uint32 unit, nic_dir_t dir, uint32 *pVal)
{
    if (dir == NIC_DIR_RX)
    {
        ioal_mem32_field_read(unit, RTL9330_PDMA_CTRL_ADDR, RTL9330_PDMA_CTRL_RX_EN_OFFSET,
                                                              RTL9330_PDMA_CTRL_RX_EN_MASK, pVal);
    }
    else
    {
        ioal_mem32_field_read(unit, RTL9330_PDMA_CTRL_ADDR, RTL9330_PDMA_CTRL_TX_EN_OFFSET,
                                                              RTL9330_PDMA_CTRL_TX_EN_MASK, pVal);
    }
    return RT_ERR_OK;
}

int32 r9330_nicEnable_set(uint32 unit, nic_dir_t dir, uint32 val)
{
    if (dir == NIC_DIR_RX)
    {
        ioal_mem32_field_write(unit, RTL9330_PDMA_CTRL_ADDR, RTL9330_PDMA_CTRL_RX_EN_OFFSET,
                                                              RTL9330_PDMA_CTRL_RX_EN_MASK, val);
    }
    else
    {
        ioal_mem32_field_write(unit, RTL9330_PDMA_CTRL_ADDR, RTL9330_PDMA_CTRL_TX_EN_OFFSET,
                                                              RTL9330_PDMA_CTRL_TX_EN_MASK, val);
    }

    if (val)
        RT_LOG(LOG_DEBUG, MOD_NIC, "NIC (R9330) Rx Start... ");
    else
        RT_LOG(LOG_DEBUG, MOD_NIC, "NIC (R9330) Rx Stop... ");

    return RT_ERR_OK;
}

int32 r9330_nicTxFetch_set(uint32 unit, nic_txRing_t type, uint32 val)
{
#if 0
    if (type == NIC_TXRING_HIGH)
        ioal_mem32_field_write(unit, RTL9330_PDMA_TX_CPU_PTR_ADDR, RTL9330_PDMA_TX_CPU_PTR_FETCH_OFFSET,
                                                          RTL9330_PDMA_TX_CPU_PTR_FETCH_MASK, val);
    else
        ioal_mem32_field_write(unit, RTL9330_DMA_IF_CTRL_ADDR, RTL9330_DMA_IF_CTRL_TX_LOW_FETCH_OFFSET,
                                                          RTL9330_DMA_IF_CTRL_TX_LOW_FETCH_MASK, val);
#endif
    return RT_ERR_OK;
}

int32 r9330_nicTxBusySts_get(uint32 unit, nic_txRing_t type, uint32 *pVal)
{
    if (type == NIC_TXRING_HIGH)
        ioal_mem32_field_read(unit, RTL9330_PDMA_TX_HW_PTR_ADDR(1), RTL9330_PDMA_TX_HW_PTR_TX_BUSY_OFFSET,
                                                          RTL9330_PDMA_TX_HW_PTR_TX_BUSY_MASK, pVal);
    else
        ioal_mem32_field_read(unit, RTL9330_PDMA_TX_HW_PTR_ADDR(0), RTL9330_PDMA_TX_HW_PTR_TX_BUSY_OFFSET,
                                                          RTL9330_PDMA_TX_HW_PTR_TX_BUSY_MASK, pVal);
    return RT_ERR_OK;
}

int32 r9330_cpuTagId_get(uint32 unit, uint32 *pVal)
{
#if 0
    ioal_mem32_field_read(unit, RTL9330_MAC_CPU_TAG_ID_CTRL_ADDR, RTL9330_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_OFFSET,
                                                          RTL9330_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_MASK, pVal);
#endif
    return RT_ERR_OK;
}

int32 r9330_cpuTagId_set(uint32 unit, uint32 val)
{
#if 0
    ioal_mem32_field_write(unit, RTL9330_MAC_CPU_TAG_ID_CTRL_ADDR, RTL9330_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_OFFSET,
                                                          RTL9330_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_MASK, val);
#endif
    return RT_ERR_OK;
}

int32 r9330_supportJumboSize_get(uint32 unit, uint32 *pVal)
{
    *pVal = NIC_9330_JUMBO_SIZE;
    return RT_ERR_OK;
}

int32 r9330_cpuTagFromRaw_cnvt(uint32 unit, uint8 *pCpuTag, drv_nic_pkt_t *pPacket)
{
    nic_9330_cpuTag_t   *pHdrtag;


    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);
    

    pHdrtag = (nic_9330_cpuTag_t*)pPacket->data;

#if 0   //MAC_L2_CPU_TAG_SWAP.SWAP_EN = 0
    pHdrtag  = (nic_9330_cpuTag_t*)osal_alloc(sizeof(nic_9330_cpuTag_t));
    if (NULL == pHdrtag)
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "Out of memory !");
        return RT_ERR_FAILED;
    }

    osal_memset(pHdrtag, 0, sizeof(nic_9330_cpuTag_t));

    pHdrtag->un.rx.QID              = (*pCpuTag >> 2) & 0x3f;
    pHdrtag->un.rx.GIP              = ((*pCpuTag & 0x3) << 8) | *(pCpuTag + 1);
    pCpuTag += 2;
    pHdrtag->un.rx.ACL_MD           = (*pCpuTag << 8) | *(pCpuTag + 1);
    pCpuTag += 2;
    pHdrtag->un.rx.OTAG_PAR_EXIST   = (*pCpuTag >> 7) & 0x1;
    pHdrtag->un.rx.ITAG_PAR_EXIST   = (*pCpuTag >> 6) & 0x1;
    pHdrtag->un.rx.CPU_PKT_FMT      = (*pCpuTag >> 5) & 0x1;
    pHdrtag->un.rx.IL3IF            = ((*pCpuTag & 0x1f) << 8) | *(pCpuTag + 1);
    pCpuTag += 2;
    pHdrtag->un.rx.BCD              = (*pCpuTag << 5) | ((*(pCpuTag + 1) >> 3) & 0x1f);
    pCpuTag += 1;
    pHdrtag->un.rx.DISCARD_MON      = (*pCpuTag >> 2) & 0x1;
    pHdrtag->un.rx.TS_SEC_0         = *pCpuTag & 0x3;
    pCpuTag += 1;
    pHdrtag->un.rx.TS_SEC_1         = (*pCpuTag >> 6) & 0x3;
    pHdrtag->un.rx.TS_NSEC          = ((*pCpuTag & 0x3f) << 24) | (*(pCpuTag + 1) << 16) | (*(pCpuTag + 2) << 8) | *(pCpuTag + 3);
    pCpuTag += 4;
    pHdrtag->un.rx.SFLOW_HIT        = (*pCpuTag >> 7) & 0x1;
    pHdrtag->un.rx.SFLOW_DIR        = (*pCpuTag >> 6) & 0x1;
    pHdrtag->un.rx.PKT_BFD_START    = ((*pCpuTag & 0x3f) << 6) | ((*(pCpuTag + 1) >> 6) & 0x3);
    pCpuTag += 1;
    pHdrtag->un.rx.FLOW_SAMPLE      = (*pCpuTag >> 5) & 0x1;
    pHdrtag->un.rx.DOS              = (*pCpuTag >> 4) & 0x1;
    pHdrtag->un.rx.MAC_CST          = (*pCpuTag >> 3) & 0x1;
    pHdrtag->un.rx.NEW_SA           = (*pCpuTag >> 2) & 0x1;
    pHdrtag->un.rx.PMV_FBD          = (*pCpuTag >> 1) & 0x1;
    pHdrtag->un.rx.L2_STTC_PMV      = *pCpuTag & 0x1;
    pCpuTag += 1;
    pHdrtag->un.rx.L2_DYN_PMV       = (*pCpuTag >> 7) & 0x1;
    pHdrtag->un.rx.L2_HASH_FULL     = (*pCpuTag >> 6) & 0x1;
    pHdrtag->un.rx.TRUNC_OSZ        = (*pCpuTag >> 5) & 0x1;
    pHdrtag->un.rx.IPORT            = ((*pCpuTag & 0x1f) << 1) | ((*(pCpuTag + 1) >> 7) & 0x1);
    pCpuTag += 1;
    pHdrtag->un.rx.REASON           = *pCpuTag & 0x7f;
#endif

    _nic_9330_rx_reason_translate(pPacket, pHdrtag);
    
    pPacket->rx_tag.qid             = pHdrtag->un.rx.QID;
    pPacket->rx_tag.dev_id          = pHdrtag->un.rx.GIP >> 6;
    pPacket->rx_tag.source_port     = pHdrtag->un.rx.GIP & 0x3f;
    pPacket->rx_tag.acl_index       = pHdrtag->un.rx.ACL_MD;
    pPacket->rx_tag.svid_tagged     = pHdrtag->un.rx.OTAG_PAR_EXIST;
    pPacket->rx_tag.cvid_tagged     = pHdrtag->un.rx.ITAG_PAR_EXIST;
    pPacket->rx_tag.cpu_pkt_fmt     = pHdrtag->un.rx.CPU_PKT_FMT;
    pPacket->rx_tag.l3_if           = pHdrtag->un.rx.IL3IF;
    pPacket->rx_tag.fvid            = pHdrtag->un.rx.BCD;
    pPacket->rx_tag.discard_mon     = pHdrtag->un.rx.DISCARD_MON;
    pPacket->rx_tag.ts_sec          = (pHdrtag->un.rx.TS_SEC_0 << 2) | pHdrtag->un.rx.TS_SEC_1;
    pPacket->rx_tag.ts_nsec         = pHdrtag->un.rx.TS_NSEC;
    if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_RX))
    {
        pPacket->rx_tag.ext_dev_id      = pPacket->rx_tag.dev_id;
        pPacket->rx_tag.ext_source_port = pPacket->rx_tag.source_port;
    }
    pPacket->rx_tag.pkt_bfd_start   = pHdrtag->un.rx.PKT_BFD_START;
    pPacket->rx_tag.oversize        = pHdrtag->un.rx.TRUNC_OSZ;
    if (pHdrtag->un.rx.DISCARD_MON)
    {
        pPacket->rx_tag.physical_rx_port = 0;
        pPacket->rx_tag.reason           = ((pHdrtag->un.rx.IPORT & 0x1) << 7) | pHdrtag->un.rx.REASON;
    }
    else
    {
        
        pPacket->rx_tag.physical_rx_port = pHdrtag->un.rx.IPORT;
        pPacket->rx_tag.reason           = pHdrtag->un.rx.REASON;
    }


    pPacket->data   += NIC_9330_RX_TAG_SIZE;
    pPacket->length -= NIC_9330_RX_TAG_SIZE;

    return RT_ERR_OK;
}

int32 r9330_cpuTxTagFromRaw_cnvt(uint32 unit, uint8 *txTag, drv_nic_pkt_t *pPacket)
{
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);
    
    // TODO: should update to the newest version later
    // RTL9330_CPU_Tag_v0.30_20200818_evinlien
    if ((txTag[0] >> 4) == 1)
        pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_DST_IF;
    else if ((txTag[0] >> 4) == 2)
        pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_PHYISCAL_ONE_HOP;
    else if ((txTag[0] >> 4) == 3)
        pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_LOGICAL_ONE_HOP;
    else if ((txTag[0] >> 4) == 4)
        pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_UCST_CPU;
    else if ((txTag[0] >> 4) == 5)
        pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_BCST_CPU;
    else if ((txTag[0] >> 4) == 6)
        pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_PHYISCAL_BCST_CPU;
    else
        pPacket->tx_tag.fwd_type    = txTag[0] >> 4;
    pPacket->tx_tag.acl_act     = (txTag[0] >> 2) & 0x1;
    pPacket->tx_tag.dg_pkt      = txTag[0] & 0x1;
    pPacket->tx_tag.bp_stp      = txTag[1] >> 7;
    pPacket->tx_tag.bp_vlan_egr = (txTag[1] >> 6) & 0x1;
    pPacket->tx_tag.as_priority = (txTag[1] >> 5) & 0x1;
    pPacket->tx_tag.pg          = txTag[1] & 0x7;
    pPacket->tx_tag.dm_act      = (txTag[2] >> 6) & 0x3;
    pPacket->tx_tag.dm_offset   = txTag[2] & 0x3f;
    pPacket->tx_tag.priority    = (txTag[3] >> 2) & 0x3f;
    pPacket->tx_tag.ori_tagif_en= (txTag[3] >> 1) & 0x1;
    pPacket->tx_tag.ori_itagif  = txTag[3] & 0x1;
    pPacket->tx_tag.ori_otagif  = txTag[4] >> 7;
    pPacket->tx_tag.ori_evtagif = (txTag[4] >> 6) & 0x1;
    pPacket->tx_tag.fvid_en     = (txTag[4] >> 5) & 0x1;
    pPacket->tx_tag.fvid        = ((txTag[4] & 0x1f) << 8) | txTag[5];
    pPacket->tx_tag.src_filter_en = txTag[6] >> 7;
    pPacket->tx_tag.as_tagSts   = ~((txTag[6] >> 6) & 0x1);
    pPacket->tx_tag.spn         = ((txTag[6] & 0xf) << 8) | txTag[7];
    pPacket->tx_tag.dst_data    = (txTag[8] << 8) | txTag[9];

    return RT_ERR_OK;
}

int32 r9330_cpuTagToRaw_cnvt(uint32 unit, drv_nic_pkt_t *pPacket, uint8 *pCpuTag)
{
    uint8               *ptr8   = pCpuTag - NIC_9330_TX_TAG_SIZE;
    nic_9330_cpuTag_t   *pHdrtag = (nic_9330_cpuTag_t *)ptr8;
    uint16              *ptr16;


    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);

    if (pPacket->as_txtag)
    {
        switch (pPacket->tx_tag.fwd_type)
        {
            case NIC_FWD_TYPE_ALE:
                pHdrtag->un.tx.FWD_CODE = 0;break;
            case NIC_FWD_TYPE_DST_IF:
                pHdrtag->un.tx.FWD_CODE = 1;break;
            case NIC_FWD_TYPE_PHYISCAL_ONE_HOP:
                pHdrtag->un.tx.FWD_CODE = 2;break;
            case NIC_FWD_TYPE_LOGICAL_ONE_HOP:
                pHdrtag->un.tx.FWD_CODE = 3;break;
            case NIC_FWD_TYPE_UCST_CPU:
                pHdrtag->un.tx.FWD_CODE = 4;break;
            case NIC_FWD_TYPE_BCST_CPU:
                pHdrtag->un.tx.FWD_CODE = 5;break;
            case NIC_FWD_TYPE_PHYISCAL_BCST_CPU:
                pHdrtag->un.tx.FWD_CODE = 6;break;
            default:
                osal_printf("FATAL Error: fwd_type is not support!\n");
                return RT_ERR_FAILED;
        }

        pHdrtag->un.tx.rsvd_1        = 0;
        pHdrtag->un.tx.ACL_ACT       = pPacket->tx_tag.acl_act;
        pHdrtag->un.tx.rsvd_2        = 0;
        pHdrtag->un.tx.DG_PKT        = pPacket->tx_tag.dg_pkt;
        pHdrtag->un.tx.BP_STP        = pPacket->tx_tag.bp_stp;
        pHdrtag->un.tx.BP_VLAN_EGR   = pPacket->tx_tag.bp_vlan_egr;
        pHdrtag->un.tx.AS_QID        = pPacket->tx_tag.as_priority;
        pHdrtag->un.tx.rsvd_3        = 0;
        pHdrtag->un.tx.PG            = pPacket->tx_tag.pg;
        pHdrtag->un.tx.DM_ACT        = pPacket->tx_tag.dm_act;
        pHdrtag->un.tx.DM_OFFSET     = pPacket->tx_tag.dm_offset;
        pHdrtag->un.tx.QID           = pPacket->tx_tag.priority;
        pHdrtag->un.tx.ORI_TAGIF_EN  = pPacket->tx_tag.ori_tagif_en;
        pHdrtag->un.tx.ORI_ITAGIF    = pPacket->tx_tag.ori_itagif;
        pHdrtag->un.tx.ORI_OTAGIF    = pPacket->tx_tag.ori_otagif;
        pHdrtag->un.tx.ORI_EVTAGIF   = pPacket->tx_tag.ori_evtagif;
        pHdrtag->un.tx.BCD_EN        = pPacket->tx_tag.fvid_en;
        pHdrtag->un.tx.BCD           = pPacket->tx_tag.fvid;
        pHdrtag->un.tx.GIP_SIMU_EN   = pPacket->tx_tag.src_filter_en;
        pHdrtag->un.tx.PKT_MOD_EN    = pPacket->tx_tag.as_tagSts ? 0 : 1;
        pHdrtag->un.tx.rsvd_4        = 0;
        pHdrtag->un.tx.GIP           = pPacket->tx_tag.spn;
        pHdrtag->un.tx.DST_DATA      = pPacket->tx_tag.dst_data;
        //osal_printf("%s():%d  priority:%d  dst_data:%#x  bcd:%d\n", __FUNCTION__, __LINE__, pPacket->tx_tag.priority, pPacket->tx_tag.dst_data, pPacket->tx_tag.fvid);
        //osal_printf("%s():%d  pCpuTag:%p  ptr8:%p  val:%x %x %x %x %x %x %x %x %x %x\n", __FUNCTION__, __LINE__, pCpuTag, ptr8, *ptr8, *(ptr8+1), *(ptr8+2), *(ptr8+3), *(ptr8+4), *(ptr8+5), *(ptr8+6), *(ptr8+7), *(ptr8+8), *(ptr8+9));
        //ori = ptr8;

        ptr8 += 4;
        ptr16 = (uint16*)ptr8;
        *ptr16 = byte_swap16(*ptr16);
        ptr16++;
        *ptr16 = byte_swap16(*ptr16);
        ptr16++;
        *ptr16 = byte_swap16(*ptr16);

        //ptr8 = ori;
        //osal_printf("%s():%d  pCpuTag:%p  ptr8:%p  val:%x %x %x %x %x %x %x %x %x %x\n", __FUNCTION__, __LINE__, pCpuTag, ptr8, *ptr8, *(ptr8+1), *(ptr8+2), *(ptr8+3), *(ptr8+4), *(ptr8+5), *(ptr8+6), *(ptr8+7), *(ptr8+8), *(ptr8+9));


        pPacket->head   -= NIC_9330_TX_TAG_SIZE;
        pPacket->data   -= NIC_9330_TX_TAG_SIZE;
        pPacket->length += NIC_9330_TX_TAG_SIZE;
    }

    return RT_ERR_OK;
}

int32 r9330_cpuTag_dump(uint32 unit, drv_nic_pkt_t *pPacket)
{
    uint32 val;

    osal_printf("=== [NIC RX Debug - CPU Rx Tag Information] ============ \n");
    osal_printf(" QID : %d \n", pPacket->rx_tag.qid);
    osal_printf(" UNIT : %d \n", pPacket->rx_tag.dev_id);
    osal_printf(" SPN : %d \n", pPacket->rx_tag.source_port);
    osal_printf(" ACL_MD : %d \n", pPacket->rx_tag.acl_index);
    osal_printf(" OTAGIF : %d \n", pPacket->rx_tag.svid_tagged);
    osal_printf(" ITAGIF : %d \n", pPacket->rx_tag.cvid_tagged);
    osal_printf(" CPU_PKT_FMT : %d \n", pPacket->rx_tag.cpu_pkt_fmt);
    osal_printf(" IL3IF : %d \n", pPacket->rx_tag.l3_if);
    osal_printf(" BCD : %d \n", pPacket->rx_tag.fvid);
    osal_printf(" DISCARD_MON : %d \n", pPacket->rx_tag.discard_mon);
    osal_printf(" TS_SEC : %d \n", pPacket->rx_tag.ts_sec);
    osal_printf(" TS_NSEC : %d \n", pPacket->rx_tag.ts_nsec);
    if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_RX))
        osal_printf(" SFLOW_HIT : 1 (RX)\n");
    else if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_TX))
        osal_printf(" SFLOW_HIT : 1 (TX)\n");
    else
        osal_printf(" SFLOW_HIT : 0 \n");
    osal_printf(" PKT_BFD_START : %d \n", pPacket->rx_tag.pkt_bfd_start);
    val = IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_FLOW_SAMPLE) ? 1 : 0;
    osal_printf(" FLOW_SAMPLE : %d\n", val);
    val = IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_ATTACK) ? 1 : 0;
    osal_printf(" DOS : %d \n", val);
    osal_printf(" MAC_CST : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_MAC_CONSTRAINT));
    osal_printf(" NEW_SA : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_NEW_SA));
    osal_printf(" PMV_FORBID : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_PMV_FBD));
    osal_printf(" L2_STTC_PMV : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_STC_L2_PMV));
    osal_printf(" L2_DYN_PMV : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_DYN_L2_PMV));
    osal_printf(" TRUNC_OSZ : %d \n", pPacket->rx_tag.oversize);
    osal_printf(" HASH_FULL : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_HASH_FULL));
    osal_printf(" RPN : %d \n", pPacket->rx_tag.physical_rx_port);
    osal_printf(" REASON : %d \n", pPacket->rx_tag.reason);

    return RT_ERR_OK;
}

int32 r9330_rawTag_dump(uint32 unit, uint8 *pCpuTag)
{
    nic_9330_cpuTag_t   *pHdrtag = (nic_9330_cpuTag_t *)pCpuTag;

    osal_printf("=== [NIC TX Debug - CPU Tx Tag Information] ============ \n");
    osal_printf(" FWD_CODE : 0x%0x \n", pHdrtag->un.tx.FWD_CODE);
    osal_printf(" ACL_ACT : 0x%0x \n", pHdrtag->un.tx.ACL_ACT);
    osal_printf(" DG_PKT : 0x%0x \n", pHdrtag->un.tx.DG_PKT);
    osal_printf(" BP_STP : 0x%0x \n", pHdrtag->un.tx.BP_STP);
    osal_printf(" BP_VLAN_EGR : 0x%0x \n", pHdrtag->un.tx.BP_VLAN_EGR);
    osal_printf(" AS_QID : 0x%0x \n", pHdrtag->un.tx.AS_QID);
    osal_printf(" PG : 0x%0x \n", pHdrtag->un.tx.PG);
    osal_printf(" QID : 0x%0x \n", pHdrtag->un.tx.QID);
    osal_printf(" ORI_TAGIF_EN : 0x%0x \n", pHdrtag->un.tx.ORI_TAGIF_EN);
    osal_printf(" ORI_ITAGIF : 0x%0x \n", pHdrtag->un.tx.ORI_ITAGIF);
    osal_printf(" ORI_OTAGIF : 0x%0x \n", pHdrtag->un.tx.ORI_OTAGIF);
    osal_printf(" ORI_EVTAGIF : 0x%0x \n", pHdrtag->un.tx.ORI_EVTAGIF);
    osal_printf(" BCD_EN : 0x%0x \n", pHdrtag->un.tx.BCD_EN);
    osal_printf(" BCD : 0x%0x \n", pHdrtag->un.tx.BCD);
    osal_printf(" GIP_SIMU_EN : 0x%0x \n", pHdrtag->un.tx.GIP_SIMU_EN);
    osal_printf(" PKT_MOD_EN : 0x%0x \n", pHdrtag->un.tx.PKT_MOD_EN);
    osal_printf(" GIP : 0x%0x \n", pHdrtag->un.tx.GIP);
    osal_printf(" DM_ACT : 0x%0x \n", pHdrtag->un.tx.DM_ACT);
    osal_printf(" DM_OFFSET : 0x%0x \n", pHdrtag->un.tx.DM_OFFSET);
    osal_printf(" DST_DATA : 0x%0x \n", pHdrtag->un.tx.DST_DATA);

    return RT_ERR_OK;
}

int32 r9330_cpuTagLen_get(uint32 unit, uint32* rxTag_len, uint32* txTag_len)
{
    *rxTag_len = NIC_9330_RX_TAG_SIZE;
    *txTag_len = NIC_9330_TX_TAG_SIZE;
    return RT_ERR_OK;
}

