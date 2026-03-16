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
 * $Revision: 76857 $
 * $Date: 2017-03-23 16:32:16 +0800 (Thu, 23 Mar 2017) $
 *
 * Purpose : MIIM service APIs in the SDK.
 *
 * Feature : MIIM service APIs
 *
 */

/*
 * Include Files
 */
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <hal/common/miim.h>
#include <hal/common/halctrl.h>
#include <hal/phy/phydef.h>
#if defined(CONFIG_SDK_RTL8212B) || defined(CONFIG_SDK_RTL8214B) || defined(CONFIG_SDK_RTL8214FB)
#include <hal/phy/phy_rtl8214fb.h>
#endif
#if defined(CONFIG_SDK_RTL8218B) || defined(CONFIG_SDK_RTL8218FB) || defined(CONFIG_SDK_RTL8214FC)
#include <hal/phy/phy_rtl8218b.h>
#endif
#include <hal/mac/drv/drv.h>

/*
 * Macro Definition
 */


/*
 * Function Declaration
 */

/* Function Name:
 *      hal_miim_read
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      phy_reg - PHY register
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_read(
    uint32      unit,
    rtk_port_t  port,
    uint32      page,
    uint32      phy_reg,
    uint32      *pData)
{
    int32   ret = RT_ERR_FAILED;
    uint32  access_page, fixed_page;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, page=0x%x, phy_reg=0x%x", unit, port, page, phy_reg);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_read)
        return RT_ERR_FAILED;

    if (NULL == pHalCtrl->pPhy_ctrl[port])
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_read(unit, port, page, phy_reg, pData);
        return ret;
    }

    fixed_page = HAL_MIIM_PAGE_ID_MAX(unit);
    access_page = page;

    if (access_page > fixed_page)
    {
        /* Use currently page access mechanism */
        MACDRV(pHalCtrl)->fMdrv_miim_write(unit, port, PHY_PAGE_0, PHY_PAGE_SELECTION_REG, access_page);
        access_page = fixed_page;
    }

    ret = MACDRV(pHalCtrl)->fMdrv_miim_read(unit, port, access_page, phy_reg, pData);

#if !defined(__MODEL_USER__)
#if defined(CONFIG_SDK_RTL8212B) || defined(CONFIG_SDK_RTL8214B) || defined(CONFIG_SDK_RTL8214FB)
    if ((PHY_MODEL_ID_RTL8214B == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9) &&
        ((access_page == PHY_PAGE_0) || (access_page>=7 && access_page<=15)))
    {
        uint32  auto_1000f_shadow = 0;

        if (((*pData)&0x0200) == 0)
        {
            if ((ret = phy_8214fb_auto_1000f_get(unit, port, &auto_1000f_shadow)) != RT_ERR_OK)
            {
                RT_LOG(LOG_WARNING, MOD_HAL, "phy_8214fb_auto_1000f_get(unit=%d, port=%d) failed", unit, port);
                return ret;
            }
            (*pData) |= (auto_1000f_shadow<<9);
        }
    }
#endif

#if defined(CONFIG_SDK_RTL8218B) || defined(CONFIG_SDK_RTL8218FB) || defined(CONFIG_SDK_RTL8214FC)
    if ((PHY_MODEL_ID_RTL8218B_EXT == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9))
    {
        uint32  auto_1000f_shadow = 0;

        if (((*pData)&0x0200) == 0)
        {
            if ((ret = phy_8218b_auto_1000f_get(unit, port, &auto_1000f_shadow)) != RT_ERR_OK)
            {
                RT_LOG(LOG_WARNING, MOD_HAL, "phy_8218b_auto_1000f_get(unit=%d, port=%d) failed", unit, port);
                return ret;
            }
            (*pData) |= (auto_1000f_shadow<<9);
        }
    }
#endif
#endif

    /* E0007313 */
    if (((PHY_MODEL_ID_RTL8218B_EXT == pHalCtrl->pPhy_ctrl[port]->phy_model_id)) && (access_page != fixed_page))
    {
        MACDRV(pHalCtrl)->fMdrv_miim_write(unit, port, PHY_PAGE_0, PHY_PAGE_SELECTION_REG, 0);
    }
    /* End of E0007313 */
    return ret;
} /* end of hal_miim_read */


