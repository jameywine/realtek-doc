/*
 * Copyright (C) 2015 Realtek Semiconductor Corp.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Revision: 80179 $
 * $Date: 2017-06-28 17:33:43 +0800 (Wed, 28 Jun 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) CAPWAP
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rtusr_util.h>
#include <rtdrv/rtdrv_netfilter.h>

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
 * Note:
 *      Must initialize CAPWAP module before calling any CAPWAP APIs.
 */
int32
rtk_capwap_init(uint32 unit)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    SETSOCKOPT(RTDRV_CAPWAP_INIT, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_capwap_init */

/* Function Name:
 *      rtk_capwapEnable_get
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
 * Note:
 *      (1) In 9310, there are two flexible tables supported.
 *          Flexible table 0 can be used as BSSID table or MPLS decap table or IP-MAC binding table(1K~2K-1).
 *          Flexible table 1 can be used as BSSID list table or MPLS Next Hop table.
 *      (2) Enable CAPWAP to configure flexible table 0 as BSSID table and flexible table 1 as BSSID list table.
 *          Disable CAPWAP to configure flexible table 0 as IP-MAC binding table(1K~2K-1) and flexible table 1 as MPLS Next Hop table.
 */
int32
rtk_capwap_enable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_ENABLE_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
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
 * Note:
 *      (1) In 9310, there are two flexible tables supported.
 *          Flexible table 0 can be used as BSSID table or MPLS decap table or IP-MAC binding table(1K~2K-1).
 *          Flexible table 1 can be used as BSSID list table or MPLS Next Hop table.
 *      (2) Enable CAPWAP to configure flexible table 0 as BSSID table and flexible table 1 as BSSID list table.
 *          Disable CAPWAP to configure flexible table 0 as IP-MAC binding table(1K~2K-1) and flexible table 1 as MPLS Next Hop table.
 */
int32
rtk_capwap_enable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_CAPWAP_ENABLE_SET, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_udpPort_get(uint32 unit, uint32 *pCtrl_port, uint32 *pData_port)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pCtrl_port), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pData_port), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_UDPPORT_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pCtrl_port, &cfg.ctrl_port, sizeof(uint32));
    osal_memcpy(pData_port, &cfg.data_port, sizeof(uint32));

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_udpPort_set(uint32 unit, uint32 ctrl_port, uint32 data_port)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ctrl_port, &ctrl_port, sizeof(uint32));
    osal_memcpy(&cfg.data_port, &data_port, sizeof(uint32));
    SETSOCKOPT(RTDRV_CAPWAP_UDPPORT_SET, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_fwdAction_get(uint32 unit, rtk_capwap_fwdPktType_t type, rtk_action_t *pAction)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pAction), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_capwap_fwdPktType_t));
    GETSOCKOPT(RTDRV_CAPWAP_FWDACTION_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pAction, &cfg.action, sizeof(rtk_action_t));

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_fwdAction_set(uint32 unit, rtk_capwap_fwdPktType_t type, rtk_action_t action)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_capwap_fwdPktType_t));
    osal_memcpy(&cfg.action, &action, sizeof(rtk_action_t));
    SETSOCKOPT(RTDRV_CAPWAP_FWDACTION_SET, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_trapTarget_get(uint32 unit, rtk_trapTarget_t *pTarget)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pTarget), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_TRAPTARGET_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pTarget, &cfg.target, sizeof(rtk_trapTarget_t));

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_trapTarget_set(uint32 unit, rtk_trapTarget_t target)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.target, &target, sizeof(rtk_trapTarget_t));
    SETSOCKOPT(RTDRV_CAPWAP_TRAPTARGET_SET, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      There are two hash algorithm supported in 9310.
 */
int32
rtk_capwap_bssidTblHashAlgo_get(uint32 unit, uint32 *pAlgo)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pAlgo), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_BSSIDTBLHASHALGO_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pAlgo, &cfg.algo, sizeof(uint32));

    return RT_ERR_OK;
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
 * Note:
 *      There are two hash algorithm supported in 9310.
 */
