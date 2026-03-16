/*
 * Copyright (C) 2009-2017 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 75125 $
 * $Date: 2017-01-09 15:13:30 +0800 (Mon, 09 Jan 2017) $
 *
 * Purpose : Definition system related command and APIs in the SDK diagnostic shell.
 *
 * Feature : The file have include the following module and sub-modules
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
#include <stdio.h>
#include <hwp/hw_profile.h>
#include <hwp/hwp_util.h>
#include <osal/memory.h>
#ifdef CONFIG_SDK_STACKING_PROTO
  #include <stk/src/rtstk_util.h>
#endif

/*
  * sys dump hwp ( all | info | unit | parsed_info )
  */
cparser_result_t cparser_cmd_sys_dump_hwp_all_info_unit_parsed_info(cparser_context_t *context)
{
    uint32  unit = 0;
    int     len;
    char    *str;

    DIAG_OM_GET_UNIT_ID(unit);

    str = TOKEN_STR(3);
    len = strlen(str);

    if (!strncmp("info", str, len))
    {
        hwp_info_show(unit);
    }
    else if (!strncmp("parsed_info", str, len))
    {
        hwp_parsedInfo_show(unit);
    }
    else if (!strncmp("unit", str, len))
    {
        hwp_unit_show(unit);
    }
    else if (!strncmp("all", str, len))
    {
        hwp_info_show(unit);
        hwp_unit_show(unit);
        hwp_parsedInfo_show(unit);
    }

    return CPARSER_OK;

}
cparser_result_t cparser_cmd_sys_load_port_config_port_ports_all(cparser_context_t *context,
    char **ports_ptr)
{
    uint32 unit = 0;
    int32 file_length = 0;
    uint32 port = 0;
    uint32 complete_size = 0;
    FILE *pFile = NULL;
    uint8 *image = NULL;
    diag_portlist_t portlist;

    DIAG_OM_GET_UNIT_ID(unit);

    pFile = fopen("/lib/modules/PHY_image", "rb" );

    if (pFile != NULL)
    {
        if(fseek(pFile, 0, SEEK_END))
        {
            diag_util_printf("Seek failed.");
            fclose(pFile);
            return CPARSER_NOT_OK;
        }

        if ((file_length = ftell(pFile)) < 0)
        {
            fclose(pFile);
            return CPARSER_NOT_OK;
        }

        osal_printf("File_length = %d\n", file_length);
        image = (uint8*)osal_alloc(file_length * sizeof(uint8));

        if (image == NULL)
        {
            fclose(pFile);
            return CPARSER_NOT_OK;
        }

        fseek(pFile, 0, SEEK_SET);
        if (0 == fread(image, file_length, sizeof(uint8), pFile))
        {
            osal_printf("Read file size less than file_length(%d) \n", file_length);
            fclose(pFile);
            osal_free(image);
            return CPARSER_NOT_OK;
        }

        if (RT_ERR_OK != DIAG_UTIL_EXTRACT_PORTLIST(portlist, 4))
        {
            fclose(pFile);
            osal_free(image);
            return CPARSER_NOT_OK;
        }
        DIAG_UTIL_PORTMASK_SCAN(portlist, port)
        {
            do
            {
                if(RT_ERR_OK != rtk_port_imageFlash_load(unit, port, file_length, (image + complete_size)))
                {
                    fclose(pFile);
                    osal_free(image);
                    return CPARSER_NOT_OK;
                }
                complete_size += RTK_PORT_FLASHIMG_SIZE;
            }while(complete_size < file_length);
            complete_size = 0;
        }
        osal_free(image);
        fclose(pFile);
    }
    else
    {
        diag_util_printf("File not exist.");
        return CPARSER_NOT_OK;
    }

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_rtstk_dc_dcbox_ta_cfg_port_param(cparser_context_t *context,
    uint32_t *param_ptr)
{
#ifdef CONFIG_SDK_STACKING_PROTO

    uint32  unit = 0;
    int     len;
    char    *str;
    uint32  parameter_num;

    DIAG_OM_GET_UNIT_ID(unit);

    str = TOKEN_STR(1);
    len = strlen(str);


    if(param_ptr != NULL)
        parameter_num = *param_ptr;
    else
        parameter_num = unit;

    if (!strncmp("dc", str, len))
    {
        rtstk_show_dc(parameter_num);
    }
    else if (!strncmp("dcbox", str, len))
    {
        rtstk_show_dcbox(2, NULL, NULL);
    }
    else if (!strncmp("ta", str, len))
    {
        rtstk_show_ta(parameter_num);
    }
    else if (!strncmp("cfg", str, len))
    {
        rtstk_show_cfg(parameter_num);
    }
    else if (!strncmp("port", str, len))
    {
        rtstk_show_port(parameter_num);
    }
#endif //CONFIG_SDK_STACKING_PROTO

    return CPARSER_OK;

}