/* Function Name:
 *      hal_miim_write
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_write(
    uint32      unit,
    rtk_port_t  port,
    uint32      page,
    uint32      phy_reg,
    uint32      data)
{
    int32   ret = RT_ERR_FAILED;
    uint32  access_page, fixed_page;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, page=0x%x, phy_reg=0x%x, data=0x%x", unit, port, page, phy_reg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_write)
        return RT_ERR_FAILED;

    if (NULL == pHalCtrl->pPhy_ctrl[port])
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_write(unit, port, page, phy_reg, data);
        return ret;
    }

    fixed_page = HAL_MIIM_PAGE_ID_MAX(unit);
    access_page = page;

    if (access_page > fixed_page)
    {
        /* Use currently page access mechanism */
        MACDRV(pHalCtrl)->fMdrv_miim_write(unit, port, PHY_PAGE_0, PHY_PAGE_SELECTION_REG, access_page);
        access_page = fixed_page;
    }

#if !defined(__MODEL_USER__)
#if defined(CONFIG_SDK_RTL8212B) || defined(CONFIG_SDK_RTL8214B) || defined(CONFIG_SDK_RTL8214FB)
    if ((PHY_MODEL_ID_RTL8214B == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9) &&
        ((access_page == PHY_PAGE_0) || (access_page>=7 && access_page<=15)))
    {
        uint32  auto_1000f_shadow = 0;

        auto_1000f_shadow = (data >> 9) & 0x1;
        if ((ret = phy_8214fb_auto_1000f_set(unit, port, auto_1000f_shadow)) != RT_ERR_OK)
        {
            RT_LOG(LOG_WARNING, MOD_HAL, "phy_8214fb_auto_1000f_set(unit=%d, port=%d) failed", unit, port);
            return ret;
        }
    }
#endif

#if defined(CONFIG_SDK_RTL8218B) || defined(CONFIG_SDK_RTL8218FB) || defined(CONFIG_SDK_RTL8214FC)
    if ((PHY_MODEL_ID_RTL8218B_EXT == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9))
    {
        uint32  auto_1000f_shadow = 0;

        auto_1000f_shadow = (data >> 9) & 0x1;
        if ((ret = phy_8218b_auto_1000f_set(unit, port, auto_1000f_shadow)) != RT_ERR_OK)
        {
            RT_LOG(LOG_WARNING, MOD_HAL, "phy_8218b_auto_1000f_set(unit=%d, port=%d) failed", unit, port);
            return ret;
        }
    }
#endif
#endif

    ret = MACDRV(pHalCtrl)->fMdrv_miim_write(unit, port, access_page, phy_reg, data);

    /* E0007313 */
    if (((PHY_MODEL_ID_RTL8218B_EXT == pHalCtrl->pPhy_ctrl[port]->phy_model_id)) && (access_page != fixed_page))
    {
        MACDRV(pHalCtrl)->fMdrv_miim_write(unit, port, 0, 31, 0);
    }
    /* End of E0007313 */
    return ret;
} /* end of hal_miim_write */


