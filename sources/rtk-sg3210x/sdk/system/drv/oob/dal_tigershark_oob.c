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
 * Purpose : DRV APIs definition.
 *
 * Feature : Out-of-band interface relative API
 *
 */


/*
 * Include Files
 */

#include <common/rt_autoconf.h>
#ifdef __UBOOT__
  #include <linux/dma-mapping.h>
#else
  #include <linux/pci.h>
#endif
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
  #include <asm/cacheflush.h>
  #include <linux/time.h>
  #include <osal/atomic.h>
#endif

#include <common/type.h>

#include <private/drv/oob/dal_tigershark_oob.h>
#include <soc/socdef/rtl9311b_soc_reg.h>
#include <ioal/mem32.h>
#include <osal/lib.h>
#include <osal/time.h>
#include <osal/print.h>
#include <osal/sem.h>
#include <osal/isr.h>
#include <osal/memory.h>
#include <osal/cache.h>
#include <soc/type.h>
#include <common/debug/rt_log.h>
#include <hwp/hw_profile.h>
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <private/drv/oob/dal_common_oob.h>
#include <drv/drv_mgmt.h>
#include <drv/intr/intr.h>
#include <common/util/rt_util_intr.h>
#include <dal/dal_construct.h>

/*
 * Symbol Definition
 */

#define     RX_JUMBO_SIZE       16379
#define     TX_JUMBO_SIZE       9216
#define     RX_RING_NUM     1
#define     TX_RING_NUM     5

#define     HOL_SUPP_THR_ON                 8
#define     HOL_SUPP_THR_OFF                12      /* ON should be smaller than OFF*/
#define     RX_TIMER_EXP                    3   /*0~0xf means  set*4 TU*/
#define     RX_INTR_MIT                     1   /*0~0xf means  set*4 packet*/
#define     TX_TIMER_EXP                    3   /*0~0xf means  set*4 TU*/
#define     TX_INTR_MIT                     1   /*0~0xf means  set*4 packet*/
#define     INTR_TIMER_SEL                  0
#define     DESC_FMT_EXT_R9311B            1
#define     SHORT_DESC_FMT_R9311B          1
#define     TX_THR              0x3
#define     RX_THR              0x3

#define     MAX_DMA_SEL         0x1


#define     RX_DESC_SIZE        16
#define     TX_DESC_SIZE        16


#define     RTL9311B_EN_IO_CMD_TXF1_4_OFFSET       RTL9311B_ETN_IO_CMD_TxFN1st_OFFSET
#define     RTL9311B_EN_IO_CMD_TXF1_4_MASK         (0xf << RTL9311B_EN_IO_CMD_TXF1_4_OFFSET)

#define     RTL9311B_IMR0_IMR0_TDU_OFFSET      RTL9311B_IMR0_IMR0_TDU1_OFFSET
#define     RTL9311B_IMR0_IMR0_TDU_MASK        (0x1F << RTL9311B_IMR0_IMR0_TDU_OFFSET)

#define     RTL9311B_IMR0_IMR0_TOK_OFFSET      RTL9311B_IMR0_IMR0_TOK1_OFFSET
#define     RTL9311B_IMR0_IMR0_TOK_MASK        (0x1F << RTL9311B_IMR0_IMR0_TOK_OFFSET)

#define     RTL9311B_ISR1_ISR_TDU_OFFSET       RTL9311B_ISR1_ISR_TDU1_OFFSET
#define     RTL9311B_ISR1_ISR_TDU_MASK         (0x1F << RTL9311B_ISR1_ISR_TDU1_OFFSET)

#define     RTL9311B_ISR1_ISR_TOK_OFFSET       RTL9311B_ISR1_ISR_TOK1_OFFSET
#define     RTL9311B_ISR1_ISR_TOK_MASK         (0x1F << RTL9311B_ISR1_ISR_TOK_OFFSET)

#define     RTL9311B_ACCESS_PHY_TIMEOUT_TIME   (5 * 1000 * 1000) /* microseconds */
#define     RTL9311B_ACCESS_SDS_TIMEOUT_TIME   (5 * 1000 * 1000)   /* microseconds */ /* TBD */


/* SerDes config */
static oob_sdsPatchConfType_t   oob_sds_patch_config[] =
{
    {0x45, 0x62,  96,  100,  0x007C},
    {0x45, 0x6E, 106,  104,  0x0061},
    {0x45, 0x6E, 110,  110,  0x0065},
    {0x45, 0x66, 102,  102,  0x0064},
    {0x45, 0x61, 110,  103,  0x0054},
    {0x52, 0x6E,  97,   97,  0x0064},
    {0x52, 0x67, 107,  104,  0x0064},
};


/*
 * Macro Definition
 */




/*
 * Data Type Definition
 */

typedef struct oob_reg_field_s
{
    uint32  reg;
    uint32  offset;
    uint32  mask;
} oob_reg_field_t;


/*
 * Data Declaration
 */

static uint32   rxRingIdSize_9311B[] = {32};
static uint32   txRingIdSize_9311B[] = {16, 16, 16, 16, 16};
static osal_mutex_t         oob_sem[RTK_MAX_NUM_OF_UNIT] = { 0 };
static uint32   oob_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};

static const oob_reg_field_t tx_ring_base_addr[] = {
    {RTL9311B_TXFDP1_ADDR, RTL9311B_TXFDP1_TxFDP1_OFFSET, RTL9311B_TXFDP1_TxFDP1_MASK},
    {RTL9311B_TXFDP2_ADDR, RTL9311B_TXFDP2_TxFDP2_OFFSET, RTL9311B_TXFDP2_TxFDP2_MASK},
    {RTL9311B_TXFDP3_ADDR, RTL9311B_TXFDP3_TxFDP3_OFFSET, RTL9311B_TXFDP3_TxFDP3_MASK},
    {RTL9311B_TXFDP4_ADDR, RTL9311B_TXFDP4_TxFDP4_OFFSET, RTL9311B_TXFDP4_TxFDP4_MASK},
    {RTL9311B_TXFDP5_ADDR, RTL9311B_TXFDP5_TxFDP5_OFFSET, RTL9311B_TXFDP5_TxFDP5_MASK}
    };

static const oob_reg_field_t tx_ring_cur_addr[] = {
    {RTL9311B_TXCDO1_ADDR, RTL9311B_TXCDO1_TxCDO1_OFFSET, RTL9311B_TXCDO1_TxCDO1_MASK},
    {RTL9311B_TXCDO2_ADDR, RTL9311B_TXCDO2_TxCDO2_OFFSET, RTL9311B_TXCDO2_TxCDO2_MASK},
    {RTL9311B_TXCDO3_ADDR, RTL9311B_TXCDO3_TxCDO3_OFFSET, RTL9311B_TXCDO3_TxCDO3_MASK},
    {RTL9311B_TXCDO4_ADDR, RTL9311B_TXCDO4_TxCDO4_OFFSET, RTL9311B_TXCDO4_TxCDO4_MASK},
    {RTL9311B_TXCDO5_ADDR, RTL9311B_TXCDO5_TxCDO5_OFFSET, RTL9311B_TXCDO5_TxCDO5_MASK},
    };

static const oob_reg_field_t rx_ring_enable_addr[] = {
    {RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_RXRING1_OFFSET, RTL9311B_IO_CMD1_RXRING1_MASK},
    {RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_RXRING2_OFFSET, RTL9311B_IO_CMD1_RXRING2_MASK},
    {RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_RXRING3_OFFSET, RTL9311B_IO_CMD1_RXRING3_MASK},
    {RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_RXRING4_OFFSET, RTL9311B_IO_CMD1_RXRING4_MASK},
    {RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_RXRING5_OFFSET, RTL9311B_IO_CMD1_RXRING5_MASK},
    {RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_RXRING6_OFFSET, RTL9311B_IO_CMD1_RXRING6_MASK},
    };




/*
 * Macro Definition
 */
#define OOB_SEM_LOCK(unit)    \
do {\
    if (oob_sem[unit] == 0) \
        oob_sem[unit] = osal_sem_mutex_create(); \
    if (osal_sem_mutex_take(oob_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_BSP), "semaphore lock failed");\
        return RT_ERR_SEM_LOCK_FAILED;\
    }\
} while(0)

#define OOB_SEM_UNLOCK(unit)   \
do {\
    if (osal_sem_mutex_give(oob_sem[unit]) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_BSP), "semaphore unlock failed");\
        return RT_ERR_SEM_UNLOCK_FAILED;\
    }\
} while(0)






/*
 * Function Declaration
 */

static int32    _ioal_soc_mem32_field_read(uint32 unit, uintptr addr, uint32 offset, uint32 mask, uint32 *pVal)
{
    uint32  tmp;
    int32   ret = RT_ERR_FAILED;

    RT_ERR_CHK(ioal_soc_mem32_read(unit, addr, &tmp), ret);
    *pVal = (tmp & mask) >> offset;

    return RT_ERR_OK;
}

static int32    _ioal_soc_mem32_field_write(uint32 unit, uintptr addr, uint32 offset, uint32 mask, uint32 val)
{
    uint32  tmp;
    int32   ret = RT_ERR_FAILED;

    RT_ERR_CHK(ioal_soc_mem32_read(unit, addr, &tmp), ret);
    tmp = (tmp & ~mask) | ((val << offset) & mask);
    RT_ERR_CHK(ioal_soc_mem32_write(unit, addr, tmp), ret);

    return RT_ERR_OK;
}


int32   _dal_tigershark_oob_exit(uint32 unit, rtk_port_t port)
{
    if (oob_sem[unit])
    {
        osal_sem_mutex_destroy(oob_sem[unit]);
        oob_sem[unit] = 0;
    }
    return RT_ERR_OK;
}


int32   dal_tigershark_oob_init_sts_get(uint32 unit, rtk_port_t port, uint32* pVal)
{
    *pVal = oob_init[unit];
    return RT_ERR_OK;
}

int32   dal_tigershark_oob_init_sts_set(uint32 unit, rtk_port_t port, uint32 sts)
{
    oob_init[unit] = sts;
    return RT_ERR_OK;
}



int32   dal_tigershark_oob_nic_intr_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    drv_intr_data_t     sts_data;

    RT_LOG(LOG_DEBUG, MOD_OOB, "unit %u port %u nic intr enable set:%u", unit, port, enable);
    if (DISABLED == enable)
    {
        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_RXDONE, DISABLED);
        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_RXRUNOUT, DISABLED);
        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_TXDONE, DISABLED);
        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_TXRUNOUT, DISABLED);
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_RXDONE, &sts_data);
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_RXRUNOUT, &sts_data);
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_TXDONE, &sts_data);
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_TXRUNOUT, &sts_data);
    }
    else
    {
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_RXDONE, &sts_data);
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_RXRUNOUT, &sts_data);
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_TXDONE, &sts_data);
        rtl9311b_intr_gmacData_get(unit, INTR_ISR_GMAC_TXRUNOUT, &sts_data);
        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_RXDONE, ENABLED);
        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_RXRUNOUT, ENABLED);
        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_TXDONE, ENABLED);
//        rtl9311b_intr_gmacImrEnable_set(unit, INTR_ISR_GMAC_TXRUNOUT, ENABLED);
    }
    return RT_ERR_OK;
}



int32   dal_tigershark_oob_desc_to_raw(uint32 unit, rtk_port_t port, oob_dir_t dir, oob_desc_t* pDesc, uint8* pVal)
{
    uint32* ptr = NULL;

    ptr = (uint32*)(pVal + 12);
    *ptr = CPU_to_BE32(pDesc->rsv);
    ptr--;
    *ptr = CPU_to_BE32(pDesc->vlan_info);
    ptr--;
    *ptr = CPU_to_BE32((uint32)(pDesc->buf_addr & 0xffffffff));
    ptr--;
    *ptr = CPU_to_BE32(pDesc->info);

    return RT_ERR_OK;
}


