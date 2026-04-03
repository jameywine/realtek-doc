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
 * $Revision$
 * $Date$
 *
 * Purpose : Definition the basic types in the SDK.
 *
 * Feature : type definition
 *
 */

#ifndef __COMMON_RT_TYPE_H__
#define __COMMON_RT_TYPE_H__

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/type.h>
#include <common/util/rt_bitop.h>


/*
 * Symbol Definition
 */

typedef enum rtk_chip_e
{
    RTK_CHIP_MAC=0,
    RTK_CHIP_PHY,
    RTK_CHIP_END
}rtk_chip_t;

typedef enum rtk_chip_cpu_type_s
{
    RTK_CHIP_CPU_MIPS,
    RTK_CHIP_CPU_ARM64,
    RTK_CHIP_CPU_END
} rtk_chip_cpu_type_t;


/* for system */
#define RTK_MAX_NUM_OF_UNIT_LOCAL                   6                               /*  the maximum number of units in one CPU system (usually in a hardware board) */
#define RTK_MAX_NUM_OF_UNIT                         16                              /*  the maximum number of units in whole stacking system */
#define RTK_MAX_NUM_OF_DEV                         16                                /*  the maximum number of devices in whole stacking system */
#define RTK_MAX_UNIT_ID                             (RTK_MAX_NUM_OF_UNIT - 1)
#define RTK_LOADER_UNIT_ID                          0
#define RTK_SOC_UNIT_ID                             0
#define RTK_MAX_NUM_OF_PCIE_EP                      3       /* maximum number of connected PCI-E EP */
#define RTK_MAX_NUM_OF_PCI_MSI                      31      /* maximum number of PCI Message Signaled Interrupts*/
#define RTK_MAX_NUM_OF_SW_PORT                      96      /* maximum number of switch port */
#define RTK_MAX_NUM_OF_OOB_PORT                     4       /* maximum number of OOB port */

#define RTK_MAX_PORT_PER_UNIT                       (RTK_MAX_NUM_OF_SW_PORT+RTK_MAX_NUM_OF_OOB_PORT)    /* max number of ports supported by a chip */
#define RTK_MAX_NUM_OF_PORT                         (RTK_MAX_PORT_PER_UNIT * RTK_MAX_NUM_OF_UNIT) /* max number of ports supported by whole stacking system */
#define RTK_MAX_PORT_ID_PER_UNIT                    (RTK_MAX_PORT_PER_UNIT - 1)
#define RTK_MAX_NUM_OF_PORTS                        RTK_MAX_PORT_PER_UNIT           /* this one will be replaced by RTK_MAX_PORT_PER_UNIT later, once we have time free */
#define RTK_MAX_NUM_OF_SW_SDS                       24                              /* max number of switch's SERDES supported by a chip */
#define RTK_MAX_NUM_OF_OOB_SDS                      4                               /* max number of oob's SERDES supported by a chip */
#define RTK_MAX_SDS_PER_UNIT                        (RTK_MAX_NUM_OF_SW_SDS+RTK_MAX_NUM_OF_OOB_SDS)      /* max number of SERDES supported by a chip */
#define RTK_MAX_SDS_PER_PORT                        4                               /* maximum number of SerDes of a port(mac ID).  */
#define RTK_MAX_SC_PER_UNIT                         8                               /* max number of SerDes Converter connected by a chip */
#define RTK_MAX_PHY_PER_UNIT                        16                               /* max number of PHY connected by a chip */
#define RTK_MAX_LED_PER_PORT                        5                               /* max number of led connected by a port */
#define RTK_MAX_LED_MOD                             5                               /* max number of led mode */
#define RTK_MAX_SDS_PER_PHY                         4                               /* max SerDes number of a PHY */
#define RTK_MAX_PORT_PER_PHY                        8                               /* max port number of a PHY */
#define RTK_MAX_PHY_INDEX                           (RTK_MAX_NUM_OF_UNIT * RTK_MAX_PHY_PER_UNIT)

#define RTK_MAX_TBL_SET                             16                              /* max number of table access set */

