/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision: 79582 $
 * $Date: 2017-06-13 16:50:29 +0800 (Tue, 13 Jun 2017) $
 *
 * Purpose : Realtek Switch SDK Rtusr API Module
 *
 * Feature : The file have include the following module and sub-modules
 *           1) trunk
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rtk/stack.h>
#include <rtusr_util.h>
#include <rtdrv/rtdrv_netfilter.h>


int32
rtk_stack_port_get (uint32 unit, rtk_portmask_t *pStkPorts)
{
    rtdrv_stackCfg_t stack_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pStkPorts), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_PORT_GET, &stack_cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pStkPorts, &stack_cfg.stkPorts, sizeof(rtk_portmask_t));

    return RT_ERR_OK;
}   /* end of rtk_stack_port_get */

int32
rtk_stack_port_set (uint32 unit, rtk_portmask_t *pStkPorts)
{
    rtdrv_stackCfg_t stack_cfg;

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&stack_cfg.stkPorts, pStkPorts, sizeof(rtk_portmask_t));
    SETSOCKOPT(RTDRV_STACK_PORT_SET, &stack_cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_stack_port_set */

int32
rtk_stack_unit_get(uint32 unit, uint32 *pMyUnit)
{
    rtdrv_stackCfg_t stack_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pMyUnit), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_UNIT_GET, &stack_cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pMyUnit, &stack_cfg.unitIdx, sizeof(uint32));

    return RT_ERR_OK;
}   /* end of rtk_stack_unit_get */

int32
rtk_stack_unit_set(uint32 unit, uint32 myUnit)
{
    rtdrv_stackCfg_t stack_cfg;

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&stack_cfg.unitIdx, &myUnit, sizeof(uint32));
    SETSOCKOPT(RTDRV_STACK_UNIT_SET, &stack_cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_stack_unit_set */

int32
rtk_stack_masterUnit_get(uint32 unit, uint32 *pMasterUnit)
{
    rtdrv_stackCfg_t stack_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pMasterUnit), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_MASTERUNIT_GET, &stack_cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pMasterUnit, &stack_cfg.unitIdx, sizeof(uint32));

    return RT_ERR_OK;
}   /* end of rtk_stack_masterUnit_get */

int32
rtk_stack_masterUnit_set(uint32 unit, uint32 masterUnit)
{
    rtdrv_stackCfg_t stack_cfg;

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&stack_cfg.unitIdx, &masterUnit, sizeof(uint32));
    SETSOCKOPT(RTDRV_STACK_MASTERUNIT_SET, &stack_cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_stack_masterUnit_set */

int32
rtk_stack_loopGuard_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_stackCfg_t stack_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_LOOPGUARD_GET, &stack_cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pEnable, &stack_cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
}   /* end of rtk_stack_loopGuard_get */

int32
rtk_stack_loopGuard_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_stackCfg_t stack_cfg;

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&stack_cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_STACK_LOOPGUARD_SET, &stack_cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_stack_loopGuard_set */

int32
rtk_stack_unitPortMap_get (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts)
{
    rtdrv_stackCfg_t stack_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pStkPorts), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&stack_cfg.unitIdx, &dstUnit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_UNITPORTMAP_GET, &stack_cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pStkPorts, &stack_cfg.stkPorts, sizeof(rtk_portmask_t));

    return RT_ERR_OK;
}   /* end of rtk_stack_unitPortMap_get */

