/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 79862 $
 * $Date: 2017-06-20 16:07:27 +0800 (Tue, 20 Jun 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) port
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rtk/port.h>
#include <rtusr_util.h>
#include <rtdrv/rtdrv_netfilter.h>

int32 rtk_port_link_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pStatus)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_LINK_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pStatus = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_linkMedia_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pStatus, rtk_port_media_t *pMedia)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_LINKMEDIA_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pStatus = port_cfg.data;
    *pMedia = port_cfg.media;

    return RT_ERR_OK;
}

int32 rtk_port_speedDuplex_get(uint32 unit, rtk_port_t port, rtk_port_speed_t *pSpeed, rtk_port_duplex_t *pDuplex)
{
    rtdrv_port_speedDuplex_t sd_status;

    osal_memset(&sd_status, 0, sizeof(rtdrv_port_speedDuplex_t));
    sd_status.unit = unit;
    sd_status.port = port;
    GETSOCKOPT(RTDRV_PORT_SPEED_DUPLEX_GET, &sd_status, rtdrv_port_speedDuplex_t, 1);
    *pSpeed = sd_status.speed;
    *pDuplex = sd_status.duplex;

    return RT_ERR_OK;
}

int32 rtk_port_flowctrl_get(uint32 unit, rtk_port_t port, uint32 *pTxStatus, uint32 *pRxStatus)
{
    rtdrv_port_flowctrl_t fc_status;

    osal_memset(&fc_status, 0, sizeof(rtdrv_port_flowctrl_t));
    fc_status.unit = unit;
    fc_status.port = port;
    GETSOCKOPT(RTDRV_PORT_FLOW_CTRL_GET, &fc_status, rtdrv_port_flowctrl_t, 1);
    *pTxStatus = fc_status.tx_status;
    *pRxStatus = fc_status.rx_status;

    return RT_ERR_OK;
}

