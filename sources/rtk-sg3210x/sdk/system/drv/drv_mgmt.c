/*
 * Copyright (C) 2009-2021 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : Use to Management bsp driver
 *
 * Feature :
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <common/rt_chip.h>

#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>

#include <drv/drv_mgmt.h>
#include <osal/memory.h>
#include <osal/sem.h>
#include <osal/lib.h>

/*
 * Symbol Definition
 */
#define DRV_MGMT_RET_CHECK(_ret, _unit, _str) \
    if (_ret != RT_ERR_OK && _ret != RT_ERR_CHIP_NOT_SUPPORTED)  RT_LOG(LOG_WARNING, MOD_BSP, "unit %d: failed to init "_str", ret=0x%X\n", _unit, _ret);
#define DRV_MGMT_EXIT_CHECK(_ret, _unit, _str) \
    if (_ret != RT_ERR_OK && _ret != RT_ERR_CHIP_NOT_SUPPORTED)  RT_LOG(LOG_WARNING, MOD_BSP, "unit %d: failed to exit "_str", ret=0x%X\n", _unit, _ret);

/*
 * Data Declaration
 */
static uint32           drv_mgmt_init_state = INIT_NOT_COMPLETED;
drv_mgmt_info_t         *pDrvMgmt_node[RTK_MAX_NUM_OF_UNIT];
drv_mapper_t            *mgmt_drv_mapper[RTK_MAX_NUM_OF_UNIT] = { NULL };

#ifdef CONFIG_SDK_OOB_PHY_8213B
void drv_phy_8213b_Mapper_init(drv_mapper_t *pMapper, uint32 unit, rtk_port_t port);
#endif

static drv_mgmt_oobPhyCtrl_t drv_mgmt_oobPhyDrv_list[] =
{
    #ifdef RTK_USERMODE_LKM
    #else
    #ifdef CONFIG_SDK_OOB_PHY_8213B
        { phy_8213b_Identify,     PHY_MODEL_ID_RTL8213B,     PHY_REV_NO_C, RTK_PHYTYPE_RTL8213B,  drv_phy_8213b_Mapper_init, &oob_phy_rtl8213b_info},
        { phy_8363sc_vb_Identify, PHY_MODEL_ID_RTL8363SC_VB, PHY_REV_NO_C, RTK_PHYTYPE_RTL8363SC_VB, drv_phy_8213b_Mapper_init, &oob_phy_rtl8213b_info},
    #endif
    #endif
};



/*
 * Function Declaration
 */
int drv_common_init(uint32 unit, drv_mgmt_bsp_type_t bsp_type)
{
    int32 ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, MOD_INIT, "[%s]u:%u bsp_type:%d\n", __FUNCTION__, unit, bsp_type);

#ifdef RTK_USERMODE_LKM
    /* Init drv for User-Mode kernel space */
    switch (bsp_type)
    {
        case DRV_TYPE_SOC:
            ret = drv_gpio_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "GPIO");

            ret = drv_tc_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "TC");

    #ifndef __BOOTLOADER__
            ret = drv_intr_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "INTR");
    #endif
            break;

        case DRV_TYPE_SWCORE:
          #ifndef __BOOTLOADER__
          #if defined(CONFIG_SDK_RTL9311B)
            ret = drv_intr_swcore_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "INTR");
          #endif
          #endif
            ret = drv_i2c_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "I2C");

            ret = drv_rtl8231_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "RTL8231");
            break;
        default:
            return RT_ERR_INPUT;
    }
#else
    switch (bsp_type)
    {
        case DRV_TYPE_SOC:
            ret = drv_gpio_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "GPIO");

            ret = drv_tc_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "TC");
    #ifndef __BOOTLOADER__
            ret = drv_watchdog_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "WATCHDOG");

            ret = drv_uart_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "UART1");

            ret = drv_intr_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "INTR");
    #endif
            break;

        case DRV_TYPE_SWCORE:
          #ifndef __BOOTLOADER__
          #if defined(CONFIG_SDK_RTL9311B)
            ret = drv_intr_swcore_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "INTR");
          #endif
          #endif

            ret = drv_i2c_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "I2C");

            ret = drv_rtl8231_init(unit);
            DRV_MGMT_RET_CHECK(ret, unit, "RTL8231");
            break;
        default:
            return RT_ERR_INPUT;
    }
#endif
    return ret;
}

int drv_common_exit(uint32 unit, drv_mgmt_bsp_type_t bsp_type)
{
    int32 ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, MOD_INIT, "[%s]u:%u bsp_type:%d\n", __FUNCTION__, unit, bsp_type);

#ifdef RTK_USERMODE_LKM
    /* Init drv for User-Mode kernel space */
    switch (bsp_type)
    {
        case DRV_TYPE_SOC:
    #ifndef __BOOTLOADER__
            ret = drv_intr_exit(unit);
            DRV_MGMT_EXIT_CHECK(ret, unit, "INTR");
    #endif
            break;

        case DRV_TYPE_SWCORE:
    #ifndef __BOOTLOADER__
            ret = drv_intr_swcore_exit(unit);
            DRV_MGMT_EXIT_CHECK(ret, unit, "INTR");
    #endif
            break;
        default:
            return RT_ERR_INPUT;
    }
#else
    switch (bsp_type)
    {
        case DRV_TYPE_SOC:
    #ifndef __BOOTLOADER__
            ret = drv_intr_exit(unit);
            DRV_MGMT_EXIT_CHECK(ret, unit, "INTR");
    #endif
            break;

        case DRV_TYPE_SWCORE:
    #ifndef __BOOTLOADER__
            ret = drv_intr_swcore_exit(unit);
            DRV_MGMT_EXIT_CHECK(ret, unit, "INTR");
    #endif
            break;
        default:
            return RT_ERR_INPUT;
    }
#endif
    return ret;
}

int32 drv_common_unavail(void)
{
    return RT_ERR_CHIP_NOT_SUPPORTED;
}

/* Per-chip Mapper Declaration */
#if defined(CONFIG_SDK_RTL8380)
void drv_maple_intrMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->intr.swcoreIsrSts_get               , rtl8380_intr_swcoreIsrSts_get);
    F_00000U(pMapper->intr.swcoreImrEnable_set            , rtl8380_intr_swcoreImrEnable_set);
    F_00000U(pMapper->intr.swcoreSts_get                  , rtl8380_intr_swcoreSts_get);
    F_00000U(pMapper->intr.swcore_exit                    , rtl8380_intr_swcore_exit);
}

void drv_maple_gpioMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->gpio.init                           , rtl8380_gpio_init);
    F_00000U(pMapper->gpio.direction_set                  , rtl8380_gpio_direction_set);
    F_00000U(pMapper->gpio.control_set                    , rtl8380_gpio_control_set);
    F_00000U(pMapper->gpio.intrMode_set                   , rtl8380_gpio_intrMode_set);
    F_00000U(pMapper->gpio.dataBit_init                   , rtl8380_gpio_dataBit_init);
    F_00000U(pMapper->gpio.dataBit_get                    , rtl8380_gpio_dataBit_get);
    F_00000U(pMapper->gpio.dataBit_set                    , rtl8380_gpio_dataBit_set);
    F_00000U(pMapper->gpio.intrStatus_get                 , rtl8380_gpio_intrStatus_get);
    F_00000U(pMapper->gpio.intrStatus_clear               , rtl8380_gpio_intrStatus_clear);
    F_00000U(pMapper->gpio.portRange_get                  , rtl8380_gpio_portRange_get);
}

void drv_maple_tcMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->tc.init                             , tc_init);
    F_00000U(pMapper->tc.tc_enable_set                    , tc_enable_set);
    F_00000U(pMapper->tc.tcMode_set                       , tc_mode_set);
    F_00000U(pMapper->tc.tcDivFactor_set                  , tc_divFactor_set);
    F_00000U(pMapper->tc.tcDataInitValue_set              , tc_dataInitValue_set);
    F_00000U(pMapper->tc.tcIntEnable_set                  , tc_intEnable_set);
    F_00000U(pMapper->tc.tcIntState_get                   , tc_intState_get);
    F_00000U(pMapper->tc.tcIntState_clear                 , tc_intState_clear);
    F_00000U(pMapper->tc.tcCounterValue_get               , tc_counterValue_get);
}

void drv_maple_watchdogMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->watchdog.scale_set                  , watchdog_scale_set);
    F_000000(pMapper->watchdog.scale_get                  , watchdog_scale_get);
    F_000000(pMapper->watchdog.enable_set                 , watchdog_enable_set);
    F_000000(pMapper->watchdog.enable_get                 , watchdog_enable_get);
    F_000000(pMapper->watchdog.kick                       , watchdog_kick);
    F_000000(pMapper->watchdog.init                       , watchdog_init);
    F_000000(pMapper->watchdog.threshold_set              , watchdog_threshold_set);
    F_000000(pMapper->watchdog.threshold_get              , watchdog_threshold_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_maple_i2cMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->i2c.init                            , r8380_i2c_init);
    F_00000U(pMapper->i2c.i2c_dev_init                    , r8380_i2c_dev_init);
    F_00000U(pMapper->i2c.i2c_write                       , r8380_i2c_write);
    F_00000U(pMapper->i2c.i2c_read                        , r8380_i2c_read);
    F_00000U(pMapper->i2c.i2c_type_set                    , NULL);
    F_00000U(pMapper->i2c.i2c_type_get                    , NULL);
}

void drv_maple_rtl8231Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->rtl8231.mdc_read                    , r8380_rtl8231_mdc_read);
    F_00000U(pMapper->rtl8231.mdc_write                   , r8380_rtl8231_mdc_write);
    F_00000U(pMapper->rtl8231.init                        , r8380_rtl8231_init);
    F_00000U(pMapper->rtl8231.mdcSem_register             , r8380_rtl8231_mdcSem_register);
    F_00000U(pMapper->rtl8231.mdcSem_unregister           , r8380_rtl8231_mdcSem_unregister);
}

void drv_maple_uart1Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->uart1.init                          , uart_init);
    F_000000(pMapper->uart1.interface_set                 , uart_interface_set);
    F_000000(pMapper->uart1.tstc                          , uart_tstc);
    F_000000(pMapper->uart1.poll_getc                     , uart_getc);
    F_000000(pMapper->uart1.poll_putc                     , uart_putc);
    F_000000(pMapper->uart1.baudrate_get                  , uart_baudrate_get);
    F_000000(pMapper->uart1.baudrate_set                  , uart_baudrate_set);
    F_000000(pMapper->uart1.starttx                       , serial_starttx);
    F_000000(pMapper->uart1.clearfifo                     , serial_clearfifo);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

