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
 * $Revision$
 * $Date$
 *
 * Purpose : Definition of Network Interface Controller API
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) CPU Tag
 *           (2) NIC Tx
 *           (3) NIC Rx
 *
 */

/*
 * Include Files
 */
#ifdef __UBOOT__
#include <linux/dma-mapping.h>
#else
#include <linux/pci.h>
#endif
#include <dev_config.h>
#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
#include <osal/wait.h>
#include <osal/workqueue.h>
#endif
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_mapper.h>
#include <private/drv/swcore/swcore.h>
#include <private/drv/swcore/chip_probe.h>
#include <drv/l2ntfy/l2ntfy.h>
#include <private/drv/l2ntfy/l2ntfy_util.h>
#include <soc/type.h>
#include <common/debug/rt_log.h>
#include <common/error.h>
#include <ioal/mem32.h>
#include <osal/isr.h>
#include <osal/cache.h>
#include <osal/lib.h>
#include <osal/memory.h>
#include <osal/print.h>
#include <osal/thread.h>
#include <osal/time.h>
#include <osal/sem.h>
#include <osal/spl.h>
#ifndef __BOOTLOADER__
#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
#include <osal/workqueue.h>
#endif
#endif
#include <private/drv/nic/nic_diag.h>
#include <hal/chipdef/chip.h>
#if defined(CONFIG_SDK_DRIVER_L2NTFY)
#include <drv/l2ntfy/l2ntfy.h>
#endif
#include <hwp/hw_profile.h>
#include <common/util/rt_util_time.h>
#include <ioal/ioal_init.h>
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
#include <osal/wait.h>
#include <osal/atomic.h>
#endif

/*
 * Symbol Definition
 */
#define CACHE_LINE_SIZE                 32

#define NIC_RING_WRAPBIT                (0x1 << 1)
#define NIC_RING_SWOWNBIT               (0x1 << 0)
#define NIC_ADDR_MASK                   0xFFFFFFFC
#define DEBUG_DUMP_PKT_LEN              256
#define MAX_NIC_PKT_CNT                 10000
#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
#define JUMBO_CLUSTER_NUM               5
#define NIC_RX_THREAD_STACK_SIZE        8192
#define NIC_RX_THREAD_PRI               0
#endif

#define RX_PTR_UPD_THR                  8
#define CPU_PTR_FLAG_OFFSET             31
#define HW_PTR_FLAG_OFFSET              31
#define PKT_MORE_FLAG_OFFSET            31
#define TX_FETCH_FLAG_OFFSET            30

#define DEBUG_DUMP_PKT_LEN      256
#define CRCPOLY                 0xEDB88320
#define CRC_TBL_SIZE            256

#define MEM_BASE_ADDR_INT       0x10000000
#define MEM_BASE_ADDR_EXT       0

#if defined(CONFIG_SDK_DRIVER_L2NTFY) && defined(CONFIG_SDK_RTL9311B)
#define NTFY_INTR_NUM           3
#else
#define NTFY_INTR_NUM           0
#endif


/*
 * Data Type Definition
 */
typedef struct nic_rx_cb_entry_s
{
    drv_nic_rx_cb_f rx_callback;
    void *pCookie;
} nic_rx_cb_entry_t;

typedef struct nic_pkthdr_s
{
    uint8 *buf_addr;
#if defined(CONFIG_SDK_ENDIAN_LITTLE)
    /* word [0] */
    uint16  buf_size;
    uint16  reserve;
    /* word [1] */
    uint16  buf_len;
    uint16  pkt_offset:14;
    uint16      :1;
    uint16  more:1;
#else
    /* word [0] */
    uint16  reserve;
    uint16  buf_size;
    /* word [1] */
    uint16  more:1;
    uint16      :1;
    uint16  pkt_offset:14;
    uint16  buf_len;
#endif
    uint8   cpuTag[CPUTAG_SIZE];

    /* Used by Software */
    struct drv_nic_pkt_s *packet;
    uint32  *ring_entry;
    drv_nic_tx_cb_f tx_callback;
    void    *cookie;
} nic_pkthdr_t;

typedef struct pkt_priv_s
{
    drv_nic_pkt_t   *packet;
    drv_nic_tx_cb_f tx_callback;
    void            *cookie;
} pkt_priv_t;


#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
typedef struct nic_collectArrayList_s
{
    uint32                  unit;
    osal_list_head_t        list;
    struct drv_nic_pkt_s    *pPacket;
    uint8                   jumbo;
} nic_collectArrayList_t;
#endif

/*
 * Data Declaration
 */
static uint32   nic_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};
static nic_rx_cb_entry_t _nic_rx_cb_tbl[RTK_MAX_NUM_OF_UNIT][NIC_RX_CB_PRIORITY_NUMBER];
static drv_nic_initCfg_t _nic_init_conf[RTK_MAX_NUM_OF_UNIT];

/* Pointer for Software */
static nic_desc_t   **pNic_rxRBase[RTK_MAX_NUM_OF_UNIT];
static nic_desc_t   **pNic_txRBase[RTK_MAX_NUM_OF_UNIT];
static pkt_priv_t 	**pRx_pkt_privAry[RTK_MAX_NUM_OF_UNIT];
static pkt_priv_t 	**pTx_pkt_privAry[RTK_MAX_NUM_OF_UNIT];
static drv_nic_pkt_t **pPacket[RTK_MAX_NUM_OF_UNIT];
static uint32       *jumboFlag[RTK_MAX_NUM_OF_UNIT];
static uint32       *jumboBuffAlloc[RTK_MAX_NUM_OF_UNIT];

static uint32       *rx_cpu_ptr[RTK_MAX_NUM_OF_UNIT];
static uint32       *rx_cpu_flag[RTK_MAX_NUM_OF_UNIT];
static uint32       *cur_rx[RTK_MAX_NUM_OF_UNIT];
static uint32       *cur_rx_flag[RTK_MAX_NUM_OF_UNIT];
static uint32       *tx_cpu_ptr[RTK_MAX_NUM_OF_UNIT];
static uint32       *tx_cpu_flag[RTK_MAX_NUM_OF_UNIT];
static uint32       *cur_tx[RTK_MAX_NUM_OF_UNIT];
static uint32       *cur_tx_flag[RTK_MAX_NUM_OF_UNIT];

static uint32       *rxRingIdSize[RTK_MAX_NUM_OF_UNIT];
static uint32       *txRingIdSize[RTK_MAX_NUM_OF_UNIT];
static uint32       rxRingNum[RTK_MAX_NUM_OF_UNIT];
static uint32       txRingNum[RTK_MAX_NUM_OF_UNIT];
static uint32       rx_cTag_len[RTK_MAX_NUM_OF_UNIT] = {0};
static uint32       tx_cTag_len[RTK_MAX_NUM_OF_UNIT] = {0};
static uint32       totalRxRingNum[RTK_MAX_NUM_OF_UNIT] = {0};
static uint32       totalTxRingNum[RTK_MAX_NUM_OF_UNIT] = {0};

/* NIC Tx/Rx debug information
 * The machanism is always enabled
 */
