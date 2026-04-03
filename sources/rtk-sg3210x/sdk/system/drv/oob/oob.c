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
 * Purpose : Definition those public Out-of-Band Management port APIs and its data type in the SDK.
 *
 * Feature : The file have include the following module and sub-modules
 *           (1) Out-of-Band Management port
 */

/*
 * Include Files
 */

#include <common/rt_autoconf.h>
#include <common/type.h>
#include <dev_config.h>
#include <drv/oob/oob.h>

#include <soc/type.h>
#include <private/drv/swcore/chip_probe.h>
#include <private/drv/oob/oob_mapper.h>
#include <private/drv/oob/dal_common_oob.h>
#include <private/drv/nic/nic_diag.h>
#include <drv/nic/nic.h>
#include <common/rt_type.h>
#include <common/debug/rt_log.h>
#include <osal/lib.h>
#include <osal/isr.h>
#include <osal/memory.h>
#include <osal/cache.h>
#include <osal/sem.h>


/*
 * Symbol Definition
 */



/*
 * Data Type Definition
 */



/*
 * Data Declaration
 */



/*
 * Function Declaration
 */

int32 drv_oob_mac_init(uint32 unit, rtk_port_t port)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)]._mac_init(unit, port);
}

/* Function Name:
 *      drv_oob_txData_set
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
drv_oob_txData_set(uint32 unit, rtk_port_t port, uint8 isAuto, uint8 *pTxData, uint32 len)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);
    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].txData_set(unit, port, isAuto, pTxData, len);
}
/* Function Name:
 *      drv_oob_diagPkt_send
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
drv_oob_diagPkt_send(uint32 unit, rtk_port_t port, uint32 num)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);
    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].diagPkt_send(unit, port, num);
}

/* Function Name:
 *      drv_oob_init
 * Description:
 *      Initialize oob module of the specified device.
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
 *      9311B
 * Note:
 *      Must initialize oob module before calling any oob APIs.
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_init(uint32 unit, rtk_port_t port, drv_nic_initCfg_t *pInitCfg)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].init(unit, port, pInitCfg);
}

/* Function Name:
 *      drv_oob_exit
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
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_exit(uint32 unit, rtk_port_t port)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].exit(unit, port);
}

/* Function Name:
 *      drv_oob_desc_init
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
 *      None
 */
int32
drv_oob_desc_init(uint32 unit, rtk_port_t port, uintptr vir_base, uintptr phy_base)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].desc_init(unit, port, vir_base, phy_base);
}

#if defined(CONFIG_SDK_KERNEL_LINUX_USER_MODE)
/* Function Name:
 *      drv_oob_pktBuf_get
 * Description:
 *      Get packet buffer start address in user mode.
 * Input:
 *      unit     - unit id
 *      pVAddr  - pointer to virtual address
 *      pPAddr  - pointer to physical address
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 * Applicable:
 *      9311b, 9330
 * Note:
 *      None
 * Changes:
 *      None
 */
int32
drv_oob_pktBuf_baseAdddr_get(uint32 unit, rtk_port_t port, uintptr* pVAddr, uintptr* pPAddr)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pktBuf_baseAdddr_get(unit, port, pVAddr, pPAddr);
}
#endif

/* Function Name:
 *      drv_oob_pkt_tx
 * Description:
 *      Transmit a packet via oob of the specified device.
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
 *      9311B
 * Note:
 *      When fTxCb is NULL, driver will free packet and not callback any more.
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_pkt_tx(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket, drv_nic_tx_cb_f fTxCb, void *pCookie)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pkt_tx(unit, port, pPacket, fTxCb, pCookie);
}

/* Function Name:
 *      drv_oob_rx_status_get
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
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_rx_status_get(uint32 unit, rtk_port_t port, uint32 *pStatus)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].oobEnable_get(unit, port, OOB_DIR_RX, pStatus);
}

/* Function Name:
 *      drv_oob_rx_register
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
 *      9311B
 * Note:
 *      If flags have turn on the NIC_FLAG_RX_CRC_INCLUDE flag, means that asking packet
 *      handed to upper layer should include CRC.
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_rx_register(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb, void *pCookie, uint32 flags)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].rx_register(unit, port, priority, fRxCb, pCookie, flags);
}

/* Function Name:
 *      drv_oob_rx_unregister
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
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_rx_unregister(uint32 unit, rtk_port_t port, uint8 priority, drv_nic_rx_cb_f fRxCb)
{
    /* Check arguments */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].rx_unregister(unit, port, priority, fRxCb);
}


