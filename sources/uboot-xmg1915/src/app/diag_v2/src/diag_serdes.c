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
 * $Revision: 71708 $
 * $Date: 2016-09-19 11:31:17 +0800 (Mon, 19 Sep 2016) $
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
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>
#include <hal/mac/serdes.h>


#ifdef CMD_SERDES_GET_ID_SDSID_PAGE_PAGE_REG_REG
/*
 * serdes get id <UINT:sdsId> page <UINT:page> reg <UINT:reg>
 */
cparser_result_t
cparser_cmd_serdes_get_id_sdsId_page_page_reg_reg(cparser_context_t *context,
    uint32_t *sdsId_ptr,
    uint32_t *page_ptr,
    uint32_t *reg_ptr)
{
    uint32  unit;
    int32   ret;
    uint32  data;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(hal_serdes_reg_get(unit, *sdsId_ptr, *page_ptr, *reg_ptr, &data), ret);

    diag_util_printf("unit %u serdes %u page %u(0x%X) reg %u(0x%X)\n", unit, *sdsId_ptr, *page_ptr, *page_ptr, *reg_ptr, *reg_ptr);
    diag_util_printf("Data: 0x%04X\n", data);

    return CPARSER_OK;
}
#endif /* CMD_SERDES_GET_ID_SDSID_PAGE_PAGE_REG_REG */



#ifdef CMD_SERDES_SET_ID_SDSID_PAGE_PAGE_REG_REG_DATA_DATA
/*
 * serdes set id <UINT:sdsId> page <UINT:page> reg <UINT:reg> data <UINT:data>
 */
cparser_result_t
cparser_cmd_serdes_set_id_sdsId_page_page_reg_reg_data_data(cparser_context_t *context,
    uint32_t *sdsId_ptr,
    uint32_t *page_ptr,
    uint32_t *reg_ptr,
    uint32_t *data_ptr)
{
    uint32  unit;
    int32   ret;

    DIAG_UTIL_PARAM_CHK();
    DIAG_OM_GET_UNIT_ID(unit);
    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(hal_serdes_reg_set(unit, *sdsId_ptr, *page_ptr, *reg_ptr, *data_ptr), ret);
    diag_util_printf("unit %u serdes %u page %u(0x%X) reg %u(0x%X) set data 0x%04X\n",
        unit, *sdsId_ptr, *page_ptr, *page_ptr, *reg_ptr, *reg_ptr, *data_ptr);

    return CPARSER_OK;
}
#endif /* CMD_SERDES_SET_ID_SDSID_PAGE_PAGE_REG_REG_DATA_DATA */




