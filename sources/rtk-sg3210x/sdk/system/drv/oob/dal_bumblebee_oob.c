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

#include <private/drv/oob/dal_bumblebee_oob.h>
#include <soc/socdef/rtl9330_soc_reg.h>
#include <ioal/mem32.h>
#include <osal/lib.h>
#include <osal/time.h>
#include <osal/print.h>
#include <osal/sem.h>
#include <osal/isr.h>
#include <osal/memory.h>
#include <osal/cache.h>
#include <osal/spl.h>
#include <soc/type.h>
#include <common/debug/rt_log.h>
#include <hwp/hw_profile.h>
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <private/drv/oob/dal_common_oob.h>
#include <drv/drv_mgmt.h>
#include <drv/intr/intr.h>
#include <common/util/rt_util_intr.h>


/*
 * Symbol Definition
 */
#define     RX_RING_NUM     1
#define     TX_RING_NUM     5

#define     HOL_SUPP_THR_ON                 6
#define     HOL_SUPP_THR_OFF                10      /* ON should be smaller than OFF*/
#define     RX_TIMER_EXP                    3   /*0~0xf means  set*4 TU*/
#define     RX_INTR_MIT                     1   /*0~0xf means  set*4 packet*/
#define     TX_TIMER_EXP                    3   /*0~0xf means  set*4 TU*/
#define     TX_INTR_MIT                     1   /*0~0xf means  set*4 packet*/
#define     INTR_TIMER_SEL                  0
#define     DESC_FMT_EXT_R9330            1
#define     SHORT_DESC_FMT_R9330          0
#define     TX_THR              0x3
#define     RX_THR              0x2

#define     RX_JUMBO_SIZE       16379
#define     TX_JUMBO_SIZE       9216

#define     OOB_TX_DESC_R9330                 (200)    /* Number of Tx descriptor registers */
#define     OOB_RX_DESC_R9330                 (800)    /* Number of Rx descriptor registers */

#define     RX_DESC_SIZE        16
#define     TX_DESC_SIZE        20


#define     RTL9330_EN_IO_CMD_TXF1_4_OFFSET       RTL9330_ETN_IO_CMD_TxFN1st_OFFSET
#define     RTL9330_EN_IO_CMD_TXF1_4_MASK         (0xf << RTL9330_EN_IO_CMD_TXF1_4_OFFSET)

#define     RTL9330_IMR0_IMR0_TDU_OFFSET      RTL9330_IMR0_IMR0_TDU1_OFFSET
#define     RTL9330_IMR0_IMR0_TDU_MASK        (0x1F << RTL9330_IMR0_IMR0_TDU_OFFSET)

#define     RTL9330_IMR0_IMR0_TOK_OFFSET      RTL9330_IMR0_IMR0_TOK1_OFFSET
#define     RTL9330_IMR0_IMR0_TOK_MASK        (0x1F << RTL9330_IMR0_IMR0_TOK_OFFSET)

#define     RTL9330_ISR1_ISR_TDU_OFFSET       RTL9330_ISR1_ISR_TDU1_OFFSET
#define     RTL9330_ISR1_ISR_TDU_MASK         (0x1F << RTL9330_ISR1_ISR_TDU1_OFFSET)

#define     RTL9330_ISR1_ISR_TOK_OFFSET       RTL9330_ISR1_ISR_TOK1_OFFSET
#define     RTL9330_ISR1_ISR_TOK_MASK         (0x1F << RTL9330_ISR1_ISR_TOK_OFFSET)

#define     RTL9330_ACCESS_PHY_TIMEOUT_TIME   ((64+64) * 1000 * 1000) /* microseconds */
#define     RTL9330_ACCESS_SDS_TIMEOUT_TIME   ((1000) * 1000 * 1000)   /* microseconds */ /* TBD */


#define     DBG_HWINFO_BIT      (0x1 << 31)



#define     LOCK_WITH_MUTEX


typedef struct pkt_priv_s
{
    drv_nic_pkt_t   *packet;
    drv_nic_tx_cb_f tx_callback;
    void            *cookie;
} pkt_priv_t;


typedef struct nic_rx_cb_entry_s
{
    drv_nic_rx_cb_f rx_callback;
    void *pCookie;
} nic_rx_cb_entry_t;




static drv_nic_initCfg_t _oob_init_conf;


static uint32       rx_crc_cnt = 0;
static uint64       rx_success_cnt = 0;
static uint64       tx_success_cnt = 0;
static uint32       rx_err_cnt = 0;
static uint32       tx_failed_cnt = 0;
static uint32       rx_failed_cnt = 0;
static uint32       mb_alloc_fail_cnt = 0;
static uint32       rx_alloc_fail_cnt = 0;

static uint32       rxCRCInclude = 0;
static uint32       rx_jumbo_size = RX_JUMBO_SIZE;
//static uint32       tx_jumbo_size = 0;


static uint32       oob_debug_flag = 0;

uintptr                 oob_tx_desc_vaddr = 0;
uintptr                 oob_tx_desc_phy_addr = 0;
uintptr                 oob_rx_desc_vaddr = 0;
uintptr                 oob_rx_desc_phy_addr = 0;


/* SerDes config */
static oob_sdsPatchConfType_t   oob_sds_patch_config[] = {
    {0x21, 0x06,  4,  0, 0x0018},
    {0x21, 0x0a, 14, 12, 0x0005},
    {0x21, 0x0a, 10, 10, 0x0001},
    {0x21, 0x02,  2,  2, 0x0000},
    {0x36, 0x0a,  5,  5, 0x0000},
    {0x36, 0x03, 15, 12, 0x0000},
};

/*
 * Macro Definition
 */
#ifdef __BOOTLOADER__
  #define OOB_LOCK(unit, port)
  #define OOB_UNLOCK(unit, port)
  #define OOB_RX_LOCK(unit, port, flags)
  #define OOB_RX_UNLOCK(unit, port, flags)
  #define OOB_TX_LOCK(unit, port, flags)
  #define OOB_TX_UNLOCK(unit, port, flags)
#else
#if defined(LOCK_WITH_MUTEX)
    static osal_mutex_t     oob_rx_sem[RTK_MAX_NUM_OF_UNIT];
    static osal_mutex_t     oob_tx_sem[RTK_MAX_NUM_OF_UNIT];
  #define OOB_LOCK(unit, port)
  #define OOB_UNLOCK(unit, port)
  
  #define OOB_RX_LOCK(unit, port, flags)    \
      do {\
         if (osal_sem_mutex_take(oob_rx_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
         {\
             RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_OOB), "oob_rx semaphore lock failed");\
             return RT_ERR_SEM_LOCK_FAILED;\
         }\
      } while(0)
  #define OOB_RX_UNLOCK(unit, port, flags)  \
      do {\
         if (osal_sem_mutex_give(oob_rx_sem[unit]) != RT_ERR_OK)\
         {\
             RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_OOB), "oob_rx semaphore unlock failed");\
             return RT_ERR_SEM_UNLOCK_FAILED;\
         }\
      } while(0)
  #define OOB_TX_LOCK(unit, port, flags)    \
      do {\
         if (osal_sem_mutex_take(oob_tx_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
         {\
             RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_OOB), "oob_tx semaphore lock failed");\
             return RT_ERR_SEM_LOCK_FAILED;\
         }\
      } while(0)
  #define OOB_TX_UNLOCK(unit, port, flags)   \
      do {\
         if (osal_sem_mutex_give(oob_tx_sem[unit]) != RT_ERR_OK)\
         {\
             RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_OOB), "oob_tx semaphore unlock failed");\
             return RT_ERR_SEM_UNLOCK_FAILED;\
         }\
      } while(0)
#else
    osal_spinlock_t         oob_spl_rx_lock, oob_spl_tx_lock;
  #define OOB_LOCK(unit, port)
  #define OOB_UNLOCK(unit, port)
  #define OOB_RX_LOCK(unit, port, flags)               osal_spl_spin_lock_irqsave(oob_spl_rx_lock, flags)
  #define OOB_RX_UNLOCK(unit, port, flags)             osal_spl_spin_unlock_irqrestore(oob_spl_rx_lock, flags)
  #define OOB_TX_LOCK(unit, port, flags)               osal_spl_spin_lock_irqsave(oob_spl_tx_lock, flags)
  #define OOB_TX_UNLOCK(unit, port, flags)             osal_spl_spin_unlock_irqrestore(oob_spl_tx_lock, flags)
#endif
#endif
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      dma_map_single(dev, usr_addr, size, dir)
#elif defined(__BOOTLOADER__)
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      dma_map_single(usr_addr, size, dir)
#else
    static uintptr          pkt_buf_vir_addres;   //TODO: SST-1027
    static uintptr          pkt_phy_addr;         //TODO: SST-1027
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      ((usr_addr - pkt_buf_vir_addres) + pkt_phy_addr)
  #define unlikely(x) (x)
#endif


#define DEBUG_DUMP_PKT_LEN          256
#define RX_INSERT_BYTE              2







/*
 * Data Type Definition
 */

typedef struct reg_field_s
{
    uint32  reg;
    uint32  offset;
    uint32  mask;
} reg_field_t;


/*
 * Data Declaration
 */

static uint32   rxRingIdSize_9330[] = {16};
static uint32   txRingIdSize_9330[] = {16, 16, 16, 16, 16};
static uint32   oob_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};
static osal_mutex_t         oob_sem[RTK_MAX_NUM_OF_UNIT] = { 0 };
#ifndef __BOOTLOADER__
static isr_param_t oob_isr_param;
static isr_param_t oob_isr_param_rxrunout;
static isr_param_t oob_isr_param_txdone;
static isr_param_t oob_isr_param_txrunout;
#endif

static const reg_field_t tx_ring_base_addr[] = {
    {RTL9330_TXFDP1_ADDR, RTL9330_TXFDP1_TxFDP1_OFFSET, RTL9330_TXFDP1_TxFDP1_MASK},
    {RTL9330_TXFDP2_ADDR, RTL9330_TXFDP2_TxFDP2_OFFSET, RTL9330_TXFDP2_TxFDP2_MASK},
    {RTL9330_TXFDP3_ADDR, RTL9330_TXFDP3_TxFDP3_OFFSET, RTL9330_TXFDP3_TxFDP3_MASK},
    {RTL9330_TXFDP4_ADDR, RTL9330_TXFDP4_TxFDP4_OFFSET, RTL9330_TXFDP4_TxFDP4_MASK},
    {RTL9330_TXFDP5_ADDR, RTL9330_TXFDP5_TxFDP5_OFFSET, RTL9330_TXFDP5_TxFDP5_MASK}
    };

static const reg_field_t tx_ring_cur_addr[] = {
    {RTL9330_TXCDO1_ADDR, RTL9330_TXCDO1_TxCDO1_OFFSET, RTL9330_TXCDO1_TxCDO1_MASK},
    {RTL9330_TXCDO2_ADDR, RTL9330_TXCDO2_TxCDO2_OFFSET, RTL9330_TXCDO2_TxCDO2_MASK},
    {RTL9330_TXCDO3_ADDR, RTL9330_TXCDO3_TxCDO3_OFFSET, RTL9330_TXCDO3_TxCDO3_MASK},
    {RTL9330_TXCDO4_ADDR, RTL9330_TXCDO4_TxCDO4_OFFSET, RTL9330_TXCDO4_TxCDO4_MASK},
    {RTL9330_TXCDO5_ADDR, RTL9330_TXCDO5_TxCDO5_OFFSET, RTL9330_TXCDO5_TxCDO5_MASK},
    };

static const reg_field_t rx_ring_enable_addr[] = {
    {RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_RXRING1_OFFSET, RTL9330_IO_CMD1_RXRING1_MASK},
    {RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_RXRING2_OFFSET, RTL9330_IO_CMD1_RXRING2_MASK},
    {RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_RXRING3_OFFSET, RTL9330_IO_CMD1_RXRING3_MASK},
    {RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_RXRING4_OFFSET, RTL9330_IO_CMD1_RXRING4_MASK},
    {RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_RXRING5_OFFSET, RTL9330_IO_CMD1_RXRING5_MASK},
    {RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_RXRING6_OFFSET, RTL9330_IO_CMD1_RXRING6_MASK},
    };


static uint32   _oob_rx_intr_cb_cnt = 0;
static nic_rx_cb_entry_t _oob_rx_cb_tbl[NIC_RX_CB_PRIORITY_NUMBER] = {{0}};

static uint32       rx_desc_len = RX_DESC_SIZE;
static uint32       tx_desc_len = TX_DESC_SIZE;
static uint32       rxRingNum = RX_RING_NUM;
static uint32       txRingNum = TX_RING_NUM;
static uint32       totalRxRingNum = 0, totalTxRingNum = 0;

static uint32       *rxRingIdSize = rxRingIdSize_9330;
static uint32       *txRingIdSize = txRingIdSize_9330;
static uint8        **pOob_rxRBase = NULL;
static uint8        **pOob_txRBase = NULL;
static pkt_priv_t 	**pRx_pkt_privAry = NULL;
static pkt_priv_t 	**pTx_pkt_privAry = NULL;

static uint32       *rx_cpu_ptr = NULL;
static uint32       *rx_cpu_flag = NULL;
static uint32       *cur_rx = NULL;
static uint32       *cur_rx_flag = NULL;
static uint32       *tx_cpu_ptr = NULL;
static uint32       *cur_tx = NULL;
static uint32       *jumboFlag = NULL;
static uint32       *jumboBuffAlloc = NULL;
static uint32       *rx_cmplt_flag = NULL;

static drv_nic_pkt_t **pPacket = NULL;


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

#if 1




#if 0
drv_nic_rx_t   dal_bumblebee_oob_rxCallback_handle(uint32 unit, rtk_port_t port, drv_nic_pkt_t* pPacket)
{
    uint32  i = 0;
    drv_nic_rx_t oob_rx_handle = NIC_RX_NOT_HANDLED;
    uint8           handled = FALSE;

    if (0 != _oob_rx_intr_cb_cnt)
    {
        for (i = 0; i < NIC_RX_CB_PRIORITY_NUMBER; i++)
        {
            if (_oob_rx_cb_tbl[i].rx_callback != NULL)
            {
                oob_rx_handle = _oob_rx_cb_tbl[i].rx_callback(unit, pPacket, _oob_rx_cb_tbl[i].pCookie);
            }
            switch (oob_rx_handle)
            {
                case NIC_RX_NOT_HANDLED:
                    break;
                case NIC_RX_HANDLED:
                    handled = TRUE;
                    break;
                case NIC_RX_HANDLED_OWNED:
                    handled = TRUE;
                    if (jumboFlag[ringId] == 0)
                        pRx_pkt_privAry[ringId][cur_rx[ringId]].packet = NULL;
                    break;
                default:
                    break;
            }
            if (handled)
            {
                handled = FALSE;
                break;
            }
        }
    }

    return  oob_rx_handle;


}

