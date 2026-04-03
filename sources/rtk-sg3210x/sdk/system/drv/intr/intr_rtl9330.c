/* Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 93380 $
 * $Date: 2018-11-08 17:49:37 +0800 (Thu, 08 Nov 2018) $
 *
 * Purpose : Definition of Interrupt control API
 *
 * Feature : The file includes the following modules
 *           (1) Interrupt
 *
 */

 /*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/debug/rt_log.h>
#include <osal/lib.h>
#include <osal/isr.h>
#include <drv/intr/intr.h>
#include <private/drv/intr/intr_mapper.h>
#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>
#include <hwp/hw_profile.h>
#include <private/drv/swcore/swcore_rtl9330.h>
#include <private/drv/intr/intr_rtl9330.h>
#include <soc/socdef/rtl9330_soc_reg.h>
#include <osal/sem.h>
#include <dev_config.h>
#include <osal/spl.h>


/*
 * Symbol Definition
 */
#ifdef __KERNEL__
#define INTR_SEM_LOCK(unit)    \
do {\
    osal_spl_spin_lock_irqsave(intr_lock, &flags); \
} while(0)
#define INTR_SEM_UNLOCK(unit)   \
do {\
    osal_spl_spin_unlock_irqrestore(intr_lock, &flags); \
} while(0)
#else
#define INTR_SEM_LOCK(unit)    \
do {\
    if (flags); /* avoid compile warning */ \
    if (osal_sem_mutex_take(intr_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_BSP), "semaphore lock failed");\
        return RT_ERR_SEM_LOCK_FAILED;\
    }\
} while(0)
#define INTR_SEM_UNLOCK(unit)   \
do {\
    if (osal_sem_mutex_give(intr_sem[unit]) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_BSP), "semaphore unlock failed");\
        return RT_ERR_SEM_UNLOCK_FAILED;\
    }\
} while(0)
#endif


#define     RTL9330_IMR0_IMR0_TDU_OFFSET      RTL9330_IMR0_IMR0_TDU1_OFFSET
#define     RTL9330_IMR0_IMR0_TDU_MASK        (0x1F << RTL9330_IMR0_IMR0_TDU_OFFSET)

#define     RTL9330_IMR0_IMR0_TOK_OFFSET      RTL9330_IMR0_IMR0_TOK1_OFFSET
#define     RTL9330_IMR0_IMR0_TOK_MASK        (0x1F << RTL9330_IMR0_IMR0_TOK_OFFSET)

#define     RTL9330_ISR1_ISR_TDU_OFFSET       RTL9330_ISR1_ISR_TDU1_OFFSET
#define     RTL9330_ISR1_ISR_TDU_MASK         (0x1F << RTL9330_ISR1_ISR_TDU1_OFFSET)

#define     RTL9330_ISR1_ISR_TOK_OFFSET       RTL9330_ISR1_ISR_TOK1_OFFSET
#define     RTL9330_ISR1_ISR_TOK_MASK         (0x1F << RTL9330_ISR1_ISR_TOK_OFFSET)


/*
 * Data Declaration
 */
static uint32   intr_init_status[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};

#ifdef __KERNEL__
static osal_spinlock_t     intr_lock = 0;
#else
static osal_mutex_t        intr_sem[RTK_MAX_NUM_OF_UNIT] = { 0 };
#endif


drv_isr_idRegBitMap_t rtl9330_isr_glb_reg_id_map [] = {
    { RTL9330_IGLB_SRC_STS_IGLB_LINK_CHG_MASK,   INTR_ISR_PORT_LINK_CHG },
    /*{ RTL9330_ISR_GLB_SRC_STS_ISR_GLB_EXT_GPIO_MASK,   INTR_ISR_EXT_GPIO },
    { RTL9330_ISR_GLB_SRC_STS_ISR_GLB_OAM_DYGASP_MASK, INTR_ISR_OAM_DYGASP },
    { RTL9330_ISR_GLB_SRC_STS_ISR_GLB_CCM_MASK,        INTR_ISR_CCM },*/
};

