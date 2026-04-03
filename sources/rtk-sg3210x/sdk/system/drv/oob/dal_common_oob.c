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

#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <common/util/rt_bitop.h>
#include <common/util/rt_util.h>
#include <osal/sem.h>
#include <osal/lib.h>
#include <osal/memory.h>
#include <osal/thread.h>
#include <osal/spl.h>
#include <dal/dal_common.h>
#include <rtk/default.h>

#include <hwp/hw_profile.h>

#include <drv/oob/oob.h>
#include <private/drv/oob/oob_mapper.h>
#include <private/drv/oob/dal_common_oob.h>
#include <drv/gpio/generalCtrl_gpio.h>
#include <common/util/rt_util_intr.h>

#include <common/rt_autoconf.h>
#ifdef __UBOOT__
  #include <linux/dma-mapping.h>
#else
  #include <linux/pci.h>
  #include <linux/version.h>
#endif
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
  #include <osal/wait.h>
  #include <osal/atomic.h>
#endif

#include <dal/dal_construct.h>

/*
 * Symbol Definition
 */
typedef enum oob_phyIntrSource_e {
    OOB_LINK_INTR_SRC_INTR = 0, /* interrupt source is internally from GMAC polling PHY result */
    OOB_LINK_INTR_SRC_GPIO,     /* interrupt source is internally from GPIO pin */
    OOB_LINK_INTR_SRC_END,
} oob_phyIntrSource_t;


/* link monitor control block */
typedef struct dal_common_oob_linkMon_cb_s {
    osal_thread_t       polling_thread_id;
    uint32              scan_interval_us;
    uint32              link_monitor_enable;
    oob_phyIntrSource_t intr_source;

    uint32              link_change_units[((RTK_MAX_NUM_OF_UNIT - 1)/32) + 1]; /* interrupt usage for GMAC */
    rtk_portmask_t      link_change_portmask[RTK_MAX_NUM_OF_UNIT]; /* interrupt usage for GMAC */

    uint32              link_swScan_units[((RTK_MAX_NUM_OF_UNIT - 1)/32) + 1]; /* polling usage */
    rtk_portmask_t      link_swScan_portmask[RTK_MAX_NUM_OF_UNIT]; /* polling usage */

    rtk_portmask_t      link_status[RTK_MAX_NUM_OF_UNIT];
    uint32              num_of_linkMon_callback_f;
    rtk_port_linkMon_callback_t  linkMon_callback_f[RTK_MAX_NUM_OF_LINKMON_CB];
} dal_common_oob_linkMon_cb_t;

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


/*
 * Macro Definition
 */

#define     DESC_RSV_CNT        16
#define     DBG_HWINFO_BIT      (0x1 << 31)



#ifdef __BOOTLOADER__
  #define OOB_LOCK(unit, port)
  #define OOB_UNLOCK(unit, port)
  #define OOB_RX_LOCK(unit, port, flags)
  #define OOB_RX_UNLOCK(unit, port, flags)
  #define OOB_TX_LOCK(unit, port, flags)
  #define OOB_TX_UNLOCK(unit, port, flags)
#else
  #if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
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
      osal_spinlock_t         oob_spl_rx_lock[RTK_MAX_NUM_OF_UNIT], oob_spl_tx_lock[RTK_MAX_NUM_OF_UNIT];
      #define OOB_LOCK(unit, port)
      #define OOB_UNLOCK(unit, port)
      #define OOB_RX_LOCK(unit, port, flags)               osal_spl_spin_lock_irqsave(oob_spl_rx_lock[unit], flags)
      #define OOB_RX_UNLOCK(unit, port, flags)             osal_spl_spin_unlock_irqrestore(oob_spl_rx_lock[unit], flags)
      #define OOB_TX_LOCK(unit, port, flags)               osal_spl_spin_lock_irqsave(oob_spl_tx_lock[unit], flags)
      #define OOB_TX_UNLOCK(unit, port, flags)             osal_spl_spin_unlock_irqrestore(oob_spl_tx_lock[unit], flags)
  #endif
#endif
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      dma_map_single(dev, usr_addr, size, dir)
#elif defined(__BOOTLOADER__)
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      dma_map_single(usr_addr, size, dir)
#else
  #define DMA_ADDR_VIR2PHY(dev, usr_addr, size, dir)      ((usr_addr - _pkt_vir_base) + _pkt_phy_base)
  #define unlikely(x) (x)
#endif

#if defined(CONFIG_TARGET_ARCH_ARM)
  #define MEMORY_BARRIER()        ({ __asm__ __volatile__ ("dsb 0xF" : : : "memory");})
#else
  #define MEMORY_BARRIER()        ({ __asm__ __volatile__ ("": : :"memory"); })
#endif


#define DEBUG_DUMP_PKT_LEN          256
#define RX_INSERT_BYTE              2



#define DAL_CONS_NONE_STD_PHY(unit, idp)                        ((HWP_PHY_MODEL_BY_PORT(unit, idp) == RTK_PHYTYPE_RTL8213B) || (HWP_PHY_MODEL_BY_PORT(unit, idp) == RTK_PHYTYPE_RTL8363SC_VB))
#define DAL_CONS_BUS(u, p)                                      HWP_PORT_SMI(u, p)
#define DAL_CONS_DRV_SUPPORT(_pfunc)                            ((uintptr)_pfunc != (uintptr)drv_common_unavail)
#define DAL_CONS_DRV(name)                                      RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].name
#define DAL_CONS_IDP_CHK(unit, idp)                             ((HWP_PORT_SMI(unit, idp) != HWP_NONE) && !(HWP_PHY_EXIST(unit, idp) && DAL_CONS_NONE_STD_PHY(unit, idp)))
#define DAL_CONS_INDR_MODE_GET                                  DAL_CONS_DRV(_miiBusMdxProto_get)
#define DAL_CONS_INDR_MODE_SET                                  DAL_CONS_DRV(_miiBusMdxProto_set)
#define DAL_CONS_INDR_M0_READ(u, p, a, r, d)                    DAL_CONS_DRV(_portMiim_read)(u, p, DAL_CONS_REG(r), d)
#define DAL_CONS_INDR_M1_READ(u, p, a, r, d)                    DAL_CONS_DRV(_portMiimMmd_read)(u, p, a, DAL_CONS_REG(r), d)
#define DAL_CONS_INDR_WRITE(u, id, a, r, d)                     if (DAL_CONS_DRV(_construct_reg_write)) DAL_CONS_DRV(_construct_reg_write)(u, id, a, r, d);
#define DAL_CONS_GEN_REG(d, v)                                  DAL_CONS_GEN_REG_CAL(d, v, oob_construct_gen_conf)

/*
 * Data Declaration
 */
static uint32   linkMon_init = INIT_NOT_COMPLETED;
static osal_sem_t   link_change_sem = 0;
static osal_sem_t   link_scan_sem = 0;
static dal_common_oob_linkMon_cb_t     *pLinkMon_cb = NULL;
static uint32  isDataAuto = 0;
static uint8   txData[JUMBO_FRAME_SIZE_MAX] = {0};
static uint32  packetLen = 0;


static drv_nic_initCfg_t _oob_init_conf;

static uint32       rx_crc_cnt = 0;
static uint64       rx_success_cnt = 0;
static uint64       tx_success_cnt = 0;
static uint32       rx_err_cnt = 0;
static uint32       tx_failed_cnt = 0;
static uint32       rx_failed_cnt = 0;
static uint32       mb_alloc_fail_cnt = 0;
static uint32       rx_alloc_fail_cnt = 0;
static uint32       rx_runout_cnt = 0;
static uint32       rx_fs_with_no_ls_cnt = 0;
static uint32       rx_rcdf_rwt_cnt = 0;


static uint32       rxCRCInclude = 0;
static uint32       rx_jumbo_size = 0;
static uint32       tx_jumbo_size = 0;

static uint32       oob_debug_flag = 0;

static uintptr      oob_tx_desc_vaddr = 0;
static uintptr      oob_tx_desc_phy_addr = 0;
static uintptr      oob_rx_desc_vaddr = 0;
static uintptr      oob_rx_desc_phy_addr = 0;

static uint32   _oob_rx_intr_cb_cnt = 0;
static nic_rx_cb_entry_t _oob_rx_cb_tbl[NIC_RX_CB_PRIORITY_NUMBER] = {{NULL, NULL}};

static uint32       rx_desc_len = 0;
static uint32       tx_desc_len = 0;
static uint32       rxRingNum = 0;
static uint32       txRingNum = 0;
static uint32       totalRxRingNum = 0, totalTxRingNum = 0;

static uint32       *rxRingIdSize = NULL;
static uint32       *txRingIdSize = NULL;
static uint8        **pOob_rxRBase = NULL;
static uint8        **pOob_txRBase = NULL;
static pkt_priv_t 	**pRx_pkt_privAry = NULL;
static pkt_priv_t 	**pTx_pkt_privAry = NULL;

static uint32       *rx_cpu_ptr = NULL;
static uint32       *rx_cpu_flag = NULL;
static uint32       *cur_rx = NULL;
static uint32       *cur_rx_flag = NULL;
static uint32       *tx_cpu_ptr = NULL;
static uint32       *tx_cpu_flag = NULL;
static uint32       *cur_tx = NULL;
static uint32       *cur_tx_flag = NULL;

static uint32       *jumboFlag = NULL;
static uint32       *jumboBuffAlloc = NULL;
static uint32       *rx_cmplt_flag = NULL;

static drv_nic_pkt_t **pPacket = NULL;
static uintptr      _desc_vir_base = 0;
static uintptr      _desc_phy_base = 0;
static uintptr      _desc_vir_tail = 0;
static uintptr      _desc_phy_tail = 0;
#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
static uintptr      _pkt_vir_base = 0;
static uintptr      _pkt_phy_base = 0;
#endif

static uint32       oob_ring_init[RTK_MAX_NUM_OF_UNIT] = {INIT_NOT_COMPLETED};

#ifndef __BOOTLOADER__
static isr_param_t oob_isr_param;
static isr_param_t oob_isr_param_rxrunout;
static isr_param_t oob_isr_param_txdone;
//static isr_param_t oob_isr_param_txrunout;
#endif

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))

  static osal_event_t       oob_alloc_fail_event;
  static osal_thread_t      oob_alloc_fail_thread_id = 0;
  static osal_atomic_t      oob_alloc_thrd_run = OSAL_ATOMIC_INIT(0);
  static uint32             oob_alloc_monThr_acc_cnt = 0;
  static uint32             thrd_unit = 0;
  static uint32             thrd_port = 0;

#endif


uint32 rtk_rEfuse_oob_config_offset = 0;
uint32 rtk_rEfuse_oob_config_offset_s[RTK_MAX_NUM_OF_UNIT][RTK_MAX_SDS_PER_UNIT] = { { 0 } };
uint32 rtk_rEfuse_oob_config[RTK_MAX_NUM_OF_UNIT][RTK_MAX_SDS_PER_UNIT] = { { 0 } };



/* general config */
uint32 oob_construct_gen_conf[][2] =
    {
        {0x0100, 0x00C8},{0x0104, 0x4091},{0x0108, 0x0070},
    };

/*
 * Function Declaration
 */
