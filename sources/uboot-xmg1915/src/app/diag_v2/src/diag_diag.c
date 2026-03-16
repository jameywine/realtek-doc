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
 * $Revision: 81010 $
 * $Date: 2017-08-06 16:43:05 +0800 (Sun, 06 Aug 2017) $
 *
 * Purpose : Definition those Diagnostic command and APIs in the SDK diagnostic shell.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) Locol/Remote Loopback
 *           2) RTCT
 *
 */

/*
 * Include Files
 */
#include <common/debug/rt_log.h>
#include <common/rt_error.h>
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/util/rt_util.h>
#include <ioal/ioal_init.h>
#include <ioal/mem32.h>
#include <rtk/diag.h>
#include <rtk/port.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>
#if defined(CONFIG_SDK_RTL8295R)
  #include <hal/mac/rtl8295.h>
  #include <hal/phy/phy_rtl8295.h>
  #include <hal/phy/phy_rtl8295_patch.h>
#endif

#define PORT_NUM_IN_8218B   8

#ifdef CMD_DIAG_GET_CABLE_DOCTOR_PORT_PORTS_ALL
static void _cparser_cmd_diag_get_cable_doctor_port_ports_all_channelStatus_display(char *channName, rtk_rtctChannelStatus_t *channelStatus);

/*
 * diag get cable-doctor port ( <PORT_LIST:ports> | all )
 */
cparser_result_t cparser_cmd_diag_get_cable_doctor_port_ports_all(cparser_context_t *context,
    char **ports_ptr)
{
    uint32              unit = 0;
    int32               ret = RT_ERR_FAILED;
    rtk_port_t          port = 0;
    rtk_rtctResult_t    rtctResult;
    diag_portlist_t     portlist;


    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();
    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4), ret);

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        osal_memset(&rtctResult, 0, sizeof(rtk_rtctResult_t));
        if ((ret = rtk_diag_portRtctResult_get(unit, port, &rtctResult)) != RT_ERR_OK)
        {
            DIAG_ERR_PRINT(ret);
            return CPARSER_NOT_OK;
        }

        if (rtctResult.linkType == PORT_SPEED_1000M)
        {
            diag_util_mprintf("Port %2d (type GE):\n", port);
            diag_util_printf("  channel A: \n");
            diag_util_printf("    Status : ");
            if (rtctResult.ge_result.channelAShort)
                diag_util_printf("[Short]");
            if (rtctResult.ge_result.channelAOpen)
                diag_util_printf("[Open]");
            if (rtctResult.ge_result.channelAMismatch)
                diag_util_printf("[Mismatch]");
            if (rtctResult.ge_result.channelALinedriver)
                diag_util_printf("[Linedriver]");
            if (!(rtctResult.ge_result.channelAShort | rtctResult.ge_result.channelAOpen |
                rtctResult.ge_result.channelAMismatch | rtctResult.ge_result.channelALinedriver))
                diag_util_printf("[Normal]");
            diag_util_printf("\n");

            diag_util_printf("    Cable Length : %d.%02d (m)\n", rtctResult.ge_result.channelALen/100, rtctResult.ge_result.channelALen%100);

            diag_util_printf("  channel B: \n");
            diag_util_printf("    Status : ");
            if (rtctResult.ge_result.channelBShort)
                diag_util_printf("[Short]");
            if (rtctResult.ge_result.channelBOpen)
                diag_util_printf("[Open]");
            if (rtctResult.ge_result.channelBMismatch)
                diag_util_printf("[Mismatch]");
            if (rtctResult.ge_result.channelBLinedriver)
                diag_util_printf("[Linedriver]");
            if (!(rtctResult.ge_result.channelBShort | rtctResult.ge_result.channelBOpen |
                rtctResult.ge_result.channelBMismatch | rtctResult.ge_result.channelBLinedriver))
                diag_util_printf("[Normal]");
            diag_util_printf("\n");

            diag_util_printf("    Cable Length : %d.%02d (m)\n", rtctResult.ge_result.channelBLen/100, rtctResult.ge_result.channelBLen%100);

            diag_util_printf("  channel C: \n");
            diag_util_printf("    Status : ");
            if (rtctResult.ge_result.channelCShort)
                diag_util_printf("[Short]");
            if (rtctResult.ge_result.channelCOpen)
                diag_util_printf("[Open]");
            if (rtctResult.ge_result.channelCMismatch)
                diag_util_printf("[Mismatch]");
            if (rtctResult.ge_result.channelCLinedriver)
                diag_util_printf("[Linedriver]");
            if (!(rtctResult.ge_result.channelCShort | rtctResult.ge_result.channelCOpen |
                rtctResult.ge_result.channelCMismatch | rtctResult.ge_result.channelCLinedriver))
                diag_util_printf("[Normal]");
            diag_util_printf("\n");

            diag_util_printf("    Cable Length : %d.%02d (m)\n", rtctResult.ge_result.channelCLen/100, rtctResult.ge_result.channelCLen%100);

            diag_util_printf("  channel D: \n");
            diag_util_printf("    Status : ");
            if (rtctResult.ge_result.channelDShort)
                diag_util_printf("[Short]");
            if (rtctResult.ge_result.channelDOpen)
                diag_util_printf("[Open]");
            if (rtctResult.ge_result.channelDMismatch)
                diag_util_printf("[Mismatch]");
            if (rtctResult.ge_result.channelDLinedriver)
                diag_util_printf("[Linedriver]");
            if (!(rtctResult.ge_result.channelDShort | rtctResult.ge_result.channelDOpen |
                rtctResult.ge_result.channelDMismatch | rtctResult.ge_result.channelDLinedriver))
                diag_util_printf("[Normal]");
            diag_util_printf("\n");

            diag_util_printf("    Cable Length : %d.%02d (m)\n", rtctResult.ge_result.channelDLen/100, rtctResult.ge_result.channelDLen%100);

        }
        else if (rtctResult.linkType == PORT_SPEED_100M)
        {
            diag_util_mprintf("Port %2d (type FE):\n", port);

            diag_util_printf("  Rx channel : \n");
            diag_util_printf("    Status : ");
            if (rtctResult.fe_result.isRxShort)
                diag_util_printf("[Short]");
            if (rtctResult.fe_result.isRxOpen)
                diag_util_printf("[Open]");
            if (rtctResult.fe_result.isRxMismatch)
                diag_util_printf("[Mismatch]");
            if (rtctResult.fe_result.isRxLinedriver)
                diag_util_printf("[Linedriver]");
            if (!(rtctResult.fe_result.isRxShort | rtctResult.fe_result.isRxOpen |
                rtctResult.fe_result.isRxMismatch | rtctResult.fe_result.isRxLinedriver))
                diag_util_printf("[Normal]");
            diag_util_printf("\n");

            diag_util_printf("    Cable Length : %d.%02d (m)\n", rtctResult.fe_result.rxLen/100, rtctResult.fe_result.rxLen%100);

            diag_util_printf("  Tx channel : \n");
            diag_util_printf("    Status : ");
            if (rtctResult.fe_result.isTxShort)
                diag_util_printf("[Short]");
            if (rtctResult.fe_result.isTxOpen)
                diag_util_printf("[Open]");
            if (rtctResult.fe_result.isTxMismatch)
                diag_util_printf("[Mismatch]");
            if (rtctResult.fe_result.isTxLinedriver)
                diag_util_printf("[Linedriver]");
            if (!(rtctResult.fe_result.isTxShort | rtctResult.fe_result.isTxOpen |
                rtctResult.fe_result.isTxMismatch | rtctResult.fe_result.isTxLinedriver))
                diag_util_printf("[Normal]");
            diag_util_printf("\n");

            diag_util_printf("    Cable Length : %d.%02d (m)\n", rtctResult.fe_result.txLen/100, rtctResult.fe_result.txLen%100);
        }
        else
        {
            diag_util_mprintf("Port %2d (type XGE):\n", port);
            _cparser_cmd_diag_get_cable_doctor_port_ports_all_channelStatus_display("A", &rtctResult.channels_result.a);
            _cparser_cmd_diag_get_cable_doctor_port_ports_all_channelStatus_display("B", &rtctResult.channels_result.b);
            _cparser_cmd_diag_get_cable_doctor_port_ports_all_channelStatus_display("C", &rtctResult.channels_result.c);
            _cparser_cmd_diag_get_cable_doctor_port_ports_all_channelStatus_display("D", &rtctResult.channels_result.d);
        }
   }

    return CPARSER_OK;
}

