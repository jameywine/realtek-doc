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
 * Purpose : All header files of customer API will be located in this file.
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) Extern customer's API to other RTK processes.
 *
 */

#ifndef __RTK_CUSTOMER_API_H__
#define __RTK_CUSTOMER_API_H__

/*
 * Include Files
 *      Any customer added header file which will be called by other RTK
 *      processes that not include by customer API, please add the header
 *      files in this customer_api.h.
 */
#include <rtk/customer/customer_api_demo.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Function Declaration
 */

/* Module Name : customer */

/* Function Name:
 *      rtk_customer_api_init
 * Description:
 *      This API is hooked in RTK initial flow already, and customer can fill the
 *      initial process in this API.
 * Input:
 *      unit            - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID               - invalid unit id
 *      RT_ERR_OUT_OF_RANGE     - dumped table index is out of range
 * Applicable:
 *
 * Note:
 * 	  This API is exported to other kernel module, then other modules can
 *      initial the customer API part, too.
 * Changes:
 *      None
 */

extern int32
rtk_customer_api_init(void);

/* Function Name:
 *      senao_debug_gpy_phy_usxgmii_port_get
 * Description:
 *      Get Maxlinear GPY PHY's MAC side USXGMII serdes link status
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 *      RT_ERR_PORT_NOT_SUPPORTED   - This function is not supported by the PHY of this port
 * Applicable:
 *      External PHY - Maxlinear GPY2xx
 * Note:
 *      None
 * Changes:
 *      [SDK_3.6.6]
 *          New added function.
 */
extern int32
senao_debug_gpy_phy_usxgmii_port_get(uint32 unit, rtk_port_t port);

extern int32
senao_gpy_phy_vr_reset(uint32 unit, rtk_port_t port);

extern int32
senao_gpy_phy_aneg_reset(uint32 unit, rtk_port_t port);

extern int32
senao_gpy_phy_eye_diagram_get(uint32 unit, rtk_port_t port, uint32 *pBuff);

extern int32
senao_gpy_phy_eye_diagram_set(uint32 unit, rtk_port_t port, uint32 *pBuff);

extern int32
senao_gpy_phy_usxgmii_loopback_get(uint32 unit, rtk_port_t port, uint32 *pmode);

extern int32
senao_gpy_phy_usxgmii_loopback_set(uint32 unit, rtk_port_t port, uint32 mode);

extern int32
senao_gpy_phy_usxgmii_alignmentmarker_get(uint32 unit, rtk_port_t port, uint32 *pam);

extern int32
senao_gpy_phy_usxgmii_alignmentmarker_set(uint32 unit, rtk_port_t port, uint32 am);


#endif /* __RTK_CUSTOMER_API_H__ */