/* Function Name:
 *      hal_miim_park_read
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      parkPage    - PHY park page
 *      phy_reg - PHY register
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_park_read(
    uint32      unit,
    rtk_port_t  port,
    uint32      page,
    uint32      parkPage,
    uint32      phy_reg,
    uint32      *pData)
{
    int32   ret = RT_ERR_FAILED;
    uint32  access_page;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, page=0x%x, parkPage=0x%x, phy_reg=0x%x", unit, port, page, parkPage, phy_reg);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_park_read)
        return RT_ERR_FAILED;

    if (NULL == pHalCtrl->pPhy_ctrl[port])
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_park_read(unit, port, page, parkPage, phy_reg, pData);
        return ret;
    }

    access_page = page;

    ret = MACDRV(pHalCtrl)->fMdrv_miim_park_read(unit, port, access_page, parkPage, phy_reg, pData);

#if !defined(__MODEL_USER__)
#if defined(CONFIG_SDK_RTL8212B) || defined(CONFIG_SDK_RTL8214B) || defined(CONFIG_SDK_RTL8214FB)
    if ((PHY_MODEL_ID_RTL8214B == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9) &&
        ((access_page == PHY_PAGE_0) || (access_page>=7 && access_page<=15)))
    {
        uint32  auto_1000f_shadow = 0;

        if (((*pData)&0x0200) == 0)
        {
            if ((ret = phy_8214fb_auto_1000f_get(unit, port, &auto_1000f_shadow)) != RT_ERR_OK)
            {
                RT_LOG(LOG_WARNING, MOD_HAL, "phy_8214fb_auto_1000f_get(unit=%d, port=%d) failed", unit, port);
                return ret;
            }
            (*pData) |= (auto_1000f_shadow<<9);
        }
    }
#endif
#endif

#if !defined(__MODEL_USER__)
#if defined(CONFIG_SDK_RTL8218B) || defined(CONFIG_SDK_RTL8218FB) || defined(CONFIG_SDK_RTL8214FC)
    if ((PHY_MODEL_ID_RTL8218B_EXT == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9))
    {
        uint32  auto_1000f_shadow = 0;

        if (((*pData)&0x0200) == 0)
        {
            if ((ret = phy_8218b_auto_1000f_get(unit, port, &auto_1000f_shadow)) != RT_ERR_OK)
            {
                RT_LOG(LOG_WARNING, MOD_HAL, "phy_8218b_auto_1000f_get(unit=%d, port=%d) failed", unit, port);
                return ret;
            }
            (*pData) |= (auto_1000f_shadow<<9);
        }
    }
#endif
#endif

    return ret;
} /* end of hal_miim_read */


/* Function Name:
 *      hal_miim_park_write
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      page    - PHY page
 *      parkPage    - PHY park page
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_park_write(
    uint32      unit,
    rtk_port_t  port,
    uint32      page,
    uint32      parkPage,
    uint32      phy_reg,
    uint32      data)
{
    int32   ret = RT_ERR_FAILED;
    uint32  access_page;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, page=0x%x, parkPage=0x%x, phy_reg=0x%x, data=0x%x", unit, port, page, parkPage, phy_reg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_park_write)
        return RT_ERR_FAILED;

    if (NULL == pHalCtrl->pPhy_ctrl[port])
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_park_write(unit, port, page, parkPage, phy_reg, data);
        return ret;
    }

    access_page = page;

#if !defined(__MODEL_USER__)
#if defined(CONFIG_SDK_RTL8212B) || defined(CONFIG_SDK_RTL8214B) || defined(CONFIG_SDK_RTL8214FB)
    if ((PHY_MODEL_ID_RTL8214B == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9) &&
        ((access_page == PHY_PAGE_0) || (access_page>=7 && access_page<=15)))
    {
        uint32  auto_1000f_shadow = 0;

        auto_1000f_shadow = (data >> 9) & 0x1;
        if ((ret = phy_8214fb_auto_1000f_set(unit, port, auto_1000f_shadow)) != RT_ERR_OK)
        {
            RT_LOG(LOG_WARNING, MOD_HAL, "phy_8214fb_auto_1000f_set(unit=%d, port=%d) failed", unit, port);
            return ret;
        }
    }
#endif
#endif

#if !defined(__MODEL_USER__)
#if defined(CONFIG_SDK_RTL8218B) || defined(CONFIG_SDK_RTL8218FB) || defined(CONFIG_SDK_RTL8214FC)
    if ((PHY_MODEL_ID_RTL8218B_EXT == pHalCtrl->pPhy_ctrl[port]->phy_model_id) &&
        (PHY_REV_NO_B == pHalCtrl->pPhy_ctrl[port]->phy_rev_id) && (phy_reg == 9))
    {
        uint32  auto_1000f_shadow = 0;

        auto_1000f_shadow = (data >> 9) & 0x1;
        if ((ret = phy_8218b_auto_1000f_set(unit, port, auto_1000f_shadow)) != RT_ERR_OK)
        {
            RT_LOG(LOG_WARNING, MOD_HAL, "phy_8218b_auto_1000f_set(unit=%d, port=%d) failed", unit, port);
            return ret;
        }
    }
#endif
#endif

    ret = MACDRV(pHalCtrl)->fMdrv_miim_park_write(unit, port, access_page, parkPage, phy_reg, data);

    return ret;
} /* end of hal_miim_write */

