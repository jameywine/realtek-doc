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
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Definition those public MPLS routing APIs and its data type in the SDK.
 *
 * Feature : The file have include the following module and sub-modules
 *              1) MPLS
 */



/*
 * Include Files
 */
#include <netinet/in.h>
#include <sys/socket.h>
#include <rtk/mpls.h>
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

/* Module Name : MPLS */

int32
rtk_mpls_init(uint32 unit)
{
    rtdrv_mplsCfg_t cfg;

    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));

    cfg.unit = unit;
    SETSOCKOPT(RTDRV_MPLS_INIT, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_mpls_init */

int32
rtk_mpls_ttlInherit_get(uint32 unit, rtk_mpls_ttlInherit_t *inherit)
{
    rtdrv_mplsCfg_t cfg;

    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));

    cfg.unit = unit;
    GETSOCKOPT(RTDRV_MPLS_TTLINHERIT_GET, &cfg, rtdrv_mplsCfg_t, 1);

    *inherit = cfg.u.inherit;

    return RT_ERR_OK;
}    /* end of rtk_mpls_ttlInherit_get */

int32
rtk_mpls_ttlInherit_set(uint32 unit, rtk_mpls_ttlInherit_t inherit)
{
    rtdrv_mplsCfg_t cfg;

    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));

    cfg.unit = unit;
    cfg.u.inherit = inherit;

    SETSOCKOPT(RTDRV_MPLS_TTLINHERIT_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_mpls_ttlInherit_set */

int32
rtk_mpls_enable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_mplsCfg_t cfg;

    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));

    cfg.unit = unit;
    GETSOCKOPT(RTDRV_MPLS_ENABLE_GET, &cfg, rtdrv_mplsCfg_t, 1);
    *pEnable = cfg.enable;

    return RT_ERR_OK;
}    /* end of rtk_mpls_enable_get */

