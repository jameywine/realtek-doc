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
 * $Revision: 81339 $
 * $Date: 2017-08-16 16:49:00 +0800 (Wed, 16 Aug 2017) $
 *
 * Purpose : Realtek Switch SDK Core Module.
 *
 * Feature : Realtek Switch SDK Core Module
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/debug/rt_log.h>
#include <ioal/ioal_init.h>
#include <drv/nic/nic.h>
#include <drv/watchdog/watchdog.h>
#include <osal/isr.h>
#include <dev_config.h>
#include <private/drv/intr/intr.h>
#include <ioal/mem32.h>
#include <hwp/hw_profile.h>
#include <private/drv/swcore/swcore.h>

#if defined(CONFIG_SDK_DRIVER_GPIO)
  #include <drv/gpio/gpio.h>
#endif

#if defined(CONFIG_SDK_TC_DRV)
  #include <drv/tc/tc.h>
  #if defined(CONFIG_SDK_TC_TC1_TIME)
    #include <common/util/rt_util_time.h>
  #endif
#endif

#if defined(CONFIG_SDK_DRIVER_I2C)
#include <drv/i2c/i2c.h>
#endif

#if defined(CONFIG_SDK_RTL8231)
  #include <private/drv/rtl8231/rtl8231_probe.h>
#endif

#if defined(CONFIG_SDK_RTL8390)
  #include <private/drv/swcore/swcore_rtl8390.h>
#endif

#if defined(CONFIG_SDK_DRIVER_L2NTFY)
  #include <drv/l2ntfy/l2ntfy.h>
#endif

#if defined(CONFIG_SDK_UART1)
  #include <drv/uart/uart.h>
#endif

#if defined(CONFIG_SDK_DRIVER_SPI)
  #include <drv/spi/spi.h>
#endif

#if defined(CONFIG_SDK_DRIVER_NIC) && defined(__BOOTLOADER__)
extern int uboot_rtnic_drv_init(uint32 unit);
#endif

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Declaration
 */
static uint32 rtcore_init_status = INIT_NOT_COMPLETED;

/*
 * Function Declaration
 */



/* Function Name:
 *      rtcore_customer_init
 * Description:
 *      Customization for rtcore initialization
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Note:
 *      1. INIT must be initialized before using all of APIs in each modules
 */
int32
rtcore_customer_init(void)
{
#if defined(CONFIG_SDK_FPGA_PLATFORM) && defined(CONFIG_SDK_DRIVER_I2C)
    i2c_devConf_t   i2cDevConf;
    uint32  unit = HWP_MY_UNIT_ID();

  #if defined(CONFIG_SDK_RTL8390)
    if (HWP_8390_50_FAMILY(unit))
    {
        drv_generalCtrlGpio_pinConf_t gpioConfig;
        uint32  dev = 0;
        uint32  pinSDA = 2, pinSCK = 1;
        int32 ret;

        gpioConfig.direction = GPIO_DIR_OUT;
        gpioConfig.default_value = 1;
        gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
        gpioConfig.int_gpio.interruptEnable = GPIO_INT_DISABLE;
        gpioConfig.ext_gpio.debounce = 0;
        gpioConfig.ext_gpio.inverter = 0;
        RT_ERR_CHK(drv_generalCtrlGPIO_pin_init(unit, dev, pinSDA, &gpioConfig), ret);

        gpioConfig.direction = GPIO_DIR_OUT;
        gpioConfig.default_value = 1;
        gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
        gpioConfig.int_gpio.interruptEnable = GPIO_INT_DISABLE;
        gpioConfig.ext_gpio.debounce = 0;
        gpioConfig.ext_gpio.inverter = 0;
        RT_ERR_CHK(drv_generalCtrlGPIO_pin_init(unit, dev, pinSCK, &gpioConfig), ret);

        i2cDevConf.device_id = dev;
        i2cDevConf.dev_addr = 0x5c;
        i2cDevConf.scl_dev = 0;
        i2cDevConf.scl_pin_id = pinSCK;
        i2cDevConf.sda_dev = 0;
        i2cDevConf.sda_pin_id = pinSDA;
        i2cDevConf.mem_addr_width = I2C_ADDR_WIDTH_BYTE_2;
        i2cDevConf.data_width = I2C_DATA_WIDTH_BYTE_4;
        i2cDevConf.scl_delay = 2;
    }
  #endif  /* CONFIG_SDK_RTL8390 */
    drv_i2c_dev_init(unit, &i2cDevConf);
#endif  /* CONFIG_SDK_FPGA_PLATFORM */

    return RT_ERR_OK;

}


/* Function Name:
 *      rtcore_init
 * Description:
 *      Initialize RTCORE module with the specified device
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Note:
 *      1. INIT must be initialized before using all of APIs in each modules
 */
