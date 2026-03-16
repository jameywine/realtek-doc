/*
 * Copyright (C) 2009 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 81010 $
 * $Date: 2017-08-06 16:43:05 +0800 (Sun, 06 Aug 2017) $
 *
 * Purpose : PHY 8295 Driver APIs.
 *
 * Feature : PHY 8295 Driver APIs
 *
 */
#ifndef __PHY_8295_PATCH_H__
#define __PHY_8295_PATCH_H__

/*
 * Include Files
 */

/*
 * Symbol Definition
 */
typedef enum
{
    PHY_8295_PATCH_DBG_OUT = (0x1 << 1),
    PHY_8295_PATCH_DBG_BUF = (0x1 << 2),
}phy_8295_patch_dbg_t;

typedef enum
{
    PHY_8295_PATCH_TYPE_TXATTR_S18431 = 0,
    PHY_8295_PATCH_TYPE_TXATTR_S1DACLONG,
    PHY_8295_PATCH_TYPE_END
} phy_8295_patch_type_t;

typedef enum phy_8295_rxCali_process_sts_e
{
    PHY_8295_RXCALI_PROCESS_STS_IDLE = 0,
    PHY_8295_RXCALI_PROCESS_STS_PROCESS,
    PHY_8295_RXCALI_PROCESS_STS_END
} phy_8295_rxCali_process_sts_t;

typedef struct phy_8295_pageRegVal_s {
    uint16      page;
    uint16      reg;
    uint16      val;
} phy_8295_pageRegVal_t;

typedef struct phy_8295_rxCaliConf_s {
    uint32          s1rxCaliDacLongCableOffset;
} phy_8295_rxCaliConf_t;

typedef struct phy_8295_rxCaliParam_s
{
    uint32      cable_offset;
} phy_8295_rxCaliParam_t;



/*
 * Data Declaration
 */

/*
 * Macro Declaration
 */
#define PHY_8295R_PATCH_PARAM_INIT(unit, port, type, patch_arr, struct_t)      phy_8295r_patchParam_init(unit, port, type, patch_arr, sizeof(patch_arr)/sizeof(struct_t))

/*
 * Function Declaration
 */
extern void
phy_8295_patch_20160627(uint32 unit, rtk_port_t BasePort);

extern void
phy_8295_patch_20160711(uint32 unit, uint32 BasePort);

extern void
phy_8295_patch_start_tap0(uint32 unit, uint32 port);

extern void
phy_8295_patch_rxCali(uint32 unit, uint32 basePort, uint32 sdsId, phy_8295_rxCaliParam_t *param);

extern void
phy_8295r_patch_set(uint32 unit, uint32 port);

extern void
phy_8214qf_patch_set(uint32 unit, uint32 port);

extern int32
phy_8295r_patch_txAttri_set(uint32 unit, rtk_port_t port, rtk_port_10gMedia_t media);

extern int32
phy_8295r_patchParam_init(uint32 unit, rtk_port_t port, phy_8295_patch_type_t type, void *prv, int32 size);

extern int32
phy_8295r_rxCaliConf_set(uint32 unit, phy_8295_rxCaliConf_t *conf);

extern int32
phy_8295r_rxCaliConf_get(uint32 unit, phy_8295_rxCaliConf_t *conf);

/* diag API */
extern int32
phy_8295_patch_debugEnable_set(uint32 enable);


#endif /* __PHY_8295_PATCH_H__ */