#if defined(CONFIG_SDK_DRIVER_NIC)
void drv_maple_nicMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->nic.init                            , r8380_init);
    F_00000U(pMapper->nic.pkt_tx                          , r8380_pkt_tx);
    F_00000U(pMapper->nic.rx_start                        , r8380_rx_start);
    F_00000U(pMapper->nic.rx_stop                         , r8380_rx_stop);
    F_00000U(pMapper->nic.rx_register                     , r8380_rx_register);
    F_00000U(pMapper->nic.rx_unregister                   , r8380_rx_unregister);
    F_00000U(pMapper->nic.pkt_alloc                       , r8380_pkt_alloc);
    F_00000U(pMapper->nic.pkt_free                        , r8380_pkt_free);
    F_00000U(pMapper->nic.ringInfo_get                    , r8380_ringInfo_get);
    F_00000U(pMapper->nic.cpuPortTxRxEnable_set           , r8380_cpuPortTxRxEnable_set);
    F_00000U(pMapper->nic.intrMask_get                    , r8380_intrMask_get);
    F_00000U(pMapper->nic.intrMask_set                    , r8380_intrMask_set);
    F_00000U(pMapper->nic.intrSts_get                     , r8380_intrSts_get);
    F_00000U(pMapper->nic.intrSts_set                     , r8380_intrSts_set);
    F_00000U(pMapper->nic.swNicRst_get                    , r8380_swNicRst_get);
    F_00000U(pMapper->nic.swNicRst_set                    , r8380_swNicRst_set);
    F_00000U(pMapper->nic.swQueRst_get                    , r8380_swQueRst_get);
    F_00000U(pMapper->nic.swQueRst_set                    , r8380_swQueRst_set);
    F_00000U(pMapper->nic.cpuL2FloodMask_add              , r8380_cpuL2FloodMask_add);
    F_00000U(pMapper->nic.cpuL2FloodMask_remove           , r8380_cpuL2FloodMask_remove);
    F_00000U(pMapper->nic.cpuForceLinkupEnable_set        , r8380_cpuForceLinkupEnable_set);
    F_00000U(pMapper->nic.holRingSize_set                 , r8380_holRingSize_set);
    F_00000U(pMapper->nic.holRingCnt_get                  , r8380_holRingCnt_get);
    F_00000U(pMapper->nic.holRingCnt_set                  , r8380_holRingCnt_set);
    F_00000U(pMapper->nic.ntfyBaseAddr_get                , r8380_ntfyBaseAddr_get);
    F_00000U(pMapper->nic.ntfyBaseAddr_set                , r8380_ntfyBaseAddr_set);
    F_00000U(pMapper->nic.ringBaseAddr_get                , r8380_ringBaseAddr_get);
    F_00000U(pMapper->nic.ringBaseAddr_set                , r8380_ringBaseAddr_set);
    F_00000U(pMapper->nic.ringCurAddr_get                 , r8380_ringCurAddr_get);
    F_00000U(pMapper->nic.rxTruncateLength_get            , r8380_rxTruncateLength_get);
    F_00000U(pMapper->nic.rxTruncateLength_set            , r8380_rxTruncateLength_set);
    F_00000U(pMapper->nic.nicEnable_get                   , r8380_nicEnable_get);
    F_00000U(pMapper->nic.nicEnable_set                   , r8380_nicEnable_set);
    F_00000U(pMapper->nic.nicTxFetch_set                  , r8380_nicTxFetch_set);
    F_00000U(pMapper->nic.nicTxBusySts_get                , r8380_nicTxBusySts_get);
    F_00000U(pMapper->nic.cpuTagId_get                    , r8380_cpuTagId_get);
    F_00000U(pMapper->nic.cpuTagId_set                    , r8380_cpuTagId_set);
    F_00000U(pMapper->nic.supportJumboSize_get            , r8380_supportJumboSize_get);
    F_00000U(pMapper->nic.cpuTagFromRaw_cnvt              , r8380_cpuTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTagToRaw_cnvt                , r8380_cpuTagToRaw_cnvt);
    F_00000U(pMapper->nic.cpuTag_dump                     , r8380_cpuTag_dump);
    F_00000U(pMapper->nic.rawTag_dump                     , r8380_rawTag_dump);
    F_00000U(pMapper->nic.dbg_set                         , r8380_debug_set);
    F_00000U(pMapper->nic.dbg_get                         , r8380_debug_get);
    F_00000U(pMapper->nic.cntr_dump                       , r8380_counter_dump);
    F_00000U(pMapper->nic.cntr_clear                      , r8380_counter_clear);
    F_00000U(pMapper->nic.ringbuf_dump                    , r8380_bufStatus_dump);
    F_00000U(pMapper->nic.pkthdr_mbuf_dump                , r8380_pkthdrMbuf_dump);
    F_00000U(pMapper->nic.rx_status_get                   , r8380_rxStatus_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}
#endif /*CONFIG_SDK_DRIVER_NIC*/

void drv_mapleMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

#ifndef __BOOTLOADER__
    drv_maple_intrMapper_init(pMapper);
    drv_maple_watchdogMapper_init(pMapper);
    drv_maple_uart1Mapper_init(pMapper);
#endif /*__BOOTLOADER__*/
    drv_maple_gpioMapper_init(pMapper);
    drv_maple_tcMapper_init(pMapper);
    drv_maple_rtl8231Mapper_init(pMapper);
}

void drv_swcore_mapleMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

    drv_maple_i2cMapper_init(pMapper);
    drv_maple_rtl8231Mapper_init(pMapper);
#if defined(CONFIG_SDK_DRIVER_NIC)
    drv_maple_nicMapper_init(pMapper);
#endif /*CONFIG_SDK_DRIVER_NIC*/
}
#endif

#if defined(CONFIG_SDK_RTL8390)
void drv_cypress_intrMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->intr.swcoreIsrSts_get               , rtl8390_intr_swcoreIsrSts_get);
    F_00000U(pMapper->intr.swcoreImrEnable_set            , rtl8390_intr_swcoreImrEnable_set);
    F_00000U(pMapper->intr.swcoreSts_get                  , rtl8390_intr_swcoreSts_get);
    F_00000U(pMapper->intr.swcore_exit                    , rtl8390_intr_swcore_exit);

}

void drv_cypress_gpioMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->gpio.init                           , rtl8390_gpio_init);
    F_00000U(pMapper->gpio.direction_set                  , rtl8390_gpio_direction_set);
    F_00000U(pMapper->gpio.control_set                    , rtl8390_gpio_control_set);
    F_00000U(pMapper->gpio.intrMode_set                   , rtl8390_gpio_intrMode_set);
    F_00000U(pMapper->gpio.dataBit_init                   , rtl8390_gpio_dataBit_init);
    F_00000U(pMapper->gpio.dataBit_get                    , rtl8390_gpio_dataBit_get);
    F_00000U(pMapper->gpio.dataBit_set                    , rtl8390_gpio_dataBit_set);
    F_00000U(pMapper->gpio.intrStatus_get                 , rtl8390_gpio_intrStatus_get);
    F_00000U(pMapper->gpio.intrStatus_clear               , rtl8390_gpio_intrStatus_clear);
    F_00000U(pMapper->gpio.portRange_get                  , rtl8390_gpio_portRange_get);
}

void drv_cypress_tcMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->tc.init                             , tc_init);
    F_00000U(pMapper->tc.tc_enable_set                    , tc_enable_set);
    F_00000U(pMapper->tc.tcMode_set                       , tc_mode_set);
    F_00000U(pMapper->tc.tcDivFactor_set                  , tc_divFactor_set);
    F_00000U(pMapper->tc.tcDataInitValue_set              , tc_dataInitValue_set);
    F_00000U(pMapper->tc.tcIntEnable_set                  , tc_intEnable_set);
    F_00000U(pMapper->tc.tcIntState_get                   , tc_intState_get);
    F_00000U(pMapper->tc.tcIntState_clear                 , tc_intState_clear);
    F_00000U(pMapper->tc.tcCounterValue_get               , tc_counterValue_get);
}

void drv_cypress_watchdogMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->watchdog.scale_set                  , watchdog_scale_set);
    F_000000(pMapper->watchdog.scale_get                  , watchdog_scale_get);
    F_000000(pMapper->watchdog.enable_set                 , watchdog_enable_set);
    F_000000(pMapper->watchdog.enable_get                 , watchdog_enable_get);
    F_000000(pMapper->watchdog.kick                       , watchdog_kick);
    F_000000(pMapper->watchdog.init                       , watchdog_init);
    F_000000(pMapper->watchdog.threshold_set              , watchdog_threshold_set);
    F_000000(pMapper->watchdog.threshold_get              , watchdog_threshold_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_cypress_i2cMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->i2c.init                            , r8390_i2c_init);
    F_00000U(pMapper->i2c.i2c_dev_init                    , r8390_i2c_dev_init);
    F_00000U(pMapper->i2c.i2c_write                       , r8390_i2c_write);
    F_00000U(pMapper->i2c.i2c_read                        , r8390_i2c_read);
    F_00000U(pMapper->i2c.i2c_type_set                    , NULL);
    F_00000U(pMapper->i2c.i2c_type_get                    , NULL);
}

void drv_cypress_rtl8231Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->rtl8231.mdc_read                    , r8390_rtl8231_mdc_read);
    F_00000U(pMapper->rtl8231.mdc_write                   , r8390_rtl8231_mdc_write);
    F_00000U(pMapper->rtl8231.init                        , r8390_rtl8231_init);
    F_00000U(pMapper->rtl8231.mdcSem_register             , r8390_rtl8231_mdcSem_register);
    F_00000U(pMapper->rtl8231.mdcSem_unregister           , r8390_rtl8231_mdcSem_unregister);
}

void drv_cypress_uart1Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->uart1.init                          , uart_init);
    F_00000U(pMapper->uart1.interface_set                 , uart_interface_set);
    F_00000U(pMapper->uart1.tstc                          , uart_tstc);
    F_00000U(pMapper->uart1.poll_getc                     , uart_getc);
    F_00000U(pMapper->uart1.poll_putc                     , uart_putc);
    F_00000U(pMapper->uart1.baudrate_get                  , uart_baudrate_get);
    F_00000U(pMapper->uart1.baudrate_set                  , uart_baudrate_set);
    F_00000U(pMapper->uart1.starttx                       , serial_starttx);
    F_00000U(pMapper->uart1.clearfifo                     , serial_clearfifo);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

#if defined(CONFIG_SDK_DRIVER_NIC)
void drv_cypress_nicMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->nic.init                            , r8390_init);
    F_00000U(pMapper->nic.pkt_tx                          , r8390_pkt_tx);
    F_00000U(pMapper->nic.rx_start                        , r8390_rx_start);
    F_00000U(pMapper->nic.rx_stop                         , r8390_rx_stop);
    F_00000U(pMapper->nic.rx_register                     , r8390_rx_register);
    F_00000U(pMapper->nic.rx_unregister                   , r8390_rx_unregister);
    F_00000U(pMapper->nic.pkt_alloc                       , r8390_pkt_alloc);
    F_00000U(pMapper->nic.pkt_free                        , r8390_pkt_free);
    F_00000U(pMapper->nic.ringInfo_get                    , r8390_ringInfo_get);
    F_00000U(pMapper->nic.cpuPortTxRxEnable_set           , r8390_cpuPortTxRxEnable_set);
    F_00000U(pMapper->nic.intrMask_get                    , r8390_intrMask_get);
    F_00000U(pMapper->nic.intrMask_set                    , r8390_intrMask_set);
    F_00000U(pMapper->nic.intrSts_get                     , r8390_intrSts_get);
    F_00000U(pMapper->nic.intrSts_set                     , r8390_intrSts_set);
    F_00000U(pMapper->nic.swNicRst_get                    , r8390_swNicRst_get);
    F_00000U(pMapper->nic.swNicRst_set                    , r8390_swNicRst_set);
    F_00000U(pMapper->nic.swQueRst_get                    , r8390_swQueRst_get);
    F_00000U(pMapper->nic.swQueRst_set                    , r8390_swQueRst_set);
    F_00000U(pMapper->nic.cpuL2FloodMask_add              , r8390_cpuL2FloodMask_add);
    F_00000U(pMapper->nic.cpuL2FloodMask_remove           , r8390_cpuL2FloodMask_remove);
    F_00000U(pMapper->nic.cpuForceLinkupEnable_set        , r8390_cpuForceLinkupEnable_set);
    F_00000U(pMapper->nic.holRingSize_set                 , r8390_holRingSize_set);
    F_00000U(pMapper->nic.holRingCnt_get                  , r8390_holRingCnt_get);
    F_00000U(pMapper->nic.holRingCnt_set                  , r8390_holRingCnt_set);
    F_00000U(pMapper->nic.ntfyBaseAddr_get                , r8390_ntfyBaseAddr_get);
    F_00000U(pMapper->nic.ntfyBaseAddr_set                , r8390_ntfyBaseAddr_set);
    F_00000U(pMapper->nic.ringBaseAddr_get                , r8390_ringBaseAddr_get);
    F_00000U(pMapper->nic.ringBaseAddr_set                , r8390_ringBaseAddr_set);
    F_00000U(pMapper->nic.ringCurAddr_get                 , r8390_ringCurAddr_get);
    F_00000U(pMapper->nic.rxTruncateLength_get            , r8390_rxTruncateLength_get);
    F_00000U(pMapper->nic.rxTruncateLength_set            , r8390_rxTruncateLength_set);
    F_00000U(pMapper->nic.nicEnable_get                   , r8390_nicEnable_get);
    F_00000U(pMapper->nic.nicEnable_set                   , r8390_nicEnable_set);
    F_00000U(pMapper->nic.nicTxFetch_set                  , r8390_nicTxFetch_set);
    F_00000U(pMapper->nic.nicTxBusySts_get                , r8390_nicTxBusySts_get);
    F_00000U(pMapper->nic.cpuTagId_get                    , r8390_cpuTagId_get);
    F_00000U(pMapper->nic.cpuTagId_set                    , r8390_cpuTagId_set);
    F_00000U(pMapper->nic.supportJumboSize_get            , r8390_supportJumboSize_get);
    F_00000U(pMapper->nic.cpuTagFromRaw_cnvt              , r8390_cpuTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTagToRaw_cnvt                , r8390_cpuTagToRaw_cnvt);
    F_00000U(pMapper->nic.cpuTag_dump                     , r8390_cpuTag_dump);
    F_00000U(pMapper->nic.rawTag_dump                     , r8390_rawTag_dump);
    F_00000U(pMapper->nic.dbg_set                         , r8390_debug_set);
    F_00000U(pMapper->nic.dbg_get                         , r8390_debug_get);
    F_00000U(pMapper->nic.cntr_dump                       , r8390_counter_dump);
    F_00000U(pMapper->nic.cntr_clear                      , r8390_counter_clear);
    F_00000U(pMapper->nic.ringbuf_dump                    , r8390_bufStatus_dump);
    F_00000U(pMapper->nic.pkthdr_mbuf_dump                , r8390_pkthdrMbuf_dump);
    F_00000U(pMapper->nic.rx_status_get                   , r8390_rxStatus_get);
    F_00000U(pMapper->nic.nic_reset                       , NULL);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}