#endif
int32   _dal_bumblebee_oob_rx_register(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb, void *pCookie)
{
    int32   ret = RT_ERR_OK;
    RT_INIT_CHK(oob_init[unit]);
    /* Check arguments */
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    RT_PARAM_CHK(priority > NIC_RX_CB_PRIORITY_MAX, RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(NULL == fRxCb, RT_ERR_NULL_POINTER);

    OOB_SEM_LOCK(unit);

    if (NULL == _oob_rx_cb_tbl[priority].rx_callback)
    {
        _oob_rx_cb_tbl[priority].rx_callback = fRxCb;
        _oob_rx_cb_tbl[priority].pCookie     = pCookie;
        _oob_rx_intr_cb_cnt++;

    }
    else
    {
        /* Handler is already existing */
        ret = RT_ERR_FAILED;
    }

    OOB_SEM_UNLOCK(unit);
    return ret;
}

int32   _dal_bumblebee_oob_rx_unregister(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb)
{
    int32   ret = RT_ERR_OK;
    RT_INIT_CHK(oob_init[unit]);
    /* Check arguments */
    RT_PARAM_CHK(NULL == fRxCb, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(priority > NIC_RX_CB_PRIORITY_MAX, RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);

    if (_oob_rx_cb_tbl[priority].rx_callback == fRxCb)
    {
        _oob_rx_cb_tbl[priority].rx_callback = NULL;
        _oob_rx_cb_tbl[priority].pCookie     = NULL;
        _oob_rx_intr_cb_cnt--;
    }
    else
    {
        /* Handler is nonexistent */
        ret = RT_ERR_FAILED;
    }


    OOB_SEM_UNLOCK(unit);
    return ret;
}

#if 0
int32   _dal_bumblebee_oob_isr_register(uint32 unit, rtk_port_t port, osal_isr_t fIsr)
{
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && !defined(CONFIG_SDK_EXTERNAL_CPU)
    my_isr_param[unit].unit = unit;
    my_isr_param[unit].port = port;
    if (gIsrRegistered == FALSE)
    {
        if (RT_ERR_OK != (ret = osal_isr_register(RTK_DEV_INT_GMAC_INTR, fIsr, (void *)&my_isr_param[unit])))
        {
            RT_LOG(LOG_DEBUG, MOD_OOB, "Error - Register OOB IRQ handler failed!");
            goto err_alloc;
        }
        gIsrRegistered = TRUE;
    }


#endif

}

int32   dal_bumblebee_oob_isr_unregister(uint32 unit, rtk_port_t port)
{
    int32   ret = RT_ERR_OK;
#if !defined(CONFIG_SDK_EXTERNAL_CPU) && defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    if (gIsrRegistered)
    {
        if ((ret = osal_isr_unregister(RTK_DEV_INT_GMAC_INTR)) != RT_ERR_OK)
        {
            return ret;
        }
        osal_free(  [unit]);
        my_isr_param[unit] = NULL;
        gIsrRegistered = FALSE;
    }
#endif
    return ret;
}
#endif

int32   _dal_bumblebee_oob_exit(uint32 unit, rtk_port_t port)
{
    oob_init[unit] = INIT_NOT_COMPLETED;
    if (oob_sem[unit])
    {
        osal_sem_mutex_destroy(oob_sem[unit]);
        oob_sem[unit] = 0;
    }
    return RT_ERR_OK;
}



int32   _dal_bumblebee_oob_initCfg_set(uint32 unit, rtk_port_t port, drv_nic_initCfg_t *pInitCfg)
{
    RT_PARAM_CHK(NULL == pInitCfg->rx_pkt_alloc, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(NULL == pInitCfg->pkt_alloc, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(NULL == pInitCfg->pkt_free, RT_ERR_NULL_POINTER);
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    RT_PARAM_CHK(NULL == pInitCfg->dev, RT_ERR_NULL_POINTER);
#endif

    _oob_init_conf.pkt_size  = pInitCfg->pkt_size;
    _oob_init_conf.pkt_alloc = pInitCfg->pkt_alloc;
    _oob_init_conf.pkt_free  = pInitCfg->pkt_free;
    _oob_init_conf.rx_pkt_alloc = pInitCfg->rx_pkt_alloc;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    _oob_init_conf.dev       = pInitCfg->dev;
#endif
    return RT_ERR_OK;
}

int32   _dal_bumblebee_oob_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32   i = 0;
    if (ENABLED == enable)
    {
        dal_bumblebee_oob_ethModuleEnable_set(unit, port, enable);
        for (i = 0; i < rxRingNum; i++)
        {
            dal_bumblebee_oob_ringEnable_set(unit, port, OOB_DIR_RX, i, enable);
        }
        dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_TX, TRUE);
        dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_RX, TRUE);

    }
    else
    {
        dal_bumblebee_oob_ethModuleEnable_set(unit, port, enable);

        for (i = 0; i < rxRingNum; i++)
        {
            dal_bumblebee_oob_ringEnable_set(unit, port, OOB_DIR_RX, i, enable);
        }
        dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_TX, FALSE);
        dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_RX, FALSE);
    }


    return RT_ERR_OK;
}


int32   _dal_bumblebee_oob_nic_intr_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    drv_intr_data_t     sts_data;

    RT_LOG(LOG_DEBUG, MOD_OOB, "unit %u port %u nic intr enable set:%u", unit, port);
    if (DISABLED == enable)
    {
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_RXDONE, DISABLED);
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_RXRUNOUT, DISABLED);
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_TXDONE, DISABLED);
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_TXRUNOUT, DISABLED);
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_RXDONE, &sts_data);
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_RXRUNOUT, &sts_data);
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_TXDONE, &sts_data);
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_TXRUNOUT, &sts_data);
    }
    else
    {
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_RXDONE, ENABLED);
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_RXRUNOUT, ENABLED);
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_TXDONE, ENABLED);
        rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_TXRUNOUT, ENABLED);
    }
    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_nic_sw_dataBase_free(uint32 unit, rtk_port_t port)
{
    ALLOC_ERR_FREE(pPacket, osal_free);
    ALLOC_ERR_FREE(rx_cmplt_flag, osal_free);
    ALLOC_ERR_FREE(jumboBuffAlloc, osal_free);
    ALLOC_ERR_FREE(jumboFlag, osal_free);
    ALLOC_ERR_FREE(cur_tx, osal_free);
    ALLOC_ERR_FREE(tx_cpu_ptr, osal_free);
    ALLOC_ERR_FREE(cur_rx_flag, osal_free);
    ALLOC_ERR_FREE(cur_rx, osal_free);
    ALLOC_ERR_FREE(rx_cpu_flag, osal_free);
    ALLOC_ERR_FREE(rx_cpu_ptr, osal_free);

    return RT_ERR_FAILED;
}