#if defined(RTK_INTR_EVENT_PROC)
static int32 _dal_common_oob_linkMon_intr_handler(void *isr_param, oob_phyIntrSource_t intr_src);
static int32 _dal_common_oob_linkMon_mac_intr_handler(void *isr_param);
static int32 _dal_common_oob_linkMon_gpio_intr_handler(void *isr_param);
#endif
static void _dal_common_oob_linkMon_hwScan(void);
static void _dal_common_oob_linkMon_interrupt_update_port_status(uint32 unit, rtk_port_t port);
static void _dal_common_oob_linkMon_thread(void *pInput);
static void _dal_common_oob_linkMon_swScan(void);
static void _dal_common_oob_linkMon_polling_update_port_status(uint32 unit, rtk_port_t port);

#if defined(RTK_INTR_EVENT_PROC)
int32
_dal_common_oob_linkMon_mac_intr_handler(void *isr_param)
{
    int32 ret;

    ret = _dal_common_oob_linkMon_intr_handler(isr_param, OOB_LINK_INTR_SRC_INTR);

    drv_intr_imrEnable_set(HWP_MY_UNIT_ID(), INTR_ISR_GMAC_LINKCHG, ENABLED);
    return ret;

}

static int32
_dal_common_oob_linkMon_gpio_intr_handler(void *isr_param)
{
    return _dal_common_oob_linkMon_intr_handler(isr_param, OOB_LINK_INTR_SRC_GPIO);
}
#endif

static int32 dal_common_oob_linkMon_phyInterrupt_set(uint32  unit, uint32 port)
{
#if defined(RTK_INTR_EVENT_PROC)
    drv_generalCtrlGpio_pinConf_t phy_gpio_pin;
    drv_generalCtrlGpio_devConf_t phy_gpio_dev;
    drv_generalCtrlGpio_devId_t devId;
    uint32  pinId;
    int32	ret;
    uint32 my_unit = HWP_MY_UNIT_ID();

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port=%u PHY intr dev/pin=%u/%u\n", unit, port,
            HWP_PHY_INTR_GPIO_DEV(unit, port), HWP_PHY_INTR_GPIO_PIN(unit, port));

    if ((devId = HWP_PHY_INTR_GPIO_DEV(unit, port)) == HWP_NONE)
        return RT_ERR_CHIP_NOT_SUPPORTED;

    pinId = HWP_PHY_INTR_GPIO_PIN(unit, port);
    osal_memset(&phy_gpio_dev, 0, sizeof(drv_generalCtrlGpio_devConf_t));
    phy_gpio_dev.direction = GPIO_DIR_IN;
    phy_gpio_dev.default_value = 1;
    if (devId >= GEN_CTRL_DEV_ID_TO_EXT_DEV_ID_OFFSET)
    {
        phy_gpio_dev.ext_gpio.access_mode = EXT_GPIO_ACCESS_MODE_MDC;
        phy_gpio_dev.ext_gpio.address = 0;
    }

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port=%u dev %u addr %u \n", unit, port, devId, phy_gpio_dev.ext_gpio.address);
    if ((ret = drv_generalCtrlGPIO_dev_init(my_unit, devId, &phy_gpio_dev)) != RT_ERR_OK)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port=%u dev %u addr %u init ret=%x\n", unit, port, devId, phy_gpio_dev.ext_gpio.address, ret);
        return ret;
    }
    if ((ret = drv_generalCtrlGPIO_devEnable_set(my_unit, devId, ENABLED)) != RT_ERR_OK)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port=%u GPIO enable ret=%x\n", unit, port, ret);
    }

    osal_memset(&phy_gpio_pin, 0, sizeof(drv_generalCtrlGpio_pinConf_t));
    phy_gpio_pin.default_value = 1;
    phy_gpio_pin.ext_gpio.direction = GPIO_DIR_IN;
    phy_gpio_pin.ext_gpio.interruptEnable = EXT_GPIO_INT_BOTH_EDGE;
    if ((ret = drv_generalCtrlGPIO_pin_init(my_unit, devId, pinId, &phy_gpio_pin)) != RT_ERR_OK)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port=%u GPIO pin init ret=%x\n", unit, port, ret);
    }
    if ((ret = drv_generalCtrlGPIO_intrHandler_register(my_unit, devId, pinId, _dal_common_oob_linkMon_gpio_intr_handler)) != RT_ERR_OK)
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port=%u GPIO handler register ret=%x\n", unit, port, ret);
    }

    return ret;
#else
    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB unit %u port=%u INTR_EVENT_PROC is undef\n", unit, port);
    return RT_ERR_OK;
#endif
}

#if defined(RTK_INTR_EVENT_PROC)
static int32
_dal_common_oob_linkMon_intr_handler(void *isr_param, oob_phyIntrSource_t intr_source)
{
    uint32  unit, port;
    //uint32  intr_sts;
    drv_intr_data_t         intr_data;

    unit = HWP_MY_UNIT_ID();
    port = RTK_OOB_IDX_TO_PORT(0);

    RT_LOG(LOG_DEBUG, (MOD_OOB), "%s:%u:OOB linkMon intr hdlr (src=%u)", __FUNCTION__, __LINE__, intr_source);

    osal_sem_take(link_scan_sem, OSAL_SEM_WAIT_FOREVER);

    if (intr_source == OOB_LINK_INTR_SRC_INTR)
    {
        //intr_sts = 0;
        //if (RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].intrSts_get(unit, port, OOB_LINK_CHG, &intr_sts) != RT_ERR_OK)
        if (INTR_CTRL(unit).swcoreSts_get(unit, INTR_ISR_GMAC_LINKCHG, &intr_data) != RT_ERR_OK)
        {
            return RT_ERR_FAILED;
        }

        RTK_PORTMASK_PORT_SET(pLinkMon_cb->link_change_portmask[unit], port);
        BITMAP_SET(pLinkMon_cb->link_change_units, unit);
    }
    else if (intr_source == OOB_LINK_INTR_SRC_GPIO)
    {
        /* consider 8213B chip case, it uses LED instead of interrupt pin, there will be no intr state  */
        RTK_PORTMASK_PORT_SET(pLinkMon_cb->link_change_portmask[unit], port);
        BITMAP_SET(pLinkMon_cb->link_change_units, unit);
     }

    osal_sem_give(link_scan_sem);

    if (pLinkMon_cb->link_monitor_enable == ENABLED)
    {
        /* wakeup monitor thread */
        osal_sem_give(link_change_sem);
    }

    return RT_ERR_OK;
}
#endif

/* Function Name:
 *      _dal_common_oob_linkMon_thread
 * Description:
 *      Link monitor polling thread
 * Input:
 *      None.
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *
 */
static void _dal_common_oob_linkMon_thread(void *pInput)
{
    int32   ret;

    /* forever loop */
    while (pLinkMon_cb->scan_interval_us != 0)
    {
        /* wait semaphore for link scan interval */
        ret = osal_sem_take(link_change_sem, pLinkMon_cb->scan_interval_us);

        /* Link Scan Part */
        if (RT_ERR_OK == ret)
        {/* if take semaphore, mean signal from hardware */
            _dal_common_oob_linkMon_hwScan();
        }
        else
        {
            _dal_common_oob_linkMon_swScan();
        }
    }

    osal_thread_exit(0);

    return;
}


/* Function Name:
 *      _dal_common_oob_linkMon_hwScan
 * Description:
 *      Unregister callback function for link change notification
 * Input:
 *      None.
 * Output:
 *      None
 * Return:
 *      None.
 * Note:
 *
 */
static void _dal_common_oob_linkMon_hwScan(void)
{
    uint32  unit;
    uint32  port;

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB linkMon hwScan.\n");
    /* scan each changed port */
    HWP_UNIT_TRAVS_LOCAL(unit)
    {
        if (!BITMAP_IS_SET(pLinkMon_cb->link_change_units, unit))
        {/* if this unit is not changed, continue */
            continue;
        }

        RT_LOG(LOG_TRACE, MOD_OOB, "OOB linkMon_hwScan unit=%u\n", unit);
        /* already process this unit, clear link change units */
        BITMAP_CLEAR(pLinkMon_cb->link_change_units, unit);

        osal_sem_take(link_scan_sem, OSAL_SEM_WAIT_FOREVER);

        /* update port status in changed portmask */
        HWP_OOB_PORT_TRAVS(unit, port)
        {
            if (RTK_PORTMASK_IS_PORT_SET(pLinkMon_cb->link_change_portmask[unit], port))
            {/* only update changed portmask */
                RT_LOG(LOG_TRACE, MOD_OOB, "OOB linkMon_hwScan port=%u\n", port);
                RTK_PORTMASK_PORT_CLEAR(pLinkMon_cb->link_change_portmask[unit], port);

                _dal_common_oob_linkMon_interrupt_update_port_status(unit, port);
            }
        }
        osal_sem_give(link_scan_sem);
    }
}


/* Function Name:
 *      _dal_common_oob_linkMon_swScan
 * Description:
 *      Unregister callback function for link change notification
 * Input:
 *      None.
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *
 */
static void _dal_common_oob_linkMon_swScan(void)
{
    uint32  unit;
    uint32  port;

    /* todo: need to skip disabled port */
    /* scan each changed port */
    HWP_UNIT_TRAVS_LOCAL(unit)
    {
        if (!BITMAP_IS_SET(pLinkMon_cb->link_swScan_units, unit))
        {/* if this unit is not changed, continue */
            continue;
        }
        osal_sem_take(link_scan_sem, OSAL_SEM_WAIT_FOREVER);

        /* update port status in sw scan portmask */
        HWP_OOB_PORT_TRAVS(unit, port)
        {
            if (RTK_PORTMASK_IS_PORT_SET(pLinkMon_cb->link_swScan_portmask[unit], port))
            {/* only update software scan portmask */
                _dal_common_oob_linkMon_polling_update_port_status(unit, port);
            }
        }
        osal_sem_give(link_scan_sem);
    }
}


/* Function Name:
 *      _dal_common_oob_linkMon_polling_update_port_status
 * Description:
 *      Update port status and callback to upper layer
 * Input:
 *      unit            - the unit need to be updated
 *      port            - the port need to be updated
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *
 */
static void
_dal_common_oob_linkMon_polling_update_port_status(uint32 unit, rtk_port_t port)
{
    int32   ret = 0;
    uint32  i;
    rtk_port_linkStatus_t  new_link, current_link;


    /* get newest port status */
    if ((ret = dal_common_oob_phyLink_get(unit, port, &new_link)) != RT_ERR_OK)
    {
        return;
    }

    /* get current link status */
    current_link = (RTK_PORTMASK_IS_PORT_SET(pLinkMon_cb->link_status[unit], port))? PORT_LINKUP: PORT_LINKDOWN;

    if (current_link != new_link)
    {
        if (PORT_LINKUP == new_link)
        {
            RTK_PORTMASK_PORT_SET(pLinkMon_cb->link_status[unit], port);
        }
        else
        {
            RTK_PORTMASK_PORT_CLEAR(pLinkMon_cb->link_status[unit], port);
        }

        RT_LOG(LOG_DEBUG, MOD_OOB, "OOB s/w scan link change. unit %u port %u\n", unit, port);
        for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
        {
            if (pLinkMon_cb->linkMon_callback_f[i] != NULL)
            {
                (pLinkMon_cb->linkMon_callback_f[i])(unit, port, new_link);
            }
        }
    }

    return;
}


