/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008-2015
 * All rights reserved.
 *
 * $Revision: 80179 $
 * $Date: 2017-06-28 17:33:43 +0800 (Wed, 28 Jun 2017) $
 *
 * Purpose : Definition those public ACL APIs and its data type in the SDK .
 *
 * Feature : The file have include the following module and sub-modules
 *            1) CAPWAP
 *
 */

#ifndef __RTK_CAPWAP_H__
#define __RTK_CAPWAP_H__

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <rtk/vlan.h>

/*
 * Symbol Definition
 */
#define RTK_CAPWAP_WL_INFO_LEN_MAX   8
#define CAPWAP_MCAST_BSSID_ARRAY_SIZE_MAX   32

#define RTK_CAPWAP_FLAG_ENTRY_REPLACE     0x00000001/* replace the existing entry with the new info */


/*
 * Data Declaration
 */
typedef enum rtk_capwap_fwdPktType_e
{
    CAPWAP_FWDPKTTYPE_CTRL_FRAME,   /* CAPWAP control packet */
    CAPWAP_FWDPKTTYPE_INVALID_HDR,  /* invalid CAPWAP header */
    CAPWAP_FWDPKTTYPE_SA_MISS,      /* inner SA lookup miss */
    CAPWAP_FWDPKTTYPE_END,
} rtk_capwap_fwdPktType_t;

typedef enum rtk_capwap_otagPriSrc_e
{
    CAPWAP_OTAG_PRISRC_FORCE,        /* use specified priority */
    CAPWAP_OTAG_PRISRC_ORIGINAL,     /* from original outer tag */
    CAPWAP_OTAG_PRISRC_INTPRI_REMARK,/* from internal priority remarking */
    CAPWAP_OTAG_PRISRC_END,
} rtk_capwap_otagPriSrc_t;

typedef enum rtk_capwap_itagPriSrc_e
{
    CAPWAP_ITAG_PRISRC_FORCE,        /* use specified priority */
    CAPWAP_ITAG_PRISRC_ORIGINAL,     /* from original inner tag */
    CAPWAP_ITAG_PRISRC_INTPRI_REMARK,/* from internal priority remarking */
    CAPWAP_ITAG_PRISRC_END,
} rtk_capwap_itagPriSrc_t;

typedef struct rtk_capwap_qosProfile_e
{
    rtk_capwap_otagPriSrc_t otag_src;
    rtk_pri_t               opri;/* only used when CAPWAP_OTAG_PRISRC_FORCE */
    rtk_capwap_itagPriSrc_t itag_src;
    rtk_pri_t               ipri;/* only used when CAPWAP_ITAG_PRISRC_FORCE */
} rtk_capwap_qosProfile_t;

typedef enum rtk_capwap_ovidCmd_e
{
    CAPWAP_OVID_CMD_UNTAG_PRITAG,/* assign outer VID to outer untagged and outer priority tagged packet */
    CAPWAP_OVID_CMD_UNTAG,       /* assign outer VID to outer untagged packet */
    CAPWAP_OVID_CMD_ALL,         /* assign outer VID to all packet type */
    CAPWAP_OVID_CMD_END,
} rtk_capwap_ovidCmd_t;

typedef enum rtk_capwap_ividCmd_e
{
    CAPWAP_IVID_CMD_UNTAG_PRITAG,/* assign inner VID to inner untagged and outer priority tagged packet */
    CAPWAP_IVID_CMD_UNTAG,       /* assign inner VID to inner untagged packet */
    CAPWAP_IVID_CMD_ALL,         /* assign inner VID to all packet type */
    CAPWAP_IVID_CMD_END,
} rtk_capwap_ividCmd_t;

typedef struct rtk_capwap_bssidEntry_e
{
    rtk_mac_t            bssid;     /* BSSID MAC address. Search Key. */
    uint32               flags;     /* refer to RTK_CAPWAP_FLAG_XXX */

    /* fields used when tunnel termination */
    rtk_capwap_ovidCmd_t ovid_cmd;
    rtk_vlan_t           ovid;
    rtk_capwap_ividCmd_t ivid_cmd;
    rtk_vlan_t           ivid;
    rtk_vlanType_t       fwd_vlan;  /* fowarding VLAN */
    rtk_pri_t            int_pri;   /* internal priority for CAPWAP tunnel terminated packet */
    uint32               priGrp_idx;/* index to priority selection group. Refer to rtk_qos_priSelGroup_set() */

    /* fields used when tunnel start */
    rtk_intf_id_t        intf_id;   /* interface ID. Refer to rtk_tunnel_intf_create() */
    uint32               qosPro_idx;/* index to QoS profile. Refer to rtk_capwap_qosProfile_set() */
    uint32               radio_id;  /* radio identifier whose value is between 1 and 31 */
    rtk_enable_t         tid_encap; /* enable 802.11e QoS Control field encapsulation */
    uint8                wl_info[RTK_CAPWAP_WL_INFO_LEN_MAX];/* wireless specific info */

    uint32               entry_idx; /* returned physical entry index after a successful read/write */
} rtk_capwap_bssidEntry_t;

