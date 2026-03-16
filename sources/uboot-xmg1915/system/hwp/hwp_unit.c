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
 * $Revision: 75682 $
 * $Date: 2017-02-08 19:48:28 +0800 (Wed, 08 Feb 2017) $
 *
 * Purpose : Define hardware profile structure, API, and macros.
 *           Hardware profile describes the hardware board design, includes what switch ASIC,
 *           PHY chip, BSP, ...etc have been designed.
 *
 * Feature :
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <hwp/hw_profile.h>
#include <osal/sem.h>



/*
 * Symbol Definition
 */
extern osal_mutex_t hwp_unit_sem;

#define HWP_SEM_LOCK()    \
    do {\
        if (osal_sem_mutex_take(hwp_unit_sem, OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_INIT), "semaphore lock failed");\
            return RT_ERR_SEM_LOCK_FAILED;\
        }\
    } while(0)

#define HWP_SEM_UNLOCK()   \
    do {\
        if (osal_sem_mutex_give(hwp_unit_sem) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_INIT), "semaphore unlock failed");\
            return RT_ERR_SEM_UNLOCK_FAILED;\
        }\
    } while(0)


/*
 * Data Type Definition
 */


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
hwp_unit_add(uint32 unit, hwp_hwProfile_t *hwp, uint32 local_unit_id)
{
    int i;

    if ((NULL==hwp)||(NULL == hwp->parsed_info)||(hwp == &hwpEmpty))
        return RT_ERR_FAILED;

    for(i=0;i<RTK_MAX_NUM_OF_UNIT_LOCAL;i++)
    {
        if (unit == hwp_myLocalChipUnitID[i])
            return RT_ERR_FAILED;
    }

    HWP_SEM_LOCK();

    UNITMAP(unit) = &((hwp_parsedInfo_t *)hwp->parsed_info)->unitInfo[local_unit_id];

    HWP_SEM_UNLOCK();
    return RT_ERR_OK;

}


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
hwp_unit_del(uint32 unit)
{
    int i;

    for(i=0; i<RTK_MAX_NUM_OF_UNIT_LOCAL; i++)
    {
        if (unit == hwp_myLocalChipUnitID[i])
        {
            return RT_ERR_FAILED;
        }
    }

    HWP_SEM_LOCK();

    UNITMAP(unit) = &parsedInfoEmpty.unitInfo[HWP_LOCALUNIT_EMPTY];

    HWP_SEM_UNLOCK();
    return RT_ERR_OK;

}


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
hwp_unitID_change(uint32 fromID, uint32 toID)
{
    uint32 local_unit_id;

    if (UNITMAP(toID) != &unitInfoEmpty)
        return RT_ERR_FAILED;

    HWP_SEM_LOCK();

    if (RT_ERR_OK == _hwp_localUnitId_get(&local_unit_id,UNITMAP(fromID)))
    {
        hwp_myLocalChipUnitID[local_unit_id] = toID;
    }

    UNITMAP(toID) = UNITMAP(fromID);
    UNITMAP(fromID) = &unitInfoEmpty;

    HWP_SEM_UNLOCK();

    return RT_ERR_OK;

}

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
hwp_unitID_exchange(uint32 unitX, uint32 unitY)
{
    uint32 i;
    hwp_unitInfo_t *unitMapTemp;

    HWP_SEM_LOCK();

    unitMapStruct[unitX].flag &= ~(HWP_UNITMAPFLAG_LOCALCHIP);
    unitMapStruct[unitY].flag &= ~(HWP_UNITMAPFLAG_LOCALCHIP);

    for(i=0; i<RTK_MAX_NUM_OF_UNIT_LOCAL; i++)
    {
        if (hwp_myLocalChipUnitID[i] == unitX)
        {
            hwp_myLocalChipUnitID[i] = unitY;
            unitMapStruct[unitY].flag |= HWP_UNITMAPFLAG_LOCALCHIP;
        }
        else if (hwp_myLocalChipUnitID[i] == unitY)
        {
            hwp_myLocalChipUnitID[i] = unitX;
            unitMapStruct[unitX].flag |= HWP_UNITMAPFLAG_LOCALCHIP;
        }
    }

    unitMapTemp = UNITMAP(unitX);
    UNITMAP(unitX) = UNITMAP(unitY);
    UNITMAP(unitY) = unitMapTemp;

    HWP_SEM_UNLOCK();

    return RT_ERR_OK;

}




