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
 * $Revision$
 * $Date$
 *
 * Purpose : Definition of SPI API
 *
 *
 */

/*
 * Include Files
 */
#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>
#include <drv/i2c/i2c.h>
#include <hal/chipdef/chip.h>
#include <private/drv/i2c/i2c_mapper.h>
#include <common/debug/rt_log.h>
#include <hwp/hw_profile.h>
#include <osal/lib.h>
#include <osal/sem.h>
#include <osal/print.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */

i2c_devInfo_t gI2C_dev[RTK_MAX_NUM_OF_UNIT][I2C_DEV_ID_END];


static osal_mutex_t  i2c_sem[RTK_MAX_NUM_OF_UNIT];

/* SENAO Frank support SENAO_I2C_VIRTUAL_DEVICE*/
#define SENAO_I2C_VIRTUAL_DEVICE
static uint8         i2c_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};
/*
 * Macro Definition
 */
#define I2C_SEM_LOCK(unit) \
            do {\
                if (osal_sem_mutex_take(i2c_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
                {\
                    return RT_ERR_SEM_LOCK_FAILED;\
                }\
            } while(0)

#define I2C_SEM_UNLOCK(unit) \
            do {\
                if (osal_sem_mutex_give(i2c_sem[unit]) != RT_ERR_OK)\
                {\
                    return RT_ERR_SEM_UNLOCK_FAILED;\
                }\
            } while(0)


/*
 * Function Declaration
 */

/* Function Name:
 *      drv_i2c_init
 * Description:
 *      Initialize I2C module.
 * Input:
 *      unit                - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      Must initialize i2c module before calling any i2c APIs.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_i2c_init(uint32 unit)
{
    uint32 idx;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

#ifdef SENAO_I2C_VIRTUAL_DEVICE
    if (i2c_init[unit] != INIT_COMPLETED)
    {
    RT_INIT_MSG("  I2C init (unit %u)\n", unit);

    /* create semaphore */
    i2c_sem[unit] = osal_sem_mutex_create();
    if (0 == i2c_sem[unit])
    {
        return RT_ERR_FAILED;
    }

    for(idx = 0; idx < I2C_DEV_ID_END; idx++)
        osal_memset(&gI2C_dev[unit][idx], 0x0, sizeof(i2c_devInfo_t));

        i2c_init[unit] = INIT_COMPLETED;
    return I2C_CTRL(unit).init(unit);
    } else {
        return RT_ERR_OK;
    }

#else

    RT_INIT_MSG("  I2C init (unit %u)\n", unit);

    /* create semaphore */
    i2c_sem[unit] = osal_sem_mutex_create();
    if (0 == i2c_sem[unit])
    {
        return RT_ERR_FAILED;
    }

    for(idx = 0; idx < I2C_DEV_ID_END; idx++)
        osal_memset(&gI2C_dev[unit][idx], 0x0, sizeof(i2c_devInfo_t));

    return I2C_CTRL(unit).init(unit);
#endif
} /* end of drv_i2c_init */


/* Function Name:
 *      drv_i2c_dev_init
 * Description:
 *      Initialize I2C module of the specified device.
 * Input:
 *      unit                - unit id
 *      *i2c_dev         - i2c_devConf_t pointer
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      Must initialize i2c module before calling any i2c APIs.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_i2c_dev_init(uint32 unit, i2c_devConf_t *i2c_dev)
{
    uint32  dev_id;
    int32   ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

    RT_INIT_MSG("  I2C device init (unit %u)\n", unit);

    if(i2c_dev->device_id >= I2C_DEV_ID_END)
        return RT_ERR_OUT_OF_RANGE;

    if(i2c_dev->read_type >= I2C_INTF_READ_TYPE_END)
        return RT_ERR_OUT_OF_RANGE;

    I2C_SEM_LOCK(unit);
    dev_id = i2c_dev->device_id;
    osal_memcpy(&gI2C_dev[unit][dev_id].conf, i2c_dev, sizeof(i2c_devConf_t));

    ret = I2C_CTRL(unit).i2c_dev_init(unit, &gI2C_dev[unit][dev_id].conf);
    I2C_SEM_UNLOCK(unit);

    if(ret == RT_ERR_OK)
        gI2C_dev[unit][dev_id].init_flag = INIT_COMPLETED;


    return ret;
} /* end of drv_i2c_dev_init */