#define RTK_MAX_NUM_OF_SW_SMI                       5
#define RTK_MAX_NUM_OF_OOB_SMI                      1
#define RTK_MAX_SMI_PER_UNIT                        (RTK_MAX_NUM_OF_SW_SMI+RTK_MAX_NUM_OF_OOB_SMI)


#define RTK_OOB_PORT_START                          (RTK_MAX_NUM_OF_SW_PORT)    /* starting port number of OOB port */
#define RTK_OOB_PORT_END                            (RTK_OOB_PORT_START+RTK_MAX_NUM_OF_OOB_PORT-1)
#define RTK_OOB_SDS_START                           (RTK_MAX_NUM_OF_SW_SDS)
#define RTK_OOB_SDS_END                             (RTK_OOB_SDS_START+RTK_MAX_NUM_OF_OOB_SDS-1)
#define RTK_OOB_SMI_START                           (RTK_MAX_NUM_OF_SW_SMI)
#define RTK_OOB_SMI_END                             (RTK_OOB_SMI_START+RTK_MAX_NUM_OF_OOB_SMI-1)
#define RTK_OOB_PORT_TO_IDX(p)                      (p-RTK_OOB_PORT_START)
#define RTK_OOB_SDS_TO_IDX(s)                       (s-RTK_OOB_SDS_START)
#define RTK_OOB_SMI_TO_IDX(s)                       (s-RTK_OOB_SMI_START)
#define RTK_OOB_IDX_TO_PORT(idx)                    (idx+RTK_OOB_PORT_START)
#define RTK_OOB_IDX_TO_SDS(idx)                     (idx+RTK_OOB_SDS_START)
#define RTK_OOB_IDX_TO_SMI(idx)                     (idx+RTK_OOB_SMI_START)

#define RTK_MAX_MACSEC_SA_PER_PORT                  64                              /* max number of Secure Association by a port*/
#define RTK_MAX_MACSEC_SC_PER_PORT                  RTK_MAX_MACSEC_SA_PER_PORT/4    /* max number of Secure Channel by a port (4 AN per SC) */

#define RTK_TOTAL_NUM_OF_BYTE_FOR_1BIT_PORT_LIST    ((RTK_MAX_PORT_PER_UNIT+7)/8)
#define RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST    BITMAP_ARRAY_CNT(RTK_MAX_PORT_PER_UNIT)
#define RTK_MAX_TRUNK_PER_UNIT                      128                              /* max number of trunk by a chip */
#define RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_TRUNK_LIST   BITMAP_ARRAY_CNT(RTK_MAX_TRUNK_PER_UNIT)

#define RTK_TRAP_MODULE_LIST                        32
#define RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_TRAP_LIST    BITMAP_ARRAY_CNT(RTK_TRAP_MODULE_LIST)


/* for modules */
#define RTK_MAX_NUM_OF_VLAN_GROUPS                  16

