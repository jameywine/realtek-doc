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
 * $Revision: 102096 $
 * $Date: 2019-11-28 17:50:27 +0800 (周四, 28 11月 2019) $
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
#include <private/drv/nic/nic_rtl9311b.h>
#include <private/drv/swcore/swcore_rtl9311b.h>
#include <drv/nic/nic.h>
#include <osal/lib.h>
#include <hwp/hw_profile.h>
#include <common/debug/rt_log.h>
#include <common/rt_type.h>
#include <osal/time.h>
#include <osal/memory.h>
#include <ioal/ioal_init.h>

/*
 * Symbol Definition
 */
#define PORT_CPU				28
#define NIC_9311B_RXRING_NUM     (12)
#define NIC_9311B_TXRING_NUM     (4)
#define NIC_9311B_QUE_NUM        32
#define ADDR_LO                 0
#define ADDR_HI                 1

#define NIC_9311B_RX_PREFETCH        0x1
#define NIC_9311B_TX_PREFETCH        0x1
#define NIC_9311B_RX_PREFETCH_THR    0x1
#define NIC_9311B_TX_PREFETCH_THR    0x1
#define NIC_9311B_RX_CLOSE_THR       0x1
#define NIC_9311B_RX_CLOSE_TIMEOUT   0x1
#define NIC_9311B_RX_RING_SURP       0x0
#define NIC_9311B_L2NTFY_RING_IDX    12
#define NIC_9311B_RX_CPUTAG_LEN      16
#define NIC_9311B_TX_CPUTAG_LEN      16
#define NIC_TEST_TEMP       0

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
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_INVALID_VXLAN_HDR,           0, },
    {NIC_RX_REASON_VXLAN_CTRL_PKT,              0, },
    {NIC_RX_REASON_VXLAN_TT_EXCPT,              0, },
    {NIC_RX_REASON_OAM,                         0, },
    {NIC_RX_REASON_CFM,                         0, },
    {NIC_RX_REASON_CFM_ETHDM,                   0, },
    {NIC_RX_REASON_PARSE_EXCPT,                 0, },                   /* 10 */
    {NIC_RX_REASON_MALFORM,                     0, },
    {NIC_RX_REASON_SEC_IP_MAC_BIND,             0, },
    {NIC_RX_REASON_IPUC_RPF,                    0, },
    {NIC_RX_REASON_INNER_OUTTER_CFI,            0, },
    {NIC_RX_REASON_IVC,                         0, },
    {NIC_RX_REASON_IGR_VLAN_FILTER,             0, },
    {NIC_RX_REASON_L2_UC_MC_LUMIS,              0, },
    {NIC_RX_REASON_IP4_6_MC_LUMIS,              0, },
    {NIC_RX_REASON_RMA_PTP,                     NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA_USR_DEF,                 NIC_RX_REASON_RMA, },   /* 20 */
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_ECID_EQ_PCID,                0, },
    {NIC_RX_REASON_RMA_BPDU,                    NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA_LACP,                    NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA_LLDP,                    NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_EAPOL,                       NIC_RX_REASON_RMA, },
    {NIC_RX_REASON_RMA,                         0, },
    {NIC_RX_REASON_TUNL_IF_MAC,                 0, },
    {NIC_RX_REASON_TUNL_IP_CHK,                 0, },                   /* 30 */
    {NIC_RX_REASON_ROUTER_IF_MAC,               0, },
    {NIC_RX_REASON_L3UC_NON_IP,                 0, },
    {NIC_RX_REASON_IP4_6_HDR_ERROR,             0, },
    {NIC_RX_REASON_ROUTE_IP_CHK,                0, },
    {NIC_RX_REASON_L3_IP_MAC_MISMATCH,          0, },
    {NIC_RX_REASON_IP6_HOPBYHOP_OPTION,         0, },
    {NIC_RX_REASON_IP6_ROUTE_HDR,               0, },
    {NIC_RX_REASON_IP4_OPTIONS,                 0, },
    {NIC_RX_REASON_IP4_6_ROUTE_LUMIS,           0, },
    {NIC_RX_REASON_IPUC_NULL_ROUTE,             0, },                   /* 40 */
    {NIC_RX_REASON_IPUC_PBR_NULL_ROUTE,         0, },
    {NIC_RX_REASON_IPUC_HOST_ROUTE,             0, },
    {NIC_RX_REASON_IPUC_NET_ROUTE,              0, },
    {NIC_RX_REASON_IPMC_BDG_ENTRY,              0, },
    {NIC_RX_REASON_IPMC_ROUTE_ENTRY,            0, },
    {NIC_RX_REASON_IPMC_ASSERT,                 0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_ROUT_EXCPT_NH_ROUTE_TO_TUNL, 0, },
    {NIC_RX_REASON_IPUC_TTL,                    0, },                   /* 50 */
    {NIC_RX_REASON_IPMC_TTL,                    0, },
    {NIC_RX_REASON_IPUC_MTU,                    0, },
    {NIC_RX_REASON_IPMC_MTU,                    0, },
    {NIC_RX_REASON_IP4_6_ICMP_REDIR,            0, },
    {NIC_RX_REASON_IGMP_MLD,                    0, },
    {NIC_RX_REASON_DHCP_DHCP6,                  0, },
    {NIC_RX_REASON_ARP_REQ_REP_GRA,             0, },
    {NIC_RX_REASON_NEIGHBOR_DISCOVER,           0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_DUMMY,                       0, },
    {NIC_RX_REASON_NORMAL_FWD,                  0, },
};


/*QueueId:                             0                       8         */
static uint32   rxRingIdSize_9311B[] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
static uint32   txRingIdSize_9311B[] = {64, 64, 64, 64};
static uint8    nic_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};
#if NIC_TEST_TEMP
static uint32   cpuQ2ring_map[32] = {  0, 1, 2,  3,  4, 5, 6,  7,
                                       8, 9, 10, 11, 0, 1, 2,  3,
                                       4, 5, 6,  7,  8, 9, 10, 11,
                                       0, 1, 2,  3,  4, 5, 6,  7};
#endif

/*
 * Macro Definition
 */


/*
 * Function Declaration
 */

int32 r9311B_cpuQ2rxRing_get(uint32 unit, uint32 qid, uint32* pVal);
int32 r9311B_cpuQ2rxRing_set(uint32 unit, uint32 qid, uint32 ringId);


static int32 _nic_rx_reason_translate(drv_nic_pkt_t *pPacket, nic_9311B_cpuTag_t *pCpuTag)
{
#define RTL9311B_MAX_REASON_NM  128
    uint16 reason = pCpuTag->un.rx.REASON;

    if (pCpuTag->un.rx.ACL_OF_HIT == 1)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_ACL_HIT);
    if (pCpuTag->un.rx.TT_HIT)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_TUNL_TMNT);
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
    if (pCpuTag->un.rx.HASH_FULL)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_HASH_FULL);
    if (pCpuTag->un.rx.INVLD_SA)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_INVALID_SA);
    if (pCpuTag->un.rx.ATK_TYPE)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_ATTACK);
    if (pCpuTag->un.rx.MIR_HIT)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_MIRROR);
    if (NIC_9311B_SFLOW_RX == pCpuTag->un.rx.SFLOW)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_RX);
    else if (NIC_9311B_SFLOW_TX == pCpuTag->un.rx.SFLOW)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_TX);
    if (pCpuTag->un.rx.L2_ERR_PKT)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_CRC_ERROR);
    if (pCpuTag->un.rx.L3_ERR_PKT)
        NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L3_CHKSUM_ERROR);


    if (reason < RTL9311B_MAX_REASON_NM)
    {
        NIC_REASON_MASK_SET(*pPacket, reasonTbl[reason][0]);
        NIC_REASON_MASK_SET(*pPacket, reasonTbl[reason][1]);
    }

    return RT_ERR_OK;
}

#if 0
static int32
_raw_data_dump(uint8* ptr, uint32 len)
{
    uint32  i;

    for (i = 0; i < len; i++)
    {
        if (i == len)
            break;
        if (0 == (i % 16))
            osal_printf("[%04x] ", i);
        osal_printf("%02x ", *(unsigned char*)(ptr + i));
        if (15 == (i % 16))
            osal_printf("\n");
    }
    osal_printf("\n");
    return RT_ERR_OK;
}