int32
rtk_capwap_bssidTblHashAlgo_set(uint32 unit, uint32 algo)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.algo, &algo, sizeof(uint32));
    SETSOCKOPT(RTDRV_CAPWAP_BSSIDTBLHASHALGO_SET, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_qosProfile_get(uint32 unit, uint32 index, rtk_capwap_qosProfile_t *pProfile)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pProfile), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.index, &index, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_QOSPROFILE_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pProfile, &cfg.profile, sizeof(rtk_capwap_qosProfile_t));

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_qosProfile_set(uint32 unit, uint32 index, rtk_capwap_qosProfile_t profile)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.index, &index, sizeof(uint32));
    osal_memcpy(&cfg.profile, &profile, sizeof(rtk_capwap_qosProfile_t));
    SETSOCKOPT(RTDRV_CAPWAP_QOSPROFILE_SET, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      The TID is always remarked from drop precedence and internal priority even it is a 802.11e packet originally.
 */
int32
rtk_capwap_tidRemarking_get(uint32 unit, uint32 dp, rtk_pri_t int_pri, uint32 *pTid)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pTid), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.dp, &dp, sizeof(uint32));
    osal_memcpy(&cfg.int_pri, &int_pri, sizeof(rtk_pri_t));
    GETSOCKOPT(RTDRV_CAPWAP_TIDREMARKING_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pTid, &cfg.tid, sizeof(uint32));

    return RT_ERR_OK;
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
 * Note:
 *      The TID is always remarked from drop precedence and internal priority even it is a 802.11e packet originally.
 */
int32
rtk_capwap_tidRemarking_set(uint32 unit, uint32 dp, rtk_pri_t int_pri, uint32 tid)
{
    rtdrv_capwapCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.dp, &dp, sizeof(uint32));
    osal_memcpy(&cfg.int_pri, &int_pri, sizeof(rtk_pri_t));
    osal_memcpy(&cfg.tid, &tid, sizeof(uint32));
    SETSOCKOPT(RTDRV_CAPWAP_TIDREMARKING_SET, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      RT_ERR_ENTRY_EXIST is returned when the entry is existed. Specify RTK_CAPWAP_FLAG_ENTRY_REPLACE
 *      flag to overwrite an existing entry.
 */
int32
rtk_capwap_bssidEntry_add(uint32 unit, rtk_capwap_bssidEntry_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.bssid_entry, pEntry, sizeof(rtk_capwap_bssidEntry_t));
    SETSOCKOPT(RTDRV_CAPWAP_BSSIDENTRY_ADD, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      Specify pEntry->bssid to delete the entry.
 */
int32
rtk_capwap_bssidEntry_del(uint32 unit, rtk_capwap_bssidEntry_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.bssid_entry, pEntry, sizeof(rtk_capwap_bssidEntry_t));
    SETSOCKOPT(RTDRV_CAPWAP_BSSIDENTRY_DEL, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      Specify pEntry->bssid to get the entry.
 */
int32
rtk_capwap_bssidEntry_get(uint32 unit, rtk_capwap_bssidEntry_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_BSSIDENTRY_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pEntry, &cfg.bssid_entry, sizeof(rtk_capwap_bssidEntry_t));

    return RT_ERR_OK;
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
 * Note:
 *      (1) Input -1 for getting the first valid BSSID entry.
 *      (2) The pScan_idx is both the input and output argument.
 */
int32
rtk_capwap_bssidEntryNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_bssidEntry_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pScan_idx), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_BSSIDENTRYNEXTVALID_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pScan_idx, &cfg.scan_idx, sizeof(int32));
    osal_memcpy(pEntry, &cfg.bssid_entry, sizeof(rtk_capwap_bssidEntry_t));

    return RT_ERR_OK;
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
 * Note:
 *      RT_ERR_ENTRY_EXIST is returned when the entry is existed. Specify RTK_CAPWAP_FLAG_ENTRY_REPLACE
 *      flag to overwrite an existing entry.
 */
int32
rtk_capwap_ucast_add(uint32 unit, rtk_capwap_ucast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ucast_entry, pEntry, sizeof(rtk_capwap_ucast_t));
    SETSOCKOPT(RTDRV_CAPWAP_UCAST_ADD, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_ucast_del(uint32 unit, rtk_capwap_ucast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ucast_entry, pEntry, sizeof(rtk_capwap_ucast_t));
    SETSOCKOPT(RTDRV_CAPWAP_UCAST_DEL, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_ucast_get(uint32 unit, rtk_capwap_ucast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_UCAST_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pEntry, &cfg.ucast_entry, sizeof(rtk_capwap_ucast_t));

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_ucastNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_ucast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pScan_idx), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_UCASTNEXTVALID_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pScan_idx, &cfg.scan_idx, sizeof(int32));
    osal_memcpy(pEntry, &cfg.ucast_entry, sizeof(rtk_capwap_ucast_t));

    return RT_ERR_OK;
}   /* end of rtk_capwap_ucastNextValid_get */

/* Function Name:
 *      rtk_capwap_mcast_add
 * Description:
 *      Add a multicast entry to wireless client table.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to multicast entry
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
 * Note:
 *      RT_ERR_ENTRY_EXIST is returned when the entry is existed. Specify RTK_CAPWAP_FLAG_ENTRY_REPLACE
 *      flag to overwrite an existing entry.
 */
