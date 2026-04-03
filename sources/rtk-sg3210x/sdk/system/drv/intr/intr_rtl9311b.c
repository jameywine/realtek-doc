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
#include <private/drv/swcore/swcore_rtl9311b.h>
#include <private/drv/intr/intr_rtl9311b.h>
#include <soc/socdef/rtl9311b_soc_reg.h>
#include <osal/sem.h>
#include <dev_config.h>
#include <osal/spl.h>

#if !defined(__BOOTLOADER__)
#include <common/util/rt_util_time.h>
#include <common/rtcore/rtcore_init.h>
#include <rtcore/rtcore.h>
#endif
#if !defined(__KERNEL__)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

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


#define     RTL9311B_IMR0_IMR0_TDU_OFFSET      RTL9311B_IMR0_IMR0_TDU1_OFFSET
#define     RTL9311B_IMR0_IMR0_TDU_MASK        (0x1F << RTL9311B_IMR0_IMR0_TDU_OFFSET)

#define     RTL9311B_IMR0_IMR0_TOK_OFFSET      RTL9311B_IMR0_IMR0_TOK1_OFFSET
#define     RTL9311B_IMR0_IMR0_TOK_MASK        (0x1F << RTL9311B_IMR0_IMR0_TOK_OFFSET)

#define     RTL9311B_ISR1_ISR_TDU_OFFSET       RTL9311B_ISR1_ISR_TDU1_OFFSET
#define     RTL9311B_ISR1_ISR_TDU_MASK         (0x1F << RTL9311B_ISR1_ISR_TDU1_OFFSET)

#define     RTL9311B_ISR1_ISR_TOK_OFFSET       RTL9311B_ISR1_ISR_TOK1_OFFSET
#define     RTL9311B_ISR1_ISR_TOK_MASK         (0x1F << RTL9311B_ISR1_ISR_TOK_OFFSET)


/*
 * Data Declaration
 */
static uint32   intr_init_status[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};

#ifdef __KERNEL__
static osal_spinlock_t     intr_lock = 0;
#else
static osal_mutex_t        intr_sem[RTK_MAX_NUM_OF_UNIT] = { 0 };
#endif


drv_isr_idRegBitMap_t rtl9311b_isr_glb_reg_id_map [] = {
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_LINK_CHG_MASK,   INTR_ISR_PORT_LINK_CHG },
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_EXT_GPIO_MASK,   INTR_ISR_EXT_GPIO },
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_OAM_DYGASP_MASK, INTR_ISR_OAM_DYGASP },
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_CCM_MASK,        INTR_ISR_CCM },
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_ECC1_MASK,       INTR_ISR_ECC1BIT },
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_ECC2_MASK,       INTR_ISR_ECC2BIT },
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_PARITY_MASK,     INTR_ISR_PARITY },
    { RTL9311B_ISR_GLB_SRC_STS_ISR_GLB_REG_PARITY_MASK, INTR_ISR_PARITY_REG },
};

/* Register: RTL9311B_INT_MS_ADDR */
drv_isr_idRegBitMap_t rtl9311b_isr_gmac_INT_MS_id_map [] = {
    { RTL9311B_INT_MS_ISR_ROK_MASK,     INTR_ISR_GMAC_RXDONE },
    { RTL9311B_INT_MS_ISR_RDU_MASK,     INTR_ISR_GMAC_RXRUNOUT },
    { RTL9311B_INT_MS_ISR_LinkChg_MASK, INTR_ISR_GMAC_LINKCHG },
};