int32   dal_bumblebee_oob_nic_sw_dataBase_init(uint32 unit, rtk_port_t port)
{
    ALLOC_ERR_RET(rx_cpu_ptr,   osal_alloc, sizeof(int32) * rxRingNum);
    ALLOC_ERR_HDL(rx_cpu_flag,  osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(cur_rx,       osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(cur_rx_flag,  osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(tx_cpu_ptr,   osal_alloc, sizeof(uint32) * txRingNum, err_alloc);
    ALLOC_ERR_HDL(cur_tx,       osal_alloc, sizeof(uint32) * txRingNum, err_alloc);
    ALLOC_ERR_HDL(jumboFlag,    osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(jumboBuffAlloc, osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(rx_cmplt_flag, osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(pPacket, osal_alloc, sizeof(drv_nic_pkt_t *) * rxRingNum, err_alloc);



    osal_memset(rx_cpu_ptr, 0, sizeof(uint32) * rxRingNum);
    osal_memset(rx_cpu_flag, 0, sizeof(uint32) * rxRingNum);
    osal_memset(cur_rx, 0, sizeof(uint32) * rxRingNum);
    osal_memset(cur_rx_flag, 0, sizeof(uint32) * rxRingNum);
    osal_memset(tx_cpu_ptr, 0, sizeof(uint32) * txRingNum);
    osal_memset(cur_tx, 0, sizeof(uint32) * txRingNum);
    osal_memset(jumboFlag, 0, sizeof(uint32) * rxRingNum);
    osal_memset(jumboBuffAlloc, 0, sizeof(uint32) * rxRingNum);
    osal_memset(rx_cmplt_flag, 0, sizeof(uint32) * rxRingNum);


    return RT_ERR_OK;

err_alloc:

    return dal_bumblebee_oob_nic_sw_dataBase_free(unit, port);

}



int32
dal_bumblebee_oob_desc_init(uint32 unit, rtk_port_t port, uintptr vir_base, uintptr phy_base)
{
    oob_tx_desc_vaddr     = vir_base;
    oob_rx_desc_vaddr     = oob_tx_desc_vaddr + OOB_TX_DESC_R9330 * sizeof(oob_desc_t);
    oob_tx_desc_phy_addr  = phy_base;
    oob_rx_desc_phy_addr  = oob_tx_desc_phy_addr + OOB_TX_DESC_R9330 * sizeof(oob_desc_t);

    return RT_ERR_OK;

}

int32   dal_bumblebee_oob_ring_free(uint32 unit, rtk_port_t port)
{
    uint32  i;

    for (i = 0; i < rxRingNum; i++)
    {
        ALLOC_ERR_FREE(pRx_pkt_privAry[i], osal_free);
    }

    for (i = 0; i < txRingNum; i++)
    {
        ALLOC_ERR_FREE(pTx_pkt_privAry[i], osal_free);
    }

    ALLOC_ERR_FREE(pOob_rxRBase, osal_free);
    ALLOC_ERR_FREE(pOob_txRBase, osal_free);
    ALLOC_ERR_FREE(pRx_pkt_privAry, osal_free);
    ALLOC_ERR_FREE(pTx_pkt_privAry, osal_free);

    return RT_ERR_OK;
}







int32   _dal_common_oob_raw_data_dump(uint8* ptr, uint32 len)
{
    uint32  i;

    osal_printf("=== [NIC Debug SHOW] =======address = %p, length = %d\n", ptr, len);
    for (i = 0; i < len; i++)
    {
        if (i == len)
            break;
        if (0 == (i % 16))
            osal_printf("[%04x] ", i);
        osal_printf("%02x ", *(unsigned char*)(ptr + i));
        if (15 == (i % 16))
            osal_printf("\n");
    }
    osal_printf("\n");
    return RT_ERR_OK;
}

int32 _dal_common_oob_pkt_dump(uint32 unit, drv_nic_pkt_t *pPacket, nic_dir_t dir)
{
    int i;
    int dump_len = DEBUG_DUMP_PKT_LEN; /* debug dump maximum length */

    if (dir == NIC_DIR_TX)
        osal_printf("=== [NIC TX Debug] ================================= Len: %d \n", pPacket->length);
    else
        osal_printf("=== [NIC RX Debug] ================================= Len: %d \n", pPacket->length);

    for (i = 0; i < dump_len; i++)
    {
        if (i == (pPacket->length))
            break;
        if (0 == (i % 16))
            osal_printf("[%04X] ", i);
        osal_printf("%02X ", *(unsigned char*)(pPacket->data + i));
        if (15 == (i % 16))
            osal_printf("\n");
    }
    osal_printf("\n");

    return 0;
}


int32    _dal_bumblebee_oob_pkt_tx(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie);

static int32    dal_bumblebee_oob_isr_rxRoutine(uint32 unit, uint32 port, uint32 ringId)
{
    uint32  i = 0;
    uint32  rx_hw_ptr = 0;
    uint32  alloc_cnt = 0;
    uint8*  pDesc = NULL;
    oob_desc_t  desc;
    drv_nic_rx_t oob_rx_handle = NIC_RX_NOT_HANDLED;
    uintptr         mapping = 0;
#if !defined(LOCK_WITH_MUTEX)
    unsigned long   spl_flags;
#endif

    OOB_RX_LOCK(unit, port, &spl_flags);

    if (ringId >= rxRingNum)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "invalid ringId(%d)!", ringId);
        rx_failed_cnt++;
        OOB_RX_UNLOCK(unit, port, &spl_flags);
        return RT_ERR_FAILED;
    }


    dal_bumblebee_oob_hwPtr_get(unit, port, NIC_DIR_RX, ringId, &rx_hw_ptr);

    do
    {
		uint32	        pkt_size = 0;
        drv_nic_pkt_t   *cur_pkt = NULL;


        pDesc    = pOob_rxRBase[ringId] + cur_rx[ringId] * rx_desc_len;

#if defined(__BOOTLOADER__)
        dma_unmap_single((uintptr)pDesc, sizeof(oob_desc_t), DMA_FROM_DEVICE);
#endif

        if (NULL == (cur_pkt = pRx_pkt_privAry[ringId][cur_rx[ringId]].packet))
            break;


//    osal_printf("%s():%d cur_rx[%d] = %d, cur_pkt->data = %p\n", __FUNCTION__, __LINE__, ringId, cur_rx[ringId], cur_pkt->data);

        dal_bumblebee_oob_raw_to_desc(unit, port, OOB_DIR_RX, pDesc, &desc);

        if (oob_debug_flag & DEBUG_RX_RAW_LEN_BIT)
        {
            _dal_common_oob_raw_data_dump(pDesc, rx_desc_len);
            dal_bumblebee_oob_desc_dump(unit, port, OOB_DIR_RX, &desc);
        }

        if (desc.info & DESC_INFO_OWN_MASK)
        {
            RT_LOG(LOG_DEBUG, MOD_OOB, "%s():%d ETHERNET own! cur_rx[%d] = 0x%x, rx_hw_ptr = 0x%x\n",
                    __FUNCTION__, __LINE__, ringId, cur_rx[ringId], rx_hw_ptr);
            break;
        }

        if (desc.info & RX_DESC_INFO_CRC_MASK)
        {
            rx_crc_cnt++;
        }

        pkt_size = desc.info & RX_DESC_INFO_LEN_MASK;


#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
        dma_unmap_single(_oob_init_conf.dev, desc.buf_addr, _oob_init_conf.pkt_size + RX_INSERT_BYTE, DMA_FROM_DEVICE);
#elif defined(__BOOTLOADER__)
        dma_unmap_single(desc.buf_addr , _oob_init_conf.pkt_size + RX_INSERT_BYTE, DMA_FROM_DEVICE);
#endif

        if (FALSE == jumboFlag[ringId])
        {
            if ((desc.info & DESC_INFO_FS_MASK) && (desc.info & DESC_INFO_LS_MASK))
            {
                /* normal packet */
                pPacket[ringId]         = cur_pkt;
                pPacket[ringId]->length = pkt_size;
                pPacket[ringId]->data += RX_INSERT_BYTE;
                pPacket[ringId]->tail   = pPacket[ringId]->data + pkt_size;
                if (unlikely(pPacket[ringId]->tail > pPacket[ringId]->end))
                    osal_printf("%s():%d  !!!!  head:%p  data:%p  tail:%p  end:%p\n", __FUNCTION__, __LINE__, pPacket[ringId]->head, pPacket[ringId]->data, pPacket[ringId]->tail, pPacket[ringId]->end);
                rx_cmplt_flag[ringId] = 1;
            }
            else if ((desc.info & DESC_INFO_FS_MASK) && (0 == (desc.info & DESC_INFO_LS_MASK)))
            {
                /* jumbo head */
                jumboFlag[ringId] = TRUE;
                if (RT_ERR_OK == _oob_init_conf.rx_pkt_alloc(unit, rx_jumbo_size, 0, &pPacket[ringId]))
                {
                    jumboBuffAlloc[ringId] = TRUE;
                    osal_memcpy(pPacket[ringId]->data, cur_pkt->data + RX_INSERT_BYTE, pkt_size);
                    pPacket[ringId]->length = pkt_size;
                }
                else
                {
                    RT_LOG(LOG_DEBUG, MOD_OOB, "Out of memory ! (alloc a new packet data buffer failed)");
                    pPacket[ringId] = NULL;
                    rx_failed_cnt++;
                }
                _oob_init_conf.pkt_free(unit, cur_pkt);
                pRx_pkt_privAry[ringId][cur_rx[ringId]].packet = NULL;
                rx_cmplt_flag[ringId] = 0;
            }
            else
            {
                osal_printf("%s():%d Receive error, jumbo flag = %d, receive desc with fs = %x, ls = %x\n",
                            __FUNCTION__, __LINE__, jumboFlag[ringId], (desc.info & DESC_INFO_FS_MASK), (desc.info & DESC_INFO_LS_MASK));
                break;
            }

            /*VLAN TAG process*/
            if (desc.vlan_info & DESC_VLAN_INFO_VLAN_TAG_MASK)
            {
                pPacket[ringId]->rx_tag.cvid_tagged = 1;
                pPacket[ringId]->rx_tag.inner_vid = ((desc.vlan_info & DESC_VLAN_INFO_VID_H_MASK) << 8)
                                                    | ((desc.vlan_info & DESC_VLAN_INFO_VID_L_MASK) >> 8);
                pPacket[ringId]->rx_tag.inner_pri = (desc.vlan_info & DESC_VLAN_INFO_PRI_MASK) >> DESC_VLAN_INFO_PRI_OFFSET;

            }


        }
        else
        {
            if (unlikely(desc.info & DESC_INFO_FS_MASK))
            {
                osal_printf("%s():%d Receive error, jumbo flag = %d, receive desc with fs = %x, ls = %x\n",
                            __FUNCTION__, __LINE__, jumboFlag[ringId], (desc.info & DESC_INFO_FS_MASK), (desc.info & DESC_INFO_LS_MASK));
            }
            /* jumbo tail and jumbo other */
            if (jumboBuffAlloc[ringId])
            {
                osal_memcpy(pPacket[ringId]->data + pPacket[ringId]->length, cur_pkt->data, pkt_size);
                pPacket[ringId]->length += pkt_size;
                pPacket[ringId]->tail   = pPacket[ringId]->data + pPacket[ringId]->length;
                if (unlikely(pPacket[ringId]->tail > pPacket[ringId]->end))
                    osal_printf("%s():%d  !!!!  head:%p  data:%p  tail:%p  end:%p\n", __FUNCTION__, __LINE__, pPacket[ringId]->head, pPacket[ringId]->data, pPacket[ringId]->tail, pPacket[ringId]->end);
            }
            _oob_init_conf.pkt_free(unit, cur_pkt);
            pRx_pkt_privAry[ringId][cur_rx[ringId]].packet = NULL;


            if ((desc.info & DESC_INFO_LS_MASK))
            {
                /* jumbo tail */
                jumboFlag[ringId] = FALSE;
                jumboBuffAlloc[ringId] = FALSE;
                rx_cmplt_flag[ringId] = 1;
            }

        }

        if (1 == rx_cmplt_flag[ringId])
        {

            uint8           handled = FALSE;

            rx_success_cnt++;

            /* NIC Rx debug message */
            if (oob_debug_flag & DEBUG_RX_RAW_LEN_BIT)
            {
                osal_printf("%s():%d  cur_rx[%d] = %d, addr = %p, size = %d\n", __FUNCTION__, __LINE__, ringId, cur_rx[ringId], pPacket[ringId]->data, pPacket[ringId]->length);
                _dal_common_oob_pkt_dump(unit, pPacket[ringId], NIC_DIR_RX);
            }

            if (oob_debug_flag & DEBUG_CPU_LOOPBACK_BIT)
            {
                if (RT_ERR_OK != dal_bumblebee_oob_pkt_tx(unit, port, pPacket[ringId], NULL, NULL))
                {
                    _oob_init_conf.pkt_free(unit, pPacket[ringId]);
                }
                if (jumboFlag[ringId] == 0)
                    pRx_pkt_privAry[ringId][cur_rx[ringId]].packet = NULL;
            }
            else
            {
                oob_rx_handle = NIC_RX_NOT_HANDLED;
                if (0 == rxCRCInclude)
                {
                    pPacket[ringId]->length -= 4;
                    pPacket[ringId]->tail   -= 4;
                }
                if (0 != _oob_rx_intr_cb_cnt)
                {
                    for (i = 0; i < NIC_RX_CB_PRIORITY_NUMBER; i++)
                    {
                        if (_oob_rx_cb_tbl[i].rx_callback != NULL)
                        {
                            oob_rx_handle = _oob_rx_cb_tbl[i].rx_callback(unit, pPacket[ringId], _oob_rx_cb_tbl[i].pCookie);
                        }
                        switch (oob_rx_handle)
                        {
                            case NIC_RX_NOT_HANDLED:
                                break;
                            case NIC_RX_HANDLED:
                                handled = TRUE;
                                break;
                            case NIC_RX_HANDLED_OWNED:
                                handled = TRUE;
                                if (jumboFlag[ringId] == 0)
                                    pRx_pkt_privAry[ringId][cur_rx[ringId]].packet = NULL;
                                break;
                            default:
                                break;
                        }
                        if (handled)
                        {
                            handled = FALSE;
                            break;
                        }
                    }
                }
                else
                {
                    /* Process non-interrupt callback function */
                }

                if (NIC_RX_HANDLED_OWNED != oob_rx_handle)
                {
                    _oob_init_conf.pkt_free(unit, pPacket[ringId]);
                    if (jumboFlag[ringId] == 0)
                        pRx_pkt_privAry[ringId][cur_rx[ringId]].packet = NULL;
                }
            }
        }

        if (NULL == pRx_pkt_privAry[ringId][cur_rx[ringId]].packet)
        {
            drv_nic_pkt_t *ptr = NULL;

            /* Alloc a new packet data buffer */
            if (RT_ERR_OK == _oob_init_conf.rx_pkt_alloc(unit, _oob_init_conf.pkt_size, 0, &ptr))
            {
                alloc_cnt++;

                if (alloc_cnt == rxRingIdSize[ringId])
                {
//                    dal_bumblebee_oob_cpuPtr_set(unit, port, OOB_DIR_RX, ringId, rx_cpu_ptr[ringId]);
                    alloc_cnt = 0;
                }

                pRx_pkt_privAry[ringId][rx_cpu_ptr[ringId]].packet = ptr;

                ptr->data = (uint8*) MAKE_4B_ALGN((uintptr)ptr->data);

                mapping     = (uintptr)DMA_ADDR_VIR2PHY(_oob_init_conf.dev, pRx_pkt_privAry[ringId][rx_cpu_ptr[ringId]].packet->data, _oob_init_conf.pkt_size, DMA_FROM_DEVICE);

                dal_bumblebee_oob_cpuPtr_set(unit, port, OOB_DIR_RX, ringId, rx_cpu_ptr[ringId]);
//                osal_printf("%s():%d, rx_cpu_ptr[%d] = %d, pPacket->data = %p, mapping = %lx\n", __FUNCTION__, __LINE__, ringId, rx_cpu_ptr[ringId], ptr->data, mapping);

                rx_cpu_ptr[ringId]++;
                if (rx_cpu_ptr[ringId] == rxRingIdSize[ringId])
                {
                    rx_cpu_ptr[ringId]  = 0;
                    rx_cpu_flag[ringId] ^= 1;
                }

                desc.buf_addr = mapping;
                desc.info = INFO_SET_LEN(desc, _oob_init_conf.pkt_size);
                desc.info = INFO_SET_OWN(desc);
#if defined(__BOOTLOADER__)
                flush_dcache_range((uintptr)ptr->data, (uintptr)ptr->data + _oob_init_conf.pkt_size);
#endif
//                dal_bumblebee_oob_cpuPtr_set(unit, port, OOB_DIR_RX, ringId, rx_cpu_ptr[ringId]);
            }
            else
            {
                osal_printf("%s():%d  Out of memory ! (alloc a new packet data buffer failed)\n", __FUNCTION__, __LINE__);
                //reclaim_mbuf = FALSE;
                rx_alloc_fail_cnt++;
                if (alloc_cnt == rxRingIdSize[ringId])
                {
//                    dal_bumblebee_oob_cpuPtr_set(unit, port, OOB_DIR_RX, ringId, rx_cpu_ptr[ringId]);
                    alloc_cnt = 0;
                }
                break;
            }
        }

        dal_bumblebee_oob_desc_to_raw(unit, port, OOB_DIR_RX, &desc, pDesc);

#if 0
        _dal_common_oob_raw_data_dump(pDesc, rx_desc_len);

        osal_printf("================RX_END==\n");
#endif

        cur_rx[ringId]++;
        if (cur_rx[ringId] == rxRingIdSize[ringId])
        {
            cur_rx[ringId]      = 0;
            cur_rx_flag[ringId] ^= 1;
        }

    } while (cur_rx[ringId] != rx_hw_ptr);
    OOB_RX_UNLOCK(unit, port, &spl_flags);

    return RT_ERR_OK;
}



int32   dal_bumblebee_oob_isr_txRoutine(uint32 unit, uint32 port,  uint32 ringId)
{

    uint32      tx_hw_ptr = 0;
    pkt_priv_t  *cur_pkt_priv = NULL;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
    uint8*  pDesc = NULL;
    oob_desc_t  desc;
    uint32  len = 0;
#endif
#if !defined(LOCK_WITH_MUTEX)
        unsigned long   spl_flags;
#endif
    OOB_TX_LOCK(unit, port, &spl_flags);

    if (ringId >= txRingNum)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "invalid ringId(%d)!", ringId);
        OOB_TX_UNLOCK(unit, port, &spl_flags);
        return RT_ERR_FAILED;
    }

    /* Get HW current pointer */
    dal_bumblebee_oob_hwPtr_get(unit, port, OOB_DIR_TX, ringId, &tx_hw_ptr);

    do
    {
        drv_nic_pkt_t   *cur_pkt = NULL;

        cur_pkt_priv = pTx_pkt_privAry[ringId] + cur_tx[ringId];
        if (NULL == (cur_pkt = cur_pkt_priv->packet))
            break;

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
        memset(&desc, 0, sizeof(oob_desc_t));
        pDesc    = pOob_txRBase[ringId] + cur_tx[ringId] * tx_desc_len;
  #if defined(__BOOTLOADER__)
        dma_unmap_single((uintptr)pDesc, sizeof(oob_desc_t), DMA_FROM_DEVICE);      //TODO, do not need logically
  #endif
        dal_bumblebee_oob_raw_to_desc(unit, port, OOB_DIR_TX, pDesc, &desc);

        if (desc.info & DESC_INFO_OWN_MASK)
        {
            RT_LOG(LOG_DEBUG, MOD_OOB,"%s() %d: ETHERMODULE OWN, cur_tx[%d] = 0x%x, tx_hw_ptr = 0x%x, tx_cpu_ptr[%d] = 0x%x\n",
                        __FUNCTION__, __LINE__, ringId, cur_tx[ringId], tx_hw_ptr, ringId, tx_cpu_ptr[ringId]);
            break;
        }

        len     = cur_pkt->txIncludeCRC ? cur_pkt->length - 4 : cur_pkt->length;
  #if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
        dma_unmap_single(_oob_init_conf.dev, desc.buf_addr, len, DMA_TO_DEVICE);
  #else
        dma_unmap_single(desc.buf_addr, len, DMA_TO_DEVICE);
  #endif
#endif

        if (cur_pkt_priv->tx_callback == NULL)
        {
            _oob_init_conf.pkt_free(unit, cur_pkt);
        }
        else
        {
            cur_pkt_priv->tx_callback(unit, cur_pkt, cur_pkt_priv->cookie);
            cur_pkt_priv->tx_callback = NULL;
        }
        cur_pkt_priv->packet = NULL;

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
        dal_bumblebee_oob_desc_to_raw(unit, port, OOB_DIR_TX, &desc, pDesc);
#endif

        cur_tx[ringId]++;
        if (cur_tx[ringId] == txRingIdSize[ringId])
        {
            cur_tx[ringId]      = 0;
        }

    } while (cur_tx[ringId] != tx_hw_ptr);


    OOB_TX_UNLOCK(unit, port, &spl_flags);

    return RT_ERR_OK;

}


static int32    dal_bumblebee_oob_isr_mbRoutine(uint32   unit, uint32 port)
{
    uint32  ringId = 0;
    uint32  alloc_cnt = 0;
    uint8*  pDesc = NULL;
    oob_desc_t  desc;
    drv_nic_pkt_t   *pPacket = NULL;
    uintptr         mapping = 0;
#if !defined(LOCK_WITH_MUTEX)
    unsigned long   spl_flags;
#endif
    OOB_RX_LOCK(unit, port, &spl_flags);


    for (ringId = 0; ringId < rxRingNum; ringId++)
    {
        alloc_cnt = 0;
        do
        {
            if (rx_cpu_ptr[ringId] == cur_rx[ringId] && (rx_cpu_flag[ringId] != cur_rx_flag[ringId]))
                break;

            /* Alloc a new packet data buffer */
            if (RT_ERR_OK != _oob_init_conf.rx_pkt_alloc(unit, _oob_init_conf.pkt_size, 0, &pPacket))
            {
                RT_LOG(LOG_DEBUG, MOD_OOB, "Out of memory ! (alloc a new packet data buffer failed)");
                mb_alloc_fail_cnt++;
                osal_time_usleep(100);
                break;
            }
            osal_memset(&desc, 0, sizeof(oob_desc_t));
            pRx_pkt_privAry[ringId][rx_cpu_ptr[ringId]].packet = pPacket;
            pPacket->data = (uint8*) MAKE_4B_ALGN((uintptr)pPacket->data);
            mapping = (uintptr)DMA_ADDR_VIR2PHY(_oob_init_conf.dev, pPacket->data, _oob_init_conf.pkt_size, DMA_FROM_DEVICE);
//            osal_printf("%s() %d: rx_cpu_ptr[0] = %d, cur_rx[0] = %d\n", __FUNCTION__, __LINE__, rx_cpu_ptr[0], cur_rx[0]);
//            osal_printf("%s():%d, rx_cpu_ptr[%d] = %d, pPacket->data = %p, mapping = %lx\n", __FUNCTION__, __LINE__, ringId, rx_cpu_ptr[ringId], pPacket->data, mapping);
            desc.buf_addr = mapping;
            desc.info = INFO_SET_LEN(desc, _oob_init_conf.pkt_size);
            desc.info = INFO_SET_OWN(desc);
#if defined(__BOOTLOADER__)
            flush_dcache_range((uintptr)pPacket->data, (uintptr)pPacket->data + _oob_init_conf.pkt_size);
#endif

            pDesc    = pOob_rxRBase[ringId] + rx_cpu_ptr[ringId] * rx_desc_len;

            dal_bumblebee_oob_cpuPtr_set(unit, port, OOB_DIR_RX, ringId, rx_cpu_ptr[ringId]);
            rx_cpu_ptr[ringId]++;
            if (rx_cpu_ptr[ringId] >= rxRingIdSize[ringId])
            {
                rx_cpu_ptr[ringId] = 0;
                rx_cpu_flag[ringId] ^= 1;
                desc.info = INFO_SET_EOR(desc);
            }

            dal_bumblebee_oob_desc_to_raw(unit, port, OOB_DIR_RX, &desc, pDesc);
#if 0
            osal_printf("%s():%d, DUMP rx desc raw data, rx_cpu_ptr[%d] = %d, pDesc = %p, mapping = %lx:\n", __FUNCTION__, __LINE__, ringId, rx_cpu_ptr[ringId], pDesc, mapping);
            _dal_common_oob_raw_data_dump(pDesc, rx_desc_len);
#endif


            alloc_cnt++;
            if (alloc_cnt >= rxRingIdSize[ringId])
            {
                break;
            }
        } while (1);
    }

    OOB_RX_UNLOCK(unit, port, &spl_flags);

    return  RT_ERR_OK;
}


int32   dal_bumblebee_oob_nic_ring_init(uint32 unit, rtk_port_t port)
{
    int32   ret = RT_ERR_FAILED;
    uint32  i = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    RT_PARAM_CHK((oob_rx_desc_vaddr == 0), RT_ERR_MEM_ALLOC);
    RT_PARAM_CHK((oob_tx_desc_vaddr == 0), RT_ERR_MEM_ALLOC);


    ALLOC_ERR_RET(pOob_rxRBase,   osal_alloc, sizeof(uint8*) * rxRingNum);
    ALLOC_ERR_HDL(pOob_txRBase,   osal_alloc, sizeof(uint8*) * txRingNum, err_alloc);
    ALLOC_ERR_HDL(pRx_pkt_privAry,   osal_alloc, sizeof(pkt_priv_t*) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(pTx_pkt_privAry,   osal_alloc, sizeof(pkt_priv_t*) * txRingNum, err_alloc);

    osal_memset(pOob_rxRBase, 0, sizeof(uint8*) * rxRingNum);
    osal_memset(pOob_txRBase, 0, sizeof(uint8*) * txRingNum);
    osal_memset(pRx_pkt_privAry, 0, sizeof(pkt_priv_t*) * rxRingNum);
    osal_memset(pTx_pkt_privAry, 0, sizeof(pkt_priv_t*) * txRingNum);


    oob_rx_desc_vaddr = MAKE_4B_ALGN(oob_rx_desc_vaddr);
    oob_rx_desc_phy_addr = MAKE_4B_ALGN(oob_rx_desc_phy_addr);

//    osal_printf("%s:%d, oob_rx_desc_vaddr = %lx, oob_rx_desc_phy_addr = %lx\n", __FUNCTION__, __LINE__, oob_rx_desc_vaddr, oob_rx_desc_phy_addr);

    for (i = 0; i < rxRingNum; i++)
    {
        pOob_rxRBase[i] = (uint8*)(oob_rx_desc_vaddr + rx_desc_len * totalRxRingNum);
        osal_memset(pOob_rxRBase[i], 0, rx_desc_len * rxRingIdSize[i]);

//        osal_printf("%s:%d, pOob_rxRBase[%d] = %p\n", __FUNCTION__, __LINE__, i, pOob_rxRBase[i]);

		pRx_pkt_privAry[i] = (pkt_priv_t*) osal_alloc(sizeof(pkt_priv_t) * rxRingIdSize[i]);
		if (NULL == pRx_pkt_privAry[i])
        {
            ret = RT_ERR_MEM_ALLOC;
            RT_LOG(LOG_DEBUG, MOD_OOB, "Error: Out of memory!");
            goto err_alloc;
        }
        osal_memset(pRx_pkt_privAry[i], 0, sizeof(pkt_priv_t) * rxRingIdSize[i]);

        dal_bumblebee_oob_ringBaseAddr_set(unit, port, OOB_DIR_RX, i, oob_rx_desc_phy_addr + rx_desc_len * totalRxRingNum);
        totalRxRingNum += rxRingIdSize[i];
    }


    oob_tx_desc_vaddr = MAKE_4B_ALGN(oob_tx_desc_vaddr);
    oob_tx_desc_phy_addr = MAKE_4B_ALGN(oob_tx_desc_phy_addr);

//    osal_printf("%s:%d, oob_tx_desc_vaddr = %lx, oob_tx_desc_phy_addr = %lx\n", __FUNCTION__, __LINE__, oob_tx_desc_vaddr, oob_tx_desc_phy_addr);

    for (i = 0; i < txRingNum; i++)
    {
        pOob_txRBase[i] = (uint8*)(oob_tx_desc_vaddr + tx_desc_len * totalTxRingNum);
        osal_memset(pOob_txRBase[i], 0, tx_desc_len * txRingIdSize[i]);

//        osal_printf("%s:%d, pOob_txRBase[%d] = %p\n", __FUNCTION__, __LINE__, i, pOob_txRBase[i]);

		pTx_pkt_privAry[i] = (pkt_priv_t*) osal_alloc(sizeof(pkt_priv_t) * txRingIdSize[i]);
		if (NULL == pTx_pkt_privAry[i])
        {
            ret = RT_ERR_MEM_ALLOC;
            RT_LOG(LOG_DEBUG, MOD_OOB, "Error: Out of memory!");
            goto err_alloc;
        }
        osal_memset(pTx_pkt_privAry[i], 0, sizeof(pkt_priv_t) * txRingIdSize[i]);

        dal_bumblebee_oob_ringBaseAddr_set(unit, port, OOB_DIR_TX, i, oob_tx_desc_phy_addr + tx_desc_len * totalTxRingNum);
        totalTxRingNum += txRingIdSize[i];
    }


    return RT_ERR_OK;

err_alloc:

    dal_bumblebee_oob_ring_free(unit, port);
    return ret;

}

osal_isrret_t
dal_bumblebee_oob_isr_handler(void *isr_param)
{
    int32  i = 0;
    uint32  cpu_iisr = 0;
    uint32  unit = ((isr_param_t *)isr_param)->unit;
    uint32  port = ((isr_param_t *)isr_param)->port;
#ifndef __BOOTLOADER__
    uint32  event_id = ((isr_param_t *)isr_param)->eventId;
#endif
    drv_intr_data_t isr_data;


    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:event %u process\n", __FUNCTION__, __LINE__, event_id);
    /* Handle Rx */
#ifndef __BOOTLOADER__
    //if (event_id == INTR_ISR_GMAC_RXDONE) //should always RX handled (update rx_cpu_ptr, cur_rx...) then buffer run-out handld
#endif
    {
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_RXDONE, &isr_data);
        cpu_iisr = isr_data.u.rx_done;
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (RXDONE)\n", __FUNCTION__, __LINE__, cpu_iisr);
        if (cpu_iisr)
        {
            for (i = rxRingNum - 1; i >= 0; i--)
            {
                if (cpu_iisr & (0x1 << i))
                {
                    dal_bumblebee_oob_isr_rxRoutine(unit, port, i);
                }
            }
        }
        drv_intr_imrEnable_set(unit, INTR_ISR_GMAC_RXDONE, ENABLED);
    }

    /* Handle Tx */
#ifndef __BOOTLOADER__
    if (event_id == INTR_ISR_GMAC_TXDONE)
#endif
    {
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_TXDONE, &isr_data);
        cpu_iisr = isr_data.u.tx_done;
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (TXDONE)\n", __FUNCTION__, __LINE__, cpu_iisr);
        if (cpu_iisr)
        {
            for (i = txRingNum - 1; i >= 0; i--)
            {
                if (cpu_iisr & (0x1 << i))
                {
                    dal_bumblebee_oob_isr_txRoutine(unit, port, i);
                }
            }
        }
        drv_intr_imrEnable_set(unit, INTR_ISR_GMAC_TXDONE, ENABLED);
    }

    /* mBuffer Runout */
#ifndef __BOOTLOADER__
    if (event_id == INTR_ISR_GMAC_RXRUNOUT)
#endif
    {
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_RXRUNOUT, &isr_data);
        cpu_iisr = isr_data.u.rx_runout;
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (RXRUNOUT)\n", __FUNCTION__, __LINE__, cpu_iisr);
        if (cpu_iisr)
        {
            dal_bumblebee_oob_isr_mbRoutine(unit, port);
        }
        drv_intr_imrEnable_set(unit, INTR_ISR_GMAC_RXRUNOUT, ENABLED);
    }

    /* Tx all done */
#ifndef __BOOTLOADER__
    if (event_id == INTR_ISR_GMAC_TXRUNOUT)
#endif
    {
        _rtl9330_intr_gmacData_get(unit, INTR_ISR_GMAC_TXRUNOUT, &isr_data);
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (TXRUNOUT)\n", __FUNCTION__, __LINE__, isr_data.u.tx_runout);
        drv_intr_imrEnable_set(unit, INTR_ISR_GMAC_TXRUNOUT, ENABLED);
    }

    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:event %u end\n", __FUNCTION__, __LINE__, event_id);

    return OSAL_INT_HANDLED;

}




