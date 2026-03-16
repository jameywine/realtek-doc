/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 80490 $
 * $Date: 2017-07-11 14:29:37 +0800 (Tue, 11 Jul 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) switch
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <drv/watchdog/watchdog.h>
#include <drv/tc/tc.h>
#include <rtk/switch.h>
#include <rtusr_util.h>
#include <rtdrv/rtdrv_netfilter.h>

int32 rtk_switch_cpuMaxPktLen_get(uint32 unit, rtk_switch_pktDir_t dir, uint32 *pLen)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    switch_cfg.dir = dir;
    GETSOCKOPT(RTDRV_SWITCH_CPU_MAX_PKTLEN_GET, &switch_cfg, rtdrv_switchCfg_t, 1);
    *pLen = switch_cfg.maxLen;

    return RT_ERR_OK;
}

int32 rtk_switch_cpuMaxPktLen_set(uint32 unit, rtk_switch_pktDir_t dir, uint32 len)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    switch_cfg.dir = dir;
    switch_cfg.maxLen = len;
    SETSOCKOPT(RTDRV_SWITCH_CPU_MAX_PKTLEN_SET, &switch_cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_switch_maxPktLenLinkSpeed_get(uint32 unit, rtk_switch_maxPktLen_linkSpeed_t speed, uint32 *pLen)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    switch_cfg.speed = speed;
    GETSOCKOPT(RTDRV_SWITCH_MAX_PKTLEN_LINK_SPEED_GET, &switch_cfg, rtdrv_switchCfg_t, 1);
    *pLen = switch_cfg.maxLen;
    return RT_ERR_OK;
}

int32
rtk_switch_maxPktLenLinkSpeed_set(uint32 unit, rtk_switch_maxPktLen_linkSpeed_t speed, uint32 len)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    switch_cfg.speed = speed;
    switch_cfg.maxLen = len;
    SETSOCKOPT(RTDRV_SWITCH_MAX_PKTLEN_LINK_SPEED_SET, &switch_cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_switch_maxPktLenTagLenCntIncEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_MAX_PKTLEN_TAGLENCNT_GET, &switch_cfg, rtdrv_switchCfg_t, 1);
    *pEnable = switch_cfg.enable;
    return RT_ERR_OK;
}

int32 rtk_switch_maxPktLenTagLenCntIncEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    switch_cfg.enable = enable;
    SETSOCKOPT(RTDRV_SWITCH_MAX_PKTLEN_TAGLENCNT_SET, &switch_cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_switch_snapMode_get(uint32 unit, rtk_snapMode_t *pSnapMode)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_SNAP_MODE_GET, &switch_cfg, rtdrv_switchCfg_t, 1);
    *pSnapMode = switch_cfg.snapMode;
    return RT_ERR_OK;
}

int32 rtk_switch_snapMode_set(uint32 unit, rtk_snapMode_t snapMode)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    switch_cfg.snapMode = snapMode;
    SETSOCKOPT(RTDRV_SWITCH_SNAP_MODE_SET, &switch_cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_switch_deviceInfo_get(uint32 unit, rtk_switch_devInfo_t *pDevInfo)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_DEVICE_INFO_GET, &switch_cfg, rtdrv_switchCfg_t, 1);
    osal_memcpy(pDevInfo, &switch_cfg.devInfo, sizeof(rtk_switch_devInfo_t));
    return RT_ERR_OK;
}
int32 rtk_switch_deviceCapability_print(uint32 unit)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));

    switch_cfg.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_DEVICE_CAPABILITY_PRINT, &switch_cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_switch_chksumFailAction_get(
    uint32                              unit,
    rtk_port_t                          port,
    rtk_switch_chksum_fail_t            failType,
    rtk_action_t                        *pAction)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    switch_cfg.port = port;
    switch_cfg.failType = failType;
    GETSOCKOPT(RTDRV_SWITCH_CHKSUMFAILACTION_GET, &switch_cfg, rtdrv_switchCfgParam_t, 1);
    *pAction = switch_cfg.action;

    return RT_ERR_OK;
} /* end of rtk_switch_chksumFailAction_get */