int32 rtk_port_cpuPortId_get(uint32 unit, rtk_port_t *pPort)
{
    rtdrv_unitCfg_t unit_cfg;

    osal_memset(&unit_cfg, 0, sizeof(rtdrv_unitCfg_t));
    unit_cfg.unit = unit;
    GETSOCKOPT(RTDRV_PORT_CPU_PORT_ID_GET, &unit_cfg, rtdrv_unitCfg_t, 1);
    *pPort = unit_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_phyAutoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_EN_AUTONEGO_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnabled = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_phyAutoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enabled;
    SETSOCKOPT(RTDRV_PORT_EN_AUTONEGO_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_phyAutoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    rtdrv_port_autoNegoAbility_t an_ability;

    osal_memset(&an_ability, 0, sizeof(rtdrv_port_autoNegoAbility_t));
    an_ability.unit = unit;
    an_ability.port = port;
    GETSOCKOPT(RTDRV_PORT_AUTONEGO_ABIL_GET, &an_ability, rtdrv_port_autoNegoAbility_t, 1);
    osal_memcpy(pAbility, &an_ability.ability, sizeof(rtk_port_phy_ability_t));

    return RT_ERR_OK;
}

int32 rtk_port_phyAutoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    rtdrv_port_autoNegoAbility_t an_ability;

    osal_memset(&an_ability, 0, sizeof(rtdrv_port_autoNegoAbility_t));
    an_ability.unit = unit;
    an_ability.port = port;
    osal_memcpy(&an_ability.ability, pAbility, sizeof(rtk_port_phy_ability_t));
    SETSOCKOPT(RTDRV_PORT_AUTONEGO_ABIL_SET, &an_ability, rtdrv_port_autoNegoAbility_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_phyForceModeAbility_get(uint32 unit, rtk_port_t port, rtk_port_speed_t *pSpeed, rtk_port_duplex_t *pDuplex, rtk_enable_t *pFlowControl)
{
    rtdrv_port_forceModeAbility_t fm_ability;

    osal_memset(&fm_ability, 0, sizeof(rtdrv_port_forceModeAbility_t));
    fm_ability.unit = unit;
    fm_ability.port = port;
    GETSOCKOPT(RTDRV_PORT_FORCE_MODE_ABIL_GET, &fm_ability, rtdrv_port_forceModeAbility_t, 1);
    *pSpeed = fm_ability.speed;
    *pDuplex = fm_ability.duplex;
    *pFlowControl = fm_ability.flowctrl;

    return RT_ERR_OK;
}

int32 rtk_port_phyForceModeAbility_set(uint32 unit, rtk_port_t port, rtk_port_speed_t speed, rtk_port_duplex_t duplex, rtk_enable_t flowControl)
{
    rtdrv_port_forceModeAbility_t fm_ability;

    osal_memset(&fm_ability, 0, sizeof(rtdrv_port_forceModeAbility_t));
    fm_ability.unit = unit;
    fm_ability.port = port;
    fm_ability.speed = speed;
    fm_ability.duplex = duplex;
    fm_ability.flowctrl = flowControl;
    SETSOCKOPT(RTDRV_PORT_FORCE_MODE_ABIL_SET, &fm_ability, rtdrv_port_forceModeAbility_t, 1);

    return RT_ERR_OK;
}

int32
rtk_port_phyMasterSlave_get(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   *pMasterSlaveCfg,
    rtk_port_masterSlave_t   *pMasterSlaveActual)
{
    rtdrv_port_masterSlave_t masterSlave_cfg;

    osal_memset(&masterSlave_cfg, 0, sizeof(rtdrv_port_masterSlave_t));
    masterSlave_cfg.unit = unit;
    masterSlave_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_MASTER_SLAVE_GET, &masterSlave_cfg, rtdrv_port_masterSlave_t, 1);
    *pMasterSlaveCfg = masterSlave_cfg.masterSlaveCfg;
    *pMasterSlaveActual = masterSlave_cfg.masterSlaveActual;

    return RT_ERR_OK;
}

int32
rtk_port_phyMasterSlave_set(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   masterSlaveCfg)
{
    rtdrv_port_masterSlave_t masterSlave_cfg;

    osal_memset(&masterSlave_cfg, 0, sizeof(rtdrv_port_masterSlave_t));
    masterSlave_cfg.unit = unit;
    masterSlave_cfg.port = port;
    masterSlave_cfg.masterSlaveCfg = masterSlaveCfg;
    SETSOCKOPT(RTDRV_PORT_MASTER_SLAVE_SET, &masterSlave_cfg, rtdrv_port_masterSlave_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_phyReg_get(uint32 unit, rtk_port_t port, uint32 page, rtk_port_phy_reg_t reg, uint32 *pData)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.port = port;
    phy_data.page = page;
    phy_data.reg = reg;
    GETSOCKOPT(RTDRV_PORT_PHY_REG_GET, &phy_data, rtdrv_port_phyReg_t, 1);
    *pData = phy_data.data;

    return RT_ERR_OK;
}

int32 rtk_port_phyReg_set(uint32 unit, rtk_port_t port, uint32 page, rtk_port_phy_reg_t reg, uint32 data)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.port = port;
    phy_data.page = page;
    phy_data.reg = reg;
    phy_data.data = data;
    SETSOCKOPT(RTDRV_PORT_PHY_REG_SET, &phy_data, rtdrv_port_phyReg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_port_phyExtParkPageReg_get(
    uint32              unit,
    rtk_port_t          port,
    uint32              mainPage,
    uint32              extPage,
    uint32              parkPage,
    rtk_port_phy_reg_t  reg,
    uint32              *pData)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.port = port;
    phy_data.page = mainPage;
    phy_data.extPage = extPage;
    phy_data.parkPage = parkPage;
    phy_data.reg = reg;
    GETSOCKOPT(RTDRV_PORT_PHY_EXT_PARK_PAGE_REG_GET, &phy_data, rtdrv_port_phyReg_t, 1);
    *pData = phy_data.data;

    return RT_ERR_OK;
}    /* end of rtk_port_phyExtParkPageReg_get */

int32
rtk_port_phyExtParkPageReg_set(
    uint32              unit,
    rtk_port_t          port,
    uint32              mainPage,
    uint32              extPage,
    uint32              parkPage,
    rtk_port_phy_reg_t  reg,
    uint32              data)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.port = port;
    phy_data.page = mainPage;
    phy_data.extPage = extPage;
    phy_data.parkPage = parkPage;
    phy_data.reg = reg;
    phy_data.data = data;
    SETSOCKOPT(RTDRV_PORT_PHY_EXT_PARK_PAGE_REG_SET, &phy_data, rtdrv_port_phyReg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_port_phyExtParkPageReg_set */

int32
rtk_port_phymaskExtParkPageReg_set(
    uint32              unit,
    rtk_portmask_t      *pPortmask,
    uint32              mainPage,
    uint32              extPage,
    uint32              parkPage,
    rtk_port_phy_reg_t  reg,
    uint32              data)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.portmask = *pPortmask;
    phy_data.page = mainPage;
    phy_data.extPage = extPage;
    phy_data.parkPage = parkPage;
    phy_data.reg = reg;
    phy_data.data = data;
    SETSOCKOPT(RTDRV_PORT_PHYMASK_EXT_PARK_PAGE_REG_SET, &phy_data, rtdrv_port_phyReg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_port_phymaskExtParkPageReg_set */

int32
rtk_port_phyMmdReg_get(
    uint32              unit,
    rtk_port_t          port,
    uint32              mmdAddr,
    uint32              mmdReg,
    uint32              *pData)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.port = port;
    phy_data.mmdAddr= mmdAddr;
    phy_data.reg = mmdReg;
    GETSOCKOPT(RTDRV_PORT_PHY_MMD_REG_GET, &phy_data, rtdrv_port_phyReg_t, 1);
    *pData = phy_data.data;

    return RT_ERR_OK;
}    /* end of rtk_port_phyMmdReg_get */

int32
rtk_port_phyMmdReg_set(
    uint32              unit,
    rtk_port_t          port,
    uint32              mmdAddr,
    uint32              mmdReg,
    uint32              data)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.port = port;
    phy_data.mmdAddr= mmdAddr;
    phy_data.reg = mmdReg;
    phy_data.data = data;
    SETSOCKOPT(RTDRV_PORT_PHY_MMD_REG_SET, &phy_data, rtdrv_port_phyReg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_port_phyMmdReg_set */

int32
rtk_port_phymaskMmdReg_set(
    uint32              unit,
    rtk_portmask_t      *pPortmask,
    uint32              mmdAddr,
    uint32              mmdReg,
    uint32              data)
{
    rtdrv_port_phyReg_t phy_data;

    osal_memset(&phy_data, 0, sizeof(rtdrv_port_phyReg_t));
    phy_data.unit = unit;
    phy_data.portmask = *pPortmask;
    phy_data.mmdAddr= mmdAddr;
    phy_data.reg = mmdReg;
    phy_data.data = data;
    SETSOCKOPT(RTDRV_PORT_PHYMASK_MMD_REG_SET, &phy_data, rtdrv_port_phyReg_t, 1);

    return RT_ERR_OK;
}    /* end of rtk_port_phymaskMmdReg_set */

int32 rtk_port_isolation_get(uint32 unit, rtk_port_t port, rtk_portmask_t *pPortmask)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_ISOLATION_GET, &port_cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pPortmask, &port_cfg.portmask, sizeof(rtk_portmask_t));

    return RT_ERR_OK;
}

int32 rtk_port_isolation_set(uint32 unit, rtk_port_t port, rtk_portmask_t *pPortmask)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    osal_memcpy(&port_cfg.portmask, pPortmask, sizeof(rtk_portmask_t));
    SETSOCKOPT(RTDRV_PORT_ISOLATION_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_isolationExt_get(uint32 unit, uint32 srcUnit, rtk_port_t srcPort, rtk_portmask_t *pPortmask)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.srcUnit = srcUnit;
    port_cfg.port = srcPort;
    GETSOCKOPT(RTDRV_PORT_ISOLATIONEXT_GET, &port_cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pPortmask, &port_cfg.portmask, sizeof(rtk_portmask_t));

    return RT_ERR_OK;
}

int32 rtk_port_isolationExt_set(uint32 unit, uint32 srcUnit, rtk_port_t srcPort, rtk_portmask_t *pPortmask)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.srcUnit = srcUnit;
    port_cfg.port = srcPort;
    osal_memcpy(&port_cfg.portmask, pPortmask, sizeof(rtk_portmask_t));
    SETSOCKOPT(RTDRV_PORT_ISOLATIONEXT_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_isolation_add(uint32 unit, rtk_port_t port, rtk_port_t iso_port)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.targetPort = iso_port;
    SETSOCKOPT(RTDRV_PORT_ISOLATION_ADD, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
} /* end of rtk_port_isolation_add */


int32 rtk_port_isolation_del(uint32 unit, rtk_port_t port, rtk_port_t iso_port)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.targetPort = iso_port;
    SETSOCKOPT(RTDRV_PORT_ISOLATION_DEL, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
} /* end of rtk_port_isolation_del */

int32 rtk_port_isolationRestrictRoute_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    GETSOCKOPT(RTDRV_PORT_ISOLATION_RESTRICT_ROUTE_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnable = port_cfg.data;

    return RT_ERR_OK;
} /* end of rtk_port_isolation_del */

int32 rtk_port_isolationRestrictRoute_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.data = enable;
    SETSOCKOPT(RTDRV_PORT_ISOLATION_RESTRICT_ROUTE_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
} /* end of rtk_port_isolation_del */

int32 rtk_port_adminEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_EN_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnabled = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_adminEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enabled;
    SETSOCKOPT(RTDRV_PORT_EN_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_txEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_TX_EN_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnabled = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_txEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enabled;
    SETSOCKOPT(RTDRV_PORT_TX_EN_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_rxEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_RX_EN_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnabled = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_rxEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enabled;
    SETSOCKOPT(RTDRV_PORT_RX_EN_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_backpressureEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_BACK_PRESSURE_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnabled = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_backpressureEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enabled)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enabled;
    SETSOCKOPT(RTDRV_PORT_BACK_PRESSURE_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_phyComboPortMedia_get(uint32 unit, rtk_port_t port, rtk_port_media_t *pMedia)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_PHY_MEDIA_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pMedia = port_cfg.media;

    return RT_ERR_OK;
}

int32 rtk_port_phyComboPortMedia_set(uint32 unit, rtk_port_t port, rtk_port_media_t media)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.media = media;
    SETSOCKOPT(RTDRV_PORT_PHY_MEDIA_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}


int32 rtk_port_greenEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_GREEN_ENABLE_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnable = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_greenEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enable;
    SETSOCKOPT(RTDRV_PORT_GREEN_ENABLE_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_gigaLiteEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_GIGA_LITE_ENABLE_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnable = port_cfg.data;

    return RT_ERR_OK;
}

int32 rtk_port_gigaLiteEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enable;
    SETSOCKOPT(RTDRV_PORT_GIGA_LITE_ENABLE_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_port_phyCrossOverMode_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t *pMode)
{
    rtdrv_portCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_portCfg_t));
    config.unit = unit;
    config.port = port;
    GETSOCKOPT(RTDRV_PORT_PHY_CROSSOVERMODE_GET, &config, rtdrv_portCfg_t, 1);
    *pMode = config.data;

    return RT_ERR_OK;
} /* end of rtk_port_phyCrossOverMode_get */

int32
rtk_port_phyCrossOverMode_set(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t mode)
{
    rtdrv_portCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_portCfg_t));
    config.unit = unit;
    config.port = port;
    config.data = mode;
    SETSOCKOPT(RTDRV_PORT_PHY_CROSSOVERMODE_SET, &config, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
} /* end of rtk_port_phyCrossOverMode_set */

int32
rtk_port_phyCrossOverStatus_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_status_t *pStatus)
{
    rtdrv_portCfg_t config;

    osal_memset(&config, 0, sizeof(rtdrv_portCfg_t));
    config.unit = unit;
    config.port = port;
    GETSOCKOPT(RTDRV_PORT_PHY_CROSSOVERSTATUS_GET, &config, rtdrv_portCfg_t, 1);
    *pStatus = config.data;

    return RT_ERR_OK;
} /* end of rtk_port_phyCrossOverStatus_get */


int32 rtk_port_phyComboPortFiberMedia_get(uint32 unit, rtk_port_t port, rtk_port_fiber_media_t *pMedia)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_PHY_FIBER_MEDIA_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pMedia = port_cfg.fiber_media;

    return RT_ERR_OK;
} /* end of rtk_port_phyComboPortFiberMedia_get */

