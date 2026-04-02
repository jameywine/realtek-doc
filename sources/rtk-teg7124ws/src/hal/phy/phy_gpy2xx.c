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
 * $Revision: 75479 $
 * $Date: 2017-01-20 15:17:16 +0800 (Fri, 20 Jan 2017) $
 *
 * Purpose : PHY GPY241 Driver APIs.
 *
 * Feature : PHY GPY241 Driver APIs
 *
 */


/*
 * Include Files
 */
#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <rtk/init.h>
#include <soc/type.h>
#include <hal/common/halctrl.h>
#include <hal/mac/serdes.h>
#include <hal/mac/miim_common_drv.h>
#include <hal/phy/phy_gpy2xx.h>
#include "types.h"
#include "gpy211_common.h"
#include "gpy211_regs.h"
#include "gpy211.h"
#include <registers/phy/std.h>
#include <registers/phy/phy.h>
#include <registers/phy/aneg.h>
#include <registers/phy/pmapmd.h>
#include <ioal/mem32.h>
#include <osal/time.h>
#include <osal/sem.h>
#include <hwp/hw_profiles/hwp_senao_non_rtl_phy_info.c>

/*
 * Macro Declaration
 */
#define PHY_GPY211_ERR_DBG        8
#define PHY_GPY211_WARNING_DBG    7
#define PHY_GPY211_PORTING_DBG    1
#define PHY_GPY211_DIS_DBG        0

#define PHY_GPY211_DBG(DBG_FLAG, fmt, args...) if (DBG_FLAG >= dbg_level) { printf(fmt, ##args); }
#define PHY_GPY_LOG(fmt, args...)      if (PHY_GPY211_PORTING_DBG >= dbg_level) { printf(fmt, ##args); }
#define PHY_GPY_ERR(fmt, args...)      if (PHY_GPY211_ERR_DBG >= dbg_level) { printf(fmt, ##args); }

#define PORT_NUM_IN_GPY2xx        4

uint32 dbg_level = PHY_GPY211_WARNING_DBG;

/*
 * Data Declaration
 */
rt_phyInfo_t phy_gpy2xx_info =
{
    .phy_num    = PORT_NUM_IN_GPY2xx,
    .eth_type   = HWP_2_5GE,
    .isComboPhy = {0, 0, 0, 0},

    /* RTK_PHYINFO_FLAG_1G_MMD_CFG: MAC polling GPY241(2.5G PHY) 1G speed status. */
    /* RTK_PHYINFO_FLAG_NO_RES_REG: not using RTK proprietary PHY status resolution register. */
    .flags      = (RTK_PHYINFO_FLAG_1G_MMD_CFG | RTK_PHYINFO_FLAG_NO_RES_REG),

/*  MAC polling GPY241 1G status
    duplex: 0.24.3,  speed: 0.24.1,  link partner speed: 0.24.1
    .xGePhyLocalDuplexAbilityDev            = 0,
    .xGePhyLocalDuplexAbilityAddr           = 0x18,
    .xGePhyLocalDuplexAbilityBit            = 3,

    .xGePhyLocal1000MSpeedAbilityDev        = 0,
    .xGePhyLocal1000MSpeedAbilityAddr       = 0x18,
    .xGePhyLocal1000MSpeedAbilityBit        = 1,

    .xGePhyLinkPartner1000MSpeedAbilityDev  = 0,
    .xGePhyLinkPartner1000MSpeedAbilityAddr = 0x18,
    .xGePhyLinkPartner1000MSpeedAbilityBit  = 1,     */


/*  MAC polling GPY241 1G status
    duplex: 0.24.3,  speed: 0.9.9,  link partner speed: 0.10.11  */
    .xGePhyLocalDuplexAbilityDev            = 0,
    .xGePhyLocalDuplexAbilityAddr           = 0x18,
    .xGePhyLocalDuplexAbilityBit            = 3,

    .xGePhyLocal1000MSpeedAbilityDev        = 0,
    .xGePhyLocal1000MSpeedAbilityAddr       = 0x9,
    .xGePhyLocal1000MSpeedAbilityBit        = 9,

    .xGePhyLinkPartner1000MSpeedAbilityDev  = 0,
    .xGePhyLinkPartner1000MSpeedAbilityAddr = 0xA,
    .xGePhyLinkPartner1000MSpeedAbilityBit  = 11,
};

/* The structure should be initialized before executing PHY init (gpy2xx_init) */
static struct gpy211_device phy[HWP_SENAO_MAX_NON_RTL_PORT_NUM]; 

static osal_mutex_t   phy_sem[RTK_MAX_NUM_OF_UNIT] = {(osal_mutex_t)NULL};
#define PHY_SEM_LOCK(unit)    \
do {\
    if (osal_sem_mutex_take(phy_sem[unit], OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_DAL|MOD_SDS), "semaphore lock failed");\
        return RT_ERR_SEM_LOCK_FAILED;\
    }\
} while(0)

#define PHY_SEM_UNLOCK(unit)   \
do {\
    if (osal_sem_mutex_give(phy_sem[unit]) != RT_ERR_OK)\
    {\
        RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_DAL|MOD_SDS), "semaphore unlock failed");\
        return RT_ERR_SEM_UNLOCK_FAILED;\
    }\
} while(0)


/*
 * Function Declaration
 */
int32 phy_gpy2xx_init(uint32 unit, rtk_port_t port);
int32 phy_gpy2xx_eye_diagram_init(uint32 unit, rtk_port_t port);
static int gpy211_mdio_read(void *busdata, u16 phyaddr, u32 regnum);
static int gpy211_mdio_write(void *busdata, u16 phyaddr, u32 regnum, u16 data);
extern phyp_senao_phy_info_t *senao_non_rtl_phy_info;


static struct {
    const char *name;
    enum link_mode_bit_indices bit;
} phy_advert[] = {
    {"10baseT_Half", LINK_MODE_10baseT_Half_BIT},
    {"10baseT_Full", LINK_MODE_10baseT_Full_BIT},
    {"100baseT_Half", LINK_MODE_100baseT_Half_BIT},
    {"100baseT_Full", LINK_MODE_100baseT_Full_BIT},
    {"1000baseT_Half", LINK_MODE_1000baseT_Half_BIT},
    {"1000baseT_Full", LINK_MODE_1000baseT_Full_BIT},
    {"2500baseT_Full", LINK_MODE_2500baseT_Full_BIT},
    {"2500baseT_FR", LINK_MODE_2500baseT_FR_BIT},
    {"5000baseT_Full", LINK_MODE_5000baseT_Full_BIT},
    {"5000baseT_FR", LINK_MODE_5000baseT_FR_BIT},
    {"Autoneg", LINK_MODE_Autoneg_BIT},
    {"Pause", LINK_MODE_Pause_BIT},
    {"Asym_Pause", LINK_MODE_Asym_Pause_BIT},
};

static int print_advert(char *buf, unsigned int size, u64 param)
{
    char *p = buf;
    unsigned int flag = 0;
    unsigned int total_len = 0;
    unsigned int len;
    unsigned int i;

    strncpy(buf, "none", size);
    buf[size - 1] = 0;

    for (i = 0; i < ARRAY_SIZE(phy_advert); i++) {
        if ((param & ((u64)1 << phy_advert[i].bit))) {
            len = strlen(phy_advert[i].name);

            if (flag == 0) {
                if (total_len + len + 1 >= size)
                    break;

                flag++;
            } else {
                if (total_len + len + 4 >= size)
                    break;

                p[0] = ' ';
                p[1] = '|';
                p[2] = ' ';
                p += 3;
                total_len += 3;
            }

            strcpy(p, phy_advert[i].name);
            p += len;
            total_len += len;
        }
    }

    return (int)total_len;
}


static int gpy211_mdio_read(void *busdata, u16 phyaddr, u32 regnum)
{
/*
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, phyaddr:0x%x, regnum:0x%x \n", __func__, __LINE__, phyaddr, regnum);
    PHY_GPY_LOG("phyaddr 0x%X regnum 0x%X mdio read api", phyaddr, regnum);
*/
    hal_control_t   *pHalCtrl;
    int32            ret = 0;
    uint32           unit = 0;
    rtk_port_t       port;
    uint32           dev_id = 0;
    uint32           addr = 0;
    uint32           data = 0;

    /* MDIO_MMD_PMAPMD(dev=1), MDIO_MMD_PCS(dev=3), MDIO_MMD_AN(dev=7), MDIO_MMD_VEND1(dev=30), MDIO_MMD_VEND2(dev=31) */
    if (regnum & MII_ADDR_C45)
    {
        dev_id = (regnum >> 16) & 0x1F;
        addr = regnum & 0xFFFF;
    }
    else /* STD (dev=0) */
    {
        dev_id = 0;
        addr = regnum;
    }

    if(_phy_gpy2xx_gpy2xxPort_to_rtkPort(&unit, &port, phyaddr) != RT_ERR_OK)
        return RT_ERR_FAILED;


    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, dev_id, addr, &data)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, ">> %s %d, ret=%d mido read fail.\n", __func__, __LINE__, ret);
        PHY_GPY_ERR("U%u P%u dev_id:0x%X addr:0x%X read fail", unit, port, dev_id, addr);
        return RT_ERR_FAILED;
    }

    return data;
}

static int gpy211_mdio_write(void *busdata, u16 phyaddr, u32 regnum, u16 data)
{
/*
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, phyaddr:0x%x, regnum:0x%x, data:0x%x \n", __func__, __LINE__, phyaddr, regnum, data);
    PHY_GPY_LOG("phyaddr 0x%X regnum 0x%X data 0x%X mdio write api", phyaddr, regnum, data);
*/
    hal_control_t   *pHalCtrl;
    int32            ret = 0;
    uint32           unit = 0;
    rtk_port_t       port;
    uint32           dev_id = 0;
    uint32           addr = 0;

    /* MDIO_MMD_PMAPMD(dev=1), MDIO_MMD_PCS(dev=3), MDIO_MMD_AN(dev=7), MDIO_MMD_VEND1(dev=30), MDIO_MMD_VEND2(dev=31) */
    if (regnum & MII_ADDR_C45)
    {
        dev_id = (regnum >> 16) & 0x1F;
        addr = regnum & 0xFFFF;
    }
    else /* STD (dev=0) */
    {
        dev_id = 0;
        addr = regnum;
    }

    if(_phy_gpy2xx_gpy2xxPort_to_rtkPort(&unit, &port, phyaddr) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, dev_id, addr, data)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, ">> %s %d, ret=%d mido write fail \n", __func__, __LINE__, ret);
        PHY_GPY_ERR("U%u P%u dev_id:0x%X addr:0x%X write fail", unit, port, dev_id, addr);
        return RT_ERR_FAILED;
    }

    return ret;
}

static void
_phy_gpy2xx_phy_lock(void *lock_data)
{
    osal_mutex_t sem = *(osal_mutex_t *)lock_data;

    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, sem:%d\n", __func__, __LINE__, sem);

    do {
        if (osal_sem_mutex_take(sem, OSAL_SEM_WAIT_FOREVER) != RT_ERR_OK)
        {
            RT_ERR(RT_ERR_SEM_LOCK_FAILED, (MOD_DAL|MOD_SDS), "semaphore lock failed");
        }
    } while(0);

    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Exit >> %s %d, sem:%d\n", __func__, __LINE__, sem);
}

static void
_phy_gpy2xx_phy_unlock(void *lock_data)
{
    osal_mutex_t sem = *(osal_mutex_t *)lock_data;

    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, sem:%d\n", __func__, __LINE__, sem);

    do {
        if (osal_sem_mutex_give(sem) != RT_ERR_OK)
        {
            RT_ERR(RT_ERR_SEM_UNLOCK_FAILED, (MOD_DAL|MOD_SDS), "semaphore unlock failed");
        }
    } while(0);

    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Exit >> %s %d, sem:%d\n", __func__, __LINE__, sem);
}

/* Function Name:
 *      _phy_gpy2xx_struct_gpy211_device_per_port
 * Description:
 *      Init gpy211_device with different port number.
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None 
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
_phy_gpy2xx_struct_gpy211_device_per_port(uint32 unit, rtk_port_t port)
{
    uint32    index = 0;
    uint32    gpy_phy_id = 0;

    if(senao_non_rtl_phy_info==NULL)
        return RT_ERR_FAILED;

    for(index = 0; senao_non_rtl_phy_info->port_mapping[index].unit != HWP_END; index++)
    {
        if(unit == senao_non_rtl_phy_info->port_mapping[index].unit && port == senao_non_rtl_phy_info->port_mapping[index].mac_id)
        {
            gpy_phy_id = senao_non_rtl_phy_info->port_mapping[index].phy_id;

            phy[gpy_phy_id].lock = _phy_gpy2xx_phy_lock;       // callback func pointer for 'lock'
            phy[gpy_phy_id].unlock = _phy_gpy2xx_phy_unlock;   // callback func pointer for 'unlock'
            phy[gpy_phy_id].lock_data = (void *)&phy_sem[unit];// pointer to lock_data

            phy[gpy_phy_id].mdiobus_read = gpy211_mdio_read;   // callback func pointer for 'mdio_read'
            phy[gpy_phy_id].mdiobus_write = gpy211_mdio_write; // callback func pointer for 'mdio_write'

            phy[gpy_phy_id].mdiobus_data = NULL;               // pointer to mdio_data
            phy[gpy_phy_id].smdio_addr = 0;                    // smdio_addr (not used)
            phy[gpy_phy_id].phy_addr = gpy_phy_id;             // phy_addr (here is gpy_phy_id)

            phy[gpy_phy_id].priv_data = NULL;                  // pointer to priv_data
            phy[gpy_phy_id].shared_data = NULL;                // pointer to shared_data
/*
            phy[gpy_phy_id].id = {0};                          // PHY ID
            phy[gpy_phy_id].link = {0};                        // Link data

            phy[gpy_phy_id].wol_supported = 0x60;              // WOL support
            phy[gpy_phy_id].macsec_supported = 0;              // max MACSec SAs supported
            phy[gpy_phy_id].nr_of_sas = 0;                     // gmac_data
            phy[gpy_phy_id].gmac_data = NULL;                  // macsec_data
            phy[gpy_phy_id].macsec_data = NULL;

            phy[gpy_phy_id].ptp_clock = 0x320;                 // PTP clock
            phy[gpy_phy_id].def_addend = 0xaabbccdd;           // PTP default addend
*/
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, unit:%d, port:%d\n, phy_addr:%d, gpy211_device init success.\n", __func__, __LINE__, unit, port, gpy_phy_id);
            return RT_ERR_OK;
        }
    }
    PHY_GPY_ERR("U%u P%u port gpy211_device init value fail.", unit, port);
    return RT_ERR_FAILED;
}

