/*
 * Copyright (C) 2009-2015 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 76849 $
 * $Date: 2017-03-23 15:18:19 +0800 (Thu, 23 Mar 2017) $
 *
 * Purpose : Define board profile structure, API, and macros.
 *
 * Feature :
 *
 */

#ifndef __HWP_UNIT_H__
#define __HWP_UNIT_H__

#include <hwp/hw_profile.h>
/*
 * Function Declaration
 */

/* Function Name:
 *      hwp_unit_add
 * Description:
 *      Add a unit into hardware profile database if the unit has not been added (empty).
 * Input:
 *      unit        - unit ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
hwp_unit_add(uint32 unit, hwp_hwProfile_t *hwp, uint32 local_unit_id);


/* Function Name:
 *      hwp_unit_del
 * Description:
 *      Remove a unit from hardware profile database. Local unit could not be deleted,
 *      except changing (by hwp_unitID_change()) it to another one.
 * Input:
 *      unit        - unit to delete
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
hwp_unit_del(uint32 unit);


/* Function Name:
 *      hwp_unitID_change
 * Description:
 *      Change a unit ID from fromID to toID, and delete fromID.
 * Input:
 *      fromID        - original unit ID
 *      toID          - new unit ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
hwp_unitID_change(uint32 fromID, uint32 toID);

/* Function Name:
 *      hwp_unitID_exchange
 * Description:
 *      Exchange two units.
 * Input:
 *      unitX, unitY    - will exchange these two units
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
hwp_unitID_exchange(uint32 unitX, uint32 unitY);



#endif /* __HWP_UNIT_H__ */
