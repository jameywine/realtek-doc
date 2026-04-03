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
 * Purpose : SPI master driver to max.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) SPI device-type-2 (dev6) register read and write
 *           2) dev6 Frame format:
 *              (A) 8-bit command byte: [reg4,reg3,reg2,reg1,reg0,X,Read/Write,X]
 *                  (a) reg4~reg0: 5-bit register address
 *                  (b) X: don't care
 *                  (c) Read/Write: 0-read, 1-write
 *              (B) Then, 8-bit read/write data: [First-bit, ......, Last-bit]
 *
 */
#include <common/rt_autoconf.h>
#include <osal/sem.h>
#include <osal/time.h>
#include <osal/lib.h>
#include <common/debug/rt_log.h>
#include <common/rt_type.h>
#include <ioal/ioal_init.h>
#include <ioal/mem32.h>
#include <private/drv/swcore/swcore_rtl9300.h>
#include <hal/chipdef/chip.h>
#include <drv/gpio/generalCtrl_gpio.h>
#include <drv/spi/spi.h>
#include <private/drv/spi/spi_dev6.h>
#include <hwp/hw_profile.h>

/*
 * ================================(Customization)================================
 */

/* define SPI_MASTER_INTF to choose which interface as SPI interface */
#define SPI_MASTER_INTF_9300GPIO              1
#define SPI_MASTER_INTF                       SPI_MASTER_INTF_9300GPIO


/* define this will using another API to access GPIO, to get lower API overhead */
#define SPI_USING_DIRECT_GPIO_REG_ACCESS

/* semaphore, please define per your requirement */
//to sync tne usage of GPIO DATA register
#define SPI_GpioDataReg_LOCK()
#define SPI_GpioDataReg_UNLOCK()

//to sync the usage of SPI master interface
#define SPI_MasterIntf_LOCK()
#define SPI_MasterIntf_UNLOCK()

#if (RT_SPI_USE_FLASH_SEMA == 1) && defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && defined(__KERNEL__)
  #include <linux/version.h>
  #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26))
    #include <asm/semaphore.h>
  #else
    #include <linux/semaphore.h>
  #endif
  #include <linux/slab.h>
#endif


/* GPIO pin number definition to simulate SPI */
typedef enum spiGpio_pin_define_e{
    SPI_GPIO_PIN_SCLK  =3,
    SPI_GPIO_PIN_MOSI  =5,
    SPI_GPIO_PIN_MISO  =4,
    SPI_GPIO_PIN_SS    =7,
    SPI_GPIO_PIN_RES   =0,
    SPI_GPIO_PIN_INT   =1,
}spiGpio_pin_define_t;


/*
 * ============================(End of customization)============================
 */

uint32 sgPin_SCLK  = SPI_GPIO_PIN_SCLK;
uint32 sgPin_MOSI  = SPI_GPIO_PIN_MOSI;
uint32 sgPin_MISO  = SPI_GPIO_PIN_MISO;
uint32 sgPin_SS    = SPI_GPIO_PIN_SS;
uint32 sgPin_RES   = SPI_GPIO_PIN_RES;
uint32 sgPin_INT   = SPI_GPIO_PIN_INT;
static uint8 spi_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};

#define SS_ACTIVE       0
#define SS_INACTIVE     1


/* define GPIO register address */
unsigned volatile int gpio_data_register=0xb800330C;

/* define GPIO pin bitmap for operation */
typedef struct gpio_pin_info_s{
    uint8   pin_num;        /* which GPIO pin */
    uint32  pin_mask;       /* turn on the register bit of this GPIO pin */
    uint32  pin_mask_not;   /* ~pin_mask */
}gpio_pin_info_t;

gpio_pin_info_t gpioInfo[]={
    { 0, 0x00000001, 0xfffffffe},
    { 1, 0x00000002, 0xfffffffd},
    { 2, 0x00000004, 0xfffffffb},
    { 3, 0x00000008, 0xfffffff7},
    { 4, 0x00000010, 0xffffffef},
    { 5, 0x00000020, 0xffffffdf},
    { 6, 0x00000040, 0xffffffbf},
    { 7, 0x00000080, 0xffffff7f},
    { 8, 0x00000100, 0xfffffeff},
    { 9, 0x00000200, 0xfffffdff},
    { 10, 0x00000400, 0xfffffbff},
    { 11, 0x00000800, 0xfffff7ff},
    { 12, 0x00001000, 0xffffefff},
    { 13, 0x00002000, 0xffffdfff},
    { 14, 0x00004000, 0xffffbfff},
    { 15, 0x00008000, 0xffff7fff},
    { 16, 0x00010000, 0xfffeffff},
    { 17, 0x00020000, 0xfffdffff},
    { 18, 0x00040000, 0xfffbffff},
    { 19, 0x00080000, 0xfff7ffff},
    { 20, 0x00100000, 0xffefffff},
    { 21, 0x00200000, 0xffdfffff},
    { 22, 0x00400000, 0xffbfffff},
    { 23, 0x00800000, 0xff7fffff},

};