/* Function Name:
 *      drv_i2c_spec_dev_data_width_init
 * Description:
 *      Initialize I2C data width of the specified device.
 * Input:
 *      unit                - unit id
 *      *i2c_dev         - i2c_devConf_t pointer
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      Must initialize i2c module before calling any i2c APIs.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_i2c_spec_dev_data_width_init(uint32 unit, uint32  dev_id, drv_i2c_dataWidth_t data_width)
{
    int32   ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

    if(dev_id >= I2C_DEV_ID_END)
        return RT_ERR_OUT_OF_RANGE;

    if(gI2C_dev[unit][dev_id].init_flag != INIT_COMPLETED)
        return RT_ERR_FAILED;

    I2C_SEM_LOCK(unit);
    gI2C_dev[unit][dev_id].conf.data_width = data_width;

    ret = I2C_CTRL(unit).i2c_dev_init(unit, &gI2C_dev[unit][dev_id].conf);
    I2C_SEM_UNLOCK(unit);

    if(ret == RT_ERR_OK)
        gI2C_dev[unit][dev_id].init_flag = INIT_COMPLETED;

    return ret;
} /* end of drv_i2c_dev_init */

/* Function Name:
 *      drv_i2c_write
 * Description:
 *      Transmit data via I2C of the specified device.
 * Input:
 *      unit                - unit id
 *      *i2c_dev         - i2c_devConf_t pointer
 *      reg_idx           - register index
 *      *pBuff            - transfer data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_i2c_write(uint32 unit, drv_i2c_devId_t i2c_dev_id, uint32 reg_idx, uint8 *pBuff)
{
    int32 ret = RT_ERR_OK;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

    if(i2c_dev_id >= I2C_DEV_ID_END)
        return RT_ERR_OUT_OF_RANGE;

    if(gI2C_dev[unit][i2c_dev_id].init_flag != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    I2C_SEM_LOCK(unit);
    if((ret = I2C_CTRL(unit).i2c_write(unit, &gI2C_dev[unit][i2c_dev_id].conf, reg_idx, pBuff)) != RT_ERR_OK)
    {
        I2C_SEM_UNLOCK(unit);
        return ret;
    }
    I2C_SEM_UNLOCK(unit);

    return ret;


} /* end of drv_i2c_write */


/* Function Name:
 *      drv_i2c_read
 * Description:
 *      Start the Receice data from I2C.
 * Input:
 *      unit                - unit id
 *      *i2c_dev         - i2c_devConf_t pointer
 *      reg_idx           - register index
 * Output:
 *      pBuff               - received data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_i2c_read(uint32 unit, drv_i2c_devId_t i2c_dev_id, uint32 reg_idx, uint8 *pBuff)
{
    int32 ret = RT_ERR_OK;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

    if(i2c_dev_id >= I2C_DEV_ID_END)
        return RT_ERR_OUT_OF_RANGE;

    if(gI2C_dev[unit][i2c_dev_id].init_flag != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    I2C_SEM_LOCK(unit);
    if((ret = I2C_CTRL(unit).i2c_read(unit, &gI2C_dev[unit][i2c_dev_id].conf, reg_idx, pBuff)) != RT_ERR_OK)
    {
        I2C_SEM_UNLOCK(unit);
        return ret;
    }
    I2C_SEM_UNLOCK(unit);

    return ret;
} /* end of drv_i2c_read */