static uint32       nic_debug_flag[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_tx_success_cntr[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_tx_failed_cntr[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_rx_success_cntr[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_rx_failed_cntr[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_rx_crc_cntr[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_tx_isr_cntr[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_tx_ring_cntr[RTK_MAX_NUM_OF_UNIT];
static uint32       nic_rx_err_cnt[RTK_MAX_NUM_OF_UNIT] = {0};
static uint32       nic_rx_unExp_err_cnt[RTK_MAX_NUM_OF_UNIT] = {0};
static uint32       rx_runout_cnt[RTK_MAX_NUM_OF_UNIT] = {0};
static uint32       nic_rx_isr_alloc_fail_cnt[RTK_MAX_NUM_OF_UNIT] = {0};
static uint32       nic_rx_mb_alloc_fail_cnt[RTK_MAX_NUM_OF_UNIT] = {0};

static uint32       cpuTagId[RTK_MAX_NUM_OF_UNIT];
static uint32       rxCRCInclude[RTK_MAX_NUM_OF_UNIT];
static uint32       gJumboSize[RTK_MAX_NUM_OF_UNIT];

#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
static osal_event_t                 drv_nicIsr_event;
static osal_atomic_t                nic_rx_pkt_cnt = OSAL_ATOMIC_INIT(0);
static osal_atomic_t                nic_mem_lock = OSAL_ATOMIC_INIT(0);
static uint32                       nicLockCnt, nicReleaseCnt;
static nic_collectArrayList_t       nicListHead;
osal_thread_t                       rxThread_id;
//static osal_spinlock_t              irqLock;
#endif


#if (!defined(CONFIG_SDK_EXTERNAL_CPU) && defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)) || defined(__BOOTLOADER__)
static isr_param_t      *my_isr_param[RTK_MAX_NUM_OF_UNIT];
#endif

#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
uintptr                 pkt_buf_vir_addres[RTK_MAX_NUM_OF_UNIT];
uintptr                 pkt_phy_addr[RTK_MAX_NUM_OF_UNIT];
#endif

uint32                  gIsrRegistered[RTK_MAX_NUM_OF_UNIT];

static uint32           crcTable[CRC_TBL_SIZE];
static uint32           _nic_rx_intr_cb_cnt[RTK_MAX_NUM_OF_UNIT] = {0};

static uintptr                  tx_desc_vaddr[RTK_MAX_NUM_OF_UNIT];
static uintptr                  tx_desc_phy_addr[RTK_MAX_NUM_OF_UNIT];
static uintptr                  rx_desc_vaddr[RTK_MAX_NUM_OF_UNIT];
static uintptr                  rx_desc_phy_addr[RTK_MAX_NUM_OF_UNIT];
static uintptr                  mem_base_addr[RTK_MAX_NUM_OF_UNIT];

uint64                  nml_rx_alloc_cnt[RTK_MAX_NUM_OF_UNIT];
uint64                  nml_tx_alloc_cnt[RTK_MAX_NUM_OF_UNIT];
uint64                  nml_free_cnt[RTK_MAX_NUM_OF_UNIT];
uint64                  jumbo_rx_alloc_cnt[RTK_MAX_NUM_OF_UNIT];
uint64                  jumbo_tx_alloc_cnt[RTK_MAX_NUM_OF_UNIT];
uint64                  jumbo_free_cnt[RTK_MAX_NUM_OF_UNIT];
uint32                  free_fail_cnt[RTK_MAX_NUM_OF_UNIT];

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
  static osal_event_t     alloc_fail_event;
  static osal_thread_t    alloc_fail_thread_id = 0;
  static osal_atomic_t    alloc_thrd_run = OSAL_ATOMIC_INIT(0);
  static uint32       nic_alloc_monThr_acc_cnt = 0;

#endif


/*
 * Macro Definition
 */
#ifdef __BOOTLOADER__
  #define NIC_LOCK(unit)
  #define NIC_UNLOCK(unit)
  #define NIC_RX_LOCK(unit, flags)
  #define NIC_RX_UNLOCK(unit, flags)
  #define NIC_TX_LOCK(unit, flags)
  #define NIC_TX_UNLOCK(unit, flags)
#else
  #if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
static osal_mutex_t     nic_sem[RTK_MAX_NUM_OF_UNIT];
static osal_mutex_t     nic_tx_sem[RTK_MAX_NUM_OF_UNIT];

	#define NIC_LOCK(unit)    \
	do {\
	    if (osal_sem_mutex_take(nic_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
	    {\
	        RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_NIC), "nic semaphore lock failed");\
	        return RT_ERR_SEM_LOCK_FAILED;\
	    }\
	} while(0)
	#define NIC_UNLOCK(unit)   \
	do {\
	    if (osal_sem_mutex_give(nic_sem[unit]) != RT_ERR_OK)\
	    {\
	        RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_NIC), "nic semaphore unlock failed");\
	        return RT_ERR_SEM_UNLOCK_FAILED;\
	    }\
	} while(0)

    #define NIC_RX_LOCK(unit, flags)
    #define NIC_RX_UNLOCK(unit, flags)
    #define NIC_TX_LOCK(unit, flags)    \
    do {\
        if (osal_sem_mutex_take(nic_tx_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_NIC), "nic_tx semaphore lock failed");\
            return RT_ERR_SEM_LOCK_FAILED;\
        }\
    } while(0)
    #define NIC_TX_UNLOCK(unit, flags)   \
    do {\
        if (osal_sem_mutex_give(nic_tx_sem[unit]) != RT_ERR_OK)\
        {\
            RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_NIC), "nic_tx semaphore unlock failed");\
            return RT_ERR_SEM_UNLOCK_FAILED;\
        }\
    } while(0)
  #else
    osal_spinlock_t         spl_rx_lock[RTK_MAX_NUM_OF_UNIT], spl_tx_lock[RTK_MAX_NUM_OF_UNIT];
    #define NIC_LOCK(unit)
    #define NIC_UNLOCK(unit)
    #define NIC_RX_LOCK(unit, flags)               osal_spl_spin_lock_irqsave(spl_rx_lock[unit], flags)
    #define NIC_RX_UNLOCK(unit, flags)             osal_spl_spin_unlock_irqrestore(spl_rx_lock[unit], flags)
    #define NIC_TX_LOCK(unit, flags)               osal_spl_spin_lock_irqsave(spl_tx_lock[unit], flags)
    #define NIC_TX_UNLOCK(unit, flags)             osal_spl_spin_unlock_irqrestore(spl_tx_lock[unit], flags)
  #endif
#endif
#if defined(CONFIG_TARGET_ARCH_ARM)
  #define MEMORY_BARRIER()        ({ __asm__ __volatile__ ("dsb 0xF" : : : "memory");})
#else
  #define MEMORY_BARRIER()        ({ __asm__ __volatile__ ("": : :"memory"); })
#endif

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      dma_map_single(dev, usr_addr, size, dir)
#elif defined(__BOOTLOADER__)
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      dma_map_single(usr_addr, size, dir)
#else
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      ((usr_addr - pkt_buf_vir_addres[unit]) + pkt_phy_addr[unit])
  #define unlikely(x) (x)
  #define likely(x)   (x)
#endif
#define PHY_2_DESC_ADDR(mapping)    (CPU_to_BE32((uint64)mapping & 0xffffffff) | (((uint64)CPU_to_BE32(((uint64)mapping >> 32) & 0xffffffff)) << 32))
#define DESC_ADDR_2_PHY(addr)    (BE32_to_CPU((uint64)addr & 0xffffffff) | (((uint64)BE32_to_CPU(((uint64)addr >> 32) & 0xffffffff)) << 32))
#define INFO_MAKE_LEN(len)      ((len) << 2)
#define INFO_MAKE_MORE(x)          ((x) << 31)

#if !defined(__KERNEL__)
#define cpu_to_le16
#define cpu_to_le32
#define cpu_to_le64
#define le16_to_cpu
#define le32_to_cpu
#define le64_to_cpu
#endif /* !defined(__KERNEL__) */



/*
 * Function Declaration
 */
int32 _nic_ring_free(uint32 unit);
int32 _nic_ring_init(uint32 unit);

int32 _nic_hw_init(uint32 unit)
{
    return NIC_CTRL(unit).init(unit, NULL);
}

int32 _nic_ringInfo_get(uint32 unit, nic_dir_t type, uint32 **rxRingIdSizeList, uint32 *pRingNum)
{
    return NIC_CTRL(unit).ringInfo_get(unit, type, rxRingIdSizeList, pRingNum);
}

int32 _nic_local_ringSts_get(uint32 unit, uint32 *pSts)
{
    return NIC_CTRL(unit).local_ringSts_get(unit, pSts);
}

int32 _nic_rx_close_set(uint32 unit, uint32 threshold, uint32 timeout)
{
    return NIC_CTRL(unit).rx_close_set(unit, threshold, timeout);
}

int32 _nic_cpuPortTxRxEnable_set(uint32 unit, rtk_enable_t enable)
{
    return NIC_CTRL(unit).cpuPortTxRxEnable_set(unit, enable);
}

int32 _nic_intrMask_get(uint32 unit, nic_intr_type_t type, uint32 *pMask)
{
    return NIC_CTRL(unit).intrMask_get(unit, type, pMask);
}

int32 _nic_intrMask_set(uint32 unit, nic_intr_type_t type, uint32 mask)
{
    return NIC_CTRL(unit).intrMask_set(unit, type, mask);
}

int32 _nic_intrSts_get(uint32 unit, nic_intr_type_t type, uint32 *pVal)
{
    return NIC_CTRL(unit).intrSts_get(unit, type, pVal);
}

int32 _nic_intrSts_set(uint32 unit, nic_intr_type_t type, uint32 val)
{
    return NIC_CTRL(unit).intrSts_set(unit, type, val);
}

int32 _nic_swNicRst_get(uint32 unit, uint32 *pStatus)
{
    return NIC_CTRL(unit).swNicRst_get(unit, pStatus);
}

int32 _nic_swNicRst_set(uint32 unit)
{
    return NIC_CTRL(unit).swNicRst_set(unit);
}

int32 _nic_swQueRst_get(uint32 unit, uint32 *pStatus)
{
    return NIC_CTRL(unit).swQueRst_get(unit, pStatus);
}

int32 _nic_swQueRst_set(uint32 unit)
{
    return NIC_CTRL(unit).swQueRst_set(unit);
}

int32 _nic_cpuL2FloodMask_add(uint32 unit)
{
    return NIC_CTRL(unit).cpuL2FloodMask_add(unit);
}

int32 _nic_cpuL2FloodMask_remove(uint32 unit)
{
    return NIC_CTRL(unit).cpuL2FloodMask_remove(unit);
}

int32 _nic_cpuForceLinkupEnable_set(uint32 unit, rtk_enable_t enable)
{
    return NIC_CTRL(unit).cpuForceLinkupEnable_set(unit, enable);
}

int32 _nic_holRingCnt_get(uint32 unit, uint32 ring, uint32 *pVal)
{
    return NIC_CTRL(unit).holRingCnt_get(unit, ring, pVal);
}

int32 _nic_holRingSize_set(uint32 unit, uint32 ring, uint32 val)
{
    return NIC_CTRL(unit).holRingSize_set(unit, ring, val);
}

int32 _nic_holRingCnt_set(uint32 unit, uint32 ring, uint32 val)
{
    return NIC_CTRL(unit).holRingCnt_set(unit, ring, val);
}

int32 _nic_ntfyBaseAddr_get(uint32 unit, uintptr *pAddr)
{
    return NIC_CTRL(unit).ntfyBaseAddr_get(unit, pAddr);
}

int32 _nic_ntfyBaseAddr_set(uint32 unit, uintptr addr)
{
    return NIC_CTRL(unit).ntfyBaseAddr_set(unit, addr);
}

int32 _nic_ringBaseAddr_get(uint32 unit, nic_dir_t dir, uint32 ring, uintptr *pAddr)
{
    return NIC_CTRL(unit).ringBaseAddr_get(unit, dir, ring, pAddr);
}

int32 _nic_ringBaseAddr_set(uint32 unit, nic_dir_t dir, uint32 ring, uintptr addr)
{
    return NIC_CTRL(unit).ringBaseAddr_set(unit, dir, ring, addr);
}

int32 _nic_ringCurAddr_get(uint32 unit, nic_dir_t dir, uint32 ring, uintptr *pAddr)
{
    return NIC_CTRL(unit).ringCurAddr_get(unit, dir, ring, pAddr);
}

int32 _nic_cpuPtr_get(uint32 unit, nic_dir_t dir, uint32 ring, uint32 *pVal)
{
    return NIC_CTRL(unit).cpuPtr_get(unit, dir, ring, pVal);
}

int32 _nic_cpuPtr_set(uint32 unit, nic_dir_t dir, uint32 ring, uint32 val)
{
    return NIC_CTRL(unit).cpuPtr_set(unit, dir, ring, val);
}

int32 _nic_hwPtr_get(uint32 unit, nic_dir_t dir, uint32 ring, uint32 *pVal)
{
    return NIC_CTRL(unit).hwPtr_get(unit, dir, ring, pVal);
}

int32 _nic_hwPtr_set(uint32 unit, nic_dir_t dir, uint32 ring, uint32 val)
{
    return NIC_CTRL(unit).hwPtr_set(unit, dir, ring, val);
}

int32 _nic_rxTruncateEnable_get(uint32 unit, uint32 *pVal)
{
    return NIC_CTRL(unit).rxTruncateEnable_get(unit, pVal);
}

int32 _nic_rxTruncateEnable_set(uint32 unit, uint32 val)
{
    return NIC_CTRL(unit).rxTruncateEnable_set(unit, val);
}

int32 _nic_rxTruncateLength_get(uint32 unit, uint32 *pVal)
{
    return NIC_CTRL(unit).rxTruncateLength_get(unit, pVal);
}

int32 _nic_rxTruncateLength_set(uint32 unit, uint32 val)
{
    return NIC_CTRL(unit).rxTruncateLength_set(unit, val);
}

int32 _nic_nicEnable_get(uint32 unit, nic_dir_t dir, uint32 *pVal)
{
    return NIC_CTRL(unit).nicEnable_get(unit, dir, pVal);
}

int32 _nic_nicEnable_set(uint32 unit, nic_dir_t dir, uint32 val)
{
    return NIC_CTRL(unit).nicEnable_set(unit, dir, val);
}

int32 _nic_nicTxFetch_set(uint32 unit, nic_txRing_t type, uint32 val)
{
    return NIC_CTRL(unit).nicTxFetch_set(unit, type, val);
}

int32 _nic_nicTxBusySts_get(uint32 unit, nic_txRing_t type, uint32 *pVal)
{
    return NIC_CTRL(unit).nicTxBusySts_get(unit, type, pVal);
}

int32 _nic_cpuTagId_get(uint32 unit, uint32 *pVal)
{
    return NIC_CTRL(unit).cpuTagId_get(unit, pVal);
}

int32 _nic_cpuTagId_set(uint32 unit, uint32 val)
{
    return NIC_CTRL(unit).cpuTagId_set(unit, val);
}

int32 _nic_supportJumboSize_get(uint32 unit, uint32 *pVal)
{
    return NIC_CTRL(unit).supportJumboSize_get(unit, pVal);
}

int32 _nic_cpuTagFromRaw_cnvt(uint32 unit, uint8 *pCpuTag, drv_nic_pkt_t *pPacket)
{
    return NIC_CTRL(unit).cpuTagFromRaw_cnvt(unit, pCpuTag, pPacket);
}

int32 _nic_cpuTagToRaw_cnvt(uint32 unit, drv_nic_pkt_t *pPacket, uint8 *pCpuTag)
{
    return NIC_CTRL(unit).cpuTagToRaw_cnvt(unit, pPacket, pCpuTag);
}

int32 _nic_cpuTag_dump(uint32 unit, drv_nic_pkt_t *pPacket)
{
    return NIC_CTRL(unit).cpuTag_dump(unit, pPacket);
}

int32 _nic_rawTag_dump(uint32 unit, uint8 *pCpuTag)
{
    return NIC_CTRL(unit).rawTag_dump(unit, pCpuTag);
}

int32 _nic_cpuTagLen_get(uint32 unit, uint32* rxTag_len, uint32* txTag_len)
{
    return NIC_CTRL(unit).cpuTagLen_get(unit, rxTag_len, txTag_len);
}

int32   _nic_sramEntry_get(uint32 unit, nic_dbg_opt_t* pOpt, nic_dbg_sram_entry_t* pEntry)
{
    return NIC_CTRL(unit).sramEntry_get(unit, pOpt, pEntry);
}


int32   _nic_desc_dump(uint32 unit, nic_dir_t dir, uint32 ringId, uint32 index)
{
    int i;
    int dump_len = sizeof(nic_desc_t); /* debug dump maximum length */
    nic_desc_t* desc = (NIC_DIR_RX == dir) ? pNic_rxRBase[unit][ringId] + index : pNic_txRBase[unit][ringId] + index;

    osal_printf("[DESC%d_%02d:%px] ", ringId, index, desc);
    for (i = 0; i < dump_len; i++)
    {
        osal_printf("%02X", *((uint8*)desc + i));
        if ((i % 4) == 3)
            osal_printf(" ");
    }

    if (NIC_DIR_RX == dir)
    {
        if (pRx_pkt_privAry[unit][ringId][index].packet)
            osal_printf("   [pktbuf:%px]", pRx_pkt_privAry[unit][ringId][index].packet->data);
        else
            osal_printf("   [pktbuf_addr: null]");
    }
    else
    {
        if (pTx_pkt_privAry[unit][ringId][index].packet)
            osal_printf("   [pktbuf:%px]", pTx_pkt_privAry[unit][ringId][index].packet->data);
        else
            osal_printf("   [pktbuf_addr: null]");
    }
    osal_printf("\n");
    return 0;
}

int32 _nic_pkt_dump(uint32 unit, drv_nic_pkt_t *pPacket, nic_dir_t dir)
{
    int i;
    int dump_len = DEBUG_DUMP_PKT_LEN; /* debug dump maximum length */

    if (dir == NIC_DIR_TX)
        osal_printf("=== [NIC TX Debug] ================================= address %px, Len: %d \n", pPacket->data, pPacket->length);
    else
        osal_printf("=== [NIC RX Debug] ================================= address %px, Len: %d \n", pPacket->data, pPacket->length);

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

void _makeCrcTable(void)
{
    static uint32   init_complete = 0;
    uint32 c;
    uint16 n, k;

    if (init_complete)
        return;
    for (n = 0; n < CRC_TBL_SIZE; n++)
    {
        c = n;
        for (k = 8; k > 0; k--)
        {
            if ((c & 1) != 0)
                c = CRCPOLY ^ (c >> 1);
            else
                c = c >> 1;
        }
        crcTable[n] = c;
    }
    init_complete = 1;
}

uint32 _chksum_crc32 (unsigned char *block, unsigned int length)
{
   uint32 crc;
   unsigned long i;

   crc = 0xFFFFFFFF;
   for (i = 0; i < length; i++)
   {
      crc = ((crc >> 8) & 0x00FFFFFF) ^ crcTable[(crc ^ *block) & 0xFF];
      block++;
   }
   return (crc ^ 0xFFFFFFFF);
}

int _rxChkCRC(uint32 unit, drv_nic_pkt_t *pPacket)
{
    int32   ret = 0;
    uint32  crcVal, len;

    len = rxCRCInclude[unit] ? pPacket->length : pPacket->length + 4;
    crcVal = _chksum_crc32(pPacket->data, len - 4);
    if ((pPacket->data[len - 1] != (crcVal >> 24) || pPacket->data[len - 2] != ((crcVal >> 16) & 0xff) ||
	            pPacket->data[len - 3] != ((crcVal >> 8) & 0xff) || pPacket->data[len - 4] != (crcVal & 0xff)))
    {
        osal_printf("%s():%d CRC error  len:%d  crcVal:%#x\n", __FUNCTION__, __LINE__, len, crcVal);
        _nic_pkt_dump(0, pPacket, NIC_DIR_RX);
        nic_rx_crc_cntr[unit]++;
        ret = RT_ERR_FAILED;
    }

    return ret;
}

#if defined(CONFIG_SDK_RTL9311B)

static int32    _err_flag_chk(uint8* ptr)
{
    int32   i = 0;
    for (i = 0; i < 15; i++)
    {
        if (*(ptr+i))
        {
            return RT_ERR_FAILED;
        }
    }
    return RT_ERR_OK;
}

static int32 _raw_data_dump(uint8* ptr, uint32 len)
{
    uint32  i;

    osal_printf("=== [NIC Debug SHOW] =======address = %px, length = %d\n", ptr, len);
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
#endif

/*
 * Function Declaration
 */
int32
_nic_isr_rxRoutine(uint32 unit, uint32 ringId)
{
    uint32  i;
    //int32   ret = RT_ERR_FAILED;
    //uint32  releaseCnt = 0;
    drv_nic_rx_t nic_rx_handle = NIC_RX_NOT_HANDLED;
    nic_desc_t  *desc;
    uint32  rx_hw_ptr = 0, rx_hw_flag = 0, alloc_cnt = 0;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_flags;
#endif


    RT_LOG(LOG_DEBUG, MOD_NIC, "ringId = %d", ringId);

    NIC_RX_LOCK(unit, &spl_flags);

    if (ringId >= rxRingNum[unit])
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "invalid ringId(%d)!", ringId);
        nic_rx_failed_cntr[unit]++;
        NIC_RX_UNLOCK(unit, &spl_flags);
        return RT_ERR_FAILED;
    }

    /* Get HW current pointer */
    _nic_hwPtr_get(unit, NIC_DIR_RX, ringId, &rx_hw_ptr);
    rx_hw_flag = rx_hw_ptr >> HW_PTR_FLAG_OFFSET;
    rx_hw_ptr  &=  ~(1 << HW_PTR_FLAG_OFFSET);

    if (rx_hw_ptr >= rxRingIdSize[unit][ringId])
    {
        osal_printf("%s() %d: unit %d ring %d rx_hw_ptr out of range, rx_hw_ptr = %x\n",
            __FUNCTION__, __LINE__, unit, ringId, rx_hw_flag << HW_PTR_FLAG_OFFSET | rx_hw_ptr);
        NIC_RX_UNLOCK(unit, &spl_flags);
        return RT_ERR_FAILED;
    }

    do
    {
        uint8           handled = FALSE;
        uint8           reclaim_mbuf = TRUE;
		uint32	        pkt_size = 0, more = 0;
        drv_nic_pkt_t   *cur_pkt = NULL;
        uint32  err_flag = 0;

        if ((cur_rx[unit][ringId] == rx_hw_ptr) && (cur_rx_flag[unit][ringId] == rx_hw_flag))
        {
            if (alloc_cnt != 0)
            {
                _nic_cpuPtr_set(unit, NIC_DIR_RX, ringId, (rx_cpu_flag[unit][ringId] << CPU_PTR_FLAG_OFFSET) | rx_cpu_ptr[unit][ringId]);
                alloc_cnt = 0;
            }

            break;
        }


        if (NULL == (cur_pkt = pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet))
            break;


        desc        = pNic_rxRBase[unit][ringId] + cur_rx[unit][ringId];
#if defined(__BOOTLOADER__)
        dma_unmap_single((uintptr)desc, sizeof(nic_desc_t), DMA_FROM_DEVICE);
#endif
		pkt_size    = (BE32_to_CPU(desc->info) >> 2) & 0xffff;
        more        = (BE32_to_CPU(desc->info) >> PKT_MORE_FLAG_OFFSET) & 0x1;
        err_flag    = (BE32_to_CPU(desc->meta) >> 1) & 0x1;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
        dma_unmap_single(_nic_init_conf[unit].dev, DESC_ADDR_2_PHY(desc->addr) + mem_base_addr[unit], _nic_init_conf[unit].pkt_size, DMA_FROM_DEVICE);
#elif defined(__BOOTLOADER__)
        dma_unmap_single(DESC_ADDR_2_PHY(desc->addr) + mem_base_addr[unit], _nic_init_conf[unit].pkt_size, DMA_FROM_DEVICE);
#endif
		if (more && jumboFlag[unit][ringId] == FALSE)     /* Jumbo head */
		{
            jumboFlag[unit][ringId] = TRUE;
            if (RT_ERR_OK == _nic_init_conf[unit].rx_pkt_alloc(unit, gJumboSize[unit], 0, &pPacket[unit][ringId]))
            {
                jumboBuffAlloc[unit][ringId] = TRUE;
#if defined(CONFIG_SDK_RTL9311B)
                if (err_flag && (CHIP_REV_ID_A == RTK_CHIP_REV(unit)) && (RTK_9311B_FAMILY_ID(unit)))
                {
                    if (RT_ERR_OK != _err_flag_chk(cur_pkt->data + 15))
                    {
                        _raw_data_dump(cur_pkt->data, pkt_size);

                        osal_memcpy(pPacket[unit][ringId]->data, cur_pkt->data, pkt_size);
                        pPacket[unit][ringId]->length = pkt_size;

                        nic_rx_unExp_err_cnt[unit]++;

                    }
                    else
                    {
                        osal_memcpy(pPacket[unit][ringId]->data, cur_pkt->data, 15);
                        osal_memcpy(pPacket[unit][ringId]->data + 15, cur_pkt->data + 30, pkt_size - 30);
                        pPacket[unit][ringId]->length = (pkt_size - 15);
                        nic_rx_err_cnt[unit]++;
                    }
                }
                else
#endif
#if defined(CONFIG_SDK_RTL9330)
                if (err_flag && (RTK_9330_FAMILY_ID(unit)))
                {
                    // TODO:
                    //abandon this packet
                }
                else
#endif
                {
                    osal_memcpy(pPacket[unit][ringId]->data, cur_pkt->data, pkt_size);
                    pPacket[unit][ringId]->length = pkt_size;
                }
            }
            else
            {
                RT_LOG(LOG_DEBUG, MOD_NIC, "Out of memory ! (alloc a new packet data buffer failed)");
                pPacket[unit][ringId] = NULL;
                nic_rx_failed_cntr[unit]++;
            }
            _nic_init_conf[unit].pkt_free(unit, cur_pkt);
            pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet = NULL;
		}
        else if (jumboFlag[unit][ringId] == TRUE)     /* Jumbo other */
        {
            if (jumboBuffAlloc[unit][ringId])
            {
                osal_memcpy(pPacket[unit][ringId]->data + pPacket[unit][ringId]->length, cur_pkt->data, pkt_size);
                pPacket[unit][ringId]->length += pkt_size;
                pPacket[unit][ringId]->tail   = pPacket[unit][ringId]->data + pPacket[unit][ringId]->length;
                if (unlikely(pPacket[unit][ringId]->tail > pPacket[unit][ringId]->end))
                    osal_printf("%s():%d  !!!!  head:%px  data:%px  tail:%px  end:%px\n", __FUNCTION__, __LINE__, pPacket[unit][ringId]->head, pPacket[unit][ringId]->data, pPacket[unit][ringId]->tail, pPacket[unit][ringId]->end);
            }
            _nic_init_conf[unit].pkt_free(unit, cur_pkt);
            pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet = NULL;
        }
        else        /* Normal packet */
        {
#if defined(CONFIG_SDK_RTL9311B)
            if (err_flag && (CHIP_REV_ID_A == RTK_CHIP_REV(unit)) && (RTK_9311B_FAMILY_ID(unit)))
            {
                if (RT_ERR_OK != _err_flag_chk(cur_pkt->data + 15))
                {
                    _raw_data_dump(cur_pkt->data, pkt_size);
                    nic_rx_unExp_err_cnt[unit]++;
                }
                else
                {
                    osal_memcpy(cur_pkt->data + 15, cur_pkt->data + 30, pkt_size - 30);
                    pkt_size -= 15;
                    nic_rx_err_cnt[unit]++;
                }
            }
#endif
#if defined(CONFIG_SDK_RTL9330)
            if (err_flag && (RTK_9330_FAMILY_ID(unit)))
            {
                // TODO:
                //abandon this packet
            }
#endif
            pPacket[unit][ringId]         = cur_pkt;
            pPacket[unit][ringId]->length = pkt_size;
            pPacket[unit][ringId]->tail   = pPacket[unit][ringId]->data + pkt_size;
            if (unlikely(pPacket[unit][ringId]->tail > pPacket[unit][ringId]->end))
                osal_printf("%s():%d  !!!!  head:%px  data:%px  tail:%px  end:%px\n", __FUNCTION__, __LINE__, pPacket[unit][ringId]->head, pPacket[unit][ringId]->data, pPacket[unit][ringId]->tail, pPacket[unit][ringId]->end);
        }


        if ((0 == rxCRCInclude[unit]) && (0 == more) && pPacket[unit][ringId] != NULL)
        {
            pPacket[unit][ringId]->length -= 4;
            pPacket[unit][ringId]->tail   -= 4;
        }

        //CPU Tag process
        if ((0 == more) && (jumboFlag[unit][ringId] == 0 || (jumboFlag[unit][ringId] && jumboBuffAlloc[unit][ringId])))
        {
            /* NIC Rx debug message */
            if (nic_debug_flag[unit] & DEBUG_RX_RAW_LEN_BIT)
            {
                _nic_desc_dump(unit, NIC_DIR_RX, ringId, cur_rx[unit][ringId]);
                _nic_pkt_dump(unit, pPacket[unit][ringId], NIC_DIR_RX);
            }

            if (RTK_9330_FAMILY_ID(unit) || RTK_9311B_FAMILY_ID(unit))
            {
                if (BE32_to_CPU(desc->meta) & 0x1)
                {
                    _nic_cpuTagFromRaw_cnvt(unit, pPacket[unit][ringId]->data, pPacket[unit][ringId]);
                }
                else
                {
                    RT_LOG(LOG_FATAL_ERR, MOD_NIC, "Error - packet without CRX tag!");
                }
            }

            /* NIC Rx debug message */
            if (nic_debug_flag[unit] & DEBUG_RX_CPU_TAG_BIT)
            {
                _nic_cpuTag_dump(unit, pPacket[unit][ringId]);
            }

            if ((0 == more) && (jumboFlag[unit][ringId] == 0 || (jumboFlag[unit][ringId] && jumboBuffAlloc[unit][ringId])) && (nic_debug_flag[unit] & DEBUG_CPU_CALCRC_BIT))
                _rxChkCRC(unit, pPacket[unit][ringId]);
        }

        if (jumboFlag[unit][ringId] == 0 || (jumboFlag[unit][ringId] && (0 == more) && jumboBuffAlloc[unit][ringId]))
        {
            nic_rx_handle = NIC_RX_NOT_HANDLED;

            if (nic_debug_flag[unit] & DEBUG_CPU_LOOPBACK_BIT)
            {
                pPacket[unit][ringId]->as_txtag = 1;
#if defined(CONFIG_SDK_RTL9330)
                if (RTK_9330_FAMILY_ID(unit))
                {
                    pPacket[unit][ringId]->tx_tag.fwd_type = NIC_FWD_TYPE_DST_IF;
                    pPacket[unit][ringId]->tx_tag.dst_data = 0x101C;
                }
#endif
#if defined(CONFIG_SDK_RTL9311B)
                if (RTK_9311B_FAMILY_ID(unit))
                {
                    pPacket[unit][ringId]->tx_tag.fwd_type = NIC_FWD_TYPE_PHYISCAL;
                    pPacket[unit][ringId]->tx_tag.dev_id = pPacket[unit][ringId]->rx_tag.dev_id;
                    pPacket[unit][ringId]->tx_tag.priority = ringId % txRingNum[unit];
                    pPacket[unit][ringId]->tx_tag.dst_port_mask = 0x1 << pPacket[unit][ringId]->rx_tag.source_port;
                }
#endif
                pPacket[unit][ringId]->data += rx_cTag_len[unit];
                pPacket[unit][ringId]->length -= rx_cTag_len[unit];
                if (RT_ERR_OK != drv_nic_pkt_tx(unit, pPacket[unit][ringId], NULL, NULL))
                {
                    _nic_init_conf[unit].pkt_free(unit, pPacket[unit][ringId]);
                }
                if (jumboFlag[unit][ringId] == 0)
                    pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet = NULL;
            }
            else
            {
                if (0 != _nic_rx_intr_cb_cnt[unit])
                {
                    pPacket[unit][ringId]->data += rx_cTag_len[unit];
                    pPacket[unit][ringId]->length -= rx_cTag_len[unit];
                    for (i = 0; i < NIC_RX_CB_PRIORITY_NUMBER; i++)
                    {
                        if (_nic_rx_cb_tbl[unit][i].rx_callback != NULL)
                        {
                            nic_rx_handle = _nic_rx_cb_tbl[unit][i].rx_callback(unit, pPacket[unit][ringId], _nic_rx_cb_tbl[unit][i].pCookie);
                        }
                        switch (nic_rx_handle)
                        {
                            case NIC_RX_NOT_HANDLED:
                                break;
                            case NIC_RX_HANDLED:
                                handled = TRUE;
                                break;
                            case NIC_RX_HANDLED_OWNED:
                                handled = TRUE;
                                if (jumboFlag[unit][ringId] == 0)
                                    pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet = NULL;
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
#if 0
                    /* Process non-interrupt callback function */
                    schd_queId = pPacket[unit][ringId]->rx_tag.qid;
                    nic_rx_queueInfo_get(unit, schd_queId, &pRx_queue);
                    if ((pRx_queue->drop_thresh > 0) && (pRx_queue->count < pRx_queue->drop_thresh))
                    {
                        if ((ret = nic_rx_pkt_enqueue(schd_queId, pPacket[unit][ringId])) == RT_ERR_OK)
                        {
                            /* Notify RX thread */
                            nic_rx_thread_notify(unit);
                            nic_rx_handle = NIC_RX_HANDLED_OWNED;
                            if (jumboFlag[unit][ringId] == 0)
                                pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet = NULL;
                        }
                        else
                        {
                            //RT_LOG(LOG_WARNING, (MOD_NIC), "RX enqueue %d failed!", schd_queId);
                            stats.rx_lack_buf_cntr++;
                        }
                    }
                    else
                    {
                        //RT_LOG(LOG_WARNING, (MOD_NIC), "RX too fast, directly drop\n");
                        stats.rx_lack_buf_cntr++;
#if defined(CONFIG_SDK_CPU_RATE_PROCESS)
                        nic_rx_cpu_cfg_get(CPU_RATE_CFG_CAL_REFILL, &rate_en);
                        if(rate_en == TRUE)
                        {
                            nic_rx_cpu_cnt_get(&bkt_cnt);
                            if (bkt_cnt == 0)
                                nic_rx_thread_notify(unit);
                        }
#endif
                    }
#endif
                }

                if (NIC_RX_HANDLED_OWNED != nic_rx_handle)
                {
//osal_printf("%s: %d\n", __FUNCTION__, __LINE__);
                    _nic_init_conf[unit].pkt_free(unit, pPacket[unit][ringId]);
                    if (jumboFlag[unit][ringId] == 0)
                        pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet = NULL;
                }
            }
        }

        if (NULL == pRx_pkt_privAry[unit][ringId][cur_rx[unit][ringId]].packet)
        {
            drv_nic_pkt_t *ptr;

            /* Alloc a new packet data buffer */
            if (RT_ERR_OK == _nic_init_conf[unit].rx_pkt_alloc(unit, _nic_init_conf[unit].pkt_size, 0, &ptr))
            {
                pRx_pkt_privAry[unit][ringId][rx_cpu_ptr[unit][ringId]].packet = ptr;

                if(TRUE == reclaim_mbuf)
                {
                    uintptr mapping;
                    mapping     = (uintptr)DMA_ADDR_VIR2PHY(_nic_init_conf[unit].dev, pRx_pkt_privAry[unit][ringId][rx_cpu_ptr[unit][ringId]].packet->data, _nic_init_conf[unit].pkt_size, DMA_FROM_DEVICE) - mem_base_addr[unit];
                    desc->addr = PHY_2_DESC_ADDR(mapping);
                    desc->info  = CPU_to_BE32(INFO_MAKE_LEN(_nic_init_conf[unit].pkt_size));
#if defined(__BOOTLOADER__)
                    flush_dcache_range((uintptr)desc, (uintptr)desc + sizeof(nic_desc_t));
                    flush_dcache_range((uintptr)ptr->data, (uintptr)ptr->data + _nic_init_conf[unit].pkt_size);
#endif
                }
                alloc_cnt++;
                rx_cpu_ptr[unit][ringId]++;
                if (rx_cpu_ptr[unit][ringId] == rxRingIdSize[unit][ringId])
                {
                    rx_cpu_flag[unit][ringId] ^= 1;
                    rx_cpu_ptr[unit][ringId]  = 0;
                }

                if (alloc_cnt == rxRingIdSize[unit][ringId])
                {
                    _nic_cpuPtr_set(unit, NIC_DIR_RX, ringId, (rx_cpu_flag[unit][ringId] << CPU_PTR_FLAG_OFFSET) | rx_cpu_ptr[unit][ringId]);
                    alloc_cnt = 0;
                }
            }
            else
            {
                osal_printf("%s():%d  Out of memory ! (alloc a new packet data buffer failed)\n", __FUNCTION__, __LINE__);
                nic_rx_isr_alloc_fail_cnt[unit]++;
                //reclaim_mbuf = FALSE;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
                if (0 == osal_atomic_read(&alloc_thrd_run))
                {
                    osal_wake_up(alloc_fail_event);
                }
#endif
            }
        }

        if (jumboFlag[unit][ringId] == 0 || (jumboFlag[unit][ringId] && (more == 0) && jumboBuffAlloc[unit][ringId]))
        {
            nic_rx_success_cntr[unit]++;
        }

        if (jumboFlag[unit][ringId] && (more == 0))
        {
            jumboFlag[unit][ringId]       = FALSE;
            jumboBuffAlloc[unit][ringId]  = FALSE;
        }

        cur_rx[unit][ringId]++;
        if (cur_rx[unit][ringId] == rxRingIdSize[unit][ringId])
        {
            cur_rx_flag[unit][ringId] ^= 1;
            cur_rx[unit][ringId]      = 0;
        }
    } while (1);

    NIC_RX_UNLOCK(unit, &spl_flags);

    return RT_ERR_OK;
}

int32
_nic_isr_txRoutine(uint32 unit, uint32 ringId)
{
    uint32      tx_hw_ptr, tx_hw_flag;
    pkt_priv_t  *cur_pkt_priv;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
    nic_desc_t  *desc;
    uint32      len;
#endif
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_flags;
#endif

    if (ringId >= txRingNum[unit])
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "invalid ringId(%d)!", ringId);
        return RT_ERR_FAILED;
    }


    NIC_TX_LOCK(unit, &spl_flags);

    /* Get HW current pointer */
    _nic_hwPtr_get(unit, NIC_DIR_TX, ringId, &tx_hw_ptr);
    tx_hw_flag = tx_hw_ptr >> HW_PTR_FLAG_OFFSET;
    tx_hw_ptr  &=  0xffff;

    do
    {
        drv_nic_pkt_t   *cur_pkt;


        if ((cur_tx[unit][ringId] == tx_hw_ptr) && (cur_tx_flag[unit][ringId] == tx_hw_flag))
        {
            break;
        }


        cur_pkt_priv = pTx_pkt_privAry[unit][ringId] + cur_tx[unit][ringId];
        if (NULL == (cur_pkt = cur_pkt_priv->packet))
            break;


#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
        desc    = pNic_txRBase[unit][ringId] + cur_tx[unit][ringId];
        len     = cur_pkt->txIncludeCRC ? cur_pkt->length - 4 : cur_pkt->length;
  #if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
        dma_unmap_single(_nic_init_conf[unit].dev, DESC_ADDR_2_PHY(desc->addr) + mem_base_addr[unit], len, DMA_TO_DEVICE);
  #else
        dma_unmap_single(DESC_ADDR_2_PHY(desc->addr) + mem_base_addr[unit], len, DMA_TO_DEVICE);
  #endif
#endif
        if (cur_pkt_priv->tx_callback == NULL)
        {
            _nic_init_conf[unit].pkt_free(unit, cur_pkt);
        }
        else
        {
            cur_pkt_priv->tx_callback(unit, cur_pkt, cur_pkt_priv->cookie);
            cur_pkt_priv->tx_callback = NULL;
        }
        cur_pkt_priv->packet = NULL;


        cur_tx[unit][ringId]++;
        if (cur_tx[unit][ringId] == txRingIdSize[unit][ringId])
        {
            cur_tx_flag[unit][ringId] ^= 1;
            cur_tx[unit][ringId]      = 0;
        }
    } while (1);

    NIC_TX_UNLOCK(unit, &spl_flags);

    return RT_ERR_OK;
}

static int32
_nic_isr_mbRoutine(uint32 unit)
{
    int32           ret = RT_ERR_OK;
    int32           ringId;
    drv_nic_pkt_t   *pPacket;
    uintptr         mapping;
    nic_desc_t      *desc;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_flags;
#endif

    NIC_RX_LOCK(unit, &spl_flags);

    for (ringId = 0; ringId < rxRingNum[unit]; ringId++)
    {
        do
        {
            if ((rx_cpu_ptr[unit][ringId] == cur_rx[unit][ringId]) && (rx_cpu_flag[unit][ringId] != cur_rx_flag[unit][ringId]))
            {
                /* ring is full */
                break;
            }
            if (NULL == pRx_pkt_privAry[unit][ringId][rx_cpu_ptr[unit][ringId]].packet)
            {
                /* Alloc a new packet data buffer */
                if (RT_ERR_OK != _nic_init_conf[unit].rx_pkt_alloc(unit, _nic_init_conf[unit].pkt_size, 0, &pPacket))
                {
                    nic_rx_mb_alloc_fail_cnt[unit]++;
    //                osal_printf("%s():%d  Out of memory ! (alloc a new packet data buffer failed)\n", __FUNCTION__, __LINE__);
                    ret = RT_ERR_FAILED;
                    break;
                }

                pRx_pkt_privAry[unit][ringId][rx_cpu_ptr[unit][ringId]].packet = pPacket;
                mapping = (uintptr)DMA_ADDR_VIR2PHY(_nic_init_conf[unit].dev, pRx_pkt_privAry[unit][ringId][rx_cpu_ptr[unit][ringId]].packet->data, _nic_init_conf[unit].pkt_size, DMA_FROM_DEVICE) - mem_base_addr[unit];
                desc    = pNic_rxRBase[unit][ringId] + rx_cpu_ptr[unit][ringId];
                desc->addr = PHY_2_DESC_ADDR(mapping);
                desc->info = CPU_to_BE32(INFO_MAKE_LEN(_nic_init_conf[unit].pkt_size));
                if (0)//(0 == rx_cpu_ptr[unit][ringId])
                    osal_printf("%s():%d  [%d][%d] desc->addr:%#llx (%px)  desc->info:%#x\n", __FUNCTION__, __LINE__, ringId, rx_cpu_ptr[unit][ringId], desc->addr, pPacket->data, desc->info);
#if defined(__BOOTLOADER__)
                flush_dcache_range((uintptr)desc, (uintptr)desc + sizeof(nic_desc_t));
                flush_dcache_range((uintptr)pPacket->data, (uintptr)pPacket->data + _nic_init_conf[unit].pkt_size);
#endif
            }
            rx_cpu_ptr[unit][ringId]++;
            if (rx_cpu_ptr[unit][ringId] == rxRingIdSize[unit][ringId])
            {
                rx_cpu_flag[unit][ringId] ^= 1;
                rx_cpu_ptr[unit][ringId]  = 0;
            }

        } while (1);
        _nic_cpuPtr_set(unit, NIC_DIR_RX, ringId, (rx_cpu_flag[unit][ringId] << CPU_PTR_FLAG_OFFSET) | rx_cpu_ptr[unit][ringId]);
    }

    NIC_RX_UNLOCK(unit, &spl_flags);

    return ret;
}

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
static void _nic_alloc_fail_handle_thread(void *pInput)
{
    int32   ret = RT_ERR_OK;
    uint32  unit = 0;
    while (1)
    {
        osal_wait_event(alloc_fail_event);
        osal_atomic_set(&alloc_thrd_run, 1);
        nic_alloc_monThr_acc_cnt++;
//        osal_printf("%s():%d  nic_alloc_monThr_acc_cnt = %d!\n", __FUNCTION__, __LINE__, nic_alloc_monThr_acc_cnt);
        ret = RT_ERR_OK;

        HWP_UNIT_TRAVS_LOCAL(unit)
        {
            if (RT_ERR_OK != _nic_isr_mbRoutine(unit))
            {
                ret = RT_ERR_FAILED;
            }
        }
        if (RT_ERR_OK == ret)
        {
            HWP_UNIT_TRAVS_LOCAL(unit)
            {
                _nic_intrSts_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL);
                _nic_intrMask_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL);
            }
            osal_atomic_set(&alloc_thrd_run, 0);
        }
        else
        {
            osal_time_usleep(200000);
            osal_wake_up(alloc_fail_event);
        }
    }

    osal_thread_exit(0);

}
#endif

