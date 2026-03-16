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
 * $Revision: 80415 $
 * $Date: 2017-07-06 18:58:48 +0800 (Thu, 06 Jul 2017) $
 *
 * Purpose : Realtek Switch SDK Rtdrv Netfilter Module.
 *
 * Feature : Realtek Switch SDK Rtdrv Netfilter Module
 *
 */

/*
 * Include Files
 */
#include <asm/uaccess.h>
#include <linux/netfilter.h>
#include <common/rt_autoconf.h>
#include <common/rt_error.h>
#include <common/debug/mem.h>
#include <osal/print.h>
#include <hal/mac/mem.h>
#include <ioal/mem32.h>
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <drv/watchdog/watchdog.h>
#include <rtdrv/ext/rtdrv_netfilter_ext_9310.h>
#include <osal/memory.h>
#if (defined(CONFIG_SDK_DRIVER_TEST) || defined(CONFIG_SDK_DRIVER_TEST_MODULE))
#include <sdk/sdk_test.h>
#endif
#include <dal/mango/dal_mango_l2.h>
#include <hal/mac/reg.h>
#include <hal/common/halctrl.h>
#include <hal/chipdef/mango/rtk_mango_reg_struct.h>
#include <hal/chipdef/mango/rtk_mango_table_struct.h>
#ifdef CONFIG_SDK_MODEL_MODE
#include <model_comm.h>
#include <tc.h>
#include <virtualmac/vmac_target.h>
#include <osal/time.h>
#endif
#include <rtk/trunk.h>

/*
 * Symbol Definition
 */
#define EXT_PACKET_HDR_LEN  60
#define PKTGEN_STREAM_NUM   2

/*
 * Data Declaration
 */
drv_nic_pkt_t *pDiagExtPacket;
uint8 pDiagExtPacketHdr[52][PKTGEN_STREAM_NUM][EXT_PACKET_HDR_LEN];	/* enough to comprise ethernet, IP and TCP header */
uint8 include_vlan_header[52][PKTGEN_STREAM_NUM];
uint8 spg_payload_type[52][PKTGEN_STREAM_NUM];
uint32 spg_payload_pattern[52][PKTGEN_STREAM_NUM];
uint32 oTag[52][PKTGEN_STREAM_NUM];
static uint32 linkMon_wake_up;

/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */

static void
_nic_tx_callback(uint32 unit, drv_nic_pkt_t *pPacket, void *pCookie)
{
    if (pPacket == NULL)
    {
        goto _exit;
    }
    osal_free(pCookie);
    osal_free(pPacket);

_exit:
    return;
}

#ifdef CONFIG_SDK_FPGA_PLATFORM
static int32
_fpga_info_get(uint32 unit, uint32 *pRtl, uint32 *pDate, uint32 *pTime, uint32 *pVersion)
{
    reg_array_field_read(unit, MANGO_ACL_RSVDr, REG_ARRAY_INDEX_NONE, 0, MANGO_DUMY_12f, pRtl);
    reg_array_field_read(unit, MANGO_ACL_RSVDr, REG_ARRAY_INDEX_NONE, 1, MANGO_DUMY_12f, pDate);
    reg_array_field_read(unit, MANGO_ACL_RSVDr, REG_ARRAY_INDEX_NONE, 2, MANGO_DUMY_12f, pTime);
    reg_array_field_read(unit, MANGO_ACL_RSVDr, REG_ARRAY_INDEX_NONE, 3, MANGO_DUMY_12f, pVersion);

    return RT_ERR_OK;
}


#define FPGA_REG_WRITE(_unit, _reg, _val)   \
do {                                    \
    uint32 __value = _val;              \
    uint32 __check;                     \
    reg_write(_unit, _reg, &__value);   \
    reg_read(_unit, _reg, &__check);    \
    if (__value != __check) osal_printf("write 0x%08X, read-back 0x%08X\n", __value, __check); \
} while (0)

#define FPGA_REG_FIELD_WRITE(_unit, _reg, _idx1, _idx2, _field, _val)   \
do {                                                                    \
    uint32 __value = _val;                                              \
    uint32 __check;                                                     \
    reg_array_field_write(_unit, _reg, _idx1, _idx2, _field, &__value); \
    reg_array_field_read(_unit, _reg, _idx1, _idx2, _field, &__check);  \
    if (__value != __check) osal_printf("write 0x%08X, read-back 0x%08X\n", __value, __check); \
} while (0)

#define FPGA_REG_SET(_unit, _reg, _idx1, _idx2, _val)   \
do {                                                        \
    uint32 __value = _val;                                  \
    uint32 __check;                                         \
    reg_array_write(_unit, _reg, _idx1, _idx2, &__value);   \
    reg_array_read(_unit, _reg, _idx1, _idx2, &__check);    \
    if (__value != __check) osal_printf("write 0x%08X, read-back 0x%08X\n", __value, __check); \
} while (0)