#endif
/* Function Name:
 *      r9311B_init
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
r9311B_init(uint32 unit, drv_nic_initCfg_t *pInitCfg)
{
    uint32  i = 0;
//    uint32  temp, cnt = 0;





    RT_INIT_REENTRY_CHK(nic_init[unit]);

    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE)
    {
        ioal_mem32_field_write(unit, RTL9311B_PDMA_RX_RING_CPU_CTRL_ADDR, RTL9311B_PDMA_RX_RING_CPU_CTRL_RXRING_MAP_OFFSET, RTL9311B_PDMA_RX_RING_CPU_CTRL_RXRING_MAP_MASK, 0x1FFF);
        ioal_mem32_field_write(unit, RTL9311B_PDMA_TX_RING_CPU_CTRL_ADDR, RTL9311B_PDMA_TX_RING_CPU_CTRL_TXRING_MAP_OFFSET, RTL9311B_PDMA_TX_RING_CPU_CTRL_TXRING_MAP_MASK, 0xf);
    }
#if NIC_TEST_TEMP
    /* for test*/
    /* set crc action forward */
    ioal_mem32_field_write(unit, RTL9311B_TRAP_CTRL_ADDR, RTL9311B_TRAP_CTRL_L2_CRC_ERR_ACT_OFFSET, RTL9311B_TRAP_CTRL_L2_CRC_ERR_ACT_MASK, 0);

    /* set cpu port rx/tx max len*/
    ioal_mem32_field_write(unit, RTL9311B_MAC_L2_CPU_MAX_LEN_CTRL_ADDR, RTL9311B_MAC_L2_CPU_MAX_LEN_CTRL_CPU_PORT_TX_MAX_LEN_OFFSET, RTL9311B_MAC_L2_CPU_MAX_LEN_CTRL_CPU_PORT_TX_MAX_LEN_MASK, 0x3000);
    ioal_mem32_field_write(unit, RTL9311B_MAC_L2_CPU_MAX_LEN_CTRL_ADDR, RTL9311B_MAC_L2_CPU_MAX_LEN_CTRL_CPU_PORT_RX_MAX_LEN_OFFSET, RTL9311B_MAC_L2_CPU_MAX_LEN_CTRL_CPU_PORT_RX_MAX_LEN_MASK, 0x3000);

    /* disable cpu-tx packet crc recaculate */
    ioal_mem32_field_write(unit, RTL9311B_MAC_L2_CPU_CRC_ADDR, RTL9311B_MAC_L2_CPU_CRC_CRC_CPU_RC_EN_OFFSET, RTL9311B_MAC_L2_CPU_CRC_CRC_CPU_RC_EN_MASK, 0);


    /* set BPDU trap to cpu queue 1*/
    /* set EAPOL trap to cpu queue 2*/
    /* set LLDP trap to cpu queue 3*/
    /* set OAM PDU trap to queue 4*/
    /* set CFM (CCM) trap to queue 5*/
    /* set L2MC_LOOKUP_MISS trap to queue 6*/
    /* set PTP trap to queue 7*/
    /* set RMA-XX trap to queue 9*/
    /* set VLAN_PORT_IGR_FLTR trap to queue 8*/
    /* set RMA-USR-DEF trap to queue 10*/
    /* set LACP trap to queue 11*/
    for (i = 0; i < 28; i++)
    {
        ioal_mem32_field_write(unit, RTL9311B_RMA_PORT_BPDU_CTRL_ADDR(i), RTL9311B_RMA_PORT_BPDU_CTRL_ACT_OFFSET(i), RTL9311B_RMA_PORT_BPDU_CTRL_ACT_MASK(i), 0x2);
        ioal_mem32_field_write(unit, RTL9311B_RMA_PORT_LLDP_CTRL_ADDR(i), RTL9311B_RMA_PORT_LLDP_CTRL_ACT_OFFSET(i), RTL9311B_RMA_PORT_LLDP_CTRL_ACT_MASK(i), 0x2);
        ioal_mem32_field_write(unit, RTL9311B_RMA_PORT_EAPOL_CTRL_ADDR(i), RTL9311B_RMA_PORT_EAPOL_CTRL_ACT_OFFSET(i), RTL9311B_RMA_PORT_EAPOL_CTRL_ACT_MASK(i), 0x2);
        /*oam config: non-OAMPDU forward, OAM PDU trap to cpu, 4*/
        ioal_mem32_field_write(unit, RTL9311B_OAM_PORT_ACT_CTRL_ADDR(i), RTL9311B_OAM_PORT_ACT_CTRL_PAR_ACT_OFFSET, RTL9311B_OAM_PORT_ACT_CTRL_PAR_ACT_MASK, 0x1);
        ioal_mem32_field_write(unit, RTL9311B_OAM_PORT_ACT_CTRL_ADDR(i), RTL9311B_OAM_PORT_ACT_CTRL_MUX_ACT_OFFSET, RTL9311B_OAM_PORT_ACT_CTRL_MUX_ACT_MASK, 0x1);
        ioal_mem32_field_write(unit, RTL9311B_OAM_CTRL_ADDR, RTL9311B_OAM_CTRL_EN_OFFSET, RTL9311B_OAM_CTRL_EN_MASK, 0x1);
        /*CCM config: action trap*/
        ioal_mem32_write(unit, RTL9311B_CFM_RX_CCM_CTRL_ADDR, 0xAAAA);

        /*vlan profile 0 - L2MC_LOOKUP_MISS action trap*/
        ioal_mem32_field_write(unit, RTL9311B_VLAN_PROFILE_SET_ADDR(0), RTL9311B_VLAN_PROFILE_SET_L2MC_BRIDGE_LU_MIS_ACT_OFFSET, RTL9311B_VLAN_PROFILE_SET_L2MC_BRIDGE_LU_MIS_ACT_MASK, 0x2);

        /*PTP config: action trap*/
        ioal_mem32_field_write(unit, RTL9311B_RMA_PORT_PTP_CTRL_ADDR(i), RTL9311B_RMA_PORT_PTP_CTRL_UDP_ACT_OFFSET, RTL9311B_RMA_PORT_PTP_CTRL_UDP_ACT_MASK, 0x2);
        ioal_mem32_field_write(unit, RTL9311B_RMA_PORT_PTP_CTRL_ADDR(i), RTL9311B_RMA_PORT_PTP_CTRL_ETH2_ACT_OFFSET, RTL9311B_RMA_PORT_PTP_CTRL_ETH2_ACT_MASK, 0x2);


        /* RMA-01-2F config: action trap*/
        ioal_mem32_write(unit, RTL9311B_RMA_CTRL_0_ADDR, 0xAAAAAAAA);
        ioal_mem32_write(unit, RTL9311B_RMA_CTRL_1_ADDR, 0xAAAAAAAA);
        ioal_mem32_write(unit, RTL9311B_RMA_CTRL_2_ADDR, 0xAAAAAAAA);

        /* RMA-USR-DEF config: use entry 0, MAC 01-80-C2-00-00-30,  action trap*/
        ioal_mem32_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), 0x0180);
        ioal_mem32_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0) + 4, 0xc2000030);
        ioal_mem32_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0) + 8, 0x0180);
        ioal_mem32_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0) + 12, 0xc2000030);
        ioal_mem32_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0) + 16, 0x11);


        /* vlan ingress fliter action config: trap, vlan egress filter disable*/
        ioal_mem32_field_write(unit, RTL9311B_VLAN_PORT_IGR_FLTR_ADDR(i), RTL9311B_VLAN_PORT_IGR_FLTR_IGR_FLTR_ACT_OFFSET(i), RTL9311B_VLAN_PORT_IGR_FLTR_IGR_FLTR_ACT_MASK(i), 0x2);
        ioal_mem32_field_write(unit, RTL9311B_VLAN_PORT_EGR_FLTR_ADDR(i), RTL9311B_VLAN_PORT_EGR_FLTR_EGR_FLTR_EN_OFFSET(i), RTL9311B_VLAN_PORT_EGR_FLTR_EGR_FLTR_EN_MASK(i), 0);

//        ioal_mem32_field_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), RTL9311B_RMA_USR_DEF_CTRL_ADDR_MAX_HI_OFFSET, RTL9311B_RMA_USR_DEF_CTRL_ADDR_MAX_HI_MASK, 0x0180);
//        ioal_mem32_field_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), RTL9311B_RMA_USR_DEF_CTRL_ADDR_MAX_LO_OFFSET, RTL9311B_RMA_USR_DEF_CTRL_ADDR_MAX_LO_MASK, 0xc2000030);
//        ioal_mem32_field_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), RTL9311B_RMA_USR_DEF_CTRL_ADDR_MIN_HI_OFFSET, RTL9311B_RMA_USR_DEF_CTRL_ADDR_MIN_HI_MASK, 0x0180);
//        ioal_mem32_field_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), RTL9311B_RMA_USR_DEF_CTRL_ADDR_MIN_LO_OFFSET, RTL9311B_RMA_USR_DEF_CTRL_ADDR_MIN_LO_MASK, 0xc2000030);
//        ioal_mem32_field_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), RTL9311B_RMA_USR_DEF_CTRL_ACT_OFFSET, RTL9311B_RMA_USR_DEF_CTRL_ACT_MASK, 0x2);
//        ioal_mem32_field_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), RTL9311B_RMA_USR_DEF_CTRL_TYPE_OFFSET, RTL9311B_RMA_USR_DEF_CTRL_TYPE_MASK, 0);
//        ioal_mem32_field_write(unit, RTL9311B_RMA_USR_DEF_CTRL_ADDR(0), RTL9311B_RMA_USR_DEF_CTRL_EN_OFFSET, RTL9311B_RMA_USR_DEF_CTRL_EN_MASK, 1);

        /* LACP config: ???*/

    }

    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_3_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_3_RMA_BPDU_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_3_RMA_BPDU_MASK, 1);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_4_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_LLDP_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_LLDP_MASK, 3);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_4_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_EAPOL_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_EAPOL_MASK, 2);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_1_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_1_OAMPDU_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_1_OAMPDU_MASK, 4);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_1_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_1_CFM_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_1_CFM_MASK, 5);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_2_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_2_L2_UC_MC_BRIDGE_LU_MISS_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_2_L2_UC_MC_BRIDGE_LU_MISS_MASK, 6);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_3_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_3_PTP_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_3_PTP_MASK, 7);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_2_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_2_IGR_VLAN_FLTR_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_2_IGR_VLAN_FLTR_MASK, 8);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_4_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_XX_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_XX_MASK, 9);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_3_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_3_RMA_USR_DEF_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_3_RMA_USR_DEF_MASK, 10);
    ioal_mem32_field_write(unit, RTL9311B_QM_RSN2CPUQID_CTRL_4_ADDR, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_LACP_OFFSET, RTL9311B_QM_RSN2CPUQID_CTRL_4_RMA_LACP_MASK, 11);


    for (i = 0; i < sizeof(cpuQ2ring_map) / sizeof(uint32); i++)
    {
        r9311B_cpuQ2rxRing_set(unit, i, cpuQ2ring_map[i]);
    }


    /*TIGERSHARK-936*/
    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE)
    {
        ioal_mem32_field_write(unit, RTL9311B_PDMA_PHYSICAL_ADDR_MSK_ADDR(0), RTL9311B_PDMA_PHYSICAL_ADDR_MSK_MSK_OFFSET, RTL9311B_PDMA_PHYSICAL_ADDR_MSK_MSK_MASK, 0);
        ioal_mem32_field_write(unit, RTL9311B_PDMA_PHYSICAL_ADDR_MSK_ADDR(1), RTL9311B_PDMA_PHYSICAL_ADDR_MSK_MSK_OFFSET, RTL9311B_PDMA_PHYSICAL_ADDR_MSK_MSK_MASK, 0);
    }
    else
    {
        ioal_mem32_field_write(unit, RTL9311B_PDMA_EXT_PHYSICAL_ADDR_MSK_ADDR(0), RTL9311B_PDMA_EXT_PHYSICAL_ADDR_MSK_MSK_OFFSET, RTL9311B_PDMA_EXT_PHYSICAL_ADDR_MSK_MSK_MASK, 0);
        ioal_mem32_field_write(unit, RTL9311B_PDMA_EXT_PHYSICAL_ADDR_MSK_ADDR(1), RTL9311B_PDMA_EXT_PHYSICAL_ADDR_MSK_MSK_OFFSET, RTL9311B_PDMA_EXT_PHYSICAL_ADDR_MSK_MSK_MASK, 0);
    }

