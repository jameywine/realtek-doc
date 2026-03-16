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
 * $Revision: 71708 $
 * $Date: 2016-09-19 11:31:17 +0800 (Mon, 19 Sep 2016) $
 *
 * Purpose : Define the utility macro and function in the SDK.
 *
 * Feature : SDK common utility (time)
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/error.h>
#include <osal/print.h>
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <drv/tc/tc.h>
#include <hwp/hw_profile.h>
#include <osal/isr.h>
#include <common/util/rt_util_time.h>
#include <dev_config.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
static rt_util_timeTcIsr_db_t tcIsr_Db;
static rt_timeval_t rt_tv1={"",0,0,0};
static uint32 tc_unit;
static rt_timeval_t rt_util_hpt_recorded_tv[RT_UTIL_TIME_RECORD_MAX];
static int rt_util_hpt_record_max=0;

/*
 * Macro Definition
 */


/*
 * Function Declaration
 */

/* Function Name:
 *      _rt_util_tc_isr_t
 * Description:
 *      ISR of TC
 * Input:
 *      *p  - user data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *
 */
osal_isrret_t
_rt_util_tc_isr_t(void *p)
{

    rt_util_timeTcIsr_db_t *db = (rt_util_timeTcIsr_db_t *)p;
    rt_tv1.sec++;
    drv_tc_intState_clear(tc_unit, db->tc_id);

    return RT_ERR_OK;
}


/* Function Name:
 *      rt_util_hpt_init
 * Description:
 *      Init the HPT (high precision time) module, now using TC1
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *
 */
int32
rt_util_hpt_init(void)
{

    tc_unit=HWP_MY_UNIT_ID();

    /* set TC1 to 10 ns resolution */
    drv_tc_mode_set(tc_unit, TC_ID1, TC_MODE_TIMER);
    drv_tc_divFactor_set(tc_unit, TC_ID1, 2);
    drv_tc_dataInitValue_set(tc_unit, TC_ID1, 0x5F5E100);

    osal_isr_unregister(RTK_DEV_TC1);
    tcIsr_Db.tc_id = TC_ID1;
    osal_isr_register(RTK_DEV_TC1, _rt_util_tc_isr_t, (void *)&tcIsr_Db);
    drv_tc_intEnable_set(tc_unit,TC_ID1,ENABLED);

    drv_tc_enable_set(tc_unit, TC_ID1, ENABLED);

    return RT_ERR_OK;
}


/* Function Name:
 *      rt_util_hpt_get
 * Description:
 *      Get the current timer tick of TC1
 * Input:
 *      None
 * Output:
 *      *tv     - the value
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *
 */
int32
rt_util_hpt_get(rt_timeval_t *tv)
{
    uint32 value;

    drv_tc_counterValue_get(tc_unit,TC_ID1,&value);
    tv->sec = rt_tv1.sec;
    tv->usec = value/100;
    tv->nsec = (value%100)*10;
    return RT_ERR_OK;
}


/* Function Name:
 *      rt_util_hpt_record
 * Description:
 *      Get the current timer tick of TC1, and record it into a variable. Later it can be printed out by rt_util_hptRecord_print().
 *      Just call it with a interger as its first parameter. Like:
 *          int i=o;
 *          i=rt_util_hpt_record(i, __FUNCTION__,__LINE__);
 *          .....
 *          i=rt_util_hpt_record(i, __FUNCTION__,__LINE__);
 * Input:
 *      int i               - user should declare a integer for this parameter (init as 0), the function will return "next" i for next time calling
 *      func_name      - put __FUNCTION__
 *      line                - put __LINE__
 * Output:
 *      None
 * Return:
 *      next i
 * Note:
 *
 */
int
rt_util_hpt_record(int i, const char *func_name, int line)
{
    if(i>=RT_UTIL_TIME_RECORD_MAX)
        return RT_UTIL_TIME_RECORD_MAX;

    rt_util_hpt_get(&rt_util_hpt_recorded_tv[i]);
    osal_snprintf(rt_util_hpt_recorded_tv[i].str,RT_UTIL_TIME_RECORD_STR_MAX,"%s(%d)",func_name,line);
    rt_util_hpt_record_max = i;
    return i+1;
}

/* Function Name:
 *      rt_util_hpt_print
 * Description:
 *      Print out rt_timeval_t contents, with user's string "str".
 * Input:
 *      usr_str            - any user's string which will prepend to the printing message.
 *      tv                  - the rt_timeval_t to print
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *
 */
void
rt_util_hpt_print(uint8 *usr_str,rt_timeval_t *tv)
{
    osal_printf("%s%s(%d.%06dsec,%dns)\n",tv->str,usr_str,tv->sec,tv->usec,tv->nsec);
}


/* Function Name:
 *      rt_util_hptRecord_print
 * Description:
 *      Print the contents that rt_util_hpt_record() recorded.
 * Input:
 *      usr_str            - any user's string which will insert to the printing message.
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *
 */
void
rt_util_hptRecord_print(uint8 *usr_str)
{
    int i;
    for(i=0;i<=rt_util_hpt_record_max;i++)
    {
        osal_printf("%s%s(%d.%06dsec,%dns)\n",rt_util_hpt_recorded_tv[i].str,
                                              usr_str,
                                              rt_util_hpt_recorded_tv[i].sec,
                                              rt_util_hpt_recorded_tv[i].usec,
                                              rt_util_hpt_recorded_tv[i].nsec);
    }
    rt_util_hpt_record_max = 0;
}


