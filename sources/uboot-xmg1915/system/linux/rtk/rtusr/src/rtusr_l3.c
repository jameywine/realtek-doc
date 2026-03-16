/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) counter
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rtusr_util.h>
#include <rtdrv/rtdrv_netfilter.h>


int32
rtk_l3_init(uint32 unit)
{
    rtdrv_unitCfg_t data;

    /* function body */
    osal_memset(&data, 0, sizeof(rtdrv_unitCfg_t));
    data.unit = unit;
    SETSOCKOPT(RTDRV_L3_INIT, &data, rtdrv_unitCfg_t, 1);

    return RT_ERR_OK;
} /* end of rtk_l3_init */

int32
rtk_l3_routeEntry_get(uint32 unit, uint32 index, rtk_l3_routeEntry_t *pEntry)
{
    rtdrv_l3_routeEntry_t entry_cfg;

    /* function body */
    osal_memset(&entry_cfg, 0, sizeof(rtdrv_l3_routeEntry_t));
    entry_cfg.unit = unit;
    entry_cfg.index = index;
    GETSOCKOPT(RTDRV_L3_ROUTE_ROUTEENTRY_GET, &entry_cfg, rtdrv_l3_routeEntry_t, 1);
    osal_memcpy(pEntry, &entry_cfg.entry, sizeof(rtk_l3_routeEntry_t));

    return RT_ERR_OK;
}

int32
rtk_l3_routeEntry_set(uint32 unit, uint32 index, rtk_l3_routeEntry_t *pEntry)
{
    rtdrv_l3_routeEntry_t entry_cfg;

    /* function body */
    osal_memset(&entry_cfg, 0, sizeof(rtdrv_l3_routeEntry_t));
    entry_cfg.unit = unit;
    entry_cfg.index = index;
    osal_memcpy(&entry_cfg.entry, pEntry, sizeof(rtk_l3_routeEntry_t));
    SETSOCKOPT(RTDRV_L3_ROUTE_ROUTEENTRY_SET, &entry_cfg, rtdrv_l3_routeEntry_t, 1);

    return RT_ERR_OK;
}

int32
rtk_l3_routeSwitchMacAddr_get(uint32 unit, uint32 index, rtk_mac_t *pMac)
{
    rtdrv_l3_config_t config;

    /* function body */
    osal_memset(&config, 0, sizeof(rtdrv_l3_config_t));
    config.unit = unit;
    config.index = index;
    GETSOCKOPT(RTDRV_L3_ROUTE_SWITCHMACADDR_GET, &config, rtdrv_l3_config_t, 1);
    osal_memcpy(pMac, &config.mac, sizeof(rtk_mac_t));

    return RT_ERR_OK;
}

