/*
 * Copyright (C) 2019 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 75479 $
 * $Date: 2017-01-20 15:17:16 +0800 (Fri, 20 Jan 2017) $
 *
 * Purpose : Customer PHY Driver sample codes
 *
 * Feature : Customer PHY Driver sample codes
 *
 */

#ifndef __HAL_PHY_PHY_CUST2_H__
#define __HAL_PHY_PHY_CUST2_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <rtk/port.h>


/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
extern rt_phyInfo_t phy_cust2_info;



/*
 * Macro Declaration
 */


/*
 * Function Declaration
 */

/* Function Name:
 *      phy_identify_cust2
 * Description:
 *      Identify the port is CUST2 PHY or not
 *      This function will be called in "phy probe" stage for checking if the port's PHY is CUST2 PHY
 * Input:
 *      unit     - unit id
 *      port     - port id
 *      model_id - model id (not used)
 *      rev_id   - revision id (not used)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - port is CUST2 PHY, the port will then hook with this PHY driver
 *      RT_ERR_FAILED           - access failure or others
 *      RT_ERR_PHY_NOT_MATCH    - port is not CUST2 PHY
 * Note:
 *      None
 */
extern int32
phy_identify_cust2(uint32 unit, rtk_port_t port, uint32 model_id, uint32 rev_id);

/* Function Name:
 *      phy_cust2drv_mapperInit
 * Description:
 *      Initialize PHY driver.
 * Input:
 *      pPhydrv - pointer of phy driver
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
extern void
phy_cust2drv_mapperInit(rt_phydrv_t *pPhydrv);


/* Function Name:
 *      phy_cust2_swMacPollPhyStatus_get
 * Description:
 *      Get PHY status
 * Input:
 *      unit    - unit ID
 *      port    - port ID
 * Output:
 *      pphyStatus  - PHY status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
phy_cust2_swMacPollPhyStatus_get(uint32 unit, uint32 port, rtk_port_swMacPollPhyStatus_t *pphyStatus);

/* Function Name:
 *      phy_cust2_macIntfSerdesMode_get
 * Description:
 *      Get serdes mode
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pserdesMode - serdes mode
 * Return:
 *      None
 * Note:
 *      None
 */
/*extern int32
phy_cust2_macIntfSerdesMode_get(uint32 unit, rtk_port_t port, rt_serdesMode_t *pserdesMode);*/


/* Function Name:
 *      phy_cust2_imageFlash_load
 * Description:
 *      load Aquantia PHY image to flash
 * Input:
 *      unit - unit id
 *      port - port id
 *      size - image size
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
/*extern int32
phy_cust2_imageFlash_load(uint32 unit, rtk_port_t port, uint32 size, uint8 *image);*/


#endif /* __HAL_PHY_PHY_CUST2_H__ */












