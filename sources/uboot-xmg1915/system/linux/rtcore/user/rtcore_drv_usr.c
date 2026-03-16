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
 * $Revision: 74045 $
 * $Date: 2016-12-06 16:44:01 +0800 (Tue, 06 Dec 2016) $
 *
 * Purpose : Realtek Switch SDK Core Module In User Space.
 *
 * Feature : Realtek Switch SDK Core Module In User Space.
 *
 */

/*
 * Include Files
 */
#include <fcntl.h>
#include <sys/ioctl.h>
#include <common/error.h>
#include <common/debug/rt_log.h>
#include <private/drv/intr/intr.h>
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <ioal/mem32.h>
#include <rtcore/rtcore.h>
#include <rtcore/user/rtcore_drv_usr.h>
#include <osal/lib.h>
#include <osal/thread.h>
#include <hwp/hw_profile.h>


/*
 * Symbol Definition
 */
#define RTCORE_INTR_HDL_MAX             (8)
#define RTCORE_NIC_INTR_THREAD_NAME     "NIC_INTR_THREAD"
#define RTCORE_SWCORE_INTR_THREAD_NAME  "SWCORE_INTR_THREAD"

typedef struct rtcore_intr_hdl_entry_s {
    void (*fHandler)(void*);
    void *pHandler_param;
} rtcore_intr_hdl_entry_t;

/*
 * Data Declaration
 */
static int32 _fd = 0;

/* for SWITCH CORE module */
static rtcore_intr_hdl_entry_t _swcore_intr_hdl[RTCORE_INTR_HDL_MAX];
static uint32 _num_of_sw_handlers = 0;
static rtcore_ioctl_t _sw_dio;

/* for NIC module */
static rtcore_intr_hdl_entry_t _nic_intr_hdl;
static uint32 _num_of_nic_handlers = 0;
static rtcore_ioctl_t _nic_dio;

/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */
static void *_rtcore_nic_intr_thread(void *pArg);
static void *_rtcore_sw_intr_thread(void *pArg);
static int32 _rtcore_interrupt_enable(uint32 unit, rtcore_intr_type_t type);
static int32 _rtcore_interrupt_disable(uint32 unit, rtcore_intr_type_t type);
static int32 _rtcore_interrupt_wait(uint32 unit, rtcore_intr_type_t type);
static void _sw_interrupt_handler(void);

/* Function Name:
 *      rtcore_usr_init
 * Description:
 *      Initialize RTCORE user layer module.
 * Input:
 *      NONE
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
rtcore_usr_init(void)
{

    if ((_fd = open(RTCORE_DEV_NAME, O_RDWR)) < 0)
    {
        RT_ERR(RT_ERR_FAILED, MOD_RTCORE, "Can't not correctly open rtcore device\n");
        return RT_ERR_FAILED;
    }

    rtcore_usr_intr_attach(HWP_MY_UNIT_ID(), (void *)drv_intr_swcore_handler, NULL, INTR_TYPE_SWCORE);

    drv_intr_enable_set(HWP_MY_UNIT_ID(), LINK_CHANGE_INTR);

    return RT_ERR_OK;
} /* end of rtcore_usr_init */

/* Function Name:
 *      rtcore_usr_intr_attach
 * Description:
 *      Connect interrupt with rtcore module
 * Input:
 *      unit          - unit id
 *      fHandler      - The interrupt handler function that is going to be attached.
 *                      It is required to be called with 'pHandler_param' argument.
 *      pHandler_param- The argument passed to 'fHandler' interrupt handler when interrupt happen.
 *      type          - The specific interrupt type that we wants to connect with.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Note:
 *      None
 */
int32
rtcore_usr_intr_attach(uint32 unit, void (*fHandler)(void*), void *pHandler_param, rtcore_intr_type_t type)
{
    int32 ret;

    /* Check arguments */
    RT_PARAM_CHK(type >= INTR_TYPE_END, RT_ERR_INPUT);

    switch (type)
    {
        case INTR_TYPE_NIC:
            {
                _nic_intr_hdl.pHandler_param = pHandler_param;
                ret = osal_thread_create(RTCORE_NIC_INTR_THREAD_NAME, 8096, 0, (void *)_rtcore_nic_intr_thread, NULL);
                if (0 == ret)
                {
                    RT_ERR(ret, MOD_RTCORE, "NIC interrupt thread create failed");
                    return RT_ERR_FAILED;
                }
                _num_of_nic_handlers++;
            }
            break;
        case INTR_TYPE_SWCORE:
            {
                RT_PARAM_CHK(NULL == fHandler, RT_ERR_NULL_POINTER);
                _swcore_intr_hdl[_num_of_sw_handlers].fHandler = fHandler;
                _swcore_intr_hdl[_num_of_sw_handlers].pHandler_param = pHandler_param;
                if (0 == _num_of_sw_handlers)
                {
                    ret = osal_thread_create(RTCORE_SWCORE_INTR_THREAD_NAME, 8096, 0, (void *)_rtcore_sw_intr_thread, NULL);
                    if (0 == ret)
                    {
                        RT_ERR(ret, MOD_RTCORE, "SWCORE interrupt thread create failed");
                        return RT_ERR_FAILED;
                    }
                }
                _num_of_sw_handlers++;
            }
            break;
        default:
            return RT_ERR_FAILED;
     }

    return RT_ERR_OK;
} /* end of rtcore_usr_intr_attach */