int32    _dal_bumblebee_oob_pkt_tx(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie)
{
    uint32      ringId = 0;
    uintptr     mapping = 0;
    oob_desc_tx_t  desc;
    uint8*      pDesc = NULL;
    pkt_priv_t  *cur_pkt_priv = NULL;
    uint32      len = 0;

    if (NULL == pPacket)
    {
        osal_printf("%s():%d  Error - pPacket is NULL!\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }
    if (pPacket->length <= 4)
    {
        osal_printf("%s():%d  Error - pPacket length is less than 4!\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }

    /* Get HW current pointer */
    ringId          = (pPacket->tx_tag.priority > 3) ? 3 : pPacket->tx_tag.priority;

    pDesc    = pOob_txRBase[ringId] + tx_cpu_ptr[ringId] * tx_desc_len;
    cur_pkt_priv    = pTx_pkt_privAry[ringId] + tx_cpu_ptr[ringId];
    osal_memset(&desc, 0, sizeof(oob_desc_t));

    dal_bumblebee_oob_raw_to_desc(unit, port, OOB_DIR_TX, pDesc, (oob_desc_t*)&desc);


    if (desc.info & DESC_INFO_OWN_MASK)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "TX RING %d do not have available descriptor can be used, tx_cpu_ptr[%d] = 0x%x, cur_tx[%d] = 0x%x\n",
                ringId, ringId, tx_cpu_ptr[ringId], ringId, cur_tx[ringId]);
        return RT_ERR_FAILED;
    }
    if (NULL != cur_pkt_priv->packet)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB,"%s():%d  pkt_buf is not recycled yet\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }

    cur_pkt_priv->tx_callback   = fTxCb;    /* Tx Callback function */
    cur_pkt_priv->cookie        = pCookie;
    cur_pkt_priv->packet        = pPacket;


    if (oob_debug_flag & DEBUG_CPU_LOOPBACK_BIT)
    {
        desc.info = INFO_CLEAR_CRC(desc);
        len = pPacket->length;
    }
    else
    {
        desc.info = INFO_SET_CRC(desc);

        if (0 == pPacket->txIncludeCRC)
        {
            len = pPacket->length;
        }
        else
        {
            len = pPacket->length - 4;
        }
    }
    mapping = (uintptr)DMA_ADDR_VIR2PHY(_oob_init_conf.dev, pPacket->data, len, DMA_TO_DEVICE);

    desc.buf_addr = mapping;
    desc.info = INFO_SET_LEN(desc, len);
    desc.info = INFO_SET_FS(desc);
    desc.info = INFO_SET_LS(desc);
    desc.info = INFO_SET_OWN(desc);

    /* TODO: packet carry tx vlan tag info */
    if (oob_debug_flag & DEBUG_CPU_LOOPBACK_BIT)
    {
        if (pPacket->rx_tag.cvid_tagged)
        {
            desc.vlan_info = VLAN_INFO_SET_VLD(desc);
            desc.vlan_info = VLAN_INFO_SET_VID(desc, pPacket->rx_tag.inner_vid);
            desc.vlan_info = VLAN_INFO_SET_PRI(desc, pPacket->rx_tag.inner_pri);
            desc.vlan_info = VLAN_INFO_SET_CFI(desc, 0);
        }
    }

    tx_cpu_ptr[ringId]++;
    if (tx_cpu_ptr[ringId] == txRingIdSize[ringId])
    {
        tx_cpu_ptr[ringId]  = 0;
        desc.info = INFO_SET_EOR(desc);
    }
    if ((oob_debug_flag & DEBUG_TX_RAW_LEN_BIT))
    {
        _dal_common_oob_pkt_dump(unit, pPacket, NIC_DIR_TX);
    }

    dal_bumblebee_oob_desc_to_raw(unit, port, OOB_DIR_TX, (oob_desc_t*)&desc, pDesc);
    if (oob_debug_flag & DEBUG_TX_RAW_LEN_BIT)
    {
        _dal_common_oob_raw_data_dump(pDesc, tx_desc_len);
        dal_bumblebee_oob_desc_dump(unit, port, OOB_DIR_TX, (oob_desc_t*)&desc);
    }

    dal_bumblebee_oob_tx_kick(unit, port, ringId);
    //RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobTxFetch_set(unit, port, ringId);
    return RT_ERR_OK;
}




int32   dal_bumblebee_oob_init_gmac(uint32 unit, rtk_port_t port, drv_nic_initCfg_t *pInitCfg)
{
    uint32  ret = RT_ERR_FAILED;



    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._enable_set(unit, port, DISABLED), ret);

    /* Reset to default value */
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_intr_enable_set(unit, port, DISABLED), ret);

//    _oob_hw_init(unit);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_init(unit, port, pInitCfg), ret);

    RT_ERR_CHK(dal_bumblebee_oob_mac_init(unit, port), ret);


    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_sw_dataBase_init(unit, port), ret);

//    _oob_descSize_get(unit, &rx_desc_len, &tx_desc_len);
    rx_crc_cnt = 0;
    rx_success_cnt = 0;
    tx_success_cnt = 0;
    rx_err_cnt = 0;

    totalRxRingNum = 0;
    totalTxRingNum = 0;
    rxCRCInclude = 1;

    oob_debug_flag = 0;


    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_ring_init(unit, port), ret);

    /* Prepare the mBufs once */
    RT_ERR_HDL(dal_bumblebee_oob_isr_mbRoutine(unit, port), err_alloc, ret);

    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_intr_enable_set(unit, port, ENABLED), ret);

    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._enable_set(unit, port, ENABLED), ret);

    return ret;

err_alloc:
    dal_bumblebee_oob_nic_sw_dataBase_free(unit, port);


//    _oob_debug_info_dump(unit);
    return ret;
}


int32   dal_bumblebee_oob_desc_to_raw(uint32 unit, rtk_port_t port, oob_dir_t dir, oob_desc_t* pDesc, uint8* pVal)
{
    uint32* ptr = NULL;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
    uint32 desc_size;
#endif

    if (OOB_DIR_RX == dir)
    {
        ptr = (uint32*)(pVal + 8);
        *ptr = CPU_to_BE32(pDesc->vlan_info);
        ptr--;
        *ptr = CPU_to_BE32((uint32)(pDesc->buf_addr & 0xffffffff));
        ptr--;
        *ptr = CPU_to_BE32(pDesc->info);
    }
    else
    {
        oob_desc_tx_t *pTxDesc = (oob_desc_tx_t*)pDesc;
        ptr = (uint32*)(pVal + 12);
        *ptr = CPU_to_BE32(pTxDesc->vlan_info);
        ptr--;
        *ptr = CPU_to_BE32((uint32)((pTxDesc->buf_addr >> 32) & 0xffffffff));
        ptr--;
        *ptr = CPU_to_BE32((uint32)(pTxDesc->buf_addr & 0xffffffff));
        ptr--;
        *ptr = CPU_to_BE32(pTxDesc->info);
    }

    MEMORY_BARRIER();

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
    if (OOB_DIR_RX == dir)
        desc_size = sizeof(oob_desc_t);
    else
        desc_size = sizeof(oob_desc_tx_t);
#endif

#if defined(__BOOTLOADER__)
    flush_dcache_range((uintptr)pVal, (uintptr)pVal + desc_size);
#elif defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    if (oob_debug_flag & DEBUG_CPU_LOOPBACK_BIT)
    {
        flush_icache_range((uintptr)pVal, (uintptr)pVal + desc_size);
    }
#endif
    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_raw_to_desc(uint32 unit, rtk_port_t port, oob_dir_t dir, uint8* pVal, oob_desc_t* pDesc)
{
    uint32* ptr = (uint32*)pVal;

    pDesc->info = BE32_to_CPU(*ptr);
    ptr++;
    pDesc->buf_addr = BE32_to_CPU(*ptr);
    ptr++;
    pDesc->vlan_info = BE32_to_CPU(*ptr);

    return RT_ERR_OK;
}




int32
dal_bumblebee_oob_init(uint32 unit, rtk_port_t port, drv_nic_initCfg_t *pInitCfg)
{
    int32 ret = RT_ERR_FAILED;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    /* Check arguments */
    RT_PARAM_CHK(NULL == pInitCfg, RT_ERR_NULL_POINTER);


#if !defined(__BOOTLOADER__)
  #if defined(LOCK_WITH_MUTEX)
        /* create semaphore */
        oob_rx_sem[unit] = osal_sem_mutex_create();
        if (0 == oob_rx_sem[unit])
        {
            RT_ERR(RT_ERR_FAILED, (MOD_OOB), "oob_rx semaphore create failed");
            return RT_ERR_FAILED;
        }
        oob_tx_sem[unit] = osal_sem_mutex_create();
        if (0 == oob_tx_sem[unit])
        {
            RT_ERR(RT_ERR_FAILED, (MOD_OOB), "oob_tx semaphore create failed");
            return RT_ERR_FAILED;
        }
  #else
        oob_spl_rx_lock = osal_spl_spin_lock_create();
        oob_spl_tx_lock = osal_spl_spin_lock_create();
  #endif
#endif


    /* Initialize the NIC module */
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._init(unit, port, pInitCfg), ret);
#if 0
#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
    ret = osal_wait_module_create(&drv_oobIsr_event);
    if ((osal_thread_t)NULL == (rxThread_id = osal_thread_create("RTK NIC Rx Thread", NIC_RX_THREAD_STACK_SIZE, NIC_RX_THREAD_PRI, (void *)_oob_rx_thread, NULL)))
    {
        osal_printf("RTK NIC Rx Thread create failed\n");

        return RT_ERR_FAILED;
    }
#endif
#endif


#ifndef __BOOTLOADER__
    osal_memset(&oob_isr_param, 0, sizeof(isr_param_t));
    oob_isr_param.unit = unit;
    oob_isr_param.port = RTK_OOB_IDX_TO_PORT(0);
    oob_isr_param.eventId = INTR_ISR_GMAC_RXDONE;
    rt_util_intr_isr_register(INTR_ISR_GMAC_RXDONE, dal_bumblebee_oob_isr_handler, &oob_isr_param);

    osal_memset(&oob_isr_param_txdone, 0, sizeof(isr_param_t));
    oob_isr_param_txdone.unit = unit;
    oob_isr_param_txdone.port = RTK_OOB_IDX_TO_PORT(0);
    oob_isr_param_txdone.eventId = INTR_ISR_GMAC_TXDONE;
    rt_util_intr_isr_register(INTR_ISR_GMAC_TXDONE, dal_bumblebee_oob_isr_handler, &oob_isr_param_txdone);

    osal_memset(&oob_isr_param_rxrunout, 0, sizeof(isr_param_t));
    oob_isr_param_rxrunout.unit = unit;
    oob_isr_param_rxrunout.port = RTK_OOB_IDX_TO_PORT(0);
    oob_isr_param_rxrunout.eventId = INTR_ISR_GMAC_RXRUNOUT;
    rt_util_intr_isr_register(INTR_ISR_GMAC_RXRUNOUT, dal_bumblebee_oob_isr_handler, &oob_isr_param_rxrunout);

    osal_memset(&oob_isr_param_txrunout, 0, sizeof(isr_param_t));
    oob_isr_param_txrunout.unit = unit;
    oob_isr_param_txrunout.port = RTK_OOB_IDX_TO_PORT(0);
    oob_isr_param_txrunout.eventId = INTR_ISR_GMAC_TXRUNOUT;
    rt_util_intr_isr_register(INTR_ISR_GMAC_TXRUNOUT, dal_bumblebee_oob_isr_handler, &oob_isr_param_txrunout);
#endif

    return ret;

}


