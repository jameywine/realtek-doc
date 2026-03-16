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
 * $Revision: 77260 $
 * $Date: 2017-04-09 12:45:29 +0800 (Sun, 09 Apr 2017) $
 *
 * Purpose : Definition of TRUNK API
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) User Configuration Trunk
 *
 */

/*
 * Include Files
 */
#include <common/rt_error.h>
#include <common/util/rt_util.h>
#include <dal/dal_mgmt.h>
#include <rtk/default.h>
#include <rtk/stack.h>
#include <hwp/hwp_unit.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */

/* Module Name : STACK */

/* Function Name:
 *      rtk_stack_init
 * Description:
 *      Initialize stack module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      9300, 9310
 * Note:
 *      Must initialize stack module before calling any stack APIs.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_init(uint32 unit)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_init(unit);
} /* end of rtk_stack_init */

/* Function Name:
 *      rtk_stack_cascadeMode_init
 * Description:
 *      Initialize cascade mode.
 * Input:
 *      unit  - unit id
 * Output:
 *      none
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_cascadeMode_init(uint32 unit)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_cascade_init(unit);
} /* end of rtk_stack_cascadeMode_init */

/* Module Name    : STACK                    */
/* Sub-module Name: User configuration stack */

/* Function Name:
 *      rtk_stack_port_get
 * Description:
 *      Get the stacking port from the specified device.
 * Input:
 *      unit  - unit id
 * Output:
 *      pStkPorts - pointer buffer of stacking ports
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_port_get (uint32 unit, rtk_portmask_t *pStkPorts)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_port_get(unit, pStkPorts);
}

/* Function Name:
 *      rtk_stack_port_set
 * Description:
 *      Set stacking ports to the specified device.
 * Input:
 *      unit - unit id
 *      pStkPorts - pointer buffer of stacking ports
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      In 9300, only port 24~27 can be configured as stacking ports.
 *      In 9310, only port 52~55 can be configured as stacking ports.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_port_set (uint32 unit, rtk_portmask_t *pStkPorts)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_port_set(unit, pStkPorts);
}

/* Function Name:
 *      rtk_stack_unit_get
 * Description:
 *      Get the switch unit ID from the device.
 * Input:
 *      unit                   - unit id
 * Output:
 *      pMyUnit              - pointer buffer of uind ID of the switch
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_unit_get(uint32 unit, uint32 *pMyUnit)
{
    RT_PARAM_CHK((NULL == pMyUnit), RT_ERR_NULL_POINTER);

    return  RT_MAPPER(unit)->stack_unit_get(unit, pMyUnit);
}

/* Function Name:
 *      rtk_stack_unit_set
 * Description:
 *      Set the switch unit ID to the specified device.
 * Input:
 *      unit                   - unit id
 *      myUnit              - uind ID of the switch
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID           - invalid unit id
 *      RT_ERR_NOT_INIT          - The module is not initial
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) 9300, 9310 supports 16 stacked units, thus myUnit ranges from 0 to 15.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_unit_set(uint32 unit, uint32 myUnit)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    hwp_unitID_change(unit, myUnit);

    return  RT_MAPPER(unit)->stack_unit_set(unit, myUnit);
}

/* Function Name:
 *      rtk_stack_masterUnit_get
 * Description:
 *      Get the master unit ID from the specified device.
 * Input:
 *      unit                   - unit id
 * Output:
 *      pMasterUnit        - pointer buffer of uind ID of the master switch
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_masterUnit_get(uint32 unit, uint32 *pMasterUnit)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_masterUnit_get(unit, pMasterUnit);
}

/* Function Name:
 *      rtk_stack_masterUnit_set
 * Description:
 *      Set the master unit ID to the specified device.
 * Input:
 *      unit                   - unit id
 *      masterUnit         - uind ID of the master switch
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID           - invalid unit id
 *      RT_ERR_NOT_INIT          - The module is not initial
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) 9300, 9310 supports 16 stacked units, thus masterUnit ranges from 0 to 15.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_masterUnit_set(uint32 unit, uint32 masterUnit)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_masterUnit_set(unit, masterUnit);
}

/* Function Name:
 *      rtk_stack_loopGuard_get
 * Description:
 *      Get the enable status of dropping packets with source unit ID the same as the unit ID of the switch from the specified device.
 * Input:
 *      unit          - unit id
 * Output:
 *      pEnable     - pointer buffer of enable state of loop guard mechanism
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_loopGuard_get(uint32 unit, rtk_enable_t *pEnable)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_loopGuard_get(unit, pEnable);
}


/* Function Name:
 *      rtk_stack_loopGuard_set
 * Description:
 *      Set the enable status of dropping packets with source unit ID the same as the unit ID of the switch to the specified device.
 * Input:
 *      unit          - unit id
 *      enable      - enable state of loop guard mechanism
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_loopGuard_set(uint32 unit, rtk_enable_t enable)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_loopGuard_set(unit, enable);
}

/* Function Name:
 *      rtk_stack_unitPortMap_get
 * Description:
 *      Get the stacking ports that packets with specific target unit should forward to from the specified device.
 * Input:
 *      unit                   - unit id
 *      dstUnit                - unid ID of forwarding target
 * Output:
 *      pStkPorts           - pointer buffer of egress stacking ports
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_unitPortMap_get (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_unitPortMap_get(unit, dstUnit, pStkPorts);
}


/* Function Name:
 *      rtk_stack_unitPortMap_set
 * Description:
 *      Set the stacking ports that packets with specific target unit should forward to for the specified device.
 * Input:
 *      unit                   - unit id
 *      dstUnit              - unid ID of forwarding target
 *      pStkPorts           - pointer buffer of egress stacking ports
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) Stacking ports in 9300 ranges from 24 to 27.
 *      (2) Stacking ports in 9310 ranges from 52 to 55.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_unitPortMap_set (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_unitPortMap_set(unit, dstUnit, pStkPorts);
}

/* Function Name:
 *      rtk_stack_nonUcastBlockPort_get
 * Description:
 *      Get the stacking ports that would block ingress and egress non-unicast packets from the specified device.
 * Input:
 *      unit                   - unit id
 *      srcUnit               - source unit id
 * Output:
 *      pBlockStkPorts    - pointer buffer of blocked stacking ports
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_nonUcastBlockPort_get (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_nonUcastBlockPort_get(unit, srcUnit, pBlockStkPorts);
}


/* Function Name:
 *      rtk_stack_nonUcastBlockPort_set
 * Description:
 *      Set the stacking ports that would block ingress and egress non-unicast packets to the specified device.
 * Input:
 *      unit                   - unit id
 *      srcUnit               - source unit id
 *      pBlockStkPorts    - pointer buffer of blocked stacking ports
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) Blockable stacking ports in 9300 ranges from 24 to 27.
 *      (2) Blockable stacking ports in 9310 ranges from 52 to 55.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_nonUcastBlockPort_set (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_nonUcastBlockPort_set(unit, srcUnit, pBlockStkPorts);
}

/* Function Name:
 *      rtk_stack_rmtIntrTxEnable_get
 * Description:
 *      Get enable status of Remote Interrupt Notification transmission.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - pointer to enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrTxEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrTxEnable_get(unit, pEnable);
}   /* end of rtk_stack_rmtIntrTxEnable_get */