static int32
_fpga_init(uint32 unit, uint32 fpgaVer)
{
    uint32 i, val, idx;
    rtk_port_t  port;

    /* CPU port simulation */
    if (fpgaVer == 9)
    {
        osal_printf("enable CPU port simulation\n");
        FPGA_REG_WRITE(unit, MANGO_GLB_DEBUG_SELECTr, 0x40000000);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_L2_GLOBAL_CTRL2r, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_EXT_CPU_ENf, 0x1);

        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_RX_PAUSE_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_TX_PAUSE_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_SPD_SELf, 0x2);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_DUP_SELf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_RLFD_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FEFI_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MSTR_SLV_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_EEE_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FC_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_SPD_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_DUP_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MEDIA_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, 0x1);

        rtk_vlan_port_add(unit, 1, 56, TRUE);

        FPGA_REG_WRITE(unit, MANGO_FC_CPU_Q_EGR_FORCE_DROP_CTRLr, 0x0);

        return RT_ERR_OK;
    } else if (fpgaVer == 10)
    {
        osal_printf("disable CPU port simulation\n");
        FPGA_REG_WRITE(unit, MANGO_GLB_DEBUG_SELECTr, 0x0);

        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_RX_PAUSE_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_TX_PAUSE_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_SPD_SELf, 0x2);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_DUP_SELf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_RLFD_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FEFI_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MSTR_SLV_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_EEE_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FC_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_SPD_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_DUP_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MEDIA_ENf, 0x1);
        FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, 0x1);

        rtk_vlan_port_del(unit, 1, 56);

        FPGA_REG_WRITE(unit, MANGO_FC_CPU_Q_EGR_FORCE_DROP_CTRLr, 0x1);

        return RT_ERR_OK;
    }

    osal_printf("Reset table (by using init reg)\n");
    FPGA_REG_FIELD_WRITE(unit, MANGO_MEM_IGR_INITr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_LINK_LIST_INITf, 0x1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MEM_ENCAP_INITr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_MEM_INITf, 0x1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MEM_EGR_INITr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_MEM_INITf, 0x1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MEM_MIB_INITr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_MEM_RSTf, 0x1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MEM_ACL_INITr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_MEM_INITf, 0x1);
    FPGA_REG_WRITE(unit, MANGO_MEM_ALE_INIT_0r, 0xffffffff);
    FPGA_REG_WRITE(unit, MANGO_MEM_ALE_INIT_1r, 0xffffffff);
    FPGA_REG_WRITE(unit, MANGO_MEM_ALE_INIT_2r, 0xffffffff);
    /* reset learning counter */
    val = 0;
    reg_field_write(unit, MANGO_L2_LRN_CONSTRT_CNTr, MANGO_LRN_CNTf, &val);
    HWP_ETHER_PORT_TRAVS(unit, port)
        reg_array_field_write(unit, MANGO_L2_LRN_PORT_CONSTRT_CNTr, port, REG_ARRAY_INDEX_NONE, MANGO_LRN_CNTf, &val);
    for (idx = 0; idx < HAL_L2_FID_LEARN_LIMIT_ENTRY_MAX(unit); idx++)
        reg_array_field_write(unit, MANGO_L2_LRN_VLAN_CONSTRT_CNTr, REG_ARRAY_INDEX_NONE, idx, MANGO_LRN_CNTf, &val);


    osal_printf("Reset extra HSA memory\n");
    FPGA_REG_FIELD_WRITE(unit, MANGO_MEM_EGR_CTRLr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_LINK_INITf, 1);
    //osal_time_mdelay(100);  /* wait for 100mS */

    /* set EXT_CPU_EN enable, so that packet won't pile up and causing egress drop */
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_L2_GLOBAL_CTRL2r, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_EXT_CPU_ENf, 0x1);

    if (fpgaVer >= 1)
    {
        /* FE patch */
        osal_printf("FPGA patch (FE)\n");

        if (fpgaVer == 2)
        {
            osal_printf("System clock has been changed to 325Mhz\n");
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_L2_GLOBAL_CTRL2r, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_SYS_CLK_SELf, 0x1);

            // QoS T, B value (for FE version only)
            FPGA_REG_SET(unit, MANGO_IGBW_LB_CTRLr,         REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x00350156);
            FPGA_REG_SET(unit, MANGO_EGBW_LB_CTRLr,         REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x00350156);
            FPGA_REG_SET(unit, MANGO_EGBW_CPU_PPS_LB_CTRLr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x00013601);
            FPGA_REG_SET(unit, MANGO_WFQ_LB_CTRLr,          REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x080102ac);
            FPGA_REG_SET(unit, MANGO_STORM_LB_CTRLr,        REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x00350156);
            FPGA_REG_SET(unit, MANGO_STORM_LB_PPS_CTRLr,    REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x000004D1);
            FPGA_REG_SET(unit, MANGO_STORM_LB_PROTO_CTRLr,  REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x0009AF8E);
            FPGA_REG_SET(unit, MANGO_METER_BYTE_TB_CTRLr,   REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x00350156);
            FPGA_REG_SET(unit, MANGO_METER_PKT_TB_CTRLr,    REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x01360001);
        }
        else
        {
            osal_printf("System clock has been changed to 175Mhz\n");
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_L2_GLOBAL_CTRL2r, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, MANGO_SYS_CLK_SELf, 0x2);

            // QoS T, B value (for FE version only)
            FPGA_REG_SET(unit, MANGO_IGBW_LB_CTRLr,         REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x003e02e7);
            FPGA_REG_SET(unit, MANGO_EGBW_LB_CTRLr,         REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x003e02e7);
            FPGA_REG_SET(unit, MANGO_EGBW_CPU_PPS_LB_CTRLr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x0000a701);
            FPGA_REG_SET(unit, MANGO_WFQ_LB_CTRLr,          REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x08010744);
            FPGA_REG_SET(unit, MANGO_STORM_LB_CTRLr,        REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x003e02e7);
            FPGA_REG_SET(unit, MANGO_STORM_LB_PPS_CTRLr,    REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x000002a1);
            FPGA_REG_SET(unit, MANGO_STORM_LB_PROTO_CTRLr,  REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x00053725);
            FPGA_REG_SET(unit, MANGO_METER_BYTE_TB_CTRLr,   REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x003e02e7);
            FPGA_REG_SET(unit, MANGO_METER_PKT_TB_CTRLr,    REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x00a70001);

            // Write Egress Port Burst Size
            for (i=0; i<=53; i++)
            {
                if ((4 <= i) && (i <= 51)) { continue; }
                FPGA_REG_FIELD_WRITE(unit, MANGO_EGBW_PORT_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_BURSTf, 0x1B58);
            }
        }

        // force port 0~3 FC_EN/HALF_DUPLEX/100M/LINK_UP
        // force port 1 FC_EN/HALF_DUPLEX/100M/LINK_UP
        // force port 2 FC_EN/HALF_DUPLEX/100M/LINK_UP
        // force port 3 FC_EN/HALF_DUPLEX/100M/LINK_UP
        for (i=0; i<=3; i++)
        {
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MEDIA_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_DUP_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_SPD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FC_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_EEE_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MSTR_SLV_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FEFI_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_RLFD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_DUP_SELf, 0);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_SPD_SELf, 0x1);
        }

        // force port 52 FC_EN/FULL_DUPLEX/1000M/LINK_UP
        // force port 53 FC_EN/FULL_DUPLEX/1000M/LINK_UP
        for (i=52; i<=53; i++)
        {
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MEDIA_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_DUP_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_SPD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FC_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_EEE_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MSTR_SLV_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FEFI_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_RLFD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, 1);
        }
    }
    else
    {
        /* L2/L3 patch */
        osal_printf("FPGA patch (L2/L3)\n");

        // force port 0 FC_EN/FULL_DUPLEX/1000M/LINK_UP
        // force port 1 FC_EN/FULL_DUPLEX/1000M/LINK_UP
        // force port 2 FC_EN/FULL_DUPLEX/1000M/LINK_UP
        // force port 3 FC_EN/FULL_DUPLEX/1000M/LINK_UP
        for (i=0; i<=3; i++)
        {
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MEDIA_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_DUP_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_SPD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FC_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_EEE_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MSTR_SLV_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FEFI_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_RLFD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, 1);
        }

        // force port 52 FC_EN/FULL_DUPLEX/10G/LINK_UP
        // force port 53 FC_EN/FULL_DUPLEX/10G/LINK_UP
        for (i=52; i<=53; i++)
        {
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MEDIA_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_DUP_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_SPD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FC_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_EEE_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_MSTR_SLV_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FEFI_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_RLFD_ENf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, 1);
            FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, i, REG_ARRAY_INDEX_NONE, MANGO_SMI_SPD_SELf, 0x4);
        }
    }

    // VLAN
    for (i = 0; i <= 55; i++)
        rtk_vlan_port_add(unit, 1, i, TRUE);

    // Flow Contrl
    // adjust flow control threshold to fit in FPGA
    // set FC_GLB_DROP_THR, FPGA packet buffer has 730 page in total
    FPGA_REG_SET(unit, MANGO_FC_GLB_DROP_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x2da);
    // set FC_GLB_HI_THR (ON, OFF) = (434, 428)
    FPGA_REG_SET(unit, MANGO_FC_GLB_HI_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x1B201AC);
    // set FC_GLB_LO_THR (ON, OFF) = (301, 295)
    FPGA_REG_SET(unit, MANGO_FC_GLB_LO_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x12D0127);
    // set FC_GLB_FCOFF_HI_THR (ON, OFF) = (434, 428)
    FPGA_REG_SET(unit, MANGO_FC_GLB_FCOFF_HI_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x1B201AC);
    // set FC_GLB_FCOFF_LO_THR (ON, OFF) = (301, 295)
    FPGA_REG_SET(unit, MANGO_FC_GLB_FCOFF_LO_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x12D0127);
    // set FC_GLB_SYS_UTIL_THR
    FPGA_REG_SET(unit, MANGO_FC_GLB_SYS_UTIL_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0);

    // set FC_JUMBO_HI_THR (ON, OFF) = (434, 428)
    FPGA_REG_SET(unit, MANGO_FC_JUMBO_HI_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x1B201AC);
    // set FC_JUMBO_LO_THR (ON, OFF) = (301, 295)
    FPGA_REG_SET(unit, MANGO_FC_JUMBO_LO_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x12D0127);
    // set FC_JUMBO_FCOFF_HI_THR (ON, OFF) = (434, 428)
    FPGA_REG_SET(unit, MANGO_FC_JUMBO_FCOFF_HI_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x1B201AC);
    // set FC_JUMBO_FCOFF_LO_THR (ON, OFF) = (301, 295)
    FPGA_REG_SET(unit, MANGO_FC_JUMBO_FCOFF_LO_THRr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x12D0127);

    // set RX groups 0~2 of FC_PORT_HI_THR (ON, OFF) = (38, 31)
    // set RX groups 0~2 of FC_PORT_LO_THR (ON, OFF) = (3, 2)
    // set RX groups 0~2 of FC_PORT_FCOFF_HI_THR (ON, OFF) = (38, 31)
    // set RX groups 0~2 of FC_PORT_FCOFF_LO_THR (ON, OFF) = (3, 2)
    // set RX groups 0~2 of FC_PORT_GUAR_THR = (3)
    for (i=0; i<=2; i++)
    {
        FPGA_REG_SET(unit, MANGO_FC_PORT_HI_THRr, REG_ARRAY_INDEX_NONE, i, 0x26001f);
        FPGA_REG_SET(unit, MANGO_FC_PORT_LO_THRr, REG_ARRAY_INDEX_NONE, i, 0x14000f);
        FPGA_REG_SET(unit, MANGO_FC_PORT_FCOFF_HI_THRr, REG_ARRAY_INDEX_NONE, i, 0x26001f);
        FPGA_REG_SET(unit, MANGO_FC_PORT_FCOFF_LO_THRr, REG_ARRAY_INDEX_NONE, i, 0x14000f);
        FPGA_REG_SET(unit, MANGO_FC_PORT_GUAR_THRr, REG_ARRAY_INDEX_NONE, i, 0x3);
    }
    // for Jumbo mode
    // set RX groups 3 of FC_PORT_HI_THR (ON, OFF) = (100, 60)
    // set RX groups 3 of FC_PORT_LO_THR (ON, OFF) = (42, 12)
    // set RX groups 3 of FC_PORT_FCOFF_HI_THR (ON, OFF) = (100, 60)
    // set RX groups 3 of FC_PORT_FCOFF_LO_THR (ON, OFF) = (42, 12)
    // set RX groups 3 of FC_PORT_GUAR_THR = (6)
    for (i=3; i<=3; i++)
    {
        FPGA_REG_SET(unit, MANGO_FC_PORT_HI_THRr, REG_ARRAY_INDEX_NONE, i, 0x64003c);
        FPGA_REG_SET(unit, MANGO_FC_PORT_LO_THRr, REG_ARRAY_INDEX_NONE, i, 0x2a000c);
        FPGA_REG_SET(unit, MANGO_FC_PORT_FCOFF_HI_THRr, REG_ARRAY_INDEX_NONE, i, 0x64003c);
        FPGA_REG_SET(unit, MANGO_FC_PORT_FCOFF_LO_THRr, REG_ARRAY_INDEX_NONE, i, 0x2a000c);
        FPGA_REG_SET(unit, MANGO_FC_PORT_GUAR_THRr, REG_ARRAY_INDEX_NONE, i, 0x6);
    }

    // set egress queue drop theshold of queue 0~3 in group 0~1 FC_Q_EGR_DROP_THR (ON, OFF) = (78, 68)
    for (i=0; i<=1; i++)
    {
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 0, i, 0x4e0044);
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 1, i, 0x4e0044);
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 2, i, 0x4e0044);
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 3, i, 0x4e0044);
    }
    // for Jumbo mode
    // set egress queue drop theshold of queue 0~3 in group 2 FC_Q_EGR_DROP_THR (ON, OFF) = (88, 58)
    for (i=2; i<=2; i++)
    {
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 0, i, 0x58003a);
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 1, i, 0x58003a);
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 2, i, 0x58003a);
        FPGA_REG_SET(unit, MANGO_FC_Q_EGR_DROP_THRr, 3, i, 0x58003a);
    }

    // set CPU port egress queue drop theshold FC_CPU_Q_EGR_DROP_THR (ON, OFF) = (78, 68)
    FPGA_REG_SET(unit, MANGO_FC_CPU_Q_EGR_DROP_THRr, REG_ARRAY_INDEX_NONE, 0, 0x4e0044);
    FPGA_REG_SET(unit, MANGO_FC_CPU_Q_EGR_DROP_THRr, REG_ARRAY_INDEX_NONE, 1, 0x4e0044);
    FPGA_REG_SET(unit, MANGO_FC_CPU_Q_EGR_DROP_THRr, REG_ARRAY_INDEX_NONE, 2, 0x4e0044);
    FPGA_REG_SET(unit, MANGO_FC_CPU_Q_EGR_DROP_THRr, REG_ARRAY_INDEX_NONE, 3, 0x4e0044);

    // Jumbo mode setting - Enable Jumbo mode, packet length threshold = 2000B, exit jumbo mode page threshold = 6 pages.
    FPGA_REG_SET(unit, MANGO_FC_JUMBO_THR_ADJUSTr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0x87D00006);

    // sprintf 'port set port all state enable'

    // set CPU port Tx/Rx enable
    // sprintf 'reg set 0x7c00 0x3'
    //FPGA_REG_SET(unit, MANGO_MAC_L2_PORT_CTRLr, 56, REG_ARRAY_INDEX_NONE, 0x3);
    // CPU port Force link-up
    // sprintf 'reg set 0xdf8 0x1AA01'
    FPGA_REG_SET(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, 0x32A01);

    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, 1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_DUP_ENf, 1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_SPD_ENf, 1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_FC_ENf, 1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, 1);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_SPD_SELf, 0x2);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_RX_PAUSE_ENf, 0);
    FPGA_REG_FIELD_WRITE(unit, MANGO_MAC_FORCE_MODE_CTRLr, 56, REG_ARRAY_INDEX_NONE, MANGO_SMI_TX_PAUSE_ENf, 0);

    // Force CPU port egress queue drop when congestion
    // sprintf 'reg set 0xa9c8 0x1'
    FPGA_REG_SET(unit, MANGO_FC_CPU_Q_EGR_FORCE_DROP_CTRLr, REG_ARRAY_INDEX_NONE, 0, 0x1);

    // set ingress queue drop theshold of ingress queue 0~2 for all port (ON, OFF) = (220, 210)
    FPGA_REG_SET(unit, MANGO_IGBW_Q_DROP_THRr, REG_ARRAY_INDEX_NONE, 0, 0xDC00D2);
    FPGA_REG_SET(unit, MANGO_IGBW_Q_DROP_THRr, REG_ARRAY_INDEX_NONE, 1, 0xDC00D2);
    FPGA_REG_SET(unit, MANGO_IGBW_Q_DROP_THRr, REG_ARRAY_INDEX_NONE, 2, 0xDC00D2);

    // QoS Scheduling - Set queue empty threshold to be 0
    FPGA_REG_SET(unit, MANGO_SCHED_CTRLr, REG_ARRAY_INDEX_NONE, REG_ARRAY_INDEX_NONE, 0);

    return RT_ERR_OK;
}

uint32 _zigzag(uint32 ori, uint32 index)
{
    uint32 data = 0, mod;

    mod = index % 32;
    if (mod > 16)
        mod = 32 - mod;

    data = ((ori << 1) & (0xffff << (mod + 1))) | (ori & ((1 << mod) - 1));

    return data & 0x1ffff;
}

uint32 _zigzagReverse(uint32 zData, uint32 index)
{
    uint32 data = 0, mod;

    mod = index % 32;
    if (mod > 16)
        mod = 32 - mod;

    data = ((zData >> (mod + 1)) << mod) | (zData & ((1 << mod) - 1));

    return data & 0xffff;
}

int32 _mdx_read(uint32 unit, uint32 reg, uint32 *pValue)
{
    uint32 tmp = 0, cnt = 0, data;

    data = ((reg & 0x1f) << 7) | 0x9;
    reg_write(unit, MANGO_TEST_MDX_CTRLr, &data);
    do
    {
        reg_field_read(unit, MANGO_TEST_MDX_CTRLr, MANGO_MDX_REQf, &tmp);
        cnt++;
    } while (tmp != 0 && cnt < 1000);

    reg_field_read(unit, MANGO_TEST_MDX_CTRLr, MANGO_MDX_NO_TAf, &tmp);
    if (tmp)
        return RT_ERR_FAILED;

    reg_read(unit, MANGO_TEST_MDX_DATAr, pValue);

    return RT_ERR_OK;
}