/* Function Name:
 *      drv_oob_pkt_alloc
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
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_pkt_alloc(uint32 unit, rtk_port_t port, int32 size, uint32 flags, drv_nic_pkt_t **ppPacket)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    /* Dispatch */
    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pkt_alloc(unit, port, size, flags, ppPacket);
}


/* Function Name:
 *      drv_oob_pkt_free
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
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_pkt_free(uint32 unit, rtk_port_t port, drv_nic_pkt_t *pPacket)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);
    /* Check arguments */
    RT_PARAM_CHK(NULL == pPacket, RT_ERR_NULL_POINTER);

    /* Dispatch */
    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pkt_free(unit, port, pPacket);

}



/* Function Name:
 *      drv_oob_reset
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
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_reset(uint32 unit, rtk_port_t port)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].reset(unit, port);
}

/* Function Name:
 *      drv_oob_dbg_get
 * Description:
 *      Get OOB debug flags of the specified device.
 * Input:
 *      unit   - unit id
 * Output:
 *      pFlags - OOB debug flags
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B
 * Note:
 *      There are 4 BIT flags as following:
 *      - DEBUG_RX_RAW_LEN_BIT
 *      - DEBUG_RX_CPU_TAG_BIT
 *      - DEBUG_TX_RAW_LEN_BIT
 *      - DEBUG_TX_CPU_TAG_BIT
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_dbg_get(uint32 unit, rtk_port_t port, uint32 *pFlags)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].dbg_get(unit, port, pFlags);
}

/* Function Name:
 *      drv_oob_dbg_set
 * Description:
 *      Set OOB debug flags of the specified device.
 * Input:
 *      unit  - unit id
 *      flags - debug flags
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B
 * Note:
 *      There are 4 BIT flags can be selected as following:
 *      - DEBUG_RX_RAW_LEN_BIT
 *      - DEBUG_RX_CPU_TAG_BIT
 *      - DEBUG_TX_RAW_LEN_BIT
 *      - DEBUG_TX_CPU_TAG_BIT
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_dbg_set(uint32 unit, rtk_port_t port, uint32 flags)
{
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].dbg_set(unit, port, flags);
}


/* Function Name:
 *      drv_oob_mgmtMacAddr_get
 * Description:
 *      Get MAC address of OOB
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pMac - management MAC address
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_mgmtMacAddr_get(uint32 unit, rtk_port_t port, rtk_mac_t *pMac)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].mgmtMacAddr_get(unit, port, pMac);
    return ret;
}

/* Function Name:
 *      drv_oob_mgmtMacAddr_set
 * Description:
 *      Set MAC address of OOB
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      pMac - management MAC address
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_mgmtMacAddr_set(uint32 unit, rtk_port_t port, rtk_mac_t *pMac)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].mgmtMacAddr_set(unit, port, pMac);
    return ret;
}


/* Function Name:
 *      drv_oob_rxMode_get
 * Description:
 *      Configure receive mode
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      rx_mode - receive mode
 * Output:
 *      pEnable - enable/disabled specified mode
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_rxMode_get(uint32 unit, rtk_port_t port, drv_oob_rxMode_t rx_mode, rtk_enable_t *pEnable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].rxMode_get(unit, port, rx_mode, pEnable);
    return ret;
}

/* Function Name:
 *      drv_oob_rxMode_set
 * Description:
 *      Configure receive mode
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      rx_mode - receive mode
 *      enable - enable/disabled specified mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_rxMode_set(uint32 unit, rtk_port_t port, drv_oob_rxMode_t rx_mode, rtk_enable_t enable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].rxMode_set(unit, port, rx_mode, enable);
    return ret;
}

/* Function Name:
 *      drv_oob_jumboModeEnable_get
 * Description:
 *      Configure MAC of supporting jumbo frame
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      dir - TX or RX
 * Output:
 *      pEnable - ENABLED to support jumbo frame
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_jumboModeEnable_get(uint32 unit, rtk_port_t port, rtk_switch_pktDir_t dir, rtk_enable_t *pEnable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].jumboModeEnable_get(unit, port, dir, pEnable);
    return ret;
}

/* Function Name:
 *      drv_oob_jumboModeEnable_set
 * Description:
 *      Configure MAC of supporting jumbo frame
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      dir - TX or RX
 *      enable - ENABLED to support jumbo frame
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_jumboModeEnable_set(uint32 unit, rtk_port_t port, rtk_switch_pktDir_t dir, rtk_enable_t enable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].jumboModeEnable_set(unit, port, dir, enable);
    return ret;
}

/* Function Name:
 *      drv_oob_pollingPhyEnable_get
 * Description:
 *      Get MAC auto polling PHY PCS status configuration.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pEnable - enable/disable MAC auto polling PHY.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_pollingPhyEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pollingPhyEnable_get(unit, port, pEnable);
    return ret;
}

/* Function Name:
 *      drv_oob_pollingPhyEnable_set
 * Description:
 *      Configure MAC to auto polling PHY PCS status.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      enable - enable/disable MAC auto polling PHY.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_pollingPhyEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].pollingPhyEnable_set(unit, port, enable);
    return ret;
}


/* Function Name:
 *      drv_oob_forceFc_set
 * Description:
 *      Force MAC flow control status.
 *      When PHY's local pause is 0 and link partner's pause is 0,
 *      this API is able to configure MAC to tx/rx pause.
 *      Disable forceFc to will use polling PHY result.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pForceFc - ENABLED: force flow control.
 *                DISABLED: disable force flow control, MAC will use polling PHY status to tx/rx pause.
 *      pTxPause - enable/disable TX pause frame
 *      pRxPause - enable/disable RX pause frame
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceFc_get(uint32 unit, rtk_port_t port, rtk_enable_t *pForceFc, rtk_enable_t *pTxPause, rtk_enable_t *pRxPause)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceFc_get(unit, port, pForceFc, pTxPause, pRxPause);
    return ret;
}

/* Function Name:
 *      drv_oob_forceFc_set
 * Description:
 *      Force MAC flow control status.
 *      When PHY's local pause is 0 and link partner's pause is 0,
 *      this API is able to configure MAC to tx/rx pause.
 *      Disable forceFc to will use polling PHY result.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      forceFc - ENABLED: force flow control.
 *                DISABLED: disable force flow control, MAC will use polling PHY status to tx/rx pause.
 *      txPause - enable/disable TX pause frame
 *      rxPause - enable/disable RX pause frame
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceFc_set(uint32 unit, rtk_port_t port, rtk_enable_t forceFc, rtk_enable_t txPause, rtk_enable_t rxPause)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceFc_set(unit, port, forceFc, txPause, rxPause);
    return ret;
}

/* Function Name:
 *      drv_oob_forceFullDuplex_get
 * Description:
 *      Get force MAC in full duplex mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pEnable - ENABLED, force MAC in full duplex mode.
 *               DISABLED, duplex status will get from polling PHY status.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceFullDuplex_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceFullDuplex_get(unit, port, pEnable);
    return ret;
}

/* Function Name:
 *      drv_oob_forceFullDuplex_set
 * Description:
 *      Force MAC in full duplex mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      enable - ENABLED, force MAC in full duplex mode.
 *               DISABLED, duplex status will get from polling PHY status.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceFullDuplex_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceFullDuplex_set(unit, port, enable);
    return ret;
}

/* Function Name:
 *      drv_oob_forceSpeed_get
 * Description:
 *      Get force MAC speed configuration.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pForceSpd - ENABLED, enable force MAC speed.
 *                 DISABLED, disable force speed, speed will obtain from polling PHY status.
 *      pSpeed - configure speed when forceSpd is ENABLED.
 *              When DISABLED, will always return PORT_SPEED_10M.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceSpeed_get(uint32 unit, rtk_port_t port, rtk_enable_t *pForceSpd, rtk_port_speed_t *pSpeed)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceSpeed_get(unit, port, pForceSpd, pSpeed);
    return ret;
}

/* Function Name:
 *      drv_oob_forceSpeed_set
 * Description:
 *      Force MAC speed.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      forceSpd - ENABLED, enable force MAC speed.
 *                 DISABLED, disable force speed, speed will obtain from polling PHY status.
 *      speed - configure speed when forceSpd is ENABLED.
 *              When DISABLED, this field will ignore, please use PORT_SPEED_10M.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceSpeed_set(uint32 unit, rtk_port_t port, rtk_enable_t forceSpd, rtk_port_speed_t speed)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceSpeed_set(unit, port, forceSpd, speed);
    return ret;
}

/* Function Name:
 *      drv_oob_forceLink_get
 * Description:
 *      Get force MAC in link up mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pEnable - ENABLED, force MAC in link up mode.
 *                DISABLED, the MAC's link status is from polling PHY status.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceLink_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceLink_get(unit, port, pEnable);
    return ret;
}

/* Function Name:
 *      drv_oob_forceLink_set
 * Description:
 *      Force MAC in link up mode.
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      enable - ENABLED, force MAC in link up mode.
 *               DISABLED, the MAC's link status is from polling PHY status.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_forceLink_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].forceLink_set(unit, port, enable);
    return ret;
}


/* Function Name:
 *      drv_oob_stat_getAll
 * Description:
 *      Get all counters in the specified device.
 * Input:
 *      unit - unit id
 *      port - OOB port ID. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pCntrs - statistic counter
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
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
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_stat_getAll(uint32 unit, rtk_port_t port, rtk_stat_port_cntr_t *pCntrs)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].stat_getAll(unit, port, pCntrs);
    return ret;
}

/* Function Name:
 *      drv_oob_miiBus_read
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit - unit id
 *      mii_bus - bus ID. Using RTK_OOB_IDX_TO_SMI(N) for bus id. N starts from 0.
 *      phy_addr - address of the PHY
 *      phy_reg - PHY register (0 ~ 31)
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_miiBus_read(uint32 unit, uint32 mii_bus, uint32 phy_addr, uint32 phy_reg, uint32 *pDdata)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((mii_bus < RTK_OOB_SMI_START) || (mii_bus > RTK_OOB_SMI_END)), RT_ERR_OUT_OF_RANGE);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_SMI_TO_IDX(mii_bus)].miiBus_read(unit, mii_bus, phy_addr, phy_reg, pDdata);
    return ret;
}

/* Function Name:
 *      drv_oob_miiBus_write
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit - unit id
 *      mii_bus - bus ID. Using RTK_OOB_IDX_TO_SMI(N) for bus id. N starts from 0.
 *      phy_addr - address of the PHY
 *      phy_reg - PHY register (0 ~ 31)
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_miiBus_write(uint32 unit, uint32 mii_bus, uint32 phy_addr, uint32 phy_reg, uint32 data)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((mii_bus < RTK_OOB_SMI_START) || (mii_bus > RTK_OOB_SMI_END)), RT_ERR_OUT_OF_RANGE);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_SMI_TO_IDX(mii_bus)].miiBus_write(unit, mii_bus, phy_addr, phy_reg, data);
    return ret;
}

/* Function Name:
 *      drv_oob_sdsReg_get
 * Description:
 *      Get SerDes register through indirect access
 * Input:
 *      unit    - unit id
 *      sds     - SerDes ID. Using RTK_OOB_IDX_TO_SDS(N) for serdes ID, N starts from 0
 *      page    - page
 *      reg     - register index/address
 * Output:
 *      pData   - register value
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_sdsReg_get(uint32 unit, uint32 sds, uint32 page, uint32 reg, uint32 *pData)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((sds < RTK_OOB_SDS_START) || (sds > RTK_OOB_SDS_END)), RT_ERR_OUT_OF_RANGE);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_SDS_TO_IDX(sds)].sdsReg_get(unit, sds, page, reg, pData);
    return ret;
}

/* Function Name:
 *      drv_oob_sdsReg_set
 * Description:
 *      Set SerDes register through indirect access
 * Input:
 *      unit    - unit id
 *      sds     - SerDes ID. Using RTK_OOB_IDX_TO_SDS(N) for serdes ID, N starts from 0
 *      page    - page
 *      reg     - register index/address
 *      data    - register value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_sdsReg_set(uint32 unit, uint32 sds, uint32 page, uint32 reg, uint32 data)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((sds < RTK_OOB_SDS_START) || (sds > RTK_OOB_SDS_END)), RT_ERR_OUT_OF_RANGE);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_SDS_TO_IDX(sds)].sdsReg_set(unit, sds, page, reg, data);
    return ret;
}

/* Function Name:
 *      drv_oob_phy_connect
 * Description:
 *      Probe PHY if PHY driver is found, init PHY and config MAC accorfing to PHY ability(e.g. polling-PHY)
 * Input:
 *      unit - unit id
 *      port - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phy_connect(uint32 unit, rtk_port_t port)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phy_connect(unit, port);
    return ret;
}

/* Function Name:
 *      drv_oob_linkMon_enable
 * Description:
 *      Enable link monitor thread
 * Input:
 *      unit    - unit id
 *      port    - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      scan_interval_us        - scan interval in us.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_THREAD_EXIST - The LinkMon thread is created already, and it's work.
 *      RT_ERR_THREAD_CREATE_FAILED - The LinkMon thread created failed.
 * Applicable:
 *      9311B
 * Note:
 *      When enable link monitor thread, all link change interrupt will be handled by thread.
 *      Link monitor thread is per-system thread, no matter the unit/port value is, it set to the same thread.
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_linkMon_enable(uint32 unit, rtk_port_t port, uint32 scan_interval_us)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].linkMon_enable(unit, port, scan_interval_us);
    return ret;
}

/* Function Name:
 *      drv_oob_linkMon_disable
 * Description:
 *      Disable link scan thread
 * Input:
 *      unit    - unit id
 *      port    - OOB port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - initialize success
 *      RT_ERR_FAILED   - initialize fail
 *      RT_ERR_NOT_INIT - The module is not initial
 * Applicable:
 *      9311B
 * Note:
 *      When disable link monitor thread, all link change interrupt will be callback to upper layer.
 *      Link monitor thread is per-system thread, no matter the unit/port value is, it set to the same thread.
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_linkMon_disable(uint32 unit, rtk_port_t port)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].linkMon_disable(unit, port);
    return ret;
}

/* Function Name:
 *      drv_oob_linkMonSwScanEnable_get
 * Description:
 *      Get software linkscan for certain unit
 * Input:
 *      unit - unit id
 *      port - OOB port ID. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pSwScan_portmask    - portmask for software scan
 * Return:
 *      RT_ERR_OK           - initialize success
 *      RT_ERR_FAILED       - initialize fail
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_linkMonSwScanEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].linkMonSwScanEnable_get(unit, port, pEnable);
    return ret;
}

/* Function Name:
 *      drv_oob_linkMonSwScanEnable_set
 * Description:
 *      Configure software linkscan for certain unit
 * Input:
 *      unit - unit id
 *      port - OOB port ID. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      enable - enable software link scan for the port
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - initialize success
 *      RT_ERR_FAILED       - initialize fail
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_linkMonSwScanEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].linkMonSwScanEnable_set(unit, port, enable);
    return ret;
}

/* Function Name:
 *      drv_oob_linkMon_register
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
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_linkMon_register(uint32 unit, rtk_port_t port, rtk_port_linkMon_callback_t linkMon_callback)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].linkMon_register(unit, port, linkMon_callback);
    return ret;
}

/* Function Name:
 *      drv_oob_linkMon_unregister
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
 * Applicable:
 *      9311B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function. */