int32
rtk_stack_unitPortMap_set (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts)
{
    rtdrv_stackCfg_t stack_cfg;

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    osal_memcpy(&stack_cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&stack_cfg.unitIdx, &dstUnit, sizeof(uint32));
    osal_memcpy(&stack_cfg.stkPorts, pStkPorts, sizeof(rtk_portmask_t));
    SETSOCKOPT(RTDRV_STACK_UNITPORTMAP_SET, &stack_cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_stack_unitPortMap_set */

int32
rtk_stack_nonUcastBlockPort_get (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts)
{
    rtdrv_stackCfg_t stack_cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pBlockStkPorts), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    stack_cfg.unit = unit;
    stack_cfg.srcUnit = srcUnit;
    GETSOCKOPT(RTDRV_STACK_NONUCASTBLOCKPORT_GET, &stack_cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pBlockStkPorts, &stack_cfg.stkPorts, sizeof(rtk_portmask_t));

    return RT_ERR_OK;
}   /* end of rtk_stack_nonUcastBlockPort_get */

int32
rtk_stack_nonUcastBlockPort_set (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts)
{
    rtdrv_stackCfg_t stack_cfg;

    /* function body */
    osal_memset(&stack_cfg, 0, sizeof(stack_cfg));
    stack_cfg.unit = unit;
    stack_cfg.srcUnit = srcUnit;
    osal_memcpy(&stack_cfg.stkPorts, pBlockStkPorts, sizeof(rtk_portmask_t));
    SETSOCKOPT(RTDRV_STACK_NONUCASTBLOCKPORT_SET, &stack_cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
}   /* end of rtk_stack_nonUcastBlockPort_set */

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
 * Note:
 *      None
 */
int32
rtk_stack_rmtIntrTxEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_stackCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_RMTINTRTXENABLE_GET, &cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_stack_rmtIntrTxEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_stackCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_STACK_RMTINTRTXENABLE_SET, &cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_stack_rmtIntrTxTriggerEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_stackCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_RMTINTRTXTRIGGERENABLE_GET, &cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
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
 * Note:
 *      The enable status will be clear automatically once the transmission has been done.
 */
int32
rtk_stack_rmtIntrTxTriggerEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_stackCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_STACK_RMTINTRTXTRIGGERENABLE_SET, &cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_stack_rmtIntrRxSeqCmpMargin_get(uint32 unit, int32 *pMargin)
{
    rtdrv_stackCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pMargin), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_RMTINTRRXSEQCMPMARGIN_GET, &cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pMargin, &cfg.margin, sizeof(int32));

    return RT_ERR_OK;
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
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
rtk_stack_rmtIntrRxSeqCmpMargin_set(uint32 unit, int32 margin)
{
    rtdrv_stackCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.margin, &margin, sizeof(int32));
    SETSOCKOPT(RTDRV_STACK_RMTINTRRXSEQCMPMARGIN_SET, &cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_stack_rmtIntrRxForceUpdateEnable_get(uint32 unit, rtk_enable_t *pEnable)
{
    rtdrv_stackCfg_t cfg;

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    GETSOCKOPT(RTDRV_STACK_RMTINTRRXFORCEUPDATEENABLE_GET, &cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pEnable, &cfg.enable, sizeof(rtk_enable_t));

    return RT_ERR_OK;
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
 * Note:
 *      The enable status will be clear automatically once the updating has been done.
 */
int32
rtk_stack_rmtIntrRxForceUpdateEnable_set(uint32 unit, rtk_enable_t enable)
{
    rtdrv_stackCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.enable, &enable, sizeof(rtk_enable_t));
    SETSOCKOPT(RTDRV_STACK_RMTINTRRXFORCEUPDATEENABLE_SET, &cfg, rtdrv_stackCfg_t, 1);

    return RT_ERR_OK;
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
 * Note:
 *      None
 */
int32
rtk_stack_rmtIntrInfo_get(uint32 unit, rtk_stack_rmtIntrInfo_t *pInfo)
{
    rtdrv_stackCfg_t cfg;

    /* function body */
    osal_memset(&cfg, 0, sizeof(cfg));
    osal_memcpy(&cfg.unit, &unit, sizeof(uint32));
    osal_memcpy(&cfg.info, pInfo, sizeof(rtk_stack_rmtIntrInfo_t));
    GETSOCKOPT(RTDRV_STACK_RMTINTRINFO_GET, &cfg, rtdrv_stackCfg_t, 1);
    osal_memcpy(pInfo, &cfg.info, sizeof(rtk_stack_rmtIntrInfo_t));

    return RT_ERR_OK;
}   /* end of rtk_stack_rmtIntrInfo_get */