int32 _mdx_write(uint32 unit, uint32 reg, uint32 value)
{
    uint32 tmp = 0, cnt = 0, data;

    data = value & 0xffff;
    reg_write(unit, MANGO_TEST_MDX_DATAr, &data);
    data = ((reg & 0x1f) << 7) | 0xb;
    reg_write(unit, MANGO_TEST_MDX_CTRLr, &data);
    do
    {
        reg_field_read(unit, MANGO_TEST_MDX_CTRLr, MANGO_MDX_REQf, &tmp);
        cnt++;
    } while (tmp != 0 && cnt < 1000);

    if (tmp != 0 && cnt >= 1000)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

static int32
_testio_set_ePhy(uint32 unit, uint32 reg, uint32 value)
{
    int32   ret;
    uint32  data = 0, cnt;


    osal_printf("%s():%d  reg:%#x  value:%#x\n", __FUNCTION__, __LINE__, reg, value);

    /* REG=0x8*/
    if ((ret = _mdx_write(unit, 0x8, value & 0xffff)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xA*/
    if ((ret = _mdx_write(unit, 0xa, 0x8000 | (reg & 0x1f))))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xC*/
    if ((ret = _mdx_write(unit, 0xc, 0xde20)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xE*/
    if ((ret = _mdx_write(unit, 0xe, 0x800f)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }

    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    /* REG=0xC*/
    if ((ret = _mdx_write(unit, 0xc, 0xde20)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xE*/
    if ((ret = _mdx_write(unit, 0xe, 0x8000)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }

    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xa, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));

    return RT_ERR_OK;
}

static int32
_testio_get_ePhy(uint32 unit, uint32 reg, uint32 *pValue)
{
    int32   ret;
    uint32  data = 0, cnt;


    //osal_printf("%s():%d  reg:%#x  \n", __FUNCTION__, __LINE__, reg);

    /* REG=0x8*/
    if ((ret = _mdx_write(unit, 0x8, 0x0)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xA*/
    if ((ret = _mdx_write(unit, 0xa, reg & 0x1f)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xC*/
    if ((ret = _mdx_write(unit, 0xc, 0xde20)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xE*/
    if ((ret = _mdx_write(unit, 0xe, 0x800f)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }

    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    /* REG=0xC*/
    if ((ret = _mdx_write(unit, 0xc, 0xde20)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    /* REG=0xE*/
    if ((ret = _mdx_write(unit, 0xe, 0x8000)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }

    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xa, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while (((data >> 15) == 0) && (cnt < 1000));


    if ((ret = _mdx_read(unit, 0x8, pValue)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }

    return RT_ERR_OK;
}

static int32
_testio_set_eFuse(uint32 unit, uint32 index, uint32 reg, uint32 value)
{
    int32   ret;
    uint32  data = 0, zData = 0, cnt;


    osal_printf("%s():%d  Efuse reg:%#x  value:%#x\n", __FUNCTION__, __LINE__, reg, value);


    /* Set Efuse sequential program mode */
    if ((ret = _mdx_write(unit, 0x8, 0x0)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xa, 0x0)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xc, 0xdd04)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x8003)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));



    /* Efuse write first 17-bit data */

    /* Write Efuse command and EPhy addr */
    data = reg | 0x80bf;
    zData = _zigzag(data, index);
    if ((ret = _mdx_write(unit, 0x8, zData & 0xffff)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    data = index | 0x4000 | ((zData >> 16) << 7);
    if ((ret = _mdx_write(unit, 0xa, data)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x800f)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x8000)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xa, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));



    /* Efuse write 2nd 17-bit data */

    /* Write Efuse data */
    data = value;
    zData = _zigzag(data, index + 1);
    if ((ret = _mdx_write(unit, 0x8, zData & 0xffff)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    data = index | 0x4000 | ((zData >> 16) << 7);
    if ((ret = _mdx_write(unit, 0xa, data)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x800f)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x8000)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xa, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));

    return RT_ERR_OK;
}

static int32
_testio_get_eFuse(uint32 unit, uint32 index, uint32 *pReg, uint32 *pValue)
{
    int32   ret;
    uint32  data = 0, zData = 0, cnt;

    //osal_printf("%s():%d  reg:%#x  \n", __FUNCTION__, __LINE__, reg);

    /* Efuse read first 17-bit data */
    if ((ret = _mdx_write(unit, 0x8, 0x0)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xa, 0x8000 | index)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x800c)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x8000)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xa, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while (((data >> 15) == 0) && (cnt < 1000));

    zData = ((data >> 7) & 0x1) << 8;

    if ((ret = _mdx_read(unit, 0x8, &data)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }

    zData |= data;
    *pReg = (_zigzagReverse(zData, index) >> 8) & 0x7f;



    /* Efuse read 2nd 17-bit data */
    if ((ret = _mdx_write(unit, 0x8, 0x0)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xa, 0x8000 | (index + 1))))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x800c)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    if ((ret = _mdx_write(unit, 0xc, 0xdd00)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    if ((ret = _mdx_write(unit, 0xe, 0x8000)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }
    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xe, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while ((data >> 15) && (cnt < 1000));


    cnt = 0;
    do
    {
        if ((ret = _mdx_read(unit, 0xa, &data)))
        {
            osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
        }
        cnt++;
    } while (((data >> 15) == 0) && (cnt < 1000));

    zData = ((data >> 7) & 0x1) << 8;

    if ((ret = _mdx_read(unit, 0x8, &data)))
    {
        osal_printf("%s():%d Failed\n", __FUNCTION__, __LINE__);
    }

    zData |= data;
    *pValue = _zigzagReverse(zData, index + 1);

    return RT_ERR_OK;
}
#endif

static int32
_sdk_tc_reset(uint32 unit)
{
    int32 ret = RT_ERR_OK;
    int32 base;
    uint32 value;
    uint32 idx;
    rtk_mcast_group_t mcast_group;
    rtk_portmask_t emptyPm;
    rtk_trk_egrPort_t trk_egr_ports;
    rtk_mirror_entry_t  mirrorEntry;
    rtk_mirror_rspanTag_t tag;
    rtk_trunk_distAlgoShift_t shift;
    rtk_l2_flushCfg_t   l2FlushCfg;
    rtk_unit_port_t unitPort;
    rtk_port_t port;
    rtk_pie_rangeCheck_ip_t         ipRng;
    rtk_pie_rangeCheck_t            range;
    rtk_acl_templateIdx_t           tmplateInfo;
    rtk_acl_clear_t                 clearInfo;

    /* reset H/W table, register */
    //osal_printf("Reset table (by using init reg)\n");
    value = 0xffffffff;
    osal_memset(&l2FlushCfg, 0, sizeof(rtk_l2_flushCfg_t));
#if 0
    reg_write(unit, MANGO_MEM_ALE_INIT_0r, &value);
    reg_write(unit, MANGO_MEM_ALE_INIT_1r, &value);
    reg_write(unit, MANGO_MEM_ALE_INIT_2r, &value);
#endif

    //osal_printf("Reset extra HSA memory\n");
    value = 1;
#if 0
    reg_field_write(unit, MANGO_MEM_EGR_CTRLr, MANGO_LINK_INITf, &value);
#endif
    //osal_time_mdelay(100);  /* wait for 100mS */

    /* reset QOS */
    for(port = 0; port < 56; port ++)
    {
        rtk_rate_portEgrBwCtrlEnable_set(unit, port, DISABLED);
        for(idx = 0; idx < 8; idx ++)
        {
            rtk_rate_portEgrQueueBwCtrlEnable_set(unit,  port, idx, DISABLED);
            rtk_rate_portEgrQueueAssuredBwCtrlEnable_set(unit,  port, idx, DISABLED);
            rtk_rate_portEgrQueueAssuredBwCtrlMode_set(unit,  port, idx, ASSURED_MODE_SHARE);
        }
        rtk_qos_port1pRemarkEnable_set(unit, port, DISABLED);
        rtk_qos_portOut1pRemarkEnable_set(unit, port, DISABLED);
        rtk_qos_portDeiRemarkEnable_set(unit, port, DISABLED);
        rtk_qos_portDscpRemarkEnable_set(unit, port, DISABLED);
    }

    /* reset Tunnel */

    /*reset L2*/

    /* reset IPMC */
    //osal_printf("Reset IPMC module\n");
    ret = rtk_ipmc_addr_delAll(unit, RTK_IPMC_FLAG_NONE);   /* IPv4 */
    if (RT_ERR_OK != ret) osal_printf("%s():%d FATAL - ret (%d) != 0\n", __FUNCTION__, __LINE__, ret);
    ret = rtk_ipmc_addr_delAll(unit, RTK_IPMC_FLAG_IPV6);   /* IPv6 */
    if (RT_ERR_OK != ret) osal_printf("%s():%d FATAL - ret (%d) != 0\n", __FUNCTION__, __LINE__, ret);

    /* reset MCAST */
    //osal_printf("Reset MCAST module\n");
    base = -1;
    ret = rtk_mcast_group_getNext(unit, RTK_MCAST_TYPE_IP, &base, &mcast_group);
    if (RT_ERR_OK != ret) osal_printf("%s():%d FATAL - ret (%d) != 0\n", __FUNCTION__, __LINE__, ret);
    while (base != -1)
    {
        ret = rtk_mcast_egrIf_delAll(unit, mcast_group);
        if (RT_ERR_OK != ret) osal_printf("%s():%d FATAL - ret (%d) != 0\n", __FUNCTION__, __LINE__, ret);
        ret = rtk_mcast_group_destroy(unit, mcast_group);
        if (RT_ERR_OK != ret) osal_printf("%s():%d FATAL - ret (%d) != 0\n", __FUNCTION__, __LINE__, ret);

        /* get next */
        ret = rtk_mcast_group_getNext(unit, RTK_MCAST_TYPE_IP, &base, &mcast_group);
        if (RT_ERR_OK != ret) osal_printf("%s():%d FATAL - ret (%d) != 0\n", __FUNCTION__, __LINE__, ret);
    }

    /* reset L3 */
    //osal_printf("Reset L3 module\n");
    ret = rtk_l3_globalCtrl_set(unit, RTK_L3_GCT_NONE, 0);
    if (RT_ERR_OK != ret) osal_printf("%s():%d - ret = 0x%x\n", __FUNCTION__, __LINE__, ret);

    /* reset BPE (802.1BR) */

    /* reset L2 */
    //osal_printf("Reset L2 module\n");
    l2FlushCfg.act = FLUSH_ACT_FLUSH_ALL_UC;
    ret = rtk_l2_ucastAddr_flush(unit, &l2FlushCfg);
    if (RT_ERR_OK != ret) osal_printf("%s():%d FATAL - ret (%d) != 0\n", __FUNCTION__, __LINE__, ret);
    /* remove L2 MC entry */
    value = 0x80000070;
    reg_write(unit, MANGO_MEM_ALE_INIT_0r, &value);
    value = 0x00000060;
    reg_write(unit, MANGO_MEM_ALE_INIT_2r, &value);
    /* reset learning counter */
    value = 0;
    reg_field_write(unit, MANGO_L2_LRN_CONSTRT_CNTr, MANGO_LRN_CNTf, &value);
    HWP_ETHER_PORT_TRAVS(unit, port)
        reg_array_field_write(unit, MANGO_L2_LRN_PORT_CONSTRT_CNTr, port, REG_ARRAY_INDEX_NONE, MANGO_LRN_CNTf, &value);
    for (idx = 0; idx < HAL_L2_FID_LEARN_LIMIT_ENTRY_MAX(unit); idx++)
        reg_array_field_write(unit, MANGO_L2_LRN_VLAN_CONSTRT_CNTr, REG_ARRAY_INDEX_NONE, idx, MANGO_LRN_CNTf, &value);

    /* reset STP */

    /* reset VLAN */
    //osal_printf("Reset VLAN module\n");
    ret = rtk_vlan_destroyAll(unit, 1);
    for(port = 0; port < 56; port ++)
    {
        rtk_vlan_portIgrVlanCnvtEnable_set(unit, port, DISABLED);
        rtk_vlan_portEgrVlanCnvtEnable_set(unit, port, DISABLED);
        rtk_vlan_portIgrVlanTransparentEnable_set(unit, port, INNER_VLAN, DISABLED);
        rtk_vlan_portIgrVlanTransparentEnable_set(unit, port, OUTER_VLAN, DISABLED);
    }

    /* reset LAG (trunk) */
    //osal_printf("Reset Trunk module\n");
    osal_memset(&emptyPm, 0, sizeof(emptyPm));
    osal_memset(&trk_egr_ports, 0, sizeof(trk_egr_ports));

    for(idx = 0; idx < 128; idx ++)
    {
        rtk_trunk_localPort_set (unit, idx, &emptyPm);
        rtk_trunk_egrPort_set(unit, idx, &trk_egr_ports);
        rtk_trunk_distributionAlgorithmTypeBind_set(unit, idx, BIND_TYPE_L2, 0);
        rtk_trunk_distributionAlgorithmTypeBind_set(unit, idx, BIND_TYPE_IPV4, 0);
        rtk_trunk_distributionAlgorithmTypeBind_set(unit, idx, BIND_TYPE_IPV6, 0);
        rtk_trunk_trafficSeparateEnable_set(unit, idx, SEPARATE_KNOWN_MULTI, DISABLED);
        rtk_trunk_trafficSeparateEnable_set(unit, idx, SEPARATE_FLOOD, DISABLED);
    }

    rtk_trunk_mode_set(unit, TRUNK_MODE_STANDALONE);

    for(idx = 0; idx < 8; idx ++)
    {
        rtk_trunk_stkTrkPort_set(unit, idx, &emptyPm);
        rtk_trunk_stkDistributionAlgorithmTypeBind_set(unit, idx, BIND_TYPE_L2, 0);
        rtk_trunk_stkDistributionAlgorithmTypeBind_set(unit, idx, BIND_TYPE_IPV4, 0);
        rtk_trunk_stkDistributionAlgorithmTypeBind_set(unit, idx, BIND_TYPE_IPV6, 0);
    }
    rtk_trunk_stkTrkHash_set(unit, STACK_TRK_HASH_RECALCULATE);

    osal_memset(&shift, 0, sizeof(shift));
    rtk_trunk_distributionAlgorithmShiftGbl_set(unit, &shift);

    for (port = 0; port <= RTK_MAX_NUM_OF_PORTS; ++port)
    {
        unitPort.unit = 0;
        unitPort.port = port;
        rtk_trunk_srcPortMap_set(unit, unitPort, 0, 0);
    }

    /* reset Mirror */
    //osal_printf("Reset Mirror module\n");
    osal_memset(&mirrorEntry, 0, sizeof(mirrorEntry));
    osal_memset(&tag, 0, sizeof(tag));

    for(idx = 0; idx < 4; idx ++)
    {
        rtk_mirror_group_set(unit, idx, &mirrorEntry);
        rtk_mirror_rspanEgrMode_set(unit, idx, RSPAN_EGR_NO_MODIFY);
        rtk_mirror_rspanTag_set(unit, idx, &tag);
        rtk_mirror_sflowMirrorSampleRate_set(unit, idx, 0);
    }
    rtk_mirror_egrQueue_set(unit, DISABLED, 0);

    /*reset stacking */
    //osal_printf("Reset Stacking module\n");
    rtk_stack_port_set (unit, &emptyPm);
    rtk_stack_unit_set(unit, 0);
    rtk_stack_masterUnit_set(unit, 0);
    rtk_stack_loopGuard_set(unit, ENABLED);
    for(idx = 0; idx < 16; idx ++)
    {
        rtk_stack_unitPortMap_set (unit, idx, &emptyPm);
        rtk_stack_nonUcastBlockPort_set (unit, idx, &emptyPm);
    }

    /* reset ACL */

    for (idx = 0; idx < HAL_MAX_NUM_OF_PIE_BLOCK(unit); ++idx)
    {
        /* block lookup enable */
        rtk_pie_blockLookupEnable_set(unit, idx, ENABLED);

        /* block default phase is ingress */
        rtk_pie_phase_set(unit, idx, PIE_PHASE_VACL);
    }

    HWP_PORT_TRAVS_EXCEPT_CPU(unit, idx)
    {
        rtk_acl_portPhaseLookupEnable_set(unit, idx, ACL_PHASE_VACL, ENABLED);
        rtk_acl_portPhaseLookupEnable_set(unit, idx, ACL_PHASE_EACL, ENABLED);
    }

    /* Range Check */
    osal_memset(&range, 0, sizeof(rtk_pie_rangeCheck_t));
    for (idx = 0; idx < HAL_MAX_NUM_OF_RANGE_CHECK(unit); ++idx)
        rtk_pie_rangeCheck_set(unit, idx, &range);

    /* IPv4 or IPv6 SIP/DIP */
    osal_memset(&ipRng, 0, sizeof(rtk_pie_rangeCheck_ip_t));
    for (idx = 0; idx < HAL_MAX_NUM_OF_RANGE_CHECK_IP(unit); ++idx)
        rtk_pie_rangeCheckIp_set(unit, idx, &ipRng);

    osal_memset(&tmplateInfo, 0, sizeof(rtk_acl_templateIdx_t));
    for (idx = 0; idx < HAL_MAX_NUM_OF_PIE_BLOCK(unit); ++idx)
    {
        rtk_acl_templateSelector_set(unit, idx, tmplateInfo);
    }

    clearInfo.start_idx = 0;
    clearInfo.end_idx = (HAL_MAX_NUM_OF_PIE_BLOCK(unit) *
            HAL_MAX_NUM_OF_PIE_BLOCKSIZE(unit)) - 1;
    rtk_acl_rule_del(unit, ACL_PHASE_VACL, &clearInfo);

    return ret;
}


/* Function Name:
 *      do_rtdrv_ext_set_ctl
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
int32 do_rtdrv_ext_set_ctl(struct sock *sk, int cmd, void *user_in, unsigned int len)
{
    void            *user = (char *)user_in + sizeof(rtdrv_msgHdr_t);
    rtdrv_ext_union_t buf;
    int32 ret = RT_ERR_FAILED;
    uint32 idx;
    rtk_portmask_t trk_igr_ports;
    rtk_trk_egrPort_t trk_egr_ports;
    auto_rcvy_txerr_cnt_entry_t txerrCntr;
    uint32 myUnit;
    rtk_portmask_t ingressPorts;
    uint32 val;
    rtk_port_t port;
    rtk_unit_port_t unit_port;

    osal_memset(&txerrCntr, 0, sizeof(auto_rcvy_txerr_cnt_entry_t));

    switch(cmd)
    {
    /** INIT **/
    /** L2 **/
        case RTDRV_EXT_L2_AGING_UNIT_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_ext_l2Cfg_t));
            ret = reg_field_write(buf.l2_cfg.unit, MANGO_L2_AGE_CTRLr, MANGO_AGE_UNITf, &buf.l2_cfg.aging_time);
            break;

        case RTDRV_EXT_L2_CMA_ENABLE_SET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_ext_l2Cfg_t));
            ret = reg_field_write(buf.l2_cfg.unit, MANGO_L2_CTRLr, MANGO_LUTCAM_ENf, &buf.l2_cfg.enable);
            rtk_l2_init(buf.l2_cfg.unit);
            break;
    /** ACL **/
        case RTDRV_EXT_ACL_MIRROR_SFLOW_BYPASS_POST_SET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_ext_aclCfg_t));
            ret = reg_field_write(buf.acl_cfg.unit, MANGO_PIE_ENCAP_CTRLr, MANGO_EGR_MIR_SFLOW_BYPASS_POSTf, &buf.acl_cfg.enable);
            break;
    /** PORT **/
        case RTDRV_EXT_PORT_MACFORCESTATE_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_write(buf.port_cfg.unit, MANGO_MAC_FORCE_MODE_CTRLr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, &buf.port_cfg.state);
            break;

        case RTDRV_EXT_PORT_MACFORCELINK_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_write(buf.port_cfg.unit, MANGO_MAC_FORCE_MODE_CTRLr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, &buf.port_cfg.state);
            break;

        case RTDRV_EXT_PORT_MACFORCEFLOWCTRL_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_write(buf.port_cfg.unit, MANGO_MAC_FORCE_MODE_CTRLr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_SMI_RX_PAUSE_ENf, &buf.port_cfg.state);
            ret = reg_array_field_write(buf.port_cfg.unit, MANGO_MAC_FORCE_MODE_CTRLr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_SMI_TX_PAUSE_ENf, &buf.port_cfg.state);
            break;

        case RTDRV_EXT_PORT_SPECIAL_CONGEST_DRAIN_OUT_THRESH_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_field_write(buf.port_cfg.unit, MANGO_SC_DRAIN_OUT_THRr, MANGO_DRAIN_OUT_THRf, &buf.port_cfg.full_th);
            break;

        case RTDRV_EXT_PORT_SPECIAL_CONGEST_DRAIN_OUT_THRESH_HALF_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_field_write(buf.port_cfg.unit, MANGO_SC_DRAIN_OUT_THRr, MANGO_DRAIN_OUT_THR_Hf, &buf.port_cfg.half_th);
            break;

        case RTDRV_EXT_PORT_SPECIAL_CONGEST_PORT_SUSTAIN_TIMER_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_write(buf.port_cfg.unit, MANGO_SC_PORT_TIMERr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_CNGST_SUST_TMR_LMTf, &buf.port_cfg.full_sec);
            break;

        case RTDRV_EXT_PORT_SPECIAL_CONGEST_PORT_SUSTAIN_TIMER_HALF_SET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_write(buf.port_cfg.unit, MANGO_SC_PORT_TIMERr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_CNGST_SUST_TMR_LMT_Hf, &buf.port_cfg.half_sec);
            break;

    /** VLAN **/
    /** STP **/
    /** REG **/
    /** COUNTER **/
        case RTDRV_EXT_STACK_DEBUG_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_ext_stackCfg_t));

            ret = reg_field_write(buf.port_cfg.unit, MANGO_STK_DBG_CTRLr, MANGO_STK_PORT_DEBUGf, &buf.stack_cfg.enable);

            break;
        case RTDRV_EXT_MIB_TX_ERR_CNTR_RESET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_mibCfg_t));
            ret = table_read(buf.mib_cfg.unit, MANGO_AUTO_RECOVERY_TXERR_CNTt, buf.mib_cfg.port, (uint32 *) &txerrCntr);
            ret = table_field_set(buf.mib_cfg.unit, MANGO_AUTO_RECOVERY_TXERR_CNTt,
                MANGO_AUTO_RECOVERY_TXERR_CNT_TXERR_CNTtf, &buf.mib_cfg.rst_val, (uint32 *) &txerrCntr);
            ret = table_write(buf.mib_cfg.unit, MANGO_AUTO_RECOVERY_TXERR_CNTt, buf.mib_cfg.port, (uint32 *) &txerrCntr);
            break;

    /** TRAP **/
    /** FILTER **/
    /** PIE **/
    /** QOS **/
    /** TRUNK **/
        case RTDRV_EXT_TRUNK_RESET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_ext_trunkCfg_t));

            osal_memset(&trk_igr_ports, 0, sizeof(trk_igr_ports));
            osal_memset(&trk_egr_ports, 0, sizeof(trk_egr_ports));

            for(idx = 0; idx < 128; idx ++)
            {
                rtk_trunk_localPort_set (buf.trunk_cfg.unit, idx, &trk_igr_ports);
                rtk_trunk_egrPort_set(buf.trunk_cfg.unit, idx, &trk_egr_ports);
                rtk_trunk_distributionAlgorithmTypeBind_set(buf.trunk_cfg.unit, idx, BIND_TYPE_L2, 0);
                rtk_trunk_distributionAlgorithmTypeBind_set(buf.trunk_cfg.unit, idx, BIND_TYPE_IPV4, 0);
                rtk_trunk_distributionAlgorithmTypeBind_set(buf.trunk_cfg.unit, idx, BIND_TYPE_IPV6, 0);
                rtk_trunk_trafficSeparateEnable_set(buf.trunk_cfg.unit, idx, SEPARATE_KNOWN_MULTI, DISABLED);
                rtk_trunk_trafficSeparateEnable_set(buf.trunk_cfg.unit, idx, SEPARATE_FLOOD, DISABLED);
            }

            for(idx = 0; idx < RTK_MAX_NUM_OF_UNIT; idx ++)
            {
                HWP_PORT_TRAVS_EXCEPT_CPU(buf.trunk_cfg.unit, port)
                {
                    unit_port.unit = idx;
                    unit_port.port = port;
                    rtk_trunk_srcPortMap_set(buf.trunk_cfg.unit, unit_port, 0, 0);
                }
            }

            rtk_trunk_mode_set(buf.trunk_cfg.unit, TRUNK_MODE_STANDALONE);

            ret = RT_ERR_OK;
            break;
        case RTDRV_EXT_TRUNK_MEMBER_SET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_ext_trunkCfg_t));

            rtk_stack_unit_get(buf.trunk_cfg.unit, &myUnit);
            osal_memset(&ingressPorts, 0, sizeof(ingressPorts));

            for(idx = 0; idx < buf.trunk_cfg.trk_ports.num_ports; idx ++)
            {
                if(buf.trunk_cfg.trk_ports.egr_port[idx].unit == myUnit)
                {
                    ingressPorts.bits[buf.trunk_cfg.trk_ports.egr_port[idx].port/32] |= 1 << buf.trunk_cfg.trk_ports.egr_port[idx].port%32;
                }
            }

            rtk_trunk_localPort_set(buf.trunk_cfg.unit, buf.trunk_cfg.id, &ingressPorts);
            rtk_trunk_egrPort_set(buf.trunk_cfg.unit, buf.trunk_cfg.id, &buf.trunk_cfg.trk_ports);

            break;
    /** STACK **/
        case RTDRV_EXT_MIB_RST_VAL_SET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_ext_mibCfg_t));
            ret = reg_field_write(buf.mib_cfg.unit, MANGO_STAT_RSTr, MANGO_RST_MIB_VALf, &buf.mib_cfg.rst_val);
            break;
    /** DOT1X **/
    /** FLOWCTRL **/
    /** RATE **/
        case RTDRV_EXT_RATE_EGR_INCLUDE_CPU_TAG_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_EGBW_ENCAP_CTRLr,
                    MANGO_CPU_TAG_FEED_BACKf, &buf.switch_cfg.enable);
            break;
    /** SVLAN **/
    /** SWITCH **/
        case RTDRV_EXT_SWITCH_48PASS1_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_HALF_48PASS1_ENf, &buf.switch_cfg.half_48pass1);
            break;
        case RTDRV_EXT_SWITCH_MAC_48PASS1_DROP_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_MAC_48PASS1_DROP_ENf, &buf.switch_cfg.enable);
            break;
        case RTDRV_EXT_SWITCH_LIMITPAUSE_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_LIMIT_PAUSE_ENf, &buf.switch_cfg.value);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_1G_100M_10M_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_SEL_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_1G_100M_10M_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_10G_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_10G_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_10G_SEL_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_10G_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_2_5G_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_2P5G_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_2_5G_SEL_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_2P5G_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_5G_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_5G_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_5G_SEL_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_5G_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            break;

        case RTDRV_EXT_SWITCH_IPGMINLEN_10M_100M_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_LIMIT_IPG_CFG_10M_100Mf, &buf.switch_cfg.min_ipg);
            break;

        case RTDRV_EXT_SWITCH_IPGMINLEN_1G_2_5G_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_LIMIT_IPG_CFG_1G_2P5Gf, &buf.switch_cfg.min_ipg);
            break;

        case RTDRV_EXT_SWITCH_BKPRES_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_BKPRES_MTHD_SELf, &buf.switch_cfg.bkpres);
            break;
        case RTDRV_EXT_SWITCH_BYPASSTXCRC_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_BYP_TX_CRCf, &buf.switch_cfg.bypass_tx_crc);
            break;
        case RTDRV_EXT_SWITCH_PASSALLMODE_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_PASS_ALL_MODE_ENf, &buf.switch_cfg.pass_all_mode);
            break;