/*
 * Display RTCT channel status
 */
static void
_cparser_cmd_diag_get_cable_doctor_port_ports_all_channelStatus_display(char *channName, rtk_rtctChannelStatus_t *channelStatus)
{
    uint32  is_normal = TRUE;

    diag_util_printf("  channel %s: \n", channName);
    diag_util_printf("    Status : ");
    if (channelStatus->channelShort)
    {
        diag_util_printf("[Short]");
        is_normal = FALSE;
    }
    if (channelStatus->channelOpen)
    {
        diag_util_printf("[Open]");
        is_normal = FALSE;
    }
    if (channelStatus->channelLowMismatch)
    {
        diag_util_printf("[Low-Mismatch]");
        is_normal = FALSE;
    }
    if (channelStatus->channelHighMismatch)
    {
        diag_util_printf("[High-Mismatch]");
        is_normal = FALSE;
    }
    if (channelStatus->channelCrossoverA)
    {
        diag_util_printf("[Crossover-A]");
        is_normal = FALSE;
    }
    if (channelStatus->channelCrossoverB)
    {
        diag_util_printf("[Crossover-B]");
        is_normal = FALSE;
    }
    if (channelStatus->channelCrossoverC)
    {
        diag_util_printf("[Crossover-C]");
        is_normal = FALSE;
    }
    if (channelStatus->channelCrossoverD)
    {
        diag_util_printf("[Crossover-D]");
        is_normal = FALSE;
    }

    if (is_normal == TRUE)
        diag_util_printf("[Normal]");
    diag_util_printf("\n");

    diag_util_printf("    Cable Length : %d.%02d (m)\n", channelStatus->channelLen/100, channelStatus->channelLen%100);
}
#endif

#ifdef CMD_DIAG_SET_CABLE_DOCTOR_PORT_PORTS_ALL_START
/*
 * diag set cable-doctor port ( <PORT_LIST:ports> | all ) start
 */
cparser_result_t cparser_cmd_diag_set_cable_doctor_port_ports_all_start(cparser_context_t *context,
    char **ports_ptr)
{
    uint32          unit = 0;
    int32           ret = RT_ERR_FAILED;
    diag_portlist_t               portlist;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4), ret);

    DIAG_UTIL_ERR_CHK(rtk_diag_rtctEnable_set(unit, &portlist.portmask), ret);

    return CPARSER_OK;
}
#endif

/*
 * diag dump table <UINT:index>
 */
#ifdef CMD_DIAG_DUMP_TABLE_INDEX
cparser_result_t cparser_cmd_diag_dump_table_index(cparser_context_t *context,
    uint32_t *index_ptr)
{
    int32 return_value;
    uint32 unit;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if (('t' == TOKEN_CHAR(2,0))&&(4 == TOKEN_NUM))
    {
        DIAG_UTIL_ERR_CHK(rtk_diag_table_whole_read(unit, *index_ptr), return_value);
    }
    else
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
}
#endif


#ifdef CMD_DIAG_DUMP_TABLE_INDEX_START_INDEX_END_INDEX_DETAIL
/*
 * diag dump table <UINT:index> <UINT:start_index> <UINT:end_index> { detail }
 */
