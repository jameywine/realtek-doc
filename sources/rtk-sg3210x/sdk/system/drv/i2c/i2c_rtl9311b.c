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
 * $Revision: 102431 $
 * $Date: 2019-12-10 09:03:14 +0800 (周二, 10 12月 2019) $
 *
 * Purpose : I2C master driver.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) i2c read and write
 *
 */
#include <common/rt_type.h>
#include <common/debug/rt_log.h>
#include <osal/lib.h>
#include <drv/i2c/i2c.h>
#include <ioal/mem32.h>
#include <hwp/hw_profile.h>
#include <private/drv/i2c/i2c_rtl9311b.h>
#include <private/drv/swcore/swcore_rtl9311b.h>
#include <private/drv/i2c/i2c_software_drv.h>
#include <drv/gpio/generalCtrl_gpio.h>
#include <osal/time.h>
#include <common/util/rt_util.h>

extern i2c_devInfo_t gI2C_dev[RTK_MAX_NUM_OF_UNIT][I2C_DEV_ID_END];
static uint8 i2c_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};


static int32 r9311b_data_write(uint32 unit, i2c_devConf_t *i2c_dev, uint8 *pBuff)
{
    uint32  data_width = 0;
    uint32  reg_idx = 0;
    uint32  buff_offset = 0;
    uint32  reg_data = 0;

    data_width = gI2C_dev[unit][i2c_dev->device_id].conf.data_width;
    reg_idx = 0;
    buff_offset = 0;

    switch (gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id)
    {
        case I2C_INTF_CONTROLLER_ID0:
            while (data_width)
            {
                ioal_mem32_read(unit, RTL9311B_I2C_MST1_DATA_CTRL_ADDR(reg_idx), &reg_data);
                reg_data &= ~(RTL9311B_I2C_MST2_DATA_CTRL_DATA_MASK(reg_idx));
                reg_data |= ((*(pBuff + buff_offset)) << (RTL9311B_I2C_MST1_DATA_CTRL_DATA_OFFSET(reg_idx)));
                ioal_mem32_write(unit, RTL9311B_I2C_MST1_DATA_CTRL_ADDR(reg_idx), reg_data);
                reg_idx++;
                buff_offset++;
                data_width--;
            }
            break;

        case I2C_INTF_CONTROLLER_ID1:
            while (data_width)
            {
                ioal_mem32_read(unit, RTL9311B_I2C_MST2_DATA_CTRL_ADDR(reg_idx), &reg_data);
                reg_data &= ~(RTL9311B_I2C_MST2_DATA_CTRL_DATA_MASK(reg_idx));
                reg_data |= ((*(pBuff + buff_offset)) << (RTL9311B_I2C_MST2_DATA_CTRL_DATA_OFFSET(reg_idx)));
                ioal_mem32_write(unit, RTL9311B_I2C_MST2_DATA_CTRL_ADDR(reg_idx), reg_data);
                reg_idx++;
                buff_offset++;
                data_width--;
            }
            break;

        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

static int32 r9311b_data_read(uint32 unit, i2c_devConf_t *i2c_dev, uint8 *pBuff)
{
    uint32  data_width = 0;
    uint32  reg_idx = 0;
    uint32  buff_offset = 0;
    uint32  reg_data = 0;

    data_width = gI2C_dev[unit][i2c_dev->device_id].conf.data_width;
    reg_idx = 0;
    buff_offset = 0;

    switch (gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id)
    {
        case I2C_INTF_CONTROLLER_ID0:
            while (data_width)
            {
                ioal_mem32_read(unit, RTL9311B_I2C_MST1_DATA_CTRL_ADDR(reg_idx), &reg_data);
                reg_data = (reg_data >> RTL9311B_I2C_MST1_DATA_CTRL_DATA_OFFSET(reg_idx));
                *(pBuff + buff_offset) = (reg_data & RTL9311B_INT_I2C_DATA_REG_BYTE_MASK);
                reg_idx++;
                buff_offset++;
                data_width--;
            }
            break;

        case I2C_INTF_CONTROLLER_ID1:
            while (data_width)
            {
                ioal_mem32_read(unit, RTL9311B_I2C_MST2_DATA_CTRL_ADDR(reg_idx), &reg_data);
                reg_data = (reg_data >> RTL9311B_I2C_MST2_DATA_CTRL_DATA_OFFSET(reg_idx));
                *(pBuff + buff_offset) = (reg_data & RTL9311B_INT_I2C_DATA_REG_BYTE_MASK);
                reg_idx++;
                buff_offset++;
                data_width--;
            }
            break;

        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

int32 r9311b_i2c_execution(uint32 unit, uint32 intf_id)
{
    uint32  reg_data = 0;
    WAIT_COMPLETE_VAR();

    switch (intf_id)
    {
        case I2C_INTF_CONTROLLER_ID0:
            /* Set Trig */
            ioal_mem32_read(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, &reg_data);
            reg_data |= RTL9311B_I2C_MST1_CTRL1_TRIG_MASK;
            ioal_mem32_write(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, reg_data);
            /* Check Complete or Not */
            WAIT_COMPLETE(1000000)
            {
                ioal_mem32_read(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, &reg_data);
                if ((reg_data & RTL9311B_I2C_MST1_CTRL1_TRIG_MASK) == 0)
                    break;
            }
            if(WAIT_COMPLETE_IS_TIMEOUT())
            {
                return RT_ERR_FAILED;
            }
            /* Check Failed or Not */
            ioal_mem32_read(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, &reg_data);
            if ((reg_data & RTL9311B_I2C_MST1_CTRL1_FAIL_MASK) != 0)
                return RT_ERR_FAILED;

            break;

        case I2C_INTF_CONTROLLER_ID1:
            /* Set Trig */
            ioal_mem32_read(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, &reg_data);
            reg_data |= RTL9311B_I2C_MST2_CTRL1_TRIG_MASK;
            ioal_mem32_write(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, reg_data);
            /* Check Complete or Not */
            WAIT_COMPLETE(1000000)
            {
                ioal_mem32_read(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, &reg_data);
                if ((reg_data & RTL9311B_I2C_MST2_CTRL1_TRIG_MASK) == 0)
                    break;
            }
            if(WAIT_COMPLETE_IS_TIMEOUT())
            {
                return RT_ERR_FAILED;
            }
            /* Check Failed or Not*/
            ioal_mem32_read(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, &reg_data);
            if ((reg_data & RTL9311B_I2C_MST2_CTRL1_FAIL_MASK) != 0)
                return RT_ERR_FAILED;

            break;

        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

int32 r9311b_i2c_init(uint32 unit)
{
    RT_INIT_REENTRY_CHK(i2c_init[unit]);
    i2c_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}

int32 r9311b_i2c_dev_init(uint32 unit, i2c_devConf_t *i2c_dev)
{
    uint32  reg_data = 0;
    uint32  scl_freq = 0;
    uint32  i2c_addrwidth = 0;
    uint32  i2c_datawidth = 0;

    if ((gI2C_dev[unit][i2c_dev->device_id].conf.sda_pin_id >= I2C_9311b_INTF_CONTROLLER_SDA_END)
            && (gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id != I2C_INTF_SOFTWARE_DRV_ID))
    {
        return RT_ERR_FAILED;
    }

    /* Set Related SDA pin to I2C function
       0b0: GPIO
       0b1: SDA */
    ioal_mem32_read(unit, RTL9311B_INTERFACE_SEL_ADDR, &reg_data);
    switch(gI2C_dev[unit][i2c_dev->device_id].conf.sda_pin_id)
    {
        case 0:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO10_FUNC_SEL_OFFSET);
            break;
        case 1:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO11_FUNC_SEL_OFFSET);
            break;
        case 2:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO12_FUNC_SEL_OFFSET);
            break;
        case 3:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO13_FUNC_SEL_OFFSET);
            break;
        case 4:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO14_FUNC_SEL_OFFSET);
            break;
        case 5:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO15_FUNC_SEL_OFFSET);
            break;
        case 6:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO16_FUNC_SEL_OFFSET);
            break;
        case 7:
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO17_FUNC_SEL_OFFSET);
            break;
        default:
            break;
    }
    ioal_mem32_write(unit, RTL9311B_INTERFACE_SEL_ADDR, reg_data);


    switch (gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id)
    {
        case I2C_INTF_CONTROLLER_ID0:
            /* Set SCL pin GPIO */
            ioal_mem32_read(unit, RTL9311B_INTERFACE_SEL_ADDR, &reg_data);
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO8_FUNC_SEL_OFFSET);
            ioal_mem32_write(unit, RTL9311B_INTERFACE_SEL_ADDR, reg_data);

            ioal_mem32_read(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, &reg_data);
            /* Set SDA pin */
            reg_data &= ~RTL9311B_I2C_MST1_CTRL1_SDA_OUT_SEL_MASK;
            reg_data |= (gI2C_dev[unit][i2c_dev->device_id].conf.sda_pin_id << RTL9311B_I2C_MST1_CTRL1_SDA_OUT_SEL_OFFSET);
            /* Set Slave Device Address */
            reg_data &= ~RTL9311B_I2C_MST1_CTRL1_DEV_ADDR_MASK;
            reg_data |= (gI2C_dev[unit][i2c_dev->device_id].conf.dev_addr << RTL9311B_I2C_MST1_CTRL1_DEV_ADDR_OFFSET);
            ioal_mem32_write(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, reg_data);

            ioal_mem32_read(unit, RTL9311B_I2C_MST1_CTRL2_ADDR, &reg_data);
            /* Set Data Width */
            reg_data &= ~RTL9311B_I2C_MST1_CTRL2_DATA_WIDTH_MASK;
            reg_data |= ((gI2C_dev[unit][i2c_dev->device_id].conf.data_width - 1) << RTL9311B_I2C_MST1_CTRL2_DATA_WIDTH_OFFSET);
            /* Set Memory Address Width */
            reg_data &= ~RTL9311B_I2C_MST1_CTRL2_MEM_ADDR_WIDTH_MASK;
            reg_data |= ((gI2C_dev[unit][i2c_dev->device_id].conf.mem_addr_width) << RTL9311B_I2C_MST1_CTRL2_MEM_ADDR_WIDTH_OFFSET);
            /* Set SCL Freq */
            switch (gI2C_dev[unit][i2c_dev->device_id].conf.clk_freq)
            {
                case I2C_CLK_STD_MODE:
                    scl_freq = 1;
                    break;
                case I2C_CLK_FAST_MODE:
                    scl_freq = 2;
                    break;
                case I2C_CLK_RT_50K:
                    scl_freq = 0;
                    break;
                case I2C_CLK_RT_2P5M:
                    scl_freq = 3;
                    break;
                default:
                    return RT_ERR_FAILED;
            }
            reg_data &= ~RTL9311B_I2C_MST1_CTRL2_SCL_FREQ_MASK;
            reg_data |= ((scl_freq) << RTL9311B_I2C_MST1_CTRL2_SCL_FREQ_OFFSET);
             /* Set RD MODE */
            switch (gI2C_dev[unit][i2c_dev->device_id].conf.read_type)
            {
                case I2C_INTF_READ_TYPE_RANDOM:
                    reg_data &= ~RTL9311B_I2C_MST1_CTRL2_READ_MODE_MASK;
                    break;
                case I2C_INTF_READ_TYPE_SEQUENTIAL:
                    reg_data |= RTL9311B_I2C_MST1_CTRL2_READ_MODE_MASK;
                    break;
                default:
                    return RT_ERR_FAILED;
            }
            ioal_mem32_write(unit, RTL9311B_I2C_MST1_CTRL2_ADDR, reg_data);
            break;

        case I2C_INTF_CONTROLLER_ID1:
            /* Set SCL pin GPIO */
            ioal_mem32_read(unit, RTL9311B_INTERFACE_SEL_ADDR, &reg_data);
            reg_data |= (0x1 << RTL9311B_INTERFACE_SEL_GPIO9_FUNC_SEL_OFFSET);
            ioal_mem32_write(unit, RTL9311B_INTERFACE_SEL_ADDR, reg_data);

            ioal_mem32_read(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, &reg_data);
            /* Set SDA pin*/
            reg_data &= ~RTL9311B_I2C_MST2_CTRL1_SDA_OUT_SEL_MASK;
            reg_data |= (gI2C_dev[unit][i2c_dev->device_id].conf.sda_pin_id << RTL9311B_I2C_MST2_CTRL1_SDA_OUT_SEL_OFFSET);
            /* Set Slave Device Address*/
            reg_data &= ~RTL9311B_I2C_MST2_CTRL1_DEV_ADDR_MASK;
            reg_data |= (gI2C_dev[unit][i2c_dev->device_id].conf.dev_addr << RTL9311B_I2C_MST2_CTRL1_DEV_ADDR_OFFSET);
            ioal_mem32_write(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, reg_data);

            ioal_mem32_read(unit, RTL9311B_I2C_MST2_CTRL2_ADDR, &reg_data);
            /* Set Data Width */
            reg_data &= ~RTL9311B_I2C_MST2_CTRL2_DATA_WIDTH_MASK;
            reg_data |= ((gI2C_dev[unit][i2c_dev->device_id].conf.data_width - 1) << RTL9311B_I2C_MST2_CTRL2_DATA_WIDTH_OFFSET);
            /* Set Memory Address Width */
            reg_data &= ~RTL9311B_I2C_MST2_CTRL2_MEM_ADDR_WIDTH_MASK;
            reg_data |= ((gI2C_dev[unit][i2c_dev->device_id].conf.mem_addr_width) << RTL9311B_I2C_MST2_CTRL2_MEM_ADDR_WIDTH_OFFSET);
            /* Set SCL Freq */
            switch (gI2C_dev[unit][i2c_dev->device_id].conf.clk_freq)
            {
                case I2C_CLK_STD_MODE:
                    scl_freq = 1;
                    break;
                case I2C_CLK_FAST_MODE:
                    scl_freq = 2;
                    break;
                case I2C_CLK_RT_50K:
                    scl_freq = 0;
                    break;
                case I2C_CLK_RT_2P5M:
                    scl_freq = 3;
                    break;
                default:
                    return RT_ERR_FAILED;
            }
            reg_data &= ~RTL9311B_I2C_MST2_CTRL2_SCL_FREQ_MASK;
            reg_data |= ((scl_freq) << RTL9311B_I2C_MST2_CTRL2_SCL_FREQ_OFFSET);
             /* Set RD MODE */
            switch(gI2C_dev[unit][i2c_dev->device_id].conf.read_type){
                case I2C_INTF_READ_TYPE_RANDOM:
                    reg_data &= ~RTL9311B_I2C_MST2_CTRL2_READ_MODE_MASK;
                    break;
                case I2C_INTF_READ_TYPE_SEQUENTIAL:
                    reg_data |= RTL9311B_I2C_MST2_CTRL2_READ_MODE_MASK;
                    break;
                default:
                    return RT_ERR_FAILED;
            }

            ioal_mem32_write(unit, RTL9311B_I2C_MST2_CTRL2_ADDR, reg_data);
            break;

        case I2C_INTF_SOFTWARE_DRV_ID:
            if ((drv_software_i2c_init(i2c_dev->scl_dev, i2c_dev->scl_pin_id, i2c_dev->sda_dev, i2c_dev->sda_pin_id, i2c_dev->device_id)) != RT_ERR_OK)
            {
                return RT_ERR_FAILED;
            }
            if (((i2c_dev->mem_addr_width) >= I2C_ADDR_WIDTH_BYTE_END) || ((i2c_dev->data_width) >= I2C_DATA_WIDTH_BYTE_END))
            {
                return RT_ERR_FAILED;
            }

            i2c_addrwidth = i2c_dev->mem_addr_width;
            i2c_datawidth = i2c_dev->data_width;

            drv_software_i2c_type_set(i2c_addrwidth, i2c_datawidth, i2c_dev->dev_addr, i2c_dev->scl_delay, i2c_dev->device_id);
            break;

        default:
            return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