#if 0   /* removed, not support */
        case RTDRV_EXT_SWITCH_RXCHECKCRC_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_RX_CHK_CRC_ENf, &buf.switch_cfg.rx_check_crc);
            break;
#endif
        case RTDRV_EXT_SWITCH_PADDINGUNDSIZE_SET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_PADDING_UND_SIZE_ENf, &buf.switch_cfg.enable);
            break;
        case RTDRV_EXT_SWITCH_INTR_LINK_CHANGE_ENABLE_SET:
            if (!linkMon_wake_up)
            {
                copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
                ret = rtk_port_linkMon_enable(buf.switch_cfg.unit, 10000);
                linkMon_wake_up = 1;
            }
            break;
        case RTDRV_EXT_SWITCH_INTR_LINK_CHANGE_DISABLE_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = rtk_port_linkMon_disable(buf.switch_cfg.unit);
            linkMon_wake_up = 0;
            break;

        case RTDRV_EXT_SWITCH_PADDINCONTENT_SET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_MAC_L2_PADDING_SELr,
                    MANGO_PADDING_SELf, &buf.switch_cfg.value);
            break;

        case RTDRV_EXT_SWITCH_SW_QUERE_RESET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_write(buf.switch_cfg.unit, MANGO_RST_GLB_CTRLr,
                    MANGO_SW_Q_RSTf, &buf.switch_cfg.value);
            break;

    /** NIC **/
        case RTDRV_EXT_NIC_PKT_SEND_SET:
            copy_from_user(&buf.nicSend_cfg, user, sizeof(rtdrv_ext_nicSendCfg_t));

        {
            uint8 pkt1[] = { /* Pause frame */
                0x01, 0x80, 0xc2, 0x00, 0x00, 0x01, 0x00, 0x1A, 0x1B, 0x33, 0x44, 0x55, 0x00, 0x00, 0x00, 0x01,
                0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            };
            uint8 *tx_buff;
            drv_nic_pkt_t *pPacket;

            pPacket = osal_alloc(sizeof(drv_nic_pkt_t));
            if (pPacket == NULL)
            {
                /* out of memory */
                ret = RT_ERR_FAILED;
                goto FAIL_EXIT;
            }

            tx_buff = osal_alloc(1600);
            if (tx_buff == NULL)
            {
                osal_free(pPacket);
                ret = RT_ERR_FAILED;
                goto FAIL_EXIT;
            }

            {
                if(buf.nicSend_cfg.isCpuTag == TRUE)
                    pPacket->as_txtag = 1;
                else
                    pPacket->as_txtag = 0;

            #if defined(CONFIG_SDK_RTL8380) || defined(CONFIG_SDK_RTL8390)
                if(buf.nicSend_cfg.isTrunkHash == TRUE)
                    pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_LOGICAL;
                else
                    pPacket->tx_tag.fwd_type = NIC_FWD_TYPE_PHYISCAL;
            #endif

                pPacket->tx_tag.dst_port_mask       = buf.nicSend_cfg.txPortmask.bits[0];
                pPacket->tx_tag.dst_port_mask_1     = buf.nicSend_cfg.txPortmask.bits[1];

                pPacket->tx_tag.as_priority         = 1;
                pPacket->tx_tag.priority            = 7;
            }

            osal_memcpy(&pkt1[0], &buf.nicSend_cfg.dst_mac.octet[0], 6);
            osal_memcpy(&pkt1[6], &buf.nicSend_cfg.src_mac.octet[0], 6);


            /* raw packet */
            pPacket->buf_id = (void *)NULL;
            pPacket->head = tx_buff;
            pPacket->data = tx_buff + 2 + 12;
            pPacket->tail = tx_buff + 2 + 12 + 64;
            pPacket->end = tx_buff + 1600;
            pPacket->length = 60;
            pPacket->next = NULL;

            osal_memcpy(pPacket->data, pkt1, 60);

            if (RT_ERR_OK == drv_nic_pkt_tx(buf.nicSend_cfg.unit, pPacket, _nic_tx_callback, (void *)tx_buff))
            {

            }
            else
            {
                osal_free(pPacket);
                osal_free(tx_buff);
                ret = RT_ERR_FAILED;
                goto FAIL_EXIT;
            }
          }

            break;
    /** Remote Access **/
        case RTDRV_EXT_REMOTE_ACCESS_REG_SET:

            copy_from_user(&buf.remoteAccess_cfg, user, sizeof(rtdrv_ext_remoteAccessCfg_t));

            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_UNITf, &buf.remoteAccess_cfg.targetUnit);
            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_ADDRf, &buf.remoteAccess_cfg.addr);
            val = 1; /*write*/
            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_RWOPf, &val);
            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_DATAr,
                MANGO_DATAf, &buf.remoteAccess_cfg.value);

            val = 1; /*exec*/
            ret = reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_EXECf, &val);

            break;

    /** Diag Register/Table access **/
        case RTDRV_EXT_DIAG_REGARRAY_SET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = rtk_diag_regArray_set(buf.diag_cfg.unit, buf.diag_cfg.reg, buf.diag_cfg.idx1, buf.diag_cfg.idx2, (uint32 *)&buf.diag_cfg.data);
            break;

        case RTDRV_EXT_DIAG_REGARRAYFIELD_SET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = rtk_diag_regArrayField_set(buf.diag_cfg.unit, buf.diag_cfg.reg, buf.diag_cfg.idx1, buf.diag_cfg.idx2, buf.diag_cfg.field, (uint32 *)&buf.diag_cfg.data);
            break;

        case RTDRV_EXT_DIAG_TABLEENTRY_SET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = rtk_diag_tableEntry_set(buf.diag_cfg.unit, buf.diag_cfg.table, buf.diag_cfg.addr, (uint32 *)&buf.diag_cfg.data);
            break;

        case RTDRV_EXT_SDK_TC_RESET:    /* sdk reset tc - reset DUT to default configuration */
            copy_from_user(&buf.sdk_cfg, user, sizeof(rtdrv_ext_sdkCfg_t));
            ret = _sdk_tc_reset(buf.sdk_cfg.unit);
            break;


    /** MPLS **/
    /** EEE **/
    /** IOL **/
        case RTDRV_EXT_IOL_COLLISION_MAX_ATTEMPT_SET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_write(buf.iol_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_IOL_MAX_RETRY_ENf, &buf.iol_cfg.action);
            break;
        case RTDRV_EXT_IOL_ERROR_LENGTH_SET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_write(buf.iol_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IOL_LEN_ERR_ENf, &buf.iol_cfg.action);
            break;
        case RTDRV_EXT_IOL_LATE_COLLISION_SET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_write(buf.iol_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_LATE_COLI_DROP_ENf, &buf.iol_cfg.action);
            break;
        case RTDRV_EXT_IOL_MAX_LENGTH_SET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_write(buf.iol_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IOL_MAX_LEN_ENf, &buf.iol_cfg.enable);
            break;

    /** MODEL TEST **/