#define RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_VLAN_GROUP   BITMAP_ARRAY_CNT(RTK_MAX_NUM_OF_VLAN_GROUPS)
#define RTK_MAX_NUM_OF_MIRRORING_PORT               1
#define RTK_MAX_NUM_OF_TRUNK_HASH_VAL               32
#define RTK_MAX_NUM_OF_VLAN_ID                      4096
#define RTK_VLAN_ID_MIN                             0
#define RTK_VLAN_ID_MAX                             4095
#define RTK_FID_MIN                                 0
#define RTK_FID_MAX                                 4095
#define RTK_ETHERTYPE_MAX                           0xFFFF
#if defined(CONFIG_SDK_RTL9310) ||defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9311B)
#define RTK_MAX_NUM_OF_QUEUE                        32
#define RTK_IP_MTU_MAX                              0xFFFF
#define RTK_IP_TTL_MAX                              255
#else
#define RTK_MAX_NUM_OF_QUEUE                        8
#endif
#define RTK_MAX_NUM_OF_PRIORITY                     8
#define RTK_VALUE_OF_DEI_MAX                        1
#define RTK_VALUE_OF_DEI_MIN                        0
#define RTK_VALUE_OF_DSCP_MAX                       63
#define RTK_VALUE_OF_DSCP_MIN                       0
#define RTK_VALUE_OF_IP_PRECEDENCE_MAX              7
#define RTK_VALUE_OF_TOS_MAX                        0xFF
#define RTK_DOT1P_PRIORITY_MAX                      7
#define RTK_DOT1P_DEI_MAX                           1
#define RTK_DOT1P_CFI_MAX                           1
#define RTK_DROP_PRECEDENCE_MAX                     2
#define RTK_LINKMON_SCAN_INTERVAL_MIN               10000
#define RTK_LOSMON_SCAN_INTERVAL_MIN                1000000
#define RTK_WA_SCAN_INTERVAL_MIN                    500000
#define RTK_MACMON_SCAN_INTERVAL_MIN                1000000
#define RTK_EEE_TX_SLEEP_RATE_FE_MAX                3125
#define RTK_EEE_TX_SLEEP_RATE_GE_MAX                31250
#define RTK_TPID_MAX                                0xFFFF
#define RTK_MAX_NUM_OF_SERDES                       14
#define RTK_MAX_SERDES_ID                           (RTK_MAX_NUM_OF_SERDES - 1)
#define RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_SERDES_LIST  BITMAP_ARRAY_CNT(RTK_MAX_NUM_OF_SERDES)
#define RTK_PORT_SPEC_CONGEST_TIME_MAX              0xF
#define RTK_MAX_NUM_OF_LINKMON_CB                   4
#define RTK_MAX_MPLS_LABEL                          0xFFFFF

#define RTK_MAX_NUM_OF_PIE_BLOCKS                   32
#define RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PIE_BLOCK    BITMAP_ARRAY_CNT(RTK_MAX_NUM_OF_PIE_BLOCKS)

#define RTK_EYE_MON_X_MAX                           64
#define RTK_EYE_MON_Y_MAX                           64
#define RTK_EYE_MON_ARXIS_X_MAX                     (RTK_EYE_MON_X_MAX  >> 1)
#define RTK_EYE_MON_ARXIS_Y_MAX                     (RTK_EYE_MON_Y_MAX * 2)
#define RTK_EYE_MON_FRAME_MAX                       16
#define RTK_EYE_MON_DATA_POS(_f, _x, _y)            ((RTK_EYE_MON_X_MAX * RTK_EYE_MON_Y_MAX * _f) + (RTK_EYE_MON_Y_MAX *_x) + _y)
#define RTK_EYE_MON_YAXIS_CHK_OFST                  10


#define CODE_TBC                                    0

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || (defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && !defined(__KERNEL__)) /* kernel mode in kernel; user mode in user */
    #define RTK_INTR_EVENT_PROC             1
#else
    #undef  RTK_INTR_EVENT_PROC
#endif

/*
 * Data Type Declaration
 */
typedef uint32  rtk_vlan_t;         /* vlan id type                     */
typedef uint32  rtk_fid_t;          /* filter id type                   */
typedef uint32  rtk_stg_t;          /* spanning tree instance id type   */
typedef uint32  rtk_port_t;         /* port id type                     */
typedef uint32  rtk_trunk_t;        /* trunk id type                    */
typedef uint32  rtk_intf_id_t;      /* interface id type                */
typedef uint32  rtk_vrf_id_t;       /* VRF id type                      */
typedef uint32  rtk_pri_t;          /* priority vlaue                   */
typedef uint32  rtk_qid_t;          /* queue id type                    */
typedef uint32  rtk_filter_id_t;    /* filter id type                   */
typedef uint32  rtk_pie_id_t;       /* PIE id type                      */
typedef uint32  rtk_acl_id_t;       /* ACL id type                      */
typedef uint32  rtk_trk_t;          /* trunk id type                    */
typedef uint32  rtk_stk_trk_t;      /* stacking port trunk id type      */
typedef uint32  rtk_bpe_ecid_t;     /* e-channel id of port extension   */

typedef enum rtk_port_media_e
{
    PORT_MEDIA_COPPER = 0,
    PORT_MEDIA_FIBER,
    PORT_MEDIA_END
} rtk_port_media_t;

