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
 * Purpose : Definition those public GPIO routing APIs and its data type in the SDK.
 *
 * Feature : The file have include the following module and sub-modules
 *           (1) Internal GPIO
 *
 */

/*
 * Include Files
 */

#include <common/rt_type.h>
#include <common/debug/rt_log.h>
#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>
#include <private/drv/gpio/gpio.h>
#include <private/drv/gpio/gpio_mapper.h>
#include <hwp/hw_profile.h>
#include <common/util/rt_util_intr.h>

#ifndef __BOOTLOADER__
  #include <dev_config.h>
  #include <osal/wait.h>
  #include <osal/thread.h>
#endif //__BOOTLOADER__
#include <soc/type.h>

#if (defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || (defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && defined(__KERNEL__)))
  #include <osal/atomic.h>
#endif
#include <osal/sem.h>



/*
 * Symbol Definition
 */
#define GPIO_SEM_LOCK(unit)    \
do {\
    if (osal_sem_mutex_take(gpio_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_BSP), "semaphore lock failed");\
        return RT_ERR_SEM_LOCK_FAILED;\
    }\
} while(0)

#define GPIO_SEM_UNLOCK(unit)   \
do {\
    if (osal_sem_mutex_give(gpio_sem[unit]) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_BSP), "semaphore unlock failed");\
        return RT_ERR_SEM_UNLOCK_FAILED;\
    }\
} while(0)


#define I2C_TIME_SINGLE		(2)
#define I2C_TIME_DOUBLE		(2*I2C_TIME_SINGLE)
#define I2C_TIME_QUAD		(4*I2C_TIME_SINGLE)

#define I2C_SCL_ID(t)  	(getI2C_SCL_ID(t))
#define I2C_SDA_ID(t)  	(getI2C_SDA_ID(t))
#define POE_RESET_ID 	(getPoe_Reset_ID())
#define I2C_DELAY_LOOP_COUNT                    (15)
#define I2C_MAXIMUM_SCL_WAITCOUNT               (1000)

#define I2C_ADD_READ_BIT(addr)    	((addr<<1) | 0x01)
#define I2C_ADD_WRITE_BIT(addr)		((addr<<1) & 0xFE)

typedef enum
{
    TP_GPIO_DAT_LOW = 0,
    TP_GPIO_DAT_HIGH,
    TP_GPIO_DAT_TURN
} tp_gpio_dat_t;

/* define GPIO direction */
typedef enum tp_gpio_direction_e
{
	TP_GPIO_DIR_IN = 0,
	TP_GPIO_DIR_OUT,
	TP_GPIO_DIR_END
} tp_gpio_direction_t;

/* define GPIO Interrupt Type */
typedef enum tp_gpio_interruptType_e
{
	TP_GPIO_INT_DISABLE = 0,
	TP_GPIO_INT_FALLING_EDGE,
	TP_GPIO_INT_RISING_EDGE,
	TP_GPIO_INT_BOTH_EDGE,
	TP_GPIO_INT_TYPE_END
} tp_gpio_interruptType_t;

typedef enum
{
	I2C_IOCTL_NULL 			= 0,
	I2C_IOCTL_BUS_INIT 		= 1,
	I2C_IOCTL_ADDR_PROG 	= 2,
	I2C_IOCTL_SCL_ID 		= 3,
	I2C_IOCTL_SDA_ID 		= 4,
	I2C_IOCTL_ADDR_ID 		= 5,
}I2C_IOCTL_ID;

/*
 * Data Declaration
 */
static osal_mutex_t         gpio_sem[RTK_MAX_NUM_OF_UNIT];


typedef struct gpio_database_s
{
    uint32  register_pin;
    uint32  init_pin;
    uint32  action_status;
    drv_gpioIsr_cb_f callback;

} gpio_database_t;


static gpio_database_t data[GPIO_INTERNAL_PIN_END];
rtk_enable_t threadEnabled = DISABLED;

#if defined(RTK_INTR_EVENT_PROC) /* kernel mode in kernel; user mode in user */
static drv_gpioIsr_cb_f gpioInterruptCb[GPIO_INTERNAL_PIN_END];
static osal_event_t drv_gpioIsr_event;
#endif

#if (defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || (defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && defined(__KERNEL__)))
  /* GPIO ISR status shadow database in kernel space */
  uint8 gpio_pin_isr_sts_shadow[RTK_MAX_NUM_OF_UNIT][GPIO_INTERNAL_PIN_END];
#endif

#define I2C_DRV_TYPE_END 3

static osal_mutex_t g_i2c_mutex = 0;
static uint32 g_i2c_scl_id[I2C_DRV_TYPE_END] = {0};
static uint32 g_i2c_sda_id[I2C_DRV_TYPE_END] = {0};
static uint32 g_poe_reset_id = GPIO_ID(GPIO_PORT_A, 5);


uint32 getI2C_SCL_ID(uint8 drv_type)
{
    uint32 value = 0;
    if (drv_type >= I2C_DRV_TYPE_END)
    {
        return -1;
    }
    if (0 == g_i2c_mutex)
    {
        if (0 == (g_i2c_mutex=osal_sem_mutex_create()))
        {
            return -1;
        }
    }
    osal_sem_mutex_take(g_i2c_mutex, OSAL_SEM_WAIT_FOREVER);
    value = g_i2c_scl_id[drv_type];
    osal_sem_give(g_i2c_mutex);
    return value;
}

int setI2C_SCL_ID(uint8 drv_type, uint32 value)
{
    if (drv_type >= I2C_DRV_TYPE_END)
    {
        return -1;
    }
    if (0 == g_i2c_mutex)
    {
        if (0 == (g_i2c_mutex=osal_sem_mutex_create()))
        {
            return -1;
        }
    }
    osal_sem_mutex_take(g_i2c_mutex, OSAL_SEM_WAIT_FOREVER);
    g_i2c_scl_id[drv_type] = value;
    osal_sem_give(g_i2c_mutex);
    return 0;
}

uint32 getI2C_SDA_ID(uint8 drv_type)
{
    uint32 value = 0;
    if (drv_type >= I2C_DRV_TYPE_END)
    {
        return -1;
    }
    if (0 == g_i2c_mutex)
    {
        if (0 == (g_i2c_mutex=osal_sem_mutex_create()))
        {
            return -1;
        }
    }
    osal_sem_mutex_take(g_i2c_mutex, OSAL_SEM_WAIT_FOREVER);
    value = g_i2c_sda_id[drv_type];
    osal_sem_give(g_i2c_mutex);
    return value;
}