/* Function Name:
 *      _dal_common_oob_linkMon_interrupt_update_port_status
 * Description:
 *      Update port status and callback to upper layer by interrupt thread
 * Input:
 *      unit            - the unit need to be updated
 *      port            - the port need to be updated
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *
 */
static void
_dal_common_oob_linkMon_interrupt_update_port_status(uint32 unit, rtk_port_t port)
{
    int32   ret = 0;
    uint32  i;
    rtk_port_linkStatus_t  new_link, current_link;
    rtk_enable_t enable;
    oob_pollingPhyStatus_t  pollSts;
    uint32  is_poll_status = FALSE;

    /* get newest port status */


    if ((RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pollingPhyEnable_get(unit, port, &enable) == RT_ERR_OK) && (enable == ENABLED))
    {
        if (((ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._pollingPhyStatus_get(unit, port, &pollSts)) == RT_ERR_OK))
        {
            new_link = pollSts.link;
            is_poll_status = TRUE;
        }
    }

    if (is_poll_status == FALSE)
    {
        if ((ret = dal_common_oob_phyLink_get(unit, port, &new_link)) != RT_ERR_OK)
        {
            return;
        }
    }

    /* get current link status */
    current_link = (RTK_PORTMASK_IS_PORT_SET(pLinkMon_cb->link_status[unit], port))? PORT_LINKUP: PORT_LINKDOWN;

    RT_LOG(LOG_DEBUG, MOD_OOB, "OOB h/w intr link change. unit %u port %u link=%u\n", unit, port, new_link);
    if (current_link != new_link)
    {
        if (PORT_LINKUP == new_link)
        {
            RTK_PORTMASK_PORT_SET(pLinkMon_cb->link_status[unit], port);
        }
        else
        {
            RTK_PORTMASK_PORT_CLEAR(pLinkMon_cb->link_status[unit], port);
        }

        for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
        {
            if (pLinkMon_cb->linkMon_callback_f[i] != NULL)
            {
                (pLinkMon_cb->linkMon_callback_f[i])(unit, port, new_link);
            }
        }
    }
    else
    {
        /* Process the per-port link change interrupt status is ON, but get link status is not changed case.
         * It is possible due to link change happens twice times, like UP->DOWN->UP or DOWN->UP->DOWN.
         * In this condition, need to callback upper register function twice time.
         */
        if (PORT_LINKUP == new_link)
        {
            RTK_PORTMASK_PORT_SET(pLinkMon_cb->link_status[unit], port);
            /* notification PORT_LINKDOWN */
            for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
            {
                if (pLinkMon_cb->linkMon_callback_f[i] != NULL)
                {
                    (pLinkMon_cb->linkMon_callback_f[i])(unit, port, PORT_LINKDOWN);
                }
            }
            for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
            {
                if (pLinkMon_cb->linkMon_callback_f[i] != NULL)
                {
                    (pLinkMon_cb->linkMon_callback_f[i])(unit, port, new_link);
                }
            }
        }
        else
        {
            RTK_PORTMASK_PORT_CLEAR(pLinkMon_cb->link_status[unit], port);

            for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
            {
                if (pLinkMon_cb->linkMon_callback_f[i] != NULL)
                {
                    (pLinkMon_cb->linkMon_callback_f[i])(unit, port, PORT_LINKUP);
                }
            }

            for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
            {
                if (pLinkMon_cb->linkMon_callback_f[i] != NULL)
                {
                    (pLinkMon_cb->linkMon_callback_f[i])(unit, port, new_link);
                }
            }
        }
    }

    return;
}



/* Function Name:
 *      dal_common_oob_linkMon_init
 * Description:
 *      Initial Link Monitor component
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *
 */
int32 _dal_common_oob_linkMon_init(void)
{

    RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB linkMon init");
    /* if was initilized, return RT_ERR_OK */
    RT_INIT_REENTRY_CHK_NO_WARNING(linkMon_init);


    /* init value */
    linkMon_init = INIT_NOT_COMPLETED;

    /* allocate memory for control block */
    pLinkMon_cb = osal_alloc(sizeof(dal_common_oob_linkMon_cb_t));
    if (0 == pLinkMon_cb){
        RT_INIT_ERR(RT_ERR_FAILED, (MOD_INIT|MOD_DAL), "link monitor allocate memory failed");
        return RT_ERR_FAILED;
    }
    osal_memset(pLinkMon_cb, 0, sizeof(dal_common_oob_linkMon_cb_t));

    /* create semaphore for sync, this semaphore is empty in beginning */
    link_change_sem = osal_sem_create(0);

    if (0 == link_change_sem){
        osal_free(pLinkMon_cb);
        RT_INIT_ERR(RT_ERR_FAILED, (MOD_INIT|MOD_DAL), "link monitor semaphore create failed");
        return RT_ERR_FAILED;
    }

    /* create semaphore for sync, this semaphore is empty in beginning */
    link_scan_sem = osal_sem_create(0);
    if (0 == link_scan_sem){
        osal_free(pLinkMon_cb);
        osal_sem_destroy(link_change_sem);
        RT_INIT_ERR(RT_ERR_FAILED, (MOD_INIT|MOD_DAL), "link monitor semaphore create failed");
        return RT_ERR_FAILED;
    }
    osal_sem_give(link_scan_sem);

    pLinkMon_cb->link_monitor_enable = ENABLED;
    pLinkMon_cb->scan_interval_us = OSAL_SEM_WAIT_FOREVER; /* wait interrupt */
    /* create polling thread */
    pLinkMon_cb->polling_thread_id = osal_thread_create("OobLinkMonThread",
                                RTK_DEFAULT_LINK_MON_POLLING_STACK_SIZE, RTK_DEFAULT_LINK_MON_POLLING_THREAD_PRI,
                                (void *)_dal_common_oob_linkMon_thread, NULL);

    if (0 == (pLinkMon_cb->polling_thread_id))
    {
        RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB linkMon thread create failed: id = %ld", pLinkMon_cb->polling_thread_id);
        return RT_ERR_THREAD_CREATE_FAILED;
    }

    linkMon_init = INIT_COMPLETED;

    return RT_ERR_OK;

}

/* Function Name:
 *      _dal_common_oob_linkMon_exit
 * Description:
 *      Deinitial Link Monitor component
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 * Note:
 *
 */
