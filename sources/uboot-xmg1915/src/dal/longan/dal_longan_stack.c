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
 * $Revision: 81029 $
 * $Date: 2017-08-07 14:34:45 +0800 (Mon, 07 Aug 2017) $
 *
 * Purpose : Definition those public STACK APIs and its data type in the SDK.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) Stack
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/util/rt_bitop.h>
#include <common/util/rt_util.h>
#include <common/debug/rt_log.h>
#include <osal/sem.h>
#include <osal/lib.h>
#include <osal/memory.h>
#include <hal/chipdef/allmem.h>
#include <hal/chipdef/allreg.h>
#include <hal/chipdef/longan/rtk_longan_table_struct.h>
#include <hal/chipdef/longan/rtk_longan_reg_struct.h>
#include <hal/mac/reg.h>
#include <hal/mac/mem.h>
#include <hal/common/halctrl.h>
#include <dal/longan/dal_longan_stack.h>
#include <dal/longan/dal_longan_l2.h>
#include <dal/longan/dal_longan_port.h>
#include <dal/longan/dal_longan_trunk.h>
#include <dal/longan/dal_longan_flowctrl.h>


#include <rtk/default.h>
#include <rtk/stack.h>

#include <hal/phy/phy_rtl9300.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
static uint32                   stack_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};
static osal_mutex_t         stack_sem[RTK_MAX_NUM_OF_UNIT];

/*
 * Macro Definition
 */
/* stack semaphore handling */
#define STACK_SEM_LOCK(unit)    \
do {\
    if (osal_sem_mutex_take(stack_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_DAL|MOD_STACK), "semaphore lock failed");\
        return RT_ERR_SEM_LOCK_FAILED;\
    }\
} while(0)

#define STACK_SEM_UNLOCK(unit)   \
do {\
    if (osal_sem_mutex_give(stack_sem[unit]) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_DAL|MOD_STACK), "semaphore unlock failed");\
        return RT_ERR_SEM_UNLOCK_FAILED;\
    }\
} while(0)


#define RTK_STACK_CASCADE_PORTMASK ((1<<24) | (1<<25))

/*
 * Function Declaration
 */

static int32 _dal_longan_stack_init_config(uint32 unit);
static int32 _dal_longan_stack_cascadeMode_init_config(uint32 unit, rtk_stack_cascadeCfg_t *pCascadeCfg);

/* Function Name:
 *      dal_longan_stackMapper_init
 * Description:
 *      Hook stack module of the specified device.
 * Input:
 *      pMapper - pointer of mapper
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 * Note:
 *      Must Hook stack module before calling any stack APIs.
 */
