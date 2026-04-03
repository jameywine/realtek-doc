#ifndef __RTK_BUMBLEBEE_TABLEFIELD_LIST_H__
#define __RTK_BUMBLEBEE_TABLEFIELD_LIST_H__

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#ifdef __BOOTLOADER__
#include <hal/chipdef/bumblebee/rtk_bumblebee_uboot_feature_def.h>
#else   /* __BOOTLOADER__ */
#include <hal/chipdef/bumblebee/rtk_bumblebee_feature_def.h>
#endif  /* __BOOTLOADER__ */

#if defined(CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_VLAN_MBR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_VLAN_UNTAG_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_VLAN_MBR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_RANGE_CHECK)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IVC_VLAN_RANGE_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_RANGE_CHECK */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EVA_PF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EVC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EVC_BCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EVC_BCAM_SRAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IVA_PF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IVC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IVC_BCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IVC_BCAM_SRAM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_CROSS_CONNECT)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_BCAM_L2_VXC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_VXC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SFT_L2_VXC_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_CROSS_CONNECT */
#if defined(CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EPORT_TO_LAG_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_GIP_TO_LAG_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_LAG_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_LAG_NON_UNI_FWD_PPM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_MSTI_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MSTI_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_PORT_ISO_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_BCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_CAM_MC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_CAM_UC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_LRN_LEVEL_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_MC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_UC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L2_UC_TYPE2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MC_PMSK_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SFT_MC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SFT_UC_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_EGRESS_BANDWIDTH_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EGBW_NODE_SET0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EGBW_NODE_SET1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EGBW_Q_SET0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EGBW_Q_SET1_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_EGRESS_BANDWIDTH_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_METER_MARKER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EACL_METER_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EACL_METER_PKT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_METER_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_METER_PKT_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_METER_MARKER */
#if defined(CONFIG_SDK_CHIP_FEATURE_SERVICE_METER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SERV_METER_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SERV_METER_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SERV_METER_PKT_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SERVICE_METER */
#if defined(CONFIG_SDK_CHIP_FEATURE_OVERSUBSCRIPTION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_OSB_MEM_CNTR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_OVERSUBSCRIPTION */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLOWCONTROL___BACKPRESSURE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_CPU_ABS_PG_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_IGR_PG_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_MC_ABS_PG_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_MIR_ABS_PG_CNTR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLOWCONTROL___BACKPRESSURE */
#if defined(CONFIG_SDK_CHIP_FEATURE_CONGESTION_AVOIDANCE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_E2E_Q_STAT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_EGR_Q_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_WRED_CTRL_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_WRED_DP_PROFILE_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_WRED_THD_PROFILE_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CONGESTION_AVOIDANCE */
#if defined(CONFIG_SDK_CHIP_FEATURE_INGRESS_PRIORITY_DECISION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_DSCP_MAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_PCP_DEI_MAP_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INGRESS_PRIORITY_DECISION */
#if defined(CONFIG_SDK_CHIP_FEATURE_REMARKING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_DSCP_RMK_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_PCP_DEI_RMK_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_UL_PCP_DEI_RMK_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_REMARKING */
#if defined(CONFIG_SDK_CHIP_FEATURE_TSN)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_INDV_RECOV_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SEQ_GEN_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SEQ_RECOV_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_TSN */
#if defined(CONFIG_SDK_CHIP_FEATURE_RATE_MONITOR)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_AFD_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RATE_MONITOR */
#if defined(CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EL3IF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EL3IF_NAPT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_L3_NHOP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_L3_NHOP_BIDIR_NAPT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_L3_NHOP_UNIDIR_NAPT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_EL3IF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IL3IF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IL3IF_PF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_ECMP_GRP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_ECMP_MBR_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_ECMP_MBR_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_FRR_GRP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L2IP6MC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L2IP6MC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L2IP6MC_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L2IP6MC_3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L2IPMC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L2IPMC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IP6MC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IP6MC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IP6MC_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IP6MC_3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IP6UC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IP6UC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IPMC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IPMC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_L3IPUC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L2IP6MC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L2IP6MC_0_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L2IP6MC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L2IP6MC_1_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L2IPMC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L2IPMC_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L3IP6MC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L3IP6MC_0_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L3IP6MC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L3IP6MC_1_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L3IPMC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_HOST_TCAM_L3IPMC_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_MTU_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP64_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP64_0_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP64_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP64_1_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_0_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_1_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_2_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_DIP_3_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF64_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF64_0_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF64_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF64_1_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_0_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_1_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_2_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IP6UC_URPF_3_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IPUC_DIP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IPUC_DIP_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IPUC_URPF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NET_TCAM_IPUC_URPF_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NHOP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_NHOP_BIDIR_NAPT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_PROT_GRP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_L3_RPF_GRP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MY_MAC_TCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MY_MAC_TCAM_SRAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_EREPL_DPM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_EREPL_LIST_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_TM_EREPL_LIST_1_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_IP_TNL_ENCAP_IPV4_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_IP_TNL_ENCAP_IPV6_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_IP_TNL_ENCAP_IPV6_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IP_TNL_DECAP_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IP_TNL_DECAP_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IP_TNL_DECAP_TCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IP_TNL_DECAP_TCAM_SRAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_TNL_MY_MAC_TCAM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL */
#if defined(CONFIG_SDK_CHIP_FEATURE_L3_MPLS)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_MPLS_LSP_TC_MAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_MPLS_PW_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_MPLS_PW_SEQ_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_MPLS_PW_TC_MAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_TNL_ENCAP_MPLS_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MPLS_DECAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MPLS_DECAP_TCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MPLS_DECAP_TCAM_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_MPLS_PW_SEQ_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_L3_MPLS */
#if defined(CONFIG_SDK_CHIP_FEATURE_SRV6)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_SRH_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_SRH_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_SRH_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SR6_ENCAP_SRH_3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SR6_SID_TCAM_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SR6_SID_TCAM_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SR6_SID_TCAM_SRAM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SRV6 */
#if defined(CONFIG_SDK_CHIP_FEATURE_STATISTIC_COUNTER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_STAT_PORT_CNT_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_STAT_PORT_CNT_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_STAT_PORT_E_Q_CNT_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_STATISTIC_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLOW_BASED_COUNTER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EACL_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_CNTR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLOW_BASED_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_SERVICE_COUNTER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SERV_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_SERV_MICRO_MAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SERV_AFD_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SERV_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SERV_MICRO_MAP_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SERVICE_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_DEBUG_COUNTER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_DBG_EPORT_CNT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_DBG_IPORT_CNT_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_DEBUG_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_IPFIX)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EACL_IPFIX_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_IPFIX_CNTR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IPFIX */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLOW_IDENTIFICATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_FLOW_IDENTITY_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_FLOW_IDENTITY_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_FLOW_IDENTITY_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_FLOW_IDENTITY_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_FLOW_IDENTITY_3_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLOW_IDENTIFICATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_PIE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_PIE_EBLK_KEY_SEL_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_PIE_IBLK_KEY_SEL_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_PIE_VBLK_KEY_SEL_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PIE */
#if defined(CONFIG_SDK_CHIP_FEATURE_ACL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EACL_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EACL_KEY_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_FWD_PORT_BITMAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IACL_KEY_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_VACL_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_VACL_KEY_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ACL */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP6_MAC_BIND_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP6_MAC_BIND_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP6_MAC_BIND_BCAM_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP6_MAC_BIND_BCAM_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP6_MAC_BIND_BCAM_SRAM_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP6_MAC_BIND_BCAM_SRAM_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP_MAC_BIND_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP_MAC_BIND_BCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IP_MAC_BIND_BCAM_SRAM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING */
#if defined(CONFIG_SDK_CHIP_FEATURE_CFM)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_LMEP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_LMEP_TCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_LMEP_TCAM_ACT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_LMEP_TX_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_LMEP_TX_CNT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_MA_STATE_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_RMEP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_RMEP_RX_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_RMEP_TCAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_RMEP_TCAM_ACT_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CFM */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLEXIBLE_PIPELINE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_ENTRY1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_ENTRY2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_ENTRY_CRC0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_ENTRY_CRC1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_ENTRY_CRC2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_ENTRY_CRC3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_FULL_ACT0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_FULL_ACT1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_FULL_ACT2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_FULL_ACT3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_PARTIAL_ACT0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_PARTIAL_ACT1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_PARTIAL_ACT2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_PARTIAL_ACT3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_PARTIAL_ACT4_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_SFT_FLEX_KEY_PARTIAL_ACT5_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLEXIBLE_PIPELINE */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_ETAG_PCP_DEI_RMK_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_PE_FWD_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_PE_FWD_CAM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION */
#if defined(CONFIG_SDK_CHIP_FEATURE_VP__VIRTUAL_PORT_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_EVP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_VP_SH_GRP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_EVP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IVP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_IVP_PRE_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SH_PMSK_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VP__VIRTUAL_PORT_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_BCD__BROADCAST_DOMAIN_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_BCD_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_EPP_BCD_FID_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_BCD_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_BCD_FID_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_DST_BCD_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_EGR_FLTR_PMSK_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_BCD__BROADCAST_DOMAIN_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_SHARED_FORWARDING_TABLE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_tableField_t RTL9330_IPP_SFT_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SHARED_FORWARDING_TABLE */



#endif    /* __RTK_BUMBLEBEE_TABLEFIELD_LIST_H__ */