#endif


    // Set prefetch size
    ioal_mem32_field_write(unit, RTL9311B_PDMA_PREFETCH_CTRL_ADDR, RTL9311B_PDMA_PREFETCH_CTRL_RX_PREFETCH_SIZE_OFFSET, RTL9311B_PDMA_PREFETCH_CTRL_RX_PREFETCH_SIZE_MASK, NIC_9311B_RX_PREFETCH);
    ioal_mem32_field_write(unit, RTL9311B_PDMA_PREFETCH_CTRL_ADDR, RTL9311B_PDMA_PREFETCH_CTRL_TX_PREFETCH_SIZE_OFFSET, RTL9311B_PDMA_PREFETCH_CTRL_TX_PREFETCH_SIZE_MASK, NIC_9311B_TX_PREFETCH);

    // Set prefetch threshold
    ioal_mem32_field_write(unit, RTL9311B_PDMA_PREFETCH_CTRL_ADDR, RTL9311B_PDMA_PREFETCH_CTRL_RX_PREFETCH_THR_OFFSET, RTL9311B_PDMA_PREFETCH_CTRL_RX_PREFETCH_THR_MASK, NIC_9311B_RX_PREFETCH_THR);
    ioal_mem32_field_write(unit, RTL9311B_PDMA_PREFETCH_CTRL_ADDR, RTL9311B_PDMA_PREFETCH_CTRL_TX_PREFETCH_THR_OFFSET, RTL9311B_PDMA_PREFETCH_CTRL_TX_PREFETCH_THR_MASK, NIC_9311B_TX_PREFETCH_THR);

    // Set RX close threshold
    ioal_mem32_field_write(unit, RTL9311B_PDMA_PREFETCH_CTRL_ADDR, RTL9311B_PDMA_PREFETCH_CTRL_RX_CLOSE_THR_OFFSET, RTL9311B_PDMA_PREFETCH_CTRL_RX_CLOSE_THR_MASK, NIC_9311B_RX_CLOSE_THR);
    for (i = 0; i < (NIC_9311B_RXRING_NUM); i++) /*include l2 ntfy ring*/
    {
        // Set RX close timeout
        ioal_mem32_field_write(unit, RTL9311B_PDMA_CLOSE_TIMEOUT_CTRL_ADDR(i), RTL9311B_PDMA_CLOSE_TIMEOUT_CTRL_TIMEOUT_OFFSET(i), RTL9311B_PDMA_CLOSE_TIMEOUT_CTRL_TIMEOUT_MASK(i), NIC_9311B_RX_CLOSE_TIMEOUT);
        // HOL
        ioal_mem32_field_write(unit, RTL9311B_PDMA_HOL_RX_RING_SUPP_AMOUNT_ADDR(i), RTL9311B_PDMA_HOL_RX_RING_SUPP_AMOUNT_AMOUNT_OFFSET(i), RTL9311B_PDMA_HOL_RX_RING_SUPP_AMOUNT_AMOUNT_MASK(i), NIC_9311B_RX_RING_SURP);
        // RX ring size
        ioal_mem32_field_write(unit, RTL9311B_PDMA_RX_RING_SIZE_ADDR(i), RTL9311B_PDMA_RX_RING_SIZE_SIZE_OFFSET(i), RTL9311B_PDMA_RX_RING_SIZE_SIZE_MASK(i), rxRingIdSize_9311B[i]);
    }
    for (i = 0; i < NIC_9311B_TXRING_NUM; i++)
    {
        // TX ring size
        ioal_mem32_field_write(unit, RTL9311B_PDMA_TX_RING_SIZE_ADDR(i), RTL9311B_PDMA_TX_RING_SIZE_SIZE_OFFSET(i), RTL9311B_PDMA_TX_RING_SIZE_SIZE_MASK(i), txRingIdSize_9311B[i]);
    }


    nic_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}