/* This enum is used for some specified PHY that have supported 1000BASE-X
 * and 100BASE-FX auto detection. Ex: RTL8212B/RTL8214B//RTL8214FB
 */
typedef enum rtk_port_fiber_media_e
{
    PORT_FIBER_MEDIA_1000 = 0,
    PORT_FIBER_MEDIA_100,
    PORT_FIBER_MEDIA_AUTO,
    PORT_FIBER_MEDIA_END
} rtk_port_fiber_media_t;

/* rtk_portmask_t is a per-unit data structure. That is, a port-bit-map of a unit */
typedef struct rtk_portmask_s
{
    rtk_bitmap_t    bits[RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST];
} rtk_portmask_t;

typedef struct rtk_trkmask_s
{
    rtk_bitmap_t    bits[RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_TRUNK_LIST];
} rtk_trkmask_t;

typedef struct rtk_vlan_groupMask_s
{
    rtk_bitmap_t  bits[RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_VLAN_GROUP];
} rtk_vlan_groupMask_t;

typedef struct rtk_pie_blockmask_s
{
    rtk_bitmap_t    bits[RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PIE_BLOCK];
} rtk_pie_blockmask_t;

typedef struct rt_portType_info_s
{
    uint32  portNum;
    rtk_portmask_t  portmask;
} rt_portType_info_t;


/* frame type of protocol vlan - reference 802.1v standard */
typedef enum rtk_vlan_protoVlan_frameType_e
{
    FRAME_TYPE_UNKNOWN = 0,
    FRAME_TYPE_ETHERNET,
    FRAME_TYPE_RFC1042,
    FRAME_TYPE_SNAP8021H,/* Not supported by any chip */
    FRAME_TYPE_SNAPOTHER,/* Not supported by any chip */
    FRAME_TYPE_LLCOTHER,
    FRAME_TYPE_END
} rtk_vlan_protoVlan_frameType_t;

/* Protocol-and-port-based Vlan structure */
typedef struct rtk_vlan_protoAndPortInfo_s
{
    uint32                          proto_type;
    rtk_vlan_protoVlan_frameType_t  frame_type;
    rtk_vlan_t                      cvid;
    rtk_pri_t                       cpri;
}rtk_vlan_protoAndPortInfo_t;


#define     IPV6_ADDR_LEN   16
typedef uint32  rtk_ip_addr_t;
typedef struct
{
    uint8   octet[IPV6_ADDR_LEN];
} rtk_ipv6_addr_t;

typedef enum rtk_l4Proto_e
{
    L4PROTO_TCP = 0,
    L4PROTO_UDP,
    L4PROTO_END
} rtk_l4Proto_t;

typedef enum rtk_action_e
{
    ACTION_FORWARD = 0,
    ACTION_DROP,
    ACTION_TRAP2CPU,
    ACTION_COPY2CPU,
    ACTION_TRAP2MASTERCPU,
    ACTION_COPY2MASTERCPU,
    ACTION_FLOOD_IN_VLAN,
    ACTION_LOOPBACK,
    ACTION_LINK_FAULT_DETECT,
    ACTION_END
} rtk_action_t;

typedef enum rtk_pktType_e
{
    ETHERNET_II_PACKET = 0,
    SNAP_PACKET,
    PKT_TYPE_END
} rtk_pktType_t;

typedef enum rtk_vlanType_e
{
    INNER_VLAN = 0,
    OUTER_VLAN,
    VLAN_TYPE_END
} rtk_vlanType_t;

typedef enum rtk_snapMode_e
{
    SNAP_MODE_AAAA03000000 = 0,         /* compare 0xAAAA03000000 */
    SNAP_MODE_AAAA03,                   /* compare 0xAAAA03 */
    SNAP_MODE_END
} rtk_snapMode_t;

typedef enum rtk_pktFormat_e
{
    ORIGINAL_PACKET = 0,
    MODIFIED_PACKET,
    PKT_FORMAT_NOP,
    PKT_FORMAT_END
} rtk_pktFormat_t;

