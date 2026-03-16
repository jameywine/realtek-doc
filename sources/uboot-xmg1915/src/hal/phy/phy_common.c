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
 * $Revision: 80690 $
 * $Date: 2017-07-20 11:40:05 +0800 (Thu, 20 Jul 2017) $
 *
 * Purpose : PHY Common Driver APIs.
 *
 * Feature : PHY Common Driver APIs
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <osal/time.h>
#include <osal/print.h>
#include <hal/common/miim.h>
#include <hal/phy/phydef.h>
#include <hal/phy/phy_common.h>
#include <hal/mac/miim_common_drv.h>


/*
 * Symbol Definition
 */


/*
 * Function Declaration
 */

/* Function Name:
 *      phy_common_unavail
 * Description:
 *      Return chip not support
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_PORT_NOT_SUPPORTED   - functions not supported by this port
 * Note:
 *      None
 */
int32 phy_common_unavail(void)
{
    return RT_ERR_PORT_NOT_SUPPORTED;
} /* end of phy_common_unavail */

/* Function Name:
 *      phy_common_autoNegoEnable_get
 * Description:
 *      Get autonegotiation enable status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_common_autoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32   ret;
    uint32  phyData0;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    if (phyData0 & AutoNegotiationEnable_MASK)
    {
        *pEnable = ENABLED;
    }
    else
    {
        *pEnable = DISABLED;
    }

    return ret;
} /* end of phy_common_autoNegoEnable_get */

/* Function Name:
 *      phy_common_autoNegoEnable_set
 * Description:
 *      Set autonegotiation enable status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_common_autoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32   ret;
    uint32  phyData0;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    phyData0 = phyData0 & ~(AutoNegotiationEnable_MASK | RestartAutoNegotiation_MASK);
    phyData0 = phyData0 | ((enable << AutoNegotiationEnable_OFFSET) | (1 << RestartAutoNegotiation_OFFSET));

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData0)) != RT_ERR_OK)
        return ret;

    return ret;
} /* end of phy_common_autoNegoEnable_set */


/* Function Name:
 *      phy_common_duplex_get
 * Description:
 *      Get duplex mode status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pDuplex - pointer to PHY duplex mode status
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_common_duplex_get(uint32 unit, rtk_port_t port, uint32 *pDuplex)
{
    int32   ret;
    uint32  phyData0;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    *pDuplex = (phyData0 & DuplexMode_MASK) >> DuplexMode_OFFSET;

    return ret;
} /* end of phy_common_duplex_get */

/* Function Name:
 *      phy_common_duplex_get
 * Description:
 *      Set duplex mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      duplex        - duplex mode of the port, full or half
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_common_duplex_set(uint32 unit, rtk_port_t port, uint32 duplex)
{
    int32   ret;
    uint32  phyData0;

    /* get value from CHIP*/
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    phyData0 = phyData0 & ~(DuplexMode_MASK);
    phyData0 = phyData0 | (duplex << DuplexMode_OFFSET);

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData0)) != RT_ERR_OK)
        return ret;

    return ret;
} /* end of phy_common_duplex_set */

/* Function Name:
 *      phy_common_enable_set
 * Description:
 *      Set PHY interface status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      enable        - admin configuration of PHY interface
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_common_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32   ret;
    uint32  phyData;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
        return ret;

    if (ENABLED == enable)
    {
        phyData &= ~(PowerDown_MASK);
    }
    else
    {
        phyData &= ~(PowerDown_MASK);
        phyData |= (1 << PowerDown_OFFSET);
    }

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_common_enable_set */

/* Function Name:
 *      phy_common_enable_get
 * Description:
 *      Get PHY interface status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      enable        - admin configuration of PHY interface
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_common_enable_get(uint32 unit, rtk_port_t port, rtk_enable_t *enable)
{
    int32   ret;
    uint32  phyData;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
        return ret;

    if ((phyData & PowerDown_MASK) >> PowerDown_OFFSET)
    {
       *enable = DISABLED;
    }
    else
    {
       *enable = ENABLED;
    }

    return RT_ERR_OK;
} /* end of phy_common_enable_set */

