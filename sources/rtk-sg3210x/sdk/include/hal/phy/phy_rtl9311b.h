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
 * $Revision: 100544 $
 * $Date: 2019-09-27 15:06:01 +0800 (周五, 27 9月 2019) $
 *
 * Purpose : MAC 9311b internal PHY Driver APIs.
 *
 * Feature : MAC 9311b internal PHY Driver APIs
 *
 */

#ifndef __HAL_PHY_PHY_RTL9311B_H__
#define __HAL_PHY_PHY_RTL9311B_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <rtk/sds.h>

/*
 * Symbol Definition
 */
#define PORT_NUM_IN_9311bSDS     (1)

/*
 * Data Declaration
 */
extern rt_phyInfo_t phy_rtl9311bsds_info;

/*
 * Function Declaration
 */

/* Function Name:
 *      phy_rtl9311b_leq_get
 * Description:
 *      Get SerDes LEQ
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 * Output:
 *      pLeq - LEQ configuration
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      No semaphore
 */
extern int32
phy_rtl9311b_leq_get(uint32 unit, uint32 sds, rtk_sds_leq_t *pLeq);

/* Function Name:
 *      phy_rtl9311b_leq_set
 * Description:
 *      Set SerDes LEQ
 * Input:
 *      unit - unit id
 *      sds  - user SerDes id
 *      pLeq  - LEQ configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 * Note:
 *      No semaphore
 */
extern int32
phy_rtl9311b_leq_set(uint32 unit, uint32 sds, rtk_sds_leq_t *pLeq);


/* Function Name:
 *      phy_rtl9311b_serdes_ge_mapperInit
 * Description:
 *      Initialize PHY 9311b serdes.
 * Input:
 *      pPhydrv - pointer of phy driver
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void phy_rtl9311b_serdes_mapperInit(rt_phydrv_t *pPhydrv);

/* Function Name:
 *      phy_rtl9311b_10gmedia_get
 * Description:
 *      Get 9311b 10 serdes PHY media type.
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pMedia - pointer buffer of phy media type
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - invalid parameter
 *      RT_ERR_NULL_POINTER - input parameter is null pointer
 * Note:
 *      None
 */
extern int32
phy_rtl9311b_serdesMedia_get(uint32 unit, rtk_port_t port, rtk_port_serdesMedia_t *pMedia);

/* Function Name:
 *      phy_rtl9311b_serdesMedia_set
 * Description:
 *
 * Input:
 *      unit - unit id
 *      port - port id
 *      media
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_INPUT
 *      RT_ERR_CHIP_NOT_SUPPORTED
 *      RT_ERR_TIMEOUT
 *      RT_ERR_FAILED
 * Note:
 *      None
 */

extern int32
phy_rtl9311b_serdesMedia_set(uint32 unit, rtk_port_t port, rtk_port_serdesMedia_t media);

/* Function Name:
 *      phy_rtl9311b_sdsCustConfig_init
 * Description:
 *      Serdes Customer Configuration
 * Input:
 *      unit - unit id
 *      sds  - SerDes id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
phy_rtl9311b_sdsCustConfig_init(uint32 unit, uint32 sds, rtk_port_serdesMedia_t media);


/* Function Name:
 *      phy_rtl9311b_sds_loopback_get
 * Description:
 *      Get serdes Loopback status of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      pEnable           -loopback mode status;
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
phy_rtl9311b_sds_loopback_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable);

/* Function Name:
 *      phy_rtl9311b_sds_loopback_set
 * Description:
 *      Get serdes Loopback status of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      enable           -loopback mode status;
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
phy_rtl9311b_sds_loopback_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

/* Function Name:
 *      phy_rtl9311b_remoteFault_handle
 * Description:
 *      Handle fiber remote fault
 * Input:
 *      unit      - unit id
 *      port      - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 * Note:
 *      None
 */
extern int32
phy_rtl9311b_remoteFault_handle(uint32 unit, rtk_port_t port);

#endif /* __HAL_PHY_PHY_RTL9311B_H__ */