int32
_dal_common_oob_linkMon_exit(void)
{
    int32	ret;
    linkMon_init = INIT_NOT_COMPLETED;

    RT_LOG(LOG_DEBUG, (MOD_OOB), "OOB linkMon exit");
    ret = osal_thread_destroy(pLinkMon_cb->polling_thread_id);
    if (RT_ERR_OK != ret)
    {
        RT_ERR(ret, (MOD_COMMON), "");
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      dal_common_oob_linkMon_enable
 * Description:
 *      Enable link monitor thread
 * Input:
 *      scan_interval_us        - scan interval in us.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_THREAD_EXIST - The LinkMon thread is created already, and it's work.
 *      RT_ERR_THREAD_CREATE_FAILED - The LinkMon thread created failed.
 * Note:
 *      When enable link monitor thread, all link change interrupt will be handled by thread.
 *
 */
int32
dal_common_oob_linkMon_enable(uint32 unit,  rtk_port_t port, uint32 scan_interval_us)
{
    RT_LOG(LOG_DEBUG, (MOD_OOB), "scan_interval_us=%u",
           scan_interval_us);

    /* check Init status */
    RT_INIT_CHK(linkMon_init);

    /* parameter check */
    RT_PARAM_CHK((scan_interval_us < RTK_LINKMON_SCAN_INTERVAL_MIN), RT_ERR_OUT_OF_RANGE);

    pLinkMon_cb->link_monitor_enable = ENABLED;
    pLinkMon_cb->scan_interval_us = scan_interval_us;

    osal_sem_give(link_change_sem);
    return RT_ERR_OK;
}

/* Function Name:
 *      dal_common_oob_linkMon_disable
 * Description:
 *      Disable link scan thread
 * Input:
 *      None.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *      When disable link monitor thread, all link change interrupt will be callback to upper layer.
 */
int32
dal_common_oob_linkMon_disable(uint32 unit, rtk_port_t port)
{
    RT_LOG(LOG_DEBUG, (MOD_OOB), "oob_linkMon_disable");

    /* check Init status */
    RT_INIT_CHK(linkMon_init);

    pLinkMon_cb->link_monitor_enable = DISABLED;
    pLinkMon_cb->scan_interval_us = OSAL_SEM_WAIT_FOREVER;
    osal_sem_give(link_change_sem);

    return RT_ERR_OK;

}

/* Function Name:
 *      dal_common_oob_linkMon_register
 * Description:
 *      Register callback function for link change notification
 * Input:
 *      linkMon_callback    - callback function for link change
 * Output:
 *      None
 * Return:
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *
 */
int32
dal_common_oob_linkMon_register(uint32 unit, rtk_port_t port, rtk_port_linkMon_callback_t linkMon_callback)
{
    int32 ret;
 #if defined(RTK_INTR_EVENT_PROC)
    drv_intr_data_t         intr_data;
#endif
    uint32  i, available_index = RTK_MAX_NUM_OF_LINKMON_CB;

    RT_LOG(LOG_DEBUG, (MOD_OOB), "linkMon_callback=%llx",
           (uint64)(uintptr)linkMon_callback);

    /* check Init status */
    RT_INIT_CHK(linkMon_init);

    /* parameter check */
    RT_PARAM_CHK((NULL == linkMon_callback), RT_ERR_NULL_POINTER);

    if (unit != HWP_MY_UNIT_ID())
    {
        return RT_ERR_UNIT_ID;
    }

    /* check the callback function available index and check CB already exist or not? */
    for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
    {
        if (pLinkMon_cb->linkMon_callback_f[i] == NULL && (available_index == RTK_MAX_NUM_OF_LINKMON_CB))
        {
            available_index = i;
        }
        if ((pLinkMon_cb->linkMon_callback_f[i] != NULL) && (pLinkMon_cb->linkMon_callback_f[i] == linkMon_callback))
            return RT_ERR_CB_FUNCTION_EXIST;
    }

    if (available_index < RTK_MAX_NUM_OF_LINKMON_CB)
        pLinkMon_cb->linkMon_callback_f[available_index] = linkMon_callback;
    else
        return RT_ERR_CB_FUNCTION_FULL;

    if (pLinkMon_cb->num_of_linkMon_callback_f == 0)
    {
        RT_LOG(LOG_DEBUG, (MOD_OOB), "oob %u PHY EXIST:%u GPIO: %u", port, HWP_PHY_EXIST(unit, port), HWP_PHY_INTR_GPIO_DEV(unit, port));
        if (HWP_PHY_EXIST(unit, port) && (HWP_PHY_INTR_GPIO_DEV(unit, port) != HWP_NONE))
        {
            RT_LOG(LOG_DEBUG, (MOD_OOB), "oob register GPIO intr");
            /* interrupt source is GPIO */
            pLinkMon_cb->intr_source = OOB_LINK_INTR_SRC_GPIO;
            if ((ret = dal_common_oob_linkMon_phyInterrupt_set(unit, port)) != RT_ERR_OK)
            {
                RT_LOG(LOG_DEBUG, (MOD_OOB), "phy interrupt set fail");
            }
        }
        else
        {
            RT_LOG(LOG_DEBUG, (MOD_OOB), "register GMAC interrupt");
            /* interrupt source is GMAC */
            pLinkMon_cb->intr_source = OOB_LINK_INTR_SRC_INTR;
#if defined(RTK_INTR_EVENT_PROC)
            rt_util_intr_isr_register(INTR_ISR_GMAC_LINKCHG, _dal_common_oob_linkMon_mac_intr_handler, NULL);
            INTR_CTRL(unit).swcoreSts_get(unit, INTR_ISR_GMAC_LINKCHG, &intr_data);
            INTR_CTRL(unit).swcoreImrEnable_set(unit, INTR_ISR_GMAC_LINKCHG, ENABLED);
#endif
        }
    }
    pLinkMon_cb->num_of_linkMon_callback_f++;
    return RT_ERR_OK;
}


/* Function Name:
 *      dal_common_oob_linkMon_unregister
 * Description:
 *      Unregister callback function for link change notification
 * Input:
 *      linkMon_callback    - callback function for link change
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_NOT_INIT - The module is not initial
 * Note:
 *
 */
int32
dal_common_oob_linkMon_unregister(uint32 unit, rtk_port_t port, rtk_port_linkMon_callback_t linkMon_callback)
{
    uint32  i, exist_index = RTK_MAX_NUM_OF_LINKMON_CB;


    RT_LOG(LOG_DEBUG, (MOD_OOB), "oob_linkMon_unregister");

    /* check Init status */
    RT_INIT_CHK(linkMon_init);

    /* parameter check */
    RT_PARAM_CHK((NULL == linkMon_callback), RT_ERR_NULL_POINTER);

    if (unit != HWP_MY_UNIT_ID())
    {
        return RT_ERR_UNIT_ID;
    }

    /* find the exist callback function */
    for (i = 0; i < RTK_MAX_NUM_OF_LINKMON_CB; i++)
    {
        if ((pLinkMon_cb->linkMon_callback_f[i] != NULL) && (pLinkMon_cb->linkMon_callback_f[i] == linkMon_callback))
        {
            exist_index = i;
            break;
        }
    }

    if (exist_index < RTK_MAX_NUM_OF_LINKMON_CB)
        pLinkMon_cb->linkMon_callback_f[exist_index] = NULL;
    else
        return RT_ERR_CB_FUNCTION_NOT_FOUND;

    pLinkMon_cb->num_of_linkMon_callback_f--;

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_common_oob_linkMon_swScanEnable_set
 * Description:
 *      Configure portmask of software linkscan for certain unit
 * Input:
 *      unit                - callback function for link change
 *      pSwScan_portmask    - portmask for software scan
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - initialize success
 *      RT_ERR_FAILED       - initialize fail
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *
 */
int32
dal_common_oob_linkMon_swScanEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    /* check Init status */
    RT_INIT_CHK(linkMon_init);


    if (enable == ENABLED)
    {
        RTK_PORTMASK_PORT_SET(pLinkMon_cb->link_swScan_portmask[unit], port);
    }
    else
    {
        RTK_PORTMASK_PORT_CLEAR(pLinkMon_cb->link_swScan_portmask[unit], port);
    }

    if (0 == RTK_PORTMASK_GET_PORT_COUNT(pLinkMon_cb->link_swScan_portmask[unit]))
    {
        BITMAP_CLEAR(pLinkMon_cb->link_swScan_units, unit);
    }
    else
    {
        BITMAP_SET(pLinkMon_cb->link_swScan_units, unit);
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_common_oob_linkMon_swScanEnable_get
 * Description:
 *      Get portmask of software linkscan for certain unit
 * Input:
 *      unit                - callback function for link change
 * Output:
 *      pSwScan_portmask    - portmask for software scan
 * Return:
 *      RT_ERR_OK           - initialize success
 *      RT_ERR_FAILED       - initialize fail
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
dal_common_oob_linkMon_swScanEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    /* check Init status */
    RT_INIT_CHK(linkMon_init);

    /* parameter check */
    RT_PARAM_CHK((NULL == pEnable), RT_ERR_NULL_POINTER);

    *pEnable = RTK_PORTMASK_IS_PORT_SET(pLinkMon_cb->link_swScan_portmask[unit], port) ? ENABLED:DISABLED;

    return RT_ERR_OK;
}


void
_dal_common_oob_regConfig_init(uint32 unit, rtk_port_t port)
{
    uint32      id, idp, x0, w = 0, w0 = 0, w1 = 0, w2 = 0, w3 = 0, data, val = 0;
    int32       ret1, ret2, i;

    for (id = RTK_OOB_SDS_START; id <= RTK_OOB_SDS_END; id++)
    {
        rtk_rEfuse_oob_config[unit][id] = 0;
        if (!HWP_SDS_EXIST(unit, id) || ((idp = HWP_SDS_ID2MACID(unit, id)) == HWP_NONE))
        {
            continue;
        }

        if (!DAL_CONS_IDP_CHK(unit, idp))
        {
            continue;
        }

        if (!DAL_CONS_DRV_SUPPORT(DAL_CONS_INDR_MODE_GET))
        {
            x0 = DAL_CONS_ACC_MODE0;
        }
        else
        {
            if (DAL_CONS_INDR_MODE_GET(unit, idp, &x0) != RT_ERR_OK)
            {
                continue;
            }
        }

        if (x0 != DAL_CONS_ACC_MODE0)
        {
            if (!DAL_CONS_DRV_SUPPORT(DAL_CONS_INDR_MODE_SET))
            {
                DAL_CONS_INDR_MODE_SET(unit, idp, DAL_CONS_ACC_MODE0);
            }
        }

        w = w0 = w1 = w2 = w3 = 0;
        if (((ret1 = DAL_CONS_INDR_M0_READ(unit, idp, 0x0000, 0x005C, &w0)) == RT_ERR_OK) && ((w0 & 0xFFFF) != 0xFFFF))
        {
            DAL_CONS_INDR_M0_READ(unit, idp, 0x0000, 0x005D, &w1);
            DAL_CONS_INDR_M0_READ(unit, idp, 0x0000, 0x0044, &w);
            data = DAL_CONS_REG_DATA(w, w0, w1);
        }
        else
        {
            if (!DAL_CONS_DRV_SUPPORT(DAL_CONS_INDR_MODE_SET))
            {
                continue;
            }
            w = 0;
            DAL_CONS_INDR_MODE_SET(unit, idp, DAL_CONS_ACC_MODE1);
            if (((ret2 = DAL_CONS_INDR_M1_READ(unit, idp, 0x0001, 0x005D, &w3)) != RT_ERR_OK) || ((w3 & 0xFFFF) == 0xFFFF))
            {
                DAL_CONS_INDR_MODE_SET(unit, idp, x0);
                continue;
            }
            DAL_CONS_INDR_M1_READ(unit, idp, 0x001F, 0xA46A, &w);
            DAL_CONS_INDR_M1_READ(unit, idp, 0x0001, 0x005C, &w2);
            data = DAL_CONS_REG_DATA(w, w2, w3);
        }

        rtk_rEfuse_oob_config[unit][id] = data;
        DAL_CONS_INDR_WRITE(unit, id, 0x001A, id, data);

        if (!DAL_CONS_DRV_SUPPORT(DAL_CONS_INDR_MODE_SET))
            DAL_CONS_INDR_MODE_SET(unit, id, x0);
        RT_LOG(LOG_DEBUG, MOD_DAL, "%s:%u: rtk_rEfuse_oob_config[unit][id]=%x \n", __FUNCTION__, __LINE__, rtk_rEfuse_oob_config[unit][id]);
    }

    for (id = RTK_OOB_SDS_START; id <= RTK_OOB_SDS_END; id++)
    {
        if (rtk_rEfuse_oob_config[unit][id] == 0)
            continue;
        DAL_CONS_GEN_REG(rtk_rEfuse_oob_config[unit][id], val);
        rtk_rEfuse_oob_config_offset_s[unit][id] = val;
        rtk_rEfuse_oob_config_offset |= val;
        RT_LOG(LOG_DEBUG, MOD_DAL, "%s:%u: %u rtk_rEfuse_oob_config[unit][id]=%x val=%x \n", __FUNCTION__, __LINE__, id, rtk_rEfuse_oob_config[unit][id], val);
        DAL_CONS_INDR_WRITE(unit, id, 0x001C, id, val);
    }



}


/* Function Name:
 *      dal_common_oob_phyLink_get
 * Description:
 *      Get PHY link status from standard register (1.2).
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pLink - pointer to the link status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_common_oob_phyLink_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pLink)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._phyLink_get(unit, port, pLink);
}

/* Function Name:
 *      dal_common_oob_phy_connect
 * Description:
 *      Probe PHY if PHY driver is found, init PHY and config MAC accorfing to PHY ability(e.g. polling-PHY)
 * Input:
 *      unit - unit id
 *      port - OOB port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
dal_common_oob_phy_connect(uint32 unit, rtk_port_t port)
{
    int32   ret;
    rtk_enable_t enable;
    drv_oob_phyInfo_t phyInfo;

    RT_LOG(LOG_DEBUG, (MOD_OOB), "%s:%u:OOB PHY connect unit=%u port=%u\n", __FUNCTION__, __LINE__, unit, port);
    if ((ret = drv_mgmt_phy_probe(unit, port)) != RT_ERR_OK)
    {
        RT_LOG(LOG_DEBUG, (MOD_OOB), "%s:%u:OOB PHY connect unit=%u port=%u phy probe 0x%x\n", __FUNCTION__, __LINE__, unit, port, ret);
        return ret;
    }
    RT_LOG(LOG_DEBUG, (MOD_OOB), "%s:%u:OOB PHY connect unit=%u port=%u phy init\n", __FUNCTION__, __LINE__, unit, port);
    if ((ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phy_init(unit, port)) != RT_ERR_OK)
    {
        RT_LOG(LOG_DEBUG, (MOD_OOB), "%s:%u:OOB PHY connect unit=%u port=%u phy init 0x%x\n", __FUNCTION__, __LINE__, unit, port, ret);
    }

    RT_LOG(LOG_DEBUG, (MOD_OOB), "%s:%u:OOB PHY connect unit=%u port=%u polling init \n", __FUNCTION__, __LINE__, unit, port);
    /* config  MAC polling PHY */
    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._phyInfo_get(unit, port, &phyInfo);
    enable = (phyInfo.support_poll_std_reg == TRUE) ? ENABLED : DISABLED;
    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pollingPhyEnable_set(unit, port, enable);
    if (enable == ENABLED)
    {
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceSpeed_set(unit, port, DISABLED, PORT_SPEED_10M);
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceFullDuplex_set(unit, port, DISABLED);
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceLink_set(unit, port, DISABLED);
    }
    else
    {
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceSpeed_set(unit, port, ENABLED, PORT_SPEED_1000M);
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceFullDuplex_set(unit, port, ENABLED);
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceLink_set(unit, port, ENABLED);
    }

    return RT_ERR_OK;
}



void
dal_common_oob_tx_callback(uint32 unit, drv_nic_pkt_t *pPacket, void *pCookie)
{
    return;
}

/* Function Name:
 *      dal_common_oob_txData_set
 * Description:
 *      Set TX data of CPU TX packet through OOB port.
 * Input:
 *      unit    - unit id
 *      port    - oob port index
 *      isAuto - if packet data is auto generated
 *      pTxData    - pointer of packet TX data, it would be ignored if isAuto is true
 *      len   - packet size
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
dal_common_oob_txData_set(uint32 unit, rtk_port_t port, uint8 isAuto, uint8 *pTxData, uint32 len)
{
    uint32  bufSize;

    if (NULL == pTxData)
        return RT_ERR_NULL_POINTER;

    bufSize = (len <= sizeof(txData)) ? len : sizeof(txData);
    isDataAuto = isAuto;
    osal_memcpy(txData, pTxData, bufSize);
    packetLen = bufSize;

    return RT_ERR_OK;
}
/* Function Name:
 *      dal_common_oob_diagPkt_send
 * Description:
 *      Trigger to send packets from CPU through OOB port.
 * Input:
 *      unit    - unit id
 *      port    - oob port index
 *      num   - number of TX frame
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
dal_common_oob_diagPkt_send(uint32 unit, rtk_port_t port, uint32 num)
{
    int32   ret, cnt = 0, i, j, retryCnt = 5000;
    drv_nic_pkt_t *pPacket;

    if (packetLen == 0)
        packetLen = 64;

    if (RT_ERR_OK != RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pkt_alloc(unit, port, packetLen, 0, &pPacket))
    {
        osal_printf("[%s]: Alloc packet failed.\n", __FUNCTION__);
        return RT_ERR_FAILED;
    }

    pPacket->length         = packetLen;
    pPacket->tail           = pPacket->data + packetLen;
    pPacket->txIncludeCRC   = FALSE;

    if(isDataAuto)
    {
        /* Setting DA/SA */
        for (i = 0; i < 12; i++)
            pPacket->data[i] = 0;
        pPacket->data[5] = 0x01;
        pPacket->data[7] = 0xE0;
        pPacket->data[8] = 0x4C;

        for (i = 12, j = 0; i < packetLen; i++, j++)
            pPacket->data[i] = j & 0xff;
    }
    else
    {
        osal_memcpy(pPacket->data, txData, packetLen);
    }

    while (retryCnt > 0)
    {
        pPacket->data[9]    = cnt >> 16;
        pPacket->data[10]   = (cnt >> 8) & 0xff;
        pPacket->data[11]   = cnt & 0xff;
        if ((ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pkt_tx(unit, port, pPacket, dal_common_oob_tx_callback, NULL)))
        {
            osal_printf("%s():%d  ret:%#x\n", __FUNCTION__, __LINE__, ret);
            //break;
            retryCnt--;
            continue;
        }
        if (num != 0)
        {
            cnt++;
            if (cnt == num)
                break;
        }
    }

    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pkt_free(unit, port, pPacket);

    return RT_ERR_OK;
}   /* end of drv_nic_diagPkt_send */