int setI2C_SDA_ID(uint8 drv_type, uint32 value)
{
    if (drv_type >= I2C_DRV_TYPE_END)
    {
        return -1;
    }
    if (0 == g_i2c_mutex)
    {
        if (0 == (g_i2c_mutex=osal_sem_mutex_create()))
        {
            return -1;
        }
    }
    osal_sem_mutex_take(g_i2c_mutex, OSAL_SEM_WAIT_FOREVER);
    g_i2c_sda_id[drv_type] = value;
    osal_sem_give(g_i2c_mutex);
    return 0;
}

uint32 getPoe_Reset_ID(void)
{
    uint32 value = 0;
    if (0 == g_i2c_mutex)
    {
        if (0 == (g_i2c_mutex=osal_sem_mutex_create()))
        {
            return -1;
        }
    }
    osal_sem_mutex_take(g_i2c_mutex, OSAL_SEM_WAIT_FOREVER);
    value = g_poe_reset_id;
    osal_sem_give(g_i2c_mutex);
    return value;
}

int setPoe_Reset_ID(uint32 value)
{
    if (0 == g_i2c_mutex)
    {
        if (0 == (g_i2c_mutex=osal_sem_mutex_create()))
        {
            return -1;
        }
    }
    osal_sem_mutex_take(g_i2c_mutex, OSAL_SEM_WAIT_FOREVER);
    g_poe_reset_id = value;
    osal_sem_give(g_i2c_mutex);
    return 0;
}




/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      drv_gpio_init
 * Description:
 *      GPIO driver initilization
 * Input:
 *      unit  - UNIT ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          The purpose of the function is changed. The original pin initialization function
 *          is implemented in the new API: drv_gpio_pin_init.
 */
int32
drv_gpio_init(uint32 unit)
{
    int32       ret;

    RT_INIT_MSG("    GPIO Init");

#if defined(RTK_INTR_EVENT_PROC)
        ret = osal_wait_module_create(&drv_gpioIsr_event);
        if(ret != RT_ERR_OK)
        {
             RT_INIT_MSG(": (WAIT INIT Fail)\n");
            return RT_ERR_FAILED;
        }
#endif


    if (!GPIO_CHK(unit))
    {
        RT_INIT_MSG(": (Not support)\n");
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    /* create semaphore */
    gpio_sem[unit] = osal_sem_mutex_create();
    if (0 == gpio_sem[unit])
    {
        RT_INIT_MSG(": (GPIO semaphore create failed)\n");
        return RT_ERR_FAILED;
    }


    if (GPIO_CTRL(unit).init != NULL)
    {
        if ((ret = GPIO_CTRL(unit).init(unit)) != RT_ERR_OK)
        {
            RT_INIT_MSG(": (Fail)\n");
            return ret;
        }
    }

    RT_INIT_MSG("\n");

    return RT_ERR_OK;
}

/* Function Name:
 *      drv_gpio_direction_set
 * Description:
 *      Set GPIO pin as input or output pin
 * Input:
 *      unit            - UNIT ID
 *      gpioId          - The GPIO port that will be configured
 *      direction       - Data direction, in or out
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_OUT_OF_RANGE
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_gpio_direction_set(
    uint32 unit,
    gpioID gpioId,
    drv_gpio_direction_t direction)
{
    uint32 port = GPIO_PORT(gpioId);
    uint32 pin = GPIO_PIN(gpioId);
    int32  ret;

    /* parameter check */
    RT_PARAM_CHK(!GPIO_PORT_CHK(port), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_PIN_CHK(pin), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_DIR_CHK(direction), RT_ERR_FAILED);


    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).direction_set == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_SEM_LOCK(unit);
    if ((ret = GPIO_CTRL(unit).direction_set(unit, port, pin, direction)) != RT_ERR_OK)
    {
        GPIO_SEM_UNLOCK(unit);
        return ret;
    }
    GPIO_SEM_UNLOCK(unit);
    return RT_ERR_OK;
} /* end of drv_gpio_direction_set */

/* Function Name:
 *      drv_gpio_pin_init
 * Description:
 *      Init GPIO port
 * Input:
 *      unit            - UNIT ID
 *      gpioId          - The GPIO port that will be configured
 *      function        - Pin control function
 *      direction       - Data direction, in or out
 *      interruptEnable - Interrupt mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_OUT_OF_RANGE
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_gpio_pin_init(
    uint32 unit,
    gpioID gpioId,
    drv_gpio_control_t function,
    drv_gpio_direction_t direction,
    drv_gpio_interruptType_t interruptEnable)
{
    uint32 port = GPIO_PORT(gpioId);
    uint32 pin = GPIO_PIN(gpioId);
    int32  ret;

    /* parameter check */
    RT_PARAM_CHK(!GPIO_PORT_CHK(port), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_PIN_CHK(pin), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_CTRLFUNC_CHK(function), RT_ERR_FAILED);
    RT_PARAM_CHK(!GPIO_DIR_CHK(direction), RT_ERR_FAILED);
    RT_PARAM_CHK(!GPIO_INT_CHK(interruptEnable), RT_ERR_FAILED);

    if (!GPIO_CHK(unit))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_SEM_LOCK(unit);
    if (GPIO_CTRL(unit).control_set != NULL)
    {
        if ((ret = GPIO_CTRL(unit).control_set(unit, port, pin, function)) != RT_ERR_OK)
        {
            GPIO_SEM_UNLOCK(unit);
            return ret;
        }
    }

    if (GPIO_CTRL(unit).direction_set != NULL)
    {
        if ((ret = GPIO_CTRL(unit).direction_set(unit, port, pin, direction)) != RT_ERR_OK)
        {
            GPIO_SEM_UNLOCK(unit);
            return ret;
        }
    }

    if (GPIO_CTRL(unit).intrMode_set != NULL)
    {
        if ((ret = GPIO_CTRL(unit).intrMode_set(unit, port, pin, interruptEnable)) != RT_ERR_OK)
        {
            GPIO_SEM_UNLOCK(unit);
            return ret;
        }
    }

    data[PORT_AND_PIN_TO_PINID(port,pin)].init_pin = GPIO_FLAG_ON;

    GPIO_SEM_UNLOCK(unit);
    return RT_ERR_OK;
} /* end of drv_gpio_pin_init */