int32 rtk_switch_chksumFailAction_set(
    uint32                              unit,
    rtk_port_t                          port,
    rtk_switch_chksum_fail_t            failType,
    rtk_action_t                        action)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    switch_cfg.port = port;
    switch_cfg.failType = failType;
    switch_cfg.action = action;
    SETSOCKOPT(RTDRV_SWITCH_CHKSUMFAILACTION_SET, &switch_cfg, rtdrv_switchCfgParam_t, 1);

    return RT_ERR_OK;
} /* end of rtk_switch_chksumFailAction_set */


int32 rtk_switch_recalcCRCEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    switch_cfg.port = port;
    GETSOCKOPT(RTDRV_SWITCH_RECALCCRCENABLE_GET, &switch_cfg, rtdrv_switchCfgParam_t, 1);
    *pEnable = switch_cfg.enable;

    return RT_ERR_OK;
} /* end of rtk_switch_recalcCRCEnable_get */


int32 rtk_switch_recalcCRCEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    switch_cfg.port = port;
    switch_cfg.enable = enable;
    SETSOCKOPT(RTDRV_SWITCH_RECALCCRCENABLE_SET, &switch_cfg, rtdrv_switchCfgParam_t, 1);

    return RT_ERR_OK;
} /* end of rtk_switch_recalcCRCEnable_set */


int32 rtk_switch_mgmtMacAddr_get(uint32 unit, rtk_mac_t *pMac)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_MGMTMACADDR_GET, &switch_info, rtdrv_switchCfgInfo_t, 1);
    osal_memcpy(pMac, &switch_info.mac, sizeof(rtk_mac_t));

    return RT_ERR_OK;
} /* end of rtk_switch_mgmtMacAddr_get */


int32 rtk_switch_mgmtMacAddr_set(uint32 unit, rtk_mac_t *pMac)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    osal_memcpy( &switch_info.mac, pMac, sizeof(rtk_mac_t));
    SETSOCKOPT(RTDRV_SWITCH_MGMTMACADDR_SET, &switch_info, rtdrv_switchCfgInfo_t, 1);

    return RT_ERR_OK;
}  /* end of rtk_switch_mgmtMacAddr_set */


int32 rtk_switch_IPv4Addr_get(uint32 unit, uint32 *pIpAddr)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_IPV4ADDR_GET, &switch_info, rtdrv_switchCfgInfo_t, 1);
    *pIpAddr = switch_info.ipv4Addr;

    return RT_ERR_OK;
} /* end of rtk_switch_IPv4Addr_get */


int32 rtk_switch_IPv4Addr_set(uint32 unit, uint32 ipAddr)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    switch_info.ipv4Addr = ipAddr;
    SETSOCKOPT(RTDRV_SWITCH_IPV4ADDR_SET, &switch_info, rtdrv_switchCfgInfo_t, 1);

    return RT_ERR_OK;
} /* end of rtk_switch_IPv4Addr_set */


int32
rtk_switch_pkt2CpuTypeFormat_get(uint32 unit, rtk_switch_pkt2CpuType_t type,
    rtk_pktFormat_t *pFormat)
{
    rtdrv_switchCfgInfo_t cfg;

    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfgInfo_t));
    cfg.unit = unit;
    cfg.trap_type = type;
    GETSOCKOPT(RTDRV_SWITCH_PKT2CPUTYPEFORMAT_GET, &cfg, rtdrv_switchCfgInfo_t, 1);

    *pFormat = cfg.format;

    return RT_ERR_OK;
}   /* end of rtk_switch_pkt2CpuTypeFormat_get */

int32
rtk_switch_pkt2CpuTypeFormat_set(uint32 unit, rtk_switch_pkt2CpuType_t type,
    rtk_pktFormat_t format)
{
    rtdrv_switchCfgInfo_t cfg;

    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfgInfo_t));
    cfg.unit = unit;
    cfg.trap_type = type;
    cfg.format = format;
    SETSOCKOPT(RTDRV_SWITCH_PKT2CPUTYPEFORMAT_SET, &cfg, rtdrv_switchCfgInfo_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_switch_pkt2CpuTypeFormat_set */

int32 rtk_switch_pppoeIpParseEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_PPPOE_IP_PARSE_ENABLE_GET, &switch_info, rtdrv_switchCfgInfo_t, 1);
    *pEnable = switch_info.enable;

    return RT_ERR_OK;
} /* end of rtk_switch_pppoeIpParseEnable_get */

