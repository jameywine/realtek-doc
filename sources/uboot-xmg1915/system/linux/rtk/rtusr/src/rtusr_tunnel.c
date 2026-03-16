/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose :
 *
 * Feature :
 *
 */

/*
 * Include Files
 */
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rtk/tunnel.h>
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
 *      rtk_tunnel_init
 * Description:
 *      Initialize Tunnel module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Note:
 *      Must initialize Tunnel module before calling any tunneling APIs.
 */
int32
rtk_tunnel_init(uint32 unit)
{
    rtdrv_tunnelCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    SETSOCKOPT(RTDRV_TUNNEL_INIT, &cfg, rtdrv_tunnelCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_tunnel_init */

/* Module Name    : Tunneling                */
/* Sub-module Name: Tunneling error handling */

/* Function Name:
 *      rtk_tunnel_info_t_init
 * Description:
 *      Initialize a rtk_tunnel_info_t_init structure
 * Input:
 *      unit  - unit id
 *      pInfo - pointer to tunnel information
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
rtk_tunnel_info_t_init(rtk_tunnel_info_t *pInfo)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pInfo), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.info, pInfo, sizeof(rtk_tunnel_info_t));
    SETSOCKOPT(RTDRV_TUNNEL_INFO_T_INIT, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pInfo, &cfg.info, sizeof(rtk_tunnel_info_t));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_info_t_init */

/* Function Name:
 *      rtk_tunnel_info_get
 * Description:
 *      Get tunnel-related information
 * Input:
 *      unit  - unit id
 *      pInfo - pointer to tunnel information
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
rtk_tunnel_info_get(uint32 unit, rtk_tunnel_info_t *pInfo)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pInfo), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_TUNNEL_INFO_GET, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pInfo, &cfg.info, sizeof(rtk_tunnel_info_t));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_info_get */

/* Function Name:
 *      rtk_tunnel_info_t_init
 * Description:
 *      Initialize a rtk_tunnel_info_t_init structure
 * Input:
 *      unit  - unit id
 *      pInfo - pointer to tunnel information
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
rtk_tunnel_intf_t_init(rtk_tunnel_intf_t *pIntf)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_tunnel_intf_t));
    SETSOCKOPT(RTDRV_TUNNEL_INTF_T_INIT, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pIntf, &cfg.intf, sizeof(rtk_tunnel_intf_t));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intf_t_init */

/* Function Name:
 *      rtk_tunnel_intf_create
 * Description:
 *      Create a new tunnel interface
 * Input:
 *      unit  - unit id
 *      pIntf - pointer to tunnel interface containing the basic inputs
 * Output:
 *      pIntf - pointer to tunnel interface (including all info)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID                  - invalid unit id
 *      RT_ERR_NOT_INIT                 - the module is not initial
 *      RT_ERR_NULL_POINTER             - input parameter may be a null pointer
 *      RT_ERR_INTF_MTU_VARIETY_EXCEEDS - variety of MTU values is over the maximum H/W supports
 * Note:
 *      (1) Basic required input parameters of the pRoute as input:
 *          type and corresponding fields about that tunnel.
 */
int32
rtk_tunnel_intf_create(uint32 unit, rtk_tunnel_intf_t *pIntf)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_tunnel_intf_t));
    SETSOCKOPT(RTDRV_TUNNEL_INTF_CREATE, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pIntf, &cfg.intf, sizeof(rtk_tunnel_intf_t));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intf_create */

/* Function Name:
 *      rtk_tunnel_intf_destroy
 * Description:
 *      Destroy a tunnel interface
 * Input:
 *      unit   - unit id
 *      intfId - tunnel interface ID
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
rtk_tunnel_intf_destroy(uint32 unit, rtk_intf_id_t intfId)
{
    rtdrv_tunnelCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    SETSOCKOPT(RTDRV_TUNNEL_INTF_DESTROY, &cfg, rtdrv_tunnelCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intf_destroy */