/* Function Name:
 *      drv_gpio_dataBit_init
 * Description:
 *      Initialize the bit value of a specified GPIO ID
 * Input:
 *      unit   - UNIT ID
 *      gpioId - GPIO ID
 * Output:
 *      data   - Data to write
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_NULL_POINTER
 *      RT_ERR_OUT_OF_RANGE
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      Only the GPO pin need to call the API to init default value.
 * Changes:
 *      None
 */
int32 drv_gpio_dataBit_init(uint32 unit, gpioID gpioId, uint32 data)
{
    uint32 port = GPIO_PORT(gpioId);
    uint32 pin = GPIO_PIN(gpioId);
    int32  ret;

    /* parameter check */
    RT_PARAM_CHK(!GPIO_PORT_CHK(port), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_PIN_CHK(pin), RT_ERR_OUT_OF_RANGE);

    RT_LOG(LOG_FUNC_ENTER, MOD_GENERAL, "[%s():%d] (port=%d,pin=%d)=%d\n", \
            __FUNCTION__, __LINE__, port, pin, data );

    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).dataBit_init == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_SEM_LOCK(unit);
    if ((ret = GPIO_CTRL(unit).dataBit_init(unit, port, pin, data)) != RT_ERR_OK)
    {
        GPIO_SEM_UNLOCK(unit);
        return ret;
    }

    GPIO_SEM_UNLOCK(unit);
    return RT_ERR_OK;
} /* end of drv_gpio_dataBit_init */

/* Function Name:
 *      drv_gpio_dataBit_get
 * Description:
 *      Get the bit value of a specified GPIO ID
 * Input:
 *      unit   - UNIT ID
 *      gpioId - GPIO ID
 * Output:
 *      pData   - Pointer to store return value
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_NULL_POINTER
 *      RT_ERR_OUT_OF_RANGE
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_gpio_dataBit_get(uint32 unit, gpioID gpioId, uint32 *pData)
{
    uint32 port = GPIO_PORT(gpioId);
    uint32 pin = GPIO_PIN(gpioId);
    int32  ret;

    /* parameter check */
    RT_PARAM_CHK(!GPIO_PORT_CHK(port), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_PIN_CHK(pin), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK((NULL == pData), RT_ERR_NULL_POINTER);

    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).dataBit_get == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_SEM_LOCK(unit);
    if ((ret = GPIO_CTRL(unit).dataBit_get(unit, port, pin, pData)) != RT_ERR_OK)
    {
        GPIO_SEM_UNLOCK(unit);
        return ret;
    }

    GPIO_SEM_UNLOCK(unit);

    RT_LOG(LOG_FUNC_ENTER, MOD_GENERAL, "[%s():%d] (port=%d,pin=%d)=%d\n", \
            __FUNCTION__, __LINE__, port, pin, *pData );

    return RT_ERR_OK;
} /* end of drv_gpio_dataBit_get */

/* Function Name:
 *      drv_gpio_dataBit_set
 * Description:
 *      Set GPIO data
 * Input:
 *      unit   - UNIT ID
 *      gpioId - GPIO ID
 *      data   - Data to write
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_OUT_OF_RANGE
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_gpio_dataBit_set(uint32 unit, gpioID gpioId, uint32 data)
{
    uint32 port = GPIO_PORT(gpioId);
    uint32 pin = GPIO_PIN(gpioId);
    int32  ret;

    /* parameter check */
    RT_PARAM_CHK(!GPIO_PORT_CHK(port), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_PIN_CHK(pin), RT_ERR_OUT_OF_RANGE);

    RT_LOG(LOG_FUNC_ENTER, MOD_GENERAL, "[%s():%d] (port=%d,pin=%d)=%d\n", \
            __FUNCTION__, __LINE__, port, pin, data );


    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).dataBit_set == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_SEM_LOCK(unit);
    if ((ret = GPIO_CTRL(unit).dataBit_set(unit, port, pin, data)) != RT_ERR_OK)
    {
        GPIO_SEM_UNLOCK(unit);
        return ret;
    }
    GPIO_SEM_UNLOCK(unit);
    return RT_ERR_OK;
} /* end of drv_gpio_dataBit_set */


/* Function Name:
 *      drv_gpio_isr_get
 * Description:
 *      Get the interrupt status register value of a specified GPIO ID
 * Input:
 *      gpioId  - GPIO ID
 * Output:
 *      pIsr    - Pointer to store return value
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_OUT_OF_RANGE
 *      RT_ERR_NULL_POINTER
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_gpio_isr_get(gpioID gpioId, uint32 *pIsr)
{
    uint32 port = GPIO_PORT(gpioId);
    uint32 pin = GPIO_PIN(gpioId);
    int32  ret;
    uint32 unit = HWP_MY_UNIT_ID();

    /* parameter check */
    RT_PARAM_CHK(!GPIO_PORT_CHK(port), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_PIN_CHK(pin), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK((NULL == pIsr), RT_ERR_NULL_POINTER);

    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).intrStatus_get == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    if ((ret = GPIO_CTRL(unit).intrStatus_get(unit, port, pin, pIsr)) != RT_ERR_OK)
    {
        return ret;
    }
    return RT_ERR_OK;
} /* end of drv_gpio_isr_get */

