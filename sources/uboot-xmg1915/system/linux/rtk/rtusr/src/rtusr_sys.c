/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 75072 $
 * $Date: 2017-01-05 20:13:38 +0800 (Thu, 05 Jan 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) l2 address
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
hwp_info_show(uint32 unit)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = unit;
    GETSOCKOPT(RTDRV_SYS_HWP_DUMP_INFO, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}

int32
hwp_parsedInfo_show(uint32 unit)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = unit;
    GETSOCKOPT(RTDRV_SYS_HWP_DUMP_PARSED, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}

int32
hwp_unit_show(uint32 unit)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = unit;
    GETSOCKOPT(RTDRV_SYS_HWP_DUMP_UNIT, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtstk_show_dc(uint32 unit)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = unit;
    GETSOCKOPT(RTDRV_SYS_RTSTK_DC_SHOW, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtstk_show_dcbox(int indent, void *box, void *uplink_PortConn)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = 0;
    GETSOCKOPT(RTDRV_SYS_RTSTK_DCBOX_SHOW, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtstk_show_ta(uint32 unit)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = unit;
    GETSOCKOPT(RTDRV_SYS_RTSTK_TA_SHOW, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtstk_show_cfg(uint32 unit)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = unit;
    GETSOCKOPT(RTDRV_SYS_RTSTK_CFG_SHOW, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtstk_show_port(uint32 unit)
{
    rtdrv_sysCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_sysCfg_t));
    config.unit = unit;
    GETSOCKOPT(RTDRV_SYS_RTSTK_PORT_SHOW, &config, rtdrv_sysCfg_t, 1);

    return RT_ERR_OK;
}


