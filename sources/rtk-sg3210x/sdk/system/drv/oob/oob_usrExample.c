/*
 * Copyright (C) 2022 Realtek Semiconductor Corp.
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
 * Purpose : Example for init OOB user mode.
 *
 * Feature :
 *           (1) Example for inital OOB user mode.
 *           (2) Loopback test
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <drv/nic/nic.h>
#include <drv/oob/oob.h>
#include <hwp/hw_profile.h>
#include <soc/type.h>
#include <common/debug/rt_log.h>
#include <ioal/mem32.h>
#include <ioal/ioal_init.h>
#include <osal/lib.h>
#include <osal/memory.h>
#include <osal/print.h>
#include <osal/time.h>
#include <osal/sem.h>
#include <private/drv/oob/oob_usrExample.h>

/*
 * Symbol Definition
 */

#define OOB_PKTBUF_ALLOC(size)      osal_alloc(size)
#define OOB_PKTBUF_FREE(pktbuf)     osal_free(pktbuf)

//#define OOB_HEAD_RSVD               (16)
//#define OOB_PKT_MAXLEN              (1600)
//#define OOB_TAIL_RSVD               (26)
//#define OOB_BUF_LEN                 (OOB_HEAD_RSVD + OOB_PKT_MAXLEN + OOB_TAIL_RSVD)
//#define OOB_DUMP_PACKET             (1)
//#define OOB_RX_DESCRIPTOR_NUM       (16)

/*
 * Macro Definition
 */