/* Function Name:
 *      drv_gpio_isr_clear
 * Description:
 *      Clear the interrupt status register value of a specified GPIO ID.
 * Input:
 *      gpioId - GPIO ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_OUT_OF_RANGE
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_gpio_isr_clear(gpioID gpioId)
{
    uint32 port = GPIO_PORT(gpioId);
    uint32 pin = GPIO_PIN(gpioId);
    int32  ret;
    uint32 unit = HWP_MY_UNIT_ID();

    /* parameter check */
    RT_PARAM_CHK(!GPIO_PORT_CHK(port), RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!GPIO_PIN_CHK(pin), RT_ERR_OUT_OF_RANGE);

    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).intrStatus_clear == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    if ((ret = GPIO_CTRL(unit).intrStatus_clear(unit, port, pin)) != RT_ERR_OK)
    {
        return ret;
    }
    return RT_ERR_OK;
} /* end of drv_gpio_isr_clear */

 int adPoeGpioInit(gpioID gpioId,tp_gpio_direction_t direction,tp_gpio_interruptType_t interruptEnable)
 {
	 return drv_gpio_pin_init(0,gpioId, GPIO_CTRLFUNC_NORMAL, direction, interruptEnable);   
 }
 /*!
  *\Function:
  * 	 i2c_gpio_init
  *\Description:
  * 	 init i2c gpio. (SCL SDA)
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 
 int adPoeGpioSet(gpioID gpioId,unsigned int data)
 {
	 return drv_gpio_dataBit_set(0,gpioId, data);
 }
 /*!
  *\Function:
  * 	 i2c_gpio_init
  *\Description:
  * 	 init i2c gpio. (SCL SDA)
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 
 int adPoeGpioGet(gpioID gpioId,unsigned int *data)
 {
	 return drv_gpio_dataBit_get(0,gpioId, data);
 }
 
 /*!
  *\Function:
  * 	 i2c_gpio_init
  *\Description:
  * 	 init i2c gpio. (SCL SDA)
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 void i2c_gpio_init(uint8 drv_type)
 {
	 /* SCL: config as GPIO pins and output HIGH*/
	 adPoeGpioInit(I2C_SCL_ID(drv_type), TP_GPIO_DIR_OUT, TP_GPIO_INT_DISABLE);   
	 adPoeGpioSet(I2C_SCL_ID(drv_type), TP_GPIO_DAT_HIGH);
	 /* SDA: config as GPIO pins and input */
	 adPoeGpioInit(I2C_SDA_ID(drv_type), TP_GPIO_DIR_IN, TP_GPIO_INT_DISABLE);
 }
 
 /*!
  *\Function:
  * 	 i2c_delay
  *\Description:
  * 	 Delay function to facilitate i2c timing.
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 I2C_DELAY_LOOP_COUNT in adPoeI2C.h should be tuned
  * 	 for the intended system setup.
  */
 static void i2c_delay_us(unsigned int loop_cnt)
 {
	 //osal_time_udelay(loop_cnt);
	 //delaytime+=loop_cnt;
 
	 int loop;
	 for (loop = 0; loop < (loop_cnt * I2C_DELAY_LOOP_COUNT); loop++)
	 {
		 loop = loop;
	 }
 }
 
 /*!
  *\Function:
  * 	 i2c_scl_out
  *\Description:
  * 	 SCL (clock) line output
  *\Input:
  * 	 bit_value	 -	1 or 0
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 /*static */void i2c_scl_out(uint8 drv_type, uint8 bit_value)
 {
	 /* DIR: Output */
	 adPoeGpioInit(I2C_SCL_ID(drv_type), TP_GPIO_DIR_OUT, TP_GPIO_INT_DISABLE);
	 /* DAT */
	 if (0 == bit_value)
	 {
		 adPoeGpioSet(I2C_SCL_ID(drv_type), TP_GPIO_DAT_LOW);
	 }
	 else
	 {
		 adPoeGpioSet(I2C_SCL_ID(drv_type), TP_GPIO_DAT_HIGH);
	 }
 }
 
 
 /*!
  *\Function:
  * 	 i2c_scl_in
  *\Description:
  * 	 SCL (clock) line input
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 bit_value	 -	1 or 0
  *\Note:
  * 	 n/a
  */
 /*static */uint8 i2c_scl_in(uint8 drv_type)
 {
	 unsigned int data = 0;
	 adPoeGpioInit(I2C_SCL_ID(drv_type), TP_GPIO_DIR_IN, TP_GPIO_INT_DISABLE); 
	 adPoeGpioGet(I2C_SCL_ID(drv_type), &data);
	 return (uint8)data;
 }
 
 
 /*!
  *\Function:
  * 	 i2c_sda_out
  *\Description:
  * 	 SDA (data) line output
  *\Input:
  * 	 bit_value	 -	1 or 0
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 /*static */void i2c_sda_out(uint8 drv_type, uint8 bit_value)
 {
	 if (0 == bit_value)
	 {
		 /* DIR: Output */
		 adPoeGpioInit(I2C_SDA_ID(drv_type), TP_GPIO_DIR_OUT, TP_GPIO_INT_DISABLE); 
		 /* DAT: LOW */
		 adPoeGpioSet(I2C_SDA_ID(drv_type), TP_GPIO_DAT_LOW);
	 }
	 else
	 {
		 adPoeGpioInit(I2C_SDA_ID(drv_type), TP_GPIO_DIR_IN, TP_GPIO_INT_DISABLE); 
	 }
 }
 
 
 /*!
  *\Function:
  * 	 i2c_sda_in
  *\Description:
  * 	 SDA (data) line input
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 bit_value	 -	1 or 0
  *\Note:
  * 	 n/a
  */
 /*static */uint8 i2c_sda_in(uint8 drv_type)
 {
	/* volatile */unsigned int dat = 0;
	 adPoeGpioInit(I2C_SDA_ID(drv_type), TP_GPIO_DIR_IN, TP_GPIO_INT_DISABLE); 
	 adPoeGpioGet(I2C_SDA_ID(drv_type), &dat);
	 return (uint8)dat;
 }
 
 
 /*!
  *\Function:
  * 	 i2c_wait_for_scl
  *\Description:
  * 	 Waits for the SCL line to return high.
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 Some I2C slave devices will delay the SCL rising edge 
  * 	 for flow control reasons.	
  * 	 The I2C_MAXIMUM_SCL_WAITCOUNT parameter should be tuned 
  * 	 for the intended system setup.
  */
 void i2c_wait_for_scl(uint8 drv_type)
 {
	 int waitcount;
 
	 /* Wait for SCL to come high */
	 for (waitcount = 0; waitcount < I2C_MAXIMUM_SCL_WAITCOUNT; waitcount++)
	 {
		 if (1 == i2c_scl_in(drv_type))
		 {
			 return;
		 }
	 }
 }
 
 
 /*!
  *\Function:
  * 	 i2c_start
  *\Description:
  * 	 Performs an I2C start condition
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 static void i2c_start(uint8 drv_type)
 {
	 i2c_sda_out(drv_type, TP_GPIO_DAT_LOW);
	 i2c_delay_us(I2C_TIME_QUAD);
	 i2c_scl_out(drv_type, TP_GPIO_DAT_LOW);
	 i2c_delay_us(I2C_TIME_QUAD);
 }
 
 
 /*!
  *\Function:
  * 	 i2c_bit
  *\Description:
  * 	 Performs an I2C bit write
  *\Input:
  * 	 signle_bit  - bit value to write on I2C bus
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 static void i2c_bit(uint8 drv_type, uint8 signle_bit)
 {
	 i2c_delay_us(I2C_TIME_SINGLE);
	 i2c_sda_out(drv_type, signle_bit);
	 i2c_delay_us(I2C_TIME_SINGLE);
	 i2c_scl_out(drv_type, TP_GPIO_DAT_HIGH);
	 i2c_delay_us(I2C_TIME_DOUBLE);
	 i2c_scl_out(drv_type, TP_GPIO_DAT_LOW);
 }
 
 
 /*!
  *\Function:
  * 	 i2c_read_bit
  *\Description:
  * 	 Performs an I2C bit read
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 ret_val - the value on SDA line during the read timeslot
  *\Note:
  * 	 n/a
  */
 static uint8 i2c_read_bit(uint8 drv_type)
 {
	 volatile uint8 ret_val;
 
	 i2c_delay_us(I2C_TIME_DOUBLE);
	 i2c_scl_out(drv_type, TP_GPIO_DAT_HIGH);
	 i2c_delay_us(I2C_TIME_SINGLE);
	 ret_val = i2c_sda_in(drv_type);
	 i2c_delay_us(I2C_TIME_SINGLE);
	 i2c_scl_out(drv_type, TP_GPIO_DAT_LOW);
 
	 return ret_val;
 }
 
 
 /*!
  *\Function:
  * 	 i2c_stop
  *\Description:
  * 	 Performs an I2C stop condition
  *\Input:
  * 	 n/a
  *\Output:
  * 	 n/a
  *\Return:
  * 	 n/a
  *\Note:
  * 	 n/a
  */
 static void i2c_stop(uint8 drv_type)
 {
	 i2c_sda_out(drv_type, TP_GPIO_DAT_LOW);
	 i2c_delay_us(I2C_TIME_QUAD);
	 i2c_scl_out(drv_type, TP_GPIO_DAT_HIGH);
	 i2c_delay_us(I2C_TIME_QUAD);
	 i2c_sda_out(drv_type, TP_GPIO_DAT_HIGH);
	 i2c_delay_us(I2C_TIME_QUAD);
 }
 
 
 /*!
  *\Function:
  * 	 i2c_read_byte
  *\Description:
  * 	 Performs an I2C byte read
  *\Input:
  * 	 do_ack  - 1: assert acknowledge after reading 8 bits
  * 			   0: not assert ACK
  *\Output:
  * 	 n/a
  *\Return:
  * 	 result  - byte value on SDA line during read timeslots
  *\Note:
  * 	 n/a
  */
 static uint8 i2c_read_byte(uint8 drv_type, uint8 do_ack)
 {
	 uint8 i;
	 volatile uint8 result = 0;
 
	 for (i = 0; i < 8; i++)
	 {
		 result <<= 1;
		 result |= (i2c_read_bit(drv_type) & 0x01);
	 }
 
	 if (do_ack == 0)
	 {
		 i2c_bit(drv_type, 1);	/* No ACK	  */
	 }
	 else
	 {
		 i2c_bit(drv_type, 0);	/* Do the ACK */
	 }
 
	 /* Release the SDA bus */
	 i2c_sda_in(drv_type);
 
	 return result;
 }
 
 
 
 /*!
  *\Function:
  * 	 i2c_write_byte
  *\Description:
  * 	 Performs an I2C byte write
  *\Input:
  * 	 single_byte - value to write to bus
  *\Output:
  * 	 n/a
  *\Return:
  * 	 returns 0 if byte was acknowledged
  *\Note:
  * 	 n/a
  */
 static uint8 i2c_write_byte(uint8 drv_type, uint8 single_byte)
 {
	 uint8 i;
 
	 for (i = 0; i < 8; i++)
	 {
		 if (single_byte & 0x80)
		 {
			 i2c_bit(drv_type, 1);
		 }
		 else
		 {
			 i2c_bit(drv_type, 0);
		 }
		 
		 single_byte <<= 1;
	 }
 
	 i2c_sda_in(drv_type);
 
	 return i2c_read_bit(drv_type);
 }
 
 
 
 /*!
  *\Function:
  * 	 i2c_select
  *\Description:
  * 	 Perform I2C start and address selection
  *\Input:
  * 	 dev_addr	 - device address.
  * 				   Upper 7 bits are address,
  * 				   LS bit denotes read if 1, write if 0
  *\Output:
  * 	 n/a
  *\Return:
  * 	 returns 0 if device acknowledged address selection
  *\Note:
  * 	 n/a
  */
 /*static*/ uint8 i2c_select(uint8 drv_type, uint8 dev_addr)
 {
	 i2c_start(drv_type);
	 return i2c_write_byte(drv_type, dev_addr);
 }
 
 
 /*!
  *\Function:
  * 	 i2c_write_block
  *\Description:
  * 	 I2C start, 
  * 	 address selection, 
  * 	 write specified bytes, 
  * 	 I2C stop
  *\Input:
  * 	 dev_addr	 - device address, LS bit automatically set to 0
  * 	 barr		 - pointer to array of bytes to write
  * 	 length 	 - number of bytes to write
  *\Output:
  * 	 n/a
  *\Return:
  * 	 returns 0 if device acknowledged data transfer
  *\Note:
  * 	 n/a
  */
 uint8 i2c_write_block(uint8 drv_type, uint8 dev_addr, uint8 *barr, int length)
 {
	 int i;
 
	 /* write device address and R/W# bit */
	 if (i2c_select(drv_type, I2C_ADD_WRITE_BIT(dev_addr)) != 0)
	 {
		 i2c_stop(drv_type);
		 //DBG(" %s : select addr %x write error\r\n", __FUNCTION__,dev_addr);
		 return 1;
	 }
 
	 i2c_delay_us(I2C_TIME_QUAD);
 
	 for (i = 0; i < length; i++)
	 {
		 if (i2c_write_byte(drv_type, barr[i]) != 0)
		 {
			 i2c_stop(drv_type);
			 //DBG(" %s : write_byte error\r\n", __FUNCTION__);
			 return 1;
		 }
		 
		 i2c_delay_us(I2C_TIME_QUAD);
	 }
 
	 i2c_stop(drv_type);
 
	 return 0;
 }
 
 
 
 /*!
  *\Function:
  * 	 i2c_read_block
  *\Description:
  * 	 I2C start, 
  * 	 address selection, 
  * 	 read specified nubmer of bytes, 
  * 	 I2C stop
  *\Input:
  * 	 dev_addr	 - device address, LS bit automatically set to 0
  * 	 length 	 - number of bytes to read
  *\Output:
  * 	 barr		 - pointer to array for read of bytes
  *\Return:
  * 	 returns 0 if device acknowledged data transfer
  *\Note:
  * 	 n/a
  */
 uint8 i2c_read_block(uint8 drv_type, uint8 dev_addr, uint8 *barr, int length)
 {
	 int i;
 
	 /* write device address and R/W# bit */
	 if (i2c_select(drv_type, I2C_ADD_READ_BIT(dev_addr)) != 0)
	 {
		 /* need to do i2c_stop if address selection is not acked */
		 i2c_stop(drv_type);
		 //DBG(" %s : select addr %x error\r\n", __FUNCTION__,dev_addr);
		 return 1;
	 }
 
	 i2c_wait_for_scl(drv_type);
 
	 /* Reduce the length by one.  The last byte must not be ACKed. */
	 length--;
 
	 /* Read every byte but the last */
	 for (i = 0; i < length; i++)
	 {
		 barr[i] = i2c_read_byte(drv_type, 1);
		 i2c_wait_for_scl(drv_type);
	 }
 
	 /* Don't ack the last byte */
	 barr[i] = i2c_read_byte(drv_type, 0);
 
	 i2c_stop(drv_type);
 
	 return 0;
 }
 
 
 
 /*!
  *\Function:
  * 	 i2c_write_read_block
  *\Description:
  * 	 I2C start, 
  * 	 address selection, 
  * 	 write specified bytes,
  * 	 do (re)start, 
  * 	 address selection, 
  * 	 read specified nubmer of bytes, 
  * 	 I2C stop
  *\Input:
  * 	 dev_addr	 - device address, LS bit automatically set to 0
  * 	 barr1		 - pointer to array for write bytes
  * 	 length1	 - number of bytes to write
  * 	 length2	 - number of bytes to read
  *\Output:
  * 	 barr2		 - pointer to array for read of bytes 
  *\Return:
  * 	 returns 0 if device acknowledged data transfer
  *\Note:
  * 	 n/a
  */
 uint8 i2c_write_read_block(uint8 drv_type, uint8 dev_addr, uint8 *barr1, int length1, uint8 *barr2, int length2)
 {
	 int i;
 
	 /* write device address and R/W# bit */
	 if (i2c_select(drv_type, I2C_ADD_WRITE_BIT(dev_addr)) != 0)
	 {
		 i2c_stop(drv_type);
		 //DBG(" %s : select addr %x write error\r\n", __FUNCTION__,dev_addr);
		 return 1;
	 }
		 
	 for (i = 0; i < length1; i++)
	 {
		 i2c_write_byte(drv_type, barr1[i]);
	 }
 
	 /* perform restart */
	 i2c_sda_out(drv_type, TP_GPIO_DAT_HIGH);
	 i2c_delay_us(I2C_TIME_DOUBLE);
	 i2c_scl_out(drv_type, TP_GPIO_DAT_HIGH);
	 i2c_delay_us(I2C_TIME_DOUBLE);
	 if (i2c_select(drv_type, I2C_ADD_READ_BIT(dev_addr)) != 0)
	 {
		 i2c_stop(drv_type);
		 //DBG(" %s : select addr %x read error\r\n", __FUNCTION__,dev_addr);
		 return 1;
	 }
	 /* Reduce the length by one. The last byte must not be ACKed. */
	 length2--;
 
	 /* Read every byte but the last */
	 for (i = 0; i < length2; i++)
	 {
		 barr2[i] = i2c_read_byte(drv_type, 1);
	 }
 
	 /* Don't ack the last byte */
	 barr2[i] = i2c_read_byte(drv_type, 0);
 
	 i2c_stop(drv_type);
 
	 return 0;
 }


 /*typedef enum
{
	I2C_IOCTL_NULL 			= 0,
	I2C_IOCTL_BUS_INIT 		= 1,
	I2C_IOCTL_ADDR_PROG 	= 2,
	I2C_IOCTL_SCL_ID 		= 3,
	I2C_IOCTL_SDA_ID 		= 4,
	I2C_IOCTL_ADDR_ID 		= 5,
}I2C_IOCTL_ID;*/