/* Function Name:
 *      rtk_tunnel_intf_destroyAll
 * Description:
 *      Destroy all tunnel interfaces
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
rtk_tunnel_intf_destroyAll(uint32 unit)
{
    rtdrv_tunnelCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    SETSOCKOPT(RTDRV_TUNNEL_INTF_DESTROYALL, &cfg, rtdrv_tunnelCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intf_destroyAll */

/* Function Name:
 *      rtk_tunnel_intf_get
 * Description:
 *      Get a tunnel interface by interface ID.
 * Input:
 *      unit  - unit id
 *      pIntf - pointer to tunnel interface (interface id)
 * Output:
 *      pIntf - pointer to tunnel interface (all info)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 *      None
 */
int32
rtk_tunnel_intf_get(uint32 unit, rtk_tunnel_intf_t *pIntf)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_tunnel_intf_t));
    GETSOCKOPT(RTDRV_TUNNEL_INTF_GET, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pIntf, &cfg.intf, sizeof(rtk_tunnel_intf_t));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intf_get */

/* Function Name:
 *      rtk_tunnel_intf_set
 * Description:
 *      Set an tunnel interface by interface ID.
 * Input:
 *      unit  - unit id
 *      type  - search key type
 *      pIntf - pointer to tunnel interface (interface id)
 * Output:
 *      pIntf - pointer to tunnel interface (all info)
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID                  - invalid unit id
 *      RT_ERR_NOT_INIT                 - the module is not initial
 *      RT_ERR_NULL_POINTER             - input parameter may be a null pointer
 *      RT_ERR_INTF_MTU_VARIETY_EXCEEDS - variety of MTU values is over the maximum H/W supports
 * Note:
 *      None
 */
int32
rtk_tunnel_intf_set(uint32 unit, rtk_tunnel_intf_t *pIntf)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pIntf), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intf, pIntf, sizeof(rtk_tunnel_intf_t));
    SETSOCKOPT(RTDRV_TUNNEL_INTF_SET, &cfg, rtdrv_tunnelCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intf_set */

/* Function Name:
 *      rtk_tunnel_intfStat_get
 * Description:
 *      Get a statistic counter of the specified tunnel interface
 * Input:
 *      unit     - unit id
 *      intfId   - tunnel interface id
 *      stat     - type of statistic counter
 * Output:
 *      pValue64 - pointer to the 64-bit unsigned-integer value of the statistic result
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID         - invalid unit id
 *      RT_ERR_NOT_INIT        - the module is not initial
 *      RT_ERR_NULL_POINTER    - input parameter may be a null pointer
 *      RT_ERR_NOT_TUNNEL_INTF - input interface type is not tunnel
 * Note:
 */
int32
rtk_tunnel_intfStat_get(uint32 unit, rtk_intf_id_t intfId, rtk_tunnel_stat_t stat, uint64 *pValue64)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pValue64), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    osal_memcpy(&cfg.stat, &stat, sizeof(rtk_tunnel_stat_t));
    GETSOCKOPT(RTDRV_TUNNEL_INTFSTAT_GET, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pValue64, &cfg.value64, sizeof(uint64));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intfStat_get */

/* Function Name:
 *      rtk_tunnel_qosProfile_get
 * Description:
 *      Get the QoS profile with the specified index.
 * Input:
 *      unit     - unit id
 *      idx      - index of QoS profile
 * Output:
 *      pProfile - pointer to the QoS prifle
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - the module is not initial
 *      RT_ERR_INPUT        - invalid input parameter
 *      RT_ERR_NULL_POINTER - input parameter may be a null pointer
 * Note:
 */
int32
rtk_tunnel_qosProfile_get(uint32 unit, uint32 idx, rtk_tunnel_qosProfile_t *pProfile)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pProfile), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.idx, &idx, sizeof(uint32));
    GETSOCKOPT(RTDRV_TUNNEL_QOSPROFILE_GET, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pProfile, &cfg.profile, sizeof(rtk_tunnel_qosProfile_t));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_qosProfile_get */