/* Function Name:
 *      r9311B_rx_start
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
r9311B_rx_start(uint32 unit)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9311B_rx_stop
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
r9311B_rx_stop(uint32 unit)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9311B_rx_register
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
r9311B_rx_register(uint32 unit, uint8 priority, drv_nic_rx_cb_f fRxCb, void *pCookie, uint32 flags)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9311B_rx_unregister
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
r9311B_rx_unregister(uint32 unit, uint8 priority, drv_nic_rx_cb_f fRxCb)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9311B_pkt_alloc
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
r9311B_pkt_alloc(uint32 unit, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9311B_pkt_free
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
r9311B_pkt_free(uint32 unit, drv_nic_pkt_t *pPacket)
{
    return RT_ERR_OK;
}

/* NIC Tx/Rx debug */
/* Function Name:
 *      r9311B_debug_set
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
r9311B_debug_set(uint32 unit, uint32 flags)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9311B_debug_get
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
r9311B_debug_get(uint32 unit, uint32 *pFlags)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9311B_counter_dump
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
r9311B_counter_dump(uint32 unit)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9311B_counter_clear
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
r9311B_counter_clear(uint32 unit)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9311B_bufStatus_dump
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
r9311B_bufStatus_dump(uint32 unit)
{
    return RT_ERR_OK;
}


/* Function Name:
 *      r9311B_pkthdrMbuf_dump
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
r9311B_pkthdrMbuf_dump(uint32 unit, uint32 mode, uint32 start, uint32 end, uint32 flags)
{
    return RT_ERR_OK;
}

/* Function Name:
 *      r9311B_rxStatus_get
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
r9311B_rxStatus_get(uint32 unit, uint32 *pStatus)
{
    return RT_ERR_OK;
}

int32 r9311B_local_ringSts_get(uint32 unit, uint32 *pSts)
{
    ioal_mem32_field_read(unit, RTL9311B_PDMA_LOCAL_RING_STS_ADDR, RTL9311B_PDMA_LOCAL_RING_STS_LOCAL_RING_EMPTY_OFFSET, RTL9311B_PDMA_LOCAL_RING_STS_LOCAL_RING_EMPTY_MASK, pSts);

    return RT_ERR_OK;
}

int32 r9311B_ringInfo_get(uint32 unit, nic_dir_t type, uint32 **ringIdSizeList, uint32 *pRingNum)
{
    if (type == NIC_DIR_RX)
    {
        *ringIdSizeList = rxRingIdSize_9311B;
        *pRingNum = NIC_9311B_RXRING_NUM;
    }
    else
    {
        *ringIdSizeList = txRingIdSize_9311B;
        *pRingNum = NIC_9311B_TXRING_NUM;
    }

    return RT_ERR_OK;
}

int32 r9311B_cpuPortTxRxEnable_set(uint32 unit, rtk_enable_t enable)
{
    uint32      val;

    val = (enable == ENABLED) ? 1 : 0;

    ioal_mem32_field_write(unit, RTL9311B_MAC_L2_PORT_CTRL_ADDR(PORT_CPU), RTL9311B_MAC_L2_PORT_CTRL_TX_EN_OFFSET, RTL9311B_MAC_L2_PORT_CTRL_TX_EN_MASK, val);
    ioal_mem32_field_write(unit, RTL9311B_MAC_L2_PORT_CTRL_ADDR(PORT_CPU), RTL9311B_MAC_L2_PORT_CTRL_RX_EN_OFFSET, RTL9311B_MAC_L2_PORT_CTRL_RX_EN_MASK, val);

    ioal_mem32_field_write(unit, RTL9311B_VLAN_PORT_IGR_FLTR_ADDR(PORT_CPU), RTL9311B_VLAN_PORT_IGR_FLTR_IGR_FLTR_ACT_OFFSET(PORT_CPU), RTL9311B_VLAN_PORT_IGR_FLTR_IGR_FLTR_ACT_MASK(PORT_CPU), 0);

    return RT_ERR_OK;
}

int32 r9311B_intrMask_get(uint32 unit, nic_intr_type_t type, uint32 *pMask)
{

    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE)
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_RX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_RX_DONE_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_RX_DONE_MSK_RX_DONE_MASK, pMask);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_RX_RUNOUT_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_RX_RUNOUT_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_INTR_EXT_RX_RUNOUT_MSK_RX_RUN_OUT_MASK, pMask);
        else if (type == NIC_TX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_DONE_MASK, pMask);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_ALL_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_ALL_DONE_MASK, pMask);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_DONE_MASK, pMask);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_RUN_OUT_MASK, pMask);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK, pMask);
    }
    else
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_RX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_RX_DONE_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_INTR_RX_DONE_MSK_RX_DONE_MASK, pMask);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_RX_RUNOUT_MSK_ADDR, RTL9311B_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_MASK, pMask);
        else if (type == NIC_TX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_DONE_OFFSET, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_DONE_MASK, pMask);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_OFFSET, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_MASK, pMask);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_DONE_MASK, pMask);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_RUN_OUT_MASK, pMask);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK, pMask);
    }
    return RT_ERR_OK;
}

int32 r9311B_intrMask_set(uint32 unit, nic_intr_type_t type, uint32 mask)
{
    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE)
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_EXT_RX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_RX_DONE_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_RX_DONE_MSK_RX_DONE_MASK, mask);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_EXT_RX_RUNOUT_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_RX_RUNOUT_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_INTR_EXT_RX_RUNOUT_MSK_RX_RUN_OUT_MASK, mask);
        else if (type == NIC_TX_DONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_DONE_MASK, mask);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_ALL_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_TX_DONE_MSK_TX_ALL_DONE_MASK, mask);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_DONE_MASK, mask);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_RX_RUN_OUT_MASK, mask);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK, mask);
    }
    else
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_RX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_RX_DONE_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_INTR_RX_DONE_MSK_RX_DONE_MASK, mask);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_RX_RUNOUT_MSK_ADDR, RTL9311B_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_INTR_RX_RUNOUT_MSK_RX_RUN_OUT_MASK, mask);
        else if (type == NIC_TX_DONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_DONE_OFFSET, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_DONE_MASK, mask);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_INTR_TX_DONE_MSK_ADDR, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_OFFSET, RTL9311B_PDMA_INTR_TX_DONE_MSK_TX_ALL_DONE_MASK, mask);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_DONE_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_DONE_MASK, mask);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_MSK_RX_RUN_OUT_MASK, mask);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_field_write(unit, RTL9311B_PDMA_L2_NTFY_INTR_MSK_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_MSK_LOCAL_NTFY_BUF_RUN_OUT_MASK, mask);
    }
    return RT_ERR_OK;
}

int32 r9311B_intrSts_get(uint32 unit, nic_intr_type_t type, uint32 *pVal)
{
    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE)
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, RTL9311B_PDMA_INTR_EXT_STS_RX_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_STS_RX_DONE_MASK, pVal);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, RTL9311B_PDMA_INTR_EXT_STS_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_INTR_STS_RX_RUN_OUT_MASK, pVal);
        else if (type == NIC_TX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, RTL9311B_PDMA_INTR_EXT_STS_TX_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_STS_TX_DONE_MASK, pVal);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, RTL9311B_PDMA_INTR_EXT_STS_TX_ALL_DONE_OFFSET, RTL9311B_PDMA_INTR_EXT_STS_TX_ALL_DONE_MASK, pVal);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_DONE_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_DONE_MASK, pVal);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_RUN_OUT_MASK, pVal);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_ADDR, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_MASK, pVal);
    }
    else
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_STS_ADDR, RTL9311B_PDMA_INTR_STS_RX_DONE_OFFSET, RTL9311B_PDMA_INTR_STS_RX_DONE_MASK, pVal);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_STS_ADDR, RTL9311B_PDMA_INTR_STS_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_INTR_STS_RX_RUN_OUT_MASK, pVal);
        else if (type == NIC_TX_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_STS_ADDR, RTL9311B_PDMA_INTR_STS_TX_DONE_OFFSET, RTL9311B_PDMA_INTR_STS_TX_DONE_MASK, pVal);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_INTR_STS_ADDR, RTL9311B_PDMA_INTR_STS_TX_ALL_DONE_OFFSET, RTL9311B_PDMA_INTR_STS_TX_ALL_DONE_MASK, pVal);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_L2_NTFY_INTR_STS_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_DONE_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_DONE_MASK, pVal);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_L2_NTFY_INTR_STS_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_RUN_OUT_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_RUN_OUT_MASK, pVal);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_field_read(unit, RTL9311B_PDMA_L2_NTFY_INTR_STS_ADDR, RTL9311B_PDMA_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_OFFSET, RTL9311B_PDMA_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_MASK, pVal);
    }
    return RT_ERR_OK;
}

int32 r9311B_intrSts_set(uint32 unit, nic_intr_type_t type, uint32 val)
{
    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_PCIE)
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, (val << RTL9311B_PDMA_INTR_EXT_STS_RX_DONE_OFFSET) & RTL9311B_PDMA_INTR_EXT_STS_RX_DONE_MASK);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, (val << RTL9311B_PDMA_INTR_EXT_STS_RX_RUN_OUT_OFFSET) & RTL9311B_PDMA_INTR_EXT_STS_RX_RUN_OUT_MASK);
        else if (type == NIC_TX_DONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, (val << RTL9311B_PDMA_INTR_EXT_STS_TX_DONE_OFFSET) & RTL9311B_PDMA_INTR_EXT_STS_TX_DONE_MASK);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_EXT_STS_ADDR, (val << RTL9311B_PDMA_INTR_EXT_STS_TX_ALL_DONE_OFFSET) & RTL9311B_PDMA_INTR_EXT_STS_TX_ALL_DONE_MASK);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_ADDR, (val << RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_DONE_OFFSET) & RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_DONE_MASK);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_write(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_ADDR, (val << RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_RUN_OUT_OFFSET) & RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_RX_RUN_OUT_MASK);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_write(unit, RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_ADDR, (val << RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_OFFSET) & RTL9311B_PDMA_EXT_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_MASK);
    }
    else
    {
        if (type == NIC_RX_DONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_STS_ADDR, (val << RTL9311B_PDMA_INTR_STS_RX_DONE_OFFSET) & RTL9311B_PDMA_INTR_STS_RX_DONE_MASK);
        else if (type == NIC_RX_RUNOUT)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_STS_ADDR, (val << RTL9311B_PDMA_INTR_STS_RX_RUN_OUT_OFFSET) & RTL9311B_PDMA_INTR_STS_RX_RUN_OUT_MASK);
        else if (type == NIC_TX_DONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_STS_ADDR, (val << RTL9311B_PDMA_INTR_STS_TX_DONE_OFFSET) & RTL9311B_PDMA_INTR_STS_TX_DONE_MASK);
        else if (type == NIC_TX_ALLDONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_INTR_STS_ADDR, (val << RTL9311B_PDMA_INTR_STS_TX_ALL_DONE_OFFSET) & RTL9311B_PDMA_INTR_STS_TX_ALL_DONE_MASK);
        else if (type == NIC_NTFY_DONE)
            ioal_mem32_write(unit, RTL9311B_PDMA_L2_NTFY_INTR_STS_ADDR, (val << RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_DONE_OFFSET) & RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_DONE_MASK);
        else if (type == NIC_NTFY_BUF_RUNOUT)
            ioal_mem32_write(unit, RTL9311B_PDMA_L2_NTFY_INTR_STS_ADDR, (val << RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_RUN_OUT_OFFSET) & RTL9311B_PDMA_L2_NTFY_INTR_STS_RX_RUN_OUT_MASK);
        else if (type == NIC_NTFY_LOCALBUF_RUNOUT)
            ioal_mem32_write(unit, RTL9311B_PDMA_L2_NTFY_INTR_STS_ADDR, (val << RTL9311B_PDMA_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_OFFSET) & RTL9311B_PDMA_L2_NTFY_INTR_STS_LOCAL_NTFY_BUF_RUN_OUT_MASK);
    }
    return RT_ERR_OK;
}

int32 r9311B_swNicRst_get(uint32 unit, uint32 *pStatus)
{
    ioal_mem32_field_read(unit, RTL9311B_RST_GLB_CTRL_0_ADDR, RTL9311B_RST_GLB_CTRL_0_SW_NIC_RST_OFFSET, RTL9311B_RST_GLB_CTRL_0_SW_NIC_RST_MASK, pStatus);
    return RT_ERR_OK;
}

int32 r9311B_swNicRst_set(uint32 unit)
{
    RT_LOG(LOG_DEBUG, MOD_NIC, "Reset NIC (R9311B)... ");
    ioal_mem32_field_write(unit, RTL9311B_RST_GLB_CTRL_0_ADDR, RTL9311B_RST_GLB_CTRL_0_SW_NIC_RST_OFFSET, RTL9311B_RST_GLB_CTRL_0_SW_NIC_RST_MASK, 1);
    return RT_ERR_OK;
}

int32 r9311B_swQueRst_get(uint32 unit, uint32 *pStatus)
{
    ioal_mem32_field_read(unit, RTL9311B_RST_GLB_CTRL_0_ADDR, RTL9311B_RST_GLB_CTRL_0_SW_Q_RST_OFFSET, RTL9311B_RST_GLB_CTRL_0_SW_Q_RST_MASK, pStatus);

    return RT_ERR_OK;
}

int32 r9311B_swQueRst_set(uint32 unit)
{
    ioal_mem32_field_write(unit, RTL9311B_RST_GLB_CTRL_0_ADDR, RTL9311B_RST_GLB_CTRL_0_SW_Q_RST_OFFSET, RTL9311B_RST_GLB_CTRL_0_SW_Q_RST_MASK, 1);
    osal_time_mdelay(50); /* delay 50mS */
    return RT_ERR_OK;
}

int32 r9311B_cpuL2FloodMask_add(uint32 unit)
{
#if 0
    uint32 val;

    ioal_mem32_field_read(unit, RTL9311B_L2_UNKN_UC_FLD_PMSK_ADDR, 0, 0x1FFFFFF, &val);
    val |= 0x1000000;
    ioal_mem32_field_write(unit, RTL9311B_L2_UNKN_UC_FLD_PMSK_ADDR, 0, 0x1FFFFFF, val);
#endif
    return RT_ERR_OK;
}

int32 r9311B_cpuL2FloodMask_remove(uint32 unit)
{
#if 0
    uint32 val;

    /* Remove the CPU port from Lookup Miss Flooding Portmask */
    ioal_mem32_field_read(unit, RTL9311B_L2_UNKN_UC_FLD_PMSK_ADDR, 0, 0x1FFFFFF, &val);
    val &= ~(0x1000000);
    ioal_mem32_field_write(unit, RTL9311B_L2_UNKN_UC_FLD_PMSK_ADDR, 0, 0x1FFFFFF, val);
#endif
    return RT_ERR_OK;
}

int32 r9311B_cpuForceLinkupEnable_set(uint32 unit, rtk_enable_t enable)
{
    uint32      val, i;

    /* Force link */
    val = (enable == ENABLED) ? 1 : 0;
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_LINK_EN_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_LINK_EN_MASK, val);
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_LINK_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_LINK_MASK, val);

    /* Force duplex: Full duplex */
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_DUP_EN_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_DUP_EN_MASK, 1);
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_DUP_SEL_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_DUP_SEL_MASK, 1);

    /* Force speed: 1000M */
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_SPD_EN_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_SPD_EN_MASK, 1);
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_SPD_SEL_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_SPD_SEL_MASK, 4);

    /* Force flow control: enable */
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_FC_EN_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_FORCE_FC_EN_MASK, 1);
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_RX_PAUSE_EN_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_RX_PAUSE_EN_MASK, 1);
    ioal_mem32_field_write(unit, RTL9311B_MAC_FORCE_MODE_CTRL_ADDR(PORT_CPU), \
        RTL9311B_MAC_FORCE_MODE_CTRL_SMI_TX_PAUSE_EN_OFFSET, RTL9311B_MAC_FORCE_MODE_CTRL_SMI_TX_PAUSE_EN_MASK, 1);

    /* Egress Queue Drop */
    for (i = 0; i < NIC_9311B_QUE_NUM; i++)
        ioal_mem32_field_write(unit, RTL9311B_FC_CPU_Q_EGR_FORCE_DROP_CTRL_ADDR(i), \
            RTL9311B_FC_CPU_Q_EGR_FORCE_DROP_CTRL_EN_OFFSET(i), RTL9311B_FC_CPU_Q_EGR_FORCE_DROP_CTRL_EN_MASK(i), 1);

    return RT_ERR_OK;
}