/* Reference Time */
typedef struct rtk_time_timeStamp_s
{
    uint64 sec;
    uint32 nsec;
} rtk_time_timeStamp_t;

/* Message Type */
typedef enum rtk_time_ptpMsgType_e
{
    PTP_MSG_TYPE_SYNC = 0,
    PTP_MSG_TYPE_DELAY_REQ = 1,
    PTP_MSG_TYPE_PDELAY_REQ = 2,
    PTP_MSG_TYPE_PDELAY_RESP = 3,
} rtk_time_ptpMsgType_t;

/* trap packet target */
typedef enum rtk_trapTarget_e
{
    RTK_TRAP_LOCAL,
    RTK_TRAP_MASTER,
    RTK_TRAP_END,
} rtk_trapTarget_t;

/* TIME packet identifier */
typedef struct rtk_time_ptpIdentifier_s
{
    rtk_time_ptpMsgType_t   msgType;
    uint32                  sequenceId;
} rtk_time_ptpIdentifier_t;

typedef enum rtk_time_oper_e
{
    TIME_OPER_START = 0,
    TIME_OPER_STOP,
    TIME_OPER_LATCH,
    TIME_OPER_CMD_EXEC,
    TIME_OPER_FREQ_APPLY,
    TIME_OPER_END
} rtk_time_oper_t;

typedef struct rtk_time_operCfg_s
{
    rtk_time_oper_t oper;
    rtk_enable_t rise_tri;
    rtk_enable_t fall_tri;
} rtk_time_operCfg_t;

typedef struct rtk_dev_port_s {
    uint32       devID;
    rtk_port_t  port;
} rtk_dev_port_t;

typedef struct rtk_text_s
{
    char text[64];
}rtk_text_t;

typedef struct {
    unsigned int    reg;
    unsigned int    val;
} confcode_mac_regval_t;

typedef struct rtk_regFieldId_s
{
    uint32          reg;
    uint32          field;
} rtk_regFieldId_t;

typedef enum drv_smi_mdxProtoSel_e
{
    DRV_SMI_MDX_PROTO_C22 = 0,
    DRV_SMI_MDX_PROTO_C45,
    DRV_SMI_MDX_PROTO_END,
}drv_smi_mdxProtoSel_t;

typedef struct rtk_sds_eyeParam_s
{
    uint32 impedance;
    uint32 pre_amp;
    uint32 main_amp;
    uint32 post_amp;
    rtk_enable_t   pre_en;
    rtk_enable_t   post_en;
    rtk_enable_t   main_en;
}rtk_sds_eyeParam_t;

typedef struct rtk_sds_rxCaliConf_s {
    #if defined(CONFIG_SDK_RTL9300)
    uint32      dacLongCableOffset;
    uint16      tap0_init_val;
    uint16      vth_min;
    uint16      eqHoldEnable;
    #endif  /* CONFIG_SDK_RTL9300 */

    uint16      dfeTap1_4Enable;

    #if defined(CONFIG_SDK_RTL9310) || defined(CONFIG_SDK_RTL9311B)
    uint32      ofst;
    uint8       dfeAuto;
    uint8       leqAuto;
    #endif  /* CONFIG_SDK_RTL9310 */
} rtk_sds_rxCaliConf_t;

typedef struct rt_eye_monBmap_s {
    rtk_bitmap_t        arr[BITMAP_ARRAY_CNT(RTK_EYE_MON_ARXIS_Y_MAX)];
} rtk_eye_monBmap_t;


typedef union rtk_attachIdentifier_s
{
    struct
    {
        uint32      unit;               /* new unit ID */
        uint32      local_unit_index;   /* unit ID(index) of this chip on this hwp */
        uint32      if_local;           /* TRUE: local hardware board, FALSE: remote stacking board */
    } hwp;

    struct
    {
        uint32      unit;               /* local unit ID(index) */
    } mac;

    struct
    {
        uint32      unit;               /* local unit ID(index) */
        uint8       baseMacId;
    } phy;
} rtk_attachIdentifier_t;


#endif /* __COMMON_RT_TYPE_H__ */