int32 rtk_switch_pppoeIpParseEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    switch_info.enable = enable;
    SETSOCKOPT(RTDRV_SWITCH_PPPOE_IP_PARSE_ENABLE_SET, &switch_info, rtdrv_switchCfgInfo_t, 1);

    return RT_ERR_OK;
} /* end of rtk_switch_pppoeIpParseEnable_set */

int32 rtk_switch_cpuPktTruncateEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_CPU_PKT_TRUNCATE_EN_GET, &switch_cfg, rtdrv_switchCfgParam_t, 1);
    *pEnable = switch_cfg.enable;

    return RT_ERR_OK;
}

int32 rtk_switch_cpuPktTruncateEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    switch_cfg.enable = enable;
    SETSOCKOPT(RTDRV_SWITCH_CPU_PKT_TRUNCATE_EN_SET, &switch_cfg, rtdrv_switchCfgParam_t, 1);

    return RT_ERR_OK;
}

int32 rtk_switch_cpuPktTruncateLen_get(uint32 unit, uint32 *pLen)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_CPU_PKT_TRUNCATE_LEN_GET, &switch_cfg, rtdrv_switchCfgParam_t, 1);
    *pLen = switch_cfg.maxLen;

    return RT_ERR_OK;
}

int32 rtk_switch_cpuPktTruncateLen_set(uint32 unit, uint32 len)
{
    rtdrv_switchCfgParam_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfgParam_t));
    switch_cfg.unit = unit;
    switch_cfg.maxLen = len;
    SETSOCKOPT(RTDRV_SWITCH_CPU_PKT_TRUNCATE_LEN_SET, &switch_cfg, rtdrv_switchCfgParam_t, 1);

    return RT_ERR_OK;
}

int32
drv_watchdog_enable_get(uint32 unit, uint32 *pEnable)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_WATCHDOG_ENABLE_GET, &switch_info, rtdrv_switchCfgInfo_t, 1);
    *pEnable = switch_info.enable;

    return RT_ERR_OK;
} /* end of drv_watchdog_enable_get */

int32
drv_watchdog_enable_set(uint32 unit, uint32 enable)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    switch_info.enable = enable;
    SETSOCKOPT(RTDRV_SWITCH_WATCHDOG_ENABLE_SET, &switch_info, rtdrv_switchCfgInfo_t, 1);

    return RT_ERR_OK;
} /* end of drv_watchdog_enable_set */

int32
drv_watchdog_kick(uint32 unit)
{
    rtdrv_switchCfgInfo_t switch_info;

    osal_memset(&switch_info, 0, sizeof(rtdrv_switchCfgInfo_t));
    switch_info.unit = unit;
    SETSOCKOPT(RTDRV_SWITCH_WATCHDOG_KICK, &switch_info, rtdrv_switchCfgInfo_t, 1);

    return RT_ERR_OK;
} /* end of drv_watchdog_enable_set */


int32
drv_watchdog_scale_get(uint32 unit, drv_watchdog_scale_t *pScale)
{
    rtdrv_watchdogCfgInfo_t watchdog_info;

    osal_memset(&watchdog_info, 0, sizeof(rtdrv_watchdogCfgInfo_t));
    watchdog_info.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_WATCHDOG_SCALE_GET, &watchdog_info, rtdrv_watchdogCfgInfo_t, 1);
    *pScale = watchdog_info.scale;

    return RT_ERR_OK;
} /* end of drv_watchdog_scale_get */

int32
drv_watchdog_scale_set(uint32 unit, drv_watchdog_scale_t scale)
{
    rtdrv_watchdogCfgInfo_t watchdog_info;

    osal_memset(&watchdog_info, 0, sizeof(rtdrv_watchdogCfgInfo_t));
    watchdog_info.unit = unit;
    watchdog_info.scale = scale;
    SETSOCKOPT(RTDRV_SWITCH_WATCHDOG_SCALE_SET, &watchdog_info, rtdrv_watchdogCfgInfo_t, 1);

    return RT_ERR_OK;
} /* end of drv_watchdog_scale_set */

