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
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Definition those RTL8231 command and APIs in the SDK diagnostic shell.
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
#include <private/drv/rtl8231/rtl8231.h>
#include <drv/gpio/gpio.h>
#include <drv/gpio/ext_gpio.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>

#ifdef CMD_RTL8231_GET_MDC_PHY_ID_PAGE_REGISTER
/*
 * rtl8231 get mdc <UINT:phy_id> <UINT:page> <UINT:register>
 */
cparser_result_t cparser_cmd_rtl8231_get_mdc_phy_id_page_register(cparser_context_t *context,
    uint32_t *phy_id_ptr,
    uint32_t *page_ptr,
    uint32_t *register_ptr)
{
    uint32      unit = 0;
    uint32      reg_data = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((phy_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    //DIAG_UTIL_PARAM_RANGE_CHK((*phy_id_ptr > ??), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((page_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*page_ptr > 31), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((register_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*register_ptr > 31), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_rtl8231_mdc_read(unit, *phy_id_ptr, *page_ptr, *register_ptr, &reg_data), ret);
    diag_util_printf("    0x%04X  ", reg_data);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_RTL8231_SET_MDC_PHY_ID_PAGE_REGISTER_DATA
/*
 * rtl8231 set mdc <UINT:phy_id> <UINT:page> <UINT:register> <UINT:data>
 */
cparser_result_t cparser_cmd_rtl8231_set_mdc_phy_id_page_register_data(cparser_context_t *context,
    uint32_t *phy_id_ptr,
    uint32_t *page_ptr,
    uint32_t *register_ptr,
    uint32_t *data_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((phy_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    //DIAG_UTIL_PARAM_RANGE_CHK((*phy_id_ptr > ??), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((page_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*page_ptr > 31), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((register_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*register_ptr > 31), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((data_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_ERR_CHK(drv_rtl8231_mdc_write(unit, *phy_id_ptr, *page_ptr, *register_ptr, *data_ptr), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_DUMP_DEV_DEV_ID
/*
 * ext-gpio dump dev <UINT:dev_id>
 */
cparser_result_t cparser_cmd_ext_gpio_dump_dev_dev_id(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0, value, ready;
    uint32      gpio_idx, gpio_data = 0;
    int32       ret = RT_ERR_FAILED;
    rtk_enable_t    enable;
    drv_extGpio_devConf_t   extGpio_devConfData;
    drv_extGpio_conf_t      extGpio_pinConfData;
    drv_gpio_direction_t    gpio_direction = 0;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    if ((ret = drv_extGpio_dev_get(unit, *dev_id_ptr, &extGpio_devConfData)) != RT_ERR_OK)
    {
        diag_util_mprintf("    Device is invalid\n");
    }
    else
    {
        DIAG_UTIL_ERR_CHK(drv_extGpio_devReady_get(unit, *dev_id_ptr, &ready), ret);
        diag_util_mprintf("    Device Status: %s\n", ready ? "Ready" : "Not Ready");
        if (ready)
        {
            if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
            {
                DIAG_UTIL_ERR_CHK(drv_extGpio_devEnable_get(unit, *dev_id_ptr, &enable), ret);
                diag_util_mprintf("    Device State: %s\n", enable ? DIAG_STR_ENABLE : DIAG_STR_DISABLE);
                diag_util_mprintf("\n");

                diag_util_mprintf("    ==================================\n");
                diag_util_mprintf("     GPIO# | Sel. | Direction | Value \n");
                diag_util_mprintf("    ==================================\n");
                for (gpio_idx = 0; gpio_idx <= 56; gpio_idx++)
                {
                    if ((ret = drv_extGpio_direction_get(unit, *dev_id_ptr, gpio_idx, &gpio_direction)) != RT_ERR_OK)
                        continue;
                    if ((ret = drv_extGpio_dataBit_get(unit, *dev_id_ptr, gpio_idx, &gpio_data)) != RT_ERR_OK)
                        continue;
                    diag_util_mprintf("       %02d  | GPIO |    %03s    |  %d\n", gpio_idx, \
                        (gpio_direction == GPIO_DIR_IN)?"IN":"OUT", gpio_data);
                }
            }
            else
            {
                DIAG_UTIL_ERR_CHK(drv_extGpio_devEnable_get(unit, *dev_id_ptr, &enable), ret);
                diag_util_mprintf("    Device State: %s\n", enable ? DIAG_STR_ENABLE : DIAG_STR_DISABLE);
                DIAG_UTIL_ERR_CHK(drv_extGpio_syncEnable_get(unit, *dev_id_ptr, &enable), ret);
                diag_util_mprintf("    GPIO Sync State: %s\n", enable ? DIAG_STR_ENABLE : DIAG_STR_DISABLE);
                DIAG_UTIL_ERR_CHK(drv_extGpio_syncStatus_get(unit, *dev_id_ptr, &value), ret);
                diag_util_mprintf("    GPIO Sync Status: %s\n", value ? "In Progress" : "Completed");

                diag_util_mprintf("\n");

                diag_util_mprintf("    ======================================================\n");
                diag_util_mprintf("     GPIO# | Sel. | Direction | Debounce | Invert | Value \n");
                diag_util_mprintf("    ======================================================\n");
                for (gpio_idx = 0; gpio_idx < EXT_GPIO_ID_END; gpio_idx++)
                {
                    osal_memset(&extGpio_pinConfData, 0, sizeof(drv_extGpio_conf_t));

                    if ((ret = drv_extGpio_pin_get(unit, *dev_id_ptr, gpio_idx, &extGpio_pinConfData)) != RT_ERR_OK)
                        continue;
                    if ((ret = drv_extGpio_dataBit_get(unit, *dev_id_ptr, gpio_idx, &gpio_data)) != RT_ERR_OK)
                        continue;
                    if (gpio_idx >= EXT_GPIO_ID31)
                    {
                        diag_util_mprintf("       %02d  | GPIO |    %03s    |    %03s   |   %03s  |  %d\n", gpio_idx, \
                            (extGpio_pinConfData.direction == GPIO_DIR_IN)?"IN":"OUT", \
                            (extGpio_pinConfData.debounce)?"Yes":"No", \
                            (extGpio_pinConfData.inverter)?"Yes":"No", \
                            gpio_data);
                    }
                    else
                    {
                        diag_util_mprintf("       %02d  | GPIO |    %03s    |    ---   |   %03s  |  %d\n", gpio_idx, \
                            (extGpio_pinConfData.direction == GPIO_DIR_IN)?"IN":"OUT", \
                            (extGpio_pinConfData.inverter)?"Yes":"No", \
                            gpio_data);
                    }
                }
            }
        }
    }
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_PIN_GPIO_ID
/*
 * ext-gpio get dev <UINT:dev_id> pin <UINT:gpio_id>
 */
cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_pin_gpio_id(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *gpio_id_ptr)
{
    uint32      unit = 0;
    uint32      gpio_data = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((gpio_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr > 56), CPARSER_ERR_INVALID_PARAMS);
    else
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr >= EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_extGpio_dataBit_get(unit, *dev_id_ptr, *gpio_id_ptr, &gpio_data), ret);
    diag_util_printf("    0x%04X  ", gpio_data);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_PIN_GPIO_ID_DIRECTION
/*
 * ext-gpio get dev <UINT:dev_id> pin <UINT:gpio_id> direction
 */

cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_pin_gpio_id_direction(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *gpio_id_ptr)
{
    uint32      unit = 0;
    drv_gpio_direction_t    gpio_direction = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((gpio_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr > 56), CPARSER_ERR_INVALID_PARAMS);
    else
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr >= EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_extGpio_direction_get(unit, *dev_id_ptr, *gpio_id_ptr, &gpio_direction), ret);
    if (gpio_direction == GPIO_DIR_IN)
        diag_util_printf("    Input Pin");
    else
        diag_util_printf("    Output Pin");
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_READY
/*
 * ext-gpio get dev <UINT:dev_id> ready
 */
cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_ready(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0;
    uint32      is_ready = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_extGpio_devReady_get(unit, *dev_id_ptr, &is_ready), ret);
    if (is_ready)
        diag_util_printf("    Ready");
    else
        diag_util_printf("    Not ready");
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_STATE
/*
 * ext-gpio get dev <UINT:dev_id> state
 */
cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_state(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0;
    uint32      is_active = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_extGpio_devEnable_get(unit, *dev_id_ptr, &is_active), ret);
    if (is_active)
        diag_util_printf("    Active");
    else
        diag_util_printf("    Inactive");
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_SYNC_STATE
/*
 * ext-gpio get dev <UINT:dev_id> sync state
 */
cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_sync_state(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0;
    uint32      is_sync_enable = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_extGpio_syncEnable_get(unit, *dev_id_ptr, &is_sync_enable), ret);
    if (is_sync_enable)
        diag_util_printf("    GPIO Sync Config: enabled");
    else
        diag_util_printf("    GPIO Sync Config: disabled");
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_SYNC_STATUS
/*
 * ext-gpio get dev <UINT:dev_id> sync status
 */
cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_sync_status(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0;
    uint32      sync_in_progress = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_extGpio_syncStatus_get(unit, *dev_id_ptr, &sync_in_progress), ret);
    if (sync_in_progress)
        diag_util_printf("    GPIO Sync Status: in progress");
    else
        diag_util_printf("    GPIO Sync Status: completed");
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_REGISTER
/*
 * ext-gpio get dev <UINT:dev_id> <UINT:register>
 */
cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_register(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *register_ptr)
{
    uint32      unit = 0;
    uint32      data = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((register_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*register_ptr > 30), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    DIAG_UTIL_ERR_CHK(drv_extGpio_reg_read(unit, *dev_id_ptr, *register_ptr, &data), ret);
    diag_util_printf("    Register %d: 0x%04x", *register_ptr, data);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_GET_DEV_DEV_ID_READ_I2C_REGISTER_NUMBER
/*
 * ext-gpio get dev <UINT:dev_id> read i2c <UINT:register> <UINT:number>
 */
cparser_result_t cparser_cmd_ext_gpio_get_dev_dev_id_read_i2c_register_number(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *register_ptr,
    uint32_t *number_ptr)
{
    uint32      unit = 0;
    uint32      data = 0, reg, i;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((register_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    reg = *register_ptr;
    for (i = 0; i < *number_ptr; i++)
    {
        reg = (*register_ptr) + i;
        DIAG_UTIL_ERR_CHK(drv_extGpio_i2c_read(unit, *dev_id_ptr, reg, &data), ret);
        diag_util_printf("    Register %u: 0x%04x (%c)", reg, data, data);
        diag_util_mprintf("\n");
    }

    return CPARSER_OK;
}

#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_WRITE_I2C_REGISTER_DATA
/*
 * ext-gpio set dev <UINT:dev_id> write i2c <UINT:register> <UINT:data>
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_write_i2c_register_data(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *register_ptr,
    uint32_t *data_ptr)
{
    uint32      unit = 0;
    uint32      data = 0, reg;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((register_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((data_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_OUTPUT_INIT();

    reg = *register_ptr;
    DIAG_UTIL_ERR_CHK(drv_extGpio_i2c_write(unit, *dev_id_ptr, reg, data), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_INIT_I2C_SCK_PIN_SCK_GPIO_ID_SDA_PIN_SDA_GPIO_ID
/*
 * ext-gpio set dev <UINT:dev_id> init i2c sck-pin <UINT:sck_gpio_id> sda-pin <UINT:sda_gpio_id>
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_init_i2c_sck_pin_sck_gpio_id_sda_pin_sda_gpio_id(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *sck_gpio_id_ptr,
    uint32_t *sda_gpio_id_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((sck_gpio_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*sck_gpio_id_ptr >= EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((sda_gpio_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*sda_gpio_id_ptr >= EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_ERR_CHK(drv_extGpio_i2c_init(unit, *dev_id_ptr, *sck_gpio_id_ptr, *sda_gpio_id_ptr), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_INIT_MDC_PHY_ID_PAGE
/*
 * ext-gpio set dev <UINT:dev_id> init mdc <UINT:phy_id> <UINT:page>
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_init_mdc_phy_id_page(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *phy_id_ptr,
    uint32_t *page_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    drv_extGpio_devConf_t   data;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((phy_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    //DIAG_UTIL_PARAM_RANGE_CHK((*phy_id_ptr > ??), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((page_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*page_ptr > 30), CPARSER_ERR_INVALID_PARAMS);

    data.access_mode = EXT_GPIO_ACCESS_MODE_MDC;
    data.address = *phy_id_ptr;
    data.page = *page_ptr;

    DIAG_UTIL_ERR_CHK(drv_extGpio_dev_init(unit, *dev_id_ptr, &data), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_PIN_GPIO_ID_INIT_IN_OUT_DEBOUNCE_INVERTER
/*
 * ext-gpio set dev <UINT:dev_id> pin <UINT:gpio_id> init ( in | out ) { debounce } { inverter }
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_pin_gpio_id_init_in_out_debounce_inverter(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *gpio_id_ptr)
{
    uint32      unit = 0;
    int32       flag_num;
    int32       ret = RT_ERR_FAILED;
    drv_extGpio_conf_t  data;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((gpio_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr >= EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);
    osal_memset(&data, 0, sizeof(drv_extGpio_conf_t));

    if ('i' == TOKEN_CHAR(7, 0))
        data.direction = GPIO_DIR_IN;
    else
        data.direction = GPIO_DIR_OUT;

    /*from first optional token*/
    for(flag_num = 8; flag_num < TOKEN_NUM; flag_num++)
    {
        if ('d' == TOKEN_CHAR(flag_num, 0))
        {
            data.debounce = TRUE;
        }
        else if ('i' == TOKEN_CHAR(flag_num, 0))
        {
            data.inverter = TRUE;
        }
        else
        {
            diag_util_printf("User config: Error!\n");
            return CPARSER_NOT_OK;
        }
    }

    DIAG_UTIL_ERR_CHK(drv_extGpio_pin_init(unit, *dev_id_ptr, *gpio_id_ptr, &data), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_PIN_GPIO_ID_DIRECTION_IN_OUT
/*
 * ext-gpio set dev <UINT:dev_id> pin <UINT:gpio_id> direction ( in | out )
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_pin_gpio_id_direction_in_out(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *gpio_id_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    drv_gpio_direction_t  gpio_direction = 0;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((gpio_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr > 56), CPARSER_ERR_INVALID_PARAMS);
    else
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr >= EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);

    if ('i' == TOKEN_CHAR(7, 0))
        gpio_direction = GPIO_DIR_IN;
    else
        gpio_direction = GPIO_DIR_OUT;

    DIAG_UTIL_ERR_CHK(drv_extGpio_direction_set(unit, *dev_id_ptr, *gpio_id_ptr, gpio_direction), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_PIN_GPIO_ID_DATA
/*
 * ext-gpio set dev <UINT:dev_id> pin <UINT:gpio_id> <UINT:data>
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_pin_gpio_id_data(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *gpio_id_ptr,
    uint32_t *data_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((gpio_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    if (DIAG_OM_GET_FAMILYID(RTL8390_FAMILY_ID) || DIAG_OM_GET_FAMILYID(RTL8350_FAMILY_ID))
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr > 56), CPARSER_ERR_INVALID_PARAMS);
    else
        DIAG_UTIL_PARAM_RANGE_CHK((*gpio_id_ptr >= EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((data_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_ERR_CHK(drv_extGpio_dataBit_set(unit, *dev_id_ptr, *gpio_id_ptr, *data_ptr), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_STATE_DISABLE_ENABLE
/*
 * ext-gpio set dev <UINT:dev_id> state ( disable | enable )
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_state_disable_enable(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    rtk_enable_t    enable;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    if ('d' == TOKEN_CHAR(5, 0))
        enable = DISABLED;
    else
        enable = ENABLED;

    DIAG_UTIL_ERR_CHK(drv_extGpio_devEnable_set(unit, *dev_id_ptr, enable), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_SYNC_START
/*
 * ext-gpio set dev <UINT:dev_id> sync start
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_sync_start(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_ERR_CHK(drv_extGpio_sync_start(unit, *dev_id_ptr), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_SYNC_STATE_DISABLE_ENABLE
/*
 * ext-gpio set dev <UINT:dev_id> sync state ( disable | enable )
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_sync_state_disable_enable(cparser_context_t *context,
    uint32_t *dev_id_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;
    rtk_enable_t    enable;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);

    if ('d' == TOKEN_CHAR(6, 0))
        enable = DISABLED;
    else
        enable = ENABLED;

    DIAG_UTIL_ERR_CHK(drv_extGpio_syncEnable_set(unit, *dev_id_ptr, enable), ret);

    return CPARSER_OK;
}
#endif

#ifdef CMD_EXT_GPIO_SET_DEV_DEV_ID_REGISTER_DATA
/*
 * ext-gpio set dev <UINT:dev_id> <UINT:register> <UINT:data>
 */
cparser_result_t cparser_cmd_ext_gpio_set_dev_dev_id_register_data(cparser_context_t *context,
    uint32_t *dev_id_ptr,
    uint32_t *register_ptr,
    uint32_t *data_ptr)
{
    uint32      unit = 0;
    int32       ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((dev_id_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_id_ptr >= EXT_GPIO_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((register_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*register_ptr > 30), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((data_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_ERR_CHK(drv_extGpio_reg_write(unit, *dev_id_ptr, *register_ptr, *data_ptr), ret);

    return CPARSER_OK;
}
#endif