/* Register: RTL9330_INT_MS_ADDR */
drv_isr_idRegBitMap_t rtl9330_isr_gmac_INT_MS_id_map [] = {
    { RTL9330_INT_MS_ISR_ROK_MASK,     INTR_ISR_GMAC_RXDONE },
    { RTL9330_INT_MS_ISR_RDU_MASK,     INTR_ISR_GMAC_RXRUNOUT },
    { RTL9330_INT_MS_ISR_LinkChg_MASK, INTR_ISR_GMAC_LINKCHG },
};

/* Register: RTL9330_ISR1_ADDR */
drv_isr_idRegBitMap_t rtl9330_isr_gmac_ISR1_id_map [] = {
    { RTL9330_ISR1_ISR_TOK_MASK,     INTR_ISR_GMAC_TXDONE },
    { RTL9330_ISR1_ISR_TDU_MASK,     INTR_ISR_GMAC_TXRUNOUT },
};

extern uint32      drv_intr_imrExtGpio0Data[];
extern uint32      drv_intr_imrExtGpio1Data[];

/*
 * Macro Declaration
 */


/*
 * Function Declaration
 */

int32
rtl9330_intr_init(uint32 unit)
{
    uint32  data = 0;

    RT_INIT_REENTRY_CHK(intr_init_status[unit]);


#ifdef __KERNEL__
    if (intr_lock == 0)
    {
        intr_lock = osal_spl_spin_lock_create();
        if (intr_lock == 0)
        {
            RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_BSP), "intr lock create fail");
            return RT_ERR_FAILED;            
        }
    }
#else
    if (intr_sem[unit] == 0)
    {
        intr_sem[unit] = osal_sem_mutex_create();
        if (0 == intr_sem[unit])
        {
            RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_BSP), "intr semaphore create fail");
            return RT_ERR_FAILED;
        }
    }
#endif


    /* Init TC4 timer */
    ioal_soc_mem32_read(unit, RTL9330_TC4CTRL_ADDR, &data);
    data |= (1 << RTL9330_TC4CTRL_TC4MODE_OFFSET);
    ioal_soc_mem32_write(unit, RTL9330_TC4CTRL_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9330_TC4CTRL_ADDR, &data);
    data |= (2 << RTL9330_TC4CTRL_TC4DIVFACTOR_OFFSET);
    ioal_soc_mem32_write(unit, RTL9330_TC4CTRL_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9330_TC4DATA_ADDR, &data);
    data |= (0x2 << RTL9330_TC4DATA_TC1DATA_OFFSET);
    ioal_soc_mem32_write(unit, RTL9330_TC4DATA_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9330_TC4INTR_ADDR, &data);
    data |= (1 << RTL9330_TC4INTR_TC4IE_OFFSET);
    ioal_soc_mem32_write(unit, RTL9330_TC4INTR_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9330_TC4CTRL_ADDR, &data);
    data |= (1 << RTL9330_TC4CTRL_TC4EN_OFFSET);
    ioal_soc_mem32_write(unit, RTL9330_TC4CTRL_ADDR, data);

    /* Init delayed interrupt with TC4 for link change interrupt */
    ioal_soc_mem32_read(unit, RTL9330_TC4_DLY_INTR_ADDR, &data);
    data |= (0x3 << RTL9330_TC4_DLY_INTR_DELAYED_IP_SEL_OFFSET);
    ioal_soc_mem32_write(unit, RTL9330_TC4_DLY_INTR_ADDR, data);

    intr_init_status[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}