/* Function Name:
 *      drv_nic_isr_handler
 * Description:
 *      NIC event handler.
 * Input:
 *      isr_param   - argument passed at interrupt time
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B ,9330
 * Note:
 *      Must initialize nic module before calling any nic APIs.
 * Changes:
 *      None
 */
osal_isrret_t
drv_nic_isr_handler(void *isr_param)
{
    int32  i;
    uint32  cpu_iisr;
    uint32  unit    = ((isr_param_t *)isr_param)->unit;
    uint32  extNic  = ((isr_param_t *)isr_param)->ext_nic;
    uint32  eventId = ((isr_param_t *)isr_param)->eventId;
    uint32  ringId;
    uint32  intrMask;
    uint32  rx_done_imr = 0;
    uint32  tx_done_imr = 0;
    uint32  rx_runout_imr = 0;
    uint32  tx_done_0_2 = 0;
    int32   ret = RT_ERR_OK;

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    NIC_LOCK(unit);

    if (extNic == FALSE)
    {
//        osal_printf("%s():%d  unit:%d  eventId:%d\n", __FUNCTION__, __LINE__, unit, eventId);
        if (eventId >= NIC_RX_DONE_0 && eventId <= NIC_RX_DONE_11)
        {
            ringId = eventId - NIC_RX_DONE_0;
            /*disable IMR*/
            _nic_intrMask_get(unit, NIC_RX_DONE, &rx_done_imr);
            rx_done_imr &= ~(0x1 << ringId);
            _nic_intrMask_set(unit, NIC_RX_DONE, rx_done_imr);
            /* clear pending flag and handle*/
            _nic_intrSts_set(unit, NIC_RX_DONE, 0x1 << ringId);
            _nic_isr_rxRoutine(unit, ringId);
            /*restore IMR*/
            rx_done_imr |= (0x1 << ringId);
            _nic_intrMask_set(unit, NIC_RX_DONE, rx_done_imr);
        }
        else if (eventId >= NIC_RX_RUNOUT_0 && eventId <= NIC_RX_RUNOUT_11)
        {
            _nic_intrMask_get(unit, NIC_RX_RUNOUT, &rx_runout_imr);
            _nic_intrMask_set(unit, NIC_RX_RUNOUT, 0);

            rx_runout_cnt[unit]++;
            ret = _nic_isr_mbRoutine(unit);
            _nic_intrSts_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL);
            if (RT_ERR_OK == ret)
            {

                _nic_intrMask_set(unit, NIC_RX_RUNOUT, rx_runout_imr);
            }
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
            else
            {
                if (0 == osal_atomic_read(&alloc_thrd_run))
                {
                    osal_wake_up(alloc_fail_event);
                }
            }
#endif
        }
        else if (eventId >= NIC_TX_DONE_0 && eventId <= NIC_TX_DONE_3)
        {
            ringId = eventId - NIC_TX_DONE_0;
            //osal_printf("%s():%d  ringId:%d\n", __FUNCTION__, __LINE__, ringId);
            /*disable IMR*/
            _nic_intrMask_get(unit, NIC_TX_DONE, &tx_done_imr);
            tx_done_imr &= ~(0x1 << ringId);
            _nic_intrMask_set(unit, NIC_TX_DONE, tx_done_imr);
            /*clear pending flag and handle*/
            _nic_intrSts_set(unit, NIC_TX_DONE, 0x1 << ringId);
            _nic_isr_txRoutine(unit, ringId);

            /*restore IMR*/
            tx_done_imr |= (0x1 << ringId);
            _nic_intrMask_set(unit, NIC_TX_DONE, tx_done_imr);
        }
        else if (NIC_TX_DONE_0_2 == eventId)
        {
            _nic_intrSts_get(unit, NIC_TX_DONE, &cpu_iisr);
            _nic_intrMask_get(unit, NIC_TX_DONE, &tx_done_0_2);
            intrMask = tx_done_0_2 & 0x8;
            _nic_intrMask_set(unit, NIC_TX_DONE, intrMask);
            cpu_iisr &= tx_done_0_2;
            if (cpu_iisr & 0x7)
            {
                for (i = txRingNum[unit] - 2; i >= 0; i--)
                {
                    if (cpu_iisr & (0x1 << i))
                    {
                        _nic_intrSts_set(unit, NIC_TX_DONE, 0x1 << i);
                        _nic_isr_txRoutine(unit, i);
                    }
                }
            }
            _nic_intrMask_set(unit, NIC_TX_DONE, tx_done_0_2);
        }
#if defined(CONFIG_SDK_DRIVER_L2NTFY)
        else if (NIC_L2NTFY_RX_DONE == eventId)
        {
            _nic_intrMask_set(unit, NIC_NTFY_DONE, 0);
            _nic_intrSts_set(unit, NIC_NTFY_DONE, 1);
            drv_l2ntfy_isr_handler(unit, isr_param);
            if (((isr_param_t *)isr_param)->ntfyIntrEnableCB)
            {
                _nic_intrMask_set(unit, NIC_NTFY_DONE, 1);
            }
        }
        else if (NIC_L2NTFY_BUF_RX_RUNOUT == eventId)
        {
            _nic_intrMask_get(unit, NIC_NTFY_BUF_RUNOUT, &intrMask);
            _nic_intrSts_get(unit, NIC_NTFY_BUF_RUNOUT, &cpu_iisr);
            if (cpu_iisr & intrMask)
            {
                _nic_intrMask_set(unit, NIC_NTFY_BUF_RUNOUT, 0);
                _nic_intrSts_set(unit, NIC_NTFY_BUF_RUNOUT, cpu_iisr);
                drv_l2ntfy_bufRunout_handler(unit, isr_param);
            }
            _nic_intrMask_get(unit, NIC_NTFY_LOCALBUF_RUNOUT, &intrMask);
            _nic_intrSts_get(unit, NIC_NTFY_LOCALBUF_RUNOUT, &cpu_iisr);
            if (cpu_iisr & intrMask)
            {
                _nic_intrMask_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, 0);
                _nic_intrSts_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, cpu_iisr);
                drv_l2ntfy_localBufRunout_handler(unit, isr_param);
            }
        }
        else if (NIC_L2NTFY_BUF_RUNOUT == eventId)
        {
            /*handler will enable IMR*/
            _nic_intrMask_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, 0);
            _nic_intrSts_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, 1);
            drv_l2ntfy_localBufRunout_handler(unit, isr_param);
        }
        else if (NIC_L2NTFY_RX_RUNOUT == eventId)
        {
            /*handler will enable IMR*/
            _nic_intrMask_set(unit, NIC_NTFY_BUF_RUNOUT, 0);
            _nic_intrSts_set(unit, NIC_NTFY_BUF_RUNOUT, 1);
            drv_l2ntfy_bufRunout_handler(unit, isr_param);
        }