cparser_result_t
cparser_cmd_diag_dump_table_index_start_index_end_index_detail(cparser_context_t *context,
    uint32_t *index_ptr,
    uint32_t *start_index_ptr,
    uint32_t *end_index_ptr)
{
    uint32      is_detail = FALSE;
    uint32      unit, i;
    int32       ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (TOKEN_NUM > 6)
    {
        for (i = 6; i < TOKEN_NUM; i++)
        {
            if (strncmp(TOKEN_STR(i), "d", 1) == 0) /* detail */
            {
                is_detail = TRUE;
            }
        }/* end for */
    }

    DIAG_UTIL_ERR_CHK(rtk_diag_tableEntry_read(unit, *index_ptr, *start_index_ptr, *end_index_ptr, is_detail), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_DIAG_WHOLEDUMP_MAC_REG_PHY_REG_SOC_REG_TABLE_ALL
cparser_result_t cparser_cmd_diag_wholedump_mac_reg_phy_reg_soc_reg_table_all(cparser_context_t *context)
{
    int32 return_value;
    uint32 unit;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if ('s' == TOKEN_CHAR(2,0))        /*Dump SoC registers*/
    {
            DIAG_UTIL_ERR_CHK(rtk_diag_peripheral_register_dump(unit), return_value);
    }
    else if('m' == TOKEN_CHAR(2,0))    /*Dump MAC registers*/
    {
          DIAG_UTIL_ERR_CHK(rtk_diag_reg_whole_read(unit), return_value);
    }
    else if('t'== TOKEN_CHAR(2,0))    /*Dump tables*/
    {
          DIAG_UTIL_ERR_CHK(rtk_diag_table_whole_read(unit, 0xff), return_value);
    }
    else if('p'== TOKEN_CHAR(2,0)) /*Dump PHY registers*/
    {
          DIAG_UTIL_ERR_CHK(rtk_diag_phy_reg_whole_read(unit), return_value);
    }
    else if('a'== TOKEN_CHAR(2,0)) /*Dump All*/
    {
          DIAG_UTIL_ERR_CHK(rtk_diag_peripheral_register_dump(unit), return_value);
          DIAG_UTIL_ERR_CHK(rtk_diag_reg_whole_read(unit), return_value);
          DIAG_UTIL_ERR_CHK(rtk_diag_phy_reg_whole_read(unit), return_value);
          DIAG_UTIL_ERR_CHK(rtk_diag_table_whole_read(unit, 0xff), return_value);
    }
    else
    {
        diag_util_printf("User config: Error!\n");
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
}
#endif

#ifdef CMD_DIAG_GET_SERDES_SDSID_LINK_STATUS
/*
 * diag get serdes <UINT:sdsId> link-status
 */
cparser_result_t
cparser_cmd_diag_get_serdes_sdsId_link_status(
    cparser_context_t *context,
    uint32_t *sdsId_ptr)
{
    uint32  unit, val;
    int32   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
    {
        uint32  addr;

        if ((*sdsId_ptr) > 13)
            return CPARSER_NOT_OK;

        addr = 0xa07c + (((*sdsId_ptr) / 2) * 0x400);
        if ((*sdsId_ptr) % 2 != 0)
            addr += 0x100;

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, addr, &val), ret);
        diag_util_mprintf("sds %d status: 0x%x\n", *sdsId_ptr, val);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_get_serdes_sdsId_link_status */
#endif

#ifdef CMD_DIAG_RESET_SERDES_SDSID
/*
 * diag reset serdes <UINT:sdsId>
 */
cparser_result_t
cparser_cmd_diag_reset_serdes_sdsId(
    cparser_context_t *context,
    uint32_t *sdsId_ptr)
{
    uint32  unit, val;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
    {
        uint32  addr, ofst;

        ofst = ((*sdsId_ptr) / 2) * 0x400;

        switch ((*sdsId_ptr))
        {
            case 0 ... 7:
            case 10 ... 11:
                addr = 0xa3c0 + ofst;
                ioal_mem32_read(unit, addr, &val);
                val |= ((1 << 20) | (1 << 22));
                val &= ~((1 << 21) | (1 << 23));
                ioal_mem32_write(unit, addr, val);
                val |= (0xf << 20);
                ioal_mem32_write(unit, addr, val);
                val &= ~(0xf << 20);
                ioal_mem32_write(unit, addr, val);

                addr = 0xa340 + ofst;
                ioal_mem32_read(unit, addr, &val);
                val |= ((1 << 24) | (1 << 26));
                val &= ~((1 << 25) | (1 << 27));
                ioal_mem32_write(unit, addr, val);
                val |= (0xf << 24);
                ioal_mem32_write(unit, addr, val);
                val &= ~(0xf << 24);
                ioal_mem32_write(unit, addr, val);
                break;
            case 8 ... 9:
                addr = 0xb3f8;
                ioal_mem32_read(unit, addr, &val);
                val |= ((1 << 16) | (1 << 18));
                val &= ~((1 << 17) | (1 << 19));
                ioal_mem32_write(unit, addr, val);
                val |= (0xf << 16);
                ioal_mem32_write(unit, addr, val);
                val &= ~(0xf << 16);
                ioal_mem32_write(unit, addr, val);

                val |= ((1 << 24) | (1 << 26));
                val &= ~((1 << 25) | (1 << 27));
                ioal_mem32_write(unit, addr, val);
                val |= (0xf << 24);
                ioal_mem32_write(unit, addr, val);
                val &= ~(0xf << 24);
                ioal_mem32_write(unit, addr, val);
                break;
            case 12 ... 13:
                addr = 0xbbf8;
                ioal_mem32_read(unit, addr, &val);
                val |= ((1 << 16) | (1 << 18));
                val &= ~((1 << 17) | (1 << 19));
                ioal_mem32_write(unit, addr, val);
                val |= (0xf << 16);
                ioal_mem32_write(unit, addr, val);
                val &= ~(0xf << 16);
                ioal_mem32_write(unit, addr, val);

                val |= ((1 << 24) | (1 << 26));
                val &= ~((1 << 25) | (1 << 27));
                ioal_mem32_write(unit, addr, val);
                val |= (0xf << 24);
                ioal_mem32_write(unit, addr, val);
                val &= ~(0xf << 24);
                ioal_mem32_write(unit, addr, val);
                break;
            default:
                return CPARSER_NOT_OK;
        }

        addr = 0xa004 + ofst;
        ioal_mem32_read(unit, addr, &val);
        val &= ~(0xFFFF << 16);
        val |= (0x7146 << 16);
        ioal_mem32_write(unit, addr, val);
        val &= ~(0xFFFF << 16);
        val |= (0x7106 << 16);
        ioal_mem32_write(unit, addr, val);

        addr = 0xa004 + ofst + 0x100;
        ioal_mem32_read(unit, addr, &val);
        val &= ~(0xFFFF << 16);
        val |= (0x7146 << 16);
        ioal_mem32_write(unit, addr, val);
        val &= ~(0xFFFF << 16);
        val |= (0x7106 << 16);
        ioal_mem32_write(unit, addr, val);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_reset_serdes_sdsId */
#endif

#ifdef CMD_DIAG_GET_SERDES_SDSID_RX_SYM_ERR
/*
 * diag get serdes <UINT:sdsId> rx-sym-err
 */
cparser_result_t
cparser_cmd_diag_get_serdes_sdsId_rx_sym_err(
    cparser_context_t *context,
    uint32_t *sdsId_ptr)
{
    uint32  unit, val;
    int32   ret;
    int8    i;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
    {
        uint32  addr;

        if ((*sdsId_ptr) > 13)
            return CPARSER_NOT_OK;

        diag_util_mprintf("sds %d\n", *sdsId_ptr);
        addr = 0xa070 + (((*sdsId_ptr) / 2) * 0x400);
        if ((*sdsId_ptr) % 2 != 0)
            addr += 0x100;

        for (i = 0x10; i <= 0x13; ++i)
        {
            DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, addr, i), ret);
            DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, addr, &val), ret);
            diag_util_mprintf(" CH%d: 0x%x\n", i - 0x10, val >> 16);
        }
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_get_serdes_sdsId_rx_sym_err */
#endif

#ifdef CMD_DIAG_GET_PHY_PHYID_SERDES_LINK_STATUS
/*
 * diag get phy <UINT:phyId> serdes link-status
 */
cparser_result_t
cparser_cmd_diag_get_phy_phyId_serdes_link_status(
    cparser_context_t *context,
    uint32_t *phyId_ptr)
{
    rtk_switch_devInfo_t    devInfo;
    rtk_port_t              port;
    uint32                  unit;
    uint32                  maxPage = 0x1fff, regVal, oriReg;
    uint32                  sdsPage[] = {0x40f, 0x42f};
    int32                   ret;
    uint8                   i;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    osal_memset(&devInfo, 0, sizeof(rtk_switch_devInfo_t));
    DIAG_UTIL_ERR_CHK(rtk_switch_deviceInfo_get(unit, &devInfo), ret);

    port = (*phyId_ptr) * PORT_NUM_IN_8218B;

    if (RTK_PORTMASK_IS_PORT_SET(devInfo.serdes.portmask, port))
        return CPARSER_OK;

    if (!RTK_PORTMASK_IS_PORT_SET(devInfo.ether.portmask, port))
        return CPARSER_OK;

    DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, 0, 3, &regVal), ret);
    if (regVal == 0xC981)
    {
        diag_util_mprintf("PHY Port ID: %d\n", port);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, maxPage, 30, &oriReg), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, 8), ret);

        for (i = 0; i < sizeof(sdsPage)/sizeof(uint32); ++i)
        {
            if ((ret = rtk_port_phyReg_get(unit, port, sdsPage[i], 0x16, &regVal)) != RT_ERR_OK)
            {
                rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                DIAG_ERR_PRINT(ret);
                return CPARSER_NOT_OK;
            }
            diag_util_mprintf(" status %d: 0x%04x\n", i, regVal);
        }

        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg), ret);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_get_phy_phyId_serdes_link_status */
