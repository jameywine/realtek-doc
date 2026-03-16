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
 * $Revision: 81463 $
 * $Date: 2017-08-17 16:56:09 +0800 (Thu, 17 Aug 2017) $
 *
 * Purpose : Realtek Switch SDK Rtdrv Netfilter Module.
 *
 * Feature : Realtek Switch SDK Rtdrv Netfilter Module
 *
 */

/*
 * Include Files
 */
#include <linux/version.h>

#include <asm/uaccess.h>
#include <linux/netfilter.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
#include <linux/module.h>
#endif
#include <common/rt_autoconf.h>
#include <common/rt_error.h>
#include <common/debug/mem.h>
#include <osal/print.h>
#include <hal/mac/mem.h>
#include <ioal/mem32.h>
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <drv/watchdog/watchdog.h>
#include <private/drv/rtl8231/rtl8231.h>
#include <drv/gpio/ext_gpio.h>
#include <hwp/hw_profile.h>
#include <hwp/hwp_util.h>
#if defined(CONFIG_SDK_UART1)
#include <drv/uart/uart.h>
#endif
#include <rtdrv/rtdrv_netfilter.h>
#if (defined(CONFIG_SDK_DRIVER_TEST) || defined(CONFIG_SDK_DRIVER_TEST_MODULE))
#include <sdk/sdk_test.h>
#include <common/unittest_util.h>
#endif
#include <drv/gpio/gpio.h>
#if defined(CONFIG_SDK_DRIVER_I2C)
#include <drv/i2c/i2c.h>
#endif
#if defined(CONFIG_SDK_DRIVER_SPI)
#include <drv/spi/spi.h>
#include <private/drv/spi/spi_private.h>
#endif
#if defined(CONFIG_SDK_RTL8295) || defined(CONFIG_SDK_RTL8295R) || defined(CONFIG_SDK_RTL8214QF)
#include <hal/mac/rtl8295.h>
#include <hal/phy/phydef.h>
#include <hal/phy/phy_rtl8295_patch.h>
#include <hal/phy/phy_rtl8295.h>
#endif/* defined(CONFIG_SDK_RTL8295) || defined(CONFIG_SDK_RTL8295R) || defined(CONFIG_SDK_RTL8214QF)  */
#include <osal/memory.h>
#include <hwp/hw_profile.h>
#include <hal/mac/serdes.h>

#if defined(CONFIG_SDK_RTL8390)
#include <hal/phy/phy_rtl8390.h>
#endif

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
extern struct nf_sockopt_ops rtdrv_sockopts;

#if defined(CONFIG_SDK_APP_DIAG_EXT)
extern struct nf_sockopt_ops rtdrv_ext_sockopts;
#endif

static rtdrv_module_t rtdrv_module_db_set[RTDRV_MODULE_MAX_NUM];
static rtdrv_module_t rtdrv_module_db_get[RTDRV_MODULE_MAX_NUM];
rtdrv_test_module_t sdkTest_drv = {
    .mode_get = NULL,
    .mode_set = NULL,
    .run = NULL,
    .run_id = NULL,
};


/*
 * Macro Declaration
 */

rtdrv_union_t                   buf;

/*
 * Function Declaration
 */

int32 rtdrv_cmdFunc_register(rtdrv_setGet_t access, uint32 offset, int32 (*func)(int, void *))
{
    int module_index = offset >> RTDRV_MODULE_SHIFT;

    if ((module_index < 0) || (module_index > RTDRV_MODULE_MAX_NUM))
        return RT_ERR_FAILED;

    if (NULL == func)
        return RT_ERR_OK;

    if (RTDRV_SET == access)
    {
        if (TRUE == rtdrv_module_db_set[module_index].valid)
            return RT_ERR_FAILED;

        rtdrv_module_db_set[module_index].valid  = TRUE;
        rtdrv_module_db_set[module_index].offset = offset;
        rtdrv_module_db_set[module_index].func   = func;
    }
    else
    {
        if (TRUE == rtdrv_module_db_get[module_index].valid)
            return RT_ERR_FAILED;

        rtdrv_module_db_get[module_index].valid  = TRUE;
        rtdrv_module_db_get[module_index].offset = offset;
        rtdrv_module_db_get[module_index].func   = func;
    }
    return RT_ERR_OK;
}
EXPORT_SYMBOL(rtdrv_cmdFunc_register);
EXPORT_SYMBOL(sdkTest_drv);

int32 do_rtdrv_set_ctl_init(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** INIT **/
        case RTDRV_INIT_RTKAPI:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_init();

        default:
            break;
    }
    return ret;
}

int32 do_rtdrv_set_ctl_l2(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /* L2 */
        case RTDRV_L2_INIT:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_init(buf.l2_cfg.l2_common.unit);
            break;

        case RTDRV_L2_FLUSH_LINK_DOWN_PORT_ADDR_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_flushLinkDownPortAddrEnable_set(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.enable);
            break;

        case RTDRV_L2_UCASTADDR_FLUSH:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_ucastAddr_flush(buf.l2_cfg.l2_flush.unit, &buf.l2_cfg.l2_flush.flush);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_LIMIT_LEARNING_CNT_SET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_limitLearningCnt_set(buf.l2_learn.unit, buf.l2_learn.mac_cnt);
            break;

        case RTDRV_L2_PORT_LIMIT_LEARNING_CNT_SET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_portLimitLearningCnt_set(buf.l2_learn.unit, buf.l2_learn.port, buf.l2_learn.mac_cnt);
            break;
#endif
        case RTDRV_L2_LIMIT_LEARNING_NUM_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_limitLearningNum_set(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.type, &buf.l2_cfg.l2_macLimit.cnt);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_LIMIT_LEARNING_CNT_ACT_SET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_limitLearningCntAction_set(buf.l2_learn.unit, buf.l2_learn.action);
            break;

        case RTDRV_L2_PORT_LIMIT_LEARNING_CNT_ACT_SET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_portLimitLearningCntAction_set(buf.l2_learn.unit, buf.l2_learn.port, buf.l2_learn.action);
            break;

        case RTDRV_L2_FID_LIMIT_LEARNING_CNT_ACT_SET:
            copy_from_user(&buf.l2_FidLearn, user, sizeof(rtdrv_l2_learnFidCnt_t));
            ret = rtk_l2_fidLearningCntAction_set(buf.l2_FidLearn.unit, buf.l2_FidLearn.action);
            break;
#endif
        case RTDRV_L2_LIMIT_LEARNING_ACT_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_limitLearningAction_set(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.type, &buf.l2_cfg.l2_macLimit.action);
            break;

        case RTDRV_L2_FID_LIMIT_LEARNING_ENTRY_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_fidLimitLearningEntry_set(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.fidLimitEntryId, &buf.l2_cfg.l2_macLimit.entry);
            break;

        case RTDRV_L2_FID_LEARNING_CNT_RESET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_fidLearningCnt_reset(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.fidLimitEntryId);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_AGING_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_l2_aging_set(buf.unit_cfg.unit, buf.unit_cfg.data);
            break;

#endif
        case RTDRV_L2_AGING_TIME_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_agingTime_set(buf.l2_cfg.l2_age.unit, buf.l2_cfg.l2_age.type, buf.l2_cfg.l2_age.ageTime);;
            break;

        case RTDRV_L2_PORT_AGING_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portAgingEnable_set(buf.l2_cfg.l2_age.unit, buf.l2_cfg.l2_age.port, buf.l2_cfg.l2_age.enable);
            break;

        case RTDRV_L2_TRK_AGING_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_trkAgingEnable_set(buf.l2_cfg.l2_age.unit, buf.l2_cfg.l2_age.trunk, buf.l2_cfg.l2_age.enable);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_HASH_ALGO_SET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_hashAlgo_set(buf.l2_learnCfg.unit, buf.l2_learnCfg.hash_algo);
            break;
#endif
        case RTDRV_L2_BUCKET_HASH_ALGO_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_bucketHashAlgo_set(buf.l2_cfg.l2_hash.unit, buf.l2_cfg.l2_hash.bucket, buf.l2_cfg.l2_hash.hashAlgo);
            break;

        case RTDRV_L2_VLANMODE_SET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_vlanMode_set(buf.l2_learnCfg.unit, buf.l2_learnCfg.port, buf.l2_learnCfg.vlanMode);
            break;

        case RTDRV_L2_LEARNING_FULL_ACT_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_learningFullAction_set(buf.l2_cfg.l2_learn.unit, buf.l2_cfg.l2_learn.action);
            break;

        case RTDRV_L2_PORT_NEW_MAC_OP_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portNewMacOp_set(buf.l2_cfg.l2_learn.unit, buf.l2_cfg.l2_learn.port, buf.l2_cfg.l2_learn.lrnMode, buf.l2_cfg.l2_learn.action);
            break;

        case RTDRV_L2_ADDR_DEL:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_addr_del(buf.l2_cfg.l2_ucAddr.unit, buf.l2_cfg.l2_ucAddr.vid, &buf.l2_cfg.l2_ucAddr.mac);
            break;

        case RTDRV_L2_ADDR_DEL_ALL:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_addr_delAll(buf.l2_cfg.l2_ucAddr.unit, buf.l2_cfg.l2_ucAddr.include_static);
            break;

        case RTDRV_L2_MCAST_ADDR_DEL:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastAddr_del(buf.l2_cfg.l2_mcAddr.unit, buf.l2_cfg.l2_mcAddr.vid, &buf.l2_cfg.l2_mcAddr.mac);
            break;

        case RTDRV_L2_MCAST_ADDR_ADDBYINDEX:
            copy_from_user(&buf.mcast_data, user, sizeof(rtdrv_l2_mcastAddrData_t));
            ret = rtk_l2_mcastAddr_addByIndex(buf.mcast_data.unit, &(buf.mcast_data.m_data));
            break;

        case RTDRV_L2_IPMCMODE_SET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_ipmcMode_set(buf.l2_learnCfg.unit, buf.l2_learnCfg.ipmcMode);
            break;

        case RTDRV_L2_IP_MCAST_ADDR_ADD:
            copy_from_user(&buf.ipMcast_data, user, sizeof(rtk_l2_ipMcastAddr_t));
            ret = rtk_l2_ipMcastAddr_add(buf.ipMcast_data.unit, &buf.ipMcast_data.ip_m_data);
            break;

        case RTDRV_L2_IP_MCAST_ADDR_DEL:
            copy_from_user(&buf.ipMcast_data, user, sizeof(rtk_l2_ipMcastAddr_t));
            ret = rtk_l2_ipMcastAddr_del(buf.ipMcast_data.unit, buf.ipMcast_data.ip_m_data.sip, buf.ipMcast_data.ip_m_data.dip,
                                         buf.ipMcast_data.ip_m_data.rvid);
            break;

        case RTDRV_L2_IP_MCAST_ADDR_SET:
            copy_from_user(&buf.ipMcast_data, user, sizeof(rtk_l2_ipMcastAddr_t));
            ret = rtk_l2_ipMcastAddr_set(buf.ipMcast_data.unit, &buf.ipMcast_data.ip_m_data);
            break;

        case RTDRV_L2_IP_MCAST_ADDR_ADDBYINDEX:
            copy_from_user(&buf.ipMcast_data, user, sizeof(rtk_l2_ipMcastAddr_t));
            ret = rtk_l2_ipMcastAddr_addByIndex(buf.ipMcast_data.unit, &buf.ipMcast_data.ip_m_data);
            break;

        case RTDRV_L2_IPMC_DIP_CHK_SET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_ipMcastAddrChkEnable_set(buf.l2_learnCfg.unit, buf.l2_learnCfg.dip_check);
            break;

        case RTDRV_L2_IPMC_VLAN_COMPARE_SET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_ipMcstFidVidCompareEnable_set(buf.l2_common.unit, buf.l2_common.value);
            break;

        case RTDRV_L2_IP6MCASTMODE_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_l2_ip6mcMode_set(buf.unit_cfg.unit, buf.unit_cfg.data);
            break;

        case RTDRV_L2_HASHCAREBYTE_SET:
            copy_from_user(&buf.l2_hashCareByte, user, sizeof(rtdrv_l2_hashCareByte_t));
            ret = rtk_l2_ip6CareByte_set(buf.l2_hashCareByte.unit, buf.l2_hashCareByte.type, buf.l2_hashCareByte.value);
            break;

        case RTDRV_L2_IP6_MCAST_ADDR_ADD:
            copy_from_user(&buf.ip6Mcast_data, user, sizeof(rtdrv_l2_ip6McstAddrData_t));
            ret = rtk_l2_ip6McastAddr_add(buf.ip6Mcast_data.unit, &buf.ip6Mcast_data.ip6_m_data);
            break;

        case RTDRV_L2_IP6_MCAST_ADDR_DEL:
            copy_from_user(&buf.ip6Mcast_data, user, sizeof(rtdrv_l2_ip6McstAddrData_t));
            ret = rtk_l2_ip6McastAddr_del(buf.ip6Mcast_data.unit, buf.ip6Mcast_data.ip6_m_data.sip, buf.ip6Mcast_data.ip6_m_data.dip,
                                         buf.ip6Mcast_data.ip6_m_data.rvid);
            break;

        case RTDRV_L2_IP6_MCAST_ADDR_SET:
            copy_from_user(&buf.ip6Mcast_data, user, sizeof(rtdrv_l2_ip6McstAddrData_t));
            ret = rtk_l2_ip6McastAddr_set(buf.ip6Mcast_data.unit, &buf.ip6Mcast_data.ip6_m_data);
            break;

        case RTDRV_L2_IP6_MCAST_ADDR_ADDBYINDEX:
            copy_from_user(&buf.ip6Mcast_data, user, sizeof(rtdrv_l2_ip6McstAddrData_t));
            ret = rtk_l2_ip6McastAddr_addByIndex(buf.ip6Mcast_data.unit, &buf.ip6Mcast_data.ip6_m_data);
            break;

        case RTDRV_L2_MCAST_FWD_INDEX_FREE:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastFwdIndex_free(buf.l2_cfg.l2_portmask.unit, buf.l2_cfg.l2_portmask.index);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_MCAST_FWD_PORTMASK_SET:
            copy_from_user(&buf.l2_fwdEntryContent, user, sizeof(rtdrv_l2_fwdTblEntry_t));
            ret = rtk_l2_mcastFwdPortmask_set(buf.l2_fwdEntryContent.unit,
                buf.l2_fwdEntryContent.entryIdx, &buf.l2_fwdEntryContent.portMask, buf.l2_fwdEntryContent.crossVlan);
            break;
#endif
        case RTDRV_L2_MCAST_FWD_PORTMASK_ENTRY_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastFwdPortmaskEntry_set(buf.l2_cfg.l2_portmask.unit, buf.l2_cfg.l2_portmask.index, &buf.l2_cfg.l2_portmask.portmask);
            break;

        case RTDRV_L2_CPU_MAC_ADDR_ADD:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_cpuMacAddr_add(buf.l2_cfg.l2_ucAddr.unit, buf.l2_cfg.l2_ucAddr.vid, &buf.l2_cfg.l2_ucAddr.mac);
            break;

        case RTDRV_L2_CPU_MAC_ADDR_DEL:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_cpuMacAddr_del(buf.l2_cfg.l2_ucAddr.unit, buf.l2_cfg.l2_ucAddr.vid, &buf.l2_cfg.l2_ucAddr.mac);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_PORT_LEGAL_MOVETO_ACTION_SET:
            copy_from_user(&buf.l2_action, user, sizeof(rtdrv_l2_portAct_t));
            ret = rtk_l2_legalPortMoveAction_set(buf.l2_action.unit, buf.l2_action.port, buf.l2_action.action);
            break;

        case RTDRV_L2_DYNM_PORTMOVE_FORBID_ACTION_SET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_dynamicPortMoveForbidAction_set(buf.l2_common.unit, buf.l2_common.value);
            break;

#endif
        case RTDRV_L2_PORT_MOVE_ACT_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portMoveAction_set(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.type, &buf.l2_cfg.l2_portMove.action);
            break;

        case RTDRV_L2_PORT_MOVE_LEARN_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portMoveLearn_set(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.type, &buf.l2_cfg.l2_portMove.learn);
            break;

        case RTDRV_L2_LEGAL_MOVETO_FLUSH_ENABLE_SET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_legalPortMoveFlushAddrEnable_set(buf.l2_common.unit, buf.l2_common.port, buf.l2_common.value);
            break;

        case RTDRV_L2_STTC_PORT_MOVE_ACTION_SET:
            copy_from_user(&buf.l2_action, user, sizeof(rtdrv_l2_portAct_t));
            ret = rtk_l2_staticPortMoveAction_set(buf.l2_action.unit, buf.l2_action.port, buf.l2_action.action);
            break;

        case RTDRV_L2_LOOKUP_MISS_FLOOD_PORTMASK_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_lookupMissFloodPortMask_set(buf.l2_cfg.l2_lookUpMiss.unit, buf.l2_cfg.l2_lookUpMiss.type, &buf.l2_cfg.l2_lookUpMiss.flood_portmask);
            break;

        case RTDRV_L2_LOOKUP_MISS_FLOOD_PORTMASK_ADD:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_lookupMissFloodPortMask_add(buf.l2_cfg.l2_lookUpMiss.unit, buf.l2_cfg.l2_lookUpMiss.type, buf.l2_cfg.l2_lookUpMiss.port);
            break;

        case RTDRV_L2_LOOKUP_MISS_FLOOD_PORTMASK_DEL:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_lookupMissFloodPortMask_del(buf.l2_cfg.l2_lookUpMiss.unit, buf.l2_cfg.l2_lookUpMiss.type, buf.l2_cfg.l2_lookUpMiss.port);
            break;

        case RTDRV_L2_LOOKUP_MISS_FLOOD_PMSK_SET_WITH_IDX:
            copy_from_user(&buf.l2_lkMiss, user, sizeof(rtdrv_l2_lkMiss_t));
            ret = rtk_l2_lookupMissFloodPortMask_setByIndex(buf.l2_lkMiss.unit, buf.l2_lkMiss.type, buf.l2_lkMiss.index, &buf.l2_lkMiss.portMask);
            break;

        case RTDRV_L2_LOOKUP_MISS_FLOODPORTMASK_IDX_SET:
            copy_from_user(&buf.l2_lkMiss, user, sizeof(rtdrv_l2_lkMiss_t));
            ret = rtk_l2_lookupMissFloodPortMaskIdx_set(buf.l2_lkMiss.unit, buf.l2_lkMiss.type, buf.l2_lkMiss.index);
            break;

        case RTDRV_L2_PORT_LOOKUP_MISS_ACTION_SET:
            copy_from_user(&buf.l2_lkMiss, user, sizeof(rtdrv_l2_lkMiss_t));
            ret = rtk_l2_portLookupMissAction_set(buf.l2_lkMiss.unit, buf.l2_lkMiss.port, buf.l2_lkMiss.type, buf.l2_lkMiss.action);
            break;

        case RTDRV_L2_PORT_UCAST_LOOKUP_MISS_ACTION_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portUcastLookupMissAction_set(buf.l2_cfg.l2_lookUpMiss.unit, buf.l2_cfg.l2_lookUpMiss.port, buf.l2_cfg.l2_lookUpMiss.action);
            break;

        case RTDRV_L2_SRC_PORT_EGR_FILTER_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_srcPortEgrFilterMask_set(buf.l2_cfg.l2_common.unit, &buf.l2_cfg.l2_common.srcPortFilterPortmask);
            break;

        case RTDRV_L2_SRC_PORT_EGR_FILTER_ADD:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_srcPortEgrFilterMask_add(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.port);
            break;

        case RTDRV_L2_SRC_PORT_EGR_FILTER_DEL:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_srcPortEgrFilterMask_del(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.port);
            break;

        case RTDRV_L2_EXCEPTION_ADDR_ACTION_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_exceptionAddrAction_set(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.exceptType, buf.l2_cfg.l2_common.action);
            break;

        case RTDRV_L2_ZERO_SA_LEARNING_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_zeroSALearningEnable_set(buf.l2_cfg.l2_learn.unit, buf.l2_cfg.l2_learn.enable);
            break;

        case RTDRV_L2_SECURE_MAC_MODE_SET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_secureMacMode_set(buf.l2_common.unit, buf.l2_common.value);
            break;

        case RTDRV_L2_PORT_DYNM_PORTMOVE_FORBID_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portDynamicPortMoveForbidEnable_set(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.port, buf.l2_cfg.l2_portMove.enable);
            break;

        case RTDRV_L2_TRK_DYNM_PORTMOVE_FORBID_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_trkDynamicPortMoveForbidEnable_set(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.trunk, buf.l2_cfg.l2_portMove.enable);
            break;

        case RTDRV_L2_PORT_MAC_FILTER_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portMacFilterEnable_set(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.port, buf.l2_cfg.l2_common.filterMode, buf.l2_cfg.l2_common.enable);
            break;

        case RTDRV_L2_PORT_CTRL_TYPE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portCtrl_set(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.port, buf.l2_cfg.l2_common.portCtrlType, buf.l2_cfg.l2_common.arg);
            break;

        case RTDRV_L2_STK_LEARNING_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_stkLearningEnable_set(buf.l2_cfg.l2_learn.unit, buf.l2_cfg.l2_learn.enable);
            break;

        case RTDRV_L2_STK_KEEP_AGE_VALID_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_stkKeepUcastEntryValid_set(buf.l2_cfg.l2_age.unit, buf.l2_cfg.l2_age.enable);
            break;

        case RTDRV_L2_MCASTADDRECIDPMSK_ADD:
            copy_from_user(&buf.l2Ecid_cfg, user, sizeof(rtdrv_l2EcidCfg_t));
            ret = rtk_l2_mcastAddrEcidPmsk_add(buf.l2Ecid_cfg.unit, &buf.l2Ecid_cfg.mcastAddr, &buf.l2Ecid_cfg.entry);
            copy_to_user(user, &buf.l2Ecid_cfg, sizeof(rtdrv_l2EcidCfg_t));
            break;

        case RTDRV_L2_MCASTADDRECIDPMSK_DEL:
            copy_from_user(&buf.l2Ecid_cfg, user, sizeof(rtdrv_l2EcidCfg_t));
            ret = rtk_l2_mcastAddrEcidPmsk_del(buf.l2Ecid_cfg.unit, buf.l2Ecid_cfg.mcastAddr, &buf.l2Ecid_cfg.entry);
            break;
        default:
            ret = RT_ERR_FAILED;
            break;
    }

    return ret;
}

int32 do_rtdrv_set_ctl_l2ntfy(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_L2NTFY)
    /*L2 Notification module*/
        case RTDRV_L2NTFY_ENABLE_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_enable_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.enable);
            break;

        case RTDRV_L2NTFY_BACK_PRESSURE_THR_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_backPressureThresh_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.thresh);
            break;

        case RTDRV_L2NTFY_EVENT_ENABLE_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_notificationEventEnable_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.event, buf.l2ntfy_cfg.enable);
            break;

        case RTDRV_L2NTFY_COUNTER_DUMP:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_counter_dump(buf.l2ntfy_cfg.unit);
            break;

        case RTDRV_L2NTFY_COUNTER_CLEAR:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_counter_clear(buf.l2ntfy_cfg.unit);
            break;

        case RTDRV_L2NTFY_DBG_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_debug_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.dbgFlag);
            break;

        case RTDRV_L2NTFY_EVENT_DUMP:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_event_dump(buf.l2ntfy_cfg.unit);
            break;

        case RTDRV_L2NTFY_DST_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_dst_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.dst);
            break;

        case RTDRV_L2NTFY_RESET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_reset(buf.l2ntfy_cfg.unit);
            break;

        case RTDRV_L2NTFY_MAGIC_NUM_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_magicNum_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.magicNum);
            break;

        case RTDRV_L2NTFY_MACADDR_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_macAddr_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.addrType, &buf.l2ntfy_cfg.mac);
            break;

        case RTDRV_L2NTFY_MAXEVENT_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_maxEvent_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.maxEvent);
            break;

        case RTDRV_L2NTFY_TIMEOUT_SET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_timeout_set(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.mode, buf.l2ntfy_cfg.timeout);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_set_ctl_l3(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /*L3*/
        case RTDRV_L3_INIT :
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_l3_init(buf.unit_cfg.unit);
            break;

        case RTDRV_L3_ROUTE_ROUTEENTRY_SET:
            copy_from_user(&buf.l3_route_entry, user, sizeof(rtdrv_l3_routeEntry_t));
            ret = rtk_l3_routeEntry_set(buf.l3_route_entry.unit, buf.l3_route_entry.index, &buf.l3_route_entry.entry);
            break;

        case RTDRV_L3_ROUTE_SWITCHMACADDR_SET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_l3_routeSwitchMacAddr_set(buf.l3_config.unit, buf.l3_config.index, &buf.l3_config.mac);
            break;

        case RTDRV_L3_INFO_T_INIT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_info_t_init(&buf.l3_cfg.info);
            break;

        case RTDRV_L3_INTF_T_INIT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intf_t_init(&buf.l3_cfg.intf);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_INTF_CREATE:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intf_create(buf.l3_cfg.unit, &buf.l3_cfg.intf);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_INTF_DESTROY:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intf_destroy(buf.l3_cfg.unit, buf.l3_cfg.intfId);
            break;

        case RTDRV_L3_INTF_DESTROYALL:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intf_destroyAll(buf.l3_cfg.unit);
            break;

        case RTDRV_L3_INTF_SET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intf_set(buf.l3_cfg.unit, buf.l3_cfg.type, &buf.l3_cfg.intf);
            break;

        case RTDRV_L3_NEXTHOP_T_INIT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_nextHop_t_init(&buf.l3_cfg.nextHop);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_NEXTHOP_CREATE:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_nextHop_create(buf.l3_cfg.unit, buf.l3_cfg.flags, &buf.l3_cfg.nextHop, &buf.l3_cfg.nhId);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_NEXTHOP_DESTROY:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_nextHop_destroy(buf.l3_cfg.unit, buf.l3_cfg.nhId);
            break;

        case RTDRV_L3_ECMP_CREATE:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmp_create(buf.l3_cfg.unit, buf.l3_cfg.flags, buf.l3_cfg.nhIdCnt, &buf.l3_cfg.nhIdArray[0], &buf.l3_cfg.ecmpId);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ECMP_DESTROY:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmp_destroy(buf.l3_cfg.unit, buf.l3_cfg.ecmpPId);
            break;

        case RTDRV_L3_ECMP_ADD:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmp_add(buf.l3_cfg.unit, buf.l3_cfg.ecmpId, buf.l3_cfg.nhId);
            break;

        case RTDRV_L3_ECMP_DEL:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmp_del(buf.l3_cfg.unit, buf.l3_cfg.ecmpId, buf.l3_cfg.nhId);
            break;

        case RTDRV_L3_ECMPHASHTBL_SET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmpHashTbl_set(buf.l3_cfg.unit, buf.l3_cfg.ecmpId, buf.l3_cfg.nhIdxCount, &buf.l3_cfg.nhIdxArray[0]);
            break;

        case RTDRV_L3_ECMPCTRL_SET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmpCtrl_set(buf.l3_cfg.unit, buf.l3_cfg.ecmpId, buf.l3_cfg.ecmpCtrlType, buf.l3_cfg.arg);
            break;

        case RTDRV_L3_KEY_T_INIT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_key_t_init(&buf.l3_cfg.key);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_HOST_T_INIT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_t_init(&buf.l3_cfg.host);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_HOST_ADD:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_add(buf.l3_cfg.unit, &buf.l3_cfg.host);
            break;

        case RTDRV_L3_HOST_DEL:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_del(buf.l3_cfg.unit, &buf.l3_cfg.host);
            break;

        case RTDRV_L3_HOST_DELALL:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_delAll(buf.l3_cfg.unit, buf.l3_cfg.flags);
            break;

        case RTDRV_L3_HOST_DEL_BYNETWORK:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_del_byNetwork(buf.l3_cfg.unit, &buf.l3_cfg.route);
            break;

        case RTDRV_L3_HOST_DEL_BYINTFID:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_del_byIntfId(buf.l3_cfg.unit, buf.l3_cfg.intfId, buf.l3_cfg.flags);
            break;

        case RTDRV_L3_ROUTE_T_INIT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_t_init(&buf.l3_cfg.route);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ROUTE_ADD:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_add(buf.l3_cfg.unit, &buf.l3_cfg.route);
            break;

        case RTDRV_L3_ROUTE_DEL:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_del(buf.l3_cfg.unit, &buf.l3_cfg.route);
            break;

        case RTDRV_L3_ROUTE_DELALL:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_delAll(buf.l3_cfg.unit, buf.l3_cfg.flags);
            break;

        case RTDRV_L3_ROUTE_DEL_BYINTFID:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_del_byIntfId(buf.l3_cfg.unit, buf.l3_cfg.flags, buf.l3_cfg.intfId);
            break;

        case RTDRV_L3_GLOBALCTRL_SET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_globalCtrl_set(buf.l3_cfg.unit, buf.l3_cfg.globalCtrlType, buf.l3_cfg.arg);
            break;

        case RTDRV_L3_INTFCTRL_SET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intfCtrl_set(buf.l3_cfg.unit, buf.l3_cfg.intfId, buf.l3_cfg.type, buf.l3_cfg.arg);
            break;

        case RTDRV_L3_PORTCTRL_SET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_portCtrl_set(buf.l3_cfg.unit, buf.l3_cfg.port, buf.l3_cfg.type, buf.l3_cfg.arg);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_set_ctl_mcast(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if (defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310))
        /*MCAST*/
        case RTDRV_MCAST_INIT:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_mcast_init(buf.unit_cfg.unit);
            break;

        case RTDRV_MCAST_GROUP_DESTROY:
            copy_from_user(&buf.mcast_cfg, user, sizeof(rtdrv_mcastCfg_t));
            ret = rtk_mcast_group_destroy(buf.mcast_cfg.unit, buf.mcast_cfg.group);
            break;

        case RTDRV_MCAST_NEXTHOP_ADD:
            copy_from_user(&buf.mcast_cfg, user, sizeof(rtdrv_mcastCfg_t));
            ret = rtk_mcast_egrIf_add(buf.mcast_cfg.unit, buf.mcast_cfg.group, &buf.mcast_cfg.nhEntry);
            break;

        case RTDRV_MCAST_NEXTHOP_DEL:
            copy_from_user(&buf.mcast_cfg, user, sizeof(rtdrv_mcastCfg_t));
            ret = rtk_mcast_egrIf_del(buf.mcast_cfg.unit, buf.mcast_cfg.group, &buf.mcast_cfg.nhEntry);
            break;

        case RTDRV_MCAST_NEXTHOP_DELALL:
            copy_from_user(&buf.mcast_cfg, user, sizeof(rtdrv_mcastCfg_t));
            ret = rtk_mcast_egrIf_delAll(buf.mcast_cfg.unit, buf.mcast_cfg.group);
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_ipmc(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if (defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310))

        /*IPMCAST*/
        case RTDRV_IPMC_INIT:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_ipmc_init(buf.unit_cfg.unit);
            break;

        case RTDRV_IPMC_ADDR_ADD:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_addr_add(buf.ipmc_cfg.unit, &buf.ipmc_cfg.ipmcEntry);
            break;

        case RTDRV_IPMC_ADDR_DEL:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_addr_del(buf.ipmc_cfg.unit, &buf.ipmc_cfg.ipmcEntry);
            break;

        case RTDRV_IPMC_ADDR_DELALL:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_addr_delAll(buf.ipmc_cfg.unit, buf.ipmc_cfg.flags);
            break;

        case RTDRV_IPMC_STAT_MONT_CREATE:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_statMont_create(buf.ipmc_cfg.unit, &buf.ipmc_cfg.statMont);
            break;

        case RTDRV_IPMC_STAT_MONT_DESTROY:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_statMont_destroy(buf.ipmc_cfg.unit, &buf.ipmc_cfg.statMont);
            break;

        case RTDRV_IPMC_STAT_RESET:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_statCntr_reset(buf.ipmc_cfg.unit, &buf.ipmc_cfg.statKey);
            break;

        case RTDRV_IPMC_GLOBALCTRL_SET:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_globalCtrl_set(buf.ipmc_cfg.unit, buf.ipmc_cfg.globalCtrlType, buf.ipmc_cfg.arg);
            break;
    #endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_set_ctl_tunnel(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /*Tunnel*/
#if defined(CONFIG_SDK_RTL9310)
        case RTDRV_TUNNEL_INIT:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_init(buf.tunnel_cfg.unit);
            break;

        case RTDRV_TUNNEL_INFO_T_INIT:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_info_t_init(&buf.tunnel_cfg.info);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_INTF_T_INIT:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intf_t_init(&buf.tunnel_cfg.intf);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_INTF_CREATE:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intf_create(buf.tunnel_cfg.unit, &buf.tunnel_cfg.intf);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_INTF_DESTROY:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intf_destroy(buf.tunnel_cfg.unit, buf.tunnel_cfg.intfId);
            break;

        case RTDRV_TUNNEL_INTF_DESTROYALL:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intf_destroyAll(buf.tunnel_cfg.unit);
            break;

        case RTDRV_TUNNEL_INTF_SET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intf_set(buf.tunnel_cfg.unit, &buf.tunnel_cfg.intf);
            break;

        case RTDRV_TUNNEL_QOSPROFILE_SET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_qosProfile_set(buf.tunnel_cfg.unit, buf.tunnel_cfg.idx, buf.tunnel_cfg.profile);
            break;

        case RTDRV_TUNNEL_GLOBALCTRL_SET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_globalCtrl_set(buf.tunnel_cfg.unit, buf.tunnel_cfg.type, buf.tunnel_cfg.arg);
            break;

        case RTDRV_TUNNEL_INTFCTRL_SET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intfCtrl_set(buf.tunnel_cfg.unit, buf.tunnel_cfg.intfId, buf.tunnel_cfg.intfType, buf.tunnel_cfg.intfArg);
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_set_ctl_vxlan(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /*VXLAN*/
#if defined(CONFIG_SDK_RTL9310)
        case RTDRV_VXLAN_INIT:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_init(buf.vxlan_cfg.unit);
            break;

        case RTDRV_VXLAN_VNI_ADD:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_vni_add(buf.vxlan_cfg.unit, buf.vxlan_cfg.entry);
            break;

        case RTDRV_VXLAN_VNI_DEL:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_vni_del(buf.vxlan_cfg.unit, buf.vxlan_cfg.entry);
            break;

        case RTDRV_VXLAN_VNI_DELALL:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_vni_delAll(buf.vxlan_cfg.unit);
            break;

        case RTDRV_VXLAN_VNI_SET:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_vni_set(buf.vxlan_cfg.unit, buf.vxlan_cfg.entry);
            break;

        case RTDRV_VXLAN_GLOBALCTRL_SET:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_globalCtrl_set(buf.vxlan_cfg.unit, buf.vxlan_cfg.type, buf.vxlan_cfg.arg);
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_port(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /** PORT **/
        case RTDRV_PORT_EN_AUTONEGO_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyAutoNegoEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_AUTONEGO_ABIL_SET:
            copy_from_user(&buf.autonego_ability, user, sizeof(rtdrv_port_autoNegoAbility_t));
            ret = rtk_port_phyAutoNegoAbility_set(buf.autonego_ability.unit, buf.autonego_ability.port,
                                                  &buf.autonego_ability.ability);
            break;

        case RTDRV_PORT_FORCE_MODE_ABIL_SET:
            copy_from_user(&buf.forcemode_ability, user, sizeof(rtdrv_port_forceModeAbility_t));
            ret = rtk_port_phyForceModeAbility_set(buf.forcemode_ability.unit, buf.forcemode_ability.port,
                                                   buf.forcemode_ability.speed, buf.forcemode_ability.duplex,
                                                   buf.forcemode_ability.flowctrl);
            break;

        case RTDRV_PORT_PHY_REG_SET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phyReg_set(buf.phy_data.unit, buf.phy_data.port, buf.phy_data.page,
                                      buf.phy_data.reg, buf.phy_data.data);
            break;

        case RTDRV_PORT_PHY_EXT_PARK_PAGE_REG_SET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phyExtParkPageReg_set(buf.phy_data.unit, buf.phy_data.port, buf.phy_data.page,
                                      buf.phy_data.extPage, buf.phy_data.parkPage, buf.phy_data.reg, buf.phy_data.data);
            break;

        case RTDRV_PORT_PHYMASK_EXT_PARK_PAGE_REG_SET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phymaskExtParkPageReg_set(buf.phy_data.unit, &buf.phy_data.portmask, buf.phy_data.page,
                                      buf.phy_data.extPage, buf.phy_data.parkPage, buf.phy_data.reg, buf.phy_data.data);
            break;

        case RTDRV_PORT_PHY_MMD_REG_SET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phyMmdReg_set(buf.phy_data.unit, buf.phy_data.port, buf.phy_data.mmdAddr,
                                      buf.phy_data.reg, buf.phy_data.data);
            break;

        case RTDRV_PORT_PHYMASK_MMD_REG_SET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phymaskMmdReg_set(buf.phy_data.unit, &buf.phy_data.portmask, buf.phy_data.mmdAddr,
                                      buf.phy_data.reg, buf.phy_data.data);
            break;

        case RTDRV_PORT_MASTER_SLAVE_SET:
            copy_from_user(&buf.masterSlave_cfg, user, sizeof(rtdrv_port_masterSlave_t));
            ret = rtk_port_phyMasterSlave_set(buf.masterSlave_cfg.unit, buf.masterSlave_cfg.port, buf.masterSlave_cfg.masterSlaveCfg);
            break;

        case RTDRV_PORT_ISOLATION_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolation_set(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.portmask);
            break;

        case RTDRV_PORT_ISOLATIONEXT_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolationExt_set(buf.port_cfg.unit, buf.port_cfg.srcUnit, buf.port_cfg.port, &buf.port_cfg.portmask);
            break;

         case RTDRV_PORT_ISOLATION_ADD:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolation_add(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.targetPort);
            break;

        case RTDRV_PORT_ISOLATION_DEL:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolation_del(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.targetPort);
            break;

        case RTDRV_PORT_ISOLATION_RESTRICT_ROUTE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolationRestrictRoute_set(buf.port_cfg.unit, buf.port_cfg.data);
            break;

        case RTDRV_PORT_EN_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_adminEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_BACK_PRESSURE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_backpressureEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_PHY_MEDIA_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyComboPortMedia_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.media);
            break;

        case RTDRV_PORT_GREEN_ENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_greenEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_GIGA_LITE_ENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_gigaLiteEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_PHY_CROSSOVERMODE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyCrossOverMode_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_TX_EN_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_txEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_RX_EN_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_rxEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_PHY_FIBER_MEDIA_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyComboPortFiberMedia_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.fiber_media);
            break;

        case RTDRV_PORT_LINKDOWN_POWERSAVING_ENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_linkDownPowerSavingEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_VLAN_ISOLATION_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_vlanBasedIsolationEntry_set(buf.port_cfg.unit, buf.port_cfg.index, &buf.port_cfg.vlanIsoEntry);
            break;

        case RTDRV_PORT_VLAN_ISOLATION_VLANSOURCE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_vlanBasedIsolation_vlanSource_set(buf.port_cfg.unit, buf.port_cfg.vlanIsoSrc);
            break;

        case RTDRV_PORT_VLAN_ISOLATION_EGRBYPASS_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_vlanBasedIsolationEgrBypass_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_DOWNSPEEDENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_downSpeedEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;
        case RTDRV_PORT_FIBERDOWNSPEEDENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberDownSpeedEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;
        case RTDRV_PORT_FIBERNWAYFORCELINKENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberNwayForceLinkEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;
        case RTDRV_PORT_FIBERUNIDIRENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberUnidirEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.enable);
            break;
        case RTDRV_PORT_FIBEROAMLOOPBACKENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberOAMLoopBackEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;
        case RTDRV_PORT_10GMEDIA_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_10gMedia_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.media_10g);
            break;
        case RTDRV_PORT_PHYLOOPBACKENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyLoopBackEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;
        case RTDRV_PORT_10GSDS_RESTART:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_10gSds_restart(buf.port_cfg.unit, buf.port_cfg.port);
            break;

        case RTDRV_PORT_10G_INIT:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_10g_init(buf.port_cfg.unit, buf.port_cfg.port);
            break;

        case RTDRV_PORT_PHYFIBERTXDIS_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyFiberTxDis_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.enable);
            break;

        case RTDRV_PORT_PHYFIBERTXDISPIN_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyFiberTxDisPin_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_PORT_FIBERRXENABLE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberRxEnable_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.enable);
            break;

        case RTDRV_PORT_PHYIEEETESTMODE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyIeeeTestMode_set(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.testMode);
            break;
        case RTDRV_PORT_PHYPOLAR_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyPolar_set(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.polarCtrl);
            break;
        case RTDRV_PORT_PHYEYEMONITOR_START:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyEyeMonitor_start(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;
        case RTDRV_PORT_IMAGEFLASH_LOAD:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_imageFlash_load(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.image_size, buf.port_cfg.image);
            break;
        case RTDRV_PORT_PHYSDS_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phySds_set(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.sdsCfg);
            break;
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_vlan(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** VLAN **/
        case RTDRV_VLAN_PORT_SET:
            copy_from_user(&buf.vlan_port_data, user, sizeof(rtdrv_vlan_port_t));
            ret = rtk_vlan_port_set(buf.vlan_port_data.unit, buf.vlan_port_data.vid, &buf.vlan_port_data.member,
                                    &buf.vlan_port_data.untag);
            break;

        case RTDRV_VLAN_PORT_PVID_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portPvid_set(buf.vlan_cfg.unit, buf.vlan_cfg.port,buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PROTO_GROUP_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_protoGroup_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &(buf.vlan_cfg.protoGroup));
            break;

        case RTDRV_VLAN_PORT_PROTO_VLAN_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portProtoVlan_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.idx, &(buf.vlan_cfg.protoVlanCfg));
            break;

        case RTDRV_VLAN_PORT_IGR_TPID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrTpid_set(buf.vlan_cfg.unit, buf.vlan_cfg.port,buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGR_TPID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTpid_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGR_TPID_SRC_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTpidSrc_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_IGR_EXTRA_TAG_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrExtraTagEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_IGR_VLAN_TRANSPARENT_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanTransparentEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGR_VLAN_TRANSPARENT_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanTransparentEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_ACCEPT_FRAME_TYPE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portAcceptFrameType_set(buf.vlan_cfg.unit, buf.vlan_cfg.port,buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EN_MCAST_LEAKY_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_vlan_mcastLeakyEnable_set(buf.unit_cfg.unit, buf.unit_cfg.data);
            break;

        case RTDRV_VLAN_SVLMODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_svlMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.mode);
            break;

        case RTDRV_VLAN_SVLFID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_svlFid_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.fid);
            break;

        case RTDRV_VLAN_STG_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_stg_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_L2_LOOKUP_SVL_FID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2LookupSvlFid_set(buf.vlan_cfg.unit, buf.vlan_cfg.macType, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_CREATE:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_create(buf.vlan_cfg.unit, buf.vlan_cfg.vid);
            break;

        case RTDRV_VLAN_DESTROY:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_destroy(buf.vlan_cfg.unit, buf.vlan_cfg.vid);
            break;

        case RTDRV_VLAN_DESTROY_ALL:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_vlan_destroyAll(buf.unit_cfg.unit, buf.unit_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGR_FILTER_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrFilterEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_ADD:
            copy_from_user(&buf.vlan_port_data, user, sizeof(rtdrv_vlan_port_t));
            ret = rtk_vlan_port_add(buf.vlan_port_data.unit, buf.vlan_port_data.vid, buf.vlan_port_data.port,
                                    buf.vlan_port_data.is_untag);
            break;

        case RTDRV_VLAN_PORT_DEL:
            copy_from_user(&buf.vlan_port_data, user, sizeof(rtdrv_vlan_port_t));
            ret = rtk_vlan_port_del(buf.vlan_port_data.unit, buf.vlan_port_data.vid, buf.vlan_port_data.port);
            break;

        case RTDRV_VLAN_MCASTGROUP_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_mcastGroup_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.groupId);
            break;

        case RTDRV_VLAN_LUTMODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2LookupMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.macType, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_GROUPMASK_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_groupMask_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.groupMask);
            break;

        case RTDRV_VLAN_PROFILE_IDX_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_profileIdx_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PROFILE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_profile_set(buf.vlan_cfg.unit, buf.vlan_cfg.data, &buf.vlan_cfg.profile);
            break;

        case RTDRV_VLAN_PORT_FORWARD_VLAN_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portFwdVlan_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.tagMode, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_IGR_FILTER_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrFilter_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_PVID_MODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portPvidMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type,buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_MAC_BASED_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlan_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    buf.vlan_cfg.data, &buf.vlan_cfg.mac, buf.vlan_cfg.vid,
                    buf.vlan_cfg.data1);
            break;

        case RTDRV_VLAN_MAC_BASED_WITH_MSK_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlanWithMsk_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    buf.vlan_cfg.data, &buf.vlan_cfg.mac, &buf.vlan_cfg.msk, buf.vlan_cfg.vid,
                    buf.vlan_cfg.data1);
            break;

        case RTDRV_VLAN_MAC_BASED_WITH_PORT_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlanWithPort_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    buf.vlan_cfg.data, &buf.vlan_cfg.mac, &buf.vlan_cfg.msk,
                    buf.vlan_cfg.port, buf.vlan_cfg.port_msk, buf.vlan_cfg.vid, buf.vlan_cfg.data1);
            break;

        case RTDRV_VLAN_MAC_BASED_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portMacBasedVlanEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port,buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_MAC_BASED_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlanEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx,&buf.vlan_cfg.macEntry);
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ipSubnetBasedVlan_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    buf.vlan_cfg.data, buf.vlan_cfg.sip, buf.vlan_cfg.sip_msk,
                    buf.vlan_cfg.vid, buf.vlan_cfg.data1);
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_WITH_PORT_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ipSubnetBasedVlanWithPort_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    buf.vlan_cfg.data, buf.vlan_cfg.sip, buf.vlan_cfg.sip_msk,
                    buf.vlan_cfg.port, buf.vlan_cfg.port_msk, buf.vlan_cfg.vid, buf.vlan_cfg.data1);
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIpSubnetBasedVlanEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port,buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ipSubnetBasedVlanEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx,&buf.vlan_cfg.ipEntry);
            break;

        case RTDRV_VLAN_TPID_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_tpidEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.tagType, buf.vlan_cfg.idx, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EGR_TAG_STS_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTagSts_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_IGRVLANCNVT_BLKMODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtBlkMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_IGRVLANCNVT_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.igrCnvtEntry);
            break;

        case RTDRV_VLAN_PORTIGRVLANCNVTENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanCnvtEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.enable);
            break;

        case RTDRV_VLAN_EGRVLANCNVT_DBLTAG_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtDblTagEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EGRVLANCNVT_VIDSRC_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtVidSource_set(buf.vlan_cfg.unit, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EGRVLANCNVT_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.egrCnvtEntry);
            break;

        case RTDRV_VLAN_PORTEGRVLANCNVTENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.enable);
            break;

        case RTDRV_VLAN_AGGRENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_aggrEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.enable);
            break;

        case RTDRV_VLAN_PORT_VLANAGGR_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_VLANAGGR_PRI_ENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrPriEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.enable);
            break;

        case RTDRV_VLAN_LEAKYSTPFILTER_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_leakyStpFilter_set(buf.vlan_cfg.unit, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EXCEPT_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_except_set(buf.vlan_cfg.unit, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORTIGRCNVTDFLTACT_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrCnvtDfltAct_set(buf.vlan_cfg.unit,
                    buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_IGRVLANCNVTENTRY_DELALL:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtEntry_delAll(buf.vlan_cfg.unit);
            break;

        case RTDRV_VLAN_EGRVLANCNVTENTRY_DELALL:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtEntry_delAll(buf.vlan_cfg.unit);
            break;

        case RTDRV_VLAN_PORT_IGRTAGKEEPTYPE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrTagKeepType_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data, buf.vlan_cfg.data1);
            break;

        case RTDRV_VLAN_PORT_EGRTAGKEEPTYPE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTagKeepType_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data, buf.vlan_cfg.data1);
            break;

        case RTDRV_VLAN_PORT_VLANAGGRCTRL_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrCtrl_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.vlanAggrCtrl);
            break;

        case RTDRV_VLAN_PORT_EGRVLANCNVTVIDSOURCE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtVidSource_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGRVLANCNVTVIDTARGET_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtVidTarget_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORTIGRCNVTLUMISACT_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanCnvtLuMisAct_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORTEGRCNVTLUMISACT_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtLuMisAct_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_IGRVLANCNVTRANGECHECKSET_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanCnvtRangeCheckSet_set(buf.vlan_cfg.unit,buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_IGRVLANCNVTRANGECHECKENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtRangeCheckEntry_set(buf.vlan_cfg.unit,buf.vlan_cfg.setIdx, buf.vlan_cfg.idx, &buf.vlan_cfg.rangeCheck);
            break;

         case RTDRV_VLAN_EGRVLANCNVTRANGECHECKSET_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtRangeCheckSet_set(buf.vlan_cfg.unit,buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EGRVLANCNVTRANGECHECKENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtRangeCheckEntry_set(buf.vlan_cfg.unit,buf.vlan_cfg.setIdx, buf.vlan_cfg.idx, &buf.vlan_cfg.rangeCheck);
            break;

#ifdef CONFIG_SDK_DRIVER_RTK_LEGACY_API
        case RTDRV_VLAN_PORT_VLANAGGRVIDSOURCE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrVidSource_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_VLANAGGRPRITAGVIDSOURCE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrPriTagVidSource_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_UCAST_LUTMODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2UcastLookupMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_MCAST_LUTMODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2McastLookupMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_INNER_ACCEPT_FRAME_TYPE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portInnerAcceptFrameType_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_VLAN_PORT_OUTER_ACCEPT_FRAME_TYPE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portOuterAcceptFrameType_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_VLAN_PORT_INNER_PVID_MODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portInnerPvidMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_OUTER_PVID_MODE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portOuterPvidMode_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_INNER_PVID_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portInnerPvid_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_VLAN_PORT_OUTER_PVID_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portOuterPvid_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.data);
            break;

        case RTDRV_VLAN_INNER_TPID_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_innerTpidEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_OUTER_TPID_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_outerTpidEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EXTRA_TPID_ENTRY_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_extraTpidEntry_set(buf.vlan_cfg.unit, buf.vlan_cfg.idx, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_IGR_INNER_TPID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrInnerTpid_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_IGR_OUTER_TPID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrOuterTpid_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGR_INNER_TPID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrInnerTpid_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGR_OUTER_TPID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrOuterTpid_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EGR_INNER_TAG_STS_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrInnerTagSts_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_EGR_OUTER_TAG_STS_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrOuterTagSts_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_EGRVLANCNVTLOOKUPMISSACT_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtLookupMissAct_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.data);
            break;
#endif

        case RTDRV_VLAN_EGRVLANCNVTRANGECHECKVID_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtRangeCheckVid_set(buf.vlan_cfg.unit,
                    buf.vlan_cfg.idx, &buf.vlan_cfg.rangeCheck);
            break;

        case RTDRV_VLAN_ECIDPMSK_ADD:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ecidPmsk_add(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.entry);
            break;

        case RTDRV_VLAN_ECIDPMSK_DEL:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ecidPmsk_del(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.entry);
            break;

        case RTDRV_VLAN_BSSID_ADD:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_bssid_add(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.bssid);
            break;

        case RTDRV_VLAN_BSSID_DEL:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_bssid_del(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.bssid);
            break;

        case RTDRV_VLAN_TRKVLANAGGRENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_trkVlanAggrEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.tid, buf.vlan_cfg.enable);
            break;

        case RTDRV_VLAN_TRKVLANAGGRPRIENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_trkVlanAggrPriEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.tid, buf.vlan_cfg.enable);
            break;

        case RTDRV_VLAN_TRKVLANAGGRCTRL_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_trkVlanAggrCtrl_set(buf.vlan_cfg.unit, buf.vlan_cfg.tid, buf.vlan_cfg.vlanAggrCtrl);
            break;

        case RTDRV_VLAN_PORTPRIVATEVLANENABLE_SET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portPrivateVlanEnable_set(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.enable);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_oam(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310))
    /*OAM*/
        case RTDRV_OAM_INIT :
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_oam_init(buf.port_cfg.unit);
            break;

        case RTDRV_OAM_PORTDYINGGASPPAYLOAD_SET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_portDyingGaspPayload_set(buf.dyingGasp_cfg.unit,
                    buf.dyingGasp_cfg.port, buf.dyingGasp_cfg.payload,
                    buf.dyingGasp_cfg.cnt);
            break;

        case RTDRV_OAM_DYINGGASPSEND_SET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_dyingGaspSend_set(buf.dyingGasp_cfg.unit, buf.dyingGasp_cfg.enable);
            break;

        case RTDRV_OAM_AUTODYINGGASPENABLE_SET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_autoDyingGaspEnable_set(buf.dyingGasp_cfg.unit, buf.dyingGasp_cfg.port, buf.dyingGasp_cfg.enable);
            break;

        case RTDRV_OAM_DYINGGASPWAITTIME_SET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_dyingGaspWaitTime_set(buf.dyingGasp_cfg.unit, buf.dyingGasp_cfg.waitTime);
            break;

        case RTDRV_OAM_DYINGGASPPKTCNT_SET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_dyingGaspPktCnt_set(buf.dyingGasp_cfg.unit, buf.dyingGasp_cfg.cnt);
            break;

        case RTDRV_OAM_LOOPBACKMACSWAPENABLE_SET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_oam_loopbackMacSwapEnable_set(buf.misc_cfg.unit,
                                                    buf.misc_cfg.loopbackEnable);
            break;

        case RTDRV_OAM_PORTLOOPBACKMUXACTION_SET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_oam_portLoopbackMuxAction_set(buf.misc_cfg.unit,
                    buf.misc_cfg.port, buf.misc_cfg.action);
            break;

        case RTDRV_OAM_PDULEARNINGENABLE_SET:
            copy_from_user(&buf.oam_cfg, user, sizeof(rtdrv_oamCfg_t));
            ret = rtk_oam_pduLearningEnable_set(buf.oam_cfg.unit, buf.oam_cfg.enable);
            break;

        case RTDRV_OAM_CFMCCMPCP_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmPcp_set(buf.ccm_cfg.unit,
                                        buf.ccm_cfg.ccmFrame.outer_pri);
            break;

        case RTDRV_OAM_CFMCCMCFI_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmCfi_set(buf.ccm_cfg.unit,
                                        buf.ccm_cfg.ccmFrame.outer_dei);
            break;

        case RTDRV_OAM_CFMCCMTPID_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmTpid_set(buf.ccm_cfg.unit,
                                         buf.ccm_cfg.ccmFrame.outer_tpid);
            break;

        case RTDRV_OAM_CFMCCMRESETLIFETIME_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstLifetime_set(buf.ccm_cfg.unit,
                                                  buf.ccm_cfg.cfmIdx,
                                                  buf.ccm_cfg.ccmFlag);
            break;
#if (defined CONFIG_SDK_RTL8390)
        case RTDRV_OAM_CFMCCMMEPID_SET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_oam_cfmCcmMepid_set(buf.misc_cfg.unit,
                                          buf.misc_cfg.mepid);
            break;

        case RTDRV_OAM_CFMCCMINTERVALFIELD_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmIntervalField_set(buf.ccm_cfg.unit,
                                                  buf.ccm_cfg.ccmFlag);
            break;

        case RTDRV_OAM_CFMCCMMDL_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmMdl_set(buf.cfm_cfg.unit,
                                        buf.cfm_cfg.cfmCfg.md_level);
            break;
#endif  /* (defined CONFIG_SDK_RTL8390) */

        case RTDRV_OAM_CFMCCMINSTTXMEPID_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxMepid_set(buf.cfm_cfg.unit,
                    buf.cfm_cfg.instance, buf.cfm_cfg.mepid);
            break;

        case RTDRV_OAM_CFMCCMINSTTXINTERVALFIELD_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxIntervalField_set(buf.cfm_cfg.unit,
                    buf.cfm_cfg.instance, buf.cfm_cfg.interval);
            break;

        case RTDRV_OAM_CFMCCMINSTTXMDL_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxMdl_set(buf.cfm_cfg.unit,
                    buf.cfm_cfg.instance, buf.cfm_cfg.mdl);
            break;

        case RTDRV_OAM_CFMCCMINSTTAGSTATUS_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTagStatus_set(buf.cfm_cfg.unit,
                                                  buf.cfm_cfg.cfmIdx,
                                                  buf.cfm_cfg.enable);
            break;

        case RTDRV_OAM_CFMCCMINSTVID_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstVid_set(buf.ccm_cfg.unit,
                                            buf.ccm_cfg.cfmIdx,
                                            buf.ccm_cfg.ccmFrame.outer_vid);
            break;

        case RTDRV_OAM_CFMCCMINSTMAID_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstMaid_set(buf.cfm_cfg.unit,
                                             buf.cfm_cfg.cfmIdx,
                                             buf.cfm_cfg.maid);
            break;

        case RTDRV_OAM_CFMCCMINSTTXSTATUS_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxStatus_set(buf.cfm_cfg.unit,
                                                 buf.cfm_cfg.cfmIdx,
                                                 buf.cfm_cfg.enable);
            break;

        case RTDRV_OAM_CFMCCMINSTINTERVAL_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstInterval_set(buf.ccm_cfg.unit,
                                                 buf.ccm_cfg.cfmIdx,
                                                 buf.ccm_cfg.ccmInterval);
            break;
#if (defined CONFIG_SDK_RTL8390)
        case RTDRV_OAM_CFMCCMTXINSTPORT_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmTxInstPort_set(buf.ccm_cfg.unit,
                                               buf.ccm_cfg.cfmIdx,
                                               buf.ccm_cfg.portIdx,
                                               buf.ccm_cfg.port);
            break;
#endif  /* (defined CONFIG_SDK_RTL8390) */
        case RTDRV_OAM_CFMCCMRXINSTVID_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmRxInstVid_set(buf.ccm_cfg.unit,
                                              buf.ccm_cfg.cfmIdx,
                                              buf.ccm_cfg.ccmFrame.outer_vid);
            break;
#if (defined CONFIG_SDK_RTL8390)
        case RTDRV_OAM_CFMCCMRXINSTPORT_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmRxInstPort_set(buf.ccm_cfg.unit,
                                               buf.ccm_cfg.cfmIdx,
                                               buf.ccm_cfg.portIdx,
                                               buf.ccm_cfg.port);
            break;
#endif  /* (defined CONFIG_SDK_RTL8390) */
        case RTDRV_OAM_CFMCCMINSTTXMEMBER_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstTxMember_set(buf.ccm_cfg.unit, buf.ccm_cfg.instance, &buf.ccm_cfg.member);
            break;

        case RTDRV_OAM_CFMCCMINSTRXMEMBER_SET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstRxMember_set(buf.ccm_cfg.unit, buf.ccm_cfg.instance, &buf.ccm_cfg.member);
            break;

        case RTDRV_OAM_CFMETHDMPORTENABLE_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmPortEthDmEnable_set(buf.cfm_cfg.unit,
                                                 buf.cfm_cfg.port,
                                                 buf.cfm_cfg.enable);
            break;

        case RTDRV_OAM_CFMETHDMTXDELAY_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmTxDelay_set(buf.cfm_cfg.unit,
                                                 buf.cfm_cfg.txDelay);
            break;

        case RTDRV_OAM_CFMETHDMREFTIME_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmRefTime_set(buf.cfm_cfg.unit,
                                                 buf.cfm_cfg.timeStamp);
            break;

        case RTDRV_OAM_CFMETHDMREFTIMEENABLE_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmRefTimeEnable_set(buf.cfm_cfg.unit,
                                                 buf.cfm_cfg.enable);
            break;

        case RTDRV_OAM_CFMETHDMREFTIMEFREQ_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmRefTimeFreq_set(buf.cfm_cfg.unit,
                                                 buf.cfm_cfg.freq);
            break;

        case RTDRV_OAM_LINKFAULTMONENABLE_SET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_linkFaultMonEnable_set(buf.cfm_cfg.unit, buf.cfm_cfg.enable);
            break;
#endif  /* (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9310)) */

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_bpe(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** BPE **/
        case RTDRV_BPE_INIT:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_init(buf.bpe_cfg.unit);
            break;

        case RTDRV_BPE_PORTFWDMODE_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portFwdMode_set(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.mode);
            break;

        case RTDRV_BPE_PORTECIDNAMESPACEGROUPID_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portEcidNameSpaceGroupId_set(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.groupId);
            break;

        case RTDRV_BPE_PORTPCID_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portPcid_set(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.type, buf.bpe_cfg.pcid);
            break;

        case RTDRV_BPE_PORTPCIDACT_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portPcidAct_set(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.action);
            break;

        case RTDRV_BPE_PORTEGRTAGSTS_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portEgrTagSts_set(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.status);
            break;

        case RTDRV_BPE_PORTEGRVLANTAGSTS_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portEgrVlanTagSts_set(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.vlan_status);
            break;

        case RTDRV_BPE_PVIDENTRY_ADD:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_pvidEntry_add(buf.bpe_cfg.unit, &buf.bpe_cfg.pvid_entry);
            break;

        case RTDRV_BPE_PVIDENTRY_DEL:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_pvidEntry_del(buf.bpe_cfg.unit, &buf.bpe_cfg.pvid_entry);
            break;

        case RTDRV_BPE_FWDENTRY_ADD:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_fwdEntry_add(buf.bpe_cfg.unit, &buf.bpe_cfg.fwd_entry);
            break;

        case RTDRV_BPE_FWDENTRY_DEL:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_fwdEntry_del(buf.bpe_cfg.unit, &buf.bpe_cfg.fwd_entry);
            break;

        case RTDRV_BPE_GLOBALCTRL_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_globalCtrl_set(buf.bpe_cfg.unit, buf.bpe_cfg.type, buf.bpe_cfg.arg);
            break;

        case RTDRV_BPE_PORTCTRL_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portCtrl_set(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.type, buf.bpe_cfg.arg);
            break;

        case RTDRV_BPE_PRIREMARKING_SET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_priRemarking_set(buf.bpe_cfg.unit, buf.bpe_cfg.src, buf.bpe_cfg.val, buf.bpe_cfg.pri);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_stp(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** STP **/
        case RTDRV_STP_MSTP_STATE_SET:
            copy_from_user(&buf.stp_cfg, user, sizeof(rtdrv_stpCfg_t));
            ret = rtk_stp_mstpState_set(buf.stp_cfg.unit, buf.stp_cfg.msti, buf.stp_cfg.port, buf.stp_cfg.stp_state);
            break;

        case RTDRV_STP_MSTP_INSTANCE_CREATE:
            copy_from_user(&buf.stp_cfg, user, sizeof(rtdrv_stpCfg_t));
            ret = rtk_stp_mstpInstance_create(buf.stp_cfg.unit, buf.stp_cfg.msti);
            break;

        case RTDRV_STP_MSTP_INSTANCE_DESTROY:
            copy_from_user(&buf.stp_cfg, user, sizeof(rtdrv_stpCfg_t));
            ret = rtk_stp_mstpInstance_destroy(buf.stp_cfg.unit, buf.stp_cfg.msti);
            break;

        case RTDRV_STP_MSTP_MODE_SET:
            copy_from_user(&buf.stp_cfg, user, sizeof(rtdrv_stpCfg_t));
            ret = rtk_stp_mstpInstanceMode_set(buf.stp_cfg.unit, buf.stp_cfg.msti_mode);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_reg(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** REG **/
        case RTDRV_REG_REGISTER_SET:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ioal_mem32_write(buf.reg_cfg.unit, buf.reg_cfg.reg, buf.reg_cfg.value);
            ret = RT_ERR_OK; /*xxx_reg_register_set(buf.reg_cfg.unit, buf.reg_cfg.reg, buf.reg_cfg.value);*/
            break;

        case RTDRV_TABLE_WRITE:
            copy_from_user(&buf.tbl_cfg, user, sizeof(rtdrv_tblCfg_t));
            ret = table_write(buf.tbl_cfg.unit, buf.tbl_cfg.table, buf.tbl_cfg.addr, buf.tbl_cfg.value);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_set_ctl_counter(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** COUNTER **/
        case RTDRV_COUNTER_GLOBAL_RESET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_global_reset(buf.counter_cfg.unit);
            break;

        case RTDRV_COUNTER_PORT_RESET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_port_reset(buf.counter_cfg.unit, buf.counter_cfg.port);
            break;

        case RTDRV_COUNTER_TAGLENCNT_SET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_tagLenCntIncEnable_set(buf.counter_cfg.unit, buf.counter_cfg.tagCnt_type, buf.counter_cfg.enable);
            break;

        case RTDRV_COUNTER_STACKHDRLENCNT_SET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_stackHdrLenCntIncEnable_set(buf.counter_cfg.unit, buf.counter_cfg.type, buf.counter_cfg.enable);
            break;

        case RTDRV_COUNTER_FLEXCNTR_CFG_SET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_flexibleCntRange_set(buf.counter_cfg.unit, buf.counter_cfg.cntr_idx, &buf.counter_cfg.range);
            break;

        case RTDRV_COUNTER_ENABLE_SET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_enable_set(buf.counter_cfg.unit, buf.counter_cfg.enable);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_time(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** TIME **/
        case RTDRV_TIME_PORT_PTP_ENABLE_SET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_portPtpEnable_set(buf.time_cfg.unit, buf.time_cfg.port, buf.time_cfg.enable);
            break;

        case RTDRV_TIME_REF_TIME_SET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_refTime_set(buf.time_cfg.unit, buf.time_cfg.timeStamp);
            break;

        case RTDRV_TIME_REF_TIME_ADJUST_SET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_refTimeAdjust_set(buf.time_cfg.unit, buf.time_cfg.sign, buf.time_cfg.timeStamp);
            break;

        case RTDRV_TIME_REF_TIME_ENABLE_SET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_refTimeEnable_set(buf.time_cfg.unit, buf.time_cfg.enable);
            break;

        case RTDRV_TIME_REF_TIME_FREQ_SET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_refTimeFreq_set(buf.time_cfg.unit, buf.time_cfg.freq);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_trap(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** TRAP **/
        case RTDRV_TRAP_RMAACTION_SET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaAction_set(buf.trap_cfg.unit, &buf.trap_cfg.rma_frame, buf.trap_cfg.rma_action);
            break;

        case RTDRV_TRAP_BYPASS_STP_SET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_bypassStp_set(buf.trap_cfg.unit, buf.trap_cfg.bypassStp_frame, buf.trap_cfg.enable);
            break;

        case RTDRV_TRAP_BYPASS_VLAN_SET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_bypassVlan_set(buf.trap_cfg.unit, buf.trap_cfg.bypassVlan_frame, buf.trap_cfg.enable);
            break;

        case RTDRV_TRAP_USERDEFINERMA_SET:
            copy_from_user(&buf.l2_trap_cfg, user, sizeof(rtdrv_trapL2userRmaCfg_t));
            ret = rtk_trap_userDefineRma_set(buf.l2_trap_cfg.unit, buf.l2_trap_cfg.rma_index, &buf.l2_trap_cfg.rma_frame);
            break;

        case RTDRV_TRAP_USERDEFINERMAENABLE_SET:
            copy_from_user(&buf.l2_trap_cfg, user, sizeof(rtdrv_trapL2userRmaCfg_t));
            ret = rtk_trap_userDefineRmaEnable_set(buf.l2_trap_cfg.unit, buf.l2_trap_cfg.rma_index, buf.l2_trap_cfg.enable);
            break;

        case RTDRV_TRAP_USERDEFINERMAACTION_SET:
            copy_from_user(&buf.l2_trap_cfg, user, sizeof(rtdrv_trapL2userRmaCfg_t));
            ret = rtk_trap_userDefineRmaAction_set(buf.l2_trap_cfg.unit, buf.l2_trap_cfg.rma_index, buf.l2_trap_cfg.rma_action);
            break;

        case RTDRV_TRAP_MGMTFRAMEACTION_SET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFrameAction_set(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.frameType, buf.mgm_trap_cfg.rma_action);
            break;

        case RTDRV_TRAP_MGMTFRAMEPRI_SET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFramePri_set(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.frameType, buf.mgm_trap_cfg.priority);
            break;

        case RTDRV_TRAP_MGMTFRAMEQID_SET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFrameQueue_set(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.qType, buf.mgm_trap_cfg.qid);
            break;

        case RTDRV_TRAP_PORTMGMTFRAMEACTION_SET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_portMgmtFrameAction_set(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.port,
                        buf.mgm_trap_cfg.frameType, buf.mgm_trap_cfg.rma_action);
            break;

        case RTDRV_TRAP_PKTWITHCFIACTION_SET:
            copy_from_user(&buf.other_trap_cfg, user, sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_pktWithCFIAction_set(buf.other_trap_cfg.unit,
                    buf.other_trap_cfg.action);
            break;

        case RTDRV_TRAP_PKTWITHOUTERCFIACTION_SET:
            copy_from_user(&buf.other_trap_cfg, user, sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_pktWithOuterCFIAction_set(buf.other_trap_cfg.unit,
                    buf.other_trap_cfg.action);
            break;

        case RTDRV_TRAP_PKTWITHCFIPRI_SET:
            copy_from_user(&buf.other_trap_cfg, user, sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_pktWithCFIPri_set(buf.other_trap_cfg.unit,
                    buf.other_trap_cfg.priority);
            break;
#ifdef CONFIG_SDK_DRIVER_RTK_LEGACY_API
        case RTDRV_TRAP_CFMUNKNOWNFRAMEACT_SET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_trap_cfmUnknownFrameAct_set(buf.misc_cfg.unit,
                                                  buf.misc_cfg.action);
            break;

        case RTDRV_TRAP_CFMLOOPBACKACT_SET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmLoopbackLinkTraceAct_set(buf.cfm_trap_cfg.unit,
                                             buf.cfm_trap_cfg.md_level,
                                             buf.cfm_trap_cfg.action);
            break;

        case RTDRV_TRAP_CFMCCMACT_SET:
            copy_from_user(&buf.oam_trap_cfg, user, sizeof(rtdrv_trapOamCfg_t));
            ret = rtk_trap_cfmCcmAct_set(buf.oam_trap_cfg.unit,
                                         buf.oam_trap_cfg.md_level,
                                         buf.oam_trap_cfg.action);
            break;

        case RTDRV_TRAP_CFMETHDMACT_SET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmEthDmAct_set(buf.cfm_trap_cfg.unit,
                                           buf.cfm_trap_cfg.md_level,
                                           buf.cfm_trap_cfg.action);
            break;
#endif  /* CONFIG_SDK_DRIVER_RTK_LEGACY_API */
        case RTDRV_TRAP_CFMFRAMETRAPPRI_SET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmFrameTrapPri_set(buf.cfm_trap_cfg.unit, buf.cfm_trap_cfg.priority);
            break;

        case RTDRV_TRAP_OAMPDUACTION_SET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_oamPDUAction_set(buf.cfm_trap_cfg.unit, buf.cfm_trap_cfg.action);
            break;

        case RTDRV_TRAP_OAMPDUPRI_SET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_oamPDUPri_set(buf.cfm_trap_cfg.unit, buf.cfm_trap_cfg.priority);
            break;

        case RTDRV_TRAP_PORTOAMLOOPBACKPARACTION_SET:
            copy_from_user(&buf.oam_trap_cfg, user, sizeof(rtdrv_trapOamCfg_t));
            ret = rtk_trap_portOamLoopbackParAction_set(buf.oam_trap_cfg.unit,
                    buf.oam_trap_cfg.port, buf.oam_trap_cfg.action);
            break;

        case RTDRV_TRAP_ROUTEEXCEPTIONACTION_SET:
            copy_from_user(&buf.routeException_trap_cfg, user,
                    sizeof(rtdrv_trapRouteExceptionCfg_t));
            ret = rtk_trap_routeExceptionAction_set(
                    buf.routeException_trap_cfg.unit,
                    buf.routeException_trap_cfg.type,
                    buf.routeException_trap_cfg.action);
            break;

        case RTDRV_TRAP_ROUTEEXCEPTIONPRI_SET:
            copy_from_user(&buf.routeException_trap_cfg, user,
                    sizeof(rtdrv_trapRouteExceptionCfg_t));
            ret = rtk_trap_routeExceptionPri_set(
                    buf.routeException_trap_cfg.unit,
                    buf.routeException_trap_cfg.type,
                    buf.routeException_trap_cfg.priority);
            break;

        case RTDRV_TRAP_USERDEFINERMALEARNINGENABLE_SET:
            copy_from_user(&buf.mgmuser_trap_cfg, user,
                    sizeof(rtdrv_trapUserMgmRmaCfg_t));
            ret = rtk_trap_userDefineRmaLearningEnable_set(
                    buf.mgmuser_trap_cfg.unit, buf.mgmuser_trap_cfg.mgmt_idx,
                    buf.mgmuser_trap_cfg.enable);
            break;

        case RTDRV_TRAP_RMALEARNINGENABLE_SET:
            copy_from_user(&buf.trap_cfg, user,
                    sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaLearningEnable_set(
                    buf.trap_cfg.unit, &buf.trap_cfg.rma_frame,
                    buf.trap_cfg.enable);
            break;

        case RTDRV_TRAP_MGMTFRAMELEARNINGENABLE_SET:
            copy_from_user(&buf.mgm_trap_cfg, user,
                    sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFrameLearningEnable_set(buf.mgm_trap_cfg.unit,
                    buf.mgm_trap_cfg.frameType, buf.mgm_trap_cfg.enable);
            break;

        case RTDRV_TRAP_MGMTFRAMEMGMTVLANENABLE_SET:
            copy_from_user(&buf.other_trap_cfg, user,
                    sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_mgmtFrameMgmtVlanEnable_set(
                    buf.other_trap_cfg.unit, buf.other_trap_cfg.enable);
            break;

        case RTDRV_TRAP_BPDUFLOODPORTMASK_SET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_bpduFloodPortmask_set(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            break;

        case RTDRV_TRAP_EAPOLFLOODPORTMASK_SET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_eapolFloodPortmask_set(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            break;

        case RTDRV_TRAP_LLDPFLOODPORTMASK_SET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_lldpFloodPortmask_set(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            break;

        case RTDRV_TRAP_USERDEFINEFLOODPORTMASK_SET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_userDefineFloodPortmask_set(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            break;

        case RTDRV_TRAP_RMAFLOODPORTMASK_SET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_rmaFloodPortmask_set(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            break;

        case RTDRV_TRAP_RMACANCELMIRROR_SET:
            copy_from_user(&buf.trap_cfg, user,sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaCancelMirror_set(buf.trap_cfg.unit, buf.trap_cfg.enable);
            break;

        case RTDRV_TRAP_RMAGROUPACTION_SET:
            copy_from_user(&buf.rma_grp_act_cfg, user,
                    sizeof(rtdrv_rmaGroupType_t));
            ret = rtk_trap_rmaGroupAction_set(
                    buf.rma_grp_act_cfg.unit, buf.rma_grp_act_cfg.rmaGroup_frameType, buf.rma_grp_act_cfg.rma_action);
            break;

        case RTDRV_TRAP_RMAGROUPLEARNINGENABLE_SET:
            copy_from_user(&buf.rma_grp_lrn_cfg, user,
                    sizeof(rtdrv_rmaGroupLearn_t));
            ret = rtk_trap_rmaGroupLearningEnable_set(
                    buf.rma_grp_lrn_cfg.unit, buf.rma_grp_lrn_cfg.rmaGroup_frameType, buf.rma_grp_lrn_cfg.enable);
            break;

        case RTDRV_TRAP_MGMTFRAMESELFARPENABLE_SET:
            copy_from_user(&buf.other_trap_cfg, user,
                    sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_mgmtFrameSelfARPEnable_set(
                    buf.other_trap_cfg.unit, buf.other_trap_cfg.enable);
            break;

        case RTDRV_TRAP_RMALOOKUPMISSACTIONENABLE_SET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaLookupMissActionEnable_set(buf.trap_cfg.unit, buf.trap_cfg.enable);
            break;

        case RTDRV_TRAP_CFMACT_SET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_cfmAct_set(buf.cfm_trap_cfg.unit, buf.cfm_trap_cfg.type,
                    buf.cfm_trap_cfg.md_level, buf.cfm_trap_cfg.action);
            break;

        case RTDRV_TRAP_CFMTARGET_SET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_cfmTarget_set(buf.cfm_trap_cfg.unit, buf.cfm_trap_cfg.target);
            break;

        case RTDRV_TRAP_OAMTARGET_SET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_oamTarget_set(buf.trap_cfg.unit, buf.trap_cfg.target);
            break;

        case RTDRV_TRAP_MGMTFRAMETARGET_SET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_mgmtFrameTarget_set(buf.trap_cfg.unit, buf.trap_cfg.target);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_acl(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** ACL **/
        case RTDRV_ACL_ENTRY_DATA_WRITE:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntryField_write(
                    buf.acl_cfg.unit,
                    buf.acl_cfg.phase,
                    buf.acl_cfg.index,
                    buf.acl_cfg.field_type,
                    buf.acl_cfg.field_data,
                    buf.acl_cfg.field_mask);
            break;

        case RTDRV_ACL_RULEVALIDATE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleValidate_set(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, buf.acl_cfg.status);
            break;

        case RTDRV_ACL_RULEENTRY_WRITE:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntry_write(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, buf.acl_cfg.entry_buffer);
            break;

        case RTDRV_ACL_RULEENTRYFIELD_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntryField_set(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, buf.acl_cfg.entry_buffer,
                    buf.acl_cfg.field_type, buf.acl_cfg.field_data,
                    buf.acl_cfg.field_mask);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEOPERATION_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleOperation_set(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, &buf.acl_cfg.oper);
            break;

        case RTDRV_ACL_RULEACTION_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleAction_set(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, &buf.acl_cfg.action);
            break;

        case RTDRV_ACL_BLOCKPWRENABLE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_blockPwrEnable_set(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, buf.acl_cfg.status);
            break;

        case RTDRV_ACL_BLOCKAGGREGATORENABLE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_blockGroupEnable_set(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, buf.acl_cfg.blk_group,
                    buf.acl_cfg.status);
            break;

        case RTDRV_ACL_STATPKTCNT_CLEAR:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_statPktCnt_clear(buf.acl_cfg.unit, buf.acl_cfg.index);
            break;

        case RTDRV_ACL_STATBYTECNT_CLEAR:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_statByteCnt_clear(buf.acl_cfg.unit, buf.acl_cfg.index);
            break;

        case RTDRV_ACL_STAT_CLEARALL:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_stat_clearAll(buf.acl_cfg.unit);
            break;

        case RTDRV_ACL_RANGECHECKL4PORT_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckL4Port_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_l4Port);
            break;

        case RTDRV_ACL_RANGECHECKVID_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckVid_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_vid);
            break;

        case RTDRV_ACL_METER_MODE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_meterMode_set(buf.acl_cfg.unit, buf.acl_cfg.blockIdx, buf.acl_cfg.meterMode);
            break;

        case RTDRV_ACL_METER_BURST_SIZE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_meterBurstSize_set(buf.acl_cfg.unit,
                    buf.acl_cfg.meterMode, &buf.acl_cfg.burstSize);
            break;

        case RTDRV_ACL_RANGECHECKSRCPORT_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckSrcPort_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_port);
            break;

        case RTDRV_ACL_RANGECHECKPACKETLEN_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckPacketLen_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_pktLen);
            break;
        case RTDRV_ACL_LOOPBACKENABLE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_loopBackEnable_set(buf.acl_cfg.unit, buf.acl_cfg.enable);
            break;
        case RTDRV_ACL_LIMITLOOPBACKTIMES_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_limitLoopbackTimes_set(buf.acl_cfg.unit, buf.acl_cfg.lb_times);
            break;
        case RTDRV_ACL_PORTLOOKUPENABLE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_portLookupEnable_set(buf.acl_cfg.unit,
                    buf.acl_cfg.port, buf.acl_cfg.status);
            break;

        case RTDRV_ACL_LOOKUPMISSACT_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_lookupMissAct_set(buf.acl_cfg.unit,
                    buf.acl_cfg.port, buf.acl_cfg.lmAct);
            break;

        case RTDRV_ACL_RANGECHECKFIELDSEL_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckFieldSelector_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_fieldSel);
            break;
        case RTDRV_ACL_PARTITION_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_partition_set(buf.acl_cfg.unit, buf.acl_cfg.blockIdx);
            break;

        case RTDRV_ACL_TEMPLATEFIELDINTENTVLANTAG_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_templateFieldIntentVlanTag_set(buf.acl_cfg.unit, buf.acl_cfg.tagType);
            break;

        case RTDRV_ACL_RANGECHECKDSTPORT_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckDstPort_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_port);
            break;

        case RTDRV_ACL_BLOCKRESULTMODE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_blockResultMode_set(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, buf.acl_cfg.blk_mode);
            break;

        case RTDRV_ACL_PORTPHASELOOKUPENABLE_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_portPhaseLookupEnable_set(buf.acl_cfg.unit,
                    buf.acl_cfg.port, buf.acl_cfg.phase, buf.acl_cfg.status);
            break;

        case RTDRV_ACL_TEMPLATESELECTOR_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_templateSelector_set(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, buf.acl_cfg.template_idx);
            break;

        case RTDRV_ACL_STATCNT_CLEAR:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_statCnt_clear(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, buf.acl_cfg.mode);
            break;

        case RTDRV_ACL_RULE_DEL:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_rule_del(buf.acl_cfg.unit, buf.acl_cfg.phase, &buf.acl_cfg.clrIdx);
            break;

        case RTDRV_ACL_RULE_MOVE:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_rule_move(buf.acl_cfg.unit, buf.acl_cfg.phase, &buf.acl_cfg.mv);
            break;

        case RTDRV_ACL_RULEENTRYFIELD_VALIDATE:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntryField_validate(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, buf.acl_cfg.field_type);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_pie(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** PIE **/
        case RTDRV_PIE_METER_INCLUDE_IFG_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterIncludeIfg_set(buf.pie_cfg.unit, buf.pie_cfg.ifg_include);
            break;

        case RTDRV_PIE_METER_ENTRY_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterEntry_set(buf.pie_cfg.unit, buf.pie_cfg.meterIdx, &buf.pie_cfg.meterEntry);
            break;

        case RTDRV_PIE_BLOCKLOOKUPENABLE_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_blockLookupEnable_set(buf.pie_cfg.unit,
                    buf.pie_cfg.blockIdx, buf.pie_cfg.status);
            break;

        case RTDRV_PIE_BLOCKGROUPING_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_blockGrouping_set(buf.pie_cfg.unit,
                    buf.pie_cfg.blockIdx, buf.pie_cfg.grpId, buf.pie_cfg.logicId);
            break;

        case RTDRV_PIE_TEMPLATE_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_template_set(buf.pie_cfg.unit, buf.pie_cfg.index,
                    &buf.pie_cfg.template);
            break;

        case RTDRV_PIE_RANGECHECKIP_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_pie_rangeCheckIp_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_ip);
            break;

        case RTDRV_PIE_RANGECHECK_SET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_pie_rangeCheck_set(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range);
            break;

        case RTDRV_PIE_FIELDSELECTOR_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_fieldSelector_set(buf.pie_cfg.unit,
                    buf.pie_cfg.index, &buf.pie_cfg.fs);
            break;

        case RTDRV_PIE_PHASE_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_phase_set(buf.pie_cfg.unit, buf.pie_cfg.blockIdx, buf.pie_cfg.phase);
            break;

        case RTDRV_PIE_TEMPLATE_VLANSEL_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_templateVlanSel_set(buf.pie_cfg.unit, buf.pie_cfg.phase, buf.pie_cfg.templateIdx, buf.pie_cfg.vlanSel);
            break;

        case RTDRV_PIE_METER_DPSEL_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterDpSel_set(buf.pie_cfg.unit, buf.pie_cfg.dpSel);
            break;

        case RTDRV_PIE_ARPMACSEL_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_arpMacSel_set(buf.pie_cfg.unit, buf.pie_cfg.arpMacSel);
            break;

        case RTDRV_PIE_INTFSEL_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_intfSel_set(buf.pie_cfg.unit, buf.pie_cfg.intfSel);
            break;

        case RTDRV_PIE_TEMPLATEVLANFMTSEL_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_templateVlanFmtSel_set(buf.pie_cfg.unit, buf.pie_cfg.phase, buf.pie_cfg.templateIdx, buf.pie_cfg.vlanFmtSel);
            break;

        case RTDRV_PIE_METERTRTCMTYPE_SET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterTrtcmType_set(buf.pie_cfg.unit, buf.pie_cfg.type);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_qos(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** QOS **/
        case RTDRV_QOS_QUEUE_NUM_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_queueNum_set(buf.qos_cfg.unit, buf.qos_cfg.queue_num);
            break;

        case RTDRV_QOS_PRI_MAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priMap_set(buf.qos_cfg.unit, buf.qos_cfg.queue_num, &buf.qos_cfg.pri2qid);
            break;

        case RTDRV_QOS_PRI2QID_MAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_pri2QidMap_set(buf.qos_cfg.unit, buf.qos_cfg.int_pri, buf.qos_cfg.queue);
            break;

        case RTDRV_QOS_CPUQID2QID_MAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_cpuQid2QidMap_set(buf.qos_cfg.unit, buf.qos_cfg.cpuQid, buf.qos_cfg.queue);
            break;

        case RTDRV_QOS_CPUQID2SQID_MAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_cpuQid2StackQidMap_set(buf.qos_cfg.unit, buf.qos_cfg.cpuQid, buf.qos_cfg.queue);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_1P_PRI_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pPriRemap_set(buf.qos_cfg.unit, buf.qos_cfg.dot1p_pri, buf.qos_cfg.int_pri);
            break;

        case RTDRV_QOS_OUTER_1P_PRI_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pPriRemap_set(buf.qos_cfg.unit, buf.qos_cfg.dot1p_pri, buf.qos_cfg.dei, buf.qos_cfg.int_pri);
            break;
#endif

        case RTDRV_QOS_DEI_DP_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiDpRemap_set(buf.qos_cfg.unit, buf.qos_cfg.dei, buf.qos_cfg.dp);
            break;

        case RTDRV_QOS_PORT_DEI_SRC_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDEISrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.deiSrc);
            break;

        case RTDRV_QOS_PORT_DP_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDpSel_set(buf.qos_cfg.unit, buf.qos_cfg.port, &(buf.qos_cfg.weightOfDpSel));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_DSCP_DP_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpDpRemap_set(buf.qos_cfg.unit, buf.qos_cfg.dscp, buf.qos_cfg.dp);
            break;
#endif
        case RTDRV_QOS_DP_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dpRemap_set(buf.qos_cfg.unit, buf.qos_cfg.dpSrcType, buf.qos_cfg.dpSrcRemap, buf.qos_cfg.dp);
            break;

        case RTDRV_QOS_PRI_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priRemap_set(buf.qos_cfg.unit, buf.qos_cfg.priSrcType, buf.qos_cfg.priSrcRemap, buf.qos_cfg.int_pri);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_DSCP_PRI_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpPriRemap_set(buf.qos_cfg.unit, buf.qos_cfg.dscp, buf.qos_cfg.int_pri);
            break;

        case RTDRV_QOS_PORT_PRI_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPri_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.int_pri);
            break;

        case RTDRV_QOS_PORT_INNER_PRI_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portInnerPri_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.int_pri);
            break;

        case RTDRV_QOS_PORT_OUTER_PRI_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuterPri_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.int_pri);
            break;
#endif

        case RTDRV_QOS_DP_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dpSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.dpSrcType);
            break;

        case RTDRV_QOS_PRI_SEL_GROUP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priSelGroup_set(buf.qos_cfg.unit, buf.qos_cfg.index, &(buf.qos_cfg.priSelWeight));
            break;

        case RTDRV_QOS_PORT_PRI_SEL_GROUP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPriSelGroup_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.index);
            break;

        case RTDRV_QOS_PORT_1P_REMARK_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pRemarkEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.remark_enable);
            break;

        case RTDRV_QOS_1P_REMARKING_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pRemarking_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_1p, buf.qos_cfg.rmkval_1p, buf.qos_cfg.dot1p_pri);
            break;

        case RTDRV_QOS_1P_REMARK_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pRemarkSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_1p);
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPri_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.dot1p_dflt_pri);
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_EXT_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPriExt_set(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, buf.qos_cfg.dot1p_dflt_pri);
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPriSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.dflt_src_1p);
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_SRC_SEL_EXT_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPriSrcSelExt_set(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, buf.qos_cfg.dflt_src_1p);
            break;

        case RTDRV_QOS_1P_DFLT_PRI_CFG_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pDfltPriCfgSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.dot1p_dflt_cfg_dir);
            break;

        case RTDRV_QOS_PORT_OUT_1P_REMARK_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOut1pRemarkEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.remark_enable);
            break;

        case RTDRV_QOS_OUTER_1P_REMARKING_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pRemarking_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_outer1p, buf.qos_cfg.rmkval_outer1p, buf.qos_cfg.dot1p_pri);
            break;

        case RTDRV_QOS_OUT_1P_REMARK_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pRemarkSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_outer1p);
            break;

        case RTDRV_QOS_PORT_OUTER_1P_DFLT_PRI_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPri_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.out1p_dflt_pri);
            break;

        case RTDRV_QOS_PORT_OUTER_1P_DFLT_PRI_EXT_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPriExt_set(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, buf.qos_cfg.out1p_dflt_pri);
            break;

        case RTDRV_QOS_OUTER_1P_DFLT_PRI_CFG_SRC_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pDfltPriCfgSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.out1p_dflt_cfg_dir);
            break;

        case RTDRV_QOS_PORT_DSCP_REMARK_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDscpRemarkEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.remark_enable);
            break;

        case RTDRV_QOS_DSCP_REMARKING_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpRemarking_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_dscp, buf.qos_cfg.rmkval_dscp, buf.qos_cfg.dscp);
            break;

        case RTDRV_QOS_DSCP_REMARK_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpRemarkSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_dscp);
            break;

        case RTDRV_QOS_PORT_DEI_REMARK_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDeiRemarkEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.remark_enable);
            break;

        case RTDRV_QOS_DEI_REMARKING_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiRemarking_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_dei, buf.qos_cfg.rmkval_dei, buf.qos_cfg.dei);
            break;

        case RTDRV_QOS_DEI_REMARK_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiRemarkSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_dei);
            break;

        case RTDRV_QOS_PORT_DEI_REMARK_TAG_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDeiRemarkTagSel_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.deiSrc);
            break;

        case RTDRV_QOS_1P_DFLT_PRI_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pDfltPri_set(buf.qos_cfg.unit, buf.qos_cfg.dot1p_dflt_pri);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_1P_REMARK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pRemark_set(buf.qos_cfg.unit, buf.qos_cfg.int_pri, buf.qos_cfg.dot1p_pri);
            break;

        case RTDRV_QOS_OUT_1P_REMARK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pRemark_set(buf.qos_cfg.unit, buf.qos_cfg.int_pri, buf.qos_cfg.dot1p_pri);
            break;
#endif

        case RTDRV_QOS_PORT_OUT_1P_DFLT_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPriSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.out1p_dflt_src);
            break;

        case RTDRV_QOS_PORT_OUT_1P_DFLT_SRC_SEL_EXT_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPriSrcSelExt_set(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, buf.qos_cfg.out1p_dflt_src);
            break;

        case RTDRV_QOS_1P_DFLT_PRI_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pDfltPriSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.dflt_src_1p);
            break;

        case RTDRV_QOS_OUTER_1P_DFLT_PRI_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pDfltPri_set(buf.qos_cfg.unit, buf.qos_cfg.out1p_dflt_pri);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_DSCP_REMARK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpRemark_set(buf.qos_cfg.unit, buf.qos_cfg.int_pri, buf.qos_cfg.dscp);
            break;

        case RTDRV_QOS_DSCP2DSCP_REMARK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscp2DscpRemark_set(buf.qos_cfg.unit, buf.qos_cfg.org_dscp, buf.qos_cfg.dscp);
            break;

        case RTDRV_QOS_DSCP2DOT1P_REMARK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscp2Dot1pRemark_set(buf.qos_cfg.unit, buf.qos_cfg.org_dscp, buf.qos_cfg.dot1p_pri);
            break;

        case RTDRV_QOS_DSCP2OUT1P_REMARK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscp2Outer1pRemark_set(buf.qos_cfg.unit, buf.qos_cfg.org_dscp, buf.qos_cfg.dot1p_pri);
            break;

        case RTDRV_QOS_DEI_REMARK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiRemark_set(buf.qos_cfg.unit, buf.qos_cfg.dp, buf.qos_cfg.dei);
            break;
#endif

        case RTDRV_QOS_PORT_OUTER_1P_REMARK_SRC_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pRemarkSrcSel_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.rmksrc_outer1p);
            break;

        case RTDRV_QOS_SCHEDULING_ALGORITHM_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_schedulingAlgorithm_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.scheduling_type);
            break;

        case RTDRV_QOS_SCHEDULING_QUEUE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_schedulingQueue_set(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.qweights);
            break;

        case RTDRV_QOS_CONG_AVOID_ALGO_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidAlgo_set(buf.qos_cfg.unit, buf.qos_cfg.congAvoid_algo);
            break;

        case RTDRV_QOS_PORT_CONG_AVOID_ALGO_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portCongAvoidAlgo_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.congAvoid_algo);
            break;

        case RTDRV_QOS_CONG_AVOID_SYS_THRESH_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidSysThresh_set(buf.qos_cfg.unit, buf.qos_cfg.dp, &(buf.qos_cfg.congAvoid_thresh));
            break;

        case RTDRV_QOS_CONG_AVOID_SYS_DROP_PROBABILITY_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidSysDropProbability_set(buf.qos_cfg.unit, buf.qos_cfg.dp, buf.qos_cfg.data);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_CONG_AVOID_GLOBAL_QUEUE_THRESH_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidGlobalQueueThresh_set(buf.qos_cfg.unit, buf.qos_cfg.queue, buf.qos_cfg.dp, &(buf.qos_cfg.congAvoid_thresh));
            break;

        case RTDRV_QOS_CONG_AVOID_GLOBAL_QUEUE_DROP_PROBABILITY_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidGlobalQueueDropProbability_set(buf.qos_cfg.unit, buf.qos_cfg.queue, buf.qos_cfg.dp, buf.qos_cfg.data);
            break;
#endif

        case RTDRV_QOS_CONG_AVOID_GLOBAL_QUEUE_CONFIG_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidGlobalQueueConfig_set(buf.qos_cfg.unit, buf.qos_cfg.queue, buf.qos_cfg.dp, &(buf.qos_cfg.congAvoid_thresh));
            break;

        case RTDRV_QOS_AVB_SR_CLASS_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portAvbStreamReservationClassEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.srClass, buf.qos_cfg.enable);
            break;

        case RTDRV_QOS_AVB_SR_CONFIG_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_avbStreamReservationConfig_set(buf.qos_cfg.unit, &buf.qos_cfg.srConf);
            break;

        case RTDRV_QOS_PKT2CPU_PRI_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_pkt2CpuPriRemap_set(buf.qos_cfg.unit, buf.qos_cfg.int_pri, buf.qos_cfg.new_pri);
            break;

        case RTDRV_QOS_RSPAN_PRI_REMAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_rspanPriRemap_set(buf.qos_cfg.unit, buf.qos_cfg.rspan_pri, buf.qos_cfg.int_pri);
            break;

        case RTDRV_QOS_PRI2IGR_QUEUE_MAP_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPri2IgrQMap_set(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.pri2qid);
            break;

        case RTDRV_QOS_PRI2IGR_QUEUE_MAP_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPri2IgrQMapEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.enable);
            break;

        case RTDRV_QOS_PORT_IGR_QUEUE_WEIGHT_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portIgrQueueWeight_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.queue, buf.qos_cfg.data);
            break;

        case RTDRV_QOS_PORT_IGR_QUEUE_STRICT_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portIgrQueueStrictEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.queue, buf.qos_cfg.enable);
            break;

        case RTDRV_QOS_INVLD_DSCP_VAL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_invldDscpVal_set(buf.qos_cfg.unit, buf.qos_cfg.dscp);
            break;

        case RTDRV_QOS_INVLD_DSCP_MASK_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_invldDscpMask_set(buf.qos_cfg.unit, buf.qos_cfg.dscp);
            break;

        case RTDRV_QOS_PORT_INVLD_DSCP_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portInvldDscpEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.enable);
            break;

        case RTDRV_QOS_INVLD_DSCP_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_invldDscpEnable_set(buf.qos_cfg.unit, buf.qos_cfg.enable);
            break;
#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_SYS_PORT_PRI_REMAP_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPriRemapEnable_set(buf.qos_cfg.unit, buf.qos_cfg.enable);
            break;
#endif

        case RTDRV_QOS_SYS_PORT_PRI_REMAP_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_sysPortPriRemapSel_set(buf.qos_cfg.unit, buf.qos_cfg.portPriRemap_type);
            break;

        case RTDRV_QOS_PORT_PORT_PRI_REMAP_SEL_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPortPriRemapSel_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.portPriRemap_type);
            break;

        case RTDRV_QOS_PORT_IPRI_REMAP_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portInnerPriRemapEnable_set(buf.qos_cfg.unit, buf.qos_cfg.enable);
            break;

        case RTDRV_QOS_PORT_OPRI_REMAP_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuterPriRemapEnable_set(buf.qos_cfg.unit, buf.qos_cfg.enable);
            break;

        case RTDRV_QOS_PRI_REMAP_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priRemapEnable_set(buf.qos_cfg.unit, buf.qos_cfg.priSrcType, buf.qos_cfg.enable);
            break;

        case RTDRV_QOS_PORT_QUEUE_STRICT_ENABLE_SET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portQueueStrictEnable_set(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.queue, buf.qos_cfg.enable);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_trunk(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** TRUNK **/
        case RTDRV_TRUNK_MODE_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_mode_set(buf.trunk_cfg.unit, buf.trunk_cfg.mode);
            break;

        case RTDRV_TRUNK_PORT_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_port_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, &buf.trunk_cfg.trk_member);
            break;

        case RTDRV_TRUNK_LOCAL_PORT_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_localPort_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, &buf.trunk_cfg.trk_member);
            break;

        case RTDRV_TRUNK_EGR_PORT_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_egrPort_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, &buf.trunk_cfg.trk_egr_ports);
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_BIND_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmBind_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, buf.trunk_cfg.algo_id);
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_TYPE_BIND_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmTypeBind_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, buf.trunk_cfg.bindType, buf.trunk_cfg.algo_id);
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_PARAM_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmParam_set(buf.trunk_cfg.unit, buf.trunk_cfg.algo_id, buf.trunk_cfg.algo_bitmask);
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_TYPE_PARAM_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmTypeParam_set(buf.trunk_cfg.unit, buf.trunk_cfg.paramType, buf.trunk_cfg.algo_id, buf.trunk_cfg.algo_bitmask);
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_SHIFT_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmShift_set(buf.trunk_cfg.unit, buf.trunk_cfg.algo_id, &buf.trunk_cfg.shift);
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_SHIFT_GBL_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmShiftGbl_set(buf.trunk_cfg.unit, &buf.trunk_cfg.shift);
            break;

        case RTDRV_TRUNK_TRAFFIC_SEPARATE_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_trafficSeparate_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, buf.trunk_cfg.separate);
            break;

        case RTDRV_TRUNK_TRAFFIC_SEPARATE_ENABLE_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_trafficSeparateEnable_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, buf.trunk_cfg.separate, buf.trunk_cfg.enable);
            break;

        case RTDRV_TRUNK_TRAFFIC_SEPARATE_DIVISION_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_trafficSeparateDivision_set(buf.trunk_cfg.unit, buf.trunk_cfg.enable);
            break;

        case RTDRV_TRUNK_TUNNEL_HASH_SRC_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_tunnelHashSrc_set(buf.trunk_cfg.unit, buf.trunk_cfg.tunnelHashSrc);
            break;

        case RTDRV_TRUNK_STACK_TRUNK_PORT_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_stkTrkPort_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid,  &buf.trunk_cfg.trk_member);
            break;

        case RTDRV_TRUNK_STACK_TRUNK_HASH_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_stkTrkHash_set(buf.trunk_cfg.unit, buf.trunk_cfg.stkTrkHash);
            break;

        case RTDRV_TRUNK_STACK_DIST_ALGO_TYPE_BIND_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_stkDistributionAlgorithmTypeBind_set(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid,
                buf.trunk_cfg.bindType, buf.trunk_cfg.algo_id);
            break;

        case RTDRV_TRUNK_LOCALFIRST_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_localFirst_set(buf.trunk_cfg.unit, buf.trunk_cfg.localFirst);
            break;

        case RTDRV_TRUNK_LOCALFIRSTFAILOVER_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_localFirstFailOver_set(buf.trunk_cfg.unit, buf.trunk_cfg.congstAvoid, buf.trunk_cfg.linkFailAvoid);
            break;

        case RTDRV_TRUNK_SRCPORTMAP_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_srcPortMap_set(buf.trunk_cfg.unit, buf.trunk_cfg.unitPort, buf.trunk_cfg.isTrkMbr, buf.trunk_cfg.trk_gid);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_debug(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** DEBUG **/
        case RTDRV_DEBUG_EN_LOG_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rt_log_enable_set(buf.unit_cfg.data);
            break;

        case RTDRV_DEBUG_LOGLV_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rt_log_level_set(buf.unit_cfg.data);
            break;

        case RTDRV_DEBUG_LOGLVMASK_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rt_log_mask_set(buf.unit_cfg.data);
            break;

        case RTDRV_DEBUG_LOGTYPE_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rt_log_type_set(buf.unit_cfg.data);
            break;

        case RTDRV_DEBUG_LOGFORMAT_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rt_log_format_set(buf.unit_cfg.data);
            break;

        case RTDRV_DEBUG_MODMASK_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rt_log_moduleMask_set(buf.unit_cfg.data64);
            break;

        case RTDRV_DEBUG_MEM_WRITE:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ret = debug_mem_write(buf.reg_cfg.unit, buf.reg_cfg.reg, buf.reg_cfg.value);
            break;

        case RTDRV_DEBUG_MEM_SHOW:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ret = debug_mem_show(buf.reg_cfg.unit);
            break;

        case RTDRV_DEBUG_HSB_DUMP:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_dumpHsb(buf.unit_cfg.unit);
            break;

#if defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL9300)
        case RTDRV_DEBUG_HSM_IDX_DUMP:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_dumpHsmIdx(buf.unit_cfg.unit, buf.unit_cfg.data);
            break;
#endif

#if defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9310)
        case RTDRV_DEBUG_HSM_DUMP:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_dumpHsm(buf.unit_cfg.unit);
            break;
#endif

        case RTDRV_DEBUG_HSA_DUMP:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_dumpHsa(buf.unit_cfg.unit);
            break;

#if defined(CONFIG_SDK_RTL9310)
        case RTDRV_DEBUG_HSB_OPENFLOW_DUMP:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_dumpHsb_openflow(buf.unit_cfg.unit);
            break;
        case RTDRV_DEBUG_HSM_OPENFLOW_DUMP:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_dumpHsm_openflow(buf.unit_cfg.unit);
            break;
        case RTDRV_DEBUG_HSA_OPENFLOW_DUMP:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_dumpHsa_openflow(buf.unit_cfg.unit);
            break;
        case RTDRV_DEBUG_REPCTQ_STICK_ENABLE_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_repctQueueStickEnable_set(buf.unit_cfg.unit, buf.unit_cfg.enable);
            break;
        case RTDRV_DEBUG_REPCTQ_FETCH_ENABLE_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_repctQueueFetchEnable_set(buf.unit_cfg.unit, buf.unit_cfg.enable);
            break;
        case RTDRV_DEBUG_FLOWCTRL_RPECT_QUEUE_USED_PAGE_CNT_RESET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_resetFlowCtrlRepctQueueUsedPageCnt(buf.unit_cfg.unit);
            break;
#endif


#if defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9310)
        case RTDRV_DEBUG_FLOWCTRL_PORT_USED_PAGE_CNT_INGRESS_RESET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_resetFlowCtrlIgrPortUsedPageCnt(buf.unit_cfg.unit, buf.unit_cfg.port);
            break;
        case RTDRV_DEBUG_FLOWCTRL_PORT_USED_PAGE_CNT_EGRESS_RESET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_resetFlowCtrlEgrPortUsedPageCnt(buf.unit_cfg.unit, buf.unit_cfg.port);
            break;
        case RTDRV_DEBUG_FLOWCTRL_SYSTEM_USED_PAGE_CNT_RESET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_resetFlowCtrlSystemUsedPageCnt(buf.unit_cfg.unit);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_uart(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_UART1)
        case RTDRV_UART1_PUTC:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = drv_uart_putc(buf.unit_cfg.unit, buf.unit_cfg.data8);
            break;

        case RTDRV_UART1_BAUDRATE_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = drv_uart_baudrate_set(buf.unit_cfg.unit, buf.unit_cfg.data);
            break;
        case RTDRV_UART1_INTERFACE_SET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = drv_uart_interface_set(buf.unit_cfg.unit, buf.unit_cfg.data);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_mirror(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** MIRROR **/
        case RTDRV_MIRROR_GROUP_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_group_set(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, &buf.mirror_cfg.mirrorEntry);
            break;

        case RTDRV_MIRROR_RSPAN_IGR_MODE_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_rspanIgrMode_set(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, buf.mirror_cfg.data);
            break;

        case RTDRV_MIRROR_RSPAN_EGR_MODE_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_rspanEgrMode_set(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, buf.mirror_cfg.data);
            break;

        case RTDRV_MIRROR_RSPAN_TAG_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_rspanTag_set(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, &buf.mirror_cfg.rspan_tag);
            break;

        case RTDRV_MIRROR_SFLOW_MIRROR_SAMPLE_RATE_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowMirrorSampleRate_set(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, buf.mirror_cfg.data);
            break;

        case RTDRV_MIRROR_EGRQUEUE_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_egrQueue_set(buf.mirror_cfg.unit, buf.mirror_cfg.enable, buf.mirror_cfg.qid);
            break;

        case RTDRV_MIRROR_SFLOW_PORT_IGR_SAMPLE_RATE_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowPortIgrSampleRate_set(buf.mirror_cfg.unit, buf.mirror_cfg.port, buf.mirror_cfg.data);
            break;

        case RTDRV_MIRROR_SFLOW_PORT_EGR_SAMPLE_RATE_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowPortEgrSampleRate_set(buf.mirror_cfg.unit, buf.mirror_cfg.port, buf.mirror_cfg.data);
            break;

        case RTDRV_MIRROR_SFLOW_SAMPLE_CTRL_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowSampleCtrl_set(buf.mirror_cfg.unit, buf.mirror_cfg.sample_ctrl);
            break;

        case RTDRV_MIRROR_SFLOWSAMPLETARGET_SET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowSampleTarget_set(buf.mirror_cfg.unit, buf.mirror_cfg.target);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_stack(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** STACK **/
        case RTDRV_STACK_INIT:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_init(buf.stack_cfg.unit);
            break;

        case RTDRV_STACK_PORT_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_port_set(buf.stack_cfg.unit, &buf.stack_cfg.stkPorts);
            break;

        case RTDRV_STACK_UNIT_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_unit_set(buf.stack_cfg.unit, buf.stack_cfg.unitIdx);
            break;

        case RTDRV_STACK_MASTERUNIT_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_masterUnit_set(buf.stack_cfg.unit, buf.stack_cfg.unitIdx);
            break;

        case RTDRV_STACK_LOOPGUARD_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_loopGuard_set(buf.stack_cfg.unit, buf.stack_cfg.enable);
            break;

        case RTDRV_STACK_UNITPORTMAP_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_unitPortMap_set(buf.stack_cfg.unit, buf.stack_cfg.unitIdx, &buf.stack_cfg.stkPorts);
            break;

        case RTDRV_STACK_NONUCASTBLOCKPORT_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_nonUcastBlockPort_set(buf.stack_cfg.unit, buf.stack_cfg.srcUnit, &buf.stack_cfg.stkPorts);
            break;

        case RTDRV_STACK_RMTINTRTXENABLE_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrTxEnable_set(buf.stack_cfg.unit, buf.stack_cfg.enable);
            break;

        case RTDRV_STACK_RMTINTRTXTRIGGERENABLE_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrTxTriggerEnable_set(buf.stack_cfg.unit, buf.stack_cfg.enable);
            break;

        case RTDRV_STACK_RMTINTRRXSEQCMPMARGIN_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrRxSeqCmpMargin_set(buf.stack_cfg.unit, buf.stack_cfg.margin);
            break;

        case RTDRV_STACK_RMTINTRRXFORCEUPDATEENABLE_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrRxForceUpdateEnable_set(buf.stack_cfg.unit, buf.stack_cfg.enable);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_flowctrl(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** FLOWCTRL **/
        case RTDRV_FLOWCTRL_INIT:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_init(buf.flowctrl_cfg.unit);
            break;

        case RTDRV_FLOWCTRL_PORT_PAUSEON_ACTION_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portPauseOnAction_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.pauseOn_action);
            break;

        case RTDRV_FLOWCTRL_PORT_PAUSEON_ALLOWED_PAGENUM_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portPauseOnAllowedPageNum_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.data);
            break;

        case RTDRV_FLOWCTRL_PAUSEON_ALLOWED_PKTLEN_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_pauseOnAllowedPktLen_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.data);
            break;

        case RTDRV_FLOWCTRL_PAUSEON_ALLOWED_PKTNUM_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_pauseOnAllowedPktNum_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.data);
            break;

        case RTDRV_FLOWCTRL_IGR_SYS_PAUSE_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrSystemPauseThresh_set(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            break;

        case RTDRV_FLOWCTRL_IGR_JUMBO_SYS_PAUSE_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrJumboSystemPauseThresh_set(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            break;

        case RTDRV_FLOWCTRL_IGR_PAUSE_THR_GROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrPauseThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.thresh);
            break;

        case RTDRV_FLOWCTRL_PORT_IGR_PORT_THR_GROUP_SEL_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portIgrPortThreshGroupSel_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.grp_idx);
            break;

        case RTDRV_FLOWCTRL_IGR_SYS_CONGEST_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrSystemCongestThresh_set(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            break;

        case RTDRV_FLOWCTRL_IGR_JUMBO_SYS_CONGEST_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrJumboSystemCongestThresh_set(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            break;

        case RTDRV_FLOWCTRL_IGR_CONGEST_THR_GROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrCongestThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.thresh);
            break;

        case RTDRV_FLOWCTRL_EGR_SYS_DROP_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrSystemDropThresh_set(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_DROP_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortDropThresh_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_DROP_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortDropEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_QUEUE_DROP_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortQueueDropEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_EGR_QUEUE_DROP_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrQueueDropThresh_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_EGR_CPU_QUEUE_DROP_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrCpuQueueDropThresh_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_PORT_EGR_DROP_REFCONGEST_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portEgrDropRefCongestEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_DROP_THR_GROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortDropThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_EGR_QUEUE_DROP_THR_GROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrQueueDropThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_PORT_EGR_DROP_THR_GROUP_SEL_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portEgrDropThreshGroupSel_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.grp_idx);
            break;

        case RTDRV_FLOWCTRL_EGR_QUEUE_DROP_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrQueueDropEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.queue, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_PORT_EGR_QUEUE_DROP_FORCE_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portEgrQueueDropForceEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.queue, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_DROP_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueueDropEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.queue, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_PAUSE_DROP_THR_GROUP_SEL_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueuePauseDropThreshGroupSel_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.grp_idx);
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_DROP_THR_GROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueueDropThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, buf.flowctrl_cfg.queue,&buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_PAUSE_THR_GROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueuePauseThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_DROP_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueueDropThresh_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.queue,&buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_PORT_HOL_TRAFFIC_DROP_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portHolTrafficDropEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_HOL_TRAFFIC_TYPE_DROP_ENABLE_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_holTrafficTypeDropEnable_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.type, buf.flowctrl_cfg.enable);
            break;

        case RTDRV_FLOWCTRL_SPECIAL_CONGEST_THR_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_specialCongestThreshold_set(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_E2E_CASCADE_PORT_THRESH_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_e2eCascadePortThresh_set(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_E2E_REMOTE_PORTPAUSETHRESHGROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_e2eRemotePortPauseThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_E2E_REMOTE_PORTCONGESTTHRESHGROUP_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_e2eRemotePortCongestThreshGroup_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.dropThresh);
            break;

        case RTDRV_FLOWCTRL_E2E_PORT_REMOTE_PORT_THRESH_GROUP_SEL_SET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portE2eRemotePortThreshGroupSel_set(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.grp_idx);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_rate(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** RATE **/
        case RTDRV_RATE_INIT:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_init(buf.rate_cfg.unit);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_RATE_IGR_INCLUDE_IFG_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBandwidthCtrlIncludeIfg_set(buf.rate_cfg.unit, buf.rate_cfg.ifg_include);
            break;

        case RTDRV_RATE_EGR_INCLUDE_IFG_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrBandwidthCtrlIncludeIfg_set(buf.rate_cfg.unit, buf.rate_cfg.ifg_include);
            break;

        case RTDRV_RATE_STORM_INCLUDE_IFG_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlIncludeIfg_set(buf.rate_cfg.unit, buf.rate_cfg.ifg_include);
            break;

#endif
        case RTDRV_RATE_INCLUDE_IFG_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_includeIfg_set(buf.rate_cfg.unit, buf.rate_cfg.module, buf.rate_cfg.ifg_include);
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwCtrlEnable_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_RATE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwCtrlRate_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.rate);
            break;

        case RTDRV_RATE_IGR_BWCTRL_LOW_THRESH_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBandwidthLowThresh_set(buf.rate_cfg.unit, buf.rate_cfg.thresh);
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_RATE_PORT_IGR_BWCTRL_HIGH_THRESH_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBandwidthHighThresh_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.thresh);
            break;
#endif

        case RTDRV_RATE_IGR_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBwCtrlBurstSize_set(buf.rate_cfg.unit, buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_IGR_PORT_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrPortBwCtrlBurstSize_set(buf.rate_cfg.unit,buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwCtrlBurstSize_set(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.igrBwCfg);
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_EXCEED_RESET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBandwidthCtrlExceed_reset(buf.rate_cfg.unit,buf.rate_cfg.port);
            break;

        case RTDRV_RATE_IGR_BWCTRL_BYPASS_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBandwidthCtrlBypass_set(buf.rate_cfg.unit, buf.rate_cfg.igrBypassType, buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_FLOWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwFlowctrlEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_RATE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlRate_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.rate);
            break;

        case RTDRV_RATE_IGR_QUEUE_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrQueueBwCtrlBurstSize_set(buf.rate_cfg.unit,buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlBurstSize_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_EXCEED_RESET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlExceed_reset(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue);
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_FIXED_BWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueFixedBwCtrlEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_EGR_BWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrBwCtrlEnable_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_EGR_BWCTRL_RATE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrBwCtrlRate_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.rate);
            break;

        case RTDRV_RATE_PORT_EGR_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrBwCtrlBurstSize_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_CPU_PORT_EGR_BWCTRL_RATE_MODE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_cpuEgrBandwidthCtrlRateMode_set(buf.rate_cfg.unit, buf.rate_cfg.rate_mode);
            break;

        case RTDRV_RATE_EGR_PORT_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrPortBwCtrlBurstSize_set(buf.rate_cfg.unit,buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_BWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueBwCtrlEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_BWCTRL_RATE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueBwCtrlRate_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.rate);
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueBwCtrlBurstSize_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_RATE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlRate_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, buf.rate_cfg.rate);
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlBurstSize_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_MODE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlMode_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.assured_mode);
            break;

        case RTDRV_RATE_EGR_QUEUE_FIXED_BWCTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrQueueFixedBandwidthEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_EGR_QUEUE_BWCTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrQueueBwCtrlBurstSize_set(buf.rate_cfg.unit,buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_RATE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlRate_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type,buf.rate_cfg.rate);
            break;

        case RTDRV_RATE_STORM_CTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlBurstSize_set(buf.rate_cfg.unit,buf.rate_cfg.storm_type,buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlBurstSize_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type,buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_EXCEED_RESET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlExceed_reset(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type);
            break;

        case RTDRV_RATE_STORM_CTRL_RATE_MODE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlRateMode_set(buf.rate_cfg.unit,buf.rate_cfg.storm_rate_mode);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_RATE_MODE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlRateMode_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_rate_mode);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_TYPE_SEL_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlTypeSel_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.storm_type, buf.rate_cfg.storm_sel);
            break;

        case RTDRV_RATE_STORM_CTRL_BYPASS_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlBypass_set(buf.rate_cfg.unit, buf.rate_cfg.stormBypassType, buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoEnable_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_proto_type,buf.rate_cfg.enable);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_RATE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoRate_set(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_proto_type,buf.rate_cfg.rate);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_BURST_SIZE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoBurstSize_set(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.storm_proto_type, buf.rate_cfg.burst_size);
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_EXCEED_RESET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoExceed_reset(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.storm_proto_type);
            break;

        case RTDRV_RATE_STORM_CTRL_PROTO_VLAN_CONSTRT_ENABLE_SET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormCtrlProtoVlanConstrtEnable_set(buf.rate_cfg.unit, buf.rate_cfg.storm_proto_type, buf.rate_cfg.enable);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_switch(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SWITCH **/
        case RTDRV_SWITCH_CPU_MAX_PKTLEN_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_cpuMaxPktLen_set(buf.switch_cfg.unit, buf.switch_cfg.dir, buf.switch_cfg.maxLen);
            break;

        case RTDRV_SWITCH_MAX_PKTLEN_LINK_SPEED_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_maxPktLenLinkSpeed_set(buf.switch_cfg.unit, buf.switch_cfg.speed, buf.switch_cfg.maxLen);
            break;

        case RTDRV_SWITCH_PORT_MAX_PKTLEN_LINK_SPEED_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_portMaxPktLenLinkSpeed_set(buf.switch_cfg.unit, buf.switch_cfg.port, buf.switch_cfg.speed, buf.switch_cfg.maxLen);
            break;

        case RTDRV_SWITCH_MAX_PKTLEN_TAGLENCNT_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_maxPktLenTagLenCntIncEnable_set(buf.switch_cfg.unit, buf.switch_cfg.enable);
            break;

        case RTDRV_SWITCH_SNAP_MODE_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_snapMode_set(buf.switch_cfg.unit, buf.switch_cfg.snapMode);
            break;

        case RTDRV_SWITCH_PORT_MAX_PKTLEN_TAGLENCNT_INCENABLE_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_portMaxPktLenTagLenCntIncEnable_set(buf.switch_cfg.unit, buf.switch_cfg.port, buf.switch_cfg.enable);
            break;

        case RTDRV_SWITCH_CHKSUMFAILACTION_SET:
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_chksumFailAction_set(buf.switch_cfgParam.unit, buf.switch_cfgParam.port,
                buf.switch_cfgParam.failType, buf.switch_cfgParam.action);
            break;

        case RTDRV_SWITCH_RECALCCRCENABLE_SET:
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_recalcCRCEnable_set(buf.switch_cfgParam.unit, buf.switch_cfgParam.port, buf.switch_cfgParam.enable);
            break;

        case RTDRV_SWITCH_MGMTMACADDR_SET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_mgmtMacAddr_set(buf.switch_cfgInfo.unit, &buf.switch_cfgInfo.mac);
            break;

        case RTDRV_SWITCH_IPV4ADDR_SET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_IPv4Addr_set(buf.switch_cfgInfo.unit, buf.switch_cfgInfo.ipv4Addr);
            break;

        case RTDRV_SWITCH_PPPOE_IP_PARSE_ENABLE_SET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_pppoeIpParseEnable_set(buf.switch_cfgInfo.unit, buf.switch_cfgInfo.enable);
            break;

        case RTDRV_SWITCH_WATCHDOG_ENABLE_SET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = drv_watchdog_enable_set(buf.switch_cfgInfo.unit, buf.switch_cfgInfo.enable);
            break;
        case RTDRV_SWITCH_WATCHDOG_KICK:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = drv_watchdog_kick(buf.switch_cfgInfo.unit);
            break;

        case RTDRV_SWITCH_WATCHDOG_SCALE_SET:
            copy_from_user(&buf.watchdog_cfg, user, sizeof(rtdrv_watchdogCfgInfo_t));
            ret = drv_watchdog_scale_set(buf.switch_cfgInfo.unit, buf.watchdog_cfg.scale);
            break;
        case RTDRV_SWITCH_WATCHDOG_THRESHOLD_SET:
            copy_from_user(&buf.watchdog_cfg, user, sizeof(rtdrv_watchdogCfgInfo_t));
            ret = drv_watchdog_threshold_set(buf.watchdog_cfg.unit, &buf.watchdog_cfg.threshold);
            break;

        case RTDRV_SWITCH_TC_ENABLE_SET:
            copy_from_user(&buf.tc_cfg, user, sizeof(rtdrv_tcCfgInfo_t));
            ret = drv_tc_enable_set(buf.tc_cfg.unit, buf.tc_cfg.id, buf.tc_cfg.enable);
            break;

        case RTDRV_SWITCH_TC_MODE_SET:
            copy_from_user(&buf.tc_cfg, user, sizeof(rtdrv_tcCfgInfo_t));
            ret = drv_tc_mode_set(buf.tc_cfg.unit, buf.tc_cfg.id, buf.tc_cfg.mode);
            break;

        case RTDRV_SWITCH_TC_DIVFACTOR_SET:
            copy_from_user(&buf.tc_cfg, user, sizeof(rtdrv_tcCfgInfo_t));
            ret = drv_tc_divFactor_set(buf.tc_cfg.unit, buf.tc_cfg.id, buf.tc_cfg.value);
            break;

        case RTDRV_SWITCH_TC_DATAINITVALUE_SET:
            copy_from_user(&buf.tc_cfg, user, sizeof(rtdrv_tcCfgInfo_t));
            ret = drv_tc_dataInitValue_set(buf.tc_cfg.unit, buf.tc_cfg.id, buf.tc_cfg.value);
            break;

        case RTDRV_SWITCH_PKT2CPUTYPEFORMAT_SET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_pkt2CpuTypeFormat_set(buf.switch_cfgInfo.unit,
                    buf.switch_cfgInfo.trap_type, buf.switch_cfgInfo.format);
            break;

        case RTDRV_SWITCH_CPU_PKT_TRUNCATE_EN_SET:
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_cpuPktTruncateEnable_set(buf.switch_cfgParam.unit, buf.switch_cfgParam.enable);
            break;

        case RTDRV_SWITCH_CPU_PKT_TRUNCATE_LEN_SET:
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_cpuPktTruncateLen_set(buf.switch_cfgParam.unit, buf.switch_cfgParam.maxLen);
            break;

        case RTDRV_SWITCH_FLEXTBLFMT_SET:
            copy_from_user(&buf.switch_cfgTable, user, sizeof(rtdrv_switchCfgTable_t));
            ret = rtk_switch_flexTblFmt_set(buf.switch_cfgTable.unit, buf.switch_cfgTable.tbl_fmt);
            break;

        case RTDRV_SWITCH_TAGPAUSEACTION_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_tagPauseAction_set(buf.switch_cfg.unit, buf.switch_cfg.action);
            break;

        case RTDRV_SWITCH_HALF_CONSECUTIVE_RETRY_ENABLE_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_halfConsecutiveRetryEnable_set(buf.switch_cfg.unit, buf.switch_cfg.enable);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_sys(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SYSTEM **/
        case RTDRV_SYS_HWP_UNIT_ADD:
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_set_ctl_nic(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_NIC) && defined(CONFIG_SDK_DRIVER_NIC_KERNEL_MODE)
    /** NIC **/
        case RTDRV_NIC_RX_START:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_rx_start(buf.nic_cfg.unit);
            break;

        case RTDRV_NIC_RX_STOP:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_rx_stop(buf.nic_cfg.unit);
            break;

        case RTDRV_NIC_DEBUG_SET:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_dbg_set(buf.nic_cfg.unit, buf.nic_cfg.flags);
            break;

        case RTDRV_NIC_COUNTER_DUMP:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_cntr_dump(buf.nic_cfg.unit);
            break;

        case RTDRV_NIC_COUNTER_CLEAR:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_cntr_clear(buf.nic_cfg.unit);
            break;

        case RTDRV_NIC_BUFFER_DUMP:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_ringbuf_dump(buf.nic_cfg.unit);
            break;

        case RTDRV_NIC_PKTHDR_MBUF_DUMP:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_pktHdrMBuf_dump(buf.nic_cfg.unit, buf.nic_cfg.mode, buf.nic_cfg.start,
                                              buf.nic_cfg.end, buf.nic_cfg.flags);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_sdk(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SDK **/
        case RTDRV_SDK_TEST:
            copy_from_user(&buf.sdk_cfg, user, sizeof(rtdrv_sdkCfg_t));
            if (sdkTest_drv.run != NULL)
                ret = sdkTest_drv.run(buf.sdk_cfg.unit, buf.sdk_cfg.item);
            break;

        case RTDRV_SDK_TEST_ID:
            copy_from_user(&buf.sdk_cfg, user, sizeof(rtdrv_sdkCfg_t));
            if (sdkTest_drv.run_id != NULL)
                ret = sdkTest_drv.run_id(buf.sdk_cfg.unit, buf.sdk_cfg.start, buf.sdk_cfg.end);
            break;

        case RTDRV_SDK_TEST_MODE_SET:
            copy_from_user(&buf.sdk_cfg, user, sizeof(rtdrv_sdkCfg_t));
            if (sdkTest_drv.mode_set != NULL)
                ret = sdkTest_drv.mode_set(buf.sdk_cfg.mode);
            break;
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_eee(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** EEE **/
        case RTDRV_EEE_PORT_ENABLE_SET:
            copy_from_user(&buf.eee_cfg, user, sizeof(rtdrv_eeeCfg_t));
            ret = rtk_eee_portEnable_set(buf.eee_cfg.unit, buf.eee_cfg.port, buf.eee_cfg.enable);
            break;

        case RTDRV_EEEP_PORT_ENABLE_SET:
            copy_from_user(&buf.eee_cfg, user, sizeof(rtdrv_eeeCfg_t));
            ret = rtk_eeep_portEnable_set(buf.eee_cfg.unit, buf.eee_cfg.port, buf.eee_cfg.enable);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_sec(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SEC **/
        case RTDRV_SEC_PORT_ATTACK_PREVENT_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_portAttackPrevent_set(buf.sec_cfg.unit, buf.sec_cfg.port, buf.sec_cfg.attack_type,
                                    buf.sec_cfg.action);
            break;

        case RTDRV_SEC_PORT_ATTACK_PREVENT_ENABLE_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_portAttackPreventEnable_set(buf.sec_cfg.unit, buf.sec_cfg.port, buf.sec_cfg.enable);
            break;

        case RTDRV_SEC_ATTACK_PREVENT_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_attackPreventAction_set(buf.sec_cfg.unit, buf.sec_cfg.attack_type,
                                    buf.sec_cfg.action);
            break;

        case RTDRV_SEC_MIN_IPV6_FRAG_LEN_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_minIPv6FragLen_set(buf.sec_cfg.unit, buf.sec_cfg.data);
            break;

        case RTDRV_SEC_MAX_PING_LEN_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_maxPingLen_set(buf.sec_cfg.unit, buf.sec_cfg.data);
            break;

        case RTDRV_SEC_MIN_TCP_HDR_LEN_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_minTCPHdrLen_set(buf.sec_cfg.unit, buf.sec_cfg.data);
            break;

        case RTDRV_SEC_SMURF_NETMASK_LEN_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_smurfNetmaskLen_set(buf.sec_cfg.unit, buf.sec_cfg.data);
            break;

        case RTDRV_SEC_TRAPTARGET_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_trapTarget_set(buf.sec_cfg.unit, buf.sec_cfg.target);
            break;

        case RTDRV_SEC_IPMACBINDACTION_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_ipMacBindAction_set(buf.sec_cfg.unit, buf.sec_cfg.lumisAct, buf.sec_cfg.matchAct, buf.sec_cfg.mismatchAct);
            break;

        case RTDRV_SEC_PORTIPMACBINDENABLE_SET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_portIpMacBindEnable_set(buf.sec_cfg.unit, buf.sec_cfg.port, buf.sec_cfg.type, buf.sec_cfg.enable);
            break;

        case RTDRV_SEC_IPMACBINDENTRY_ADD:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_ipMacBindEntry_add(buf.sec_cfg.unit, &buf.sec_cfg.entry);
            break;

        case RTDRV_SEC_IPMACBINDENTRY_DEL:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_ipMacBindEntry_del(buf.sec_cfg.unit, &buf.sec_cfg.entry);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_led(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** LED **/
        case RTDRV_LED_SYS_ENABLE_SET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_sysEnable_set(buf.led_cfg.unit, buf.led_cfg.type, buf.led_cfg.enable);
            break;

        case RTDRV_LED_PORTLEDENTITYSWCTRLENABLE_SET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_portLedEntitySwCtrlEnable_set(buf.led_cfg.unit,
                    buf.led_cfg.port, buf.led_cfg.entity, buf.led_cfg.enable);
            break;

        case RTDRV_LED_SWCTRL_START:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_swCtrl_start(buf.led_cfg.unit);
            break;

        case RTDRV_LED_PORTLEDENTITYSWCTRLMODE_SET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_portLedEntitySwCtrlMode_set(buf.led_cfg.unit,
                    buf.led_cfg.port, buf.led_cfg.entity, buf.led_cfg.media,
                    buf.led_cfg.mode);
            break;

        case RTDRV_LED_SYSMODE_SET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_sysMode_set(buf.led_cfg.unit, buf.led_cfg.mode);
            break;

        case RTDRV_LED_BLINKTIME_SET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_blinkTime_set(buf.led_cfg.unit, buf.led_cfg.time);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_mpls(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /* MPLS */
#if (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9310))
        case RTDRV_MPLS_INIT:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_init(buf.mpls_cfg.unit);
            break;

        case RTDRV_MPLS_TTLINHERIT_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_ttlInherit_set(buf.mpls_cfg.unit, buf.mpls_cfg.u.inherit);
            break;

        case RTDRV_MPLS_ENABLE_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_enable_set(buf.mpls_cfg.unit, buf.mpls_cfg.enable);
            break;

        case RTDRV_MPLS_TRAPTARGET_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_trapTarget_set(buf.mpls_cfg.unit, buf.mpls_cfg.u.target);
            break;

        case RTDRV_MPLS_EXCEPTIONCTRL_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_exceptionCtrl_set(buf.mpls_cfg.unit, buf.mpls_cfg.u.exceptionType, buf.mpls_cfg.action);
            break;

        case RTDRV_MPLS_NEXTHOP_CREATE:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_nextHop_create(buf.mpls_cfg.unit, &buf.mpls_cfg.u.nexthop, &buf.mpls_cfg.pathId);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_NEXTHOP_DESTROY:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_nextHop_destroy(buf.mpls_cfg.unit, buf.mpls_cfg.pathId);
            break;

        case RTDRV_MPLS_NEXTHOP_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_nextHop_set(buf.mpls_cfg.unit, buf.mpls_cfg.pathId, &buf.mpls_cfg.u.nexthop);
            break;

        case RTDRV_MPLS_ENCAP_CREATE:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_encap_create(buf.mpls_cfg.unit, &buf.mpls_cfg.u.encap, &buf.mpls_cfg.entryId);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_ENCAP_DESTROY:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_encap_destroy(buf.mpls_cfg.unit, buf.mpls_cfg.entryId);
            break;

        case RTDRV_MPLS_ENCAP_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_encap_set(buf.mpls_cfg.unit, buf.mpls_cfg.entryId, &buf.mpls_cfg.u.encap);
            break;

        case RTDRV_MPLS_HASHALGO_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_hashAlgo_set(buf.mpls_cfg.unit, buf.mpls_cfg.hashAlgo);
            break;

        case RTDRV_MPLS_DECAP_CREATE:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_decap_create(buf.mpls_cfg.unit, &buf.mpls_cfg.u.decap, &buf.mpls_cfg.entryId);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_DECAP_DESTROY:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_decap_destroy(buf.mpls_cfg.unit, buf.mpls_cfg.entryId);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_DECAP_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_decap_set(buf.mpls_cfg.unit, buf.mpls_cfg.entryId, &buf.mpls_cfg.u.decap);
            break;

        case RTDRV_MPLS_EGRTCMAP_SET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_egrTcMap_set(buf.mpls_cfg.unit, &buf.mpls_cfg.src, buf.mpls_cfg.tc);
            break;
#endif  /* (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9310)) */

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_sc(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL8295) || defined(CONFIG_SDK_RTL8295R) || defined(CONFIG_SDK_RTL8214QF)
        case RTDRV_DIAG_SC_REG_WRITE:
            copy_from_user(&buf.sc_regInfo, user, sizeof(rtdrv_scRegInfo_t));
            ret = hal_rtl8295_reg_write(buf.sc_regInfo.unit, buf.sc_regInfo.port, buf.sc_regInfo.addr, buf.sc_regInfo.data);
            break;
        case RTDRV_DIAG_SC_SDS_WRITE:
            copy_from_user(&buf.sc_sdsInfo, user, sizeof(rtdrv_scSdsInfo_t));
            ret = hal_rtl8295_sds_write(buf.sc_sdsInfo.unit, buf.sc_sdsInfo.port, buf.sc_sdsInfo.sds, buf.sc_sdsInfo.page, buf.sc_sdsInfo.reg, buf.sc_sdsInfo.data);
            break;
        case RTDRV_DIAG_SC_PATCH:
            copy_from_user(&buf.sc_patch, user, sizeof(rtdrv_scPatch_t));
            ret = phy_8295_diag_set(buf.sc_patch.unit, buf.sc_patch.port, buf.sc_patch.mdxMacId, buf.sc_patch.sds, buf.sc_patch.name);
            break;
        case RTDRV_DIAG_SC_PATCH_DBG:
            copy_from_user(&buf.sc_patchDbg, user, sizeof(rtdrv_scPatch_t));
            ret = phy_8295_patch_debugEnable_set(buf.sc_patchDbg.enable);
            break;
#endif /* defined(CONFIG_SDK_RTL8295) || defined(CONFIG_SDK_RTL8295R) || defined(CONFIG_SDK_RTL8214QF) */
#if defined(CONFIG_SDK_RTL8295R)
  #if defined(CONFIG_SDK_RTL8390)
        case RTDRV_DIAG_SC_8390_10GMEDIA_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = phy_8390_10gMedia_set(buf.port_cfg.unit, buf.port_cfg.port, buf.port_cfg.media_10g);
            break;
  #endif
        case RTDRV_DIAG_SC_8295R_RXCALICONF_SET:
            {
                phy_8295_rxCaliConf_t   rxCaliConf;

                copy_from_user(&buf.rxCaliConf, user, sizeof(rtdrv_8295r_rxCaliConf_t));
                if ((ret = phy_8295r_rxCaliConf_get(buf.rxCaliConf.unit, &rxCaliConf)) == RT_ERR_OK)
                {
                    rxCaliConf.s1rxCaliDacLongCableOffset = buf.rxCaliConf.s1rxCaliDacLongCableOffset;
                    ret = phy_8295r_rxCaliConf_set(buf.rxCaliConf.unit, &rxCaliConf);
                }
            }
            break;
#endif/* defined(CONFIG_SDK_RTL8295R) */

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_rtl8231(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL8231)
    /** RTL8231 **/
        case RTDRV_RTL8231_I2C_WRITE:
            copy_from_user(&buf.rtl8231_cfg, user, sizeof(rtdrv_rtl8231Cfg_t));
            ret = drv_rtl8231_i2c_write(buf.rtl8231_cfg.unit, buf.rtl8231_cfg.phyId_or_slaveAddr, buf.rtl8231_cfg.reg_addr, buf.rtl8231_cfg.data);
            break;

        case RTDRV_RTL8231_MDC_WRITE:
            copy_from_user(&buf.rtl8231_cfg, user, sizeof(rtdrv_rtl8231Cfg_t));
            ret = drv_rtl8231_mdc_write(buf.rtl8231_cfg.unit, buf.rtl8231_cfg.phyId_or_slaveAddr, buf.rtl8231_cfg.page, buf.rtl8231_cfg.reg_addr, buf.rtl8231_cfg.data);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_extgpio(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL8231)
        case RTDRV_EXTGPIO_DEV_INIT:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_dev_init(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, &buf.extGpio_cfg.extGpio_devConfData);
            break;

        case RTDRV_EXTGPIO_DEV_ENABLE_SET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_devEnable_set(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.data);
            break;

        case RTDRV_EXTGPIO_SYNC_ENABLE_SET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_syncEnable_set(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.data);
            break;

        case RTDRV_EXTGPIO_SYNC_START:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_sync_start(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev);
            break;

        case RTDRV_EXTGPIO_PIN_INIT:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_pin_init(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.gpioId, &buf.extGpio_cfg.extGpio_confData);
            break;

        case RTDRV_EXTGPIO_DATABIT_SET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_dataBit_set(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.gpioId, buf.extGpio_cfg.data);
            break;

        case RTDRV_EXTGPIO_REG_WRITE:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_reg_write(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.reg, buf.extGpio_cfg.data);
            break;

        case RTDRV_EXTGPIO_DIRECTION_SET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_direction_set(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.gpioId, buf.extGpio_cfg.data);
            break;
        case RTDRV_EXTGPIO_I2C_INIT:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_i2c_init(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.gpioId, buf.extGpio_cfg.data);
            break;

        case RTDRV_EXTGPIO_I2C_WRITE:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_i2c_write(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.reg, buf.extGpio_cfg.data);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_gpio(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#ifdef CONFIG_SDK_DRIVER_GPIO
    /** Internal GPIO **/
        case RTDRV_GPIO_PIN_INIT:
            copy_from_user(&buf.gpio_cfg, user, sizeof(rtdrv_gpioCfg_t));
            ret = drv_gpio_pin_init(buf.gpio_cfg.unit, buf.gpio_cfg.gpioId, buf.gpio_cfg.function, buf.gpio_cfg.direction, buf.gpio_cfg.interruptEnable);
            break;

        case RTDRV_GPIO_DATABIT_INIT:
            copy_from_user(&buf.gpio_cfg, user, sizeof(rtdrv_gpioCfg_t));
            ret = drv_gpio_dataBit_init(buf.gpio_cfg.unit, buf.gpio_cfg.gpioId, buf.gpio_cfg.data);
            break;

        case RTDRV_GPIO_DATABIT_SET:
            copy_from_user(&buf.gpio_cfg, user, sizeof(rtdrv_gpioCfg_t));
            ret = drv_gpio_dataBit_set(buf.gpio_cfg.unit, buf.gpio_cfg.gpioId, buf.gpio_cfg.data);
            break;

        case RTDRV_GENCTRL_GPIO_DEV_INIT:
            copy_from_user(&buf.genCtrlGPIO_cfg, user, sizeof(rtdrv_generalCtrlGpioCfg_t));
            ret = drv_generalCtrlGPIO_dev_init(buf.genCtrlGPIO_cfg.unit, buf.genCtrlGPIO_cfg.dev, &buf.genCtrlGPIO_cfg.genCtrl_gpioDev);
            break;
        case RTDRV_GENCTRL_GPIO_PIN_INIT:
            copy_from_user(&buf.genCtrlGPIO_cfg, user, sizeof(rtdrv_generalCtrlGpioCfg_t));
            ret = drv_generalCtrlGPIO_pin_init(buf.genCtrlGPIO_cfg.unit, buf.genCtrlGPIO_cfg.dev, buf.genCtrlGPIO_cfg.gpioId, &buf.genCtrlGPIO_cfg.genCtrl_gpioPin);
            break;
        case RTDRV_GENCTRL_GPIO_DEV_ENABLE:
            copy_from_user(&buf.genCtrlGPIO_cfg, user, sizeof(rtdrv_generalCtrlGpioCfg_t));
            ret = drv_generalCtrlGPIO_devEnable_set(buf.genCtrlGPIO_cfg.unit, buf.genCtrlGPIO_cfg.dev, buf.genCtrlGPIO_cfg.data);
            break;
        case RTDRV_GENCTRL_GPIO_DATABIT_SET:
            copy_from_user(&buf.genCtrlGPIO_cfg, user, sizeof(rtdrv_generalCtrlGpioCfg_t));
            ret = drv_generalCtrlGPIO_dataBit_set(buf.genCtrlGPIO_cfg.unit, buf.genCtrlGPIO_cfg.dev, buf.genCtrlGPIO_cfg.gpioId, buf.genCtrlGPIO_cfg.data);
            break;
#endif/* CONFIG_SDK_DRIVER_GPIO */

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_i2c(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_I2C)
        /**I2C**/
        case RTDRV_I2C_INIT:
            copy_from_user(&buf.i2c_cfg, user, sizeof(rtdrv_i2c_devCfg_t));
            ret = drv_i2c_init(buf.i2c_cfg.unit_id);
            break;
        case RTDRV_I2C_DEV_INIT:
        {
            i2c_devConf_t                   i2c_dev;

            copy_from_user(&buf.i2c_cfg, user, sizeof(rtdrv_i2c_devCfg_t));
            i2c_dev.device_id = buf.i2c_cfg.device_id;
            i2c_dev.mem_addr_width= buf.i2c_cfg.mem_addr_width;
            i2c_dev.dev_addr= buf.i2c_cfg.dev_addr;
            i2c_dev.data_width= buf.i2c_cfg.data_width;
            i2c_dev.clk_freq= buf.i2c_cfg.scl_freq;
            i2c_dev.scl_delay= buf.i2c_cfg.scl_delay;
            i2c_dev.scl_dev= buf.i2c_cfg.scl_dev;
            i2c_dev.scl_pin_id= buf.i2c_cfg.scl_pin_id;
            i2c_dev.sda_dev= buf.i2c_cfg.sda_dev;
            i2c_dev.sda_pin_id= buf.i2c_cfg.sda_pin_id;
            i2c_dev.i2c_interface_id = buf.i2c_cfg.i2c_interface_id;
            ret = drv_i2c_dev_init(buf.i2c_cfg.unit_id, &i2c_dev);
            break;
        }
        case RTDRV_I2C_WRITE:
            copy_from_user(&buf.i2c_cfg, user, sizeof(rtdrv_i2c_devCfg_t));
            ret = drv_i2c_write(buf.i2c_cfg.unit_id, buf.i2c_cfg.device_id, buf.i2c_cfg.reg_idx, &buf.i2c_cfg.rwdata);
           break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_spi(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_SPI)
        case RTDRV_SPI_WRITE:
            copy_from_user(&buf.spi_cfg, user, sizeof(rtdrv_spiCfg_t));
            ret = drv_spi_write(buf.spi_cfg.unit, buf.spi_cfg.addr, &buf.spi_cfg.data);
            break;
        case RTDRV_SPI_INIT:
            copy_from_user(&buf.spi_initInfo, user, sizeof(rtdrv_spiInitInfo_t));
            ret = drv_spiPin_init(buf.spi_initInfo.unit, &buf.spi_initInfo.init_info);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_diag(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /* DIAG */
        case RTDRV_DIAG_RTCTENABLE_SET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_diagCfg_t));
            ret = rtk_diag_rtctEnable_set(buf.diag_cfg.unit, &buf.diag_cfg.portmask);
            break;
        #if (defined(CONFIG_SDK_APP_DIAG_EXT) && defined (CONFIG_SDK_RTL9300))
         case RTDRV_DIAG_DEBUG_FIELD_SET:
             copy_from_user(&buf.diag_debug_cfg, user, sizeof(rtdrv_diag_debug_cfg_t));
             ret = rtk_diag_table_reg_field_set((uint32)buf.diag_debug_cfg.unit, &buf.diag_debug_cfg.diag_debug);
             break;
        #endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_set_ctl_openflow(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL9310)
        /* OpenFlow */
        case RTDRV_OF_INIT:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_init(buf.openflow_cfg.unit);
            break;

        case RTDRV_OF_CLASSIFIER_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_classifier_set(buf.openflow_cfg.unit, buf.openflow_cfg.classifyType, buf.openflow_cfg.classifyData);
            break;

        case RTDRV_OF_FLOWENTRYVALIDATE_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryValidate_set(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.valid);
            break;

        case RTDRV_OF_FLOWENTRYFIELD_WRITE:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryField_write(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.matchFieldType, buf.openflow_cfg.fieldData, buf.openflow_cfg.fieldMask);
            break;

        case RTDRV_OF_FLOWENTRYOPERATION_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryOperation_set(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.pOperation);
            break;

        case RTDRV_OF_FLOWENTRYINSTRUCTION_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryInstruction_set(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.flowInsData);
            break;

        case RTDRV_OF_FLOWENTRY_DEL:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntry_del(buf.openflow_cfg.unit, buf.openflow_cfg.phase, &buf.openflow_cfg.clrIdx);
            break;

        case RTDRV_OF_FLOWENTRY_MOVE:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntry_move(buf.openflow_cfg.unit, buf.openflow_cfg.phase, &buf.openflow_cfg.moveIdx);
            break;

        case RTDRV_OF_FTTEMPLATESELECTOR_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_ftTemplateSelector_set(buf.openflow_cfg.unit, buf.openflow_cfg.block_idx, buf.openflow_cfg.template_idx);
            break;

        case RTDRV_OF_FLOWCNTMODE_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowCntMode_set(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.mode);
            break;

        case RTDRV_OF_FLOWCNT_CLEAR:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowCnt_clear(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.flowCntType);
            break;

        case RTDRV_OF_FLOWCNTTHRESH_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowCntThresh_set(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.threshold);
            break;

        case RTDRV_OF_TTLEXCPT_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_ttlExcpt_set(buf.openflow_cfg.unit, buf.openflow_cfg.action);
            break;

        case RTDRV_OF_MAXLOOPBACK_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_maxLoopback_set(buf.openflow_cfg.unit, buf.openflow_cfg.times);
            break;

        case RTDRV_OF_L2FLOWTBLMATCHFIELD_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowTblMatchField_set(buf.openflow_cfg.unit, buf.openflow_cfg.l2Field);
            break;

        case RTDRV_OF_L2FLOWENTRY_ADD:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowEntry_add(buf.openflow_cfg.unit, &buf.openflow_cfg.l2Entry);
            break;

        case RTDRV_OF_L2FLOWENTRY_DEL:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowEntry_del(buf.openflow_cfg.unit, &buf.openflow_cfg.l2Entry);
            break;

        case RTDRV_OF_L2FLOWENTRY_DELALL:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowEntry_delAll(buf.openflow_cfg.unit);
            break;

        case RTDRV_OF_L2FLOWTBLHASHALGO_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowTblHashAlgo_set(buf.openflow_cfg.unit, buf.openflow_cfg.block, buf.openflow_cfg.algo);
            break;

        case RTDRV_OF_L3FLOWTBLPRI_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3FlowTblPri_set(buf.openflow_cfg.unit, buf.openflow_cfg.table);
            break;

        case RTDRV_OF_L3CAMFLOWTBLMATCHFIELD_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3CamFlowTblMatchField_set(buf.openflow_cfg.unit, buf.openflow_cfg.l3CamField);
            break;

        case RTDRV_OF_L3HASHFLOWTBLMATCHFIELD_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowTblMatchField_set(buf.openflow_cfg.unit, buf.openflow_cfg.l3HashField);
            break;

        case RTDRV_OF_L3HASHFLOWTBLHASHALGO_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowTblHashAlgo_set(buf.openflow_cfg.unit, buf.openflow_cfg.block, buf.openflow_cfg.algo);
            break;

        case RTDRV_OF_L3CAMFLOWENTRY_ADD:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3CamFlowEntry_add(buf.openflow_cfg.unit, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.l3CamEntry);
            break;

        case RTDRV_OF_L3CAMFLOWENTRY_DEL:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3CamFlowEntry_del(buf.openflow_cfg.unit, buf.openflow_cfg.idx);
            break;

        case RTDRV_OF_L3CAMFLOWENTRY_MOVE:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3CamFlowEntry_move(buf.openflow_cfg.unit, &buf.openflow_cfg.moveIdx);
            break;

        case RTDRV_OF_L3HASHFLOWENTRY_ADD:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowEntry_add(buf.openflow_cfg.unit, &buf.openflow_cfg.l3HashEntry);
            break;

        case RTDRV_OF_L3HASHFLOWENTRY_DEL:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowEntry_del(buf.openflow_cfg.unit, &buf.openflow_cfg.l3HashEntry);
            break;

        case RTDRV_OF_L3HASHFLOWENTRY_DELALL:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowEntry_delAll(buf.openflow_cfg.unit);
            break;

        case RTDRV_OF_GROUPENTRY_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_groupEntry_set(buf.openflow_cfg.unit, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.grpEntry);
            break;

        case RTDRV_OF_GROUPTBLHASHPARA_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_groupTblHashPara_set(buf.openflow_cfg.unit, &buf.openflow_cfg.para);
            break;

        case RTDRV_OF_ACTIONBUCKET_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_actionBucket_set(buf.openflow_cfg.unit, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.actionBktEntry);
            break;

        case RTDRV_OF_TRAPTARGET_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_trapTarget_set(buf.openflow_cfg.unit, buf.openflow_cfg.target);
            break;

        case RTDRV_OF_TBLMISSACTION_SET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_tblMissAction_set(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.tblMissAct);
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_capwap(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL9310)
    /* CAPWAP */
        case RTDRV_CAPWAP_INIT:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_init(buf.capwap_cfg.unit);
            break;

        case RTDRV_CAPWAP_ENABLE_SET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_enable_set(buf.capwap_cfg.unit, buf.capwap_cfg.enable);
            break;

        case RTDRV_CAPWAP_UDPPORT_SET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_udpPort_set(buf.capwap_cfg.unit, buf.capwap_cfg.ctrl_port, buf.capwap_cfg.data_port);
            break;

        case RTDRV_CAPWAP_FWDACTION_SET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_fwdAction_set(buf.capwap_cfg.unit, buf.capwap_cfg.type, buf.capwap_cfg.action);
            break;

        case RTDRV_CAPWAP_TRAPTARGET_SET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_trapTarget_set(buf.capwap_cfg.unit, buf.capwap_cfg.target);
            break;

        case RTDRV_CAPWAP_BSSIDTBLHASHALGO_SET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_bssidTblHashAlgo_set(buf.capwap_cfg.unit, buf.capwap_cfg.algo);
            break;

        case RTDRV_CAPWAP_QOSPROFILE_SET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_qosProfile_set(buf.capwap_cfg.unit, buf.capwap_cfg.index, buf.capwap_cfg.profile);
            break;

        case RTDRV_CAPWAP_TIDREMARKING_SET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_tidRemarking_set(buf.capwap_cfg.unit, buf.capwap_cfg.dp, buf.capwap_cfg.int_pri, buf.capwap_cfg.tid);
            break;

        case RTDRV_CAPWAP_BSSIDENTRY_ADD:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_bssidEntry_add(buf.capwap_cfg.unit, &buf.capwap_cfg.bssid_entry);
            break;

        case RTDRV_CAPWAP_BSSIDENTRY_DEL:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_bssidEntry_del(buf.capwap_cfg.unit, &buf.capwap_cfg.bssid_entry);
            break;

        case RTDRV_CAPWAP_UCAST_ADD:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_ucast_add(buf.capwap_cfg.unit, &buf.capwap_cfg.ucast_entry);
            break;

        case RTDRV_CAPWAP_UCAST_DEL:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_ucast_del(buf.capwap_cfg.unit, &buf.capwap_cfg.ucast_entry);
            break;

#if 0
        case RTDRV_CAPWAP_MCAST_ADD:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_mcast_add(buf.capwap_cfg.unit, &buf.capwap_cfg.mcast_entry);
            break;

        case RTDRV_CAPWAP_MCAST_DEL:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_mcast_del(buf.capwap_cfg.unit, &buf.capwap_cfg.mcast_entry);
            break;
#endif

        case RTDRV_CAPWAP_MCASTBSSID_ADD:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_mcastBssid_add(buf.capwap_cfg.unit, &buf.capwap_cfg.mcastBssid_entry);
            break;

        case RTDRV_CAPWAP_MCASTBSSID_DEL:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_mcastBssid_del(buf.capwap_cfg.unit, &buf.capwap_cfg.mcastBssid_entry);
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_set_ctl_serdes(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /* SerDes */
        case RTDRV_SERDES_REG_SET:
            copy_from_user(&buf.serdes_reg_cfg, user, sizeof(rtdrv_serdes_reg_t));
            ret = hal_serdes_reg_set(buf.serdes_reg_cfg.unit, buf.serdes_reg_cfg.sdsId, buf.serdes_reg_cfg.page, buf.serdes_reg_cfg.reg, buf.serdes_reg_cfg.data);
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


/* Function Name:
 *      do_rtdrv_set_ctl
 * Description:
 *      This function is called whenever a process tries to do setsockopt
 * Input:
 *      *sk   - network layer representation of sockets
 *      cmd   - ioctl commands
 *      *user - data buffer handled between user and kernel space
 *      len   - data length
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32 do_rtdrv_set_ctl(struct sock *sk, int cmd, void *user_in, unsigned int len)
{
    int32   ret = RT_ERR_FAILED;
    void    *user = (char *)user_in + sizeof(rtdrv_msgHdr_t);
    int     module_offset, module_index;

    module_offset = cmd & RTDRV_MODULE_MASK;
    module_index = module_offset >> RTDRV_MODULE_SHIFT;

    if ((module_index < 0) || (module_index > RTDRV_MODULE_MAX_NUM))
        return RT_ERR_FAILED;

    if (rtdrv_module_db_set[module_index].valid == TRUE)
        ret = rtdrv_module_db_set[module_index].func(cmd, user);

    copy_to_user(&((rtdrv_msgHdr_t *)user_in)->ret_code, &ret, sizeof(ret));

    return 0;
}

/* Function Name:
 *      do_rtdrv_get_ctl
 * Description:
 *      This function is called whenever a process tries to do getsockopt
 * Input:
 *      *sk   - network layer representation of sockets
 *      cmd   - ioctl commands
 * Output:
 *      *user - data buffer handled between user and kernel space
 *      len   - data length
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32 do_rtdrv_get_ctl(struct sock *sk, int cmd, void *user_in, int *len)
{
    int32                           ret = RT_ERR_FAILED;
    void                            *user = (char *)user_in + sizeof(rtdrv_msgHdr_t);

    int     module_offset, module_index;

    module_offset = cmd & RTDRV_MODULE_MASK;
    module_index = module_offset >> RTDRV_MODULE_SHIFT;

    if ((module_index < 0) || (module_index > RTDRV_MODULE_MAX_NUM))
        return RT_ERR_FAILED;

    if (rtdrv_module_db_get[module_index].valid == TRUE)
        ret = rtdrv_module_db_get[module_index].func(cmd, user);

    copy_to_user(&((rtdrv_msgHdr_t *)user_in)->ret_code, &ret, sizeof(ret));

    return 0;

}

int32 do_rtdrv_get_ctl_l2(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /* L2 */
        case RTDRV_L2_FLUSH_LINK_DOWN_PORT_ADDR_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_flushLinkDownPortAddrEnable_get(buf.l2_cfg.l2_common.unit, &buf.l2_cfg.l2_common.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_LEARNING_CNT_GET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_learningCnt_get(buf.l2_learn.unit, &buf.l2_learn.mac_cnt);
            copy_to_user(user, &buf.l2_learn, sizeof(rtdrv_l2_learnCnt_t));
            break;

        case RTDRV_L2_PORT_LEARNING_CNT_GET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_portLearningCnt_get(buf.l2_learn.unit, buf.l2_learn.port, &buf.l2_learn.mac_cnt);
            copy_to_user(user, &buf.l2_learn, sizeof(rtdrv_l2_learnCnt_t));
            break;

        case RTDRV_L2_FID_LEARNING_CNT_GET:
            copy_from_user(&buf.l2_FidLearn, user, sizeof(rtdrv_l2_learnFidCnt_t));
            ret = rtk_l2_fidLearningCnt_get(buf.l2_FidLearn.unit, buf.l2_FidLearn.entryIdx, &buf.l2_FidLearn.mac_cnt);
            copy_to_user(user, &buf.l2_FidLearn, sizeof(rtdrv_l2_learnFidCnt_t));
            break;
#endif
        case RTDRV_L2_MAC_LEARNING_CNT_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_macLearningCnt_get(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.type, &buf.l2_cfg.l2_macLimit.cnt);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_LIMIT_LEARNING_CNT_GET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_limitLearningCnt_get(buf.l2_learn.unit, &buf.l2_learn.mac_cnt);
            copy_to_user(user, &buf.l2_learn, sizeof(rtdrv_l2_learnCnt_t));
            break;

        case RTDRV_L2_PORT_LIMIT_LEARNING_CNT_GET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_portLimitLearningCnt_get(buf.l2_learn.unit, buf.l2_learn.port, &buf.l2_learn.mac_cnt);
            copy_to_user(user, &buf.l2_learn, sizeof(rtdrv_l2_learnCnt_t));
            break;
#endif
        case RTDRV_L2_LIMIT_LEARNING_NUM_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
             ret = rtk_l2_limitLearningNum_get(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.type, &buf.l2_cfg.l2_macLimit.cnt);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_LIMIT_LEARNING_CNT_ACT_GET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_limitLearningCntAction_get(buf.l2_learn.unit, &buf.l2_learn.action);
            copy_to_user(user, &buf.l2_learn, sizeof(rtdrv_l2_learnCnt_t));
            break;

        case RTDRV_L2_PORT_LIMIT_LEARNING_CNT_ACT_GET:
            copy_from_user(&buf.l2_learn, user, sizeof(rtdrv_l2_learnCnt_t));
            ret = rtk_l2_portLimitLearningCntAction_get(buf.l2_learn.unit, buf.l2_learn.port, &buf.l2_learn.action);
            copy_to_user(user, &buf.l2_learn, sizeof(rtdrv_l2_learnCnt_t));
            break;

        case RTDRV_L2_FID_LIMIT_LEARNING_CNT_ACT_GET:
            copy_from_user(&buf.l2_FidLearn, user, sizeof(rtdrv_l2_learnFidCnt_t));
            ret = rtk_l2_fidLearningCntAction_get(buf.l2_FidLearn.unit, &buf.l2_FidLearn.action);
            copy_to_user(user, &buf.l2_FidLearn, sizeof(rtdrv_l2_learnFidCnt_t));
            break;
#endif
        case RTDRV_L2_LIMIT_LEARNING_ACT_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_limitLearningAction_get(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.type, &buf.l2_cfg.l2_macLimit.action);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_FID_LIMIT_LEARNING_ENTRY_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_fidLimitLearningEntry_get(buf.l2_cfg.l2_macLimit.unit, buf.l2_cfg.l2_macLimit.fidLimitEntryId, &buf.l2_cfg.l2_macLimit.entry);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_AGING_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_l2_aging_get(buf.unit_cfg.unit, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
#endif
        case RTDRV_L2_AGING_TIME_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_agingTime_get(buf.l2_cfg.l2_age.unit, buf.l2_cfg.l2_age.type, &buf.l2_cfg.l2_age.ageTime);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_PORT_AGING_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portAgingEnable_get(buf.l2_cfg.l2_age.unit, buf.l2_cfg.l2_age.port, &buf.l2_cfg.l2_age.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_TRK_AGING_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_trkAgingEnable_get(buf.l2_cfg.l2_age.unit, buf.l2_cfg.l2_age.trunk, &buf.l2_cfg.l2_age.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_HASH_ALGO_GET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_hashAlgo_get(buf.l2_learnCfg.unit, &buf.l2_learnCfg.hash_algo);
            copy_to_user(user, &buf.l2_learnCfg, sizeof(rtdrv_l2_learnCfg_t));
            break;
#endif
        case RTDRV_L2_BUCKET_HASH_ALGO_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_bucketHashAlgo_get(buf.l2_cfg.l2_hash.unit, buf.l2_cfg.l2_hash.bucket, &buf.l2_cfg.l2_hash.hashAlgo);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_VLANMODE_GET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_vlanMode_get(buf.l2_learnCfg.unit, buf.l2_learnCfg.port, &buf.l2_learnCfg.vlanMode);
            copy_to_user(user, &buf.l2_learnCfg, sizeof(rtdrv_l2_learnCfg_t));
            break;

        case RTDRV_L2_LEARNING_FULL_ACT_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_learningFullAction_get(buf.l2_cfg.l2_learn.unit, &buf.l2_cfg.l2_learn.action);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_PORT_NEW_MAC_OP_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portNewMacOp_get(buf.l2_cfg.l2_learn.unit, buf.l2_cfg.l2_learn.port, &buf.l2_cfg.l2_learn.lrnMode, &buf.l2_cfg.l2_learn.action);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_ADDR_INIT:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_addr_init(buf.l2_cfg.l2_ucAddr.unit, buf.l2_cfg.l2_ucAddr.vid, &buf.l2_cfg.l2_ucAddr.mac, &buf.l2_cfg.l2_ucAddr.ucast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_ADDR_ADD:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_addr_add(buf.l2_cfg.l2_ucAddr.unit, &buf.l2_cfg.l2_ucAddr.ucast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_ADDR_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_addr_get(buf.l2_cfg.l2_ucAddr.unit, &buf.l2_cfg.l2_ucAddr.ucast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_ADDR_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_addr_set(buf.l2_cfg.l2_ucAddr.unit, &buf.l2_cfg.l2_ucAddr.ucast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_NEXT_VALID_ADDR_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_nextValidAddr_get(buf.l2_cfg.l2_ucAddr.unit, &buf.l2_cfg.l2_ucAddr.scan_idx, buf.l2_cfg.l2_ucAddr.include_static, &buf.l2_cfg.l2_ucAddr.ucast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_MCAST_ADDR_INIT:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastAddr_init(buf.l2_cfg.l2_mcAddr.unit, buf.l2_cfg.l2_mcAddr.vid, &buf.l2_cfg.l2_mcAddr.mac, &buf.l2_cfg.l2_mcAddr.mcast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_MCAST_ADDR_ADD:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastAddr_add(buf.l2_cfg.l2_mcAddr.unit, &buf.l2_cfg.l2_mcAddr.mcast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_MCAST_ADDR_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastAddr_get(buf.l2_cfg.l2_mcAddr.unit, &buf.l2_cfg.l2_mcAddr.mcast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_MCAST_ADDR_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastAddr_set(buf.l2_cfg.l2_mcAddr.unit, &buf.l2_cfg.l2_mcAddr.mcast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_NEXT_VALID_MCAST_ADDR_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_nextValidMcastAddr_get(buf.l2_cfg.l2_mcAddr.unit, &buf.l2_cfg.l2_mcAddr.scan_idx, &buf.l2_cfg.l2_mcAddr.mcast);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_IPMCMODE_GET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_ipmcMode_get(buf.l2_learnCfg.unit, &buf.l2_learnCfg.ipmcMode);
            copy_to_user(user, &buf.l2_learnCfg, sizeof(rtdrv_l2_learnCfg_t));
            break;

        case RTDRV_L2_IP_MCAST_ADDR_EX_INIT:
            copy_from_user(&buf.ipMcast_data, user, sizeof(rtdrv_l2_ipMcstAddrData_t));
            ret = rtk_l2_ipMcastAddrExt_init(buf.ipMcast_data.unit, &buf.ipMcast_data.ipMcast_hashKey, &buf.ipMcast_data.ip_m_data);
            copy_to_user(user, &buf.ipMcast_data, sizeof(rtdrv_l2_ipMcstAddrData_t));
            break;

        case RTDRV_L2_IP_MCAST_ADDR_GET:
            copy_from_user(&buf.ipMcast_data, user, sizeof(rtdrv_l2_ipMcstAddrData_t));
            ret = rtk_l2_ipMcastAddr_get(buf.ipMcast_data.unit, &buf.ipMcast_data.ip_m_data);
            copy_to_user(user, &buf.ipMcast_data, sizeof(rtdrv_l2_ipMcstAddrData_t));
            break;

        case RTDRV_L2_NEXT_VALID_IPMCASTADDR_GET:
            copy_from_user(&buf.ipMcast_data, user, sizeof(rtdrv_l2_ipMcstAddrData_t));
            ret = rtk_l2_nextValidIpMcastAddr_get(buf.ipMcast_data.unit, &buf.ipMcast_data.index, &buf.ipMcast_data.ip_m_data);
            copy_to_user(user, &buf.ipMcast_data, sizeof(rtdrv_l2_ipMcstAddrData_t));
            break;

        case RTDRV_L2_IPMC_DIP_CHK_GET:
            copy_from_user(&buf.l2_learnCfg, user, sizeof(rtdrv_l2_learnCfg_t));
            ret = rtk_l2_ipMcastAddrChkEnable_get(buf.l2_learnCfg.unit, &buf.l2_learnCfg.dip_check);
            copy_to_user(user, &buf.l2_learnCfg, sizeof(rtdrv_l2_learnCfg_t));
            break;

        case RTDRV_L2_IPMC_VLAN_COMPARE_GET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_ipMcstFidVidCompareEnable_get(buf.l2_common.unit, &buf.l2_common.value);
            copy_to_user(user, &buf.l2_common, sizeof(rtdrv_l2_common_t));
            break;

        case RTDRV_L2_IP6MCASTMODE_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_l2_ip6mcMode_get(buf.unit_cfg.unit, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_L2_HASHCAREBYTE_GET:
            copy_from_user(&buf.l2_hashCareByte, user, sizeof(rtdrv_l2_hashCareByte_t));
            ret = rtk_l2_ip6CareByte_get(buf.l2_hashCareByte.unit, buf.l2_hashCareByte.type, &buf.l2_hashCareByte.value);
            copy_to_user(user, &buf.l2_hashCareByte, sizeof(rtdrv_l2_hashCareByte_t));
            break;

        case RTDRV_L2_IP6_MCAST_ADDR_EX_INIT:
            copy_from_user(&buf.ip6Mcast_data, user, sizeof(rtdrv_l2_ip6McstAddrData_t));
            ret = rtk_l2_ip6McastAddrExt_init(buf.ip6Mcast_data.unit, &buf.ip6Mcast_data.ip6Mcast_hashKey, &buf.ip6Mcast_data.ip6_m_data);
            copy_to_user(user, &buf.ip6Mcast_data, sizeof(rtdrv_l2_ip6McstAddrData_t));
            break;

        case RTDRV_L2_IP6_MCAST_ADDR_GET:
            copy_from_user(&buf.ip6Mcast_data, user, sizeof(rtdrv_l2_ip6McstAddrData_t));
            ret = rtk_l2_ip6McastAddr_get(buf.ip6Mcast_data.unit, &buf.ip6Mcast_data.ip6_m_data);
            copy_to_user(user, &buf.ip6Mcast_data, sizeof(rtdrv_l2_ip6McstAddrData_t));
            break;

        case RTDRV_L2_IP6_MCAST_ADDR_GETNEXT:
            copy_from_user(&buf.ip6Mcast_data, user, sizeof(rtdrv_l2_ip6McstAddrData_t));
            ret = rtk_l2_nextValidIp6McastAddr_get(buf.ip6Mcast_data.unit, &buf.ip6Mcast_data.index, &buf.ip6Mcast_data.ip6_m_data);
            copy_to_user(user, &buf.ip6Mcast_data, sizeof(rtdrv_l2_ip6McstAddrData_t));
            break;

        case RTDRV_L2_MCAST_FWD_INDEX_ALLOC:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastFwdIndex_alloc(buf.l2_cfg.l2_portmask.unit, &buf.l2_cfg.l2_portmask.index);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_MCASTFWDINDEXFREECOUNT_GET:
            copy_from_user(&buf.l2_fwdEntryContent, user, sizeof(rtdrv_l2_fwdTblEntry_t));
            ret = rtk_l2_mcastFwdIndexFreeCount_get(buf.l2_fwdEntryContent.unit, &buf.l2_fwdEntryContent.freeCount);
            copy_to_user(user, &buf.l2_fwdEntryContent, sizeof(rtdrv_l2_fwdTblEntry_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_MCAST_FWD_PORTMASK_GET:
            copy_from_user(&buf.l2_fwdEntryContent, user, sizeof(rtdrv_l2_fwdTblEntry_t));
            ret = rtk_l2_mcastFwdPortmask_get(buf.l2_fwdEntryContent.unit,
                buf.l2_fwdEntryContent.entryIdx, &buf.l2_fwdEntryContent.portMask, &buf.l2_fwdEntryContent.crossVlan);
            copy_to_user(user, &buf.l2_fwdEntryContent, sizeof(rtdrv_l2_fwdTblEntry_t));
            break;
#endif
        case RTDRV_L2_MCAST_FWD_PORTMASK_ENTRY_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_mcastFwdPortmaskEntry_get(buf.l2_cfg.l2_portmask.unit, buf.l2_cfg.l2_portmask.index, &buf.l2_cfg.l2_portmask.portmask);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_L2_PORT_LEGAL_MOVETO_ACTION_GET:
            copy_from_user(&buf.l2_action, user, sizeof(rtdrv_l2_portAct_t));
            ret = rtk_l2_legalPortMoveAction_get(buf.l2_action.unit, buf.l2_action.port, &buf.l2_action.action);
            copy_to_user(user, &buf.l2_action, sizeof(rtdrv_l2_portAct_t));
            break;

        case RTDRV_L2_DYNM_PORTMOVE_FORBID_ACTION_GET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_dynamicPortMoveForbidAction_get(buf.l2_common.unit, &buf.l2_common.value);
            copy_to_user(user, &buf.l2_common, sizeof(rtdrv_l2_common_t));
            break;
#endif
        case RTDRV_L2_PORT_MOVE_ACT_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portMoveAction_get(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.type, &buf.l2_cfg.l2_portMove.action);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_PORT_MOVE_LEARN_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portMoveLearn_get(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.type, &buf.l2_cfg.l2_portMove.learn);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_LEGAL_MOVETO_FLUSH_ENABLE_GET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_legalPortMoveFlushAddrEnable_get(buf.l2_common.unit, buf.l2_common.port, &buf.l2_common.value);
            copy_to_user(user, &buf.l2_common, sizeof(rtdrv_l2_common_t));
            break;

        case RTDRV_L2_STTC_PORT_MOVE_ACTION_GET:
            copy_from_user(&buf.l2_action, user, sizeof(rtdrv_l2_portAct_t));
            ret = rtk_l2_staticPortMoveAction_get(buf.l2_action.unit, buf.l2_action.port, &buf.l2_action.action);
            copy_to_user(user, &buf.l2_action, sizeof(rtdrv_l2_portAct_t));
            break;

        case RTDRV_L2_LOOKUP_MISS_FLOOD_PORTMASK_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_lookupMissFloodPortMask_get(buf.l2_cfg.l2_lookUpMiss.unit, buf.l2_cfg.l2_lookUpMiss.type, &buf.l2_cfg.l2_lookUpMiss.flood_portmask);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_LOOKUP_MISS_FLOODPORTMASK_IDX_GET:
            copy_from_user(&buf.l2_lkMiss, user, sizeof(rtdrv_l2_lkMiss_t));
            ret = rtk_l2_lookupMissFloodPortMaskIdx_get(buf.l2_lkMiss.unit, buf.l2_lkMiss.type, &buf.l2_lkMiss.index);
            copy_to_user(user, &buf.l2_lkMiss, sizeof(rtdrv_l2_lkMiss_t));
            break;

        case RTDRV_L2_PORT_LOOKUP_MISS_ACTION_GET:
            copy_from_user(&buf.l2_lkMiss, user, sizeof(rtdrv_l2_lkMiss_t));
            ret = rtk_l2_portLookupMissAction_get(buf.l2_lkMiss.unit, buf.l2_lkMiss.port, buf.l2_lkMiss.type, &buf.l2_lkMiss.action);
            copy_to_user(user, &buf.l2_lkMiss, sizeof(rtdrv_l2_lkMiss_t));
            break;

        case RTDRV_L2_PORT_UCAST_LOOKUP_MISS_ACTION_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portUcastLookupMissAction_get(buf.l2_cfg.l2_lookUpMiss.unit, buf.l2_cfg.l2_lookUpMiss.port, &buf.l2_cfg.l2_lookUpMiss.action);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_SRC_PORT_EGR_FILTER_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_srcPortEgrFilterMask_get(buf.l2_cfg.l2_common.unit, &buf.l2_cfg.l2_common.srcPortFilterPortmask);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_EXCEPTION_ADDR_ACTION_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_exceptionAddrAction_get(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.exceptType, &buf.l2_cfg.l2_common.action);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_ADDR_ENTRY_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_addrEntry_get(buf.l2_cfg.l2_search.unit, buf.l2_cfg.l2_search.index, &buf.l2_cfg.l2_search.l2_entry);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_CONFLICT_ADDR_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_conflictAddr_get(buf.l2_cfg.l2_search.unit, &buf.l2_cfg.l2_search.l2_entry, buf.l2_cfg.l2_search.cfAddrList, buf.l2_cfg.l2_search.cfAddrList_size, &buf.l2_cfg.l2_search.cf_retCnt);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_ZERO_SA_LEARNING_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_zeroSALearningEnable_get(buf.l2_cfg.l2_learn.unit, &buf.l2_cfg.l2_learn.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_SECURE_MAC_MODE_GET:
            copy_from_user(&buf.l2_common, user, sizeof(rtdrv_l2_common_t));
            ret = rtk_l2_secureMacMode_get(buf.l2_common.unit, &buf.l2_common.value);
            copy_to_user(user, &buf.l2_common, sizeof(rtdrv_l2_common_t));
            break;

        case RTDRV_L2_PORT_DYNM_PORTMOVE_FORBID_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portDynamicPortMoveForbidEnable_get(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.port, &buf.l2_cfg.l2_portMove.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_TRK_DYNM_PORTMOVE_FORBID_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_trkDynamicPortMoveForbidEnable_get(buf.l2_cfg.l2_portMove.unit, buf.l2_cfg.l2_portMove.trunk, &buf.l2_cfg.l2_portMove.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_PORT_MAC_FILTER_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portMacFilterEnable_get(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.port, buf.l2_cfg.l2_common.filterMode, &buf.l2_cfg.l2_common.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_HW_NEXT_VALID_ADDR_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_hwNextValidAddr_get(buf.l2_cfg.l2_search.unit, &buf.l2_cfg.l2_search.scan_idx, buf.l2_cfg.l2_search.type, &buf.l2_cfg.l2_search.l2_entry);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_PORT_CTRL_TYPE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_portCtrl_get(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.port, buf.l2_cfg.l2_common.portCtrlType, &buf.l2_cfg.l2_common.arg);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_STATUS_TYPE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_status_get(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.stsType, buf.l2_cfg.l2_common.port, &buf.l2_cfg.l2_common.value);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_STK_LEARNING_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_stkLearningEnable_get(buf.l2_cfg.l2_learn.unit, &buf.l2_cfg.l2_learn.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_STK_KEEP_AGE_VALID_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_stkKeepUcastEntryValid_get(buf.l2_cfg.l2_age.unit, &buf.l2_cfg.l2_age.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_MCASTADDRECIDPMSK_GET:
            copy_from_user(&buf.l2Ecid_cfg, user, sizeof(rtdrv_l2EcidCfg_t));
            ret = rtk_l2_mcastAddrEcidPmsk_get(buf.l2Ecid_cfg.unit, &buf.l2Ecid_cfg.mcastAddr, &buf.l2Ecid_cfg.entry);
            copy_to_user(user, &buf.l2Ecid_cfg, sizeof(rtdrv_l2EcidCfg_t));
            break;

        case RTDRV_L2_MCASTADDRECIDPMSKNEXTVALID_GET:
            copy_from_user(&buf.l2Ecid_cfg, user, sizeof(rtdrv_l2EcidCfg_t));
            ret = rtk_l2_mcastAddrEcidPmskNextValid_get(buf.l2Ecid_cfg.unit, &buf.l2Ecid_cfg.mcastAddr, &buf.l2Ecid_cfg.entry);
            copy_to_user(user, &buf.l2Ecid_cfg, sizeof(rtdrv_l2EcidCfg_t));
            break;

        case RTDRV_L2_ENTRY_CNT_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_entryCnt_get(buf.l2_cfg.l2_common.unit, buf.l2_cfg.l2_common.entryType, &buf.l2_cfg.l2_common.value);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        case RTDRV_L2_MAC_HASHIDX_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_l2Cfg_t));
            ret = rtk_l2_hashIdx_get(buf.l2_cfg.l2_common.unit, &buf.l2_cfg.l2_common.macHashIdx);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_l2Cfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}



int32 do_rtdrv_get_ctl_l2ntfy(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_L2NTFY)
    /*L2 Notification module*/
        case RTDRV_L2NTFY_ENABLE_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_enable_get(buf.l2ntfy_cfg.unit, &buf.l2ntfy_cfg.enable);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_BACK_PRESSURE_THR_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_backPressureThresh_get(buf.l2ntfy_cfg.unit, &buf.l2ntfy_cfg.thresh);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_EVENT_ENABLE_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_notificationEventEnable_get(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.event, &buf.l2ntfy_cfg.enable);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_DBG_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_debug_get(buf.l2ntfy_cfg.unit, &buf.l2ntfy_cfg.dbgFlag);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_DST_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_dst_get(buf.l2ntfy_cfg.unit, &buf.l2ntfy_cfg.dst);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_MAGIC_NUM_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_magicNum_get(buf.l2ntfy_cfg.unit, &buf.l2ntfy_cfg.magicNum);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_MACADDR_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_macAddr_get(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.addrType, &buf.l2ntfy_cfg.mac);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_MAXEVENT_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_maxEvent_get(buf.l2ntfy_cfg.unit, &buf.l2ntfy_cfg.maxEvent);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;

        case RTDRV_L2NTFY_TIMEOUT_GET:
            copy_from_user(&buf.l2ntfy_cfg, user, sizeof(rtdrv_l2ntfyCfg_t));
            ret = drv_l2ntfy_timeout_get(buf.l2ntfy_cfg.unit, buf.l2ntfy_cfg.mode, &buf.l2ntfy_cfg.timeout);
            copy_to_user(user, &buf.l2ntfy_cfg, sizeof(rtdrv_l2ntfyCfg_t));
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}




int32 do_rtdrv_get_ctl_l3(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /*L3*/
        case RTDRV_L3_ROUTE_ROUTEENTRY_GET:
            copy_from_user(&buf.l3_route_entry, user, sizeof(rtdrv_l3_routeEntry_t));
            ret = rtk_l3_routeEntry_get(buf.l3_route_entry.unit, buf.l3_route_entry.index, &buf.l3_route_entry.entry);
            copy_to_user(user, &buf.l3_route_entry, sizeof(rtdrv_l3_routeEntry_t));
            break;

        case RTDRV_L3_ROUTE_SWITCHMACADDR_GET:
            copy_from_user(&buf.l3_config, user, sizeof(rtdrv_l3_config_t));
            ret = rtk_l3_routeSwitchMacAddr_get(buf.l3_config.unit, buf.l3_config.index, &buf.l3_config.mac);
            copy_to_user(user, &buf.l3_config, sizeof(rtdrv_l3_config_t));
            break;

        case RTDRV_L3_INFO_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_info_get(buf.l3_cfg.unit, &buf.l3_cfg.info);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_INTF_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intf_get(buf.l3_cfg.unit, buf.l3_cfg.type, &buf.l3_cfg.intf);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_INTFSTAT_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intfStat_get(buf.l3_cfg.unit, buf.l3_cfg.intfId, buf.l3_cfg.stat, &buf.l3_cfg.value64);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_NEXTHOP_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_nextHop_get(buf.l3_cfg.unit, buf.l3_cfg.nhId, &buf.l3_cfg.nextHop);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_NEXTHOPPATH_FIND:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_nextHopPath_find(buf.l3_cfg.unit, &buf.l3_cfg.nextHop, &buf.l3_cfg.nhId);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ECMP_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmp_get(buf.l3_cfg.unit, buf.l3_cfg.ecmpId, buf.l3_cfg.nhIdArraySize, &buf.l3_cfg.nhIdArray[0], &buf.l3_cfg.nhIdCount);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ECMP_FIND:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmp_find(buf.l3_cfg.unit, buf.l3_cfg.nhIdCount, &buf.l3_cfg.nhIdArray[0], &buf.l3_cfg.ecmpId);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ECMPHASHTBL_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmpHashTbl_get(buf.l3_cfg.unit, buf.l3_cfg.ecmpId, buf.l3_cfg.nhIdxCount, &buf.l3_cfg.nhIdxArray[0]);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ECMPCTRL_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_ecmpCtrl_get(buf.l3_cfg.unit, buf.l3_cfg.ecmpId, buf.l3_cfg.ecmpCtrlType, &buf.l3_cfg.arg);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_HOST_FIND:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_find(buf.l3_cfg.unit, &buf.l3_cfg.host);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_HOSTCONFLICT_GET:
            {
                rtk_l3_host_t *pHosts;
                rtdrv_l3Cfg_t *pUser = user;

                copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
                if (NULL == (pHosts = osal_alloc(sizeof(rtk_l3_host_t) * buf.l3_cfg.maxHost)))
                {
                    ret = RT_ERR_FAILED;
                }
                else
                {
                    ret = rtk_l3_hostConflict_get(buf.l3_cfg.unit, &buf.l3_cfg.key, pHosts, buf.l3_cfg.maxHost, &buf.l3_cfg.hostCount);
                    copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
                    copy_to_user(pUser->pHostArray, pHosts, sizeof(rtk_l3_host_t) * buf.l3_cfg.maxHost);
                    osal_free(pHosts);
                }
            }
            break;

        case RTDRV_L3_HOST_AGE:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_age(buf.l3_cfg.unit, buf.l3_cfg.flags, buf.l3_cfg.fHostCb, &buf.l3_cfg.cookie);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_HOST_GETNEXT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_host_getNext(buf.l3_cfg.unit, buf.l3_cfg.flags, &buf.l3_cfg.base, &buf.l3_cfg.host);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ROUTE_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_get(buf.l3_cfg.unit, &buf.l3_cfg.route);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ROUTE_AGE:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_age(buf.l3_cfg.unit, buf.l3_cfg.flags, buf.l3_cfg.fRouteCb, &buf.l3_cfg.cookie);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_ROUTE_GETNEXT:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_route_getNext(buf.l3_cfg.unit, buf.l3_cfg.flags, &buf.l3_cfg.base, &buf.l3_cfg.route);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_GLOBALCTRL_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_globalCtrl_get(buf.l3_cfg.unit, buf.l3_cfg.globalCtrlType, &buf.l3_cfg.arg);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_INTFCTRL_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_intfCtrl_get(buf.l3_cfg.unit, buf.l3_cfg.intfId, buf.l3_cfg.type, &buf.l3_cfg.arg);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;

        case RTDRV_L3_PORTCTRL_GET:
            copy_from_user(&buf.l3_cfg, user, sizeof(rtdrv_l3Cfg_t));
            ret = rtk_l3_portCtrl_get(buf.l3_cfg.unit, buf.l3_cfg.port, buf.l3_cfg.type, &buf.l3_cfg.arg);
            copy_to_user(user, &buf.l3_cfg, sizeof(rtdrv_l3Cfg_t));
            break;


        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_mcast(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if (defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310))
        /*MCAST*/
        case RTDRV_MCAST_GROUP_CREATE:
            copy_from_user(&buf.mcast_cfg, user, sizeof(rtdrv_mcastCfg_t));
            ret = rtk_mcast_group_create(buf.mcast_cfg.unit, buf.mcast_cfg.flags, buf.mcast_cfg.type, &buf.mcast_cfg.group);
            copy_to_user(user, &buf.mcast_cfg, sizeof(rtdrv_mcastCfg_t));
            break;

        case RTDRV_MCAST_GROUP_GETNEXT:
            copy_from_user(&buf.mcast_cfg, user, sizeof(rtdrv_mcastCfg_t));
            ret = rtk_mcast_group_getNext(buf.mcast_cfg.unit, buf.mcast_cfg.type, &buf.mcast_cfg.base, &buf.mcast_cfg.group);
            copy_to_user(user, &buf.mcast_cfg, sizeof(rtdrv_mcastCfg_t));
            break;

        case RTDRV_MCAST_NEXTHOP_GET:
            {
                rtk_mcast_egrif_t      *kNhArry;
                rtdrv_mcastCfg_t        *pUser = user;
                copy_from_user(&buf.mcast_cfg, user, sizeof(rtdrv_mcastCfg_t));
                kNhArry = osal_alloc(sizeof(rtk_mcast_egrif_t) * buf.mcast_cfg.maxNum);
                if (kNhArry == NULL)
                {
                    ret = RT_ERR_FAILED;
                }
                else
                {
                    ret = rtk_mcast_egrIf_get(buf.mcast_cfg.unit, buf.mcast_cfg.group, buf.mcast_cfg.maxNum, kNhArry, &buf.mcast_cfg.nexthopNum);
                    copy_to_user(user, &buf.mcast_cfg, sizeof(rtdrv_mcastCfg_t));
                    copy_to_user(pUser->pNhArry, kNhArry, sizeof(rtk_mcast_egrif_t) * buf.mcast_cfg.maxNum);
                    osal_free(kNhArry);
                }
                break;
            }
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_ipmc(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if (defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310))
        /*IPMCAST*/
        case RTDRV_IPMC_ADDR_INIT:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_addr_t_init(&buf.ipmc_cfg.ipmcEntry);
            copy_to_user(user, &buf.ipmc_cfg, sizeof(rtdrv_ipmcCfg_t));
            break;

        case RTDRV_IPMC_ADDR_FIND:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_addr_find(buf.ipmc_cfg.unit, &buf.ipmc_cfg.ipmcEntry);
            copy_to_user(user, &buf.ipmc_cfg, sizeof(rtdrv_ipmcCfg_t));
            break;

        case RTDRV_IPMC_NEXTVALID_ADDR_GET:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_nextValidAddr_get(buf.ipmc_cfg.unit, buf.ipmc_cfg.flags, &buf.ipmc_cfg.base, &buf.ipmc_cfg.ipmcEntry);
            copy_to_user(user, &buf.ipmc_cfg, sizeof(rtdrv_ipmcCfg_t));
            break;

        case RTDRV_IPMC_STAT_GET:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_statCntr_get(buf.ipmc_cfg.unit, &buf.ipmc_cfg.statKey, &buf.ipmc_cfg.statCnt);
            copy_to_user(user, &buf.ipmc_cfg, sizeof(rtdrv_ipmcCfg_t));
            break;

        case RTDRV_IPMC_GLOBALCTRL_GET:
            copy_from_user(&buf.ipmc_cfg, user, sizeof(rtdrv_ipmcCfg_t));
            ret = rtk_ipmc_globalCtrl_get(buf.ipmc_cfg.unit, buf.ipmc_cfg.globalCtrlType, &buf.ipmc_cfg.arg);
            copy_to_user(user, &buf.ipmc_cfg, sizeof(rtdrv_ipmcCfg_t));
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_tunnel(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /* Tunnel */
#if defined(CONFIG_SDK_RTL9310)
        case RTDRV_TUNNEL_INFO_GET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_info_get(buf.tunnel_cfg.unit, &buf.tunnel_cfg.info);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_INTF_GET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intf_get(buf.tunnel_cfg.unit, &buf.tunnel_cfg.intf);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_INTFSTAT_GET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intfStat_get(buf.tunnel_cfg.unit, buf.tunnel_cfg.intfId, buf.tunnel_cfg.stat, &buf.tunnel_cfg.value64);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_QOSPROFILE_GET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_qosProfile_get(buf.tunnel_cfg.unit, buf.tunnel_cfg.idx, &buf.tunnel_cfg.profile);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_GLOBALCTRL_GET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_globalCtrl_get(buf.tunnel_cfg.unit, buf.tunnel_cfg.type, &buf.tunnel_cfg.arg);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;

        case RTDRV_TUNNEL_INTFCTRL_GET:
            copy_from_user(&buf.tunnel_cfg, user, sizeof(rtdrv_tunnelCfg_t));
            ret = rtk_tunnel_intfCtrl_get(buf.tunnel_cfg.unit, buf.tunnel_cfg.intfId, buf.tunnel_cfg.intfType, &buf.tunnel_cfg.intfArg);
            copy_to_user(user, &buf.tunnel_cfg, sizeof(rtdrv_tunnelCfg_t));
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_get_ctl_vxlan(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /* Tunnel */
#if defined(CONFIG_SDK_RTL9310)
        case RTDRV_VXLAN_VNI_GET:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_vni_get(buf.vxlan_cfg.unit, &buf.vxlan_cfg.entry);
            copy_to_user(user, &buf.vxlan_cfg, sizeof(rtdrv_vxlanCfg_t));
            break;

        case RTDRV_VXLAN_VNI_GETNEXT:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_vni_getNext(buf.vxlan_cfg.unit, &buf.vxlan_cfg.base, &buf.vxlan_cfg.entry);
            copy_to_user(user, &buf.vxlan_cfg, sizeof(rtdrv_vxlanCfg_t));
            break;

        case RTDRV_VXLAN_GLOBALCTRL_GET:
            copy_from_user(&buf.vxlan_cfg, user, sizeof(rtdrv_vxlanCfg_t));
            ret = rtk_vxlan_globalCtrl_get(buf.vxlan_cfg.unit, buf.vxlan_cfg.type, &buf.vxlan_cfg.arg);
            copy_to_user(user, &buf.vxlan_cfg, sizeof(rtdrv_vxlanCfg_t));
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_port(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** PORT **/
        case RTDRV_PORT_LINK_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_link_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_SPEED_DUPLEX_GET:
            copy_from_user(&buf.speed_duplex, user, sizeof(rtdrv_port_speedDuplex_t));
            ret = rtk_port_speedDuplex_get(buf.speed_duplex.unit, buf.speed_duplex.port, &buf.speed_duplex.speed,
                                           &buf.speed_duplex.duplex);
            copy_to_user(user, &buf.speed_duplex, sizeof(rtdrv_port_speedDuplex_t));
            break;

        case RTDRV_PORT_FLOW_CTRL_GET:
            copy_from_user(&buf.port_flowctrl, user, sizeof(rtdrv_port_flowctrl_t));
            ret = rtk_port_flowctrl_get(buf.port_flowctrl.unit, buf.port_flowctrl.port, &buf.port_flowctrl.tx_status,
                                        &buf.port_flowctrl.rx_status);
            copy_to_user(user, &buf.port_flowctrl, sizeof(rtdrv_port_flowctrl_t));
            break;

        case RTDRV_PORT_EN_AUTONEGO_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyAutoNegoEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_AUTONEGO_ABIL_GET:
            copy_from_user(&buf.autonego_ability, user, sizeof(rtdrv_port_autoNegoAbility_t));
            ret = rtk_port_phyAutoNegoAbility_get(buf.autonego_ability.unit, buf.autonego_ability.port,
                                                  &buf.autonego_ability.ability);
            copy_to_user(user, &buf.autonego_ability, sizeof(rtdrv_port_autoNegoAbility_t));
            break;

        case RTDRV_PORT_FORCE_MODE_ABIL_GET:
            copy_from_user(&buf.forcemode_ability, user, sizeof(rtdrv_port_forceModeAbility_t));
            ret = rtk_port_phyForceModeAbility_get(buf.forcemode_ability.unit, buf.forcemode_ability.port,
                                                   &buf.forcemode_ability.speed, &buf.forcemode_ability.duplex,
                                                   &buf.forcemode_ability.flowctrl);
            copy_to_user(user, &buf.forcemode_ability, sizeof(rtdrv_port_forceModeAbility_t));
            break;

        case RTDRV_PORT_CPU_PORT_ID_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_port_cpuPortId_get(buf.unit_cfg.unit, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_PORT_PHY_REG_GET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phyReg_get(buf.phy_data.unit, buf.phy_data.port, buf.phy_data.page, buf.phy_data.reg,
                                      &buf.phy_data.data);
            copy_to_user(user, &buf.phy_data, sizeof(rtdrv_port_phyReg_t));
            break;

        case RTDRV_PORT_PHY_EXT_PARK_PAGE_REG_GET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phyExtParkPageReg_get(buf.phy_data.unit, buf.phy_data.port, buf.phy_data.page, buf.phy_data.extPage,
                buf.phy_data.parkPage, buf.phy_data.reg, &buf.phy_data.data);
            copy_to_user(user, &buf.phy_data, sizeof(rtdrv_port_phyReg_t));
            break;

        case RTDRV_PORT_PHY_MMD_REG_GET:
            copy_from_user(&buf.phy_data, user, sizeof(rtdrv_port_phyReg_t));
            ret = rtk_port_phyMmdReg_get(buf.phy_data.unit, buf.phy_data.port, buf.phy_data.mmdAddr, buf.phy_data.reg, &buf.phy_data.data);
            copy_to_user(user, &buf.phy_data, sizeof(rtdrv_port_phyReg_t));
            break;

        case RTDRV_PORT_MASTER_SLAVE_GET:
            copy_from_user(&buf.masterSlave_cfg, user, sizeof(rtdrv_port_masterSlave_t));
            ret = rtk_port_phyMasterSlave_get(buf.masterSlave_cfg.unit, buf.masterSlave_cfg.port, &buf.masterSlave_cfg.masterSlaveCfg, &buf.masterSlave_cfg.masterSlaveActual);
            copy_to_user(user, &buf.masterSlave_cfg, sizeof(rtdrv_port_masterSlave_t));
            break;

        case RTDRV_PORT_ISOLATION_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolation_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.portmask);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_ISOLATIONEXT_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolationExt_get(buf.port_cfg.unit, buf.port_cfg.srcUnit, buf.port_cfg.port, &buf.port_cfg.portmask);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_ISOLATION_RESTRICT_ROUTE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_isolationRestrictRoute_get(buf.port_cfg.unit, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_EN_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_adminEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_BACK_PRESSURE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_backpressureEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_PHY_MEDIA_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyComboPortMedia_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.media);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_GREEN_ENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_greenEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_GIGA_LITE_ENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_gigaLiteEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_PHY_CROSSOVERMODE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyCrossOverMode_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_TX_EN_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_txEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_RX_EN_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_rxEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_PHY_FIBER_MEDIA_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyComboPortFiberMedia_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.fiber_media);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_LINKMEDIA_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_linkMedia_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data, &buf.port_cfg.media);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_LINKDOWN_POWERSAVING_ENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_linkDownPowerSavingEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_VLAN_ISOLATION_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_vlanBasedIsolationEntry_get(buf.port_cfg.unit, buf.port_cfg.index, &buf.port_cfg.vlanIsoEntry);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_VLAN_ISOLATION_VLANSOURCE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_vlanBasedIsolation_vlanSource_get(buf.port_cfg.unit, &buf.port_cfg.vlanIsoSrc);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_VLAN_ISOLATION_EGRBYPASS_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_vlanBasedIsolationEgrBypass_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_DOWNSPEEDENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_downSpeedEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;
        case RTDRV_PORT_FIBERDOWNSPEEDENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberDownSpeedEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;
        case RTDRV_PORT_FIBERNWAYFORCELINKENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberNwayForceLinkEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;
        case RTDRV_PORT_FIBERUNIDIRENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberUnidirEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.enable);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_PHYLOOPBACKENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyLoopBackEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.enable);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_10GMEDIA_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_10gMedia_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.media_10g);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_PORT_PHY_CROSSOVERSTATUS_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyCrossOverStatus_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
        break;

        case RTDRV_PORT_FIBERRXENABLE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_fiberRxEnable_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.enable);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;
        case RTDRV_PORT_PHYPOLAR_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phyPolar_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.polarCtrl);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;
        case RTDRV_PORT_PHYSDS_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_port_phySds_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.sdsCfg);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_oam(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
     /*OAM*/
        case RTDRV_OAM_AUTODYINGGASPENABLE_GET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_autoDyingGaspEnable_get(buf.dyingGasp_cfg.unit, buf.dyingGasp_cfg.port, &buf.dyingGasp_cfg.enable);
            copy_to_user(user, &buf.dyingGasp_cfg, sizeof(rtdrv_oamDyingGaspCfg_t));
            break;

        case RTDRV_OAM_DYINGGASPWAITTIME_GET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_dyingGaspWaitTime_get(buf.dyingGasp_cfg.unit, &buf.dyingGasp_cfg.waitTime);
            copy_to_user(user, &buf.dyingGasp_cfg, sizeof(rtdrv_oamDyingGaspCfg_t));
            break;

        case RTDRV_OAM_DYINGGASPPKTCNT_GET:
            copy_from_user(&buf.dyingGasp_cfg, user, sizeof(rtdrv_oamDyingGaspCfg_t));
            ret = rtk_oam_dyingGaspPktCnt_get(buf.dyingGasp_cfg.unit, &buf.dyingGasp_cfg.cnt);
            copy_to_user(user, &buf.dyingGasp_cfg, sizeof(rtdrv_oamDyingGaspCfg_t));
            break;

        case RTDRV_OAM_LOOPBACKMACSWAPENABLE_GET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_oam_loopbackMacSwapEnable_get(buf.misc_cfg.unit,
                                                    &buf.misc_cfg.loopbackEnable);
            copy_to_user(user, &buf.misc_cfg, sizeof(rtdrv_oamCfmMiscCfg_t));
            break;

        case RTDRV_OAM_PORTLOOPBACKMUXACTION_GET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_oam_portLoopbackMuxAction_get(buf.misc_cfg.unit,
                    buf.misc_cfg.port, &buf.misc_cfg.action);
            copy_to_user(user, &buf.misc_cfg, sizeof(rtdrv_oamCfmMiscCfg_t));
            break;

        case RTDRV_OAM_PDULEARNINGENABLE_GET:
            copy_from_user(&buf.oam_cfg, user, sizeof(rtdrv_oamCfg_t));
            ret = rtk_oam_pduLearningEnable_get(buf.oam_cfg.unit, &buf.oam_cfg.enable);
            copy_to_user(user, &buf.oam_cfg, sizeof(rtdrv_oamCfg_t));
            break;

        case RTDRV_OAM_CFMCCMPCP_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmPcp_get(buf.ccm_cfg.unit,
                                        &buf.ccm_cfg.ccmFrame.outer_pri);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMCFI_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmCfi_get(buf.ccm_cfg.unit,
                                        &buf.ccm_cfg.ccmFrame.outer_dei);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMTPID_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmTpid_get(buf.ccm_cfg.unit,
                                         &buf.ccm_cfg.ccmFrame.outer_tpid);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMRESETLIFETIME_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstLifetime_get(buf.ccm_cfg.unit,
                                                  buf.ccm_cfg.cfmIdx,
                                                  &buf.ccm_cfg.ccmFlag);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMMEPID_GET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_oam_cfmCcmMepid_get(buf.misc_cfg.unit,
                                          &buf.misc_cfg.mepid);
            copy_to_user(user, &buf.misc_cfg, sizeof(rtdrv_oamCfmMiscCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINTERVALFIELD_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmIntervalField_get(buf.ccm_cfg.unit,
                                                  &buf.ccm_cfg.ccmFlag);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMMDL_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmMdl_get(buf.cfm_cfg.unit,
                                        &buf.cfm_cfg.cfmCfg.md_level);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTTXMEPID_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxMepid_get(buf.cfm_cfg.unit,
                    buf.cfm_cfg.instance, &buf.cfm_cfg.mepid);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTTXINTERVALFIELD_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxIntervalField_get(buf.cfm_cfg.unit,
                    buf.cfm_cfg.instance, &buf.cfm_cfg.interval);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTTXMDL_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxMdl_get(buf.cfm_cfg.unit,
                    buf.cfm_cfg.instance, &buf.cfm_cfg.mdl);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTTAGSTATUS_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTagStatus_get(buf.cfm_cfg.unit,
                                                  buf.cfm_cfg.cfmIdx,
                                                  &buf.cfm_cfg.enable);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTVID_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstVid_get(buf.ccm_cfg.unit,
                                            buf.ccm_cfg.cfmIdx,
                                            &buf.ccm_cfg.ccmFrame.outer_vid);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTMAID_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstMaid_get(buf.cfm_cfg.unit,
                                             buf.cfm_cfg.cfmIdx,
                                             &buf.cfm_cfg.maid);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTTXSTATUS_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmCcmInstTxStatus_get(buf.cfm_cfg.unit,
                                                 buf.cfm_cfg.cfmIdx,
                                                 &buf.cfm_cfg.enable);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTINTERVAL_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstInterval_get(buf.ccm_cfg.unit,
                                                 buf.ccm_cfg.cfmIdx,
                                                 &buf.ccm_cfg.ccmInterval);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMTXINSTPORT_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmTxInstPort_get(buf.ccm_cfg.unit,
                                               buf.ccm_cfg.cfmIdx,
                                               buf.ccm_cfg.portIdx,
                                               &buf.ccm_cfg.port);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMRXINSTVID_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmRxInstVid_get(buf.ccm_cfg.unit,
                                              buf.ccm_cfg.cfmIdx,
                                              &buf.ccm_cfg.ccmFrame.outer_vid);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMRXINSTPORT_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmRxInstPort_get(buf.ccm_cfg.unit,
                                               buf.ccm_cfg.cfmIdx,
                                               buf.ccm_cfg.portIdx,
                                               &buf.ccm_cfg.port);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMKEEPALIVE_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstAliveTime_get(buf.ccm_cfg.unit,
                                              buf.ccm_cfg.cfmIdx,
                                              buf.ccm_cfg.portIdx,
                                              &buf.ccm_cfg.ccmInterval);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTTXMEMBER_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstTxMember_get(buf.ccm_cfg.unit, buf.ccm_cfg.instance, &buf.ccm_cfg.member);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMCCMINSTRXMEMBER_GET:
            copy_from_user(&buf.ccm_cfg, user, sizeof(rtdrv_oamCcmCfg_t));
            ret = rtk_oam_cfmCcmInstRxMember_get(buf.ccm_cfg.unit, buf.ccm_cfg.instance, &buf.ccm_cfg.member);
            copy_to_user(user, &buf.ccm_cfg, sizeof(rtdrv_oamCcmCfg_t));
            break;

        case RTDRV_OAM_CFMETHDMPORTENABLE_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmPortEthDmEnable_get(buf.cfm_cfg.unit,
                                              buf.cfm_cfg.port,
                                              &buf.cfm_cfg.enable);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        case RTDRV_OAM_CFMETHDMRXTIMESTAMP_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmRxTimestamp_get(buf.cfm_cfg.unit,
                                              buf.cfm_cfg.index,
                                              &buf.cfm_cfg.timeStamp);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;
        case RTDRV_OAM_CFMETHDMTXDELAY_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmTxDelay_get(buf.cfm_cfg.unit,
                                              &buf.cfm_cfg.txDelay);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;
        case RTDRV_OAM_CFMETHDMREFTIME_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmRefTime_get(buf.cfm_cfg.unit,
                                              &buf.cfm_cfg.timeStamp);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;
        case RTDRV_OAM_CFMETHDMREFTIMEENABLE_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmRefTimeEnable_get(buf.cfm_cfg.unit,
                                              &buf.cfm_cfg.enable);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;
        case RTDRV_OAM_CFMETHDMREFTIMEFREQ_GET:
            copy_from_user(&buf.cfm_cfg, user, sizeof(rtdrv_oamCfmCfg_t));
            ret = rtk_oam_cfmEthDmRefTimeFreq_get(buf.cfm_cfg.unit,
                                              &buf.cfm_cfg.freq);
            copy_to_user(user, &buf.cfm_cfg, sizeof(rtdrv_oamCfmCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_vlan(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {

    /** VLAN **/
        case RTDRV_VLAN_PORT_GET:
            copy_from_user(&buf.vlan_port_data, user, sizeof(rtdrv_vlan_port_t));
            ret = rtk_vlan_port_get(buf.vlan_port_data.unit, buf.vlan_port_data.vid, &buf.vlan_port_data.member,
                                    &buf.vlan_port_data.untag);
            copy_to_user(user, &buf.vlan_port_data, sizeof(rtdrv_vlan_port_t));
            break;

        case RTDRV_VLAN_MCASTGROUP_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_mcastGroup_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.groupId);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_PVID_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portPvid_get(buf.vlan_cfg.unit, buf.vlan_cfg.port,buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_VLAN_PROTO_GROUP_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_protoGroup_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.protoGroup);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_PROTO_VLAN_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portProtoVlan_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.idx, &buf.vlan_cfg.protoVlanCfg);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_IGR_TPID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrTpid_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGR_TPID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTpid_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGR_TPID_SRC_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTpidSrc_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_IGR_EXTRA_TAG_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrExtraTagEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_IGR_VLAN_TRANSPARENT_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanTransparentEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGR_VLAN_TRANSPARENT_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanTransparentEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_ACCEPT_FRAME_TYPE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portAcceptFrameType_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;


        case RTDRV_VLAN_EN_MCAST_LEAKY_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = rtk_vlan_mcastLeakyEnable_get(buf.unit_cfg.unit, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_VLAN_SVLMODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_svlMode_get(buf.vlan_cfg.unit, &buf.vlan_cfg.mode);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_SVLFID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_svlFid_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.fid);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_STG_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_stg_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_L2_LOOKUP_SVL_FID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2LookupSvlFid_get(buf.vlan_cfg.unit, buf.vlan_cfg.macType, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGR_FILTER_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrFilterEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_LUTMODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2LookupMode_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid,buf.vlan_cfg.macType, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_GROUPMASK_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_groupMask_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.groupMask);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PROFILE_IDX_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_profileIdx_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PROFILE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_profile_get(buf.vlan_cfg.unit, buf.vlan_cfg.data, &buf.vlan_cfg.profile);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_FORWARD_VLAN_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portFwdVlan_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.tagMode, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_IGR_FILTER_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrFilter_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_IGRTAGKEEPTYPE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrTagKeepType_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data, &buf.vlan_cfg.data1);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGRTAGKEEPTYPE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTagKeepType_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data, &buf.vlan_cfg.data1);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_PVID_MODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portPvidMode_get(buf.vlan_cfg.unit, buf.vlan_cfg.port,buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_MAC_BASED_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlan_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    &buf.vlan_cfg.data, &buf.vlan_cfg.mac, &buf.vlan_cfg.vid,
                    &buf.vlan_cfg.data1);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_MAC_BASED_WITH_MSK_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlanWithMsk_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    &buf.vlan_cfg.data, &buf.vlan_cfg.mac, &buf.vlan_cfg.msk, &buf.vlan_cfg.vid,
                    &buf.vlan_cfg.data1);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_MAC_BASED_WITH_PORT_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlanWithPort_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    &buf.vlan_cfg.data, &buf.vlan_cfg.mac, &buf.vlan_cfg.msk,
                    &buf.vlan_cfg.port, &buf.vlan_cfg.port_msk, &buf.vlan_cfg.vid, &buf.vlan_cfg.data1);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_MAC_BASED_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portMacBasedVlanEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port,&buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_MAC_BASED_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_macBasedVlanEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx,&buf.vlan_cfg.macEntry);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ipSubnetBasedVlan_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    &buf.vlan_cfg.data, &buf.vlan_cfg.sip, &buf.vlan_cfg.sip_msk, &buf.vlan_cfg.vid,
                    &buf.vlan_cfg.data1);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_WITH_PORT_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ipSubnetBasedVlanWithPort_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx,
                    &buf.vlan_cfg.data, &buf.vlan_cfg.sip, &buf.vlan_cfg.sip_msk,
                    &buf.vlan_cfg.port, &buf.vlan_cfg.port_msk, &buf.vlan_cfg.vid, &buf.vlan_cfg.data1);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIpSubnetBasedVlanEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port,&buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IP_SUBNET_BASED_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ipSubnetBasedVlanEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx,&buf.vlan_cfg.ipEntry);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_TPID_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_tpidEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.tagType, buf.vlan_cfg.idx, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGR_TAG_STS_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrTagSts_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IGRVLANCNVT_BLKMODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtBlkMode_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IGRVLANCNVT_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.igrCnvtEntry);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORTIGRVLANCNVTENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanCnvtEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.enable);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGRVLANCNVT_DBLTAG_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtDblTagEnable_get(buf.vlan_cfg.unit, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGRVLANCNVT_VIDSRC_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtVidSource_get(buf.vlan_cfg.unit, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGRVLANCNVT_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.egrCnvtEntry);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORTEGRVLANCNVTENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.enable);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_AGGRENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_aggrEnable_get(buf.vlan_cfg.unit, &buf.vlan_cfg.enable);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_VLANAGGR_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_VLANAGGR_PRI_ENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrPriEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.enable);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_LEAKYSTPFILTER_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_leakyStpFilter_get(buf.vlan_cfg.unit, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EXCEPT_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_except_get(buf.vlan_cfg.unit, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORTIGRCNVTDFLTACT_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrCnvtDfltAct_get(buf.vlan_cfg.unit,
                    buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_VLANAGGRCTRL_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrCtrl_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.vlanAggrCtrl);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGRVLANCNVTVIDSOURCE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtVidSource_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGRVLANCNVTVIDTARGET_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtVidTarget_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORTIGRCNVTLUMISACT_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanCnvtLuMisAct_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORTEGRCNVTLUMISACT_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtLuMisAct_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, buf.vlan_cfg.type, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IGRVLANCNVTHITINDICATION_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtHitIndication_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, buf.vlan_cfg.flag, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGRVLANCNVTHITINDICATION_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtHitIndication_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, buf.vlan_cfg.flag, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IGRVLANCNVTRANGECHECKSET_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrVlanCnvtRangeCheckSet_get(buf.vlan_cfg.unit,buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_IGRVLANCNVTRANGECHECKENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_igrVlanCnvtRangeCheckEntry_get(buf.vlan_cfg.unit,buf.vlan_cfg.setIdx, buf.vlan_cfg.idx, &buf.vlan_cfg.rangeCheck);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

         case RTDRV_VLAN_EGRVLANCNVTRANGECHECKSET_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtRangeCheckSet_get(buf.vlan_cfg.unit,buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGRVLANCNVTRANGECHECKENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtRangeCheckEntry_get(buf.vlan_cfg.unit,buf.vlan_cfg.setIdx, buf.vlan_cfg.idx, &buf.vlan_cfg.rangeCheck);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

#ifdef CONFIG_SDK_DRIVER_RTK_LEGACY_API
        case RTDRV_VLAN_PORT_VLANAGGRVIDSOURCE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrVidSource_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_VLANAGGRPRITAGVIDSOURCE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portVlanAggrPriTagVidSource_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_UCAST_LUTMODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2UcastLookupMode_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_MCAST_LUTMODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_l2McastLookupMode_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_INNER_ACCEPT_FRAME_TYPE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portInnerAcceptFrameType_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_VLAN_PORT_OUTER_ACCEPT_FRAME_TYPE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portOuterAcceptFrameType_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_VLAN_PORT_INNER_PVID_MODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portInnerPvidMode_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            break;

        case RTDRV_VLAN_PORT_OUTER_PVID_MODE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portOuterPvidMode_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_INNER_PVID_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portInnerPvid_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_portCfg_t));
            break;

        case RTDRV_VLAN_PORT_OUTER_PVID_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_portCfg_t));
            ret = rtk_vlan_portOuterPvid_get(buf.port_cfg.unit, buf.port_cfg.port, &buf.port_cfg.data);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_INNER_TPID_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_innerTpidEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_OUTER_TPID_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_outerTpidEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EXTRA_TPID_ENTRY_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_extraTpidEntry_get(buf.vlan_cfg.unit, buf.vlan_cfg.idx, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_IGR_INNER_TPID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrInnerTpid_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_IGR_OUTER_TPID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portIgrOuterTpid_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGR_INNER_TPID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrInnerTpid_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGR_OUTER_TPID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrOuterTpid_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGR_INNER_TAG_STS_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrInnerTagSts_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_EGR_OUTER_TAG_STS_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrOuterTagSts_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORT_EGRVLANCNVTLOOKUPMISSACT_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portEgrVlanCnvtLookupMissAct_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.data);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;
#endif

        case RTDRV_VLAN_EGRVLANCNVTRANGECHECKVID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_egrVlanCnvtRangeCheckVid_get(buf.vlan_cfg.unit,
                    buf.vlan_cfg.idx, &buf.vlan_cfg.rangeCheck);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_ECIDPMSK_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ecidPmsk_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.entry);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_ECIDPMSKNEXTVALID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_ecidPmskNextValid_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, &buf.vlan_cfg.entry);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_BSSID_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_bssid_get(buf.vlan_cfg.unit, buf.vlan_cfg.vid, buf.vlan_cfg.arraySize, &buf.vlan_cfg.bssidArray, &buf.vlan_cfg.bssidCnt);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_TRKVLANAGGRENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_trkVlanAggrEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.tid, &buf.vlan_cfg.enable);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_TRKVLANAGGRPRIENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_trkVlanAggrPriEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.tid, &buf.vlan_cfg.enable);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_TRKVLANAGGRCTRL_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_trkVlanAggrCtrl_get(buf.vlan_cfg.unit, buf.vlan_cfg.tid, &buf.vlan_cfg.vlanAggrCtrl);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        case RTDRV_VLAN_PORTPRIVATEVLANENABLE_GET:
            copy_from_user(&buf.vlan_cfg, user, sizeof(rtdrv_vlanCfg_t));
            ret = rtk_vlan_portPrivateVlanEnable_get(buf.vlan_cfg.unit, buf.vlan_cfg.port, &buf.vlan_cfg.enable);
            copy_to_user(user, &buf.vlan_cfg, sizeof(rtdrv_vlanCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_bpe(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** BPE **/
        case RTDRV_BPE_PORTFWDMODE_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portFwdMode_get(buf.bpe_cfg.unit, buf.bpe_cfg.port, &buf.bpe_cfg.mode);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PORTECIDNAMESPACEGROUPID_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portEcidNameSpaceGroupId_get(buf.bpe_cfg.unit, buf.bpe_cfg.port, &buf.bpe_cfg.groupId);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PORTPCID_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portPcid_get(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.type, &buf.bpe_cfg.pcid);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PORTPCIDACT_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portPcidAct_get(buf.bpe_cfg.unit, buf.bpe_cfg.port, &buf.bpe_cfg.action);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PORTEGRTAGSTS_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portEgrTagSts_get(buf.bpe_cfg.unit, buf.bpe_cfg.port, &buf.bpe_cfg.status);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PORTEGRVLANTAGSTS_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portEgrVlanTagSts_get(buf.bpe_cfg.unit, buf.bpe_cfg.port, &buf.bpe_cfg.vlan_status);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PVIDENTRY_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_pvidEntry_get(buf.bpe_cfg.unit, &buf.bpe_cfg.pvid_entry);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PVIDENTRYNEXTVALID_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_pvidEntryNextValid_get(buf.bpe_cfg.unit, &buf.bpe_cfg.scan_idx, &buf.bpe_cfg.pvid_entry);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_FWDENTRY_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_fwdEntry_get(buf.bpe_cfg.unit, &buf.bpe_cfg.fwd_entry);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_FWDENTRYNEXTVALID_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_fwdEntryNextValid_get(buf.bpe_cfg.unit, &buf.bpe_cfg.scan_idx, &buf.bpe_cfg.fwd_entry);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_GLOBALCTRL_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_globalCtrl_get(buf.bpe_cfg.unit, buf.bpe_cfg.type, &buf.bpe_cfg.arg);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PORTCTRL_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_portCtrl_get(buf.bpe_cfg.unit, buf.bpe_cfg.port, buf.bpe_cfg.type, &buf.bpe_cfg.arg);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        case RTDRV_BPE_PRIREMARKING_GET:
            copy_from_user(&buf.bpe_cfg, user, sizeof(rtdrv_bpeCfg_t));
            ret = rtk_bpe_priRemarking_get(buf.bpe_cfg.unit, buf.bpe_cfg.src, buf.bpe_cfg.val, &buf.bpe_cfg.pri);
            copy_to_user(user, &buf.bpe_cfg, sizeof(rtdrv_bpeCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_stp(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** STP **/
        case RTDRV_STP_MSTP_STATE_GET:
            copy_from_user(&buf.stp_cfg, user, sizeof(rtdrv_stpCfg_t));
            ret = rtk_stp_mstpState_get(buf.stp_cfg.unit, buf.stp_cfg.msti, buf.stp_cfg.port, &buf.stp_cfg.stp_state);
            copy_to_user(user, &buf.stp_cfg, sizeof(rtdrv_stpCfg_t));
            break;

        case RTDRV_STP_MSTP_INSTANCE_EXIST_GET:
            copy_from_user(&buf.stp_cfg, user, sizeof(rtdrv_stpCfg_t));
            ret = rtk_stp_isMstpInstanceExist_get(buf.stp_cfg.unit, buf.stp_cfg.msti, &buf.stp_cfg.msti_isExist);
            copy_to_user(user, &buf.stp_cfg, sizeof(rtdrv_stpCfg_t));
            break;

        case RTDRV_STP_MSTP_MODE_GET:
            copy_from_user(&buf.stp_cfg, user, sizeof(rtdrv_stpCfg_t));
            ret = rtk_stp_mstpInstanceMode_get(buf.stp_cfg.unit, &buf.stp_cfg.msti_mode);
            copy_to_user(user, &buf.stp_cfg, sizeof(rtdrv_stpCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_reg(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** REG **/
        case RTDRV_REG_REGISTER_GET:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ioal_mem32_read(buf.reg_cfg.unit, buf.reg_cfg.reg, &buf.reg_cfg.value);
            ret = RT_ERR_OK; /*xxx_reg_register_get(buf.reg_cfg.unit, buf.reg_cfg.reg, &buf.reg_cfg.value);*/
            copy_to_user(user, &buf.reg_cfg, sizeof(rtdrv_regCfg_t));
            break;

        case RTDRV_REG_IDX2ADDR_GET:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ret = reg_idx2Addr_get(buf.reg_cfg.unit, (uint32)buf.reg_cfg.reg, &buf.reg_cfg.value);
            copy_to_user(user, &buf.reg_cfg, sizeof(rtdrv_regCfg_t));
            break;

        case RTDRV_REG_IDXMAX_GET:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ret = reg_idxMax_get(buf.reg_cfg.unit, &buf.reg_cfg.value);
            copy_to_user(user, &buf.reg_cfg, sizeof(rtdrv_regCfg_t));
            break;

        case RTDRV_REG_INFO_GET:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ret = reg_info_get(buf.reg_cfg.unit, (uint32)buf.reg_cfg.reg, &buf.reg_cfg.data);
            copy_to_user(user, &buf.reg_cfg, sizeof(rtdrv_regCfg_t));
            break;

        case RTDRV_TABLE_READ:
            copy_from_user(&buf.tbl_cfg, user, sizeof(rtdrv_tblCfg_t));
            ret = table_read(buf.tbl_cfg.unit, buf.tbl_cfg.table, buf.tbl_cfg.addr, buf.tbl_cfg.value);
            copy_to_user(user, &buf.reg_cfg, sizeof(rtdrv_tblCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_counter(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** COUNTER **/
        case RTDRV_COUNTER_GLOBAL_GET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_global_get(buf.counter_cfg.unit, buf.counter_cfg.cntr_idx, &(buf.counter_cfg.cntr));
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        case RTDRV_COUNTER_GLOBAL_GETALL:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_global_getAll(buf.counter_cfg.unit, &buf.counter_cfg.global_cnt);
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        case RTDRV_COUNTER_PORT_GET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_port_get(buf.counter_cfg.unit, buf.counter_cfg.port, buf.counter_cfg.cntr_idx, &(buf.counter_cfg.cntr));
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        case RTDRV_COUNTER_PORT_GETALL:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_port_getAll(buf.counter_cfg.unit, buf.counter_cfg.port, &buf.counter_cfg.port_cnt);
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        case RTDRV_COUNTER_TAGLENCNT_GET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_tagLenCntIncEnable_get(buf.counter_cfg.unit, buf.counter_cfg.tagCnt_type, &buf.counter_cfg.enable);
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        case RTDRV_COUNTER_STACKHDRLENCNT_GET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_stackHdrLenCntIncEnable_get(buf.counter_cfg.unit, buf.counter_cfg.type, &buf.counter_cfg.enable);
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        case RTDRV_COUNTER_FLEXCNTR_CFG_GET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_flexibleCntRange_get(buf.counter_cfg.unit, buf.counter_cfg.cntr_idx, &buf.counter_cfg.range);
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        case RTDRV_COUNTER_ENABLE_GET:
            copy_from_user(&buf.counter_cfg, user, sizeof(rtdrv_counterCfg_t));
            ret = rtk_stat_enable_get(buf.counter_cfg.unit, &buf.counter_cfg.enable);
            copy_to_user(user, &buf.counter_cfg, sizeof(rtdrv_counterCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_time(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** TIME **/
        case RTDRV_TIME_PORT_PTP_ENABLE_GET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_portPtpEnable_get(buf.time_cfg.unit, buf.time_cfg.port, &buf.time_cfg.enable);
            copy_to_user(user, &buf.time_cfg, sizeof(rtdrv_timeCfg_t));
            break;

        case RTDRV_TIME_PORT_PTP_RX_TIME_GET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_portPtpRxTimestamp_get(buf.time_cfg.unit, buf.time_cfg.port, buf.time_cfg.identifier, &buf.time_cfg.timeStamp);
            copy_to_user(user, &buf.time_cfg, sizeof(rtdrv_timeCfg_t));
            break;

        case RTDRV_TIME_PORT_PTP_TX_TIME_GET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_portPtpTxTimestamp_get(buf.time_cfg.unit, buf.time_cfg.port, buf.time_cfg.identifier, &buf.time_cfg.timeStamp);
            copy_to_user(user, &buf.time_cfg, sizeof(rtdrv_timeCfg_t));
            break;

        case RTDRV_TIME_REF_TIME_GET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_refTime_get(buf.time_cfg.unit, &buf.time_cfg.timeStamp);
            copy_to_user(user, &buf.time_cfg, sizeof(rtdrv_timeCfg_t));
            break;

        case RTDRV_TIME_REF_TIME_ENABLE_GET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_refTimeEnable_get(buf.time_cfg.unit, &buf.time_cfg.enable);
            copy_to_user(user, &buf.time_cfg, sizeof(rtdrv_timeCfg_t));
            break;

        case RTDRV_TIME_REF_TIME_FREQ_GET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_refTimeFreq_get(buf.time_cfg.unit, &buf.time_cfg.freq);
            copy_to_user(user, &buf.time_cfg, sizeof(rtdrv_timeCfg_t));
            break;

        case RTDRV_TIME_CORRECTION_FIELD_TRANSPARENT_VALUE_GET:
            copy_from_user(&buf.time_cfg, user, sizeof(rtdrv_timeCfg_t));
            ret = rtk_time_correctionFieldTransparentValue_get(buf.time_cfg.unit, buf.time_cfg.oriCf, buf.time_cfg.rxTimeStamp, &buf.time_cfg.transCf);
            copy_to_user(user, &buf.time_cfg, sizeof(rtdrv_timeCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_trap(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** TRAP **/
        case RTDRV_TRAP_RMAACTION_GET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaAction_get(buf.trap_cfg.unit, &buf.trap_cfg.rma_frame, &buf.trap_cfg.rma_action);
            copy_to_user(user, &buf.trap_cfg, sizeof(rtdrv_trapCfg_t));
            break;

        case RTDRV_TRAP_BYPASS_STP_GET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_bypassStp_get(buf.trap_cfg.unit, buf.trap_cfg.bypassStp_frame, &buf.trap_cfg.enable);
            copy_to_user(user, &buf.trap_cfg, sizeof(rtdrv_trapCfg_t));
            break;

        case RTDRV_TRAP_BYPASS_VLAN_GET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_bypassVlan_get(buf.trap_cfg.unit, buf.trap_cfg.bypassVlan_frame, &buf.trap_cfg.enable);
            copy_to_user(user, &buf.trap_cfg, sizeof(rtdrv_trapCfg_t));
            break;

        case RTDRV_TRAP_USERDEFINERMA_GET:
            copy_from_user(&buf.l2_trap_cfg, user, sizeof(rtdrv_trapL2userRmaCfg_t));
            ret = rtk_trap_userDefineRma_get(buf.l2_trap_cfg.unit, buf.l2_trap_cfg.rma_index, &buf.l2_trap_cfg.rma_frame);
            copy_to_user(user, &buf.l2_trap_cfg, sizeof(rtdrv_trapL2userRmaCfg_t));
            break;

        case RTDRV_TRAP_USERDEFINERMAENABLE_GET:
            copy_from_user(&buf.l2_trap_cfg, user, sizeof(rtdrv_trapL2userRmaCfg_t));
            ret = rtk_trap_userDefineRmaEnable_get(buf.l2_trap_cfg.unit, buf.l2_trap_cfg.rma_index, &buf.l2_trap_cfg.enable);
            copy_to_user(user, &buf.l2_trap_cfg, sizeof(rtdrv_trapL2userRmaCfg_t));
            break;

        case RTDRV_TRAP_USERDEFINERMAACTION_GET:
            copy_from_user(&buf.l2_trap_cfg, user, sizeof(rtdrv_trapL2userRmaCfg_t));
            ret = rtk_trap_userDefineRmaAction_get(buf.l2_trap_cfg.unit, buf.l2_trap_cfg.rma_index, &buf.l2_trap_cfg.rma_action);
            copy_to_user(user, &buf.l2_trap_cfg, sizeof(rtdrv_trapL2userRmaCfg_t));
            break;

        case RTDRV_TRAP_MGMTFRAMEACTION_GET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFrameAction_get(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.frameType, (rtk_mgmt_action_t*)&buf.mgm_trap_cfg.rma_action);
            copy_to_user(user, &buf.mgm_trap_cfg, sizeof(rtdrv_trapMgmRmaCfg_t));
            break;

        case RTDRV_TRAP_MGMTFRAMEPRI_GET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFramePri_get(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.frameType, &buf.mgm_trap_cfg.priority);
            copy_to_user(user, &buf.mgm_trap_cfg, sizeof(rtdrv_trapMgmRmaCfg_t));
            break;

        case RTDRV_TRAP_MGMTFRAMEQID_GET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFrameQueue_get(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.qType, &buf.mgm_trap_cfg.qid);
            copy_to_user(user, &buf.mgm_trap_cfg, sizeof(rtdrv_trapMgmRmaCfg_t));
            break;

        case RTDRV_TRAP_PORTMGMTFRAMEACTION_GET:
            copy_from_user(&buf.mgm_trap_cfg, user, sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_portMgmtFrameAction_get(buf.mgm_trap_cfg.unit, buf.mgm_trap_cfg.port,
                        buf.mgm_trap_cfg.frameType, (rtk_mgmt_action_t*)&buf.mgm_trap_cfg.rma_action);
            copy_to_user(user, &buf.mgm_trap_cfg, sizeof(rtdrv_trapMgmRmaCfg_t));
            break;

        case RTDRV_TRAP_PKTWITHCFIACTION_GET:
            copy_from_user(&buf.other_trap_cfg, user, sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_pktWithCFIAction_get(buf.other_trap_cfg.unit,
                    &buf.other_trap_cfg.action);
            copy_to_user(user, &buf.other_trap_cfg, sizeof(rtdrv_trapOtherCfg_t));
            break;

        case RTDRV_TRAP_PKTWITHOUTERCFIACTION_GET:
            copy_from_user(&buf.other_trap_cfg, user, sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_pktWithOuterCFIAction_get(buf.other_trap_cfg.unit,
                    &buf.other_trap_cfg.action);
            copy_to_user(user, &buf.other_trap_cfg, sizeof(rtdrv_trapOtherCfg_t));
            break;

        case RTDRV_TRAP_PKTWITHCFIPRI_GET:
            copy_from_user(&buf.other_trap_cfg, user, sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_pktWithCFIPri_get(buf.other_trap_cfg.unit,
                    &buf.other_trap_cfg.priority);
            copy_to_user(user, &buf.other_trap_cfg, sizeof(rtdrv_trapOtherCfg_t));
            break;
#ifdef CONFIG_SDK_DRIVER_RTK_LEGACY_API
        case RTDRV_TRAP_CFMUNKNOWNFRAMEACT_GET:
            copy_from_user(&buf.misc_cfg, user, sizeof(rtdrv_oamCfmMiscCfg_t));
            ret = rtk_trap_cfmUnknownFrameAct_get(buf.misc_cfg.unit,
                                                  &buf.misc_cfg.action);
            copy_to_user(user, &buf.misc_cfg, sizeof(rtdrv_oamCfmMiscCfg_t));
            break;

        case RTDRV_TRAP_CFMLOOPBACKACT_GET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmLoopbackLinkTraceAct_get(buf.cfm_trap_cfg.unit,
                                              buf.cfm_trap_cfg.md_level,
                                              &buf.cfm_trap_cfg.action);
            copy_to_user(user, &buf.cfm_trap_cfg, sizeof(rtdrv_trapCfmCfg_t));
            break;

        case RTDRV_TRAP_CFMCCMACT_GET:
            copy_from_user(&buf.oam_trap_cfg, user, sizeof(rtdrv_trapOamCfg_t));
            ret = rtk_trap_cfmCcmAct_get(buf.oam_trap_cfg.unit,
                                         buf.oam_trap_cfg.md_level,
                                         &buf.oam_trap_cfg.action);
            copy_to_user(user, &buf.oam_trap_cfg, sizeof(rtdrv_trapOamCfg_t));
            break;

        case RTDRV_TRAP_CFMETHDMACT_GET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmEthDmAct_get(buf.cfm_trap_cfg.unit,
                                           buf.cfm_trap_cfg.md_level,
                                           &buf.cfm_trap_cfg.action);
            copy_to_user(user, &buf.cfm_trap_cfg, sizeof(rtdrv_trapCfmCfg_t));
            break;
#endif  /* CONFIG_SDK_DRIVER_RTK_LEGACY_API */
        case RTDRV_TRAP_CFMFRAMETRAPPRI_GET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmFrameTrapPri_get(buf.cfm_trap_cfg.unit, &buf.cfm_trap_cfg.priority);
            copy_to_user(user, &buf.cfm_trap_cfg, sizeof(rtdrv_trapCfmCfg_t));
            break;

        case RTDRV_TRAP_OAMPDUACTION_GET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_oamPDUAction_get(buf.cfm_trap_cfg.unit, &buf.cfm_trap_cfg.action);
            copy_to_user(user, &buf.cfm_trap_cfg, sizeof(rtdrv_trapCfmCfg_t));
            break;

        case RTDRV_TRAP_OAMPDUPRI_GET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_oamPDUPri_get(buf.cfm_trap_cfg.unit, &buf.cfm_trap_cfg.priority);
            copy_to_user(user, &buf.cfm_trap_cfg, sizeof(rtdrv_trapCfmCfg_t));
            break;

        case RTDRV_TRAP_PORTOAMLOOPBACKPARACTION_GET:
            copy_from_user(&buf.oam_trap_cfg, user, sizeof(rtdrv_trapOamCfg_t));
            ret = rtk_trap_portOamLoopbackParAction_get(buf.oam_trap_cfg.unit,
                    buf.oam_trap_cfg.port, &buf.oam_trap_cfg.action);
            copy_to_user(user, &buf.oam_trap_cfg, sizeof(rtdrv_trapOamCfg_t));
            break;

        case RTDRV_TRAP_ROUTEEXCEPTIONACTION_GET:
            copy_from_user(&buf.routeException_trap_cfg, user,
                    sizeof(rtdrv_trapRouteExceptionCfg_t));
            ret = rtk_trap_routeExceptionAction_get(
                    buf.routeException_trap_cfg.unit,
                    buf.routeException_trap_cfg.type,
                    &buf.routeException_trap_cfg.action);
            copy_to_user(user, &buf.routeException_trap_cfg,
                    sizeof(rtdrv_trapRouteExceptionCfg_t));
            break;

        case RTDRV_TRAP_ROUTEEXCEPTIONPRI_GET:
            copy_from_user(&buf.routeException_trap_cfg, user,
                    sizeof(rtdrv_trapRouteExceptionCfg_t));
            ret = rtk_trap_routeExceptionPri_get(
                    buf.routeException_trap_cfg.unit,
                    buf.routeException_trap_cfg.type,
                    &buf.routeException_trap_cfg.priority);
            copy_to_user(user, &buf.routeException_trap_cfg,
                    sizeof(rtdrv_trapRouteExceptionCfg_t));
            break;

        case RTDRV_TRAP_USERDEFINERMALEARNINGENABLE_GET:
            copy_from_user(&buf.mgmuser_trap_cfg, user,
                    sizeof(rtdrv_trapUserMgmRmaCfg_t));
            ret = rtk_trap_userDefineRmaLearningEnable_get(
                    buf.mgmuser_trap_cfg.unit, buf.mgmuser_trap_cfg.mgmt_idx,
                    &buf.mgmuser_trap_cfg.enable);
            copy_to_user(user, &buf.mgmuser_trap_cfg,
                    sizeof(rtdrv_trapUserMgmRmaCfg_t));
            break;

        case RTDRV_TRAP_RMALEARNINGENABLE_GET:
            copy_from_user(&buf.trap_cfg, user,
                    sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaLearningEnable_get(
                    buf.trap_cfg.unit, &buf.trap_cfg.rma_frame,
                    &buf.trap_cfg.enable);
            copy_to_user(user, &buf.trap_cfg, sizeof(rtdrv_trapCfg_t));
            break;

        case RTDRV_TRAP_MGMTFRAMELEARNINGENABLE_GET:
            copy_from_user(&buf.mgm_trap_cfg, user,
                    sizeof(rtdrv_trapMgmRmaCfg_t));
            ret = rtk_trap_mgmtFrameLearningEnable_get(buf.mgm_trap_cfg.unit,
                    buf.mgm_trap_cfg.frameType, &buf.mgm_trap_cfg.enable);
            copy_to_user(user, &buf.mgm_trap_cfg,
                    sizeof(rtdrv_trapMgmRmaCfg_t));
            break;

        case RTDRV_TRAP_MGMTFRAMEMGMTVLANENABLE_GET:
            copy_from_user(&buf.other_trap_cfg, user,
                    sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_mgmtFrameMgmtVlanEnable_get(buf.other_trap_cfg.unit,
                    &buf.other_trap_cfg.enable);
            copy_to_user(user, &buf.other_trap_cfg,
                    sizeof(rtdrv_trapOtherCfg_t));
            break;

        case RTDRV_TRAP_BPDUFLOODPORTMASK_GET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_bpduFloodPortmask_get(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            copy_to_user(user, &buf.flood_pmsk_cfg,sizeof(rtdrv_floodPmskCfg_t));
            break;

        case RTDRV_TRAP_EAPOLFLOODPORTMASK_GET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_eapolFloodPortmask_get(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            copy_to_user(user, &buf.flood_pmsk_cfg,sizeof(rtdrv_floodPmskCfg_t));
            break;

        case RTDRV_TRAP_LLDPFLOODPORTMASK_GET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_lldpFloodPortmask_get(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            copy_to_user(user, &buf.flood_pmsk_cfg,sizeof(rtdrv_floodPmskCfg_t));
            break;

        case RTDRV_TRAP_USERDEFINEFLOODPORTMASK_GET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_userDefineFloodPortmask_get(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            copy_to_user(user, &buf.flood_pmsk_cfg,sizeof(rtdrv_floodPmskCfg_t));
            break;

        case RTDRV_TRAP_RMAFLOODPORTMASK_GET:
            copy_from_user(&buf.flood_pmsk_cfg, user,sizeof(rtdrv_floodPmskCfg_t));
            ret = rtk_trap_rmaFloodPortmask_get(buf.flood_pmsk_cfg.unit, &buf.flood_pmsk_cfg.pmsk);
            copy_to_user(user, &buf.flood_pmsk_cfg,sizeof(rtdrv_floodPmskCfg_t));
            break;

        case RTDRV_TRAP_RMACANCELMIRROR_GET:
            copy_from_user(&buf.trap_cfg, user,sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaCancelMirror_get(buf.trap_cfg.unit, &buf.trap_cfg.enable);
            copy_to_user(user, &buf.trap_cfg,sizeof(rtdrv_trapCfg_t));
            break;

        case RTDRV_TRAP_RMAGROUPACTION_GET:
            copy_from_user(&buf.rma_grp_act_cfg, user,
                    sizeof(rtdrv_rmaGroupType_t));
            ret = rtk_trap_rmaGroupAction_get(
                    buf.rma_grp_act_cfg.unit, buf.rma_grp_act_cfg.rmaGroup_frameType, &buf.rma_grp_act_cfg.rma_action);
            copy_to_user(user, &buf.rma_grp_act_cfg,
                    sizeof(rtdrv_rmaGroupType_t));
            break;

        case RTDRV_TRAP_RMAGROUPLEARNINGENABLE_GET:
            copy_from_user(&buf.rma_grp_lrn_cfg, user,
                    sizeof(rtdrv_rmaGroupLearn_t));
            ret = rtk_trap_rmaGroupLearningEnable_get(
                    buf.rma_grp_lrn_cfg.unit, buf.rma_grp_lrn_cfg.rmaGroup_frameType, &buf.rma_grp_lrn_cfg.enable);
            copy_to_user(user, &buf.rma_grp_lrn_cfg,
                    sizeof(rtk_enable_t));
            break;

        case RTDRV_TRAP_MGMTFRAMESELFARPENABLE_GET:
            copy_from_user(&buf.other_trap_cfg, user,
                    sizeof(rtdrv_trapOtherCfg_t));
            ret = rtk_trap_mgmtFrameSelfARPEnable_get(buf.other_trap_cfg.unit,
                    &buf.other_trap_cfg.enable);
            copy_to_user(user, &buf.other_trap_cfg,
                    sizeof(rtdrv_trapOtherCfg_t));
            break;

        case RTDRV_TRAP_RMALOOKUPMISSACTIONENABLE_GET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_rmaLookupMissActionEnable_get(buf.trap_cfg.unit, &buf.trap_cfg.enable);
            copy_to_user(user, &buf.trap_cfg, sizeof(rtdrv_trapCfg_t));
            break;

        case RTDRV_TRAP_CFMACT_GET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmAct_get(buf.cfm_trap_cfg.unit, buf.cfm_trap_cfg.type,
                    buf.cfm_trap_cfg.md_level, &buf.cfm_trap_cfg.action);
            copy_to_user(user, &buf.cfm_trap_cfg, sizeof(rtdrv_trapCfmCfg_t));
            break;

        case RTDRV_TRAP_CFMTARGET_GET:
            copy_from_user(&buf.cfm_trap_cfg, user, sizeof(rtdrv_trapCfmCfg_t));
            ret = rtk_trap_cfmTarget_get(buf.cfm_trap_cfg.unit, &buf.cfm_trap_cfg.target);
            copy_to_user(user, &buf.cfm_trap_cfg, sizeof(rtdrv_trapCfmCfg_t));
            break;

        case RTDRV_TRAP_OAMTARGET_GET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_oamTarget_get(buf.trap_cfg.unit, &buf.trap_cfg.target);
            copy_to_user(user, &buf.trap_cfg, sizeof(rtdrv_trapCfg_t));
            break;

        case RTDRV_TRAP_MGMTFRAMETARGET_GET:
            copy_from_user(&buf.trap_cfg, user, sizeof(rtdrv_trapCfg_t));
            ret = rtk_trap_mgmtFrameTarget_get(buf.trap_cfg.unit, &buf.trap_cfg.target);
            copy_to_user(user, &buf.trap_cfg, sizeof(rtdrv_trapCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_acl(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** ACL **/
        case RTDRV_ACL_ENTRY_FIELD_SIZE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntryFieldSize_get(buf.acl_cfg.unit, buf.acl_cfg.field_type, &buf.acl_cfg.size);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_ENTRY_SIZE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntrySize_get(buf.acl_cfg.unit, buf.acl_cfg.phase, &buf.acl_cfg.size);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_ENTRY_DATA_READ:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntry_read(buf.acl_cfg.unit, buf.acl_cfg.phase, buf.acl_cfg.index, buf.acl_cfg.entry_buffer);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEVALIDATE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleValidate_get(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, &buf.acl_cfg.status);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEENTRYFIELD_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntryField_get(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, buf.acl_cfg.entry_buffer,
                    buf.acl_cfg.field_type, buf.acl_cfg.field_data,
                    buf.acl_cfg.field_mask);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEENTRYFIELD_READ:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntryField_read(buf.acl_cfg.unit,
                    buf.acl_cfg.phase, buf.acl_cfg.index,
                    buf.acl_cfg.field_type, buf.acl_cfg.field_data,
                    buf.acl_cfg.field_mask);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEENTRYFIELD_CHECK:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleEntryField_check(buf.acl_cfg.unit,
                    buf.acl_cfg.phase, buf.acl_cfg.field_type);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEOPERATION_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleOperation_get(buf.acl_cfg.unit,
                    buf.acl_cfg.phase, buf.acl_cfg.index,
                    &buf.acl_cfg.oper);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEACTION_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleAction_get(buf.acl_cfg.unit,
                    buf.acl_cfg.phase, buf.acl_cfg.index,
                    &buf.acl_cfg.action);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;
#if ((defined CONFIG_SDK_RTL8390) || (defined CONFIG_SDK_RTL8380))
        case RTDRV_ACL_BLOCKPWRENABLE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_blockPwrEnable_get(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, &buf.acl_cfg.status);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_BLOCKAGGREGATORENABLE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_blockGroupEnable_get(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, buf.acl_cfg.blk_group,
                    &buf.acl_cfg.status);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_STATPKTCNT_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_statPktCnt_get(buf.acl_cfg.unit, buf.acl_cfg.index,
                    &buf.acl_cfg.size);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_STATBYTECNT_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_statByteCnt_get(buf.acl_cfg.unit, buf.acl_cfg.index,
                    &buf.acl_cfg.count);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_METER_MODE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_meterMode_get(buf.acl_cfg.unit, buf.acl_cfg.blockIdx, &buf.acl_cfg.meterMode);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_METER_BURST_SIZE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_meterBurstSize_get(buf.acl_cfg.unit, buf.acl_cfg.meterMode, &buf.acl_cfg.burstSize);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RANGECHECKL4PORT_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckL4Port_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_l4Port);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;

        case RTDRV_ACL_RANGECHECKVID_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckVid_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_vid);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;

        case RTDRV_ACL_RANGECHECKSRCPORT_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckSrcPort_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_port);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;

        case RTDRV_ACL_RANGECHECKPACKETLEN_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckPacketLen_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_pktLen);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;
#endif  /* defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL8380) */
#if (defined CONFIG_SDK_RTL9300)
        case RTDRV_ACL_LOOPBACKENABLE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_loopBackEnable_get(buf.acl_cfg.unit, &buf.acl_cfg.enable);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;
#endif
#if ((defined CONFIG_SDK_RTL9300) || (defined CONFIG_SDK_RTL9310))
        case RTDRV_ACL_LIMITLOOPBACKTIMES_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_limitLoopbackTimes_get(buf.acl_cfg.unit, &buf.acl_cfg.lb_times);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;
#endif
#if (defined CONFIG_SDK_RTL8380)
        case RTDRV_ACL_PORTLOOKUPENABLE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_portLookupEnable_get(buf.acl_cfg.unit,
                    buf.acl_cfg.port, &buf.acl_cfg.status);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_LOOKUPMISSACT_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_lookupMissAct_get(buf.acl_cfg.unit,
                    buf.acl_cfg.port, &buf.acl_cfg.lmAct);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RANGECHECKFIELDSEL_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckFieldSelector_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_fieldSel);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;
#endif  /* (defined CONFIG_SDK_RTL8380) */
#if (defined CONFIG_SDK_RTL8390)
        case RTDRV_ACL_PARTITION_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_partition_get(buf.acl_cfg.unit, &buf.acl_cfg.blockIdx);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_TEMPLATEFIELDINTENTVLANTAG_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_templateFieldIntentVlanTag_get(buf.acl_cfg.unit,
                    &buf.acl_cfg.tagType);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RANGECHECKDSTPORT_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_acl_rangeCheckDstPort_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_port);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;

        case RTDRV_ACL_BLOCKRESULTMODE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_blockResultMode_get(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, &buf.acl_cfg.blk_mode);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;
#endif  /* defined(CONFIG_SDK_RTL8390) */
        case RTDRV_ACL_PORTPHASELOOKUPENABLE_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_portPhaseLookupEnable_get(buf.acl_cfg.unit,
                    buf.acl_cfg.port, buf.acl_cfg.phase, &buf.acl_cfg.status);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_TEMPLATESELECTOR_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_templateSelector_get(buf.acl_cfg.unit,
                    buf.acl_cfg.blockIdx, &buf.acl_cfg.template_idx);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_STATCNT_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_statCnt_get(buf.acl_cfg.unit, buf.acl_cfg.phase,
                    buf.acl_cfg.index, buf.acl_cfg.mode, &buf.acl_cfg.cnt);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_ACL_RULEHITINDICATION_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_acl_ruleHitIndication_get(buf.acl_cfg.unit,
                    buf.acl_cfg.phase, buf.acl_cfg.index,
                    buf.acl_cfg.reset, &buf.acl_cfg.isHit);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_pie(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** PIE **/
        case RTDRV_PIE_METER_INCLUDE_IFG_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterIncludeIfg_get(buf.pie_cfg.unit, &buf.pie_cfg.ifg_include);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_METER_ENTRY_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterEntry_get(buf.pie_cfg.unit, buf.pie_cfg.meterIdx, &buf.pie_cfg.meterEntry);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_METER_EXCEED_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterExceed_get(buf.pie_cfg.unit, buf.pie_cfg.meterIdx, &buf.pie_cfg.isExceed);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_METER_EXCEED_AGGREGATION_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterExceedAggregation_get(buf.pie_cfg.unit, &buf.pie_cfg.exceedMask);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_BLOCKLOOKUPENABLE_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_blockLookupEnable_get(buf.pie_cfg.unit,
                    buf.pie_cfg.blockIdx, &buf.pie_cfg.status);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_BLOCKGROUPING_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_aclCfg_t));
            ret = rtk_pie_blockGrouping_get(buf.pie_cfg.unit,
                    buf.pie_cfg.blockIdx, &buf.pie_cfg.grpId,
                    &buf.pie_cfg.logicId);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_aclCfg_t));
            break;

        case RTDRV_PIE_TEMPLATE_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_template_get(buf.pie_cfg.unit,
                    buf.pie_cfg.index, &buf.pie_cfg.template);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_TEMPLATEFIELD_CHECK:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_templateField_check(buf.pie_cfg.unit,
                    buf.pie_cfg.phase, buf.pie_cfg.field_type);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_RANGECHECKIP_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_pie_rangeCheckIp_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range_ip);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;

        case RTDRV_PIE_RANGECHECK_GET:
            copy_from_user(&buf.rangeCheck_cfg, user, sizeof(rtdrv_rangeCheckCfg_t));
            ret = rtk_pie_rangeCheck_get(buf.rangeCheck_cfg.unit,
                    buf.rangeCheck_cfg.index, &buf.rangeCheck_cfg.range);
            copy_to_user(user, &buf.rangeCheck_cfg, sizeof(rtdrv_rangeCheckCfg_t));
            break;

        case RTDRV_PIE_FIELDSELECTOR_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_fieldSelector_get(buf.pie_cfg.unit, buf.pie_cfg.index,
                    &buf.pie_cfg.fs);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_PHASE_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_phase_get(buf.pie_cfg.unit, buf.pie_cfg.blockIdx, (rtk_pie_phase_t *)&buf.pie_cfg.phase);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_TEMPLATE_VLANSEL_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_templateVlanSel_get(buf.pie_cfg.unit, buf.pie_cfg.phase, buf.pie_cfg.templateIdx, &buf.pie_cfg.vlanSel);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_METER_DPSEL_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterDpSel_get(buf.pie_cfg.unit, &buf.pie_cfg.dpSel);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_ARPMACSEL_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_arpMacSel_get(buf.pie_cfg.unit, &buf.pie_cfg.arpMacSel);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_INTFSEL_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_intfSel_get(buf.pie_cfg.unit, &buf.pie_cfg.intfSel);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_TEMPLATEVLANFMTSEL_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_templateVlanFmtSel_get(buf.pie_cfg.unit, buf.pie_cfg.phase, buf.pie_cfg.templateIdx, &buf.pie_cfg.vlanFmtSel);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        case RTDRV_PIE_METERTRTCMTYPE_GET:
            copy_from_user(&buf.pie_cfg, user, sizeof(rtdrv_pieCfg_t));
            ret = rtk_pie_meterTrtcmType_get(buf.pie_cfg.unit, &buf.pie_cfg.type);
            copy_to_user(user, &buf.pie_cfg, sizeof(rtdrv_pieCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_qos(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** QOS **/
        case RTDRV_QOS_QUEUE_NUM_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_queueNum_get(buf.qos_cfg.unit, &buf.qos_cfg.queue_num);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PRI_MAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priMap_get(buf.qos_cfg.unit, buf.qos_cfg.queue_num, &buf.qos_cfg.pri2qid);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PRI2QID_MAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_pri2QidMap_get(buf.qos_cfg.unit, buf.qos_cfg.int_pri, &buf.qos_cfg.queue);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_CPUQID2QID_MAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_cpuQid2QidMap_get(buf.qos_cfg.unit, buf.qos_cfg.cpuQid, &buf.qos_cfg.queue);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_CPUQID2SQID_MAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_cpuQid2StackQidMap_get(buf.qos_cfg.unit, buf.qos_cfg.cpuQid, &buf.qos_cfg.queue);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_1P_PRI_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pPriRemap_get(buf.qos_cfg.unit, buf.qos_cfg.dot1p_pri, &buf.qos_cfg.int_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_OUTER_1P_PRI_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pPriRemap_get(buf.qos_cfg.unit, buf.qos_cfg.dot1p_pri, buf.qos_cfg.dei, &(buf.qos_cfg.int_pri));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#endif

        case RTDRV_QOS_DEI_DP_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiDpRemap_get(buf.qos_cfg.unit, buf.qos_cfg.dei, &buf.qos_cfg.dp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_DEI_SRC_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDEISrcSel_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.deiSrc);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_DP_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDpSel_get(buf.qos_cfg.unit, buf.qos_cfg.port, &(buf.qos_cfg.weightOfDpSel));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_DSCP_DP_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpDpRemap_get(buf.qos_cfg.unit, buf.qos_cfg.dscp, &buf.qos_cfg.dp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#endif
        case RTDRV_QOS_DP_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dpRemap_get(buf.qos_cfg.unit, buf.qos_cfg.dpSrcType, buf.qos_cfg.dpSrcRemap, &buf.qos_cfg.dp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PRI_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priRemap_get(buf.qos_cfg.unit, buf.qos_cfg.priSrcType, buf.qos_cfg.priSrcRemap, &buf.qos_cfg.int_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_DSCP_PRI_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpPriRemap_get(buf.qos_cfg.unit, buf.qos_cfg.dscp, &buf.qos_cfg.int_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_PRI_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPri_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.int_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_INNER_PRI_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portInnerPri_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.int_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_OUTER_PRI_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuterPri_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.int_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#endif

        case RTDRV_QOS_DP_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dpSrcSel_get(buf.qos_cfg.unit, &buf.qos_cfg.dpSrcType);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PRI_SEL_GROUP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priSelGroup_get(buf.qos_cfg.unit, buf.qos_cfg.index, &(buf.qos_cfg.priSelWeight));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_PRI_SEL_GROUP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPriSelGroup_get(buf.qos_cfg.unit, buf.qos_cfg.port, &(buf.qos_cfg.index));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_1P_REMARK_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pRemarkEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.remark_enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_1P_REMARKING_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pRemarking_get(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_1p, buf.qos_cfg.rmkval_1p, &buf.qos_cfg.dot1p_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_1P_REMARK_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pRemarkSrcSel_get(buf.qos_cfg.unit, &buf.qos_cfg.rmksrc_1p);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPri_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.dot1p_dflt_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_EXT_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPriExt_get(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, &buf.qos_cfg.dot1p_dflt_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPriSrcSel_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.dflt_src_1p);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_1P_DFLT_PRI_SRC_SEL_EXT_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_port1pDfltPriSrcSelExt_get(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, &buf.qos_cfg.dflt_src_1p);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_1P_DFLT_PRI_CFG_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pDfltPriCfgSrcSel_get(buf.qos_cfg.unit, &buf.qos_cfg.dot1p_dflt_cfg_dir);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_OUT_1P_REMARK_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOut1pRemarkEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.remark_enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_OUTER_1P_REMARKING_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pRemarking_get(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_outer1p, buf.qos_cfg.rmkval_outer1p, &buf.qos_cfg.dot1p_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_OUT_1P_REMARK_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pRemarkSrcSel_get(buf.qos_cfg.unit, &buf.qos_cfg.rmksrc_outer1p);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_OUTER_1P_DFLT_PRI_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPri_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.out1p_dflt_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_OUTER_1P_DFLT_PRI_EXT_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPriExt_get(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, &buf.qos_cfg.out1p_dflt_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_OUTER_1P_DFLT_PRI_CFG_SRC_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pDfltPriCfgSrcSel_get(buf.qos_cfg.unit,&buf.qos_cfg.out1p_dflt_cfg_dir);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_DSCP_REMARK_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDscpRemarkEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.remark_enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DSCP_REMARKING_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpRemarking_get(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_dscp, buf.qos_cfg.rmkval_dscp, &buf.qos_cfg.dscp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DSCP_REMARK_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpRemarkSrcSel_get(buf.qos_cfg.unit, &buf.qos_cfg.rmksrc_dscp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_DEI_REMARK_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDeiRemarkEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.remark_enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DEI_REMARKING_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiRemarking_get(buf.qos_cfg.unit, buf.qos_cfg.rmksrc_dei, buf.qos_cfg.rmkval_dei, &buf.qos_cfg.dei);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_DEI_REMARK_TAG_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portDeiRemarkTagSel_get(buf.qos_cfg.unit, buf.qos_cfg.port, &(buf.qos_cfg.deiSrc));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DEI_REMARK_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiRemarkSrcSel_get(buf.qos_cfg.unit, &buf.qos_cfg.rmksrc_dei);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_1P_DFLT_PRI_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pDfltPri_get(buf.qos_cfg.unit, &buf.qos_cfg.dot1p_dflt_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_1P_REMARK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pRemark_get(buf.qos_cfg.unit, buf.qos_cfg.int_pri, &buf.qos_cfg.dot1p_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_OUT_1P_REMARK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pRemark_get(buf.qos_cfg.unit, buf.qos_cfg.int_pri, &buf.qos_cfg.dot1p_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#endif

        case RTDRV_QOS_PORT_OUTER_1P_DFLT_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPriSrcSel_get(buf.qos_cfg.unit, buf.qos_cfg.port, &(buf.qos_cfg.out1p_dflt_src));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_OUTER_1P_DFLT_SRC_SEL_EXT_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pDfltPriSrcSelExt_get(buf.qos_cfg.unit, buf.qos_cfg.srcUnit, buf.qos_cfg.port, &(buf.qos_cfg.out1p_dflt_src));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_1P_DFLT_PRI_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_1pDfltPriSrcSel_get(buf.qos_cfg.unit, &buf.qos_cfg.dflt_src_1p);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_OUTER_1P_DFLT_PRI_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_outer1pDfltPri_get(buf.qos_cfg.unit, &buf.qos_cfg.out1p_dflt_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_DSCP_REMARK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscpRemark_get(buf.qos_cfg.unit, buf.qos_cfg.int_pri, &buf.qos_cfg.dscp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DSCP2DSCP_REMARK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscp2DscpRemark_get(buf.qos_cfg.unit, buf.qos_cfg.org_dscp, &buf.qos_cfg.dscp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DSCP2DOT1P_REMARK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscp2Dot1pRemark_get(buf.qos_cfg.unit, buf.qos_cfg.org_dscp, &buf.qos_cfg.dot1p_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DSCP2OUT1P_REMARK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_dscp2Outer1pRemark_get(buf.qos_cfg.unit, buf.qos_cfg.org_dscp, &buf.qos_cfg.dot1p_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_DEI_REMARK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_deiRemark_get(buf.qos_cfg.unit, buf.qos_cfg.dp, &(buf.qos_cfg.dei));
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#endif

        case RTDRV_QOS_SCHEDULING_ALGORITHM_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_schedulingAlgorithm_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.scheduling_type);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_SCHEDULING_QUEUE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_schedulingQueue_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.qweights);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_CONG_AVOID_ALGO_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidAlgo_get(buf.qos_cfg.unit, &buf.qos_cfg.congAvoid_algo);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_CONG_AVOID_ALGO_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portCongAvoidAlgo_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.congAvoid_algo);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_CONG_AVOID_SYS_THRESH_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidSysThresh_get(buf.qos_cfg.unit, buf.qos_cfg.dp, &buf.qos_cfg.congAvoid_thresh);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_CONG_AVOID_SYS_DROP_PROBABILITY_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidSysDropProbability_get(buf.qos_cfg.unit, buf.qos_cfg.dp, &buf.qos_cfg.data);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_CONG_AVOID_GLOBAL_QUEUE_THRESH_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidGlobalQueueThresh_get(buf.qos_cfg.unit, buf.qos_cfg.queue, buf.qos_cfg.dp, &buf.qos_cfg.congAvoid_thresh);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_CONG_AVOID_GLOBAL_QUEUE_DROP_PROBABILITY_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidGlobalQueueDropProbability_get(buf.qos_cfg.unit, buf.qos_cfg.queue, buf.qos_cfg.dp, &buf.qos_cfg.data);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#endif

        case RTDRV_QOS_CONG_AVOID_GLOBAL_QUEUE_CONFIG_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_congAvoidGlobalQueueConfig_get(buf.qos_cfg.unit, buf.qos_cfg.queue, buf.qos_cfg.dp, &buf.qos_cfg.congAvoid_thresh);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_AVB_SR_CLASS_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portAvbStreamReservationClassEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.srClass, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_AVB_SR_CONFIG_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_avbStreamReservationConfig_get(buf.qos_cfg.unit, &buf.qos_cfg.srConf);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PKT2CPU_PRI_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_pkt2CpuPriRemap_get(buf.qos_cfg.unit, buf.qos_cfg.int_pri, &buf.qos_cfg.new_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_RSPAN_PRI_REMAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_rspanPriRemap_get(buf.qos_cfg.unit, buf.qos_cfg.rspan_pri, &buf.qos_cfg.int_pri);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PRI2IGR_QUEUE_MAP_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPri2IgrQMap_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.pri2qid);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PRI2IGR_QUEUE_MAP_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPri2IgrQMapEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_IGR_QUEUE_WEIGHT_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portIgrQueueWeight_get(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.queue, &buf.qos_cfg.data);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_IGR_QUEUE_STRICT_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portIgrQueueStrictEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.queue, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_OUTER_1P_REMARK_SRC_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuter1pRemarkSrcSel_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.rmksrc_outer1p);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_INVLD_DSCP_VAL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_invldDscpVal_get(buf.qos_cfg.unit, &buf.qos_cfg.dscp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_INVLD_DSCP_MASK_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_invldDscpMask_get(buf.qos_cfg.unit, &buf.qos_cfg.dscp);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_INVLD_DSCP_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portInvldDscpEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_INVLD_DSCP_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_invldDscpEnable_get(buf.qos_cfg.unit, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_QOS_SYS_PORT_PRI_REMAP_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPriRemapEnable_get(buf.qos_cfg.unit, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;
#endif

        case RTDRV_QOS_SYS_PORT_PRI_REMAP_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_sysPortPriRemapSel_get(buf.qos_cfg.unit, &buf.qos_cfg.portPriRemap_type);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_PORT_PRI_REMAP_SEL_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portPortPriRemapSel_get(buf.qos_cfg.unit, buf.qos_cfg.port, &buf.qos_cfg.portPriRemap_type);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_IPRI_REMAP_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portInnerPriRemapEnable_get(buf.qos_cfg.unit, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_OPRI_REMAP_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portOuterPriRemapEnable_get(buf.qos_cfg.unit, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PRI_REMAP_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_priRemapEnable_get(buf.qos_cfg.unit, buf.qos_cfg.priSrcType, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        case RTDRV_QOS_PORT_QUEUE_STRICT_ENABLE_GET:
            copy_from_user(&buf.qos_cfg, user, sizeof(rtdrv_qosCfg_t));
            ret = rtk_qos_portQueueStrictEnable_get(buf.qos_cfg.unit, buf.qos_cfg.port, buf.qos_cfg.queue, &buf.qos_cfg.enable);
            copy_to_user(user, &buf.qos_cfg, sizeof(rtdrv_qosCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_trunk(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** TRUNK **/
        case RTDRV_TRUNK_MODE_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_mode_get(buf.trunk_cfg.unit, &buf.trunk_cfg.mode);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_PORT_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_port_get(buf.trunk_cfg.unit,buf.trunk_cfg.trk_gid, &buf.trunk_cfg.trk_member);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_LOCAL_PORT_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_localPort_get(buf.trunk_cfg.unit,buf.trunk_cfg.trk_gid, &buf.trunk_cfg.trk_member);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_EGR_PORT_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_egrPort_get(buf.trunk_cfg.unit,buf.trunk_cfg.trk_gid, &buf.trunk_cfg.trk_egr_ports);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_BIND_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmBind_get(buf.trunk_cfg.unit,buf.trunk_cfg.trk_gid, &buf.trunk_cfg.algo_id);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_TYPE_BIND_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmTypeBind_get(buf.trunk_cfg.unit,buf.trunk_cfg.trk_gid,buf.trunk_cfg.bindType, &buf.trunk_cfg.algo_id);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_PARAM_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmParam_get(buf.trunk_cfg.unit,buf.trunk_cfg.algo_id, &buf.trunk_cfg.algo_bitmask);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_TYPE_PARAM_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmTypeParam_get(buf.trunk_cfg.unit,buf.trunk_cfg.paramType, buf.trunk_cfg.algo_id, &buf.trunk_cfg.algo_bitmask);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_SHIFT_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmShift_get(buf.trunk_cfg.unit,buf.trunk_cfg.algo_id, &buf.trunk_cfg.shift);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_DISTRIBUTION_ALGORITHM_SHIFT_GBL_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_distributionAlgorithmShiftGbl_get(buf.trunk_cfg.unit, &buf.trunk_cfg.shift);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_TRAFFIC_SEPARATE_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_trafficSeparate_get(buf.trunk_cfg.unit,buf.trunk_cfg.trk_gid, &buf.trunk_cfg.separate);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_TRAFFIC_SEPARATE_ENABLE_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_trafficSeparateEnable_get(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid, buf.trunk_cfg.separate, &buf.trunk_cfg.enable);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_TRAFFIC_SEPARATE_DIVISION_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_trafficSeparateDivision_get(buf.trunk_cfg.unit, &buf.trunk_cfg.enable);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_TUNNEL_HASH_SRC_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_tunnelHashSrc_get(buf.trunk_cfg.unit, &buf.trunk_cfg.tunnelHashSrc);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_STACK_TRUNK_PORT_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_stkTrkPort_get(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid,  &buf.trunk_cfg.trk_member);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;


        case RTDRV_TRUNK_STACK_TRUNK_HASH_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_stkTrkHash_get(buf.trunk_cfg.unit, &buf.trunk_cfg.stkTrkHash);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_STACK_DIST_ALGO_TYPE_BIND_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_stkDistributionAlgorithmTypeBind_get(buf.trunk_cfg.unit, buf.trunk_cfg.trk_gid,
                buf.trunk_cfg.bindType, &buf.trunk_cfg.algo_id);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_LOCALFIRST_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_localFirst_get(buf.trunk_cfg.unit, &buf.trunk_cfg.localFirst);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        case RTDRV_TRUNK_LOCALFIRSTFAILOVER_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_localFirstFailOver_get(buf.trunk_cfg.unit, &buf.trunk_cfg.congstAvoid, &buf.trunk_cfg.linkFailAvoid);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;


        case RTDRV_TRUNK_SRCPORTMAP_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_trunkCfg_t));
            ret = rtk_trunk_srcPortMap_get(buf.trunk_cfg.unit, buf.trunk_cfg.unitPort, &buf.trunk_cfg.isTrkMbr, &buf.trunk_cfg.trk_gid);
            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_trunkCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_debug(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** DEBUG **/
        case RTDRV_DEBUG_EN_LOG_GET:
            ret = rt_log_enable_get(&buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_LOGLV_GET:
            ret = rt_log_level_get(&buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_LOGLVMASK_GET:
            ret = rt_log_mask_get(&buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_LOGTYPE_GET:
            ret = rt_log_type_get(&buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_LOGFORMAT_GET:
            ret = rt_log_format_get(&buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_MODMASK_GET:
            ret = rt_log_moduleMask_get(&buf.unit_cfg.data64);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_LOGCFG_GET:
            ret = rt_log_config_get((uint32 *)&buf.log_cfg);
            copy_to_user(user, &buf.log_cfg, sizeof(rtdrv_logCfg_t));
            break;

        case RTDRV_DEBUG_MEM_READ:
            copy_from_user(&buf.reg_cfg, user, sizeof(rtdrv_regCfg_t));
            ret = debug_mem_read(buf.reg_cfg.unit, buf.reg_cfg.reg, &buf.reg_cfg.value);
            copy_to_user(user, &buf.reg_cfg, sizeof(rtdrv_regCfg_t));
            break;

#if defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL8380)||defined(CONFIG_SDK_RTL9310)||defined(CONFIG_SDK_RTL9300)
        case RTDRV_DEBUG_MIB_DBG_CNTR_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_getDbgCntr(buf.unit_cfg.unit, buf.unit_cfg.mibType, &buf.unit_cfg.cntr);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
#endif
#if defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL9310) || defined(CONFIG_SDK_RTL9300)
        case RTDRV_DEBUG_FLOWCTRL_PORT_USED_PAGE_CNT_INGRESS_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_getFlowCtrlIgrPortUsedPageCnt(buf.unit_cfg.unit, buf.unit_cfg.port, &buf.unit_cfg.cntr, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_FLOWCTRL_PORT_USED_PAGE_CNT_EGRESS_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_getFlowCtrlEgrPortUsedPageCnt(buf.unit_cfg.unit, buf.unit_cfg.port, &buf.unit_cfg.cntr, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_FLOWCTRL_SYSTEM_USED_PAGE_CNT_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_getFlowCtrlSystemUsedPageCnt(buf.unit_cfg.unit, &buf.unit_cfg.cntr, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_FLOWCTRL_PORT_QUEUE_USED_PAGE_CNT_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_getFlowCtrlPortQueueUsedPageCnt(buf.unit_cfg.unit, buf.unit_cfg.port,
                &buf.unit_cfg.qCntr, &buf.unit_cfg.qMaxCntr);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
#endif
#if defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310)
        case RTDRV_DEBUG_WATCHDOG_CNT_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_getWatchdogCnt(buf.unit_cfg.unit, buf.unit_cfg.cntr, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
#endif
#if defined(CONFIG_SDK_RTL9310)
        case RTDRV_DEBUG_FLOWCTRL_RPECT_QUEUE_CNTR_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_getFlowCtrlRepctQueueCntrInfo(buf.unit_cfg.unit, &buf.unit_cfg.repctCntr);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_DEBUG_REPCTQ_EMPTY_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_repctQueueEmptyStatus_get(buf.unit_cfg.unit, &buf.unit_cfg.enable);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
        case RTDRV_DEBUG_REPCTQ_STICK_ENABLE_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_repctQueueStickEnable_get(buf.unit_cfg.unit, &buf.unit_cfg.enable);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
        case RTDRV_DEBUG_REPCTQ_FETCH_ENABLE_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = hal_repctQueueFetchEnable_get(buf.unit_cfg.unit, &buf.unit_cfg.enable);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_uart(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_UART1)
        case RTDRV_UART1_GETC:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = drv_uart_getc(buf.unit_cfg.unit, &buf.unit_cfg.data8, buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;

        case RTDRV_UART1_BAUDRATE_GET:
            copy_from_user(&buf.unit_cfg, user, sizeof(rtdrv_unitCfg_t));
            ret = drv_uart_baudrate_get(buf.unit_cfg.unit, &buf.unit_cfg.data);
            copy_to_user(user, &buf.unit_cfg, sizeof(rtdrv_unitCfg_t));
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_mirror(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** MRIIOR **/
        case RTDRV_MIRROR_GROUP_INIT:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_group_init(buf.mirror_cfg.unit,&buf.mirror_cfg.mirrorEntry);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_GROUP_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_group_get(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, &buf.mirror_cfg.mirrorEntry);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_RSPAN_IGR_MODE_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_rspanIgrMode_get(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, &buf.mirror_cfg.data);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_RSPAN_EGR_MODE_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_rspanEgrMode_get(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, &buf.mirror_cfg.data);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_RSPAN_TAG_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_rspanTag_get(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, &buf.mirror_cfg.rspan_tag);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_SFLOW_MIRROR_SAMPLE_RATE_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowMirrorSampleRate_get(buf.mirror_cfg.unit, buf.mirror_cfg.mirror_id, &buf.mirror_cfg.data);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_EGRQUEUE_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_egrQueue_get(buf.mirror_cfg.unit, &buf.mirror_cfg.enable, &buf.mirror_cfg.qid);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_SFLOW_PORT_IGR_SAMPLE_RATE_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowPortIgrSampleRate_get(buf.mirror_cfg.unit, buf.mirror_cfg.port, &buf.mirror_cfg.data);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_SFLOW_PORT_EGR_SAMPLE_RATE_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowPortEgrSampleRate_get(buf.mirror_cfg.unit, buf.mirror_cfg.port, &buf.mirror_cfg.data);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_SFLOW_SAMPLE_CTRL_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowSampleCtrl_get(buf.mirror_cfg.unit, &buf.mirror_cfg.sample_ctrl);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        case RTDRV_MIRROR_SFLOWSAMPLETARGET_GET:
            copy_from_user(&buf.mirror_cfg, user, sizeof(rtdrv_mirrorCfg_t));
            ret = rtk_mirror_sflowSampleTarget_get(buf.mirror_cfg.unit, &buf.mirror_cfg.target);
            copy_to_user(user, &buf.mirror_cfg, sizeof(rtdrv_mirrorCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_stack(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** STACK **/
#if (defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310))
        case RTDRV_STACK_PORT_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_port_get(buf.stack_cfg.unit, &buf.stack_cfg.stkPorts);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_UNIT_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_unit_get(buf.stack_cfg.unit, &buf.stack_cfg.unitIdx);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_MASTERUNIT_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_masterUnit_get(buf.stack_cfg.unit, &buf.stack_cfg.unitIdx);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_LOOPGUARD_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_loopGuard_get(buf.stack_cfg.unit, &buf.stack_cfg.enable);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_UNITPORTMAP_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_unitPortMap_get(buf.stack_cfg.unit, buf.stack_cfg.unitIdx, &buf.stack_cfg.stkPorts);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_NONUCASTBLOCKPORT_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_nonUcastBlockPort_get(buf.stack_cfg.unit, buf.stack_cfg.srcUnit, &buf.stack_cfg.stkPorts);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_RMTINTRTXENABLE_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrTxEnable_get(buf.stack_cfg.unit, &buf.stack_cfg.enable);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_RMTINTRTXTRIGGERENABLE_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrTxTriggerEnable_get(buf.stack_cfg.unit, &buf.stack_cfg.enable);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_RMTINTRRXSEQCMPMARGIN_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrRxSeqCmpMargin_get(buf.stack_cfg.unit, &buf.stack_cfg.margin);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_RMTINTRRXFORCEUPDATEENABLE_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrRxForceUpdateEnable_get(buf.stack_cfg.unit, &buf.stack_cfg.enable);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;

        case RTDRV_STACK_RMTINTRINFO_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_stackCfg_t));
            ret = rtk_stack_rmtIntrInfo_get(buf.stack_cfg.unit, &buf.stack_cfg.info);
            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_stackCfg_t));
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_flowctrl(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** FLOWCRTL **/
        case RTDRV_FLOWCTRL_PORT_PAUSEON_ACTION_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portPauseOnAction_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.pauseOn_action);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PORT_PAUSEON_ALLOWED_PAGENUM_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portPauseOnAllowedPageNum_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.data);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PAUSEON_ALLOWED_PKTLEN_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_pauseOnAllowedPktLen_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.data);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PAUSEON_ALLOWED_PKTNUM_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_pauseOnAllowedPktNum_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.data);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_SYS_PAUSE_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrSystemPauseThresh_get(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_JUMBO_SYS_PAUSE_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrJumboSystemPauseThresh_get(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_PAUSE_THR_GROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrPauseThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.thresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PORT_IGR_PORT_THR_GROUP_SEL_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portIgrPortThreshGroupSel_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.grp_idx);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_SYS_CONGEST_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrSystemCongestThresh_get(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_JUMBO_SYS_CONGEST_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrJumboSystemCongestThresh_get(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.thresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_CONGEST_THR_GROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrCongestThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.thresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_SYS_DROP_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrSystemDropThresh_get(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_DROP_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortDropThresh_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_DROP_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortDropEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_QUEUE_DROP_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortQueueDropEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_QUEUE_DROP_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrQueueDropThresh_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_CPU_QUEUE_DROP_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrCpuQueueDropThresh_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PORT_EGR_DROP_REFCONGEST_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portEgrDropRefCongestEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_PORT_DROP_THR_GROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrPortDropThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_QUEUE_DROP_THR_GROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrQueueDropThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PORT_EGR_DROP_THR_GROUP_SEL_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portEgrDropThreshGroupSel_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.grp_idx);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_EGR_QUEUE_DROP_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_egrQueueDropEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.queue,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PORT_EGR_QUEUE_DROP_FORCE_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portEgrQueueDropForceEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.queue,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_DROP_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueueDropEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, buf.flowctrl_cfg.queue,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_PAUSE_DROP_THR_GROUP_SEL_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueuePauseDropThreshGroupSel_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.grp_idx);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_DROP_THR_GROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueueDropThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, buf.flowctrl_cfg.queue,&buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_PAUSE_THR_GROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueuePauseThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, buf.flowctrl_cfg.queue, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_IGR_QUEUE_DROP_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_igrQueueDropThresh_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.queue,&buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_PORT_HOL_TRAFFIC_DROP_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portHolTrafficDropEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_HOL_TRAFFIC_TYPE_DROP_ENABLE_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_holTrafficTypeDropEnable_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.type,  &buf.flowctrl_cfg.enable);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_SPECIAL_CONGEST_THR_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_specialCongestThreshold_get(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_E2E_CASCADE_PORT_THRESH_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_e2eCascadePortThresh_get(buf.flowctrl_cfg.unit, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_E2E_REMOTE_PORTPAUSETHRESHGROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_e2eRemotePortPauseThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_E2E_REMOTE_PORTCONGESTTHRESHGROUP_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_e2eRemotePortCongestThreshGroup_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.grp_idx, &buf.flowctrl_cfg.dropThresh);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        case RTDRV_FLOWCTRL_E2E_PORT_REMOTE_PORT_THRESH_GROUP_SEL_GET:
            copy_from_user(&buf.flowctrl_cfg, user, sizeof(rtdrv_flowctrlCfg_t));
            ret = rtk_flowctrl_portE2eRemotePortThreshGroupSel_get(buf.flowctrl_cfg.unit, buf.flowctrl_cfg.port, &buf.flowctrl_cfg.grp_idx);
            copy_to_user(user, &buf.flowctrl_cfg, sizeof(rtdrv_flowctrlCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_rate(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** RATE **/
#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_RATE_IGR_INCLUDE_IFG_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBandwidthCtrlIncludeIfg_get(buf.rate_cfg.unit, &buf.rate_cfg.ifg_include);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_EGR_INCLUDE_IFG_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrBandwidthCtrlIncludeIfg_get(buf.rate_cfg.unit, &buf.rate_cfg.ifg_include);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_STORM_INCLUDE_IFG_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlIncludeIfg_get(buf.rate_cfg.unit, &buf.rate_cfg.ifg_include);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;
#endif
        case RTDRV_RATE_INCLUDE_IFG_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_includeIfg_get(buf.rate_cfg.unit, buf.rate_cfg.module, &buf.rate_cfg.ifg_include);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwCtrlEnable_get(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_RATE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwCtrlRate_get(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.rate);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_IGR_BWCTRL_LOW_THRESH_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBandwidthLowThresh_get(buf.rate_cfg.unit, &buf.rate_cfg.thresh);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

#if defined(CONFIG_SDK_DRIVER_RTK_LEGACY_API)
        case RTDRV_RATE_PORT_IGR_BWCTRL_HIGH_THRESH_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBandwidthHighThresh_get(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.thresh);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;
#endif

        case RTDRV_RATE_IGR_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBwCtrlBurstSize_get(buf.rate_cfg.unit, &buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_IGR_PORT_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrPortBwCtrlBurstSize_get(buf.rate_cfg.unit,&buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwCtrlBurstSize_get(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.igrBwCfg);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_EXCEED_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBandwidthCtrlExceed_get(buf.rate_cfg.unit,buf.rate_cfg.port,&buf.rate_cfg.isExceed);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_IGR_BWCTRL_BYPASS_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrBandwidthCtrlBypass_get(buf.rate_cfg.unit, buf.rate_cfg.igrBypassType, &buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_BWCTRL_FLOWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrBwFlowctrlEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_RATE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlRate_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.rate);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_IGR_QUEUE_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_igrQueueBwCtrlBurstSize_get(buf.rate_cfg.unit,&buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlBurstSize_get(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, &buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_BWCTRL_EXCEED_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueBwCtrlExceed_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.isExceed);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_IGR_QUEUE_FIXED_BWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portIgrQueueFixedBwCtrlEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_BWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrBwCtrlEnable_get(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_BWCTRL_RATE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrBwCtrlRate_get(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.rate);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrBwCtrlBurstSize_get(buf.rate_cfg.unit, buf.rate_cfg.port, &buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_CPU_PORT_EGR_BWCTRL_RATE_MODE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_cpuEgrBandwidthCtrlRateMode_get(buf.rate_cfg.unit, &buf.rate_cfg.rate_mode);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_EGR_PORT_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrPortBwCtrlBurstSize_get(buf.rate_cfg.unit,&buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_BWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueBwCtrlEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_BWCTRL_RATE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueBwCtrlRate_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.rate);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueBwCtrlBurstSize_get(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, &buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_RATE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlRate_get(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, &buf.rate_cfg.rate);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlBurstSize_get(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.queue, &buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_EGR_QUEUE_ASSURED_BWCTRL_MODE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portEgrQueueAssuredBwCtrlMode_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.assured_mode);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_EGR_QUEUE_FIXED_BWCTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrQueueFixedBandwidthEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.queue,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_EGR_QUEUE_BWCTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_egrQueueBwCtrlBurstSize_get(buf.rate_cfg.unit,&buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_RATE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlRate_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type,&buf.rate_cfg.rate);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_STORM_CTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlBurstSize_get(buf.rate_cfg.unit,buf.rate_cfg.storm_type,&buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlBurstSize_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type,&buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_EXCEED_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlExceed_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_type,&buf.rate_cfg.isExceed);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_STORM_CTRL_RATE_MODE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlRateMode_get(buf.rate_cfg.unit,&buf.rate_cfg.storm_rate_mode);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_RATE_MODE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlRateMode_get(buf.rate_cfg.unit,buf.rate_cfg.port,&buf.rate_cfg.storm_rate_mode);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_TYPE_SEL_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlTypeSel_get(buf.rate_cfg.unit, buf.rate_cfg.port, buf.rate_cfg.storm_type, &buf.rate_cfg.storm_sel);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_STORM_CTRL_BYPASS_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormControlBypass_get(buf.rate_cfg.unit, buf.rate_cfg.stormBypassType, &buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoEnable_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_proto_type,&buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_RATE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoRate_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_proto_type,&buf.rate_cfg.rate);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_BURST_SIZE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoBurstSize_get(buf.rate_cfg.unit,buf.rate_cfg.port,buf.rate_cfg.storm_proto_type,&buf.rate_cfg.burst_size);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_PORT_STORM_CTRL_PROTO_EXCEED_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_portStormCtrlProtoExceed_get(buf.rate_cfg.unit,buf.rate_cfg.port, buf.rate_cfg.storm_proto_type, &buf.rate_cfg.isExceed);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        case RTDRV_RATE_STORM_CTRL_PROTO_VLAN_CONSTRT_ENABLE_GET:
            copy_from_user(&buf.rate_cfg, user, sizeof(rtdrv_rateCfg_t));
            ret = rtk_rate_stormCtrlProtoVlanConstrtEnable_get(buf.rate_cfg.unit, buf.rate_cfg.storm_proto_type, &buf.rate_cfg.enable);
            copy_to_user(user, &buf.rate_cfg, sizeof(rtdrv_rateCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_switch(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SWITCH **/
        case RTDRV_SWITCH_CPU_MAX_PKTLEN_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_cpuMaxPktLen_get(buf.switch_cfg.unit, buf.switch_cfg.dir, &buf.switch_cfg.maxLen);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        case RTDRV_SWITCH_MAX_PKTLEN_LINK_SPEED_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_maxPktLenLinkSpeed_get(buf.switch_cfg.unit, buf.switch_cfg.speed, &buf.switch_cfg.maxLen);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        case RTDRV_SWITCH_PORT_MAX_PKTLEN_LINK_SPEED_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_portMaxPktLenLinkSpeed_get(buf.switch_cfg.unit, buf.switch_cfg.port, buf.switch_cfg.speed, &buf.switch_cfg.maxLen);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        case RTDRV_SWITCH_MAX_PKTLEN_TAGLENCNT_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_maxPktLenTagLenCntIncEnable_get(buf.switch_cfg.unit, &buf.switch_cfg.enable);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        case RTDRV_SWITCH_SNAP_MODE_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_snapMode_get(buf.switch_cfg.unit, &buf.switch_cfg.snapMode);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        case RTDRV_SWITCH_PORT_MAX_PKTLEN_TAGLENCNT_INCENABLE_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_portMaxPktLenTagLenCntIncEnable_get(buf.switch_cfg.unit, buf.switch_cfg.port, &buf.switch_cfg.enable);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        case RTDRV_SWITCH_DEVICE_INFO_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_deviceInfo_get(buf.switch_cfg.unit, &buf.switch_cfg.devInfo);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;
        case RTDRV_SWITCH_DEVICE_CAPABILITY_PRINT:
            ret = rtk_switch_deviceCapability_print(buf.switch_cfg.unit);
            break;

        case RTDRV_SWITCH_CHKSUMFAILACTION_GET :
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_chksumFailAction_get(buf.switch_cfgParam.unit, buf.switch_cfgParam.port,
                buf.switch_cfgParam.failType, &buf.switch_cfgParam.action);
            copy_to_user(user, &buf.switch_cfgParam, sizeof(rtdrv_switchCfgParam_t));
            break;

        case RTDRV_SWITCH_RECALCCRCENABLE_GET :
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_recalcCRCEnable_get(buf.switch_cfgParam.unit, buf.switch_cfgParam.port,
                &buf.switch_cfgParam.enable);
            copy_to_user(user, &buf.switch_cfgParam, sizeof(rtdrv_switchCfgParam_t));
            break;

        case RTDRV_SWITCH_MGMTMACADDR_GET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_mgmtMacAddr_get(buf.switch_cfgInfo.unit, &buf.switch_cfgInfo.mac);
            copy_to_user(user, &buf.switch_cfgInfo, sizeof(rtdrv_switchCfgInfo_t));
            break;

        case RTDRV_SWITCH_IPV4ADDR_GET :
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_IPv4Addr_get(buf.switch_cfgInfo.unit, &buf.switch_cfgInfo.ipv4Addr);
            copy_to_user(user, &buf.switch_cfgInfo, sizeof(rtdrv_switchCfgInfo_t));
            break;

        case RTDRV_SWITCH_PKT2CPUTYPEFORMAT_GET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_pkt2CpuTypeFormat_get(buf.switch_cfgInfo.unit,
                    buf.switch_cfgInfo.trap_type, &buf.switch_cfgInfo.format);
            copy_to_user(user, &buf.switch_cfgInfo, sizeof(rtdrv_switchCfgInfo_t));
            break;

        case RTDRV_SWITCH_PPPOE_IP_PARSE_ENABLE_GET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = rtk_switch_pppoeIpParseEnable_get(buf.switch_cfgInfo.unit, &buf.switch_cfgInfo.enable);
            copy_to_user(user, &buf.switch_cfgInfo, sizeof(rtdrv_switchCfgInfo_t));
            break;

        case RTDRV_SWITCH_WATCHDOG_ENABLE_GET:
            copy_from_user(&buf.switch_cfgInfo, user, sizeof(rtdrv_switchCfgInfo_t));
            ret = drv_watchdog_enable_get(buf.switch_cfgInfo.unit, &buf.switch_cfgInfo.enable);
            copy_to_user(user, &buf.switch_cfgInfo, sizeof(rtdrv_switchCfgInfo_t));
            break;

        case RTDRV_SWITCH_WATCHDOG_SCALE_GET:
            copy_from_user(&buf.watchdog_cfg, user, sizeof(rtdrv_watchdogCfgInfo_t));
            ret = drv_watchdog_scale_get(buf.switch_cfgInfo.unit, &buf.watchdog_cfg.scale);
            copy_to_user(user, &buf.watchdog_cfg, sizeof(rtdrv_watchdogCfgInfo_t));
            break;

        case RTDRV_SWITCH_WATCHDOG_THRESHOLD_GET:
            copy_from_user(&buf.watchdog_cfg, user, sizeof(rtdrv_watchdogCfgInfo_t));
            ret = drv_watchdog_threshold_get(buf.watchdog_cfg.unit, &buf.watchdog_cfg.threshold);
            copy_to_user(user, &buf.watchdog_cfg, sizeof(rtdrv_watchdogCfgInfo_t));
            break;

        case RTDRV_SWITCH_TC_COUNTERVALUE_GET:
            copy_from_user(&buf.tc_cfg, user, sizeof(rtdrv_tcCfgInfo_t));
            ret = drv_tc_counterValue_get(buf.tc_cfg.unit, buf.tc_cfg.id, &buf.tc_cfg.value);
            copy_to_user(user, &buf.tc_cfg, sizeof(rtdrv_tcCfgInfo_t));
            break;

        case RTDRV_SWITCH_CPU_PKT_TRUNCATE_EN_GET:
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_cpuPktTruncateEnable_get(buf.switch_cfgParam.unit, &buf.switch_cfgParam.enable);
            copy_to_user(user, &buf.switch_cfgParam, sizeof(rtdrv_switchCfgParam_t));
            break;

        case RTDRV_SWITCH_CPU_PKT_TRUNCATE_LEN_GET:
            copy_from_user(&buf.switch_cfgParam, user, sizeof(rtdrv_switchCfgParam_t));
            ret = rtk_switch_cpuPktTruncateLen_get(buf.switch_cfgParam.unit, &buf.switch_cfgParam.maxLen);
            copy_to_user(user, &buf.switch_cfgParam, sizeof(rtdrv_switchCfgParam_t));
            break;

        case RTDRV_SWITCH_FLEXTBLFMT_GET:
            copy_from_user(&buf.switch_cfgTable, user, sizeof(rtdrv_switchCfgTable_t));
            ret = rtk_switch_flexTblFmt_get(buf.switch_cfgTable.unit, &buf.switch_cfgTable.tbl_fmt);
            copy_to_user(user, &buf.switch_cfgTable, sizeof(rtdrv_switchCfgTable_t));
            break;

        case RTDRV_SWITCH_TAGPAUSEACTION_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_tagPauseAction_get(buf.switch_cfg.unit, &buf.switch_cfg.action);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        case RTDRV_SWITCH_HALF_CONSECUTIVE_RETRY_ENABLE_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_switchCfg_t));
            ret = rtk_switch_halfConsecutiveRetryEnable_get(buf.switch_cfg.unit, &buf.switch_cfg.enable);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_switchCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

#ifdef CONFIG_SDK_STACKING_PROTO
int32 (*rtdrv_rtstk_dc_show)(uint32) = NULL;
int32 (*rtdrv_rtstk_dcbox_show)(int,void *,void *) = NULL;
int32 (*rtdrv_rtstk_ta_show)(uint32) = NULL;
int32 (*rtdrv_rtstk_cfg_show)(uint32) = NULL;
int32 (*rtdrv_rtstk_port_show)(uint32) = NULL;

int32 rtdrv_callbackDcShow_reg(int32 (*func)(uint32))
{
    rtdrv_rtstk_dc_show = func;
    return RT_ERR_OK;
}
int32 rtdrv_callbackDcBoxShow_reg(int32 (*func)(int,void *,void *))
{
    rtdrv_rtstk_dcbox_show = func;
    return RT_ERR_OK;
}
int32 rtdrv_callbackTaShow_reg(int32 (*func)(uint32))
{
    rtdrv_rtstk_ta_show = func;
    return RT_ERR_OK;
}
int32 rtdrv_callbackCfgShow_reg(int32 (*func)(uint32))
{
    rtdrv_rtstk_cfg_show = func;
    return RT_ERR_OK;
}
int32 rtdrv_callbackPortShow_reg(int32 (*func)(uint32))
{
    rtdrv_rtstk_port_show = func;
    return RT_ERR_OK;
}

EXPORT_SYMBOL(rtdrv_callbackDcShow_reg);
EXPORT_SYMBOL(rtdrv_callbackDcBoxShow_reg);
EXPORT_SYMBOL(rtdrv_callbackTaShow_reg);
EXPORT_SYMBOL(rtdrv_callbackCfgShow_reg);
EXPORT_SYMBOL(rtdrv_callbackPortShow_reg);




#endif //CONFIG_SDK_STACKING_PROTO

int32 do_rtdrv_get_ctl_sys(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SYSTEM **/
        case RTDRV_SYS_HWP_DUMP_INFO:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            ret = hwp_info_show(buf.sys_cfg.unit);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;
        case RTDRV_SYS_HWP_DUMP_PARSED:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            ret = hwp_parsedInfo_show(buf.sys_cfg.unit);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;
        case RTDRV_SYS_HWP_DUMP_UNIT:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            ret = hwp_unit_show(buf.sys_cfg.unit);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;
#ifdef CONFIG_SDK_STACKING_PROTO
        case RTDRV_SYS_RTSTK_DC_SHOW:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            if (rtdrv_rtstk_dc_show)
                ret = rtdrv_rtstk_dc_show(buf.sys_cfg.unit);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;
        case RTDRV_SYS_RTSTK_DCBOX_SHOW:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            if (rtdrv_rtstk_dcbox_show)
                ret = rtdrv_rtstk_dcbox_show(2, NULL, NULL);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;
        case RTDRV_SYS_RTSTK_TA_SHOW:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            if (rtdrv_rtstk_ta_show)
                ret = rtdrv_rtstk_ta_show(buf.sys_cfg.unit);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;
        case RTDRV_SYS_RTSTK_CFG_SHOW:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            if (rtdrv_rtstk_cfg_show)
                ret = rtdrv_rtstk_cfg_show(buf.sys_cfg.unit);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;
        case RTDRV_SYS_RTSTK_PORT_SHOW:
            copy_from_user(&buf.sys_cfg, user, sizeof(rtdrv_sysCfg_t));
            if (rtdrv_rtstk_port_show)
                ret = rtdrv_rtstk_port_show(buf.sys_cfg.unit);
            copy_to_user(user, &buf.sys_cfg, sizeof(rtdrv_sysCfg_t));
            break;

#endif //CONFIG_SDK_STACKING_PROTO
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 do_rtdrv_get_ctl_nic(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_NIC) && defined(CONFIG_SDK_DRIVER_NIC_KERNEL_MODE)
    /** NIC **/
        case RTDRV_NIC_DEBUG_GET:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_dbg_get(buf.nic_cfg.unit, &buf.nic_cfg.flags);
            copy_to_user(user, &buf.nic_cfg, sizeof(rtdrv_nicCfg_t));
            break;

        case RTDRV_NIC_RX_STATUS_GET:
            copy_from_user(&buf.nic_cfg, user, sizeof(rtdrv_nicCfg_t));
            ret = drv_nic_rx_status_get(buf.nic_cfg.unit, &buf.nic_cfg.rx_status);
            copy_to_user(user, &buf.nic_cfg, sizeof(rtdrv_nicCfg_t));
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_sdk(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SDK **/
        case RTDRV_SDK_TEST_MODE_GET:
            copy_from_user(&buf.sdk_cfg, user, sizeof(rtdrv_sdkCfg_t));
            if (sdkTest_drv.mode_get != NULL)
                ret = sdkTest_drv.mode_get(&buf.sdk_cfg.mode);
            copy_to_user(user, &buf.sdk_cfg, sizeof(rtdrv_sdkCfg_t));
            break;
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_eee(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** EEE **/
        case RTDRV_EEE_PORT_ENABLE_GET:
            copy_from_user(&buf.eee_cfg, user, sizeof(rtdrv_eeeCfg_t));
            ret = rtk_eee_portEnable_get(buf.eee_cfg.unit, buf.eee_cfg.port, &buf.eee_cfg.enable);
            copy_to_user(user, &buf.eee_cfg, sizeof(rtdrv_eeeCfg_t));
            break;

        case RTDRV_EEE_PORT_STATE_GET:
            copy_from_user(&buf.eee_cfg, user, sizeof(rtdrv_eeeCfg_t));
            ret = rtk_eee_portState_get(buf.eee_cfg.unit, buf.eee_cfg.port, &buf.eee_cfg.enable);
            copy_to_user(user, &buf.eee_cfg, sizeof(rtdrv_eeeCfg_t));
            break;

        case RTDRV_EEEP_PORT_ENABLE_GET:
            copy_from_user(&buf.eee_cfg, user, sizeof(rtdrv_eeeCfg_t));
            ret = rtk_eeep_portEnable_get(buf.eee_cfg.unit, buf.eee_cfg.port, &buf.eee_cfg.enable);
            copy_to_user(user, &buf.eee_cfg, sizeof(rtdrv_eeeCfg_t));
            break;

        case RTDRV_EEE_PORT_POWER_STATE_GET:
            copy_from_user(&buf.eee_cfg, user, sizeof(rtdrv_eeeCfg_t));
            ret = rtk_eee_portPowerState_get(buf.eee_cfg.unit, buf.eee_cfg.port, buf.eee_cfg.direction, &buf.eee_cfg.state);
            copy_to_user(user, &buf.eee_cfg, sizeof(rtdrv_eeeCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_sec(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** SEC **/
        case RTDRV_SEC_PORT_ATTACK_PREVENT_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_portAttackPrevent_get(buf.sec_cfg.unit, buf.sec_cfg.port, buf.sec_cfg.attack_type
                                        , &buf.sec_cfg.action);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_PORT_ATTACK_PREVENT_ENABLE_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_portAttackPreventEnable_get(buf.sec_cfg.unit, buf.sec_cfg.port, &buf.sec_cfg.enable);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_ATTACK_PREVENT_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_attackPreventAction_get(buf.sec_cfg.unit, buf.sec_cfg.attack_type
                                        , &buf.sec_cfg.action);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_MIN_IPV6_FRAG_LEN_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_minIPv6FragLen_get(buf.sec_cfg.unit, &buf.sec_cfg.data);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_MAX_PING_LEN_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_maxPingLen_get(buf.sec_cfg.unit, &buf.sec_cfg.data);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_MIN_TCP_HDR_LEN_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_minTCPHdrLen_get(buf.sec_cfg.unit, &buf.sec_cfg.data);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_SMURF_NETMASK_LEN_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_smurfNetmaskLen_get(buf.sec_cfg.unit, &buf.sec_cfg.data);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_TRAPTARGET_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_trapTarget_get(buf.sec_cfg.unit, &buf.sec_cfg.target);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_IPMACBINDACTION_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_ipMacBindAction_get(buf.sec_cfg.unit, &buf.sec_cfg.lumisAct, &buf.sec_cfg.matchAct, &buf.sec_cfg.mismatchAct);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_PORTIPMACBINDENABLE_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_portIpMacBindEnable_get(buf.sec_cfg.unit, buf.sec_cfg.port, buf.sec_cfg.type, &buf.sec_cfg.enable);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_IPMACBINDENTRY_GETNEXT:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_ipMacBindEntry_getNext(buf.sec_cfg.unit, &buf.sec_cfg.base, &buf.sec_cfg.entry);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        case RTDRV_SEC_ATTACKPREVENTHIT_GET:
            copy_from_user(&buf.sec_cfg, user, sizeof(rtdrv_secCfg_t));
            ret = rtk_sec_attackPreventHit_get(buf.sec_cfg.unit, buf.sec_cfg.attack_type, &buf.sec_cfg.data);
            copy_to_user(user, &buf.sec_cfg, sizeof(rtdrv_secCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_led(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /** LED **/
        case RTDRV_LED_SYS_ENABLE_GET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_sysEnable_get(buf.led_cfg.unit, buf.led_cfg.type, &buf.led_cfg.enable);
            copy_to_user(user, &buf.led_cfg, sizeof(rtdrv_ledCfg_t));
            break;

        case RTDRV_LED_PORTLEDENTITYSWCTRLENABLE_GET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_portLedEntitySwCtrlEnable_get(buf.led_cfg.unit,
                    buf.led_cfg.port, buf.led_cfg.entity, &buf.led_cfg.enable);
            copy_to_user(user, &buf.led_cfg, sizeof(rtdrv_ledCfg_t));
            break;

        case RTDRV_LED_PORTLEDENTITYSWCTRLMODE_GET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_portLedEntitySwCtrlMode_get(buf.led_cfg.unit,
                    buf.led_cfg.port, buf.led_cfg.entity, buf.led_cfg.media,
                    &buf.led_cfg.mode);
            copy_to_user(user, &buf.led_cfg, sizeof(rtdrv_ledCfg_t));
            break;

        case RTDRV_LED_SYSMODE_GET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_sysMode_get(buf.led_cfg.unit, &buf.led_cfg.mode);
            copy_to_user(user, &buf.led_cfg, sizeof(rtdrv_ledCfg_t));
            break;

        case RTDRV_LED_BLINKTIME_GET:
            copy_from_user(&buf.led_cfg, user, sizeof(rtdrv_ledCfg_t));
            ret = rtk_led_blinkTime_get(buf.led_cfg.unit, &buf.led_cfg.time);
            copy_to_user(user, &buf.led_cfg, sizeof(rtdrv_ledCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_mpls(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
        /* MPLS */
#if (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9310))
        case RTDRV_MPLS_TTLINHERIT_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_ttlInherit_get(buf.mpls_cfg.unit, &buf.mpls_cfg.u.inherit);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_ENABLE_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_enable_get(buf.mpls_cfg.unit, &buf.mpls_cfg.enable);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_TRAPTARGET_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_trapTarget_get(buf.mpls_cfg.unit, &buf.mpls_cfg.u.target);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_EXCEPTIONCTRL_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_exceptionCtrl_get(buf.mpls_cfg.unit, buf.mpls_cfg.u.exceptionType, &buf.mpls_cfg.action);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_NEXTHOP_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_nextHop_get(buf.mpls_cfg.unit, buf.mpls_cfg.pathId, &buf.mpls_cfg.u.nexthop);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_ENCAP_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_encap_get(buf.mpls_cfg.unit, buf.mpls_cfg.entryId, &buf.mpls_cfg.u.encap);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_ENCAPID_FIND:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_encapId_find(buf.mpls_cfg.unit, &buf.mpls_cfg.u.encap, &buf.mpls_cfg.entryId);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_HASHALGO_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_hashAlgo_get(buf.mpls_cfg.unit, &buf.mpls_cfg.hashAlgo);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_DECAP_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_decap_get(buf.mpls_cfg.unit, buf.mpls_cfg.entryId, &buf.mpls_cfg.u.decap);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_DECAPID_FIND:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_decapId_find(buf.mpls_cfg.unit, &buf.mpls_cfg.u.decap, &buf.mpls_cfg.entryId);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;

        case RTDRV_MPLS_EGRTCMAP_GET:
            copy_from_user(&buf.mpls_cfg, user, sizeof(rtdrv_mplsCfg_t));
            ret = rtk_mpls_egrTcMap_get(buf.mpls_cfg.unit, &buf.mpls_cfg.src, &buf.mpls_cfg.tc);
            copy_to_user(user, &buf.mpls_cfg, sizeof(rtdrv_mplsCfg_t));
            break;
#endif  /* (defined(CONFIG_SDK_RTL8390) || defined(CONFIG_SDK_RTL9310)) */

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_sc(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL8295) || defined(CONFIG_SDK_RTL8295R) || defined(CONFIG_SDK_RTL8214QF)
        case RTDRV_DIAG_SC_REG_READ:
            copy_from_user(&buf.sc_regInfo, user, sizeof(rtdrv_scRegInfo_t));
            ret = hal_rtl8295_reg_read(buf.sc_regInfo.unit, buf.sc_regInfo.port, buf.sc_regInfo.addr, &buf.sc_regInfo.data);
            copy_to_user(user, &buf.sc_regInfo, sizeof(rtdrv_scRegInfo_t));
            break;
        case RTDRV_DIAG_SC_SDS_READ:
            copy_from_user(&buf.sc_sdsInfo, user, sizeof(rtdrv_scSdsInfo_t));
            ret = hal_rtl8295_sds_read(buf.sc_sdsInfo.unit, buf.sc_sdsInfo.port, buf.sc_sdsInfo.sds, buf.sc_sdsInfo.page, buf.sc_sdsInfo.reg, &buf.sc_sdsInfo.data);
            copy_to_user(user, &buf.sc_sdsInfo, sizeof(rtdrv_scSdsInfo_t));
            break;
#endif
#if defined(CONFIG_SDK_RTL8295R)
        case RTDRV_DIAG_SC_8295R_RXCALICONF_GET:
            {
                phy_8295_rxCaliConf_t   rxCaliConf;

                copy_from_user(&buf.rxCaliConf, user, sizeof(rtdrv_8295r_rxCaliConf_t));
                ret = phy_8295r_rxCaliConf_get(buf.rxCaliConf.unit, &rxCaliConf);
                buf.rxCaliConf.s1rxCaliDacLongCableOffset = rxCaliConf.s1rxCaliDacLongCableOffset;
                copy_to_user(user, &buf.rxCaliConf, sizeof(rtdrv_8295r_rxCaliConf_t));
            }
            break;
#endif/* defined(CONFIG_SDK_RTL8295R) */

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_rtl8231(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL8231)
    /** RTL8231 **/
        case RTDRV_RTL8231_I2C_READ:
            copy_from_user(&buf.rtl8231_cfg, user, sizeof(rtdrv_rtl8231Cfg_t));
            ret = drv_rtl8231_i2c_read(buf.rtl8231_cfg.unit, buf.rtl8231_cfg.phyId_or_slaveAddr, buf.rtl8231_cfg.reg_addr, &buf.rtl8231_cfg.data);
            copy_to_user(user, &buf.rtl8231_cfg, sizeof(rtdrv_rtl8231Cfg_t));
            break;

        case RTDRV_RTL8231_MDC_READ:
            copy_from_user(&buf.rtl8231_cfg, user, sizeof(rtdrv_rtl8231Cfg_t));
            ret = drv_rtl8231_mdc_read(buf.rtl8231_cfg.unit, buf.rtl8231_cfg.phyId_or_slaveAddr, buf.rtl8231_cfg.page, buf.rtl8231_cfg.reg_addr, &buf.rtl8231_cfg.data);
            copy_to_user(user, &buf.rtl8231_cfg, sizeof(rtdrv_rtl8231Cfg_t));
            break;

        case RTDRV_EXTGPIO_DEV_READY_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_devReady_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_DEV_ENABLE_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_devEnable_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_SYNC_ENABLE_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_syncEnable_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_SYNC_STATUS_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_syncStatus_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_DATABIT_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_dataBit_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.gpioId, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_REG_READ:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_reg_read(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.reg, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_DEV_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_dev_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, &buf.extGpio_cfg.extGpio_devConfData);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_PIN_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_pin_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.gpioId, &buf.extGpio_cfg.extGpio_confData);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_DIRECTION_GET:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_direction_get(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.gpioId, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;

        case RTDRV_EXTGPIO_I2C_READ:
            copy_from_user(&buf.extGpio_cfg, user, sizeof(rtdrv_extGpioCfg_t));
            ret = drv_extGpio_i2c_read(buf.extGpio_cfg.unit, buf.extGpio_cfg.dev, buf.extGpio_cfg.reg, &buf.extGpio_cfg.data);
            copy_to_user(user, &buf.extGpio_cfg, sizeof(rtdrv_extGpioCfg_t));
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_gpio(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#ifdef CONFIG_SDK_DRIVER_GPIO
        case RTDRV_GPIO_DATABIT_GET:
            copy_from_user(&buf.gpio_cfg, user, sizeof(rtdrv_gpioCfg_t));
            ret = drv_gpio_dataBit_get(buf.gpio_cfg.unit, buf.gpio_cfg.gpioId, &buf.gpio_cfg.data);
            copy_to_user(user, &buf.gpio_cfg, sizeof(rtdrv_gpioCfg_t));
            break;

        case RTDRV_GENCTRL_GPIO_DATABIT_GET:
            copy_from_user(&buf.gpio_cfg, user, sizeof(rtdrv_gpioCfg_t));
            ret = drv_generalCtrlGPIO_dataBit_get(buf.genCtrlGPIO_cfg.unit, buf.genCtrlGPIO_cfg.dev, buf.genCtrlGPIO_cfg.gpioId, &buf.genCtrlGPIO_cfg.data);
            if(ret != RT_ERR_OK)
                osal_printf("\nGet drv_generalCtrlGPIO_dataBit_get() ERROR = %d\n",ret);
            copy_to_user(user, &buf.gpio_cfg, sizeof(rtdrv_gpioCfg_t));
            break;
#endif /* CONFIG_SDK_DRIVER_GPIO */

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_i2c(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_I2C)
        case RTDRV_I2C_READ:
            copy_from_user(&buf.i2c_cfg, user, sizeof(rtdrv_i2c_devCfg_t));
            ret = drv_i2c_read(buf.i2c_cfg.unit_id, buf.i2c_cfg.device_id, buf.i2c_cfg.reg_idx, &buf.i2c_cfg.rwdata);
            copy_to_user(user, &buf.i2c_cfg, sizeof(rtdrv_i2c_devCfg_t));
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}
int32 do_rtdrv_get_ctl_spi(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_DRIVER_SPI)
        case RTDRV_SPI_READ:
            copy_from_user(&buf.spi_cfg, user, sizeof(rtdrv_spiCfg_t));
            ret = drv_spi_read(buf.spi_cfg.unit, buf.spi_cfg.addr, &buf.spi_cfg.data);
            copy_to_user(user, &buf.spi_cfg, sizeof(rtdrv_spiCfg_t));
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_diag(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /* DIAG */
        case RTDRV_DIAG_RTCTRESULT_GET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_diagCfg_t));
            ret = rtk_diag_portRtctResult_get(buf.diag_cfg.unit, buf.diag_cfg.port, &buf.diag_cfg.rtctResult);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_diagCfg_t));
            break;
        case RTDRV_DIAG_TABLE_WHOLE_READ:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_diagCfg_t));
            ret = rtk_diag_table_whole_read((uint32)buf.diag_cfg.unit, (uint32)buf.diag_cfg.target_index);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_diagCfg_t));
            break;
         case RTDRV_DIAG_REG_WHOLE_READ:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_diagCfg_t));
            ret = rtk_diag_reg_whole_read((uint32)buf.diag_cfg.unit);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_diagCfg_t));
            break;
         case RTDRV_DIAG_PERIPHERAL_REG_READ:
             copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_diagCfg_t));
             ret = rtk_diag_peripheral_register_dump((uint32)buf.diag_cfg.unit);
             copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_diagCfg_t));
             break;
         case RTDRV_DIAG_PHY_REG_READ:
             copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_diagCfg_t));
             ret = rtk_diag_phy_reg_whole_read((uint32)buf.diag_cfg.unit);
             copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_diagCfg_t));
             break;
        case RTDRV_DIAG_TABLE_ENTRY_READ:
             copy_from_user(&buf.table_entry_info, user, sizeof(rtdrv_diag_tableEntryRead_t));
             ret = rtk_diag_tableEntry_read(buf.table_entry_info.unit, buf.table_entry_info.table_index, buf.table_entry_info.ent_start_index, buf.table_entry_info.ent_end_index, buf.table_entry_info.detail);
             copy_to_user(user, &buf.table_entry_info, sizeof(rtdrv_diag_tableEntryRead_t));
             break;
         #if (defined(CONFIG_SDK_APP_DIAG_EXT) && defined (CONFIG_SDK_RTL9300))
         case RTDRV_DIAG_DEBUG_FIELD_GET:
             copy_from_user(&buf.diag_debug_cfg, user, sizeof(rtdrv_diag_debug_cfg_t));
             ret = rtk_diag_table_reg_field_get((uint32)buf.diag_debug_cfg.unit, &buf.diag_debug_cfg.diag_debug);
             copy_to_user(user, &buf.diag_debug_cfg, sizeof(rtdrv_diag_debug_cfg_t));
             break;
        #endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_openflow(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL9310)
        /* OpenFlow */
        case RTDRV_OF_CLASSIFIER_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_classifier_get(buf.openflow_cfg.unit, buf.openflow_cfg.classifyType, &buf.openflow_cfg.classifyData);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWMATCHFIELDSIZE_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowMatchFieldSize_get(buf.openflow_cfg.unit, buf.openflow_cfg.matchFieldType, &buf.openflow_cfg.field_size);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYSIZE_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntrySize_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, &buf.openflow_cfg.entry_size);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYVALIDATE_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryValidate_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.pValid);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYFIELD_CHECK:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryField_check(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.matchFieldType);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYSETFIELD_CHECK:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntrySetField_check(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.field_id, buf.openflow_cfg.setFieldType);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYFIELD_READ:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryField_read(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.matchFieldType, buf.openflow_cfg.fieldData, buf.openflow_cfg.fieldMask);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYOPERATION_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryOperation_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.pOperation);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYINSTRUCTION_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryInstruction_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.flowInsData);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWENTRYHITSTS_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowEntryHitSts_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.reset, &buf.openflow_cfg.isHit);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FTTEMPLATESELECTOR_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_ftTemplateSelector_get(buf.openflow_cfg.unit, buf.openflow_cfg.block_idx, &buf.openflow_cfg.template_idx);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWCNTMODE_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowCntMode_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.mode);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWCNT_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowCnt_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, buf.openflow_cfg.flowCntType, &buf.openflow_cfg.flowCnt);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWCNTTHRESH_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowCntThresh_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.threshold);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_TTLEXCPT_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_ttlExcpt_get(buf.openflow_cfg.unit, &buf.openflow_cfg.action);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_MAXLOOPBACK_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_maxLoopback_get(buf.openflow_cfg.unit, &buf.openflow_cfg.times);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L2FLOWTBLMATCHFIELD_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowTblMatchField_get(buf.openflow_cfg.unit, &buf.openflow_cfg.l2Field);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L2FLOWENTRYSETFIELD_CHECK:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowEntrySetField_check(buf.openflow_cfg.unit, buf.openflow_cfg.field_id, buf.openflow_cfg.setFieldType);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L2FLOWENTRY_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowEntry_get(buf.openflow_cfg.unit, &buf.openflow_cfg.l2Entry);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L2FLOWENTRYNEXTVALID_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowEntryNextValid_get(buf.openflow_cfg.unit, &buf.openflow_cfg.scan_idx, &buf.openflow_cfg.l2Entry);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L2FLOWTBLHASHALGO_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowTblHashAlgo_get(buf.openflow_cfg.unit, buf.openflow_cfg.block, &buf.openflow_cfg.algo);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L2FLOWENTRYHITSTS_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l2FlowEntryHitSts_get(buf.openflow_cfg.unit, &buf.openflow_cfg.l2Entry, buf.openflow_cfg.reset, &buf.openflow_cfg.isHit);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3FLOWTBLPRI_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3FlowTblPri_get(buf.openflow_cfg.unit, &buf.openflow_cfg.table);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3CAMFLOWTBLMATCHFIELD_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3CamFlowTblMatchField_get(buf.openflow_cfg.unit, &buf.openflow_cfg.l3CamField);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3HASHFLOWTBLMATCHFIELD_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowTblMatchField_get(buf.openflow_cfg.unit, &buf.openflow_cfg.l3HashField);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3HASHFLOWTBLHASHALGO_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowTblHashAlgo_get(buf.openflow_cfg.unit, buf.openflow_cfg.block, &buf.openflow_cfg.algo);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3FLOWENTRYSETFIELD_CHECK:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3FlowEntrySetField_check(buf.openflow_cfg.unit, buf.openflow_cfg.field_id, buf.openflow_cfg.setFieldType);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3CAMFLOWENTRY_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3CamFlowEntry_get(buf.openflow_cfg.unit, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.l3CamEntry);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3CAMFLOWENTRYHITSTS_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3CamflowEntryHitSts_get(buf.openflow_cfg.unit, buf.openflow_cfg.entry_idx, buf.openflow_cfg.reset, &buf.openflow_cfg.isHit);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3HASHFLOWENTRY_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowEntry_get(buf.openflow_cfg.unit, &buf.openflow_cfg.l3HashEntry);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3HASHFLOWENTRYNEXTVALID_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashFlowEntryNextValid_get(buf.openflow_cfg.unit, &buf.openflow_cfg.scan_idx, &buf.openflow_cfg.l3HashEntry);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_L3HASHFLOWENTRYHITSTS_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_l3HashflowEntryHitSts_get(buf.openflow_cfg.unit, &buf.openflow_cfg.l3HashEntry, buf.openflow_cfg.reset, &buf.openflow_cfg.isHit);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_GROUPENTRY_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_groupEntry_get(buf.openflow_cfg.unit, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.grpEntry);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_GROUPTBLHASHPARA_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_groupTblHashPara_get(buf.openflow_cfg.unit, &buf.openflow_cfg.para);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_ACTIONBUCKET_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_actionBucket_get(buf.openflow_cfg.unit, buf.openflow_cfg.entry_idx, &buf.openflow_cfg.actionBktEntry);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_TRAPTARGET_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_trapTarget_get(buf.openflow_cfg.unit, &buf.openflow_cfg.target);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_TBLMISSACTION_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_tblMissAction_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, &buf.openflow_cfg.tblMissAct);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;

        case RTDRV_OF_FLOWTBLCNT_GET:
            copy_from_user(&buf.openflow_cfg, user, sizeof(rtdrv_openflowCfg_t));
            ret = rtk_of_flowTblCnt_get(buf.openflow_cfg.unit, buf.openflow_cfg.phase, buf.openflow_cfg.flowtblCntType, &buf.openflow_cfg.tblCnt);
            copy_to_user(user, &buf.openflow_cfg, sizeof(rtdrv_openflowCfg_t));
            break;
#endif
        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_capwap(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
#if defined(CONFIG_SDK_RTL9310)
    /* CAPWAP */
        case RTDRV_CAPWAP_ENABLE_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_enable_get(buf.capwap_cfg.unit, &buf.capwap_cfg.enable);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_UDPPORT_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_udpPort_get(buf.capwap_cfg.unit, &buf.capwap_cfg.ctrl_port, &buf.capwap_cfg.data_port);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_FWDACTION_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_fwdAction_get(buf.capwap_cfg.unit, buf.capwap_cfg.type, &buf.capwap_cfg.action);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_TRAPTARGET_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_trapTarget_get(buf.capwap_cfg.unit, &buf.capwap_cfg.target);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_BSSIDTBLHASHALGO_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_bssidTblHashAlgo_get(buf.capwap_cfg.unit, &buf.capwap_cfg.algo);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_QOSPROFILE_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_qosProfile_get(buf.capwap_cfg.unit, buf.capwap_cfg.index, &buf.capwap_cfg.profile);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_TIDREMARKING_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_tidRemarking_get(buf.capwap_cfg.unit, buf.capwap_cfg.dp, buf.capwap_cfg.int_pri, &buf.capwap_cfg.tid);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_BSSIDENTRY_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_bssidEntry_get(buf.capwap_cfg.unit, &buf.capwap_cfg.bssid_entry);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_BSSIDENTRYNEXTVALID_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_bssidEntryNextValid_get(buf.capwap_cfg.unit, &buf.capwap_cfg.scan_idx, &buf.capwap_cfg.bssid_entry);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_UCAST_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_ucast_get(buf.capwap_cfg.unit, &buf.capwap_cfg.ucast_entry);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_UCASTNEXTVALID_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_ucastNextValid_get(buf.capwap_cfg.unit, &buf.capwap_cfg.scan_idx, &buf.capwap_cfg.ucast_entry);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_MCASTBSSID_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_mcastBssid_get(buf.capwap_cfg.unit, &buf.capwap_cfg.mcastBssid_entry, buf.capwap_cfg.arraySize, &buf.capwap_cfg.bssidArray, &buf.capwap_cfg.bssidCnt);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;

        case RTDRV_CAPWAP_BSSIDLISTNEXTVALID_GET:
            copy_from_user(&buf.capwap_cfg, user, sizeof(rtdrv_capwapCfg_t));
            ret = rtk_capwap_bssidListNextValid_get(buf.capwap_cfg.unit, &buf.capwap_cfg.scan_idx, &buf.capwap_cfg.bssidList_entry);
            copy_to_user(user, &buf.capwap_cfg, sizeof(rtdrv_capwapCfg_t));
            break;
#endif

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_serdes(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /* SerDes */
        case RTDRV_SERDES_REG_GET:
            copy_from_user(&buf.serdes_reg_cfg, user, sizeof(rtdrv_serdes_reg_t));
            ret = hal_serdes_reg_get(buf.serdes_reg_cfg.unit, buf.serdes_reg_cfg.sdsId, buf.serdes_reg_cfg.page, buf.serdes_reg_cfg.reg, &buf.serdes_reg_cfg.data);
            copy_to_user(user, &buf.serdes_reg_cfg, sizeof(rtdrv_serdes_reg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}


int32 do_rtdrv_get_ctl_hwp(int cmd, void *user)
{
    int32 ret = RT_ERR_FAILED;

    switch(cmd)
    {
    /* HWP */
        case RTDRV_HWP_UNIT_GET_NEXT:
            copy_from_user(&buf.hwp_unit_cfg, user, sizeof(rtdrv_hwp_unitCfg_t));
            ret = hwp_unit_get_next(buf.hwp_unit_cfg.unit, &buf.hwp_unit_cfg.nextUnit);
            copy_to_user(user, &buf.hwp_unit_cfg, sizeof(rtdrv_hwp_unitCfg_t));
            break;

        default:
            ret = RT_ERR_FAILED;
            break;
    }
    return ret;
}

int32 rtdrv_modules_init(void)
{
    int i;

    for(i=0;i<RTDRV_MODULE_MAX_NUM;i++)
    {
        rtdrv_module_db_set[i].valid = FALSE;
        rtdrv_module_db_get[i].valid = FALSE;
    }

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_INIT_OFFSET, do_rtdrv_set_ctl_init);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_INIT_OFFSET, NULL);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_L2_OFFSET, do_rtdrv_set_ctl_l2);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_L2_OFFSET, do_rtdrv_get_ctl_l2);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_L2NTFY_OFFSET,do_rtdrv_set_ctl_l2ntfy);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_L2NTFY_OFFSET,do_rtdrv_get_ctl_l2ntfy);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_L3_OFFSET, do_rtdrv_set_ctl_l3);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_L3_OFFSET, do_rtdrv_get_ctl_l3);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_MCAST_OFFSET,do_rtdrv_set_ctl_mcast);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_MCAST_OFFSET,do_rtdrv_get_ctl_mcast);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_IPMC_OFFSET,do_rtdrv_set_ctl_ipmc);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_IPMC_OFFSET,do_rtdrv_get_ctl_ipmc);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_TUNNEL_OFFSET,do_rtdrv_set_ctl_tunnel);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_TUNNEL_OFFSET,do_rtdrv_get_ctl_tunnel);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_PORT_OFFSET,do_rtdrv_set_ctl_port);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_PORT_OFFSET,do_rtdrv_get_ctl_port);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_VLAN_OFFSET,do_rtdrv_set_ctl_vlan);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_VLAN_OFFSET,do_rtdrv_get_ctl_vlan);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_STP_OFFSET,do_rtdrv_set_ctl_stp);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_STP_OFFSET,do_rtdrv_get_ctl_stp);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_REG_OFFSET,do_rtdrv_set_ctl_reg);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_REG_OFFSET,do_rtdrv_get_ctl_reg);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_COUNTER_OFFSET,do_rtdrv_set_ctl_counter);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_COUNTER_OFFSET,do_rtdrv_get_ctl_counter);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_TIME_OFFSET,do_rtdrv_set_ctl_time);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_TIME_OFFSET,do_rtdrv_get_ctl_time);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_TRAP_OFFSET,do_rtdrv_set_ctl_trap);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_TRAP_OFFSET,do_rtdrv_get_ctl_trap);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_QOS_OFFSET,do_rtdrv_set_ctl_qos);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_QOS_OFFSET,do_rtdrv_get_ctl_qos);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_TRUNK_OFFSET,do_rtdrv_set_ctl_trunk);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_TRUNK_OFFSET,do_rtdrv_get_ctl_trunk);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_MIRROR_OFFSET,do_rtdrv_set_ctl_mirror);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_MIRROR_OFFSET,do_rtdrv_get_ctl_mirror);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_STACK_OFFSET,do_rtdrv_set_ctl_stack);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_STACK_OFFSET,do_rtdrv_get_ctl_stack);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_FLOWCTRL_OFFSET,do_rtdrv_set_ctl_flowctrl);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_FLOWCTRL_OFFSET,do_rtdrv_get_ctl_flowctrl);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_RATE_OFFSET,do_rtdrv_set_ctl_rate);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_RATE_OFFSET,do_rtdrv_get_ctl_rate);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_SWITCH_OFFSET,do_rtdrv_set_ctl_switch);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_SWITCH_OFFSET,do_rtdrv_get_ctl_switch);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_SYS_OFFSET,do_rtdrv_set_ctl_sys);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_SYS_OFFSET,do_rtdrv_get_ctl_sys);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_NIC_OFFSET,do_rtdrv_set_ctl_nic);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_NIC_OFFSET,do_rtdrv_get_ctl_nic);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_OAM_OFFSET,do_rtdrv_set_ctl_oam);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_OAM_OFFSET,do_rtdrv_get_ctl_oam);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_EEE_OFFSET,do_rtdrv_set_ctl_eee);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_EEE_OFFSET,do_rtdrv_get_ctl_eee);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_SEC_OFFSET,do_rtdrv_set_ctl_sec);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_SEC_OFFSET,do_rtdrv_get_ctl_sec);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_ACL_OFFSET,do_rtdrv_set_ctl_acl);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_ACL_OFFSET,do_rtdrv_get_ctl_acl);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_PIE_OFFSET,do_rtdrv_set_ctl_pie);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_PIE_OFFSET,do_rtdrv_get_ctl_pie);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_MPLS_OFFSET,do_rtdrv_set_ctl_mpls);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_MPLS_OFFSET,do_rtdrv_get_ctl_mpls);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_DIAG_OFFSET,do_rtdrv_set_ctl_diag);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_DIAG_OFFSET,do_rtdrv_get_ctl_diag);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_DEBUG_OFFSET,do_rtdrv_set_ctl_debug);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_DEBUG_OFFSET,do_rtdrv_get_ctl_debug);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_UART_OFFSET,do_rtdrv_set_ctl_uart);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_UART_OFFSET,do_rtdrv_get_ctl_uart);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_LED_OFFSET,do_rtdrv_set_ctl_led);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_LED_OFFSET,do_rtdrv_get_ctl_led);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_RTL8231_OFFSET,do_rtdrv_set_ctl_rtl8231);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_RTL8231_OFFSET,do_rtdrv_get_ctl_rtl8231);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_EXTGPIO_OFFSET,do_rtdrv_set_ctl_extgpio);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_EXTGPIO_OFFSET,NULL);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_GPIO_OFFSET,do_rtdrv_set_ctl_gpio);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_GPIO_OFFSET,do_rtdrv_get_ctl_gpio);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_SPI_OFFSET,do_rtdrv_set_ctl_spi);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_SPI_OFFSET,do_rtdrv_get_ctl_spi);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_VXLAN_OFFSET,do_rtdrv_set_ctl_vxlan);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_VXLAN_OFFSET,do_rtdrv_get_ctl_vxlan);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_OPENFLOW_OFFSET,do_rtdrv_set_ctl_openflow);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_OPENFLOW_OFFSET,do_rtdrv_get_ctl_openflow);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_CAPWAP_OFFSET,do_rtdrv_set_ctl_capwap);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_CAPWAP_OFFSET,do_rtdrv_get_ctl_capwap);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_I2C_OFFSET,do_rtdrv_set_ctl_i2c);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_I2C_OFFSET,do_rtdrv_get_ctl_i2c);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_HWP_OFFSET,NULL);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_HWP_OFFSET,do_rtdrv_get_ctl_hwp);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_SDK_OFFSET,do_rtdrv_set_ctl_sdk);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_SDK_OFFSET,do_rtdrv_get_ctl_sdk);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_BPE_OFFSET,do_rtdrv_set_ctl_bpe);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_BPE_OFFSET,do_rtdrv_get_ctl_bpe);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_SERDES_OFFSET,do_rtdrv_set_ctl_serdes);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_SERDES_OFFSET,do_rtdrv_get_ctl_serdes);

    rtdrv_cmdFunc_register(RTDRV_SET, RTDRV_SC_OFFSET,do_rtdrv_set_ctl_sc);
    rtdrv_cmdFunc_register(RTDRV_GET, RTDRV_SC_OFFSET,do_rtdrv_get_ctl_sc);

    return RT_ERR_OK;
}


/* Function Name:
 *      rtdrv_init
 * Description:
 *      Init driver and register netfilter socket option
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
static int32 __init rtdrv_init(void)
{
    int32 ret=RT_ERR_FAILED;

    RT_INIT_MSG("RTDRV Driver Module Initialize\n");

    /* register netfilter socket option */
    if (nf_register_sockopt(&rtdrv_sockopts))
    {
        RT_INIT_ERR(ret, (MOD_INIT), "nf_register_sockopt failed.\n");
        return ret;
    }



#if defined(CONFIG_SDK_APP_DIAG_EXT)

    RT_INIT_MSG("RTDRV EXT Driver Module Initialize\n");
    /* register netfilter socket option */
    if (nf_register_sockopt(&rtdrv_ext_sockopts))
    {
        RT_INIT_ERR(ret, (MOD_INIT), "nf_register_sockopt failed.\n");
        return ret;
    }

#endif /* CONFIG_SDK_APP_DIAG_EXT */

    rtdrv_modules_init();

    return RT_ERR_OK;
}

/* Function Name:
 *      rtdrv_exit
 * Description:
 *      Exit driver and unregister netfilter socket option
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void __exit rtdrv_exit(void)
{
    nf_unregister_sockopt(&rtdrv_sockopts);

    RT_INIT_MSG("Exit RTDRV Driver Module\n");

#if defined(CONFIG_SDK_APP_DIAG_EXT)
    nf_unregister_sockopt(&rtdrv_ext_sockopts);
    RT_INIT_MSG("Exit RTDRV EXT Driver Module\n");
#endif /* CONFIG_SDK_APP_DIAG_EXT */
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
struct nf_sockopt_ops rtdrv_sockopts = {
    { NULL, NULL }, PF_INET,
    RTDRV_BASE_CTL, RTDRV_SET_MAX+1, do_rtdrv_set_ctl, NULL,
    RTDRV_BASE_CTL, RTDRV_GET_MAX+1, do_rtdrv_get_ctl, NULL
};
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
struct nf_sockopt_ops rtdrv_sockopts = {
    { NULL, NULL }, PF_INET,
    RTDRV_BASE_CTL, RTDRV_SET_MAX+1, do_rtdrv_set_ctl,
    RTDRV_BASE_CTL, RTDRV_GET_MAX+1, do_rtdrv_get_ctl
};
#endif


module_init(rtdrv_init);
module_exit(rtdrv_exit);

MODULE_DESCRIPTION ("Switch SDK User/Kernel Driver Module");
MODULE_LICENSE("GPL");