/* Register: RTL9311B_ISR1_ADDR */
drv_isr_idRegBitMap_t rtl9311b_isr_gmac_ISR1_id_map [] = {
    { RTL9311B_ISR1_ISR_TOK_MASK,     INTR_ISR_GMAC_TXDONE },
    { RTL9311B_ISR1_ISR_TDU_MASK,     INTR_ISR_GMAC_TXRUNOUT },
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
rtl9311b_intr_init(uint32 unit)
{
    uint32  data = 0;

    /* Init TC4 timer */
    ioal_soc_mem32_read(unit, RTL9311B_TC4CTRL_ADDR, &data);
    data |= (1 << RTL9311B_TC4CTRL_TC4MODE_OFFSET);
    ioal_soc_mem32_write(unit, RTL9311B_TC4CTRL_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9311B_TC4CTRL_ADDR, &data);
    data |= (2 << RTL9311B_TC4CTRL_TC4DIVFACTOR_OFFSET);
    ioal_soc_mem32_write(unit, RTL9311B_TC4CTRL_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9311B_TC4DATA_ADDR, &data);
    data |= (0x2 << RTL9311B_TC4DATA_TC1DATA_OFFSET);
    ioal_soc_mem32_write(unit, RTL9311B_TC4DATA_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9311B_TC4INTR_ADDR, &data);
    data |= (1 << RTL9311B_TC4INTR_TC4IE_OFFSET);
    ioal_soc_mem32_write(unit, RTL9311B_TC4INTR_ADDR, data);

    ioal_soc_mem32_read(unit, RTL9311B_TC4CTRL_ADDR, &data);
    data |= (1 << RTL9311B_TC4CTRL_TC4EN_OFFSET);
    ioal_soc_mem32_write(unit, RTL9311B_TC4CTRL_ADDR, data);

    /* Init delayed interrupt with TC4 for link change interrupt */
    ioal_soc_mem32_read(unit, RTL9311B_TC4_DLY_INTR_ADDR, &data);
    data |= (0x3 << RTL9311B_TC4_DLY_INTR_DELAYED_IP_SEL_OFFSET);
    ioal_soc_mem32_write(unit, RTL9311B_TC4_DLY_INTR_ADDR, data);


    return RT_ERR_OK;
}


int32
rtl9311b_intr_swcore_init(uint32 unit)
{
    uint32 val, reg_data, reg;
    int32 ret, i;
    uint32  ecc_sub_imrs[][2]  =
        { /* reg, enable-bits */
            { RTL9311B_IMR_ECC_ADDR,                     (RTL9311B_IMR_ECC_IMR_INT_ERR1_PARSER_MASK |
                                                          RTL9311B_IMR_ECC_IMR__INT_ERR2_PARSER_MASK) },
                                                          //RTL9311B_IMR_ECC_IMR_INT_ERR1_MAC_MASK | RTL9311B_IMR_ECC_IMR_INT_ERR2_MAC_MASK) }, /* do not enable it */
            { RTL9311B_ALE_GLB_STATUS_MASK_FOR_ECC_ADDR, (RTL9311B_ALE_GLB_STATUS_MASK_FOR_ECC_IMR_ALE_ECC_MASK |
                                                          RTL9311B_ALE_GLB_STATUS_MASK_FOR_ECC_IMR_ALE_ECC2_MASK) },
            { RTL9311B_ECC_INT_MASK_MASK_ADDR,           (RTL9311B_ECC_INT_MASK_MASK_MASK2_MASK |
                                                          RTL9311B_ECC_INT_MASK_MASK_MASK1_MASK) },
            { RTL9311B_ENCAP_ECC_MULTIBIT_INT_MASK_ADDR, (RTL9311B_ENCAP_ECC_MULTIBIT_INT_MASK_MASK2_MASK |
                                                          RTL9311B_ENCAP_ECC_MULTIBIT_INT_MASK_MASK1_MASK) },
            { RTL9311B_SER_NIC_IMR_ECC1BIT_ADDR,         (RTL9311B_SER_NIC_IMR_ECC1BIT_ECC1BIT_TX_MASK |
                                                          RTL9311B_SER_NIC_IMR_ECC1BIT_ECC1BIT_RX_MASK) },
            { RTL9311B_SER_NIC_IMR_ECC2BIT_ADDR,         (RTL9311B_SER_NIC_IMR_ECC2BIT_ECC2BIT_TX_MASK |
                                                          RTL9311B_SER_NIC_IMR_ECC2BIT_ECC2BIT_RX_MASK) },
            {RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_ADDR, (RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_IMR_INGR_ECC_ERR1_MASK |
                                                               RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_IMR_INGR_ECC_ERR2_MASK) },
        };


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


    /* enable ECC sub-IMRs, main IMR, IMR_ECC1 and IMR_ECC2 remain disabled */
    for (i = 0; i < (sizeof(ecc_sub_imrs)/(sizeof(uint32)*2)); i++)
    {
        reg = ecc_sub_imrs[i][0];
        val = ecc_sub_imrs[i][1];
        if ((ret = ioal_mem32_read(unit, reg, &reg_data)) != RT_ERR_OK)
        {
            RT_ERR(ret, (MOD_BSP), "init ECC sub-IMRs fail: 0x%X", reg);
            return ret;
        }
        reg_data |= val;
        if ((ret = ioal_mem32_write(unit, reg, reg_data)) != RT_ERR_OK)
        {
            RT_ERR(ret, (MOD_BSP), "init ECC sub-IMRs fail: 0x%X=0x%X", reg, reg_data);
            return ret;
        }
    }
    intr_init_status[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}


int32
rtl9311b_intr_swcoreIsrSts_get(uint32 unit, drv_intr_isrBitmap_t *pisr_sts)
{
    uint32      reg_data = 0;
    int32       i, size;
    unsigned long       flags = 0;

    INTR_ISR_BITMAP_RESET(*pisr_sts);
    INTR_SEM_LOCK(unit);

    ioal_mem32_read(unit, RTL9311B_ISR_GLB_SRC_STS_ADDR, &reg_data);
    size = sizeof(rtl9311b_isr_glb_reg_id_map)/sizeof(drv_isr_idRegBitMap_t);
    for (i = 0; i < size; i++)
    {
        if (reg_data & rtl9311b_isr_glb_reg_id_map[i].reg_bit_mask)
        {
            INTR_ISR_BITMAP_SET(*pisr_sts, rtl9311b_isr_glb_reg_id_map[i].isr_id);
        }
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}



#if defined(__KERNEL__) || defined(__BOOTLOADER__)
int32
rtl9311b_intr_gmacIsrSts_get(uint32 unit, drv_intr_isrBitmap_t *pisr_sts)
{
    uint32      reg_data;
    int32       i, size;
    unsigned long       flags = 0;

    if (!(HWP_CPU_EMBEDDED() && (unit == HWP_MY_UNIT_ID())))
    {
        RT_LOG(LOG_DEBUG, MOD_BSP, "%s:%u Embeded=%u my_unit=%u unit=%u\n", __FUNCTION__, __LINE__, HWP_CPU_EMBEDDED(), HWP_MY_UNIT_ID(), unit);
        return RT_ERR_FAILED;
    }

    INTR_ISR_BITMAP_RESET(*pisr_sts);
    reg_data = 0;
    INTR_SEM_LOCK(unit);

    ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
    RT_LOG(LOG_DEBUG, MOD_BSP, "%s:%u INT_MS=%x\n", __FUNCTION__, __LINE__, reg_data);
    size = sizeof(rtl9311b_isr_gmac_INT_MS_id_map)/sizeof(drv_isr_idRegBitMap_t);
    for (i = 0; i < size; i++)
    {
        if (reg_data & rtl9311b_isr_gmac_INT_MS_id_map[i].reg_bit_mask)
        {
            INTR_ISR_BITMAP_SET(*pisr_sts, rtl9311b_isr_gmac_INT_MS_id_map[i].isr_id);
        }
    }

    reg_data = 0;
    ioal_soc_mem32_read(unit, RTL9311B_ISR1_ADDR, &reg_data);
    RT_LOG(LOG_DEBUG, MOD_BSP, "%s:%u ISR1=%x\n", __FUNCTION__, __LINE__, reg_data);
    size = sizeof(rtl9311b_isr_gmac_ISR1_id_map)/sizeof(drv_isr_idRegBitMap_t);
    for (i = 0; i < size; i++)
    {
        if (reg_data & rtl9311b_isr_gmac_ISR1_id_map[i].reg_bit_mask)
        {
            INTR_ISR_BITMAP_SET(*pisr_sts, rtl9311b_isr_gmac_ISR1_id_map[i].isr_id);
        }
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}
#else
int32
rtl9311b_intr_gmacIsrSts_get(uint32 unit, drv_intr_isrBitmap_t *pisr_sts)
{
    int32 ret;
    rtcore_ioctl_t  dio;

    if (rtcore_dev_fd < 0)
    {
        return RT_ERR_FAILED;
    }

    dio.data[0] = unit;
    ret = ioctl(rtcore_dev_fd, RTCORE_INTR_GMACISRSTS_GET, &dio);
    if (ret)
    {
        return RT_ERR_FAILED;
    }
    osal_memcpy((uint8 *)pisr_sts, (uint8 *)&dio.data[1], sizeof(drv_intr_isrBitmap_t));
    return dio.ret;
}
#endif


int32
rtl9311b_intr_isrStsByDevId_get(uint32 unit, int32 dev_type, drv_intr_isrBitmap_t *pisr_sts)
{
    switch (dev_type)
    {
        case RTK_DEV_SWCORE:
            return rtl9311b_intr_swcoreIsrSts_get(unit, pisr_sts);
        case RTK_DEV_LINK_CHG:
            INTR_ISR_BITMAP_SET(*pisr_sts, INTR_ISR_PORT_LINK_CHG);
            return RT_ERR_OK;
        case RTK_DEV_OAM_DYGASP:
            INTR_ISR_BITMAP_SET(*pisr_sts, INTR_ISR_OAM_DYGASP);
            return RT_ERR_OK;
        case RTK_DEV_INT_GMAC_INTR:
            return rtl9311b_intr_gmacIsrSts_get(unit, pisr_sts);
        default:
            break;
    }

    return RT_ERR_FAILED;
}


#if defined(__KERNEL__) || defined(__BOOTLOADER__)
int32
rtl9311b_intr_gmacImrEnable_get(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t *pEnable)
{
    uint32      reg_data;
    unsigned long       flags = 0;

    if (!(HWP_CPU_EMBEDDED() && (unit == HWP_MY_UNIT_ID())))
    {
        return RT_ERR_FAILED;
    }

    INTR_SEM_LOCK(unit);
    if (isrId == INTR_ISR_GMAC_RXDONE)
    {
        //IMR get/set
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        if (reg_data & RTL9311B_INT_MS_IMR_ROK_MASK)
            *pEnable = ENABLED;
        else
            *pEnable = DISABLED;
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_RXRUNOUT)
    {
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        if (reg_data & RTL9311B_INT_MS_IMR_RDU_MASK)
            *pEnable = ENABLED;
        else
            *pEnable = DISABLED;
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXDONE)
    {
        /* TX RING 1 */
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        if (reg_data & RTL9311B_INT_MS_IMR_TOK_MASK)
            *pEnable = ENABLED;
        else
            *pEnable = DISABLED;
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXRUNOUT)
    {
        /* TX RING 1 */
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        if (reg_data & RTL9311B_INT_MS_IMR_TDU_MASK)
            *pEnable = ENABLED;
        else
            *pEnable = DISABLED;
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_LINKCHG)
    {
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        if (reg_data & RTL9311B_INT_MS_IMR_LinkChg_MASK)
            *pEnable = ENABLED;
        else
            *pEnable = DISABLED;
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;
}
#else
int32
rtl9311b_intr_gmacImrEnable_get(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t *pEnable)
{
    int32 ret;
    rtcore_ioctl_t  dio;

    if (rtcore_dev_fd < 0)
    {
        return RT_ERR_FAILED;
    }

    dio.data[0] = unit;
    dio.data[1] = isrId;
    ret = ioctl(rtcore_dev_fd, RTCORE_INTR_GMAIMRENABLE_GET, &dio);
    if (ret)
    {
        return RT_ERR_FAILED;
    }
    osal_memcpy((uint8 *)pEnable, (uint8 *)&dio.data[2], sizeof(rtk_enable_t));
    return dio.ret;
}
#endif

#if defined(__KERNEL__) || defined(__BOOTLOADER__)
int32
rtl9311b_intr_gmacImrEnable_set(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t enable)
{
    uint32      val;
    uint32      reg_data;
    unsigned long       flags = 0;

    if (!(HWP_CPU_EMBEDDED() && (unit == HWP_MY_UNIT_ID())))
    {
        return RT_ERR_FAILED;
    }

    INTR_SEM_LOCK(unit);
    if (isrId == INTR_ISR_GMAC_RXDONE)
    {
        //IMR get/set
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9311B_INT_MS_IMR_ROK_MASK) & 0xffff0000);
        reg_data |= ((val << RTL9311B_INT_MS_IMR_ROK_OFFSET) & RTL9311B_INT_MS_IMR_ROK_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_RXRUNOUT)
    {
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9311B_INT_MS_IMR_RDU_MASK) & 0xffff0000);
        reg_data |= ((val << RTL9311B_INT_MS_IMR_RDU_OFFSET) & RTL9311B_INT_MS_IMR_RDU_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXDONE)
    {
        /* TX RING 1 */
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9311B_INT_MS_IMR_TOK_MASK) & 0xffff0000);
        reg_data |= (((val & 0x1) << RTL9311B_INT_MS_IMR_TOK_OFFSET) & RTL9311B_INT_MS_IMR_TOK_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_field_write(unit, RTL9311B_IMR0_ADDR, RTL9311B_IMR0_IMR0_TOK_OFFSET, RTL9311B_IMR0_IMR0_TOK_MASK, 0x1F);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXRUNOUT)
    {
        /* TX RING 1 */
        val = (enable == DISABLED) ? 0 : 0x1F;
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9311B_INT_MS_IMR_TDU_MASK) & 0xffff0000);
        reg_data |= (((val & 0x1) << RTL9311B_INT_MS_IMR_TDU_OFFSET) & RTL9311B_INT_MS_IMR_TDU_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_field_write(unit, RTL9311B_IMR0_ADDR, RTL9311B_IMR0_IMR0_TDU_OFFSET, RTL9311B_IMR0_IMR0_TDU_MASK, 0x1F);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_LINKCHG)
    {
        val = (enable == DISABLED) ? 0 : 1;
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        reg_data &= ((~RTL9311B_INT_MS_IMR_LinkChg_MASK) & 0xffff0000);
        reg_data |= (((val & 0x1) << RTL9311B_INT_MS_IMR_LinkChg_OFFSET) & RTL9311B_INT_MS_IMR_LinkChg_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;
}
#else
int32
rtl9311b_intr_gmacImrEnable_set(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t enable)
{
    int32 ret;
    rtcore_ioctl_t  dio;

    if (rtcore_dev_fd < 0)
    {
        return RT_ERR_FAILED;
    }

    dio.data[0] = unit;
    dio.data[1] = isrId;
    dio.data[2] = enable;
    ret = ioctl(rtcore_dev_fd, RTCORE_INTR_GMACIMRENABLE_SET, &dio);
    if (ret)
    {
        return RT_ERR_FAILED;
    }
    return dio.ret;
}
#endif

int32
_rtl9311b_intr_swcoreImrEnable_set(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t enable)
{
    uint32      val;
    uint32      reg_data;
    unsigned long       flags = 0;

    INTR_SEM_LOCK(unit);

    RT_LOG(LOG_DEBUG, MOD_BSP, "%s:%u:unit=%u isrId=%u en=%u\n", __FUNCTION__, __LINE__, unit, isrId, enable);
    if (isrId == INTR_ISR_OAM_DYGASP)
    {
        val = (enable == DISABLED) ? 0 : 1;
        ioal_mem32_read(unit, RTL9311B_IMR_MISC_ADDR, &reg_data);
        reg_data = (reg_data & ~RTL9311B_IMR_MISC_IMR_OAM_DYGASP_MASK) | ((val << RTL9311B_IMR_MISC_IMR_OAM_DYGASP_OFFSET) & RTL9311B_IMR_MISC_IMR_OAM_DYGASP_MASK);
        ioal_mem32_write(unit, RTL9311B_IMR_MISC_ADDR, reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_CCM)
    {
        val = (enable == DISABLED) ? 0x0 : 0xFFFF;
        ioal_mem32_read(unit, RTL9311B_IMR_MISC_ADDR, &reg_data);
        reg_data = (reg_data & ~RTL9311B_IMR_MISC_IMR_CCM_MASK) | ((val << RTL9311B_IMR_MISC_IMR_CCM_OFFSET) & RTL9311B_IMR_MISC_IMR_CCM_MASK);
        ioal_mem32_write(unit, RTL9311B_IMR_MISC_ADDR, reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PORT_LINK_CHG)
    {
        if (enable == DISABLED)
        {
            ioal_mem32_write(unit, RTL9311B_IMR_PORT_LINK_STS_CHG_ADDR(0), 0x0);
        }
        else
        {
            ioal_mem32_write(unit, RTL9311B_IMR_PORT_LINK_STS_CHG_ADDR(0), 0x0FFFFFFF);
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_EXT_GPIO)
    {
        if (enable == DISABLED)
        {
            ioal_mem32_write(unit, RTL9311B_IMR_EXT_GPIO_ADDR(0), 0x0);
            ioal_mem32_write(unit, RTL9311B_IMR_EXT_GPIO_ADDR(32), 0x0);
        }
        else
        {
            ioal_mem32_write(unit, RTL9311B_IMR_EXT_GPIO_ADDR(0), drv_intr_imrExtGpio0Data[unit]);
            ioal_mem32_write(unit, RTL9311B_IMR_EXT_GPIO_ADDR(32), drv_intr_imrExtGpio1Data[unit]);
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_ECC1BIT)
    {
        ioal_mem32_read(unit, RTL9311B_IMR_ECC_ADDR, &reg_data);
        if (enable == DISABLED)
        {
            reg_data &= ~(RTL9311B_IMR_ECC_IMR_ECC1_MASK);
        }
        else
        {
            reg_data |= (RTL9311B_IMR_ECC_IMR_ECC1_MASK);
        }
        ioal_mem32_write(unit, RTL9311B_IMR_ECC_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_ECC2BIT)
    {
        ioal_mem32_read(unit, RTL9311B_IMR_ECC_ADDR, &reg_data);
        if (enable == DISABLED)
        {
            reg_data &= ~(RTL9311B_IMR_ECC_IMR_ECC2_MASK);
        }
        else
        {
            reg_data |= (RTL9311B_IMR_ECC_IMR_ECC2_MASK);
        }
        ioal_mem32_write(unit, RTL9311B_IMR_ECC_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PARITY)
    {
        ioal_mem32_read(unit, RTL9311B_IMR_ECC_ADDR, &reg_data);
        if (enable == DISABLED)
        {
            reg_data &= ~(RTL9311B_IMR_ECC_IMR_PARITY_MASK);
        }
        else
        {
            reg_data |= (RTL9311B_IMR_ECC_IMR_PARITY_MASK);
        }
        ioal_mem32_write(unit, RTL9311B_IMR_ECC_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PARITY_REG)
    {
        ioal_mem32_read(unit, RTL9311B_IMR_ECC_ADDR, &reg_data);
        if (enable == DISABLED)
        {
            reg_data &= ~(RTL9311B_IMR_ECC_IMR_REG_PARITY_MASK);
        }
        else
        {
            reg_data |= (RTL9311B_IMR_ECC_IMR_REG_PARITY_MASK);
        }
        ioal_mem32_write(unit, RTL9311B_IMR_ECC_ADDR, reg_data);
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;

}

int32
rtl9311b_intr_swcoreImrEnable_set(uint32 unit, drv_intr_isr_t isrId, rtk_enable_t enable)
{

    if (_rtl9311b_intr_swcoreImrEnable_set(unit, isrId, enable) == RT_ERR_OK)
    {
        return RT_ERR_OK;
    }

    if (rtl9311b_intr_gmacImrEnable_set(unit, isrId, enable) == RT_ERR_OK)
    {
        return RT_ERR_OK;
    }

    return RT_ERR_FAILED;
}


int32
_rtl9311b_intr_paritySubSts_get(uint32 unit, drv_intr_eccSts_t *parity)
{
    int32 i;
    uint32  reg_data = 0, reg;
    uint32  parity_sts_regs[] = {   RTL9311B_ACLACT_PARITY_INT_ISR_ADDR,
                                    RTL9311B_ACLLOG_PARITY_INT_ISR_ADDR,
                                    RTL9311B_ALE_GLB_STATUS_FOR_PARITY_ADDR,
                                    RTL9311B_ALE_GLB_STATUS_FOR_PARITY_1_ADDR,
                                    RTL9311B_ALE_GLB_STATUS_FOR_PARITY_2_ADDR,
                                    RTL9311B_ENCAP_SRAM_PARITY_INT_ISR_ADDR
                                };

    parity->sts_num = 0;
    for (i = 0; i < sizeof(parity_sts_regs)/sizeof(uint32); i++)
    {
        reg = parity_sts_regs[i];
        ioal_mem32_read(unit, reg, &reg_data);
        if (reg_data != 0)
        {
            parity->sts[parity->sts_num].reg = reg;
            parity->sts[parity->sts_num].data = reg_data;
            parity->sts_num++;
            ioal_mem32_write(unit, reg, reg_data);   /* clear */
        }
    }
    return RT_ERR_OK;
}


int32
_rtl9311b_intr_ecc1SubSts_get(uint32 unit, drv_intr_eccSts_t *pEcc)
{
    int32 i;
    uint32  reg_data = 0, reg, msk;
    uint32  writeback_data = 0;  /* clear pending, but don't touch RW field */
    /* reg, status mask */
    /* TIGERSHARK-1267 */
    uint32  ecc1_sts_regs[][2] = {{ RTL9311B_ISR_ECC_ADDR,                      (RTL9311B_ISR_ECC_ISR_INT_ERR1_PARSER_MASK) },
                                  { RTL9311B_ALE_GLB_STATUS_FOR_ECC_ADDR,       (RTL9311B_ALE_GLB_STATUS_FOR_ECC_ISR_ALE_ECC_MASK) },
                                  { RTL9311B_ECC_INT_MASK_ISR_ADDR,             (RTL9311B_ECC_INT_MASK_ISR_FLAG1_MASK)},
                                  { RTL9311B_ENCAP_ECC_MULTIBIT_INT_ISR_ADDR,   (RTL9311B_ENCAP_ECC_MULTIBIT_INT_ISR_FLAG1_MASK) },
                                  { RTL9311B_SER_NIC_ISR_ECC1BIT_ADDR,          (RTL9311B_SER_NIC_ISR_ECC1BIT_ECC1BIT_TX_MASK | RTL9311B_SER_NIC_ISR_ECC1BIT_ECC1BIT_RX_MASK)},
                                  { RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_ADDR, (RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_ISR_INGR_ECC_ERR1_MASK) },
                                };

    pEcc->sts_num = 0;
    for (i = 0; i < sizeof(ecc1_sts_regs)/(sizeof(uint32)*2); i++)
    {
        reg = ecc1_sts_regs[i][0];
        msk = ecc1_sts_regs[i][1];
        ioal_mem32_read(unit, reg, &reg_data);
        if (reg == RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_ADDR)
            writeback_data = reg_data & (msk | RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_IMR_INGR_ECC_ERR2_MASK \
                    | RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_IMR_INGR_ECC_ERR1_MASK);
        else
            writeback_data = reg_data & msk;
        reg_data = reg_data & msk;
        if (reg_data != 0)
        {
            pEcc->sts[pEcc->sts_num].reg = reg;
            pEcc->sts[pEcc->sts_num].data = reg_data;
            pEcc->sts_num++;
            ioal_mem32_write(unit, reg, writeback_data);  /* clear */
        }
    }
    return RT_ERR_OK;
}


int32
_rtl9311b_intr_ecc2SubSts_get(uint32 unit, drv_intr_eccSts_t *pEcc)
{
    int32 i;
    uint32  reg_data = 0, reg, msk;
    uint32  writeback_data = 0;  /* clear pending, but don't touch RW field */
    /* reg, status mask */
    /* TIGERSHARK-1267 */
    uint32  ecc2_sts_regs[][2] = {{ RTL9311B_ISR_ECC_ADDR,                      (RTL9311B_ISR_ECC_ISR_INT_ERR2_PARSER_MASK) },
                                  { RTL9311B_ALE_GLB_STATUS_FOR_ECC_ADDR,       (RTL9311B_ALE_GLB_STATUS_FOR_ECC_ISR_ALE_ECC2_MASK) },
                                  { RTL9311B_ECC_INT_MASK_ISR_ADDR,             (RTL9311B_ECC_INT_MASK_ISR_FLAG2_MASK) },
                                  { RTL9311B_ENCAP_ECC_MULTIBIT_INT_ISR_ADDR,   (RTL9311B_ENCAP_ECC_MULTIBIT_INT_ISR_FLAG2_MASK) },
                                  { RTL9311B_SER_NIC_ISR_ECC2BIT_ADDR,          (RTL9311B_SER_NIC_ISR_ECC2BIT_ECC2BIT_TX_MASK | RTL9311B_SER_NIC_ISR_ECC2BIT_ECC2BIT_RX_MASK) },
                                  { RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_ADDR, (RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_ISR_INGR_ECC_ERR2_MASK) },
                                };

    pEcc->sts_num = 0;
    for (i = 0; i < sizeof(ecc2_sts_regs)/(sizeof(uint32)*2); i++)
    {
        reg = ecc2_sts_regs[i][0];
        msk = ecc2_sts_regs[i][1];
        ioal_mem32_read(unit, reg, &reg_data);
        if (reg == RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_ADDR)
            writeback_data = reg_data & (msk | RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_IMR_INGR_ECC_ERR2_MASK \
                    | RTL9311B_ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_CFG_IMR_INGR_ECC_ERR1_MASK);
        else
            writeback_data = reg_data & msk;
        reg_data = reg_data & msk;
        if (reg_data != 0)
        {
            pEcc->sts[pEcc->sts_num].reg = reg;
            pEcc->sts[pEcc->sts_num].data = reg_data;
            pEcc->sts_num++;
            ioal_mem32_write(unit, reg, writeback_data);  /* clear */
        }
    }
    return RT_ERR_OK;
}

int32
_rtl9311b_intr_swcoreSts_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    uint32      reg_data = 0;
    unsigned long       flags = 0;
    int32 ret;

    INTR_SEM_LOCK(unit);
    osal_memset(pFunc_sts, 0, sizeof(drv_intr_data_t));
    if (isrId == INTR_ISR_OAM_DYGASP)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_MISC_ADDR, &reg_data);
        ioal_mem32_write(unit, RTL9311B_ISR_MISC_ADDR, RTL9311B_ISR_MISC_ISR_OAM_DYGASP_MASK);
        pFunc_sts->u.oam_dygsp = (reg_data & RTL9311B_ISR_MISC_ISR_OAM_DYGASP_MASK) ? 1 : 0;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_CCM)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_MISC_ADDR, &reg_data);
        ioal_mem32_write(unit, RTL9311B_ISR_MISC_ADDR, RTL9311B_ISR_MISC_ISR_CCM_MASK);
        pFunc_sts->u.ccm = (reg_data & RTL9311B_ISR_MISC_ISR_CCM_MASK) >> RTL9311B_ISR_MISC_ISR_CCM_OFFSET;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PORT_LINK_CHG)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_PORT_LINK_STS_CHG_ADDR(0), &reg_data);
        RTK_PORTMASK_WORD_SET(pFunc_sts->u.portmask, 0, reg_data);
        ioal_mem32_write(unit, RTL9311B_ISR_PORT_LINK_STS_CHG_ADDR(0), reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_EXT_GPIO)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_EXT_GPIO_ADDR(0), &reg_data);
        pFunc_sts->u.ext_gpio[0] = reg_data;
        ioal_mem32_write(unit, RTL9311B_ISR_EXT_GPIO_ADDR(0), reg_data);

        ioal_mem32_read(unit, RTL9311B_ISR_EXT_GPIO_ADDR(32), &reg_data);
        pFunc_sts->u.ext_gpio[1] = reg_data;
        ioal_mem32_write(unit, RTL9311B_ISR_EXT_GPIO_ADDR(32), reg_data);

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PARITY)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_ECC_ADDR, &reg_data);
        pFunc_sts->u.parity.sts_bit = 0;
        pFunc_sts->u.parity.sts_num = 0;
        if (reg_data & RTL9311B_ISR_ECC_ISR_PARITY_MASK)
        {
            pFunc_sts->u.parity.sts_bit = 1;
            reg_data = RTL9311B_ISR_ECC_ISR_PARITY_MASK;
            if ((ret = _rtl9311b_intr_paritySubSts_get(unit, &pFunc_sts->u.parity)) != RT_ERR_OK)
            {
                INTR_SEM_UNLOCK(unit);
                return ret;
            }
            ioal_mem32_write(unit, RTL9311B_ISR_ECC_ADDR, reg_data);    /* clear */
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_PARITY_REG)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_ECC_ADDR, &reg_data);
        pFunc_sts->u.parity.sts_bit = 0;
        pFunc_sts->u.parity.sts_num = 0;
        if (reg_data & RTL9311B_ISR_ECC_ISR_REG_PARITY_MASK)
        {
            pFunc_sts->u.parity.sts_bit = 1;
            reg_data = RTL9311B_ISR_ECC_ISR_REG_PARITY_MASK;
            ioal_mem32_write(unit, RTL9311B_ISR_ECC_ADDR, reg_data);    /* clear */
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_ECC1BIT)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_ECC_ADDR, &reg_data);
        pFunc_sts->u.ecc1.sts_bit = 0;
        pFunc_sts->u.ecc1.sts_num = 0;
        if (reg_data & RTL9311B_ISR_ECC_ISR_ECC1_MASK)
        {
            pFunc_sts->u.ecc1.sts_bit = 1;
            reg_data = RTL9311B_ISR_ECC_ISR_ECC1_MASK;
            if ((ret = _rtl9311b_intr_ecc1SubSts_get(unit, &pFunc_sts->u.ecc1)) != RT_ERR_OK)
            {
                INTR_SEM_UNLOCK(unit);
                return ret;
            }
            ioal_mem32_write(unit, RTL9311B_ISR_ECC_ADDR, reg_data);    /* clear */
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_ECC2BIT)
    {
        ioal_mem32_read(unit, RTL9311B_ISR_ECC_ADDR, &reg_data);
        pFunc_sts->u.ecc2.sts_bit = 0;
        pFunc_sts->u.ecc2.sts_num = 0;
        if (reg_data & RTL9311B_ISR_ECC_ISR_ECC2_MASK)
        {
            pFunc_sts->u.ecc2.sts_bit = 1;
            reg_data = RTL9311B_ISR_ECC_ISR_ECC2_MASK;
            if ((ret = _rtl9311b_intr_ecc2SubSts_get(unit, &pFunc_sts->u.ecc2)) != RT_ERR_OK)
            {
                INTR_SEM_UNLOCK(unit);
                return ret;
            }
            ioal_mem32_write(unit, RTL9311B_ISR_ECC_ADDR, reg_data);    /* clear */
        }
        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }


    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;

}


