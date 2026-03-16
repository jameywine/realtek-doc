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
 * $Revision: 76998 $
 * $Date: 2017-03-30 10:52:26 +0800 (Thu, 30 Mar 2017) $
 *
 * Purpose : Definition of Stacking API
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) User Configuration Stacking
 *
 */

#ifndef __RTK_STACK_H__
#define __RTK_STACK_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <rtk/vlan.h>

/*
 * Symbol Definition
 */

typedef struct rtk_stack_rmtIntrInfo_s
{
    struct
    {
        uint8 last_rx_seq_id;
        uint8 rx_seq_id;
        uint16 isr_ccm;
        rtk_portmask_t port_link_sts;
    } unit[RTK_MAX_NUM_OF_UNIT];
} rtk_stack_rmtIntrInfo_t;

typedef struct rtk_stack_estTxHdr_s
{
    rtk_mac_t dmac;
    rtk_mac_t smac;
    uint8 vlan_if;
    rtk_vlan_t vlan;

    union {
        struct ipv4GreTx_s {
            uint8 tos;
            uint16 id;
            uint8 flag;
            uint8 ttl;
            rtk_ip_addr_t sip;
            rtk_ip_addr_t dip;
        } ipv4GreTx;
        struct ipv6GreTx_s {
            uint8 class;
            uint32 flowLabel;
            uint8 hop_limit;
            rtk_ipv6_addr_t sip;
            rtk_ipv6_addr_t dip;
        } ipv6GreTx;
    };
}rtk_stack_estTxHdr_t;

typedef struct rtk_stack_estRxHdr_s
{
    uint8 dmac_cmp;
    uint8 smac_cmp;
    uint8 vlan_cmp;
    rtk_mac_t dmac;
    rtk_mac_t smac;
    rtk_vlan_t vlan;

    union {
        struct ipv4GreRx_s {
            uint8 tos_cmp;
            uint8 id_cmp;
            uint8 flag_cmp;
            uint8 ttl_cmp;
            uint8 checksum_cmp;
            uint8 sip_cmp;
            uint8 dip_cmp;
            uint8 tos;
            uint16 id;
            uint8 flag;
            uint8 ttl;
            uint16 checksum;
            rtk_ip_addr_t sip;
            rtk_ip_addr_t dip;
        } ipv4GreRx;
        struct ipv6GreRx_s {
            uint8 class_cmp;
            uint8 flowLabel_cmp;
            uint8 hopLimit_cmp;
            uint8 sip_cmp;
            uint8 dip_cmp;
            uint8 class;
            uint32 flowLabel;
            uint8 hopLimit;
            rtk_ipv6_addr_t sip;
            rtk_ipv6_addr_t dip;
        } ipv6GreRx;
    };
}rtk_stack_estRxHdr_t;

typedef enum rtk_stack_estType_e
{
    TYPE_L2 = 0,
    TYPE_IPV4_GRE,
    TYPE_IPV6_GRE,
    TYPE_END
} rtk_stack_estType_t;

typedef enum rtk_stack_estTxHdrField_e
{
    TX_HDR_FIELD_VLAN_CFI_PRI = 0,
    TX_HDR_FIELD_IP4_ID,
    TX_HDR_FIELD_END,
} rtk_stack_estTxHdrField_t;

typedef enum rtk_stack_estTxHdrPriSrcMode_e
{
    TX_HDR_PRI_SRC_MODE_FORCE = 0,
    TX_HDR_PRI_SRC_MODE_INHERIT,
    TX_HDR_PRI_SRC_MODE_INTPRI,
    TX_HDR_PRI_SRC_MODE_END
} rtk_stack_estTxHdrPriSrcMode_t;

typedef enum rtk_stack_estTxHdrIdSrcMode_e
{
    TX_HDR_ID_SRC_MODE_FORCE = 0,
    TX_HDR_ID_SRC_MODE_INCREMENTAL,
    TX_HDR_ID_SRC_MODE_END
} rtk_stack_estTxHdrIdSrcMode_t;

typedef struct rtk_stack_estTxHdrSrcMode_s
{
    union {
        rtk_stack_estTxHdrPriSrcMode_t cfiPriSrcMode;
        rtk_stack_estTxHdrIdSrcMode_t ip4IdSrcMode;
    };
}rtk_stack_estTxHdrSrcMode_t;

typedef enum rtk_stack_estRxHdrErrAction_e
{
    ERR_ACTION_DROP = 0,
    ERR_ACTION_TRAP,
    ERR_ACTION_END
} rtk_stack_estRxHdrErrAction_t;

typedef struct rtk_stack_cascadeCfg_s
{
    rtk_enable_t e2e_fc_en;
    rtk_enable_t e2e_fc_normalPort_en;
    rtk_enable_t e2e_ntfy_en;
}rtk_stack_cascadeCfg_t;


/*
 * Data Declaration
 */

/*
 * Macro Declaration
 */

/*
 * Function Declaration
 */

/* Module Name : STACK */

/* Function Name:
 *      rtk_stack_init
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      Must initialize stack module before calling any stack APIs.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_init(uint32 unit);

/* Function Name:
 *      rtk_stack_cascadeMode_init
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
extern int32
rtk_stack_cascadeMode_init(uint32 unit);


/* Module Name    : STACK                    */
/* Sub-module Name: User configuration stack */