int32 phy_common_speed_get(uint32 unit, rtk_port_t port, uint32 *pSpeed)
{
    int32   ret = RT_ERR_FAILED;
    uint32  phyData;
    uint32  A, B, C, D, E, F, G, H, I, i;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 1, &phyData)) != RT_ERR_OK)
        return ret;
    A = (phyData >> 8) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 6, &phyData)) != RT_ERR_OK)
        return ret;
    B = (phyData >> 0) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 0, &phyData)) != RT_ERR_OK)
        return ret;
    C = (phyData >> 12) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 0, &phyData)) != RT_ERR_OK)
        return ret;
    D = (phyData >> 6) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 0, &phyData)) != RT_ERR_OK)
        return ret;
    E = (phyData >> 13) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 5, &phyData)) != RT_ERR_OK)
        return ret;
    F = (phyData >> 7) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 5, &phyData)) != RT_ERR_OK)
        return ret;
    G = (phyData >> 5) & 1;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 9, &phyData)) != RT_ERR_OK)
        return ret;
    H = (phyData >> 9) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 10, &phyData)) != RT_ERR_OK)
        return ret;
    H &= (phyData >> 11) & 1;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 4, &phyData)) != RT_ERR_OK)
        return ret;
    I = (phyData >> 8) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 5, &phyData)) != RT_ERR_OK)
        return ret;
    I &= (phyData >> 8) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 4, &phyData)) != RT_ERR_OK)
        return ret;
    i = (phyData >> 7) & 1;
    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, 5, &phyData)) != RT_ERR_OK)
        return ret;
    i &= (phyData >> 7) & 1;
    I = I | i;

    if ((A & B & C & H) | (A & ~C & D & ~E))
        *pSpeed = PORT_SPEED_1000M; /* 1000M */
    else if ((B & ~C & ~D & E)|(B & C & ~H & I)|(~B & ~C & ~D & E)|(~B & C & F & ~G))
        *pSpeed = PORT_SPEED_100M;  /* 100M */
    else
        *pSpeed = PORT_SPEED_10M;   /* 10M */

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_common_reg_get
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      phy_reg - PHY register
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_get(uint32 unit,  rtk_port_t port, uint32 page, uint32 phy_reg, uint32 *pData)
{
    return hal_miim_read(unit, port, page, phy_reg, pData);
}