#endif /*CONFIG_SDK_DRIVER_NIC*/

void drv_cypressMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

#ifndef __BOOTLOADER__
    drv_cypress_intrMapper_init(pMapper);
    drv_cypress_watchdogMapper_init(pMapper);
    drv_cypress_uart1Mapper_init(pMapper);
#endif /*__BOOTLOADER__*/
    drv_cypress_gpioMapper_init(pMapper);
    drv_cypress_tcMapper_init(pMapper);
    drv_cypress_rtl8231Mapper_init(pMapper);
}

void drv_swcore_cypressMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

    drv_cypress_i2cMapper_init(pMapper);
    drv_cypress_rtl8231Mapper_init(pMapper);
#if defined(CONFIG_SDK_DRIVER_NIC)
    drv_cypress_nicMapper_init(pMapper);
#endif /*CONFIG_SDK_DRIVER_NIC*/
}
#endif

#if defined(CONFIG_SDK_RTL9300)
void drv_longan_intrMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->intr.swcoreIsrSts_get               , rtl9300_intr_swcoreIsrSts_get);
    F_00000U(pMapper->intr.swcoreImrEnable_set            , rtl9300_intr_swcoreImrEnable_set);
    F_00000U(pMapper->intr.swcoreSts_get                  , rtl9300_intr_swcoreSts_get);
    F_00000U(pMapper->intr.swcore_exit                    , rtl9300_intr_swcore_exit);
}

void drv_longan_gpioMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->gpio.init                           , rtl9300_gpio_init);
    F_00000U(pMapper->gpio.direction_set                  , rtl9300_gpio_direction_set);
    F_00000U(pMapper->gpio.control_set                    , NULL);/* not support */
    F_00000U(pMapper->gpio.intrMode_set                   , rtl9300_gpio_intrMode_set);
    F_00000U(pMapper->gpio.dataBit_init                   , NULL);/* not needed */
    F_00000U(pMapper->gpio.dataBit_get                    , rtl9300_gpio_dataBit_get);
    F_00000U(pMapper->gpio.dataBit_set                    , rtl9300_gpio_dataBit_set);
    F_00000U(pMapper->gpio.intrStatus_get                 , rtl9300_gpio_intrStatus_get);
    F_00000U(pMapper->gpio.intrStatus_clear               , rtl9300_gpio_intrStatus_clear);
    F_00000U(pMapper->gpio.portRange_get                  , rtl9300_gpio_portRange_get);
}

void drv_longan_tcMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->tc.init                             , tc_init);
    F_00000U(pMapper->tc.tc_enable_set                    , tc_enable_set);
    F_00000U(pMapper->tc.tcMode_set                       , tc_mode_set);
    F_00000U(pMapper->tc.tcDivFactor_set                  , tc_divFactor_set);
    F_00000U(pMapper->tc.tcDataInitValue_set              , tc_dataInitValue_set);
    F_00000U(pMapper->tc.tcIntEnable_set                  , tc_intEnable_set);
    F_00000U(pMapper->tc.tcIntState_get                   , tc_intState_get);
    F_00000U(pMapper->tc.tcIntState_clear                 , tc_intState_clear);
    F_00000U(pMapper->tc.tcCounterValue_get               , tc_counterValue_get);
}

void drv_longan_watchdogMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->watchdog.scale_set                  , watchdog_scale_set);
    F_000000(pMapper->watchdog.scale_get                  , watchdog_scale_get);
    F_000000(pMapper->watchdog.enable_set                 , watchdog_enable_set);
    F_000000(pMapper->watchdog.enable_get                 , watchdog_enable_get);
    F_000000(pMapper->watchdog.kick                       , watchdog_kick);
    F_000000(pMapper->watchdog.init                       , watchdog_init);
    F_000000(pMapper->watchdog.threshold_set              , watchdog_threshold_set);
    F_000000(pMapper->watchdog.threshold_get              , watchdog_threshold_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_longan_i2cMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->i2c.init                            , r9300_i2c_init);
    F_00000U(pMapper->i2c.i2c_dev_init                    , r9300_i2c_dev_init);
    F_00000U(pMapper->i2c.i2c_write                       , r9300_i2c_write);
    F_00000U(pMapper->i2c.i2c_read                        , r9300_i2c_read);
    F_00000U(pMapper->i2c.i2c_type_set                    , NULL);
    F_00000U(pMapper->i2c.i2c_type_get                    , NULL);
}

void drv_longan_rtl8231Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->rtl8231.mdc_read                    , r9300_rtl8231_mdc_read);
    F_00000U(pMapper->rtl8231.mdc_write                   , r9300_rtl8231_mdc_write);
    F_00000U(pMapper->rtl8231.init                        , r9300_rtl8231_init);
    F_00000U(pMapper->rtl8231.intrStatus_get              , r9300_rtl8231_pinIntrStatus_get);
    F_00000U(pMapper->rtl8231.intrStatus_clear            , r9300_rtl8231_pinIntrStatus_clear);
    F_00000U(pMapper->rtl8231.extGPIOIntrStatus_get       , r9300_rtl8231_extGPIOIntrStatus_get);
    F_00000U(pMapper->rtl8231.pinIntrEnable_get           , r9300_rtl8231_pinIntrEnable_get);
    F_00000U(pMapper->rtl8231.pinIntrEnable_set           , r9300_rtl8231_pinIntrEnable_set);
    F_00000U(pMapper->rtl8231.pinIntrMode_get             , r9300_rtl8231_pinIntrMode_get);
    F_00000U(pMapper->rtl8231.pinIntrMode_set             , r9300_rtl8231_pinIntrMode_set);
    F_00000U(pMapper->rtl8231.directAccess_set            , r9300_rtl8231_directAccess_set);
}

void drv_longan_uart1Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->uart1.init                          , uart_init);
    F_000000(pMapper->uart1.interface_set                 , uart_interface_set);
    F_000000(pMapper->uart1.tstc                          , uart_tstc);
    F_000000(pMapper->uart1.poll_getc                     , uart_getc);
    F_000000(pMapper->uart1.poll_putc                     , uart_putc);
    F_000000(pMapper->uart1.baudrate_get                  , uart_baudrate_get);
    F_000000(pMapper->uart1.baudrate_set                  , uart_baudrate_set);
    F_000000(pMapper->uart1.starttx                       , serial_starttx);
    F_000000(pMapper->uart1.clearfifo                     , serial_clearfifo);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

#if defined(CONFIG_SDK_DRIVER_NIC)
void drv_longan_nicMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->nic.init                            , r9300_init);
    F_00000U(pMapper->nic.pkt_tx                          , r9300_pkt_tx);
    F_00000U(pMapper->nic.rx_start                        , r9300_rx_start);
    F_00000U(pMapper->nic.rx_stop                         , r9300_rx_stop);
    F_00000U(pMapper->nic.rx_register                     , r9300_rx_register);
    F_00000U(pMapper->nic.rx_unregister                   , r9300_rx_unregister);
    F_00000U(pMapper->nic.pkt_alloc                       , r9300_pkt_alloc);
    F_00000U(pMapper->nic.pkt_free                        , r9300_pkt_free);
    F_00000U(pMapper->nic.ringInfo_get                    , r9300_ringInfo_get);
    F_00000U(pMapper->nic.cpuPortTxRxEnable_set           , r9300_cpuPortTxRxEnable_set);
    F_00000U(pMapper->nic.intrMask_get                    , r9300_intrMask_get);
    F_00000U(pMapper->nic.intrMask_set                    , r9300_intrMask_set);
    F_00000U(pMapper->nic.intrSts_get                     , r9300_intrSts_get);
    F_00000U(pMapper->nic.intrSts_set                     , r9300_intrSts_set);
    F_00000U(pMapper->nic.swNicRst_get                    , r9300_swNicRst_get);
    F_00000U(pMapper->nic.swNicRst_set                    , r9300_swNicRst_set);
    F_00000U(pMapper->nic.swQueRst_get                    , r9300_swQueRst_get);
    F_00000U(pMapper->nic.swQueRst_set                    , r9300_swQueRst_set);
    F_00000U(pMapper->nic.cpuL2FloodMask_add              , r9300_cpuL2FloodMask_add);
    F_00000U(pMapper->nic.cpuL2FloodMask_remove           , r9300_cpuL2FloodMask_remove);
    F_00000U(pMapper->nic.cpuForceLinkupEnable_set        , r9300_cpuForceLinkupEnable_set);
    F_00000U(pMapper->nic.holRingSize_set                 , r9300_holRingSize_set);
    F_00000U(pMapper->nic.holRingCnt_get                  , r9300_holRingCnt_get);
    F_00000U(pMapper->nic.holRingCnt_set                  , r9300_holRingCnt_set);
    F_00000U(pMapper->nic.ntfyBaseAddr_get                , r9300_ntfyBaseAddr_get);
    F_00000U(pMapper->nic.ntfyBaseAddr_set                , r9300_ntfyBaseAddr_set);
    F_00000U(pMapper->nic.ringBaseAddr_get                , r9300_ringBaseAddr_get);
    F_00000U(pMapper->nic.ringBaseAddr_set                , r9300_ringBaseAddr_set);
    F_00000U(pMapper->nic.ringCurAddr_get                 , r9300_ringCurAddr_get);
    F_00000U(pMapper->nic.rxTruncateLength_get            , r9300_rxTruncateLength_get);
    F_00000U(pMapper->nic.rxTruncateLength_set            , r9300_rxTruncateLength_set);
    F_00000U(pMapper->nic.nicEnable_get                   , r9300_nicEnable_get);
    F_00000U(pMapper->nic.nicEnable_set                   , r9300_nicEnable_set);
    F_00000U(pMapper->nic.nicTxFetch_set                  , r9300_nicTxFetch_set);
    F_00000U(pMapper->nic.nicTxBusySts_get                , r9300_nicTxBusySts_get);
    F_00000U(pMapper->nic.cpuTagId_get                    , r9300_cpuTagId_get);
    F_00000U(pMapper->nic.cpuTagId_set                    , r9300_cpuTagId_set);
    F_00000U(pMapper->nic.supportJumboSize_get            , r9300_supportJumboSize_get);
    F_00000U(pMapper->nic.cpuTagFromRaw_cnvt              , r9300_cpuTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTagToRaw_cnvt                , r9300_cpuTagToRaw_cnvt);
    F_00000U(pMapper->nic.cpuTag_dump                     , r9300_cpuTag_dump);
    F_00000U(pMapper->nic.rawTag_dump                     , r9300_rawTag_dump);
    F_00000U(pMapper->nic.dbg_set                         , r9300_debug_set);
    F_00000U(pMapper->nic.dbg_get                         , r9300_debug_get);
    F_00000U(pMapper->nic.cntr_dump                       , r9300_counter_dump);
    F_00000U(pMapper->nic.cntr_clear                      , r9300_counter_clear);
    F_00000U(pMapper->nic.ringbuf_dump                    , r9300_bufStatus_dump);
    F_00000U(pMapper->nic.pkthdr_mbuf_dump                , r9300_pkthdrMbuf_dump);
    F_00000U(pMapper->nic.rx_status_get                   , r9300_rxStatus_get);
    F_00000U(pMapper->nic.nic_reset                       , NULL);
    F_00000U(pMapper->nic.cpuTxTagFromRaw_cnvt            , r9300_cpuTxTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTagLen_get                   , r9300_cpuTagLen_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}