int32
rtk_l3_routeSwitchMacAddr_set(uint32 unit, uint32 index, rtk_mac_t *pMac)
{
    rtdrv_l3_config_t config;

    /* function body */
    osal_memset(&config, 0, sizeof(rtdrv_l3_config_t));
    config.unit = unit;
    config.index = index;
    osal_memcpy( &config.mac, pMac, sizeof(rtk_mac_t));
    SETSOCKOPT(RTDRV_L3_ROUTE_SWITCHMACADDR_SET, &config, rtdrv_l3_config_t, 1);

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_l3_info_get
 * Description:
 *      Get L3-related information
 * Input:
 *      unit  - unit id
 *      pInfo - pointer to L3 information
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_info_get(uint32 unit, rtk_l3_info_t *pInfo)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pInfo), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_L3_INFO_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pInfo, &cfg.info, sizeof(rtk_l3_info_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_info_get */

/* Function Name:
 *      rtk_l3_intf_t_init
 * Description:
 *      Initialize a rtk_l3_intf_t_init structure
 * Input:
 *      unit  - unit id
 *      pIntf - pointer to L3 interface
 * Output:
 *      pIntf - pointer to L3 interface (initialized)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_intf_t_init(rtk_l3_intf_t *pIntf)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_l3_intf_t));
    SETSOCKOPT(RTDRV_L3_INTF_T_INIT, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pIntf, &cfg.intf, sizeof(rtk_l3_intf_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_intf_t_init */


/* Function Name:
 *      rtk_l3_intf_create
 * Description:
 *      Create a new L3 interface
 * Input:
 *      unit  - unit id
 *      pIntf - pointer to L3 interface containing the basic inputs
 * Output:
 *      pIntf - pointer to L3 interface (including all info)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID                  - invalid unit id
 *      RT_ERR_NOT_INIT                 - the module is not initial
 *      RT_ERR_NULL_POINTER             - input parameter may be a null pointer
 *      RT_ERR_INTF_MTU_VARIETY_EXCEEDS - variety of MTU values is over the maximum H/W supports
 * Note:
 *      (1) Basic required input parameters of the pIntf as input:
 *          mac_addr, vid.
 */
int32
rtk_l3_intf_create(uint32 unit, rtk_l3_intf_t *pIntf)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_l3_intf_t));
    SETSOCKOPT(RTDRV_L3_INTF_CREATE, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pIntf, &cfg.intf, sizeof(rtk_l3_intf_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_intf_create */


/* Function Name:
 *      rtk_l3_intf_destroy
 * Description:
 *      Destroy an L3 interface
 * Input:
 *      unit   - unit id
 *      intfId - L3 interface ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 * Note:
 */
int32
rtk_l3_intf_destroy(uint32 unit, rtk_intf_id_t intfId)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    SETSOCKOPT(RTDRV_L3_INTF_DESTROY, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_intf_destroy */


/* Function Name:
 *      rtk_l3_intf_destroyAll
 * Description:
 *      Destroy all L3 interfaces
 * Input:
 *      unit  - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_intf_destroyAll(uint32 unit)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    SETSOCKOPT(RTDRV_L3_INTF_DESTROYALL, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_intf_destroyAll */


/* Function Name:
 *      rtk_l3_intf_get
 * Description:
 *      Get an L3 interface by interface ID/VID/MAC+VID.
 * Input:
 *      unit  - unit id
 *      type  - search key type
 *      pIntf - pointer to L3 interface (interface id, mac_addr, and vid)
 * Output:
 *      pIntf - pointer to L3 interface (all info)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *          RTK_L3_INTFKEYTYPE_VID          - identify by VLAN ID
 *          RTK_L3_INTFKEYTYPE_MAC_AND_VID  - identify by MAC and VLAN ID
 */
int32
rtk_l3_intf_get(uint32 unit, rtk_l3_intfKeyType_t type, rtk_l3_intf_t *pIntf)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_l3_intfKeyType_t));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_l3_intf_t));
    GETSOCKOPT(RTDRV_L3_INTF_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pIntf, &cfg.intf, sizeof(rtk_l3_intf_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_intf_get */


/* Function Name:
 *      rtk_l3_intf_set
 * Description:
 *      Set an L3 interface by interface ID/VID/MAC+VID.
 * Input:
 *      unit  - unit id
 *      type  - search key type
 *      pIntf - pointer to L3 interface (interface id, mac_addr, and vid)
 * Output:
 *      pIntf - pointer to L3 interface (all info)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID                  - invalid unit id
 *      RT_ERR_NOT_INIT                 - the module is not initial
 *      RT_ERR_NULL_POINTER             - input parameter may be a null pointer
 *      RT_ERR_INTF_MTU_VARIETY_EXCEEDS - variety of MTU values is over the maximum H/W supports
 * Note:
 *          RTK_L3_INTFKEYTYPE_VID          - identify by VLAN ID
 *          RTK_L3_INTFKEYTYPE_MAC_AND_VID  - identify by MAC and VLAN ID
 */
int32
rtk_l3_intf_set(uint32 unit, rtk_l3_intfKeyType_t type, rtk_l3_intf_t *pIntf)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_l3_intfKeyType_t));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_l3_intf_t));
    SETSOCKOPT(RTDRV_L3_INTF_SET, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_intf_set */


/* Function Name:
 *      rtk_l3_intfStat_get
 * Description:
 *      Get a statistic counter of the specified interface
 * Input:
 *      unit     - unit id
 *      intfId   - L3 interface id
 *      stat     - type of statistic counter
 * Output:
 *      pValue64 - pointer to the 64-bit unsigned-integer value of the statistic result
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_intfStat_get(uint32 unit, rtk_intf_id_t intfId, rtk_l3_stat_t stat, uint64 *pValue64)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pValue64), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    osal_memcpy(&cfg.stat, &stat, sizeof(rtk_l3_stat_t));
    GETSOCKOPT(RTDRV_L3_INTFSTAT_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pValue64, &cfg.value64, sizeof(uint64));

    return RT_ERR_OK;
}   /* end of rtk_l3_intfStat_get */


/* Function Name:
 *      rtk_l3_nextHop_t_init
 * Description:
 *      Initialize a rtk_l3_nextHop_t structure
 * Input:
 *      unit     - unit id
 *      pNextHop - pointer to L3 nexthop info (uninitilized)
 * Output:
 *      pNextHop - pointer to L3 nexthop info (initilized)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_nextHop_t_init(rtk_l3_nextHop_t *pNextHop)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNextHop), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.nextHop, pNextHop, sizeof(rtk_l3_nextHop_t));
    SETSOCKOPT(RTDRV_L3_NEXTHOP_T_INIT, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pNextHop, &cfg.nextHop, sizeof(rtk_l3_nextHop_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_nextHop_t_init */


/* Function Name:
 *      rtk_l3_nextHop_create
 * Description:
 *      Create an L3 nexthop and get the returned path ID
 * Input:
 *      unit     - unit id
 *      flags    - optional flags
 *      pNextHop - pointer to nexthop
 * Output:
 *      pPathId  - pointer to L3 path ID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *      (1) If the flag RTK_L3_FLAG_REPLACE is set, then replace the existing entry
 *          with the new info based on the input path ID (nhId).
 *          Otherwise, SDK will allocate a path ID for this new nexthop entry.
 */
int32
rtk_l3_nextHop_create(uint32 unit, rtk_l3_flag_t flags, rtk_l3_nextHop_t *pNextHop, rtk_l3_pathId_t *pNhId)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNextHop), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pNhId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    osal_memcpy(&cfg.nextHop, pNextHop, sizeof(rtk_l3_nextHop_t));
    osal_memcpy(&cfg.nhId, pNhId, sizeof(rtk_l3_pathId_t));
    SETSOCKOPT(RTDRV_L3_NEXTHOP_CREATE, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pNhId, &cfg.nhId, sizeof(rtk_l3_pathId_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_nextHop_create */


/* Function Name:
 *      rtk_l3_nextHop_destroy
 * Description:
 *      Destroy an L3 Next-Hop
 * Input:
 *      unit   - unit id
 *      pathId - pointer to L3 path ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - the module is not initial
 * Note:
 */
int32
rtk_l3_nextHop_destroy(uint32 unit, rtk_l3_pathId_t nhId)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.nhId, &nhId, sizeof(rtk_l3_pathId_t));
    SETSOCKOPT(RTDRV_L3_NEXTHOP_DESTROY, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_nextHop_destroy */


/* Function Name:
 *      rtk_l3_nextHop_get
 * Description:
 *      Get an L3 Next-Hop by path ID
 * Input:
 *      unit     - unit id
 *      pathId   - L3 path ID
 * Output:
 *      pNextHop - pointer to nexthop
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_nextHop_get(uint32 unit, rtk_l3_pathId_t nhId,  rtk_l3_nextHop_t *pNextHop)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNextHop), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.nhId, &nhId, sizeof(rtk_l3_pathId_t));
    GETSOCKOPT(RTDRV_L3_NEXTHOP_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pNextHop, &cfg.nextHop, sizeof(rtk_l3_nextHop_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_nextHop_get */


/* Function Name:
 *      rtk_l3_nextHopPath_find
 * Description:
 *      Find an path ID pointing to a nexthop
 * Input:
 *      unit     - unit id
 *      pNextHop - pointer to nexthop
 * Output:
 *      pPathId  - pointer to L3 path ID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_nextHopPath_find(uint32 unit, rtk_l3_nextHop_t *pNextHop, rtk_l3_pathId_t *pNhId)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNextHop), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pNhId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.nextHop, pNextHop, sizeof(rtk_l3_nextHop_t));
    GETSOCKOPT(RTDRV_L3_NEXTHOPPATH_FIND, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pNhId, &cfg.nhId, sizeof(rtk_l3_pathId_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_nextHopPath_find */


/* Function Name:
 *      rtk_l3_ecmp_create
 * Description:
 *      Create an ECMP path (contains one or more nexthop paths)
 * Input:
 *      unit       - unit id
 *      flags      - optional flags (REPLACE flag for updating)
 *      pathCnt    - size of the allocated array
 *      pIntfArray - pointer to the path ID array
 *      pEcmpIntf  - pointer to the ECMP path ID
 * Output:
 *      pEcmpIntf  - pointer to the ECMP path ID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_ecmp_create(uint32 unit, rtk_l3_flag_t flags, uint32 nhIdCnt, rtk_l3_pathId_t *pNhIdArray, rtk_l3_pathId_t *pEcmpId)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNhIdArray), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEcmpId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    osal_memcpy(&cfg.nhIdCnt, &nhIdCnt, sizeof(uint32));
    osal_memcpy(&cfg.nhIdArray, pNhIdArray, sizeof(rtk_l3_pathId_t) * RTK_L3_ECMP_NEXTHOP_CNT_MAX);
    SETSOCKOPT(RTDRV_L3_ECMP_CREATE, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pEcmpId, &cfg.ecmpId, sizeof(rtk_l3_pathId_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmp_create */


/* Function Name:
 *      rtk_l3_ecmp_destroy
 * Description:
 *      Destroy an ECMP path
 * Input:
 *      unit       - unit id
 *      ecmpPathId - pointer to the ECMP path ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_ecmp_destroy(uint32 unit, rtk_l3_pathId_t ecmpPId)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpPId, &ecmpPId, sizeof(rtk_l3_pathId_t));
    SETSOCKOPT(RTDRV_L3_ECMP_DESTROY, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmp_destroy */


/* Function Name:
 *      rtk_l3_ecmp_get
 * Description:
 *      Get all path IDs of an ECMP path
 * Input:
 *      unit          - unit id
 *      ecmpPathId    - ECMP path ID
 *      nhIdArraySize - size of allocated entries in pIntf_array
 * Output:
 *      pNhIdArray - array of ECMP path IDs
 *      pNhIdCount - number of entries of intf_count actually filled in.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_ecmp_get(uint32 unit, rtk_l3_pathId_t ecmpId, uint32 nhIdArraySize, rtk_l3_pathId_t *pNhIdArray, uint32 *pNhIdCount)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNhIdArray), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pNhIdCount), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpId, &ecmpId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.nhIdArraySize, &nhIdArraySize, sizeof(uint32));
    GETSOCKOPT(RTDRV_L3_ECMP_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pNhIdArray, &cfg.nhIdArray, sizeof(rtk_l3_pathId_t));
    osal_memcpy(pNhIdCount, &cfg.nhIdCount, sizeof(uint32));

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmp_get */


/* Function Name:
 *      rtk_l3_ecmp_add
 * Description:
 *      Add a nexthop in an ECMP entry
 * Input:
 *      unit       - unit id
 *      ecmpPathId - ECMP path ID
 *      pathId     - nexthop path ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_ecmp_add(uint32 unit, rtk_l3_pathId_t ecmpId, rtk_l3_pathId_t nhId)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpId, &ecmpId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.nhId, &nhId, sizeof(rtk_l3_pathId_t));
    SETSOCKOPT(RTDRV_L3_ECMP_ADD, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmp_add */


/* Function Name:
 *      rtk_l3_ecmp_del
 * Description:
 *      Delete a nexthop from an ECMP entry
 * Input:
 *      unit   - unit id
 *      ecmpId - ECMP path ID
 *      nhId   - nexthop path ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_ecmp_del(uint32 unit, rtk_l3_pathId_t ecmpId, rtk_l3_pathId_t nhId)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpId, &ecmpId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.nhId, &nhId, sizeof(rtk_l3_pathId_t));
    SETSOCKOPT(RTDRV_L3_ECMP_DEL, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmp_del */


/* Function Name:
 *      rtk_l3_ecmp_find
 * Description:
 *      Find a nexthop pointing to a ECMP path
 * Input:
 *      unit       - unit id
 *      nhIdCount  - number of path IDs in pIntf_array
 *      pNhIdArray - pointer to the path IDs
 * Output:
 *      pEcmpId    - ECMP path ID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_ecmp_find(uint32 unit, uint32 nhIdCount, rtk_l3_pathId_t *pNhIdArray, rtk_l3_pathId_t *pEcmpId)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNhIdArray), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEcmpId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.nhIdCount, &nhIdCount, sizeof(uint32));
    GETSOCKOPT(RTDRV_L3_ECMP_FIND, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pNhIdArray, &cfg.nhIdArray, sizeof(rtk_l3_pathId_t));
    osal_memcpy(pEcmpId, &cfg.ecmpId, sizeof(rtk_l3_pathId_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmp_find */

int32
rtk_l3_ecmpHashTbl_get(uint32 unit, rtk_l3_pathId_t ecmpId, uint32 nhIdxCount, uint32 *pNhIdxArray)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNhIdxArray), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpId, &ecmpId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.nhIdxCount, &nhIdxCount, sizeof(uint32));
    GETSOCKOPT(RTDRV_L3_ECMPHASHTBL_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pNhIdxArray, &cfg.nhIdxArray, sizeof(uint32) * RTK_L3_ECMP_HASH_IDX_MAX);

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmpHashTbl_get */