#endif
    }
    else
    {
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
        _nic_intrMask_get(unit, NIC_RX_DONE, &rx_done_imr);
        _nic_intrMask_get(unit, NIC_TX_DONE, &tx_done_imr);
        _nic_intrMask_get(unit, NIC_RX_RUNOUT, &rx_runout_imr);

        _nic_intrSts_set(unit, NIC_RX_DONE, 0);
        _nic_intrSts_set(unit, NIC_TX_DONE, 0);
        _nic_intrSts_set(unit, NIC_RX_RUNOUT, 0);
#else
        rx_done_imr = NIC_RX_RING_ALL;
        rx_runout_imr = NIC_RX_RING_ALL;
        tx_done_imr = NIC_TX_RING_ALL;
#endif
        /* Handle Rx */
        _nic_intrSts_get(unit, NIC_RX_DONE, &cpu_iisr);
        cpu_iisr &= rx_done_imr;
        if (cpu_iisr)
        {
            for (i = rxRingNum[unit] - 1; i >= 0; i--)
            {
                if (cpu_iisr & (0x1 << i))
                {
                    _nic_intrSts_set(unit, NIC_RX_DONE, 0x1 << i);
                    _nic_isr_rxRoutine(unit, i);
                }
            }
        }

        /* Handle Tx */
        _nic_intrSts_get(unit, NIC_TX_DONE, &cpu_iisr);
        cpu_iisr &= tx_done_imr;
        if (cpu_iisr)
        {
            for (i = txRingNum[unit] - 1; i >= 0; i--)
            {
                if (cpu_iisr & (0x1 << i))
                {
                    _nic_intrSts_set(unit, NIC_TX_DONE, 0x1 << i);
                    _nic_isr_txRoutine(unit, i);
                }
            }
        }

        /* mBuffer Runout */
        _nic_intrSts_get(unit, NIC_RX_RUNOUT, &cpu_iisr);
        cpu_iisr &= rx_runout_imr;
        if (cpu_iisr)
        {
            rx_runout_cnt[unit]++;
            if (RT_ERR_OK == _nic_isr_mbRoutine(unit))
            {
                _nic_intrSts_set(unit, NIC_RX_RUNOUT, cpu_iisr);
            }
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
            else
            {
                if (0 == osal_atomic_read(&alloc_thrd_run))
                {
                    osal_wake_up(alloc_fail_event);
                }
            }
#endif
        }
        _nic_intrMask_set(unit, NIC_RX_DONE, rx_done_imr);
        _nic_intrMask_set(unit, NIC_RX_RUNOUT, rx_runout_imr);
        _nic_intrMask_set(unit, NIC_TX_DONE, tx_done_imr);

#if !defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && defined(CONFIG_SDK_DRIVER_L2NTFY)
        /* L2 Notification handler,  only for 9311B */
        _nic_intrMask_get(unit, NIC_NTFY_DONE, &intrMask);
        _nic_intrSts_get(unit, NIC_NTFY_DONE, &cpu_iisr);
        if (cpu_iisr & intrMask)
        {
            _nic_intrMask_set(unit, NIC_NTFY_DONE, 0);
            _nic_intrSts_set(unit, NIC_NTFY_DONE, cpu_iisr);
            drv_l2ntfy_isr_handler(unit, isr_param);
            if (((isr_param_t *)isr_param)->ntfyIntrEnableCB)
            {
                _nic_intrMask_set(unit, NIC_NTFY_DONE, intrMask);
            }
        }
        _nic_intrMask_get(unit, NIC_NTFY_BUF_RUNOUT, &intrMask);
        _nic_intrSts_get(unit, NIC_NTFY_BUF_RUNOUT, &cpu_iisr);
        if (cpu_iisr & intrMask)
        {
            _nic_intrMask_set(unit, NIC_NTFY_BUF_RUNOUT, 0);
            _nic_intrSts_set(unit, NIC_NTFY_BUF_RUNOUT, cpu_iisr);
            drv_l2ntfy_bufRunout_handler(unit, isr_param);
        }
        _nic_intrMask_get(unit, NIC_NTFY_LOCALBUF_RUNOUT, &intrMask);
        _nic_intrSts_get(unit, NIC_NTFY_LOCALBUF_RUNOUT, &cpu_iisr);
        if (cpu_iisr & intrMask)
        {
            _nic_intrMask_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, 0);
            _nic_intrSts_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, cpu_iisr);
            drv_l2ntfy_localBufRunout_handler(unit, isr_param);
        }
#endif

    }

    NIC_UNLOCK(unit);

    return OSAL_INT_HANDLED;
}