int32 rtk_port_phyComboPortFiberMedia_set(uint32 unit, rtk_port_t port, rtk_port_fiber_media_t media)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.fiber_media = media;
    SETSOCKOPT(RTDRV_PORT_PHY_FIBER_MEDIA_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
} /* end of rtk_port_phyComboPortFiberMedia_set */

int32 rtk_port_linkDownPowerSavingEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_LINKDOWN_POWERSAVING_ENABLE_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnable = port_cfg.data;

    return RT_ERR_OK;
} /* end of rtk_port_linkDownPowerSavingEnable_get */

int32 rtk_port_linkDownPowerSavingEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enable;
    SETSOCKOPT(RTDRV_PORT_LINKDOWN_POWERSAVING_ENABLE_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
} /* end of rtk_port_linkDownPowerSavingEnable_set */

int32 rtk_port_vlanBasedIsolationEntry_get(uint32 unit, uint32 index, rtk_port_vlanIsolationEntry_t* pEntry)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.index = index;
    GETSOCKOPT(RTDRV_PORT_VLAN_ISOLATION_GET, &port_cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pEntry, &(port_cfg.vlanIsoEntry), sizeof(rtk_port_vlanIsolationEntry_t));

    return RT_ERR_OK;
}

int32 rtk_port_vlanBasedIsolationEntry_set(uint32 unit, uint32 index, rtk_port_vlanIsolationEntry_t* pEntry)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.index = index;
    osal_memcpy(&(port_cfg.vlanIsoEntry), pEntry, sizeof(rtk_port_vlanIsolationEntry_t));
    SETSOCKOPT(RTDRV_PORT_VLAN_ISOLATION_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32 rtk_port_vlanBasedIsolation_vlanSource_get(uint32 unit, rtk_port_vlanIsolationSrc_t *pVlanSrc)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    GETSOCKOPT(RTDRV_PORT_VLAN_ISOLATION_VLANSOURCE_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pVlanSrc = port_cfg.vlanIsoSrc;

    return RT_ERR_OK;
}

int32 rtk_port_vlanBasedIsolation_vlanSource_set(uint32 unit, rtk_port_vlanIsolationSrc_t vlanSrc)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.vlanIsoSrc = vlanSrc;
    SETSOCKOPT(RTDRV_PORT_VLAN_ISOLATION_VLANSOURCE_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_port_vlanBasedIsolationEgrBypass_get(uint32 unit, rtk_port_t port,rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t port_cfg;

    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    GETSOCKOPT(RTDRV_PORT_VLAN_ISOLATION_EGRBYPASS_GET, &port_cfg, rtdrv_portCfg_t, 1);
    *pEnable = port_cfg.data;

    return RT_ERR_OK;
}

int32
rtk_port_vlanBasedIsolationEgrBypass_set(uint32 unit, rtk_port_t port,rtk_enable_t enable)
{
    rtdrv_portCfg_t port_cfg;

    osal_memset(&port_cfg, 0, sizeof(rtdrv_portCfg_t));
    port_cfg.unit = unit;
    port_cfg.port = port;
    port_cfg.data = enable;
    SETSOCKOPT(RTDRV_PORT_VLAN_ISOLATION_EGRBYPASS_SET, &port_cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_port_fiberDownSpeedEnable_get(uint32 unit, rtk_port_t port,
    rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_PORT_FIBERDOWNSPEEDENABLE_GET, &cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pEnable, &cfg.data, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_port_fiberDownSpeedEnable_get */

int32
rtk_port_fiberDownSpeedEnable_set(uint32 unit, rtk_port_t port,
    rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.data, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_FIBERDOWNSPEEDENABLE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_fiberDownSpeedEnable_set */

int32
rtk_port_downSpeedEnable_get(uint32 unit, rtk_port_t port,
    rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_PORT_DOWNSPEEDENABLE_GET, &cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pEnable, &cfg.data, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_port_fiberDownSpeedEnable_get */

int32
rtk_port_downSpeedEnable_set(uint32 unit, rtk_port_t port,
    rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.data, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_DOWNSPEEDENABLE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_fiberDownSpeedEnable_set */


int32
rtk_port_fiberNwayForceLinkEnable_get(uint32 unit, rtk_port_t port,
    rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_PORT_FIBERNWAYFORCELINKENABLE_GET, &cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pEnable, &cfg.data, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_port_fiberNwayForceLinkEnable_get */

int32
rtk_port_fiberNwayForceLinkEnable_set(uint32 unit, rtk_port_t port,
    rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.data, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_FIBERNWAYFORCELINKENABLE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_fiberNwayForceLinkEnable_set */

/* Function Name:
 *      rtk_port_fiberUnidirEnable_get
 * Description:
 *      Get fiber unidirection enable status of the specific port
 * Input:
 *      unit           - unit id
 *      port           - port id
 * Output:
 *      pEnable       - - pointer to the enable status of mac local loopback
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
rtk_port_fiberUnidirEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_PORT_FIBERUNIDIRENABLE_GET, &cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_port_fiberUnidirEnable_get */

/* Function Name:
 *      rtk_port_fiberUnidirEnable_set
 * Description:
 *      Set fiber unidirection enable status of the specific port
 * Input:
 *      unit           - unit id
 *      port           - port id
 *      enable        - enable status of mac local loopback
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
rtk_port_fiberUnidirEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_FIBERUNIDIRENABLE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_fiberUnidirEnable_set */

int32
rtk_port_phyLoopBackEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_PORT_PHYLOOPBACKENABLE_GET, &cfg, rtdrv_portCfg_t, 1);

    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_port_phyLoopBackEnable_set */

int32
rtk_port_phyLoopBackEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.data, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_PHYLOOPBACKENABLE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_phyLoopBackEnable_set */


int32
rtk_port_fiberOAMLoopBackEnable_set(uint32 unit, rtk_port_t port,
    rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.data, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_FIBEROAMLOOPBACKENABLE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_fiberOAMLoopBackEnable_set */

int32
rtk_port_10gMedia_set(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t media)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.media_10g, &media, sizeof(rtk_port_10gMedia_t));
    SETSOCKOPT(RTDRV_PORT_10GMEDIA_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_10gMedia_set */

int32
rtk_port_10gMedia_get(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t *media)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == media), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_PORT_10GMEDIA_GET, &cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(media, &cfg.media_10g, sizeof(rtk_port_10gMedia_t));

    return RT_ERR_OK;
}   /* end of rtk_port_10gMedia_get */

int32
rtk_port_10gSds_restart(uint32 unit, rtk_port_t port)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    SETSOCKOPT(RTDRV_PORT_10GSDS_RESTART, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_10gSds_restart */

int32
rtk_port_10g_init(uint32 unit, rtk_port_t port)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    SETSOCKOPT(RTDRV_PORT_10G_INIT, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_10g_init */

int32
rtk_port_phyFiberTxDis_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_PHYFIBERTXDIS_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_phyFiberTxDis_set */

int32
rtk_port_phyFiberTxDisPin_set(uint32 unit, rtk_port_t port, uint32 data)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.data, &data, sizeof(uint32));
    SETSOCKOPT(RTDRV_PORT_PHYFIBERTXDISPIN_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_phyFiberTxDis_set */

int32
rtk_port_fiberRxEnable_get(uint32 unit, rtk_port_t port,
    rtk_enable_t *pEnable)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_PORT_FIBERRXENABLE_GET, &cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_port_fiberRxEnable_get */

int32
rtk_port_fiberRxEnable_set(uint32 unit, rtk_port_t port,
    rtk_enable_t enable)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_PORT_FIBERRXENABLE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_fiberRxEnable_set */


int32
rtk_port_phyIeeeTestMode_set(uint32 unit, rtk_port_t port, rtk_port_phyTestMode_t *pTestMode)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.testMode, pTestMode, sizeof(rtk_port_phyTestMode_t));
    SETSOCKOPT(RTDRV_PORT_PHYIEEETESTMODE_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}


int32
rtk_port_phyPolar_get(uint32 unit, rtk_port_t port, rtk_port_phyPolarCtrl_t *pPolarCtrl)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.polarCtrl, pPolarCtrl, sizeof(rtk_port_phyPolarCtrl_t));
    GETSOCKOPT(RTDRV_PORT_PHYPOLAR_GET, &cfg, rtdrv_portCfg_t, 1);
    osal_memcpy(pPolarCtrl, &cfg.polarCtrl, sizeof(rtk_port_phyPolarCtrl_t));

    return RT_ERR_OK;
}


int32
rtk_port_phyPolar_set(uint32 unit, rtk_port_t port, rtk_port_phyPolarCtrl_t *pPolarCtrl)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.polarCtrl, pPolarCtrl, sizeof(rtk_port_phyPolarCtrl_t));
    SETSOCKOPT(RTDRV_PORT_PHYPOLAR_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_port_phyEyeMonitor_start(uint32 unit, rtk_port_t port, uint32 frameNum)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(int32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.data, &frameNum, sizeof(uint32));
    SETSOCKOPT(RTDRV_PORT_PHYEYEMONITOR_START, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_phyEyeMonitor_start */

int32
rtk_port_imageFlash_load(uint32 unit, rtk_port_t port, uint32 size, uint8 *image)
{
    rtdrv_portCfg_t cfg;

    /* function body */
    memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    memcpy(&cfg.unit, &unit, sizeof(int32));
    memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    memcpy(&cfg.image_size, &size, sizeof(uint32));
    memcpy(&cfg.image, image, sizeof(uint8) * RTK_PORT_FLASHIMG_SIZE);
    SETSOCKOPT(RTDRV_PORT_IMAGEFLASH_LOAD, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}

int32
rtk_port_phySds_get(uint32 unit, rtk_port_t port, rtk_sdsCfg_t *sdsCfg)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == sdsCfg), RT_ERR_NULL_POINTER);

    /* function body */
    memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    memcpy(&cfg.unit, &unit, sizeof(uint32));
    memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    memcpy(&cfg.sdsCfg, sdsCfg, sizeof(rtk_sdsCfg_t));
    GETSOCKOPT(RTDRV_PORT_PHYSDS_GET, &cfg, rtdrv_portCfg_t, 1);
    memcpy(sdsCfg, &cfg.sdsCfg, sizeof(rtk_sdsCfg_t));

    return RT_ERR_OK;
}   /* end of rtk_port_phySds_get */

int32
rtk_port_phySds_set(uint32 unit, rtk_port_t port, rtk_sdsCfg_t *sdsCfg)
{
    rtdrv_portCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == sdsCfg), RT_ERR_NULL_POINTER);

    /* function body */
    memset(&cfg, 0, sizeof(rtdrv_portCfg_t));
    memcpy(&cfg.unit, &unit, sizeof(uint32));
    memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    memcpy(&cfg.sdsCfg, sdsCfg, sizeof(rtk_sdsCfg_t));
    SETSOCKOPT(RTDRV_PORT_PHYSDS_SET, &cfg, rtdrv_portCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_port_phySds_set */