/* Function Name:
 *      rtk_tunnel_qosProfile_set
 * Description:
 *      Set the QoS profile with the specified index.
 * Input:
 *      unit     - unit id
 *      idx      - index of QoS profile
 *      pProfile - pointer to the QoS prifle
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - the module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 */
int32
rtk_tunnel_qosProfile_set(uint32 unit, uint32 idx, rtk_tunnel_qosProfile_t profile)
{
    rtdrv_tunnelCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.idx, &idx, sizeof(uint32));
    osal_memcpy(&cfg.profile, &profile, sizeof(rtk_tunnel_qosProfile_t));
    SETSOCKOPT(RTDRV_TUNNEL_QOSPROFILE_SET, &cfg, rtdrv_tunnelCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_tunnel_qosProfile_set */


/* Function Name:
 *      rtk_tunnel_globalCtrl_get
 * Description:
 *      Get the global configuration of the specified control type
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
rtk_tunnel_globalCtrl_get(uint32 unit, rtk_tunnel_globalCtrlType_t type, int32 *pArg)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_tunnel_globalCtrlType_t));
    GETSOCKOPT(RTDRV_TUNNEL_GLOBALCTRL_GET, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pArg, &cfg.arg, sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_globalCtrl_get */

/* Function Name:
 *      rtk_tunnel_globalCtrl_set
 * Description:
 *      Set the global configuration of the specified control type
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
rtk_tunnel_globalCtrl_set(uint32 unit, rtk_tunnel_globalCtrlType_t type, int32 arg)
{
    rtdrv_tunnelCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_tunnel_globalCtrlType_t));
    osal_memcpy(&cfg.arg, &arg, sizeof(int32));
    SETSOCKOPT(RTDRV_TUNNEL_GLOBALCTRL_SET, &cfg, rtdrv_tunnelCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_tunnel_globalCtrl_set */

/* Function Name:
 *      rtk_tunnel_intfCtrl_get
 * Description:
 *      Get the configuration of the specified control type and interface ID
 * Input:
 *      unit   - unit id
 *      intfId - tunnel interface id
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
rtk_tunnel_intfCtrl_get(uint32 unit, rtk_intf_id_t intfId, rtk_tunnel_intfCtrlType_t type, rtk_tunnel_intfCtrlParam_t *pArg)
{
    rtdrv_tunnelCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    osal_memcpy(&cfg.intfType, &type, sizeof(rtk_tunnel_intfCtrlType_t));
    GETSOCKOPT(RTDRV_TUNNEL_INTFCTRL_GET, &cfg, rtdrv_tunnelCfg_t, 1);
    osal_memcpy(pArg, &cfg.intfArg, sizeof(rtk_tunnel_intfCtrlParam_t));

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intfCtrl_get */

/* Function Name:
 *      rtk_tunnel_intfCtrl_set
 * Description:
 *      Set the configuration of the specified control type and interface ID
 * Input:
 *      unit   - unit id
 *      intfId - tunnel interface id
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
rtk_tunnel_intfCtrl_set(uint32 unit, rtk_intf_id_t intfId, rtk_tunnel_intfCtrlType_t type, rtk_tunnel_intfCtrlParam_t arg)
{
    rtdrv_tunnelCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_tunnelCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.intfId, &intfId, sizeof(rtk_intf_id_t));
    osal_memcpy(&cfg.intfType, &type, sizeof(rtk_tunnel_intfCtrlType_t));
    osal_memcpy(&cfg.intfArg, &arg, sizeof(rtk_tunnel_intfCtrlParam_t));
    SETSOCKOPT(RTDRV_TUNNEL_INTFCTRL_SET, &cfg, rtdrv_tunnelCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_tunnel_intfCtrl_set */