int32   dal_tigershark_oob_raw_to_desc(uint32 unit, rtk_port_t port, oob_dir_t dir, uint8* pVal, oob_desc_t* pDesc)
{
    uint32* ptr = (uint32*)pVal;

    pDesc->info = BE32_to_CPU(*ptr);
    ptr++;
    pDesc->buf_addr = BE32_to_CPU(*ptr);
    ptr++;
    pDesc->vlan_info = BE32_to_CPU(*ptr);
    ptr++;
    pDesc->rsv = BE32_to_CPU(*ptr);
    return RT_ERR_OK;
}


int32   dal_tigershark_oob_nic_init(uint32  unit, rtk_port_t port)
{
    int32   ret = RT_ERR_FAILED;
    uint32  temp = 0;
    uint32  val = 0;
    hol_ring_thr_t  thr;

    RT_INIT_REENTRY_CHK(oob_init[unit]);

    OOB_SEM_LOCK(unit);

    /* config max_dma_sel */
    temp = MAX_DMA_SEL & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_max_dma_sel_0_OFFSET, RTL9311B_ETN_IO_CMD_max_dma_sel_0_MASK, temp);
    temp = (MAX_DMA_SEL >> 1) & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_max_dma_sel_1_OFFSET, RTL9311B_ETN_IO_CMD_max_dma_sel_1_MASK, temp);


    /*RX/TX expire time, mitigation config*/
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_TXPktTimer_OFFSET, RTL9311B_ETN_IO_CMD_TXPktTimer_MASK, TX_TIMER_EXP);
    temp = TX_INTR_MIT & 0x7;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_TxIntMitigation_2to0_OFFSET, RTL9311B_ETN_IO_CMD_TxIntMitigation_2to0_MASK, temp);
    temp = (TX_INTR_MIT >> 3) & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_TxIntMitigation_3_OFFSET, RTL9311B_ETN_IO_CMD_TxIntMitigation_3_MASK, temp);

    temp = RX_TIMER_EXP & 0x7;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_RXPktTimer_2to0_OFFSET, RTL9311B_ETN_IO_CMD_RXPktTimer_2to0_MASK, temp);
    temp = (RX_TIMER_EXP >> 3) & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_RXPktTimer_3_OFFSET, RTL9311B_ETN_IO_CMD_RXPktTimer_3_MASK, temp);
    temp = RX_INTR_MIT & 0x7;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_RxIntMitigation_2to0_OFFSET, RTL9311B_ETN_IO_CMD_RxIntMitigation_2to0_MASK, temp);
    temp = (RX_INTR_MIT >> 3) & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_RxIntMitigation_3_OFFSET, RTL9311B_ETN_IO_CMD_RxIntMitigation_3_MASK, temp);

    /*interrupt timer*/
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_reg_int_timer_sel_OFFSET, RTL9311B_ETN_IO_CMD_reg_int_timer_sel_MASK, INTR_TIMER_SEL);

    /*clear pending flag*/
    ioal_soc_mem32_write(unit, RTL9311B_INT_MS_ADDR, 0xffff);


    /* RX/TX threshold */
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_tsh_OFFSET, RTL9311B_ETN_IO_CMD_tsh_MASK, TX_THR);
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_RXFTH_OFFSET, RTL9311B_ETN_IO_CMD_RXFTH_MASK, RX_THR);

    /* RX use one ring only, enable 4GB & 1GB lx bus address*/
    val = (1 << RTL9311B_IO_CMD1_EN_1GB_OFFSET)
            | (1 << RTL9311B_IO_CMD1_EN_4GB_OFFSET)
            | (0 << RTL9311B_IO_CMD1_EN_RX_MRING_OFFSET)
            | (DESC_FMT_EXT_R9311B << RTL9311B_IO_CMD1_Dsc_format_extra_OFFSET);


    ioal_soc_mem32_read(unit, RTL9311B_IO_CMD1_ADDR, &temp);
    temp &= ~(RTL9311B_IO_CMD1_EN_1GB_MASK | RTL9311B_IO_CMD1_EN_4GB_MASK | RTL9311B_IO_CMD1_EN_RX_MRING_MASK | RTL9311B_IO_CMD1_Dsc_format_extra_MASK);
    temp |= val;
    ioal_soc_mem32_write(unit, RTL9311B_IO_CMD1_ADDR, temp);

    /*config IO CMD*/
    _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_ShortDesFormat_OFFSET, RTL9311B_ETN_IO_CMD_ShortDesFormat_MASK, SHORT_DESC_FMT_R9311B);
//    _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_Dsc_format_extra_OFFSET, RTL9311B_IO_CMD1_Dsc_format_extra_MASK, DESC_FMT_EXT_R9311B);


    OOB_SEM_UNLOCK(unit);


    /*config Rx ring size*/
    RT_ERR_CHK(dal_tigershark_oob_ringSize_set(unit, port, OOB_DIR_RX, 0, rxRingIdSize_9311B[0]), ret);

    thr.high = HOL_SUPP_THR_ON;
    thr.low = HOL_SUPP_THR_OFF;

    /*config hol threshold*/
    RT_ERR_CHK(dal_tigershark_oob_holRingThr_set(unit, port, OOB_DIR_RX, 0, thr), ret);

    return ret;
}

int32   dal_tigershark_oob_ringInfo_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ** ringIdSizeList, uint32 *pRingNum)
{
    if (dir == OOB_DIR_RX)
    {
        *ringIdSizeList = rxRingIdSize_9311B;
        *pRingNum = RX_RING_NUM;
    }
    else
    {
        *ringIdSizeList = txRingIdSize_9311B;
        *pRingNum = TX_RING_NUM;
    }

    return RT_ERR_OK;
}



int32   dal_tigershark_oob_ringSize_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32* pVal)
{
    uint32 temp;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        _ioal_soc_mem32_field_read(unit, RTL9311B_RXCDO1_ADDR, RTL9311B_RXCDO1_RxRingSize_1_7to4_OFFSET, RTL9311B_RXCDO1_RxRingSize_1_7to4_MASK, &temp);
        _ioal_soc_mem32_field_read(unit, RTL9311B_RXCDO1_ADDR, RTL9311B_RXCDO1_RxRingSize_1_11to8_OFFSET, RTL9311B_RXCDO1_RxRingSize_1_11to8_MASK, pVal);
        *pVal = ((*pVal) << 8) | temp;
    }
    else
    {

    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}


int32   dal_tigershark_oob_ringSize_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 val)
{
    uint32 temp = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        /*config Rx ring size*/
        temp = (val - 1) & 0xff;
        _ioal_soc_mem32_field_write(unit, RTL9311B_RXCDO1_ADDR, RTL9311B_RXCDO1_RxRingSize_1_7to4_OFFSET, RTL9311B_RXCDO1_RxRingSize_1_7to4_MASK, temp);
        temp = ((val - 1) >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9311B_RXCDO1_ADDR, RTL9311B_RXCDO1_RxRingSize_1_11to8_OFFSET, RTL9311B_RXCDO1_RxRingSize_1_11to8_MASK, temp);
    }
    else
    {

    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}

int32   dal_tigershark_oob_holRingThr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, hol_ring_thr_t* pThr)
{
    uint32 temp;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        _ioal_soc_mem32_field_read(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_OFFSET, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_MASK, &temp);
        _ioal_soc_mem32_field_read(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_OFFSET, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_MASK, &pThr->high);
        pThr->high = (pThr->high << 8) | temp;

        _ioal_soc_mem32_field_read(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_OFFSET, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_MASK, &temp);
        _ioal_soc_mem32_field_read(unit, RTL9311B_RX_PSE1_ADDR, RTL9311B_RX_PSE1_Rx_Pse_Des_Thres_off_1_OFFSET, RTL9311B_RX_PSE1_Rx_Pse_Des_Thres_off_1_MASK, &pThr->low);
        pThr->low = (pThr->low << 8) | temp;
    }
    else
    {

    }

    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}



int32   dal_tigershark_oob_holRingThr_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, hol_ring_thr_t thr)
{
    uint32 temp = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        temp = thr.high & 0xff;
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_OFFSET, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_MASK, temp);
        temp = (thr.high >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_OFFSET, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_MASK, temp);

        temp = thr.low & 0xff;
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_OFFSET, RTL9311B_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_MASK, temp);
        temp = (thr.low >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9311B_RX_PSE1_ADDR, RTL9311B_RX_PSE1_Rx_Pse_Des_Thres_off_1_OFFSET, RTL9311B_RX_PSE1_Rx_Pse_Des_Thres_off_1_MASK, temp);
    }
    else
    {

    }

    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}


int32   dal_tigershark_oob_ringBaseAddr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uintptr *pVal)
{
    uint32  temp = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9311B_RXFDP1_ADDR, RTL9311B_RXFDP1_RxFDP1_OFFSET, RTL9311B_RXFDP1_RxFDP1_MASK, &temp);
    }
    else
    {
        _ioal_soc_mem32_field_read(unit, tx_ring_base_addr[ring].reg, tx_ring_base_addr[ring].offset, tx_ring_base_addr[ring].mask, &temp);
    }
    OOB_SEM_UNLOCK(unit);
    *pVal = (uintptr)temp;
    return RT_ERR_OK;
}

int32   dal_tigershark_oob_ringBaseAddr_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uintptr val)
{
    uint32  temp = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    temp = (uint32)(val & 0xffffffff);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        ioal_soc_mem32_write(unit, RTL9311B_RXFDP1_ADDR, temp);
    }
    else
    {
        ioal_soc_mem32_write(unit, tx_ring_base_addr[ring].reg, temp);
    }
    OOB_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}


int32   dal_tigershark_oob_ringCurAddr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uintptr *pVal)
{
    uint32  offset = 0;
    uintptr  base = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    dal_tigershark_oob_ringBaseAddr_get(unit, port, dir, ring, &base);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9311B_RXCDO1_ADDR, RTL9311B_RXCDO1_RxCDO1_OFFSET, RTL9311B_RXCDO1_RxCDO1_MASK, &offset);
    }
    else
    {
        _ioal_soc_mem32_field_read(unit, tx_ring_cur_addr[ring].reg, tx_ring_cur_addr[ring].offset, tx_ring_cur_addr[ring].mask, &offset);
    }
    OOB_SEM_UNLOCK(unit);

    *pVal = (offset << 4) + base;
    return RT_ERR_OK;
}



int32   dal_tigershark_oob_cpuPtr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 *pVal)
{
    uint32  ptr_l;
    uint32  ptr_h;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_OFFSET, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_MASK, &ptr_l);
        _ioal_soc_mem32_field_read(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_OFFSET, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_MASK, &ptr_h);
        *pVal = (ptr_h << 8) | ptr_l;
    }
    else
    {

    }
    OOB_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}

int32   dal_tigershark_oob_cpuPtr_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 val)
{
    uint32  ptr_l;
    uint32  ptr_h;

    if (dir == OOB_DIR_RX)
    {
        ptr_l = val & 0xff;
        ptr_h = (val >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_OFFSET, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_MASK, ptr_l);
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETNRXCPU1_ADDR, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_OFFSET, RTL9311B_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_MASK, ptr_h);
    }
    else
    {

    }
    return RT_ERR_OK;
}

int32   dal_tigershark_oob_hwPtr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 *pVal)
{
    if (dir == OOB_DIR_TX)
        _ioal_soc_mem32_field_read(unit, tx_ring_cur_addr[ring].reg, tx_ring_cur_addr[ring].offset, tx_ring_cur_addr[ring].mask, pVal);
    else
        _ioal_soc_mem32_field_read(unit, RTL9311B_RXCDO1_ADDR, RTL9311B_RXCDO1_RxCDO1_OFFSET, RTL9311B_RXCDO1_RxCDO1_MASK, pVal);

    return RT_ERR_OK;
}