#endif /*CONFIG_SDK_DRIVER_NIC*/

void drv_longanMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

#ifndef __BOOTLOADER__
    drv_longan_intrMapper_init(pMapper);
    drv_longan_watchdogMapper_init(pMapper);
    drv_longan_uart1Mapper_init(pMapper);
#endif /*__BOOTLOADER__*/
    drv_longan_gpioMapper_init(pMapper);
    drv_longan_tcMapper_init(pMapper);
    drv_longan_rtl8231Mapper_init(pMapper);
}

void drv_swcore_longanMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

    drv_longan_i2cMapper_init(pMapper);
    drv_longan_rtl8231Mapper_init(pMapper);
#if defined(CONFIG_SDK_DRIVER_NIC)
    drv_longan_nicMapper_init(pMapper);
#endif /*CONFIG_SDK_DRIVER_NIC*/
}
#endif

#if defined(CONFIG_SDK_RTL9310)
void drv_mango_intrMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->intr.swcoreIsrSts_get               , rtl9310_intr_swcoreIsrSts_get);
    F_00000U(pMapper->intr.swcoreImrEnable_set            , rtl9310_intr_swcoreImrEnable_set);
    F_00000U(pMapper->intr.swcoreSts_get                  , rtl9310_intr_swcoreSts_get);
    F_00000U(pMapper->intr.swcore_exit                    , rtl9310_intr_swcore_exit);
}

void drv_mango_gpioMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->gpio.init                           , gpio_common_init);
    F_00000U(pMapper->gpio.direction_set                  , gpio_common_direction_set);
    F_00000U(pMapper->gpio.control_set                    , NULL);/* not support */
    F_00000U(pMapper->gpio.intrMode_set                   , gpio_common_intrMode_set);
    F_00000U(pMapper->gpio.dataBit_init                   , NULL);/* not needed */
    F_00000U(pMapper->gpio.dataBit_get                    , gpio_common_dataBit_get);
    F_00000U(pMapper->gpio.dataBit_set                    , gpio_common_dataBit_set);
    F_00000U(pMapper->gpio.intrStatus_get                 , gpio_common_intrStatus_get);
    F_00000U(pMapper->gpio.intrStatus_clear               , gpio_common_intrStatus_clear);
    F_00000U(pMapper->gpio.portRange_get                  , gpio_common_portRange_get);
}

void drv_mango_tcMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->tc.init                             , tc_init);
    F_00000U(pMapper->tc.tc_enable_set                    , tc_enable_set);
    F_00000U(pMapper->tc.tcMode_set                       , tc_mode_set);
    F_00000U(pMapper->tc.tcDivFactor_set                  , tc_divFactor_set);
    F_00000U(pMapper->tc.tcDataInitValue_set              , tc_dataInitValue_set);
    F_00000U(pMapper->tc.tcIntEnable_set                  , tc_intEnable_set);
    F_00000U(pMapper->tc.tcIntState_get                   , tc_intState_get);
    F_00000U(pMapper->tc.tcIntState_clear                 , tc_intState_clear);
    F_00000U(pMapper->tc.tcCounterValue_get               , tc_counterValue_get);
}

void drv_mango_watchdogMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->watchdog.scale_set                  , watchdog_scale_set);
    F_000000(pMapper->watchdog.scale_get                  , watchdog_scale_get);
    F_000000(pMapper->watchdog.enable_set                 , watchdog_enable_set);
    F_000000(pMapper->watchdog.enable_get                 , watchdog_enable_get);
    F_000000(pMapper->watchdog.kick                       , watchdog_kick);
    F_000000(pMapper->watchdog.init                       , watchdog_init);
    F_000000(pMapper->watchdog.threshold_set              , watchdog_threshold_set);
    F_000000(pMapper->watchdog.threshold_get              , watchdog_threshold_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_mango_i2cMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->i2c.init                            , r9310_i2c_init);
    F_00000U(pMapper->i2c.i2c_dev_init                    , r9310_i2c_dev_init);
    F_00000U(pMapper->i2c.i2c_write                       , r9310_i2c_write);
    F_00000U(pMapper->i2c.i2c_read                        , r9310_i2c_read);
    F_00000U(pMapper->i2c.i2c_type_set                    , NULL);
    F_00000U(pMapper->i2c.i2c_type_get                    , NULL);
}

void drv_mango_rtl8231Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->rtl8231.mdc_read                    , r9310_rtl8231_mdc_read);
    F_00000U(pMapper->rtl8231.mdc_write                   , r9310_rtl8231_mdc_write);
    F_00000U(pMapper->rtl8231.init                        , r9310_rtl8231_init);
    F_00000U(pMapper->rtl8231.intrStatus_get              , r9310_rtl8231_pinIntrStatus_get);
    F_00000U(pMapper->rtl8231.intrStatus_clear            , r9310_rtl8231_pinIntrStatus_clear);
    F_00000U(pMapper->rtl8231.extGPIOIntrStatus_get       , r9310_rtl8231_extGPIOIntrStatus_get);
    F_00000U(pMapper->rtl8231.pinIntrEnable_get           , r9310_rtl8231_pinIntrEnable_get);
    F_00000U(pMapper->rtl8231.pinIntrEnable_set           , r9310_rtl8231_pinIntrEnable_set);
    F_00000U(pMapper->rtl8231.pinIntrMode_get             , r9310_rtl8231_pinIntrMode_get);
    F_00000U(pMapper->rtl8231.pinIntrMode_set             , r9310_rtl8231_pinIntrMode_set);
    F_00000U(pMapper->rtl8231.directAccess_set            , r9310_rtl8231_directAccess_set);
}

void drv_mango_uart1Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->uart1.init                          , uart_init);
    F_000000(pMapper->uart1.interface_set                 , uart_interface_set);
    F_000000(pMapper->uart1.tstc                          , uart_tstc);
    F_000000(pMapper->uart1.poll_getc                     , uart_getc);
    F_000000(pMapper->uart1.poll_putc                     , uart_putc);
    F_000000(pMapper->uart1.baudrate_get                  , uart_baudrate_get);
    F_000000(pMapper->uart1.baudrate_set                  , uart_baudrate_set);
    F_000000(pMapper->uart1.starttx                       , serial_starttx);
    F_000000(pMapper->uart1.clearfifo                     , serial_clearfifo);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

#if defined(CONFIG_SDK_DRIVER_NIC)
void drv_mango_nicMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->nic.init                            , r9310_init);
    F_00000U(pMapper->nic.pkt_tx                          , r9310_pkt_tx);
    F_00000U(pMapper->nic.rx_start                        , r9310_rx_start);
    F_00000U(pMapper->nic.rx_stop                         , r9310_rx_stop);
    F_00000U(pMapper->nic.rx_register                     , r9310_rx_register);
    F_00000U(pMapper->nic.rx_unregister                   , r9310_rx_unregister);
    F_00000U(pMapper->nic.pkt_alloc                       , r9310_pkt_alloc);
    F_00000U(pMapper->nic.pkt_free                        , r9310_pkt_free);
    F_00000U(pMapper->nic.ringInfo_get                    , r9310_ringInfo_get);
    F_00000U(pMapper->nic.cpuPortTxRxEnable_set           , r9310_cpuPortTxRxEnable_set);
    F_00000U(pMapper->nic.intrMask_get                    , r9310_intrMask_get);
    F_00000U(pMapper->nic.intrMask_set                    , r9310_intrMask_set);
    F_00000U(pMapper->nic.intrSts_get                     , r9310_intrSts_get);
    F_00000U(pMapper->nic.intrSts_set                     , r9310_intrSts_set);
    F_00000U(pMapper->nic.swNicRst_get                    , r9310_swNicRst_get);
    F_00000U(pMapper->nic.swNicRst_set                    , r9310_swNicRst_set);
    F_00000U(pMapper->nic.swQueRst_get                    , r9310_swQueRst_get);
    F_00000U(pMapper->nic.swQueRst_set                    , r9310_swQueRst_set);
    F_00000U(pMapper->nic.cpuL2FloodMask_add              , r9310_cpuL2FloodMask_add);
    F_00000U(pMapper->nic.cpuL2FloodMask_remove           , r9310_cpuL2FloodMask_remove);
    F_00000U(pMapper->nic.cpuForceLinkupEnable_set        , r9310_cpuForceLinkupEnable_set);
    F_00000U(pMapper->nic.holRingSize_set                 , r9310_holRingSize_set);
    F_00000U(pMapper->nic.holRingCnt_get                  , r9310_holRingCnt_get);
    F_00000U(pMapper->nic.holRingCnt_set                  , r9310_holRingCnt_set);
    F_00000U(pMapper->nic.ntfyBaseAddr_get                , r9310_ntfyBaseAddr_get);
    F_00000U(pMapper->nic.ntfyBaseAddr_set                , r9310_ntfyBaseAddr_set);
    F_00000U(pMapper->nic.ringBaseAddr_get                , r9310_ringBaseAddr_get);
    F_00000U(pMapper->nic.ringBaseAddr_set                , r9310_ringBaseAddr_set);
    F_00000U(pMapper->nic.ringCurAddr_get                 , r9310_ringCurAddr_get);
    F_00000U(pMapper->nic.rxTruncateLength_get            , r9310_rxTruncateLength_get);
    F_00000U(pMapper->nic.rxTruncateLength_set            , r9310_rxTruncateLength_set);
    F_00000U(pMapper->nic.nicEnable_get                   , r9310_nicEnable_get);
    F_00000U(pMapper->nic.nicEnable_set                   , r9310_nicEnable_set);
    F_00000U(pMapper->nic.nicTxFetch_set                  , r9310_nicTxFetch_set);
    F_00000U(pMapper->nic.nicTxBusySts_get                , r9310_nicTxBusySts_get);
    F_00000U(pMapper->nic.cpuTagId_get                    , r9310_cpuTagId_get);
    F_00000U(pMapper->nic.cpuTagId_set                    , r9310_cpuTagId_set);
    F_00000U(pMapper->nic.supportJumboSize_get            , r9310_supportJumboSize_get);
    F_00000U(pMapper->nic.cpuTagFromRaw_cnvt              , r9310_cpuTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTxTagFromRaw_cnvt            , r9310_cpuTxTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTagToRaw_cnvt                , r9310_cpuTagToRaw_cnvt);
    F_00000U(pMapper->nic.cpuTag_dump                     , r9310_cpuTag_dump);
    F_00000U(pMapper->nic.rawTag_dump                     , r9310_rawTag_dump);
    F_00000U(pMapper->nic.dbg_set                         , r9310_debug_set);
    F_00000U(pMapper->nic.dbg_get                         , r9310_debug_get);
    F_00000U(pMapper->nic.cntr_dump                       , r9310_counter_dump);
    F_00000U(pMapper->nic.cntr_clear                      , r9310_counter_clear);
    F_00000U(pMapper->nic.ringbuf_dump                    , r9310_bufStatus_dump);
    F_00000U(pMapper->nic.pkthdr_mbuf_dump                , r9310_pkthdrMbuf_dump);
    F_00000U(pMapper->nic.rx_status_get                   , r9310_rxStatus_get);
    F_00000U(pMapper->nic.cpuTagLen_get                   , r9310_cpuTagLen_get);
    F_00000U(pMapper->nic.nic_reset                       , NULL);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}
#endif /*CONFIG_SDK_DRIVER_NIC*/

void drv_mangoMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

#ifndef __BOOTLOADER__
    drv_mango_intrMapper_init(pMapper);
    drv_mango_watchdogMapper_init(pMapper);
    drv_mango_uart1Mapper_init(pMapper);
#endif /*__BOOTLOADER__*/
    drv_mango_gpioMapper_init(pMapper);
    drv_mango_tcMapper_init(pMapper);
    drv_mango_rtl8231Mapper_init(pMapper);
}