// Starry@20211007 add PoE MCU Fw upgrade -S
#if 0
int32
drv_i2c_poe_mcu_read(uint32 unit, uint32 dev_id, uint32 devaddr, uint8 *pBuff)
{
    int32 ret = RT_ERR_OK;
    //
    int32 old_i2c_interface_id, old_read_type;
    i2c_devConf_t *devConf;

    //RT_INIT_MSG("SENAO:%s, %04d dev=0x%x, addr_len=0x%x\n",__FUNCTION__,__LINE__, dev_id, devaddr);
    
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

    if(dev_id >= I2C_DEV_ID_END)
        return RT_ERR_OUT_OF_RANGE;

    if(gI2C_dev[unit][dev_id].init_flag != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    I2C_SEM_LOCK(unit);
    devConf = &gI2C_dev[unit][dev_id].conf;
    
    old_i2c_interface_id = gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id;
    old_read_type = devConf->read_type;
    
    gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id = I2C_INTF_SOFTWARE_DRV_ID;
    devConf->read_type = I2C_INTF_READ_TYPE_MCU_UPGRADE;
      
    if((ret = I2C_CTRL(unit).i2c_read(unit, &gI2C_dev[unit][dev_id].conf, devaddr, pBuff)) != RT_ERR_OK)
    {
    
        gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id = old_i2c_interface_id;
        devConf->read_type = old_read_type;    
        
        I2C_SEM_UNLOCK(unit);
        return ret;
    }
    
    gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id = old_i2c_interface_id;
    devConf->read_type = old_read_type;
    
    I2C_SEM_UNLOCK(unit);

    return ret;
} /* end of drv_i2c_read */

int32
drv_i2c_poe_mcu_write(uint32 unit, drv_i2c_devId_t dev_id, uint32 devaddr, uint8 *pBuff)
{
    int32 ret = RT_ERR_OK;
    //
    int32 old_i2c_interface_id;
    i2c_devConf_t *devConf;

    //RT_INIT_MSG("SENAO:%s, %04d dev=0x%x, addr_len=0x%x\n",__FUNCTION__,__LINE__, dev_id, devaddr);
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

    if(dev_id >= I2C_DEV_ID_END)
        return RT_ERR_OUT_OF_RANGE;

    if(gI2C_dev[unit][dev_id].init_flag != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    I2C_SEM_LOCK(unit);

    devConf = &gI2C_dev[unit][dev_id].conf;
    old_i2c_interface_id = gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id;
    gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id = I2C_INTF_SOFTWARE_WRITEE_MCU_UPGRADE;
    
    if((ret = I2C_CTRL(unit).i2c_write(unit, &gI2C_dev[unit][dev_id].conf, devaddr, pBuff)) != RT_ERR_OK)
    {
    
        gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id = old_i2c_interface_id;
        I2C_SEM_UNLOCK(unit);

        return ret;
    }

    gI2C_dev[unit][devConf->device_id].conf.i2c_interface_id = old_i2c_interface_id;
    I2C_SEM_UNLOCK(unit);

    return ret;


}
#endif

int32
drv_i2c_dev_addr_update  (uint32 unit, uint32 dev, int32 dev_addr)
{
    uint32  dev_id;
    int32   ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID || unit != HWP_MY_UNIT_ID()), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!I2C_CHK(unit), RT_ERR_CHIP_NOT_FOUND);

    // RT_INIT_MSG("  I2C device init (unit %u)\n", unit);

    if(dev >= I2C_DEV_ID_END)
        return RT_ERR_OUT_OF_RANGE;

    I2C_SEM_LOCK(unit);
    dev_id = dev;
    gI2C_dev[unit][dev_id].conf.dev_addr = dev_addr;

    ret = I2C_CTRL(unit).i2c_dev_init(unit, &gI2C_dev[unit][dev_id].conf);
    I2C_SEM_UNLOCK(unit);

    if(ret == RT_ERR_OK)
        gI2C_dev[unit][dev_id].init_flag = INIT_COMPLETED;

    return ret;
}
// Starry@20211007 add PoE MCU Fw upgrade -E