int32
_nic_pkt_tx(uint32 unit, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie)
{
    uint32      ringId, tx_hw_ptr, tx_hw_flag, len;
    uintptr     mapping;
    nic_desc_t  *tx_desc;
    pkt_priv_t  *cur_pkt_priv;
    uint32      tx_entry_num = 1;
    drv_nic_pkt_t*   pPkt_txTag = NULL;
    uint32      info_more = 0, meta_tagSts = 0;
    uint32      cpu_ptr_pre, cpu_flag_pre;

    if (NULL == pPacket)
    {
        osal_printf("%s():%d  Error - pPacket is NULL!\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }
    if (pPacket->length == 0)
    {
        osal_printf("%s():%d  Error - pPacket length is 0!\n", __FUNCTION__, __LINE__);
        return RT_ERR_FAILED;
    }

    /* Get HW current pointer */
    ringId          = (pPacket->tx_tag.priority > 3) ? 3 : pPacket->tx_tag.priority;
    _nic_hwPtr_get(unit, NIC_DIR_TX, ringId, &tx_hw_ptr);
    tx_hw_flag      = tx_hw_ptr >> HW_PTR_FLAG_OFFSET;
    tx_hw_ptr       &=  ~(1 << HW_PTR_FLAG_OFFSET);

    cpu_ptr_pre = tx_cpu_ptr[unit][ringId];
    cpu_flag_pre = tx_cpu_flag[unit][ringId];
    if (pPacket->as_txtag)
    {
        /*prepare CPU tx tag packet*/
        if (likely(RT_ERR_OK == _nic_init_conf[unit].pkt_alloc(unit, tx_cTag_len[unit], 0, &pPkt_txTag)))
        {
            if (likely(RT_ERR_OK == _nic_cpuTagToRaw_cnvt(unit, pPacket, pPkt_txTag->data)))
            {
                tx_entry_num = 2;
                pPkt_txTag->length = tx_cTag_len[unit];

                if (nic_debug_flag[unit] & DEBUG_TX_RAW_LEN_BIT)
                    _nic_rawTag_dump(unit, pPkt_txTag->data);
            }
            else
            {
                _nic_init_conf[unit].pkt_free(unit, pPkt_txTag);
            }
        }
        else
        {
            osal_printf("%s():%d  Out of memory ! (alloc a new packet data buffer failed)\n", __FUNCTION__, __LINE__);
        }
    }

    do
    {

        if ((tx_cpu_ptr[unit][ringId] == tx_hw_ptr) && (tx_cpu_flag[unit][ringId] != tx_hw_flag))
        {
            //TX ring full
            osal_printf("%s():%d unit %d TX ring %d full!\n", __FUNCTION__, __LINE__, unit, ringId);
            goto err_free;
        }

        tx_desc         = pNic_txRBase[unit][ringId] + tx_cpu_ptr[unit][ringId];
        cur_pkt_priv    = pTx_pkt_privAry[unit][ringId] + tx_cpu_ptr[unit][ringId];
        if (NULL != cur_pkt_priv->packet)
        {
            osal_printf("%s():%d  TX[%d][%d][%d] pkt_buf is not recycled yet\n", __FUNCTION__, __LINE__, unit, ringId, tx_cpu_ptr[unit][ringId]);
            tx_cpu_ptr[unit][ringId] = cpu_ptr_pre;
            tx_cpu_flag[unit][ringId] = cpu_flag_pre;
            goto err_free;
        }

        if (2 == tx_entry_num)
        {
            cur_pkt_priv->packet = pPkt_txTag;
            cur_pkt_priv->tx_callback   = NULL;    /* CPU TX CTAG packet should be handled by default function */
            cur_pkt_priv->cookie        = NULL;
            info_more = meta_tagSts = 1;
        }
        else
        {
            cur_pkt_priv->packet = pPacket;
            cur_pkt_priv->tx_callback   = fTxCb;    /* Tx Callback function */
            cur_pkt_priv->cookie        = pCookie;
            info_more = meta_tagSts = 0;
        }

        len = cur_pkt_priv->packet->length;
        if ((0 == pPacket->txIncludeCRC) && (0 == info_more))
        {
           len += 4;
        }

        mapping = (uintptr)DMA_ADDR_VIR2PHY(_nic_init_conf[unit].dev, cur_pkt_priv->packet->data, len, DMA_TO_DEVICE) - mem_base_addr[unit];

        tx_desc->info   = CPU_to_BE32(INFO_MAKE_LEN(len) | INFO_MAKE_MORE(info_more));
        tx_desc->addr = PHY_2_DESC_ADDR(mapping);
        //osal_printf("%s():%d  len:%d  info:%#x  addr:%llx\n", __FUNCTION__, __LINE__, len, tx_desc->info, tx_desc->addr);
        tx_desc->meta   = CPU_to_BE32(meta_tagSts);
        MEMORY_BARRIER();
#if defined(__BOOTLOADER__)
        flush_dcache_range((uintptr)tx_desc, (uintptr)tx_desc + sizeof(nic_desc_t));
        flush_dcache_range((uintptr)cur_pkt_priv->packet->data, (uintptr)cur_pkt_priv->packet->data + len);
#endif
        if ((nic_debug_flag[unit] & DEBUG_TX_RAW_LEN_BIT))
            _nic_pkt_dump(unit, cur_pkt_priv->packet, NIC_DIR_TX);
        if ((nic_debug_flag[unit] & DEBUG_TX_RAW_LEN_BIT))
            _nic_desc_dump(unit, NIC_DIR_TX, ringId, tx_cpu_ptr[unit][ringId]);

        tx_cpu_ptr[unit][ringId]++;
        if (tx_cpu_ptr[unit][ringId] == txRingIdSize[unit][ringId])
        {
            tx_cpu_flag[unit][ringId] ^= 1;
            tx_cpu_ptr[unit][ringId]  = 0;
        }

        tx_entry_num--;
    } while (tx_entry_num > 0);


    _nic_cpuPtr_set(unit, NIC_DIR_TX, ringId, (tx_cpu_flag[unit][ringId] << CPU_PTR_FLAG_OFFSET) | tx_cpu_ptr[unit][ringId] | (0x1 << TX_FETCH_FLAG_OFFSET));

    return RT_ERR_OK;

err_free:
    if (NULL != pPkt_txTag)
    {
        _nic_init_conf[unit].pkt_free(unit, pPkt_txTag);
    }
    return RT_ERR_FAILED;
}

#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
static void _nic_rx_thread(void *pInput)
{
    uint32  i, curNic_rx_pkt_cnt, ring, jumbo;
    uint32  unit;
    nic_collectArrayList_t  *pEntry, *n;
    drv_nic_rx_t nic_rx_handle = NIC_RX_NOT_HANDLED;

    while(1)
    {
        osal_wait_event(drv_nicIsr_event);

        osal_list_for_each_entry_safe(pEntry, n, &nicListHead.list, list)
        {
            unit = pEntry->unit;
            for (i = 0; i < NIC_RX_CB_PRIORITY_NUMBER; i++)
            {
                if (_nic_rx_cb_tbl[unit][i].rx_callback != NULL)
                {
                    nic_rx_handle = _nic_rx_cb_tbl[unit][i].rx_callback(unit, pEntry->pPacket, _nic_rx_cb_tbl[unit][i].pCookie);
                    if (NIC_RX_HANDLED_OWNED == nic_rx_handle)
                    {
                        break;
                    }
                }
            }
            if (nic_rx_handle != NIC_RX_HANDLED_OWNED)
            {   /* We have to free this packet here */
                _nic_init_conf[unit].pkt_free(unit, pEntry->pPacket);
            }
            jumbo = pEntry->jumbo;

            osal_isr_disable_interrupt();
            osal_list_del(&pEntry->list);
            osal_isr_enable_interrupt();

            osal_free(pEntry);

            if (jumbo)
                curNic_rx_pkt_cnt = osal_atomic_sub_return(JUMBO_CLUSTER_NUM, &nic_rx_pkt_cnt);
            else
                curNic_rx_pkt_cnt = osal_atomic_sub_return(1, &nic_rx_pkt_cnt);
            if (curNic_rx_pkt_cnt == 0 && osal_atomic_read(&nic_mem_lock))
            {
                osal_atomic_set(&nic_mem_lock, 0);
                for (ring = 0; ring < rxRingNum[unit]; ring++)
                    _nic_isr_rxRoutine(unit, ring);
                _nic_intrMask_set(unit, NIC_RX_DONE, NIC_RX_RING_ALL);
                _nic_intrMask_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL);
                nicReleaseCnt++;
            }
        }
    }
}
#endif

static int32 _nic_packet_free(uint32 unit)
{
    uint32   ringId;
    uint32   entryId;
    if (NULL == _nic_init_conf[unit].pkt_free)
        return RT_ERR_FAILED;

    for (ringId = 0; ringId < rxRingNum[unit]; ringId++)
    {
        for (entryId = 0; entryId < rxRingIdSize[unit][ringId]; entryId++)
        {
            if (NULL != pRx_pkt_privAry[unit][ringId][entryId].packet)
            {
                _nic_init_conf[unit].pkt_free(unit, pRx_pkt_privAry[unit][ringId][entryId].packet);
            }
        }
    }
    for (ringId = 0; ringId < txRingNum[unit]; ringId++)
    {
        for (entryId = 0; entryId < txRingIdSize[unit][ringId]; entryId++)
        {
            if (NULL != pTx_pkt_privAry[unit][ringId][entryId].packet)
            {
                _nic_init_conf[unit].pkt_free(unit, pTx_pkt_privAry[unit][ringId][entryId].packet);
            }
        }
    }

    return RT_ERR_OK;
}



int32 _nic_ring_init(uint32 unit)
{
    int32       ret = RT_ERR_FAILED;
    uint32      i;


    pNic_rxRBase[unit] = osal_alloc(sizeof(nic_desc_t*) * rxRingNum[unit]);
    if (NULL == pNic_rxRBase[unit])
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "Error: Out of memory!");
        return RT_ERR_FAILED;
    }
    osal_memset(pNic_rxRBase[unit], 0, sizeof(nic_desc_t*) * rxRingNum[unit]);

    pNic_txRBase[unit] = osal_alloc(sizeof(nic_desc_t*) * txRingNum[unit]);
    if (NULL == pNic_txRBase[unit])
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "Error: Out of memory!");
        goto err_alloc;
    }
    osal_memset(pNic_txRBase[unit], 0, sizeof(nic_desc_t*) * txRingNum[unit]);


	pRx_pkt_privAry[unit] = osal_alloc(sizeof(pkt_priv_t*) * rxRingNum[unit]);
    osal_memset(pRx_pkt_privAry[unit], 0, sizeof(pkt_priv_t*) * rxRingNum[unit]);
	pTx_pkt_privAry[unit] = osal_alloc(sizeof(pkt_priv_t*) * txRingNum[unit]);
    osal_memset(pTx_pkt_privAry[unit], 0, sizeof(pkt_priv_t*) * txRingNum[unit]);



    for (i = 0; i < rxRingNum[unit]; i++)
    {
        pNic_rxRBase[unit][i] = (nic_desc_t*) (rx_desc_vaddr[unit] + sizeof(nic_desc_t) * totalRxRingNum[unit]);
        if (0)//(i == 0)
        {
            osal_printf("%s():%d  pNic_rxRBase[%d][%d]:%px  rx_desc_phy_addr[%d]:%#llx\n",
                        __FUNCTION__, __LINE__, unit, i, pNic_rxRBase[unit][i], unit, (uint64)rx_desc_phy_addr[unit]);
        }
        if (NULL == pNic_rxRBase[unit][i])
        {
            ret = RT_ERR_MEM_ALLOC;
            RT_LOG(LOG_DEBUG, MOD_NIC, "Error: Out of memory!");
            goto err_alloc;
        }
        if (((uintptr)pNic_rxRBase[unit][i] & 0x3) != 0)
        {
            osal_printf("FATAL Error: pNic_rxRBase[%d][%d](0x%llX) is NOT 4 Byte-Align!\n", unit, i, (uint64)(uintptr)pNic_rxRBase[unit][i]);
            goto err_alloc;
        }
        osal_memset(pNic_rxRBase[unit][i], 0, sizeof(nic_desc_t) * rxRingIdSize[unit][i]);
        //pNic_rxRBase[unit][i] = (nic_desc_t*)UNCACHE(pNic_rxRBase[unit][i]) + 8;

		pRx_pkt_privAry[unit][i] = (pkt_priv_t*) osal_alloc(sizeof(pkt_priv_t) * rxRingIdSize[unit][i]);
		if (NULL == pRx_pkt_privAry[unit][i])
        {
            ret = RT_ERR_MEM_ALLOC;
            RT_LOG(LOG_DEBUG, MOD_NIC, "Error: Out of memory!");
            goto err_alloc;
        }
        osal_memset(pRx_pkt_privAry[unit][i], 0, sizeof(pkt_priv_t) * rxRingIdSize[unit][i]);

        _nic_ringBaseAddr_set(unit, NIC_DIR_RX, i, (uintptr)((rx_desc_phy_addr[unit] - mem_base_addr[unit]) + sizeof(nic_desc_t) * totalRxRingNum[unit]));
        totalRxRingNum[unit] += rxRingIdSize[unit][i];
    }

    for (i = 0; i < txRingNum[unit]; i++)
    {
        pNic_txRBase[unit][i] = (nic_desc_t*) (tx_desc_vaddr[unit] + sizeof(nic_desc_t) * totalTxRingNum[unit]);
        if (0)//(i == 0)
        {
            osal_printf("%s():%d  pNic_txRBase[%d][%d]:%px  tx_desc_phy_addr[%d]:%#llx\n",
                        __FUNCTION__, __LINE__, unit, i, pNic_txRBase[unit][i], unit, (uint64)tx_desc_phy_addr[unit]);
        }
        if (NULL == pNic_txRBase[unit][i])
        {
            ret = RT_ERR_MEM_ALLOC;
            RT_LOG(LOG_DEBUG, MOD_NIC, "Error: Out of memory!");
            goto err_alloc;
        }
        if (((uintptr)pNic_txRBase[unit][i] & 0x3) != 0)
        {
            osal_printf("FATAL Error: pNic_txRBase[unit][%d](0x%llX) is NOT 4 Byte-Align!\n", i, (uint64)(uintptr)pNic_txRBase[unit][i]);
            goto err_alloc;
        }
        osal_memset(pNic_txRBase[unit][i], 0, sizeof(nic_desc_t) * txRingIdSize[unit][i]);
        //pNic_txRBase[unit][i] = (nic_desc_t*)UNCACHE(pNic_txRBase[unit][i]) + 8;

		pTx_pkt_privAry[unit][i] = (pkt_priv_t*) osal_alloc(sizeof(pkt_priv_t) * txRingIdSize[unit][i]);
		if (NULL == pTx_pkt_privAry[unit][i])
        {
            ret = RT_ERR_MEM_ALLOC;
            RT_LOG(LOG_DEBUG, MOD_NIC, "Error: Out of memory!");
            goto err_alloc;
        }
        osal_memset(pTx_pkt_privAry[unit][i], 0, sizeof(pkt_priv_t) * txRingIdSize[unit][i]);

        _nic_ringBaseAddr_set(unit, NIC_DIR_TX, i, (uintptr)((tx_desc_phy_addr[unit] - mem_base_addr[unit]) + sizeof(nic_desc_t) * totalTxRingNum[unit]));
        totalTxRingNum[unit] += txRingIdSize[unit][i];
    }


    return RT_ERR_OK;

err_alloc:
    _nic_ring_free(unit);
    return ret;
}