void drv_swcore_mangoMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

    drv_mango_i2cMapper_init(pMapper);
    drv_mango_rtl8231Mapper_init(pMapper);
#if defined(CONFIG_SDK_DRIVER_NIC)
    drv_mango_nicMapper_init(pMapper);
#endif /*CONFIG_SDK_DRIVER_NIC*/

}
#endif

#ifdef CONFIG_SDK_OOB_PHY_8213B
void drv_phy_8213b_Mapper_init(drv_mapper_t *pMapper, uint32 unit, rtk_port_t port)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->oob[0].phyAutoNegoEnable_get           , phy_8213b_AutoNegoEnable_get);
    F_00000U(pMapper->oob[0].phyAutoNegoEnable_set           , phy_8213b_AutoNegoEnable_set);
    F_00000U(pMapper->oob[0].phyAutoNegoAbility_get          , phy_8213b_AutoNegoAbility_get);
    F_00000U(pMapper->oob[0].phyAutoNegoAbility_set          , phy_8213b_AutoNegoAbility_set);
    F_00000U(pMapper->oob[0].phyEnable_get                   , phy_8213b_Enable_get);
    F_00000U(pMapper->oob[0].phyEnable_set                   , phy_8213b_Enable_set);
    F_00000U(pMapper->oob[0].phySpeed_get                    , phy_8213b_Speed_get);
    F_00000U(pMapper->oob[0].phySpeed_set                    , phy_8213b_Speed_set);
    F_00000U(pMapper->oob[0].phyDuplex_get                   , phy_8213b_Duplex_get);
    F_00000U(pMapper->oob[0].phyDuplex_set                   , phy_8213b_Duplex_set);
    F_00000U(pMapper->oob[0]._phyLink_get                    , phy_8213b_Link_get);
    F_00000U(pMapper->oob[0].phyEeeEnable_get                , phy_8213b_EeeEnable_get);
    F_00000U(pMapper->oob[0].phyEeeEnable_set                , phy_8213b_EeeEnable_set);
    F_00000U(pMapper->oob[0].phy_init                        , phy_8213b_init);
    F_00000U(pMapper->oob[0].phyLink_get                     , dal_common_oob_phyLink_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}
#endif

#if defined(CONFIG_SDK_RTL9311B)

void drv_tigershark_intrMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_0000NU(pMapper->intr.init                           , rtl9311b_intr_init);
    F_0000NU(pMapper->intr.swcore_init                    , rtl9311b_intr_swcore_init);
    F_0000NU(pMapper->intr.swcoreIsrSts_get               , rtl9311b_intr_swcoreIsrSts_get);
    F_0000NU(pMapper->intr.swcoreImrEnable_set            , rtl9311b_intr_swcoreImrEnable_set);
    F_0000NU(pMapper->intr.swcoreSts_get                  , rtl9311b_intr_swcoreSts_get);
    F_0000NU(pMapper->intr._isrStsByDevType_get           , rtl9311b_intr_isrStsByDevId_get);
    F_0000NU(pMapper->intr._gmacImrEnable_get             , rtl9311b_intr_gmacImrEnable_get);
    F_0000NU(pMapper->intr.exit                           , rtl9311b_intr_exit);
    F_0000NU(pMapper->intr.swcore_exit                    , rtl9311b_intr_swcore_exit);
}

void drv_tigershark_gpioMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->gpio.init                           , gpio_common_init);
    F_00000U(pMapper->gpio.direction_set                  , gpio_common_direction_set);
    F_00000U(pMapper->gpio.control_set                    , NULL);/* not support */
    F_00000U(pMapper->gpio.intrMode_set                   , gpio_common_intrMode_set);
    F_00000U(pMapper->gpio.dataBit_init                   , NULL);/* not needed */
    F_00000U(pMapper->gpio.dataBit_get                    , gpio_common_dataBit_get);
    F_00000U(pMapper->gpio.dataBit_set                    , gpio_common_dataBit_set);
    F_00000U(pMapper->gpio.intrStatus_get                 , gpio_common_intrStatus_get);
    F_00000U(pMapper->gpio.intrStatus_clear               , gpio_common_intrStatus_clear);
    F_00000U(pMapper->gpio.portRange_get                  , gpio_common_portRange_get);
}

void drv_tigershark_tcMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->tc.init                             , tc_init);
    F_00000U(pMapper->tc.tc_enable_set                    , tc_enable_set);
    F_00000U(pMapper->tc.tcMode_set                       , tc_mode_set);
    F_00000U(pMapper->tc.tcDivFactor_set                  , tc_divFactor_set);
    F_00000U(pMapper->tc.tcDataInitValue_set              , tc_dataInitValue_set);
    F_00000U(pMapper->tc.tcIntEnable_set                  , tc_intEnable_set);
    F_00000U(pMapper->tc.tcIntState_get                   , tc_intState_get);
    F_00000U(pMapper->tc.tcIntState_clear                 , tc_intState_clear);
    F_00000U(pMapper->tc.tcCounterValue_get               , tc_counterValue_get);
}

void drv_tigershark_watchdogMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->watchdog.scale_set                  , watchdog_scale_set);
    F_000000(pMapper->watchdog.scale_get                  , watchdog_scale_get);
    F_000000(pMapper->watchdog.enable_set                 , watchdog_enable_set);
    F_000000(pMapper->watchdog.enable_get                 , watchdog_enable_get);
    F_000000(pMapper->watchdog.kick                       , watchdog_kick);
    F_000000(pMapper->watchdog.init                       , watchdog_init);
    F_000000(pMapper->watchdog.threshold_set              , watchdog_threshold_set);
    F_000000(pMapper->watchdog.threshold_get              , watchdog_threshold_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */

}

void drv_tigershark_i2cMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->i2c.init                            , r9311b_i2c_init);
    F_00000U(pMapper->i2c.i2c_dev_init                    , r9311b_i2c_dev_init);
    F_00000U(pMapper->i2c.i2c_write                       , r9311b_i2c_write);
    F_00000U(pMapper->i2c.i2c_read                        , r9311b_i2c_read);
    F_00000U(pMapper->i2c.i2c_type_set                    , NULL);
    F_00000U(pMapper->i2c.i2c_type_get                    , NULL);
}

void drv_tigershark_rtl8231Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->rtl8231.mdc_read                    , r9311b_rtl8231_mdc_read);
    F_00000U(pMapper->rtl8231.mdc_write                   , r9311b_rtl8231_mdc_write);
    F_00000U(pMapper->rtl8231.init                        , r9311b_rtl8231_init);
    F_00000U(pMapper->rtl8231.intrStatus_get              , r9311b_rtl8231_pinIntrStatus_get);
    F_00000U(pMapper->rtl8231.intrStatus_clear            , r9311b_rtl8231_pinIntrStatus_clear);
    F_00000U(pMapper->rtl8231.extGPIOIntrStatus_get       , r9311b_rtl8231_extGPIOIntrStatus_get);
    F_00000U(pMapper->rtl8231.pinIntrEnable_get           , r9311b_rtl8231_pinIntrEnable_get);
    F_00000U(pMapper->rtl8231.pinIntrEnable_set           , r9311b_rtl8231_pinIntrEnable_set);
    F_00000U(pMapper->rtl8231.pinIntrMode_get             , r9311b_rtl8231_pinIntrMode_get);
    F_00000U(pMapper->rtl8231.pinIntrMode_set             , r9311b_rtl8231_pinIntrMode_set);
    F_00000U(pMapper->rtl8231.directAccess_set            , r9311b_rtl8231_directAccess_set);
}

void drv_tigershark_uart1Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_000000(pMapper->uart1.init                          , uart_init);
    F_000000(pMapper->uart1.interface_set                 , uart_interface_set);
    F_000000(pMapper->uart1.tstc                          , uart_tstc);
    F_000000(pMapper->uart1.poll_getc                     , uart_getc);
    F_000000(pMapper->uart1.poll_putc                     , uart_putc);
    F_000000(pMapper->uart1.baudrate_get                  , uart_baudrate_get);
    F_000000(pMapper->uart1.baudrate_set                  , hsuart_baudrate_set);
    F_000000(pMapper->uart1.starttx                       , serial_starttx);
    F_000000(pMapper->uart1.clearfifo                     , serial_clearfifo);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

#if defined(CONFIG_SDK_DRIVER_NIC)
void drv_tigershark_nicMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_0000NU(pMapper->nic.init                            , r9311B_init);
    F_0000NU(pMapper->nic.pkt_tx                          , NULL);
    F_0000NU(pMapper->nic.rx_start                        , r9311B_rx_start);
    F_0000NU(pMapper->nic.rx_stop                         , r9311B_rx_stop);
    F_0000NU(pMapper->nic.rx_register                     , r9311B_rx_register);
    F_0000NU(pMapper->nic.rx_unregister                   , r9311B_rx_unregister);
    F_0000NU(pMapper->nic.pkt_alloc                       , r9311B_pkt_alloc);
    F_0000NU(pMapper->nic.pkt_free                        , r9311B_pkt_free);
    F_0000NU(pMapper->nic.local_ringSts_get               , r9311B_local_ringSts_get);
    F_0000NU(pMapper->nic.ringInfo_get                    , r9311B_ringInfo_get);
    F_0000NU(pMapper->nic.cpuPortTxRxEnable_set           , r9311B_cpuPortTxRxEnable_set);
    F_0000NU(pMapper->nic.intrMask_get                    , r9311B_intrMask_get);
    F_0000NU(pMapper->nic.intrMask_set                    , r9311B_intrMask_set);
    F_0000NU(pMapper->nic.intrSts_get                     , r9311B_intrSts_get);
    F_0000NU(pMapper->nic.intrSts_set                     , r9311B_intrSts_set);
    F_0000NU(pMapper->nic.swNicRst_get                    , r9311B_swNicRst_get);
    F_0000NU(pMapper->nic.swNicRst_set                    , r9311B_swNicRst_set);
    F_0000NU(pMapper->nic.swQueRst_get                    , r9311B_swQueRst_get);
    F_0000NU(pMapper->nic.swQueRst_set                    , r9311B_swQueRst_set);
    F_0000NU(pMapper->nic.cpuL2FloodMask_add              , r9311B_cpuL2FloodMask_add);
    F_0000NU(pMapper->nic.cpuL2FloodMask_remove           , r9311B_cpuL2FloodMask_remove);
    F_0000NU(pMapper->nic.cpuForceLinkupEnable_set        , r9311B_cpuForceLinkupEnable_set);
    F_0000NU(pMapper->nic.holRingSize_set                 , r9311B_holRingSize_set);
    F_0000NU(pMapper->nic.holRingCnt_get                  , r9311B_holRingCnt_get);
    F_0000NU(pMapper->nic.holRingCnt_set                  , r9311B_holRingCnt_set);
    F_0000NU(pMapper->nic.ntfyBaseAddr_get                , r9311B_ntfyBaseAddr_get);
    F_0000NU(pMapper->nic.ntfyBaseAddr_set                , r9311B_ntfyBaseAddr_set);
    F_0000NU(pMapper->nic.ringBaseAddr_get                , r9311B_ringBaseAddr_get);
    F_0000NU(pMapper->nic.ringBaseAddr_set                , r9311B_ringBaseAddr_set);
    F_0000NU(pMapper->nic.ringCurAddr_get                 , r9311B_ringCurAddr_get);
    F_0000NU(pMapper->nic.cpuPtr_get                      , r9311B_cpuPtr_get);
    F_0000NU(pMapper->nic.cpuPtr_set                      , r9311B_cpuPtr_set);
    F_0000NU(pMapper->nic.hwPtr_get                       , r9311B_hwPtr_get);
    F_0000NU(pMapper->nic.hwPtr_set                       , r9311B_hwPtr_set);
    F_0000NU(pMapper->nic.rxTruncateLength_get            , r9311B_rxTruncateLength_get);
    F_0000NU(pMapper->nic.rxTruncateLength_set            , r9311B_rxTruncateLength_set);
    F_0000NU(pMapper->nic.nicEnable_get                   , r9311B_nicEnable_get);
    F_0000NU(pMapper->nic.nicEnable_set                   , r9311B_nicEnable_set);
    F_0000NU(pMapper->nic.nicTxFetch_set                  , r9311B_nicTxFetch_set);
    F_0000NU(pMapper->nic.nicTxBusySts_get                , r9311B_nicTxBusySts_get);
    F_0000NU(pMapper->nic.cpuTagId_get                    , r9311B_cpuTagId_get);
    F_0000NU(pMapper->nic.cpuTagId_set                    , r9311B_cpuTagId_set);
    F_0000NU(pMapper->nic.supportJumboSize_get            , r9311B_supportJumboSize_get);
    F_0000NU(pMapper->nic.cpuTagFromRaw_cnvt              , r9311B_cpuTagFromRaw_cnvt);
    F_0000NU(pMapper->nic.cpuTagToRaw_cnvt                , r9311B_cpuTagToRaw_cnvt);
    F_0000NU(pMapper->nic.cpuTag_dump                     , r9311B_cpuTag_dump);
    F_0000NU(pMapper->nic.rawTag_dump                     , r9311B_rawTag_dump);
    F_0000NU(pMapper->nic.dbg_set                         , r9311B_debug_set);
    F_0000NU(pMapper->nic.dbg_get                         , r9311B_debug_get);
    F_0000NU(pMapper->nic.cntr_dump                       , r9311B_counter_dump);
    F_0000NU(pMapper->nic.cntr_clear                      , r9311B_counter_clear);
    F_0000NU(pMapper->nic.ringbuf_dump                    , r9311B_bufStatus_dump);
    F_0000NU(pMapper->nic.pkthdr_mbuf_dump                , r9311B_pkthdrMbuf_dump);
    F_0000NU(pMapper->nic.rx_status_get                   , r9311B_rxStatus_get);
    F_0000NU(pMapper->nic.cpuTxTagFromRaw_cnvt            , r9311B_cpuTxTagFromRaw_cnvt);
    F_0000NU(pMapper->nic.cpuTagLen_get                   , r9311B_cpuTagLen_get);
    F_0000NU(pMapper->nic.nic_reset                       , NULL);
    F_0000NU(pMapper->nic.sramEntry_get                   , r9311B_sramEntry_get);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}