#ifdef CONFIG_SDK_MODEL_MODE
        case RTDRV_EXT_MODEL_TEST_SET:
            copy_from_user(&buf.model_cfg, user, sizeof(rtdrv_ext_modelCfg_t));
            vmac_setCaredICType(buf.model_cfg.caredType);
            ret = tc_exec(buf.model_cfg.startID, buf.model_cfg.endID);
            vmac_setCaredICType(CARE_TYPE_BOTH);
            break;

        case RTDRV_EXT_MODEL_TEST_UNIT_SET:
            copy_from_user(&buf.model_cfg, user, sizeof(rtdrv_ext_modelCfg_t));
            tc_unit_set(buf.model_cfg.unit);
            vmac_setCaredICType(buf.model_cfg.caredType);
            ret = tc_exec(buf.model_cfg.startID, buf.model_cfg.endID);
            vmac_setCaredICType(CARE_TYPE_BOTH);
            break;
#endif

#ifdef CONFIG_SDK_FPGA_PLATFORM
        case RTDRV_EXT_FPGA_INIT_SET:
            copy_from_user(&buf.fpga_cfg, user, sizeof(rtdrv_ext_fpgaCfg_t));
            ret = _fpga_init(buf.fpga_cfg.unit, buf.fpga_cfg.fpgaVer);
            break;

        case RTDRV_EXT_TESTIO_SET_EPHY:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            _testio_set_ePhy(buf.diag_cfg.unit, buf.diag_cfg.reg, buf.diag_cfg.data[0]);
            break;

        case RTDRV_EXT_TESTIO_SET_EFUSE:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            _testio_set_eFuse(buf.diag_cfg.unit, buf.diag_cfg.idx1, buf.diag_cfg.reg, buf.diag_cfg.data[0]);
            break;

        case RTDRV_EXT_FPGA_TEST:
            copy_from_user(&buf.fpga_cfg, user, sizeof(rtdrv_ext_fpgaCfg_t));
            {
                uint32              unit = buf.fpga_cfg.unit;
                uint32              i, j, port;
                uint32              ebl[4][4], val, data[2], data1[2], mask[2], mask1[2], rate[4][4], burst[4][4];
                rtk_l2_ucastAddr_t  ucastEntry;
                rtk_mac_t           mac;
                rtk_vlan_t          vid;
                rtk_portmask_t      member_portmask, untag_portmask;
                rtk_l2_mcastAddr_t  mcastAddr;
                rtk_rate_assuredMode_t cfgMode[4][4];
                rtk_acl_phase_t     phase = ACL_PHASE_VACL;
                vlan_entry_t        vlan_entry;
                vlan_untag_entry_t  vlan_untag_entry;
                egr_qBw_entry_t     egrQEntry;
                const static uint16 egrQBwBurst_fieldidx[] = {MANGO_EGR_Q_BW_MAX_LB_BURST_Q0tf, MANGO_EGR_Q_BW_MAX_LB_BURST_Q1tf,
                                              MANGO_EGR_Q_BW_MAX_LB_BURST_Q2tf, MANGO_EGR_Q_BW_MAX_LB_BURST_Q3tf,
                                              MANGO_EGR_Q_BW_MAX_LB_BURST_Q4tf, MANGO_EGR_Q_BW_MAX_LB_BURST_Q5tf,
                                              MANGO_EGR_Q_BW_MAX_LB_BURST_Q6tf, MANGO_EGR_Q_BW_MAX_LB_BURST_Q7tf,
                                              MANGO_EGR_Q_BW_MAX_LB_BURST_Q8tf, MANGO_EGR_Q_BW_MAX_LB_BURST_Q9tf,
                                              MANGO_EGR_Q_BW_MAX_LB_BURST_Q10tf, MANGO_EGR_Q_BW_MAX_LB_BURST_Q11tf};

                /* Table access test */
                /* L2 */
                osal_memset(&ucastEntry, 0, sizeof(rtk_l2_ucastAddr_t));
                osal_memset(&member_portmask, 0, sizeof(rtk_portmask_t));
                osal_memset(&untag_portmask, 0, sizeof(rtk_portmask_t));
                mac.octet[0] = 0x1a;
                mac.octet[1] = 0x11;
                mac.octet[2] = 0x22;
                mac.octet[3] = 0x33;
                mac.octet[4] = 0x44;
                mac.octet[5] = 0x55;
                ucastEntry.vid = 25;
                ucastEntry.port = 1;
                osal_memcpy(&ucastEntry.mac, &mac, ETHER_ADDR_LEN);
                if ((ret = rtk_l2_addr_add(unit, &ucastEntry)))
                {
                    osal_printf("%s():%d  FAIL! Adding L2 entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((ret = rtk_l2_addr_get(unit, &ucastEntry)))
                {
                    osal_printf("%s():%d  FAIL! Getting L2 entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((ret = osal_memcmp(&ucastEntry.mac, &mac, ETHER_ADDR_LEN)))
                {
                    osal_printf("%s():%d  FAIL! Compareing L2 entry failed.\n", __FUNCTION__, __LINE__);
                }
                /* Power Saving Reset Test: Check whether table content exist */
                val = 1;
                if ((ret = reg_field_write(unit, MANGO_PS_SLOW_SYSCLK_CTRLr, MANGO_SLOW_DOWN_SYSCLK_ENf, &val)) != RT_ERR_OK)
                {
                    osal_printf("%s():%d  Error! Setting SLOW_DOWN_SYSCLK_EN to %d failed.  ret:%#x\n", __FUNCTION__, __LINE__, val, ret);
                }
                if ((ret = rtk_l2_addr_get(unit, &ucastEntry)))
                {
                    osal_printf("%s():%d  FAIL! Getting L2 entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((ret = osal_memcmp(&ucastEntry.mac, &mac, ETHER_ADDR_LEN)))
                {
                    osal_printf("%s():%d  FAIL! Compareing L2 entry failed.\n", __FUNCTION__, __LINE__);
                }
                val = 0;
                if ((ret = reg_field_write(unit, MANGO_PS_SLOW_SYSCLK_CTRLr, MANGO_SLOW_DOWN_SYSCLK_ENf, &val)) != RT_ERR_OK)
                {
                    osal_printf("%s():%d  Error! Setting SLOW_DOWN_SYSCLK_EN to %d failed.  ret:%#x\n", __FUNCTION__, __LINE__, val, ret);
                }
                if ((ret = rtk_l2_addr_get(unit, &ucastEntry)))
                {
                    osal_printf("%s():%d  FAIL! Getting L2 entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((ret = osal_memcmp(&ucastEntry.mac, &mac, ETHER_ADDR_LEN)))
                {
                    osal_printf("%s():%d  FAIL! Compareing L2 entry failed After switch SLOW_DOWN_SYSCLK_EN.\n", __FUNCTION__, __LINE__);
                }
                if ((ret = rtk_l2_addr_del(unit, 25, &mac)))
                {
                    osal_printf("%s():%d  FAIL! Deleting L2 entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }


                /* Multicast Portmask Table */
                osal_memset(&mcastAddr, 0, sizeof(rtk_l2_mcastAddr_t));
                mcastAddr.rvid = 1;
                mcastAddr.mac.octet[0] = 0x01;
                mcastAddr.mac.octet[1] = 0x11;
                mcastAddr.mac.octet[2] = 0x22;
                mcastAddr.mac.octet[3] = 0x33;
                mcastAddr.mac.octet[4] = 0x44;
                mcastAddr.mac.octet[5] = 0x55;
                mcastAddr.portmask.bits[0] = 0x1;
                mcastAddr.portmask.bits[1] = 0x300000;
                if ((ret = rtk_l2_mcastAddr_add(unit, &mcastAddr)))
                {
                    osal_printf("%s():%d  FAIL! Adding L2 multicast entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                mcastAddr.portmask.bits[0] = 0x0;
                mcastAddr.portmask.bits[1] = 0x0;
                if ((ret = rtk_l2_mcastAddr_get(unit, &mcastAddr)))
                {
                    osal_printf("%s():%d  FAIL! Getting L2 entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if (mcastAddr.portmask.bits[0] != 0x1 || mcastAddr.portmask.bits[1] != 0x300000)
                {
                    osal_printf("%s():%d  FAIL! Compareing Multicast Portmask Table entry failed.\n", __FUNCTION__, __LINE__);
                }
                if ((ret = rtk_l2_mcastAddr_del(unit, mcastAddr.rvid, &mcastAddr.mac)))
                {
                    osal_printf("%s():%d  FAIL! Deleting L2 multicast entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }


                /* VLAN */
                osal_memset(&vlan_entry, 0, sizeof(vlan_entry_t));
                osal_memset(&vlan_untag_entry, 0, sizeof(vlan_untag_entry_t));
                vid = 28;
                if ((ret = rtk_vlan_create(unit, vid)))
                {
                    osal_printf("%s():%d  FAIL! Creating VLAN entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                member_portmask.bits[0] = 0xffffffff;;
                member_portmask.bits[1] = 0x1ffffff;;
                untag_portmask.bits[0] = 0xacacacac;;
                untag_portmask.bits[1] = 0x1ffffff;;
                if ((ret = table_field_set(unit, MANGO_VLANt, MANGO_VLAN_MBRtf, member_portmask.bits, (uint32 *) &vlan_entry)) != RT_ERR_OK)
                {
                    RT_ERR(ret, (MOD_VLAN|MOD_DAL), "");
                    return ret;
                }
                if ((ret = table_write(unit, MANGO_VLANt, vid, (uint32 *) &vlan_entry)) != RT_ERR_OK)
                {
                    RT_ERR(ret, (MOD_VLAN|MOD_DAL), "");
                    return ret;
                }
                if ((ret = table_field_set(unit, MANGO_VLAN_UNTAGt, MANGO_VLAN_UNTAG_UNTAGtf, untag_portmask.bits, (uint32 *) &vlan_untag_entry)) != RT_ERR_OK)
                {
                    RT_ERR(ret, (MOD_VLAN|MOD_DAL), "");
                    return ret;
                }
                if ((ret = table_write(unit, MANGO_VLAN_UNTAGt, vid, (uint32 *) &vlan_untag_entry)) != RT_ERR_OK)
                {
                    RT_ERR(ret, (MOD_VLAN|MOD_DAL), "");
                    return ret;
                }
                if ((ret = rtk_vlan_port_get(unit, vid, &member_portmask, &untag_portmask)))
                {
                    osal_printf("%s():%d  FAIL! Getting VLAN entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((member_portmask.bits[0] != 0xffffffff) || (member_portmask.bits[1] != 0x1ffffff) ||
                    (untag_portmask.bits[0] != 0xacacacac) || (untag_portmask.bits[1] != 0x1ffffff))
                {
                    osal_printf("%s():%d  FAIL! Compareing VLAN entry failed.\n", __FUNCTION__, __LINE__);
                }
                /* Power Saving Reset Test: Check whether table content exist */
                val = 1;
                if ((ret = reg_field_write(unit, MANGO_PS_SLOW_SYSCLK_CTRLr, MANGO_SLOW_DOWN_SYSCLK_ENf, &val)) != RT_ERR_OK)
                {
                    osal_printf("%s():%d  Error! Setting SLOW_DOWN_SYSCLK_EN to %d failed.  ret:%#x\n", __FUNCTION__, __LINE__, val, ret);
                }
                if ((ret = rtk_vlan_port_get(unit, vid, &member_portmask, &untag_portmask)))
                {
                    osal_printf("%s():%d  FAIL! Getting VLAN entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((member_portmask.bits[0] != 0xffffffff) || (member_portmask.bits[1] != 0x1ffffff) ||
                    (untag_portmask.bits[0] != 0xacacacac) || (untag_portmask.bits[1] != 0x1ffffff))
                {
                    osal_printf("%s():%d  FAIL! Compareing VLAN entry failed.\n", __FUNCTION__, __LINE__);
                }
                val = 0;
                if ((ret = reg_field_write(unit, MANGO_PS_SLOW_SYSCLK_CTRLr, MANGO_SLOW_DOWN_SYSCLK_ENf, &val)) != RT_ERR_OK)
                {
                    osal_printf("%s():%d  Error! Setting SLOW_DOWN_SYSCLK_EN to %d failed.  ret:%#x\n", __FUNCTION__, __LINE__, val, ret);
                }
                if ((ret = rtk_vlan_port_get(unit, vid, &member_portmask, &untag_portmask)))
                {
                    osal_printf("%s():%d  FAIL! Getting VLAN entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((member_portmask.bits[0] != 0xffffffff) || (member_portmask.bits[1] != 0x1ffffff) ||
                    (untag_portmask.bits[0] != 0xacacacac) || (untag_portmask.bits[1] != 0x1ffffff))
                {
                    osal_printf("%s():%d  FAIL! Compareing VLAN entry failed.\n", __FUNCTION__, __LINE__);
                }
                if ((ret = rtk_vlan_destroy(unit, vid)))
                {
                    osal_printf("%s():%d  FAIL! Creating VLAN entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }


                /* ACL */

                if ((ret = rtk_pie_phase_set(unit, 0, PIE_PHASE_VACL)))
                {
                    osal_printf("%s():%d  FAIL! Set block phase failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                osal_memset(data, 0, 2);
                osal_memset(mask, 0, 2);
                if ((ret = rtk_acl_ruleEntryField_read(unit, phase, 0, USER_FIELD_SPM, (uint8 *)data, (uint8 *)mask)))
                {
                    osal_printf("%s():%d  FAIL! Getting ACL entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                data1[0] = 0x0;
                data1[1] = 0xFFFFFFFF;
                mask1[0] = 0x0;
                mask1[1] = 0xFFFFFFFF;
                if ((ret = rtk_acl_ruleEntryField_write(unit, phase, 0, USER_FIELD_SPM, (uint8 *)data1, (uint8 *)mask1)))
                {
                    osal_printf("%s():%d  FAIL! Adding ACL entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }
                if ((ret = rtk_acl_ruleEntryField_read(unit, phase, 0, USER_FIELD_SPM, (uint8 *)data, (uint8 *)mask)))
                {
                    osal_printf("%s():%d  FAIL! Getting ACL entry failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                }

                if (0 != memcmp(data, data1, 2) || 0 != memcmp(mask, mask1, 2))
                {
                    osal_printf("%s():%d  FAIL! Compareing ACL entry failed.\n", __FUNCTION__, __LINE__);
                    osal_printf("data 0x%x 0x%x mask 0x%x 0x%x\n", data[0], data[1], mask[0], mask[1]);
                }

                /* Egr Queue */ /* test PORT 0, 52, 53*/
                for (i = 0; i < 3; i++)
                {
                    port = i;
                    if (i == 1)
                        port = 52;
                    else if (i == 2)
                        port = 53;
                    for (j = 0; j < 4; j++)
                    {
                        if ((ret = rtk_rate_portEgrQueueBwCtrlEnable_get(unit, port, j, &ebl[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueBwCtrlEnable_set(unit, port, j, !ebl[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueBwCtrlEnable_get(unit, port, j, &val)))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if (val != (!ebl[i][j]))
                            osal_printf("%s():%d  FAIL! Compareing Egr Queue failed.\n", __FUNCTION__, __LINE__);
                        else
                        {
                            if ((ret = rtk_rate_portEgrQueueBwCtrlEnable_set(unit, port, j, ebl[i][j])))
                            {
                                osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                            }
                        }

                        if ((ret = rtk_rate_portEgrQueueBwCtrlBurstSize_get(unit, port, j, &burst[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        /* write EGR_Q_BW table */
                        osal_memset(&egrQEntry, 0, sizeof(egr_qBw_entry_t));
                        if ((ret = table_read(unit, MANGO_EGR_Q_BWt, port, (uint32 *) &egrQEntry)) != RT_ERR_OK)
                        {
                            osal_printf("%s():%d  FAIL! Reading EGR_Q_BW table failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        val = !burst[i][j];
                        if ((ret = table_field_set(unit, MANGO_EGR_Q_BWt, (uint32)egrQBwBurst_fieldidx[j],
                                        &val, (uint32 *) &egrQEntry)) != RT_ERR_OK)
                        {
                            osal_printf("%s():%d  FAIL! Writing EGR_Q_BW table failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = table_write(unit, MANGO_EGR_Q_BWt, port, (uint32 *) &egrQEntry)) != RT_ERR_OK)
                        {
                            osal_printf("%s():%d  FAIL! Writing EGR_Q_BW table failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueBwCtrlBurstSize_get(unit, port, j, &val)))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if (val != (!burst[i][j]))
                            osal_printf("%s():%d  FAIL! Compareing Egr Queue failed.\n", __FUNCTION__, __LINE__);
                        else
                        {
                            if ((ret = rtk_rate_portEgrQueueBwCtrlBurstSize_set(unit, port, j, burst[i][j])))
                            {
                                osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                            }
                        }

                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlEnable_get(unit, port, j, &ebl[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlEnable_set(unit, port, j, !ebl[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlEnable_get(unit, port, j, &val)))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if (val != (!ebl[i][j]))
                            osal_printf("%s():%d  FAIL! Compareing Egr Queue failed.\n", __FUNCTION__, __LINE__);
                        else
                        {
                            if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlEnable_set(unit, port, j, ebl[i][j])))
                            {
                                osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                            }
                        }

                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlRate_get(unit, port, j, &rate[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlRate_set(unit, port, j, !rate[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlRate_get(unit, port, j, &val)))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if (val != (!rate[i][j]))
                            osal_printf("%s():%d  FAIL! Compareing Egr Queue failed.\n", __FUNCTION__, __LINE__);
                        else
                        {
                            if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlRate_set(unit, port, j, rate[i][j])))
                            {
                                osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                            }
                        }

                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlMode_get(unit, port, j, &cfgMode[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlMode_set(unit, port, j, !cfgMode[i][j])))
                        {
                            osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlMode_get(unit, port, j, &val)))
                        {
                            osal_printf("%s():%d  FAIL! Reading Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                        }
                        if (val != (!cfgMode[i][j]))
                            osal_printf("%s():%d  FAIL! Compareing Egr Queue failed.\n", __FUNCTION__, __LINE__);
                        else
                        {
                            if ((ret = rtk_rate_portEgrQueueAssuredBwCtrlMode_set(unit, port, j, cfgMode[i][j])))
                            {
                                osal_printf("%s():%d  FAIL! Writing Egr Queue failed.  ret:%#x\n", __FUNCTION__, __LINE__, ret);
                            }
                        }
                    }
                }


                /* MSTI */
                for (i = 0; i < 3; i++)
                {
                    port = i;
                    if (i == 1)
                        port = 52;
                    else if (i == 2)
                        port = 53;
                    for (j = 0; j < 4; j++)
                    {
                        rtk_stp_mstpInstance_create(unit, j);
                        if ((ret = rtk_stp_mstpState_set(unit, j, port, STP_STATE_BLOCKING)))
                        {
                            osal_printf("%s():%d  FAIL! Writing MSTI failed.  ret:%#x  MSTI:%d port:%d\n", __FUNCTION__, __LINE__, ret, j, port);
                            goto FAIL_EXIT;
                        }
                        if ((ret = rtk_stp_mstpState_get(unit, j, port, &val)))
                        {
                            osal_printf("%s():%d  FAIL! Reading MSTI failed.  ret:%#x  MSTI:%d port:%d\n", __FUNCTION__, __LINE__, ret, j, port);
                            goto FAIL_EXIT;
                        }
                        if (val != STP_STATE_BLOCKING)
                            osal_printf("%s():%d  FAIL! Compareing MSTI failed. MSTI[%d].state[%d]:%d != STP_STATE_BLOCKING\n", __FUNCTION__, __LINE__, j, port, val);


                        if ((ret = rtk_stp_mstpState_set(unit, j, port, STP_STATE_FORWARDING)))
                        {
                            osal_printf("%s():%d  FAIL! Writing MSTI failed.  ret:%#x  MSTI:%d port:%d\n", __FUNCTION__, __LINE__, ret, j, port);
                            goto FAIL_EXIT;
                        }
                        if ((ret = rtk_stp_mstpState_get(unit, j, port, &val)))
                        {
                            osal_printf("%s():%d  FAIL! Reading MSTI failed.  ret:%#x  MSTI:%d port:%d\n", __FUNCTION__, __LINE__, ret, j, port);
                            goto FAIL_EXIT;
                        }
                        if (val != STP_STATE_FORWARDING)
                            osal_printf("%s():%d  FAIL! Compareing MSTI failed. MSTI[%d].state[%d]:%d != STP_STATE_FORWARDING\n", __FUNCTION__, __LINE__, j, port, val);

                        rtk_stp_mstpInstance_destroy(unit, j);
                    }
                }
            }
#endif

        default:
            break;
    }


FAIL_EXIT:

    copy_to_user(&((rtdrv_msgHdr_t *)user_in)->ret_code, &ret, sizeof(ret));

    return 0;
}

/* Function Name:
 *      do_rtdrv_ext_get_ctl
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
int32 do_rtdrv_ext_get_ctl(struct sock *sk, int cmd, void *user_in, int *len)
{
    void                *user = (char *)user_in + sizeof(rtdrv_msgHdr_t);
    rtdrv_ext_union_t   buf;
    int32               ret = RT_ERR_FAILED;
    auto_rcvy_txerr_cnt_entry_t txerrCntr;
    uint32              val;

    osal_memset(&txerrCntr, 0, sizeof(auto_rcvy_txerr_cnt_entry_t));

    switch(cmd)
    {
    /** INIT **/
    /** L2 **/
        case RTDRV_EXT_L2_AGING_UNIT_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_ext_l2Cfg_t));
            ret = reg_field_read(buf.l2_cfg.unit, MANGO_L2_AGE_CTRLr, MANGO_AGE_UNITf, &buf.l2_cfg.aging_time);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_ext_l2Cfg_t));
            break;

        case RTDRV_EXT_L2_UC_SIZE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_ext_l2Cfg_t));

            ret = table_size_get(buf.l2_cfg.unit, MANGO_L2_UCt, &buf.l2_cfg.data);

            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_ext_l2Cfg_t));
            break;

        case RTDRV_EXT_L2_CMA_ENABLE_GET:
            copy_from_user(&buf.l2_cfg, user, sizeof(rtdrv_ext_l2Cfg_t));
            ret = reg_field_read(buf.l2_cfg.unit, MANGO_L2_CTRLr, MANGO_LUTCAM_ENf, &buf.l2_cfg.enable);
            copy_to_user(user, &buf.l2_cfg, sizeof(rtdrv_ext_l2Cfg_t));
            break;

    /** ACL **/
        case RTDRV_EXT_ACL_MIRROR_SFLOW_BYPASS_POST_GET:
            copy_from_user(&buf.acl_cfg, user, sizeof(rtdrv_ext_aclCfg_t));
            ret = reg_field_read(buf.acl_cfg.unit, MANGO_PIE_ENCAP_CTRLr, MANGO_EGR_MIR_SFLOW_BYPASS_POSTf, &buf.acl_cfg.enable);
            copy_to_user(user, &buf.acl_cfg, sizeof(rtdrv_ext_aclCfg_t));
            break;

    /** PORT **/
        case RTDRV_EXT_PORT_MACFORCESTATE_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret =  reg_array_field_read(buf.port_cfg.unit, MANGO_MAC_FORCE_MODE_CTRLr, buf.port_cfg.port,
                REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINK_ENf, &buf.port_cfg.state);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_ext_portCfg_t));

        case RTDRV_EXT_PORT_MACFORCELINK_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_read(buf.port_cfg.unit, MANGO_MAC_FORCE_MODE_CTRLr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_SMI_FORCE_LINKf, &buf.port_cfg.state);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_ext_portCfg_t));
            break;

        case RTDRV_EXT_PORT_MACFORCEFLOWCTRL_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_read(buf.port_cfg.unit, MANGO_MAC_FORCE_MODE_CTRLr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_SMI_RX_PAUSE_ENf, &buf.port_cfg.state);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_ext_portCfg_t));
            break;

        case RTDRV_EXT_PORT_SPECIAL_CONGEST_DRAIN_OUT_THRESH_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_field_read(buf.port_cfg.unit, MANGO_SC_DRAIN_OUT_THRr, MANGO_DRAIN_OUT_THRf, &buf.port_cfg.full_th);
            ret = reg_field_read(buf.port_cfg.unit, MANGO_SC_DRAIN_OUT_THRr, MANGO_DRAIN_OUT_THR_Hf, &buf.port_cfg.half_th);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_ext_portCfg_t));
            break;

        case RTDRV_EXT_PORT_SPECIAL_CONGEST_PORT_GET:
            copy_from_user(&buf.port_cfg, user, sizeof(rtdrv_ext_portCfg_t));
            ret = reg_array_field_read(buf.port_cfg.unit, MANGO_SC_PORT_TIMERr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_CNGST_SUST_TMR_LMTf, &buf.port_cfg.full_sec);
            ret = reg_array_field_read(buf.port_cfg.unit, MANGO_SC_PORT_TIMERr, buf.port_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_CNGST_SUST_TMR_LMT_Hf, &buf.port_cfg.half_sec);
            copy_to_user(user, &buf.port_cfg, sizeof(rtdrv_ext_portCfg_t));
            break;
    /** VLAN **/
    /** STP **/
    /** REG **/
    /** COUNTER **/
        case RTDRV_EXT_MIB_RST_VAL_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_mibCfg_t));
            ret = reg_field_read(buf.mib_cfg.unit, MANGO_STAT_RSTr, MANGO_RST_MIB_VALf, &buf.mib_cfg.rst_val);
            copy_to_user(user, &buf.mib_cfg, sizeof(rtdrv_ext_mibCfg_t));
            break;
        case RTDRV_EXT_MIB_TX_ERR_CNTR_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_mibCfg_t));
            ret = table_read(buf.mib_cfg.unit, MANGO_AUTO_RECOVERY_TXERR_CNTt, buf.mib_cfg.port, (uint32 *) &txerrCntr);
            ret = table_field_get(buf.mib_cfg.unit, MANGO_AUTO_RECOVERY_TXERR_CNTt,
                MANGO_AUTO_RECOVERY_TXERR_CNT_TXERR_CNTtf, &buf.mib_cfg.cntr, (uint32 *) &txerrCntr);
            copy_to_user(user, &buf.mib_cfg, sizeof(rtdrv_ext_mibCfg_t));
            break;

    /** TRAP **/
    /** FILTER **/
    /** PIE **/
    /** QOS **/
    /** TRUNK **/
        case RTDRV_EXT_TRUNK_LOCAL_ENTRY_GET:
            copy_from_user(&buf.trunk_cfg, user, sizeof(rtdrv_ext_trunkCfg_t));
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_NUM_TX_CANDIf, &buf.trunk_cfg.localEntry.num_tx_candi);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_L2_HASH_MSK_IDXf, &buf.trunk_cfg.localEntry.hash_msk_idx);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT7f, &buf.trunk_cfg.localEntry.trk_port7);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT6f, &buf.trunk_cfg.localEntry.trk_port6);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT5f, &buf.trunk_cfg.localEntry.trk_port5);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT4f, &buf.trunk_cfg.localEntry.trk_port4);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT3f, &buf.trunk_cfg.localEntry.trk_port3);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT2f, &buf.trunk_cfg.localEntry.trk_port2);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT1f, &buf.trunk_cfg.localEntry.trk_port1);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_TRK_PORT0f, &buf.trunk_cfg.localEntry.trk_port0);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_SEP_FLOOD_PORTf, &buf.trunk_cfg.localEntry.sep_dlf_bcast_port);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_SEP_KWN_MC_PORTf, &buf.trunk_cfg.localEntry.sep_kwn_mc_port);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_NUM_AVL_TX_CANDIf, &buf.trunk_cfg.localEntry.num_avl_tx_candi);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT7f, &buf.trunk_cfg.localEntry.avl_trk_port7);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT6f, &buf.trunk_cfg.localEntry.avl_trk_port6);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT5f, &buf.trunk_cfg.localEntry.avl_trk_port5);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT4f, &buf.trunk_cfg.localEntry.avl_trk_port4);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT3f, &buf.trunk_cfg.localEntry.avl_trk_port3);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT2f, &buf.trunk_cfg.localEntry.avl_trk_port2);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT1f, &buf.trunk_cfg.localEntry.avl_trk_port1);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_TRK_PORT0f, &buf.trunk_cfg.localEntry.avl_trk_port0);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_SEP_FLOOD_PORTf, &buf.trunk_cfg.localEntry.avl_sep_dlf_bcast_port);
            ret = reg_array_field_read(buf.trunk_cfg.unit, MANGO_TRK_LOCAL_TBLr, REG_ARRAY_INDEX_NONE,
                    buf.trunk_cfg.id, MANGO_AVL_SEP_KWN_MC_PORTf, &buf.trunk_cfg.localEntry.avl_sep_kwn_mc_port);

            copy_to_user(user, &buf.trunk_cfg, sizeof(rtdrv_ext_trunkCfg_t));
            break;
    /** STACK **/
        case RTDRV_EXT_STACK_DEBUG_GET:
            copy_from_user(&buf.stack_cfg, user, sizeof(rtdrv_ext_stackCfg_t));

            ret = reg_field_read(buf.port_cfg.unit, MANGO_STK_DBG_CTRLr, MANGO_STK_PORT_DEBUGf, &buf.stack_cfg.enable);

            copy_to_user(user, &buf.stack_cfg, sizeof(rtdrv_ext_stackCfg_t));

            break;
    /** DOT1X **/
    /** FLOWCTRL **/
    /** RATE **/
        case RTDRV_EXT_RATE_EGR_INCLUDE_CPU_TAG_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_EGBW_ENCAP_CTRLr,
                    MANGO_CPU_TAG_FEED_BACKf, &buf.switch_cfg.enable);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
    /** SVLAN **/
    /** SWITCH **/
        case RTDRV_EXT_SWITCH_48PASS1_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_HALF_48PASS1_ENf, &buf.switch_cfg.half_48pass1);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_LIMITPAUSE_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_LIMIT_PAUSE_ENf, &buf.switch_cfg.value);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_1G_100M_10M_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_SEL_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_1G_100M_10M_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_10G_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_10G_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_10G_SEL_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_10G_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
         case RTDRV_EXT_SWITCH_IPGCOMSTN_2_5G_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_2P5G_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_2_5G_SEL_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_2P5G_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_IPGCOMSTN_5G_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_5G_COMPS_ENf, &buf.switch_cfg.ipg_cmpstn);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;

        case RTDRV_EXT_SWITCH_IPGCOMSTN_5G_SEL_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IPG_5G_SELf, &buf.switch_cfg.ipg_cmpstn_sel);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;

        case RTDRV_EXT_SWITCH_IPGMINLEN_10M_100M_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_LIMIT_IPG_CFG_10M_100Mf, &buf.switch_cfg.min_ipg);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;

        case RTDRV_EXT_SWITCH_IPGMINLEN_1G_2_5G_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_LIMIT_IPG_CFG_1G_2P5Gf, &buf.switch_cfg.min_ipg);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_BKPRES_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_BKPRES_MTHD_SELf, &buf.switch_cfg.bkpres);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_BYPASSTXCRC_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_BYP_TX_CRCf, &buf.switch_cfg.bypass_tx_crc);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
        case RTDRV_EXT_SWITCH_PASSALLMODE_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_PASS_ALL_MODE_ENf, &buf.switch_cfg.pass_all_mode);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