int32
drv_watchdog_threshold_get(uint32 unit, drv_watchdog_threshold_t *pThreshold)
{
    rtdrv_watchdogCfgInfo_t watchdog_info;

    osal_memset(&watchdog_info, 0, sizeof(rtdrv_watchdogCfgInfo_t));
    watchdog_info.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_WATCHDOG_THRESHOLD_GET, &watchdog_info, rtdrv_watchdogCfgInfo_t, 1);
    *pThreshold = watchdog_info.threshold;

    return RT_ERR_OK;
} /* end of drv_watchdog_scale_set */

int32
drv_watchdog_threshold_set(uint32 unit, drv_watchdog_threshold_t *pThreshold)
{
    rtdrv_watchdogCfgInfo_t watchdog_info;

    osal_memset(&watchdog_info, 0, sizeof(rtdrv_watchdogCfgInfo_t));
    watchdog_info.unit = unit;
    watchdog_info.threshold = *pThreshold;
    SETSOCKOPT(RTDRV_SWITCH_WATCHDOG_THRESHOLD_SET, &watchdog_info, rtdrv_watchdogCfgInfo_t, 1);

    return RT_ERR_OK;
} /* end of drv_watchdog_scale_set */

int32
drv_tc_enable_set(uint32 unit, drv_tc_id_t id, rtk_enable_t enable)
{
    rtdrv_tcCfgInfo_t tc_info;

    osal_memset(&tc_info, 0, sizeof(rtdrv_tcCfgInfo_t));
    tc_info.unit = unit;
    tc_info.id = id;
    tc_info.enable = enable;
    SETSOCKOPT(RTDRV_SWITCH_TC_ENABLE_SET, &tc_info, rtdrv_tcCfgInfo_t, 1);

    return RT_ERR_OK;
}

int32
drv_tc_mode_set(uint32 unit, drv_tc_id_t id, drv_tc_mode_t mode)
{
    rtdrv_tcCfgInfo_t tc_info;

    osal_memset(&tc_info, 0, sizeof(rtdrv_tcCfgInfo_t));
    tc_info.unit = unit;
    tc_info.id = id;
    tc_info.mode = mode;
    SETSOCKOPT(RTDRV_SWITCH_TC_MODE_SET, &tc_info, rtdrv_tcCfgInfo_t, 1);

    return RT_ERR_OK;

}
int32
drv_tc_divFactor_set(uint32 unit, drv_tc_id_t id, uint32 divFactor)
{
    rtdrv_tcCfgInfo_t tc_info;

    osal_memset(&tc_info, 0, sizeof(rtdrv_tcCfgInfo_t));
    tc_info.unit = unit;
    tc_info.id = id;
    tc_info.value = divFactor;
    SETSOCKOPT(RTDRV_SWITCH_TC_DIVFACTOR_SET, &tc_info, rtdrv_tcCfgInfo_t, 1);

    return RT_ERR_OK;

}

int32
drv_tc_dataInitValue_set(uint32 unit, drv_tc_id_t id, uint32 init_value)
{
    rtdrv_tcCfgInfo_t tc_info;

    osal_memset(&tc_info, 0, sizeof(rtdrv_tcCfgInfo_t));
    tc_info.unit = unit;
    tc_info.id = id;
    tc_info.value = init_value;
    SETSOCKOPT(RTDRV_SWITCH_TC_DATAINITVALUE_SET, &tc_info, rtdrv_tcCfgInfo_t, 1);

    return RT_ERR_OK;

}
int32
drv_tc_counterValue_get(uint32 unit, drv_tc_id_t id,uint32 *value)
{
    rtdrv_tcCfgInfo_t tc_info;

    osal_memset(&tc_info, 0, sizeof(rtdrv_tcCfgInfo_t));
    tc_info.unit = unit;
    tc_info.id = id;
    GETSOCKOPT(RTDRV_SWITCH_TC_COUNTERVALUE_GET, &tc_info, rtdrv_tcCfgInfo_t, 1);
    *value = tc_info.value;

    return RT_ERR_OK;

}

int32
rtk_switch_portMaxPktLenLinkSpeed_get(uint32 unit, rtk_port_t port,
    rtk_switch_maxPktLen_linkSpeed_t speed, uint32 *pLen)
{
    rtdrv_switchCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pLen), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.speed, &speed, sizeof(rtk_switch_maxPktLen_linkSpeed_t));
    GETSOCKOPT(RTDRV_SWITCH_PORT_MAX_PKTLEN_LINK_SPEED_GET, &cfg, rtdrv_switchCfg_t, 1);
    osal_memcpy(pLen, &cfg.maxLen, sizeof(uint32));

    return RT_ERR_OK;
}   /* end of rtk_switch_portMaxPktLenLinkSpeed_get */