/* Function Name:
 *      rtk_stack_rmtIntrTxEnable_set
 * Description:
 *      Set enable status of Remote Interrupt Notification transmission.
 * Input:
 *      unit   - unit id
 *      enable - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrTxEnable_set(uint32 unit, rtk_enable_t enable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrTxEnable_set(unit, enable);
}   /* end of rtk_stack_rmtIntrTxEnable_set */

/* Function Name:
 *      rtk_stack_rmtIntrTxTriggerEnable_get
 * Description:
 *      Get enable status of Remote Interrupt Notification transmission trigger.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - pointer to enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrTxTriggerEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrTxTriggerEnable_get(unit, pEnable);
}   /* end of rtk_stack_rmtIntrTxTriggerEnable_get */

/* Function Name:
 *      rtk_stack_rmtIntrTxTriggerEnable_set
 * Description:
 *      Set enable status of Remote Interrupt Notification transmission trigger.
 * Input:
 *      unit   - unit id
 *      enable - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      The enable status will be clear automatically once the transmission has been done.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrTxTriggerEnable_set(uint32 unit, rtk_enable_t enable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrTxTriggerEnable_set(unit, enable);
}   /* end of rtk_stack_rmtIntrTxTriggerEnable_set */

/* Function Name:
 *      rtk_stack_rmtIntrRxSeqCmpMargin_get
 * Description:
 *      Get the comparing margin of the sequence ID of receiving Remote Interrupt Notification.
 * Input:
 *      unit    - unit id
 * Output:
 *      pMargin - pointer to margin value
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrRxSeqCmpMargin_get(uint32 unit, int32 *pMargin)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrRxSeqCmpMargin_get(unit, pMargin);
}   /* end of rtk_stack_rmtIntrRxSeqCmpMargin_get */

/* Function Name:
 *      rtk_stack_rmtIntrRxSeqCmpMargin_set
 * Description:
 *      Set the comparing margin of the sequence ID of receiving Remote Interrupt Notification.
 * Input:
 *      unit   - unit id
 *      margin - margin value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrRxSeqCmpMargin_set(uint32 unit, int32 margin)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrRxSeqCmpMargin_set(unit, margin);
}   /* end of rtk_stack_rmtIntrRxSeqCmpMargin_set */

/* Function Name:
 *      rtk_stack_rmtIntrRxForceUpdateEnable_get
 * Description:
 *      Get the force enable status of updating when receives a Remote Interrupt Notification.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - pointer to enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrRxForceUpdateEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrRxForceUpdateEnable_get(unit, pEnable);
}   /* end of rtk_stack_rmtIntrRxForceUpdateEnable_get */

/* Function Name:
 *      rtk_stack_rmtIntrRxForceUpdateEnable_set
 * Description:
 *      Set the force enable status of updating when receives a Remote Interrupt Notification.
 * Input:
 *      unit   - unit id
 *      enable - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      The enable status will be clear automatically once the updating has been done.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrRxForceUpdateEnable_set(uint32 unit, rtk_enable_t enable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrRxForceUpdateEnable_set(unit, enable);
}   /* end of rtk_stack_rmtIntrRxForceUpdateEnable_set */

/* Function Name:
 *      rtk_stack_rmtIntrInfo_get
 * Description:
 *      Get the information about Remote Interrupt Notification.
 * Input:
 *      unit  - unit id
 * Output:
 *      pInfo - pointer to information
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
rtk_stack_rmtIntrInfo_get(uint32 unit, rtk_stack_rmtIntrInfo_t *pInfo)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID) || NULL == RT_MGMT(unit), RT_ERR_UNIT_ID);

    return RT_MAPPER(unit)->stack_rmtIntrInfo_get(unit, pInfo);
}   /* end of rtk_stack_rmtIntrInfo_get */