/* define command type for read/write API */
typedef enum{
    SPI_DEV6_CMD_READ     = 0x01,
    SPI_DEV6_CMD_WRITE    = 0x02,
    SPI_DEV6_CMD_NONE     = 0x03,
}spi_cmdType_t;

typedef struct spi_cmd_s{
    uint32        flags;
    spi_cmdType_t cmd_t;
    uint32        address;
    uint32        *buf;     /*request buffer*/
    uint32        size;     /*request IO size*/
}spi_cmd_t;

/* define read/write direction */
#define SPI_DEV6_DIR_WRITE                  (0x2)
#define SPI_DEV6_DIR_READ                   (0x0)

/* for using RTK general GPIO API */
drv_generalCtrlGpio_devId_t                 sg_dev = GEN_GPIO_DEV_ID0_INTERNAL;
drv_generalCtrlGpio_devId_t                 sg_dev_ext = GEN_GPIO_DEV_ID1;

#define DRV_GPIO_SET(unit,dev,pin,data)     drv_generalCtrlGPIO_dataBit_set(unit,dev,pin,data);
#define DRV_GPIO_GET(unit,dev,pin,pData)    drv_generalCtrlGPIO_dataBit_get(unit,dev,pin,pData);


/*
 * GPIO pin operation
 */

uint32 delay = 10;

uint32 spiGpio_SShi(uint32 data_reg_value)
{
    data_reg_value |= gpioInfo[sgPin_SS].pin_mask;
    REG32(gpio_data_register) = data_reg_value;

    osal_time_udelay(delay*6);
    //osal_printf("%s %x\n", __FUNCTION__, REG32(gpio_data_register));

    return data_reg_value;
}

uint32 spiGpio_SSlo(uint32 data_reg_value)
{
    data_reg_value &= gpioInfo[sgPin_SS].pin_mask_not;
    REG32(gpio_data_register) = data_reg_value;
    //osal_printf("%s %x\n", __FUNCTION__, REG32(gpio_data_register));
    osal_time_udelay(delay*2);

    return data_reg_value;
}


uint32 spiGpio_SCLKhi(uint32 data_reg_value)
{
    data_reg_value |= gpioInfo[sgPin_SCLK].pin_mask;
    REG32(gpio_data_register) = data_reg_value;
    osal_time_udelay(delay);
    //osal_printf("%s %x\n", __FUNCTION__, REG32(gpio_data_register));
    //osal_printf("%x REG32(0xb800330c):%x exp %x\n", REG32(0xbb0003a0), REG32(0xbb00330c) ,data_reg_value );

    return data_reg_value;
}

uint32 spiGpio_SCLKlo(uint32 data_reg_value)
{
    data_reg_value &= gpioInfo[sgPin_SCLK].pin_mask_not;
    REG32(gpio_data_register) = data_reg_value;
    osal_time_udelay(delay);

    //osal_printf("%s %x\n", __FUNCTION__, REG32(gpio_data_register));
    //osal_printf("Low REG32(0xb800330c):%x exp %x\n", REG32(0xbb00330c) ,data_reg_value );

    return data_reg_value;
}

uint32 spiGpio_MOSIhi(uint32 data_reg_value)
{
    data_reg_value |= gpioInfo[sgPin_MOSI].pin_mask;
    REG32(gpio_data_register) = data_reg_value;

    return data_reg_value;
}

uint32 spiGpio_SCLKlo_MOSIhi(uint32 data_reg_value)
{
    data_reg_value &= gpioInfo[sgPin_SCLK].pin_mask_not;
    data_reg_value |= gpioInfo[sgPin_MOSI].pin_mask;
    REG32(gpio_data_register) = data_reg_value;
    osal_time_udelay(delay);

    return data_reg_value;
}