int32
dal_bumblebee_oob_exit(uint32 unit, rtk_port_t port)
{

    int32 ret = RT_ERR_FAILED;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Disable NIC rx/tx*/
    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].ethModuleEnable_set(unit, port, DISABLED);
//    dal_bumblebee_oob_ethModuleEnable_set(unit, port, DISABLED);

    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobEnable_set(unit, port, OOB_DIR_TX, FALSE);
    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobEnable_set(unit, port, OOB_DIR_RX, FALSE);
//    dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_TX, FALSE);
//    dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_RX, FALSE);

#ifndef __BOOTLOADER__
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_RXDONE);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_TXDONE);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_RXRUNOUT);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_TXRUNOUT);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_LINKCHG);
#endif

    // TODO:
    //_oob_ring_buff_release(unit);
    // clear init flag

#if !defined(__BOOTLOADER__)
  #if defined(LOCK_WITH_MUTEX)
        /* destroy semaphore */
        if (oob_tx_sem[unit])
        {
            osal_sem_mutex_destroy(oob_tx_sem[unit]);
            oob_tx_sem[unit] = 0;
        }
        if (oob_rx_sem[unit])
        {
            osal_sem_mutex_destroy(oob_rx_sem[unit]);
            oob_rx_sem[unit] = 0;
        }
  #else
        osal_spl_spin_lock_destroy(oob_spl_rx_lock);
        osal_spl_spin_lock_destroy(oob_spl_tx_lock);
  #endif
#endif


#if 0

#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
    if ((ret = osal_thread_destroy(rxThread_id)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = osal_wait_module_destroy(drv_oobIsr_event)) != RT_ERR_OK)
    {
        return ret;
    }
#endif
#endif


    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._exit(unit, port);
//    _dal_bumblebee_oob_exit(unit, port);
    return ret;
}


int32
dal_bumblebee_oob_pkt_tx(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie)
{
    int32 ret = RT_ERR_FAILED;
#if !defined(LOCK_WITH_MUTEX)
    unsigned long   spl_flags;
#endif

    /* Check arguments */
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);

    /* Dispatch */
    OOB_TX_LOCK(unit, port, &spl_flags);

    ret = _dal_bumblebee_oob_pkt_tx(unit, port, pPacket, fTxCb, pCookie);
    if (RT_ERR_OK == ret)
    {
        tx_success_cnt++;
    }
    else
    {
        tx_failed_cnt++;
    }
    OOB_TX_UNLOCK(unit, port, &spl_flags);

    return ret;


}




int32
dal_bumblebee_oob_rx_register(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb, void *pCookie, uint32 flags)
{
    int32   ret = RT_ERR_FAILED;
#if !defined(LOCK_WITH_MUTEX)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif


    OOB_RX_LOCK(unit, port, &spl_rx_flags);
    OOB_TX_LOCK(unit, port, &spl_tx_flags);

    if (NIC_FLAG_RX_CRC_INCLUDE & flags)
        rxCRCInclude = 1;

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._rx_register(unit, port, priority, fRxCb, pCookie), errHandle, ret);

errHandle:
    OOB_TX_UNLOCK(unit, port, &spl_tx_flags);
    OOB_RX_UNLOCK(unit, port, &spl_rx_flags);

    return ret;
}


int32
dal_bumblebee_oob_rx_unregister(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb)
{
    int32   ret = RT_ERR_FAILED;
#if !defined(LOCK_WITH_MUTEX)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif

    OOB_RX_LOCK(unit, port, &spl_rx_flags);
    OOB_TX_LOCK(unit, port, &spl_tx_flags);

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._rx_unregister(unit, port, priority, fRxCb), errHandle, ret);

errHandle:
    OOB_TX_UNLOCK(unit, port, &spl_tx_flags);
    OOB_RX_UNLOCK(unit, port, &spl_rx_flags);

    return ret;
}

int32   dal_bumblebee_oob_pkt_alloc(uint32 unit, rtk_port_t port, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    /* Check arguments */
    RT_PARAM_CHK(NULL == _oob_init_conf.pkt_alloc, RT_ERR_NULL_POINTER);
    return _oob_init_conf.pkt_alloc(unit, size, flags, ppPacket);
}



int32   dal_bumblebee_oob_pkt_free(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket)
{

    RT_PARAM_CHK(NULL == _oob_init_conf.pkt_free, RT_ERR_NULL_POINTER);
    return _oob_init_conf.pkt_free(unit, pPacket);
}

int32   dal_bumblebee_oob_dbg_get(uint32 unit, rtk_port_t port, uint32* pFlags)
{

    *pFlags = oob_debug_flag;
    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_dbg_set(uint32 unit, rtk_port_t port, uint32 flags)
{
    oob_debug_flag = flags;
    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_reset(uint32 unit, rtk_port_t port)
{
#if !defined(LOCK_WITH_MUTEX)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif

    RT_INIT_CHK(oob_init[unit]);

    OOB_LOCK(unit, port);
    OOB_RX_LOCK(unit, port, &spl_rx_flags);
    OOB_TX_LOCK(unit, port, &spl_tx_flags);

    oob_init[unit] = INIT_NOT_COMPLETED;

    /* Disable OOB rx/tx*/
    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].ethModuleEnable_set(unit, port, DISABLED);

    OOB_TX_UNLOCK(unit, port, &spl_tx_flags);
    OOB_RX_UNLOCK(unit, port, &spl_rx_flags);
    OOB_UNLOCK(unit, port);


    dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_TX, FALSE);
    dal_bumblebee_oob_oobEnable_set(unit, port, OOB_DIR_RX, FALSE);
    rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_RXDONE, DISABLED);
    rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_RXRUNOUT, DISABLED);
    rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_TXDONE, DISABLED);
    rtl9330_intr_swcoreImrEnable_set(unit, INTR_ISR_GMAC_TXRUNOUT, DISABLED);

    // TODO:
    /*
    _nic_ring_buff_release(unit);

    ret = _nic_init(unit, &_oob_init_conf);
    if (ret)
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "NIC reset failed");
        return RT_ERR_FAILED;
    }

    _nic_nicEnable_set(unit, NIC_DIR_RX, TRUE);

    nic_init[unit] = INIT_COMPLETED; */
    return RT_ERR_OK;
}


#endif


int32   dal_bumblebee_oob_nic_init(uint32  unit, rtk_port_t port, drv_nic_initCfg_t *pInitCfg)
{
    int32   ret = RT_ERR_FAILED;
    uint32  temp = 0;
    uint32  val = 0;
    hol_ring_thr_t  thr;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    RT_ERR_CHK(_dal_bumblebee_oob_initCfg_set(unit, port, pInitCfg), ret);

    RT_ERR_CHK(dal_bumblebee_oob_swOobRst_set(unit, port), ret);

    OOB_SEM_LOCK(unit);

    /*RX/TX expire time, mitigation config*/
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_TXPktTimer_OFFSET, RTL9330_ETN_IO_CMD_TXPktTimer_MASK, TX_TIMER_EXP);
    temp = TX_INTR_MIT & 0x7;
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_TxIntMitigation_2to0_OFFSET, RTL9330_ETN_IO_CMD_TxIntMitigation_2to0_MASK, temp);
    temp = (TX_INTR_MIT >> 3) & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_TxIntMitigation_3_OFFSET, RTL9330_ETN_IO_CMD_TxIntMitigation_3_MASK, temp);

    temp = RX_TIMER_EXP & 0x7;
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_RXPktTimer_2to0_OFFSET, RTL9330_ETN_IO_CMD_RXPktTimer_2to0_MASK, temp);
    temp = (RX_TIMER_EXP >> 3) & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_RXPktTimer_3_OFFSET, RTL9330_ETN_IO_CMD_RXPktTimer_3_MASK, temp);
    temp = RX_INTR_MIT & 0x7;
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_RxIntMitigation_2to0_OFFSET, RTL9330_ETN_IO_CMD_RxIntMitigation_2to0_MASK, temp);
    temp = (RX_INTR_MIT >> 3) & 0x1;
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_RxIntMitigation_3_OFFSET, RTL9330_ETN_IO_CMD_RxIntMitigation_3_MASK, temp);

    /*interrupt timer*/
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_reg_int_timer_sel_OFFSET, RTL9330_ETN_IO_CMD_reg_int_timer_sel_MASK, INTR_TIMER_SEL);

    /*clear pending flag*/
    ioal_soc_mem32_write(unit, RTL9330_INT_MS_ADDR, 0xffff);


    /* RX/TX threshold */
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_tsh_OFFSET, RTL9330_ETN_IO_CMD_tsh_MASK, TX_THR);
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_RXFTH_OFFSET, RTL9330_ETN_IO_CMD_RXFTH_MASK, RX_THR);

    /* RX use one ring only, enable 4GB & 1GB lx bus address*/
    val = (1 << RTL9330_IO_CMD1_EN_1GB_OFFSET)
            | (1 << RTL9330_IO_CMD1_EN_4GB_OFFSET)
            | (1 << RTL9330_IO_CMD1_EN_8GB_OFFSET)
            | (0 << RTL9330_IO_CMD1_EN_RX_MRING_OFFSET)
            | (DESC_FMT_EXT_R9330 << RTL9330_IO_CMD1_Dsc_format_extra_OFFSET);


    ioal_soc_mem32_read(unit, RTL9330_IO_CMD1_ADDR, &temp);
    temp &= ~(RTL9330_IO_CMD1_EN_1GB_MASK | RTL9330_IO_CMD1_EN_4GB_MASK | RTL9330_IO_CMD1_EN_RX_MRING_MASK | RTL9330_IO_CMD1_Dsc_format_extra_MASK);
    temp |= val;
    ioal_soc_mem32_write(unit, RTL9330_IO_CMD1_ADDR, temp);

    /*config IO CMD*/
    _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_ShortDesFormat_OFFSET, RTL9330_ETN_IO_CMD_ShortDesFormat_MASK, SHORT_DESC_FMT_R9330);
//    _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_Dsc_format_extra_OFFSET, RTL9330_IO_CMD1_Dsc_format_extra_MASK, DESC_FMT_EXT_R9330);


    OOB_SEM_UNLOCK(unit);


    /*config Rx ring size*/
    dal_bumblebee_oob_ringSize_set(unit, port, OOB_DIR_RX, 0, rxRingIdSize_9330[0]);

    thr.high = HOL_SUPP_THR_ON;
    thr.low = HOL_SUPP_THR_OFF;

    /*config hol threshold*/
    dal_bumblebee_oob_holRingThr_set(unit, port, OOB_DIR_RX, 0, thr);

    oob_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_ringInfo_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ** ringIdSizeList, uint32 *pRingNum)
{
    if (dir == OOB_DIR_RX)
    {
        *ringIdSizeList = rxRingIdSize_9330;
        *pRingNum = RX_RING_NUM;
    }
    else
    {
        *ringIdSizeList = txRingIdSize_9330;
        *pRingNum = TX_RING_NUM;
    }

    return RT_ERR_OK;
}



int32   dal_bumblebee_oob_ringSize_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32* pVal)
{
    uint32 temp;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        _ioal_soc_mem32_field_read(unit, RTL9330_RXCDO1_ADDR, RTL9330_RXCDO1_RxRingSize_1_7to4_OFFSET, RTL9330_RXCDO1_RxRingSize_1_7to4_MASK, &temp);
        _ioal_soc_mem32_field_read(unit, RTL9330_RXCDO1_ADDR, RTL9330_RXCDO1_RxRingSize_1_11to8_OFFSET, RTL9330_RXCDO1_RxRingSize_1_11to8_MASK, pVal);
        *pVal = ((*pVal) << 8) | temp;
    }
    else
    {

    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_ringSize_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 val)
{
    uint32 temp = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        /*config Rx ring size*/
        temp = (val - 1) & 0xff;
        _ioal_soc_mem32_field_write(unit, RTL9330_RXCDO1_ADDR, RTL9330_RXCDO1_RxRingSize_1_7to4_OFFSET, RTL9330_RXCDO1_RxRingSize_1_7to4_MASK, temp);
        temp = ((val - 1) >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9330_RXCDO1_ADDR, RTL9330_RXCDO1_RxRingSize_1_11to8_OFFSET, RTL9330_RXCDO1_RxRingSize_1_11to8_MASK, temp);
    }
    else
    {

    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_holRingThr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, hol_ring_thr_t* pThr)
{
    uint32 temp;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        _ioal_soc_mem32_field_read(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_OFFSET, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_MASK, &temp);
        _ioal_soc_mem32_field_read(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_OFFSET, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_MASK, &pThr->high);
        pThr->high = (pThr->high << 8) | temp;

        _ioal_soc_mem32_field_read(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_OFFSET, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_MASK, &temp);
        _ioal_soc_mem32_field_read(unit, RTL9330_RX_PSE1_ADDR, RTL9330_RX_PSE1_Rx_Pse_Des_Thres_off_1_OFFSET, RTL9330_RX_PSE1_Rx_Pse_Des_Thres_off_1_MASK, &pThr->low);
        pThr->low = (pThr->low << 8) | temp;
    }
    else
    {

    }

    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}



int32   dal_bumblebee_oob_holRingThr_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, hol_ring_thr_t thr)
{
    uint32 temp = 0;

    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (OOB_DIR_RX == dir)
    {
        temp = thr.high & 0xff;
        _ioal_soc_mem32_field_write(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_OFFSET, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_7to0_MASK, temp);
        temp = (thr.high >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_OFFSET, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_on_1_11to8_MASK, temp);

        temp = thr.low & 0xff;
        _ioal_soc_mem32_field_write(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_OFFSET, RTL9330_ETNRXCPU1_Rx_Pse_Des_Thres_off_1_7to0_MASK, temp);
        temp = (thr.low >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9330_RX_PSE1_ADDR, RTL9330_RX_PSE1_Rx_Pse_Des_Thres_off_1_OFFSET, RTL9330_RX_PSE1_Rx_Pse_Des_Thres_off_1_MASK, temp);
    }
    else
    {

    }

    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_ringBaseAddr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uintptr *pVal)
{
    uint32  temp = 0;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9330_RXFDP1_ADDR, RTL9330_RXFDP1_RxFDP1_OFFSET, RTL9330_RXFDP1_RxFDP1_MASK, &temp);
    }
    else
    {
        _ioal_soc_mem32_field_read(unit, tx_ring_base_addr[ring].reg, tx_ring_base_addr[ring].offset, tx_ring_base_addr[ring].mask, &temp);
    }
    OOB_SEM_UNLOCK(unit);
    *pVal = (uintptr)temp;
    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_ringBaseAddr_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uintptr val)
{
    uint32  temp = 0;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    temp = (uint32)(val & 0xffffffff);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        ioal_soc_mem32_write(unit, RTL9330_RXFDP1_ADDR, temp);
    }
    else
    {
        ioal_soc_mem32_write(unit, tx_ring_base_addr[ring].reg, temp);
    }
    OOB_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_ringCurAddr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uintptr *pVal)
{
    uint32  offset = 0;
    uintptr  base = 0;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    dal_bumblebee_oob_ringBaseAddr_get(unit, port, dir, ring, &base);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9330_RXCDO1_ADDR, RTL9330_RXCDO1_RxCDO1_OFFSET, RTL9330_RXCDO1_RxCDO1_MASK, &offset);
    }
    else
    {
        _ioal_soc_mem32_field_read(unit, tx_ring_cur_addr[ring].reg, tx_ring_cur_addr[ring].offset, tx_ring_cur_addr[ring].mask, &offset);
    }
    OOB_SEM_UNLOCK(unit);

    *pVal = (offset << 4) + base;
    return RT_ERR_OK;
}