int32   _dal_common_oob_raw_data_dump(uint8* ptr, uint32 len)
{
    uint32  i;

    osal_printf("=== [OOB Debug SHOW] =======address = %p, length = %d\n", ptr, len);
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

int32 _dal_common_oob_pkt_dump(uint32 unit, drv_nic_pkt_t *pPacket, oob_dir_t dir)
{
    int i;
    int dump_len = DEBUG_DUMP_PKT_LEN; /* debug dump maximum length */

    if (dir == OOB_DIR_TX)
        osal_printf("=== [OOB TX Debug] ================================= Len: %d \n", pPacket->length);
    else
        osal_printf("=== [OOB RX Debug] ================================= Len: %d \n", pPacket->length);

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


static int32    _dal_common_oob_desc_dump(uint32 unit, uint32 port, oob_dir_t dir, uint32 ringId, uint32 index)
{
    int i;
    int dump_len = 0; /* debug dump maximum length */
    uint8* desc = NULL;

    if (dir == OOB_DIR_RX)
    {
        dump_len = rx_desc_len;
        desc = pOob_rxRBase[ringId] + index * rx_desc_len;
    }
    else
    {
        dump_len = tx_desc_len;
        desc = pOob_txRBase[ringId] + index * tx_desc_len;
    }


    osal_printf("[DESC%d_%02d:%p] ", ringId, index, desc);
    for (i = 0; i < dump_len; i++)
    {
        osal_printf("%02X", *(desc + i));
        if ((i % 4) == 3)
            osal_printf(" ");
    }

    if (OOB_DIR_RX == dir)
    {
        if (pRx_pkt_privAry[ringId][index].packet)
            osal_printf("   [pktbuf:%p]", pRx_pkt_privAry[ringId][index].packet->data);
        else
            osal_printf("   [pktbuf: null]");
    }
    else
    {
        if (pTx_pkt_privAry[ringId][index].packet)
            osal_printf("   [pktbuf:%p]", pTx_pkt_privAry[ringId][index].packet->data);
        else
            osal_printf("   [pktbuf: null]");
    }
    osal_printf("\n");
    return 0;
}


static int32 _dal_common_oob_packet_free(uint32 unit, rtk_port_t port)
{
    uint32  ringId;
    uint32  i;

    for (ringId = 0; ringId < rxRingNum; ringId++)
    {
        for (i = 0; i < rxRingIdSize[ringId]; i++)
        {
            if (NULL != pRx_pkt_privAry[ringId][i].packet)
            {
                dal_common_oob_pkt_free(unit, port, pRx_pkt_privAry[ringId][i].packet);
            }
        }
    }
    return RT_ERR_OK;
}


int32   dal_common_oob_ringbuf_dump(uint32 unit, rtk_port_t port, oob_dbg_opt_t* pOpt)
{
    uint32 i, j;
    uint32  value;

    /* Check init state */
    RT_INIT_CHK(oob_ring_init[unit]);

    if (OOB_DIR_RX == pOpt->dir)
    {
        osal_printf("RX RING  SW_rxFDPBase  \t      CPU_ptr      HW_ptr\n");
        for (i = 0; i < rxRingNum; i++)
        {
            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._hwPtr_get(unit, port, OOB_DIR_RX, i, &value);
            osal_printf(" %02u   %p    0x%08x   0x%08x  \n",
                        i, (pOob_rxRBase[i]), (cur_rx_flag[i] << OOB_CPU_PTR_FLAG_OFFSET) | cur_rx[i], value);
        }
        if (pOpt->ringId == NIC_DBGOPT_DUMP_ALL)
        {
            for (i = 0; i < rxRingNum; i++)
            {
                osal_printf("RX ring %u(p): \n", i);
                for (j = 0; j < rxRingIdSize[i]; j++)
                {
                    _dal_common_oob_desc_dump(unit, port, OOB_DIR_RX, i, j);

                }
                osal_printf("\n");
            }
        }
        else
        {
            if (pOpt->ringId >= rxRingNum)
            {
                osal_printf("Error ring Id\n");
                return RT_ERR_INPUT;
            }
            osal_printf("RX ring %u(p): \n", pOpt->ringId);
            for (j = 0; j < rxRingIdSize[pOpt->ringId]; j++)
            {
                _dal_common_oob_desc_dump(unit, port, OOB_DIR_RX, pOpt->ringId, j);

            }
        }
        osal_printf("\n");
    }
    else
    {
        osal_printf("TXRING  SW_txFDPBase  \t      CPU_ptr    HW_ptr\n");
        for (i = 0; i < txRingNum; i++)
        {
            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._hwPtr_get(unit, port, OOB_DIR_TX, i, &value);
            osal_printf(" %u(p)   %p    0x%08x   0x%08x  \n",
                i, (pOob_txRBase[i]), (tx_cpu_flag[i] << OOB_CPU_PTR_FLAG_OFFSET) | tx_cpu_ptr[i], value);

        }

        if (pOpt->ringId == NIC_DBGOPT_DUMP_ALL)
        {
            for (i = 0; i < txRingNum; i++)
            {
                osal_printf("TX ring %u(p): \n", i);
                for (j = 0; j < txRingIdSize[i]; j++)
                {
                    _dal_common_oob_desc_dump(unit, port, OOB_DIR_TX, i, j);

                }
                osal_printf("\n");
            }
        }
        else
        {
            if (pOpt->ringId >= txRingNum)
            {
                osal_printf("Error ring Id\n");
                return RT_ERR_INPUT;
            }
            osal_printf("TX ring %u(p): \n", pOpt->ringId);
            for (j = 0; j < txRingIdSize[pOpt->ringId]; j++)
            {
                _dal_common_oob_desc_dump(unit, port, OOB_DIR_TX, pOpt->ringId, j);

            }
            osal_printf("\n");
        }
    }

    return RT_ERR_OK;
}


static int32    _dal_common_oob_isr_rxRoutine(uint32 unit, uint32 port, uint32 ringId)
{
    uint32  i = 0;
    uint32  rx_hw_ptr = 0;
    uint8*  pDesc = NULL;
    oob_desc_t  desc;
    drv_nic_rx_t oob_rx_handle = NIC_RX_NOT_HANDLED;
    uintptr         mapping = 0;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
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


    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._hwPtr_get(unit, port, NIC_DIR_RX, ringId, &rx_hw_ptr);

    do
    {
		uint32	        pkt_size = 0;
        drv_nic_pkt_t   *cur_pkt = NULL;


        pDesc    = pOob_rxRBase[ringId] + cur_rx[ringId] * rx_desc_len;

#if defined(__BOOTLOADER__)
        dma_unmap_single((uintptr)pDesc, rx_desc_len, DMA_FROM_DEVICE);
#endif

        if (NULL == (cur_pkt = pRx_pkt_privAry[ringId][cur_rx[ringId]].packet))
            break;

        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._raw_to_desc(unit, port, OOB_DIR_RX, pDesc, &desc);

        if (oob_debug_flag & DEBUG_RX_RAW_LEN_BIT)
        {
            _dal_common_oob_raw_data_dump(pDesc, rx_desc_len);
            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_dump(unit, port, OOB_DIR_RX, &desc);
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

pkt_handle:
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
            if (unlikely(desc.info & RX_DESC_INFO_RCDF_MASK) || unlikely(desc.info & RX_DESC_INFO_RWT_MASK))
            {
                /*suppose jumbo receiving error, should drop this jumbo packet*/
//                RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_dump(unit, port, OOB_DIR_RX, &desc);
//                osal_printf("%s():%d Receive error, jumbo flag = %d, receive desc with fs = %x, ls = %x, RCDF = %x, RWT = %x\n",
//                            __FUNCTION__, __LINE__, jumboFlag[ringId], (desc.info & DESC_INFO_FS_MASK), (desc.info & DESC_INFO_LS_MASK),
//                            (desc.info & RX_DESC_INFO_RCDF_MASK), (desc.info & RX_DESC_INFO_RWT_MASK));
                rx_rcdf_rwt_cnt++;

                if (jumboBuffAlloc[ringId])
                {
                    _oob_init_conf.pkt_free(unit, pPacket[ringId]);
                    jumboBuffAlloc[ringId] = FALSE;
                }
                _oob_init_conf.pkt_free(unit, cur_pkt);
                pRx_pkt_privAry[ringId][cur_rx[ringId]].packet = NULL;
                jumboFlag[ringId] = FALSE;
            }
            else if (unlikely(desc.info & DESC_INFO_FS_MASK))
            {
                /*jumbo receive uncomplete, but received another FS desc, drop jumbo packet, keep this desc*/
//                RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_dump(unit, port, OOB_DIR_RX, &desc);
//                osal_printf("%s():%d Receive error, jumbo flag = %d, receive desc with fs = %x, ls = %x\n",
//                            __FUNCTION__, __LINE__, jumboFlag[ringId], (desc.info & DESC_INFO_FS_MASK), (desc.info & DESC_INFO_LS_MASK));
                rx_fs_with_no_ls_cnt++;

                if (jumboBuffAlloc[ringId])
                {
                    _oob_init_conf.pkt_free(unit, pPacket[ringId]);
                    jumboBuffAlloc[ringId] = FALSE;
                }
                jumboFlag[ringId] = FALSE;
                goto pkt_handle;
            }
            else
            {
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
        }

        if (1 == rx_cmplt_flag[ringId])
        {

            uint32           handled = FALSE;

            rx_success_cnt++;

            /* NIC Rx debug message */
            if (oob_debug_flag & DEBUG_RX_RAW_LEN_BIT)
            {
                osal_printf("%s():%d  cur_rx[%d] = %d, addr = %p, size = %d\n", __FUNCTION__, __LINE__, ringId, cur_rx[ringId], pPacket[ringId]->data, pPacket[ringId]->length);
                _dal_common_oob_pkt_dump(unit, pPacket[ringId], NIC_DIR_RX);
            }

            if (oob_debug_flag & DEBUG_CPU_LOOPBACK_BIT)
            {
                if (RT_ERR_OK != RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pkt_tx(unit, port, pPacket[ringId], NULL, NULL))
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

        if (NULL == pRx_pkt_privAry[ringId][rx_cpu_ptr[ringId]].packet)
        {
            drv_nic_pkt_t *ptr = NULL;

            /* Alloc a new packet data buffer */
            if (RT_ERR_OK == _oob_init_conf.rx_pkt_alloc(unit, _oob_init_conf.pkt_size, 0, &ptr))
            {
                pDesc    = pOob_rxRBase[ringId] + rx_cpu_ptr[ringId] * rx_desc_len;
                RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._raw_to_desc(unit, port, OOB_DIR_RX, pDesc, &desc);

                pRx_pkt_privAry[ringId][rx_cpu_ptr[ringId]].packet = ptr;

                ptr->data = (uint8*) MAKE_4B_ALGN((uintptr)ptr->data);

                mapping     = (uintptr)DMA_ADDR_VIR2PHY(_oob_init_conf.dev, ptr->data, _oob_init_conf.pkt_size, DMA_FROM_DEVICE);

                RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._cpuPtr_set(unit, port, OOB_DIR_RX, ringId, rx_cpu_ptr[ringId]);
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
                RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_to_raw(unit, port, OOB_DIR_RX, &desc, pDesc);
            }
            else
            {
                RT_LOG(LOG_DEBUG, MOD_OOB, "%s():%d  Out of memory ! (alloc a new packet data buffer failed)\n", __FUNCTION__, __LINE__);
                //reclaim_mbuf = FALSE;
                rx_alloc_fail_cnt++;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
                if (0 == osal_atomic_read(&oob_alloc_thrd_run))
                {
                    osal_wake_up(oob_alloc_fail_event);
                }
#endif
            }
        }
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



static int32   _dal_common_oob_isr_txRoutine(uint32 unit, uint32 port,  uint32 ringId)
{
    uint32      tx_hw_ptr = 0;
    pkt_priv_t  *cur_pkt_priv = NULL;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
    uint8*  pDesc = NULL;
    oob_desc_t  desc;
    uint32  len = 0;
#endif
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
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
    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._hwPtr_get(unit, port, OOB_DIR_TX, ringId, &tx_hw_ptr);

    do
    {
        drv_nic_pkt_t   *cur_pkt = NULL;
//        RT_LOG(LOG_DEBUG, MOD_OOB, "%s() %d: cur_tx[%d] = %x!", __FUNCTION__, __LINE__, ringId, (cur_tx_flag[ringId] << OOB_CPU_PTR_FLAG_OFFSET) | cur_tx[ringId]);

        cur_pkt_priv = pTx_pkt_privAry[ringId] + cur_tx[ringId];
        if (NULL == (cur_pkt = cur_pkt_priv->packet))
            break;

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) || defined(__BOOTLOADER__)
        memset(&desc, 0, sizeof(oob_desc_t));
        pDesc    = pOob_txRBase[ringId] + cur_tx[ringId] * tx_desc_len;
  #if defined(__BOOTLOADER__)
        dma_unmap_single((uintptr)pDesc, tx_desc_len, DMA_FROM_DEVICE);      //TODO, do not need logically
  #endif
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._raw_to_desc(unit, port, OOB_DIR_TX, pDesc, &desc);

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
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_to_raw(unit, port, OOB_DIR_TX, &desc, pDesc);
#endif

        cur_tx[ringId]++;
        if (cur_tx[ringId] == txRingIdSize[ringId])
        {
            cur_tx[ringId]      = 0;
            cur_tx_flag[ringId] ^= 1;
        }

    } while (cur_tx[ringId] != tx_hw_ptr);

    OOB_TX_UNLOCK(unit, port, &spl_flags);

    return RT_ERR_OK;

}

static int32    _dal_common_oob_isr_mbRoutine(uint32   unit, uint32 port)
{
    int32   ret = RT_ERR_OK;
    uint32  ringId = 0;
    uint32  alloc_cnt = 0;
    uint8*  pDesc = NULL;
    oob_desc_t  desc;
    drv_nic_pkt_t   *pPacket = NULL;
    uintptr         mapping = 0;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
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

            pDesc    = pOob_rxRBase[ringId] + rx_cpu_ptr[ringId] * rx_desc_len;

            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._raw_to_desc(unit, port, OOB_DIR_RX, pDesc, &desc);

            if (NULL == pRx_pkt_privAry[ringId][rx_cpu_ptr[ringId]].packet)
            {
                /* Alloc a new packet data buffer */
                if (RT_ERR_OK != _oob_init_conf.rx_pkt_alloc(unit, _oob_init_conf.pkt_size, 0, &pPacket))
                {
                    RT_LOG(LOG_DEBUG, MOD_OOB, "Out of memory ! (alloc a new packet data buffer failed)");
                    mb_alloc_fail_cnt++;
                    ret = RT_ERR_FAILED;
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
                alloc_cnt++;
            }
            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._cpuPtr_set(unit, port, OOB_DIR_RX, ringId, rx_cpu_ptr[ringId]);
            rx_cpu_ptr[ringId]++;
            if (rx_cpu_ptr[ringId] >= rxRingIdSize[ringId])
            {
                rx_cpu_ptr[ringId] = 0;
                rx_cpu_flag[ringId] ^= 1;
                desc.info = INFO_SET_EOR(desc);
            }

            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_to_raw(unit, port, OOB_DIR_RX, &desc, pDesc);
#if 0
            osal_printf("%s():%d, DUMP rx desc raw data, rx_cpu_ptr[%d] = %d, pDesc = %p, mapping = %lx:\n", __FUNCTION__, __LINE__, ringId, rx_cpu_ptr[ringId], pDesc, mapping);
            _dal_common_oob_raw_data_dump(pDesc, rx_desc_len);
#endif
            if (alloc_cnt >= rxRingIdSize[ringId])
            {
                break;
            }
        } while (1);
    }

    OOB_RX_UNLOCK(unit, port, &spl_flags);

    return  ret;
}


#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
static void _oob_alloc_fail_handle_thread(void *pInput)
{
    while (1)
    {
        osal_wait_event(oob_alloc_fail_event);
        osal_atomic_set(&oob_alloc_thrd_run, 1);
        oob_alloc_monThr_acc_cnt++;
//        osal_printf("%s():%d  oob_alloc_monThr_acc_cnt = %d!\n", __FUNCTION__, __LINE__, oob_alloc_monThr_acc_cnt);

        if (RT_ERR_OK != _dal_common_oob_isr_mbRoutine(thrd_unit, thrd_port))
        {
            osal_time_usleep(200000);
            osal_wake_up(oob_alloc_fail_event);
        }
        else
        {
            osal_atomic_set(&oob_alloc_thrd_run, 0);
        }
    }
    osal_thread_exit(0);
}
#endif



osal_isrret_t
dal_common_oob_isr_handler(void *isr_param)
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
    if (event_id == INTR_ISR_GMAC_RXDONE)
#endif
    {
        drv_intr_swcoreSts_get(unit, INTR_ISR_GMAC_RXDONE, &isr_data);
        cpu_iisr = isr_data.u.rx_done;
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (RXDONE)\n", __FUNCTION__, __LINE__, cpu_iisr);
        if (cpu_iisr)
        {
            for (i = rxRingNum - 1; i >= 0; i--)
            {
                if (cpu_iisr & (0x1 << i))
                {
                    _dal_common_oob_isr_rxRoutine(unit, port, i);
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
        drv_intr_swcoreSts_get(unit, INTR_ISR_GMAC_TXDONE, &isr_data);
        cpu_iisr = isr_data.u.tx_done;
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (TXDONE)\n", __FUNCTION__, __LINE__, cpu_iisr);
        if (cpu_iisr)
        {
            for (i = txRingNum - 1; i >= 0; i--)
            {
                if (cpu_iisr & (0x1 << i))
                {
                    _dal_common_oob_isr_txRoutine(unit, port, i);
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
        drv_intr_swcoreSts_get(unit, INTR_ISR_GMAC_RXRUNOUT, &isr_data);
        cpu_iisr = isr_data.u.rx_runout;
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (RXRUNOUT)\n", __FUNCTION__, __LINE__, cpu_iisr);
        if (cpu_iisr)
        {
            rx_runout_cnt++;
            _dal_common_oob_isr_mbRoutine(unit, port);
            /*run_out pending flag could be cleared only if new desc is valid*/
            drv_intr_swcoreSts_get(unit, INTR_ISR_GMAC_RXRUNOUT, &isr_data);
        }
        drv_intr_imrEnable_set(unit, INTR_ISR_GMAC_RXRUNOUT, ENABLED);
    }

    /* Tx all done */
#ifndef __BOOTLOADER__
    if (event_id == INTR_ISR_GMAC_TXRUNOUT)
#endif
    {
        drv_intr_swcoreSts_get(unit, INTR_ISR_GMAC_TXRUNOUT, &isr_data);
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d data=%x (TXRUNOUT)\n", __FUNCTION__, __LINE__, isr_data.u.tx_runout);
        drv_intr_imrEnable_set(unit, INTR_ISR_GMAC_TXRUNOUT, ENABLED);
    }

    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:event %u end\n", __FUNCTION__, __LINE__, event_id);

    return OSAL_INT_HANDLED;

}



static int32    _dal_common_oob_pkt_tx(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie)
{
    uint32      ringId = 0;
    uintptr     mapping = 0;
    oob_desc_t  desc;
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

    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._raw_to_desc(unit, port, OOB_DIR_TX, pDesc, &desc);


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
            /*set tx vlan info*/
            desc.vlan_info = VLAN_INFO_SET_VLD(desc);
            desc.vlan_info = VLAN_INFO_SET_VID(desc, pPacket->rx_tag.inner_vid);
            desc.vlan_info = VLAN_INFO_SET_PRI(desc, pPacket->rx_tag.inner_pri);
            desc.vlan_info = VLAN_INFO_SET_CFI(desc, 0);
        }
        else
        {
            /*clear vlan info*/
            desc.vlan_info = 0;
        }
    }

    tx_cpu_ptr[ringId]++;
    if (tx_cpu_ptr[ringId] == txRingIdSize[ringId])
    {
        tx_cpu_ptr[ringId]  = 0;
        tx_cpu_flag[ringId] ^= 1;
        desc.info = INFO_SET_EOR(desc);
    }
    if ((oob_debug_flag & DEBUG_TX_RAW_LEN_BIT))
    {
        _dal_common_oob_pkt_dump(unit, pPacket, NIC_DIR_TX);
    }

    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_to_raw(unit, port, OOB_DIR_TX, &desc, pDesc);

    MEMORY_BARRIER();
#if defined(__BOOTLOADER__)
    flush_dcache_range((uintptr)pDesc, (uintptr)pDesc + tx_desc_len);
#endif

    if (oob_debug_flag & DEBUG_TX_RAW_LEN_BIT)
    {
        _dal_common_oob_raw_data_dump(pDesc, tx_desc_len);
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._desc_dump(unit, port, OOB_DIR_TX, &desc);
    }

    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._oobTxFetch_set(unit, port, ringId);
    return RT_ERR_OK;
}


static int32   _dal_common_oob_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32   i = 0;
    if (ENABLED == enable)
    {
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ethModuleEnable_set(unit, port, enable);
        for (i = 0; i < rxRingNum; i++)
        {
            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ringEnable_set(unit, port, OOB_DIR_RX, i, enable);
        }
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobEnable_set(unit, port, OOB_DIR_TX, TRUE);
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobEnable_set(unit, port, OOB_DIR_RX, TRUE);

    }
    else
    {
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ethModuleEnable_set(unit, port, enable);

        for (i = 0; i < rxRingNum; i++)
        {
            RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ringEnable_set(unit, port, OOB_DIR_RX, i, enable);
        }
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobEnable_set(unit, port, OOB_DIR_TX, FALSE);
        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobEnable_set(unit, port, OOB_DIR_RX, FALSE);
    }


    return RT_ERR_OK;
}


static int32   _dal_common_oob_nic_sw_dataBase_free(uint32 unit, rtk_port_t port)
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

    return RT_ERR_OK;
}


static int32   _dal_common_oob_nic_sw_dataBase_init(uint32 unit, rtk_port_t port)
{

    ALLOC_ERR_RET(rx_cpu_ptr,   osal_alloc, sizeof(int32) * rxRingNum);
    ALLOC_ERR_HDL(rx_cpu_flag,  osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(cur_rx,       osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(cur_rx_flag,  osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(tx_cpu_ptr,   osal_alloc, sizeof(uint32) * txRingNum, err_alloc);
    ALLOC_ERR_HDL(tx_cpu_flag,   osal_alloc, sizeof(uint32) * txRingNum, err_alloc);
    ALLOC_ERR_HDL(cur_tx,       osal_alloc, sizeof(uint32) * txRingNum, err_alloc);
    ALLOC_ERR_HDL(cur_tx_flag,       osal_alloc, sizeof(uint32) * txRingNum, err_alloc);
    ALLOC_ERR_HDL(jumboFlag,    osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(jumboBuffAlloc, osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(rx_cmplt_flag, osal_alloc, sizeof(uint32) * rxRingNum, err_alloc);
    ALLOC_ERR_HDL(pPacket, osal_alloc, sizeof(drv_nic_pkt_t *) * rxRingNum, err_alloc);

    osal_memset(rx_cpu_ptr, 0, sizeof(uint32) * rxRingNum);
    osal_memset(rx_cpu_flag, 0, sizeof(uint32) * rxRingNum);
    osal_memset(cur_rx, 0, sizeof(uint32) * rxRingNum);
    osal_memset(cur_rx_flag, 0, sizeof(uint32) * rxRingNum);
    osal_memset(tx_cpu_ptr, 0, sizeof(uint32) * txRingNum);
    osal_memset(tx_cpu_flag, 0, sizeof(uint32) * txRingNum);
    osal_memset(cur_tx, 0, sizeof(uint32) * txRingNum);
    osal_memset(cur_tx_flag, 0, sizeof(uint32) * txRingNum);
    osal_memset(jumboFlag, 0, sizeof(uint32) * rxRingNum);
    osal_memset(jumboBuffAlloc, 0, sizeof(uint32) * rxRingNum);
    osal_memset(rx_cmplt_flag, 0, sizeof(uint32) * rxRingNum);


    return RT_ERR_OK;

err_alloc:

    _dal_common_oob_nic_sw_dataBase_free(unit, port);
    return RT_ERR_FAILED;

}


static int32   _dal_common_oob_ring_free(uint32 unit, rtk_port_t port)
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

    oob_ring_init[unit] = INIT_NOT_COMPLETED;

    return RT_ERR_OK;
}




static int32   _dal_common_oob_nic_ring_init(uint32 unit, rtk_port_t port)
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
    oob_tx_desc_vaddr = MAKE_4B_ALGN(oob_tx_desc_vaddr);
    oob_tx_desc_phy_addr = MAKE_4B_ALGN(oob_tx_desc_phy_addr);



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

        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ringBaseAddr_set(unit, port, OOB_DIR_RX, i, oob_rx_desc_phy_addr + rx_desc_len * totalRxRingNum);
        totalRxRingNum += rxRingIdSize[i];
    }



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

        RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ringBaseAddr_set(unit, port, OOB_DIR_TX, i, oob_tx_desc_phy_addr + tx_desc_len * totalTxRingNum);
        totalTxRingNum += txRingIdSize[i];
    }

    oob_ring_init[unit] = INIT_COMPLETED;

    return RT_ERR_OK;

err_alloc:

    _dal_common_oob_ring_free(unit, port);
    return ret;

}


int32
dal_common_oob_init(uint32 unit, rtk_port_t port, drv_nic_initCfg_t *pInitCfg)
{
    int32 ret = RT_ERR_FAILED;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    /* Check arguments */
    RT_PARAM_CHK(NULL == pInitCfg, RT_ERR_NULL_POINTER);
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


#if !defined(__BOOTLOADER__)
  #if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
    oob_rx_sem[unit] = 0;
    oob_tx_sem[unit] = 0;
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
        ret = RT_ERR_FAILED;
        goto err_alloc;
    }
  #else
    oob_spl_rx_lock[unit] = 0;
    oob_spl_tx_lock[unit] = 0;
    oob_spl_rx_lock[unit] = osal_spl_spin_lock_create();
    if (0 == oob_spl_rx_lock[unit])
    {
        goto err_alloc;
    }
    oob_spl_tx_lock[unit] = osal_spl_spin_lock_create();
    if (0 == oob_spl_tx_lock[unit])
    {
        goto err_alloc;
    }
  #endif
#endif

    /* disable rx/tx */
    RT_ERR_HDL(_dal_common_oob_enable_set(unit, port, DISABLED), err_alloc, ret);

    /* disable interrupt */
    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_intr_enable_set(unit, port, DISABLED), err_alloc, ret);

    //    _oob_hw_init(unit);
    /* reset */
    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._swOobRst_set(unit, port), err_alloc, ret);

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_init(unit, port), err_alloc, ret);

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._mac_init(unit, port), err_alloc, ret);

    _dal_common_oob_regConfig_init(unit, port);

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._sds_construct(unit, port), err_alloc, ret);


    RT_ERR_HDL(_dal_common_oob_nic_sw_dataBase_init(unit, port), err_alloc, ret);

    rx_crc_cnt = 0;
    rx_success_cnt = 0;
    tx_success_cnt = 0;
    rx_err_cnt = 0;

    rx_alloc_fail_cnt = 0;
    mb_alloc_fail_cnt = 0;
    rx_fs_with_no_ls_cnt = 0;
    rx_rcdf_rwt_cnt = 0;


    totalRxRingNum = 0;
    totalTxRingNum = 0;
    rxCRCInclude = 0;

    oob_debug_flag = 0;

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._supportJumboSize_get(unit, port, &rx_jumbo_size, &tx_jumbo_size), err_alloc, ret);

    RT_ERR_HDL(_dal_common_oob_nic_ring_init(unit, port), err_alloc, ret);

    /* Prepare the mBufs once */
    RT_ERR_HDL(_dal_common_oob_isr_mbRoutine(unit, port), err_alloc, ret);



#ifndef __BOOTLOADER__
        osal_memset(&oob_isr_param, 0, sizeof(isr_param_t));
        oob_isr_param.unit = unit;
        oob_isr_param.port = RTK_OOB_IDX_TO_PORT(0);
        oob_isr_param.eventId = INTR_ISR_GMAC_RXDONE;
        rt_util_intr_isr_register(INTR_ISR_GMAC_RXDONE, dal_common_oob_isr_handler, &oob_isr_param);

        osal_memset(&oob_isr_param_txdone, 0, sizeof(isr_param_t));
        oob_isr_param_txdone.unit = unit;
        oob_isr_param_txdone.port = RTK_OOB_IDX_TO_PORT(0);
        oob_isr_param_txdone.eventId = INTR_ISR_GMAC_TXDONE;
        rt_util_intr_isr_register(INTR_ISR_GMAC_TXDONE, dal_common_oob_isr_handler, &oob_isr_param_txdone);

        osal_memset(&oob_isr_param_rxrunout, 0, sizeof(isr_param_t));
        oob_isr_param_rxrunout.unit = unit;
        oob_isr_param_rxrunout.port = RTK_OOB_IDX_TO_PORT(0);
        oob_isr_param_rxrunout.eventId = INTR_ISR_GMAC_RXRUNOUT;
        rt_util_intr_isr_register(INTR_ISR_GMAC_RXRUNOUT, dal_common_oob_isr_handler, &oob_isr_param_rxrunout);
#if 0
        osal_memset(&oob_isr_param_txrunout, 0, sizeof(isr_param_t));
        oob_isr_param_txrunout.unit = unit;
        oob_isr_param_txrunout.port = RTK_OOB_IDX_TO_PORT(0);
        oob_isr_param_txrunout.eventId = INTR_ISR_GMAC_TXRUNOUT;
        rt_util_intr_isr_register(INTR_ISR_GMAC_TXRUNOUT, dal_common_oob_isr_handler, &oob_isr_param_txrunout);
#endif
#endif

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
    if (0 == oob_alloc_fail_thread_id)
    {
        RT_ERR_HDL(osal_wait_module_create(&oob_alloc_fail_event), err_alloc, ret);
        thrd_unit = unit;
        thrd_port = port;
        oob_alloc_fail_thread_id = osal_thread_create("oobAllocMonThred", 8192, 0, (void*)_oob_alloc_fail_handle_thread, NULL);
    }
#endif

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_intr_enable_set(unit, port, ENABLED), err_alloc, ret);

    RT_ERR_HDL(_dal_common_oob_enable_set(unit, port, ENABLED), err_alloc, ret);

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

    RT_ERR_HDL(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._init_sts_set(unit, port, INIT_COMPLETED), err_alloc, ret);