#endif

#ifdef CMD_DIAG_GET_PHY_PHYID_SERDES_RX_SYM_ERR
/*
 * diag get phy <UINT:phyId> serdes rx-sym-err
 */
cparser_result_t
cparser_cmd_diag_get_phy_phyId_serdes_rx_sym_err(
    cparser_context_t *context,
    uint32_t *phyId_ptr)
{
    rtk_switch_devInfo_t    devInfo;
    rtk_port_t              port;
    uint32                  unit;
    uint32                  maxPage = 0x1fff, regVal, oriReg;
    uint32                  sdsPage[] = {0x40f, 0x42f};
    int32                   ret;
    uint8                   i, j;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    osal_memset(&devInfo, 0, sizeof(rtk_switch_devInfo_t));
    DIAG_UTIL_ERR_CHK(rtk_switch_deviceInfo_get(unit, &devInfo), ret);

    port = (*phyId_ptr) * PORT_NUM_IN_8218B;

    if (RTK_PORTMASK_IS_PORT_SET(devInfo.serdes.portmask, port))
        return CPARSER_OK;

    if (!RTK_PORTMASK_IS_PORT_SET(devInfo.ether.portmask, port))
        return CPARSER_OK;

    DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, 0, 3, &regVal), ret);
    if (regVal == 0xC981)
    {
        diag_util_mprintf("PHY Port ID: %d\n", port);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, maxPage, 30, &oriReg), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, 8), ret);

        for (i = 0; i < sizeof(sdsPage)/sizeof(uint32); ++i)
        {
            diag_util_mprintf(" Sds ID: %d\n", i);

            for (j = 0x10; j <= 0x13; ++j)
            {
                if ((ret = rtk_port_phyReg_set(unit, port, sdsPage[i], 0x10, j)) != RT_ERR_OK)
                {
                    rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                    DIAG_ERR_PRINT(ret);
                    return CPARSER_NOT_OK;
                }

                if ((ret = rtk_port_phyReg_get(unit, port, sdsPage[i], 0x11, &regVal)) != RT_ERR_OK)
                {
                    rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                    DIAG_ERR_PRINT(ret);
                    return CPARSER_NOT_OK;
                }
                diag_util_mprintf("  CH%d: 0x%04x\n", (j - 0x10), regVal);
            }
        }

        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg), ret);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_get_phy_phyId_serdes_rx_sym_err */