#endif /*CONFIG_SDK_DRIVER_NIC*/


void drv_tigershark_oobMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_0000NU(pMapper->oob[0].init                            , dal_common_oob_init);
    F_0000NU(pMapper->oob[0].exit                            , dal_common_oob_exit);
    F_0000NU(pMapper->oob[0].oobEnable_get                   , dal_tigershark_oob_oobEnable_get);
    F_0000NU(pMapper->oob[0].oobEnable_set                   , dal_tigershark_oob_oobEnable_set);
    F_0000NU(pMapper->oob[0].dbg_set                         , dal_common_oob_dbg_set);
    F_0000NU(pMapper->oob[0].dbg_get                         , dal_common_oob_dbg_get);
    F_0000NU(pMapper->oob[0].stat_getAll                     , dal_common_oob_stat_getAll);
    F_0000NU(pMapper->oob[0].pkt_free                        , dal_common_oob_pkt_free);
    F_0000NU(pMapper->oob[0].pkt_alloc                       , dal_common_oob_pkt_alloc);
    F_0000NU(pMapper->oob[0].reset                           , dal_common_oob_reset);
    F_0000NU(pMapper->oob[0].rx_register                     , dal_common_oob_rx_register);
    F_0000NU(pMapper->oob[0].rx_unregister                   , dal_common_oob_rx_unregister);
    F_0000NU(pMapper->oob[0].ringbuf_dump                    , dal_common_oob_ringbuf_dump);
    F_0000NU(pMapper->oob[0].pkt_tx                          , dal_common_oob_pkt_tx);
    F_0000NU(pMapper->oob[0].desc_init                       , dal_common_oob_desc_init);
    F_0000NU(pMapper->oob[0]._ringBaseAddr_get               , dal_tigershark_oob_ringBaseAddr_get);
    F_0000NU(pMapper->oob[0]._ringBaseAddr_set               , dal_tigershark_oob_ringBaseAddr_set);
    F_0000NU(pMapper->oob[0]._ringCurAddr_get                , dal_tigershark_oob_ringCurAddr_get);
    F_0000NU(pMapper->oob[0]._cpuPtr_get                     , dal_tigershark_oob_cpuPtr_get);
    F_0000NU(pMapper->oob[0]._cpuPtr_set                     , dal_tigershark_oob_cpuPtr_set);
    F_0000NU(pMapper->oob[0]._hwPtr_get                      , dal_tigershark_oob_hwPtr_get);
    F_0000NU(pMapper->oob[0]._oobTxFetch_set                 , dal_tigershark_oob_tx_kick);
    F_0000NU(pMapper->oob[0]._supportJumboSize_get           , dal_tigershark_oob_supportJumboSize_get);
    F_0000NU(pMapper->oob[0]._descSize_get                   , dal_tigershark_oob_descSize_get);
    F_0000NU(pMapper->oob[0]._desc_to_raw                    , dal_tigershark_oob_desc_to_raw);
    F_0000NU(pMapper->oob[0]._raw_to_desc                    , dal_tigershark_oob_raw_to_desc);
    F_0000NU(pMapper->oob[0]._swOobRst_set                   , dal_tigershark_oob_swOobRst_set);
    F_0000NU(pMapper->oob[0]._ethModuleEnable_set            , dal_tigershark_oob_ethModuleEnable_set);
    F_0000NU(pMapper->oob[0]._ringEnable_set                 , dal_tigershark_oob_ringEnable_set);
    F_0000NU(pMapper->oob[0]._desc_dump                      , dal_tigershark_oob_desc_dump);
    F_0000NU(pMapper->oob[0]._mac_init                       , dal_tigershark_oob_mac_init);
    F_0000NU(pMapper->oob[0]._sds_construct                  , dal_tigershark_oob_sds_construct);
    F_0000NU(pMapper->oob[0].mgmtMacAddr_get                 , dal_tigershark_oob_mgmtMacAddr_get);
    F_0000NU(pMapper->oob[0].mgmtMacAddr_set                 , dal_tigershark_oob_mgmtMacAddr_set);
    F_0000NU(pMapper->oob[0].rxMode_get                      , dal_tigershark_oob_rxMode_get);
    F_0000NU(pMapper->oob[0].rxMode_set                      , dal_tigershark_oob_rxMode_set);
    F_0000NU(pMapper->oob[0].jumboModeEnable_get             , dal_tigershark_oob_jumboModeEnable_get);
    F_0000NU(pMapper->oob[0].jumboModeEnable_set             , dal_tigershark_oob_jumboModeEnable_set);
    F_0000NU(pMapper->oob[0].pollingPhyEnable_get            , dal_tigershark_oob_pollingPhyEnable_get);
    F_0000NU(pMapper->oob[0].pollingPhyEnable_set            , dal_tigershark_oob_pollingPhyEnable_set);
    F_0000NU(pMapper->oob[0]._pollingPhyStatus_get           , dal_tigershark_oob_pollingPhyStatus_get);
    F_0000NU(pMapper->oob[0].forceFc_get                     , dal_tigershark_oob_forceFc_get);
    F_0000NU(pMapper->oob[0].forceFc_set                     , dal_tigershark_oob_forceFc_set);
    F_0000NU(pMapper->oob[0].forceFullDuplex_get             , dal_tigershark_oob_forceFullDuplex_get);
    F_0000NU(pMapper->oob[0].forceFullDuplex_set             , dal_tigershark_oob_forceFullDuplex_set);
    F_0000NU(pMapper->oob[0].forceSpeed_get                  , dal_tigershark_oob_forceSpeed_get);
    F_0000NU(pMapper->oob[0].forceSpeed_set                  , dal_tigershark_oob_forceSpeed_set);
    F_0000NU(pMapper->oob[0].forceLink_get                   , dal_tigershark_oob_forceLink_get);
    F_0000NU(pMapper->oob[0].forceLink_set                   , dal_tigershark_oob_forceLink_set);
    F_0000NU(pMapper->oob[0].miiBus_read                     , dal_tigershark_oob_miiBus_read);
    F_0000NU(pMapper->oob[0].miiBus_write                    , dal_tigershark_oob_miiBus_write);
    F_0000NU(pMapper->oob[0].sdsReg_get                      , dal_tigershark_oob_sdsReg_get);
    F_0000NU(pMapper->oob[0].sdsReg_set                      , dal_tigershark_oob_sdsReg_set);
    F_0000NU(pMapper->oob[0].phy_connect                     , dal_common_oob_phy_connect);
    F_0000NU(pMapper->oob[0].linkMon_enable                  , dal_common_oob_linkMon_enable);
    F_0000NU(pMapper->oob[0].linkMon_disable                 , dal_common_oob_linkMon_disable);
    F_0000NU(pMapper->oob[0].linkMon_register                , dal_common_oob_linkMon_register);
    F_0000NU(pMapper->oob[0].linkMon_unregister              , dal_common_oob_linkMon_unregister);
    F_0000NU(pMapper->oob[0].linkMonSwScanEnable_get         , dal_common_oob_linkMon_swScanEnable_get);
    F_0000NU(pMapper->oob[0].linkMonSwScanEnable_set         , dal_common_oob_linkMon_swScanEnable_set);
    F_0000NU(pMapper->oob[0].txData_set                      , dal_common_oob_txData_set);
    F_0000NU(pMapper->oob[0].diagPkt_send                    , dal_common_oob_diagPkt_send);
    F_0000NU(pMapper->oob[0]._exit                           , _dal_tigershark_oob_exit);
    F_0000NU(pMapper->oob[0]._nic_intr_enable_set            , dal_tigershark_oob_nic_intr_enable_set);
    F_0000NU(pMapper->oob[0]._nic_init                       , dal_tigershark_oob_nic_init);
    F_0000NU(pMapper->oob[0]._portMiim_read                   , dal_tigershark_portMiim_read);
#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
    F_0000NU(pMapper->oob[0].pktBuf_baseAdddr_get            , dal_common_oob_pktBuf_baseAdddr_get);
#endif
    F_0000NU(pMapper->oob[0]._ringInfo_get                   , dal_tigershark_oob_ringInfo_get);
    F_0000NU(pMapper->oob[0]._hw_cfg_dump                    , dal_tigershark_oob_hw_info_dump);
    F_0000NU(pMapper->oob[0]._stat_getAll                    , dal_tigershark_oob_stat_getAll);
    F_0000NU(pMapper->oob[0]._init_sts_set                   , dal_tigershark_oob_init_sts_set);

#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_tigersharkMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

#ifndef __BOOTLOADER__
    drv_tigershark_watchdogMapper_init(pMapper);
    drv_tigershark_uart1Mapper_init(pMapper);
#endif /*__BOOTLOADER__*/
    drv_tigershark_intrMapper_init(pMapper);
    drv_tigershark_gpioMapper_init(pMapper);
    drv_tigershark_tcMapper_init(pMapper);
    drv_tigershark_oobMapper_init(pMapper);
    drv_tigershark_rtl8231Mapper_init(pMapper);
}

void drv_swcore_tigersharkMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);
    F_00000U(pMapper->drv_exit, drv_common_exit);

#ifndef __BOOTLOADER__
    drv_tigershark_intrMapper_init(pMapper);
#endif
    drv_tigershark_i2cMapper_init(pMapper);
    drv_tigershark_rtl8231Mapper_init(pMapper);
#if defined(CONFIG_SDK_DRIVER_NIC)
    drv_tigershark_nicMapper_init(pMapper);
#endif /*CONFIG_SDK_DRIVER_NIC*/
}
#endif

#if defined(CONFIG_SDK_RTL9330)
void drv_bumblebee_intrMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
    F_00000U(pMapper->intr.init                           , rtl9330_intr_init);
    F_00000U(pMapper->intr.swcoreIsrSts_get               , rtl9330_intr_swcoreIsrSts_get);
    F_00000U(pMapper->intr.swcoreImrEnable_set            , rtl9330_intr_swcoreImrEnable_set);
    F_00000U(pMapper->intr.swcoreSts_get                  , rtl9330_intr_swcoreSts_get);
    F_00000U(pMapper->intr._isrStsByDevType_get           , rtl9330_intr_isrStsByDevId_get);
}

void drv_bumblebee_gpioMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_bumblebee_tcMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_bumblebee_watchdogMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_bumblebee_i2cMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_bumblebee_rtl8231Mapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