#if !defined(__BOOTLOADER__)
    _dal_common_oob_linkMon_init();
#endif

    return ret;

err_alloc:
    RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._init_sts_set(unit, port, INIT_NOT_COMPLETED);
    _dal_common_oob_ring_free(unit, port);
    _dal_common_oob_nic_sw_dataBase_free(unit, port);
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
    if (0 != oob_alloc_fail_thread_id)
    {
        osal_thread_destroy(oob_alloc_fail_thread_id);
    }
    if (0 != oob_alloc_fail_event)
    {
        osal_wait_module_destroy(oob_alloc_fail_event);
    }
#endif
#if !defined(__BOOTLOADER__)
  #if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
    if (0 != oob_rx_sem[unit])
    {
        osal_sem_mutex_destroy(oob_rx_sem[unit]);
    }

    if (0 != oob_tx_sem[unit])
    {
        osal_sem_mutex_destroy(oob_tx_sem[unit]);
    }

  #else
    if (0 != oob_spl_rx_lock[unit])
    {
        osal_spl_spin_lock_destroy(oob_spl_rx_lock[unit]);
    }
    if (0 != oob_spl_tx_lock[unit])
    {
        osal_spl_spin_lock_destroy(oob_spl_tx_lock[unit]);
    }
  #endif
#endif

    return ret;

}