int32
rtk_l3_ecmpHashTbl_set(uint32 unit, rtk_l3_pathId_t ecmpId, uint32 nhIdxCount, uint32 *pNhIdxArray)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNhIdxArray), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpId, &ecmpId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.nhIdxCount, &nhIdxCount, sizeof(uint32));
    osal_memcpy(&cfg.nhIdxArray, pNhIdxArray, sizeof(uint32) * RTK_L3_ECMP_HASH_IDX_MAX);
    SETSOCKOPT(RTDRV_L3_ECMPHASHTBL_SET, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmpHashTbl_set */

int32
rtk_l3_ecmpCtrl_get(uint32 unit, rtk_l3_pathId_t ecmpId, rtk_l3_ecmpCtrlType_t type, int32 *pArg)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpId, &ecmpId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.ecmpCtrlType, &type, sizeof(rtk_l3_ecmpCtrlType_t));
    GETSOCKOPT(RTDRV_L3_ECMPCTRL_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pArg, &cfg.arg, sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmpCtrl_get */

int32
rtk_l3_ecmpCtrl_set(uint32 unit, rtk_l3_pathId_t ecmpId, rtk_l3_ecmpCtrlType_t type, int32 arg)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.ecmpId, &ecmpId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.ecmpCtrlType, &type, sizeof(rtk_l3_ecmpCtrlType_t));
    osal_memcpy(&cfg.arg, &arg, sizeof(int32));
    SETSOCKOPT(RTDRV_L3_ECMPCTRL_SET, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_ecmpCtrl_set */

/* Function Name:
 *      rtk_l3_key_t_init
 * Description:
 *      Initialize a rtk_l3_key_t_init structure
 * Input:
 *      unit - unit id
 *      pKey - pointer to L3 key (uninitialized)
 * Output:
 *      pKey - pointer to L3 key (initialized)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_key_t_init(rtk_l3_key_t *pKey)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pKey), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.key, pKey, sizeof(rtk_l3_key_t));
    SETSOCKOPT(RTDRV_L3_KEY_T_INIT, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pKey, &cfg.key, sizeof(rtk_l3_key_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_key_t_init */


/* Function Name:
 *      rtk_l3_host_t_init
 * Description:
 *      Initialize a rtk_l3_host_t structure
 * Input:
 *      pHost - pointer to host entry info (uninitilized)
 * Output:
 *      pHost - pointer to host entry info (initilized)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_host_t_init(rtk_l3_host_t *pHost)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pHost), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.host, pHost, sizeof(rtk_l3_host_t));
    SETSOCKOPT(RTDRV_L3_HOST_T_INIT, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pHost, &cfg.host, sizeof(rtk_l3_host_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_host_t_init */


/* Function Name:
 *      rtk_l3_host_add
 * Description:
 *      Add an entry into the L3 host routing table
 * Input:
 *      unit  - unit id
 *      pHost - pointer to rtk_l3_host_t containing the basic inputs
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID            - invalid unit id
 *      RT_ERR_NOT_INIT           - the module is not initial
 *      RT_ERR_NULL_POINTER       - input parameter may be a null pointer
 *      RT_ERR_ENTRY_NOTFOUND     - specified entry not found (when try to replace)
 *      RT_ERR_L3_PATH_ID_INVALID - the path ID (nexthop/ECMP) is invalid
 * Note:
 *      (1) Basic required input parameters of the pRoute as input keys:
 *          vrf_id, ip_addr (either ipv4 or ipv6), and pathId (if RTK_L3_FLAG_NULL_INTF isn't set).
 *          RTK_L3_FLAG_REPLACE         - replace the existing entry with the new info
 *          RTK_L3_FLAG_NULL_INTF       - the route is destined to a null interface
 *                                        (path ID will not be referred)
 *          RTK_L3_FLAG_TTL_DEC_IGNORE  - ignore TTL decreasement
 *          RTK_L3_FLAG_TTL_CHK_IGNORE  - ignore TTL check
 *          RTK_L3_FLAG_QOS_ASSIGN      - assign a new internal priority (with qos_pri field)
 */
int32
rtk_l3_host_add(uint32 unit, rtk_l3_host_t *pHost)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pHost), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.host, pHost, sizeof(rtk_l3_host_t));
    SETSOCKOPT(RTDRV_L3_HOST_ADD, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_host_add */


/* Function Name:
 *      rtk_l3_host_del
 * Description:
 *      Delete an entry from the L3 host routing table
 * Input:
 *      unit  - unit id
 *      pHost - pointer to rtk_l3_host_t containing the basic inputs
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID        - invalid unit id
 *      RT_ERR_NOT_INIT       - the module is not initial
 *      RT_ERR_NULL_POINTER   - input parameter may be a null pointer
 *      RT_ERR_ENTRY_NOTFOUND - specified entry not found
 * Note:
 *      (1) Basic required input parameters of the pRoute as input keys:
 *          vrf_id and ip_addr (either ipv4 or ipv6).
 */
int32
rtk_l3_host_del(uint32 unit, rtk_l3_host_t *pHost)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pHost), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.host, pHost, sizeof(rtk_l3_host_t));
    SETSOCKOPT(RTDRV_L3_HOST_DEL, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_host_del */


/* Function Name:
 *      rtk_l3_host_del_byNetwork
 * Description:
 *      Delete L3 host entries based on IP prefix (network)
 * Input:
 *      unit   - unit id
 *      pRoute - pointer to the structure containing the basic inputs
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *      (1) Basic required input parameters of the pRoute as input keys:
 *          vrf_id, ip_addr (either ipv4 or ipv6) and prefix_len.
 */
int32
rtk_l3_host_del_byNetwork(uint32 unit, rtk_l3_route_t *pRoute)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pRoute), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.route, pRoute, sizeof(rtk_l3_route_t));
    SETSOCKOPT(RTDRV_L3_HOST_DEL_BYNETWORK, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_host_del_byNetwork */


/* Function Name:
 *      rtk_l3_host_del_byIntfId
 * Description:
 *      Delete L3 host entries that match or do not match a specified L3 interface number
 * Input:
 *      unit  - unit id
 *      flags - control flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *          RTK_L3_FLAG_NEGATE  - to indicate the action is applied to the unmatched entries
 */
int32
rtk_l3_host_del_byIntfId(uint32 unit, rtk_intf_id_t intfId, rtk_l3_flag_t flags)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    SETSOCKOPT(RTDRV_L3_HOST_DEL_BYINTFID, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_host_del_byIntfId */


/* Function Name:
 *      rtk_l3_host_delAll
 * Description:
 *      Delete all L3 host table entries
 * Input:
 *      unit  - unit id
 *      flags - control flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_host_delAll(uint32 unit, rtk_l3_flag_t flags)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    SETSOCKOPT(RTDRV_L3_HOST_DELALL, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_host_delAll */


/* Function Name:
 *      rtk_l3_host_find
 * Description:
 *      Find an L3 host entry based on IP address
 * Input:
 *      unit  - unit id
 *      pHost - pointer to the structure containing the basic inputs
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID        - invalid unit id
 *      RT_ERR_NOT_INIT       - the module is not initial
 *      RT_ERR_NULL_POINTER   - input parameter may be a null pointer
 *      RT_ERR_ENTRY_NOTFOUND - specified entry not found
 * Note:
 *      (1) Basic required input parameters of the pRoute as input keys:
 *          vrf_id and ip_addr (either ipv4 or ipv6).
 *          RTK_L3_FLAG_HIT_CLEAR   - clear the hit bit if it's set
 */
int32
rtk_l3_host_find(uint32 unit, rtk_l3_host_t *pHost)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pHost), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.host, pHost, sizeof(rtk_l3_host_t));
    GETSOCKOPT(RTDRV_L3_HOST_FIND, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pHost, &cfg.host, sizeof(rtk_l3_host_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_host_find */


/* Function Name:
 *      rtk_l3_hostConflict_get
 * Description:
 *      Return list of conflicts in the L3 table
 * Input:
 *      unit       - unit id
 *      pKey       - IP address to test conflict condition
 *      pHostArray - pointer to the array of conflicting addresses
 *      maxHost    - maximum number of conflicts that may be returned
 * Output:
 *      pHostArray - pointer to the array of conflicting addresses returned
 *      pHostCount - actual number of conflicting addresses returned
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *      (1) Basic required input parameters of the pRoute as input keys:
 *          vrf_id and ip_addr (either ipv4 or ipv6).
 */
int32
rtk_l3_hostConflict_get(uint32 unit, rtk_l3_key_t *pKey, rtk_l3_host_t *pHostArray, int32 maxHost, int32 *pHostCount)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pKey), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pHostArray), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pHostCount), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.key, pKey, sizeof(rtk_l3_key_t));
    osal_memcpy(&cfg.maxHost, &maxHost, sizeof(int32));
    cfg.pHostArray = pHostArray;
    GETSOCKOPT(RTDRV_L3_HOSTCONFLICT_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pHostCount, &cfg.hostCount,sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_l3_hostConflict_get */


/* Function Name:
 *      rtk_l3_host_age
 * Description:
 *      Run L3 host table aging
 * Input:
 *      unit    - unit id
 *      flags   - control flags
 *      fAge    - callback function, NULL if none
 *      pCookie - user data to be passed to callback function
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *          RTK_L3_FLAG_HIT     - if set, then clear the hit bits of the traversed entries when
 *                                their hit bits are set.
 *                                Otherwise, delete the unused entries which its hit bit is clear.
 */
int32
rtk_l3_host_age(uint32 unit, rtk_l3_flag_t flags, rtk_l3_hostTraverseCallback_f fAge, void *pCookie)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL != fAge), RT_ERR_INPUT);         /* not support in user-space */
    RT_PARAM_CHK((NULL != pCookie), RT_ERR_INPUT);      /* not support in user-space */

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    osal_memcpy(&cfg.fHostCb, &fAge, sizeof(rtk_l3_hostTraverseCallback_f));
    GETSOCKOPT(RTDRV_L3_HOST_AGE, &cfg, rtdrv_l3Cfg_t, 1);
    /*osal_memcpy(pCookie, &cfg.cookie, sizeof(void));*/

    return RT_ERR_OK;
}   /* end of rtk_l3_host_age */