int32
rtcore_init(void)
{
    int32 ret = RT_ERR_FAILED;
    uint32  unit;
    uint32  local_unit;

    RT_INIT_REENTRY_CHK(rtcore_init_status);
    RT_INIT_MSG("RTCORE Driver Module Initialize\n");

    /* Initialize the ioal layer */
    RT_ERR_CHK_EHDL(ioal_init(HWP_DEFHWP_UNIT_ID), ret,
                {RT_INIT_ERR(ret, (MOD_INIT), "unit %u ioal_init fail", HWP_DEFHWP_UNIT_ID);});


#ifndef __BOOTLOADER__
    /* Initialize log module */
    RT_ERR_CHK_EHDL(rt_log_init(), ret,
                    osal_printf("Log init fail %d.\n", ret););
#endif

    /* Initialize hardware profile */
    RT_ERR_CHK_EHDL(hwp_probe(), ret,
                    RT_INIT_ERR(ret, (MOD_INIT), "Hardware-profile probe fail."););

    RT_ERR_CHK_EHDL(hwp_init(), ret,
                    RT_INIT_ERR(ret, (MOD_INIT), "Hardware-profile init fail"););


    if( HWP_DEFHWP_UNIT_ID != HWP_MY_UNIT_ID())
    {
        RT_ERR_CHK_EHDL(ioal_init_unitID_change(HWP_DEFHWP_UNIT_ID, HWP_MY_UNIT_ID()), ret,
                        RT_INIT_ERR(ret, (MOD_INIT), "ioal unit ID change failed"););
    }


    unit = HWP_MY_UNIT_ID();
    hwp_chipInfo_update(unit);

    if(HWP_CPU_EMBEDDED())
    {

#if defined(CONFIG_SDK_DRIVER_GPIO)
        if ((ret = gpio_probe(unit)) == RT_ERR_OK)
        {
            drv_gpio_init(unit);
        }
#endif

#ifndef __BOOTLOADER__
  #if defined(CONFIG_SDK_DRIVER_SPI)
        HWP_UNIT_TRAVS_LOCAL(local_unit)
        {
            RT_ERR_CHK_EHDL(drv_spi_init(local_unit), ret,
                {RT_INIT_ERR(ret, (MOD_INIT), "spi init fail (unit%d) %d!\n", local_unit, ret);});
        }
  #endif

  #if defined(CONFIG_SDK_TC_DRV)
        if ((ret = tc_probe(unit)) == RT_ERR_OK)
        {
            RT_ERR_CHK_EHDL(drv_tc_init(unit), ret,
                {RT_INIT_ERR(ret, (MOD_INIT), "tc init fail (unit%d) %d!\n", unit, ret);});
    #if defined(CONFIG_SDK_TC_TC1_TIME)
      #if defined(CONFIG_SDK_DRIVER_NIC_KERNEL_MODE)
            rt_util_hpt_init();
      #endif
    #endif
        }
  #endif

        /* watchdog */
        if ((ret = watchdog_probe(unit)) == RT_ERR_OK)
        {
            RT_ERR_CHK_EHDL(drv_watchdog_init(unit), ret,
                    {RT_INIT_ERR(ret, (MOD_INIT), "unit %u drv_watchdog_init fail %d!\n", unit, ret);});
        }
#endif //__BOOTLOADER__

#if defined(CONFIG_SDK_DRIVER_I2C)
        if ((ret = i2c_probe(unit)) == RT_ERR_OK)
        {
            RT_ERR_CHK_EHDL(drv_i2c_init(unit), ret,
            {RT_INIT_ERR(ret, (MOD_INIT), "i2c init fail (unit%d) %d!\n", unit, ret);});
        }
#endif


#if defined(CONFIG_SDK_RTL8231)
        /* rtl8231 */
        if ((ret = rtl8231_probe(unit)) == RT_ERR_OK)
        {
            RT_ERR_CHK_EHDL(drv_rtl8231_init(unit), ret,
                    {RT_INIT_ERR(ret, (MOD_INIT), "unit %u drv_rtl8231_init fail %d!\n", unit, ret);});
        }
#endif

#ifndef __BOOTLOADER__
  #if defined(CONFIG_SDK_UART1)
        if ((ret = uart_probe(unit)) == RT_ERR_OK)
        {
            RT_ERR_CHK_EHDL(drv_uart_init(unit), ret,
                        {RT_INIT_ERR(ret, (MOD_INIT), "unit %u drv_uart_init fail %d!\n", unit, ret);});
        }
  #endif
#endif

#if defined(CONFIG_SDK_DRIVER_NIC_KERNEL_MODE) || defined(__BOOTLOADER__)
  #if defined(CONFIG_SDK_DRIVER_NIC)
        if(HWP_NIC_SUPPORT(unit))
        {
            if ( (ret = nic_probe(unit)) != RT_ERR_OK )
            {
                RT_INIT_ERR(ret, (MOD_INIT), "unit %u nic_probe fail %d!\n", unit, ret);
                return ret;
            }

    #ifdef __BOOTLOADER__
            uboot_rtnic_drv_init(unit);
    #endif

        }
  #endif /* defined(CONFIG_SDK_DRIVER_NIC)*/
  #ifndef __BOOTLOADER__
       /* In 9310+8390 FPGA evnironment, we need l2 notification to be enabled in 9310 chip */
        HWP_UNIT_TRAVS_LOCAL(local_unit)
        {
    #if defined(CONFIG_SDK_DRIVER_NIC_KERNEL_MODE) && defined(CONFIG_SDK_DRIVER_L2NTFY)
            if ((ret = l2ntfy_probe(local_unit)) == RT_ERR_OK)
            {
                RT_ERR_CHK_EHDL(drv_l2ntfy_init(local_unit), ret,
                        {RT_INIT_ERR(ret, (MOD_INIT), "unit %u drv_l2ntfy_init fail %d!\n", local_unit, ret);});
            }
    #endif
        }
  #endif
#endif /* defined(CONFIG_SDK_DRIVER_NIC_KERNEL_MODE) */


    }//HWP_CPU_EMBEDDED()

#ifndef __BOOTLOADER__
    rtcore_customer_init();
#endif


    HWP_UNIT_TRAVS_LOCAL(local_unit)
    {
        hwp_chipInfo_update(local_unit);
    }

    rtcore_init_status = INIT_COMPLETED;

    return RT_ERR_OK;

} /* end of rtcore_init */