int32 r9311b_i2c_read(uint32 unit, i2c_devConf_t *i2c_dev, uint32 reg_idex, uint8 *pBuff)
{
    int32           ret = RT_ERR_OK;
    uint32          reg_data = 0;
    i2c_devConf_t   read_dev;

    osal_memcpy(&read_dev, &gI2C_dev[unit][i2c_dev->device_id].conf, sizeof(i2c_devConf_t));
    r9311b_i2c_dev_init(unit, &read_dev);

    switch (gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id)
    {
        case I2C_INTF_CONTROLLER_ID0:
            /* Set Memory Address */
            ioal_mem32_read(unit, RTL9311B_I2C_MST1_MEMADDR_CTRL_ADDR, &reg_data);
            reg_data &= ~RTL9311B_I2C_MST1_MEMADDR_CTRL_MEM_ADDR_MASK;
            reg_data |= (reg_idex << RTL9311B_I2C_MST1_MEMADDR_CTRL_MEM_ADDR_OFFSET);
            ioal_mem32_write(unit, RTL9311B_I2C_MST1_MEMADDR_CTRL_ADDR, reg_data);

            /* Set Read OP */
            ioal_mem32_read(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, &reg_data);
            reg_data &= ~RTL9311B_I2C_MST1_CTRL1_RWOP_MASK;
            ioal_mem32_write(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, reg_data);
            break;

        case I2C_INTF_CONTROLLER_ID1:
            /* Set Memory Address */
            ioal_mem32_read(unit, RTL9311B_I2C_MST2_MEMADDR_CTRL_ADDR, &reg_data);
            reg_data &= ~RTL9311B_I2C_MST2_MEMADDR_CTRL_MEM_ADDR_MASK;
            reg_data |= (reg_idex << RTL9311B_I2C_MST2_MEMADDR_CTRL_MEM_ADDR_OFFSET);
            ioal_mem32_write(unit, RTL9311B_I2C_MST2_MEMADDR_CTRL_ADDR, reg_data);

            /* Set Read OP */
            ioal_mem32_read(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, &reg_data);
            reg_data &= ~RTL9311B_I2C_MST2_CTRL1_RWOP_MASK;
            ioal_mem32_write(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, reg_data);
            break;

        case I2C_INTF_SOFTWARE_DRV_ID:
            if ((ret = drv_software_i2c_read(reg_idex, pBuff, i2c_dev->device_id)) != RT_ERR_OK)
            {
                return RT_ERR_FAILED;
            }
            return RT_ERR_OK;

        default:
            return RT_ERR_FAILED;
    }

    ret = r9311b_i2c_execution(unit, gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id);
    if (ret != RT_ERR_OK)
        return ret;

    ret = r9311b_data_read(unit, i2c_dev, pBuff);

    return ret;
}

