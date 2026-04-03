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
 * Purpose : Definition of Buffer Management API
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) Flow Control
 *           (2) Congestion Control
 *           (3) Cacade Flow Control
 *           (4) End-to-end Congestion Control
 *
 */

#ifndef __RTK_BUFFMGMT_H__
#define __RTK_BUFFMGMT_H__

/*
 * Include Files
 */
#include <common/rt_type.h>
#include <rtk/port.h>

/*
 * Symbol Definition
 */
#define BUMBLEBEE_MAX_NUM_OF_ISG                4
#define BUMBLEBEE_MAX_NUM_OF_ESG                4
#define BUMBLEBEE_MAX_NUM_OF_PG                 8
#define BUMBLEBEE_MAX_NUM_OF_QUEUE              19
#define BUMBLEBEE_MAX_NUM_OF_CPU_QUEUE          48
#define BUMBLEBEE_MAX_NUM_OF_ABS_PG             2
#define BUMBLEBEE_MAX_NUM_OF_QUEUE              19
#define BUMBLEBEE_MAX_NUM_OF_MC_ARCH_QUEUE      11


/*
 * Data Declaration
 */
/* Types of drop precedence source */
typedef enum rtk_buffmgmt_absorbMem_e
{
    BUFFMGNT_ABS_MC = 0,
    BUFFMGNT_ABS_MIR,
    BUFFMGNT_ABS_CPU,
    BUFFMGNT_ABS_END
} rtk_buffmgmt_absorbMem_t;

/* buffer management ingress system used page cnt */
typedef struct rtk_buffmgmt_sys_igrPageCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_ISG];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_ISG];
    uint32 shareCntr;
    uint32 shareMaxCntr;
    uint32 shareHrCntr;
    uint32 shareMaxHrCntr;
    uint32 totalCntr;
    uint32 totalMaxCntr;
} rtk_buffmgmt_sys_igrPageCnt_t;

/* buffer management egress system used page cnt */
typedef struct rtk_buffmgmt_sys_egrPageCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_ESG];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_ESG];
    uint32 realMaxThr[BUMBLEBEE_MAX_NUM_OF_ESG];
    uint32 guarRealCntr[BUMBLEBEE_MAX_NUM_OF_ESG];
    uint32 guarRemCntr[BUMBLEBEE_MAX_NUM_OF_ESG];
    uint32 totalCntr;
    uint32 totalMaxCntr;
} rtk_buffmgmt_sys_egrPageCnt_t;

/* buffer management ingress port used page cnt */
typedef struct rtk_buffmgmt_port_igrPageCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_PG];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_PG];
    uint32 hrCntr[BUMBLEBEE_MAX_NUM_OF_PG];
    uint32 hrMaxCntr[BUMBLEBEE_MAX_NUM_OF_PG];
} rtk_buffmgmt_port_igrPageCnt_t;

/* buffer management system absorb memory used packet cnt */
typedef struct rtk_buffmgmt_sys_absorbPktCnt_s
{
    uint32 cntr;
    uint32 maxCntr;
    uint32 shrCntr;
    uint32 maxShrCntr;
} rtk_buffmgmt_sys_absorbPktCnt_t;

/* buffer management ingress port absorb memory used packet cnt */
typedef struct rtk_buffmgmt_port_absorbPktCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_ABS_PG];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_ABS_PG];
    uint32 hrCntr[BUMBLEBEE_MAX_NUM_OF_ABS_PG];
    uint32 hrMaxCntr[BUMBLEBEE_MAX_NUM_OF_ABS_PG];
} rtk_buffmgmt_port_absorbPktCnt_t;

/* buffer management egress port used page cnt */
typedef struct rtk_buffmgmt_port_egrPageCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_QUEUE];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_QUEUE];
} rtk_buffmgmt_port_egrPageCnt_t;

/* buffer management egress port used page cnt */
typedef struct rtk_buffmgmt_cpu_egrPageCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_CPU_QUEUE];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_CPU_QUEUE];
} rtk_buffmgmt_cpu_egrPageCnt_t;

/* buffer management egress port link list packet cnt */
typedef struct rtk_buffmgmt_port_egrPktCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_MC_ARCH_QUEUE];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_MC_ARCH_QUEUE];
} rtk_buffmgmt_port_egrPktCnt_t;

/* buffer management egress port link list packet cnt */
typedef struct rtk_buffmgmt_cpu_egrPktCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_CPU_QUEUE];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_CPU_QUEUE];
} rtk_buffmgmt_cpu_egrPktCnt_t;

/* buffer management egress system link list packet cnt */
typedef struct rtk_buffmgmt_sys_egrPktCnt_s
{
    uint32 cntr[BUMBLEBEE_MAX_NUM_OF_ESG];
    uint32 maxCntr[BUMBLEBEE_MAX_NUM_OF_ESG];
    uint32 totalCntr;
    uint32 totalMaxCntr;
} rtk_buffmgmt_sys_egrPktCnt_t;


/*
 * Function Declaration
 */

/* Module Name : STAT */