#if 0   /* removed, not support */
        case RTDRV_EXT_SWITCH_RXCHECKCRC_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_read(buf.switch_cfg.unit, MANGO_MAC_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_RX_CHK_CRC_ENf, &buf.switch_cfg.rx_check_crc);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;
#endif
        case RTDRV_EXT_SWITCH_PADDINGUNDSIZE_GET:
            copy_from_user(&buf.mib_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_array_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_PORT_CTRLr, buf.switch_cfg.port,
                    REG_ARRAY_INDEX_NONE, MANGO_PADDING_UND_SIZE_ENf, &buf.switch_cfg.enable);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;

        case RTDRV_EXT_SWITCH_PADDINCONTENT_GET:
            copy_from_user(&buf.switch_cfg, user, sizeof(rtdrv_ext_switchCfg_t));
            ret = reg_field_read(buf.switch_cfg.unit, MANGO_MAC_L2_PADDING_SELr,
                    MANGO_PADDING_SELf, &buf.switch_cfg.value);
            copy_to_user(user, &buf.switch_cfg, sizeof(rtdrv_ext_switchCfg_t));
            break;

    /** NIC **/
    /** MPLS **/
    /** EEE **/
        case RTDRV_EXT_EEE_PORT_STATUS_GET:
            copy_from_user(&buf.eee_cfg, user, sizeof(rtdrv_ext_eeeCfg_t));
            ret = reg_array_field_read(buf.eee_cfg.unit,
                          MANGO_EEE_PORT_TX_STSr,
                          buf.eee_cfg.port,
                          REG_ARRAY_INDEX_NONE,
                          MANGO_EEE_EEEP_TX_STSf,
                          &buf.eee_cfg.txState);
            ret = reg_array_field_read(buf.eee_cfg.unit,
                          MANGO_EEE_PORT_RX_STSr,
                          buf.eee_cfg.port,
                          REG_ARRAY_INDEX_NONE,
                          MANGO_EEE_EEEP_RX_STSf,
                          &buf.eee_cfg.rxState);
            copy_to_user(user, &buf.eee_cfg, sizeof(rtdrv_ext_eeeCfg_t));
            break;
    /** IOL **/
        case RTDRV_EXT_IOL_COLLISION_MAX_ATTEMPT_GET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_read(buf.iol_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_IOL_MAX_RETRY_ENf, &buf.iol_cfg.action);
            copy_to_user(user, &buf.iol_cfg, sizeof(rtdrv_ext_iolCfg_t));
            break;
        case RTDRV_EXT_IOL_ERROR_LENGTH_GET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_read(buf.iol_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IOL_LEN_ERR_ENf, &buf.iol_cfg.action);
            copy_to_user(user, &buf.iol_cfg, sizeof(rtdrv_ext_iolCfg_t));
            break;
        case RTDRV_EXT_IOL_LATE_COLLISION_GET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_read(buf.iol_cfg.unit, MANGO_MAC_GLB_CTRLr,
                    MANGO_LATE_COLI_DROP_ENf, &buf.iol_cfg.action);
            copy_to_user(user, &buf.iol_cfg, sizeof(rtdrv_ext_iolCfg_t));
            break;
        case RTDRV_EXT_IOL_MAX_LENGTH_GET:
            copy_from_user(&buf.iol_cfg, user, sizeof(rtdrv_ext_iolCfg_t));
            ret = reg_field_read(buf.iol_cfg.unit, MANGO_MAC_L2_GLOBAL_CTRL1r,
                    MANGO_IOL_MAX_LEN_ENf, &buf.iol_cfg.enable);
            copy_to_user(user, &buf.iol_cfg, sizeof(rtdrv_ext_iolCfg_t));
            break;