int32 r9311B_holRingSize_set(uint32 unit, uint32 ring, uint32 val)
{
    ioal_mem32_field_write(unit, RTL9311B_PDMA_HOL_RX_RING_SUPP_AMOUNT_ADDR(ring), RTL9311B_PDMA_HOL_RX_RING_SUPP_AMOUNT_AMOUNT_OFFSET(ring),
                                RTL9311B_PDMA_HOL_RX_RING_SUPP_AMOUNT_AMOUNT_MASK(ring), val);
    return RT_ERR_OK;
}

int32 r9311B_holRingCnt_get(uint32 unit, uint32 ring, uint32 *pVal)
{
#if 0
    uint32 tmpVal;

    ioal_mem32_read(unit, RTL9311B_DMA_IF_RX_RING_CNTR_ADDR(ring), &tmpVal);
    *pVal = (tmpVal & RTL9311B_DMA_IF_RX_RING_CNTR_CNTR_MASK(ring)) >> RTL9311B_DMA_IF_RX_RING_CNTR_CNTR_OFFSET(ring);
#endif

    return RT_ERR_OK;
}

int32 r9311B_holRingCnt_set(uint32 unit, uint32 ring, uint32 val)
{
#if 0
    ioal_mem32_write(unit, RTL9311B_DMA_IF_RX_RING_CNTR_ADDR(ring), ((val << RTL9311B_DMA_IF_RX_RING_CNTR_CNTR_OFFSET(ring)) & RTL9311B_DMA_IF_RX_RING_CNTR_CNTR_MASK(ring)));
#endif

    return RT_ERR_OK;
}

int32 r9311B_ntfyBaseAddr_get(uint32 unit, uintptr *pVal)
{
    r9311B_ringBaseAddr_get(unit, NIC_DIR_RX, NIC_9311B_L2NTFY_RING_IDX, pVal);
    return RT_ERR_OK;
}

int32 r9311B_ntfyBaseAddr_set(uint32 unit, uintptr val)
{
    r9311B_ringBaseAddr_set(unit, NIC_DIR_RX, NIC_9311B_L2NTFY_RING_IDX, val);
    return RT_ERR_OK;
}

int32 r9311B_ringBaseAddr_get(uint32 unit, nic_dir_t dir, uint32 ring, uintptr *pVal)
{
#ifdef CONFIG_SDK_WORDSIZE_64
    uint32  val0, val1;
#endif
    if (dir == NIC_DIR_RX)
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uintptr)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    else
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uintptr)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    return RT_ERR_OK;
}

int32 r9311B_ringBaseAddr_set(uint32 unit, nic_dir_t dir, uint32 ring, uintptr val)
{
    if (dir == NIC_DIR_RX)
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_write(unit, RTL9311B_PDMA_RX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), val & 0xffffffff);
        ioal_mem32_write(unit, RTL9311B_PDMA_RX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_HI), val >> 32);
#else
        ioal_mem32_write(unit, RTL9311B_PDMA_RX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), val);
#endif
    }
    else
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_write(unit, RTL9311B_PDMA_TX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), val & 0xffffffff);
        ioal_mem32_write(unit, RTL9311B_PDMA_TX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_HI), val >> 32);
#else
        ioal_mem32_write(unit, RTL9311B_PDMA_TX_BASE_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), val);
#endif
    }
    return RT_ERR_OK;
}

int32 r9311B_ringCurAddr_get(uint32 unit, nic_dir_t dir, uint32 ring, uintptr *pVal)
{
#ifdef CONFIG_SDK_WORDSIZE_64
    uint32  val0, val1;
#endif

    if (dir == NIC_DIR_RX)
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_CUR_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_CUR_DESC_ADDR_CTRL_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uintptr)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_CUR_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    else
    {
#ifdef CONFIG_SDK_WORDSIZE_64
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_CUR_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), &val0);
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_CUR_DESC_ADDR_CTRL_ADDR(ring, ADDR_HI), &val1);
        *pVal = ((uintptr)val1 << 32) | val0;
#else
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_CUR_DESC_ADDR_CTRL_ADDR(ring, ADDR_LO), pVal);
#endif
    }
    return RT_ERR_OK;
}

int32 r9311B_cpuPtr_get(uint32 unit, nic_dir_t dir, uint32 ring, uint32 *pVal)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_CPU_PTR_ADDR(ring), pVal);
    else
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_CPU_PTR_ADDR(ring), pVal);

    return RT_ERR_OK;
}

int32 r9311B_cpuPtr_set(uint32 unit, nic_dir_t dir, uint32 ring, uint32 val)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_write(unit, RTL9311B_PDMA_TX_CPU_PTR_ADDR(ring), val);
    else
        ioal_mem32_write(unit, RTL9311B_PDMA_RX_CPU_PTR_ADDR(ring), val);

    return RT_ERR_OK;
}

int32 r9311B_hwPtr_get(uint32 unit, nic_dir_t dir, uint32 ring, uint32 *pVal)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_read(unit, RTL9311B_PDMA_TX_HW_PTR_ADDR(ring), pVal);
    else
        ioal_mem32_read(unit, RTL9311B_PDMA_RX_HW_PTR_ADDR(ring), pVal);

    return RT_ERR_OK;
}

int32 r9311B_hwPtr_set(uint32 unit, nic_dir_t dir, uint32 ring, uint32 val)
{
    if (dir == NIC_DIR_TX)
        ioal_mem32_write(unit, RTL9311B_PDMA_TX_HW_PTR_ADDR(ring), val);
    else
        ioal_mem32_write(unit, RTL9311B_PDMA_RX_HW_PTR_ADDR(ring), val);

    return RT_ERR_OK;
}

int32 r9311B_rxTruncateLength_get(uint32 unit, uint32 *pVal)
{
    ioal_mem32_field_read(unit, RTL9311B_PDMA_SW2NIC_TRUNCATE_ADDR, RTL9311B_PDMA_SW2NIC_TRUNCATE_TRUNCATE_LEN_OFFSET,
                                                          RTL9311B_PDMA_SW2NIC_TRUNCATE_TRUNCATE_LEN_MASK, pVal);
    return RT_ERR_OK;
}

int32 r9311B_rxTruncateLength_set(uint32 unit, uint32 val)
{
    ioal_mem32_field_write(unit, RTL9311B_PDMA_SW2NIC_TRUNCATE_ADDR, RTL9311B_PDMA_SW2NIC_TRUNCATE_TRUNCATE_LEN_OFFSET,
                                                          RTL9311B_PDMA_SW2NIC_TRUNCATE_TRUNCATE_LEN_MASK, val);
    return RT_ERR_OK;
}

int32 r9311B_nicEnable_get(uint32 unit, nic_dir_t dir, uint32 *pVal)
{
    if (dir == NIC_DIR_RX)
    {
        ioal_mem32_field_read(unit, RTL9311B_PDMA_CTRL_ADDR, RTL9311B_PDMA_CTRL_RX_EN_OFFSET,
                                                              RTL9311B_PDMA_CTRL_RX_EN_MASK, pVal);
    }
    else
    {
        ioal_mem32_field_read(unit, RTL9311B_PDMA_CTRL_ADDR, RTL9311B_PDMA_CTRL_TX_EN_OFFSET,
                                                              RTL9311B_PDMA_CTRL_TX_EN_MASK, pVal);
    }
    return RT_ERR_OK;
}

int32 r9311B_nicEnable_set(uint32 unit, nic_dir_t dir, uint32 val)
{
    if (dir == NIC_DIR_RX)
    {
        ioal_mem32_field_write(unit, RTL9311B_PDMA_CTRL_ADDR, RTL9311B_PDMA_CTRL_RX_EN_OFFSET,
                                                              RTL9311B_PDMA_CTRL_RX_EN_MASK, val);
    }
    else
    {
        ioal_mem32_field_write(unit, RTL9311B_PDMA_CTRL_ADDR, RTL9311B_PDMA_CTRL_TX_EN_OFFSET,
                                                              RTL9311B_PDMA_CTRL_TX_EN_MASK, val);
    }

    if (val)
        RT_LOG(LOG_DEBUG, MOD_NIC, "NIC (R9311B) Rx Start... ");
    else
        RT_LOG(LOG_DEBUG, MOD_NIC, "NIC (R9311B) Rx Stop... ");

    return RT_ERR_OK;
}

int32 r9311B_nicTxFetch_set(uint32 unit, nic_txRing_t type, uint32 val)
{
#if 0
    if (type == NIC_TXRING_HIGH)
        ioal_mem32_field_write(unit, RTL9311B_PDMA_TX_CPU_PTR_ADDR, RTL9311B_PDMA_TX_CPU_PTR_FETCH_OFFSET,
                                                          RTL9311B_PDMA_TX_CPU_PTR_FETCH_MASK, val);
    else
        ioal_mem32_field_write(unit, RTL9311B_DMA_IF_CTRL_ADDR, RTL9311B_DMA_IF_CTRL_TX_LOW_FETCH_OFFSET,
                                                          RTL9311B_DMA_IF_CTRL_TX_LOW_FETCH_MASK, val);
#endif
    return RT_ERR_OK;
}

int32 r9311B_nicTxBusySts_get(uint32 unit, nic_txRing_t type, uint32 *pVal)
{
    if (type == NIC_TXRING_HIGH)
        ioal_mem32_field_read(unit, RTL9311B_PDMA_TX_HW_PTR_ADDR(1), RTL9311B_PDMA_TX_HW_PTR_TX_BUSY_OFFSET,
                                                          RTL9311B_PDMA_TX_HW_PTR_TX_BUSY_MASK, pVal);
    else
        ioal_mem32_field_read(unit, RTL9311B_PDMA_TX_HW_PTR_ADDR(0), RTL9311B_PDMA_TX_HW_PTR_TX_BUSY_OFFSET,
                                                          RTL9311B_PDMA_TX_HW_PTR_TX_BUSY_MASK, pVal);
    return RT_ERR_OK;
}

int32 r9311B_cpuTagId_get(uint32 unit, uint32 *pVal)
{
#if 0
    ioal_mem32_field_read(unit, RTL9311B_MAC_CPU_TAG_ID_CTRL_ADDR, RTL9311B_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_OFFSET,
                                                          RTL9311B_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_MASK, pVal);
#endif
    return RT_ERR_OK;
}

int32 r9311B_cpuTagId_set(uint32 unit, uint32 val)
{
#if 0
    ioal_mem32_field_write(unit, RTL9311B_MAC_CPU_TAG_ID_CTRL_ADDR, RTL9311B_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_OFFSET,
                                                          RTL9311B_MAC_CPU_TAG_ID_CTRL_CPU_TAG_ID_MASK, val);
#endif
    return RT_ERR_OK;
}