/* Function Name:
 *      phy_common_reg_set
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_set(uint32 unit, rtk_port_t port, uint32 page, uint32 phy_reg, uint32 data)
{
    return hal_miim_write(unit, port, page, phy_reg, data);
}

/* Function Name:
 *      phy_common_reg_park_get
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      parkPage    - PHY park page
 *      phy_reg - PHY register
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_park_get(uint32 unit, rtk_port_t port, uint32 page, uint32 parkPage, uint32 phy_reg, uint32 *pData)
{
    return hal_miim_park_read(unit, port, page, parkPage, phy_reg, pData);
}


/* Function Name:
 *      phy_common_reg_park_set
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      parkPage    - PHY park page
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_park_set(
    uint32      unit,
    rtk_port_t  port,
    uint32      page,
    uint32      parkPage,
    uint32      phy_reg,
    uint32      data)
{
    return hal_miim_park_write(unit, port, page, parkPage, phy_reg, data);
}


/* Function Name:
 *      phy_common_reg_mmd_get
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mmdAddr - mmd device address
 *      mmdReg  - mmd reg id
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_mmd_get(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 *pData)
{
    return hal_miim_mmd_read(unit, port, mmdAddr, mmdReg, pData);
}

/* Function Name:
 *      phy_common_reg_mmd_set
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_mmd_set(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 data)
{
    return hal_miim_mmd_write(unit, port, mmdAddr, mmdReg, data);
}

/* Function Name:
 *      phy_common_reg_mmd_portmask_set
 * Description:
 *      Set PHY registers in those portmask.
 * Input:
 *      unit     - unit id
 *      portmask - portmask
 *      mmdAddr  - mmd device address
 *      mmdReg   - mmd reg id
 *      data     - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. portmask valid range is bit 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_mmd_portmask_set(
    uint32          unit,
    rtk_portmask_t  portmask,
    uint32          mmdAddr,
    uint32          mmdReg,
    uint32          data)
{
    return hal_miim_mmd_portmask_write(unit, portmask, mmdAddr, mmdReg, data);
}


/* Function Name:
 *      phy_common_reg_extParkPage_get
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg - PHY register
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_extParkPage_get(
    uint32      unit,
    rtk_port_t  port,
    uint32      mainPage,
    uint32      extPage,
    uint32      parkPage,
    uint32      phy_reg,
    uint32      *pData)
{
    return hal_miim_extParkPage_read(unit, port, mainPage, extPage, parkPage, phy_reg, pData);
}

/* Function Name:
 *      phy_common_reg_extParkPage_set
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_extParkPage_set(
    uint32      unit,
    rtk_port_t  port,
    uint32      mainPage,
    uint32      extPage,
    uint32      parkPage,
    uint32      phy_reg,
    uint32      data)
{
    return hal_miim_extParkPage_write(unit, port, mainPage, extPage, parkPage, phy_reg, data);
}

/* Function Name:
 *      phy_common_reg_extParkPage_portmask_set
 * Description:
 *      Set PHY registers in those portmask.
 * Input:
 *      unit     - unit id
 *      portmask - portmask
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg  - PHY register
 *      data     - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. portmask valid range is bit 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
phy_common_reg_extParkPage_portmask_set(
    uint32          unit,
    rtk_portmask_t  portmask,
    uint32          mainPage,
    uint32          extPage,
    uint32          parkPage,
    uint32          phy_reg,
    uint32          data)
{
    return hal_miim_extParkPage_portmask_write(unit, portmask, mainPage, extPage, parkPage, phy_reg, data);
}

/* Function Name:
 *      phy_common_masterSlave_get
 * Description:
 *      Get PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      pMasterSlaveCfg     - pointer to the PHY master slave configuration
 *      pMasterSlaveActual  - pointer to the PHY master slave actual link status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID       - invalid port id
 *      RT_ERR_NULL_POINTER  - input parameter may be null pointer
 * Note:
 *      This function only works on giga/ 10g port to get its master/slave mode configuration.
 */
int32
phy_common_masterSlave_get(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   *pMasterSlaveCfg,
    rtk_port_masterSlave_t   *pMasterSlaveActual)
{
    int32 ret;
    uint32 data;

    if ((ret = hal_miim_read(unit, port, 0, 9, &data)) != RT_ERR_OK)
        return ret;

    data = (data >> 11) & 0x3;
    switch(data)
    {
        case 0x2:
            *pMasterSlaveCfg = PORT_SLAVE_MODE;
            break;
        case 0x3:
            *pMasterSlaveCfg = PORT_MASTER_MODE;
            break;
        default:
            *pMasterSlaveCfg = PORT_AUTO_MODE;
            break;
    }

    if ((ret = hal_miim_read(unit, port, 0, 10, &data)) != RT_ERR_OK)
        return ret;
    data = (data >> 14) & 0x1;
    switch(data)
    {
        case 0x0:
            *pMasterSlaveActual = PORT_SLAVE_MODE;
            break;
        case 0x1:
            *pMasterSlaveActual = PORT_MASTER_MODE;
            break;
        default:
            *pMasterSlaveActual = PORT_SLAVE_MODE;
            break;
     }

    return ret;
}/* end of phy_common_masterSlave_get */

/* Function Name:
 *      phy_common_masterSlave_set
 * Description:
 *      Set PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      masterSlave         - PHY master slave configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID       - invalid port id
 *      RT_ERR_INPUT         - invalid input parameter
 * Note:
 *      None
 */
int32
phy_common_masterSlave_set(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   masterSlave)
{
    int32 ret;
    uint32 data;

    if ((ret = hal_miim_read(unit, port, 0, 9, &data)) != RT_ERR_OK)
        return ret;

    data &= ~ (0x3 << 11);
    switch(masterSlave)
    {
        case PORT_AUTO_MODE:
            data |= 0x0 << 11;
            break;
        case PORT_SLAVE_MODE:
            data |= 0x2 << 11;
            break;
        case PORT_MASTER_MODE:
            data |= 0x3 << 11;
            break;
        default:
            return RT_ERR_INPUT;
    }

    if ((ret = hal_miim_write(unit, port, 0, 9, data)) != RT_ERR_OK)
        return ret;

    return ret;
}/* end of phy_common_masterSlave_set */



