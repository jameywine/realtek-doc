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
 * Purpose : Definition of Init API
 *
 * Feature : Initialize All Layers of RTK Module
 *
 */

#ifndef __RTK_INIT_H__
#define __RTK_INIT_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <osal/sem.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

#define RTK_INIT_SEM_LOCK()\
    do {\
        if (osal_sem_mutex_take(rtk_init_sem, OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_FAILED, (MOD_HAL), "rtk_init_sem semaphore lock failed");\
            return RT_ERR_FAILED;\
        }\
    } while(0)

#define RTK_INIT_SEM_UNLOCK()\
    do {\
        if (osal_sem_mutex_give(rtk_init_sem) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_FAILED, (MOD_HAL), "rtk_init_sem semaphore unlock failed");\
            return RT_ERR_FAILED;\
        }\
    } while(0)


/*
 * Function Declaration
 */

/* Function Name:
 *      rtk_init
 * Description:
 *      Initialize the specified device
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8380, 8390, 9300, 9310, 9311B
 * Note:
 *      INIT must be initialized before using all of APIs in each modules
 * Changes:
 *      None
 */
extern int32
rtk_init(void);

/* Function Name:
 *      rtk_exit
 * Description:
 *      Deinitialize the specified device
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8380, 8390, 9300, 9310, 9311B
 * Note:
 *      None
 * Changes:
 *      None
 */
extern int32
rtk_exit(void);

/* Function Name:
 *      rtk_chip_attach
 * Description:
 *      Attach chip
 * Input:
 *      chipType        - attaching chip type
 *      pIdentifier     - pointer of attach identifier which identifies where to attach
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310, 9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 */
extern int32
rtk_chip_attach(rtk_chip_t chipType, rtk_attachIdentifier_t *pIdentifier);

/* Function Name:
 *      rtk_chip_detach
 * Description:
 *      Detach chip
 * Input:
 *      chipType        - attaching chip type
 *      pIdentifier     - pointer of attach identifier which identifies where to detach
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      8380, 8390, 9300, 9310, 9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 */
extern int32
rtk_chip_detach(rtk_chip_t chipType, rtk_attachIdentifier_t *pIdentifier);


#endif /* __RTK_INIT_H__ */