int32 drv_gpio_i2c_ioctl(uint8 drv_type, uint32 opcode, uint32 data)
{
  int rv = RT_ERR_OK;

	switch(opcode)
	{
		case I2C_IOCTL_BUS_INIT: /* INIT	 */
			i2c_gpio_init(drv_type);
			break;
		case I2C_IOCTL_ADDR_PROG: /* 执行I2C地址配置 */
			//rv = tps23861_saddr_program(drv_type, data);
			/* check valid! */
			if( 0 != rv )
			{
				return RT_ERR_FAILED;
			}
			
			break;
        case I2C_IOCTL_SCL_ID: /* 设置I2C时钟信号的GPIO模拟口 */
            rv = setI2C_SCL_ID(drv_type, data);
            if( 0 != rv )
            {
                return RT_ERR_FAILED;
            }
            break;
        case I2C_IOCTL_SDA_ID: /* 设置I2C数据信号的GPIO模拟口 */
            rv = setI2C_SDA_ID(drv_type, data);
            if( 0 != rv )
            {
                return RT_ERR_FAILED;
            }
            break;
		case I2C_IOCTL_ADDR_ID: /* 设置I2C数据信号的GPIO模拟口 */
           // rv = tps23861_saddr_id_config(drv_type, data);
            if( 0 != rv )
            {
                return RT_ERR_FAILED;
            }
            break;
		default:
			break;
	}
	
	return RT_ERR_OK;
} /* end of drv_gpio_dataBit_init */