/* Function Name:
 *      rtk_buffmgmt_init
 * Description:
 *      Initialize buffer management module of the specified device.
 * Input:
 *      unit - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID               - invalid unit id
 *      RT_ERR_STAT_GLOBAL_CNTR_FAIL - Could not retrieve/reset Global Counter
 *      RT_ERR_STAT_PORT_CNTR_FAIL   - Could not retrieve/reset Port Counter
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_init(uint32 unit);

/* Function Name:
 *      rtk_buffmgmt_igrSystemPktBuffCntr_get
 * Description:
 *      Get ingress system used page count of packet buffer
 * Input:
 *      unit                - unit id
 * Output:
 *      pCntr               - pointer to the used-page-count counter
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_igrSystemPktBuffCntr_get(uint32 unit, rtk_buffmgmt_sys_igrPageCnt_t *pCntr);

/* Function Name:
 *      rtk_buffmgmt_egrSystemPktBuffCntr_get
 * Description:
 *      Get egress system used page count of packet buffer
 * Input:
 *      unit                - unit id
 * Output:
 *      pCntr               - pointer to the used-page-count counter
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_egrSystemPktBuffCntr_get(uint32 unit, rtk_buffmgmt_sys_egrPageCnt_t *pCntr);

/* Function Name:
 *      rtk_buffmgmt_portIgrPktBuffCntr_get
 * Description:
 *      Get ingress port used page count of packet buffer for the specified port
 * Input:
 *      unit     - unit id
 *      port     - port id
 * Output:
 *      pCntr               - pointer to the counter of pg current used-page-count
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_portIgrPktBuffCntr_get(uint32 unit, rtk_port_t port, rtk_buffmgmt_port_igrPageCnt_t *pCntr);

/* Function Name:
 *      rtk_buffmgmt_portEgrCntr_get
 * Description:
 *      Get egress used page count and packet count for the specified port
 * Input:
 *      unit     - unit id
 *      port     - port id
 * Output:
 *      pCntr               - pointer to the egress counter of current used-page-count
 *      pPktCntr            - pointer to the egress counter of current used-pkt-count
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_portEgrCntr_get(uint32 unit, rtk_port_t port, rtk_buffmgmt_port_egrPageCnt_t *pCntr, rtk_buffmgmt_port_egrPktCnt_t *pPktCntr);

/* Function Name:
 *      rtk_buffmgmt_cpuEgrCntr_get
 * Description:
 *      Get egress used page count and packet count for the CPU port
 * Input:
 *      unit                - unit id
 * Output:
 *      pCntr               - pointer to the egress counter of current used-page-count
 *      pPktCntr            - pointer to the egress counter of current used-pkt-count
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_cpuEgrCntr_get(uint32 unit, rtk_buffmgmt_cpu_egrPageCnt_t *pCntr, rtk_buffmgmt_cpu_egrPktCnt_t *pPktCntr);

/* Function Name:
 *      rtk_buffmgmt_systemAbsorbMemoryCntr_get
 * Description:
 *      Get used packet count of absorb memory
 * Input:
 *      unit                - unit id
 *      type                - type of absorb memory
 * Output:
 *      pCntr               - pointer to the current used-packet-count counter
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_systemAbsorbMemoryCntr_get(uint32 unit, rtk_buffmgmt_absorbMem_t type, rtk_buffmgmt_sys_absorbPktCnt_t *pCntr);

/* Function Name:
 *      rtk_buffmgmt_portAbsorbMemoryCntr_get
 * Description:
 *      Get used packet count of absorb memory for the specified port
 * Input:
 *      unit     - unit id
 *      port     - port id
 *      type     - type of absorb memory
 * Output:
 *      pCntr               - pointer to the counter of pg current used-packet-count
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_portAbsorbMemoryCntr_get(uint32 unit, rtk_port_t port, rtk_buffmgmt_absorbMem_t type, rtk_buffmgmt_port_absorbPktCnt_t *pCntr);

/* Function Name:
 *      rtk_buffmgmt_systemLinkListCntr_get
 * Description:
 *      Get used packet count of link list memory
 * Input:
 *      unit                - unit id
 * Output:
 *      pCntr               - pointer to the current used-packet-count counter
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_systemLinkListCntr_get(uint32 unit, rtk_buffmgmt_sys_egrPktCnt_t *pCntr);

/* Function Name:
 *      rtk_buffmgmt_igrSystemPktBuffCntr_reset
 * Description:
 *      Reset ingress system used page count of packet buffer
 * Input:
 *      unit                - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_igrSystemPktBuffCntr_reset(uint32 unit);

/* Function Name:
 *      rtk_buffmgmt_egrSystemPktBuffCntr_reset
 * Description:
 *      Reset egress system used page count of packet buffer
 * Input:
 *      unit                - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_egrSystemPktBuffCntr_reset(uint32 unit);

/* Function Name:
 *      rtk_buffmgmt_port_reset
 * Description:
 *      Reset port used page and packet count of packet buffer
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      type                - reset type ingress or egress
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Applicable:
 *      9330
 * Note:
 *      None
 * Changes:
 *      [SDK_4.0.0]
 *          New added function.
 */
extern int32
rtk_buffmgmt_port_reset(uint32 unit, rtk_port_t port, uint32 type);

#endif /* __RTK_BUFFMGMT_H__ */