/* Function Name:
 *      hal_miim_portmask_write
 * Description:
 *      Set PHY registers in those portmask.
 * Input:
 *      unit     - unit id
 *      portmask - portmask
 *      page     - PHY page
 *      phy_reg  - PHY register
 *      data     - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. portmask valid range is bit 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_portmask_write(
    uint32          unit,
    rtk_portmask_t  portmask,
    uint32          page,
    uint32          phy_reg,
    uint32          data)
{
    uint32  access_page;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, portmask="PMSK_FMT", page=0x%x, phy_reg=0x%x, data=0x%x", unit, PMSK_ARG(portmask), page, phy_reg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_portmask_write)
        return RT_ERR_FAILED;

    access_page = page;  /* Normal case: access the page number */

    return (MACDRV(pHalCtrl)->fMdrv_miim_portmask_write(unit, portmask, access_page, phy_reg, data));
} /* end of hal_miim_portmask_write */

/* Function Name:
 *      hal_miim_broadcast_write
 * Description:
 *      Set PHY registers with broadcast mechanism.
 * Input:
 *      unit    - unit id
 *      page    - PHY page
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. page valid range is 0 ~ 31
 *      2. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_broadcast_write(
    uint32      unit,
    uint32      page,
    uint32      phy_reg,
    uint32      data)
{
    int32   ret = RT_ERR_FAILED;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, page=0x%x, reg=0x%x \
           data=0x%x", unit, page, phy_reg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_broadcast_write)
        return RT_ERR_FAILED;
    else
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_broadcast_write(unit, page, phy_reg, data);
        return ret;
    }
} /* end of hal_miim_extParkPage_write */

/* Function Name:
 *      hal_miim_extParkPage_read
 * Description:
 *      Get PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg - PHY register
 * Output:
 *      pData   - pointer buffer of read data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_extParkPage_read(
    uint32      unit,
    rtk_port_t  port,
    uint32      mainPage,
    uint32      extPage,
    uint32      parkPage,
    uint32      phy_reg,
    uint32      *pData)
{
    int32   ret = RT_ERR_FAILED;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, mainPage=0x%x, extPage=0x%x, parkPage=0x%x, reg=0x%x",
           unit, port, mainPage, extPage, parkPage, phy_reg);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_extParkPage_read)
        return RT_ERR_FAILED;
    else
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_extParkPage_read(unit, port, mainPage, extPage, parkPage, phy_reg, pData);
        return ret;
    }
} /* end of hal_miim_extParkPage_read */


/* Function Name:
 *      hal_miim_extParkPage_write
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_extParkPage_write(
    uint32      unit,
    rtk_port_t  port,
    uint32      mainPage,
    uint32      extPage,
    uint32      parkPage,
    uint32      phy_reg,
    uint32      data)
{
    int32   ret = RT_ERR_FAILED;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, mainPage=0x%x, extPage=0x%x, parkPage=0x%x, reg=0x%x \
           data=0x%x", unit, port, mainPage, extPage, parkPage, phy_reg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_extParkPage_write)
        return RT_ERR_FAILED;
    else
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_extParkPage_write(unit, port, mainPage, extPage, parkPage, phy_reg, data);
        return ret;
    }
} /* end of hal_miim_extParkPage_write */


/* Function Name:
 *      hal_miim_extParkPage_portmask_write
 * Description:
 *      Set PHY registers in those portmask.
 * Input:
 *      unit     - unit id
 *      portmask - portmask
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg  - PHY register
 *      data     - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. portmask valid range is bit 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_extParkPage_portmask_write(
    uint32          unit,
    rtk_portmask_t  portmask,
    uint32          mainPage,
    uint32          extPage,
    uint32          parkPage,
    uint32          phy_reg,
    uint32          data)
{
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, portmask="PMSK_FMT", mainPage=0x%x, extPage=0x%x, parkPage=0x%x, reg=0x%x \
           data=0x%x", unit, PMSK_ARG(portmask), mainPage, extPage, parkPage, phy_reg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_extParkPage_portmask_write)
        return RT_ERR_FAILED;

    return (MACDRV(pHalCtrl)->fMdrv_miim_extParkPage_portmask_write(unit, portmask, mainPage, extPage, parkPage, phy_reg, data));
} /* end of hal_miim_extParkPage_portmask_write */