int32   dal_bumblebee_oob_cpuPtr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 *pVal)
{
    uint32  ptr_l;
    uint32  ptr_h;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_OFFSET, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_MASK, &ptr_l);
        _ioal_soc_mem32_field_read(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_OFFSET, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_MASK, &ptr_h);
        *pVal = (ptr_h << 8) | ptr_l;
    }
    else
    {

    }
    OOB_SEM_UNLOCK(unit);
    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_cpuPtr_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 val)
{
    uint32  ptr_l;
    uint32  ptr_h;

    if (dir == OOB_DIR_RX)
    {
        ptr_l = val & 0xff;
        ptr_h = (val >> 8) & 0xf;
        _ioal_soc_mem32_field_write(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_OFFSET, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_7to0_MASK, ptr_l);
        _ioal_soc_mem32_field_write(unit, RTL9330_ETNRXCPU1_ADDR, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_OFFSET, RTL9330_ETNRXCPU1_EthrntRxCPU_Des_Num_1_11to8_MASK, ptr_h);
    }
    else
    {

    }
    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_hwPtr_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 ring, uint32 *pVal)
{
    if (dir == OOB_DIR_TX)
        _ioal_soc_mem32_field_read(unit, tx_ring_cur_addr[ring].reg, tx_ring_cur_addr[ring].offset, tx_ring_cur_addr[ring].mask, pVal);
    else
        _ioal_soc_mem32_field_read(unit, RTL9330_RXCDO1_ADDR, RTL9330_RXCDO1_RxCDO1_OFFSET, RTL9330_RXCDO1_RxCDO1_MASK, pVal);

    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_oobEnable_get(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32* pVal)
{
    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_read(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_RE_OFFSET, RTL9330_ETN_IO_CMD_RE_MASK, pVal);
    }
    else
    {
        _ioal_soc_mem32_field_read(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_TE_OFFSET, RTL9330_ETN_IO_CMD_TE_MASK, pVal);
    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}



int32   dal_bumblebee_oob_oobEnable_set(uint32 unit, rtk_port_t port, oob_dir_t dir, uint32 val)
{
    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if (dir == OOB_DIR_RX)
    {
        _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_RE_OFFSET, RTL9330_ETN_IO_CMD_RE_MASK, val);
    }
    else
    {
        _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_TE_OFFSET, RTL9330_ETN_IO_CMD_TE_MASK, val);
    }
    OOB_SEM_UNLOCK(unit);
#if 0
    if (val)
        osal_printf("OOB (R9330) %s Start... \n", (dir == OOB_DIR_RX) ? "RX" : "TX");
    else
        osal_printf("OOB (R9330) %s Stop... \n", (dir == OOB_DIR_RX) ? "RX" : "TX");
#endif
    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_tx_queue_pri_set(uint32 unit, rtk_port_t port, oob_tx_queue_pri_t type, oob_tx_queue_hi_low_t queue_cfg)
{

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if (TX_QUEUE_STRICT == type)
    {
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_Tx_hl_pri_sel_OFFSET, RTL9330_IO_CMD1_Tx_hl_pri_sel_MASK, 0);
    }
    else
    {
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_Tx_hl_pri_sel_OFFSET, RTL9330_IO_CMD1_Tx_hl_pri_sel_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_txq5_h_OFFSET, RTL9330_IO_CMD1_txq5_h_MASK, queue_cfg.queue_5_hi);
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_txq4_h_OFFSET, RTL9330_IO_CMD1_txq4_h_MASK, queue_cfg.queue_4_hi);
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_txq3_h_OFFSET, RTL9330_IO_CMD1_txq3_h_MASK, queue_cfg.queue_3_hi);
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_txq2_h_OFFSET, RTL9330_IO_CMD1_txq2_h_MASK, queue_cfg.queue_2_hi);
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_txq1_h_OFFSET, RTL9330_IO_CMD1_txq1_h_MASK, queue_cfg.queue_1_hi);
    }
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}



int32   _dal_bumblebee_oob_rxDesc_dump(uint32 unit, rtk_port_t port, oob_desc_t* pDesc)
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

int32   _dal_bumblebee_oob_txDesc_dump(uint32 unit, rtk_port_t port, oob_desc_t* pDesc)
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



int32   dal_bumblebee_oob_desc_dump(uint32 unit, rtk_port_t port, oob_dir_t dir, oob_desc_t* pDesc)
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