/* Function Name:
 *      rtcore_usr_intr_detach
 * Description:
 *      Disconnect interrupt with rtcore module
 * Input:
 *      unit - unit id
 *      type - The specific interrupt type that we wants to disconnect.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Note:
 *      None
 */
int32
rtcore_usr_intr_detach(uint32 unit, rtcore_intr_type_t type)
{
    int32 ret;

    if ((ret = _rtcore_interrupt_disable(unit, type)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_RTCORE, "Disconnect interrupt failed");
        return ret;
    }

    return RT_ERR_OK;
} /* end of rtcore_usr_intr_detach */


/* Function Name:
 *      _rtcore_nic_intr_thread
 * Description:
 *      NIC interrupt thread to receive interrupt event at user space.
 * Input:
 *      pArg - Parameter that is provided when thread create.
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void *_rtcore_nic_intr_thread(void *pArg)
{
    isr_param_t     my_isr_param;
    _rtcore_interrupt_enable(HWP_MY_UNIT_ID(), INTR_TYPE_NIC);

    while(1)
    {
        _rtcore_interrupt_wait(HWP_MY_UNIT_ID(), INTR_TYPE_NIC);
        my_isr_param.unit = _nic_dio.data[0];
        drv_nic_isr_handler(&my_isr_param);
    }
    return NULL;
} /* end of _rtcore_nic_intr_thread */

/* Function Name:
 *      _rtcore_sw_intr_thread
 * Description:
 *      SWCORE interrupt thread to receive interrupt event at user space.
 * Input:
 *      pArg - Parameter that is provided when thread create.
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void *_rtcore_sw_intr_thread(void *pArg)
{
    _rtcore_interrupt_enable(HWP_MY_UNIT_ID(), INTR_TYPE_SWCORE);

    while(1)
    {
        _rtcore_interrupt_wait(HWP_MY_UNIT_ID(), INTR_TYPE_SWCORE);
        _sw_interrupt_handler();
    }
    return NULL;
} /* end of _rtcore_swcore_intr_thread */

/* Function Name:
 *      _rtcore_interrupt_enable
 * Description:
 *      Enable specific interrupt type.
 * Input:
 *      unit - unit id
 *      type - The specific interrupt type that we wants to enable.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
static int32 _rtcore_interrupt_enable(uint32 unit, rtcore_intr_type_t type)
{
    rtcore_ioctl_t dio;

    dio.data[0] = unit;
    dio.data[1] = ENABLED;
    dio.data[2] = type;
    ioctl(_fd, RTCORE_INTR_ENABLE_SET, &dio);

    return dio.ret;
}  /* end of _rtcore_interrupt_enable */

/* Function Name:
 *      _rtcore_interrupt_disable
 * Description:
 *      Disable specific interrupt type.
 * Input:
 *      unit - unit id
 *      type - The specific interrupt type that we wants to disable.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
static int32 _rtcore_interrupt_disable(uint32 unit, rtcore_intr_type_t type)
{
    rtcore_ioctl_t dio;

    dio.data[0] = unit;
    dio.data[1] = DISABLED;
    dio.data[2] = type;
    ioctl(_fd, RTCORE_INTR_ENABLE_SET, &dio);

    return dio.ret;
} /* end of _rtcore_interrupt_disable */

/* Function Name:
 *      _rtcore_interrupt_wait
 * Description:
 *      Pending interrupt thread of specific interrupt type for waiting interrupt event.
 * Input:
 *      unit - unit id
 *      type - The specific interrupt type that we wants to wait for.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
static int32 _rtcore_interrupt_wait(uint32 unit, rtcore_intr_type_t type)
{
    rtcore_ioctl_t dio;

    dio.data[0] = unit;
    dio.data[1] = type;
    ioctl(_fd, RTCORE_INTR_WAIT, &dio);

    /* Assign interrupt mask status */
    if (INTR_TYPE_SWCORE == type)
        osal_memcpy(&_sw_dio, &dio, sizeof(rtcore_ioctl_t));
    else
        osal_memcpy(&_nic_dio, &dio, sizeof(rtcore_ioctl_t));

    return dio.ret;
} /* end of _rtcore_interrupt_disable */

/* Function Name:
 *      _sw_interrupt_handler
 * Description:
 *      Pending interrupt thread of specific interrupt type for waiting interrupt event.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void _sw_interrupt_handler(void)
{
    uint32 i;

    for(i = 0; i < _num_of_sw_handlers; i++)
    {
        if (NULL != _swcore_intr_hdl[i].fHandler)
        {
            _swcore_intr_hdl[i].fHandler((void *)&_sw_dio);
        }
    }
    return;
} /* end of _sw_interrupt_handler */