uint32 spiGpio_SCLKlo_MOSIlo(uint32 data_reg_value)
{
    data_reg_value &= gpioInfo[sgPin_SCLK].pin_mask_not;
    data_reg_value &= gpioInfo[sgPin_MOSI].pin_mask_not;
    REG32(gpio_data_register) = data_reg_value;
    osal_time_udelay(delay);

    return data_reg_value;
}

uint32 spiGpio_MISO_get(uint32 *data, uint32 data_reg_value)
{
    data_reg_value = REG32(gpio_data_register);
    *data = ((data_reg_value & gpioInfo[sgPin_MISO].pin_mask) == 0) ? 0 : 1;
    return data_reg_value;
}




uint32 _spi_bitbang_bitSet(uint32 unit, uint8 data, uint32 data_reg_value)
{
#ifdef SPI_USING_DIRECT_GPIO_REG_ACCESS
  if(data)
    data_reg_value = spiGpio_SCLKlo_MOSIhi(data_reg_value);
  else
    data_reg_value = spiGpio_SCLKlo_MOSIlo(data_reg_value);
  data_reg_value = spiGpio_SCLKhi(data_reg_value);
#else
  DRV_GPIO_SET(unit, sg_dev, sgPin_SCLK, 0);
  DRV_GPIO_SET(unit, sg_dev, sgPin_MOSI, data);
  DRV_GPIO_SET(unit, sg_dev, sgPin_SCLK, 1);
#endif
  return data_reg_value;
}


uint32 _spi_bitbang_bitGet(uint32 unit, uint32 *data, uint32 data_reg_value)
{

#ifdef SPI_USING_DIRECT_GPIO_REG_ACCESS
  data_reg_value = spiGpio_SCLKlo(data_reg_value);
  data_reg_value = spiGpio_MISO_get(data,data_reg_value);
  data_reg_value = spiGpio_SCLKhi(data_reg_value);
  //data_reg_value = spiGpio_MISO_get(data,data_reg_value);
#else
  DRV_GPIO_SET(unit, sg_dev, sgPin_SCLK, 0);
  DRV_GPIO_SET(unit, sg_dev, sgPin_SCLK, 1);
  DRV_GPIO_GET(unit, sg_dev, sgPin_MISO, data);
#endif
  return data_reg_value;
}

uint32 _spi_bitbang_read(uint32 unit, uint32 addr, uint32 *pData, uint32 data_reg_value)
{
  int i;
  uint32 cmd = 0x3, bit;


  for(i=7;i>=0;i--)
      data_reg_value = _spi_bitbang_bitSet(unit, (cmd & (1 << i)) >> i, data_reg_value);

  for(i=23;i>=0;i--)
      data_reg_value = _spi_bitbang_bitSet(unit, (addr & (1 << i)) >> i, data_reg_value);

  for(i=7;i>=0;i--)
      data_reg_value = _spi_bitbang_bitSet(unit, 0, data_reg_value);

  data_reg_value = spiGpio_MOSIhi(data_reg_value);

  *pData = 0;

  for(i = 31;i >= 0;i--)
  {
      data_reg_value = _spi_bitbang_bitGet(unit, &bit, data_reg_value);
      *pData |= (bit << i);

  }

  data_reg_value = spiGpio_SCLKlo(data_reg_value);

  return data_reg_value;
}

uint32 _spi_bitbang_write(uint32 unit, uint32 addr, uint32 data, uint32 data_reg_value)
{
  int i;
  uint32 cmd = 0x2;
  uint8 bit;


  for(i=7;i>=0;i--)
      data_reg_value = _spi_bitbang_bitSet(unit, (cmd & (1 << i)) >> i, data_reg_value);

  for(i=23;i>=0;i--)
      data_reg_value = _spi_bitbang_bitSet(unit, (addr & (1 << i)) >> i, data_reg_value);

  for(i=7;i>=0;i--)
      data_reg_value = _spi_bitbang_bitSet(unit, 0, data_reg_value);

  for(i = 31;i >= 0;i--)
  {
      bit = (data >> i) & 1;
      data_reg_value = _spi_bitbang_bitSet(unit, bit, data_reg_value);
  }

  data_reg_value = spiGpio_SCLKlo_MOSIhi(data_reg_value);

  return data_reg_value;
}