typedef struct rtk_capwap_ucast_e
{
    rtk_vlan_t  vid;  /* lookup key */
    rtk_mac_t   mac;  /* lookup key */
    uint32      flags;/* refer to RTK_CAPWAP_FLAG_XXX */

    rtk_mac_t   bssid;/* BSSID which the host binds to */
} rtk_capwap_ucast_t;

typedef struct rtk_capwap_mcast_e
{
    rtk_vlan_t      vid;     /* lookup key */
    rtk_mac_t       mac;     /* lookup key */
    uint32          flags;   /* refer to RTK_CAPWAP_FLAG_XXX */

    rtk_enable_t    wire_fwd;/* enable forwarding mcast packet to the specified portmask */
    rtk_portmask_t  portmask;
    uint32          fwdIndex;/* index of portmask table, -1 means auto allocation */
} rtk_capwap_mcast_t;

typedef struct rtk_capwap_mcastBssid_e
{
    rtk_vlan_t  vid;/* lookup key */
    rtk_mac_t   mac;/* lookup key */

    rtk_mac_t   bssid;
} rtk_capwap_mcastBssid_t;

typedef struct rtk_capwap_bssidList_e
{
    uint32  bssid_idx;
    uint32  bssid_next_idex;/* 0 means end of list */
} rtk_capwap_bssidList_t;

/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */

/* Module Name     : OpenFlow */

/* Function Name:
 *      rtk_capwap_init
 * Description:
 *      Initialize CAPWAP module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9310
 * Note:
 *      Must initialize CAPWAP module before calling any CAPWAP APIs.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_init(uint32 unit);

/* Function Name:
 *      rtk_capwap_enable_get
 * Description:
 *      Get enable state of CAPWAP module.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - pointer to enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      (1) In 9310, there are two flexible tables supported.
 *          Flexible table 0 can be used as BSSID table or MPLS decap table or IP-MAC binding table(1K~2K-1).
 *          Flexible table 1 can be used as BSSID list table or MPLS Next Hop table.
 *      (2) Enable CAPWAP to configure flexible table 0 as BSSID table and flexible table 1 as BSSID list table.
 *          Disable CAPWAP to configure flexible table 0 as IP-MAC binding table(1K~2K-1) and flexible table 1 as MPLS Next Hop table.
 * Changes:
 *      None
 */
