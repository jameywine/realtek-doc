/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008-2015
 * All rights reserved.
 *
 * $Revision: 80179 $
 * $Date: 2017-06-28 17:33:43 +0800 (Wed, 28 Jun 2017) $
 *
 * Purpose : Definition those public CAPWAP APIs and its data type in the SDK .
 *
 * Feature : The file have include the following module and sub-modules
 *            1) CAPWAP
 *
 */

/*
 * Include Files
 */
#include <common/rt_error.h>
#include <common/util/rt_util.h>
#include <dal/dal_mgmt.h>
#include <rtk/default.h>
#include <rtk/capwap.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */

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
int32
rtk_capwap_init(uint32 unit)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_init(unit);
}   /* end of rtk_capwap_init */

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
int32
rtk_capwap_enable_get(uint32 unit, rtk_enable_t *pEnable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_enable_get(unit, pEnable);
}   /* end of rtk_capwap_enable_get */

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
int32
rtk_capwap_enable_set(uint32 unit, rtk_enable_t enable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_enable_set(unit, enable);
}   /* end of rtk_capwap_enable_set */

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
int32
rtk_capwap_udpPort_get(uint32 unit, uint32 *pCtrl_port, uint32 *pData_port)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_udpPort_get(unit, pCtrl_port, pData_port);
}   /* end of rtk_capwap_udpPort_get */

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
int32
rtk_capwap_udpPort_set(uint32 unit, uint32 ctrl_port, uint32 data_port)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_udpPort_set(unit, ctrl_port, data_port);
}   /* end of rtk_capwap_udpPort_set */

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
int32
rtk_capwap_fwdAction_get(uint32 unit, rtk_capwap_fwdPktType_t type, rtk_action_t *pAction)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_fwdAction_get(unit, type, pAction);
}   /* end of rtk_capwap_fwdAction_get */

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
int32
rtk_capwap_fwdAction_set(uint32 unit, rtk_capwap_fwdPktType_t type, rtk_action_t action)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_fwdAction_set(unit, type, action);
}   /* end of rtk_capwap_fwdAction_set */

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
int32
rtk_capwap_trapTarget_get(uint32 unit, rtk_trapTarget_t *pTarget)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_trapTarget_get(unit, pTarget);
}   /* end of rtk_capwap_trapTarget_get */

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
int32
rtk_capwap_trapTarget_set(uint32 unit, rtk_trapTarget_t target)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_trapTarget_set(unit, target);
}   /* end of rtk_capwap_trapTarget_set */

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
int32
rtk_capwap_bssidTblHashAlgo_get(uint32 unit, uint32 *pAlgo)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_bssidTblHashAlgo_get(unit, pAlgo);
}   /* end of rtk_capwap_bssidTblHashAlgo_get */

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
int32
rtk_capwap_bssidTblHashAlgo_set(uint32 unit, uint32 algo)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_bssidTblHashAlgo_set(unit, algo);
}   /* end of rtk_capwap_bssidTblHashAlgo_set */

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
int32
rtk_capwap_qosProfile_get(uint32 unit, uint32 index, rtk_capwap_qosProfile_t *pProfile)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_qosProfile_get(unit, index, pProfile);
}   /* end of rtk_capwap_qosProfile_get */

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
int32
rtk_capwap_qosProfile_set(uint32 unit, uint32 index, rtk_capwap_qosProfile_t profile)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_qosProfile_set(unit, index, profile);
}   /* end of rtk_capwap_qosProfile_set */

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
int32
rtk_capwap_tidRemarking_get(uint32 unit, uint32 dp, rtk_pri_t int_pri, uint32 *pTid)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_tidRemarking_get(unit, dp, int_pri, pTid);
}   /* end of rtk_capwap_tidRemarking_get */

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
int32
rtk_capwap_tidRemarking_set(uint32 unit, uint32 dp, rtk_pri_t int_pri, uint32 tid)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_tidRemarking_set(unit, dp, int_pri, tid);
}   /* end of rtk_capwap_tidRemarking_set */

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
int32
rtk_capwap_bssidEntry_add(uint32 unit, rtk_capwap_bssidEntry_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_bssidEntry_add(unit, pEntry);
}   /* end of rtk_capwap_bssidEntry_add */

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
int32
rtk_capwap_bssidEntry_del(uint32 unit, rtk_capwap_bssidEntry_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_bssidEntry_del(unit, pEntry);
}   /* end of rtk_capwap_bssidEntry_del */

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
int32
rtk_capwap_bssidEntry_get(uint32 unit, rtk_capwap_bssidEntry_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_bssidEntry_get(unit, pEntry);
}   /* end of rtk_capwap_bssidEntry_get */

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
int32
rtk_capwap_bssidEntryNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_bssidEntry_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_bssidEntryNextValid_get(unit, pScan_idx, pEntry);
}   /* end of rtk_capwap_bssidEntryNextValid_get */

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
int32
rtk_capwap_ucast_add(uint32 unit, rtk_capwap_ucast_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_ucast_add(unit, pEntry);
}   /* end of rtk_capwap_ucast_add */

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
int32
rtk_capwap_ucast_del(uint32 unit, rtk_capwap_ucast_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_ucast_del(unit, pEntry);
}   /* end of rtk_capwap_ucast_del */

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
int32
rtk_capwap_ucast_get(uint32 unit, rtk_capwap_ucast_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_ucast_get(unit, pEntry);
}   /* end of rtk_capwap_ucast_get */

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
int32
rtk_capwap_ucastNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_ucast_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_ucastNextValid_get(unit, pScan_idx, pEntry);
}   /* end of rtk_capwap_ucastNextValid_get */

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
int32
rtk_capwap_mcastNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_mcast_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_mcastNextValid_get(unit, pScan_idx, pEntry);
}   /* end of rtk_capwap_mcastNextValid_get */

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
int32
rtk_capwap_mcastBssid_add(uint32 unit, rtk_capwap_mcastBssid_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_mcastBssid_add(unit, pEntry);
}   /* end of rtk_capwap_mcastBssid_add */

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
int32
rtk_capwap_mcastBssid_del(uint32 unit, rtk_capwap_mcastBssid_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_mcastBssid_del(unit, pEntry);
}   /* end of rtk_capwap_mcastBssid_del */

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
int32
rtk_capwap_mcastBssid_get(
    uint32                  unit,
    rtk_capwap_mcastBssid_t *pEntry,
    uint32                  arraySize,
    rtk_mac_t               *pBssidArray,
    uint32                  *pBssidCnt)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_mcastBssid_get(unit, pEntry, arraySize, pBssidArray, pBssidCnt);
}   /* end of rtk_capwap_mcastBssid_get */

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
int32
rtk_capwap_bssidListNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_bssidList_t *pEntry)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->capwap_bssidListNextValid_get(unit, pScan_idx, pEntry);
}   /* end of rtk_capwap_bssidListNextValid_get */