/* Function Name:
 *      rtk_l3_host_getNext
 * Description:
 *      Get the next valid L3 host entry (based on the base index)
 * Input:
 *      unit  - unit id
 *      flags - control flags
 *      pBase - pointer to the starting valid entry number to be searched
 * Output:
 *      pBase - pointer to the index of the returned entry (-1 means not found)
 *      pHost - L3 host entry (if found)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *      (2) Use base index -1 to indicate to search from the beginging of L3 host table
 *      (3) If the returned index is -1, means not found the next valid entry
 */
int32
rtk_l3_host_getNext(uint32 unit, rtk_l3_flag_t flags, int32 *pBase, rtk_l3_host_t *pHost)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pBase), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pHost), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    osal_memcpy(&cfg.base, pBase, sizeof(int32));
    GETSOCKOPT(RTDRV_L3_HOST_GETNEXT, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pBase, &cfg.base, sizeof(int32));
    osal_memcpy(pHost, &cfg.host, sizeof(rtk_l3_host_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_host_getNext */


/* Function Name:
 *      rtk_l3_route_t_init
 * Description:
 *      Initialize a rtk_l3_route_t structure
 * Input:
 *      pRoute - pointer to L3 route entry info (uninitilized)
 * Output:
 *      pRoute - pointer to L3 route entry info (initilized)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_route_t_init(rtk_l3_route_t *pRoute)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pRoute), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.route, pRoute, sizeof(rtk_l3_route_t));
    SETSOCKOPT(RTDRV_L3_ROUTE_T_INIT, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pRoute, &cfg.route, sizeof(rtk_l3_route_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_route_t_init */


/* Function Name:
 *      rtk_l3_route_add
 * Description:
 *      Add an IP route to the L3 route table
 * Input:
 *      unit   - unit id
 *      pRoute - pointer to rtk_l3_route_t containing the basic inputs
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID            - invalid unit id
 *      RT_ERR_NOT_INIT           - the module is not initial
 *      RT_ERR_NULL_POINTER       - input parameter may be a null pointer
 *      RT_ERR_ENTRY_NOTFOUND     - specified entry not found (when try to replace)
 *      RT_ERR_L3_PATH_ID_INVALID - the path ID (nexthop/ECMP) is invalid
 * Note:
 *      (1) Basic required input parameters of the pRoute as input:
 *          vrf_id, ip_addr (either ipv4 or ipv6), prefix_len and pathId (if RTK_L3_FLAG_NULL_INTF isn't set).
 *          RTK_L3_FLAG_REPLACE         - replace the existing entry with the new info
 *          RTK_L3_FLAG_NULL_INTF       - the route is destined to a null interface
 *                                        (path ID will not be referred)
 *          RTK_L3_FLAG_TTL_DEC_IGNORE  - ignore TTL decreasement
 *          RTK_L3_FLAG_TTL_CHK_IGNORE  - ignore TTL check
 *          RTK_L3_FLAG_QOS_ASSIGN      - assign a new internal priority (with qos_pri field)
 */
int32
rtk_l3_route_add(uint32 unit, rtk_l3_route_t *pRoute)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pRoute), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.route, pRoute, sizeof(rtk_l3_route_t));
    SETSOCKOPT(RTDRV_L3_ROUTE_ADD, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_route_add */


/* Function Name:
 *      rtk_l3_route_del
 * Description:
 *      Delete an IP route from the L3 route table
 * Input:
 *      unit   - unit id
 *      pRoute - pointer to rtk_l3_route_t containing the basic inputs
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID        - invalid unit id
 *      RT_ERR_NOT_INIT       - the module is not initial
 *      RT_ERR_NULL_POINTER   - input parameter may be a null pointer
 *      RT_ERR_ENTRY_NOTFOUND - specified entry not found
 * Note:
 *      (1) Basic required input parameters of the pRoute as input:
 *          vrf_id, ip_addr (either ipv4 or ipv6) and prefix_len.
 */
int32
rtk_l3_route_del(uint32 unit, rtk_l3_route_t *pRoute)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pRoute), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.route, pRoute, sizeof(rtk_l3_route_t));
    SETSOCKOPT(RTDRV_L3_ROUTE_DEL, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_route_del */


/* Function Name:
 *      rtk_l3_route_get
 * Description:
 *      Look up a route given the network and netmask
 * Input:
 *      unit   - unit id
 *      pRoute - pointer to rtk_l3_route_t specifying the basic inputs
 * Output:
 *      pRoute - pointer to returned rtk_l3_route_t info
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID        - invalid unit id
 *      RT_ERR_NOT_INIT       - the module is not initial
 *      RT_ERR_NULL_POINTER   - input parameter may be a null pointer
 *      RT_ERR_ENTRY_NOTFOUND - specified entry not found
 * Note:
 *      (1) Basic required input parameters of the pRoute as input:
 *          vrf_id, ip_addr (either ipv4 or ipv6) and prefix_len.
 *          RTK_L3_FLAG_HIT_CLEAR   - clear the hit bit if it's set
 */
int32
rtk_l3_route_get(uint32 unit, rtk_l3_route_t *pRoute)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pRoute), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.route, pRoute, sizeof(rtk_l3_route_t));
    GETSOCKOPT(RTDRV_L3_ROUTE_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pRoute, &cfg.route, sizeof(rtk_l3_route_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_route_get */


/* Function Name:
 *      rtk_l3_route_del_byIntfId
 * Description:
 *      Delete routes based on matching or non-matching L3 interface ID
 * Input:
 *      unit   - unit id
 *      flags  - control flags
 *      intfId - L3 interface ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 * Note:
 *          RTK_L3_FLAG_NEGATE  - to indicate the action is applied to the unmatched entries
 */
int32
rtk_l3_route_del_byIntfId(uint32 unit, rtk_l3_flag_t flags, rtk_intf_id_t intfId)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    SETSOCKOPT(RTDRV_L3_ROUTE_DEL_BYINTFID, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_route_del_byIntfId */


/* Function Name:
 *      rtk_l3_route_delAll
 * Description:
 *      Delete all L3 route table entries
 * Input:
 *      unit  - unit id
 *      flags - control flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_route_delAll(uint32 unit, rtk_l3_flag_t flags)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    SETSOCKOPT(RTDRV_L3_ROUTE_DELALL, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_route_delAll */


/* Function Name:
 *      rtk_l3_route_age
 * Description:
 *      Run L3 route table aging
 * Input:
 *      unit    - unit id
 *      flags   - control flags
 *      fAge    - callback function, NULL if none
 *      pCookie - user data to be passed to callback function
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *          RTK_L3_FLAG_HIT     - if set, then clear the hit bits of the traversed entries when
 *                                their hit bits are set.
 *                                Otherwise, delete the unused entries which its hit bit is clear.
 */
int32
rtk_l3_route_age(uint32 unit, uint32 flags, rtk_l3_routeTraverseCallback_f fAge, void *pCookie)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL != fAge), RT_ERR_INPUT);         /* not support in user-space */
    RT_PARAM_CHK((NULL != pCookie), RT_ERR_INPUT);      /* not support in user-space */

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(uint32));
    osal_memcpy(&cfg.fRouteCb, &fAge, sizeof(rtk_l3_routeTraverseCallback_f));
    GETSOCKOPT(RTDRV_L3_ROUTE_AGE, &cfg, rtdrv_l3Cfg_t, 1);
    /*osal_memcpy(pCookie, &cfg.cookie, sizeof(void));*/

    return RT_ERR_OK;
}   /* end of rtk_l3_route_age */