int _spi_dev6cmd_9300Gpio(uint32 unit, spi_cmd_t *req)
{
  uint32 regAddr = req->address;
  unsigned volatile int gpioDataReg_value;


  SPI_GpioDataReg_LOCK();

  /* store GPIO data register and pass to each API, to get more performance by skip reading IO */
  gpioDataReg_value = REG32(gpio_data_register);

  /* active chip select */
#ifdef SPI_USING_DIRECT_GPIO_REG_ACCESS
  gpioDataReg_value = spiGpio_SSlo(gpioDataReg_value);
#else
  DRV_GPIO_SET(unit, sg_dev, sgPin_SS, SS_ACTIVE);
#endif

  if(req->cmd_t==SPI_DEV6_CMD_READ)
  {
    gpioDataReg_value = _spi_bitbang_read(unit, regAddr, req->buf, gpioDataReg_value);
  }
  else
  {
    gpioDataReg_value = _spi_bitbang_write(unit, regAddr, *req->buf, gpioDataReg_value);
  }

  /* inactive chip select */
#ifdef SPI_USING_DIRECT_GPIO_REG_ACCESS
      spiGpio_SShi(gpioDataReg_value);
#else
      DRV_GPIO_SET(unit, sg_dev, sgPin_SS, SS_INACTIVE);
#endif

  SPI_GpioDataReg_UNLOCK();

  return RT_ERR_OK;
}





int32 spi_dev6SpiPin_init(uint32 unit, spi_init_info_t *init_info)
{
    drv_generalCtrlGpio_devConf_t devConfig;
    drv_generalCtrlGpio_pinConf_t gpioConfig;
    uint32 delay_step[] = {1, 3, 4, 10, 50, 100, 200, 500};
    uint32 i, buff;

    RT_INIT_REENTRY_CHK(spi_init[unit]);

    /* init GPIO device */
    drv_generalCtrlGPIO_dev_init(unit,sg_dev, &devConfig);
    drv_generalCtrlGPIO_devEnable_set(unit,sg_dev,ENABLED);

    /* init GPIO pin */
    gpioConfig.direction = GPIO_DIR_IN;
    gpioConfig.default_value = 1;
    gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
    gpioConfig.int_gpio.interruptEnable = GPIO_INT_DISABLE;
    drv_generalCtrlGPIO_pin_init(unit, sg_dev,init_info->gpioNum_miso, &gpioConfig);
    sgPin_MISO  = init_info->gpioNum_miso;

    gpioConfig.direction = GPIO_DIR_OUT;
    gpioConfig.default_value = 1;
    gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
    gpioConfig.int_gpio.interruptEnable = GPIO_INT_DISABLE;
    drv_generalCtrlGPIO_pin_init(unit, sg_dev, init_info->gpioNum_mosi, &gpioConfig);
    sgPin_MOSI  = init_info->gpioNum_mosi;

    gpioConfig.direction = GPIO_DIR_OUT;
    gpioConfig.default_value = 1;
    gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
    gpioConfig.int_gpio.interruptEnable = GPIO_INT_DISABLE;
    drv_generalCtrlGPIO_pin_init(unit, sg_dev,init_info->gpioNum_sclk, &gpioConfig);
    sgPin_SCLK  = init_info->gpioNum_sclk;

    gpioConfig.direction = GPIO_DIR_OUT;
    gpioConfig.default_value = 1;
    gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
    gpioConfig.int_gpio.interruptEnable = GPIO_INT_DISABLE;
    drv_generalCtrlGPIO_pin_init(unit, sg_dev,init_info->gpioNum_ss, &gpioConfig);
    sgPin_SS    = init_info->gpioNum_ss;

    /*
    gpioConfig.direction = GPIO_DIR_IN;
    gpioConfig.default_value = 1;
    gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
    gpioConfig.int_gpio.interruptEnable = GPIO_INT_FALLING_EDGE;
    drv_generalCtrlGPIO_pin_init(unit, sg_dev, init_info->gpioNum_int, &gpioConfig);
    sgPin_INT   = init_info->gpioNum_int;

    gpioConfig.direction = GPIO_DIR_OUT;
    gpioConfig.default_value = 1;
    gpioConfig.int_gpio.function = GPIO_CTRLFUNC_NORMAL;
    gpioConfig.int_gpio.interruptEnable = GPIO_INT_DISABLE;
    drv_generalCtrlGPIO_pin_init(unit, sg_dev, init_info->gpioNum_reset, &gpioConfig);
    sgPin_RES   = init_info->gpioNum_reset;*/

    //set CLOCK as default low
    DRV_GPIO_SET(unit, sg_dev, sgPin_SCLK, 0);

    DRV_GPIO_SET(unit, sg_dev, sgPin_MOSI, 1);


    //set CS high to inactive
    DRV_GPIO_SET(unit, sg_dev, sgPin_SS, SS_INACTIVE);

    for(i = 0; i < sizeof(delay_step)/sizeof(uint32); i ++)
    {
        delay = delay_step[i];
        spi_dev6Reg_read(0, 0x4, &buff);
        osal_printf("delay %d us, read result %x\n", delay, buff);
        if((buff >> 16) == 0x9330)
        {
            break;
        }
    }


    spi_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}