int32 _nic_ring_free(uint32 unit)
{
    uint32  i = 0;


    for (i = 0; i < rxRingNum[unit]; i++)
    {
        ALLOC_ERR_FREE(pRx_pkt_privAry[unit][i], osal_free);
    }

    for (i = 0; i < txRingNum[unit]; i++)
    {
        ALLOC_ERR_FREE(pTx_pkt_privAry[unit][i], osal_free);
    }

    ALLOC_ERR_FREE(pNic_rxRBase[unit], osal_free);
    ALLOC_ERR_FREE(pNic_txRBase[unit], osal_free);
    ALLOC_ERR_FREE(pRx_pkt_privAry[unit], osal_free);
    ALLOC_ERR_FREE(pTx_pkt_privAry[unit], osal_free);

    return RT_ERR_OK;
}


static int32    _nic_sw_dataBase_free(uint32 unit)
{
    ALLOC_ERR_FREE(pPacket[unit],           osal_free);
    ALLOC_ERR_FREE(jumboBuffAlloc[unit],    osal_free);
    ALLOC_ERR_FREE(jumboFlag[unit],         osal_free);
    ALLOC_ERR_FREE(cur_tx[unit],            osal_free);
    ALLOC_ERR_FREE(tx_cpu_ptr[unit],        osal_free);
    ALLOC_ERR_FREE(cur_rx_flag[unit],       osal_free);
    ALLOC_ERR_FREE(cur_rx[unit],            osal_free);
    ALLOC_ERR_FREE(rx_cpu_flag[unit],       osal_free);
    ALLOC_ERR_FREE(rx_cpu_ptr[unit],        osal_free);

    return RT_ERR_OK;
}


static int32   _nic_sw_dataBase_init(uint32 unit)
{

    ALLOC_ERR_RET(rx_cpu_ptr[unit],   osal_alloc, sizeof(int32) * rxRingNum[unit]);
    ALLOC_ERR_HDL(rx_cpu_flag[unit],  osal_alloc, sizeof(uint32) * rxRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(cur_rx[unit],       osal_alloc, sizeof(uint32) * rxRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(cur_rx_flag[unit],  osal_alloc, sizeof(uint32) * rxRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(tx_cpu_ptr[unit],   osal_alloc, sizeof(uint32) * txRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(tx_cpu_flag[unit],  osal_alloc, sizeof(uint32) * txRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(cur_tx[unit],       osal_alloc, sizeof(uint32) * txRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(cur_tx_flag[unit],  osal_alloc, sizeof(uint32) * txRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(jumboFlag[unit],    osal_alloc, sizeof(uint32) * rxRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(jumboBuffAlloc[unit], osal_alloc, sizeof(uint32) * rxRingNum[unit], err_alloc);
    ALLOC_ERR_HDL(pPacket[unit],      osal_alloc, sizeof(drv_nic_pkt_t *) * rxRingNum[unit], err_alloc);

    osal_memset(rx_cpu_ptr[unit],     0, sizeof(uint32) * rxRingNum[unit]);
    osal_memset(rx_cpu_flag[unit],    0, sizeof(uint32) * rxRingNum[unit]);
    osal_memset(cur_rx[unit],         0, sizeof(uint32) * rxRingNum[unit]);
    osal_memset(cur_rx_flag[unit],    0, sizeof(uint32) * rxRingNum[unit]);
    osal_memset(tx_cpu_ptr[unit],     0, sizeof(uint32) * txRingNum[unit]);
    osal_memset(tx_cpu_flag[unit],    0, sizeof(uint32) * txRingNum[unit]);
    osal_memset(cur_tx[unit],         0, sizeof(uint32) * txRingNum[unit]);
    osal_memset(cur_tx_flag[unit],    0, sizeof(uint32) * txRingNum[unit]);
    osal_memset(jumboFlag[unit],      0, sizeof(uint32) * rxRingNum[unit]);
    osal_memset(jumboBuffAlloc[unit], 0, sizeof(uint32) * rxRingNum[unit]);

    return RT_ERR_OK;

err_alloc:

    _nic_sw_dataBase_free(unit);
    return RT_ERR_FAILED;

}



static int32
_nic_init(uint32 unit, drv_nic_initCfg_t *pInitCfg)
{
    uint32  ntfy_support = 0;
    uint32  temp;
    uintptr ntfyBase = 0;
    uint32  val1, val2, val3;
    int32   i;
    int32   ret = RT_ERR_FAILED;
    int32   hol_size = 0;


    /* Check arguments */
    RT_PARAM_CHK(NULL == pInitCfg->pkt_alloc, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(NULL == pInitCfg->pkt_free, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(NULL == pInitCfg->rx_pkt_alloc, RT_ERR_NULL_POINTER);
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    RT_PARAM_CHK(NULL == pInitCfg->dev, RT_ERR_NULL_POINTER);
#endif

    pNic_rxRBase[unit]  = NULL;
    pNic_txRBase[unit]  = NULL;

    /* Reset the NIC Tx/Rx debug information */
    nic_debug_flag[unit] = 0;
    nic_tx_success_cntr[unit] = 0;
    nic_tx_failed_cntr[unit] = 0;
    nic_rx_success_cntr[unit] = 0;
    nic_rx_failed_cntr[unit] = 0;
    totalRxRingNum[unit] = 0;
    totalTxRingNum[unit] = 0;
    nic_rx_err_cnt[unit]      = 0;
    nic_rx_unExp_err_cnt[unit]  = 0;
    rx_runout_cnt[unit] = 0;

    val1 = val2 = val3 = 0;

    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_MEM)
    {
        if (RTK_9311B_FAMILY_ID(unit))
            mem_base_addr[unit] = MEM_BASE_ADDR_INT;
        else
            mem_base_addr[unit] = MEM_BASE_ADDR_EXT;
    }
    else
    {
#if defined(CONFIG_SDK_DRIVER_NIC_USER_MODE) && !defined(CONFIG_SDK_EXTERNAL_CPU)
        if (RTK_9311B_FAMILY_ID(unit))
            mem_base_addr[unit] = MEM_BASE_ADDR_INT;
        else
            mem_base_addr[unit] = MEM_BASE_ADDR_EXT;
#else
        mem_base_addr[unit] = MEM_BASE_ADDR_EXT;
#endif
    }

    _nic_init_conf[unit].pkt_size  = pInitCfg->pkt_size;
    _nic_init_conf[unit].pkt_alloc = pInitCfg->pkt_alloc;
    _nic_init_conf[unit].pkt_free  = pInitCfg->pkt_free;
    _nic_init_conf[unit].rx_pkt_alloc = pInitCfg->rx_pkt_alloc;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    _nic_init_conf[unit].dev       = pInitCfg->dev;
#endif

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
    if (0 == alloc_fail_thread_id)
    {
        RT_ERR_CHK(osal_wait_module_create(&alloc_fail_event), ret);
        alloc_fail_thread_id = osal_thread_create("nicAllocMonThred", 8192, 0, (void*)_nic_alloc_fail_handle_thread, NULL);
    }
#endif

    /* Reset NIC only */
    //_nic_prefetch_set(unit, 7, 32);

    _nic_cpuPortTxRxEnable_set(unit, DISABLED);
    _nic_cpuForceLinkupEnable_set(unit, FALSE);
    _nic_nicEnable_set(unit, NIC_DIR_TX, FALSE);
    _nic_nicEnable_set(unit, NIC_DIR_RX, FALSE);


    /* Save the setting used by L2 notification */
    if (!RTK_9330_FAMILY_ID(unit))
    {
        ntfy_support = _nic_intrMask_get(unit, NIC_NTFY_DONE, &val1);
        if (ntfy_support != RT_ERR_CHIP_NOT_SUPPORTED)
        {
            _nic_intrMask_get(unit, NIC_NTFY_BUF_RUNOUT, &val2);
            _nic_intrMask_get(unit, NIC_NTFY_LOCALBUF_RUNOUT, &val3);
            _nic_ntfyBaseAddr_get(unit, &ntfyBase);
        }
    }

    _nic_swNicRst_set(unit);
    do
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "Wait ... ");
        _nic_swNicRst_get(unit, &temp);
    } while (temp != 0);
    _nic_swQueRst_set(unit);
    do
    {
        RT_LOG(LOG_DEBUG, MOD_NIC, "Wait ... ");
        _nic_swQueRst_get(unit, &temp);
    } while (temp != 0);
    RT_LOG(LOG_DEBUG, MOD_NIC, "OK");
    /* Restore the setting used by L2 notification */
    if (!RTK_9330_FAMILY_ID(unit))
    {
        if (ntfy_support != RT_ERR_CHIP_NOT_SUPPORTED)
        {
            _nic_intrMask_set(unit, NIC_NTFY_DONE, val1);
            _nic_intrMask_set(unit, NIC_NTFY_BUF_RUNOUT, val2);
            _nic_intrMask_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, val3);
            _nic_ntfyBaseAddr_set(unit, ntfyBase);
        }
    }

    if (RTK_9330_FAMILY_ID(unit) || RTK_9311B_FAMILY_ID(unit))
    {
        _nic_hw_init(unit);
    }


    /* Set CPU port to join the Lookup Miss Flooding Portmask */
    //_nic_cpuL2FloodMask_add(unit);

    /* Reset to default value */
    _nic_intrMask_set(unit, NIC_RX_DONE, 0);
    _nic_intrMask_set(unit, NIC_RX_RUNOUT, 0);
    _nic_intrMask_set(unit, NIC_TX_DONE, 0);
    _nic_intrMask_set(unit, NIC_TX_ALLDONE, 0);
    _nic_intrSts_set(unit, NIC_RX_DONE, NIC_RX_RING_ALL);
    _nic_intrSts_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL);
    _nic_intrSts_set(unit, NIC_TX_DONE, NIC_TX_RING_ALL);
    _nic_intrSts_set(unit, NIC_TX_ALLDONE, NIC_TX_RING_ALL);
    _nic_rxTruncateLength_set(unit, 0x640);

    _nic_ringInfo_get(unit, NIC_DIR_RX, &rxRingIdSize[unit], &rxRingNum[unit]);
    _nic_ringInfo_get(unit, NIC_DIR_TX, &txRingIdSize[unit], &txRingNum[unit]);

    _nic_cpuTagLen_get(unit, &rx_cTag_len[unit], &tx_cTag_len[unit]);

    RT_ERR_CHK(_nic_sw_dataBase_init(unit), ret);

    RT_ERR_HDL(_nic_ring_init(unit), err_alloc, ret);

    /* Register NIC IRQ handler */
#if (!defined(CONFIG_SDK_EXTERNAL_CPU) && defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)) || defined(__BOOTLOADER__)
    if (ioal_db[unit].acc_method == IOAL_SWCORE_ACC_MEM)
    {
        if (gIsrRegistered[unit] == FALSE)
        {
            if (NULL == (my_isr_param[unit] = osal_alloc(sizeof(isr_param_t) * (rxRingNum[unit] * 2 + txRingNum[unit] + NTFY_INTR_NUM))))
            {
                RT_LOG(LOG_FATAL_ERR, MOD_NIC, "Error - Allocating memory for my_isr_param failed!");
                goto err_alloc;
            }
        #if defined(__BOOTLOADER__)
            my_isr_param[unit][0].unit      = unit;
            my_isr_param[unit][0].ext_nic   = TRUE;
            if (RT_ERR_OK != (ret = osal_isr_register(unit, drv_nic_isr_handler, (void *)&my_isr_param[unit][0])))
            {
                RT_LOG(LOG_DEBUG, MOD_NIC, "Error - Register NIC IRQ handler failed!");
                goto err_alloc;
            }
        #else
            /* NIC interrupt register. */
            for (i = 0; i < (rxRingNum[unit] * 2 + txRingNum[unit]); i++)
            {
                my_isr_param[unit][i].unit      = unit;
                my_isr_param[unit][i].ext_nic   = FALSE;
                my_isr_param[unit][i].eventId   = NIC_RX_DONE_0 + i;

                if (RT_ERR_OK != (ret = osal_isr_register(RTK_DEV_NIC_RX_DONE_0 + i, drv_nic_isr_handler, (void *)&my_isr_param[unit][i])))
                {
                    RT_LOG(LOG_DEBUG, MOD_NIC, "Error - Register NIC IRQ handler failed!");
                    goto err_alloc;
                }
            }
            /* L2ntfy interrupt register. */
            for (i = 0; i < NTFY_INTR_NUM; i++)
            {
                uint32 j = rxRingNum[unit] * 2 + txRingNum[unit] + i;
                my_isr_param[unit][j].unit      = unit;
                my_isr_param[unit][j].ext_nic   = FALSE;
                my_isr_param[unit][j].eventId   = NIC_L2NTFY_BUF_RUNOUT + i;

                ret = osal_isr_register(RTK_DEV_L2_NTFY_LOCAL_NTFY_BUF_RUN_OUT + i,
                                        drv_nic_isr_handler,
                                        (void *)&my_isr_param[unit][j]);
                if (RT_ERR_OK != ret)
                {
                    RT_LOG(LOG_DEBUG, MOD_NIC,
                           "Error - Register L2ntfy IRQ handler failed!");
                    goto err_alloc;
                }
            }
        #endif
            gIsrRegistered[unit] = TRUE;
        }

    }
#endif
    for (i = 0; i < rxRingNum[unit]; i++)
    {
        _nic_holRingSize_set(unit, i, hol_size);
    }

    if (!RTK_9330_FAMILY_ID(unit)  && !RTK_9311B_FAMILY_ID(unit))
        _nic_cpuTagId_get(unit, &cpuTagId[unit]);
#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
    OSAL_INIT_LIST_HEAD(&nicListHead.list);
#endif

    /* Prepare the mBufs once */
    RT_ERR_HDL(_nic_isr_mbRoutine(unit), err_alloc, ret);

#if 0
    if (RTK_9330_FAMILY_ID(unit) /* || HWP_9311_FAMILY_ID(unit) */)
    {
        i = 0;
        do
        {
            RT_LOG(LOG_DEBUG, MOD_NIC, "Wait ... ");
            _nic_local_ringSts_get(unit, &temp);
            i++;
        } while (temp != 0 && i < 1000);

        if (temp != 0)
        {
            osal_printf("%s():%d local init failed\n", __FUNCTION__, __LINE__);
            return RT_ERR_FAILED;
        }
    }
#endif


    /*Every thing is ok now, NIC can RX/TX, and enable interrupt trigger*/
    _nic_intrMask_set(unit, NIC_RX_DONE, NIC_RX_RING_ALL);
    _nic_intrMask_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL);
    _nic_intrMask_set(unit, NIC_TX_DONE, NIC_TX_RING_ALL);
    _nic_nicEnable_set(unit, NIC_DIR_RX, TRUE);
    _nic_nicEnable_set(unit, NIC_DIR_TX, TRUE);
    _nic_supportJumboSize_get(unit, &gJumboSize[unit]);

    /* CPU port: Enable MAC Tx/Rx */
    _nic_cpuPortTxRxEnable_set(unit, TRUE);
    /* CPU port: Force link-up */
    _nic_cpuForceLinkupEnable_set(unit, TRUE);

    return ret;


err_alloc:
#if (!defined(CONFIG_SDK_EXTERNAL_CPU) && defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)) || defined(__BOOTLOADER__)
    ALLOC_ERR_FREE(my_isr_param[unit], osal_free);
#endif
    _nic_packet_free(unit);
    _nic_ring_free(unit);
    _nic_sw_dataBase_free(unit);
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
    if (0 != alloc_fail_thread_id)
    {
        osal_thread_destroy(alloc_fail_thread_id);
    }
    if (0 != alloc_fail_event)
    {
        osal_wait_module_destroy(alloc_fail_event);
    }
#endif
    return ret;
}
/* Function Name:
 *      drv_nic_init
 * Description:
 *      Initialize nic module of the specified device.
 * Input:
 *      unit     - unit id
 *      pInitCfg - pointer to initial config struct of NIC
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B, 9330
 * Note:
 *      Must initialize nic module before calling any nic APIs.
 * Changes:
 *      None
 */
int32
drv_nic_init(uint32 unit, drv_nic_initCfg_t *pInitCfg)
{
    int32 ret = RT_ERR_FAILED;

    RT_INIT_REENTRY_CHK(nic_init[unit]);
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check arguments */
    RT_PARAM_CHK(NULL == pInitCfg, RT_ERR_NULL_POINTER);

    /* Check whether it is inited, if inited, return fail */
    if (INIT_COMPLETED == nic_init[unit])
        return ret;

#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
    nic_sem[unit] = 0;
    nic_tx_sem[unit] = 0;
    /* create semaphore */
    nic_sem[unit] = osal_sem_mutex_create();
    if (0 == nic_sem[unit])
    {
        RT_ERR(RT_ERR_FAILED, (MOD_NIC), "nic semaphore create failed");
        ret = RT_ERR_FAILED;
        goto err_ret;
    }
    nic_tx_sem[unit] = osal_sem_mutex_create();
    if (0 == nic_tx_sem[unit])
    {
        RT_ERR(RT_ERR_FAILED, (MOD_NIC), "nic_tx semaphore create failed");
        ret = RT_ERR_FAILED;
        goto err_ret;
    }
#elif defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && !defined(__BOOTLOADER__)
    spl_rx_lock[unit] = 0;
    spl_tx_lock[unit] = 0;
    spl_rx_lock[unit] = osal_spl_spin_lock_create();
    if (0 == spl_rx_lock[unit])
    {
        goto err_ret;
    }
    spl_tx_lock[unit] = osal_spl_spin_lock_create();
    if (0 == spl_tx_lock[unit])
    {
        goto err_ret;
    }
#endif

    /* Initialize the NIC module */
    if ((ret = _nic_init(unit, pInitCfg)) != RT_ERR_OK)
    {
        goto err_ret;
    }

#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
    ret = osal_wait_module_create(&drv_nicIsr_event);
    if ((osal_thread_t)NULL == (rxThread_id = osal_thread_create("RTK NIC Rx Thread", NIC_RX_THREAD_STACK_SIZE, NIC_RX_THREAD_PRI, (void *)_nic_rx_thread, NULL)))
    {
        osal_printf("RTK NIC Rx Thread create failed\n");

        ret = RT_ERR_FAILED;
        goto err_ret;
    }
#endif

    /* set init flag to complete init */
    nic_init[unit] = INIT_COMPLETED;
    _makeCrcTable();

    return ret;
err_ret:
#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
    if (0 != nic_sem[unit])
    {
        osal_sem_mutex_destroy(nic_sem[unit]);
    }

    if (0 != nic_tx_sem[unit])
    {
        osal_sem_mutex_destroy(nic_tx_sem[unit]);
    }
#elif defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && !defined(__BOOTLOADER__)
    if (0 != spl_rx_lock[unit])
    {
        osal_spl_spin_lock_destroy(spl_rx_lock[unit]);
    }
    if (0 != spl_tx_lock[unit])
    {
        osal_spl_spin_lock_destroy(spl_tx_lock[unit]);
    }

#endif
    return ret;

}

/* Function Name:
 *      drv_nic_exit
 * Description:
 *      Release resources holded by NIC module.
 * Input:
 *      unit        - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID          - Invalid unit id
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_nic_exit(uint32 unit)
{
    int32   ret = RT_ERR_OK;
#if !defined(CONFIG_SDK_EXTERNAL_CPU) && defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    uint32  i = 0;
#endif
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Disable NIC rx/tx*/
    RT_ERR_CHK(_nic_cpuPortTxRxEnable_set(unit, FALSE), ret);
    RT_ERR_CHK(_nic_nicEnable_set(unit, NIC_DIR_TX, FALSE), ret);
    RT_ERR_CHK(_nic_nicEnable_set(unit, NIC_DIR_RX, FALSE), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_RX_DONE, 0), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_RX_RUNOUT, 0), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_TX_DONE, 0), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_TX_ALLDONE, 0), ret);
    RT_ERR_CHK(_nic_cpuL2FloodMask_remove(unit), ret);
    RT_ERR_CHK(_nic_intrSts_set(unit, NIC_RX_DONE, NIC_RX_RING_ALL), ret);
    RT_ERR_CHK(_nic_intrSts_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL), ret);
    RT_ERR_CHK(_nic_intrSts_set(unit, NIC_TX_DONE, NIC_TX_RING_ALL), ret);
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
    if (0 != alloc_fail_thread_id)
    {
        osal_thread_destroy(alloc_fail_thread_id);
    }
    if (0 != alloc_fail_event)
    {
        osal_wait_module_destroy(alloc_fail_event);
    }