/* Function Name:
 *      hal_miim_mmd_read
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
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_mmd_read(
    uint32      unit,
    rtk_port_t  port,
    uint32      mmdAddr,
    uint32      mmdReg,
    uint32      *pData)
{
    int32   ret = RT_ERR_FAILED;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, mmdAddr=0x%x, mmdReg=0x%x, reg=0x%x",
           unit, port, mmdAddr, mmdReg);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_mmd_read)
        return RT_ERR_FAILED;
    else
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_read(unit, port, mmdAddr, mmdReg, pData);
        return ret;
    }
} /* end of hal_miim_read */


/* Function Name:
 *      hal_miim_mmd_write
 * Description:
 *      Set PHY registers.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      mainPage    - main page id
 *      extPage     - extension page id
 *      parkPage    - parking page id
 *      phy_reg - PHY register
 *      data    - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. port valid range is 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_mmd_write(
    uint32      unit,
    rtk_port_t  port,
    uint32      mmdAddr,
    uint32      mmdReg,
    uint32      data)
{
    int32   ret = RT_ERR_FAILED;
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d, mmdAddr=0x%x, mmdReg=0x%x \
           data=0x%x", unit, port, mmdAddr, mmdReg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_mmd_write)
        return RT_ERR_CHIP_NOT_SUPPORTED;
    else
    {
        ret = MACDRV(pHalCtrl)->fMdrv_miim_mmd_write(unit, port, mmdAddr, mmdReg, data);
        return ret;
    }
} /* end of hal_miim_mmd_write */


/* Function Name:
 *      hal_miim_mmd_portmask_write
 * Description:
 *      Set PHY registers in those portmask.
 * Input:
 *      unit     - unit id
 *      portmask - portmask
 *      mmdAddr  - mmd device address
 *      mmdReg   - mmd reg id
 *      data     - write data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      1. portmask valid range is bit 0 ~ 27
 *      2. page valid range is 0 ~ 31
 *      3. phy_reg valid range is 0 ~ 31
 */
int32
hal_miim_mmd_portmask_write(
    uint32          unit,
    rtk_portmask_t  portmask,
    uint32          mmdAddr,
    uint32          mmdReg,
    uint32          data)
{
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, portmask="PMSK_FMT", mmdAddr=0x%x, mmdReg=0x%x \
           data=0x%x", unit, PMSK_ARG(portmask), mmdAddr, mmdReg, data);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (NULL == MACDRV(pHalCtrl)->fMdrv_miim_mmd_portmask_write)
        return RT_ERR_FAILED;

    return (MACDRV(pHalCtrl)->fMdrv_miim_mmd_portmask_write(unit, portmask, mmdAddr, mmdReg, data));
} /* end of hal_miim_mmd_portmask_write */

/* Function Name:
 *      hal_miim_pollingEnable_get
 * Description:
 *      Get the mac polling PHY status of the specified port.
 * Input:
 *      unit     - unit id
 *      port     - port id
 * Output:
 *      pEnabled - pointer buffer of mac polling PHY status
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
hal_miim_pollingEnable_get(
    uint32          unit,
    rtk_port_t      port,
    rtk_enable_t    *pEnabled)
{
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d", unit, port);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (!HWP_PORT_EXIST(unit, port))
        return RT_ERR_FAILED;

    return (MACDRV(pHalCtrl)->fMdrv_miim_pollingEnable_get(unit, port, pEnabled));
} /* end of hal_miim_pollingEnable_get */

/* Function Name:
 *      hal_miim_pollingEnable_set
 * Description:
 *      Set the mac polling PHY status of the specified port.
 * Input:
 *      unit    - unit id
 *      port    - port id
 *      enabled - mac polling PHY status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
hal_miim_pollingEnable_set(
    uint32          unit,
    rtk_port_t      port,
    rtk_enable_t    enabled)
{
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%d", unit, port);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (!HWP_PORT_EXIST(unit, port))
        return RT_ERR_FAILED;

    return (MACDRV(pHalCtrl)->fMdrv_miim_pollingEnable_set(unit, port, enabled));
} /* end of hal_miim_pollingEnable_set */

