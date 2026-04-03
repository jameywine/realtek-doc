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
 * Purpose : Definition those public NIC(Network Interface Controller) APIs and
 *           its data type in the SDK.
 *
 * Feature : The file have include the following module and sub-modules
 *           1) CPU tag
 *           2) NIC tx
 *           3) NIC rx
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <drv/nic/nic.h>
#include <private/drv/nic/nic_diag.h>
#include <common/debug/rt_log.h>
#include <common/rt_error.h>
#include <dal/dal_mgmt.h>
#include <osal/lib.h>
#include <hwp/hw_profile.h>
#include <private/drv/nic/nic_mapper.h>
#include <private/drv/nic/nic_rtl9330.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
nic_txTagStatus_t txTagStatus;
uint8 txTag[32];
rtk_portmask_t txPortmask;
uint32  isDataAuto;
uint8   txData[JUMBO_FRAME_SIZE_MAX];
uint32  packetLen;
uint32  usr_lb_sts[RTK_MAX_NUM_OF_UNIT];

/*
 * Function Declaration
 */
#ifndef __BOOTLOADER__
extern int32 _nic_cpuTagLen_get(uint32 unit, uint32* rxTag_len, uint32* txTag_len);

void
_dummy_tx_callback(uint32 unit, drv_nic_pkt_t *pPacket, void *pCookie)
{
    return;
}

/*
 * Function Declaration
 *      drv_nic_tag_set
 * Description:
 *      Set TX tag of CPU TX packet.
 * Input:
 *      unit    - unit id
 *      tagStatus - CPU TX tag status
 *      pTxTag    - pointer to CPU TX tag, it is valid only if tagStatus is manual
 *      pPortmask    - pointer to TX portmask, it is valid only if tagStatus is auto
 * Output:
 *      None
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
 *      [SDK_3.1.1]
 *          New added function.
 */
int32
drv_nic_tag_set(uint32 unit, nic_txTagStatus_t tagStatus, uint8 *pTxTag,  rtk_portmask_t *pPortmask)
{
    uint32 rx_cTag_len, tx_cTag_len;

    if (NULL == pTxTag)
        return RT_ERR_NULL_POINTER;
    if (NULL == pPortmask)
        return RT_ERR_NULL_POINTER;

    _nic_cpuTagLen_get(unit, &rx_cTag_len, &tx_cTag_len);

    txTagStatus = tagStatus;
    osal_memcpy(txTag, pTxTag, tx_cTag_len);
    osal_memcpy(&txPortmask, pPortmask, sizeof(txPortmask));

    return RT_ERR_OK;
}   /* end of drv_nic_tag_set */

/* Function Name:
 *      drv_nic_txData_set
 * Description:
 *      Set TX data of CPU TX packet TX.
 * Input:
 *      unit    - unit id
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
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.1.1]
 *          New added function.
 */
int32
drv_nic_txData_set(uint32 unit, uint8 isAuto, uint8 *pTxData, uint32 len)
{
    uint32  bufSize;

    if (NULL == pTxData)
        return RT_ERR_NULL_POINTER;

    bufSize = (len <= sizeof(txData)) ? len : sizeof(txData);
    isDataAuto = isAuto;
    osal_memcpy(txData, pTxData, bufSize);
    packetLen = bufSize;

    return RT_ERR_OK;
}   /* end of drv_nic_txData_set */

/* Function Name:
 *      drv_nic_diagPkt_send
 * Description:
 *      Trigger to send packets from CPU.
 * Input:
 *      unit    - unit id
 *      num   - number of TX frame
 * Output:
 *      None
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
 *      [SDK_3.1.1]
 *          New added function.
 */
int32
drv_nic_diagPkt_send(uint32 unit, uint32 num)
{
#if (defined(CONFIG_SDK_RTL9310) || defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9330) || defined(CONFIG_SDK_RTL9311B))
    int32   ret, cnt = 0, i, j, retryCnt = 5000;
    drv_nic_pkt_t *pPacket;


    if (packetLen == 0)
        packetLen = 64;

    if (RT_ERR_OK != drv_nic_pkt_alloc(unit, packetLen, 0, &pPacket))
    {
        osal_printf("[%s]: Alloc packet failed.\n", __FUNCTION__);
        return RT_ERR_FAILED;
    }

    pPacket->length         = packetLen;
    pPacket->tail           = pPacket->data + packetLen;
    pPacket->txIncludeCRC   = TRUE;

    if (NIC_TXTAG_NONE == txTagStatus)
        pPacket->as_txtag       = FALSE;
    else
        pPacket->as_txtag       = TRUE;

    /* Setting CPU TX tag or payload */
    if (NIC_TXTAG_MANUAL == txTagStatus)
    {
        //osal_printf("%s():%d  txTag:%x %x %x  pPacket->data:%p\n", __FUNCTION__, __LINE__, txTag[0], txTag[1], txTag[2], pPacket->data);
        NIC_CTRL(unit).cpuTxTagFromRaw_cnvt(unit, txTag, pPacket);
    }
    else if (NIC_TXTAG_AUTO == txTagStatus)
    {
        pPacket->tx_tag.fwd_type        = NIC_FWD_TYPE_LOGICAL;
        pPacket->tx_tag.dst_port_mask   = txPortmask.bits[0];
        pPacket->tx_tag.dst_port_mask_1 = txPortmask.bits[1];
    }

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
        osal_memcpy( pPacket->data, txData, packetLen);
    }


    while (retryCnt > 0)
    {
        pPacket->data[9]    = cnt >> 16;
        pPacket->data[10]   = (cnt >> 8) & 0xff;
        pPacket->data[11]   = cnt & 0xff;
        if ((ret = drv_nic_pkt_tx(unit, pPacket, _dummy_tx_callback, NULL)))
        {
            osal_printf("%s():%d  ret:%#x\n", __FUNCTION__, __LINE__, ret);
            //break;
            retryCnt--;
            continue;
        }

        if (RTK_9330_FAMILY_ID(unit))
        {
            pPacket->head   += NIC_9330_TX_TAG_SIZE;
            pPacket->data   += NIC_9330_TX_TAG_SIZE;
            pPacket->length -= NIC_9330_TX_TAG_SIZE;
        }

        if (num != 0)
        {
            cnt++;
            if (cnt == num)
                break;
        }
    }

    drv_nic_pkt_free(unit, pPacket);
#endif

    return RT_ERR_OK;
}   /* end of drv_nic_diagPkt_send */

/* Function Name:
 *      drv_nic_loopbackMode_get
 * Description:
 *      Get the loopback mode status of user's callback example function in SDK user mode.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - enable status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 */
int32
drv_nic_loopbackMode_get(uint32 unit, rtk_enable_t *pEnable)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    *pEnable = usr_lb_sts[unit];
    return RT_ERR_OK;
}

/* Function Name:
 *      drv_nic_loopbackMode_set
 * Description:
 *      Set user's callback example function to loopback mode in SDK user mode.
 * Input:
 *      unit    - unit id
 *      enable  - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_UNIT_ID      - invalid unit id
 * Applicable:
 *      8380, 8390, 9300, 9310
 * Note:
 *      None
 */
int32
drv_nic_loopbackMode_set(uint32 unit, rtk_enable_t enable)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);

    usr_lb_sts[unit] = enable;
    return RT_ERR_OK;
}
#endif