#endif


#if !defined(CONFIG_SDK_EXTERNAL_CPU) && defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    if (gIsrRegistered[unit])
    {
        for (i = 0; i < (rxRingNum[unit] * 2 + txRingNum[unit] + NTFY_INTR_NUM); i++)
        {
            RT_ERR_CHK(osal_isr_unregister(RTK_DEV_NIC_RX_DONE_0 + i), ret);
        }

        ALLOC_ERR_FREE(my_isr_param[unit], osal_free);
        gIsrRegistered[unit] = FALSE;
    }
#endif

    RT_ERR_CHK(_nic_packet_free(unit), ret);
    RT_ERR_CHK(_nic_ring_free(unit), ret);
    RT_ERR_CHK(_nic_sw_dataBase_free(unit), ret);

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && !defined(__BOOTLOADER__)
    osal_spl_spin_lock_destroy(spl_rx_lock[unit]);
    osal_spl_spin_lock_destroy(spl_tx_lock[unit]);
#endif

#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
    if ((ret = osal_thread_destroy(rxThread_id)) != RT_ERR_OK)
    {
        return ret;
    }
    if ((ret = osal_wait_module_destroy(drv_nicIsr_event)) != RT_ERR_OK)
    {
        return ret;
    }
#endif

    nic_init[unit] = INIT_NOT_COMPLETED;
    return ret;
}


/* Function Name:
 *      drv_nic_desc_init
 * Description:
 *      Assign OS allocate momoery for TX/RX descriptor.
 * Input:
 *      unit     - unit id
 *      vir_base - virtual address of OS allocate memory
 *      phy_base - physical address of OS allocate memory
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 * Applicable:
 *      9311b, 9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_nic_desc_init(uint32 unit, uintptr vir_base, uintptr phy_base)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    tx_desc_vaddr[unit] = vir_base;
    rx_desc_vaddr[unit] = tx_desc_vaddr[unit] + NUM_TX_DESC * sizeof(nic_desc_t) * 4;
    tx_desc_phy_addr[unit] = phy_base;
    rx_desc_phy_addr[unit]    = tx_desc_phy_addr[unit] + NUM_TX_DESC * sizeof(nic_desc_t) * 4;

    return RT_ERR_OK;
}

/* Function Name:
 *      drv_nic_pkt_tx
 * Description:
 *      Transmit a packet via nic of the specified device.
 * Input:
 *      unit    - unit id
 *      pPacket - pointer to a single packet struct
 *      fTxCb   - pointer to a handler of transmited packets
 *      pCookie - application data returned with callback (can be null)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B, 9330
 * Note:
 *      When fTxCb is NULL, driver will free packet and not callback any more.
 * Changes:
 *      [SDK_3.0.0]
 *          The members of drv_nic_pkt_t has been updated.
 */
int32
drv_nic_pkt_tx(uint32 unit, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie)
{
    int32 ret = RT_ERR_FAILED;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_flags;
#endif
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check arguments */
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);

    /* Dispatch */
    NIC_TX_LOCK(unit, &spl_flags);

    /* Check init state */
    if (INIT_COMPLETED != nic_init[unit])
    {
        NIC_TX_UNLOCK(unit, &spl_flags);
        return RT_ERR_NOT_INIT;
    }

    ret = _nic_pkt_tx(unit, pPacket, fTxCb, pCookie);
    if (RT_ERR_OK == ret)
        nic_tx_success_cntr[unit]++;
    else
        nic_tx_failed_cntr[unit]++;

    NIC_TX_UNLOCK(unit, &spl_flags);


    return ret;
}


/* Function Name:
 *      drv_nic_rx_start
 * Description:
 *      Start the rx action of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_nic_rx_start(uint32 unit)
{
    int32 ret = RT_ERR_FAILED;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    ret = _nic_nicEnable_set(unit, NIC_DIR_RX, TRUE);

    return ret;
}

/* Function Name:
 *      drv_nic_rx_stop
 * Description:
 *      Stop the rx action of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_nic_rx_stop(uint32 unit)
{
    int32 ret = RT_ERR_FAILED;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    /* Check arguments */

    /* Dispatch */
    ret = _nic_nicEnable_set(unit, NIC_DIR_RX, FALSE);

    return ret;
}

/* Function Name:
 *      drv_nic_rx_status_get
 * Description:
 *      Get NIC rx status of the specified device.
 * Input:
 *      unit    - unit id
 * Output:
 *      pStatus - rx status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_CHIP_NOT_FOUND
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
int32
drv_nic_rx_status_get(uint32 unit, uint32 *pStatus)
{
    int32   ret = RT_ERR_FAILED;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    ret = _nic_nicEnable_get(unit, NIC_DIR_RX, pStatus);

    return ret;
}

/* Function Name:
 *      drv_nic_rx_register
 * Description:
 *      Register to receive callbacks for received packets of the specified device.
 * Input:
 *      unit     - unit id
 *      priority - Relative priority of the callback (255 is lowest)
 *      fRxCb    - pointer to a handler of received packets
 *      pCookie  - application data returned with callback (can be null)
 *      flags    - optional flags for reserved
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B, 9330
 * Note:
 *      If flags have turn on the NIC_FLAG_RX_CRC_INCLUDE flag, means that asking packet
 *      handed to upper layer should include CRC.
 * Changes:
 *      [SDK_3.0.0]
 *          The members of drv_nic_pkt_t within drv_nic_rx_cb_f has been updated.
 */
int32
drv_nic_rx_register(uint32 unit, uint8 priority, drv_nic_rx_cb_f fRxCb, void *pCookie, uint32 flags)
{
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check arguments */
    RT_PARAM_CHK(priority > NIC_RX_CB_PRIORITY_MAX, RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(NULL == fRxCb, RT_ERR_NULL_POINTER);

    NIC_LOCK(unit);
    NIC_RX_LOCK(unit, &spl_rx_flags);
    NIC_TX_LOCK(unit, &spl_tx_flags);
    if (NIC_FLAG_RX_CRC_INCLUDE & flags)
        rxCRCInclude[unit] = 1;

    if (NULL == _nic_rx_cb_tbl[unit][priority].rx_callback)
    {
        _nic_rx_cb_tbl[unit][priority].rx_callback = fRxCb;
        _nic_rx_cb_tbl[unit][priority].pCookie     = pCookie;
        _nic_rx_intr_cb_cnt[unit]++;
    }
    else
    {
        /* Handler is already existing */
        NIC_TX_UNLOCK(unit, &spl_tx_flags);
        NIC_RX_UNLOCK(unit, &spl_rx_flags);
        NIC_UNLOCK(unit);
        return RT_ERR_FAILED;
    }

    NIC_TX_UNLOCK(unit, &spl_tx_flags);
    NIC_RX_UNLOCK(unit, &spl_rx_flags);
    NIC_UNLOCK(unit);

    return RT_ERR_OK;
}

/* Function Name:
 *      drv_nic_rx_unregister
 * Description:
 *      Unregister to receive callbacks for received packets of the specified device.
 * Input:
 *      unit     - unit id
 *      priority - Relative priority of the callback (255 is lowest)
 *      fRxCb    - pointer to a handler of received packets (can be null)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_nic_rx_unregister(uint32 unit, uint8 priority, drv_nic_rx_cb_f fRxCb)
{
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check arguments */
    RT_PARAM_CHK(priority > NIC_RX_CB_PRIORITY_MAX, RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(NULL == fRxCb, RT_ERR_NULL_POINTER);

    NIC_LOCK(unit);
    NIC_RX_LOCK(unit, &spl_rx_flags);
    NIC_TX_LOCK(unit, &spl_tx_flags);

    if (_nic_rx_cb_tbl[unit][priority].rx_callback == fRxCb)
    {
        _nic_rx_cb_tbl[unit][priority].rx_callback = NULL;
        _nic_rx_cb_tbl[unit][priority].pCookie     = NULL;
        _nic_rx_intr_cb_cnt[unit]--;
    }
    else
    {
        /* Handler is nonexistent */
        NIC_TX_UNLOCK(unit, &spl_tx_flags);
        NIC_RX_UNLOCK(unit, &spl_rx_flags);
        NIC_UNLOCK(unit);
        return RT_ERR_FAILED;
    }

    NIC_TX_UNLOCK(unit, &spl_tx_flags);
    NIC_RX_UNLOCK(unit, &spl_rx_flags);
    NIC_UNLOCK(unit);

    return RT_ERR_OK;
}


/* Function Name:
 *      drv_nic_pkt_alloc
 * Description:
 *      Packet allocate API in the specified device.
 * Input:
 *      unit     - unit id
 *      size     - packet size
 *      flags    - flags
 * Output:
 *      ppPacket - pointer buffer of packet
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_nic_pkt_alloc(uint32 unit, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    int32 ret = RT_ERR_FAILED;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    /* Check arguments */
    RT_PARAM_CHK(NULL == _nic_init_conf[unit].pkt_alloc, RT_ERR_NULL_POINTER);

    /* Dispatch */
    ret = _nic_init_conf[unit].pkt_alloc(unit, size, flags, ppPacket);

    return ret;
}


/* Function Name:
 *      drv_nic_pkt_free
 * Description:
 *      Packet free API in the specified device.
 * Input:
 *      unit     - unit id
 *      pPacket  - pointer buffer of packet
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_nic_pkt_free(uint32 unit, drv_nic_pkt_t *pPacket)
{
    int32 ret = RT_ERR_FAILED;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);


    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    /* Check arguments */
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK(NULL == _nic_init_conf[unit].pkt_free, RT_ERR_NULL_POINTER);

    /* Dispatch */
    ret = _nic_init_conf[unit].pkt_free(unit, pPacket);

    return ret;
}



/* Function Name:
 *      drv_nic_reset
 * Description:
 *      Packet free API in the specified device.
 * Input:
 *      unit     - unit id
 *      pPacket  - pointer buffer of packet
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B, 9330
 * Note:
 *      None
 */
int32
drv_nic_reset(uint32 unit)
{
    int32   ret;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    RT_INIT_CHK(nic_init[unit]);

    nic_init[unit] = INIT_NOT_COMPLETED;

    /* Disable NIC rx/tx*/
    RT_ERR_CHK(_nic_cpuForceLinkupEnable_set(unit, FALSE), ret);
    RT_ERR_CHK(_nic_cpuPortTxRxEnable_set(unit, FALSE), ret);
    RT_ERR_CHK(_nic_nicEnable_set(unit, NIC_DIR_TX, FALSE), ret);
    RT_ERR_CHK(_nic_nicEnable_set(unit, NIC_DIR_RX, FALSE), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_RX_DONE, 0), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_RX_RUNOUT, 0), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_TX_DONE, 0), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_TX_ALLDONE, 0), ret);
    RT_ERR_CHK(_nic_intrSts_set(unit, NIC_RX_DONE, NIC_RX_RING_ALL), ret);
    RT_ERR_CHK(_nic_intrSts_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL), ret);
    RT_ERR_CHK(_nic_intrSts_set(unit, NIC_TX_DONE, NIC_TX_RING_ALL), ret);

/*reset HW config*/
    RT_ERR_CHK(_nic_swNicRst_set(unit), ret);

/*reset software database*/
    RT_ERR_CHK(_nic_sw_dataBase_init(unit), ret);
    RT_ERR_CHK(_nic_isr_mbRoutine(unit), ret);

