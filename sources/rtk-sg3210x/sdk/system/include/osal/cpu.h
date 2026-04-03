/*
 * Copyright (C) 2009-2021 Realtek Semiconductor Corp.
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
 * Purpose : Definition those APIs interface for separating OS depend system call.
 *           Let the RTK SDK call the layer and become OS independent SDK package.
 *
 * Feature : CPU relative API
 *
 */

/*
 * Include Files
 */
#include <common/type.h>


/*
 * Symbol Definition
 */


/*
 * Function Declaration
 */

/* Function Name:
 *      osal_cpu_num_cores_get
 * Description:
 *      Get the number of cores available to scheduler
 * Input:
 *      None
 * Output:
 *      pNumCores   - number of cores available to scheduler
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
extern int32
osal_cpu_num_cores_get(uint32 *pNumCores);