static osal_mutex_t         oob_pkt_sem[RTK_MAX_NUM_OF_UNIT];
#define OOB_PKT_SEM_LOCK(unit)    \
do {\
    if (osal_sem_mutex_take(oob_pkt_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
    {\
        osal_printf("semaphore lock failed\n");\
    }\
} while(0)
#define OOB_PKT_SEM_UNLOCK(unit)   \
do {\
    if (osal_sem_mutex_give(oob_pkt_sem[unit]) != RT_ERR_OK)\
    {\
         osal_printf("semaphore unlock failed\n");\
    }\
} while(0)

/*
 * Data Declaration
 */

uintptr         oob_dma_vaddr;
uintptr         oob_dma_phy_addr;
uintptr         oob_pkt_vaddr;
uintptr         oob_pkt_phy_addr;
rtk_enable_t    oob_loopback_state;
rtk_enable_t    oob_dumpPkt_state;

static uint32 oob_pktBuf_stat[RTK_MAX_NUM_OF_UNIT][OOB_ALL_DESCRIPTOR_NUM];


static int32 rtnic_oob_pkt_alloc(uint32 unit, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    drv_nic_pkt_t *pPacket = NULL;
    uint8 *pBuf             = NULL;
    uint32        buf_id;

    RT_PARAM_CHK(NULL == ppPacket, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((size > JUMBO_FRAME_SIZE_MAX), RT_ERR_INPUT);

    pPacket = osal_alloc(sizeof(drv_nic_pkt_t));
    if (pPacket == NULL)
    {
        osal_printf("[%s] Malloc pPacket fail.",__FUNCTION__);
        return RT_ERR_MEM_ALLOC;
    }

    osal_memset(pPacket, 0, sizeof(drv_nic_pkt_t));
    OOB_PKT_SEM_LOCK(unit);
    for(buf_id = OOB_TX_DESC_START_INDEX; buf_id <= OOB_TX_DESC_END_INDEX; buf_id++)
    {
        if(FALSE == oob_pktBuf_stat[unit][buf_id])
        {
            oob_pktBuf_stat[unit][buf_id] = TRUE;
            break;
        }
    }

    if ((OOB_TX_DESC_END_INDEX + 1) == buf_id)
    {
        osal_printf("%s():%d  run out of memory for TX normal packet\n", __FUNCTION__, __LINE__);
        OOB_PKT_SEM_UNLOCK(unit);
        osal_free(pPacket);
        return RT_ERR_MEM_ALLOC;
    }
    OOB_PKT_SEM_UNLOCK(unit);

    pBuf = (uint8 *)(oob_pkt_vaddr + (OOB_BUF_LEN * buf_id));

    pPacket->head = (uint8 *)pBuf;
    pPacket->data = (uint8 *)(pPacket->head + OOB_HEAD_RSVD);
    pPacket->tail = (uint8 *)(pPacket->head + OOB_PKT_MAXLEN);
    pPacket->end = (uint8 *)(pPacket->head + OOB_PKT_MAXLEN);
    pPacket->length = 0;
    pPacket->buf_id = (void *)(uintptr)buf_id;
    pPacket->next = NULL;

    *ppPacket = pPacket;
    return RT_ERR_OK;
} /* end of rtnic_pkt_alloc */

static int32 rtnic_oob_rx_pkt_alloc(uint32 unit, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    drv_nic_pkt_t *pPacket  = NULL;
    uint8 *pBuf             = NULL;
    uint32        buf_id;

    RT_PARAM_CHK(NULL == ppPacket, RT_ERR_NULL_POINTER);
    RT_PARAM_CHK((size > JUMBO_FRAME_SIZE_MAX), RT_ERR_INPUT);

    pPacket = osal_alloc(sizeof(drv_nic_pkt_t));
    if (pPacket == NULL)
    {
        osal_printf("[%s] Malloc pPacket fail.",__FUNCTION__);
        return RT_ERR_MEM_ALLOC;
    }

    osal_memset(pPacket, 0, sizeof(drv_nic_pkt_t));

    OOB_PKT_SEM_LOCK(unit);
    for(buf_id = OOB_RX_DESC_START_INDEX; buf_id <= OOB_RX_DESC_END_INDEX; buf_id++)
    {
        if(FALSE == oob_pktBuf_stat[unit][buf_id])
        {
            oob_pktBuf_stat[unit][buf_id] = TRUE;
            break;
        }
    }
    if (OOB_RX_DESCRIPTOR_NUM == buf_id)
    {
        osal_printf("%s():%d  run out of memory for RX normal packet\n", __FUNCTION__, __LINE__);
        OOB_PKT_SEM_UNLOCK(unit);
        osal_free(pPacket);
        return RT_ERR_MEM_ALLOC;
    }

    OOB_PKT_SEM_UNLOCK(unit);

    pBuf = (uint8 *)(oob_pkt_vaddr + (OOB_BUF_LEN * buf_id));

    pPacket->head = (uint8 *)pBuf;
    pPacket->data = (uint8 *)(pPacket->head + OOB_HEAD_RSVD);
    pPacket->tail = (uint8 *)(pPacket->head + OOB_PKT_MAXLEN);
    pPacket->end = (uint8 *)(pPacket->head + OOB_PKT_MAXLEN);
    pPacket->length = 0;
    pPacket->buf_id = (void *)(uintptr)buf_id;
    pPacket->next = NULL;

    *ppPacket = pPacket;
    return RT_ERR_OK;
} /* end of rtnic_oob_rx_pkt_alloc */


/* Function Name:
 *      rtnic_oob_pkt_free
 * Description:
 *      free allocated packet for drv_nic_pkt_t
 * Input:
 *      unit    - unit id
 *      pPacket - pointer buffer to the packet
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
static int32 rtnic_oob_pkt_free(uint32 unit, drv_nic_pkt_t *pPacket)
{
    uint32  buf_id;

    if (NULL == pPacket)
    {
        osal_printf("\n[%s] pPacket is NULL.",__FUNCTION__);
        return RT_ERR_FAILED;
    }

    buf_id = (uint32)(uintptr)pPacket->buf_id;

    OOB_PKT_SEM_LOCK(unit);

    if (FALSE == oob_pktBuf_stat[unit][buf_id])
    {
        osal_printf("Error: NULL at %s():%d, buf_id = %d\n", __FUNCTION__, __LINE__, buf_id);
        return RT_ERR_FAILED;
    }
    oob_pktBuf_stat[unit][buf_id] = FALSE;

    OOB_PKT_SEM_UNLOCK(unit);

    osal_free(pPacket);
    return RT_ERR_OK;
} /* end of rtnic_oob_pkt_free */

/* Function Name:
 *      rtnic_oob_tx_callback
 * Description:
 *      packet TX callback function
 *
 * Input:
 *      unit    - unit id
 *      pPacket - pointer buffer to the packet
 *      pCookie - cookie data buffer
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtnic_oob_tx_callback(uint32 unit, drv_nic_pkt_t *pPacket, void *pCookie)
{
    if (NULL == pPacket)
    {
        osal_printf("\n[%s] pPacket is NULL.",__FUNCTION__);
        return;
    }

    if ((rtnic_oob_pkt_free(unit, pPacket)) != RT_ERR_OK)
    {
        osal_printf("\n[%s] rtnic_oob_pkt_free is failed.",__FUNCTION__);
        return;
    }
    return;
} /* end of rtnic_oob_tx_callback */

/* Function Name:
 *      rtnic_oob_rx_callback
 * Description:
 *      packet RX callback function
 * Input:
 *      unit    - unit id
 *      pPacket - pointer buffer to the packet
 *      pCookie - cookie data buffer
 * Output:
 *      None
 * Return:
 *      NIC_RX_HANDLED_OWNED
 *      NIC_RX_NOT_HANDLED
 * Note:
 *      None
 */

drv_nic_rx_t rtnic_oob_rx_callback(uint32 unit, drv_nic_pkt_t *pPacket, void *pCookie)
{
    uint32          dump_loop;
    uint32          oob_port;
    drv_nic_pkt_t   *pTx_packet = NULL;
    int32           retval;

    if (NULL == pPacket)
    {
        osal_printf("\n[%s] pPacket is NULL.",__FUNCTION__);
        return RT_ERR_FAILED;
    }

    if(oob_dumpPkt_state == ENABLED)
    {
        osal_printf("\nDUMP PACKET, len = %d:", pPacket->length);
        for(dump_loop = 0; dump_loop < pPacket->length; dump_loop++)
        {
            if((dump_loop % 16) == 0)
                osal_printf("\n");
            osal_printf("0x%02x ", *(uint8 *)(pPacket->data + (dump_loop)));
        }
        osal_printf("\n");
    }

    if(oob_loopback_state == ENABLED)
    {
        HWP_OOB_PORT_TRAVS(unit, oob_port)
        {
            if ((retval = rtnic_oob_pkt_alloc(unit, OOB_PKT_MAXLEN, PKTBUF_TX, &pTx_packet)) != RT_ERR_OK)
            {
                osal_printf("\n[%s] Can't allocate TX packet buffer.\n",__FUNCTION__);
                goto _exit;
            }

            osal_memcpy(pTx_packet->data, pPacket->data, pPacket->length);
            pTx_packet->tail = pTx_packet->data + pPacket->length;
            pTx_packet->length = pPacket->length;

            if (RT_ERR_OK != drv_oob_pkt_tx(unit, oob_port, pTx_packet, rtnic_oob_tx_callback, (void *)NULL))
            {
                if (RT_ERR_OK != rtnic_oob_pkt_free(unit, pTx_packet))
                {
                    osal_printf("Error: Can't free TX packet buffer %s():%d, buf_id = %d\n", __FUNCTION__, __LINE__, (uint32)(uintptr)(pTx_packet->buf_id));
                    goto _exit;
                }
            }
        }
    }

    if (RT_ERR_OK != rtnic_oob_pkt_free(unit, pPacket))
    {
        osal_printf("Error: Can't free RX packet buffer %s():%d, buf_id = %d\n", __FUNCTION__, __LINE__, (uint32)(uintptr)(pPacket->buf_id));
        goto _exit;
    }

    return NIC_RX_HANDLED_OWNED;


_exit:
    return NIC_RX_NOT_HANDLED;
} /* end of rtnic_rx_callback */

/* Function Name:
 *      rtnic_oob_diag_loopback_set
 * Description:
 *      Configure loopback mode
 * Input:
 *      unit        - unit id
 *      enable      - enable/disabled loopback
 *      dump_enable - enable/disabled dump rx packet
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 */
int32
rtnic_oob_diag_loopback_set(uint32 unit, rtk_enable_t enable, rtk_enable_t dump_enable)
{
    oob_loopback_state = enable;
    oob_dumpPkt_state = dump_enable;

    return  RT_ERR_OK;
}

void rtnic_oob_pktBuf_init(void)
{
    uint32 unit_idx, desc_inx;

    for(unit_idx = 0; unit_idx < RTK_MAX_NUM_OF_UNIT; unit_idx++)
    {
        for(desc_inx = 0; desc_inx < OOB_ALL_DESCRIPTOR_NUM; desc_inx++)
        {
            oob_pktBuf_stat[unit_idx][desc_inx] = FALSE;
        }
        /* Create mutex semaphore */
        oob_pkt_sem[unit_idx] = osal_sem_mutex_create();
    }
}

/* Function Name:
 *      rtnic_oob_init
 * Description:
 *      Example for init oob user mode
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      -ENOMEM
 * Note:
 *      None
 */
int32 rtnic_oob_init(void)
{
    drv_nic_initCfg_t   initcfg;
    uint32              unit;
    uint32              oob_port;
    int32               ret;
    rtk_enable_t        portStatus;

    unit = HWP_MY_UNIT_ID();

    initcfg.rx_pkt_alloc   = (drv_nic_pkt_alloc_f)rtnic_oob_rx_pkt_alloc;
    initcfg.pkt_alloc = (drv_nic_pkt_alloc_f)rtnic_oob_pkt_alloc;
    initcfg.pkt_free = (drv_nic_pkt_free_f)rtnic_oob_pkt_free;
    initcfg.pkt_size = NML_FRAME_SIZE_MAX;
    initcfg.jumbo_size  = JUMBO_FRAME_SIZE_MAX;

    /* Get packet buffer start address that is mapped to user space */
    ioal_init_memRegion_get(unit, IOAL_MEM_DMA_OOB, &oob_dma_vaddr);
    ioal_init_memRegion_get(unit, IOAL_MEM_DMA_PHY_OOB, &oob_dma_phy_addr);

    rtnic_oob_pktBuf_init();

    oob_loopback_state = DISABLED;
    oob_dumpPkt_state = DISABLED;

    HWP_OOB_PORT_TRAVS(unit, oob_port)
    {
         if (RT_ERR_OK != (ret = drv_oob_desc_init(unit, oob_port, oob_dma_vaddr, oob_dma_phy_addr)))
         {
             RT_INIT_ERR(ret, (MOD_OOB), "rtnic_oob_init() fail\n");
             return RT_ERR_FAILED;
         }

         RT_ERR_CHK(drv_oob_pktBuf_baseAdddr_get(unit, oob_port, &oob_pkt_vaddr, &oob_pkt_phy_addr), ret);
         RT_ERR_CHK(drv_oob_init(unit, oob_port, &initcfg), ret);
         RT_ERR_CHK(drv_oob_rx_register(unit, oob_port, NIC_RX_CB_PRIORITY_MAX, rtnic_oob_rx_callback, NULL, 0), ret);

          ret = drv_oob_phy_connect(unit, oob_port);
          if(ret != RT_ERR_OK)
          {
              osal_printf("\n[%s][%d] unit=%d, oob_port=%d\n",__FUNCTION__,__LINE__,unit,oob_port);
              return -RT_ERR_FAILED;
          }

          portStatus = ENABLED;
          ret = drv_oob_phyEnable_set(unit, oob_port, portStatus);

          if(ret != RT_ERR_OK)
          {
              osal_printf("\n[%s][%d] unit=%d, oob_port=%d\n",__FUNCTION__,__LINE__,unit,oob_port);
              return -RT_ERR_FAILED;
          }
      }
      return  RT_ERR_OK;
}