#endif

#ifdef CMD_DIAG_CLEAR_PHY_PHYID_SERDES_RX_SYM_ERR
/*
 * diag clear phy <UINT:phyId> serdes rx-sym-err
 */
cparser_result_t
cparser_cmd_diag_clear_phy_phyId_serdes_rx_sym_err(
    cparser_context_t *context,
    uint32_t *phyId_ptr)
{
    rtk_switch_devInfo_t    devInfo;
    rtk_port_t              port;
    uint32                  unit;
    uint32                  maxPage = 0x1fff, regVal, oriReg;
    uint32                  sdsPage[] = {0x40f, 0x42f};
    int32                   ret;
    uint8                   i, j;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    osal_memset(&devInfo, 0, sizeof(rtk_switch_devInfo_t));
    DIAG_UTIL_ERR_CHK(rtk_switch_deviceInfo_get(unit, &devInfo), ret);

    port = (*phyId_ptr) * PORT_NUM_IN_8218B;

    if (RTK_PORTMASK_IS_PORT_SET(devInfo.serdes.portmask, port))
        return CPARSER_OK;

    if (!RTK_PORTMASK_IS_PORT_SET(devInfo.ether.portmask, port))
        return CPARSER_OK;

    DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, 0, 3, &regVal), ret);
    if (regVal == 0xC981)
    {
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, maxPage, 30, &oriReg), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, 8), ret);

        for (i = 0; i < sizeof(sdsPage)/sizeof(uint32); ++i)
        {
            for (j = 0x10; j <= 0x13; ++j)
            {
                if ((ret = rtk_port_phyReg_set(unit, port, sdsPage[i], 0x10, j)) != RT_ERR_OK)
                {
                    rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                    DIAG_ERR_PRINT(ret);
                    return CPARSER_NOT_OK;
                }

                if ((ret = rtk_port_phyReg_get(unit, port, sdsPage[i], 0x11, &regVal)) != RT_ERR_OK)
                {
                    rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                    DIAG_ERR_PRINT(ret);
                    return CPARSER_NOT_OK;
                }
            }
        }

        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg), ret);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_clear_phy_phyId_serdes_rx_sym_err */
#endif

#ifdef CMD_DIAG_GET_PHY_PORT_PORTS_ALL_RX_CNT
/*
 * diag get phy port ( <PORT_LIST:ports> | all ) rx-cnt
 */
cparser_result_t
cparser_cmd_diag_get_phy_port_ports_all_rx_cnt(
    cparser_context_t *context,
    char **ports_ptr)
{
    rtk_switch_devInfo_t    devInfo;
    diag_portlist_t         portlist;
    rtk_port_t              port;
    uint32                  unit;
    uint32                  maxPage = 0x1fff, regVal, oriReg;
    int32                   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4), ret);

    osal_memset(&devInfo, 0, sizeof(rtk_switch_devInfo_t));
    DIAG_UTIL_ERR_CHK(rtk_switch_deviceInfo_get(unit, &devInfo), ret);

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        if (RTK_PORTMASK_IS_PORT_SET(devInfo.serdes.portmask, port))
            continue;

        if (!RTK_PORTMASK_IS_PORT_SET(devInfo.ether.portmask, port))
            continue;

        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, 0, 3, &regVal), ret);
        if (regVal == 0xC981)
        {
            diag_util_mprintf("Port ID: %d\n", port);

            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, maxPage, 30, &oriReg), ret);
            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, 0), ret);

            if ((ret = rtk_port_phyReg_get(unit, port, 0xc81, 0x10, &regVal)) != RT_ERR_OK)
            {
                rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                DIAG_ERR_PRINT(ret);
                return CPARSER_NOT_OK;
            }
            diag_util_mprintf(" good1: 0x%x\n", regVal);

            if ((ret = rtk_port_phyReg_get(unit, port, 0xc81, 0x11, &regVal)) != RT_ERR_OK)
            {
                rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                DIAG_ERR_PRINT(ret);
                return CPARSER_NOT_OK;
            }
            diag_util_mprintf(" good2: 0x%x\n", regVal);

            if ((ret = rtk_port_phyReg_get(unit, port, 0xc81, 0x12, &regVal)) != RT_ERR_OK)
            {
                rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                DIAG_ERR_PRINT(ret);
                return CPARSER_NOT_OK;
            }
            diag_util_mprintf("    err: 0x%x\n", regVal);

            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg), ret);
        }
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_get_phy_port_ports_all_rx_cnt */
#endif

#ifdef CMD_DIAG_CLEAR_PHY_PORT_PORTS_ALL_RX_CNT
/*
 * diag clear phy port ( <PORT_LIST:ports> | all ) rx-cnt
 */