#if defined(CONFIG_SDK_DRIVER_NIC)
void drv_bumblebee_nicMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->nic.init                            , r9330_init);
    F_00000U(pMapper->nic.pkt_tx                          , NULL);
    F_00000U(pMapper->nic.rx_start                        , r9330_rx_start);
    F_00000U(pMapper->nic.rx_stop                         , r9330_rx_stop);
    F_00000U(pMapper->nic.rx_register                     , r9330_rx_register);
    F_00000U(pMapper->nic.rx_unregister                   , r9330_rx_unregister);
    F_00000U(pMapper->nic.pkt_alloc                       , r9330_pkt_alloc);
    F_00000U(pMapper->nic.pkt_free                        , r9330_pkt_free);
    F_00000U(pMapper->nic.local_ringSts_get               , r9330_local_ringSts_get);
    F_00000U(pMapper->nic.ringInfo_get                    , r9330_ringInfo_get);
    F_00000U(pMapper->nic.cpuPortTxRxEnable_set           , r9330_cpuPortTxRxEnable_set);
    F_00000U(pMapper->nic.intrMask_get                    , r9330_intrMask_get);
    F_00000U(pMapper->nic.intrMask_set                    , r9330_intrMask_set);
    F_00000U(pMapper->nic.intrSts_get                     , r9330_intrSts_get);
    F_00000U(pMapper->nic.intrSts_set                     , r9330_intrSts_set);
    F_00000U(pMapper->nic.swNicRst_get                    , r9330_swNicRst_get);
    F_00000U(pMapper->nic.swNicRst_set                    , r9330_swNicRst_set);
    F_00000U(pMapper->nic.swQueRst_get                    , r9330_swQueRst_get);
    F_00000U(pMapper->nic.swQueRst_set                    , r9330_swQueRst_set);
    F_00000U(pMapper->nic.cpuL2FloodMask_add              , r9330_cpuL2FloodMask_add);
    F_00000U(pMapper->nic.cpuL2FloodMask_remove           , r9330_cpuL2FloodMask_remove);
    F_00000U(pMapper->nic.cpuForceLinkupEnable_set        , r9330_cpuForceLinkupEnable_set);
    F_00000U(pMapper->nic.holRingSize_set                 , r9330_holRingSize_set);
    F_00000U(pMapper->nic.holRingCnt_get                  , r9330_holRingCnt_get);
    F_00000U(pMapper->nic.holRingCnt_set                  , r9330_holRingCnt_set);
    F_00000U(pMapper->nic.ntfyBaseAddr_get                , r9330_ntfyBaseAddr_get);
    F_00000U(pMapper->nic.ntfyBaseAddr_set                , r9330_ntfyBaseAddr_set);
    F_00000U(pMapper->nic.ringBaseAddr_get                , r9330_ringBaseAddr_get);
    F_00000U(pMapper->nic.ringBaseAddr_set                , r9330_ringBaseAddr_set);
    F_00000U(pMapper->nic.ringCurAddr_get                 , r9330_ringCurAddr_get);
    F_00000U(pMapper->nic.cpuPtr_get                      , r9330_cpuPtr_get);
    F_00000U(pMapper->nic.cpuPtr_set                      , r9330_cpuPtr_set);
    F_00000U(pMapper->nic.hwPtr_get                       , r9330_hwPtr_get);
    F_00000U(pMapper->nic.hwPtr_set                       , r9330_hwPtr_set);
    F_00000U(pMapper->nic.rxTruncateEnable_get            , r9330_rxTruncateEnable_get);
    F_00000U(pMapper->nic.rxTruncateEnable_set            , r9330_rxTruncateEnable_set);
    F_00000U(pMapper->nic.rxTruncateLength_get            , r9330_rxTruncateLength_get);
    F_00000U(pMapper->nic.rxTruncateLength_set            , r9330_rxTruncateLength_set);
    F_00000U(pMapper->nic.nicEnable_get                   , r9330_nicEnable_get);
    F_00000U(pMapper->nic.nicEnable_set                   , r9330_nicEnable_set);
    F_00000U(pMapper->nic.nicTxFetch_set                  , r9330_nicTxFetch_set);
    F_00000U(pMapper->nic.nicTxBusySts_get                , r9330_nicTxBusySts_get);
    F_00000U(pMapper->nic.cpuTagId_get                    , r9330_cpuTagId_get);
    F_00000U(pMapper->nic.cpuTagId_set                    , r9330_cpuTagId_set);
    F_00000U(pMapper->nic.supportJumboSize_get            , r9330_supportJumboSize_get);
    F_00000U(pMapper->nic.cpuTagFromRaw_cnvt              , r9330_cpuTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTxTagFromRaw_cnvt            , r9330_cpuTxTagFromRaw_cnvt);
    F_00000U(pMapper->nic.cpuTagToRaw_cnvt                , r9330_cpuTagToRaw_cnvt);
    F_00000U(pMapper->nic.cpuTag_dump                     , r9330_cpuTag_dump);
    F_00000U(pMapper->nic.rawTag_dump                     , r9330_rawTag_dump);
    F_00000U(pMapper->nic.dbg_set                         , r9330_debug_set);
    F_00000U(pMapper->nic.dbg_get                         , r9330_debug_get);
    F_00000U(pMapper->nic.cntr_dump                       , r9330_counter_dump);
    F_00000U(pMapper->nic.cntr_clear                      , r9330_counter_clear);
    F_00000U(pMapper->nic.ringbuf_dump                    , r9330_bufStatus_dump);
    F_00000U(pMapper->nic.pkthdr_mbuf_dump                , r9330_pkthdrMbuf_dump);
    F_00000U(pMapper->nic.rx_status_get                   , r9330_rxStatus_get);
    F_00000U(pMapper->nic.cpuTagLen_get                   , r9330_cpuTagLen_get);
    F_00000U(pMapper->nic.nic_reset                       , NULL);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}

void drv_bumblebee_oobMapper_init(drv_mapper_t *pMapper)
{
#ifdef RTK_USERMODE_LKM
    /* Hook callback for User-Mode kernel space */
#else
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space */
    F_00000U(pMapper->oob[0].init                            , dal_bumblebee_oob_init);
    F_00000U(pMapper->oob[0].exit              , dal_bumblebee_oob_exit);

    F_00000U(pMapper->oob[0].ringBaseAddr_get                , dal_bumblebee_oob_ringBaseAddr_get);
    F_00000U(pMapper->oob[0].ringBaseAddr_set                , dal_bumblebee_oob_ringBaseAddr_set);
    F_00000U(pMapper->oob[0].ringCurAddr_get                 , dal_bumblebee_oob_ringCurAddr_get);
    F_00000U(pMapper->oob[0].cpuPtr_get                      , dal_bumblebee_oob_cpuPtr_get);
    F_00000U(pMapper->oob[0].cpuPtr_set                      , dal_bumblebee_oob_cpuPtr_set);
    F_00000U(pMapper->oob[0].hwPtr_get                       , dal_bumblebee_oob_hwPtr_get);
    F_00000U(pMapper->oob[0].oobEnable_get                   , dal_bumblebee_oob_oobEnable_get);
    F_00000U(pMapper->oob[0].oobEnable_set                   , dal_bumblebee_oob_oobEnable_set);
    F_00000U(pMapper->oob[0].oobTxFetch_set                  , dal_bumblebee_oob_tx_kick);
    F_00000U(pMapper->oob[0].supportJumboSize_get            , dal_bumblebee_oob_supportJumboSize_get);
    F_00000U(pMapper->oob[0].descSize_get                    , dal_bumblebee_oob_descSize_get);
    F_00000U(pMapper->oob[0].desc_to_raw                     , dal_bumblebee_oob_desc_to_raw);
    F_00000U(pMapper->oob[0].raw_to_desc                     , dal_bumblebee_oob_raw_to_desc);
    F_00000U(pMapper->oob[0].swOobRst_set                    , dal_bumblebee_oob_swOobRst_set);
    F_00000U(pMapper->oob[0].ethModuleEnable_set             , dal_bumblebee_oob_ethModuleEnable_set);
    F_00000U(pMapper->oob[0].ringEnable_set                  , dal_bumblebee_oob_ringEnable_set);
    F_00000U(pMapper->oob[0].dbg_set                         , dal_bumblebee_oob_dbg_set);
    F_00000U(pMapper->oob[0].dbg_get                         , dal_bumblebee_oob_dbg_get);

    F_00000U(pMapper->oob[0]._mac_init                       , dal_bumblebee_oob_mac_init);
    F_00000U(pMapper->oob[0].mgmtMacAddr_get                 , dal_bumblebee_oob_mgmtMacAddr_get);
    F_00000U(pMapper->oob[0].mgmtMacAddr_set                 , dal_bumblebee_oob_mgmtMacAddr_set);
    F_00000U(pMapper->oob[0].rxMode_get                      , dal_bumblebee_oob_rxMode_get);
    F_00000U(pMapper->oob[0].rxMode_set                      , dal_bumblebee_oob_rxMode_set);
    F_00000U(pMapper->oob[0].jumboModeEnable_get             , dal_bumblebee_oob_jumboModeEnable_get);
    F_00000U(pMapper->oob[0].jumboModeEnable_set             , dal_bumblebee_oob_jumboModeEnable_set);
    F_00000U(pMapper->oob[0].pollingPhyEnable_get            , dal_bumblebee_oob_pollingPhyEnable_get);
    F_00000U(pMapper->oob[0].pollingPhyEnable_set            , dal_bumblebee_oob_pollingPhyEnable_set);
    F_00000U(pMapper->oob[0].forceFc_get                     , dal_bumblebee_oob_forceFc_get);
    F_00000U(pMapper->oob[0].forceFc_set                     , dal_bumblebee_oob_forceFc_set);
    F_00000U(pMapper->oob[0].forceFullDuplex_get             , dal_bumblebee_oob_forceFullDuplex_get);
    F_00000U(pMapper->oob[0].forceFullDuplex_set             , dal_bumblebee_oob_forceFullDuplex_set);
    F_00000U(pMapper->oob[0].forceSpeed_get                  , dal_bumblebee_oob_forceSpeed_get);
    F_00000U(pMapper->oob[0].forceSpeed_set                  , dal_bumblebee_oob_forceSpeed_set);
    F_00000U(pMapper->oob[0].forceLink_get                   , dal_bumblebee_oob_forceLink_get);
    F_00000U(pMapper->oob[0].forceLink_set                   , dal_bumblebee_oob_forceLink_set);
    F_00000U(pMapper->oob[0].stat_getAll                     , dal_bumblebee_oob_stat_getAll);
    F_00000U(pMapper->oob[0].miiBus_read                     , dal_bumblebee_oob_miiBus_read);
    F_00000U(pMapper->oob[0].miiBus_write                    , dal_bumblebee_oob_miiBus_write);
    F_00000U(pMapper->oob[0].sdsReg_get                      , dal_bumblebee_oob_sdsReg_get);
    F_00000U(pMapper->oob[0].sdsReg_set                      , dal_bumblebee_oob_sdsReg_set);
    F_00000U(pMapper->oob[0].phy_connect                     , dal_common_oob_phy_connect);
    F_00000U(pMapper->oob[0].linkMon_enable                  , dal_common_oob_linkMon_enable);
    F_00000U(pMapper->oob[0].linkMon_disable                 , dal_common_oob_linkMon_disable);
    F_00000U(pMapper->oob[0].linkMon_register                , dal_common_oob_linkMon_register);
    F_00000U(pMapper->oob[0].linkMon_unregister              , dal_common_oob_linkMon_unregister);
    F_00000U(pMapper->oob[0].linkMonSwScanEnable_get         , dal_common_oob_linkMon_swScanEnable_get);
    F_00000U(pMapper->oob[0].linkMonSwScanEnable_set         , dal_common_oob_linkMon_swScanEnable_set);
    F_00000U(pMapper->oob[0].txData_set                      , dal_common_oob_txData_set);
    F_00000U(pMapper->oob[0].diagPkt_send                    , dal_common_oob_diagPkt_send);

    F_00000U(pMapper->oob[0].pkt_free                        , dal_bumblebee_oob_pkt_free);
    F_00000U(pMapper->oob[0].pkt_alloc                       , dal_bumblebee_oob_pkt_alloc);
    F_00000U(pMapper->oob[0].reset                           , dal_bumblebee_oob_reset);
    F_00000U(pMapper->oob[0].rx_register                     , dal_bumblebee_oob_rx_register);
    F_00000U(pMapper->oob[0].rx_unregister                   , dal_bumblebee_oob_rx_unregister);
    F_00000U(pMapper->oob[0]._init                           , dal_bumblebee_oob_init_gmac);
    F_00000U(pMapper->oob[0]._exit                           , _dal_bumblebee_oob_exit);
    F_00000U(pMapper->oob[0]._enable_set                     , _dal_bumblebee_oob_enable_set);
    F_00000U(pMapper->oob[0]._nic_intr_enable_set            , _dal_bumblebee_oob_nic_intr_enable_set);
    F_00000U(pMapper->oob[0]._nic_init                       , dal_bumblebee_oob_nic_init);
    F_00000U(pMapper->oob[0]._nic_sw_dataBase_init           , dal_bumblebee_oob_nic_sw_dataBase_init);
    F_00000U(pMapper->oob[0]._nic_ring_init                  , dal_bumblebee_oob_nic_ring_init);
    F_00000U(pMapper->oob[0]._rx_register                    , _dal_bumblebee_oob_rx_register);
    F_00000U(pMapper->oob[0]._rx_unregister                  , _dal_bumblebee_oob_rx_unregister);
    F_00000U(pMapper->oob[0].desc_init                       , dal_bumblebee_oob_desc_init);
    F_00000U(pMapper->oob[0].pkt_tx                          , dal_bumblebee_oob_pkt_tx);
#endif
    /* Hook callback for Kernel-Mode kernel space & User-Mode user space & User-Mode kernel space */
}
#endif /*CONFIG_SDK_DRIVER_NIC*/