int32   dal_tigershark_oob_oobEnable_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32* pVal)
{
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_RE_OFFSET, RTL9311B_ETN_IO_CMD_RE_MASK, pVal);
    }
    else
    {
        _ioal_soc_mem32_field_read(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_TE_OFFSET, RTL9311B_ETN_IO_CMD_TE_MASK, pVal);
    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}



int32   dal_tigershark_oob_oobEnable_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 val)
{
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_RE_OFFSET, RTL9311B_ETN_IO_CMD_RE_MASK, val);
    }
    else
    {
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_TE_OFFSET, RTL9311B_ETN_IO_CMD_TE_MASK, val);
    }
    OOB_SEM_UNLOCK(unit);
#if 0
    if (val)
        osal_printf("OOB (R9311B) %s Start... \n", (dir == OOB_DIR_RX) ? "RX" : "TX");
    else
        osal_printf("OOB (R9311B) %s Stop... \n", (dir == OOB_DIR_RX) ? "RX" : "TX");
#endif
    return RT_ERR_OK;
}


int32   dal_tigershark_oob_tx_queue_pri_set(uint32 unit, rtk_port_t port, oob_tx_queue_pri_t type, oob_tx_queue_hi_low_t queue_cfg)
{

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if (TX_QUEUE_STRICT == type)
    {
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_Tx_hl_pri_sel_OFFSET, RTL9311B_IO_CMD1_Tx_hl_pri_sel_MASK, 0);
    }
    else
    {
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_Tx_hl_pri_sel_OFFSET, RTL9311B_IO_CMD1_Tx_hl_pri_sel_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_txq5_h_OFFSET, RTL9311B_IO_CMD1_txq5_h_MASK, queue_cfg.queue_5_hi);
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_txq4_h_OFFSET, RTL9311B_IO_CMD1_txq4_h_MASK, queue_cfg.queue_4_hi);
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_txq3_h_OFFSET, RTL9311B_IO_CMD1_txq3_h_MASK, queue_cfg.queue_3_hi);
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_txq2_h_OFFSET, RTL9311B_IO_CMD1_txq2_h_MASK, queue_cfg.queue_2_hi);
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_txq1_h_OFFSET, RTL9311B_IO_CMD1_txq1_h_MASK, queue_cfg.queue_1_hi);
    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}



int32   _dal_tigershark_oob_rxDesc_dump(uint32 unit, rtk_port_t port, oob_desc_t* pDesc)
{
    osal_printf("OWN\t: %d",        (pDesc->info >> DESC_INFO_OWN_OFFSET) & 0x1);
    osal_printf("\tEOR\t: %d",      (pDesc->info >> DESC_INFO_EOR_OFFSET) & 0x1);
    osal_printf("\tFS\t: %d\n",     (pDesc->info >> DESC_INFO_FS_OFFSET) & 0x1);
    osal_printf("LS\t: %d",         (pDesc->info >> DESC_INFO_LS_OFFSET) & 0x1);
    osal_printf("\tFAE\t: %d",      (pDesc->info >> RX_DESC_INFO_FAE_OFFSET) & 0x1);
    osal_printf("\tMAR\t: %d\n",    (pDesc->info >> RX_DESC_INFO_MAR_OFFSET) & 0x1);
    osal_printf("PAM\t: %d",        (pDesc->info >> RX_DESC_INFO_PAM_OFFSET) & 0x1);
    osal_printf("\tBAR\t: %d",      (pDesc->info >> RX_DESC_INFO_BAR_OFFSET) & 0x1);
    osal_printf("\tPPPOE_E8023\t: %d\n", (pDesc->info >> RX_DESC_INFO_PPPOE_E8023_OFFSET) & 0x3);
    osal_printf("RWT\t: %d",        (pDesc->info >> RX_DESC_INFO_RWT_OFFSET) & 0x1);
    osal_printf("\tRCDF\t: %d",     (pDesc->info >> RX_DESC_INFO_RCDF_OFFSET) & 0x1);
    osal_printf("\tRUNT\t: %d\n",   (pDesc->info >> RX_DESC_INFO_RUNT_OFFSET) & 0x1);
    osal_printf("CRC\t: %d",        (pDesc->info >> RX_DESC_INFO_CRC_OFFSET) & 0x1);
    osal_printf("\tPID\t: %d",      (pDesc->info >> RX_DESC_INFO_PID0_OFFSET) & 0x3);
    osal_printf("\tIPF\t: %d\n",    (pDesc->info >> RX_DESC_INFO_IPF_OFFSET) & 0x1);
    osal_printf("UDPF\t: %d",       (pDesc->info >> RX_DESC_INFO_UDPF_OFFSET) & 0x1);
    osal_printf("\tTCPF\t: %d",     (pDesc->info >> RX_DESC_INFO_TCPF_OFFSET) & 0x1);
    osal_printf("\tIPSEG\t: %d\n",  (pDesc->info >> RX_DESC_INFO_IPSEG_OFFSET) & 0x1);
    osal_printf("IPV6\t: %d",       (pDesc->info >> RX_DESC_INFO_IPV6_OFFSET) & 0x1);
    osal_printf("\tLEN\t: %d",      pDesc->info & RX_DESC_INFO_LEN_MASK);
    osal_printf("\tADDR\t: 0x%llx\n", pDesc->buf_addr);
    osal_printf("V_TAG\t: %d",      (pDesc->vlan_info >> DESC_VLAN_INFO_VLAN_TAG_OFFSET) & 0x1);
    osal_printf("\tPRI\t: %d",      (pDesc->vlan_info >> DESC_VLAN_INFO_PRI_OFFSET) & 0x1);
    osal_printf("\tCFI\t: %d\n",    (pDesc->vlan_info >> DESC_VLAN_INFO_CFI_OFFSET) & 0x1);
    osal_printf("VID\t: %d",        ((pDesc->vlan_info & 0xf) << 8) | ((pDesc->vlan_info >> 8) & 0xff));
//    osal_printf("\tREASON\t: %d\n", pDesc->reason);
//    osal_printf("PARTIAL_CHKSUM\t: %d\n", pDesc->par_chksum);
    return RT_ERR_OK;
}

int32   _dal_tigershark_oob_txDesc_dump(uint32 unit, rtk_port_t port, oob_desc_t* pDesc)
{
    osal_printf("OWN\t: %d",            (pDesc->info >> DESC_INFO_OWN_OFFSET) & 0x1);
    osal_printf("\tEOR\t: %d",          (pDesc->info >> DESC_INFO_EOR_OFFSET) & 0x1);
    osal_printf("\tFS\t: %d\n",         (pDesc->info >> DESC_INFO_FS_OFFSET) & 0x1);
    osal_printf("LS\t: %d",             (pDesc->info >> DESC_INFO_LS_OFFSET) & 0x1);
    osal_printf("\tIPCS\t: %d",         (pDesc->info >> TX_DESC_INFO_IPCS_OFFSET) & 0x1);
    osal_printf("\tUDPCS\t: %d\n",      (pDesc->info >> TX_DESC_INFO_UDPCS_OFFSET) & 0x1);
    osal_printf("TCPCS\t: %d",          (pDesc->info >> TX_DESC_INFO_TCPCS_OFFSET) & 0x1);
    osal_printf("\tCRC\t: %d",          (pDesc->info >> TX_DESC_INFO_CRC_OFFSET) & 0x1);
    osal_printf("\tDATA_LEN\t: %d\n",   pDesc->info & DESC_INFO_LEN_MASK);

    osal_printf("ADDR\t: 0x%llx\n", pDesc->buf_addr);

    osal_printf("V_TAG\t: %d",          (pDesc->vlan_info >> DESC_VLAN_INFO_VLAN_TAG_OFFSET) & 0x1);
    osal_printf("\tPRI\t: %d",          (pDesc->vlan_info >> DESC_VLAN_INFO_PRI_OFFSET) & 0x1);
    osal_printf("\tCFI\t: %d\n",        (pDesc->vlan_info >> DESC_VLAN_INFO_CFI_OFFSET) & 0x1);
    osal_printf("VID\t: %d \n",         ((pDesc->vlan_info & 0xf) << 8) | ((pDesc->vlan_info >> 8) & 0xff));


    return RT_ERR_OK;
}



int32   dal_tigershark_oob_desc_dump(uint32 unit, rtk_port_t port, oob_dir_t dir, oob_desc_t* pDesc)
{

    osal_printf("------------- DUMP %s DESC INFO -----------\n", (OOB_DIR_RX == dir) ? "RX" : "TX");
    osal_printf("OWN\t: %d",        (pDesc->info >> DESC_INFO_OWN_OFFSET) & 0x1);
    osal_printf("\tEOR\t: %d",      (pDesc->info >> DESC_INFO_EOR_OFFSET) & 0x1);
    osal_printf("\tFS\t: %d\n",     (pDesc->info >> DESC_INFO_FS_OFFSET) & 0x1);
    osal_printf("LS\t: %d",         (pDesc->info >> DESC_INFO_LS_OFFSET) & 0x1);
    if (OOB_DIR_RX == dir)
    {
        osal_printf("\tFAE\t: %d",      (pDesc->info >> RX_DESC_INFO_FAE_OFFSET) & 0x1);
        osal_printf("\tMAR\t: %d\n",    (pDesc->info >> RX_DESC_INFO_MAR_OFFSET) & 0x1);
        osal_printf("PAM\t: %d",        (pDesc->info >> RX_DESC_INFO_PAM_OFFSET) & 0x1);
        osal_printf("\tBAR\t: %d",      (pDesc->info >> RX_DESC_INFO_BAR_OFFSET) & 0x1);
        osal_printf("\tPPPOE_E8023\t: %d\n", (pDesc->info >> RX_DESC_INFO_PPPOE_E8023_OFFSET) & 0x3);
        osal_printf("RWT\t: %d",        (pDesc->info >> RX_DESC_INFO_RWT_OFFSET) & 0x1);
        osal_printf("\tRCDF\t: %d",     (pDesc->info >> RX_DESC_INFO_RCDF_OFFSET) & 0x1);
        osal_printf("\tRUNT\t: %d\n",   (pDesc->info >> RX_DESC_INFO_RUNT_OFFSET) & 0x1);
        osal_printf("CRC\t: %d",        (pDesc->info >> RX_DESC_INFO_CRC_OFFSET) & 0x1);
        osal_printf("\tPID\t: %d",      (pDesc->info >> RX_DESC_INFO_PID0_OFFSET) & 0x3);
        osal_printf("\tIPF\t: %d\n",    (pDesc->info >> RX_DESC_INFO_IPF_OFFSET) & 0x1);
        osal_printf("UDPF\t: %d",       (pDesc->info >> RX_DESC_INFO_UDPF_OFFSET) & 0x1);
        osal_printf("\tTCPF\t: %d",     (pDesc->info >> RX_DESC_INFO_TCPF_OFFSET) & 0x1);
        osal_printf("\tIPSEG\t: %d\n",  (pDesc->info >> RX_DESC_INFO_IPSEG_OFFSET) & 0x1);
        osal_printf("IPV6\t: %d",       (pDesc->info >> RX_DESC_INFO_IPV6_OFFSET) & 0x1);
        osal_printf("\tLEN\t: %d",      pDesc->info & RX_DESC_INFO_LEN_MASK);
    }
    else
    {
        osal_printf("\tIPCS\t: %d",         (pDesc->info >> TX_DESC_INFO_IPCS_OFFSET) & 0x1);
        osal_printf("\tUDPCS\t: %d\n",      (pDesc->info >> TX_DESC_INFO_UDPCS_OFFSET) & 0x1);
        osal_printf("TCPCS\t: %d",          (pDesc->info >> TX_DESC_INFO_TCPCS_OFFSET) & 0x1);
        osal_printf("\tCRC\t: %d",          (pDesc->info >> TX_DESC_INFO_CRC_OFFSET) & 0x1);
    }

    osal_printf("\tADDR\t: 0x%llx\n", pDesc->buf_addr);
    osal_printf("V_TAG\t: %d",      (pDesc->vlan_info >> DESC_VLAN_INFO_VLAN_TAG_OFFSET) & 0x1);
    osal_printf("\tPRI\t: %d",      (pDesc->vlan_info >> DESC_VLAN_INFO_PRI_OFFSET) & 0x1);
    osal_printf("\tCFI\t: %d\n",    (pDesc->vlan_info >> DESC_VLAN_INFO_CFI_OFFSET) & 0x1);
    osal_printf("VID\t: %d\n",        ((pDesc->vlan_info & 0xf) << 8) | ((pDesc->vlan_info >> 8) & 0xff));

    return RT_ERR_OK;

}