#if defined(__KERNEL__) || defined(__BOOTLOADER__)
int32
rtl9311b_intr_gmacData_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    uint32      reg_data = 0, val;
    unsigned long       flags = 0;

    if (!(HWP_CPU_EMBEDDED() && (unit == HWP_MY_UNIT_ID())))
    {
        return RT_ERR_FAILED;
    }

    INTR_SEM_LOCK(unit);

    osal_memset(pFunc_sts, 0, sizeof(drv_intr_data_t));
    if (isrId == INTR_ISR_GMAC_RXDONE)
    {
        //ISR status get and reset
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9311B_INT_MS_ISR_ROK_MASK) >> RTL9311B_INT_MS_ISR_ROK_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= ((val << RTL9311B_INT_MS_ISR_ROK_OFFSET) & RTL9311B_INT_MS_ISR_ROK_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        pFunc_sts->u.rx_done = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_RXRUNOUT)
    {
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9311B_INT_MS_ISR_RDU_MASK) >> RTL9311B_INT_MS_ISR_RDU_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= ((val << RTL9311B_INT_MS_ISR_RDU_OFFSET) & RTL9311B_INT_MS_ISR_RDU_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        pFunc_sts->u.rx_runout = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXDONE)
    {
        /* TX RING 1*/
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9311B_INT_MS_ISR_TOK_MASK) >> RTL9311B_INT_MS_ISR_TOK_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= (((val & 0x1) << RTL9311B_INT_MS_ISR_TOK_OFFSET) & RTL9311B_INT_MS_ISR_TOK_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_write(unit, RTL9311B_ISR1_ADDR, (val << RTL9311B_ISR1_ISR_TOK_OFFSET) & RTL9311B_ISR1_ISR_TOK_MASK);
        pFunc_sts->u.tx_done = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_TXRUNOUT)
    {
        /* TX RING 1*/
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9311B_INT_MS_ISR_TDU_MASK) >> RTL9311B_INT_MS_ISR_TDU_OFFSET;
        reg_data &= 0xffff0000;
        reg_data |= (((val & 0x1) << RTL9311B_INT_MS_ISR_TDU_OFFSET) & RTL9311B_INT_MS_ISR_TDU_MASK);
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        /* ring 2 ~ ring 5*/
        ioal_soc_mem32_write(unit, RTL9311B_ISR1_ADDR, (val << RTL9311B_ISR1_ISR_TDU_OFFSET) & RTL9311B_ISR1_ISR_TDU_MASK);
        pFunc_sts->u.tx_runout = val;

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }
    else if (isrId == INTR_ISR_GMAC_LINKCHG)
    {
        ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &reg_data);
        val = (reg_data & RTL9311B_INT_MS_ISR_LinkChg_MASK) >> RTL9311B_INT_MS_ISR_LinkChg_OFFSET;
        reg_data &= 0xffff0000; /* keep [31:16] the IMR config fields */
        reg_data |= RTL9311B_INT_MS_ISR_LinkChg_MASK; /* write 1 to clear */
        ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, reg_data);
        if (val)
            RTK_PORTMASK_PORT_SET(pFunc_sts->u.portmask, RTK_OOB_IDX_TO_PORT(0));

        INTR_SEM_UNLOCK(unit);
        return RT_ERR_OK;
    }

    INTR_SEM_UNLOCK(unit);
    return RT_ERR_FAILED;
}
#else
int32
rtl9311b_intr_gmacData_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    int32 ret;
    rtcore_ioctl_t  dio;

    if (rtcore_dev_fd < 0)
    {
        return RT_ERR_FAILED;
    }

    dio.data[0] = unit;
    dio.data[1] = isrId;
    ret = ioctl(rtcore_dev_fd, RTCORE_INTR_GMACDATA_GET, &dio);
    if (ret)
    {
        return RT_ERR_FAILED;
    }
    osal_memcpy((uint8 *)pFunc_sts, (uint8 *)&dio.data[2], sizeof(drv_intr_data_t));
    return dio.ret;
}
#endif