/* Function Name:
 *      _phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index
 * Description:
 *      Transfert rtk unit/port to gpy2xx phy port and get array index
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pgpy2xx_phy_id - gpy2xx port id
 *      table_index - array index
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index(uint32 unit, rtk_port_t port, uint32 *pgpy2xx_phy_id, uint32 *table_index)
{
    uint32 index = 0;

    if(senao_non_rtl_phy_info==NULL)
        return RT_ERR_FAILED;

    for(index = 0; senao_non_rtl_phy_info->port_mapping[index].unit != HWP_END; index++)
    {
        if(unit == senao_non_rtl_phy_info->port_mapping[index].unit && port == senao_non_rtl_phy_info->port_mapping[index].mac_id)
        {
            *pgpy2xx_phy_id = senao_non_rtl_phy_info->port_mapping[index].phy_id;
            *table_index = index;
            return RT_ERR_OK;
        }
    }
    PHY_GPY_ERR("U%u P%u port mapping/get table index fail", unit, port);
    return RT_ERR_FAILED;
}


/* Function Name:
 *      _phy_gpy2xx_rtkPort_to_gpy2xxPort
 * Description:
 *      Transfert rtk unit/port to gpy2xx phy port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pgpy2xx_phy_id - gpy2xx port id
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_gpy2xx_rtkPort_to_gpy2xxPort(uint32 unit, rtk_port_t port, uint32 *pgpy2xx_phy_id)
{
    uint32 index = 0;

    if(senao_non_rtl_phy_info == NULL)
        return RT_ERR_FAILED;

    for(index = 0; senao_non_rtl_phy_info->port_mapping[index].unit != HWP_END; index++)
    {
        if(unit == senao_non_rtl_phy_info->port_mapping[index].unit && port == senao_non_rtl_phy_info->port_mapping[index].mac_id)
        {
            *pgpy2xx_phy_id = senao_non_rtl_phy_info->port_mapping[index].phy_id;

            //PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, unit:%d, port:%d, pgpy2xx_phy_id:%d \n", __func__, __LINE__, unit, port, *pgpy2xx_phy_id);
            return RT_ERR_OK;
        }
    }

    PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, pgpy2xx_phy_id:%d, port mapping fail\n", __func__, __LINE__, unit, port, *pgpy2xx_phy_id);
    PHY_GPY_ERR("U%u P%u port mapping fail", unit, port);
    return RT_ERR_FAILED;
}

/* Function Name:
 *      _phy_gpy2xx_gpy2xxPort_to_rtkPort
 * Description:
 *      Transfert gpy2xx phy port to rtk unit/port
 * Input:
 *      pgpy2xx_phy_id - gpy2xx port id
 * Output:
 *      unit - unit id
 *      port - port id
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_gpy2xx_gpy2xxPort_to_rtkPort(uint32 *unit, rtk_port_t *port, uint32 pgpy2xx_phy_id)
{
    uint32 index = 0;

    if(senao_non_rtl_phy_info == NULL)
        return RT_ERR_FAILED;

    for(index = 0; senao_non_rtl_phy_info->port_mapping[index].unit != HWP_END; index++)
    {
        if(pgpy2xx_phy_id == senao_non_rtl_phy_info->port_mapping[index].phy_id)       
        {
            *unit = senao_non_rtl_phy_info->port_mapping[index].unit;
            *port = senao_non_rtl_phy_info->port_mapping[index].mac_id;

            //PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, unit:%d, port:%d, pgpy2xx_phy_id:%d \n", __func__, __LINE__, *unit, *port, pgpy2xx_phy_id);
            return RT_ERR_OK;
        }
    }

    PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, pgpy2xx_phy_id:%d, port mapping fail\n", __func__, __LINE__, *unit, *port, pgpy2xx_phy_id);
    PHY_GPY_ERR("P%d port mapping fail", pgpy2xx_phy_id);
    return RT_ERR_FAILED;
}

/* Function Name:
 *      phy_gpy2xx_identify
 * Description:
 *      Identify the port is GPY2xx PHY or not?
 * Input:
 *      unit     - unit id
 *      port     - port id
 *      model_id - model id
 *      rev_id   - revision id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - is intra serdes PHY
 *      RT_ERR_FAILED           - access failure or others
 *      RT_ERR_PHY_NOT_MATCH    - is not intra serdes PHY
 * Note:
 *      None
 */
int32
phy_gpy2xx_identify(uint32 unit, rtk_port_t port, uint32 model_id, uint32 rev_id)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d \n", __func__, __LINE__, unit, port);

    int32                   ret = 0;
    uint32                  reg0d2 = 0;
    uint32                  reg0d3 = 0;
    hal_control_t          *pHalCtrl = NULL;
    uint32                  gpy_phy_id;
    drv_smi_mdxProtoSel_t   mdxProto;


    if (HWP_PHY_MODEL_BY_PORT(unit, port) != RTK_PHYTYPE_GPY2xx)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d,  ERROR: unit:%d port:%d PHY type fail.\n", __func__, __LINE__, unit, port);
        return RT_ERR_PHY_NOT_MATCH;
    }

    if(_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_PHY_NOT_MATCH;


    /* first port delay */
    if(gpy_phy_id == 0)
       osal_time_mdelay(1000);


    RT_PARAM_CHK((NULL == (pHalCtrl = hal_ctrlInfo_get(unit))), RT_ERR_FAILED);

    if (hal_miim_portSmiMdxProto_get(unit, port, &mdxProto) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, unit:%d, port:%d, mdxProto:%d, portSmiMdxProto get fail\n", __func__, __LINE__, unit, port, mdxProto);
        return RT_ERR_PHY_NOT_MATCH;
    }

    if (mdxProto != DRV_SMI_MDX_PROTO_C45)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mdxProto:%d, mdxProto is not C45 recently.\n", __func__, __LINE__, mdxProto);
        PHY_GPY_LOG("U%u P%u change SMI to C45", unit, port);

        /* change to C45 for probing MXL PHY */
        if (hal_miim_portSmiMdxProto_set(unit, port, DRV_SMI_MDX_PROTO_C45) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, unit:%d, port:%d, portSmiMdxProto set fail\n", __func__, __LINE__, unit, port);
            PHY_GPY_LOG("U%u P%u change SMI to C45 fail", unit, port);
            /* PHY can only be accessed through C45 MDC/MDIO format. if SMI is not able to using C45, then return there is no match. */
            return RT_ERR_PHY_NOT_MATCH;
        }
    }

    /* STD_PHYID1: PHY Identifier 1 (Register 0.2) */
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0, 2, &reg0d2)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg0d2:0x%x, read PHY Identifier 1 fail.\n", __func__, __LINE__, reg0d2);
        PHY_GPY_ERR("U%u P%u mmd0x0 reg0x2 read fail", unit, port);
        return ret;
    }
    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "reg0d2:0x%x, read PHY Identifier 1 success.\n", reg0d2);

    /* STD_PHYID2: PHY Identifier 2 (Register 0.3) */
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0, 3, &reg0d3)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg0d3:0x%x, read PHY Identifier 2 fail.\n", __func__, __LINE__, reg0d3);
        PHY_GPY_ERR("U%u P%u mmd0x0 reg0x3 read fail", unit, port);
        return ret;
    }
    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "reg0d3:0x%x, read PHY Identifier 2 success.\n", reg0d3);


    /* GPY241
     * PHY Identifier 1 (Register 0.2) Reset Value: 67C9
     * PHY Identifier 2 (Register 0.3) Reset Value: DC00
     */
    // Starry@20220117 modify for Chip GPY2xx_B0BC -S
    // if ((reg0d2 != 0x67c9) || (reg0d3 != 0xdc00))
    // PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg3:0x%x, & 0xFC00=>%x\n", __func__, __LINE__, reg0d3, reg0d3 & 0xFC00);    
    if ((reg0d2 != 0x67c9) || ((reg0d3 & 0xFC00) != 0xdc00))
    // Starry@20220117 modify for Chip GPY2xx_B0BC -E
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg0d2:0x%x, reg0d3:0x%x, identify fail.\n", __func__, __LINE__, reg0d2, reg0d3);
        PHY_GPY_LOG("U%u P%u id1%x id2%x PHY NOT MATCH", unit, port, reg0d2, reg0d3);
        return RT_ERR_PHY_NOT_MATCH;
    }

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, reg0d2:0x%x, reg0d3:0x%x, identify success.\n", __func__, __LINE__, reg0d2, reg0d3);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_init
 * Description:
 *      Initialize PHY.
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_init(uint32 unit, rtk_port_t port)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d \n", __func__, __LINE__, unit, port);

    hal_control_t          *pHalCtrl;
    int32                   ret = 0;
    uint32                  gpy_phy_id;

    /* create semaphore */
    if (phy_sem[unit] == (osal_mutex_t) NULL)
    {
        phy_sem[unit] = osal_sem_mutex_create();
        if (0 == phy_sem[unit])
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, phy semaphore create failed.\n", __func__, __LINE__);
            RT_ERR(RT_ERR_FAILED, (MOD_DAL|MOD_COMMON), "phy semaphore create failed.");
            return RT_ERR_FAILED;
        }
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, phy semaphore create success.\n", __func__, __LINE__);
    }


    /* Init gpy211_device structure per port */
    if(_phy_gpy2xx_struct_gpy211_device_per_port(unit, port) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, ERROR: gpy211_device init fail.\n", __func__, __LINE__, unit, port);
        return RT_ERR_FAILED;
    }

    if(_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }


    /* Init GPY2xx PHY */
    ret = gpy2xx_init(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_init fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }


#if 0 /* 20211008 workaround: set GPY usxgmii short reach configuration and then do vr reset. */
    uint32           data = 0;

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1e, 0x8, &data)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, port:%u reg 30.8 read fail.\n", __func__, __LINE__, port);
        PHY_GPY_ERR("U%u P%u reg 30.8 read fail", unit, port);
        return RT_ERR_FAILED;
    }

    /* set reg 30.8 from 0x30da to 0x30c2 */
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x1e, 0x8, 0x30c2)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, port:%u reg 30.8 write fail.\n", __func__, __LINE__, port);
        PHY_GPY_ERR("U%u P%u reg 30.8 write fail", unit, port);
        return RT_ERR_FAILED;
    }
    osal_time_mdelay(100);
#else

    if(phy_gpy2xx_eye_diagram_init(unit, port)!= RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }

#endif

#if 1
    ret = gpy2xx_usxgmii_vr_reset(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_vr_reset fail.\n", __func__, __LINE__);
    }
    osal_time_mdelay(100);