int32
dal_common_oob_desc_init(uint32 unit, rtk_port_t port, uintptr vir_base, uintptr phy_base)
{
    int32   ret = RT_ERR_FAILED;
    uint32  i = 0;
    uint32  temp = 0;

    _desc_vir_base = vir_base;
    _desc_phy_base = phy_base;

    oob_tx_desc_vaddr     = _desc_vir_base;
    oob_tx_desc_phy_addr  = _desc_phy_base;


    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ringInfo_get(unit, port, OOB_DIR_RX, &rxRingIdSize, &rxRingNum), ret);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._ringInfo_get(unit, port, OOB_DIR_TX, &txRingIdSize, &txRingNum), ret);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._descSize_get(unit, port, &rx_desc_len, &tx_desc_len), ret);


    for (i = 0; i < txRingNum; i++)
    {
        temp += txRingIdSize[i];
    }
    oob_rx_desc_vaddr = oob_tx_desc_vaddr + (temp + DESC_RSV_CNT) * tx_desc_len;
    oob_rx_desc_phy_addr = oob_tx_desc_phy_addr + (temp + DESC_RSV_CNT) * tx_desc_len;

    for (temp = 0, i = 0; i < rxRingNum; i++)
    {
        temp += rxRingIdSize[i];
    }

    _desc_vir_tail = oob_rx_desc_vaddr + (temp + DESC_RSV_CNT) * rx_desc_len;
    _desc_phy_tail = oob_rx_desc_phy_addr + (temp + DESC_RSV_CNT) * rx_desc_len;