/* Function Name:
 *      rtk_l3_route_getNext
 * Description:
 *      Get the next valid L3 route entry (based on the base index)
 * Input:
 *      unit   - unit id
 *      flags  - control flags (applicable flags: RTK_L3_FLAG_IPV6)
 *      pBase  - pointer to the starting index number to be searched
 * Output:
 *      pBase  - pointer to the index of the returned entry (-1 means not found)
 *      pRoute - L3 route entry
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID        - invalid unit id
 *      RT_ERR_NOT_INIT       - the module is not initial
 *      RT_ERR_NULL_POINTER   - input parameter may be a null pointer
 * Note:
 *      (2) Use base index -1 to indicate to search from the beginging of L3 route table
 *      (3) If the returned index is -1, means not found the next valid entry
 */
int32
rtk_l3_route_getNext(uint32 unit, rtk_l3_flag_t flags, int32 *pBase, rtk_l3_route_t *pRoute)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pBase), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pRoute), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.flags, &flags, sizeof(rtk_l3_flag_t));
    osal_memcpy(&cfg.base, pBase, sizeof(int32));
    GETSOCKOPT(RTDRV_L3_ROUTE_GETNEXT, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pBase, &cfg.base, sizeof(int32));
    osal_memcpy(pRoute, &cfg.route, sizeof(rtk_l3_route_t));

    return RT_ERR_OK;
}   /* end of rtk_l3_route_getNext */