#endif


    /*  288K Alignment Marker should set:
        MAC side: 2880  (100 times less than actual value)
        PHY side: 18000 ( 16 times less than actual value)  */

    uint32           am = 18000;

    ret = gpy2xx_usxgmii_alignmentmarker_set(&phy[gpy_phy_id], am);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_alignmentmarker_set fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
    osal_time_mdelay(100);

    if (phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_1 || \
        phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_2 || \
        phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_3 || \
        phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_4 || \
        phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_5)
    {
        /* 288K Alignment Marker for serdes 2 */
        if((hal_serdes_reg_set(unit, 2, 6, 18, 0x0B40)) != RT_ERR_OK)
            return RT_ERR_FAILED;

        /* Enable serdes 2 AN */
        if((hal_serdes_reg_set(unit, 2, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }

    if (phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_2 || \
        phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_3 || \
        phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_5)
    {
        /* 288K Alignment Marker for serdes 3 */
        if((hal_serdes_reg_set(unit, 3, 6, 18, 0x0B40)) != RT_ERR_OK)
            return RT_ERR_FAILED;

        /* Enable serdes 3 AN */
        if((hal_serdes_reg_set(unit, 3, 7, 17, 0x054F)) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }


    /* config TPI(Twisted pair interface) side auto-nego */
    phy[gpy_phy_id].link.autoneg = 1;
    phy[gpy_phy_id].link.advertising = phy[gpy_phy_id].link.supported;
    phy[gpy_phy_id].link.speed = SPEED_2500;
    phy[gpy_phy_id].link.duplex = DUPLEX_FULL;
    phy[gpy_phy_id].link.pause = 1;
    phy[gpy_phy_id].link.asym_pause = 1;

    ret = gpy2xx_config_aneg(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, Configuring Auto-Negotiation failed.\n", __func__, __LINE__);
    }


#if 0
    osal_time_mdelay(1000);

    /* read firmware version */
    ret = gpy2xx_read_fw_info(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: Reading FW info failed.\n", __func__, __LINE__);
    }
    else
    {
        struct gpy211_id phyId = phy[gpy_phy_id].id;

        osal_printf("\nPopulated PHY ID parameters:\n----------------\n");
        osal_printf("\t%40s:\t0x%x\n", "PHY Organizationally Unique Identifier", phyId.OUI);
        osal_printf("\t%40s:\t0x%x\n", "PHY Manufacturer's Model Number", phyId.model_no);
        osal_printf("\t%40s:\t0x%x\n", "PHY Revision Number", phyId.revision);
        osal_printf("\t%40s:\t0x%x\n", "Chip Manufacturer ID", phyId.manufacturer_id);
        osal_printf("\t%40s:\t0x%x\n", "Chip Part Number", phyId.part_no);
        osal_printf("\t%40s:\t0x%x\n", "Chip Version", phyId.version);
        osal_printf("\t%40s:\t0x%x\n", "Firmware Version", ((phyId.fw_release << 15) | (phyId.fw_major << 8) | (phyId.fw_minor)));
        osal_printf("\t%40s:\t0x%x\n", "Firmware Major Version Number", phyId.fw_major);
        osal_printf("\t%40s:\t0x%x\n", "Firmware Minor Version Number", phyId.fw_minor);
        osal_printf("\t%40s:\t0x%x\n", "Firmware Release Indication", phyId.fw_release);
        osal_printf("\t%40s:\t%s\n", "The memory target used for firmware execution", (phyId.fw_memory ? ((phyId.fw_memory == 1) ? "OTP" : ((phyId.fw_memory == 2) ? "FLASH" : "SRAM")) : "ROM"));
        osal_printf("\t%40s:\t0x%x\n", "Driver Major Version Number", phyId.drv_major);
        osal_printf("\t%40s:\t0x%x\n", "Driver Minor Version Number", phyId.drv_minor);
        osal_printf("\t%40s:\t0x%x\n", "Driver Release Indication", phyId.drv_release);
    }
#endif
    return RT_ERR_OK;
}

int32
phy_gpy2xx_eye_diagram_init(uint32 unit, rtk_port_t port)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d \n", __func__, __LINE__, unit);

    int32                        ret = 0;
    uint32                       gpy_phy_id;
    struct gpy211_usxgmii_reach  reach_cfg;

    // By board model
    if (phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_1 || \
        phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_4)
    {
        // By port
        if (port == 0)
        {
            /* Serdes 2 */
            memset(&reach_cfg, 0, sizeof(struct gpy211_usxgmii_reach));

            reach_cfg.trace_len = 0x03;
            reach_cfg.tx_eq_main = 0x14;
            reach_cfg.tx_eq_pre = 0x00;
            reach_cfg.tx_eq_post = 0x00;
            reach_cfg.tx_vboost_en = 0x01;
            reach_cfg.tx_vboost_lvl = 0x05;
            reach_cfg.tx_iboost_lvl = 0x0f;

            reach_cfg.rx_eq_att_lvl = 0x07;
            reach_cfg.rx_eq_vga1_gain = 0x00;
            reach_cfg.rx_eq_vga2_gain = 0x00;
            reach_cfg.rx_eq_ctle_boost = 0x00;
            reach_cfg.rx_eq_ctle_pole = 0x00;
            reach_cfg.rx_eq_dfe_tap1 = 0x00;
            reach_cfg.rx_afe_adapt_en = 0x00;
            reach_cfg.rx_dfe_adapt_en = 0x00;

            if(_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
                return RT_ERR_FAILED;

            ret = gpy2xx_usxgmii_reach_cfg(&phy[gpy_phy_id], &reach_cfg);
            if (ret < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_reach_cfg fail.\n", __func__, __LINE__);
                return RT_ERR_FAILED;
            }
            osal_time_mdelay(100);
        }
    }
    else if (phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_3 || \
             phy_board_type == RTL9302C_GPY2xx_BOARD_TYPE_5)
    {
        if (port == 0)
        {
            /* Serdes 2 */
            memset(&reach_cfg, 0, sizeof(struct gpy211_usxgmii_reach));

            reach_cfg.trace_len = 0x03;
            reach_cfg.tx_eq_main = 0x18;
            reach_cfg.tx_eq_pre = 0x00;
            reach_cfg.tx_eq_post = 0x00;
            reach_cfg.tx_vboost_en = 0x01;
            reach_cfg.tx_vboost_lvl = 0x05;
            reach_cfg.tx_iboost_lvl = 0x0f;

            reach_cfg.rx_eq_att_lvl = 0x07;
            reach_cfg.rx_eq_vga1_gain = 0x00;
            reach_cfg.rx_eq_vga2_gain = 0x00;
            reach_cfg.rx_eq_ctle_boost = 0x00;
            reach_cfg.rx_eq_ctle_pole = 0x00;
            reach_cfg.rx_eq_dfe_tap1 = 0x00;
            reach_cfg.rx_afe_adapt_en = 0x00;
            reach_cfg.rx_dfe_adapt_en = 0x00;

            if(_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
                return RT_ERR_FAILED;

            ret = gpy2xx_usxgmii_reach_cfg(&phy[gpy_phy_id], &reach_cfg);
            if (ret < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_reach_cfg fail.\n", __func__, __LINE__);
                return RT_ERR_FAILED;
            }
            osal_time_mdelay(100);
        }
        else if (port == 8)
        {
            /* Serdes 3 */
            memset(&reach_cfg, 0, sizeof(struct gpy211_usxgmii_reach));

            reach_cfg.trace_len = 0x03;
            reach_cfg.tx_eq_main = 0x3f;
            reach_cfg.tx_eq_pre = 0x00;
            reach_cfg.tx_eq_post = 0x00;
            reach_cfg.tx_vboost_en = 0x01;
            reach_cfg.tx_vboost_lvl = 0x05;
            reach_cfg.tx_iboost_lvl = 0x0f;

            reach_cfg.rx_eq_att_lvl = 0x07;
            reach_cfg.rx_eq_vga1_gain = 0x00;
            reach_cfg.rx_eq_vga2_gain = 0x00;
            reach_cfg.rx_eq_ctle_boost = 0x00;
            reach_cfg.rx_eq_ctle_pole = 0x00;
            reach_cfg.rx_eq_dfe_tap1 = 0x00;
            reach_cfg.rx_afe_adapt_en = 0x00;
            reach_cfg.rx_dfe_adapt_en = 0x00;

            if(_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
                return RT_ERR_FAILED;

            ret = gpy2xx_usxgmii_reach_cfg(&phy[gpy_phy_id], &reach_cfg);
            if (ret < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_reach_cfg fail.\n", __func__, __LINE__);
                return RT_ERR_FAILED;
            }
            osal_time_mdelay(100);
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_reset_set
 * Description:
 *      Set PHY standard register Reset bit (0.15).
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_gpy2xx_reset_set(uint32 unit, rtk_port_t port)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                              gpy_phy_id;
    int32                               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_soft_reset(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_soft_reset fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    /* reconfig auto-nego to default. */
    phy[gpy_phy_id].link.autoneg = 1;
    phy[gpy_phy_id].link.advertising = phy[gpy_phy_id].link.supported;
    phy[gpy_phy_id].link.speed = SPEED_2500;
    phy[gpy_phy_id].link.duplex = DUPLEX_FULL;
    phy[gpy_phy_id].link.pause = 1;
    phy[gpy_phy_id].link.asym_pause = 1;

    ret = gpy2xx_config_aneg(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, Configuring Auto-Negotiation failed.\n", __func__, __LINE__);
    }

    return RT_ERR_OK;
}

int32
phy_gpy2xx_swMacPollPhyStatus_get(uint32 unit, rtk_port_t port, rtk_port_swMacPollPhyStatus_t *pphyStatus)
{
    return RT_ERR_OK;
}

#if 0
/* Function Name:
 *      _phy_gpy2xx_gpy2xxSysIntfMode_to_rtkSdsMode
 * Description:
 *      Transfert BCM System Interface Mode to RTK SerDes Mode
 * Input:
 *      gpy2xxSysIntfOpMode - BCM System Interface Mode
 * Output:
 *      prtkSerdesMode  - rtk serdes mode
 * Return:
 *      None
 * Note:
 *      None
 */
int32
_phy_gpy2xx_gpy2xxSysIntfMode_to_rtkSdsMode(BCM_API_SysIntfOpMode gpy2xxSysIntfOpMode, rt_serdesMode_t *prtkSerdesMode)
{
    return RT_ERR_OK;

}
#endif


/* Function Name:
 *      phy_gpy2xx_media_get
 * Description:
 *      Get PHY media type.
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pMedia - pointer buffer of phy media type
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - invalid parameter
 *      RT_ERR_NULL_POINTER - input parameter is null pointer
 * Note:
 *      None
 */
int32
phy_gpy2xx_media_get(uint32 unit, rtk_port_t port, rtk_port_media_t *pMedia)
{
    *pMedia = PORT_MEDIA_COPPER;
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\n%s %d, unit:%d, port:%d, pMedia:%d \n", __func__, __LINE__, unit, port, *pMedia);
    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_autoNegoEnable_get
 * Description:
 *      Get autonegotiation enable status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_autoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32               gpy_phy_id;
    uint32               table_index;
    int32                gpy_aneg;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index(unit, port, &gpy_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 1)
    {
        *pEnable = DISABLED;
        return RT_ERR_OK;
    }

    gpy_aneg = phy[gpy_phy_id].link.autoneg;

    switch (gpy_aneg)
    {
        case ENABLED:
          *pEnable = ENABLED;
          break;
        case DISABLED:
          *pEnable = DISABLED;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, gpy_aneg:%d, get autonego invalid.\n", __func__, __LINE__, gpy_aneg);
          return RT_ERR_FAILED;
    }

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "get *pEnable:%d\n", *pEnable);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_autoNegoEnable_set
 * Description:
 *      Set autonegotiation enable status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_autoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, enable:%d\n", __func__, __LINE__, unit, port, enable);

    uint32                              gpy_phy_id;
    uint32                              table_index;
    int32                               gpy_aneg;
    int32                               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index(unit, port, &gpy_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;
#if 0
    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
#endif

    switch (enable)
    {
        case ENABLED:
          gpy_aneg = ENABLED;
          break;
        case DISABLED:
          gpy_aneg = DISABLED;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid aneg state:%d\n", __func__, __LINE__, enable);
          return RT_ERR_FAILED;
    }

    if (gpy_aneg == ENABLED)
    {
        senao_non_rtl_phy_info->port_mapping[table_index].force_auto = 0;

        phy[gpy_phy_id].link.autoneg = gpy_aneg;

        /* call: __gpy211_config_advert, __gpy211_restart_aneg */
        ret = gpy2xx_config_aneg(&phy[gpy_phy_id]);
        if (ret < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_config_aneg fail.\n", __func__, __LINE__);
            return RT_ERR_FAILED;
        }
    }
    else if (gpy_aneg == DISABLED)
    {
        senao_non_rtl_phy_info->port_mapping[table_index].force_auto = 1;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_autoNegoAbility_get
 * Description:
 *      Get ability advertisement for auto negotiation of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pAbility - pointer to PHY auto negotiation ability
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_autoNegoAbility_get(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                              gpy_phy_id;
    int64                               gpy_advertising = 0;
    
    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

#if 0
    char buf[256];
    print_advert(buf, sizeof(buf), phy[gpy_phy_id].link.advertising);
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\n%20s:\t%u (%s)\n\n", "  GPHY advertising modes", (unsigned int)phy[gpy_phy_id].link.advertising, buf);
#endif

    gpy_advertising = phy[gpy_phy_id].link.advertising;

    if (gpy_advertising & GPY2XX_ADVERTISED_10baseT_Half)
        pAbility->Half_10 = 1;
    else
        pAbility->Half_10 = 0;

    if (gpy_advertising & GPY2XX_ADVERTISED_10baseT_Full)
        pAbility->Full_10 = 1;
    else
        pAbility->Full_10 = 0;

    if (gpy_advertising & GPY2XX_ADVERTISED_100baseT_Half)
        pAbility->Half_100 = 1;
    else
        pAbility->Half_100 = 0;

    if (gpy_advertising & GPY2XX_ADVERTISED_100baseT_Full)
        pAbility->Full_100 = 1;
    else
        pAbility->Full_100 = 0;

    if (gpy_advertising & GPY2XX_ADVERTISED_1000baseT_Full)
        pAbility->Full_1000 = 1;
    else
        pAbility->Full_1000 = 0;

    if (gpy_advertising & GPY2XX_ADVERTISED_2500baseT_Full)
        pAbility->adv_2_5G = 1;
    else
        pAbility->adv_2_5G = 0;

    if (gpy_advertising & GPY2XX_ADVERTISED_Pause)
        pAbility->FC = 1;
    else
        pAbility->FC = 0;

    if (gpy_advertising & GPY2XX_ADVERTISED_Asym_Pause)
        pAbility->AsyFC = 1;
    else
        pAbility->AsyFC = 0;

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "gpy_advertising: %u\n", (unsigned int)gpy_advertising);
    return RT_ERR_OK;

}

/* Function Name:
 *      phy_gpy2xx_autoNegoAbility_set
 * Description:
 *      Set ability advertisement for auto negotiation of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 *      pAbility  - auto negotiation ability that is going to set to PHY
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_autoNegoAbility_set(uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                              gpy_phy_id;
    int64                               gpy_advertising = 0;
    int32                               sdsId;
    int32                               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

#if 0
    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
#endif

    if((sdsId = HWP_PORT_SDSID(unit, port)) != HWP_NONE)
    {
        if((HWP_SDS_MODE(unit, sdsId) != RTK_MII_USXGMII_10GSXGMII) && 
            (HWP_SDS_MODE(unit, sdsId) != RTK_MII_USXGMII_10GQXGMII))
        {

            if (pAbility->Half_100)
                gpy_advertising |= GPY2XX_ADVERTISED_100baseT_Half;
        }
    }

    if (pAbility->Full_10)
        gpy_advertising |= GPY2XX_ADVERTISED_10baseT_Full;

    if (pAbility->Full_100)
        gpy_advertising |= GPY2XX_ADVERTISED_100baseT_Full;

    if (pAbility->Full_1000)
        gpy_advertising |= GPY2XX_ADVERTISED_1000baseT_Full;

    if (pAbility->adv_2_5G)
        gpy_advertising |= GPY2XX_ADVERTISED_2500baseT_Full;

    if (pAbility->FC)
        gpy_advertising |= GPY2XX_ADVERTISED_Pause;

    if (pAbility->AsyFC)
        gpy_advertising |= GPY2XX_ADVERTISED_Asym_Pause;

    phy[gpy_phy_id].link.advertising = gpy_advertising;

    /* call: __gpy211_config_advert, __gpy211_restart_aneg */
    ret = gpy2xx_config_aneg(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, ERROR: gpy2xx_config_aneg fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

#if 0
    char buf[256];
    print_advert(buf, sizeof(buf), phy[gpy_phy_id].link.advertising);
    //osal_printf("\n%20s:\t%u (%s)\n", "  GPHY advertising modes", (unsigned int)phy[gpy_phy_id].link.advertising, buf);
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\n%20s:\t%u (%s)\n", "  GPHY advertising modes", (unsigned int)phy[gpy_phy_id].link.advertising, buf);
#endif

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "gpy_advertising: %u\n", (unsigned int)gpy_advertising);
    return RT_ERR_OK;

}

/* Function Name:
 *      phy_gpy2xx_duplex_get
 * Description:
 *      Set duplex mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      duplex        - duplex mode of the port, full or half
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_duplex_get(uint32 unit, rtk_port_t port, uint32 *pDuplex)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    int32                   gpy_duplex = 0;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    gpy_duplex = phy[gpy_phy_id].link.duplex;

    switch (gpy_duplex)
    {
        case DUPLEX_FULL:
          *pDuplex = PORT_FULL_DUPLEX;
          break;
        case DUPLEX_HALF:
          *pDuplex = PORT_HALF_DUPLEX;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, get invalid duplex:%d\n", __func__, __LINE__, gpy_duplex);
          return RT_ERR_FAILED;
    }

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "get *pDuplex:%d\n", *pDuplex);
    return RT_ERR_OK;

}

/* Function Name:
 *      phy_gpy2xx_duplex_set
 * Description:
 *      Set duplex mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      duplex        - duplex mode of the port, full or half
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_duplex_set(uint32 unit, rtk_port_t port, uint32 duplex)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, duplex:%d \n", __func__, __LINE__, unit, port, duplex);

    uint32                              gpy_phy_id;
    int32                               gpy_duplex = 0;
    int32                               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

#if 0
    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
#endif

    switch(duplex)
    {
        case PORT_FULL_DUPLEX:
          gpy_duplex = DUPLEX_FULL;
          break;
        case PORT_HALF_DUPLEX:
          gpy_duplex = DUPLEX_HALF;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid input duplex:%d\n", __func__, __LINE__, duplex);
          return RT_ERR_FAILED;
    }

    phy[gpy_phy_id].link.duplex = gpy_duplex;

    /* call: __gpy211_config_advert, __gpy211_restart_aneg */
    ret = gpy2xx_config_aneg(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_config_aneg fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_speed_get
 * Description:
 *      Get link speed status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      pSpeed - pointer to PHY link speed
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_speed_get(uint32 unit, rtk_port_t port, uint32 *pSpeed)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d,\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    uint32                  table_index;
    int32                   gpy_speed = 0;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index(unit, port, &gpy_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 1)
    {
        *pSpeed = senao_non_rtl_phy_info->port_mapping[table_index].speed;
        return RT_ERR_OK;
    }

    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    gpy_speed = phy[gpy_phy_id].link.speed;

    switch (gpy_speed)
    {
        case SPEED_2500:
          *pSpeed = PORT_SPEED_2_5G;
          break;
        case SPEED_1000:
          *pSpeed = PORT_SPEED_1000M;
          break;
        case SPEED_100:
          *pSpeed = PORT_SPEED_100M;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, gpy_speed:%d, get speed invalid.\n", __func__, __LINE__, gpy_speed);
          return RT_ERR_FAILED;
    }

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "get *pSpeed: %d\n", *pSpeed);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_speed_set
 * Description:
 *      Set speed mode status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      speed         - link speed status 100
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK                 - OK
 *      RT_ERR_FAILED             - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_speed_set(uint32 unit, rtk_port_t port, uint32 speed)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, speed:%d \n", __func__, __LINE__, unit, port, speed);

    uint32              gpy_phy_id;
    uint32              table_index;
    int32               gpy_speed = 0;
    int64               gpy_advertising = 0;
    int32               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort_with_table_index(unit, port, &gpy_phy_id, &table_index) != RT_ERR_OK)
        return RT_ERR_FAILED;
#if 0
    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
#endif

#if 1
    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, Speed cannot be set if auto-negotiation is enabled.\n", __func__, __LINE__);
        PHY_GPY_ERR("Speed cannot be set if auto-negotiation is enabled\n");
        return RT_ERR_FAILED;
    }

    switch(speed)
    {
        case PORT_SPEED_10M:
          gpy_speed = SPEED_10;
          gpy_advertising = (GPY2XX_ADVERTISED_100baseT_Full | GPY2XX_ADVERTISED_Pause | GPY2XX_ADVERTISED_Asym_Pause);
          break;
          
        case PORT_SPEED_100M:
          gpy_speed = SPEED_100;
          gpy_advertising = (GPY2XX_ADVERTISED_100baseT_Full | GPY2XX_ADVERTISED_Pause | GPY2XX_ADVERTISED_Asym_Pause);
          break;
        case PORT_SPEED_1000M:
          gpy_speed = SPEED_1000;
          gpy_advertising = (GPY2XX_ADVERTISED_1000baseT_Full | GPY2XX_ADVERTISED_Pause | GPY2XX_ADVERTISED_Asym_Pause);
          break;
        case PORT_SPEED_2_5G:
          gpy_speed = SPEED_2500;
          gpy_advertising = (GPY2XX_ADVERTISED_2500baseT_Full | GPY2XX_ADVERTISED_Pause | GPY2XX_ADVERTISED_Asym_Pause);
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid input speed:%d\n", __func__, __LINE__, speed);
          return RT_ERR_FAILED;
    }

    /* GPY force mode is to lock ability and restart auto-nego. */
    if (speed == PORT_SPEED_100M || speed == PORT_SPEED_10M) {
      phy[gpy_phy_id].link.autoneg = 0;
    } else
      phy[gpy_phy_id].link.autoneg = 1;
    phy[gpy_phy_id].link.speed = gpy_speed;
    phy[gpy_phy_id].link.advertising = gpy_advertising;

    /* call: __gpy211_config_advert, __gpy211_restart_aneg */
    if ( (speed == PORT_SPEED_100M || speed == PORT_SPEED_10M) && phy[gpy_phy_id].link.autoneg == 0)
    {
      ret = PHY_READ(&phy[gpy_phy_id], STD_STD_CTRL);
      // force 100mF 
      ret &= ~(1<< STD_CTRL_SSM_POS | 1<< STD_CTRL_SSL_POS);
      
      if (speed == PORT_SPEED_100M)
        ret |=  (1<< STD_CTRL_DPLX_POS | 1<< STD_CTRL_SSL_POS);
      else  // 10MF
        ret |=  (1<< STD_CTRL_DPLX_POS);
      
      PHY_WRITE(&phy[gpy_phy_id], STD_STD_CTRL, ret | 1<< STD_CTRL_PD_POS);
      osal_time_mdelay (800);
      PHY_WRITE(&phy[gpy_phy_id], STD_STD_CTRL, (ret | 1<< STD_CTRL_PD_POS) & ~(1<< STD_CTRL_ANEN_POS));
      PHY_WRITE(&phy[gpy_phy_id], STD_STD_CTRL, (ret  & ~(1<< STD_CTRL_ANEN_POS)));      
    } else {
      ret = gpy2xx_config_aneg(&phy[gpy_phy_id]);
    }
	
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_config_aneg fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    senao_non_rtl_phy_info->port_mapping[table_index].speed = speed;
    return RT_ERR_OK;

#else

    hal_control_t       *pHalCtrl;
    uint32              data;

    if(senao_non_rtl_phy_info->port_mapping[table_index].force_auto == 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, Speed cannot be set if auto-negotiation is enabled.\n", __func__, __LINE__);
        PHY_GPY_ERR("Speed cannot be set if auto-negotiation is enabled\n");
        return RT_ERR_FAILED;
    }

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if (speed == PORT_SPEED_10M) /* ANEG to 10M full duplex */
    {
        data = 0x2000; /*disable auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x4002; /*disable 2.5G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x20, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0; /*disable 1G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x9, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0C41; /*disable 100F, 100H, 10H ability, reserve only 10F duplex*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x4, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x3240; /*restart auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
    }
    else if (speed == PORT_SPEED_100M) /* ANEG to 100M full duplex */
    {
        data = 0x2000; /*disable auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x4002; /*disable 2.5G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x20, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0; /*disable 1G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x9, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0D01; /*disable 100H, 10F, 10H ability, reserve only 100F duplex*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x4, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x3240; /*restart auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
    }
    else if (speed == PORT_SPEED_1000M) /* ANEG to 1G full duplex */
    {
        data = 0x2000; /*disable auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x4002; /*disable 2.5G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x20, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0300; /*enable 1G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x9, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0C01; /*disable 100F, 100H, 10F, 10H ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x4, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x3240; /*restart auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
    }
    else if (speed == PORT_SPEED_2_5G) /* ANEG to 2.5G full duplex */
    {
        data = 0x2000; /*disable auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x40A2; /*enable 2.5G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x7, 0x20, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0; /*disable 1G ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x9, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x0C01; /*disable 100F, 100H, 10F, 10H ability*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x4, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
        data = 0x3240; /*restart auto-negotiation*/
        if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x0, data)) != RT_ERR_OK)
        {
            PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
            PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
            return ret;
        }
    }

    osal_time_mdelay(3000);
    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
    else
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, gpy2xx_read_status success.\n", __func__, __LINE__);

        struct gpy211_link linkConfig = phy[gpy_phy_id].link;
        char buf[256];

        osal_printf("\n\nPopulated link configuration parameters:\n----------------\n");
        osal_printf("\t%40s:\t%s\n", "Auto-Negotiation (AN) state", linkConfig.autoneg ? "Enabled" : "Disabled");
        osal_printf("\t%40s:\t%d\n", "Link Speed or Partner Link Speed (AN)", linkConfig.speed);
        osal_printf("\t%40s:\t%s\n", "Duplex (Forced) or Partner Duplex (AN)", linkConfig.duplex ? "Full" : "Half");
        osal_printf("\t%40s:\t%d\n", "Partner Pause (AN)", linkConfig.pause);
        osal_printf("\t%40s:\t%d\n", "Partner Asym-pause (AN)", linkConfig.asym_pause);
        osal_printf("\t%40s:\t%s\n", "Most recently read link state", linkConfig.link ? "Up" : "Down");
        print_advert(buf, sizeof(buf), linkConfig.supported);
        osal_printf("\t%40s:\t%u (%s)\n", "GPHY supported modes", (unsigned int)linkConfig.supported, buf);
        print_advert(buf, sizeof(buf), linkConfig.advertising);
        osal_printf("\t%40s:\t%u (%s)\n", "GPHY advertising modes", (unsigned int)linkConfig.advertising, buf);
        print_advert(buf, sizeof(buf), linkConfig.lp_advertising);
        osal_printf("\t%40s:\t%u (%s)\n", "Link partner advertising modes", (unsigned int)linkConfig.lp_advertising, buf);
    }

    senao_non_rtl_phy_info->port_mapping[table_index].speed = speed;
    return RT_ERR_OK;
#endif
}

/* Function Name:
 *      phy_gpy2xx_enable_set
 * Description:
 *      Set PHY interface status of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 *      enable        - admin configuration of PHY interface
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, enable:%d \n", __func__, __LINE__, unit, port, enable);

    uint32       gpy_phy_id;
    uint32       runtime_init = INIT_NOT_COMPLETED;
    int32        gpy_power = DISABLED;
    int32        ret = 0;

    runtime_rtnic_init_state_get(&runtime_init);

    if (runtime_init == INIT_COMPLETED)
    {
        switch (enable)
        {
          case ENABLED:
            gpy_power = ENABLED;
            break;
          case DISABLED:
            gpy_power = DISABLED;
            break;
          default:
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid enable:%d\n", __func__, __LINE__, enable);
            return RT_ERR_FAILED;
        }
    }

    if(_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (gpy_power == ENABLED)
    {
        if ((ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
            return RT_ERR_FAILED;
        }
    }
    else if (gpy_power == DISABLED)
    {
        if ((ret = PHY_SET_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
            return RT_ERR_FAILED;
        }
    }

    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "unit:%d, port:%d, enable:%d, enable set success.\n", unit, port, enable);
    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_eeeEnable_get
 * Description:
 *      Get enable status of EEE function in the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 * Output:
 *      pEnable - pointer to enable status of EEE
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
int32
phy_gpy2xx_eeeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    hal_control_t      *pHalCtrl;
    uint32              reg0d24;
    uint32              gpy_phy_id;
    int32               gpy_eee;
    int32               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x0, 0x18, &reg0d24)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg0d24:0x%x, read PHY_MIISTAT fail.\n", __func__, __LINE__, reg0d24);
        PHY_GPY_ERR("U%u P%u mmd0x0 reg0x18 read fail", unit, port);
        return ret;
    }
    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "reg0d24:0x%x", reg0d24);

    gpy_eee = (reg0d24 & PHY_MIISTAT_EEE_MASK) >> PHY_MIISTAT_EEE_POS;

    *pEnable = (gpy_eee) ? ENABLED: DISABLED;

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "get *pEnable:%d\n", *pEnable);
    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_eeeEnable_set
 * Description:
 *      Set enable status of EEE function in the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - enable status of EEE
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_gpy2xx_eeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, enable:%d \n", __func__, __LINE__, unit, port, enable);

    uint32              gpy_phy_id;
    uint32              runtime_init = INIT_NOT_COMPLETED;
    int32               gpy_eee = DISABLED;
    int32               ret = 0;
    // Starry@20220218 fixed Port disable and reboot, port still enable status
    int32               GetPhyPortStatus = 0;

    runtime_rtnic_init_state_get(&runtime_init);

    if (runtime_init == INIT_COMPLETED)
    {
        switch (enable)
        {
          case ENABLED:
            gpy_eee = ENABLED;
            break;
          case DISABLED:
            gpy_eee = DISABLED;
            break;
          default:
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid eee enable:%d\n", __func__, __LINE__, enable);
            return RT_ERR_FAILED;
        }
    }

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;
    if (PHY_READ(&phy[gpy_phy_id], STD_STD_CTRL) &  (1<<STD_CTRL_PD_POS))
      GetPhyPortStatus = 0;
    else
      GetPhyPortStatus = 1;

    /* power down */
    if ((ret = PHY_SET_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
        PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
        return RT_ERR_FAILED;
    }

    if (gpy_eee == ENABLED)
    {
        /* Register 7.60.1 for capability of EEE under 100M. */
        if ((ret = PHY_SET_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_EEE_AN_ADV1, ANEG_EEE_AN_ADV1_EEE_100BTX_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d eee set fail", unit, port, enable);

            /* power up */
            if (GetPhyPortStatus && (ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
                PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
                return RT_ERR_FAILED;
            }
            return RT_ERR_FAILED;
        }

        /* Register 7.60.2 for capability of EEE under 1G. */
        if ((ret = PHY_SET_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN ,ANEG_EEE_AN_ADV1, ANEG_EEE_AN_ADV1_EEE_1000BT_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d eee set fail", unit, port, enable);

            /* power up */
            if (GetPhyPortStatus &&(ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
                PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
                return RT_ERR_FAILED;
            }
            return RT_ERR_FAILED;
        }

        /* Register 7.62.0 for capability of EEE under 2.5G. */
        if ((ret = PHY_SET_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_EEE_AN_ADV2, ANEG_EEE_AN_ADV2_EEE2G5_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d eee set fail", unit, port, enable);

            /* power up */
            if (GetPhyPortStatus && (ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
                PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
                return RT_ERR_FAILED;
            }
            return RT_ERR_FAILED;
        }
    }
    else if (gpy_eee == DISABLED)
    {
        /* Register 7.60.1 for capability of EEE under 100M. */
        if ((ret = PHY_CLR_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_EEE_AN_ADV1, ANEG_EEE_AN_ADV1_EEE_100BTX_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d eee set fail", unit, port, enable);

            /* power up */
            if (GetPhyPortStatus && (ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
                PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
                return RT_ERR_FAILED;
            }
            return RT_ERR_FAILED;
        }

        /* Register 7.60.2 for capability of EEE under 1G. */
        if ((ret = PHY_CLR_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_EEE_AN_ADV1, ANEG_EEE_AN_ADV1_EEE_1000BT_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d eee set fail", unit, port, enable);

            /* power up */
            if (GetPhyPortStatus && (ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
                PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
                return RT_ERR_FAILED;
            }
            return RT_ERR_FAILED;
        }

        /* Register 7.62.0 for capability of EEE under 2.5G. */
        if ((ret = PHY_CLR_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_EEE_AN_ADV2, ANEG_EEE_AN_ADV2_EEE2G5_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
            PHY_GPY_ERR("U%u P%u enable:%d eee set fail", unit, port, enable);

            /* power up */
            if (GetPhyPortStatus && (ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
            {
                PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
                PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
                return RT_ERR_FAILED;
            }
            return RT_ERR_FAILED;
        }
    }

    /* power up */
    if (GetPhyPortStatus && (ret = PHY_CLR_BIT(&phy[gpy_phy_id], STD_STD_CTRL, STD_CTRL_PD_POS)) < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, enable:%d, enable set fail.\n", __func__, __LINE__, unit, port, enable);
        PHY_GPY_ERR("U%u P%u enable:%d set fail", unit, port, enable);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_crossOverMode_get
 * Description:
 *      Get cross over mode in the specified port.
 * Input:
 *      unit  - unit id
 *      port  - port id
 * Output:
 *      pMode - pointer to cross over mode
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      Following value is valid
 *      - PORT_CROSSOVER_MODE_AUTO
 *      - PORT_CROSSOVER_MODE_MDI
 *      - PORT_CROSSOVER_MODE_MDIX
 */
int32
phy_gpy2xx_crossOverMode_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t *pMode)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    hal_control_t      *pHalCtrl;
    uint32              reg0d19 = 0;
    uint32              gpy_phy_id;
    int32               gpy_auto, gpy_ab, gpy_cd;
    int32               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x0, 0x13, &reg0d19)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg0d19:0x%x, read PHY_CTL1 fail.\n", __func__, __LINE__, reg0d19);
        PHY_GPY_ERR("U%u P%u mmd0x0 reg0x13 read fail", unit, port);
        return ret;
    }
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "reg0d19:0x%x\n", reg0d19);

    gpy_auto = (reg0d19 & PHY_CTL1_AMDIX_MASK) >> PHY_CTL1_AMDIX_POS;
    gpy_ab = (reg0d19 & PHY_CTL1_MDIAB_MASK) >> PHY_CTL1_MDIAB_POS;
    gpy_cd = (reg0d19 & PHY_CTL1_MDICD_MASK) >> PHY_CTL1_MDICD_POS;

    if (gpy_auto)
    {
        *pMode = PORT_CROSSOVER_MODE_AUTO;
    }
    else if ((gpy_ab == 1) && (gpy_cd == 1))
    {
        *pMode = PORT_CROSSOVER_MODE_MDIX;
    }
    else if ((gpy_ab == 0) && (gpy_cd == 0))
    {
        *pMode = PORT_CROSSOVER_MODE_MDI;
    }
    else
    {
        PHY_GPY211_DBG(PHY_GPY211_WARNING_DBG, "%s %d, ports A&B and ports C&D value is not the same.\n", __func__, __LINE__);
        PHY_GPY211_DBG(PHY_GPY211_WARNING_DBG, "%s %d, ports A&B:%d, ports C&D:%d\n", __func__, __LINE__, gpy_ab, gpy_cd);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_crossOverMode_set
 * Description:
 *      Set cross over mode in the specified port.
 * Input:
 *      unit - unit id
 *      port - port id
 *      mode - cross over mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 *      RT_ERR_PORT_ID - invalid port id
 *      RT_ERR_INPUT   - invalid input parameter
 * Note:
 *      Following value is valid
 *      - PORT_CROSSOVER_MODE_AUTO
 *      - PORT_CROSSOVER_MODE_MDI
 *      - PORT_CROSSOVER_MODE_MDIX
 */
int32
phy_gpy2xx_crossOverMode_set(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t mode)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, mode:%d\n", __func__, __LINE__, unit, port, mode);

    uint32              gpy_phy_id;
    int32               gpy_auto = ENABLED;
    int32               gpy_mdix = DISABLED;
    int32               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    switch (mode)
    {
        case PORT_CROSSOVER_MODE_AUTO:
          gpy_auto = ENABLED;
          break;
        case PORT_CROSSOVER_MODE_MDI:
          gpy_auto = DISABLED;
          gpy_mdix = DISABLED;
          break;
        case PORT_CROSSOVER_MODE_MDIX:
          gpy_auto = DISABLED;
          gpy_mdix = ENABLED;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid crossover mode:%d\n", __func__, __LINE__, mode);
          return RT_ERR_FAILED;
    }

    if (gpy_auto == ENABLED) /* Auto mode */
    {
        /* Register 0.19.0 for Auto and Manual MDI/MDI-X switching. */
        if ((ret = PHY_SET_BIT(&phy[gpy_phy_id], PHY_PHY_CTL1, PHY_CTL1_AMDIX_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, mode:%d, auto mdi/mdix set fail.\n", __func__, __LINE__, unit, port, mode);
            PHY_GPY_ERR("U%u P%u mode:%d auto mdi/mdix set fail", unit, port, mode);
            return RT_ERR_FAILED;
        }
    }
    else if ((gpy_auto == DISABLED) && (gpy_mdix == DISABLED)) /* Manaul mode MDI */
    {
        if ((ret = PHY_CLR_BIT(&phy[gpy_phy_id], PHY_PHY_CTL1, PHY_CTL1_AMDIX_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, mode:%d, manual mdi/mdix set fail.\n", __func__, __LINE__, unit, port, mode);
            PHY_GPY_ERR("U%u P%u mode:%d manual mdi/mdix set fail", unit, port, mode);
            return RT_ERR_FAILED;
        }
        /* Register 0.19.2 for Manual Ports A and B MDI/MDI-X switching. */
        if ((ret = PHY_CLR_BIT(&phy[gpy_phy_id], PHY_PHY_CTL1, PHY_CTL1_MDIAB_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, mode:%d, mdi portA portB set fail.\n", __func__, __LINE__, unit, port, mode);
            PHY_GPY_ERR("U%u P%u mode:%d mdi portA portB set fail", unit, port, mode);
            return RT_ERR_FAILED;
        }
        /* Register 0.19.3 for Manual Ports C and D MDI/MDI-X switching. */
        if ((ret = PHY_CLR_BIT(&phy[gpy_phy_id], PHY_PHY_CTL1, PHY_CTL1_MDICD_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, mode:%d, mdi portC portD set fail.\n", __func__, __LINE__, unit, port, mode);
            PHY_GPY_ERR("U%u P%u mode:%d mdi portC portD set fail", unit, port, mode);
            return RT_ERR_FAILED;
        }
    }
    else if ((gpy_auto == DISABLED) && (gpy_mdix == ENABLED)) /* Manaul mode MDIX */
    {
        if ((ret = PHY_CLR_BIT(&phy[gpy_phy_id], PHY_PHY_CTL1, PHY_CTL1_AMDIX_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, mode:%d, manual mdi/mdix set fail.\n", __func__, __LINE__, unit, port, mode);
            PHY_GPY_ERR("U%u P%u mode:%d manual mdi/mdix set fail", unit, port, mode);
            return RT_ERR_FAILED;
        }

        if ((ret = PHY_SET_BIT(&phy[gpy_phy_id], PHY_PHY_CTL1, PHY_CTL1_MDIAB_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, mode:%d, mdix portA portB set fail.\n", __func__, __LINE__, unit, port, mode);
            PHY_GPY_ERR("U%u P%u mode:%d mdix portA portB set fail", unit, port, mode);
            return RT_ERR_FAILED;
        }

        if ((ret = PHY_SET_BIT(&phy[gpy_phy_id], PHY_PHY_CTL1, PHY_CTL1_MDICD_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, mode:%d, mdix portC portD set fail.\n", __func__, __LINE__, unit, port, mode);
            PHY_GPY_ERR("U%u P%u mode:%d mdix portC portD set fail", unit, port, mode);
            return RT_ERR_FAILED;
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_crossOverStatus_get
 * Description:
 *      Get cross over status in the specified port.
 * Input:
 *      unit  - unit id
 *      port  - port id
 * Output:
 *      pMode - pointer to cross over mode status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 *      RT_ERR_PHY_FIBER_LINKUP - This feature is not supported in this mode
 * Note:
 *      Following value is valid
 *      - PORT_CROSSOVER_STATUS_MDI
 *      - PORT_CROSSOVER_STATUS_MDIX
 */
int32
phy_gpy2xx_crossOverStatus_get(uint32 unit, rtk_port_t port, rtk_port_crossOver_status_t *pStatus)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    hal_control_t      *pHalCtrl;
    uint32              reg1d130 = 0;
    uint32              gpy_phy_id;
    int32               gpy_mdix;
    int32               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    /* Register 1.130.[1:0] for the status of pair swaps at the MDI/MDI-X */
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x1, 0x82, &reg1d130)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg1d130:0x%x, read PMA_MGBT_POLARITY fail.\n", __func__, __LINE__, reg1d130);
        PHY_GPY_ERR("U%u P%u mmd0x1 reg0x82 read fail", unit, port);
        return ret;
    }
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "reg1d130:0x%x\n", reg1d130);

    gpy_mdix = (reg1d130 & PMA_MGBT_POLARITY_MDI_MDI_X_MASK) >> PMA_MGBT_POLARITY_MDI_MDI_X_POS;

    if (gpy_mdix == 0x3)
    {
        *pStatus = PORT_CROSSOVER_STATUS_MDI;
    }
    else
    {
        *pStatus = PORT_CROSSOVER_STATUS_MDIX;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_reg_mmd_get
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mmdAddr - mmd device address
 *      mmdReg  - mmd reg id
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
phy_gpy2xx_reg_mmd_get(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 *pData)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d, mmdAddr:0x%x, mmdReg:0x%x \n", __func__, __LINE__, unit, port, mmdAddr, mmdReg);
    PHY_GPY_LOG("mmdAddr 0x%X mmdReg 0x%X mdio read api", mmdAddr, mmdReg);

    hal_control_t   *pHalCtrl;
    int32            ret = 0;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, mmdAddr, mmdReg, pData)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ret=%d mido read fail.\n", __func__, __LINE__, ret);
        PHY_GPY_ERR("U%u P%u mmdAddr:0x%X mmdReg:0x%X read fail", unit, port, mmdAddr, mmdReg);
        return ret;
    }

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "pData=0x%x, mdio read success.\n\n", *pData);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_reg_mmd_set
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mmdAddr - mmd device address
 *      mmdReg  - mmd reg id
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
phy_gpy2xx_reg_mmd_set(uint32 unit, rtk_port_t port, uint32 mmdAddr, uint32 mmdReg, uint32 data)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d, mmdAddr:0x%x, mmdReg:0x%x \n", __func__, __LINE__, unit, port, mmdAddr, mmdReg);
    PHY_GPY_LOG("mmdAddr 0x%X mmdReg 0x%X mdio write api", mmdAddr, mmdReg);

    hal_control_t   *pHalCtrl;
    int32            ret = 0;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, mmdAddr, mmdReg, data)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ret=%d mido write fail \n", __func__, __LINE__, ret);
        PHY_GPY_ERR("U%u P%u mmdAddr:0x%X mmdReg:0x%X write fail", unit, port, mmdAddr, mmdReg);
        return ret;
    }

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, ">> %s %d, mdio write success\n\n", __func__, __LINE__);
    return RT_ERR_OK;

}

/* Function Name:
 *      phy_gpy2xx_masterSlave_get
 * Description:
 *      Get PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 * Output:
 *      pMasterSlaveCfg     - pointer to the PHY master slave configuration
 *      pMasterSlaveActual  - pointer to the PHY master slave actual link status
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      This function only works on giga/ 10g port to get its master/slave mode configuration.
 */
int32
phy_gpy2xx_masterSlave_get(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   *pMasterSlaveCfg,
    rtk_port_masterSlave_t   *pMasterSlaveActual)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    hal_control_t      *pHalCtrl;
    uint32              reg0d24 = 0;
    uint32              reg7d32 = 0;
    uint32              gpy_phy_id;
    int32               gpy_aneg, gpy_mstslv_cfg, gpy_mstslv_sts;
    int32               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x0, 0x18, &reg0d24)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg0d24:0x%x, read PHY_MIISTAT fail.\n", __func__, __LINE__, reg0d24);
        PHY_GPY_ERR("U%u P%u mmd0x0 reg0x18 read fail", unit, port);
        return ret;
    }
    // PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "reg0d24:0x%x\n", reg0d24);

    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0x7, 0x20, &reg7d32)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg7d32:0x%x, read PHY_MIISTAT fail.\n", __func__, __LINE__, reg7d32);
        PHY_GPY_ERR("U%u P%u mmd0x7 reg0x20 read fail", unit, port);
        return ret;
    }

    gpy_aneg = (reg7d32 & ANEG_MGBT_AN_CTRL_MS_MAN_EN_MASK) >> ANEG_MGBT_AN_CTRL_MS_MAN_EN_POS;
    gpy_mstslv_cfg = (reg7d32 & ANEG_MGBT_AN_CTRL_MSCV_MASK) >> ANEG_MGBT_AN_CTRL_MSCV_POS;
    gpy_mstslv_sts = (reg0d24 & PHY_MIISTAT_MSRES_MASK) >> PHY_MIISTAT_MSRES_POS;

    *pMasterSlaveActual = (gpy_mstslv_sts) ? PORT_MASTER_MODE: PORT_SLAVE_MODE;
    
    if (gpy_aneg == 0)
    {
        *pMasterSlaveCfg = PORT_AUTO_MODE;
    }
    else if ((gpy_aneg == 1) && (gpy_mstslv_cfg == 1))
    {
        *pMasterSlaveCfg = PORT_MASTER_MODE;
    }
    else if ((gpy_aneg == 1) && (gpy_mstslv_cfg == 0))
    {
        *pMasterSlaveCfg = PORT_SLAVE_MODE;
    }
    else
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, masterSlave Cfg get fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_masterSlave_set
 * Description:
 *      Set PHY configuration of master/slave mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      masterSlave         - PHY master slave configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_gpy2xx_masterSlave_set(
    uint32              unit,
    rtk_port_t          port,
    rtk_port_masterSlave_t   masterSlave)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, masterSlave:%d\n", __func__, __LINE__, unit, port, masterSlave);

    uint32              gpy_phy_id;
    int32               gpy_aneg = ENABLED;
    int32               gpy_mstslv = DISABLED;
    int32               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    switch (masterSlave)
    {
        case PORT_AUTO_MODE:
          gpy_aneg = ENABLED;
          break;
        case PORT_MASTER_MODE:
          gpy_aneg = DISABLED;
          gpy_mstslv = ENABLED;
          break;
        case PORT_SLAVE_MODE:
          gpy_aneg = DISABLED;
          gpy_mstslv = DISABLED;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid masterSlave mode:%d\n", __func__, __LINE__, masterSlave);
          return RT_ERR_FAILED;
    }

    if (gpy_aneg == ENABLED) /* Master Slave ANEG mode */
    {
        /* Register 7.32.15 for Master Slave config setting. */
        if ((ret = PHY_CLR_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_MGBT_AN_CTRL, ANEG_MGBT_AN_CTRL_MS_MAN_EN_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, masterSlave:%d, masterSlave aneg set fail.\n", __func__, __LINE__, unit, port, masterSlave);
            PHY_GPY_ERR("U%u P%u masterSlave:%d masterSlave aneg set fail", unit, port, masterSlave);
            return RT_ERR_FAILED;
        }
    }
    else if ((gpy_aneg == DISABLED) && (gpy_mstslv = ENABLED)) /* Manual Master mode */
    {
        if ((ret = PHY_SET_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_MGBT_AN_CTRL, ANEG_MGBT_AN_CTRL_MS_MAN_EN_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, masterSlave:%d, masterSlave manual set fail.\n", __func__, __LINE__, unit, port, masterSlave);
            PHY_GPY_ERR("U%u P%u masterSlave:%d masterSlave manual set fail", unit, port, masterSlave);
            return RT_ERR_FAILED;
        }
        if ((ret = PHY_SET_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_MGBT_AN_CTRL, ANEG_MGBT_AN_CTRL_MSCV_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, masterSlave:%d, master manual set fail.\n", __func__, __LINE__, unit, port, masterSlave);
            PHY_GPY_ERR("U%u P%u masterSlave:%d master manual set fail", unit, port, masterSlave);
            return RT_ERR_FAILED;
        }
    }
    else if ((gpy_aneg == DISABLED) && (gpy_mstslv = DISABLED)) /* Manual Slave mode */
    {
        if ((ret = PHY_SET_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_MGBT_AN_CTRL, ANEG_MGBT_AN_CTRL_MS_MAN_EN_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, masterSlave:%d, masterSlave manual set fail.\n", __func__, __LINE__, unit, port, masterSlave);
            PHY_GPY_ERR("U%u P%u masterSlave:%d masterSlave manual set fail", unit, port, masterSlave);
            return RT_ERR_FAILED;
        }
        if ((ret = PHY_CLR_MMD_BIT(&phy[gpy_phy_id], MDIO_MMD_AN, ANEG_MGBT_AN_CTRL, ANEG_MGBT_AN_CTRL_MSCV_POS)) < 0)
        {
            PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, unit:%d, port:%d, masterSlave:%d, slave manual set fail.\n", __func__, __LINE__, unit, port, masterSlave);
            PHY_GPY_ERR("U%u P%u masterSlave:%d slave manual set fail", unit, port, masterSlave);
            return RT_ERR_FAILED;
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_loopback_get
 * Description:
 *      Get PHY Loopback mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      pEnable           -loopback mode status;
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_gpy2xx_loopback_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    hal_control_t          *pHalCtrl;
    uint32                  reg0d0 = 0;
    uint32                  gpy_phy_id;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }
    
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, 0, 0, &reg0d0)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, reg0d0:0x%x, read STD_CTRL fail.\n", __func__, __LINE__, reg0d0);
        PHY_GPY_ERR("U%u P%u mmd0x0 reg0x0 read fail", unit, port);
        return ret;
    }
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, reg0d0:0x%x, read STD_CTRL success.\n", __func__, __LINE__, reg0d0);

    /* Register 0.0.14  loopback mode */
    *pEnable = (reg0d0 & STD_CTRL_LB_MASK) ? ENABLED: DISABLED;

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "get loopback mode:%d\n", *pEnable);
    return RT_ERR_OK;

}

/* Function Name:
 *      phy_gpy2xx_loopback_set
 * Description:
 *      Set PHY Loopback mode of the specific port
 * Input:
 *      unit                - unit id
 *      port                - port id
 *      enable              - ENABLED: Enable loopback;
 *                            DISABLED: Disable loopback. PHY back to normal operation.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_gpy2xx_loopback_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, enable:%d\n", __func__, __LINE__, unit, port, enable);

    uint32                  gpy_phy_id;
    int32                   ret = 0;
    enum gpy211_test_loop   loopMode = TLOOP_OFF;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    switch (enable)
    {
        case ENABLED:
          /* TLOOP_NETl=1, TLOOP_FETl=2, TLOOP_ECHO=3, TLOOP_RJTl=4, TLOOP_FELTS=5, TLOOP_NETLI=8 */
          loopMode = TLOOP_NETLI;
          break;
        case DISABLED:
          loopMode = TLOOP_OFF;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid loopback state:%d\n", __func__, __LINE__, enable);
          return RT_ERR_FAILED;
    }

    ret = gpy2xx_loopback_cfg(&phy[gpy_phy_id], loopMode);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_loopback_cfg fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_rtctResult_get
 * Description:
 *      Get test result of RTCT.
 * Input:
 *      unit        - unit id
 *      port        - the port for retriving RTCT test result
 * Output:
 *      pRtctResult - RTCT result
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT     - The module is not initial
 *      RT_ERR_UNIT_ID      - invalid unit id
 *      RT_ERR_PORT_ID      - invalid port id
 *      RT_ERR_PHY_RTCT_NOT_FINISH   - RTCT not finish. Need to wait a while.
 *      RT_ERR_TIMEOUT      - RTCT test timeout in this port.
 *      RT_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      The result unit is cm
 */
int32
phy_gpy2xx_rtctResult_get(uint32 unit, rtk_port_t port, rtk_rtctResult_t *pRtctResult)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                      gpy_phy_id;
    int32                       ret = 0;
    int32                       i = 0;
    uint8                       valid_Results[4];
    uint8                       cable_Length[4];
    uint8                       pair_State[4];
    struct gpy211_cdiag_report  cdiagReport;

    memset(&cdiagReport, 0, sizeof(struct gpy211_cdiag_report));

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

#if 0
    int32                       gpy_link;
    int32                       gpy_speed;

    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_read_status fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
    gpy_link = phy[gpy_phy_id].link.link;
    gpy_speed = phy[gpy_phy_id].link.speed;
#endif

    ret = gpy2xx_cdiag_read(&phy[gpy_phy_id], &cdiagReport);
    if (ret < 0) 
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: CDIAG test result read failed.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

#if 1 /*show detail info*/
    uint8         pairState, cableLength, validResults;

    printf("\nReturned values:\n----------------\n");
    for (i = 0; i < 4; i++) {
        printf("\nThe CDIAG test results for DSP = %s.\n", ((i == 0) ? "A" : (i == 1) ? "B" : (i == 2) ? "C" : "D"));
        printf("\nTotal valid diagnostic reports = %d.\n", cdiagReport.pair[i].num_valid_result);
        validResults = cdiagReport.pair[i].num_valid_result;

        if (validResults) {
            cableLength = cdiagReport.pair[i].results[validResults - 1].distance - cdiagReport.pair[i].results[0].distance;
            printf("\t%40s:\t%d\n", "Estimated distance of cable (in meters)", cableLength);
            pairState = cdiagReport.pair[i].results[validResults - 1].state;
            printf("\t%40s:\t%s\n", "Detected echo", ((pairState == 1) ? "REFLECTION" : (pairState == 2) ? "OPEN" : (pairState == 4) ? "SHORT" : (pairState == 8) ? "MATCHED" : "ERROR"));
            printf("\t%40s:\t%d\n", "Echo coefficient of first detected peak", (s16)(cdiagReport.pair[i].results[validResults - 1].peak));
        }

        int k = 0;
        for (k = 0; k < 3; k++) {
            printf("\tThe sum-of-square of all XC%1d coefficients :\t%d\n", k, cdiagReport.pair[i].xc_pwr[k]);
        }
    }
#endif

#if 0
    if (!gpy_link)
        pRtctResult->linkType = PORT_SPEED_1000M;

    else if (gpy_speed == SPEED_100)
        pRtctResult->linkType = PORT_SPEED_100M;

    else if (gpy_speed == SPEED_1000)
        pRtctResult->linkType = PORT_SPEED_1000M;

    else if (gpy_speed == SPEED_2500)
        pRtctResult->linkType = PORT_SPEED_2_5G;
#else

    /* only return 1000M linktype (the same as 826x) */
    pRtctResult->linkType = PORT_SPEED_1000M;
#endif


    for (i = 0; i < 4; i++)
    {
        valid_Results[i] = cdiagReport.pair[i].num_valid_result;
        if (valid_Results[i])
        {
            cable_Length[i] = cdiagReport.pair[i].results[valid_Results[i] - 1].distance - cdiagReport.pair[i].results[0].distance;
            pair_State[i] = cdiagReport.pair[i].results[valid_Results[i] - 1].state;
        }
    }

    /*pair state 2 - open
      pair state 4 - short*/

    /*PairA State*/    
    if (pair_State[0] == 2)
        pRtctResult->un.ge_result.channelAOpen = 1;
    else if (pair_State[0] == 4)
        pRtctResult->un.ge_result.channelAShort = 1;

    /*PairB State*/
    if (pair_State[1] == 2)
        pRtctResult->un.ge_result.channelBOpen = 1;
    else if (pair_State[1] == 4)
        pRtctResult->un.ge_result.channelBShort = 1;

    /*PairC State*/
    if (pair_State[2] == 2)
        pRtctResult->un.ge_result.channelCOpen = 1;
    else if (pair_State[2] == 4)
        pRtctResult->un.ge_result.channelCShort = 1;

    /*PairD State*/
    if (pair_State[3] == 2)
        pRtctResult->un.ge_result.channelDOpen = 1;
    else if (pair_State[3] == 4)
        pRtctResult->un.ge_result.channelDShort = 1;

    /*PairA Length*/
    pRtctResult->un.ge_result.channelALen = (uint32) cable_Length[0]*100;
    /*PairB Length*/
    pRtctResult->un.ge_result.channelBLen = (uint32) cable_Length[1]*100;
    /*PairC Length*/
    pRtctResult->un.ge_result.channelCLen = (uint32) cable_Length[2]*100;
    /*PairD Length*/
    pRtctResult->un.ge_result.channelDLen = (uint32) cable_Length[3]*100;


    return RT_ERR_OK;
}

/* Function Name:
 *      phy_gpy2xx_rtct_start
 * Description:
 *      Start PHY interface RTCT test of the specific port
 * Input:
 *      unit          - unit id
 *      port          - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_CHIP_NOT_SUPPORTED - chip not supported
 * Note:
 *      None
 */
int32
phy_gpy2xx_rtct_start(uint32 unit, rtk_port_t port)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    hal_control_t              *pHalCtrl;
    uint32                      reg0d31; 
    uint32                      gpy_phy_id;
    int32                       ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;


    ret = gpy2xx_cdiag_start(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: Set Cable diagnostic (CDIAG) start failed.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }
    /* delay */
    osal_time_mdelay (10000);


    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, pHalCtrl get fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u pHalCtrl get fail", unit, port);
        return RT_ERR_FAILED;
    }

    reg0d31 = 0x0; /* leave cdiag test mode */
    if ((ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, 0x0, 0x1F, reg0d31)) != RT_ERR_OK)
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, mido write fail \n", __func__, __LINE__);
        PHY_GPY_ERR("U%u P%u mdio write fail", unit, port);
        return ret;
    }

    ret = gpy2xx_restart_aneg(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_restart_aneg fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      phy_gpy2xx_ieeeTestMode_set
 * Description:
 *      Set test mode for PHY transmitter test
 * Input:
 *      unit  - unit id
 *      port  - port id
 *      pTestMode->mode  - test mode RTK_PORT_PHY_1G_TEST_MODEx which is defined in IEEE 40.6.1.1.2 for 1G.
 *                         RTK_PORT_PHY_10G_TEST_MODEx is for 10G test.
 *                         RTK_PORT_PHY_100M_TEST_MODE_xx is for 100M test.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_NOT_ALLOWED - The operation is not allowed
 *      RT_ERR_PORT_NOT_SUPPORTED - test mode is not supported
 * Note:
 *      RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS is not supported
 */
int32
phy_gpy2xx_ieeeTestMode_set(uint32 unit, rtk_port_t port, rtk_port_phyTestMode_t *pTestMode)
{
#if 1
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\n[%s]: L[%d]: Enter(unit=%u, port=%u, TestMode=%u)\n", __func__, __LINE__, unit, port, pTestMode->mode);
    return RT_ERR_OK;
#else

    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                              gpy_phy_id;
    int32                               ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;


    return RT_ERR_OK;
#endif
}

int32
phy_gpy2xx_portEyeMonitor_start(uint32 unit, rtk_port_t port, uint32 sdsId, uint32 frameNum)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\n[%s]: L[%d]: Enter(unit=%u, port=%u, sdsId=%u, frameNum=%u)\n", __func__, __LINE__, unit, port, sdsId, frameNum);
    return RT_ERR_OK;
}


/* Function Name:
*      phy_gpy2xx_linkStatus_get
* Description:
*      Get PHY link status from standard register (1.2).
* Input:
*      unit    - unit id
*      port    - port id
* Output:
*      pStatus - pointer to the link status
* Return:
*      RT_ERR_OK
*      RT_ERR_FAILED
* Note:
*      The Link Status bit (Status Register 1.2) has LL (Latching Low) attribute
*      for link failure. Please refer IEEE 802.3 for detailed.
*/
int32
phy_gpy2xx_linkStatus_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pStatus)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d,\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    int32                   gpy_link = 0;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_update_link(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_update_link fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    gpy_link = phy[gpy_phy_id].link.link;

    *pStatus = (gpy_link) ? PORT_LINKUP: PORT_LINKDOWN;

    return RT_ERR_OK;
}

/* Function Name:
*      phy_gpy2xx_usxgmii_linkStatus_get
* Description:
*      Get Maxlinear GPY PHY's MAC side USXGMII serdes link status
* Input:
*      unit    - unit id
*      port    - port id
* Output:
*      None
* Return:
*      RT_ERR_OK
*      RT_ERR_FAILED
* Note:
*        None
*/
int32
phy_gpy2xx_usxgmii_linkStatus_get(uint32 unit, rtk_port_t port)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d,\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;


/*  GPY_v2.7.1.1_EarlyRelease  */

/* 1. check aneg status */
    struct gpy211_usxgmii_aneg_status aneg_status;
    memset(&aneg_status, 0, sizeof(struct gpy211_usxgmii_aneg_status));

    osal_printf("\n\t----- 1. USXGMII aneg status -----\n");

    ret = gpy2xx_usxgmii_aneg_sts(&phy[gpy_phy_id], &aneg_status);

    if (ret < 0) {
        osal_printf("\nERROR:Egr gpy2xx_usxgmii_aneg_sts failed.\n");
    }
    else
    {
        osal_printf("\t%40s:\t0x%x\n", "phy_addr", phy[gpy_phy_id].phy_addr);
        osal_printf("\t%40s:\t%s\n", "phy_side", aneg_status.phy_side ? "usxgmii_phy" : "usxgmii_mac");
        osal_printf("\t%40s:\t%s\n", "force_mode", aneg_status.force_mode ? "Enabled" : "Disabled");
        osal_printf("\t%40s:\t%s\n", "aneg_enable", aneg_status.aneg_enable ? "Enabled" : "Disabled");
        osal_printf("\t%40s:\t%s\n", "aneg_complete", aneg_status.aneg_complete ? "Complete" : "Not Complete");
        osal_printf("\t%40s:\t%s\n", "aneg_linksts", aneg_status.aneg_linksts ? "Link Up" : "Link Down");
        osal_printf("\t%40s:\t%d\n", "aneg_speed", aneg_status.aneg_speed);
        osal_printf("\t%40s:\t%s\n", "aneg_duplex", aneg_status.aneg_duplex ? "Full Duplex" : "Half Duplex");
        osal_printf("\t%40s:\t%s\n", "mii_linksts", aneg_status.mii_linksts ? "Link Up" : "Link Down");
        osal_printf("\t%40s:\t%d\n", "mii_speed", aneg_status.mii_speed);
        osal_printf("\t%40s:\t%s\n", "mii_duplex", aneg_status.mii_duplex ? "Full Duplex" : "Half Duplex");
    }



/* 2. check mii link status */
    struct gpy211_usxgmii_mii_linksts mii_linkstatus;
    memset(&mii_linkstatus, 0, sizeof(struct gpy211_usxgmii_mii_linksts));

    osal_printf("\n\t----- 2. USXGMII mii link status -----\n");

    ret = gpy2xx_usxgmii_mii_linksts(&phy[gpy_phy_id], &mii_linkstatus);

    if (ret < 0) {
        osal_printf("\nERROR:Egr gpy2xx_usxgmii_mii_linksts failed.\n");
    }
    else
    {
        osal_printf("\t%40s:\t0x%x\n", "phy_addr", phy[gpy_phy_id].phy_addr);
        osal_printf("\t%40s:\t%s\n", "latched_linksts", mii_linkstatus.latched_linksts ? "Link Up" : "Link Down");
        osal_printf("\t%40s:\t%s\n", "linksts", mii_linkstatus.linksts ? "Link Up" : "Link Down");
    }



/* 3. check pcs pma status */
    struct gpy211_usxgmii_pcs_pma_linksts pcspma_linkstatus;
    memset(&pcspma_linkstatus, 0, sizeof(struct gpy211_usxgmii_pcs_pma_linksts));

    osal_printf("\n\t----- 3. USXGMII pcs pma link status -----\n");

    ret = gpy2xx_usxgmii_pcs_pma_linksts(&phy[gpy_phy_id], &pcspma_linkstatus);

    if (ret < 0) {
        osal_printf("\nERROR:Egr gpy2xx_usxgmii_pcs_pma_linksts failed.\n");
    }
    else
    {
        osal_printf("\t%40s:\t%s\n", "pma_latched_linksts", pcspma_linkstatus.pma_latched_linksts ? "Link Up" : "Link Down");
        osal_printf("\t%40s:\t%s\n", "pma_linksts", pcspma_linkstatus.pma_linksts ? "Link Up" : "Link Down");
        osal_printf("\t%40s:\t%s\n", "pcs_latched_linksts", pcspma_linkstatus.pcs_latched_linksts ? "Link Up" : "Link Down");
        osal_printf("\t%40s:\t%s\n", "pcs_linksts", pcspma_linkstatus.pcs_linksts ? "Link Up" : "Link Down");  
    }



/* 4. check alignmentmarker */
    u32 am = 0;

    osal_printf("\n\t----- 4. USXGMII alignmentmarker -----\n");

    ret = gpy2xx_usxgmii_alignmentmarker_get(&phy[gpy_phy_id], &am);

    if (ret < 0) {
        osal_printf("\nERROR:Egr gpy2xx_usxgmii_alignmentmarker_get failed.\n");
    }
    else
    {
        osal_printf("\t%40s:\t0x%x\n", "usxgmii_am_count", am);
    }



/* 5. check usxgmii reach setting */
    struct gpy211_usxgmii_reach reach_cfg;
    memset(&reach_cfg, 0, sizeof(struct gpy211_usxgmii_reach));

    osal_printf("\n\t----- 5. USXGMII reach setting -----\n");

    ret = gpy2xx_usxgmii_reach_get(&phy[gpy_phy_id], &reach_cfg);

    if (ret < 0) {
        osal_printf("\nERROR:Egr usxgmii reach get failed.\n");
    }
    else
    {
        osal_printf("\t%40s:\t%s\n", "trace_len", (reach_cfg.trace_len) == 0 ? "SHORT" : ((reach_cfg.trace_len) == 1 ? "MEDIUM" : ((reach_cfg.trace_len) == 2 ? "LONG" : "CUSTOM")));
        osal_printf("\t%40s:\t0x%x\n", "tx_eq_main", reach_cfg.tx_eq_main);
        osal_printf("\t%40s:\t0x%x\n", "tx_eq_pre", reach_cfg.tx_eq_pre);
        osal_printf("\t%40s:\t0x%x\n", "tx_eq_post", reach_cfg.tx_eq_post);
        osal_printf("\t%40s:\t%s\n", "tx_vboost_en", (reach_cfg.tx_vboost_en == 0) ? "Disabled" : "Enabled");
        osal_printf("\t%40s:\t0x%x\n", "tx_vboost_lvl", reach_cfg.tx_vboost_lvl);
        osal_printf("\t%40s:\t0x%x\n", "tx_iboost_lvl", reach_cfg.tx_iboost_lvl);
        osal_printf("\t%40s:\t0x%x\n", "rx_eq_att_lvl", reach_cfg.rx_eq_att_lvl);
        osal_printf("\t%40s:\t0x%x\n", "rx_eq_vga1_gain", reach_cfg.rx_eq_vga1_gain);
        osal_printf("\t%40s:\t0x%x\n", "rx_eq_vga2_gain", reach_cfg.rx_eq_vga2_gain);
        osal_printf("\t%40s:\t0x%x\n", "rx_eq_ctle_boost", reach_cfg.rx_eq_ctle_boost);
        osal_printf("\t%40s:\t0x%x\n", "rx_eq_ctle_pole", reach_cfg.rx_eq_ctle_pole);
        osal_printf("\t%40s:\t0x%x\n", "rx_eq_dfe_tap1", reach_cfg.rx_eq_dfe_tap1);
        osal_printf("\t%40s:\t%s\n", "rx_afe_adapt_en", (reach_cfg.rx_afe_adapt_en == 0) ? "Disabled" : "Enabled");
        osal_printf("\t%40s:\t%s\n", "rx_dfe_adapt_en", (reach_cfg.rx_dfe_adapt_en == 0) ? "Disabled" : "Enabled");
    }



/* 6. check vr reset status */
    int success = 0;

    osal_printf("\n\t----- 6. vr reset complete status -----\n");

    ret = gpy2xx_usxgmii_vr_reset_sts(&phy[gpy_phy_id], &success);

    if (ret < 0) {
        osal_printf("\nERROR:Egr gpy2xx_usxgmii_vr_reset_sts failed.\n");
    }
    else
    {
        osal_printf("\t%40s:\t%s\n", "VR reset ", success ? "complete/sucessfull" : "not complete/unsucessfull");
    }


/* 7. check TPI side link status */

    osal_printf("\n\t----- 7. TPI side link status -----\n");

    ret = gpy2xx_read_status(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        printf("\nERROR: Read link status failed.\n");
    }
    else
    {
        PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "%s %d, gpy2xx_read_status success.\n", __func__, __LINE__);

        struct gpy211_link linkConfig = phy[gpy_phy_id].link;
        char buf[256];

        osal_printf("\nPopulated link configuration parameters:\n----------------\n");
        osal_printf("\t%40s:\t%s\n", "Auto-Negotiation (AN) state", linkConfig.autoneg ? "Enabled" : "Disabled");
        osal_printf("\t%40s:\t%d\n", "Link Speed or Partner Link Speed (AN)", linkConfig.speed);
        osal_printf("\t%40s:\t%s\n", "Duplex (Forced) or Partner Duplex (AN)", linkConfig.duplex ? "Full" : "Half");
        osal_printf("\t%40s:\t%d\n", "Partner Pause (AN)", linkConfig.pause);
        osal_printf("\t%40s:\t%d\n", "Partner Asym-pause (AN)", linkConfig.asym_pause);
        osal_printf("\t%40s:\t%s\n", "Most recently read link state", linkConfig.link ? "Up" : "Down");
        print_advert(buf, sizeof(buf), linkConfig.supported);
        osal_printf("\t%40s:\t%u (%s)\n", "GPHY supported modes", (unsigned int)linkConfig.supported, buf);
        print_advert(buf, sizeof(buf), linkConfig.advertising);
        osal_printf("\t%40s:\t%u (%s)\n", "GPHY advertising modes", (unsigned int)linkConfig.advertising, buf);
        print_advert(buf, sizeof(buf), linkConfig.lp_advertising);
        osal_printf("\t%40s:\t%u (%s)\n", "Link partner advertising modes", (unsigned int)linkConfig.lp_advertising, buf);
    }

    return RT_ERR_OK;
}

int32
phy_gpy2xx_usxgmii_vr_reset(uint32 unit, rtk_port_t port)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d,\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_usxgmii_vr_reset(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_vr_reset fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    osal_printf("vr reset success.\n");
    return RT_ERR_OK;
}

int32
phy_gpy2xx_usxgmii_aneg_reset(uint32 unit, rtk_port_t port)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d,\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_usxgmii_aneg_rst(&phy[gpy_phy_id]);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_aneg_rst fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    osal_printf("aneg reset success.\n");
    return RT_ERR_OK;
}

const int8 EYE_DIAG_PARAM_STR[][40] =
{
  "0.trace_len(0x0-0x03) ",
  "1.tx_eq_main(0x0-0x3F) ",
  "2.tx_eq_pre(0x0-0x3F) ",
  "3.tx_eq_post(0x0-0x3F) ",
  "4.tx_vboost_en(0x0-0x1) ",
  "5.tx_vboost_lvl(0x0-0x7) ",
  "6.tx_iboost_lvl(0x0-0x7) ",
  "7.rx_eq_att_lvl(0x0-0x7) ",
  "8.rx_eq_vga1_gain(0x0-0xF) ",
  "9.rx_eq_vga2_gain(0x0-0xF) ",
  "10.rx_eq_ctle_boost(0x0-0x1F) ",
  "11.rx_eq_ctle_pole(0x0-0x7) ",
  "12.rx_eq_dfe_tap1(0x0-0xFF) ",
  "13.rx_afe_adapt_en(0x0-0x1) ",
  "14.rx_dfe_adapt_en(0x0-0x1) "
};
int32 phy_gpy2xx_usxgmii_reach_get(uint32 unit, rtk_port_t port, uint32 *pBuff)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d,\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    uint32                  Item = 0;
    int32                   ret = 0;

    struct gpy211_usxgmii_reach reach_cfg;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
      return RT_ERR_FAILED;

    ret = gpy2xx_usxgmii_reach_get(&phy[gpy_phy_id], &reach_cfg);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_reach_get fail.\n", __func__, __LINE__);
        osal_printf("ERROR: gpy2xx_usxgmii_reach_get fail\n");
        return RT_ERR_FAILED;
    }

    pBuff[Item++] = reach_cfg.trace_len;
    pBuff[Item++] = reach_cfg.tx_eq_main;
    pBuff[Item++] = reach_cfg.tx_eq_pre;
    pBuff[Item++] = reach_cfg.tx_eq_post;
    pBuff[Item++] = reach_cfg.tx_vboost_en;
    pBuff[Item++] = reach_cfg.tx_vboost_lvl;
    pBuff[Item++] = reach_cfg.tx_iboost_lvl;

    pBuff[Item++] = reach_cfg.rx_eq_att_lvl;
    pBuff[Item++] = reach_cfg.rx_eq_vga1_gain;
    pBuff[Item++] = reach_cfg.rx_eq_vga2_gain;
    pBuff[Item++] = reach_cfg.rx_eq_ctle_boost;
    pBuff[Item++] = reach_cfg.rx_eq_ctle_pole;
    pBuff[Item++] = reach_cfg.rx_eq_dfe_tap1;

    pBuff[Item++] = reach_cfg.rx_afe_adapt_en;
    pBuff[Item++] = reach_cfg.rx_dfe_adapt_en;

#if 1
    osal_printf("\nPort %d USXGMII Reach: \n", port);
    Item = 0;

    osal_printf("%s[0x%02x]=> ", EYE_DIAG_PARAM_STR[Item], pBuff[Item]);
    if ((pBuff[0] & 0x3) == 0)
    {
        osal_printf("Short\n");
    }
    else if ((pBuff[0] & 0x3) == 1)
    {
        osal_printf("Medium\n");
    }
    else if ((pBuff[0] & 0x3) == 2)
    {
        osal_printf("Long\n");
    }
    else if ((pBuff[0] & 0x3) == 3)
    {
        osal_printf("Custom\n");
    }

    for (Item = 1; Item <= 14; Item++)
    {
        osal_printf("%s[0x%02x]\n", EYE_DIAG_PARAM_STR[Item], pBuff[Item]);
    }
    osal_printf("\n");
#endif

    return RT_ERR_OK;
}

int32 phy_gpy2xx_usxgmii_reach_set(uint32 unit, rtk_port_t port, uint32 *pBuff)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\nEnter >> %s %d, unit:%d, port:%d,\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    uint32                  Item = 0;
    int32                   ret = 0;
    struct gpy211_usxgmii_reach reach_cfg;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
      return RT_ERR_FAILED;

    reach_cfg.trace_len = pBuff[Item++];
    reach_cfg.tx_eq_main = pBuff[Item++];
    reach_cfg.tx_eq_pre = pBuff[Item++];
    reach_cfg.tx_eq_post = pBuff[Item++];
    reach_cfg.tx_vboost_en = pBuff[Item++];
    reach_cfg.tx_vboost_lvl = pBuff[Item++];
    reach_cfg.tx_iboost_lvl = pBuff[Item++];

    reach_cfg.rx_eq_att_lvl = pBuff[Item++];
    reach_cfg.rx_eq_vga1_gain = pBuff[Item++];
    reach_cfg.rx_eq_vga2_gain = pBuff[Item++];
    reach_cfg.rx_eq_ctle_boost = pBuff[Item++];
    reach_cfg.rx_eq_ctle_pole = pBuff[Item++];
    reach_cfg.rx_eq_dfe_tap1 = pBuff[Item++];

    reach_cfg.rx_afe_adapt_en = pBuff[Item++];
    reach_cfg.rx_dfe_adapt_en = pBuff[Item++];


    ret = gpy2xx_usxgmii_reach_cfg(&phy[gpy_phy_id], &reach_cfg);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_reach_cfg fail.\n", __func__, __LINE__);
        osal_printf("ERROR: gpy2xx_usxgmii_reach_cfg fail\n");
        return RT_ERR_FAILED;
    }

    osal_printf("\ngpy2xx_usxgmii_reach_cfg success.\n");
    return RT_ERR_OK;
}

int32 phy_gpy2xx_usxgmii_loopback_get(uint32 unit, rtk_port_t port, uint32 *pmode)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                  gpy_phy_id;
    int32                   ret = 0;
    enum gpy211_usxgmii_loopback_mode usxgmii_loopmode = USXGMII_LOOPBACK_DISABLE;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_usxgmii_loopback_get(&phy[gpy_phy_id], &usxgmii_loopmode);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_loopback_get fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    *pmode = usxgmii_loopmode;

    osal_printf("get usxgmii loopback mode : %d\n", *pmode);
    return RT_ERR_OK;
}

int32 phy_gpy2xx_usxgmii_loopback_cfg(uint32 unit, rtk_port_t port, uint32 mode)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, mode:%d\n", __func__, __LINE__, unit, port, mode);

    uint32                  gpy_phy_id;
    int32                   ret = 0;
    enum gpy211_usxgmii_loopback_mode usxgmii_loopmode = USXGMII_LOOPBACK_DISABLE;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    switch (mode)
    {
        case 0:
          usxgmii_loopmode = USXGMII_LOOPBACK_DISABLE;
          break;
        case 1:
          usxgmii_loopmode = USXGMII_LOOPBACK_TX2RX;
          break;
        case 2:
          usxgmii_loopmode = USXGMII_LOOPBACK_RX2TX;
          break;
        default:
          PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, set invalid usxgmii loopback state:%d\n", __func__, __LINE__, mode);
          return RT_ERR_FAILED;
    }

    ret = gpy2xx_usxgmii_loopback_cfg(&phy[gpy_phy_id], usxgmii_loopmode);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_loopback_cfg fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    osal_printf("set usxgmii loopback mode : %d\n", usxgmii_loopmode);
    return RT_ERR_OK;
}

int32 phy_gpy2xx_usxgmii_alignmentmarker_get(uint32 unit, rtk_port_t port, uint32 *pam)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d\n", __func__, __LINE__, unit, port);

    uint32                  usxgmii_am = 0;
    uint32                  gpy_phy_id;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_usxgmii_alignmentmarker_get(&phy[gpy_phy_id], &usxgmii_am);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_alignmentmarker_get fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    *pam = usxgmii_am;

    osal_printf("get usxgmii alignmentmarker : %d\n", *pam);
    return RT_ERR_OK;
}

int32 phy_gpy2xx_usxgmii_alignmentmarker_set(uint32 unit, rtk_port_t port, uint32 am)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "Enter >> %s %d, unit:%d, port:%d, am:%d\n", __func__, __LINE__, unit, port, am);

    uint32                  gpy_phy_id;
    int32                   ret = 0;

    if (_phy_gpy2xx_rtkPort_to_gpy2xxPort(unit, port, &gpy_phy_id) != RT_ERR_OK)
        return RT_ERR_FAILED;

    ret = gpy2xx_usxgmii_alignmentmarker_set(&phy[gpy_phy_id], am);
    if (ret < 0)
    {
        PHY_GPY211_DBG(PHY_GPY211_ERR_DBG, "%s %d, ERROR: gpy2xx_usxgmii_alignmentmarker_set fail.\n", __func__, __LINE__);
        return RT_ERR_FAILED;
    }

    osal_printf("set usxgmii alignmentmarker : %d\n", am);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_cust1drv_mapperInit
 * Description:
 *      Initialize PHY driver.
 * Input:
 *      pPhydrv - pointer of phy driver
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
phy_gpy2xx_mapperInit(rt_phydrv_t *pPhydrv)
{
    PHY_GPY211_DBG(PHY_GPY211_PORTING_DBG, "\n[%s]: L[%d]: Enter()\n", __func__, __LINE__);
    pPhydrv->phydrv_index = RT_PHYDRV_GPY2xx;
    pPhydrv->fPhydrv_init = phy_gpy2xx_init;
    pPhydrv->fPhydrv_media_get = phy_gpy2xx_media_get;
    pPhydrv->fPhydrv_autoNegoEnable_get = phy_gpy2xx_autoNegoEnable_get;
    pPhydrv->fPhydrv_autoNegoEnable_set = phy_gpy2xx_autoNegoEnable_set;
    pPhydrv->fPhydrv_autoNegoAbility_get = phy_gpy2xx_autoNegoAbility_get;
    pPhydrv->fPhydrv_autoNegoAbility_set = phy_gpy2xx_autoNegoAbility_set;
    pPhydrv->fPhydrv_duplex_get = phy_gpy2xx_duplex_get;
    pPhydrv->fPhydrv_duplex_set = phy_gpy2xx_duplex_set;
    pPhydrv->fPhydrv_speed_get = phy_gpy2xx_speed_get;
    pPhydrv->fPhydrv_speed_set = phy_gpy2xx_speed_set;
    pPhydrv->fPhydrv_enable_set = phy_gpy2xx_enable_set;

    pPhydrv->fPhydrv_reg_mmd_get = phy_gpy2xx_reg_mmd_get;
    pPhydrv->fPhydrv_reg_mmd_set = phy_gpy2xx_reg_mmd_set;
    pPhydrv->fPhydrv_eeeEnable_get = phy_gpy2xx_eeeEnable_get;
    pPhydrv->fPhydrv_eeeEnable_set = phy_gpy2xx_eeeEnable_set;
    pPhydrv->fPhydrv_crossOverMode_get = phy_gpy2xx_crossOverMode_get;
    pPhydrv->fPhydrv_crossOverMode_set = phy_gpy2xx_crossOverMode_set;
    pPhydrv->fPhydrv_crossOverStatus_get = phy_gpy2xx_crossOverStatus_get;
    pPhydrv->fPhydrv_masterSlave_get = phy_gpy2xx_masterSlave_get;
    pPhydrv->fPhydrv_masterSlave_set = phy_gpy2xx_masterSlave_set;
    pPhydrv->fPhydrv_loopback_get = phy_gpy2xx_loopback_get;
    pPhydrv->fPhydrv_loopback_set = phy_gpy2xx_loopback_set;
    pPhydrv->fPhydrv_rtctResult_get = phy_gpy2xx_rtctResult_get;
    pPhydrv->fPhydrv_rtct_start =  phy_gpy2xx_rtct_start;
    pPhydrv->fPhydrv_ieeeTestMode_set = phy_gpy2xx_ieeeTestMode_set;
    pPhydrv->fPhydrv_portEyeMonitor_start = phy_gpy2xx_portEyeMonitor_start;
    
    pPhydrv->fPhydrv_linkStatus_get = phy_gpy2xx_linkStatus_get;
    pPhydrv->fPhydrv_reset_set = phy_gpy2xx_reset_set;
}