int32
rtk_mpls_enable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_mplsCfg_t cfg;

    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));

    cfg.unit = unit;
    cfg.enable = enable;
    SETSOCKOPT(RTDRV_MPLS_ENABLE_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_mpls_enable_set */

int32
rtk_mpls_trapTarget_get(uint32 unit, rtk_trapTarget_t *pTarget)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pTarget), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_MPLS_TRAPTARGET_GET, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pTarget, &cfg.u.target, sizeof(rtk_trapTarget_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_trapTarget_get */

int32
rtk_mpls_trapTarget_set(uint32 unit, rtk_trapTarget_t target)
{
    rtdrv_mplsCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.u.target, &target, sizeof(rtk_trapTarget_t));
    SETSOCKOPT(RTDRV_MPLS_TRAPTARGET_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_trapTarget_set */

int32
rtk_mpls_exceptionCtrl_get(uint32 unit, rtk_mpls_exceptionType_t type,
    rtk_action_t *pAction)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pAction), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.u.exceptionType, &type, sizeof(rtk_mpls_exceptionType_t));
    GETSOCKOPT(RTDRV_MPLS_EXCEPTIONCTRL_GET, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pAction, &cfg.action, sizeof(rtk_action_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_exceptionCtrl_get */

int32
rtk_mpls_exceptionCtrl_set(uint32 unit, rtk_mpls_exceptionType_t type,
    rtk_action_t action)
{
    rtdrv_mplsCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.u.exceptionType, &type, sizeof(rtk_mpls_exceptionType_t));
    osal_memcpy(&cfg.action, &action, sizeof(rtk_action_t));
    SETSOCKOPT(RTDRV_MPLS_EXCEPTIONCTRL_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_exceptionCtrl_set */

/* next hop */
int32
rtk_mpls_nextHop_create(uint32 unit, rtk_mpls_nextHop_t *pNh,
    rtk_l3_pathId_t *pPathId)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNh), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pPathId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.u.nexthop, pNh, sizeof(rtk_mpls_nextHop_t));
    SETSOCKOPT(RTDRV_MPLS_NEXTHOP_CREATE, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pPathId, &cfg.pathId, sizeof(rtk_l3_pathId_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_nextHop_create */

int32
rtk_mpls_nextHop_destroy(uint32 unit, rtk_l3_pathId_t pathId)
{
    rtdrv_mplsCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.pathId, &pathId, sizeof(rtk_l3_pathId_t));
    SETSOCKOPT(RTDRV_MPLS_NEXTHOP_DESTROY, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_nextHop_destroy */

int32
rtk_mpls_nextHop_get(uint32 unit, rtk_l3_pathId_t pathId,
    rtk_mpls_nextHop_t *pNh)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNh), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.pathId, &pathId, sizeof(rtk_l3_pathId_t));
    GETSOCKOPT(RTDRV_MPLS_NEXTHOP_GET, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pNh, &cfg.u.nexthop, sizeof(rtk_mpls_nextHop_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_nextHop_get */

int32
rtk_mpls_nextHop_set(uint32 unit, rtk_l3_pathId_t pathId,
    rtk_mpls_nextHop_t *pNh)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pNh), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.pathId, &pathId, sizeof(rtk_l3_pathId_t));
    osal_memcpy(&cfg.u.nexthop, pNh, sizeof(rtk_mpls_nextHop_t));
    SETSOCKOPT(RTDRV_MPLS_NEXTHOP_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_nextHop_set */

int32
rtk_mpls_encap_create(uint32 unit, rtk_mpls_encap_t *pEncap,
    rtk_mpls_entryId_t *pEncapId)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEncap), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEncapId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.u.encap, pEncap, sizeof(rtk_mpls_encap_t));
    SETSOCKOPT(RTDRV_MPLS_ENCAP_CREATE, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pEncapId, &cfg.entryId, sizeof(rtk_mpls_entryId_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_encap_create */

int32
rtk_mpls_encap_destroy(uint32 unit, rtk_mpls_entryId_t encapId)
{
    rtdrv_mplsCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.entryId, &encapId, sizeof(rtk_mpls_entryId_t));
    SETSOCKOPT(RTDRV_MPLS_ENCAP_DESTROY, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_encap_destroy */

int32
rtk_mpls_encap_get(uint32 unit, rtk_mpls_entryId_t encapId,
    rtk_mpls_encap_t *pEncap)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEncap), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.entryId, &encapId, sizeof(rtk_mpls_entryId_t));
    GETSOCKOPT(RTDRV_MPLS_ENCAP_GET, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pEncap, &cfg.u.encap, sizeof(rtk_mpls_encap_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_encap_get */

int32
rtk_mpls_encap_set(uint32 unit, rtk_mpls_entryId_t encapId,
    rtk_mpls_encap_t *pEncap)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEncap), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.entryId, &encapId, sizeof(rtk_mpls_entryId_t));
    osal_memcpy(&cfg.u.encap, pEncap, sizeof(rtk_mpls_encap_t));
    SETSOCKOPT(RTDRV_MPLS_ENCAP_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_encap_set */

int32
rtk_mpls_encapId_find(uint32 unit, rtk_mpls_encap_t *pEncap,
    rtk_mpls_entryId_t *pEncapId)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEncap), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEncapId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(pEncap, &cfg.u.encap, sizeof(rtk_mpls_encap_t));
    GETSOCKOPT(RTDRV_MPLS_ENCAPID_FIND, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pEncapId, &cfg.entryId, sizeof(rtk_mpls_entryId_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_encapId_find */

/* decapsulation */
int32
rtk_mpls_hashAlgo_get(uint32 unit, uint8 *pHashAlgo)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pHashAlgo), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_MPLS_HASHALGO_GET, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pHashAlgo, &cfg.hashAlgo, sizeof(uint8));

    return RT_ERR_OK;
}   /* end of rtk_mpls_hashAlgo_get */

int32
rtk_mpls_hashAlgo_set(uint32 unit, uint8 hashAlgo)
{
    rtdrv_mplsCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.hashAlgo, &hashAlgo, sizeof(uint8));
    SETSOCKOPT(RTDRV_MPLS_HASHALGO_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_hashAlgo_set */

int32
rtk_mpls_decap_create(uint32 unit, rtk_mpls_decapEntry_t *pDecap,
    rtk_mpls_entryId_t *pDecapId)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pDecap), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pDecapId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.u.decap, pDecap, sizeof(rtk_mpls_decapEntry_t));
    SETSOCKOPT(RTDRV_MPLS_DECAP_CREATE, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pDecapId, &cfg.entryId, sizeof(rtk_mpls_entryId_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_decap_create */

int32
rtk_mpls_decap_destroy(uint32 unit, rtk_mpls_entryId_t decapId)
{
    rtdrv_mplsCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.entryId, &decapId, sizeof(rtk_mpls_entryId_t));
    SETSOCKOPT(RTDRV_MPLS_DECAP_DESTROY, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_decap_destory */

int32
rtk_mpls_decap_get(uint32 unit, rtk_mpls_entryId_t decapId,
    rtk_mpls_decapEntry_t *pDecap)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pDecap), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.entryId, &decapId, sizeof(rtk_mpls_entryId_t));
    GETSOCKOPT(RTDRV_MPLS_DECAP_GET, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pDecap, &cfg.u.decap, sizeof(rtk_mpls_decapEntry_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_decap_get */

int32
rtk_mpls_decap_set(uint32 unit, rtk_mpls_entryId_t decapId,
    rtk_mpls_decapEntry_t *pDecap)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pDecap), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.entryId, &decapId, sizeof(rtk_mpls_entryId_t));
    osal_memcpy(&cfg.u.decap, pDecap, sizeof(rtk_mpls_decapEntry_t));
    SETSOCKOPT(RTDRV_MPLS_DECAP_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_decap_set */

int32
rtk_mpls_decapId_find(uint32 unit, rtk_mpls_decapEntry_t *pDecap,
    rtk_mpls_entryId_t *pDecapId)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pDecap), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pDecapId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.u.decap, pDecap, sizeof(rtk_mpls_decapEntry_t));
    GETSOCKOPT(RTDRV_MPLS_DECAPID_FIND, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pDecapId, &cfg.entryId, sizeof(rtk_mpls_entryId_t));

    return RT_ERR_OK;
}   /* end of rtk_mpls_decapId_find */

int32
rtk_mpls_egrTcMap_get(uint32 unit, rtk_mpls_egrTcMapSrc_t *pSrc, uint8 *pTc)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pSrc), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pTc), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.src, pSrc, sizeof(rtk_mpls_egrTcMapSrc_t));
    GETSOCKOPT(RTDRV_MPLS_EGRTCMAP_GET, &cfg, rtdrv_mplsCfg_t, 1);
    osal_memcpy(pTc, &cfg.tc, sizeof(uint8));

    return RT_ERR_OK;
}   /* end of rtk_mpls_egrTcMap_get */

int32
rtk_mpls_egrTcMap_set(uint32 unit, rtk_mpls_egrTcMapSrc_t *pSrc, uint8 tc)
{
    rtdrv_mplsCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pSrc), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_mplsCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.src, pSrc, sizeof(rtk_mpls_egrTcMapSrc_t));
    osal_memcpy(&cfg.tc, &tc, sizeof(uint8));
    SETSOCKOPT(RTDRV_MPLS_EGRTCMAP_SET, &cfg, rtdrv_mplsCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_mpls_egrTcMap_set */