int32 drv_gpio_i2c_read(uint8 drv_type, uint32 dev, uint32 reg, uint32 *pData)
{
	int rv=0;
	//int i,index;
	//int retry = 0;
	unsigned char temp_buffer[2];

	temp_buffer[0]=reg;
	//while( retry++ < I2C_BUS_RETRY_TIMES )
	{
		rv = i2c_write_read_block(drv_type, dev, &temp_buffer[0], 1, &temp_buffer[1], 1);
		*pData =(uint32)temp_buffer[1];
		if(rv == 0)
		{
			//printk(" drv_gpio_i2c_read : [%02X] = 0x%02x\r\n",temp_buffer[0],temp_buffer[1]);
			return RT_ERR_OK;
		}
	}
	return RT_ERR_FAILED;
} /* end of drv_gpio_dataBit_init */

/* Function Name:
 *      drv_gpio_dataBit_init
 * Description:
 *      Initialize the bit value of a specified GPIO ID
 * Input:
 *      gpioId - GPIO ID
 * Output:
 *      data   - Data to write
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_OUT_OF_RANGE
 * Note:
 *      Only the GPO pin need to call the API to init default value.
 */
int32 drv_gpio_i2c_write(uint8 drv_type, uint32 dev, uint32 reg, uint32 data)
{
	int rv=0;
	//int i,index;
	//int retry = 0;
	unsigned char temp_buffer[2];
	
	temp_buffer[0] = reg;
	temp_buffer[1] = data;
	//while( retry++ < I2C_BUS_RETRY_TIMES )
	{
		rv=i2c_write_block(drv_type, dev, temp_buffer, 2);
		if(rv == 0)
		{
			return RT_ERR_OK;
		}
	}

	return RT_ERR_FAILED;
} /* end of drv_gpio_dataBit_init */