int32
drv_oob_linkMon_unregister(uint32 unit, rtk_port_t port, rtk_port_linkMon_callback_t linkMon_callback)
{
    int32 ret;
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    ret = RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].linkMon_unregister(unit, port, linkMon_callback);
    return ret;
}

/* Function Name:
 *      drv_oob_phyAutoNegoEnable_get
 * Description:
 *      Get PHY ability of the specific port
 * Input:
 *      unit    - unit id
 *      port    - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pEnable - pointer to PHY auto negotiation status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8213B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyAutoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyAutoNegoEnable_get(unit, port, pEnable);
}   /* end of drv_oob_phyAutoNegoEnable_get */

/* Function Name:
 *      drv_oob_phyAutoNegoEnable_set
 * Description:
 *      Set PHY ability of the specific port
 * Input:
 *      unit   - unit id
 *      port   - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      enable - enable PHY auto negotiation
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - input parameter out of range
 * Applicable:
 *      8213B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyAutoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyAutoNegoEnable_set(unit, port, enable);
}   /* end of drv_oob_phyAutoNegoEnable_set */

/* Function Name:
 *      drv_oob_phyAutoNegoAbility_get
 * Description:
 *      Get PHY auto negotiation ability of the specific port
 * Input:
 *      unit    - unit id
 *      port    - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pAbility - pointer to the PHY ability
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8213B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyAutoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyAutoNegoAbility_get(unit, port, pAbility);
}   /* end of drv_oob_phyAutoNegoAbility_get */