/* Function Name:
 *      rtk_l3_globalCtrl_get
 * Description:
 *      Get the configuration of the specified control type
 * Input:
 *      unit - unit id
 *      type - control type
 * Output:
 *      pArg - pointer to the argurment
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_globalCtrl_get(uint32 unit, rtk_l3_globalCtrlType_t type, int32 *pArg)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.globalCtrlType, &type, sizeof(rtk_l3_globalCtrlType_t));
    GETSOCKOPT(RTDRV_L3_GLOBALCTRL_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pArg, &cfg.arg, sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_l3_globalCtrl_get */


/* Function Name:
 *      rtk_l3_globalCtrl_set
 * Description:
 *      Set the configuration of the specified control type
 * Input:
 *      unit - unit id
 *      type - control type
 *      arg  - argurment
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_globalCtrl_set(uint32 unit, rtk_l3_globalCtrlType_t type, int32 arg)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.globalCtrlType, &type, sizeof(rtk_l3_globalCtrlType_t));
    osal_memcpy(&cfg.arg, &arg, sizeof(int32));
    SETSOCKOPT(RTDRV_L3_GLOBALCTRL_SET, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_globalCtrl_set */


/* Function Name:
 *      rtk_l3_intfCtrl_get
 * Description:
 *      Get the configuration of the specified control type and interface ID
 * Input:
 *      unit   - unit id
 *      intfId - L3 interface id
 *      type   - control type
 * Output:
 *      pArg   - pointer to the argurment
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_l3_intfCtrl_get(uint32 unit, rtk_intf_id_t intfId, rtk_l3_intfCtrlType_t type, int32 *pArg)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_l3_globalCtrlType_t));
    GETSOCKOPT(RTDRV_L3_INTFCTRL_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pArg, &cfg.arg, sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_l3_intfCtrl_get */