int32 drv_gpio_i2c_write_block(uint8 drv_type, uint32 dev, uint8 reg, uint8 *Data, int length)
{
	int i;
	
	/* write device address and R/W# bit */
	if (i2c_select(drv_type, I2C_ADD_WRITE_BIT(dev)) != 0)
	{
		i2c_stop(drv_type);
		//DBG(" %s : select addr %x write error\r\n", __FUNCTION__,dev_addr);
		return 1;
	}
	
	i2c_delay_us(I2C_TIME_QUAD);

	if (i2c_write_byte(drv_type, reg) != 0)
	{
		i2c_stop(drv_type);
		//DBG(" %s : write_byte error\r\n", __FUNCTION__);
		return 1;
	}

		
	for (i = 0; i < length; i++)
	{
		if (i2c_write_byte(drv_type, Data[i]) != 0)
		{
			i2c_stop(drv_type);
			//DBG(" %s : write_byte error\r\n", __FUNCTION__);
			return 1;
		}
		
		i2c_delay_us(I2C_TIME_QUAD);
	}
	
	i2c_stop(drv_type);
	
	return 0;


	
} /* end of drv_gpio_dataBit_init */



int32 drv_gpio_i2c_probe(uint8 drv_type, uint32 regAddr)
{
	//int retry = 0;	
	//while( retry++ < I2C_BUS_RETRY_TIMES )
	{

		//if (i2c_select(drv_type, I2C_ADD_READ_BIT(regAddr)) != 0)
		if (i2c_select(drv_type, I2C_ADD_WRITE_BIT(regAddr)) != 0)
		{
			/* need to do i2c_stop if address selection is not acked */
			i2c_stop(drv_type);
			/*DBG(" %s : not found %x!\n",__FUNCTION__, regAddr);*/
			return RT_ERR_FAILED;
		}
		else
		{
			i2c_stop(drv_type);
			//DBG(" %s : has found %x!\n",__FUNCTION__, regAddr);
			return RT_ERR_OK;
		}
	}
	
	return RT_ERR_FAILED;

} 




#if defined(RTK_INTR_EVENT_PROC)

/* Function Name:
 *      _gpio_interrupt_handler
 * Description:
 *      Pending interrupt thread of gpio interrupt for waiting interrupt event.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static int _gpio_interrupt_handler(void)
{
    int32 port,pin = 0;
    int32 portMin = 0, portMax = -1;
    int32 ret = RT_ERR_OK;
    gpio_pin_data_t  gpio_data;
    uint32 unit = HWP_MY_UNIT_ID();

    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).portRange_get == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_CTRL(unit).portRange_get(unit, &portMin, &portMax);


    for(port = portMin; port <= portMax; port++)
    {
        for(pin = GPIO_PIN_MIN; pin <= GPIO_PIN_MAX ; pin++)
        {
            if(data[PORT_AND_PIN_TO_PINID(port,pin)].action_status == ENABLED){
                gpio_data.pin_id = PORT_AND_PIN_TO_PINID(port, pin);
                ret = drv_gpio_dataBit_get(unit, GPIO_ID(port,pin), (uint32 *)&(gpio_data.gpio_bit));
                if (RT_ERR_OK != ret)
                {
                    break;
                }
                gpio_data.dev_id = 0;
                gpioInterruptCb[PORT_AND_PIN_TO_PINID(port, pin)](&gpio_data);
                data[PORT_AND_PIN_TO_PINID(port, pin)].action_status = DISABLED;
            }
        }
    }
    return RT_ERR_OK;
}

/* Function Name:
 *      _gpio_intr_thread
 * Description:
 *      GPIO interrupt thread to receive interrupt event.
 * Input:
 *      pArg - Parameter that is provided when thread create.
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void *_gpio_intr_thread(void *pArg)
{

    while(1)
    {
        osal_wait_event(drv_gpioIsr_event);

        _gpio_interrupt_handler();
    }
    return NULL;
}

/* Function Name:
 *      gpio_intr_attach
 * Description:
 *      Connect interrupt with handle thread
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32 gpio_intr_attach(void)
{
    int32 ret;

    /* Check arguments */
    ret = osal_thread_create(GPIO_INTR_THREAD_NAME, 4096, 0, (void *)_gpio_intr_thread, NULL);
    if (0 == ret)
    {
      RT_ERR(ret, MOD_RTCORE, "GPIO interrupt thread create failed");
      return RT_ERR_FAILED;
    }
    return RT_ERR_OK;
}