#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
    _pkt_vir_base = _desc_vir_tail;
    _pkt_phy_base = _desc_phy_tail;
#endif
    return RT_ERR_OK;

}

#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
int32
dal_common_oob_pktBuf_baseAdddr_get(uint32 unit, rtk_port_t port, uintptr* pVAddr, uintptr* pPAddr)
{
    if (0 == _pkt_vir_base)
    {
        return RT_ERR_NOT_INIT;
    }
    else
    {
        *pVAddr = _pkt_vir_base;
        *pPAddr = _pkt_phy_base;
    }
    return RT_ERR_OK;
}

#endif


int32
dal_common_oob_exit(uint32 unit, rtk_port_t port)
{
    int32 ret = RT_ERR_FAILED;
    uint32  i = 0;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    /* Check init state */
    RT_INIT_CHK(oob_ring_init[unit]);

    /*disable hardware function*/
    RT_ERR_CHK(_dal_common_oob_enable_set(unit, port, DISABLED), ret);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_intr_enable_set(unit, port, DISABLED), ret);

#ifndef __BOOTLOADER__
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_RXDONE);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_TXDONE);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_RXRUNOUT);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_TXRUNOUT);
    rt_util_intr_isr_unregister(INTR_ISR_GMAC_LINKCHG);
#endif


#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))
    if (0 != oob_alloc_fail_thread_id)
    {
        osal_thread_destroy(oob_alloc_fail_thread_id);
    }
    if (0 != oob_alloc_fail_event)
    {
        osal_wait_module_destroy(oob_alloc_fail_event);
    }
#endif

    for (i = 0; i < rxRingNum; i++)
        RT_ERR_CHK(_dal_common_oob_isr_rxRoutine(unit, port, i), ret);
    for (i = 0; i < txRingNum; i++)
        RT_ERR_CHK(_dal_common_oob_isr_txRoutine(unit, port, i), ret);

    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._init_sts_set(unit, port, INIT_NOT_COMPLETED), ret);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._exit(unit, port), ret);
    RT_ERR_CHK(_dal_common_oob_packet_free(unit, port), ret);
    RT_ERR_CHK(_dal_common_oob_ring_free(unit, port), ret);
    RT_ERR_CHK(_dal_common_oob_nic_sw_dataBase_free(unit, port), ret);


#if !defined(__BOOTLOADER__)
  #if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
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
    osal_spl_spin_lock_destroy(oob_spl_rx_lock[unit]);
    osal_spl_spin_lock_destroy(oob_spl_tx_lock[unit]);
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


    return RT_ERR_OK;

}


int32
dal_common_oob_pkt_tx(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie)
{
    int32 ret = RT_ERR_FAILED;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_flags;
#endif
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    /* Check init state */
    RT_INIT_CHK(oob_ring_init[unit]);

    /* Check arguments */
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);

    /* Dispatch */
    OOB_TX_LOCK(unit, port, &spl_flags);

    ret = _dal_common_oob_pkt_tx(unit, port, pPacket, fTxCb, pCookie);
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
dal_common_oob_rx_register(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb, void *pCookie, uint32 flags)
{
    int32   ret = RT_ERR_OK;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    /* Check init state */

    RT_PARAM_CHK(priority > NIC_RX_CB_PRIORITY_MAX, RT_ERR_OUT_OF_RANGE);
    RT_PARAM_CHK(NULL == fRxCb, RT_ERR_NULL_POINTER);

    OOB_RX_LOCK(unit, port, &spl_rx_flags);
    OOB_TX_LOCK(unit, port, &spl_tx_flags);

    if (NIC_FLAG_RX_CRC_INCLUDE & flags)
        rxCRCInclude = 1;

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

    OOB_TX_UNLOCK(unit, port, &spl_tx_flags);
    OOB_RX_UNLOCK(unit, port, &spl_rx_flags);

    return ret;
}


int32
dal_common_oob_rx_unregister(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb)
{
    int32   ret = RT_ERR_FAILED;
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
    unsigned long   spl_rx_flags, spl_tx_flags;
#endif
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    /* Check init state */

    OOB_RX_LOCK(unit, port, &spl_rx_flags);
    OOB_TX_LOCK(unit, port, &spl_tx_flags);

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

    OOB_TX_UNLOCK(unit, port, &spl_tx_flags);
    OOB_RX_UNLOCK(unit, port, &spl_rx_flags);

    return ret;
}

int32   dal_common_oob_pkt_alloc(uint32 unit, rtk_port_t port, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    /* Check arguments */
    RT_PARAM_CHK(NULL == _oob_init_conf.pkt_alloc, RT_ERR_NULL_POINTER);
    return _oob_init_conf.pkt_alloc(unit, size, flags, ppPacket);
}



int32   dal_common_oob_pkt_free(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket)
{

    RT_PARAM_CHK(NULL == _oob_init_conf.pkt_free, RT_ERR_NULL_POINTER);
    return _oob_init_conf.pkt_free(unit, pPacket);
}

int32   dal_common_oob_dbg_get(uint32 unit, rtk_port_t port, uint32* pFlags)
{
    *pFlags = oob_debug_flag;
    return RT_ERR_OK;
}


int32   dal_common_oob_dbg_set(uint32 unit, rtk_port_t port, uint32 flags)
{
    oob_debug_flag = flags;

    return RT_ERR_OK;
}


int32   dal_common_oob_reset(uint32 unit, rtk_port_t port)
{
    int32   ret = RT_ERR_FAILED;

    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(!HWP_OOB_PORT(unit, port), RT_ERR_PORT_ID);
    RT_INIT_CHK(oob_ring_init[unit]);

    /* Disable OOB rx/tx*/
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._init_sts_set(unit, port, INIT_NOT_COMPLETED), ret);
    RT_ERR_CHK(_dal_common_oob_enable_set(unit, port, DISABLED), ret);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_intr_enable_set(unit, port, DISABLED), ret);

    /*reset hw config*/
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._swOobRst_set(unit, port), ret);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_init(unit, port), ret);

    /*reset software database*/
    RT_ERR_CHK(_dal_common_oob_nic_sw_dataBase_init(unit, port), ret);
    RT_ERR_CHK(_dal_common_oob_isr_mbRoutine(unit, port), ret);


    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._nic_intr_enable_set(unit, port, ENABLED), ret);
    RT_ERR_CHK(_dal_common_oob_enable_set(unit, port, ENABLED), ret);
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._init_sts_set(unit, port, INIT_COMPLETED), ret);

    return RT_ERR_OK;
}


static int32   _dal_common_oob_dump_sw_database(uint32 unit, uint32 port)
{
    uint32  i;
    osal_printf("------------- DUMP SOFTWARE DATABASE -----------\n");

    osal_printf("rx_cpu_ptr\t: 0x%x", (rx_cpu_flag[0] << OOB_CPU_PTR_FLAG_OFFSET) | rx_cpu_ptr[0]);
    osal_printf("\tcur_rx\t: 0x%x\n", (cur_rx_flag[0] << OOB_CPU_PTR_FLAG_OFFSET) | cur_rx[0]);
    osal_printf("rx_crc_cnt\t: 0x%x\n", rx_crc_cnt);

    for (i = 0; i < 5; i++)
    {
        osal_printf("tx_cpu_ptr[%d]\t: 0x%x", i, (tx_cpu_flag[i] << OOB_CPU_PTR_FLAG_OFFSET) | tx_cpu_ptr[i]);
        osal_printf("\tcur_tx[%d]\t: 0x%x\n", i, (cur_tx_flag[i] << OOB_CPU_PTR_FLAG_OFFSET) | cur_tx[i]);
    }
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

    osal_printf("rx_err_cnt\t: 0x%x\t", rx_err_cnt);
    osal_printf("\trx_runout_cnt\t: 0x%x\n", rx_runout_cnt);

    osal_printf("rx_alloc_fail_cnt\t: 0x%x", rx_alloc_fail_cnt);
    osal_printf("\tmb_alloc_fail_cnt\t: 0x%x\n", mb_alloc_fail_cnt);

    osal_printf("rx_fs_with_no_ls_cnt\t: 0x%x", rx_fs_with_no_ls_cnt);
    osal_printf("\trx_rcdf_rwt_cnt\t: 0x%x\n", rx_rcdf_rwt_cnt);

#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE) && (!defined(CONFIG_SDK_EXTERNAL_CPU))

    osal_printf("oob_alloc_thrd_run     : %0u \n", osal_atomic_read(&oob_alloc_thrd_run));
    osal_printf("oob_alloc_monThr_acc_cnt     : %0u \n", oob_alloc_monThr_acc_cnt);
#endif
    return RT_ERR_OK;

}

int32   dal_common_oob_stat_getAll(uint32 unit, rtk_port_t port, rtk_stat_port_cntr_t *pCntrs)
{
    int32    ret = RT_ERR_FAILED;
    RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._stat_getAll(unit, port, pCntrs), ret);
    if (oob_debug_flag & DBG_HWINFO_BIT)
    {
        RT_ERR_CHK(RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._hw_cfg_dump(unit, port), ret);
        RT_ERR_CHK(_dal_common_oob_dump_sw_database(unit, port), ret);
    }
    return ret;
}

