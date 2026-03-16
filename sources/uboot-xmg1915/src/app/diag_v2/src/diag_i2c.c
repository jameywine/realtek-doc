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
 * $Revision: 72443 $
 * $Date: 2016-10-14 19:09:41 +0800 (Fri, 14 Oct 2016) $
 *
 * Purpose : Definition those internal GPIO command and APIs in the SDK diagnostic shell.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) internal GPIO commands.
 */

/*
 * Include Files
 */
#include <common/debug/rt_log.h>
#include <common/rt_error.h>
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/util/rt_util.h>
#include <drv/i2c/i2c.h>
#include <diag_util.h>
#include <diag_om.h>
#include <diag_str.h>
#include <parser/cparser_priv.h>

#ifdef CMD_I2C_INIT_UNIT_UNIT_DEV_DEV_INTF_INTF_SCK_GPIODEV_SCK_GPIODEV_SCK_PIN_SCK_PIN_SDA_GPIODEV_SDA_GPIODEV_SDA_PIN_SDA_PIN_ADDR_WIDTH_ADDR_WIDTH_DATA_WIDTH_DATA_WIDTH_SCL_FREQ_SCL_FREQ_I2C_DELAY_I2C_DELAY_CHIP_ADDR_CHIP_ADDR
/*
 * i2c init unit <UINT:unit> dev <UINT:dev> intf <UINT:intf> sck-gpiodev <UINT:sck_gpiodev> sck-pin <UINT:sck_pin> sda-gpiodev <UINT:sda_gpiodev> sda-pin <UINT:sda_pin> addr-width <UINT:addr_width> data-width <UINT:data_width> scl-freq <UINT:scl_freq> i2c-delay <UINT:i2c_delay> chip_addr <UINT:chip_addr>
 */
cparser_result_t cparser_cmd_i2c_init_unit_unit_dev_dev_intf_intf_sck_gpiodev_sck_gpiodev_sck_pin_sck_pin_sda_gpiodev_sda_gpiodev_sda_pin_sda_pin_addr_width_addr_width_data_width_data_width_scl_freq_scl_freq_i2c_delay_i2c_delay_chip_addr_chip_addr(cparser_context_t *context,
    uint32_t *unit_ptr,
    uint32_t *dev_ptr,
    uint32_t *intf_ptr,
    uint32_t *sck_gpiodev_ptr,
    uint32_t *sck_pin_ptr,
    uint32_t *sda_gpiodev_ptr,
    uint32_t *sda_pin_ptr,
    uint32_t *addr_width_ptr,
    uint32_t *data_width_ptr,
    uint32_t *scl_freq_ptr,
    uint32_t *i2c_delay_ptr,
    uint32_t *chip_addr_ptr)
{
    uint32  unit = 0;
    int32   ret = RT_ERR_FAILED;

    i2c_devConf_t diag_I2C_dev;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((unit_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*unit_ptr >= RTK_MAX_NUM_OF_UNIT), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((dev_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_ptr >= I2C_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((intf_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*intf_ptr >= I2C_INTF_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((sck_gpiodev_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*sck_gpiodev_ptr >= GPIO_PORT_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((sck_pin_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*sck_pin_ptr > EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((sda_gpiodev_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*sda_gpiodev_ptr >= GPIO_PORT_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((sda_pin_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*sda_pin_ptr > EXT_GPIO_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((addr_width_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*addr_width_ptr > I2C_ADDR_WIDTH_BYTE_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((data_width_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*data_width_ptr > I2C_DATA_WIDTH_BYTE_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((scl_freq_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*scl_freq_ptr > I2C_CLK_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((i2c_delay_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((chip_addr_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*chip_addr_ptr > 0xFF), CPARSER_ERR_INVALID_PARAMS);


    DIAG_UTIL_ERR_CHK(drv_i2c_init(*unit_ptr), ret);
    diag_I2C_dev.device_id = *dev_ptr;
    diag_I2C_dev.dev_addr = *chip_addr_ptr;
    diag_I2C_dev.scl_dev = *sck_gpiodev_ptr;
    diag_I2C_dev.scl_pin_id = *sck_pin_ptr;
    diag_I2C_dev.sda_dev = *sda_gpiodev_ptr;
    diag_I2C_dev.sda_pin_id = *sda_pin_ptr;
    diag_I2C_dev.mem_addr_width = *addr_width_ptr;
    diag_I2C_dev.data_width = *data_width_ptr;
    diag_I2C_dev.clk_freq= *scl_freq_ptr;
    diag_I2C_dev.scl_delay= *i2c_delay_ptr;
    diag_I2C_dev.i2c_interface_id = *intf_ptr;

    DIAG_UTIL_ERR_CHK(drv_i2c_dev_init(*unit_ptr, &diag_I2C_dev), ret);
    return CPARSER_OK;
}
#endif

#ifdef CMD_I2C_GET_UNIT_UNIT_DEV_DEV_REG_REG
/*
 * i2c get unit <UINT:unit> dev <UINT:dev> reg <UINT:reg>
 */
cparser_result_t cparser_cmd_i2c_get_unit_unit_dev_dev_reg_reg(cparser_context_t *context,
    uint32_t *unit_ptr,
    uint32_t *dev_ptr,
    uint32_t *reg_ptr)
{
    uint32  unit = 0;
    uint8   data;
    int32   ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((unit_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*unit_ptr >= RTK_MAX_NUM_OF_UNIT), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((dev_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_ptr >= I2C_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((reg_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);

    DIAG_UTIL_ERR_CHK(drv_i2c_read(*unit_ptr, *dev_ptr, *reg_ptr, &data), ret);
    diag_util_printf("    READ:I2C Device:%d, Reg:%d, VALUE:0x%02X", *dev_ptr, *reg_ptr, data);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

#ifdef CMD_I2C_SET_UNIT_UNIT_DEV_DEV_REG_REG_DATA_DATA
/*
 * i2c set unit <UINT:unit> dev <UINT:dev> reg <UINT:reg> data <UINT:data>
 */
cparser_result_t cparser_cmd_i2c_set_unit_unit_dev_dev_reg_reg_data_data(cparser_context_t *context,
    uint32_t *unit_ptr,
    uint32_t *dev_ptr,
    uint32_t *reg_ptr,
    uint32_t *data_ptr)
{
    uint32  unit = 0;
    uint8   data;
    int32   ret = RT_ERR_FAILED;

    DIAG_OM_GET_UNIT_ID(unit);

    DIAG_UTIL_PARAM_CHK();
    DIAG_UTIL_PARAM_RANGE_CHK((unit_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*unit_ptr >= RTK_MAX_NUM_OF_UNIT), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((dev_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((*dev_ptr >= I2C_DEV_ID_END), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((reg_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);
    DIAG_UTIL_PARAM_RANGE_CHK((data_ptr == NULL), CPARSER_ERR_INVALID_PARAMS);

    data = *data_ptr;
    DIAG_UTIL_ERR_CHK(drv_i2c_write(*unit_ptr, *dev_ptr, *reg_ptr, &data), ret);
    diag_util_printf("    WRITE:I2C Device:%d, Reg:%d, VALUE:0x%02X", *dev_ptr, *reg_ptr, *data_ptr);
    diag_util_mprintf("\n");

    return CPARSER_OK;
}
#endif