int32
rtl9330_intr_swcoreIsrSts_get(uint32 unit, drv_intr_isrBitmap_t *pisr_sts)
{
    uint32      reg_data = 0;
    int32       i, size;
    unsigned long       flags = 0;

    INTR_ISR_BITMAP_RESET(*pisr_sts);
    INTR_SEM_LOCK(unit);

    ioal_mem32_read(unit, RTL9330_IGLB_SRC_STS_ADDR, &reg_data);
    size = sizeof(rtl9330_isr_glb_reg_id_map)/sizeof(drv_isr_idRegBitMap_t);
    for (i = 0; i < size; i++)
    {
        if (reg_data & rtl9330_isr_glb_reg_id_map[i].reg_bit_mask)
        {
            INTR_ISR_BITMAP_SET(*pisr_sts, rtl9330_isr_glb_reg_id_map[i].isr_id);
        }
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}



int32
rtl9330_intr_gmacIsrSts_get(uint32 unit, drv_intr_isrBitmap_t *pisr_sts)
{
    uint32      reg_data;
    int32       i, size;
    unsigned long       flags = 0;

    INTR_ISR_BITMAP_RESET(*pisr_sts);
    reg_data = 0;
    INTR_SEM_LOCK(unit);

    ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
    RT_LOG(LOG_DEBUG, MOD_BSP, "%s:%u INT_MS=%x\n", __FUNCTION__, __LINE__, reg_data);
    size = sizeof(rtl9330_isr_gmac_INT_MS_id_map)/sizeof(drv_isr_idRegBitMap_t);
    for (i = 0; i < size; i++)
    {
        if (reg_data & rtl9330_isr_gmac_INT_MS_id_map[i].reg_bit_mask)
        {
            INTR_ISR_BITMAP_SET(*pisr_sts, rtl9330_isr_gmac_INT_MS_id_map[i].isr_id);
        }
    }

    reg_data = 0;
    ioal_soc_mem32_read(unit, RTL9330_ISR1_ADDR, &reg_data);
    RT_LOG(LOG_DEBUG, MOD_BSP, "%s:%u ISR1=%x\n", __FUNCTION__, __LINE__, reg_data);
    size = sizeof(rtl9330_isr_gmac_ISR1_id_map)/sizeof(drv_isr_idRegBitMap_t);
    for (i = 0; i < size; i++)
    {
        if (reg_data & rtl9330_isr_gmac_ISR1_id_map[i].reg_bit_mask)
        {
            INTR_ISR_BITMAP_SET(*pisr_sts, rtl9330_isr_gmac_ISR1_id_map[i].isr_id);
        }
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}

int32
rtl9330_intr_isrStsByDevId_get(uint32 unit, int32 dev_type, drv_intr_isrBitmap_t *pisr_sts)
{
    switch (dev_type)
    {
        case RTK_DEV_SWCORE:
            return rtl9330_intr_swcoreIsrSts_get(unit, pisr_sts);
        case RTK_DEV_LINK_CHG:
            INTR_ISR_BITMAP_SET(*pisr_sts, INTR_ISR_PORT_LINK_CHG);
            return RT_ERR_OK;
        case RTK_DEV_OAM_DYGASP:
            INTR_ISR_BITMAP_SET(*pisr_sts, INTR_ISR_OAM_DYGASP);
            return RT_ERR_OK;
        case RTK_DEV_INT_GMAC_INTR:
            return rtl9330_intr_gmacIsrSts_get(unit, pisr_sts);
        default:
            break;
    }

    return RT_ERR_FAILED;
}


int32
rtl9330_intr_swcoreImrEnable_set(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t enable)
{
    uint32      val;
    uint32      reg_data;
    unsigned long       flags = 0;

    INTR_SEM_LOCK(unit);

    RT_LOG(LOG_DEBUG, MOD_BSP, "%s:%u:unit=%u isrId=%u en=%u\n", __FUNCTION__, __LINE__, unit, isrId, enable);
    if (isrId == INTR_ISR_OAM_DYGASP)
    {
        val = (enable == DISABLED) ? 0 : 1;
        ioal_mem32_read(unit, RTL9330_IMR_MISC_ADDR, &reg_data);
        reg_data = (reg_data & ~RTL9330_IMR_MISC_IMR_OAM_DYGASP_MASK) | ((val << RTL9330_IMR_MISC_IMR_OAM_DYGASP_OFFSET) & RTL9330_IMR_MISC_IMR_OAM_DYGASP_MASK);
        ioal_mem32_write(unit, RTL9330_IMR_MISC_ADDR, reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    /*
    else if (isrId == INTR_ISR_CCM)
    {
        val = (enable == DISABLED) ? 0x0 : 0xFFFF;
        ioal_mem32_read(unit, RTL9330_IMR_MISC_ADDR, &reg_data);
        reg_data = (reg_data & ~RTL9330_IMR_MISC_IMR_CCM_MASK) | ((val << RTL9330_IMR_MISC_IMR_CCM_OFFSET) & RTL9330_IMR_MISC_IMR_CCM_MASK);
        ioal_mem32_write(unit, RTL9330_IMR_MISC_ADDR, reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PORT_LINK_CHG)
    {
        if (enable == DISABLED)
        {
            ioal_mem32_write(unit, RTL9330_IMR_PORT_LINK_STS_CHG_ADDR(0), 0x0);
        }
        else
        {
            ioal_mem32_write(unit, RTL9330_IMR_PORT_LINK_STS_CHG_ADDR(0), 0x0FFFFFFF);
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_EXT_GPIO)
    {
        if (enable == DISABLED)
        {
            ioal_mem32_write(unit, RTL9330_IMR_EXT_GPIO_ADDR(0), 0x0);
            ioal_mem32_write(unit, RTL9330_IMR_EXT_GPIO_ADDR(32), 0x0);
        }
        else
        {
            ioal_mem32_write(unit, RTL9330_IMR_EXT_GPIO_ADDR(0), drv_intr_imrExtGpio0Data[unit]);
            ioal_mem32_write(unit, RTL9330_IMR_EXT_GPIO_ADDR(32), drv_intr_imrExtGpio1Data[unit]);
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    */
    else if (isrId == INTR_ISR_GMAC_RXDONE)
    {
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9330_INT_MS_IMR_ROK_MASK) & 0xffff0000);
        reg_data |= ((val << RTL9330_INT_MS_IMR_ROK_OFFSET) & RTL9330_INT_MS_IMR_ROK_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_RXRUNOUT)
    {
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9330_INT_MS_IMR_RDU_MASK) & 0xffff0000);
        reg_data |= ((val << RTL9330_INT_MS_IMR_RDU_OFFSET) & RTL9330_INT_MS_IMR_RDU_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXDONE)
    {
        /* TX RING 1 */
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9330_INT_MS_IMR_TOK_MASK) & 0xffff0000);
        reg_data |= (((val & 0x1) << RTL9330_INT_MS_IMR_TOK_OFFSET) & RTL9330_INT_MS_IMR_TOK_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_field_write(unit, RTL9330_IMR0_ADDR, RTL9330_IMR0_IMR0_TOK_OFFSET, RTL9330_IMR0_IMR0_TOK_MASK, 0x1F);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXRUNOUT)
    {
        /* TX RING 1 */
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9330_INT_MS_IMR_TDU_MASK) & 0xffff0000);
        reg_data |= (((val & 0x1) << RTL9330_INT_MS_IMR_TDU_OFFSET) & RTL9330_INT_MS_IMR_TDU_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_field_write(unit, RTL9330_IMR0_ADDR, RTL9330_IMR0_IMR0_TDU_OFFSET, RTL9330_IMR0_IMR0_TDU_MASK, 0x1F);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_LINKCHG)
    {
        val = (enable == DISABLED) ? 0 : 1;
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9330_INT_MS_IMR_LinkChg_MASK) & 0xffff0000);
        reg_data |= (((val & 0x1) << RTL9330_INT_MS_IMR_LinkChg_OFFSET) & RTL9330_INT_MS_IMR_LinkChg_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;

}