int32 r9311b_i2c_write(uint32 unit, i2c_devConf_t *i2c_dev, uint32 reg_idex, uint8 *pBuff)
{
    int32           ret = RT_ERR_OK;
    uint32          reg_data = 0;
    i2c_devConf_t   write_dev;

    osal_memcpy(&write_dev, &gI2C_dev[unit][i2c_dev->device_id].conf, sizeof(i2c_devConf_t));
    r9311b_i2c_dev_init(unit, &write_dev);

    switch (gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id)
    {
        case I2C_INTF_CONTROLLER_ID0:
            /* Set Memory Address */
            ioal_mem32_read(unit, RTL9311B_I2C_MST1_MEMADDR_CTRL_ADDR, &reg_data);
            reg_data &= ~RTL9311B_I2C_MST1_MEMADDR_CTRL_MEM_ADDR_MASK;
            reg_data |= (reg_idex << RTL9311B_I2C_MST1_MEMADDR_CTRL_MEM_ADDR_OFFSET);
            ioal_mem32_write(unit, RTL9311B_I2C_MST1_MEMADDR_CTRL_ADDR, reg_data);

            /* Set Write OP */
            ioal_mem32_read(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, &reg_data);
            reg_data |= RTL9311B_I2C_MST1_CTRL1_RWOP_MASK;
            ioal_mem32_write(unit, RTL9311B_I2C_MST1_CTRL1_ADDR, reg_data);
            break;

        case I2C_INTF_CONTROLLER_ID1:
            /* Set Memory Address */
            ioal_mem32_read(unit, RTL9311B_I2C_MST2_MEMADDR_CTRL_ADDR, &reg_data);
            reg_data &= ~RTL9311B_I2C_MST2_MEMADDR_CTRL_MEM_ADDR_MASK;
            reg_data |= (reg_idex << RTL9311B_I2C_MST2_MEMADDR_CTRL_MEM_ADDR_OFFSET);
            ioal_mem32_write(unit, RTL9311B_I2C_MST2_MEMADDR_CTRL_ADDR, reg_data);

            /* Set Write OP */
            ioal_mem32_read(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, &reg_data);
            reg_data |= RTL9311B_I2C_MST2_CTRL1_RWOP_MASK;
            ioal_mem32_write(unit, RTL9311B_I2C_MST2_CTRL1_ADDR, reg_data);
            break;

        case I2C_INTF_SOFTWARE_DRV_ID:
            if ((ret = drv_software_i2c_write(reg_idex, pBuff, i2c_dev->device_id)) != RT_ERR_OK)
            {
                return RT_ERR_FAILED;
            }
            return RT_ERR_OK;

        default:
            return RT_ERR_FAILED;
    }

    ret = r9311b_data_write(unit, i2c_dev, pBuff);
    if (ret != RT_ERR_OK)
        return ret;

    ret = r9311b_i2c_execution(unit, gI2C_dev[unit][i2c_dev->device_id].conf.i2c_interface_id);

    return ret;
}