int32
dal_longan_stackMapper_init(dal_mapper_t *pMapper)
{
    pMapper->stack_init = dal_longan_stack_init;
    pMapper->stack_cascade_init = dal_longan_stack_cascadeMode_init;
    pMapper->stack_port_get = dal_longan_stack_port_get;
    pMapper->stack_port_set = dal_longan_stack_port_set;
    pMapper->stack_unit_get = dal_longan_stack_unit_get;
    pMapper->stack_unit_set = dal_longan_stack_unit_set;
    pMapper->stack_masterUnit_get = dal_longan_stack_masterUnit_get;
    pMapper->stack_masterUnit_set = dal_longan_stack_masterUnit_set;
    pMapper->stack_loopGuard_get = dal_longan_stack_loopGuard_get;
    pMapper->stack_loopGuard_set = dal_longan_stack_loopGuard_set;
    pMapper->stack_unitPortMap_get = dal_longan_stack_unitPortMap_get;
    pMapper->stack_unitPortMap_set = dal_longan_stack_unitPortMap_set;
    pMapper->stack_nonUcastBlockPort_get = dal_longan_stack_nonUcastBlockPort_get;
    pMapper->stack_nonUcastBlockPort_set = dal_longan_stack_nonUcastBlockPort_set;

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_longan_stack_init
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
 * Note:
 *      Must initialize stack module before calling any stack APIs.
 */
int32
dal_longan_stack_init(uint32 unit)
{
    int32   ret;

    RT_INIT_REENTRY_CHK(stack_init[unit]);
    stack_init[unit] = INIT_NOT_COMPLETED;

    /* create semaphore */
    stack_sem[unit] = osal_sem_mutex_create();
    if (0 == stack_sem[unit])
    {
        RT_ERR(RT_ERR_FAILED, (MOD_DAL|MOD_STACK), "semaphore create failed");
        return RT_ERR_FAILED;
    }

    /* set init flag to complete init */
    stack_init[unit] = INIT_COMPLETED;

    /* initialize default configuration */
    if ((ret = _dal_longan_stack_init_config(unit)) != RT_ERR_OK)
    {
        stack_init[unit] = INIT_NOT_COMPLETED;

        RT_ERR(ret, (MOD_STACK|MOD_DAL), "init default configuration failed");
        return ret;
    }

    return RT_ERR_OK;
}   /* end of dal_longan_stack_init */

/* Function Name:
 *      dal_longan_stack_cascadeMode_init
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
dal_longan_stack_cascadeMode_init(uint32 unit)
{
    int32 ret;
    rtk_stack_cascadeCfg_t cascadeCfg;

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    if(HWP_CASCADE_MODE()==FALSE)
        return RT_ERR_OK;

    if((RTL9301_CHIP_ID_24G == HWP_CHIP_ID(unit)) && (1==HWP_CHIP_REV(unit)) )
    {
        cascadeCfg.e2e_fc_en = DISABLED;
        cascadeCfg.e2e_fc_normalPort_en = DISABLED;
        cascadeCfg.e2e_ntfy_en = DISABLED;
    }
    else if((RTL9301_CHIP_ID_24G == HWP_CHIP_ID(unit)) && (2==HWP_CHIP_REV(unit)) )
    {
        cascadeCfg.e2e_fc_en = ENABLED;
        cascadeCfg.e2e_fc_normalPort_en = ENABLED;
        cascadeCfg.e2e_ntfy_en = DISABLED;
    }
    else
    {
        cascadeCfg.e2e_fc_en = ENABLED;
        cascadeCfg.e2e_fc_normalPort_en = ENABLED;
        cascadeCfg.e2e_ntfy_en = ENABLED;
    }


    /* initialize default configuration */
    if ((ret = _dal_longan_stack_cascadeMode_init_config(unit, &cascadeCfg)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "init Cascade configuration failed");
        return ret;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      dal_longan_stack_cascadeMode_init
 * Description:
 *      Initialize cascade mode.
 * Input:
 *      unit  - unit id
 * Output:
 *      pCascadeCfg - pointer to cascade configuration
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
static int32
_dal_longan_stack_cascadeMode_init_config(uint32 unit, rtk_stack_cascadeCfg_t *pCascadeCfg)
{
    int32 ret;
    uint32  val;
    uint32 port;
    uint32 queue;
    uint32 groupId;

    uint32 masterUnitId;
    uint32 slaveUnitId;

    uint32 reg_field;

    rtk_portmask_t cascadePortmask;

    rtk_flowctrl_thresh_t thresh;
    rtk_flowctrl_drop_thresh_t drop_thresh;


    osal_memset(&cascadePortmask, 0, sizeof(rtk_portmask_t));
    HWP_GET_ATTRI_PORTMASK(unit, HWP_CASCADE_ID, cascadePortmask);

    masterUnitId = HWP_MY_UNIT_ID() ;
    slaveUnitId = HWP_CASCADE_SLAVE_UNIT_ID();

    RT_PARAM_CHK((unit!=masterUnitId && unit!=slaveUnitId), RT_ERR_UNIT_ID);

    /*unit configuration*/
    if ((ret = dal_longan_stack_unit_set(unit, unit)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_stack_masterUnit_set(unit, masterUnitId)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    /*stacking port & trunk configuration*/
    if ((ret = dal_longan_stack_port_set(unit, &cascadePortmask)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_trunk_stkTrkPort_set(unit, 0, &cascadePortmask)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_trunk_distributionAlgorithmTypeParam_set(unit, PARAM_TYPE_L2, 0, 0x1)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_trunk_distributionAlgorithmTypeParam_set(unit, PARAM_TYPE_L3, 0, 0x1)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_trunk_stkTrkHash_set(unit, STACK_TRK_HASH_RECALCULATE)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_stack_unitPortMap_set(unit, (unit==masterUnitId ? slaveUnitId : masterUnitId), &cascadePortmask)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    /*Enable stacking Auto Learn*/
    if ((ret = dal_longan_l2_stkLearningEnable_set(unit, ENABLED)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    STACK_SEM_LOCK(unit);

    /*stacking port FC disable*/
    for(port=24; port<28; port++)
    {
        if(!(cascadePortmask.bits[0] & (1<<port)))
            continue;

        val=1;
        if ((ret = reg_array_field_write(unit, LONGAN_MAC_FORCE_MODE_CTRLr, port, REG_ARRAY_INDEX_NONE, LONGAN_MAC_FORCE_FC_ENf, &val)) != RT_ERR_OK)
        {
            STACK_SEM_UNLOCK(unit);
            RT_ERR(ret, (MOD_DAL|MOD_PORT), "");
            return ret;
        }

        val=0;
        if ((ret = reg_array_field_write(unit, LONGAN_MAC_FORCE_MODE_CTRLr, port, REG_ARRAY_INDEX_NONE, LONGAN_RX_PAUSE_ENf, &val)) != RT_ERR_OK)
        {
            STACK_SEM_UNLOCK(unit);
            RT_ERR(ret, (MOD_DAL|MOD_PORT), "");
            return ret;
        }
        if ((ret = reg_array_field_write(unit, LONGAN_MAC_FORCE_MODE_CTRLr, port, REG_ARRAY_INDEX_NONE, LONGAN_TX_PAUSE_ENf, &val)) != RT_ERR_OK)
        {
            STACK_SEM_UNLOCK(unit);
            RT_ERR(ret, (MOD_DAL|MOD_PORT), "");
            return ret;
        }

        if(HWP_PORT_ETH(unit, port) == HWP_SXGE)
        {
            val = 1;
            reg_field = LONGAN_P24_IN_SXG_MODEf;
            if(port==24)
                reg_field = LONGAN_P24_IN_SXG_MODEf;
            else if(port==25)
                reg_field = LONGAN_P25_IN_SXG_MODEf;

            if(port==24 || port==25)
            {
                if ((ret = reg_field_write(unit, LONGAN_EGBW_RATE_SXG_CTRLr, reg_field, &val)) != RT_ERR_OK)
                {
                    STACK_SEM_UNLOCK(unit);
                    RT_ERR(ret, (MOD_DAL|MOD_PORT), "");
                    return ret;
                }
            }
        }
    }


    /*E2E FC configuration*/
    val = 0xfc8;
    if ((ret = reg_field_write(unit, LONGAN_FC_GLB_DROP_THRr, LONGAN_DROP_ALLf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    val = 0x4a6;
    if ((ret = reg_field_write(unit, LONGAN_FC_GLB_SYS_UTIL_THRr, LONGAN_THRf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }

    thresh.highOn   = 0x542;
    thresh.highOff  = 0x50e;
    thresh.lowOn    = 0x4a6;
    thresh.lowOff   = 0x472;
    if ((ret = dal_longan_flowctrl_igrSystemPauseThresh_set(unit, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    if ((ret = dal_longan_flowctrl_igrSystemCongestThresh_set(unit, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }

    groupId = 0;
    thresh.highOn   = 0x64;
    thresh.highOff  = 0x5a;
    thresh.lowOn    = 0x19;
    thresh.lowOff   = 0xf;
    if ((ret = dal_longan_flowctrl_igrPauseThreshGroup_set(unit, groupId, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    if ((ret = dal_longan_flowctrl_igrCongestThreshGroup_set(unit, groupId, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    val = 0xc;
    if ((ret = reg_array_field_write(unit, LONGAN_FC_PORT_GUAR_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_THRf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }

    groupId = 2;
    thresh.highOn   = 0x64;
    thresh.highOff  = 0x5a;
    thresh.lowOn    = 0x19;
    thresh.lowOff   = 0xf;
    if ((ret = dal_longan_flowctrl_igrPauseThreshGroup_set(unit, groupId, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    if ((ret = dal_longan_flowctrl_igrCongestThreshGroup_set(unit, groupId, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    val = 0xc;
    if ((ret = reg_array_field_write(unit, LONGAN_FC_PORT_GUAR_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_THRf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }

    groupId = 2;
    for(port=24; port<28; port++)
    {
       if ((ret = dal_longan_flowctrl_portIgrPortThreshGroupSel_set(unit, port, groupId)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
    }


    drop_thresh.high    = 0x46;
    drop_thresh.low     = 0x3c;
    for(queue=0; queue<12; queue++)
    {
        groupId = 0;
        if ((ret = dal_longan_flowctrl_egrQueueDropThreshGroup_set(unit, groupId, queue, &drop_thresh)) != RT_ERR_OK)
        {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
        }
        groupId = 2;
        if ((ret = dal_longan_flowctrl_egrQueueDropThreshGroup_set(unit, groupId, queue, &drop_thresh)) != RT_ERR_OK)
        {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
        }
    }

    groupId = 2;
    for(port=24; port<28; port++)
    {
       if ((ret = dal_longan_flowctrl_portEgrDropThreshGroupSel_set(unit, port, groupId)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
    }

    drop_thresh.high    = 0x46;
    drop_thresh.low     = 0x3c;
    for(queue=0; queue<32; queue++)
    {
       if ((ret = dal_longan_flowctrl_egrCpuQueueDropThresh_set(unit, queue, &drop_thresh)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
    }

    val=0x46;
    if ((ret = reg_field_write(unit, LONGAN_FC_LB_PORT_Q_EGR_DROP_THRr, LONGAN_ONf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    val=0x3c;
    if ((ret = reg_field_write(unit, LONGAN_FC_LB_PORT_Q_EGR_DROP_THRr, LONGAN_OFFf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }

    for(groupId=0; groupId<2; groupId++)
    {
       val=0x10;
       if ((ret = reg_array_field_write(unit, LONGAN_FC_REPCT_Q_HSM_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_ONf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
       val=0x8;
       if ((ret = reg_array_field_write(unit, LONGAN_FC_REPCT_Q_HSM_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_OFFf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }

       val=0xFA;
       if ((ret = reg_array_field_write(unit, LONGAN_FC_REPCT_Q_HSA_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_ONf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
       val=0xF0;
       if ((ret = reg_array_field_write(unit, LONGAN_FC_REPCT_Q_HSA_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_OFFf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
    }

    groupId =1;
    thresh.highOn   = (pCascadeCfg->e2e_fc_en==ENABLED && pCascadeCfg->e2e_fc_normalPort_en==ENABLED) ? 0x3ad : 0x3ad/2;
    thresh.highOff  = (pCascadeCfg->e2e_fc_en==ENABLED && pCascadeCfg->e2e_fc_normalPort_en==ENABLED) ? 0x35f : 0x35f/2;
    thresh.lowOn    = (pCascadeCfg->e2e_fc_en==ENABLED && pCascadeCfg->e2e_fc_normalPort_en==ENABLED) ? 0x32b : 0x32b/2;
    thresh.lowOff   = (pCascadeCfg->e2e_fc_en==ENABLED && pCascadeCfg->e2e_fc_normalPort_en==ENABLED) ? 0x2dd : 0x2dd/2;
    if ((ret = dal_longan_flowctrl_igrPauseThreshGroup_set(unit, groupId, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    if ((ret = dal_longan_flowctrl_igrCongestThreshGroup_set(unit, groupId, &thresh)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    val = 0xc;
    if ((ret = reg_array_field_write(unit, LONGAN_FC_PORT_GUAR_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_THRf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }

    groupId = 1;
    for(queue=0; queue<12; queue++)
    {
       drop_thresh.high    = 0x2bc;
       drop_thresh.low     = 0x258;
       if ((ret = dal_longan_flowctrl_egrQueueDropThreshGroup_set(unit, groupId, queue, &drop_thresh)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
    }

    groupId = 1;
    for(port=24; port<28; port++)
    {
       if(!(cascadePortmask.bits[0] & (1<<port)))
           continue;

       if ((ret = dal_longan_flowctrl_portIgrPortThreshGroupSel_set(unit, port, groupId)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }

        if ((ret = dal_longan_flowctrl_portEgrDropThreshGroupSel_set(unit, port, groupId)) != RT_ERR_OK)
        {
            STACK_SEM_UNLOCK(unit);
            RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
            return ret;
        }
    }


    for(groupId=0; groupId<2; groupId++)
    {
       val = 0x19;
       if ((ret = reg_array_field_write(unit, LONGAN_ETE_FC_ON_REMOTE_PORT_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_ONf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
       if ((ret = reg_array_field_write(unit, LONGAN_ETE_FC_OFF_REMOTE_PORT_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_ONf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
       val = 0xf;
       if ((ret = reg_array_field_write(unit, LONGAN_ETE_FC_ON_REMOTE_PORT_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_OFFf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
       if ((ret = reg_array_field_write(unit, LONGAN_ETE_FC_OFF_REMOTE_PORT_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_OFFf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
       val = 0xc;
       if ((ret = reg_array_field_write(unit, LONGAN_ETE_FC_REMOTE_PORT_GUAR_THRr, REG_ARRAY_INDEX_NONE, groupId, LONGAN_THRf, &val)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
    }

    groupId = 1;
    for(port=24; port<28; port++)
    {
       if ((ret = reg_array_field_write(unit, LONGAN_ETE_FC_REMOTE_PORT_THR_SET_SELr, port, REG_ARRAY_INDEX_NONE, LONGAN_IDXf, &groupId)) != RT_ERR_OK)
       {
           STACK_SEM_UNLOCK(unit);
           RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
           return ret;
       }
    }

    val = 0xfac;
    if ((ret = reg_field_write(unit, LONGAN_ETE_FC_CASCADE_PORT_DROP_THRr, LONGAN_ONf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }
    val = 0xf90;
    if ((ret = reg_field_write(unit, LONGAN_ETE_FC_CASCADE_PORT_DROP_THRr, LONGAN_OFFf, &val)) != RT_ERR_OK)
    {
       STACK_SEM_UNLOCK(unit);
       RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
       return ret;
    }



    val = 0;
    rt_util_upinkPort_mask2Reg(unit, &cascadePortmask, &val);
    if ((ret = reg_field_write(unit, LONGAN_ETE_FC_CTRLr, LONGAN_CASCADE_PORTMASKf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    val = pCascadeCfg->e2e_fc_en==ENABLED ? 1 : 0;
    if ((ret = reg_field_write(unit, LONGAN_ETE_FC_CTRLr, LONGAN_ENf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    val = pCascadeCfg->e2e_fc_normalPort_en==ENABLED ? 1 : 0;
    if ((ret = reg_field_write(unit, LONGAN_ETE_FC_CTRLr, LONGAN_NRM_PORT_ETE_FC_ENf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    val = pCascadeCfg->e2e_ntfy_en==ENABLED ? 1 : 0;
    if ((ret = reg_field_write(unit, LONGAN_L2_NTFY_PKT_REMOTEL_THRr, LONGAN_REMOTE_BACK_PRESSf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }
    val = 0xf;
    if ((ret = reg_field_write(unit, LONGAN_L2_NTFY_PKT_REMOTEL_THRr, LONGAN_ONf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }
    val = 0x8;
    if ((ret = reg_field_write(unit, LONGAN_L2_NTFY_PKT_REMOTEL_THRr, LONGAN_OFFf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    /*cascade Master/Slave Mode configuration*/
    val =(unit==masterUnitId ? 0: 1);
    if ((ret = reg_field_write(unit, LONGAN_STK_CASCADE_CTRLr, LONGAN_CASCADE_SLAVE_MODEf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }
    val = masterUnitId;
    if ((ret = reg_field_write(unit, LONGAN_STK_CASCADE_CTRLr, LONGAN_CASCADE_MASTER_IDf, &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

#ifndef __BOOTLOADER__
    /* Enable Ext CPU Interrupt*/
    if(unit == HWP_CASCADE_SLAVE_UNIT_ID())
    {
        val = 1;
        if ((ret = reg_field_write(unit, LONGAN_IMR_GLBr, LONGAN_IMR_EXT_CPUf, &val)) != RT_ERR_OK)
        {
            STACK_SEM_UNLOCK(unit);
            RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
            return ret;
        }
        val = 3;
        if ((ret = reg_field_write(unit, LONGAN_ISR_SW_INT_MODEr, LONGAN_SW_INT_MODEf, &val)) != RT_ERR_OK)
        {
            STACK_SEM_UNLOCK(unit);
            RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
            return ret;
        }
        val = 3;
        if ((ret = reg_field_write(unit, LONGAN_ISR_SW_INT_MODEr, LONGAN_SW_INT_PULSE_INTERVALf, &val)) != RT_ERR_OK)
        {
            STACK_SEM_UNLOCK(unit);
            RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
            return ret;
        }
    }
#endif

    STACK_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}



/* Module Name    : Trunk                    */
/* Sub-module Name: User configuration stack */

/* Function Name:
 *      dal_longan_stack_port_get
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
 * Note:
 *      None
 */
int32
dal_longan_stack_port_get (uint32 unit, rtk_portmask_t *pStkPorts)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d", unit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK((NULL == pStkPorts), RT_ERR_NULL_POINTER);

    STACK_SEM_LOCK(unit);

    /* get entry from CHIP*/
    if ((ret = reg_field_read(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_STK_PORT_SELf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);
    osal_memset(pStkPorts, 0, sizeof(*pStkPorts));
    rt_util_upinkPort_reg2Mask(unit, &val, pStkPorts);

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "pStkPorts->bits=0x%x", pStkPorts->bits[0]);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_port_get */

/* Function Name:
 *      dal_longan_stack_port_set
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
 * Note:
 *      In 9300, only port 24~27 can be configured as stacking ports.
 */
int32
dal_longan_stack_port_set (uint32 unit, rtk_portmask_t *pStkPorts)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d pStkPorts->bits=0x%x", unit,
        pStkPorts->bits[0]);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK(!HWP_PMSK_EXIST_ATTRI(unit, pStkPorts, HWP_STACK|HWP_CASCADE, HWP_OPERATION_OR), RT_ERR_PORT_MASK);

    STACK_SEM_LOCK(unit);

    val = 0;
    rt_util_upinkPort_mask2Reg(unit, pStkPorts, &val);

    /* get entry from CHIP*/
    if ((ret = reg_field_write(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_STK_PORT_SELf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_port_set */

/* Function Name:
 *      dal_longan_stack_unit_get
 * Description:
 *      Get the switch unit ID from the specified device.
 * Input:
 *      unit                   - unit id
 * Output:
 *      pMyUnit              - pointer buffer of uind ID of the switch
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
dal_longan_stack_unit_get(uint32 unit, uint32 *pMyUnit)
{
    int32 ret;
    uint32  val = 0;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d", unit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK((NULL == pMyUnit), RT_ERR_NULL_POINTER);

    STACK_SEM_LOCK(unit);

    /* get entry from CHIP*/
    if ((ret = reg_field_read(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_MY_UNIT_IDf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);
    *pMyUnit = val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "*pMyUnit = ", *pMyUnit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_unit_get */

/* Function Name:
 *      dal_longan_stack_unit_set
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
 * Note:
 *      (1) 9300 supports 16 stacked units, thus myUnit ranges from 0 to 15.
 */
int32
dal_longan_stack_unit_set(uint32 unit, uint32 myUnit)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d myUnit=%d", unit, myUnit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK(myUnit >= RTK_MAX_NUM_OF_UNIT, RT_ERR_UNIT_ID);

    STACK_SEM_LOCK(unit);

    val = myUnit;

    /* get entry from CHIP*/
    if ((ret = reg_field_write(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_MY_UNIT_IDf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_unit_set */

/* Function Name:
 *      dal_longan_stack_masterUnit_get
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
 * Note:
 *      None
 */
int32
dal_longan_stack_masterUnit_get(uint32 unit, uint32 *pMasterUnit)
{
    int32 ret;
    uint32  val = 0;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d", unit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK((NULL == pMasterUnit), RT_ERR_NULL_POINTER);

    STACK_SEM_LOCK(unit);

    /* get entry from CHIP*/
    if ((ret = reg_field_read(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_MASTER_UNIT_IDf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);
    *pMasterUnit = val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "*pMasterUnit = ", *pMasterUnit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_masterUnit_get */

/* Function Name:
 *      dal_longan_stack_masterUnit_set
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
 * Note:
 *      (1) 9300 supports 16 stacked units, thus masterUnit ranges from 0 to 15.
 */
int32
dal_longan_stack_masterUnit_set(uint32 unit, uint32 masterUnit)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d masterUnit=%d", unit, masterUnit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK(masterUnit >= RTK_MAX_NUM_OF_UNIT, RT_ERR_UNIT_ID);

    STACK_SEM_LOCK(unit);

    val = masterUnit;

    /* get entry from CHIP*/
    if ((ret = reg_field_write(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_MASTER_UNIT_IDf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_masterUnit_set */

/* Function Name:
 *      dal_longan_stack_loopGuard_get
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
 * Note:
 *      None
 */
int32
dal_longan_stack_loopGuard_get(uint32 unit, rtk_enable_t *pEnable)
{
    int32 ret;
    uint32  val = 0;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d", unit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    STACK_SEM_LOCK(unit);

    /* get entry from CHIP*/
    if ((ret = reg_field_read(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_DROP_MY_UNITf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);
    *pEnable = val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "*pEnable = ", *pEnable);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_dropLoopPacket_get */


/* Function Name:
 *      dal_longan_stack_loopGuard_set
 * Description:
 *      Set the enable status of dropping packets with source unit ID the same as the unit ID of the switch to the specified device.
 * Input:
 *      unit          - unit id
 *      enable - enable state of loop guard mechanism
 * Output:
 *      None
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
dal_longan_stack_loopGuard_set(uint32 unit, rtk_enable_t enable)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d enable=%d", unit, enable);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */

    STACK_SEM_LOCK(unit);
    RT_PARAM_CHK(enable >= RTK_ENABLE_END, RT_ERR_INPUT);

    val = enable;

    /* get entry from CHIP*/
    if ((ret = reg_field_write(unit,
                          LONGAN_STK_GLB_CTRLr,
                          LONGAN_DROP_MY_UNITf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_dropLoopPacket_set */

/* Function Name:
 *      dal_longan_stack_unitPortMap_get
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
 * Note:
 *      None
 */
int32
dal_longan_stack_unitPortMap_get (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d, dstUnit=%d",
           unit, dstUnit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK(dstUnit >= RTK_MAX_NUM_OF_UNIT, RT_ERR_UNIT_ID);
    RT_PARAM_CHK((NULL == pStkPorts), RT_ERR_NULL_POINTER);

    STACK_SEM_LOCK(unit);
    /* get entry from CHIP*/
    if ((ret = reg_array_field_read(unit,
                          LONGAN_STK_UNIT_PORT_MAP_CTRLr,
                          REG_ARRAY_INDEX_NONE,
                          dstUnit,
                          LONGAN_UNIT_PORT_MAPf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    osal_memset(pStkPorts, 0, sizeof(*pStkPorts));
    rt_util_upinkPort_reg2Mask(unit, &val, pStkPorts);

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "pStkPorts->bits=0x%x", pStkPorts->bits[0]);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_unitPortMap_get */


/* Function Name:
 *      dal_longan_stack_unitPortMap_set _set
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
 * Note:
 *      (1) Stacking ports in 9300 ranges from 24 to 27.
 */
int32
dal_longan_stack_unitPortMap_set (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d, dstUnit=%d, pStkPorts->bits=0x%x",
           unit, dstUnit, pStkPorts->bits[0]);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK(dstUnit >= RTK_MAX_NUM_OF_UNIT, RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_PMSK_EXIST_ATTRI(unit, pStkPorts, HWP_STACK|HWP_CASCADE, HWP_OPERATION_OR), RT_ERR_PORT_MASK);

    val = 0;
    rt_util_upinkPort_mask2Reg(unit, pStkPorts, &val);

    STACK_SEM_LOCK(unit);

    /* set entry to CHIP*/
    if ((ret = reg_array_field_write(unit,
                          LONGAN_STK_UNIT_PORT_MAP_CTRLr,
                          REG_ARRAY_INDEX_NONE,
                          dstUnit,
                          LONGAN_UNIT_PORT_MAPf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_unitPortMap_set */

/*
 * Function Declaration
 *      dal_longan_stack_nonUcastBlockPort_get
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
 * Note:
 *      None
 */
int32
dal_longan_stack_nonUcastBlockPort_get (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d", unit);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK((NULL == pBlockStkPorts), RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(srcUnit>= RTK_MAX_NUM_OF_UNIT, RT_ERR_UNIT_ID);

    STACK_SEM_LOCK(unit);
    /* get entry from CHIP*/
    if ((ret = reg_array_field_read(unit,
                          LONGAN_STK_NON_UNICAST_BLOCK_CTRLr,
                          REG_ARRAY_INDEX_NONE,
                          srcUnit,
                          LONGAN_NON_UNICAST_BLOCK_PMf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    osal_memset(pBlockStkPorts, 0, sizeof(*pBlockStkPorts));
    rt_util_upinkPort_reg2Mask(unit, &val, pBlockStkPorts);

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "pBlockStkPorts->bits=0x%x", pBlockStkPorts->bits[0]);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_nonUcastBlockPort_get */


/* Function Name:
 *      dal_longan_stack_nonUcastBlockPort_set
 * Description:
 *      Set the stacking ports that would block ingress and egress non-ucast packets to the specified device.
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
 * Note:
 *      (1) Blockable stacking ports in 9300 ranges from 24 to 27.
 */
int32
dal_longan_stack_nonUcastBlockPort_set (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts)
{
    int32 ret;
    uint32  val;

    RT_LOG(LOG_DEBUG, (MOD_DAL|MOD_STACK), "unit=%d, pBlockStkPorts->bits=0x%x", unit, pBlockStkPorts->bits[0]);

    /* check Init status */
    RT_INIT_CHK(stack_init[unit]);

    /* parameter check */
    RT_PARAM_CHK(srcUnit>= RTK_MAX_NUM_OF_UNIT, RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_PMSK_EXIST_ATTRI(unit, pBlockStkPorts, HWP_STACK|HWP_CASCADE, HWP_OPERATION_OR), RT_ERR_PORT_MASK);

    val = 0;
    rt_util_upinkPort_mask2Reg(unit, pBlockStkPorts, &val);

    STACK_SEM_LOCK(unit);
    /* get entry from CHIP*/
    if ((ret = reg_array_field_write(unit,
                          LONGAN_STK_NON_UNICAST_BLOCK_CTRLr,
                          REG_ARRAY_INDEX_NONE,
                          srcUnit,
                          LONGAN_NON_UNICAST_BLOCK_PMf,
                          &val)) != RT_ERR_OK)
    {
        STACK_SEM_UNLOCK(unit);
        RT_ERR(ret, (MOD_DAL|MOD_STACK), "");
        return ret;
    }

    STACK_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}   /* end of dal_longan_stack_nonUcastBlockPort_set */

/* Function Name:
 *      _dal_longan_stack_init_config
 * Description:
 *      Initialize default configuration for stack module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      Must initialize stack module before calling this API
 */
int32
_dal_longan_stack_init_config(uint32 unit)
{
    int32   ret;
    uint32 idx;
    rtk_portmask_t stkPorts;

    osal_memset(&stkPorts, 0, sizeof(rtk_portmask_t));
    stkPorts.bits[0] = RTK_DEFAULT_STACK_PORTMASK_0;
    if ((ret = dal_longan_stack_port_set (unit, &stkPorts)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_stack_unit_set (unit, unit)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_stack_masterUnit_set (unit, HWP_MY_UNIT_ID())) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    if ((ret = dal_longan_stack_loopGuard_set (unit, RTK_DEFAULT_STACK_LOOP_GUARD)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
        return ret;
    }

    osal_memset(&stkPorts, 0, sizeof(rtk_portmask_t));
    stkPorts.bits[0] = RTK_DEFAULT_STACK_PORTMASK_0;
    for(idx = 0; idx < RTK_MAX_NUM_OF_UNIT; idx ++)
    {
        if ((ret = dal_longan_stack_nonUcastBlockPort_set (unit, idx, &stkPorts)) != RT_ERR_OK)
        {
            RT_ERR(ret, (MOD_STACK|MOD_DAL), "");
            return ret;
        }
    }

    return RT_ERR_OK;
} /* end of _dal_longan_trunk_init_config */