int32 r9311B_supportJumboSize_get(uint32 unit, uint32 *pVal)
{
    *pVal = NIC_9311B_JUMBO_SIZE;
    return RT_ERR_OK;
}

int32 r9311B_cpuTagFromRaw_cnvt(uint32 unit, uint8 *pCpuTag, drv_nic_pkt_t *pPacket)
{
    nic_9311B_cpuTag_t cTag;

    RT_PARAM_CHK(NULL == pCpuTag, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);

    /*B0*/
    cTag.un.rx.SPA = (*pCpuTag) & 0x3f;
    pCpuTag++;
    /*B1*/
    cTag.un.rx.ACL_OF_HIT = ((*pCpuTag) >> 2) & 0x3;
    pCpuTag++;
    /*B2*/
    cTag.un.rx.SPN_IS_TRK = ((*pCpuTag) >> 7) & 0x1;
    cTag.un.rx.TRK_ID = (*pCpuTag) & 0x7f;
    pCpuTag++;
    /*B3*/
    cTag.un.rx.L2_ERR_PKT = ((*pCpuTag) >> 7) & 0x1;
    cTag.un.rx.L3_ERR_PKT = ((*pCpuTag) >> 6) & 0x1;
    cTag.un.rx.ATK_TYPE = (*pCpuTag) & 0x1f;
    pCpuTag++;
    /*B4*/
    cTag.un.rx.QID = ((*pCpuTag) >> 3) & 0x1f;
    cTag.un.rx.SPN = ((*pCpuTag) & 0x3) << 8;
    pCpuTag++;
    /*B5*/
    cTag.un.rx.SPN |= (*pCpuTag);
    pCpuTag++;
    /*B6*/
    cTag.un.rx.ORI_ETAG_IF = ((*pCpuTag) >> 7) & 0x1;
    cTag.un.rx.IDX = ((*pCpuTag) & 0x3f) << 8;
    pCpuTag++;
    /*B7*/
    cTag.un.rx.IDX |= (*pCpuTag);
    pCpuTag++;
    /*B8*/
    cTag.un.rx.MIR_HIT = ((*pCpuTag) >> 4) & 0xf;
    cTag.un.rx.SFLOW = ((*pCpuTag) >> 2) & 0x3;
    cTag.un.rx.TT_HIT = ((*pCpuTag) >> 1) & 0x1;
    cTag.un.rx.TT_IDX = ((*pCpuTag) & 0x1) << 8;
    pCpuTag++;
    /*B9*/
    cTag.un.rx.TT_IDX |= (*pCpuTag);
    pCpuTag++;
    /*B10*/
    cTag.un.rx.ETAG_IF = ((*pCpuTag) >> 7) & 0x1;
    cTag.un.rx.OTAG_IF = ((*pCpuTag) >> 6) & 0x1;
    cTag.un.rx.ITAG_IF = ((*pCpuTag) >> 5) & 0x1;
    cTag.un.rx.FVID_SEL = ((*pCpuTag) >> 4) & 0x1;
    cTag.un.rx.FVID = ((*pCpuTag) & 0xF) << 8;
    pCpuTag++;
    /*B11*/
    cTag.un.rx.FVID |= (*pCpuTag);
    pCpuTag++;
    /*B12*/
    cTag.un.rx.MAC_CST = ((*pCpuTag) >> 6) & 0x1;
    cTag.un.rx.DM_RXIDX = (*pCpuTag) & 0x3f;
    pCpuTag++;
    /*B13*/
    cTag.un.rx.NEW_SA = ((*pCpuTag) >> 7) & 0x1;
    cTag.un.rx.PMV_FBD = ((*pCpuTag) >> 6) & 0x1;
    cTag.un.rx.L2_STTC_PMV = ((*pCpuTag) >> 5) & 0x1;
    cTag.un.rx.L2_DYN_PMV = ((*pCpuTag) >> 4) & 0x1;
    cTag.un.rx.PORT_DATA_IS_TRK = ((*pCpuTag) >> 2) & 0x1;
    cTag.un.rx.PORT_DATA = ((*pCpuTag) & 0x3) << 8;
    pCpuTag++;
    /*B14*/
    cTag.un.rx.PORT_DATA |= (*pCpuTag);
    pCpuTag++;
    /*B15*/
    cTag.un.rx.HASH_FULL = ((*pCpuTag) >> 7) & 0x1;
    cTag.un.rx.INVLD_SA = ((*pCpuTag) >> 6) & 0x1;
    cTag.un.rx.REASON = (*pCpuTag) & 0x3f;

    _nic_rx_reason_translate(pPacket, &cTag);
    if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_ACL_HIT))
        pPacket->rx_tag.acl_index = cTag.un.rx.IDX;
    pPacket->rx_tag.physical_rx_port= cTag.un.rx.SPA;
    pPacket->rx_tag.is_trk          = cTag.un.rx.SPN_IS_TRK;
    pPacket->rx_tag.trk_id          = cTag.un.rx.TRK_ID;
    pPacket->rx_tag.atk_type        = cTag.un.rx.ATK_TYPE;
    pPacket->rx_tag.qid             = cTag.un.rx.QID;
    pPacket->rx_tag.dev_id          = cTag.un.rx.SPN >> 6;
    pPacket->rx_tag.source_port     = cTag.un.rx.SPN & 0x3f;
    pPacket->rx_tag.mirror_hit      = cTag.un.rx.MIR_HIT;
    if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_RX))
    {
        pPacket->rx_tag.ext_dev_id      = pPacket->rx_tag.dev_id;
        pPacket->rx_tag.ext_source_port = pPacket->rx_tag.source_port;
    }
    else if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_TX))
    {
        pPacket->rx_tag.ext_dev_id      = cTag.un.rx.IDX >> 6;
        pPacket->rx_tag.ext_source_port = cTag.un.rx.IDX & 0x3f;
    }
    else
    {
        pPacket->rx_tag.ext_is_trk      = cTag.un.rx.PORT_DATA_IS_TRK;
        if (pPacket->rx_tag.ext_is_trk)
            pPacket->rx_tag.ext_trk_id      = cTag.un.rx.PORT_DATA;
        else
        {
            pPacket->rx_tag.ext_dev_id      = cTag.un.rx.PORT_DATA >> 6;
            pPacket->rx_tag.ext_source_port = cTag.un.rx.PORT_DATA & 0x3f;
        }
    }
    pPacket->rx_tag.tt_idx          = cTag.un.rx.TT_IDX;
    pPacket->rx_tag.svid_tagged     = cTag.un.rx.OTAG_IF;
    pPacket->rx_tag.cvid_tagged     = cTag.un.rx.ITAG_IF;
    pPacket->rx_tag.fvid_sel        = cTag.un.rx.FVID_SEL;
    pPacket->rx_tag.fvid            = cTag.un.rx.FVID;
    pPacket->rx_tag.dm_rxIdx        = cTag.un.rx.DM_RXIDX;
    pPacket->rx_tag.reason          = cTag.un.rx.REASON;

    return RT_ERR_OK;
}

