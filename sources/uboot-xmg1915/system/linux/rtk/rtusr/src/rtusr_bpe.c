/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
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
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) BPE
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
rtk_bpe_init(uint32 unit)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    SETSOCKOPT(RTDRV_BPE_INIT, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_init */

int32
rtk_bpe_portFwdMode_get(uint32 unit, rtk_port_t port,
    rtk_bpe_fwdMode_t *pMode)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pMode), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_BPE_PORTFWDMODE_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pMode, &cfg.mode, sizeof(rtk_bpe_fwdMode_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_portFwdMode_get */

int32
rtk_bpe_portFwdMode_set(uint32 unit, rtk_port_t port, rtk_bpe_fwdMode_t mode)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.mode, &mode, sizeof(rtk_bpe_fwdMode_t));
    SETSOCKOPT(RTDRV_BPE_PORTFWDMODE_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_portFwdMode_set */

int32
rtk_bpe_portEcidNameSpaceGroupId_get(uint32 unit, rtk_port_t port,
    uint32 *pGroupId)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pGroupId), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_BPE_PORTECIDNAMESPACEGROUPID_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pGroupId, &cfg.groupId, sizeof(uint32));

    return RT_ERR_OK;
}   /* end of rtk_bpe_portEcidNameSpaceGroupId_get */