/*enalbe nic rx/tx*/

    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_RX_DONE, NIC_RX_RING_ALL), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_RX_RUNOUT, NIC_RX_RING_ALL), ret);
    RT_ERR_CHK(_nic_intrMask_set(unit, NIC_TX_DONE, NIC_TX_RING_ALL), ret);
    RT_ERR_CHK(_nic_nicEnable_set(unit, NIC_DIR_RX, TRUE), ret);
    RT_ERR_CHK(_nic_nicEnable_set(unit, NIC_DIR_TX, TRUE), ret);
    /* CPU port: Enable MAC Tx/Rx */
    RT_ERR_CHK(_nic_cpuPortTxRxEnable_set(unit, TRUE), ret);
    /* CPU port: Force link-up */
    RT_ERR_CHK(_nic_cpuForceLinkupEnable_set(unit, TRUE), ret);



    nic_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;
}


/* Function Name:
 *      drv_nic_dbg_get
 * Description:
 *      Get NIC debug flags of the specified device.
 * Input:
 *      unit   - unit id
 * Output:
 *      pFlags - NIC debug flags
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B, 9330
 * Note:
 *      There are 4 BIT flags as following:
 *      - DEBUG_RX_RAW_LEN_BIT
 *      - DEBUG_RX_CPU_TAG_BIT
 *      - DEBUG_TX_RAW_LEN_BIT
 *      - DEBUG_TX_CPU_TAG_BIT
 */
int32
drv_nic_dbg_get(uint32 unit, uint32 *pFlags)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    *pFlags = nic_debug_flag[unit];

    return RT_ERR_OK;
}

/* Function Name:
 *      drv_nic_dbg_set
 * Description:
 *      Set NIC debug flags of the specified device.
 * Input:
 *      unit  - unit id
 *      flags - NIC debug flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B, 9330
 * Note:
 *      There are 4 BIT flags can be selected as following:
 *      - DEBUG_RX_RAW_LEN_BIT
 *      - DEBUG_RX_CPU_TAG_BIT
 *      - DEBUG_TX_RAW_LEN_BIT
 *      - DEBUG_TX_CPU_TAG_BIT
 */
int32
drv_nic_dbg_set(uint32 unit, uint32 flags)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    nic_debug_flag[unit] = flags;

    return RT_ERR_OK;
}

int32
drv_nic_cntr_dump(uint32 unit)
{
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    NIC_LOCK(unit);
    NIC_RX_LOCK(unit, &spl_rx_flags);
    NIC_TX_LOCK(unit, &spl_tx_flags);
    osal_printf("Tx success counter : %0u \n", nic_tx_success_cntr[unit]);
    osal_printf("Tx failed counter  : %0u \n", nic_tx_failed_cntr[unit]);
    osal_printf("Rx success counter : %0u \n", nic_rx_success_cntr[unit]);
    osal_printf("Rx failed counter  : %0u \n", nic_rx_failed_cntr[unit]);
    osal_printf("nml_rx_alloc_cnt   : %llu \n", nml_rx_alloc_cnt[unit]);
    osal_printf("nml_free_cnt       : %llu \n", nml_free_cnt[unit]);
    osal_printf("nic_rx_err_cnt     : %0u \n", nic_rx_err_cnt[unit]);
    osal_printf("nic_rx_unExp_err_cnt     : %0u \n", nic_rx_unExp_err_cnt[unit]);
    osal_printf("rx_runout_cnt     : %0u \n", rx_runout_cnt[unit]);
    osal_printf("nic_rx_isr_alloc_fail_cnt     : %0u \n", nic_rx_isr_alloc_fail_cnt[unit]);
    osal_printf("nic_rx_mb_alloc_fail_cnt     : %0u \n", nic_rx_mb_alloc_fail_cnt[unit]);

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
    osal_printf("alloc_thrd_run     : %0u \n", osal_atomic_read(&alloc_thrd_run));
    osal_printf("nic_alloc_monThr_acc_cnt     : %0u \n", nic_alloc_monThr_acc_cnt);

#endif
#ifdef CONFIG_SDK_NIC_RX_CB_IN_THREAD
    osal_printf("nic_rx_pkt_cnt     : %0d \n", osal_atomic_read(&nic_rx_pkt_cnt));
    osal_printf("nic_mem_lock       : %0d \n", osal_atomic_read(&nic_mem_lock));
    osal_printf("nicLockCnt         : %0d \n", nicLockCnt);
    osal_printf("nicReleaseCnt      : %0d \n", nicReleaseCnt);
#endif
    NIC_TX_UNLOCK(unit, &spl_tx_flags);
    NIC_RX_UNLOCK(unit, &spl_rx_flags);
    NIC_UNLOCK(unit);

    return RT_ERR_OK;
}

int32
drv_nic_cntr_clear(uint32 unit)
{
    int32   ret = RT_ERR_OK;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    NIC_LOCK(unit);
    NIC_RX_LOCK(unit, &spl_rx_flags);
    NIC_TX_LOCK(unit, &spl_tx_flags);
    nic_tx_success_cntr[unit] = 0;
    nic_tx_failed_cntr[unit]  = 0;
    nic_rx_success_cntr[unit] = 0;
    nic_rx_failed_cntr[unit]  = 0;
    nic_tx_isr_cntr[unit]     = 0;
    nic_tx_ring_cntr[unit]    = 0;
    nml_rx_alloc_cnt[unit]    = 0;
    nml_free_cnt[unit]        = 0;
    nic_rx_err_cnt[unit]      = 0;
    nic_rx_unExp_err_cnt[unit]  = 0;
    NIC_TX_UNLOCK(unit, &spl_tx_flags);
    NIC_RX_UNLOCK(unit, &spl_rx_flags);
    NIC_UNLOCK(unit);

    return ret;
}

/* Function Name:
 *      drv_nic_ringbuf_dump
 * Description:
 *      Dump NIC buffer status of the specified device.
 * Input:
 *      unit   - unit id
 *      pOpt   - pointer to debug option
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      Following message are dump
 *      1) From SW View
 *      - Rx Ring Packet Header (FDPBase, CDPIdx, RDPIdx)
 *      - Tx Ring Packet Header (FDPBase, CDPIdx, RDPIdx)
 *      - Rx Ring mBuffer (FDPBase, CDPIdx, RDPIdx)
 *      - Tx Ring mBuffer (FDPBase, CDPIdx, RDPIdx)
 *      2) From HW View
 *      - Rx Ring Packet Header(CDPIdx)
 *      - Tx Ring Packet Header(CDPIdx)
 *      - Rx Ring mBuffer (CDPIdx)
 *      - Tx Ring mBuffer (CDPIdx)
 *      3) Register Information
 *      - CPUIIMR (CPU Interface Interrupt Mask Register)
 *      - CPUIISR (CPU Interface Interrupt Status Register)
 *      - CPUICR  (CPU Interface Control Register)
 */
int32
drv_nic_ringbuf_dump(uint32 unit, nic_dbg_opt_t *pOpt)
{
    uint32 i, j;
    uint32  value;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    if (NIC_DIR_RX == pOpt->dir)
    {
        osal_printf("RX RING  SW_rxFDPBase  \t      CPU_ptr      HW_ptr\n");
        for (i = 0; i < rxRingNum[unit]; i++)
        {
            _nic_hwPtr_get(unit, NIC_DIR_RX, i, &value);
            osal_printf(" %02u   %px    0x%08x   0x%08x  \n",
                        i, (pNic_rxRBase[unit][i]), (cur_rx_flag[unit][i] << CPU_PTR_FLAG_OFFSET) | cur_rx[unit][i], value);
        }
        osal_printf("\n");

        if (pOpt->ringId == NIC_DBGOPT_DUMP_ALL)
        {
            for (i = 0; i < rxRingNum[unit]; i++)
            {
                osal_printf("RX ring %u(p): \n", i);
                for (j = 0; j < rxRingIdSize[unit][i]; j++)
                {
                    _nic_desc_dump(unit, NIC_DIR_RX, i, j);

                }
                osal_printf("\n");
            }
        }
        else
        {
            osal_printf("RX ring %u(p): \n", pOpt->ringId);
            for (j = 0; j < rxRingIdSize[unit][pOpt->ringId]; j++)
            {
                _nic_desc_dump(unit, NIC_DIR_RX, pOpt->ringId, j);

            }
            osal_printf("\n");
        }
    }
    else
    {
        osal_printf("TXRING  SW_txFDPBase  \t      CPU_ptr    HW_ptr\n");
        for (i = 0; i < txRingNum[unit]; i++)
        {
            _nic_hwPtr_get(unit, NIC_DIR_TX, i, &value);
            osal_printf(" %u(p)   %px    0x%08x   0x%08x  \n",
                i, (pNic_txRBase[unit][i]), (tx_cpu_flag[unit][i] << CPU_PTR_FLAG_OFFSET) | tx_cpu_ptr[unit][i], value);

        }

        if (pOpt->ringId == NIC_DBGOPT_DUMP_ALL)
        {
            for (i = 0; i < txRingNum[unit]; i++)
            {
                osal_printf("TX ring %u(p): \n", i);
                for (j = 0; j < txRingIdSize[unit][i]; j++)
                {
                    _nic_desc_dump(unit, NIC_DIR_TX, i, j);

                }
                osal_printf("\n");
            }
        }
        else
        {
            osal_printf("TX ring %u(p): \n", pOpt->ringId);
            for (j = 0; j < txRingIdSize[unit][pOpt->ringId]; j++)
            {
                _nic_desc_dump(unit, NIC_DIR_TX, pOpt->ringId, j);

            }
            osal_printf("\n");
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      drv_nic_pktHdrMBuf_dump
 * Description:
 *      Dump NIC packet header and mbuf detail information of the specified device.
 * Input:
 *      unit  - unit id
 *      mode  - tx/rx mode
 *      start - start ring id
 *      end   - end ring id
 *      flags - dump flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1) valid 'mode' value:
 *      - NIC_PKTHDR_MBUF_MODE_RX
 *      - NIC_PKTHDR_MBUF_MODE_TX
 *      2) valid ring id (start .. end)
 *      - Rx (0 .. 7)
 *      - Tx (0 .. 1)
 *      3) valid 'flags' value:
 *      - TRUE: include packet raw data
 *      - FALSE: exclude packet raw data
 */
int32
drv_nic_pktHdrMBuf_dump(uint32 unit, uint32 mode, uint32 start, uint32 end, uint32 flags)
{
    uint32  i, j, *ring_size;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);


    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    osal_printf("------- Formal Information -------------------------\n");
    if (NIC_PKTHDR_MBUF_MODE_RX == mode)
    {
        osal_printf("==== Dump Rx packet header and mbuf ====\n");
        ring_size = rxRingIdSize[unit];
    }
    else
    {
        osal_printf("==== Dump Tx packet header and mbuf ====\n");
        ring_size = txRingIdSize[unit];
    }

    for (i = start; i <= end; i++)
    {
        for (j = 0; j < ring_size[i]; j++)
        {
            nic_desc_t    *pDesc;
            pkt_priv_t    *pPkt_priv;
            if (NIC_PKTHDR_MBUF_MODE_RX == mode)
            {
                pDesc = pNic_rxRBase[unit][i];
                pPkt_priv = pRx_pkt_privAry[unit][i];
            }
            else
            {
                pDesc = pNic_txRBase[unit][i];
                pPkt_priv = pTx_pkt_privAry[unit][i];
            }

            osal_printf("###################################################\n");
            osal_printf("ring[%u]_desc[%u]->buf_addr = 0x%llx\n", i, j, DESC_ADDR_2_PHY(pDesc->addr));
            osal_printf("ring[%u]_desc[%u]->buf_size = 0x%04x\n", i, j, (BE32_to_CPU(pDesc->info) >> 2) & 0xffff);
            osal_printf("ring[%u]_desc[%u]->more = 0x%08x\n", i, j, (BE32_to_CPU(pDesc->info) >> PKT_MORE_FLAG_OFFSET) & 0x1);
            osal_printf("ring[%u]_desc[%u]->tx_callback = %px\n", i, j, pPkt_priv->tx_callback);
            osal_printf("ring[%u]_desc[%u]->cookie = %px\n", i, j, pPkt_priv->cookie);
            if ((DESC_ADDR_2_PHY(pDesc->addr) != 0) && (TRUE == flags))
            {
                uint32  k;
                uint32  dump_len = DEBUG_DUMP_PKT_LEN; /* debug dump maximum length */
                uint32  pkt_len = (BE32_to_CPU(pDesc->info) >> 2) & 0xffff;
                uint8   *pPkt_data = pPkt_priv->packet->data;
                osal_printf("------------------- its raw data ----------------------\n");

                for (k = 0; k < dump_len; k++)
                {
                    if (k == pkt_len)
                        break;
                    if (0 == (k % 16))
                        osal_printf("[%04X] ", k);
                    osal_printf("%02X ", *(pPkt_data + k));
                    if (15 == (k % 16))
                        osal_printf("\n");
                }
                osal_printf("\n");
            }
        }
        osal_printf("###################################################\n");
    }
    return RT_ERR_OK;
}


int32   drv_nic_sramEntry_get(uint32 unit, nic_dbg_opt_t* pOpt, nic_dbg_sram_entry_t* pEntry)
{
   int32    ret = RT_ERR_FAILED;

   /* Check init state */
   RT_INIT_CHK(nic_init[unit]);

   /* Check arguments */
   RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
   RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);
   RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
   if(RTK_9311B_FAMILY_ID(unit))
        RT_PARAM_CHK((pOpt->ringId >= (rxRingNum[unit]+1)) && (pOpt->dir == NIC_DIR_RX), RT_ERR_INPUT);
   else
        RT_PARAM_CHK((pOpt->ringId >= rxRingNum[unit]) && (pOpt->dir == NIC_DIR_RX), RT_ERR_INPUT);
   RT_PARAM_CHK((pOpt->ringId >= txRingNum[unit]) && (pOpt->dir == NIC_DIR_TX), RT_ERR_INPUT);
   RT_PARAM_CHK((pOpt->entryId >= NIC_SRAM_ENTRY_NUM), RT_ERR_INPUT);

   ret = _nic_sramEntry_get(unit, pOpt, pEntry);

   return ret;
}



osal_isrret_t
drv_ntfy_isr_handler(void *isr_param)
{
#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE) && defined(CONFIG_SDK_DRIVER_L2NTFY)
    uint32  cpu_iisr = 0;
    uint32  unit    = ((isr_param_t *)isr_param)->unit;

    /* Check init state */
    RT_INIT_CHK(nic_init[unit]);

    NIC_LOCK(unit);

    _nic_intrSts_get(unit, NIC_NTFY_DONE, &cpu_iisr);
    if (cpu_iisr)
    {
        _nic_intrSts_set(unit, NIC_NTFY_DONE, cpu_iisr);
#if defined(CONFIG_SDK_RTL9311B)
        drv_l2ntfy_isr_handler(unit, isr_param);
#elif !defined(CONFIG_SDK_DRIVER_EXTC_NIC)
        drv_l2ntfy_isr_handler(unit, isr_param);
#endif
    }
    _nic_intrSts_get(unit, NIC_NTFY_BUF_RUNOUT, &cpu_iisr);
    if (cpu_iisr)
    {
        _nic_intrSts_set(unit, NIC_NTFY_BUF_RUNOUT, cpu_iisr);
        drv_l2ntfy_bufRunout_handler(unit, isr_param);
    }
    _nic_intrSts_get(unit, NIC_NTFY_LOCALBUF_RUNOUT, &cpu_iisr);
    if (cpu_iisr)
    {
        _nic_intrSts_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, cpu_iisr);
        drv_l2ntfy_localBufRunout_handler(unit, isr_param);
    }

    _nic_intrMask_set(unit, NIC_NTFY_DONE, ENABLED);
    _nic_intrMask_set(unit, NIC_NTFY_BUF_RUNOUT, ENABLED);
    _nic_intrMask_set(unit, NIC_NTFY_LOCALBUF_RUNOUT, ENABLED);

    NIC_UNLOCK(unit);
#endif

    return OSAL_INT_HANDLED;
}

