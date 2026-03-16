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
 * $Revision: 80801 $
 * $Date: 2017-07-25 14:46:43 +0800 (Tue, 25 Jul 2017) $
 *
 * Purpose : Definition of SPI API
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) SPI driver

 */

#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

/*
 * Include Files
 */
#if defined(CONFIG_SDK_SPI_DEV_1)
  #include <private/drv/spi/spi_dev1.h>
#elif defined(CONFIG_SDK_SPI_DEV_2)
  #include <private/drv/spi/spi_private.h>
  #include <private/drv/spi/spi_dev2.h>
#elif defined(CONFIG_SDK_SPI_DEV_3)
  #include <private/drv/spi/spi_dev3.h>
#elif defined(CONFIG_SDK_SPI_DEV_4)
  #include <private/drv/spi/spi_dev4.h>
#elif defined(CONFIG_SDK_SPI_DEV_NONE)

#endif


/*
 * Symbol Definition
 */
#define RT_SPI_USE_FLASH_SEMA   1       /* 1: Using same semaphore with FLASH driver; 0: no */


/*
 * Data Declaration
 */
typedef enum spi_cs_e{
    SPI_CS_PIN0 = 0,    /* chip select 0 */
    SPI_CS_PIN1,        /* chip select 0 */
    SPI_CS_PIN2,        /* chip select 0 */
    SPI_CS_PIN3,        /* chip select 0 */
    SPI_CS_TEST,        /* for debug purpose */
}spi_cs_t;


/*
 * Function Declaration
 */


/* Function Name:
 *      drv_spi_init
 * Description:
 *      Initialize spi module of the specified device.
 * Input:
 *      unit                - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      Must initialize spi module before calling any spi APIs.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_spi_init(uint32 unit);


/* Function Name:
 *      drv_spi_write
 * Description:
 *      Transmit data via SPI of the specified device.
 * Input:
 *      unit                - unit id
 *      mAddrs              - SPI address
 *      pBuff               - transfer data
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
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_spi_write(uint32 unit, uint32 mAddrs, uint32 *pBuff);


/* Function Name:
 *      drv_spi_read
 * Description:
 *      Start the Receice data from SPI.
 * Input:
 *      unit                - unit id
 *      mAddrs              - SPI address
 * Output:
 *      pBuff               - received data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_spi_read(uint32 unit, uint32 mAddrs, uint32 *pBuff);


#endif /* __DRV_SPI_H__ */