/* Function Name:
 *      rtk_stack_port_get
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_port_get (uint32 unit, rtk_portmask_t *pStkPorts);

/* Function Name:
 *      rtk_stack_port_set
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      In 9300, only port 24~27 can be configured as stacking ports.
 *      In 9310, only port 52~55 can be configured as stacking ports.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_port_set (uint32 unit, rtk_portmask_t *pStkPorts);

/* Function Name:
 *      rtk_stack_unit_get
 * Description:
 *      Get the switch unit ID from the device.
 * Input:
 *      unit                   - unit id
 * Output:
 *      pMyUnit              - pointer buffer of uind ID of the switch
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_unit_get(uint32 unit, uint32 *pMyUnit);


/* Function Name:
 *      rtk_stack_unit_set
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) 9300, 9310 supports 16 stacked units, thus myUnit ranges from 0 to 15.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_unit_set(uint32 unit, uint32 myUnit);

/* Function Name:
 *      rtk_stack_masterUnit_get
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_masterUnit_get(uint32 unit, uint32 *pMasterUnit);

/* Function Name:
 *      rtk_stack_masterUnit_set
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) 9300, 9310 supports 16 stacked units, thus masterUnit ranges from 0 to 15.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_masterUnit_set(uint32 unit, uint32 masterUnit);

/* Function Name:
 *      rtk_stack_loopGuard_get
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_loopGuard_get(uint32 unit, rtk_enable_t *pEnable);


/* Function Name:
 *      rtk_stack_loopGuard_set
 * Description:
 *      Set the enable status of dropping packets with source unit ID the same as the unit ID of the switch to the specified device.
 * Input:
 *      unit          - unit id
 *      enable      - enable state of loop guard mechanism
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_loopGuard_set(uint32 unit, rtk_enable_t enable);

/* Function Name:
 *      rtk_stack_unitPortMap_get
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_unitPortMap_get (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts);


/* Function Name:
 *      rtk_stack_unitPortMap_set
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) Stacking ports in 9300 ranges from 24 to 27.
 *      (2) Stacking ports in 9310 ranges from 52 to 55.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_unitPortMap_set (uint32 unit, uint32 dstUnit, rtk_portmask_t *pStkPorts);

/* Function Name:
 *      rtk_stack_nonUcastBlockPort_get
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_nonUcastBlockPort_get (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts);


/* Function Name:
 *      rtk_stack_nonUcastBlockPort_set
 * Description:
 *      Set the stacking ports that would block ingress and egress non-unicast packets to the specified device.
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
 * Applicable:
 *      9300, 9310
 * Note:
 *      (1) Blockable stacking ports in 9300 ranges from 24 to 27.
 *      (2) Blockable stacking ports in 9310 ranges from 52 to 55.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_nonUcastBlockPort_set (uint32 unit, uint32 srcUnit, rtk_portmask_t *pBlockStkPorts);

/* Function Name:
 *      rtk_stack_rmtIntrTxEnable_get
 * Description:
 *      Get enable status of Remote Interrupt Notification transmission.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - pointer to enable status
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
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrTxEnable_get(uint32 unit, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_stack_rmtIntrTxEnable_set
 * Description:
 *      Set enable status of Remote Interrupt Notification transmission.
 * Input:
 *      unit   - unit id
 *      enable - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrTxEnable_set(uint32 unit, rtk_enable_t enable);

/* Function Name:
 *      rtk_stack_rmtIntrTxTriggerEnable_get
 * Description:
 *      Get enable status of Remote Interrupt Notification transmission trigger.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - pointer to enable status
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
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrTxTriggerEnable_get(uint32 unit, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_stack_rmtIntrTxTriggerEnable_set
 * Description:
 *      Set enable status of Remote Interrupt Notification transmission trigger.
 * Input:
 *      unit   - unit id
 *      enable - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      The enable status will be clear automatically once the transmission has been done.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrTxTriggerEnable_set(uint32 unit, rtk_enable_t enable);

/* Function Name:
 *      rtk_stack_rmtIntrRxSeqCmpMargin_get
 * Description:
 *      Get the comparing margin of the sequence ID of receiving Remote Interrupt Notification.
 * Input:
 *      unit    - unit id
 * Output:
 *      pMargin - pointer to margin value
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
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrRxSeqCmpMargin_get(uint32 unit, int32 *pMargin);

/* Function Name:
 *      rtk_stack_rmtIntrRxSeqCmpMargin_set
 * Description:
 *      Set the comparing margin of the sequence ID of receiving Remote Interrupt Notification.
 * Input:
 *      unit   - unit id
 *      margin - margin value
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Applicable:
 *      9310
 * Note:
 *      None
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrRxSeqCmpMargin_set(uint32 unit, int32 margin);

/* Function Name:
 *      rtk_stack_rmtIntrRxForceUpdateEnable_get
 * Description:
 *      Get the force enable status of updating when receives a Remote Interrupt Notification.
 * Input:
 *      unit    - unit id
 * Output:
 *      pEnable - pointer to enable status
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
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrRxForceUpdateEnable_get(uint32 unit, rtk_enable_t *pEnable);

/* Function Name:
 *      rtk_stack_rmtIntrRxForceUpdateEnable_set
 * Description:
 *      Set the force enable status of updating when receives a Remote Interrupt Notification.
 * Input:
 *      unit   - unit id
 *      enable - enable status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_INPUT    - invalid input parameter
 * Applicable:
 *      9310
 * Note:
 *      The enable status will be clear automatically once the updating has been done.
 * Changes:
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrRxForceUpdateEnable_set(uint32 unit, rtk_enable_t enable);

/* Function Name:
 *      rtk_stack_rmtIntrInfo_get
 * Description:
 *      Get the information about Remote Interrupt Notification.
 * Input:
 *      unit  - unit id
 * Output:
 *      pInfo - pointer to information
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
 *      [SDK_3.0.0]
 *          New added function.
 */
extern int32
rtk_stack_rmtIntrInfo_get(uint32 unit, rtk_stack_rmtIntrInfo_t *pInfo);

#endif /* __RTK_STACK_H__ */