int32
rtk_bpe_portEcidNameSpaceGroupId_set(uint32 unit, rtk_port_t port,
    uint32 groupId)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.groupId, &groupId, sizeof(uint32));
    SETSOCKOPT(RTDRV_BPE_PORTECIDNAMESPACEGROUPID_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_portEcidNameSpaceGroupId_set */

int32
rtk_bpe_portPcid_get(uint32 unit, rtk_port_t port,
    rtk_bpe_pcidCfgType_t type, rtk_bpe_ecid_t *pPcid)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pPcid), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_bpe_pcidCfgType_t));
    GETSOCKOPT(RTDRV_BPE_PORTPCID_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pPcid, &cfg.pcid, sizeof(rtk_bpe_ecid_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_portPcid_get */

int32
rtk_bpe_portPcid_set(uint32 unit, rtk_port_t port,
    rtk_bpe_pcidCfgType_t type, rtk_bpe_ecid_t pcid)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_bpe_pcidCfgType_t));
    osal_memcpy(&cfg.pcid, &pcid, sizeof(rtk_bpe_ecid_t));
    SETSOCKOPT(RTDRV_BPE_PORTPCID_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_portPcid_set */


int32
rtk_bpe_portPcidAct_get(uint32 unit, rtk_port_t port, rtk_action_t *pAction)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pAction), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_BPE_PORTPCIDACT_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pAction, &cfg.action, sizeof(rtk_action_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_portPcidAct_get */

int32
rtk_bpe_portPcidAct_set(uint32 unit, rtk_port_t port, rtk_action_t action)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.action, &action, sizeof(rtk_action_t));
    SETSOCKOPT(RTDRV_BPE_PORTPCIDACT_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_portPcidAct_set */

int32
rtk_bpe_portEgrTagSts_get(uint32 unit, rtk_port_t port,
    rtk_bpe_tagSts_t *pStatus)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pStatus), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_BPE_PORTEGRTAGSTS_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pStatus, &cfg.status, sizeof(rtk_bpe_tagSts_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_portEgrTagSts_get */

int32
rtk_bpe_portEgrTagSts_set(uint32 unit, rtk_port_t port,
    rtk_bpe_tagSts_t status)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.status, &status, sizeof(rtk_bpe_tagSts_t));
    SETSOCKOPT(RTDRV_BPE_PORTEGRTAGSTS_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_portEgrTagSts_set */

int32
rtk_bpe_portEgrVlanTagSts_get(uint32 unit, rtk_port_t port,
    rtk_bpe_vlanTagSts_t *pVlan_status)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pVlan_status), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    GETSOCKOPT(RTDRV_BPE_PORTEGRVLANTAGSTS_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pVlan_status, &cfg.vlan_status, sizeof(rtk_bpe_vlanTagSts_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_portEgrVlanTagSts_get */

int32
rtk_bpe_portEgrVlanTagSts_set(uint32 unit, rtk_port_t port,
    rtk_bpe_vlanTagSts_t vlan_status)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.vlan_status, &vlan_status, sizeof(rtk_bpe_vlanTagSts_t));
    SETSOCKOPT(RTDRV_BPE_PORTEGRVLANTAGSTS_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_portEgrVlanTagSts_set */


int32
rtk_bpe_pvidEntry_add(uint32 unit, rtk_bpe_pvidEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.pvid_entry, pEntry, sizeof(rtk_bpe_pvidEntry_t));
    SETSOCKOPT(RTDRV_BPE_PVIDENTRY_ADD, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_pvidEntry_add */

int32
rtk_bpe_pvidEntry_del(uint32 unit, rtk_bpe_pvidEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.pvid_entry, pEntry, sizeof(rtk_bpe_pvidEntry_t));
    SETSOCKOPT(RTDRV_BPE_PVIDENTRY_DEL, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_pvidEntry_del */

int32
rtk_bpe_pvidEntry_get(uint32 unit, rtk_bpe_pvidEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.pvid_entry, pEntry, sizeof(rtk_bpe_pvidEntry_t));
    GETSOCKOPT(RTDRV_BPE_PVIDENTRY_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pEntry, &cfg.pvid_entry, sizeof(rtk_bpe_pvidEntry_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_pvidEntry_get */

int32
rtk_bpe_pvidEntryNextValid_get(uint32 unit, int32 *pScan_idx,
    rtk_bpe_pvidEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pScan_idx), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.scan_idx, pScan_idx, sizeof(int32));
    GETSOCKOPT(RTDRV_BPE_PVIDENTRYNEXTVALID_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pScan_idx, &cfg.scan_idx, sizeof(int32));
    osal_memcpy(pEntry, &cfg.pvid_entry, sizeof(rtk_bpe_pvidEntry_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_pvidEntryNextValid_get */

int32
rtk_bpe_fwdEntry_add(uint32 unit, rtk_bpe_fwdEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.fwd_entry, pEntry, sizeof(rtk_bpe_fwdEntry_t));
    SETSOCKOPT(RTDRV_BPE_FWDENTRY_ADD, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_fwdEntry_add */

int32
rtk_bpe_fwdEntry_del(uint32 unit, rtk_bpe_fwdEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.fwd_entry, pEntry, sizeof(rtk_bpe_fwdEntry_t));
    SETSOCKOPT(RTDRV_BPE_FWDENTRY_DEL, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_fwdEntry_del */

int32
rtk_bpe_fwdEntry_get(uint32 unit, rtk_bpe_fwdEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.fwd_entry, pEntry, sizeof(rtk_bpe_fwdEntry_t));
    GETSOCKOPT(RTDRV_BPE_FWDENTRY_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pEntry, &cfg.fwd_entry, sizeof(rtk_bpe_fwdEntry_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_fwdEntry_get */

int32
rtk_bpe_fwdEntryNextValid_get(uint32 unit, int32 *pScan_idx,
    rtk_bpe_fwdEntry_t *pEntry)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pScan_idx), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((NULL == pEntry), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.scan_idx, pScan_idx, sizeof(int32));
    GETSOCKOPT(RTDRV_BPE_FWDENTRYNEXTVALID_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pScan_idx, &cfg.scan_idx, sizeof(int32));
    osal_memcpy(pEntry, &cfg.fwd_entry, sizeof(rtk_bpe_fwdEntry_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_fwdEntryNextValid_get */

int32
rtk_bpe_globalCtrl_get(uint32 unit, rtk_bpe_globalCtrlType_t type,
    int32 *pArg)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_bpe_globalCtrlType_t));
    GETSOCKOPT(RTDRV_BPE_GLOBALCTRL_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pArg, &cfg.arg, sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_bpe_globalCtrl_get */

int32
rtk_bpe_globalCtrl_set(uint32 unit, rtk_bpe_globalCtrlType_t type, int32 arg)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_bpe_globalCtrlType_t));
    osal_memcpy(&cfg.arg, &arg, sizeof(int32));
    SETSOCKOPT(RTDRV_BPE_GLOBALCTRL_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_globalCtrl_set */

int32
rtk_bpe_portCtrl_get(uint32 unit, rtk_port_t port,
    rtk_bpe_portCtrlType_t type, int32 *pArg)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pArg), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_bpe_portCtrlType_t));
    GETSOCKOPT(RTDRV_BPE_PORTCTRL_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pArg, &cfg.arg, sizeof(int32));

    return RT_ERR_OK;
}   /* end of rtk_bpe_portCtrl_get */

int32
rtk_bpe_portCtrl_set(uint32 unit, rtk_port_t port,
    rtk_bpe_portCtrlType_t type, int32 arg)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.port, &port, sizeof(rtk_port_t));
    osal_memcpy(&cfg.type, &type, sizeof(rtk_bpe_portCtrlType_t));
    osal_memcpy(&cfg.arg, &arg, sizeof(int32));
    SETSOCKOPT(RTDRV_BPE_PORTCTRL_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_portCtrl_set */

int32
rtk_bpe_priRemarking_get(uint32 unit, rtk_bpe_priRmkSrc_t src,
    rtk_bpe_priRmkVal_t val, rtk_pri_t *pPri)
{
    rtdrv_bpeCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pPri), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.src, &src, sizeof(rtk_bpe_priRmkSrc_t));
    osal_memcpy(&cfg.val, &val, sizeof(rtk_bpe_priRmkVal_t));
    GETSOCKOPT(RTDRV_BPE_PRIREMARKING_GET, &cfg, rtdrv_bpeCfg_t, 1);
    osal_memcpy(pPri, &cfg.pri, sizeof(rtk_pri_t));

    return RT_ERR_OK;
}   /* end of rtk_bpe_priRemarking_get */

int32
rtk_bpe_priRemarking_set(uint32 unit, rtk_bpe_priRmkSrc_t src,
    rtk_bpe_priRmkVal_t val, rtk_pri_t pri)
{
    rtdrv_bpeCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(rtdrv_bpeCfg_t));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.src, &src, sizeof(rtk_bpe_priRmkSrc_t));
    osal_memcpy(&cfg.val, &val, sizeof(rtk_bpe_priRmkVal_t));
    osal_memcpy(&cfg.pri, &pri, sizeof(rtk_pri_t));
    SETSOCKOPT(RTDRV_BPE_PRIREMARKING_SET, &cfg, rtdrv_bpeCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_bpe_priRemarking_set */