int32 r9311B_cpuTagToRaw_cnvt(uint32 unit, drv_nic_pkt_t *pPacket, uint8 *pCpuTag)
{
    nic_9311B_cpuTag_t cTag;

    RT_PARAM_CHK(NULL == pCpuTag, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);


    if (pPacket->as_txtag)
    {
        switch (pPacket->tx_tag.fwd_type)
        {
            case NIC_FWD_TYPE_ALE:
                cTag.un.tx.FWD_TYPE = 0;break;
            case NIC_FWD_TYPE_PHYISCAL:
                cTag.un.tx.FWD_TYPE = 1;break;
            case NIC_FWD_TYPE_LOGICAL:
                cTag.un.tx.FWD_TYPE = 2;break;
            case NIC_FWD_TYPE_TRUNK:
                cTag.un.tx.FWD_TYPE = 3;break;
            case NIC_FWD_TYPE_PHYISCAL_ONE_HOP:
                cTag.un.tx.FWD_TYPE = 4;break;
            case NIC_FWD_TYPE_LOGICAL_ONE_HOP:
                cTag.un.tx.FWD_TYPE = 5;break;
            case NIC_FWD_TYPE_UCST_CPU_MIN_PORT:
                cTag.un.tx.FWD_TYPE = 6;break;
            case NIC_FWD_TYPE_UCST_CPU:
                cTag.un.tx.FWD_TYPE = 7;break;
            case NIC_FWD_TYPE_BCST_CPU:
                cTag.un.tx.FWD_TYPE = 8;break;
            default:
                osal_printf("FATAL Error: fwd_type is not support!\n");
                return RT_ERR_FAILED;
        }

        cTag.un.tx.CPUTAGIF      = TRUE;
        cTag.un.tx.ACL_ACT       = pPacket->tx_tag.acl_act;
        cTag.un.tx.CNGST_DROP    = pPacket->tx_tag.cngst_drop;
        cTag.un.tx.DM_PKT        = pPacket->tx_tag.dm_pkt;
        cTag.un.tx.DG_PKT        = pPacket->tx_tag.dg_pkt;
        cTag.un.tx.BP_FLTR       = pPacket->tx_tag.bp_fltr;
        cTag.un.tx.BP_STP        = pPacket->tx_tag.bp_stp;
        cTag.un.tx.BP_VLAN_EGR   = pPacket->tx_tag.bp_vlan_egr;
        cTag.un.tx.AS_TAGSTS     = pPacket->tx_tag.as_tagSts;
        cTag.un.tx.L3_ACT        = pPacket->tx_tag.l3_act;
        cTag.un.tx.ORI_TAGIF_EN  = pPacket->tx_tag.ori_tagif_en;
        cTag.un.tx.AS_QID        = pPacket->tx_tag.as_priority;
        cTag.un.tx.QID           = pPacket->tx_tag.priority;
        cTag.un.tx.ORI_ITAG_IF   = pPacket->tx_tag.ori_itagif;
        cTag.un.tx.ORI_OTAG_IF   = pPacket->tx_tag.ori_otagif;
        cTag.un.tx.FVID_SEL      = pPacket->tx_tag.fvid_sel;
        cTag.un.tx.FVID_EN       = pPacket->tx_tag.fvid_en;
        cTag.un.tx.FVID          = pPacket->tx_tag.fvid;
        cTag.un.tx.SRC_FLTR_EN   = pPacket->tx_tag.src_filter_en;
        cTag.un.tx.SP_IS_TRK     = pPacket->tx_tag.sp_is_trk;
        cTag.un.tx.SPN9_4        = pPacket->tx_tag.spn >> 4;
        cTag.un.tx.SPN3_0        = pPacket->tx_tag.spn & 0xf;
        cTag.un.tx.SW_DEV_ID     = pPacket->tx_tag.dev_id;
        cTag.un.tx.DPM55_32      = pPacket->tx_tag.dst_port_mask_1;
        cTag.un.tx.DPM31_0       = pPacket->tx_tag.dst_port_mask;

        /*B0*/
        *pCpuTag = 0;
        pCpuTag++;
        /*B1*/
        *pCpuTag = 0;
        pCpuTag++;
        /*B2*/
        *pCpuTag = cTag.un.tx.FWD_TYPE;
        pCpuTag++;
        /*B3*/
        *pCpuTag = ((uint8)cTag.un.tx.ACL_ACT << 7)
                    | ((uint8)cTag.un.tx.CNGST_DROP << 6)
                    | ((uint8)cTag.un.tx.DM_PKT << 5)
                    | ((uint8)cTag.un.tx.DG_PKT << 4)
                    | ((uint8)cTag.un.tx.BP_FLTR << 3)
                    | ((uint8)cTag.un.tx.BP_STP << 2)
                    | ((uint8)cTag.un.tx.BP_VLAN_EGR << 1)
                    | ((uint8)cTag.un.tx.AS_TAGSTS << 0);
        pCpuTag++;
        /*B4*/
        *pCpuTag = ((uint8)cTag.un.tx.L3_ACT << 7)
                    | ((uint8)cTag.un.tx.ORI_TAGIF_EN << 6)
                    | ((uint8)cTag.un.tx.AS_QID << 5)
                    | ((uint8)cTag.un.tx.QID << 0);
        pCpuTag++;
        /*B5*/
        *pCpuTag = ((uint8)cTag.un.tx.ORI_ITAG_IF << 7)
                    | ((uint8)cTag.un.tx.ORI_OTAG_IF << 6)
                    | ((uint8)cTag.un.tx.FVID_SEL << 5)
                    | ((uint8)cTag.un.tx.FVID_EN << 4)
                    | ((cTag.un.tx.FVID >> 8) & 0xf);
        pCpuTag++;
        /*B6*/
        *pCpuTag =  ((cTag.un.tx.FVID) & 0xff);
        pCpuTag++;
        /*B7*/
        *pCpuTag = ((uint8)cTag.un.tx.SRC_FLTR_EN << 7)
                    | ((uint8)cTag.un.tx.SP_IS_TRK << 6)
                    | ((uint8)cTag.un.tx.SPN9_4 << 0);
        pCpuTag++;
        /*B8*/
        *pCpuTag = ((uint8)cTag.un.tx.SPN3_0 << 4)
                    | ((uint8)cTag.un.tx.SW_DEV_ID << 0);
        pCpuTag++;
        /*B9*/
        *pCpuTag = (uint8)((cTag.un.tx.DPM55_32 >> 16) & 0xff);
        pCpuTag++;
        /*B10*/
        *pCpuTag = (uint8)((cTag.un.tx.DPM55_32 >> 8) & 0xff);
        pCpuTag++;
        /*B11*/
        *pCpuTag = (uint8)((cTag.un.tx.DPM55_32 >> 0) & 0xff);
        pCpuTag++;
        /*B12*/
        *pCpuTag = (uint8)((cTag.un.tx.DPM31_0 >> 24) & 0xff);
        pCpuTag++;
        /*B13*/
        *pCpuTag = (uint8)((cTag.un.tx.DPM31_0 >> 16) & 0xff);
        pCpuTag++;
        /*B14*/
        *pCpuTag = (uint8)((cTag.un.tx.DPM31_0 >> 8) & 0xff);
        pCpuTag++;
        /*B15*/
        *pCpuTag = (uint8)((cTag.un.tx.DPM31_0 >> 0) & 0xff);
//        pCpuTag++;
    }
    else
        cTag.un.tx.CPUTAGIF = FALSE;

    return RT_ERR_OK;
}

