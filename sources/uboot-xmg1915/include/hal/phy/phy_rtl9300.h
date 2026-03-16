/*
 * Copyright (C) 2016-2022 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 75845 $
 * $Date: 2017-02-16 17:03:50 +0800 (Thu, 16 Feb 2017) $
 *
 * Purpose : MAC 9300 internal PHY Driver APIs.
 *
 * Feature : MAC 9300 internal PHY Driver APIs
 *
 */

#ifndef __HAL_PHY_PHY_RTL9300_H__
#define __HAL_PHY_PHY_RTL9300_H__

/*
 * Include Files
 */
#include <common/rt_type.h>


#define PORT_NUM_IN_9300SDS                     1

typedef struct sds_reg_field_s
{
    uint32 reg;
    uint32 field;
}sds_reg_field_t;

extern rt_phyInfo_t phy_9300sds_info;

extern int32
phy_9300_10gMedia_set(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t media);

/* Function Name:
 *      phy_9300_serdes_mapperInit
 * Description:
 *      Initialize PHY 9300 serdes driver.
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
phy_9300_serdes_mapperInit(rt_phydrv_t *pPhydrv);

#endif /* __HAL_PHY_PHY_RTL9300_H__ */