cparser_result_t
cparser_cmd_diag_clear_phy_port_ports_all_rx_cnt(
    cparser_context_t *context,
    char **ports_ptr)
{
    rtk_switch_devInfo_t    devInfo;
    diag_portlist_t         portlist;
    rtk_port_t              port;
    uint32                  unit;
    uint32                  maxPage = 0x1fff, regVal, oriReg;
    int32                   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4), ret);

    osal_memset(&devInfo, 0, sizeof(rtk_switch_devInfo_t));
    DIAG_UTIL_ERR_CHK(rtk_switch_deviceInfo_get(unit, &devInfo), ret);

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        if (RTK_PORTMASK_IS_PORT_SET(devInfo.serdes.portmask, port))
            continue;

        if (!RTK_PORTMASK_IS_PORT_SET(devInfo.ether.portmask, port))
            continue;

        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, 0, 3, &regVal), ret);
        if (regVal == 0xC981)
        {
            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, maxPage, 30, &oriReg), ret);
            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, 0), ret);

            if ((ret = rtk_port_phyReg_set(unit, port, 0xc80, 0x11, 0x73)) != RT_ERR_OK)
            {
                rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                DIAG_ERR_PRINT(ret);
                return CPARSER_NOT_OK;
            }

            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg), ret);
        }
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_clear_phy_port_ports_all_rx_cnt */
#endif

#ifdef CMD_DIAG_SET_PHY_PORT_PORTS_ALL_RX_CNT_MAC_TX_PHY_RX
/*
 * diag set phy port ( <PORT_LIST:ports> | all ) rx-cnt ( mac-tx | phy-rx )
 */
cparser_result_t
cparser_cmd_diag_set_phy_port_ports_all_rx_cnt_mac_tx_phy_rx(
    cparser_context_t *context,
    char **ports_ptr)
{
    rtk_switch_devInfo_t    devInfo;
    diag_portlist_t         portlist;
    rtk_port_t              port;
    uint32                  unit, val;
    uint32                  maxPage = 0x1fff, regVal, oriReg;
    int32                   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_ERR_CHK(DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4), ret);

    osal_memset(&devInfo, 0, sizeof(rtk_switch_devInfo_t));
    DIAG_UTIL_ERR_CHK(rtk_switch_deviceInfo_get(unit, &devInfo), ret);

    if ('m' == TOKEN_CHAR(6, 0))
        val = 0x6;
    else
        val = 0x2;

    DIAG_UTIL_PORTMASK_SCAN(portlist, port)
    {
        if (RTK_PORTMASK_IS_PORT_SET(devInfo.serdes.portmask, port))
            continue;

        if (!RTK_PORTMASK_IS_PORT_SET(devInfo.ether.portmask, port))
            continue;

        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, 0, 3, &regVal), ret);
        if (regVal == 0xC981)
        {
            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_get(unit, port, maxPage, 30, &oriReg), ret);
            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, 0), ret);

            if ((ret = rtk_port_phyReg_set(unit, port, 0xc80, 0x10, val)) != RT_ERR_OK)
            {
                rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg);
                DIAG_ERR_PRINT(ret);
                return CPARSER_NOT_OK;
            }

            DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, port, maxPage, 30, oriReg), ret);
        }
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_set_phy_port_ports_all_rx_cnt_mac_tx_phy_rx */
#endif

#ifdef CMD_DIAG_SET_SERDES_SDSID_10G_SQUARE_STATE_DISABLE_ENABLE
void cmd_diag_10g_rst(uint32 unit, uint32_t sds)
{
    uint32  ofst, val, i;

    if (8 == sds)
        ofst = 0x0;
    else
        ofst = 0x800;


    ioal_mem32_read(unit, 0xb320 + ofst, &val);
    val &= ~(1 << 3);
    ioal_mem32_write(unit, 0xb320 + ofst, val);

    ioal_mem32_read(unit, 0xb340 + ofst, &val);
    val |= (1 << 15);
    ioal_mem32_write(unit, 0xb340 + ofst, val);
    /* for delay */
    for (i = 0; i < 10000; ++i);
    ioal_mem32_read(unit, 0xb340 + ofst, &val);
    val &= ~(1 << 15);
    ioal_mem32_write(unit, 0xb340 + ofst, val);

    ioal_mem32_read(unit, 0xb284 + ofst, &val);
    val |= (1 << 12);
    ioal_mem32_write(unit, 0xb284 + ofst, val);
    /* for delay */
    for (i = 0; i < 10000; ++i);
    ioal_mem32_read(unit, 0xb284 + ofst, &val);
    val &= ~(1 << 12);
    ioal_mem32_write(unit, 0xb284 + ofst, val);

    return;
}

void cmd_diag_10g_square_disable(uint32 unit, uint32_t sds)
{
    uint32  ofst, val;

    if (8 == sds)
        ofst = 0x0;
    else
        ofst = 0x800;

    ioal_mem32_read(unit, 0xb320 + ofst, &val);
    val &= ~(1 << 13);
    val &= ~(1 << 14);
    ioal_mem32_write(unit, 0xb320 + ofst, val);
    return;
}

void cmd_diag_10g_prbs9_disable(uint32 unit, uint32_t sds)
{
    uint32  ofst, val;

    if (8 == sds)
        ofst = 0x0;
    else
        ofst = 0x800;

    ioal_mem32_read(unit, 0xb254 + ofst, &val);
    val &= ~(0x3f);
    ioal_mem32_write(unit, 0xb254 + ofst, val);
    return;
}

void cmd_diag_10g_prbs31_disable(uint32 unit, uint32_t sds)
{
    uint32  ofst, val;

    if (8 == sds)
        ofst = 0x0;
    else
        ofst = 0x800;

    ioal_mem32_read(unit, 0xb254 + ofst, &val);
    val &= ~(0x3 << 4);
    ioal_mem32_write(unit, 0xb254 + ofst, val);

    ioal_mem32_read(unit, 0xb280 + ofst, &val);
    val &= ~(0x3 << 13);
    ioal_mem32_write(unit, 0xb280 + ofst, val);
    return;
}