int32
rtk_switch_portMaxPktLenLinkSpeed_set(uint32 unit, rtk_port_t port,
    rtk_switch_maxPktLen_linkSpeed_t speed, uint32 len)
{
    rtdrv_switchCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.speed, &speed, sizeof(rtk_switch_maxPktLen_linkSpeed_t));
    osal_memcpy(&cfg.maxLen, &len, sizeof(uint32));
    SETSOCKOPT(RTDRV_SWITCH_PORT_MAX_PKTLEN_LINK_SPEED_SET, &cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_switch_portMaxPktLenLinkSpeed_set */

int32
rtk_switch_portMaxPktLenTagLenCntIncEnable_get(uint32 unit, rtk_port_t port,
    rtk_enable_t *pEnable)
{
    rtdrv_switchCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_SWITCH_PORT_MAX_PKTLEN_TAGLENCNT_INCENABLE_GET, &cfg, rtdrv_switchCfg_t, 1);
    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_switch_portMaxPktLenTagLenCntIncEnable_get */

int32
rtk_switch_portMaxPktLenTagLenCntIncEnable_set(uint32 unit, rtk_port_t port,
    rtk_enable_t enable)
{
    rtdrv_switchCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_SWITCH_PORT_MAX_PKTLEN_TAGLENCNT_INCENABLE_SET, &cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_switch_portMaxPktLenTagLenCntIncEnable_set */

int32
rtk_switch_flexTblFmt_get(uint32 unit, rtk_switch_flexTblFmt_t *pFmt)
{
    rtdrv_switchCfgTable_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pFmt), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfgTable_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_SWITCH_FLEXTBLFMT_GET, &cfg, rtdrv_switchCfgTable_t, 1);
    osal_memcpy(pFmt, &cfg.tbl_fmt, sizeof(rtk_switch_flexTblFmt_t));

    return RT_ERR_OK;
}   /* end of rtk_switch_flexTblFmt_get */

int32
rtk_switch_flexTblFmt_set(uint32 unit, rtk_switch_flexTblFmt_t fmt)
{
    rtdrv_switchCfgTable_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfgTable_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.tbl_fmt, &fmt, sizeof(rtk_switch_flexTblFmt_t));
    SETSOCKOPT(RTDRV_SWITCH_FLEXTBLFMT_SET, &cfg, rtdrv_switchCfgTable_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_switch_flexTblFmt_set */

int32
rtk_switch_tagPauseAction_get(uint32 unit, rtk_action_t *action)
{
    rtdrv_switchCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == action), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_SWITCH_TAGPAUSEACTION_GET, &cfg, rtdrv_switchCfg_t, 1);
    osal_memcpy(action, &cfg.action, sizeof(rtk_action_t));

    return RT_ERR_OK;
}   /* end of rtk_switch_tagPauseAction_get */

int32
rtk_switch_tagPauseAction_set(uint32 unit, rtk_action_t action)
{
    rtdrv_switchCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_switchCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.action, &action, sizeof(rtk_action_t));
    SETSOCKOPT(RTDRV_SWITCH_TAGPAUSEACTION_SET, &cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_switch_tagPauseAction_set */

int32 rtk_switch_halfConsecutiveRetryEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));
    switch_cfg.unit = unit;
    GETSOCKOPT(RTDRV_SWITCH_HALF_CONSECUTIVE_RETRY_ENABLE_GET, &switch_cfg, rtdrv_switchCfg_t, 1);
    *pEnable = switch_cfg.enable;

    return RT_ERR_OK;
}   /* end of rtk_switch_halfConsecutiveRetryEnable_get */

int32 rtk_switch_halfConsecutiveRetryEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_switchCfg_t switch_cfg;

    osal_memset(&switch_cfg, 0, sizeof(rtdrv_switchCfg_t));
    switch_cfg.unit = unit;
    switch_cfg.enable = enable;
    SETSOCKOPT(RTDRV_SWITCH_HALF_CONSECUTIVE_RETRY_ENABLE_SET, &switch_cfg, rtdrv_switchCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_switch_halfConsecutiveRetryEnable_get */