int32
rtl9311b_intr_swcoreSts_get(uint32 unit, drv_intr_isr_t isrId, drv_intr_data_t *pFunc_sts)
{
    if (_rtl9311b_intr_swcoreSts_get(unit, isrId, pFunc_sts) == RT_ERR_OK)
    {
        return RT_ERR_OK;
    }

    if (rtl9311b_intr_gmacData_get(unit, isrId, pFunc_sts) == RT_ERR_OK)
    {
        return RT_ERR_OK;
    }
    return RT_ERR_FAILED;
}


int32
rtl9311b_intr_swcore_exit(uint32 unit)
{
    drv_intr_isr_t isrId;
    drv_intr_isrBitmap_t pisr_sts;

    /* disable IMR */
    for (isrId = 0; isrId < INTR_ISR_END; isrId++)
    {
        rtl9311b_intr_swcoreImrEnable_set(unit, isrId, DISABLED);
    }

    /* read clear status */
    rtl9311b_intr_swcoreIsrSts_get(unit, &pisr_sts);

    intr_init_status[unit] = INIT_NOT_COMPLETED;

    return RT_ERR_OK;
}

int32
rtl9311b_intr_exit(uint32 unit)
{
    /* deinit TC4 */
    ioal_soc_mem32_write(unit, RTL9311B_TC4CTRL_ADDR, 0);

    /* deinit TC4 intr */
    ioal_soc_mem32_field_write(unit, RTL9311B_TC4INTR_ADDR, RTL9311B_TC4INTR_TC4IE_OFFSET, RTL9311B_TC4INTR_TC4IE_MASK, 0x0);
    ioal_soc_mem32_field_write(unit, RTL9311B_TC4INTR_ADDR, RTL9311B_TC4INTR_TC4IP_OFFSET, RTL9311B_TC4INTR_TC4IP_MASK, 0x1);

    /* deinit delayed intr */
    ioal_soc_mem32_field_write(unit, RTL9311B_TC4_DLY_INTR_ADDR, RTL9311B_TC4_DLY_INTR_DELAYED_IP_SEL_OFFSET, RTL9311B_TC4_DLY_INTR_DELAYED_IP_SEL_MASK, 0x0);

    return RT_ERR_OK;
}