/* Function Name:
 *      drv_oob_phyAutoNegoAbility_set
 * Description:
 *      Set PHY auto negotiation ability of the specific port
 * Input:
 *      unit     - unit id
 *      port     - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      pAbility - pointer to the PHY ability
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8213B
 * Note:
 *      You can set these abilities no matter which mode PHY currently stays on
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyAutoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyAutoNegoAbility_set(unit, port, pAbility);
}   /* end of drv_oob_phyAutoNegoAbility_set */

/* Function Name:
 *      drv_oob_phyEnable_get
 * Description:
 *      Get link speed status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pSpeed - pointer to PHY link speed
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8213B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyEnable_get(unit, port, pEnable);
}   /* end of drv_oob_phyEnable_get */

/* Function Name:
 *      drv_oob_phyEnable_set
 * Description:
 *      Set PHY interface status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      enable        - admin configuration of PHY interface
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8213B
 * Note:
 *      You can set these abilities no matter which mode PHY currently stays on
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyEnable_set(unit, port, enable);
}   /* end of drv_oob_phyEnable_set */

/* Function Name:
 *      drv_oob_phySpeed_get
 * Description:
 *      Get speed mode of the specific port
 *      If autoneg is enabled, then operating mode is returned, otherwise forced mode is returned.
 * Input:
 *      unit - unit id
 *      port - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pSpeed - pointer to PHY link speed
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8213B
 * Note:
 *      You can set these abilities no matter which mode PHY currently stays on
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phySpeed_get(uint32 unit, rtk_port_t port, rtk_port_speed_t *pSpeed)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phySpeed_get(unit, port, pSpeed);
}   /* end of drv_oob_phySpeed_get */