extern int32
rtk_capwap_enable_get(uint32 unit, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_capwap_enable_set
 * Description:
 *      Set enable state of CAPWAP module.
 * Input:
 *      unit    - unit id
 *      enable  - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      (1) In 9310, there are two flexible tables supported.
 *          Flexible table 0 can be used as BSSID table or MPLS decap table or IP-MAC binding table(1K~2K-1).
 *          Flexible table 1 can be used as BSSID list table or MPLS Next Hop table.
 *      (2) Enable CAPWAP to configure flexible table 0 as BSSID table and flexible table 1 as BSSID list table.
 *          Disable CAPWAP to configure flexible table 0 as IP-MAC binding table(1K~2K-1) and flexible table 1 as MPLS Next Hop table.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_enable_set(uint32 unit, rtk_enable_t enable);

/* Function Name:
 *      rtk_capwap_udpPort_get
 * Description:
 *      Get UDP port number of CAPWAP control and data packet.
 * Input:
 *      unit	    - unit id
 * Output:
 *      pCtrl_port	- pointer to udp control port
 *      pData_port 	- pointer to udp data port
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  	- invalid unit id
 *      RT_ERR_NOT_INIT 	- The module is not initial
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_udpPort_get(uint32 unit, uint32 *pCtrl_port, uint32 *pData_port);

/* Function Name:
 *      rtk_capwap_udpPort_set
 * Description:
 *      Set UDP port number of CAPWAP control and data packet.
 * Input:
 *      unit	    - unit id
 *      ctrl_port	- udp control port
 *      data_port 	- udp data port
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_udpPort_set(uint32 unit, uint32 ctrl_port, uint32 data_port);

/* Function Name:
 *      rtk_capwap_fwdAction_get
 * Description:
 *      Get forwarding action of specific packet type.
 * Input:
 *      unit	- unit id
 *      type	- forward packet type
 * Output:
 *      pAction - pointer to forwarding action
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_fwdAction_get(uint32 unit, rtk_capwap_fwdPktType_t type, rtk_action_t *pAction);

/* Function Name:
 *      rtk_capwap_fwdAction_set
 * Description:
 *      Set forwarding action of specific packet type.
 * Input:
 *      unit	- unit id
 *      type	- forward packet type
 *      action 	- forwarding action
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_fwdAction_set(uint32 unit, rtk_capwap_fwdPktType_t type, rtk_action_t action);

/* Function Name:
 *      rtk_capwap_trapTarget_get
 * Description:
 *      Get target device for trap packet.
 * Input:
 *      unit	- unit id
 * Output:
 *      pTarget - pointer to target device
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_trapTarget_get(uint32 unit, rtk_trapTarget_t *pTarget);

/* Function Name:
 *      rtk_capwap_trapTarget_set
 * Description:
 *      Set target device for trap packet.
 * Input:
 *      unit	- unit id
 *      target 	- target device
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_trapTarget_set(uint32 unit, rtk_trapTarget_t target);

/* Function Name:
 *      rtk_capwap_bssidTblHashAlgo_get
 * Description:
 *      Get hash algorithm of BSSID table.
 * Input:
 *      unit    - unit id
 * Output:
 *      pAlgo   - pointer to hash algorithm
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      There are two hash algorithm supported in 9310.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_bssidTblHashAlgo_get(uint32 unit, uint32 *pAlgo);

/* Function Name:
 *      rtk_capwap_bssidTblHashAlgo_set
 * Description:
 *      Set hash algorithm of BSSID table.
 * Input:
 *      unit	- unit id
 *      algo    - hash algorithm
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      There are two hash algorithm supported in 9310.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_bssidTblHashAlgo_set(uint32 unit, uint32 algo);

/* Function Name:
 *      rtk_capwap_qosProfile_get
 * Description:
 *      Get QoS profile for specifying passenger's inner/outer VLAN priority for a tunnel start packet.
 * Input:
 *      unit	 - unit id
 *      index    - profile index
 * Output:
 *      pProfile - pointer to QoS profile
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_qosProfile_get(uint32 unit, uint32 index, rtk_capwap_qosProfile_t *pProfile);

/* Function Name:
 *      rtk_capwap_qosProfile_set
 * Description:
 *      Set QoS profile for specifying passenger's inner/outer VLAN priority for a tunnel start packet.
 * Input:
 *      unit	- unit id
 *      index   - profile index
 *      profile - QoS profile
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_qosProfile_set(uint32 unit, uint32 index, rtk_capwap_qosProfile_t profile);

/* Function Name:
 *      rtk_capwap_tidRemarking_get
 * Description:
 *      Get the 802.11e TID remarking.
 * Input:
 *      unit	- unit id
 *      dp      - drop precedence
 *      int_pri - internal prioirty
 * Output:
 *      pTid    - pointer to 802.11e TID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      The TID is always remarked from drop precedence and internal priority even it is a 802.11e packet originally.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_tidRemarking_get(uint32 unit, uint32 dp, rtk_pri_t int_pri, uint32 *pTid);

/* Function Name:
 *      rtk_capwap_tidRemarking_set
 * Description:
 *      Set the 802.11e TID remarking.
 * Input:
 *      unit	- unit id
 *      dp      - drop precedence
 *      int_pri - internal prioirty
 *      tid     - 802.11e TID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      The TID is always remarked from drop precedence and internal priority even it is a 802.11e packet originally.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_tidRemarking_set(uint32 unit, uint32 dp, rtk_pri_t int_pri, uint32 tid);

/* Function Name:
 *      rtk_capwap_bssidEntry_add
 * Description:
 *      Add a BSSID entry.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to BSSID entry
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 *      RT_ERR_ENTRY_EXIST	- entry already existed
 * Applicable:
 *      9310
 * Note:
 *      RT_ERR_ENTRY_EXIST is returned when the entry is existed. Specify RTK_CAPWAP_FLAG_ENTRY_REPLACE
 *      flag to overwrite an existing entry.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_bssidEntry_add(uint32 unit, rtk_capwap_bssidEntry_t *pEntry);

/* Function Name:
 *      rtk_capwap_bssidEntry_del
 * Description:
 *      Delete a BSSID entry.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to BSSID entry
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      Specify pEntry->bssid to delete the entry.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_bssidEntry_del(uint32 unit, rtk_capwap_bssidEntry_t *pEntry);

/* Function Name:
 *      rtk_capwap_bssidEntry_get
 * Description:
 *      Get a BSSID entry.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to BSSID entry
 * Output:
 *      pEntry  - pointer to BSSID entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      Specify pEntry->bssid to get the entry.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_bssidEntry_get(uint32 unit, rtk_capwap_bssidEntry_t *pEntry);

/* Function Name:
 *      rtk_capwap_bssidEntryNextValid_get
 * Description:
 *      Get a next valid BSSID entry from the specified device.
 * Input:
 *      unit	    - unit id
 *      pScan_idx   - currently scan index of BSSID table to get next.
 * Output:
 *      pEntry      - pointer to BSSID entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      (1) Input -1 for getting the first valid BSSID entry.
 *      (2) The pScan_idx is both the input and output argument.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_bssidEntryNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_bssidEntry_t *pEntry);

/* Function Name:
 *      rtk_capwap_ucast_add
 * Description:
 *      Add a unicast host to wireless client table.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to unicast entry
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 *      RT_ERR_ENTRY_EXIST	- entry already existed
 * Applicable:
 *      9310
 * Note:
 *      RT_ERR_ENTRY_EXIST is returned when the entry is existed. Specify RTK_CAPWAP_FLAG_ENTRY_REPLACE
 *      flag to overwrite an existing entry.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_ucast_add(uint32 unit, rtk_capwap_ucast_t *pEntry);

/* Function Name:
 *      rtk_capwap_ucast_del
 * Description:
 *      Delete a unicast host from wireless client table.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to unicast entry
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_ucast_del(uint32 unit, rtk_capwap_ucast_t *pEntry);

/* Function Name:
 *      rtk_capwap_ucast_get
 * Description:
 *      Get a unicast host from wireless client table.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to unicast entry
 * Output:
 *      pEntry  - pointer to unicast entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_ucast_get(uint32 unit, rtk_capwap_ucast_t *pEntry);

/* Function Name:
 *      rtk_capwap_ucastNextValid_get
 * Description:
 *      Get a next valid unicast host from wireless client table.
 * Input:
 *      unit	    - unit id
 *      pScan_idx   - currently scan index of wireless client table to get next.
 * Output:
 *      pEntry      - pointer to unicast entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_ucastNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_ucast_t *pEntry);

/* Function Name:
 *      rtk_capwap_mcastNextValid_get
 * Description:
 *      Get a next valid multicast entry from wireless client table.
 * Input:
 *      unit	    - unit id
 *      pScan_idx   - currently scan index of wireless client table to get next.
 * Output:
 *      pEntry      - pointer to unicast entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_mcastNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_mcast_t *pEntry);

/* Function Name:
 *      rtk_capwap_mcastBssid_add
 * Description:
 *      Add a BSSID to the specified multicast entry.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to multicast BSSID entry
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 *      RT_ERR_ENTRY_EXIST	- entry already existed
 * Applicable:
 *      9310
 * Note:
 *      After the corresponding multicast entry(rtk_capwap_mcast_add) and BSSID entry(rtk_capwap_bssidEntry_add) are created,
 *      use the API to add BSSID to the multicast entry.
 *      [FIXME] BSSID list valid bit could be maintained by SW shadow.
 *      [FIXME] BSSID list should be allocated by a centerialized _DAL API because several API may access it concurrently.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_mcastBssid_add(uint32 unit, rtk_capwap_mcastBssid_t *pEntry);

/* Function Name:
 *      rtk_capwap_mcastBssid_del
 * Description:
 *      Delete a BSSID from the specified multicast entry.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to multicast BSSID entry
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      [FIXME] BSSID list should be allocated by a centerialized _DAL API because several API may access it concurrently.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_mcastBssid_del(uint32 unit, rtk_capwap_mcastBssid_t *pEntry);

/* Function Name:
 *      rtk_capwap_mcastBssid_get
 * Description:
 *      Get all BSSIDs of the specified multicast entry.
 * Input:
 *      unit	    - unit id
 *      pEntry      - pointer to multicast BSSID entry
 *      arraySize   - size of allocated entries in pBssidArray
 * Output:
 *      pBssidArray - array of BSSIDs
 *      pBssidCnt   - number of entries actually filled in.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER	- input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      pEntry->bssid is not used.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_mcastBssid_get(
    uint32                  unit,
    rtk_capwap_mcastBssid_t *pEntry,
    uint32                  arraySize,
    rtk_mac_t               *pBssidArray,
    uint32                  *pBssidCnt);

/* Function Name:
 *      rtk_capwap_bssidListNextValid_get
 * Description:
 *      Get a next valid BSSID list entry from BSSID list table.
 * Input:
 *      unit	    - unit id
 *      pScan_idx   - currently scan index of BSSID list table to get next.
 * Output:
 *      pEntry      - pointer to BSSID list entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_capwap_bssidListNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_bssidList_t *pEntry);

#endif /* __RTK_CAPWAP_H__ */