int32
rtk_capwap_mcast_add(uint32 unit, rtk_capwap_mcast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.mcast_entry, pEntry, sizeof(rtk_capwap_mcast_t));
    SETSOCKOPT(RTDRV_CAPWAP_MCAST_ADD, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_capwap_mcast_add */

/* Function Name:
 *      rtk_capwap_mcast_del
 * Description:
 *      Delete a multicast entry from wireless client table.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to multicast entry
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
 * Note:
 *      None
 */
int32
rtk_capwap_mcast_del(uint32 unit, rtk_capwap_mcast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.mcast_entry, pEntry, sizeof(rtk_capwap_mcast_t));
    SETSOCKOPT(RTDRV_CAPWAP_MCAST_DEL, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_capwap_mcast_del */

/* Function Name:
 *      rtk_capwap_mcast_get
 * Description:
 *      Get a multicast entry from wireless client table.
 * Input:
 *      unit	- unit id
 *      pEntry  - pointer to multicast entry
 * Output:
 *      pEntry  - pointer to multicast entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - invalid unit id
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_INPUT            - invalid input parameter
 *      RT_ERR_NULL_POINTER	    - input parameter may be null pointer
 *      RT_ERR_ENTRY_NOT_EXIST	- entry is not existed
 * Note:
 *      None
 */
int32
rtk_capwap_mcast_get(uint32 unit, rtk_capwap_mcast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_MCAST_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pEntry, &cfg.mcast_entry, sizeof(rtk_capwap_mcast_t));

    return RT_ERR_OK;
}   /* end of rtk_capwap_mcast_get */

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
 * Note:
 *      None
 */
int32
rtk_capwap_mcastNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_mcast_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pScan_idx), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_MCASTNEXTVALID_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pScan_idx, &cfg.scan_idx, sizeof(int32));
    osal_memcpy(pEntry, &cfg.mcast_entry, sizeof(rtk_capwap_mcast_t));

    return RT_ERR_OK;
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
 * Note:
 *      After the corresponding multicast entry(rtk_capwap_mcast_add) and BSSID entry(rtk_capwap_bssidEntry_add) are created,
 *      use the API to add BSSID to the multicast entry.
 *      [FIXME] BSSID list valid bit could be maintained by SW shadow.
 *      [FIXME] BSSID list should be allocated by a centerialized _DAL API because several API may access it concurrently.
 */
int32
rtk_capwap_mcastBssid_add(uint32 unit, rtk_capwap_mcastBssid_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.mcastBssid_entry, pEntry, sizeof(rtk_capwap_mcastBssid_t));
    SETSOCKOPT(RTDRV_CAPWAP_MCASTBSSID_ADD, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      [FIXME] BSSID list should be allocated by a centerialized _DAL API because several API may access it concurrently.
 */
int32
rtk_capwap_mcastBssid_del(uint32 unit, rtk_capwap_mcastBssid_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.mcastBssid_entry, pEntry, sizeof(rtk_capwap_mcastBssid_t));
    SETSOCKOPT(RTDRV_CAPWAP_MCASTBSSID_DEL, &cfg, rtdrv_capwapCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      pEntry->bssid is not used.
 */
int32
rtk_capwap_mcastBssid_get(
    uint32                  unit,
    rtk_capwap_mcastBssid_t *pEntry,
    uint32                  arraySize,
    rtk_mac_t               *pBssidArray,
    uint32                  *pBssidCnt)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pBssidArray), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pBssidCnt), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.arraySize, &arraySize, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_MCASTBSSID_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pEntry, &cfg.mcastBssid_entry, sizeof(rtk_capwap_mcastBssid_t));
    osal_memcpy(pBssidArray, &cfg.bssidArray, sizeof(rtk_mac_t));
    osal_memcpy(pBssidCnt, &cfg.bssidCnt, sizeof(uint32));

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_capwap_bssidListNextValid_get(uint32 unit, int32 *pScan_idx, rtk_capwap_bssidList_t *pEntry)
{
    rtdrv_capwapCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pScan_idx), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_capwapCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_CAPWAP_BSSIDLISTNEXTVALID_GET, &cfg, rtdrv_capwapCfg_t, 1);
    osal_memcpy(pScan_idx, &cfg.scan_idx, sizeof(int32));
    osal_memcpy(pEntry, &cfg.bssidList_entry, sizeof(rtk_capwap_bssidList_t));

    return RT_ERR_OK;
}   /* end of rtk_capwap_bssidListNextValid_get */