/*
 * diag set serdes <UINT:sdsId> 10g-square state ( disable | enable )
 */
cparser_result_t
cparser_cmd_diag_set_serdes_sdsId_10g_square_state_disable_enable(
    cparser_context_t *context,
    uint32_t *sdsId_ptr)
{
    uint32  unit, ofst, val;
    int32   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);

    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID))
    {
        switch (*sdsId_ptr)
        {
            case 8:
                ofst = 0x0;
                break;
            case 12:
                ofst = 0x800;
                break;
            default:
                return CPARSER_NOT_OK;
        }

        if ('d' == TOKEN_CHAR(6, 0))
        {
            cmd_diag_10g_square_disable(unit, *sdsId_ptr);
            return CPARSER_OK;
        }

        cmd_diag_10g_prbs9_disable(unit, *sdsId_ptr);
        cmd_diag_10g_prbs31_disable(unit, *sdsId_ptr);
        cmd_diag_10g_rst(unit, *sdsId_ptr);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb254 + ofst, &val), ret);
        val |= (1 << 1);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb254 + ofst, val), ret);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb280 + ofst, &val), ret);
        val |= (0x3 << 13);
        val &= ~(0xF << 4);
        val |= (8 << 4);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb280 + ofst, val), ret);
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_set_serdes_sdsId_10g_square_state_disable_enable */
#endif

#ifdef CMD_DIAG_SET_SERDES_SDSID_10G_PRBS9_STATE_DISABLE_ENABLE
/*
 * diag set serdes <UINT:sdsId> 10g-prbs9 state ( disable | enable )
 */
cparser_result_t
cparser_cmd_diag_set_serdes_sdsId_10g_prbs9_state_disable_enable(
    cparser_context_t *context,
    uint32_t *sdsId_ptr)
{
    uint32  unit, ofst, val;
    int32   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID))
    {
        switch (*sdsId_ptr)
        {
            case 8:
                ofst = 0x0;
                break;
            case 12:
                ofst = 0x800;
                break;
            default:
                return CPARSER_NOT_OK;
        }

        if ('d' == TOKEN_CHAR(6, 0))
        {
            cmd_diag_10g_prbs9_disable(unit, *sdsId_ptr);
            return CPARSER_OK;
        }

        cmd_diag_10g_square_disable(unit, *sdsId_ptr);
        cmd_diag_10g_prbs31_disable(unit, *sdsId_ptr);
        cmd_diag_10g_rst(unit, *sdsId_ptr);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb254 + ofst, &val), ret);
        val |= (0x3f);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb254 + ofst, val), ret);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb254 + ofst, &val), ret);
        diag_util_printf("Serdes %d prbs9_rx_err_cnt %d\n", *sdsId_ptr, (val >> 16));
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_set_serdes_sdsId_10g_prbs9_state_disable_enable */
#endif

#ifdef CMD_DIAG_SET_SERDES_SDSID_10G_PRBS31_STATE_DISABLE_ENABLE
/*
 * diag set serdes <UINT:sdsId> 10g-prbs31 state ( disable | enable )
 */
cparser_result_t
cparser_cmd_diag_set_serdes_sdsId_10g_prbs31_state_disable_enable(
    cparser_context_t *context,
    uint32_t *sdsId_ptr)
{
    uint32  unit, ofst, val, i;
    int32   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID))
    {
        switch (*sdsId_ptr)
        {
            case 8:
                ofst = 0x0;
                break;
            case 12:
                ofst = 0x800;
                break;
            default:
                return CPARSER_NOT_OK;
        }

        if ('d' == TOKEN_CHAR(6, 0))
        {
            cmd_diag_10g_prbs31_disable(unit, *sdsId_ptr);
            return CPARSER_OK;
        }

        cmd_diag_10g_square_disable(unit, *sdsId_ptr);
        cmd_diag_10g_prbs9_disable(unit, *sdsId_ptr);
        cmd_diag_10g_rst(unit, *sdsId_ptr);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb320 + ofst, &val), ret);
        val &= ~(0x1 << 3);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb320 + ofst, val), ret);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb340 + ofst, &val), ret);
        val |= (0x1 << 15);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb340 + ofst, val), ret);
        /* for delay */
        for (i = 0; i < 10000; ++i);
        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb340 + ofst, &val), ret);
        val &= ~(0x1 << 15);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb340 + ofst, val), ret);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb284 + ofst, &val), ret);
        val |= (0x1 << 12);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb284 + ofst, val), ret);
        /* for delay */
        for (i = 0; i < 10000; ++i);
        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb284 + ofst, &val), ret);
        val &= ~(0x1 << 12);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb284 + ofst, val), ret);


        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb254 + ofst, &val), ret);
        val |= (0x3 << 4);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb254 + ofst, val), ret);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb280 + ofst, &val), ret);
        val |= (0x3 << 13);
        val &= ~(0x1F << 8);
        val |= (0x2 << 8);
        DIAG_UTIL_ERR_CHK(ioal_mem32_write(unit, 0xb280 + ofst, val), ret);

        DIAG_UTIL_ERR_CHK(ioal_mem32_read(unit, 0xb254 + ofst, &val), ret);
        diag_util_printf("Serdes %d prbs31_rx_err_cnt %d\n", *sdsId_ptr, (val >> 16));
    }

    return CPARSER_OK;
}   /* end of cparser_cmd_diag_set_serdes_sdsId_10g_prbs31_state_disable_enable */
#endif


#ifdef CONFIG_SDK_RTL8295R
/*
 * diag set 8295r sff-8431-test port <UINT:port> serdes <UINT:sdsId> pattern ( init | square8180 | prbs9 | prbs31 | disable )
 */