int32
rtl9330_intr_swcoreSts_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    uint32      reg_data = 0;
    unsigned long       flags = 0;

    INTR_SEM_LOCK(unit);
    osal_memset(pFunc_sts, 0, sizeof(drv_intr_data_t));
    if (isrId == INTR_ISR_OAM_DYGASP)
    {
        ioal_mem32_read(unit, RTL9330_ISR_MISC_ADDR, &reg_data);
        ioal_mem32_write(unit, RTL9330_ISR_MISC_ADDR, RTL9330_ISR_MISC_ISR_OAM_DYGASP_MASK);
        pFunc_sts->u.oam_dygsp = (reg_data & RTL9330_ISR_MISC_ISR_OAM_DYGASP_MASK) ? 1 : 0;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    /*
    else if (isrId == INTR_ISR_CCM)
    {
        ioal_mem32_read(unit, RTL9330_ISR_MISC_ADDR, &reg_data);
        ioal_mem32_write(unit, RTL9330_ISR_MISC_ADDR, RTL9330_ISR_MISC_ISR_CCM_MASK);
        pFunc_sts->u.ccm = (reg_data & RTL9330_ISR_MISC_ISR_CCM_MASK) >> RTL9330_ISR_MISC_ISR_CCM_OFFSET;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PORT_LINK_CHG)
    {
        ioal_mem32_read(unit, RTL9330_ISR_PORT_LINK_STS_CHG_ADDR(0), &reg_data);
        RTK_PORTMASK_WORD_SET(pFunc_sts->u.portmask, 0, reg_data);
        ioal_mem32_write(unit, RTL9330_ISR_PORT_LINK_STS_CHG_ADDR(0), reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_EXT_GPIO)
    {
        ioal_mem32_read(unit, RTL9330_ISR_EXT_GPIO_ADDR(0), &reg_data);
        pFunc_sts->u.ext_gpio[0] = reg_data;
        ioal_mem32_write(unit, RTL9330_ISR_EXT_GPIO_ADDR(0), reg_data);

        ioal_mem32_read(unit, RTL9330_ISR_EXT_GPIO_ADDR(32), &reg_data);
        pFunc_sts->u.ext_gpio[1] = reg_data;
        ioal_mem32_write(unit, RTL9330_ISR_EXT_GPIO_ADDR(32), reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    */

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;

}