int32 r9311B_cpuTag_dump(uint32 unit, drv_nic_pkt_t *pPacket)
{
    osal_printf("=== [NIC RX Debug - CPU Rx Tag Information] ============ \n");
    osal_printf(" RPN : %d \n", pPacket->rx_tag.physical_rx_port);
    osal_printf(" UNIT : %d \n", pPacket->rx_tag.dev_id);
    osal_printf(" SPN : %d \n", pPacket->rx_tag.source_port);
    osal_printf(" MIR_HIT : %d \n", pPacket->rx_tag.mirror_hit);
    if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_RX))
        osal_printf(" SFLOW_HIT : 1 \n");
    else if (IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_SFLOW_TX))
        osal_printf(" SFLOW_HIT : 2 \n");
    else
        osal_printf(" SFLOW_HIT : 0 \n");
    osal_printf(" ACL_HIT : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_ACL_HIT));
    osal_printf(" IDX : %d \n", pPacket->rx_tag.acl_index);
    osal_printf(" TT_HIT : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_TUNL_TMNT));
    osal_printf(" TT_IDX : %d \n", pPacket->rx_tag.tt_idx);
    osal_printf(" IS_TRK : %d \n", pPacket->rx_tag.is_trk);
    osal_printf(" TRK_ID : %d \n", pPacket->rx_tag.trk_id);
    osal_printf(" OTAGIF : %d \n", pPacket->rx_tag.svid_tagged);
    osal_printf(" ITAGIF : %d \n", pPacket->rx_tag.cvid_tagged);
    osal_printf(" OVID : %d \n", pPacket->rx_tag.outer_vid);
    osal_printf(" IVID : %d \n", pPacket->rx_tag.inner_vid);
    osal_printf(" FWD_VID_SEL : %d \n", pPacket->rx_tag.fvid_sel);
    osal_printf(" FVID : %d \n", pPacket->rx_tag.fvid);
    osal_printf(" QID : %d \n", pPacket->rx_tag.qid);
    osal_printf(" ATK_TYPE : %d \n", pPacket->rx_tag.atk_type);
    osal_printf(" MAC_CST : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_MAC_CONSTRAINT));
    osal_printf(" DM_RXIDX : %d \n", pPacket->rx_tag.dm_rxIdx);
    osal_printf(" NEW_SA : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_NEW_SA));
    osal_printf(" PMV_FORBID : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_PMV_FBD));
    osal_printf(" L2_STTC_PMV : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_STC_L2_PMV));
    osal_printf(" L2_DYN_PMV : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_DYN_L2_PMV));
    osal_printf(" L2_ERR_PKT : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_CRC_ERROR));
    osal_printf(" L3_ERR_PKT : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L3_CHKSUM_ERROR));
    if (pPacket->rx_tag.ext_is_trk)
        osal_printf(" ext_trkId : %d \n", pPacket->rx_tag.ext_trk_id);
    else
    {
        osal_printf(" ext_unit : %d \n", pPacket->rx_tag.ext_dev_id);
        osal_printf(" ext_source_port : %d \n", pPacket->rx_tag.ext_source_port);
    }
    osal_printf(" HASH_FULL : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_L2_HASH_FULL));
    osal_printf(" INVALID_SA : %d \n", IS_NIC_REASON_MASK_SET(*pPacket, NIC_RX_REASON_INVALID_SA));
    osal_printf(" REASON : %d \n", pPacket->rx_tag.reason);

    return RT_ERR_OK;
}

int32 r9311B_rawTag_dump(uint32 unit, uint8 *pCpuTag)
{
    nic_9311B_cpuTag_t cTag;
#if 0
    switch ((pCpuTag[2] & 0xf))
    {
        case 1:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_PHYISCAL;
            break;
        case 2:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_LOGICAL;
            break;
        case 3:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_TRUNK;
            break;
        case 4:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_PHYISCAL_ONE_HOP;
            break;
        case 5:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_LOGICAL_ONE_HOP;
            break;
        case 6:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_UCST_CPU_MIN_PORT;
            break;
        case 7:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_UCST_CPU;
            break;
        case 8:
            pHdrtag->un.tx.FWD_TYPE = NIC_FWD_TYPE_BCST_CPU;
            break;
        default:
            break;
    }
#else
    cTag.un.tx.FWD_TYPE = (pCpuTag[2] & 0xf);
#endif
    cTag.un.tx.ACL_ACT     = pCpuTag[3] >> 7;
    cTag.un.tx.CNGST_DROP  = (pCpuTag[3] >> 6) & 0x1;
    cTag.un.tx.DM_PKT      = (pCpuTag[3] >> 5) & 0x1;
    cTag.un.tx.DG_PKT      = (pCpuTag[3] >> 4) & 0x1;
    cTag.un.tx.BP_FLTR     = (pCpuTag[3] >> 3) & 0x1;
    cTag.un.tx.BP_STP      = (pCpuTag[3] >> 2) & 0x1;
    cTag.un.tx.BP_VLAN_EGR = (pCpuTag[3] >> 1) & 0x1;
    cTag.un.tx.AS_TAGSTS   = (pCpuTag[3]) & 0x1;
    cTag.un.tx.L3_ACT      = pCpuTag[4] >> 7;
    cTag.un.tx.ORI_TAGIF_EN= (pCpuTag[4] >> 6) & 0x1;
    cTag.un.tx.AS_QID = (pCpuTag[4] >> 5) & 0x1;
    cTag.un.tx.QID    = (pCpuTag[4]) & 0x1f;
    cTag.un.tx.ORI_ITAG_IF  = (pCpuTag[5] >> 7) & 0x1;
    cTag.un.tx.ORI_OTAG_IF  = (pCpuTag[5] >> 6) & 0x1;
    cTag.un.tx.FVID_SEL    = (pCpuTag[5] >> 5) & 0x1;
    cTag.un.tx.FVID_EN     = (pCpuTag[5] >> 4);
    cTag.un.tx.FVID        = ((pCpuTag[5] & 0xf) << 8) | (pCpuTag[6]);
    cTag.un.tx.SRC_FLTR_EN = (pCpuTag[7] >> 7);
    cTag.un.tx.SP_IS_TRK   = (pCpuTag[7] >> 6) & 0x1;
    cTag.un.tx.SPN9_4 = pCpuTag[7] & 0x3f;
    cTag.un.tx.SW_DEV_ID        = pCpuTag[8] & 0xf;
    cTag.un.tx.SPN3_0 = pCpuTag[8] >> 4;
    cTag.un.tx.DPM55_32 = ((uint32)pCpuTag[9] << 16) | ((uint32)pCpuTag[10] << 8) | pCpuTag[11];
    cTag.un.tx.DPM31_0 = ((uint32)pCpuTag[12] << 24) | ((uint32)pCpuTag[13] << 16) | ((uint32)pCpuTag[14] << 8) | pCpuTag[15];



    osal_printf("=== [NIC TX Debug - CPU Tx Tag Information] ============ \n");
    osal_printf(" FWD_TYPE : 0x%0x \n", cTag.un.tx.FWD_TYPE);
    osal_printf(" ACL_ACT : 0x%0x \n", cTag.un.tx.ACL_ACT);
    osal_printf(" CNGST_DROP : 0x%0x \n", cTag.un.tx.CNGST_DROP);
    osal_printf(" DM_PKT : 0x%0x \n", cTag.un.tx.DM_PKT);
    osal_printf(" DG_PKT : 0x%0x \n", cTag.un.tx.DG_PKT);
    osal_printf(" BP_FLTR : 0x%0x \n", cTag.un.tx.BP_FLTR);
    osal_printf(" BP_STP : 0x%0x \n", cTag.un.tx.BP_STP);
    osal_printf(" BP_VLAN_EGR : 0x%0x \n", cTag.un.tx.BP_VLAN_EGR);
    osal_printf(" AS_TAGSTS : 0x%0x \n", cTag.un.tx.AS_TAGSTS);
    osal_printf(" ORI_TAGIF_EN : 0x%0x \n", cTag.un.tx.ORI_TAGIF_EN);
    osal_printf(" L3_ACT : 0x%0x \n", cTag.un.tx.L3_ACT);
    osal_printf(" AS_QID : 0x%0x \n", cTag.un.tx.AS_QID);
    osal_printf(" QID : 0x%0x \n", cTag.un.tx.QID);
    osal_printf(" ORI_ITAGIF : 0x%0x \n", cTag.un.tx.ORI_ITAG_IF);
    osal_printf(" ORI_OTAGIF : 0x%0x \n", cTag.un.tx.ORI_OTAG_IF);
    osal_printf(" FVID_SEL : 0x%0x \n", cTag.un.tx.FVID_SEL);
    osal_printf(" FVID_EN : 0x%0x \n", cTag.un.tx.FVID_EN);
    osal_printf(" FVID : 0x%0x \n", cTag.un.tx.FVID);
    osal_printf(" SRC_FLTR_EN : 0x%0x \n", cTag.un.tx.SRC_FLTR_EN);
    osal_printf(" SP_IS_TRK : 0x%0x \n", cTag.un.tx.SP_IS_TRK);
    osal_printf(" SPN : 0x%0x \n", (cTag.un.tx.SPN9_4 << 4) | cTag.un.tx.SPN3_0);
    osal_printf(" SW_DEV_ID : 0x%0x \n", cTag.un.tx.SW_DEV_ID);
    osal_printf(" DPM55_32 : 0x%0x \n", cTag.un.tx.DPM55_32);
    osal_printf(" DPM31_0 : 0x%0x \n", cTag.un.tx.DPM31_0);

    return RT_ERR_OK;
}


int32 r9311B_cpuTxTagFromRaw_cnvt(uint32 unit, uint8 *txTag, drv_nic_pkt_t *pPacket)
{
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);


    switch ((txTag[2] & 0xf))
    {
        case 1:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_PHYISCAL;
            break;
        case 2:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_LOGICAL;
            break;
        case 3:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_TRUNK;
            break;
        case 4:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_PHYISCAL_ONE_HOP;
            break;
        case 5:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_LOGICAL_ONE_HOP;
            break;
        case 6:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_UCST_CPU_MIN_PORT;
            break;
        case 7:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_UCST_CPU;
            break;
        case 8:
            pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_BCST_CPU;
            break;
        default:
            break;
    }

    pPacket->tx_tag.acl_act     = txTag[3] >> 7;
    pPacket->tx_tag.cngst_drop  = (txTag[3] >> 6) & 0x1;
    pPacket->tx_tag.dm_pkt      = (txTag[3] >> 5) & 0x1;
    pPacket->tx_tag.dg_pkt      = (txTag[3] >> 4) & 0x1;
    pPacket->tx_tag.bp_fltr     = (txTag[3] >> 3) & 0x1;
    pPacket->tx_tag.bp_stp      = (txTag[3] >> 2) & 0x1;
    pPacket->tx_tag.bp_vlan_egr = (txTag[3] >> 1) & 0x1;
    pPacket->tx_tag.as_tagSts   = (txTag[3]) & 0x1;
    pPacket->tx_tag.l3_act      = txTag[4] >> 7;
    pPacket->tx_tag.ori_tagif_en= (txTag[4] >> 6) & 0x1;
    pPacket->tx_tag.as_priority = (txTag[4] >> 5) & 0x1;
    pPacket->tx_tag.priority    = (txTag[4]) & 0x1f;
    pPacket->tx_tag.ori_itagif  = (txTag[5] >> 7) & 0x1;
    pPacket->tx_tag.ori_otagif  = (txTag[5] >> 6) & 0x1;
    pPacket->tx_tag.fvid_sel    = (txTag[5] >> 5) & 0x1;
    pPacket->tx_tag.fvid_en     = (txTag[5] >> 4);
    pPacket->tx_tag.fvid        = ((txTag[5] & 0xf) << 8) | (txTag[6]);
    pPacket->tx_tag.src_filter_en = (txTag[7] >> 7);
    pPacket->tx_tag.sp_is_trk   = (txTag[7] >> 6) & 0x1;
    pPacket->tx_tag.spn         = ((txTag[7] & 0x3f) << 4) | (txTag[8] >> 4);
    pPacket->tx_tag.dev_id        = txTag[8] & 0xf;
    pPacket->tx_tag.dst_port_mask_1 = ((uint32)txTag[9] << 16) | ((uint32)txTag[10] << 8) | txTag[11];
    pPacket->tx_tag.dst_port_mask = ((uint32)txTag[12] << 24) | ((uint32)txTag[13] << 16) | ((uint32)txTag[14] << 8) | txTag[15];

    return RT_ERR_OK;
}



int32 r9311B_cpuQ2rxRing_get(uint32 unit, uint32 qid, uint32* pVal)
{
    ioal_mem32_field_read(unit, RTL9311B_PDMA_CPU_QID2RING_ADDR(qid), RTL9311B_PDMA_CPU_QID2RING_RXRING_ID_OFFSET(qid), RTL9311B_PDMA_CPU_QID2RING_RXRING_ID_MASK(qid), pVal);
    return RT_ERR_OK;
}


int32 r9311B_cpuQ2rxRing_set(uint32 unit, uint32 qid, uint32 ringId)
{
    ioal_mem32_field_write(unit, RTL9311B_PDMA_CPU_QID2RING_ADDR(qid), RTL9311B_PDMA_CPU_QID2RING_RXRING_ID_OFFSET(qid), RTL9311B_PDMA_CPU_QID2RING_RXRING_ID_MASK(qid), ringId);
    return RT_ERR_OK;
}

int32 r9311B_cpuTagLen_get(uint32 unit, uint32* rxTag_len, uint32* txTag_len)
{
    *rxTag_len = NIC_9311B_RX_CPUTAG_LEN;
    *txTag_len = NIC_9311B_TX_CPUTAG_LEN;
    return RT_ERR_OK;
}

#define RTL9311B_NIC_SRAM_ACCESS_CTRL0_ADDR_IN_USE(x)      (RTL9311B_NIC_SRAM_ACCESS_CTRL0_ADDR + (x << 2))


int32 r9311B_sramEntry_get(uint32 unit, nic_dbg_opt_t* pOpt, nic_dbg_sram_entry_t* pEntry)
{
    uint32  val = 0;
    uint32  temp = 0;

    /*enable debug*/
    val = 0x1 << RTL9311B_NIC_SRAM_ACCESS_CTRL1_DBG_ACC_PKB_EN_OFFSET;
    ioal_mem32_write(unit, RTL9311B_NIC_SRAM_ACCESS_CTRL1_ADDR, val);


    if (NIC_DIR_RX == pOpt->dir)
    {
        ;
    }
    else
    {
        val |= (0x1 << RTL9311B_NIC_SRAM_ACCESS_CTRL1_MEM_SEL_OFFSET);
        ioal_mem32_write(unit, RTL9311B_NIC_SRAM_ACCESS_CTRL1_ADDR, val);
    }
    val |= (pOpt->ringId & 0xf) << 6;
    val |= (pOpt->entryId & 0x3f);
    ioal_mem32_write(unit, RTL9311B_NIC_SRAM_ACCESS_CTRL1_ADDR, val);

    val |= (0x1 << RTL9311B_NIC_SRAM_ACCESS_CTRL1_DBG_SRMA_TRIG_OFFSET);
    ioal_mem32_write(unit, RTL9311B_NIC_SRAM_ACCESS_CTRL1_ADDR, val);


    ioal_mem32_read(unit, RTL9311B_NIC_SRAM_ACCESS_CTRL0_ADDR_IN_USE(0), &temp);

    pEntry->wrap = temp & 0x1;  /* 1 bit */
    pEntry->len = (temp >> 1) & 0xffff; /* 16 bit */
    pEntry->more = (temp >> 17) & 0x1;   /* 1 bit*/
    pEntry->tag_if = (temp >> 18) & 0x1;   /* 1 bit*/
    pEntry->err_flag = (temp >> 19) & 0x1;   /* 1 bit*/
    pEntry->truncate = (temp >> 20) & 0x1;   /* 1 bit*/
    pEntry->magic_num = (temp >> 21) & 0xf;   /* 4 bit*/


    ioal_mem32_read(unit, RTL9311B_NIC_SRAM_ACCESS_CTRL0_ADDR_IN_USE(1), &temp);
    pEntry->addr = temp;

    ioal_mem32_read(unit, RTL9311B_NIC_SRAM_ACCESS_CTRL0_ADDR_IN_USE(2), &temp);

    pEntry->addr = (pEntry->addr << 32) | temp;


    return RT_ERR_OK;
}