/* Function Name:
 *      rtk_l3_intfCtrl_set
 * Description:
 *      Set the configuration of the specified control type and interface ID
 * Input:
 *      unit   - unit id
 *      intfId - L3 interface id
 *      type   - control type
 *      arg    - argurment
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 * Note:
 */
int32
rtk_l3_intfCtrl_set(uint32 unit, rtk_intf_id_t intfId, rtk_l3_intfCtrlType_t type, int32 arg)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_l3_globalCtrlType_t));
    osal_memcpy(&cfg.arg, &arg, sizeof(int32));
    SETSOCKOPT(RTDRV_L3_INTFCTRL_SET, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_intfCtrl_set */

/* Function Name:
 *      rtk_l3_portCtrl_get
 * Description:
 *      Get the configuration of the specified control type and port
 * Input:
 *      unit - unit id
 *      port - port id
 *      type - control type
 * Output:
 *      pArg   - pointer to the argurment
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 */
int32
rtk_l3_portCtrl_get(uint32 unit, rtk_port_t port, rtk_l3_portCtrlType_t type, int32 *pArg)
{
    rtdrv_l3Cfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_l3_portCtrlType_t));
    GETSOCKOPT(RTDRV_L3_PORTCTRL_GET, &cfg, rtdrv_l3Cfg_t, 1);
    osal_memcpy(pArg, &cfg.arg, sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_l3_portCtrl_get */


/* Function Name:
 *      rtk_l3_portCtrl_set
 * Description:
 *      Set the configuration of the specified control type and port
 * Input:
 *      unit - unit id
 *      port - port id
 *      type - control type
 *      arg  - argurment
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 */
int32
rtk_l3_portCtrl_set(uint32 unit, rtk_port_t port, rtk_l3_portCtrlType_t type, int32 arg)
{
    rtdrv_l3Cfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_l3Cfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_l3_portCtrlType_t));
    osal_memcpy(&cfg.arg, &arg, sizeof(int32));
    SETSOCKOPT(RTDRV_L3_PORTCTRL_SET, &cfg, rtdrv_l3Cfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_l3_portCtrl_set */