int32
_rtl9330_intr_gmacData_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    uint32      reg_data = 0, val;
    unsigned long       flags = 0;

    INTR_SEM_LOCK(unit);

    osal_memset(pFunc_sts, 0, sizeof(drv_intr_data_t));
    if (isrId == INTR_ISR_GMAC_RXDONE)
    {
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9330_INT_MS_ISR_ROK_MASK) >> RTL9330_INT_MS_ISR_ROK_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= ((val << RTL9330_INT_MS_ISR_ROK_OFFSET) & RTL9330_INT_MS_ISR_ROK_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        pFunc_sts->u.rx_done = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_RXRUNOUT)
    {
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9330_INT_MS_ISR_RDU_MASK) >> RTL9330_INT_MS_ISR_RDU_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= ((val << RTL9330_INT_MS_ISR_RDU_OFFSET) & RTL9330_INT_MS_ISR_RDU_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        pFunc_sts->u.rx_runout = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXDONE)
    {
        /* TX RING 1*/
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9330_INT_MS_ISR_TOK_MASK) >> RTL9330_INT_MS_ISR_TOK_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= (((val & 0x1) << RTL9330_INT_MS_ISR_TOK_OFFSET) & RTL9330_INT_MS_ISR_TOK_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_write(unit, RTL9330_ISR1_ADDR, (val << RTL9330_ISR1_ISR_TOK_OFFSET) & RTL9330_ISR1_ISR_TOK_MASK);
        pFunc_sts->u.tx_done = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXRUNOUT)
    {
        /* TX RING 1*/
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9330_INT_MS_ISR_TDU_MASK) >> RTL9330_INT_MS_ISR_TDU_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= (((val & 0x1) << RTL9330_INT_MS_ISR_TDU_OFFSET) & RTL9330_INT_MS_ISR_TDU_MASK);
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_write(unit, RTL9330_ISR1_ADDR, (val << RTL9330_ISR1_ISR_TDU_OFFSET) & RTL9330_ISR1_ISR_TDU_MASK);
        pFunc_sts->u.tx_runout = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_LINKCHG)
    {
        ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9330_INT_MS_ISR_LinkChg_MASK) >> RTL9330_INT_MS_ISR_LinkChg_OFFSET;
        reg_data &= 0xffff0000; /* keep [31:16] the IMR config fields */
        reg_data |= RTL9330_INT_MS_ISR_LinkChg_MASK; /* write 1 to clear */
        ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, reg_data);
        RTK_PORTMASK_PORT_SET(pFunc_sts->u.portmask, RTK_OOB_IDX_TO_PORT(0));

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;
}


int32
rtl9330_intr_data_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    if (_rtl9330_intr_gmacData_get(unit, isrId, pFunc_sts) == RT_ERR_OK)
        return RT_ERR_OK;

    if (rtl9330_intr_swcoreSts_get(unit, isrId, pFunc_sts) == RT_ERR_OK)
        return RT_ERR_OK;

    return RT_ERR_FAILED;
}