int32 dal_bumblebee_oob_hw_info_dump(uint32 unit, rtk_port_t port)
{
    uint32  temp = 0;
    hol_ring_thr_t thr;
    uint32  i;
    osal_printf("------------- DUMP ETHERNET MODULE REG -----------\n");
    _ioal_soc_mem32_field_read(unit, RTL9330_TRX_OK_CNT_ADDR, RTL9330_TRX_OK_CNT_txok_OFFSET, RTL9330_TRX_OK_CNT_txok_MASK, &temp);
    osal_printf("TX_OK_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9330_TRX_OK_CNT_ADDR, RTL9330_TRX_OK_CNT_rxok_OFFSET, RTL9330_TRX_OK_CNT_rxok_MASK, &temp);
    osal_printf("\tRX_OK_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9330_TRXERR_CNT_ADDR, RTL9330_TRXERR_CNT_Txerr_OFFSET, RTL9330_TRXERR_CNT_Txerr_MASK, &temp);
    osal_printf("TX_ERR_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9330_TRXERR_CNT_ADDR, RTL9330_TRXERR_CNT_Rxerr_OFFSET, RTL9330_TRXERR_CNT_Rxerr_MASK, &temp);
    osal_printf("\tRX_ERR_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9330_MISSPKT_CNT_ADDR, RTL9330_MISSPKT_CNT_MissPkt_OFFSET, RTL9330_MISSPKT_CNT_MissPkt_MASK, &temp);
    osal_printf("MISS_PKT_CNT\t: %d", temp);
//    _ioal_soc_mem32_field_read(unit, RTL9330_MISSPKT_CNT_ADDR, RTL9330_MISSPKT_CNT_FAE_OFFSET, RTL9330_MISSPKT_CNT_FAE_MASK, &temp);
    osal_printf("\tALGN_ERR_CNT\t: NOT SUPPORT\n");

    _ioal_soc_mem32_field_read(unit, RTL9330_TX_COL_CNT_ADDR, RTL9330_TX_COL_CNT_Tx1Col_OFFSET, RTL9330_TX_COL_CNT_Tx1Col_MASK, &temp);
    osal_printf("COL_1_TX_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9330_TX_COL_CNT_ADDR, RTL9330_TX_COL_CNT_TxMCol_OFFSET, RTL9330_TX_COL_CNT_TxMCol_MASK, &temp);
    osal_printf("\tCOL_M_TX_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9330_RXOKPHY_CNT_ADDR, RTL9330_RXOKPHY_CNT_RxOkPhy_OFFSET, RTL9330_RXOKPHY_CNT_RxOkPhy_MASK, &temp);
    osal_printf("RX_OK_PHY_CNT\t: %d", temp);
//    _ioal_soc_mem32_field_read(unit, RTL9330_RXOKPHY_CNT_ADDR, RTL9330_RXOKPHY_CNT_OxOkBrd_OFFSET, RTL9330_RXOKPHY_CNT_OxOkBrd_MASK, &temp);
    osal_printf("\tRX_OK_BC_CNT\t: NOT SUPPORT\n");

    _ioal_soc_mem32_field_read(unit, RTL9330_RXOKMUL_CNT_ADDR, RTL9330_RXOKMUL_CNT_RxOkMul_OFFSET, RTL9330_RXOKMUL_CNT_RxOkMul_MASK, &temp);
    osal_printf("RX_OK_MC_CNT\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9330_RXOKMUL_CNT_ADDR, RTL9330_RXOKMUL_CNT_TxAbt_OFFSET, RTL9330_RXOKMUL_CNT_TxAbt_MASK, &temp);
    osal_printf("\tTX_ABORT_CNT\t: %d\n", temp);

    _ioal_soc_mem32_field_read(unit, RTL9330_TXUNDER_CNT_ADDR, RTL9330_TXUNDER_CNT_TXUNDERC_OFFSET, RTL9330_TXUNDER_CNT_TXUNDERC_MASK, &temp);
    osal_printf("TX_UDER_RUN\t: %d", temp);
    _ioal_soc_mem32_field_read(unit, RTL9330_TXUNDER_CNT_ADDR, RTL9330_TXUNDER_CNT_RDU_MISSPKTC_OFFSET, RTL9330_TXUNDER_CNT_RDU_MISSPKTC_MASK, &temp);
    osal_printf("\tRDU_MISS_PKT\t: %d\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_TRX_STATUS_ADDR, &temp);
    osal_printf("TRSR\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_COM_ADDR, &temp);
    osal_printf("\tCOM\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_INT_MS_ADDR, &temp);
    osal_printf("IMSR\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_TX_CFG_ADDR, &temp);
    osal_printf("\tTCR\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_ISR1_ADDR, &temp);
    osal_printf("ISR1\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &temp);
    osal_printf("\tRCR\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_IMR0_ADDR, &temp);
    osal_printf("IMR0\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_IMR1_ADDR, &temp);
    osal_printf("\tIMR1\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &temp);
    osal_printf("MSR\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &temp);
    osal_printf("\tMIIAR\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_CONFIG_ADDR, &temp);
    osal_printf("CONFIG\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_INTR_ADDR, &temp);
    osal_printf("\tINTR\t: 0x%08x\n", temp);

//    ioal_soc_mem32_read(unit, RTL9330_VLAN1_ADDR, &temp);
    osal_printf("VLAN1_REG\t: NOT SUPPORT");
    ioal_soc_mem32_read(unit, RTL9330_LED_CTRL_ADDR, &temp);
    osal_printf("\tLED_CTRL\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_MEM_BIST_ADDR, &temp);
    osal_printf("MEM_BIST\t: 0x%08x", temp);
//    ioal_soc_mem32_read(unit, RTL9330_PKT_GEN_ADDR, &temp);
    osal_printf("\tPKT_GEN\t: NOT SUPPORT\n");


    osal_printf("------------- DUMP CPU INTERFACE REG -----------\n");

    ioal_soc_mem32_read(unit, RTL9330_TXFDP1_ADDR, &temp);
    osal_printf("TXFDP1\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_TXCDO1_ADDR, &temp);
    osal_printf("\tTXCDO1\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_TXFDP2_ADDR, &temp);
    osal_printf("TXFDP2\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_TXCDO2_ADDR, &temp);
    osal_printf("\tTXCDO2\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_TXFDP3_ADDR, &temp);
    osal_printf("TXFDP3\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_TXCDO3_ADDR, &temp);
    osal_printf("\tTXCDO3\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_TXFDP4_ADDR, &temp);
    osal_printf("TXFDP4\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_TXCDO4_ADDR, &temp);
    osal_printf("\tTXCDO4\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_TXFDP5_ADDR, &temp);
    osal_printf("TXFDP5\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_TXCDO5_ADDR, &temp);
    osal_printf("\tTXCDO5\t: 0x%08x\n", temp);

    ioal_soc_mem32_read(unit, RTL9330_RXFDP1_ADDR, &temp);
    osal_printf("RXFDP1\t: 0x%08x", temp);
    _ioal_soc_mem32_field_read(unit, RTL9330_RXCDO1_ADDR, RTL9330_RXCDO1_RxCDO1_OFFSET, RTL9330_RXCDO1_RxCDO1_MASK, &temp);
    osal_printf("\tRXCDO1\t: 0x%08x\n", temp);

    dal_bumblebee_oob_ringSize_get(unit, port, OOB_DIR_RX, 0, &temp);
    osal_printf("RX_RingSize\t: %d\n", temp);

    dal_bumblebee_oob_cpuPtr_get(unit, port, OOB_DIR_RX, 0, &temp);
    osal_printf("EthrntRxCPU_Des_Num (RX_CPU_PTR)\t: %d\n", temp);

    dal_bumblebee_oob_holRingThr_get(unit, port, OOB_DIR_RX, 0, &thr);
    osal_printf("Rx_Pse_Des_Thres_on (HOL)\t: %d", thr.high);
    osal_printf("\tRx_Pse_Des_Thres_off (HOL)\t: %d\n", thr.low);


    ioal_soc_mem32_read(unit, RTL9330_ETN_IO_CMD_ADDR, &temp);
    osal_printf("IO_CMD\t: 0x%08x", temp);
    ioal_soc_mem32_read(unit, RTL9330_IO_CMD1_ADDR, &temp);
    osal_printf("\tIO_CMD1\t: 0x%08x\n", temp);


    osal_printf("------------- DUMP SOFTWARE DATABASE -----------\n");

    osal_printf("rx_cpu_ptr\t: 0x%x", rx_cpu_ptr[0]);
    osal_printf("\tcur_rx\t: 0x%x\n", cur_rx[0]);

    osal_printf("tx_cpu_ptr[0]\t: 0x%x", tx_cpu_ptr[0]);
    osal_printf("\ttx_cpu_ptr[1]\t: 0x%x\n", tx_cpu_ptr[1]);

    osal_printf("tx_cpu_ptr[2]\t: 0x%x", tx_cpu_ptr[2]);
    osal_printf("\ttx_cpu_ptr[3]\t: 0x%x\n", tx_cpu_ptr[3]);

    osal_printf("tx_cpu_ptr[4]\t: 0x%x", tx_cpu_ptr[4]);
    osal_printf("\trx_crc_cnt\t: 0x%x\n", rx_crc_cnt);

    for (i = 0; i < rxRingNum; i++)
    {
        osal_printf("pOob_rxRBase[%d]\t: %p\t", i, pOob_rxRBase[i]);
        if (i % 2)
        {
            osal_printf("\n");
        }
    }
    osal_printf("\n");
    for (i = 0; i < txRingNum; i++)
    {
        osal_printf("pOob_txRBase[%d]\t: %p\t", i, pOob_txRBase[i]);
        if (i % 2)
        {
            osal_printf("\n");
        }
    }
    osal_printf("\n");


    osal_printf("rx_success_cnt\t: 0x%llx", rx_success_cnt);
    osal_printf("\ttx_success_cnt\t: 0x%llx\n", tx_success_cnt);

    osal_printf("rx_err_cnt\t: 0x%x\n", rx_err_cnt);

    osal_printf("rx_alloc_fail_cnt\t: 0x%x", rx_alloc_fail_cnt);
    osal_printf("\tmb_alloc_fail_cnt\t: 0x%x\n", mb_alloc_fail_cnt);




    return RT_ERR_OK;
}


int32    dal_bumblebee_oob_tx_kick(uint32 unit, rtk_port_t port, uint32 ringId)
{

    if (ringId < 4)
    {
        _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_EN_IO_CMD_TXF1_4_OFFSET, RTL9330_EN_IO_CMD_TXF1_4_MASK, 0x1 << ringId);
    }
    else
    {
        _ioal_soc_mem32_field_write(unit, RTL9330_IO_CMD1_ADDR, RTL9330_IO_CMD1_TxFN5th_OFFSET, RTL9330_IO_CMD1_TxFN5th_MASK, 1);
    }

    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_supportJumboSize_get(uint32 unit, rtk_port_t port, uint32* pRXval, uint32* pTXval)
{
    *pRXval = RX_JUMBO_SIZE;
    *pTXval = TX_JUMBO_SIZE;

    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_descSize_get(uint32 unit, rtk_port_t port, uint32* pRXval, uint32* pTXval)
{
    *pRXval = RX_DESC_SIZE;
    *pTXval = TX_DESC_SIZE;

    return RT_ERR_OK;
}





int32    dal_bumblebee_oob_ethModuleEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  data = 0;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);

    if (DISABLED == enable)
    {

    }
    else
    {

        OOB_SEM_LOCK(unit);

        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_FORCE_FULLDUP_OFFSET, RTL9330_MEDIA_STATUS_FORCE_FULLDUP_MASK, 1);
#ifdef CONFIG_SDK_FPGA_PLATFORM
        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_FORCE_SPD_OFFSET, RTL9330_MEDIA_STATUS_FORCE_SPD_MASK, 0x1);
#else
        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_FORCE_SPD_OFFSET, RTL9330_MEDIA_STATUS_FORCE_SPD_MASK, 0x2);
#endif
        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_FORCE_LINK_OFFSET, RTL9330_MEDIA_STATUS_FORCE_LINK_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_force_spd_mode_OFFSET, RTL9330_MEDIA_STATUS_force_spd_mode_MASK, 1);


        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_Force_TRXFCE_OFFSET, RTL9330_MEDIA_STATUS_Force_TRXFCE_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_RXFCE_OFFSET, RTL9330_MEDIA_STATUS_RXFCE_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_MEDIA_STATUS_ADDR, RTL9330_MEDIA_STATUS_TXFCE_OFFSET, RTL9330_MEDIA_STATUS_TXFCE_MASK, 1);


        ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &data);

        /* CRC check config: always check CRC, remove vlan tag*/
        _ioal_soc_mem32_field_write(unit, RTL9330_COM_ADDR, RTL9330_COM_rxchksum_OFFSET, RTL9330_COM_rxchksum_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_COM_ADDR, RTL9330_COM_rxjumbo_OFFSET, RTL9330_COM_rxjumbo_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_COM_ADDR, RTL9330_COM_rxvlan_OFFSET, RTL9330_COM_rxvlan_MASK, 1);


        /* config tx */
        _ioal_soc_mem32_field_write(unit, RTL9330_TX_CFG_ADDR, RTL9330_TX_CFG_r_tx_jumbo_OFFSET, RTL9330_TX_CFG_r_tx_jumbo_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_TX_CFG_ADDR, RTL9330_TX_CFG_r_tx_multipkt_cnt_pause_OFFSET, RTL9330_TX_CFG_r_tx_multipkt_cnt_pause_MASK, 0);
        _ioal_soc_mem32_field_write(unit, RTL9330_TX_CFG_ADDR, RTL9330_TX_CFG_r_tx_nopadding_OFFSET, RTL9330_TX_CFG_r_tx_nopadding_MASK, 0);
        _ioal_soc_mem32_field_write(unit, RTL9330_ETN_IO_CMD_ADDR, RTL9330_ETN_IO_CMD_en_early_tx_OFFSET, RTL9330_ETN_IO_CMD_en_early_tx_MASK, 1);

        /*config MAR*/
        ioal_soc_mem32_write(unit, RTL9330_MAR0_ADDR, 0xffffffff);
        ioal_soc_mem32_write(unit, RTL9330_MAR4_ADDR, 0xffffffff);


        /*config RCR*/
        _ioal_soc_mem32_field_write(unit, RTL9330_RX_CFG_ADDR, RTL9330_RX_CFG_AER_OFFSET, RTL9330_RX_CFG_AER_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_RX_CFG_ADDR, RTL9330_RX_CFG_AR_OFFSET, RTL9330_RX_CFG_AR_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_RX_CFG_ADDR, RTL9330_RX_CFG_AB_OFFSET, RTL9330_RX_CFG_AB_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_RX_CFG_ADDR, RTL9330_RX_CFG_AM_OFFSET, RTL9330_RX_CFG_AM_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_RX_CFG_ADDR, RTL9330_RX_CFG_APM_OFFSET, RTL9330_RX_CFG_APM_MASK, 1);
        _ioal_soc_mem32_field_write(unit, RTL9330_RX_CFG_ADDR, RTL9330_RX_CFG_AAP_OFFSET, RTL9330_RX_CFG_AAP_MASK, 1);

        /*config CONFIG*/
        _ioal_soc_mem32_field_write(unit, RTL9330_CONFIG_ADDR, RTL9330_CONFIG_r_en_rff_afull_OFFSET, RTL9330_CONFIG_r_en_rff_afull_MASK, 0);
        _ioal_soc_mem32_field_write(unit, RTL9330_CONFIG_ADDR, RTL9330_CONFIG_R_lxm_swap_OFFSET, RTL9330_CONFIG_R_lxm_swap_MASK, 1);

        OOB_SEM_UNLOCK(unit);
    }



    return RT_ERR_OK;
}


int32   dal_bumblebee_oob_swOobRst_set(uint32 unit, rtk_port_t port)
{
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    _ioal_soc_mem32_field_write(unit, RTL9330_COM_ADDR, RTL9330_COM_rst_OFFSET, RTL9330_COM_rst_MASK, 1);
    OOB_SEM_UNLOCK(unit);

    return RT_ERR_OK;
}

int32   dal_bumblebee_oob_ringEnable_set(uint32  unit, rtk_port_t port, oob_dir_t dir, uint32 ringId, rtk_enable_t enable)
{

    RT_INIT_CHK(oob_init[unit]);
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
 *      _dal_bumblebee_oob_busyWait_ready
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
_dal_bumblebee_oob_busyWait_ready(uint32 unit, int32 (*chk_ready_f)(uint32 unit, void *pData), void *pData, osal_usecs_t timeout_us/* microseconds */)
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
_dal_bumblebee_oob_sdsIndrAccComplete_chk(uint32 unit, void *pData)
{
    uint32 regVal, *wait_bit_val = pData;
    int32 ret;

    if ((ret = ioal_soc_mem32_read(unit, RTL9330_SDS_CFG1_ADDR, &regVal)) != RT_ERR_OK)
    {
        return ret;
    }

    if (*wait_bit_val == (regVal & RTL9330_SDS_CFG1_Sds_reg_rdy_MASK))
    {
        return RT_ERR_OK;
    }
    return RT_ERR_NOT_FINISH;
}


uint32
_dal_bumblebee_oob_sds_indr_cmd(uint32 val, uint32 sds_ce_out, uint32 sds_cmd, uint32 sds_cen_in, uint32 sds_we_in)
{
    //ce_out is cd_out
    val = (val & (~RTL9330_SDS_CFG1_Sds_cd_out_MASK)) | ((sds_ce_out << RTL9330_SDS_CFG1_Sds_cd_out_OFFSET) & RTL9330_SDS_CFG1_Sds_cd_out_MASK);
    val = (val & (~RTL9330_SDS_CFG1_Sds_cmd_MASK)) | ((sds_cmd << RTL9330_SDS_CFG1_Sds_cmd_OFFSET) & RTL9330_SDS_CFG1_Sds_cmd_MASK);
    val = (val & (~RTL9330_SDS_CFG1_Sds_cen_in_MASK)) | ((sds_cen_in << RTL9330_SDS_CFG1_Sds_cen_in_OFFSET) & RTL9330_SDS_CFG1_Sds_cen_in_MASK);
    val = (val & (~RTL9330_SDS_CFG1_Sds_we_in_MASK)) | ((sds_we_in << RTL9330_SDS_CFG1_Sds_we_in_OFFSET) & RTL9330_SDS_CFG1_Sds_we_in_MASK);
    return val;
}

/* Function Name:
 *      dal_bumblebee_oob_sdsReg_get
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
dal_bumblebee_oob_sdsReg_get(uint32 unit, uint32 sds, uint32 page, uint32 reg, uint32 *data)
{
    int32 ret;
    uint32 val, fVal;

    val = 0;
    /* 1)
     *  Sds_a_in format: {sds_idx, sds_page, sds_reg}
     *      [15:11]: sds idx. Always 0 for OOB
     *      [10:05]: sds_page
     *      [04:00]: sds_reg
     */
    fVal = ((page & 0x3F) << 5) | (reg & 0x1F);
    val = (val & (~RTL9330_SDS_CFG2_Sds_a_in_MASK)) | ((fVal << RTL9330_SDS_CFG2_Sds_a_in_OFFSET) & RTL9330_SDS_CFG2_Sds_a_in_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_SDS_CFG2_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

    /* 2)
     *  sds_ce_out set 1
     *  sds_cmd set 1
     *  sds_cen_in set 1
     *  sds_we_in set 0
     */
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_SDS_CFG1_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }
    val = _dal_bumblebee_oob_sds_indr_cmd(val, 1 /*sds_ce_out*/,  1 /*sds_cmd*/, 1 /*sds_cen_in*/, 0 /*sds_we_in*/);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_SDS_CFG1_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

    /* 3)
     *  sds_ce_out set 1
     *  sds_cmd set 1
     *  sds_cen_in set 0
     *  sds_we_in set 0
     */
    val = _dal_bumblebee_oob_sds_indr_cmd(val, 1 /*sds_ce_out*/,  1 /*sds_cmd*/, 0 /*sds_cen_in*/, 0 /*sds_we_in*/);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_SDS_CFG1_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }


    if ((ret = ioal_soc_mem32_read(unit, RTL9330_SDS_CFG2_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }
    *data = (val & RTL9330_SDS_CFG2_Sds_dio_MASK) >> RTL9330_SDS_CFG2_Sds_dio_OFFSET;

  out:
    /* 5)
     *  sds_ce_out set 0
     *  sds_cmd set 0
     *  sds_cen_in set 0
     *  sds_we_in set 0
     */
    ioal_soc_mem32_read(unit, RTL9330_SDS_CFG1_ADDR, &val);
    val = _dal_bumblebee_oob_sds_indr_cmd(val, 0 /*sds_ce_out*/,  0 /*sds_cmd*/, 0 /*sds_cen_in*/, 0 /*sds_we_in*/);
    ioal_soc_mem32_write(unit, RTL9330_SDS_CFG1_ADDR, val);

    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_sdsReg_set
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
dal_bumblebee_oob_sdsReg_set(uint32 unit, uint32 sds, uint32 page, uint32 reg, uint32 data)
{
    int32 ret;
    uint32 val, fVal;


    val = 0;
    /* 1)
     *  Sds_a_in format: {sds_idx, sds_page, sds_reg}
     *      [15:11]: sds idx. Always 0 for OOB
     *      [10:05]: sds_page
     *      [04:00]: sds_reg
     *  Sds_dio = data
     */
    fVal = ((page & 0x3F) << 5) | (reg & 0x1F);
    val = (val & (~RTL9330_SDS_CFG2_Sds_a_in_MASK)) | ((fVal << RTL9330_SDS_CFG2_Sds_a_in_OFFSET) & RTL9330_SDS_CFG2_Sds_a_in_MASK);
    val = (val & (~RTL9330_SDS_CFG2_Sds_dio_MASK)) | ((data << RTL9330_SDS_CFG2_Sds_dio_OFFSET) & RTL9330_SDS_CFG2_Sds_dio_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_SDS_CFG2_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

    /* 2)
     *  sds_ce_out set 1
     *  sds_cmd set 1
     *  sds_cen_in set 1
     *  sds_we_in set 1
     */
    val = _dal_bumblebee_oob_sds_indr_cmd(val, 1 /*sds_ce_out*/,  1 /*sds_cmd*/, 0 /*sds_cen_in*/, 0 /*sds_we_in*/);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_SDS_CFG1_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "oob sds read: unit %u sds %u page %u reg %u", unit, sds, page, reg);
        goto out;
    }

  out:
    /* 4)
     *  sds_ce_out set 0
     *  sds_cmd set 0
     *  sds_cen_in set 0
     *  sds_we_in set 0
     */
    ioal_soc_mem32_read(unit, RTL9330_SDS_CFG1_ADDR, &val);
    val = _dal_bumblebee_oob_sds_indr_cmd(val, 0 /*sds_ce_out*/,  0 /*sds_cmd*/, 0 /*sds_cen_in*/, 0 /*sds_we_in*/);
    ioal_soc_mem32_write(unit, RTL9330_SDS_CFG1_ADDR, val);

    return ret;
}


int32
_dal_bumblebee_oob_sdsPatch_set(uint32 unit, int32 sds_id)
{
    int32 i;
    uint32  data, mask;

    for (i = 0; i < sizeof(oob_sds_patch_config)/sizeof(oob_sdsPatchConfType_t); i++)
    {
        dal_bumblebee_oob_sdsReg_get(unit, sds_id, oob_sds_patch_config[i].page, oob_sds_patch_config[i].reg, &data);
        mask = UINT32_BITS_MASK(oob_sds_patch_config[i].msb, oob_sds_patch_config[i].lsb);
        data = REG32_FIELD_SET(data, oob_sds_patch_config[i].val, oob_sds_patch_config[i].lsb, mask);
        dal_bumblebee_oob_sdsReg_set(unit, sds_id, oob_sds_patch_config[i].page, oob_sds_patch_config[i].reg, data);
    }

    return RT_ERR_OK;
}


int32
_dal_bumblebee_oob_sdsMode_set(uint32 unit, int32 sds_id, rt_serdesMode_t sds_mode)
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

    if ((ret = ioal_soc_mem32_read(unit, RTL9330_OOBSR2_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9330_OOBSR2_sds_mode0_MASK)) | ((fVal << RTL9330_OOBSR2_sds_mode0_OFFSET) & RTL9330_OOBSR2_sds_mode0_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_OOBSR2_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
  out:
    return ret;
}

int32
_dal_bumblebee_oob_mii_init(uint32 unit, rtk_port_t port)
{
    uint32  val = 0, fVal;
    int32 ret = RT_ERR_OK;

    /* set PHY address */
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &val)) != RT_ERR_OK)
    {
        goto out;
    }

    fVal = HWP_PHY_ADDR(unit, port);
    val = (val & (~RTL9330_MII_ACCESS_PHYAddress_MASK)) | ((fVal << RTL9330_MII_ACCESS_PHYAddress_OFFSET) & RTL9330_MII_ACCESS_PHYAddress_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MII_ACCESS_ADDR, val)) != RT_ERR_OK)
    {
        goto out;
    }

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB PHY addr %u\n", fVal);

  out:
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_mgmtMacAddr_get
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
dal_bumblebee_oob_mgmtMacAddr_get(uint32 unit, rtk_port_t port, rtk_mac_t *pMac)
{
    int32 ret;
    uint32 val;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_IDR0_ADDR , &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    pMac->octet[0] = (val >> 24) & 0xff;
    pMac->octet[1] = (val >> 16) & 0xff;
    pMac->octet[2] = (val >> 8) & 0xff;
    pMac->octet[3] = (val >> 0) & 0xff;

    if ((ret = ioal_soc_mem32_read(unit, RTL9330_IDR4_ADDR, &val)) != RT_ERR_OK)
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
 *      dal_bumblebee_oob_mgmtMacAddr_set
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
dal_bumblebee_oob_mgmtMacAddr_set(uint32 unit, rtk_port_t port, rtk_mac_t *pMac)
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
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_IDR0_ADDR , val)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_BSP), "unit %u", unit);
        goto out;
    }

    val = (pMac->octet[4] << 24) | (pMac->octet[5] << 16);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_IDR4_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, (MOD_BSP), "unit %u", unit);
        goto out;
    }

    if ((ret = ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & ~RTL9330_RX_CFG_APM_MASK) | (1 << RTL9330_RX_CFG_APM_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_RX_CFG_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_rxMode_get
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
dal_bumblebee_oob_rxMode_get(uint32 unit, rtk_port_t port, drv_oob_rxMode_t rx_mode, rtk_enable_t *pEnable)
{
    uint32 val, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    switch (rx_mode)
    {
      case OOB_RX_MODE_ACCEPT_ALLUCAST:
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        fVal = (val & RTL9330_RX_CFG_AAP_MASK) >> RTL9330_RX_CFG_AAP_OFFSET;
        *pEnable = (fVal == 1) ? ENABLED : DISABLED;
        break;
      case OOB_RX_MODE_ACCEPT_ALLMCAST:
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        fVal = (val & RTL9330_RX_CFG_APM_MASK) >> RTL9330_RX_CFG_APM_OFFSET;
        *pEnable = (fVal == 1) ? ENABLED : DISABLED;
        break;
      case OOB_RX_MODE_ACCEPT_BCAST:
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        fVal = (val & RTL9330_RX_CFG_AB_MASK) >> RTL9330_RX_CFG_AB_OFFSET;
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
 *      dal_bumblebee_oob_rxMode_set
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
dal_bumblebee_oob_rxMode_set(uint32 unit, rtk_port_t port, drv_oob_rxMode_t rx_mode, rtk_enable_t enable)
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
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9330_RX_CFG_AAP_MASK) | (fVal << RTL9330_RX_CFG_AAP_OFFSET);
        if ((ret = ioal_soc_mem32_write(unit, RTL9330_RX_CFG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        break;
      case OOB_RX_MODE_ACCEPT_ALLMCAST:
        fVal = (enable == ENABLED) ? 1 : 0;

        ioal_soc_mem32_write(unit, RTL9330_MAR0_ADDR, 0xFFFFFFFF);
        ioal_soc_mem32_write(unit, RTL9330_MAR4_ADDR, 0xFFFFFFFF);
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9330_RX_CFG_APM_MASK) | (fVal << RTL9330_RX_CFG_APM_OFFSET);
        if ((ret = ioal_soc_mem32_write(unit, RTL9330_RX_CFG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        break;
      case OOB_RX_MODE_ACCEPT_BCAST:
        fVal = (enable == ENABLED) ? 1 : 0;
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_RX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9330_RX_CFG_AB_MASK) | (fVal << RTL9330_RX_CFG_AB_OFFSET);
        if ((ret = ioal_soc_mem32_write(unit, RTL9330_RX_CFG_ADDR, val)) != RT_ERR_OK)
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
 *      dal_bumblebee_oob_jumboModeEnable_get
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
dal_bumblebee_oob_jumboModeEnable_get(uint32 unit, rtk_port_t port, rtk_switch_pktDir_t dir, rtk_enable_t *pEnable)
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
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_TX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }
        *pEnable = (val & RTL9330_TX_CFG_r_tx_jumbo_MASK) ? ENABLED : DISABLED;
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
dal_bumblebee_oob_jumboModeEnable_set(uint32 unit, rtk_port_t port, rtk_switch_pktDir_t dir, rtk_enable_t enable)
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
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_TX_CFG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9330_TX_CFG_r_tx_jumbo_MASK) | (fVal << RTL9330_TX_CFG_r_tx_jumbo_OFFSET);

        if ((ret = ioal_soc_mem32_write(unit, RTL9330_TX_CFG_ADDR, val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        //CONFIG_REG
        if ((ret = ioal_soc_mem32_read(unit, RTL9330_CONFIG_ADDR, &val)) != RT_ERR_OK)
        {
            RT_ERR(ret, MOD_BSP, "unit %u", unit);
            goto out;
        }

        val = (val & ~RTL9330_CONFIG_r_en_tx_extra_MASK) | (fVal << RTL9330_CONFIG_r_en_tx_extra_OFFSET);

        if ((ret = ioal_soc_mem32_write(unit, RTL9330_CONFIG_ADDR, val)) != RT_ERR_OK)
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
 *      dal_bumblebee_oob_pollingPhyEnable_get
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
dal_bumblebee_oob_pollingPhyEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
    fVal = (val & RTL9330_MII_ACCESS_Disable_auto_polling_MASK) >> RTL9330_MII_ACCESS_Disable_auto_polling_OFFSET;
    *pEnable = (fVal == 1) ? DISABLED : ENABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_pollingPhyEnable_set
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
dal_bumblebee_oob_pollingPhyEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal = (enable == ENABLED) ? 0 : 1;
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9330_MII_ACCESS_Disable_auto_polling_MASK)) | (fVal << RTL9330_MII_ACCESS_Disable_auto_polling_OFFSET);

    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MII_ACCESS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_forceFc_get
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
dal_bumblebee_oob_forceFc_get(uint32 unit, rtk_port_t port, rtk_enable_t *pForceFc, rtk_enable_t *pTxPause, rtk_enable_t *pRxPause)
{
    uint32  val = 0, fVal_forceFc, fVal_txPause, fVal_rxPause;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    fVal_forceFc = (val & RTL9330_MEDIA_STATUS_Force_TRXFCE_MASK) >> RTL9330_MEDIA_STATUS_Force_TRXFCE_OFFSET;
    fVal_txPause = (val & RTL9330_MEDIA_STATUS_TXFCE_MASK) >> RTL9330_MEDIA_STATUS_TXFCE_OFFSET;
    fVal_rxPause = (val & RTL9330_MEDIA_STATUS_RXFCE_MASK) >> RTL9330_MEDIA_STATUS_RXFCE_OFFSET;

    *pForceFc = (fVal_forceFc == 1) ? ENABLED : DISABLED;
    *pTxPause = (fVal_txPause == 1) ? ENABLED : DISABLED;
    *pRxPause = (fVal_rxPause == 1) ? ENABLED : DISABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_forceFc_set
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
dal_bumblebee_oob_forceFc_set(uint32 unit, rtk_port_t port, rtk_enable_t forceFc, rtk_enable_t txPause, rtk_enable_t rxPause)
{
    uint32  val = 0, fVal_forceFc, fVal_txPause, fVal_rxPause;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal_forceFc = (forceFc == ENABLED) ? 1 : 0;
    fVal_txPause = (txPause == ENABLED) ? 1 : 0;
    fVal_rxPause = (rxPause == ENABLED) ? 1 : 0;

    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9330_MEDIA_STATUS_Force_TRXFCE_MASK)) | (fVal_forceFc << RTL9330_MEDIA_STATUS_Force_TRXFCE_OFFSET);
    val = (val & (~RTL9330_MEDIA_STATUS_TXFCE_MASK)) | (fVal_txPause << RTL9330_MEDIA_STATUS_TXFCE_OFFSET);
    val = (val & (~RTL9330_MEDIA_STATUS_RXFCE_MASK)) | (fVal_rxPause << RTL9330_MEDIA_STATUS_RXFCE_OFFSET);

    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_forceFullDuplex_get
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
dal_bumblebee_oob_forceFullDuplex_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }
    fVal = (val & RTL9330_MEDIA_STATUS_FULLDUP_MASK) >> RTL9330_MEDIA_STATUS_FULLDUP_OFFSET;
    *pEnable = (fVal == 1) ? ENABLED: DISABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;

}

/* Function Name:
 *      dal_bumblebee_oob_forceFullDuplex_set
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
dal_bumblebee_oob_forceFullDuplex_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal = (enable == ENABLED) ? 1 : 0;
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9330_MEDIA_STATUS_FULLDUP_MASK)) | (fVal << RTL9330_MEDIA_STATUS_FULLDUP_OFFSET);

    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_forceSpeed_get
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
dal_bumblebee_oob_forceSpeed_get(uint32 unit, rtk_port_t port, rtk_enable_t *pForceSpd, rtk_port_speed_t *pSpeed)
{
    uint32  val = 0, fVal_spd;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
         goto out;
    }

    fVal_spd = (val & RTL9330_MEDIA_STATUS_FORCE_SPD_MASK) >> RTL9330_MEDIA_STATUS_FORCE_SPD_OFFSET;
    if (fVal_spd == 0x3)
    {
        *pForceSpd = DISABLED;
        *pSpeed = PORT_SPEED_10M;
    }
    else
    {
        *pForceSpd = ENABLED;
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
 *      dal_bumblebee_oob_forceSpeed_set
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
dal_bumblebee_oob_forceSpeed_set(uint32 unit, rtk_port_t port, rtk_enable_t forceSpd, rtk_port_speed_t speed)
{
    uint32  val = 0, fVal_spd;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    if (forceSpd == ENABLED)
    {
        if (speed == PORT_SPEED_1000M)
        {
            fVal_spd = 0x2;
        }
        else if (speed == PORT_SPEED_100M)
        {
            fVal_spd = 0x0;
        }
        else if (speed == PORT_SPEED_10M)
        {
            fVal_spd = 0x1;
        }
        else
        {
            return RT_ERR_INPUT;
        }
    }
    else
    {
        fVal_spd = 0x3;
    }

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9330_MEDIA_STATUS_FORCE_SPD_MASK)) | (fVal_spd << RTL9330_MEDIA_STATUS_FORCE_SPD_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_forceLink_get
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
dal_bumblebee_oob_forceLink_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    fVal = (val & RTL9330_MEDIA_STATUS_FORCE_LINK_MASK) >> RTL9330_MEDIA_STATUS_FORCE_LINK_OFFSET;
    *pEnable = (fVal == 1) ? ENABLED : DISABLED;

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_forceLink_set
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
dal_bumblebee_oob_forceLink_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  val = 0, fVal;
    int32 ret;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    fVal = (enable == ENABLED) ? 1 : 0;
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MEDIA_STATUS_ADDR, &val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

    val = (val & (~RTL9330_MEDIA_STATUS_FORCE_LINK_MASK)) | (fVal << RTL9330_MEDIA_STATUS_FORCE_LINK_OFFSET);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MEDIA_STATUS_ADDR, val)) != RT_ERR_OK)
    {
        RT_ERR(ret, MOD_BSP, "unit %u", unit);
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}

/* Function Name:
 *      dal_bumblebee_oob_stat_getAll
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
 *      rtk_stat_port_cntr_t \ Chip:              9330
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
dal_bumblebee_oob_stat_getAll(uint32 unit, rtk_port_t port, rtk_stat_port_cntr_t *pCntrs)
{
    uint32  val;

    RT_INIT_CHK(oob_init[unit]);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    OOB_SEM_LOCK(unit);
    ioal_soc_mem32_read(unit, RTL9330_DOT1DTPPORTINDISCARDS_ADDR, &val);
    pCntrs->dot1dTpPortInDiscards = val;

    ioal_soc_mem32_read(unit, RTL9330_IFLNOCTETS_H_ADDR, &val);
    pCntrs->ifHCInOctets = val;
    ioal_soc_mem32_read(unit, RTL9330_IFLNOCTETS_L_ADDR, &val);
    pCntrs->ifHCInOctets = (pCntrs->ifHCInOctets << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFLNUCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCInUcastPkts = val;
    ioal_soc_mem32_read(unit, RTL9330_IFLNUCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCInUcastPkts = (pCntrs->ifHCInUcastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFLNMULTICASTPKTS_H_ADDR, &val);
    pCntrs->ifHCInMulticastPkts = val;
    ioal_soc_mem32_read(unit, RTL9330_IFLNMULTICASTPKTS_L_ADDR, &val);
    pCntrs->ifHCInMulticastPkts = (pCntrs->ifHCInMulticastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFLNBROADCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCInBroadcastPkts = val;
    ioal_soc_mem32_read(unit, RTL9330_IFLNBROADCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCInBroadcastPkts = (pCntrs->ifHCInBroadcastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFOUTOCTETS_H_ADDR, &val);
    pCntrs->ifHCOutOctets = val;
    ioal_soc_mem32_read(unit, RTL9330_IFOUTOCTETS_L_ADDR, &val);
    pCntrs->ifHCOutOctets = (pCntrs->ifHCOutOctets << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFOUTUCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCOutUcastPkts = val;
    ioal_soc_mem32_read(unit, RTL9330_IFOUTUCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCOutUcastPkts = (pCntrs->ifHCOutUcastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFOUTMULTICASTPKTS_H_ADDR, &val);
    pCntrs->ifHCOutMulticastPkts = val;
    ioal_soc_mem32_read(unit, RTL9330_IFOUTMULTICASTPKTS_L_ADDR, &val);
    pCntrs->ifHCOutMulticastPkts = (pCntrs->ifHCOutMulticastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFOUTBROADCASTPKTS_H_ADDR, &val);
    pCntrs->ifHCOutBrocastPkts = val;
    ioal_soc_mem32_read(unit, RTL9330_IFOUTBROADCASTPKTS_L_ADDR, &val);
    pCntrs->ifHCOutBrocastPkts = (pCntrs->ifHCOutBrocastPkts << 32) | val;

    ioal_soc_mem32_read(unit, RTL9330_IFOUTDISCARDS_ADDR, &val);
    pCntrs->ifOutDiscards = val;

    OOB_SEM_UNLOCK(unit);

    if (oob_debug_flag & DBG_HWINFO_BIT)
    {
        dal_bumblebee_oob_hw_info_dump(unit, port);
    }
    return RT_ERR_OK;
}


int32
_dal_bumblebee_oob_miimAccComplete_chk(uint32 unit, void *pData)
{
    uint32 regVal, *wait_bit_val = pData;
    int32 ret;

    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &regVal)) != RT_ERR_OK)
    {
        return ret;
    }

    if (*wait_bit_val == (regVal & RTL9330_MII_ACCESS_Flag_MASK))
    {
        return RT_ERR_OK;
    }
    return RT_ERR_NOT_FINISH;
}

/* Function Name:
 *      dal_bumblebee_oob_miiBus_read
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
dal_bumblebee_oob_miiBus_read(uint32 unit, uint32 mii_bus, uint32 phy_addr, uint32 phy_reg, uint32 *pData)
{
    int32   ret;
    uint32  acc_reg, wait_bit_val;

    RT_INIT_CHK(oob_init[unit]);
    if (mii_bus < RTK_OOB_SMI_START || mii_bus > RTK_OOB_SMI_END)
    {
        return RT_ERR_INPUT;
    }

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    acc_reg = REG32_FIELD_SET(acc_reg, phy_addr, RTL9330_MII_ACCESS_PHYAddress_OFFSET, RTL9330_MII_ACCESS_PHYAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, phy_reg, RTL9330_MII_ACCESS_RegAddress_OFFSET, RTL9330_MII_ACCESS_RegAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, 0, RTL9330_MII_ACCESS_Flag_OFFSET, RTL9330_MII_ACCESS_Flag_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MII_ACCESS_ADDR, acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    /* wait access ready */
    wait_bit_val = RTL9330_MII_ACCESS_Flag_MASK;
    if ((ret = _dal_bumblebee_oob_busyWait_ready(unit,
                    _dal_bumblebee_oob_miimAccComplete_chk,
                    &wait_bit_val,
                    RTL9330_ACCESS_PHY_TIMEOUT_TIME)) != RT_ERR_OK)
    {
        goto out;
    }

    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    *pData = REG32_FIELD_GET(acc_reg, RTL9330_MII_ACCESS_Data_OFFSET, RTL9330_MII_ACCESS_Data_MASK);

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;

}

/* Function Name:
 *      dal_bumblebee_oob_miiBus_write
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
dal_bumblebee_oob_miiBus_write(uint32 unit, uint32 mii_bus, uint32 phy_addr, uint32 phy_reg, uint32 data)
{
    int32   ret;
    uint32  acc_reg, wait_bit_val;

    RT_INIT_CHK(oob_init[unit]);
    if (mii_bus < RTK_OOB_SMI_START || mii_bus > RTK_OOB_SMI_END)
    {
        return RT_ERR_INPUT;
    }

    OOB_SEM_LOCK(unit);
    if ((ret = ioal_soc_mem32_read(unit, RTL9330_MII_ACCESS_ADDR, &acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    acc_reg = REG32_FIELD_SET(acc_reg, phy_addr, RTL9330_MII_ACCESS_PHYAddress_OFFSET, RTL9330_MII_ACCESS_PHYAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, phy_reg, RTL9330_MII_ACCESS_RegAddress_OFFSET, RTL9330_MII_ACCESS_RegAddress_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, data, RTL9330_MII_ACCESS_Data_OFFSET, RTL9330_MII_ACCESS_Data_MASK);
    acc_reg = REG32_FIELD_SET(acc_reg, 1, RTL9330_MII_ACCESS_Flag_OFFSET, RTL9330_MII_ACCESS_Flag_MASK);
    if ((ret = ioal_soc_mem32_write(unit, RTL9330_MII_ACCESS_ADDR, acc_reg)) != RT_ERR_OK)
    {
        goto out;
    }

    /* wait access ready */
    wait_bit_val = 0x0;
    if ((ret = _dal_bumblebee_oob_busyWait_ready(unit,
                    _dal_bumblebee_oob_miimAccComplete_chk,
                    &wait_bit_val,
                    RTL9330_ACCESS_PHY_TIMEOUT_TIME)) != RT_ERR_OK)
    {
        goto out;
    }

  out:
    OOB_SEM_UNLOCK(unit);
    return ret;
}


/* Function Name:
 *      dal_bumblebee_oob_mac_init
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
dal_bumblebee_oob_mac_init(uint32 unit, rtk_port_t port)
{
    uint32  sds_id = HWP_PORT_SDSID(unit, port);
    int32 ret = RT_ERR_OK;

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB Init unit=%u, port=%u\n", unit, port);
    if (oob_sem[unit] == 0)
    {
        oob_sem[unit] = osal_sem_mutex_create();
    }

    _dal_bumblebee_oob_mii_init(unit, port);

    if (sds_id < RTK_OOB_SDS_START || sds_id > RTK_OOB_SDS_END)
    {
        ret = RT_ERR_OUT_OF_RANGE;
        RT_ERR(ret, MOD_BSP, "OOB unit %u port %u error sds id %u", unit, port, sds_id);
    }
    else
    {
        _dal_bumblebee_oob_sdsMode_set(unit, sds_id, HWP_SDS_MODE(unit, sds_id));
        _dal_bumblebee_oob_sdsPatch_set(unit, sds_id);
    }

    return ret;
}