void drv_bumblebeeMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);

#ifndef __BOOTLOADER__
    drv_bumblebee_intrMapper_init(pMapper);
#endif /*__BOOTLOADER__*/
    drv_bumblebee_gpioMapper_init(pMapper);
    drv_bumblebee_tcMapper_init(pMapper);
    drv_bumblebee_watchdogMapper_init(pMapper);

#if defined(CONFIG_SDK_DRIVER_NIC)
    drv_bumblebee_oobMapper_init(pMapper);
#endif /*CONFIG_SDK_DRIVER_NIC*/
}

void drv_swcore_bumblebeeMapper_init(drv_mapper_t *pMapper)
{
    F_00000U(pMapper->drv_init, drv_common_init);

    drv_bumblebee_i2cMapper_init(pMapper);
    drv_bumblebee_rtl8231Mapper_init(pMapper);
#if defined(CONFIG_SDK_DRIVER_NIC)
    drv_bumblebee_nicMapper_init(pMapper);
#endif /*CONFIG_SDK_DRIVER_NIC*/
}
#endif

drv_mapper_t* _drv_mgmt_mapper_allocate(uint32 unit)
{
    drv_mapper_t    *pMapper = NULL;
    uintptr         *fP;
    uint32          i = 0;

    pMapper = (drv_mapper_t *)osal_alloc(sizeof(drv_mapper_t));
    if (NULL == pMapper)
        return NULL;

    /* init all callback of mapper to a default function */
    fP = (uintptr *)pMapper;

    for (i = 0; i < sizeof(drv_mapper_t) / sizeof(void *); i++,  fP++) *fP = (uintptr)(drv_common_unavail);

    return pMapper;
}


/* Function Name:
 *      drv_mgmt_mapper_hook
 *
 * Description:
 *      Hook related mapper initialized function and execute it.
 * Input:
 *      unit    - the unit to be initialized
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - SUCCESSED to find mapper and initial for this chip
 *      RT_ERR_FAILED   - FAILED to find mapper and initial for this chip
 * Note:
 *      None
 */
static int32
drv_mgmt_mapper_hook(uint32 unit, drv_mapper_t *pMapper, drv_mgmt_bsp_type_t bsp_type)
{

    pDrvMapper_init_t drv_mapper_init = NULL;

    RT_LOG(LOG_DEBUG, MOD_INIT, "[%s]chip family = 0x%X, bsp_type:%d\n", __FUNCTION__, RTK_CHIP_PHYINFO(unit).chip_family, bsp_type);

    switch (RTK_CHIP_PHYINFO(unit).chip_family)
    {
#if defined(CONFIG_SDK_RTL8380)
        case RTL8380_FAMILY_ID:
        case RTL8330_FAMILY_ID:
            drv_mapper_init = (bsp_type == DRV_TYPE_SOC) ? drv_mapleMapper_init : drv_swcore_mapleMapper_init;
            RT_DRV_REG_TYPE(unit) = DRV_REG_8380;
            break;
#endif

#if defined(CONFIG_SDK_RTL8390)
        case RTL8390_FAMILY_ID:
        case RTL8350_FAMILY_ID:
            drv_mapper_init = (bsp_type == DRV_TYPE_SOC) ? drv_cypressMapper_init : drv_swcore_cypressMapper_init;
            RT_DRV_REG_TYPE(unit) = DRV_REG_8390;
            break;
#endif

#if defined(CONFIG_SDK_RTL9300)
        case RTL9300_FAMILY_ID:
            drv_mapper_init = (bsp_type == DRV_TYPE_SOC) ? drv_longanMapper_init : drv_swcore_longanMapper_init;
            RT_DRV_REG_TYPE(unit) = DRV_REG_9300;
            break;
#endif

#if defined(CONFIG_SDK_RTL9310)
        case RTL9310_FAMILY_ID:
            drv_mapper_init = (bsp_type == DRV_TYPE_SOC) ? drv_mangoMapper_init : drv_swcore_mangoMapper_init;
            RT_DRV_REG_TYPE(unit) = DRV_REG_9310;
            break;
#endif

#if defined(CONFIG_SDK_RTL9311B)
        case RTL9311B_FAMILY_ID:
            drv_mapper_init = (bsp_type == DRV_TYPE_SOC) ? drv_tigersharkMapper_init : drv_swcore_tigersharkMapper_init;
            RT_DRV_REG_TYPE(unit) = DRV_REG_9311B;
            break;
#endif

#if defined(CONFIG_SDK_RTL9330)
        case RTL9330_FAMILY_ID:
            drv_mapper_init = (bsp_type == DRV_TYPE_SOC) ? drv_bumblebeeMapper_init : drv_swcore_bumblebeeMapper_init;
            RT_DRV_REG_TYPE(unit) = DRV_REG_9330;
            break;
#endif

        default:
            drv_mapper_init = NULL;
            return RT_ERR_FAILED;
    }

    /*drv mapper init.*/
    drv_mapper_init(pMapper);
    RT_DRV_MGMT(unit)->init_status = INIT_COMPLETED;

    return RT_ERR_OK;
}

/* Function Name:
 *      drv_mgmt_mapper_init
 * Description:
 *      Find mapper of this unit, or allocate and hook related driver for it.
 * Input:
 *      unit         - the unit to be initialized
 * Output:
 *      None
 * Return:
 *      NULL         - FAILED to find mapper for this chip
 *      dal_mapper_t - SUCCESSED to find mapper for this chip
 * Note:
 */
drv_mapper_t*
drv_mgmt_mapper_init(uint32 unit, drv_mgmt_bsp_type_t bsp_type)
{
    int32  ret = RT_ERR_OK;

    if( INIT_COMPLETED != drv_mgmt_init_state )
    {
        RT_INIT_MSG("Error: drv_mgmt_init not initialized!\n");
        return NULL;
    }

    if (NULL == mgmt_drv_mapper[unit])
    {
        mgmt_drv_mapper[unit] = _drv_mgmt_mapper_allocate(unit);
        if (NULL == mgmt_drv_mapper[unit])
        {
            RT_INIT_ERR(RT_ERR_FAILED, MOD_RTCORE, "mgmt_drv_mapper allocate failed! (unit %d)", unit);
            return NULL;
        }
    }

    if ((ret = drv_mgmt_mapper_hook(unit,  mgmt_drv_mapper[unit], bsp_type)) != RT_ERR_OK)
    {
        osal_free(mgmt_drv_mapper[unit]);
        RT_INIT_ERR(ret, MOD_RTCORE, "drv_mgmt_mapper_hook failed! (unit %d, bsp_type:%d, chip_family_id:0x%X)",
                    unit, bsp_type, RTK_CHIP_PHYINFO(unit).chip_family);

        return NULL;
    }

    return mgmt_drv_mapper[unit];
}

/* Function Name:
 *      drv_mgmt_init
 * Description:
 *      Initilize drv database
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *      drv must call this function before do other kind of action.
 */
int32 drv_mgmt_init(void)
{
    int unit;

    /* Initialize the MGMT database */
    osal_memset(pDrvMgmt_node, 0, sizeof(pDrvMgmt_node));
    osal_memset(mgmt_drv_mapper, 0, sizeof(mgmt_drv_mapper));

    for(unit=0;unit<RTK_MAX_NUM_OF_UNIT;unit++)
    {
        /* Allocate memory for device database and initilize it. */
        pDrvMgmt_node[unit] = (drv_mgmt_info_t *)osal_alloc(sizeof(drv_mgmt_info_t));
        if (!pDrvMgmt_node[unit]){
            RT_LOG(LOG_DEBUG, MOD_BSP, "failed to create drv management node");
            return RT_ERR_FAILED;
        }
        osal_memset(pDrvMgmt_node[unit], 0, sizeof(drv_mgmt_info_t));
    }

    drv_mgmt_init_state   = INIT_COMPLETED;

    return RT_ERR_OK;
}

/* Function Name:
 *      drv_mgmt_exit
 * Description:
 *      Deinitilize drv database
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - exit fail
 *      RT_ERR_OK       - exit success
 * Note:
 *      [SDK_4.0.2]
 */
int32 drv_mgmt_exit(void)
{
    int unit;
    drv_mgmt_init_state   = INIT_NOT_COMPLETED;

    for(unit=0;unit<RTK_MAX_NUM_OF_UNIT;unit++)
    {
         if (pDrvMgmt_node[unit])
         {
             osal_free(pDrvMgmt_node[unit]);
         }
    }
    osal_memset(pDrvMgmt_node, 0, sizeof(pDrvMgmt_node));
    osal_memset(mgmt_drv_mapper, 0, sizeof(mgmt_drv_mapper));
    return RT_ERR_OK;
}

/* Function Name:
 *      drv_mgmt_phy_probe
 * Description:
 *      Probe PHY and initial mapper database
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *      None
 */
int32
drv_mgmt_phy_probe(uint32 unit, uint32 port)
{
    int     i, size;
    int32   ret = RT_ERR_PHY_NOT_MATCH;
    rt_chip_family_index_t chipFamilyIndex;
    drv_mapper_t *pMapper;

    RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB unit=%u port=%u PHY probe\n", unit, port);

    if (RT_ERR_OK != rt_chip_familyIndex_get(unit, &chipFamilyIndex))
    {
        RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB unit=%u port=%u PHY probe - mapper is NULL. chipFamilyIndex get fail\n", unit, port);
        return RT_ERR_CHIP_NOT_FOUND;
    }
    if ((pMapper = mgmt_drv_mapper[unit]) == NULL)
    {
        RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB unit=%u port=%u PHY probe - mapper is NULL. chipFamilyIndex=%u\n", unit, port, chipFamilyIndex);
        return RT_ERR_FAILED;
    }

    size = sizeof(drv_mgmt_oobPhyDrv_list) / sizeof(drv_mgmt_oobPhyCtrl_t);
    for (i = size - 1; i >= 0; i--)
    {
        if ((ret = (drv_mgmt_oobPhyDrv_list[i].chk_func)(unit, port, drv_mgmt_oobPhyDrv_list[i].phy_model_id, drv_mgmt_oobPhyDrv_list[i].phy_rev_id)) == RT_ERR_OK)
        {
            RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB unit=%u port=%u PHY probed model_id=%u\n", unit, port, drv_mgmt_oobPhyDrv_list[i].phy_model_id);
            drv_mgmt_oobPhyDrv_list[i].mapperInit_func(pMapper, unit, port);
            return RT_ERR_OK;
        }
    }

    RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB unit=%u port=%u PHY probed - no match driver.\n", unit, port);
    return ret;
}