/* Function Name:
*      drv_gpio_isr
* Description:
*      GPIO ABCD and EFG interrupt handler routine
* Input:
*      pParam - isr parameter
* Output:
*      None
* Return:
*      SYS_ERR_OK
*      SYS_ERR_FAILED
*      RT_ERR_CHIP_NOT_SUPPORTED
* Note:
*      None
*/
osal_isrret_t drv_gpio_isr(void *pParam)
{
    int32 port,pin = 0;
    int32 portMin = 0, portMax = -1;
    uint32 gIsr = 0;
    int32 ret = RT_ERR_OK;
    uint32      unit = HWP_MY_UNIT_ID();


    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).portRange_get == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_CTRL(unit).portRange_get(unit, &portMin, &portMax);


    for(port = portMin; port <= portMax; port++)
    {
        for(pin = GPIO_PIN_MIN; pin <= GPIO_PIN_MAX ; pin++ )
        {
            ret = drv_gpio_isrStsShadow_get(unit, PORT_AND_PIN_TO_PINID(port,pin), &gIsr);
            if (RT_ERR_OK != ret)
            {
                return ret;
            }

            if (gIsr != 0)
            {
                if(data[PORT_AND_PIN_TO_PINID(port,pin)].init_pin == GPIO_FLAG_ON && data[PORT_AND_PIN_TO_PINID(port,pin)].register_pin == GPIO_FLAG_ON)
                {
                    data[PORT_AND_PIN_TO_PINID(port,pin)].action_status = ENABLED;
                }
            }
        }/* end for */
    }/* end for */

    osal_wake_up(drv_gpioIsr_event);
    return RT_ERR_OK;

}


/* Function Name:
 *      drv_gpio_intrHandler_register
 * Description:
 *      register GPIO interrupt callback function for specified GPIO pin
 * Input:
 *      None
 * Output:
 *      pin             - specified GPIO pin
 *      gpioIsrCallback - callback function
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32 drv_gpio_intrHandler_register(GPIO_INTERNAL_PIN_t pin, drv_gpioIsr_cb_f gpioIsrCallback)
{

    int i = 0;
    int32 ret = RT_ERR_FAILED;
    rtk_enable_t regIntrSrc = DISABLED;


    RT_PARAM_CHK((NULL == gpioIsrCallback), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(IS_GPIO_PIN_INVALID(pin), RT_ERR_INPUT);

    if(threadEnabled == DISABLED){
        gpio_intr_attach();
        threadEnabled = ENABLED;
    }

    if(pin < GPIO_INTERNAL_PIN_END && pin >= A0){

        for(i = A0 ; i < GPIO_INTERNAL_PIN_END ; i++){
            if(data[i].register_pin == GPIO_FLAG_ON)
                regIntrSrc = ENABLED;
        }

        if(regIntrSrc == DISABLED){
            /* Register GPIO IRQ */
            RT_ERR_HDL(rt_util_intr_isr_register(INTR_ISR_GPIO, drv_gpio_isr, NULL), error, ret);

        }
        gpioInterruptCb[pin] = gpioIsrCallback;
        data[pin].register_pin = GPIO_FLAG_ON;
    }

    return RT_ERR_OK;

    error:
        RT_ERR(RT_ERR_FAILED, MOD_RTCORE, "Register GPIO IRQ and Interrupt Handler Failed!");

    return RT_ERR_FAILED;
}


/* Function Name:
 *      drv_gpio_intrHandler_unregister
 * Description:
 *      unregister GPIO interrupt callback function for specified GPIO pin
 * Input:
 *      None
 * Output:
 *      pin - specified GPIO pin
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      None
 */
int32 drv_gpio_intrHandler_unregister(GPIO_INTERNAL_PIN_t pin)
{
    int i = 0;
    int32 ret = RT_ERR_FAILED;
    rtk_enable_t unRegIntrSrc = DISABLED;

    RT_PARAM_CHK(IS_GPIO_PIN_INVALID(pin), RT_ERR_INPUT);

    data[pin].register_pin = GPIO_FLAG_OFF;

    if(pin < GPIO_INTERNAL_PIN_END && pin >= A0){

        for(i = A0 ; i < GPIO_INTERNAL_PIN_END ; i++){
            if(data[i].register_pin == GPIO_FLAG_ON)
                unRegIntrSrc = ENABLED;
        }

        if(unRegIntrSrc == DISABLED)
            RT_ERR_HDL(rt_util_intr_isr_unregister(INTR_ISR_GPIO), error, ret);


    }

    return RT_ERR_OK;


    error:
        RT_ERR(RT_ERR_FAILED, MOD_RTCORE, "Unregister GPIO IRQ and Interrupt Handler Failed!");

    return RT_ERR_FAILED;

}
#endif //#if defined(RTK_INTR_EVENT_PROC)


#if (defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || (defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && defined(__KERNEL__)))
/* Function Name:
 *      drv_gpio_isrStsShadow_backup
 * Description:
 *      back up GPIO status in shadow database
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_CHIP_NOT_SUPPORTED
 * Note:
 *      None
 */
int32
drv_gpio_isrStsShadow_backup(void)
{
    uint32  port, pin, gIsr;
    int32   ret, portMin = 0, portMax = -1;
    uint32  unit = HWP_MY_UNIT_ID();


    if (!GPIO_CHK(unit) || (GPIO_CTRL(unit).portRange_get == NULL))
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    GPIO_CTRL(unit).portRange_get(unit, &portMin, &portMax);

    for(port = portMin; port <= portMax; port++)
    {
        for(pin = GPIO_PIN_MIN; pin <= GPIO_PIN_MAX ; pin++ )
        {
            ret = drv_gpio_isr_get(GPIO_ID(port,pin),(uint32 *)&gIsr);
            if (RT_ERR_OK != ret)
            {
                break;
            }

            if (gIsr != 0)
            {
                gpio_pin_isr_sts_shadow[unit][PORT_AND_PIN_TO_PINID(port,pin)] = 1;
                drv_gpio_isr_clear(GPIO_ID(port,pin));
            }
        }/* end for */
    }/* end for */
    return RT_ERR_OK;
}

/* Function Name:
 *      drv_gpio_isrStsShadow_get
 * Description:
 *      Get shadowed GPIO ISR status and clear the status to 0
 * Input:
 *      unit - unit ID
 *      pin - specified GPIO pin
 * Output:
 *      *pData - GPIO ISR status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
drv_gpio_isrStsShadow_get(uint32 unit, GPIO_INTERNAL_PIN_t pin, uint32 *pData)
{

    if (unit >= RTK_MAX_NUM_OF_UNIT)
    {
        return RT_ERR_INPUT;
    }

    if (pin >= GPIO_INTERNAL_PIN_END)
    {
        return RT_ERR_INPUT;
    }

    osal_isr_disable_interrupt();
    *pData = gpio_pin_isr_sts_shadow[unit][pin]; /* get status */
    gpio_pin_isr_sts_shadow[unit][pin] = 0; /* clear */
    osal_isr_enable_interrupt();
    return RT_ERR_OK;
}
#endif /* #if (defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || (defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && defined(__KERNEL__))) */