/* Function Name:
 *      hal_miim_globalPollingEnable_get
 * Description:
 *      Get the global mac polling PHY status.
 * Input:
 *      unit     - unit id
 * Output:
 *      pEnabled - pointer buffer of mac polling PHY status
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
hal_miim_globalPollingEnable_get(
    uint32          unit,
    rtk_enable_t    *pEnabled)
{
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d", unit);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    return (MACDRV(pHalCtrl)->fMdrv_miim_globalPollingEnable_get(unit, pEnabled));
} /* end of hal_miim_globalPollingEnable_get */

/* Function Name:
 *      hal_miim_globalPollingEnable_set
 * Description:
 *      Set the global mac polling PHY status.
 * Input:
 *      unit    - unit id
 *      enabled - mac polling PHY status
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
hal_miim_globalPollingEnable_set(
    uint32          unit,
    rtk_enable_t    enabled)
{
    hal_control_t   *pHalCtrl;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d", unit);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    return (MACDRV(pHalCtrl)->fMdrv_miim_globalPollingEnable_set(unit, enabled));
} /* end of hal_miim_globalPollingEnable_set */



/* Function Name:
 *      hal_miim_smiMdxProto_get
 * Description:
 *      Configure MDC/MDIO protocol for an SMI interface
 * Input:
 *      unit - unit id
 *      smi_id - SMI interface ID
 * Output:
 *      *pProto - protocol as Clause 22 or Clause 45
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_INPUT  - error smi id or proto
 * Note:
 *      None
 */
extern int32
hal_miim_smiMdxProto_get(uint32 unit, uint32 smi_id, drv_smi_mdxProtoSel_t *pProto)
{
    hal_control_t   *pHalCtrl = NULL;

    //RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, smi_id=%u", unit, smi_id);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (MACDRV(pHalCtrl)->fMdrv_miim_smiMdxProto_get == NULL)
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    return (MACDRV(pHalCtrl)->fMdrv_miim_smiMdxProto_get(unit, smi_id, pProto));
}

/* Function Name:
 *      hal_miim_smiMdxProto_set
 * Description:
 *      Configure MDC/MDIO protocol for an SMI interface
 * Input:
 *      unit - unit id
 *      smi_id - SMI interface ID
 *      proto  - protocol as Clause 22 or Clause 45
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 *      RT_ERR_INPUT  - error smi id or proto
 * Note:
 *      None
 */
int32
hal_miim_smiMdxProto_set(uint32 unit, uint32 smi_id, drv_smi_mdxProtoSel_t proto)
{
    hal_control_t   *pHalCtrl = NULL;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, smi_id=%u proto %u", unit, smi_id, proto);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (MACDRV(pHalCtrl)->fMdrv_miim_smiMdxProto_set == NULL)
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    return (MACDRV(pHalCtrl)->fMdrv_miim_smiMdxProto_set(unit, smi_id, proto));
}

/* Function Name:
 *      hal_miim_pollingConfig_set
 * Description:
 *      Configure MAC polling type
 * Input:
 *      unit - unit id
 *      port - port id
 *      pPollConf.eth_type - ethernet type: HWP_FE, HWP_GE, HWP_XGE, ...
 *      pPollConf.pollExtPhy - enable MAC to send out MDIO messages to polling PHY status.
 *                             when set to disable, MAC will polling it's SerDes status.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
hal_miim_pollingConfig_set(uint32 unit, uint32 port, drv_miim_pollConf_t *pPollConf)
{
    hal_control_t   *pHalCtrl = NULL;

    RT_LOG(LOG_TRACE, MOD_HAL, "unit=%d, port=%u", unit, port);

    if ((pHalCtrl = hal_ctrlInfo_get(unit)) == NULL)
        return RT_ERR_FAILED;

    if (MACDRV(pHalCtrl)->fMdrv_miim_pollingConfig_set == NULL)
    {
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

    return (MACDRV(pHalCtrl)->fMdrv_miim_pollingConfig_set(unit, port, pPollConf));
}