/* Function Name:
 *      phy_common_masterSlaveMmd_get
 * Description:
 *      Get PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      pMasterSlaveCfg     - pointer to the PHY master slave configuration
 *      pMasterSlaveActual  - pointer to the PHY master slave actual link status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID       - invalid port id
 *      RT_ERR_NULL_POINTER  - input parameter may be null pointer
 * Note:
 *      This function only works on giga/ 10g port to get its master/slave mode configuration.
 */
int32
phy_common_masterSlaveMmd_get(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   *pMasterSlaveCfg,
    rtk_port_masterSlave_t   *pMasterSlaveActual)
{
    int32 ret;
    uint32 data;

    if ((ret = hal_miim_read(unit, port, 0, 9, &data)) != RT_ERR_OK)
        return ret;

    data = (data >> 11) & 0x3;
    switch(data)
    {
        case 0x2:
            *pMasterSlaveCfg = PORT_SLAVE_MODE;
            break;
        case 0x3:
            *pMasterSlaveCfg = PORT_MASTER_MODE;
            break;
        default:
            *pMasterSlaveCfg = PORT_AUTO_MODE;
            break;
    }

    if ((ret = hal_miim_read(unit, port, 0, 10, &data)) != RT_ERR_OK)
        return ret;
    data = (data >> 14) & 0x1;
    switch(data)
    {
        case 0x0:
            *pMasterSlaveActual = PORT_SLAVE_MODE;
            break;
        case 0x1:
            *pMasterSlaveActual = PORT_MASTER_MODE;
            break;
        default:
            *pMasterSlaveActual = PORT_SLAVE_MODE;
            break;
     }

    return ret;
}/* end of phy_common_masterSlaveMmd_get */

/* Function Name:
 *      phy_common_masterSlaveMmd_set
 * Description:
 *      Set PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      masterSlave         - PHY master slave configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID       - invalid port id
 *      RT_ERR_INPUT         - invalid input parameter
 * Note:
 *      None
 */
int32
phy_common_masterSlaveMmd_set(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   masterSlave)
{
    int32 ret;
    uint32 data;

    if ((ret = hal_miim_mmd_read(unit, port, 7, 32, &data)) != RT_ERR_OK)
        return ret;

    data &= ~ (0x3 << 14);
    switch(masterSlave)
    {
        case PORT_AUTO_MODE:
            data |= 0x0 << 14;
            break;
        case PORT_SLAVE_MODE:
            data |= 0x2 << 14;
            break;
        case PORT_MASTER_MODE:
            data |= 0x3 << 14;
            break;
        default:
            return RT_ERR_INPUT;
    }

    if ((ret = hal_miim_mmd_write(unit, port, 7, 32, data)) != RT_ERR_OK)
        return ret;

    return ret;
}/* end of phy_common_masterSlaveMmd_set */


/* Function Name:
 *      phy_common_loopback_get
 * Description:
 *      Get PHY Loopback mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      pEnable           -loopback mode status;
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_common_loopback_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32       ret;
    uint32      data;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &data)) != RT_ERR_OK)
    {
        return ret;
    }

    if(Loopback_MASK & data)
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return ret;
}

/* Function Name:
 *      phy_common_loopback_set
 * Description:
 *      Set PHY Loopback mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      enable              - ENABLED: Enable loopback;
 *                            DISABLED: Disable loopback. PHY back to normal operation.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_common_loopback_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32       ret;
    uint32      data;

    if ((ret = hal_miim_read(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &data)) != RT_ERR_OK)
    {
         return ret;
    }

    if (enable == ENABLED)
        data |= Loopback_MASK;
    else
        data &= ~Loopback_MASK;

    if ((ret = hal_miim_write(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, data)) != RT_ERR_OK)
    {
        return ret;
    }

    return RT_ERR_OK;
}