int32 _spi_dev6_9300Gpio_init(uint32 unit)
{
    spi_init_info_t init_info;

    if( unit != HWP_MY_UNIT_ID())
        return RT_ERR_OK;

    REG32(0xbb0003a0) = 0x340000;

    osal_printf("%s %d\n", __FUNCTION__, __LINE__);
    init_info.gpioNum_sclk    = sgPin_SCLK;
    init_info.gpioNum_mosi    = sgPin_MOSI;
    init_info.gpioNum_miso    = sgPin_MISO;
    init_info.gpioNum_ss      = sgPin_SS;
    /*
    init_info.gpioNum_reset   = sgPin_RES;
    init_info.gpioNum_int     = sgPin_INT;
    */
    spi_dev6SpiPin_init(unit, &init_info);
    osal_printf("%s %d\n", __FUNCTION__, __LINE__);

    // reset chip
#if 0
    DRV_GPIO_SET(unit, sg_dev, sgPin_RES,0);
    osal_time_mdelay(1);
    DRV_GPIO_SET(unit, sg_dev, sgPin_RES,1);


    //set max3421e as full duplex mode
    {
      uint8 tmp=0xFF;
      spi_dev6Reg_write(unit,17,&tmp);
    }

#endif

    osal_printf("REG32(0xbb003308):%x\n", REG32(0xbb003308));

    spi_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}


/* Function Name:
 *      spi_dev6Reg_write
 * Description:
 *      Write data to device registers through SPI interface.
 * Input:
 *      unit                - unit id
 *      regAddr             - address of register
 *      pBuff               - data to write
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      8380, 8390
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
spi_dev6Reg_write(uint32 unit, uint32 regAddr, uint32 *buff)
{
    spi_cmd_t cmd;

    /* parameter check */
    RT_PARAM_CHK((NULL == buff), RT_ERR_NULL_POINTER);

    osal_memset(&cmd, 0, sizeof(cmd));

    cmd.address = regAddr;
    cmd.buf = (uint32 *)buff;
    cmd.size = 1;
    cmd.cmd_t = SPI_DEV6_CMD_WRITE;

    _spi_dev6cmd_9300Gpio(unit, &cmd);

    //osal_printf("%s %d reg %x set %x\n", __FUNCTION__, __LINE__, regAddr, *((uint32 *)buff));

    return RT_ERR_OK;
}


/* Function Name:
 *      spi_dev6Reg_read
 * Description:
 *      Read data from device through SPI interface.
 * Input:
 *      unit                - unit id
 *      regAddr             - address of register
 *      pBuff               - to store data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      8380, 8390
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
spi_dev6Reg_read(uint32 unit, uint32 regAddr, uint32 *buff)
{
    spi_cmd_t cmd;

    /* parameter check */
    RT_PARAM_CHK((NULL == buff), RT_ERR_NULL_POINTER);
    osal_memset(&cmd, 0, sizeof(cmd));
    *buff = 0;

    cmd.address = regAddr;
    cmd.buf = buff;
    cmd.size = 1;
    cmd.cmd_t = SPI_DEV6_CMD_READ;

    _spi_dev6cmd_9300Gpio(unit, &cmd);

    //osal_printf("%s %d reg %x result %x\n", __FUNCTION__, __LINE__, regAddr, *((uint32 *)buff));

    return RT_ERR_OK;
}

/* Function Name:
 *      spi_dev6_init
 * Description:
 *      Inital SPI interface which used to access registers of device 2 (refer to above file description).
 * Input:
 *      unit                - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      8380, 8390
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32 spi_dev6_init(uint32 unit)
{
    int32 ret=RT_ERR_FAILED;

    RT_INIT_REENTRY_CHK(spi_init[unit]);

    ret = _spi_dev6_9300Gpio_init(unit);

    if (RT_ERR_OK == ret)
        spi_init[unit] = INIT_COMPLETED;

    return ret;
}