/* Function Name:
 *      drv_oob_phySpeed_set
 * Description:
 *      Set (force) speed mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      speed         - link speed status 10/100/1000
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8213B
 * Note:
 *      You can set these abilities no matter which mode PHY currently stays on
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phySpeed_set(uint32 unit, rtk_port_t port, rtk_port_speed_t speed)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phySpeed_set(unit, port, speed);
}   /* end of drv_oob_phySpeed_set */

/* Function Name:
 *      drv_oob_phyDuplex_get
 * Description:
 *      Get duplex mode of the specific port
 *      If autoneg is enabled, then operating mode is returned, otherwise forced mode is returned.
 * Input:
 *      unit - unit id
 *      port - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pDuplex - pointer to PHY duplex mode status
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8213B
 * Note:
 *      You can set these abilities no matter which mode PHY currently stays on
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyDuplex_get(uint32 unit, rtk_port_t port, rtk_port_duplex_t *pDuplex)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyDuplex_get(unit, port, pDuplex);
}   /* end of drv_oob_phyDuplex_get */

/* Function Name:
 *      drv_oob_phyDuplex_set
 * Description:
 *      Set  (force) duplex mode of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      duplex        - duplex mode of the port, full or half
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8213B
 * Note:
 *      You can set these abilities no matter which mode PHY currently stays on
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyDuplex_set(uint32 unit, rtk_port_t port, rtk_port_duplex_t duplex)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyDuplex_set(unit, port, duplex);
}   /* end of drv_oob_phyDuplex_set */