#ifdef CONFIG_SDK_FPGA_PLATFORM
        case RTDRV_EXT_FPGA_INFO_GET:
            copy_from_user(&buf.fpga_cfg, user, sizeof(rtdrv_ext_fpgaCfg_t));
            ret = _fpga_info_get(buf.fpga_cfg.unit, &buf.fpga_cfg.rtl_svn_rev, &buf.fpga_cfg.build_date, &buf.fpga_cfg.build_time, &buf.fpga_cfg.fpga_type_and_reg_profile_ver);
            copy_to_user(user, &buf.fpga_cfg, sizeof(rtdrv_ext_fpgaCfg_t));
            break;
        case RTDRV_EXT_TESTIO_GET_EPHY:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = _testio_get_ePhy(buf.diag_cfg.unit, buf.diag_cfg.reg, &buf.diag_cfg.data[0]);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_ext_diagCfg_t));
            break;
        case RTDRV_EXT_TESTIO_GET_EFUSE:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = _testio_get_eFuse(buf.diag_cfg.unit, buf.diag_cfg.idx1, &buf.diag_cfg.reg, &buf.diag_cfg.data[0]);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_ext_diagCfg_t));
            break;
#endif

    /** Remote Access **/
        case RTDRV_EXT_REMOTE_ACCESS_REG_GET:

            copy_from_user(&buf.remoteAccess_cfg, user, sizeof(rtdrv_ext_remoteAccessCfg_t));

            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_UNITf, &buf.remoteAccess_cfg.targetUnit);
            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_ADDRf, &buf.remoteAccess_cfg.addr);
            val = 0; /*read*/
            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_RWOPf, &val);

            val = 1; /*exec*/
            reg_field_write(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_EXECf, &val);

            ret = reg_field_read(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_DATAr,
                MANGO_DATAf, &buf.remoteAccess_cfg.value);
            copy_to_user(user, &buf.remoteAccess_cfg, sizeof(rtdrv_ext_remoteAccessCfg_t));

            reg_field_read(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr,
                MANGO_EXECf, &val);

            reg_read(buf.remoteAccess_cfg.unit, MANGO_RMT_REG_ACCESS_CTRLr, &val);

            break;


    /** Diag Register/Table access **/
        case RTDRV_EXT_DIAG_REGARRAY_GET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = rtk_diag_regArray_get(buf.diag_cfg.unit, buf.diag_cfg.reg, buf.diag_cfg.idx1, buf.diag_cfg.idx2, (uint32 *)&buf.diag_cfg.data);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_ext_diagCfg_t));
            break;

        case RTDRV_EXT_DIAG_REGARRAYFIELD_GET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = rtk_diag_regArrayField_get(buf.diag_cfg.unit, buf.diag_cfg.reg, buf.diag_cfg.idx1, buf.diag_cfg.idx2, buf.diag_cfg.field, (uint32 *)&buf.diag_cfg.data);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_ext_diagCfg_t));
            break;

        case RTDRV_EXT_DIAG_TABLEENTRY_GET:
            copy_from_user(&buf.diag_cfg, user, sizeof(rtdrv_ext_diagCfg_t));
            ret = rtk_diag_tableEntry_get(buf.diag_cfg.unit, buf.diag_cfg.table, buf.diag_cfg.addr, (uint32 *)&buf.diag_cfg.data);
            copy_to_user(user, &buf.diag_cfg, sizeof(rtdrv_ext_diagCfg_t));
            break;
        default:
            break;
    }

    copy_to_user(&((rtdrv_msgHdr_t *)user_in)->ret_code, &ret, sizeof(ret));

    return 0;
}

struct nf_sockopt_ops rtdrv_ext_sockopts = {
    { NULL, NULL }, PF_INET,
    RTDRV_EXT_BASE_CTL, RTDRV_EXT_SET_MAX+1, do_rtdrv_ext_set_ctl, NULL,
    RTDRV_EXT_BASE_CTL, RTDRV_EXT_GET_MAX+1, do_rtdrv_ext_get_ctl, NULL
};