int32 dal_tigershark_oob_hw_info_dump(uint32 unit, rtk_port_t port)
{
    uint32  temp = 0;
    hol_ring_thr_t thr;
    osal_printf("------------- DUMP ETHERNET MODULE REG -----------\n");
    _ioal_soc_mem32_field_read(unit, RTL9311B_TRX_OK_CNT_ADDR, RTL9311B_TRX_OK_CNT_txok_OFFSET, RTL9311B_TRX_OK_CNT_txok_MASK, &temp);
    osal_printf("TX_OK_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9311B_TRX_OK_CNT_ADDR, RTL9311B_TRX_OK_CNT_rxok_OFFSET, RTL9311B_TRX_OK_CNT_rxok_MASK, &temp);
    osal_printf("\tRX_OK_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9311B_TRXERR_CNT_ADDR, RTL9311B_TRXERR_CNT_Txerr_OFFSET, RTL9311B_TRXERR_CNT_Txerr_MASK, &temp);
    osal_printf("TX_ERR_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9311B_TRXERR_CNT_ADDR, RTL9311B_TRXERR_CNT_Rxerr_OFFSET, RTL9311B_TRXERR_CNT_Rxerr_MASK, &temp);
    osal_printf("\tRX_ERR_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9311B_MISSPKT_CNT_ADDR, RTL9311B_MISSPKT_CNT_MissPkt_OFFSET, RTL9311B_MISSPKT_CNT_MissPkt_MASK, &temp);
    osal_printf("MISS_PKT_CNT\t: %d", temp);
//    _ioal_soc_mem32_field_read(unit, RTL9311B_MISSPKT_CNT_ADDR, RTL9311B_MISSPKT_CNT_FAE_OFFSET, RTL9311B_MISSPKT_CNT_FAE_MASK, &temp);
    osal_printf("\tALGN_ERR_CNT\t: NOT SUPPORT\n");

    _ioal_soc_mem32_field_read(unit, RTL9311B_TX_COL_CNT_ADDR, RTL9311B_TX_COL_CNT_Tx1Col_OFFSET, RTL9311B_TX_COL_CNT_Tx1Col_MASK, &temp);
    osal_printf("COL_1_TX_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9311B_TX_COL_CNT_ADDR, RTL9311B_TX_COL_CNT_TxMCol_OFFSET, RTL9311B_TX_COL_CNT_TxMCol_MASK, &temp);
    osal_printf("\tCOL_M_TX_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9311B_RXOKPHY_CNT_ADDR, RTL9311B_RXOKPHY_CNT_RxOkPhy_OFFSET, RTL9311B_RXOKPHY_CNT_RxOkPhy_MASK, &temp);
    osal_printf("RX_OK_PHY_CNT\t: %d", temp);
//    _ioal_soc_mem32_field_read(unit, RTL9311B_RXOKPHY_CNT_ADDR, RTL9311B_RXOKPHY_CNT_OxOkBrd_OFFSET, RTL9311B_RXOKPHY_CNT_OxOkBrd_MASK, &temp);
    osal_printf("\tRX_OK_BC_CNT\t: NOT SUPPORT\n");

    _ioal_soc_mem32_field_read(unit, RTL9311B_RXOKMUL_CNT_ADDR, RTL9311B_RXOKMUL_CNT_RxOkMul_OFFSET, RTL9311B_RXOKMUL_CNT_RxOkMul_MASK, &temp);
    osal_printf("RX_OK_MC_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9311B_RXOKMUL_CNT_ADDR, RTL9311B_RXOKMUL_CNT_TxAbt_OFFSET, RTL9311B_RXOKMUL_CNT_TxAbt_MASK, &temp);
    osal_printf("\tTX_ABORT_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9311B_TXUNDER_CNT_ADDR, RTL9311B_TXUNDER_CNT_TXUNDERC_OFFSET, RTL9311B_TXUNDER_CNT_TXUNDERC_MASK, &temp);
    osal_printf("TX_UDER_RUN\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9311B_TXUNDER_CNT_ADDR, RTL9311B_TXUNDER_CNT_RDU_MISSPKTC_OFFSET, RTL9311B_TXUNDER_CNT_RDU_MISSPKTC_MASK, &temp);
    osal_printf("\tRDU_MISS_PKT\t: %d\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_TRX_STATUS_ADDR, &temp);
    osal_printf("TRSR\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_COM_ADDR, &temp);
    osal_printf("\tCOM\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_INT_MS_ADDR, &temp);
    osal_printf("IMSR\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_TX_CFG_ADDR, &temp);
    osal_printf("\tTCR\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_ISR1_ADDR, &temp);
    osal_printf("ISR1\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &temp);
    osal_printf("\tRCR\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_IMR0_ADDR, &temp);
    osal_printf("IMR0\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_IMR1_ADDR, &temp);
    osal_printf("\tIMR1\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &temp);
    osal_printf("MSR\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &temp);
    osal_printf("\tMIIAR\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_CONFIG_ADDR, &temp);
    osal_printf("CONFIG\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_INTR_ADDR, &temp);
    osal_printf("\tINTR\t: 0x%08x\n", temp);

//    ioal_soc_mem32_read(unit, RTL9311B_VLAN1_ADDR, &temp);
    osal_printf("VLAN1_REG\t: NOT SUPPORT");
    ioal_soc_mem32_read(unit, RTL9311B_LED_CTRL_ADDR, &temp);
    osal_printf("\tLED_CTRL\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_MEM_BIST_ADDR, &temp);
    osal_printf("MEM_BIST\t: 0x%08x", temp);
//    ioal_soc_mem32_read(unit, RTL9311B_PKT_GEN_ADDR, &temp);
    osal_printf("\tPKT_GEN\t: NOT SUPPORT\n");

    ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &temp);
    osal_printf("MEDIA_STATUS: spd1000:%u spd10:%u linkb:%u txpf:%u rxpf:%u dup:%u nw_comp=%u\n",
                    ((temp & RTL9311B_MEDIA_STATUS_SPEED_1000_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_1000_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_SPEED_10_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_10_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_LINKB_MASK) >> RTL9311B_MEDIA_STATUS_LINKB_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_TXPF_MASK) >> RTL9311B_MEDIA_STATUS_TXPF_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_RXPF_MASK) >> RTL9311B_MEDIA_STATUS_RXPF_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_FULLDUP_MASK) >> RTL9311B_MEDIA_STATUS_FULLDUP_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_NWCOMPLETE_MASK) >> RTL9311B_MEDIA_STATUS_NWCOMPLETE_OFFSET));

    osal_printf("------------- DUMP CPU INTERFACE REG -----------\n");

    ioal_soc_mem32_read(unit, RTL9311B_TXFDP1_ADDR, &temp);
    osal_printf("TXFDP1\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_TXCDO1_ADDR, &temp);
    osal_printf("\tTXCDO1\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_TXFDP2_ADDR, &temp);
    osal_printf("TXFDP2\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_TXCDO2_ADDR, &temp);
    osal_printf("\tTXCDO2\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_TXFDP3_ADDR, &temp);
    osal_printf("TXFDP3\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_TXCDO3_ADDR, &temp);
    osal_printf("\tTXCDO3\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_TXFDP4_ADDR, &temp);
    osal_printf("TXFDP4\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_TXCDO4_ADDR, &temp);
    osal_printf("\tTXCDO4\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_TXFDP5_ADDR, &temp);
    osal_printf("TXFDP5\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_TXCDO5_ADDR, &temp);
    osal_printf("\tTXCDO5\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9311B_RXFDP1_ADDR, &temp);
    osal_printf("RXFDP1\t: 0x%08x", temp);
    _ioal_soc_mem32_field_read(unit, RTL9311B_RXCDO1_ADDR, RTL9311B_RXCDO1_RxCDO1_OFFSET, RTL9311B_RXCDO1_RxCDO1_MASK, &temp);
    osal_printf("\tRXCDO1\t: 0x%08x\n", temp);

    dal_tigershark_oob_ringSize_get(unit, port, OOB_DIR_RX, 0, &temp);
    osal_printf("RX_RingSize\t: %d\n", temp);

    dal_tigershark_oob_cpuPtr_get(unit, port, OOB_DIR_RX, 0, &temp);
    osal_printf("EthrntRxCPU_Des_Num (RX_CPU_PTR)\t: %d\n", temp);

    dal_tigershark_oob_holRingThr_get(unit, port, OOB_DIR_RX, 0, &thr);
    osal_printf("Rx_Pse_Des_Thres_on (HOL)\t: %d", thr.high);
    osal_printf("\tRx_Pse_Des_Thres_off (HOL)\t: %d\n", thr.low);

    ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &temp);
    osal_printf("MEDIA_STATUS: spd1000:%u spd10:%u linkb:%u txpf:%u rxpf:%u dup:%u nw_comp=%u\n",
                    ((temp & RTL9311B_MEDIA_STATUS_SPEED_1000_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_1000_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_SPEED_10_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_10_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_LINKB_MASK) >> RTL9311B_MEDIA_STATUS_LINKB_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_TXPF_MASK) >> RTL9311B_MEDIA_STATUS_TXPF_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_RXPF_MASK) >> RTL9311B_MEDIA_STATUS_RXPF_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_FULLDUP_MASK) >> RTL9311B_MEDIA_STATUS_FULLDUP_OFFSET),
                    ((temp & RTL9311B_MEDIA_STATUS_NWCOMPLETE_MASK) >> RTL9311B_MEDIA_STATUS_NWCOMPLETE_OFFSET));


    ioal_soc_mem32_read(unit, RTL9311B_ETN_IO_CMD_ADDR, &temp);
    osal_printf("IO_CMD\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9311B_IO_CMD1_ADDR, &temp);
    osal_printf("\tIO_CMD1\t: 0x%08x\n", temp);


    return RT_ERR_OK;
}


int32    dal_tigershark_oob_tx_kick(uint32 unit, rtk_port_t port, uint32 ringId)
{

    if (ringId < 4)
    {
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_EN_IO_CMD_TXF1_4_OFFSET, RTL9311B_EN_IO_CMD_TXF1_4_MASK, 0x1 << ringId);
    }
    else
    {
        _ioal_soc_mem32_field_write(unit, RTL9311B_IO_CMD1_ADDR, RTL9311B_IO_CMD1_TxFN5th_OFFSET, RTL9311B_IO_CMD1_TxFN5th_MASK, 1);
    }

    return RT_ERR_OK;
}


int32   dal_tigershark_oob_supportJumboSize_get(uint32 unit, rtk_port_t port, uint32* pRXval, uint32* pTXval)
{
    *pRXval = RX_JUMBO_SIZE;
    *pTXval = TX_JUMBO_SIZE;

    return RT_ERR_OK;
}

int32   dal_tigershark_oob_descSize_get(uint32 unit, rtk_port_t port, uint32* pRXval, uint32* pTXval)
{
    *pRXval = RX_DESC_SIZE;
    *pTXval = TX_DESC_SIZE;

    return RT_ERR_OK;
}





int32    dal_tigershark_oob_ethModuleEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  data = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    if (DISABLED == enable)
    {

    }
    else
    {

        OOB_SEM_LOCK(unit);

        _ioal_soc_mem32_field_write(unit, RTL9311B_MEDIA_STATUS_ADDR, RTL9311B_MEDIA_STATUS_FORCE_FULLDUP_OFFSET, RTL9311B_MEDIA_STATUS_FORCE_FULLDUP_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_MEDIA_STATUS_ADDR, RTL9311B_MEDIA_STATUS_FORCE_SPD_OFFSET, RTL9311B_MEDIA_STATUS_FORCE_SPD_MASK, 0x2);
        _ioal_soc_mem32_field_write(unit, RTL9311B_MEDIA_STATUS_ADDR, RTL9311B_MEDIA_STATUS_FORCE_LINK_OFFSET, RTL9311B_MEDIA_STATUS_FORCE_LINK_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_MEDIA_STATUS_ADDR, RTL9311B_MEDIA_STATUS_force_spd_mode_OFFSET, RTL9311B_MEDIA_STATUS_force_spd_mode_MASK, 1);


        _ioal_soc_mem32_field_write(unit, RTL9311B_MEDIA_STATUS_ADDR, RTL9311B_MEDIA_STATUS_Force_TRXFCE_OFFSET, RTL9311B_MEDIA_STATUS_Force_TRXFCE_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_MEDIA_STATUS_ADDR, RTL9311B_MEDIA_STATUS_RXFCE_OFFSET, RTL9311B_MEDIA_STATUS_RXFCE_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_MEDIA_STATUS_ADDR, RTL9311B_MEDIA_STATUS_TXFCE_OFFSET, RTL9311B_MEDIA_STATUS_TXFCE_MASK, 1);


        ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &data);

        /* CRC check config: always check CRC, remove vlan tag*/
        _ioal_soc_mem32_field_write(unit, RTL9311B_COM_ADDR, RTL9311B_COM_rxchksum_OFFSET, RTL9311B_COM_rxchksum_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_COM_ADDR, RTL9311B_COM_rxjumbo_OFFSET, RTL9311B_COM_rxjumbo_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_COM_ADDR, RTL9311B_COM_rxvlan_OFFSET, RTL9311B_COM_rxvlan_MASK, 1);


        /* config tx */
        _ioal_soc_mem32_field_write(unit, RTL9311B_TX_CFG_ADDR, RTL9311B_TX_CFG_r_tx_jumbo_OFFSET, RTL9311B_TX_CFG_r_tx_jumbo_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_TX_CFG_ADDR, RTL9311B_TX_CFG_r_tx_multipkt_cnt_pause_OFFSET, RTL9311B_TX_CFG_r_tx_multipkt_cnt_pause_MASK, 0);
        _ioal_soc_mem32_field_write(unit, RTL9311B_TX_CFG_ADDR, RTL9311B_TX_CFG_r_tx_nopadding_OFFSET, RTL9311B_TX_CFG_r_tx_nopadding_MASK, 0);
        _ioal_soc_mem32_field_write(unit, RTL9311B_ETN_IO_CMD_ADDR, RTL9311B_ETN_IO_CMD_en_early_tx_OFFSET, RTL9311B_ETN_IO_CMD_en_early_tx_MASK, 1);

        /*config MAR*/
        ioal_soc_mem32_write(unit, RTL9311B_MAR0_ADDR, 0xffffffff);
        ioal_soc_mem32_write(unit, RTL9311B_MAR4_ADDR, 0xffffffff);


        /*config RCR*/
        _ioal_soc_mem32_field_write(unit, RTL9311B_RX_CFG_ADDR, RTL9311B_RX_CFG_AER_OFFSET, RTL9311B_RX_CFG_AER_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_RX_CFG_ADDR, RTL9311B_RX_CFG_AR_OFFSET, RTL9311B_RX_CFG_AR_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_RX_CFG_ADDR, RTL9311B_RX_CFG_AB_OFFSET, RTL9311B_RX_CFG_AB_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_RX_CFG_ADDR, RTL9311B_RX_CFG_AM_OFFSET, RTL9311B_RX_CFG_AM_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_RX_CFG_ADDR, RTL9311B_RX_CFG_APM_OFFSET, RTL9311B_RX_CFG_APM_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9311B_RX_CFG_ADDR, RTL9311B_RX_CFG_AAP_OFFSET, RTL9311B_RX_CFG_AAP_MASK, 1);

        /*config CONFIG*/
        _ioal_soc_mem32_field_write(unit, RTL9311B_CONFIG_ADDR, RTL9311B_CONFIG_r_en_rff_afull_OFFSET, RTL9311B_CONFIG_r_en_rff_afull_MASK, 0);

        OOB_SEM_UNLOCK(unit);
    }



    return RT_ERR_OK;
}


int32   dal_tigershark_oob_swOobRst_set(uint32 unit, rtk_port_t port)
{
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    _ioal_soc_mem32_field_write(unit, RTL9311B_COM_ADDR, RTL9311B_COM_rst_OFFSET, RTL9311B_COM_rst_MASK, 1);
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}

int32   dal_tigershark_oob_ringEnable_set(uint32  unit, rtk_port_t port, oob_dir_t dir, uint32 ringId, rtk_enable_t enable)
{

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        _ioal_soc_mem32_field_write(unit, rx_ring_enable_addr[ringId].reg, rx_ring_enable_addr[ringId].offset, rx_ring_enable_addr[ringId].mask, enable);
    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}

/* Function Name:
 *      _dal_tigershark_oob_busyWait_ready
 * Description:
 *      Call chk_ready_f until it returns RT_ERR_OK or until timeout.
 * Input:
 *      unit - unit id
 *      chk_ready_f - function pointer for checking ready bit.
 *                    return RT_ERR_NOT_FINISH, the utility will continue calls chk_ready_f.
 *                    return RT_ERR_OK, the waiting reg value is as expected.
 *                    return any other fails, the utility will return the error code.
 *      pData - user data which will pass to chk_ready_f
 *      timeout - timeout time in microseconds
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_TIMEOUT
 * Note:
 *      None
 */
int32
_dal_tigershark_oob_busyWait_ready(uint32 unit, int32 (*chk_ready_f)(uint32 unit, void *pData), void *pData, osal_usecs_t timeout_us/* microseconds */)
{
    osal_usecs_t    t, now, t_wait=0, t_max;
    int32           chkCnt=0;
    int32           ret;

    osal_memset(&t_max, 0xFF, sizeof(osal_usecs_t));
    osal_time_usecs_get(&t);
    do {
        chkCnt++;
        osal_time_usecs_get(&now);

        ret = (chk_ready_f)(unit, pData);
        switch (ret)
        {
          case RT_ERR_NOT_FINISH:
            break;
          case RT_ERR_OK:
          default:
            //RT_LOG(LOG_DEBUG, MOD_OOB, "OOB busyWait_ready done(%x) chkCnt = %u", ret, chkCnt);
            return ret;
        }

        t_wait += ((now >= t) ? (now - t) : (t_max - t + now));
        t = now;
    } while (t_wait <= timeout_us);

    if (t_wait > timeout_us)
    {
        ret = RT_ERR_TIMEOUT;
        RT_LOG(LOG_DEBUG, MOD_OOB, "OOB busyWait_ready timeout %lu chkCnt = %u", (uint64)timeout_us, chkCnt);
        return ret;
    }

    return RT_ERR_OK;
}


int32
_dal_tigershark_oob_sdsIndrAccComplete_chk(uint32 unit, void *pData)
{
    uint32 regVal, *wait_bit_val = pData;
    int32 ret;

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_SDS_CFG1_ADDR, &regVal)) != RT_ERR_OK)
    {
        return ret;
    }

    if (*wait_bit_val == (regVal & RTL9311B_SDS_CFG1_Sds_reg_rdy_MASK))
    {
        return RT_ERR_OK;
    }
    return RT_ERR_NOT_FINISH;
}


uint32
_dal_tigershark_oob_sds_indr_cmd(uint32 val, uint32 sds_ce_out, uint32 sds_cmd, uint32 sds_cen_in, uint32 sds_we_in)
{
    //ce_out is cd_out
    val = (val & (~RTL9311B_SDS_CFG1_Sds_cd_out_MASK)) | ((sds_ce_out << RTL9311B_SDS_CFG1_Sds_cd_out_OFFSET) & RTL9311B_SDS_CFG1_Sds_cd_out_MASK);
    val = (val & (~RTL9311B_SDS_CFG1_Sds_cmd_MASK)) | ((sds_cmd << RTL9311B_SDS_CFG1_Sds_cmd_OFFSET) & RTL9311B_SDS_CFG1_Sds_cmd_MASK);
    val = (val & (~RTL9311B_SDS_CFG1_Sds_cen_in_MASK)) | ((sds_cen_in << RTL9311B_SDS_CFG1_Sds_cen_in_OFFSET) & RTL9311B_SDS_CFG1_Sds_cen_in_MASK);
    val = (val & (~RTL9311B_SDS_CFG1_Sds_we_in_MASK)) | ((sds_we_in << RTL9311B_SDS_CFG1_Sds_we_in_OFFSET) & RTL9311B_SDS_CFG1_Sds_we_in_MASK);
    return val;
}

/* Function Name:
 *      dal_tigershark_oob_sdsReg_get
 * Description:
 *      Get SerDes register through indirect access
 * Input:
 *      unit    - unit id
 *      sdsId   - SerDes index
 *      page    - page
 *      reg     - register index/address
 * Output:
 *      pData   - register value
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
dal_tigershark_oob_sdsReg_get(uint32 unit, uint32 sds, uint32 page, uint32 reg, uint32 *data)
{
    int32 ret;
    uint32 val, fVal;

    OOB_SEM_LOCK(unit);
    val = 0;
    /* 1)
     *  Sds_a_in format: {sds_idx, sds_page, sds_reg}
     *      [15:11]: sds idx. Always 0 for OOB
     *      [10:05]: sds_page
     *      [04:00]: sds_reg
     */
    fVal = ((page & 0x3F) << 5) | (reg & 0x1F);
    val = (val & (~RTL9311B_SDS_CFG2_Sds_a_in_MASK)) | ((fVal << RTL9311B_SDS_CFG2_Sds_a_in_OFFSET) & RTL9311B_SDS_CFG2_Sds_a_in_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_SDS_CFG2_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

    /* 2)
     *  sds_cen_in set 1
     *  sds_we_in set 0
     */
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_SDS_CFG1_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }
    val = _dal_tigershark_oob_sds_indr_cmd(0, 0 /*sds_ce_out*/,  0 /*sds_cmd*/, 1 /*sds_cen_in*/, 0 /*sds_we_in*/);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_SDS_CFG1_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

    /* 3)
     *  sds_cen_in set 0
     *  sds_we_in set 0
     */
    val = _dal_tigershark_oob_sds_indr_cmd(0, 0 /*sds_ce_out*/,  0 /*sds_cmd*/, 0 /*sds_cen_in*/, 0 /*sds_we_in*/);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_SDS_CFG1_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_SDS_CFG2_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }
    *data = (val & RTL9311B_SDS_CFG2_Sds_dio_MASK) >> RTL9311B_SDS_CFG2_Sds_dio_OFFSET;

  out:
    /* 5)
     *  sds_cen_in set 0
     *  sds_we_in set 0
     */
    ioal_soc_mem32_read(unit, RTL9311B_SDS_CFG1_ADDR, &val);
    val = _dal_tigershark_oob_sds_indr_cmd(val, 0 /*sds_ce_out*/,  0 /*sds_cmd*/, 0 /*sds_cen_in*/, 0 /*sds_we_in*/);
    ioal_soc_mem32_write(unit, RTL9311B_SDS_CFG1_ADDR, val);

    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_sdsReg_set
 * Description:
 *      Set SerDes register through indirect access
 * Input:
 *      unit    - unit id
 *      sdsId   - SerDes index
 *      page    - page
 *      reg     - register index/address
 *      data    - register value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
dal_tigershark_oob_sdsReg_set(uint32 unit, uint32 sds, uint32 page, uint32 reg, uint32 data)
{
    int32 ret;
    uint32 val, fVal;

    OOB_SEM_LOCK(unit);
    val = 0;
    /* 1)
     *  Sds_a_in format: {sds_idx, sds_page, sds_reg}
     *      [15:11]: sds idx. Always 0 for OOB
     *      [10:05]: sds_page
     *      [04:00]: sds_reg
     *  Sds_dio = data
     */
    fVal = ((page & 0x3F) << 5) | (reg & 0x1F);
    val = (val & (~RTL9311B_SDS_CFG2_Sds_a_in_MASK)) | ((fVal << RTL9311B_SDS_CFG2_Sds_a_in_OFFSET) & RTL9311B_SDS_CFG2_Sds_a_in_MASK);
    val = (val & (~RTL9311B_SDS_CFG2_Sds_dio_MASK)) | ((data << RTL9311B_SDS_CFG2_Sds_dio_OFFSET) & RTL9311B_SDS_CFG2_Sds_dio_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_SDS_CFG2_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

    /* 2)
     *  sds_cen_in set 1
     *  sds_we_in set 1
     */
    val = _dal_tigershark_oob_sds_indr_cmd(0, 0 /*sds_ce_out*/,  0 /*sds_cmd*/, 1 /*sds_cen_in*/, 1 /*sds_we_in*/);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_SDS_CFG1_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

  out:
    /* 4)
     *  sds_cen_in set 0
     *  sds_we_in set 0
     */
    /* dummy read */
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_SDS_CFG1_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
    }
    val = _dal_tigershark_oob_sds_indr_cmd(0, 0 /*sds_ce_out*/,  0 /*sds_cmd*/, 0 /*sds_cen_in*/, 0 /*sds_we_in*/);
    ioal_soc_mem32_write(unit, RTL9311B_SDS_CFG1_ADDR, val);

    OOB_SEM_UNLOCK(unit);
    return ret;
}


int32
_dal_tigershark_oob_sdsPatch_set(uint32 unit, int32 sds_id)
{
    int32 i;
    uint32  data, mask;
    oob_sdsPatchConfType_t  *pSp;

    for (i = 0, pSp = oob_sds_patch_config; i < sizeof(oob_sds_patch_config)/sizeof(oob_sdsPatchConfType_t); i++, pSp++)
    {
        dal_tigershark_oob_sdsReg_get(unit, sds_id, SERDES_GET_PAGE(pSp->page), SERDES_GET_REG(pSp->reg), &data);
        mask = UINT32_BITS_MASK(SERDES_GET_BIT(pSp->msb), SERDES_GET_BIT(pSp->lsb));
        data = REG32_FIELD_SET(data, OOB_SERDES_GET_VAL(pSp->val), SERDES_GET_BIT(pSp->lsb), mask);
        dal_tigershark_oob_sdsReg_set(unit, sds_id, SERDES_GET_PAGE(pSp->page), SERDES_GET_REG(pSp->reg), data);
    }

    return RT_ERR_OK;
}


int32
_dal_tigershark_oob_sdsMode_set(uint32 unit, int32 sds_id, rt_serdesMode_t sds_mode)
{
    uint32  val = 0, fVal = 0;
    int32 ret;

    switch(sds_mode)
    {
      case RTK_MII_SGMII:
        fVal = 0x2;
        break;
      case RTK_MII_1000BX_FIBER:
        fVal = 0x4;
        break;
      case RTK_MII_100BX_FIBER:
        fVal = 0x5;
        break;
      default:
        break;
    }
    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB Sds Mode=%u\n", sds_mode);

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_OOBSR2_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9311B_OOBSR2_sds_mode0_MASK)) | ((fVal << RTL9311B_OOBSR2_sds_mode0_OFFSET) & RTL9311B_OOBSR2_sds_mode0_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_OOBSR2_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
  out:
    return ret;
}

int32
_dal_tigershark_oob_mii_init(uint32 unit, rtk_port_t port)
{
    uint32  val = 0, fVal;
    int32 ret = RT_ERR_OK;

    /* set PHY address */
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &val)) != RT_ERR_OK)
    {
        goto out;
    }

    fVal = HWP_PHY_ADDR(unit, port);
    val = (val & (~RTL9311B_MII_ACCESS_PHYAddress_MASK)) | ((fVal << RTL9311B_MII_ACCESS_PHYAddress_OFFSET) & RTL9311B_MII_ACCESS_PHYAddress_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MII_ACCESS_ADDR, val)) != RT_ERR_OK)
    {
        goto out;
    }

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB PHY addr %u\n", fVal);

  out:
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_mgmtMacAddr_get
 * Description:
 *      Get MAC address of OOB
 * Input:
 *      unit - unit id
 *      port - OOB port id
 * Output:
 *      pMac - management MAC address
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_mgmtMacAddr_get(uint32 unit, rtk_port_t port, rtk_mac_t *pMac)
{
    int32 ret;
    uint32 val;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_IDR0_ADDR , &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    pMac->octet[0] = (val >> 24) & 0xff;
    pMac->octet[1] = (val >> 16) & 0xff;
    pMac->octet[2] = (val >> 8) & 0xff;
    pMac->octet[3] = (val >> 0) & 0xff;

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_IDR4_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
    pMac->octet[4] = (val >> 24) & 0xff;
    pMac->octet[5] = (val >> 16) & 0xff;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_mgmtMacAddr_set
 * Description:
 *      Set MAC address of OOB
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      pMac - management MAC address
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_mgmtMacAddr_set(uint32 unit, rtk_port_t port, rtk_mac_t *pMac)
{
    int32 ret;
    uint32 val;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    RT_LOG(LOG_DEBUG, MOD_BSP, "pMac=%x-%x-%x-%x-%x-%x",
           pMac->octet[0], pMac->octet[1], pMac->octet[2],
           pMac->octet[3], pMac->octet[4], pMac->octet[5]);

    val = (pMac->octet[0] << 24) | (pMac->octet[1] << 16) | (pMac->octet[2] << 8) | (pMac->octet[3] << 0);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_IDR0_ADDR , val)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_BSP), "unit %u", unit);
        goto out;
    }

    val = (pMac->octet[4] << 24) | (pMac->octet[5] << 16);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_IDR4_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_BSP), "unit %u", unit);
        goto out;
    }

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & ~RTL9311B_RX_CFG_APM_MASK) | (1 << RTL9311B_RX_CFG_APM_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_RX_CFG_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_rxMode_get
 * Description:
 *      Configure receive mode
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      rx_mode - receive mode
 * Output:
 *      pEnable - enable/disabled specified mode
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_rxMode_get(uint32 unit, rtk_port_t port, drv_oob_rxMode_t rx_mode, rtk_enable_t *pEnable)
{
    uint32 val, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    switch (rx_mode)
    {
      case OOB_RX_MODE_ACCEPT_ALLUCAST:
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        fVal = (val & RTL9311B_RX_CFG_AAP_MASK) >> RTL9311B_RX_CFG_AAP_OFFSET;
        *pEnable = (fVal == 1) ? ENABLED : DISABLED;
        break;
      case OOB_RX_MODE_ACCEPT_ALLMCAST:
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        fVal = (val & RTL9311B_RX_CFG_APM_MASK) >> RTL9311B_RX_CFG_APM_OFFSET;
        *pEnable = (fVal == 1) ? ENABLED : DISABLED;
        break;
      case OOB_RX_MODE_ACCEPT_BCAST:
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        fVal = (val & RTL9311B_RX_CFG_AB_MASK) >> RTL9311B_RX_CFG_AB_OFFSET;
        *pEnable = (fVal == 1) ? ENABLED : DISABLED;
        break;
      default:
        ret = RT_ERR_INPUT;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_rxMode_set
 * Description:
 *      Configure receive mode
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      rx_mode - receive mode
 *      enable - enable/disabled specified mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_rxMode_set(uint32 unit, rtk_port_t port, drv_oob_rxMode_t rx_mode, rtk_enable_t enable)
{
    uint32 val, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    switch (rx_mode)
    {
      case OOB_RX_MODE_ACCEPT_ALLUCAST:
        fVal = (enable == ENABLED) ? 1 : 0;
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9311B_RX_CFG_AAP_MASK) | (fVal << RTL9311B_RX_CFG_AAP_OFFSET);
        if ((ret = ioal_soc_mem32_write(unit, RTL9311B_RX_CFG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        break;
      case OOB_RX_MODE_ACCEPT_ALLMCAST:
        fVal = (enable == ENABLED) ? 1 : 0;

        ioal_soc_mem32_write(unit, RTL9311B_MAR0_ADDR, 0xFFFFFFFF);
        ioal_soc_mem32_write(unit, RTL9311B_MAR4_ADDR, 0xFFFFFFFF);
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9311B_RX_CFG_APM_MASK) | (fVal << RTL9311B_RX_CFG_APM_OFFSET);
        if ((ret = ioal_soc_mem32_write(unit, RTL9311B_RX_CFG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        break;
      case OOB_RX_MODE_ACCEPT_BCAST:
        fVal = (enable == ENABLED) ? 1 : 0;
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9311B_RX_CFG_AB_MASK) | (fVal << RTL9311B_RX_CFG_AB_OFFSET);
        if ((ret = ioal_soc_mem32_write(unit, RTL9311B_RX_CFG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        break;
      default:
        ret = RT_ERR_INPUT;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_jumboModeEnable_get
 * Description:
 *      Configure MAC of supporting jumbo frame
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      dir - TX or RX
 * Output:
 *      pEnable - ENABLED to support jumbo frame
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_jumboModeEnable_get(uint32 unit, rtk_port_t port, rtk_switch_pktDir_t dir, rtk_enable_t *pEnable)
{
    uint32  val = 0;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    switch (dir)
    {
      case PKTDIR_TX:
        //TCR
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_TX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        *pEnable = (val & RTL9311B_TX_CFG_r_tx_jumbo_MASK) ? ENABLED : DISABLED;
        break;

      case PKTDIR_RX:
      default:
        ret = RT_ERR_INPUT;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

int32
dal_tigershark_oob_jumboModeEnable_set(uint32 unit, rtk_port_t port, rtk_switch_pktDir_t dir, rtk_enable_t enable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    switch (dir)
    {
      case PKTDIR_TX:
        fVal = (enable == ENABLED) ? 1 : 0;

        //TCR
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_TX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9311B_TX_CFG_r_tx_jumbo_MASK) | (fVal << RTL9311B_TX_CFG_r_tx_jumbo_OFFSET);

        if ((ret = ioal_soc_mem32_write(unit, RTL9311B_TX_CFG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        //CONFIG_REG
        if ((ret = ioal_soc_mem32_read(unit, RTL9311B_CONFIG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9311B_CONFIG_r_en_tx_extra_MASK) | (fVal << RTL9311B_CONFIG_r_en_tx_extra_OFFSET);

        if ((ret = ioal_soc_mem32_write(unit, RTL9311B_CONFIG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        break;

      case PKTDIR_RX:
      default:
        ret = RT_ERR_INPUT;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_pollingPhyEnable_get
 * Description:
 *      Get MAC auto polling PHY PCS status configuration.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 * Output:
 *      pEnable - enable/disable MAC auto polling PHY.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_pollingPhyEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
    fVal = (val & RTL9311B_MII_ACCESS_Disable_auto_polling_MASK) >> RTL9311B_MII_ACCESS_Disable_auto_polling_OFFSET;
    *pEnable = (fVal == 1) ? DISABLED : ENABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_pollingPhyEnable_set
 * Description:
 *      Configure MAC to auto polling PHY PCS status.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      enable - enable/disable MAC auto polling PHY.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_pollingPhyEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal = (enable == ENABLED) ? 0 : 1;
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9311B_MII_ACCESS_Disable_auto_polling_MASK)) | (fVal << RTL9311B_MII_ACCESS_Disable_auto_polling_OFFSET);

    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MII_ACCESS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}


/* Function Name:
 *      dal_tigershark_oob_pollingPhyStatus_get
 * Description:
 *      Get MAC to auto polling PHY's result status
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      enable - enable/disable MAC auto polling PHY.
 * Output:
 *      pStatus - polling restult status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_pollingPhyStatus_get(uint32 unit, rtk_port_t port, oob_pollingPhyStatus_t *pStatus)
{
    uint32 val = 0;
    uint32 spd10, spd1000;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    pStatus->link = ((val & RTL9311B_MEDIA_STATUS_LINKB_MASK) == 0) ? PORT_LINKUP : PORT_LINKDOWN;
    if (pStatus->link == PORT_LINKUP)
    {
        /* get speed */
        spd10 = (val & RTL9311B_MEDIA_STATUS_SPEED_10_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_10_OFFSET;
        spd1000 = (val & RTL9311B_MEDIA_STATUS_SPEED_1000_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_1000_OFFSET;
        if ((spd1000 == 1) && (spd10 == 0)) //1000M
        {
            pStatus->speed = PORT_SPEED_1000M;
        }
        else if ((spd1000 == 0) && (spd10 == 0))    //100M
        {
            pStatus->speed = PORT_SPEED_100M;
        }
        else if ((spd1000 == 0) && (spd10 == 1))    //10M
        {
            pStatus->speed = PORT_SPEED_10M;
        }
        else
        {
            RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port %u oob media_status 0x%x spd 2b11 out of range", unit, port, val);
            ret = RT_ERR_OUT_OF_RANGE;
            goto out;
        }

        pStatus->duplex = (val & RTL9311B_MEDIA_STATUS_FULLDUP_MASK) ? PORT_FULL_DUPLEX : PORT_HALF_DUPLEX;
    }
    else
    {
        pStatus->speed = PORT_SPEED_10M;
        pStatus->duplex = PORT_HALF_DUPLEX;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_forceFc_get
 * Description:
 *      Force MAC flow control status.
 *      When PHY's local pause is 0 and link partner's pause is 0,
 *      this API is able to configure MAC to tx/rx pause.
 *      Disable forceFc to will use polling PHY result.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 * Output:
 *      pForceFc - ENABLED: force flow control.
 *                DISABLED: disable force flow control, MAC will use polling PHY status to tx/rx pause.
 *      pTxPause - enable/disable TX pause frame
 *      pRxPause - enable/disable RX pause frame
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceFc_get(uint32 unit, rtk_port_t port, rtk_enable_t *pForceFc, rtk_enable_t *pTxPause, rtk_enable_t *pRxPause)
{
    uint32  val = 0, fVal_forceFc, fVal_txPause, fVal_rxPause;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    fVal_forceFc = (val & RTL9311B_MEDIA_STATUS_Force_TRXFCE_MASK) >> RTL9311B_MEDIA_STATUS_Force_TRXFCE_OFFSET;
    fVal_txPause = (val & RTL9311B_MEDIA_STATUS_TXFCE_MASK) >> RTL9311B_MEDIA_STATUS_TXFCE_OFFSET;
    fVal_rxPause = (val & RTL9311B_MEDIA_STATUS_RXFCE_MASK) >> RTL9311B_MEDIA_STATUS_RXFCE_OFFSET;

    *pForceFc = (fVal_forceFc == 1) ? ENABLED : DISABLED;
    *pTxPause = (fVal_txPause == 1) ? ENABLED : DISABLED;
    *pRxPause = (fVal_rxPause == 1) ? ENABLED : DISABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_forceFc_set
 * Description:
 *      Force MAC flow control status.
 *      When PHY's local pause is 0 and link partner's pause is 0,
 *      this API is able to configure MAC to tx/rx pause.
 *      Disable forceFc to will use polling PHY result.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      forceFc - ENABLED: force flow control.
 *                DISABLED: disable force flow control, MAC will use polling PHY status to tx/rx pause.
 *      txPause - enable/disable TX pause frame
 *      rxPause - enable/disable RX pause frame
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceFc_set(uint32 unit, rtk_port_t port, rtk_enable_t forceFc, rtk_enable_t txPause, rtk_enable_t rxPause)
{
    uint32  val = 0, fVal_forceFc, fVal_txPause, fVal_rxPause;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal_forceFc = (forceFc == ENABLED) ? 1 : 0;
    fVal_txPause = (txPause == ENABLED) ? 1 : 0;
    fVal_rxPause = (rxPause == ENABLED) ? 1 : 0;

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9311B_MEDIA_STATUS_Force_TRXFCE_MASK)) | (fVal_forceFc << RTL9311B_MEDIA_STATUS_Force_TRXFCE_OFFSET);
    val = (val & (~RTL9311B_MEDIA_STATUS_TXFCE_MASK)) | (fVal_txPause << RTL9311B_MEDIA_STATUS_TXFCE_OFFSET);
    val = (val & (~RTL9311B_MEDIA_STATUS_RXFCE_MASK)) | (fVal_rxPause << RTL9311B_MEDIA_STATUS_RXFCE_OFFSET);

    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_forceFullDuplex_get
 * Description:
 *      Get force MAC in full duplex mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 * Output:
 *      pEnable - ENABLED, force MAC in full duplex mode.
 *               DISABLED, duplex status will get from polling PHY status.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceFullDuplex_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
    fVal = (val & RTL9311B_MEDIA_STATUS_FORCE_FULLDUP_MASK) >> RTL9311B_MEDIA_STATUS_FORCE_FULLDUP_OFFSET;
    *pEnable = (fVal == 1) ? ENABLED: DISABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;

}

/* Function Name:
 *      dal_tigershark_oob_forceFullDuplex_set
 * Description:
 *      Force MAC in full duplex mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      enable - ENABLED, force MAC in full duplex mode.
 *               DISABLED, duplex status will get from polling PHY status.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceFullDuplex_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal = (enable == ENABLED) ? 1 : 0;
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9311B_MEDIA_STATUS_FORCE_FULLDUP_MASK)) | (fVal << RTL9311B_MEDIA_STATUS_FORCE_FULLDUP_OFFSET);

    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_forceSpeed_get
 * Description:
 *      Get force MAC speed configuration.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 * Output:
 *      pForceSpd - ENABLED, enable force MAC speed.
 *                 DISABLED, disable force speed, speed will obtain from polling PHY status.
 *      pSpeed - configure speed when forceSpd is ENABLED.
 *              When DISABLED, will always return PORT_SPEED_10M.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceSpeed_get(uint32 unit, rtk_port_t port, rtk_enable_t *pForceSpd, rtk_port_speed_t *pSpeed)
{
    uint32  val = 0, fVal_spd;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
         goto out;
    }

    *pForceSpd = (val & RTL9311B_MEDIA_STATUS_force_spd_mode_MASK) ? ENABLED : DISABLED;

    fVal_spd = (val & RTL9311B_MEDIA_STATUS_FORCE_SPD_MASK) >> RTL9311B_MEDIA_STATUS_FORCE_SPD_OFFSET;
    if (fVal_spd == 0x3)
    {
        *pSpeed = PORT_SPEED_10M;
    }
    else
    {
        if (fVal_spd == 0x2)
        {
            *pSpeed = PORT_SPEED_1000M;
        }
        else if (fVal_spd == 0x0)
        {
            *pSpeed = PORT_SPEED_100M;
        }
        else //0x1
        {
            *pSpeed = PORT_SPEED_10M;
        }
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_forceSpeed_set
 * Description:
 *      Force MAC speed.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      forceSpd - ENABLED, enable force MAC speed.
 *                 DISABLED, disable force speed, speed will obtain from polling PHY status.
 *      speed - configure speed when forceSpd is ENABLED.
 *              When DISABLED, this field will ignore, please use PORT_SPEED_10M.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceSpeed_set(uint32 unit, rtk_port_t port, rtk_enable_t forceSpd, rtk_port_speed_t speed)
{
    uint32  val = 0, fVal_spd, force_mode, sds_ablty;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    if (forceSpd == ENABLED)
    {
        force_mode = 0x1;
        if (speed == PORT_SPEED_1000M)
        {
            fVal_spd = 0x2;
            sds_ablty = 0x2;
        }
        else if (speed == PORT_SPEED_100M)
        {
            fVal_spd = 0x0;
            sds_ablty = 0x1;
        }
        else if (speed == PORT_SPEED_10M)
        {
            fVal_spd = 0x1;
            sds_ablty = 0x0;
        }
        else
        {
            return RT_ERR_INPUT;
        }
    }
    else
    {
        force_mode = 0x0;
        fVal_spd = 0x3;
        sds_ablty = 0x2;//default 1000M
    }

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9311B_MEDIA_STATUS_FORCE_SPD_MASK)) | (fVal_spd << RTL9311B_MEDIA_STATUS_FORCE_SPD_OFFSET);
    val = (val & (~RTL9311B_MEDIA_STATUS_force_spd_mode_MASK)) | (force_mode << RTL9311B_MEDIA_STATUS_force_spd_mode_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    /* set speed to serdes (SS-1832) */
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_OOBSR2_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
    val = (val & (~RTL9311B_OOBSR2_sds0_ablty_MASK)) | (sds_ablty << RTL9311B_OOBSR2_sds0_ablty_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_OOBSR2_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}


/* Function Name:
 *      dal_tigershark_oob_forceLink_get
 * Description:
 *      Get force MAC in link up mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 * Output:
 *      pEnable - ENABLED, force MAC in link up mode.
 *                DISABLED, the MAC's link status is from polling PHY status.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceLink_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    fVal = (val & RTL9311B_MEDIA_STATUS_FORCE_LINK_MASK) >> RTL9311B_MEDIA_STATUS_FORCE_LINK_OFFSET;
    *pEnable = (fVal == 1) ? ENABLED : DISABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_tigershark_oob_forceLink_set
 * Description:
 *      Force MAC in link up mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id
 *      enable - ENABLED, force MAC in link up mode.
 *               DISABLED, the MAC's link status is from polling PHY status.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_forceLink_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal = (enable == ENABLED) ? 1 : 0;
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9311B_MEDIA_STATUS_FORCE_LINK_MASK)) | (fVal << RTL9311B_MEDIA_STATUS_FORCE_LINK_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}


/* Function Name:
 *      dal_tigershark_oob_stat_getAll
 * Description:
 *      Get all counters in the specified device.
 * Input:
 *      unit - unit id
 *      port - OOB port ID
 * Output:
 *      pCntrs - statistic counter
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      Supported counters are as following:
 *      (Counters are not supported if not listed here or mark as X.)
 *      rtk_stat_port_cntr_t \ Chip:              9311B
 *          ifHCInOctets                            O
 *          ifHCInUcastPkts                         O
 *          ifHCInMulticastPkts                     O
 *          ifHCInBroadcastPkts                     O
 *          ifHCOutOctets                           O
 *          ifHCOutUcastPkts                        O
 *          ifHCOutMulticastPkts                    O
 *          ifHCOutBrocastPkts                      O
 *          ifOutDiscards                           O
 *          dot1dTpPortInDiscards                   O
 */
int32
dal_tigershark_oob_stat_getAll(uint32 unit, rtk_port_t port, rtk_stat_port_cntr_t *pCntrs)
{
    uint32  val;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    ioal_soc_mem32_read(unit, RTL9311B_DOT1DTPPORTINDISCARDS_ADDR, &val);
    pCntrs->dot1dTpPortInDiscards = val;

    ioal_soc_mem32_read(unit, RTL9311B_IFLNOCTETS_H_ADDR, &val);
    pCntrs->ifHCInOctets = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFLNOCTETS_L_ADDR, &val);
    pCntrs->ifHCInOctets = (pCntrs->ifHCInOctets << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFLNUCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCInUcastPkts = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFLNUCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCInUcastPkts = (pCntrs->ifHCInUcastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFLNMULTICASTPKTS_H_ADDR, &val);
    pCntrs->ifHCInMulticastPkts = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFLNMULTICASTPKTS_L_ADDR, &val);
    pCntrs->ifHCInMulticastPkts = (pCntrs->ifHCInMulticastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFLNBROADCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCInBroadcastPkts = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFLNBROADCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCInBroadcastPkts = (pCntrs->ifHCInBroadcastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFOUTOCTETS_H_ADDR, &val);
    pCntrs->ifHCOutOctets = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFOUTOCTETS_L_ADDR, &val);
    pCntrs->ifHCOutOctets = (pCntrs->ifHCOutOctets << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFOUTUCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCOutUcastPkts = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFOUTUCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCOutUcastPkts = (pCntrs->ifHCOutUcastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFOUTMULTICASTPKTS_H_ADDR, &val);
    pCntrs->ifHCOutMulticastPkts = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFOUTMULTICASTPKTS_L_ADDR, &val);
    pCntrs->ifHCOutMulticastPkts = (pCntrs->ifHCOutMulticastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFOUTBROADCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCOutBrocastPkts = val;
    ioal_soc_mem32_read(unit, RTL9311B_IFOUTBROADCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCOutBrocastPkts = (pCntrs->ifHCOutBrocastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9311B_IFOUTDISCARDS_ADDR, &val);
    pCntrs->ifOutDiscards = val;

    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}


int32
_dal_tigershark_oob_miimAccComplete_chk(uint32 unit, void *pData)
{
    uint32 regVal, *wait_bit_val = pData;
    int32 ret;

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &regVal)) != RT_ERR_OK)
    {
        return ret;
    }

    if (*wait_bit_val == (regVal & RTL9311B_MII_ACCESS_Flag_MASK))
    {
        return RT_ERR_OK;
    }
    return RT_ERR_NOT_FINISH;
}

/* Function Name:
 *      dal_tigershark_oob_miiBus_read
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit - unit id
 *      mii_bus - bus ID
 *      phy_addr - address of the PHY
 *      phy_reg - PHY register (0 ~ 31)
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_miiBus_read(uint32 unit, uint32 mii_bus, uint32 phy_addr, uint32 phy_reg, uint32 *pData)
{
    int32   ret;
    uint32  acc_reg, wait_bit_val;

    RT_INIT_CHK(oob_init[unit]);
    if (mii_bus < RTK_OOB_SMI_START || mii_bus > RTK_OOB_SMI_END)
    {
        return RT_ERR_INPUT;
    }

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    acc_reg = REG32_FIELD_SET(acc_reg, phy_addr, RTL9311B_MII_ACCESS_PHYAddress_OFFSET, RTL9311B_MII_ACCESS_PHYAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, phy_reg, RTL9311B_MII_ACCESS_RegAddress_OFFSET, RTL9311B_MII_ACCESS_RegAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, 0, RTL9311B_MII_ACCESS_Flag_OFFSET, RTL9311B_MII_ACCESS_Flag_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MII_ACCESS_ADDR, acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    /* wait access ready */
    wait_bit_val = RTL9311B_MII_ACCESS_Flag_MASK;
    if ((ret = _dal_tigershark_oob_busyWait_ready(unit,
                    _dal_tigershark_oob_miimAccComplete_chk,
                    &wait_bit_val,
                    RTL9311B_ACCESS_PHY_TIMEOUT_TIME)) != RT_ERR_OK)
    {
        RT_LOG(LOG_WARNING, MOD_OOB, "OOB mii read timeout phy_add %u reg %u", phy_addr, phy_reg);
        goto out;
    }

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    *pData = REG32_FIELD_GET(acc_reg, RTL9311B_MII_ACCESS_Data_OFFSET, RTL9311B_MII_ACCESS_Data_MASK);

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;

}

/* Function Name:
 *      dal_tigershark_oob_miiBus_write
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit - unit id
 *      mii_bus - bus ID
 *      phy_addr - address of the PHY
 *      phy_reg - PHY register (0 ~ 31)
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_miiBus_write(uint32 unit, uint32 mii_bus, uint32 phy_addr, uint32 phy_reg, uint32 data)
{
    int32   ret;
    uint32  acc_reg, wait_bit_val;

    RT_INIT_CHK(oob_init[unit]);
    if (mii_bus < RTK_OOB_SMI_START || mii_bus > RTK_OOB_SMI_END)
    {
        return RT_ERR_INPUT;
    }

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    acc_reg = REG32_FIELD_SET(acc_reg, phy_addr, RTL9311B_MII_ACCESS_PHYAddress_OFFSET, RTL9311B_MII_ACCESS_PHYAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, phy_reg, RTL9311B_MII_ACCESS_RegAddress_OFFSET, RTL9311B_MII_ACCESS_RegAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, data, RTL9311B_MII_ACCESS_Data_OFFSET, RTL9311B_MII_ACCESS_Data_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, 1, RTL9311B_MII_ACCESS_Flag_OFFSET, RTL9311B_MII_ACCESS_Flag_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MII_ACCESS_ADDR, acc_reg)) != RT_ERR_OK)
    {
        RT_LOG(LOG_WARNING, MOD_OOB, "OOB mii write timeout phy_add %u reg %u", phy_addr, phy_reg);
        goto out;
    }

    /* wait access ready */
    wait_bit_val = 0x0;
    if ((ret = _dal_tigershark_oob_busyWait_ready(unit,
                    _dal_tigershark_oob_miimAccComplete_chk,
                    &wait_bit_val,
                    RTL9311B_ACCESS_PHY_TIMEOUT_TIME)) != RT_ERR_OK)
    {
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}


/* Function Name:
 *      dal_tigershark_oob_syncPhySpeed_process
 * Description:
 *      Sync phy speed to GMAC SerDes
 * Input:
 *      unit - unit id
 *      port - OOB port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_syncPhySpeed_process(uint32 unit, rtk_port_t port)
{
    int32 ret;
    uint32 mStatVal, sr2Val;
    uint32 spd10, spd1000, sds_ablty;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MEDIA_STATUS_ADDR, &mStatVal)) != RT_ERR_OK)
    {
        goto out;
    }

    /* check is force speed mode */
    if (mStatVal & RTL9311B_MEDIA_STATUS_force_spd_mode_MASK)
    {
        goto out;
    }

    /* check link. 0: Link OK, 1: Link fail */
    if ((mStatVal & RTL9311B_MEDIA_STATUS_LINKB_MASK) != 0)
    {
        goto out;
    }

    /* get speed */
    spd10 = (mStatVal & RTL9311B_MEDIA_STATUS_SPEED_10_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_10_OFFSET;
    spd1000 = (mStatVal & RTL9311B_MEDIA_STATUS_SPEED_1000_MASK) >> RTL9311B_MEDIA_STATUS_SPEED_1000_OFFSET;
    if ((spd1000 == 1) && (spd10 == 0)) //1000M
    {
        sds_ablty = 0x2;
    }
    else if ((spd1000 == 0) && (spd10 == 0))    //100M
    {
        sds_ablty = 0x1;
    }
    else if ((spd1000 == 0) && (spd10 == 1))    //10M
    {
        sds_ablty = 0x0;
    }
    else
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port %u oob media_status 0x%x spd 2b11 out of range", unit, port, mStatVal);
        goto out;
    }

    /* sds */
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_OOBSR2_ADDR, &sr2Val)) != RT_ERR_OK)
    {
        goto out;
    }
    if (((sr2Val & RTL9311B_OOBSR2_sds0_ablty_MASK) >> RTL9311B_OOBSR2_sds0_ablty_OFFSET) == sds_ablty)
    {
        goto out;
    }

    RT_LOG(LOG_INFO, MOD_OOB, "OOB unit %u port %u SYNC spd 0x%x -> 0x%x", unit, port,
            ((sr2Val & RTL9311B_OOBSR2_sds0_ablty_MASK)>>RTL9311B_OOBSR2_sds0_ablty_OFFSET), sds_ablty);

    sr2Val = (sr2Val & ~RTL9311B_OOBSR2_sds0_ablty_MASK) | (sds_ablty << RTL9311B_OOBSR2_sds0_ablty_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_OOBSR2_ADDR, sr2Val)) != RT_ERR_OK)
    {
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}



/* Function Name:
 *      dal_tigershark_oob_mac_init
 * Description:
 *      oob mac init
 * Input:
 *      unit - unit id
 *      port - OOB port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_mac_init(uint32 unit, rtk_port_t port)
{
    uint32  sds_id = HWP_PORT_SDSID(unit, port);
    int32 ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB Init unit=%u, port=%u\n", unit, port);
    if (oob_sem[unit] == 0)
    {
        oob_sem[unit] = osal_sem_mutex_create();
    }

    _dal_tigershark_oob_mii_init(unit, port);

    if (sds_id < RTK_OOB_SDS_START || sds_id > RTK_OOB_SDS_END)
    {
        ret = RT_ERR_OUT_OF_RANGE;
        RT_ERR(ret, MOD_BSP, "OOB unit %u port %u error sds id %u", unit, port, sds_id);
    }
    else
    {
        _dal_tigershark_oob_sdsMode_set(unit, sds_id, HWP_SDS_MODE(unit, sds_id));
    }
    return ret;
}

/* Function Name:
 *      dal_tigershark_portMiim_read
 * Description:
 *      Miim read by port ID
 * Input:
 *      unit - unit id
 *      port - OOB port ID
 *      phy_reg - phy register
 * Output:
 *      pData - read data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_portMiim_read(uint32 unit, rtk_port_t port, uint32 phy_reg, uint32 *pData)
{
    int32   ret;
    uint32  acc_reg, wait_bit_val;
    uint32 phy_addr;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    if ((phy_addr = HWP_PHY_ADDR(unit, port)) == HWP_NONE)
    {
        return RT_ERR_OUT_OF_RANGE;
    }

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    acc_reg = REG32_FIELD_SET(acc_reg, phy_addr, RTL9311B_MII_ACCESS_PHYAddress_OFFSET, RTL9311B_MII_ACCESS_PHYAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, phy_reg, RTL9311B_MII_ACCESS_RegAddress_OFFSET, RTL9311B_MII_ACCESS_RegAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, 0, RTL9311B_MII_ACCESS_Flag_OFFSET, RTL9311B_MII_ACCESS_Flag_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9311B_MII_ACCESS_ADDR, acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    /* wait access ready */
    wait_bit_val = RTL9311B_MII_ACCESS_Flag_MASK;
    if ((ret = _dal_tigershark_oob_busyWait_ready(unit,
                    _dal_tigershark_oob_miimAccComplete_chk,
                    &wait_bit_val,
                    RTL9311B_ACCESS_PHY_TIMEOUT_TIME)) != RT_ERR_OK)
    {
        RT_LOG(LOG_WARNING, MOD_OOB, "OOB mii read timeout phy_add %u reg %u", phy_addr, phy_reg);
        goto out;
    }

    if ((ret = ioal_soc_mem32_read(unit, RTL9311B_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    *pData = REG32_FIELD_GET(acc_reg, RTL9311B_MII_ACCESS_Data_OFFSET, RTL9311B_MII_ACCESS_Data_MASK);

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}


/* Function Name:
 *      dal_tigershark_oob_sds_construct
 * Description:
 *      oob serdes construct
 * Input:
 *      unit - unit id
 *      port - OOB port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_tigershark_oob_sds_construct(uint32 unit, rtk_port_t port)
{
    int32 ret = RT_ERR_OK;
    uint32  sds_id = HWP_PORT_SDSID(unit, port);

    if (sds_id < RTK_OOB_SDS_START || sds_id > RTK_OOB_SDS_END)
    {
        ret = RT_ERR_OUT_OF_RANGE;
        RT_ERR(ret, MOD_BSP, "OOB unit %u port %u error sds id %u", unit, port, sds_id);
    }
    else
    {
        _dal_tigershark_oob_sdsPatch_set(unit, sds_id);
    }
    return ret;
}