/* Function Name:
 *      drv_oob_phyLink_get
 * Description:
 *      Get PHY link status from standard register (1.2).
 * Input:
 *      unit    - unit id
 *      port    - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pLink - pointer to the link status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8213B
 * Note:
 *      The Link Status bit (Status Register 1.2) has LL (Latching Low) attribute
 *      for link failure. Please refer IEEE 802.3 for detailed.
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyLink_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pLink)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyLink_get(unit, port, pLink);
}   /* end of drv_oob_phyLink_get */

/* Function Name:
 *      drv_oob_phyEeeEnable_get
 * Description:
 *      Get enable status of EEE function in the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      pEnable - pointer to enable status of EEE
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      8213B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyEeeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyEeeEnable_get(unit, port, pEnable);
}   /* end of drv_oob_phyEeeEnable_get */

/* Function Name:
 *      drv_oob_phyEeeEnable_set
 * Description:
 *      Set enable status of EEE function in the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 *      enable - enable status of EEE
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      8213B
 * Note:
 *      Needs to restart auto-negotiation for the configuration to take effect.
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phyEeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phyEeeEnable_set(unit, port, enable);
}   /* end of drv_oob_phyEeeEnable_set */

/* Function Name:
 *      drv_oob_phy_init
 * Description:
 *      Initial the PHY
 * Input:
 *      unit    - unit id
 *      port    - port id. Using RTK_OOB_IDX_TO_PORT(N) for port ID. N starts from 0.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 * Applicable:
 *      8213B
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
int32
drv_oob_phy_init(uint32 unit, rtk_port_t port)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].phy_init(unit, port);
}   /* end of drv_oob_phy_init */

/* Function Name:
 *      drv_oob_ringbuf_dump
 * Description:
 *      Dump OOB buffer status of the specified device.
 * Input:
 *      unit   - unit id
 *      port    - oob port index
 *      pOpt   - pointer to debug option
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Applicable:
 *      9311B
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
 * Changes:
 *      None
 */

int32
drv_oob_ringbuf_dump(uint32 unit, rtk_port_t port, oob_dbg_opt_t* pOpt)
{
    /* function body */
    RT_PARAM_CHK((unit > RTK_MAX_UNIT_ID), RT_ERR_UNIT_ID);
    RT_PARAM_CHK(((port < RTK_OOB_PORT_START) || (port > RTK_OOB_PORT_END)), RT_ERR_PORT_ID);

    return RT_DRV_MAPPER(unit)->oob[RTK_OOB_PORT_TO_IDX(port)].ringbuf_dump(unit, port, pOpt);
}

