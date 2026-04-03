#ifndef __RTK_TIGERSHARK_TABLEFIELD_LIST_H__
#define __RTK_TIGERSHARK_TABLEFIELD_LIST_H__

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#ifdef __BOOTLOADER__
#include <hal/chipdef/tigershark/rtk_tigershark_uboot_feature_def.h>
#else   /* __BOOTLOADER__ */
#include <hal/chipdef/tigershark/rtk_tigershark_feature_def.h>
#endif  /* __BOOTLOADER__ */

#if defined(CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VLAN_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VLAN_UNTAG_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VLAN_EVC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VLAN_IP_BASED_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VLAN_IVC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VLAN_MAC_BASED_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_LAG_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_SRC_TRK_MAP_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_MSTI_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_PORT_ISO_CTRL_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_CAM_MC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_CAM_UC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_MC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_UC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_MC_PMSK_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_METER_MARKER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_METER_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_METER_MARKER */
#if defined(CONFIG_SDK_CHIP_FEATURE_REMARKING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_REMARK_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_REMARKING */
#if defined(CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_ECMP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_EGR_INTF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_EGR_INTF_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_EGR_INTF_LIST_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_EGR_TUNNEL_INTF_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_HOST_ROUTE_IP6MC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_HOST_ROUTE_IP6UC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_HOST_ROUTE_IPMC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_HOST_ROUTE_IPUC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_IGR_INTF_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_IGR_INTF_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_IGR_TUNNEL_INTF_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_NEXTHOP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6MC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6MC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6MC_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6MC_3_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6MC_4_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6MC_5_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6UC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6UC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IP6UC_2_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IPMC_0_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IPMC_1_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_PREFIX_ROUTE_IPUC_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L3_ROUTER_MAC_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_TUNNEL_START_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_TUNNEL_START_ENCAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_TUNNEL_TERMINATION_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL */
#if defined(CONFIG_SDK_CHIP_FEATURE_MIB_COUNTER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_STAT_PORT_MIB_CNTR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MIB_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLOW_BASED_COUNTER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_FLOW_CNTR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLOW_BASED_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_PRIVATE_COUNTER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_AUTO_RECOVERY_TXERR_CNT_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_STAT_PORT_PRVTE_CNTR_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_STAT_PORT_PRVTE_E_Q_CNTR_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PRIVATE_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_ACL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_EACL_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_IACL_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VACL_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ACL */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_IP_MAC_BIND_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_IP_MAC_BIND_CAM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING */
#if defined(CONFIG_SDK_CHIP_FEATURE_L2_TUNNEL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_LST_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_MCAST_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_MCAST_CAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_UCAST_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_UCAST_CAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_VXLAN_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_VXLAN_CAM_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_L2_TNL_VXLAN_ENCAP_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_VXLAN_TUNNEL_START_0_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_L2_TUNNEL */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_PE_FWD_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_PE_FWD_CAM_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION */
#if defined(CONFIG_SDK_CHIP_FEATURE_MODIFIER_HSA)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_tableField_t RTL9311B_EXTERNAL_HSA_DATA_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MODIFIER_HSA */



#endif    /* __RTK_TIGERSHARK_TABLEFIELD_LIST_H__ */