cparser_result_t
cparser_cmd_diag_set_8295r_sff_8431_test_port_port_serdes_sdsId_pattern_init_square8180_prbs9_prbs31_disable(cparser_context_t *context,
    uint32_t *port_ptr,
    uint32_t *sdsId_ptr)
{
    uint32  unit, mdxMacId;
    int32   ret, len;
    uint32  sdsPageOffsetLst[] = {0, 512};
    uint32  offset;
    char    *pattern = TOKEN_STR(9);

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if (*sdsId_ptr > 1)
    {
        diag_util_printf("Invalid serdes ID %u\n", *sdsId_ptr);
        return CPARSER_OK;
    }

    mdxMacId = *port_ptr;

    len = strlen(pattern);

    if (!strncmp("disable", pattern, len))
    {
        offset = sdsPageOffsetLst[*sdsId_ptr];
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (277+offset), 18, 0x0000), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (281+offset), 22, 0x0000), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (281+offset), 23, 0x0000), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (280+offset), 16, 0x0000), ret);
    }
    else if (!strncmp("init", pattern, len))
    {
        offset = sdsPageOffsetLst[0];
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (447+offset), 19, 0x0), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (388+offset), 21, 0x4A8D), ret);
        offset = sdsPageOffsetLst[1];
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (447+offset), 19, 0x0), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (388+offset), 21, 0x4A8D), ret);
    }
    else if (!strncmp("square8180", pattern, len))
    {
        offset = sdsPageOffsetLst[*sdsId_ptr];
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (277+offset), 18, 0x0002), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (280+offset), 16, 0x6280), ret);
    }
    else if (!strncmp("prbs9", pattern, len))
    {
        offset = sdsPageOffsetLst[*sdsId_ptr];
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (277+offset), 18, 0x00C0), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (281+offset), 22, 0x0002), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (281+offset), 23, 0x0002), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (280+offset), 16, 0x6200), ret);
    }
    else if (!strncmp("prbs31", pattern, len))
    {
        offset = sdsPageOffsetLst[*sdsId_ptr];
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (277+offset), 18, 0x0030), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (281+offset), 22, 0x0002), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (281+offset), 23, 0x0002), ret);
        DIAG_UTIL_ERR_CHK(rtk_port_phyReg_set(unit, mdxMacId, (280+offset), 16, 0x6200), ret);
    }
    return CPARSER_OK;
}
#endif

#ifdef CONFIG_SDK_RTL8295R
/*
 * diag set 8295r rx-cali dac-long-cable-offset <UINT:offset>
 */
cparser_result_t
cparser_cmd_diag_set_8295r_rx_cali_dac_long_cable_offset_offset(cparser_context_t *context,
    uint32_t *offset_ptr)
{
    uint32  unit;
    int32   ret;
    phy_8295_rxCaliConf_t   rxCaliConf;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if ((ret = phy_8295r_rxCaliConf_get(unit, &rxCaliConf)) != RT_ERR_OK)
    {
        diag_util_printf("Get RX-Cali. config failed: 0x%x\n", ret);
        return CPARSER_OK;
    }

    rxCaliConf.s1rxCaliDacLongCableOffset = *offset_ptr;

    if ((ret = phy_8295r_rxCaliConf_set(unit, &rxCaliConf)) != RT_ERR_OK)
    {
        diag_util_printf("Set RX-Cali. config failed: 0x%x\n", ret);
    }

    return CPARSER_OK;

}
#endif


#ifdef CONFIG_SDK_RTL8295R
/*
 * diag get 8295r rx-cali dac-long-cable-offset
 */
cparser_result_t
cparser_cmd_diag_get_8295r_rx_cali_dac_long_cable_offset(cparser_context_t *context)
{
    uint32  unit;
    int32   ret;
    phy_8295_rxCaliConf_t   rxCaliConf;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    if ((ret = phy_8295r_rxCaliConf_get(unit, &rxCaliConf)) != RT_ERR_OK)
    {
        diag_util_printf("Get RX-Cali. config failed: 0x%x\n", ret);
        return CPARSER_OK;
    }

    diag_util_printf("s1rxCaliDacLongCableOffset: %u\n", rxCaliConf.s1rxCaliDacLongCableOffset);
    return CPARSER_OK;

}
#endif

#ifdef CONFIG_SDK_RTL8295R
/*
 * diag set 8295r rx-cali <UINT:port> <UINT:sdsId> ( enable | disable )
 */
cparser_result_t
cparser_cmd_diag_set_8295r_rx_cali_port_sdsId_enable_disable(cparser_context_t *context,
    uint32_t *port_ptr,
    uint32_t *sdsId_ptr)
{
    uint32  unit, len, dummy = 0;
    int32   ret;
    char    *str;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    str = TOKEN_STR(6);
    len = strlen(str);
    if (!strncmp("enable", str, len))
    {
        DIAG_UTIL_ERR_CHK(phy_8295_diag_set(unit, *port_ptr, dummy, *sdsId_ptr, (uint8 *)"port_cali_enable"), ret);
    }
    else if (!strncmp("disable", str, len))
    {
        DIAG_UTIL_ERR_CHK(phy_8295_diag_set(unit, *port_ptr, dummy, *sdsId_ptr, (uint8 *)"port_cali_disable"), ret);
    }

    return CPARSER_OK;
}
#endif


#ifdef CONFIG_SDK_RTL8295R
/*
 * diag set 8295r rx-cali <UINT:port> <UINT:sdsId> start
 */
cparser_result_t
cparser_cmd_diag_set_8295r_rx_cali_port_sdsId_start(cparser_context_t *context,
    uint32_t *port_ptr,
    uint32_t *sdsId_ptr)
{
    uint32  unit, dummy = 0;
    int32   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(phy_8295_diag_set(unit, *port_ptr, dummy, *sdsId_ptr, (uint8 *)"rxCali"), ret);
    return CPARSER_OK;
}
#endif


