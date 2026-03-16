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
 * $Revision: 81289 $
 * $Date: 2017-08-15 20:45:44 +0800 (Tue, 15 Aug 2017) $
 *
 * Purpose : Setup PHY parameters
 *
 * Feature : Setup PHY parameters functions
 *
 */

/*
 * Include Files
 */
#include <dal/dal_construct.h>
#include <hal/phy/construct/conftypes.h>
#include <hal/phy/construct/conf_rtl8208.c>
#include <hal/phy/construct/conf_rtl8214fb.c>
#include <hal/phy/construct/conf_rtl8214fc.c>
#include <hal/phy/construct/conf_rtl8218b.c>
#include <hal/phy/construct/conf_rtl8218d.c>

#if defined(CONFIG_SDK_RTL9300)
#include <hal/chipdef/longan/rtk_longan_reg_struct.h>
#include <hal/chipdef/longan/rtk_longan_table_struct.h>
#endif

#if (defined(CONFIG_SDK_RTL8218D))
  #include <hal/phy/phy_rtl8218d.h>
#endif

#if (defined(CONFIG_SDK_RTL8295R) || defined(CONFIG_SDK_RTL8214QF))
  #include <hal/phy/phy_rtl8295.h>
  #include <hal/phy/phy_rtl8295_patch.h>
#endif/* CONFIG_SDK_RTL8295R || CONFIG_SDK_RTL8214QF */
#if defined(CONFIG_SDK_RTL8295R)
  #include <hal/phy/construct/conf_rtl8295r_8431Dac.c>
#endif


#define RTL8218B_CHIP_ID            0x6276
#define RTL8218B_TCA_CHIP_ID        0x0455
#define RTL8218B_INT_TCA_CHIP_ID    0x0477
#define RTL8218B_INT_MP_A_CHIP_ID   0x6276
#define RTL8218B_INT_MP_B_CHIP_ID   0x6275
#define RTL8218B_TCA_CUT_A          0x0
#define RTL8218B_TCA_CUT_B          0x1
#define RTL8218FB_TCA_CHIP_ID       0x0455
#define RTL8218FB_TCA_CUT_A         0x0
#define RTL8218FB_TCA_CUT_B         0x1
#define RTL8208_CHIP_ID             0x8
#define RTL8214FC_CHIP_ID           0x6276
#define RTL8214FC_TCA_CHIP_ID       0x0455
#define RTL8214FC_INT_TCA_CHIP_ID   0x0477
#define RTL8214FC_TCA_CUT_A         0x0
#define RTL8214FC_TCA_CUT_B         0x1
#define RTL8214FC_MP_CHIP_ID        0x6276
#define RTL8214FC_MP_CUT_A          0x0


#define PRINT_PHY_INITIAL_MESSGE(phy_name,macID)   CNSTRT_MSG("        %s (MacID=%u)\n",phy_name,macID)


#if defined(CONFIG_SDK_RTL8390)

/* Function Name:
 *      rtl8218b_rtl8390_config
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      phy_base_macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8218b_rtl8390_config(uint32 unit, uint8 phy_base_macId)
{
    rtk_port_t rtl8218_phy0_macid = phy_base_macId;
    unsigned int val;
    int i;
    rtk_port_t  port_id, base_id;
    unsigned int rl_no = 0, ver_no = 0, cut_id, romId;

    CNSTRT_PRINT("%s()\n",__FUNCTION__);

    base_id = rtl8218_phy0_macid;
    RTK_MII_READ(unit,base_id, 0, 3, &val);

    RTK_MII_WRITE(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, 29, 0x0001);
    RTK_MII_WRITE(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, 31, 0x0a43);
    RTK_MII_WRITE(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, 19, 0x0002);
    RTK_MII_READ(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, 20, &rl_no);

    if (RTL8218B_CHIP_ID != rl_no)
    {
        /*Switch to right TOP address*/
        RTK_MII_WRITE(unit,base_id, 0, 29, 0x0000);
        RTK_MII_WRITE(unit,base_id, 0, 31, 0x0a42);

        /*Check the 8218B internal chip ID*/
        RTK_MII_WRITE(unit,base_id, 31, 27, 0x0002);
        RTK_MII_READ(unit,base_id, 31, 28, &val);

        /*Check the 8218B internal cut ID*/
        RTK_MII_WRITE(unit,base_id, 31, 27, 0x0004);
        RTK_MII_READ(unit,base_id, 31, 28, &cut_id);

        /*Check the 8218B status*/
        RTK_MII_WRITE(unit,base_id, 31, 31, 0xa42);
        RTK_MII_READ(unit,base_id, 31, 16, &val);

        PRINT_PHY_INITIAL_MESSGE("RTL8218B",base_id);

        if(cut_id == RTK_RTL8218B_TCA_CUT_A)
        {
            if(val == 2) /* ext init mode*/
            {
                for (i=0; i<(sizeof(construct_rtl8218b_0455A_mode2_perchip)/sizeof(confcode_prv_t)); i++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid + construct_rtl8218b_0455A_mode2_perchip[i].phy, \
                        RTK_MII_MAXPAGE8390, (int)construct_rtl8218b_0455A_mode2_perchip[i].reg, construct_rtl8218b_0455A_mode2_perchip[i].val);
                }
                for(port_id = 0; port_id < 8; port_id++)
                {
                    for (i=0; i<(sizeof(construct_rtl8218b_0455A_mode2_perport)/sizeof(confcode_rv_t)); i++)
                    {
                        if((i == 2))
                            osal_time_udelay(500*1000);  /*Wait for 8018B leave the command state*/
                        RTK_MII_WRITE(unit,rtl8218_phy0_macid + port_id, \
                        RTK_MII_MAXPAGE8390, (int)construct_rtl8218b_0455A_mode2_perport[i].reg, construct_rtl8218b_0455A_mode2_perport[i].val);
                    }
                }
            }
            else if((val == 3) || (val == 5)) /* lan mode*/
            {

                for (i=0; i<(sizeof(construct_rtl8218b_0455A_mode3_perchip)/sizeof(confcode_prv_t)); i++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid + construct_rtl8218b_0455A_mode3_perchip[i].phy, \
                        RTK_MII_MAXPAGE8390, (int)construct_rtl8218b_0455A_mode3_perchip[i].reg, construct_rtl8218b_0455A_mode3_perchip[i].val);
                }
                for(port_id = 0; port_id < 8; port_id++)
                {
                    for (i=0; i<(sizeof(construct_rtl8218b_0455A_mode3_perport)/sizeof(confcode_rv_t)); i++)
                    {
                        if((i == 2))
                            osal_time_udelay(500*1000);  /*Wait for 8018B leave the command state*/
                        RTK_MII_WRITE(unit,rtl8218_phy0_macid + port_id, \
                        RTK_MII_MAXPAGE8390, (int)construct_rtl8218b_0455A_mode3_perport[i].reg, construct_rtl8218b_0455A_mode3_perport[i].val);
                    }
                }

                /*Check the 8218B verA Patch Result*/
                RTK_MII_WRITE(unit,0, 31, 27, 0x8012); /*Reg 27 is PHY internal SRAM address*/
                RTK_MII_READ(unit,0, 31, 28, &val); /*Reg 28 is PHY internal SRAM address value*/
                if(val != 0x3f0f)
                    RTK_DBG_PRINT(0, "RTL8218B TCA Patch Failed 1!!!\n");
                RTK_MII_WRITE(unit,0, 31, 27, 0x80d1);
                RTK_MII_READ(unit,0, 31, 28, &val);
                if(val != 0xa5aa)
                    RTK_DBG_PRINT(0, "RTL8218B TCA Patch Failed 2!!!\n");
            } else {
                PRINT_PHY_INITIAL_MESSGE("RTL8218B",val);
                return;
            }
        }
        else if(cut_id == RTK_RTL8218B_TCA_CUT_B)
        {

            /* Per-Chip */
            for (i=0; i<(sizeof(construct_rtl8218b_0455B_rtl8390_perchip)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8218_phy0_macid + construct_rtl8218b_0455B_rtl8390_perchip[i].phy, \
                    RTK_MII_MAXPAGE8390, \
                    (int)construct_rtl8218b_0455B_rtl8390_perchip[i].reg, \
                    (int)construct_rtl8218b_0455B_rtl8390_perchip[i].val);
            }

            /* Per-Port */
            for(port_id = 0; port_id < 8; port_id++)
            {
                for (i=0; i<(sizeof(construct_rtl8218b_0455B_rtl8390_perport)/sizeof(confcode_rv_t)); i++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid + port_id, \
                    RTK_MII_MAXPAGE8390, \
                    (int)construct_rtl8218b_0455B_rtl8390_perport[i].reg, \
                    (int)construct_rtl8218b_0455B_rtl8390_perport[i].val);
                }
            }
        }
    }
    else
    {
        /* Version */
        RTK_MII_WRITE(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, 19, 0x0004);
        RTK_MII_READ(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, 20, &ver_no);

        RTK_MII_WRITE(unit,base_id, RTK_MII_MAXPAGE8390, 27, 0x0004);
        RTK_MII_READ(unit,base_id, RTK_MII_MAXPAGE8390, 28, &romId);

        PRINT_PHY_INITIAL_MESSGE("RTL8218B",base_id);

        /* Per-Chip */
        for (i = 0; i < (sizeof(construct_rtl8218b_6276A_rtl8390_perchip)/sizeof(confcode_prv_t)); ++i)
        {
            RTK_MII_WRITE(unit,rtl8218_phy0_macid + construct_rtl8218b_6276A_rtl8390_perchip[i].phy, \
                    RTK_MII_MAXPAGE8390, \
                    (int)construct_rtl8218b_6276A_rtl8390_perchip[i].reg,  \
                    construct_rtl8218b_6276A_rtl8390_perchip[i].val);
        }

        for (i = 0; i < (sizeof(construct_rtl8218b_6276A_rtl8390_perchip2)/sizeof(confcode_phy_patch_t)); ++i)
        {
            PHY_PATCH_SET(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, (unsigned int)construct_rtl8218b_6276A_rtl8390_perchip2[i]);
        }

        for (i = 0; i < (sizeof(construct_rtl8218b_6276A_rtl8390_perchip3)/sizeof(confcode_prv_t)); ++i)
        {
            RTK_MII_WRITE(unit,rtl8218_phy0_macid + construct_rtl8218b_6276A_rtl8390_perchip3[i].phy, \
                    RTK_MII_MAXPAGE8390, \
                    (int)construct_rtl8218b_6276A_rtl8390_perchip3[i].reg,  \
                    construct_rtl8218b_6276A_rtl8390_perchip3[i].val);
        }

        for (port_id = 0; port_id < 8; ++port_id)
        {
            rtk_port_t pid = rtl8218_phy0_macid + port_id;

            for (i = 0; i < (sizeof(construct_rtl8218b_6276A_rtl8390_perport)/sizeof(confcode_rv_t)); ++i)
                RTK_MII_WRITE(unit,pid, RTK_MII_MAXPAGE8390, \
                        (int)construct_rtl8218b_6276A_rtl8390_perport[i].reg, \
                        (int)construct_rtl8218b_6276A_rtl8390_perport[i].val);

            #ifdef CONFIG_RTL8218B_HW_ESD_IMPROVEMENT
            if (0 != romId)
            {
                for (i = 0; i < (sizeof(rtl8218b_6276B_hwEsd_perport)/sizeof(confcode_rv_t)); ++i)
                    RTK_MII_WRITE(unit, pid, RTK_MII_MAXPAGE8390, \
                            (int)rtl8218b_6276B_hwEsd_perport[i].reg, \
                            (int)rtl8218b_6276B_hwEsd_perport[i].val);
            }
            #endif  /* CONFIG_RTL8218B_HW_ESD_IMPROVEMENT */
        }

        RTK_MII_WRITE(unit,rtl8218_phy0_macid, RTK_MII_MAXPAGE8390, 30, 8);
        RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0x26e, 17, 0xb);
        RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0x26e, 16, 0x2);
        osal_time_mdelay(1);
        RTK_MII_READ(unit,rtl8218_phy0_macid, 0x26e, 19, &val);
        RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0, 30, 0);

        val = (val >> 4) & 0xf;
        if (1 == val)
        {
            for (port_id = 0; port_id < 8; ++port_id)
            {
                rtk_port_t pid = rtl8218_phy0_macid + port_id;

                for (i = 0; i < (sizeof(construct_rtl8218b_6276C_rtl8390_IPD_perport)/sizeof(confcode_rv_t)); ++i)
                    RTK_MII_WRITE(unit,pid, RTK_MII_MAXPAGE8390, \
                            (int)construct_rtl8218b_6276C_rtl8390_IPD_perport[i].reg, \
                            (int)construct_rtl8218b_6276C_rtl8390_IPD_perport[i].val);
            }

            for (i = 0; i < (sizeof(construct_rtl8218b_6276C_rtl8390_IPD_perchip)/sizeof(confcode_prv_t)); ++i)
            {
                RTK_MII_WRITE(unit,rtl8218_phy0_macid + construct_rtl8218b_6276C_rtl8390_IPD_perchip[i].phy, \
                        RTK_MII_MAXPAGE8390, \
                        (int)construct_rtl8218b_6276C_rtl8390_IPD_perchip[i].reg,  \
                        construct_rtl8218b_6276C_rtl8390_IPD_perchip[i].val);
            }
        }
    }

    return;
} /* end of rtl8218b_rtl8390_config */

#if defined(CONFIG_SDK_RTL8218FB)
/* Function Name:
 *      rtl8218fb_rtl8390_config
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      phy_base_macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8218fb_rtl8390_config(uint32 unit, uint8 phy_base_macId)
{
    rtk_port_t rtl8218fb_phy0_macid = phy_base_macId;
    unsigned int val;
    int i;
    rtk_port_t  port_id, base_id;
    unsigned int rl_no = 0, romId;

    CNSTRT_PRINT("%s()\n",__FUNCTION__);

    base_id = rtl8218fb_phy0_macid;

    RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, RTK_MII_MAXPAGE8390, 29, 0x0001);
    RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, RTK_MII_MAXPAGE8390, 31, 0x0a43);
    RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, RTK_MII_MAXPAGE8390, 19, 0x0002);
    RTK_MII_READ(unit,rtl8218fb_phy0_macid, RTK_MII_MAXPAGE8390, 20, &rl_no);

    PRINT_PHY_INITIAL_MESSGE("RTL8218FB", base_id);

    RTK_MII_WRITE(unit,base_id, RTK_MII_MAXPAGE8390, 27, 0x0004);
    RTK_MII_READ(unit,base_id, RTK_MII_MAXPAGE8390, 28, &romId);

    for (i = 0; i < (sizeof(construct_rtl8218fb_6276A_rtl8390_perchip_serdes)/sizeof(confcode_prv_t)); ++i)
    {
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid + construct_rtl8218fb_6276A_rtl8390_perchip_serdes[i].phy, \
                RTK_MII_MAXPAGE8390, \
                (int)construct_rtl8218fb_6276A_rtl8390_perchip_serdes[i].reg, \
                (int)construct_rtl8218fb_6276A_rtl8390_perchip_serdes[i].val);
    }

    for (i = 0; i < (sizeof(construct_rtl8218b_6276A_rtl8390_perchip2)/sizeof(confcode_phy_patch_t)); ++i)
    {
        PHY_PATCH_SET(unit,rtl8218fb_phy0_macid, RTK_MII_MAXPAGE8390, construct_rtl8218b_6276A_rtl8390_perchip2[i]);
    }

    for (i = 0; i < (sizeof(construct_rtl8218fb_6276A_rtl8390_perchip_serdes3)/sizeof(confcode_prv_t)); ++i)
    {
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid + construct_rtl8218fb_6276A_rtl8390_perchip_serdes3[i].phy, \
                RTK_MII_MAXPAGE8390, \
                (int)construct_rtl8218fb_6276A_rtl8390_perchip_serdes3[i].reg, \
                (int)construct_rtl8218fb_6276A_rtl8390_perchip_serdes3[i].val);
    }

    for (port_id = 0; port_id < 8; ++port_id)
    {
        rtk_port_t pid = rtl8218fb_phy0_macid + port_id;

        for (i = 0; i < (sizeof(construct_rtl8218fb_6276A_rtl8390_perport)/sizeof(confcode_rv_t)); ++i)
            RTK_MII_WRITE(unit,pid, RTK_MII_MAXPAGE8390, \
                    (int)construct_rtl8218fb_6276A_rtl8390_perport[i].reg, \
                    (int)construct_rtl8218fb_6276A_rtl8390_perport[i].val);
    }

    RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, RTK_MII_MAXPAGE8390, \
            0x1e, 0x8);
    RTK_MII_READ(unit,rtl8218fb_phy0_macid, 0x260, 0x13, &val);
    val &= (0x1F << 8);
    val |= 0x4020;
    RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0x260, 0x13, val);
    RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, RTK_MII_MAXPAGE8390, \
            0x1e, 0x0);

    for (i = 0; i < (sizeof(construct_rtl8218fb_6276A_rtl8390_fiber_perchip)/sizeof(confcode_prv_t)); ++i)
    {
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid + construct_rtl8218fb_6276A_rtl8390_fiber_perchip[i].phy, \
                RTK_MII_MAXPAGE8390, \
                (int)construct_rtl8218fb_6276A_rtl8390_fiber_perchip[i].reg, \
                (int)construct_rtl8218fb_6276A_rtl8390_fiber_perchip[i].val);
    }

    return;
} /* end of rtl8218fb_rtl8390_config */
#endif  /* defined(CONFIG_RTL8218FB) */

/* Function Name:
 *      rtl8214fc_rtl8390_config
 * Description:
 *      Configuration code for RTL8214FC.
 * Input:
 *      phy_base_macId - the macId of PHY0 of the RTL8214FC
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8214fc_rtl8390_config(uint32 unit, uint8 phy_base_macId)
{
    rtk_port_t rtl8214_phy0_macid = phy_base_macId;
    unsigned int val;
    int i;
    rtk_port_t  port_id, base_id;
    unsigned int rl_no, romId;

    CNSTRT_PRINT("%s()\n",__FUNCTION__);

    base_id = rtl8214_phy0_macid;

    RTK_MII_WRITE(unit,rtl8214_phy0_macid, RTK_MII_MAXPAGE8390, 29, 0x0001);
    RTK_MII_WRITE(unit,rtl8214_phy0_macid, RTK_MII_MAXPAGE8390, 31, 0x0a43);
    RTK_MII_WRITE(unit,rtl8214_phy0_macid, RTK_MII_MAXPAGE8390, 19, 0x0002);
    RTK_MII_READ(unit,rtl8214_phy0_macid, RTK_MII_MAXPAGE8390, 20, &rl_no);

    RTK_MII_WRITE(unit,base_id, RTK_MII_MAXPAGE8390, 27, 0x0004);
    RTK_MII_READ(unit,base_id, RTK_MII_MAXPAGE8390, 28, &romId);

    PRINT_PHY_INITIAL_MESSGE("RTL8214FC", rtl8214_phy0_macid);

    /* Serdes */
    for (i = 0; i < (sizeof(construct_rtl8214fc_6276A_rtl8390_perchip_serdes)/sizeof(confcode_prv_t)); ++i)
    {
        RTK_MII_WRITE(unit,rtl8214_phy0_macid + construct_rtl8214fc_6276A_rtl8390_perchip_serdes[i].phy, \
                RTK_MII_MAXPAGE8390, \
                (int)construct_rtl8214fc_6276A_rtl8390_perchip_serdes[i].reg, \
                (int)construct_rtl8214fc_6276A_rtl8390_perchip_serdes[i].val);
        osal_time_mdelay(5);
    }

    for (i = 0; i < (sizeof(construct_rtl8214fc_6276A_rtl8390_perchip_serdes2)/sizeof(confcode_phy_patch_t)); ++i)
    {
        PHY_PATCH_SET(unit,rtl8214_phy0_macid, RTK_MII_MAXPAGE8390, construct_rtl8214fc_6276A_rtl8390_perchip_serdes2[i]);
    }

    for (i = 0; i < (sizeof(construct_rtl8214fc_6276A_rtl8390_perchip_serdes3)/sizeof(confcode_prv_t)); ++i)
    {
        RTK_MII_WRITE(unit,rtl8214_phy0_macid + construct_rtl8214fc_6276A_rtl8390_perchip_serdes3[i].phy, \
                RTK_MII_MAXPAGE8390, \
                (int)construct_rtl8214fc_6276A_rtl8390_perchip_serdes3[i].reg, \
                (int)construct_rtl8214fc_6276A_rtl8390_perchip_serdes3[i].val);
        osal_time_mdelay(5);
    }

    for (port_id = 0; port_id < 4; ++port_id)
    {
        rtk_port_t pid = rtl8214_phy0_macid + port_id;

        for (i = 0; i < (sizeof(construct_rtl8214fc_6276A_rtl8390_perport)/sizeof(confcode_rv_t)); ++i)
        {
            //printf("p%d %d) reg %x val %x\n", pid, i, construct_rtl8214fc_6276A_rtl8390_perport[i].reg, construct_rtl8214fc_6276A_rtl8390_perport[i].val);
            RTK_MII_WRITE(unit,pid, RTK_MII_MAXPAGE8390, \
                    (int)construct_rtl8214fc_6276A_rtl8390_perport[i].reg, \
                    (int)construct_rtl8214fc_6276A_rtl8390_perport[i].val);
        }
    }

    RTK_MII_WRITE(unit,rtl8214_phy0_macid, RTK_MII_MAXPAGE8390, \
            0x1e, 0x8);
    RTK_MII_READ(unit,rtl8214_phy0_macid, 0x260, 0x13, &val);
    val &= (0x1F << 8);
    val |= 0x4020;
    RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0x260, 0x13, val);
    RTK_MII_WRITE(unit,rtl8214_phy0_macid, RTK_MII_MAXPAGE8390, \
            0x1e, 0x0);

    for (i = 0; i < (sizeof(construct_rtl8214fc_6276A_rtl8390_fiber_perchip)/sizeof(confcode_prv_t)); ++i)
    {
        //printf("%d) port %d reg %x val %x\n", i, construct_rtl8214fc_6276A_rtl8390_fiber_perchip[i].phy, construct_rtl8214fc_6276A_rtl8390_fiber_perchip[i].reg, construct_rtl8214fc_6276A_rtl8390_fiber_perchip[i].val);
        RTK_MII_WRITE(unit,rtl8214_phy0_macid + construct_rtl8214fc_6276A_rtl8390_fiber_perchip[i].phy, \
                RTK_MII_MAXPAGE8390, \
                (int)construct_rtl8214fc_6276A_rtl8390_fiber_perchip[i].reg, \
                (int)construct_rtl8214fc_6276A_rtl8390_fiber_perchip[i].val);
    }

    return;
} /* end of rtl8214fc_rtl8390_config */

#endif//CONFIG_SDK_RTL8390




/* Function Name:
 *      rtl8218b_rtl8380_config
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      phy_base_macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
#if defined(CONFIG_SDK_RTL8380)
void rtl8218b_rtl8380_config(uint32 unit, uint8 phy_base_macId)
{
    rtk_port_t rtl8218_phy0_macid = phy_base_macId;
    unsigned int val,cut_id;
    int i;
    rtk_port_t  port_id, base_id;
    int loop;

    base_id = rtl8218_phy0_macid;

    RTK_MII_READ(unit,rtl8218_phy0_macid, 0, 3, &val);


    /*Check the 8218B internal chip ID*/
    RTK_MII_WRITE(unit,phy_base_macId, 31, 27, 0x0002);
    RTK_MII_READ(unit,phy_base_macId, 31, 28, &val);

    if((val != RTL8218B_TCA_CHIP_ID) && (val != RTL8218B_INT_TCA_CHIP_ID) \
            && (val != RTL8218B_INT_MP_A_CHIP_ID) && (val != RTL8218B_CHIP_ID) && (val != RTL8218B_INT_MP_B_CHIP_ID)){
        PRINT_PHY_INITIAL_MESSGE("Error chip ID", val);
        return;
    }

   PRINT_PHY_INITIAL_MESSGE("RTL8218B", base_id);

   if(RTL8218B_TCA_CHIP_ID == val)
   {
        /*Check the 8218B internal cut ID*/
        RTK_MII_WRITE(unit,base_id, 31, 27, 0x0004);
        RTK_MII_READ(unit,base_id, 31, 28, &cut_id);


        /*Check the 8218B status*/
        RTK_MII_WRITE(unit,base_id, 31, 31, 0xa42);
        RTK_MII_READ(unit,base_id, 31, 16, &val);

        if(RTL8218B_TCA_CUT_A == cut_id)
        {
            /*Not maintain right now*/
        }
        else if(cut_id == RTL8218B_TCA_CUT_B)
        {
            /* Per-Chip */
            for (i=0; i<(sizeof(rtl8218B_0455B_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8218_phy0_macid + rtl8218B_0455B_rtl8380_perchip[i].phy, \
                    RTK_MII_MAXPAGE8380, (int)rtl8218B_0455B_rtl8380_perchip[i].reg, rtl8218B_0455B_rtl8380_perchip[i].val);
            }

            /*Enable Phy*/
            for(port_id = 0; port_id < 8; port_id++)
            {
               RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
               RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x00, 0x1140);
            }
            osal_time_mdelay(100);

            /*Patch request*/
            for(port_id = 0; port_id < 8; port_id++)
            {
               RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
               RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
            }
            osal_time_mdelay(300);

            /*Patch ready check*/
            for(port_id = 0; port_id < 8; port_id++)
            {
                for(loop = 0; loop < 100; loop++)
                {
                    RTK_MII_READ(unit,rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                    if(val & 0x40)break;
                }

                if(loop >= 100)
                {
                    osal_printf("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                    return;
                }
            }

            /*Use broadcast ID method to patch phy*/
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x16, 0xff00+HWP_PHY_ADDR(unit,rtl8218_phy0_macid));
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
            osal_time_mdelay(1);

            /* Per-Port */
            for (i=0; i<(sizeof(rtl8218B_0455B_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8218_phy0_macid, \
                    RTK_MII_MAXPAGE8380, (int)rtl8218B_0455B_rtl8380_perport[i].reg, rtl8218B_0455B_rtl8380_perport[i].val);
            }

            /*Disable broadcast ID*/
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x16, 0+HWP_PHY_ADDR(unit,rtl8218_phy0_macid));
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
            osal_time_mdelay(1);
        }
        else
        {
            /*right now nothing to do*/
        }

   }
   else if(val == RTL8218B_INT_TCA_CHIP_ID)
   {
        /*Enable Phy*/
        ioal_mem32_write(unit, 0xa100, 0x000080a8);
        osal_time_mdelay(100);

        /*Patch request*/
        for(port_id = 0; port_id < 8; port_id++)
        {
            RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
        }
        osal_time_mdelay(300);

        /*Patch ready check*/
        for(port_id = 0; port_id < 8; port_id++)
        {
            for(loop = 0; loop < 100; loop++)
            {
                RTK_MII_READ(unit,rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                if(val & 0x40)break;
            }

            if(loop >= 100)
            {
                osal_printf("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                return;
            }
        }
        /*Use broadcast ID method to pathc internal phy*/
        ioal_mem32_write(unit, 0x0120, 0xff2810);
        osal_time_mdelay(1);

        /* Per-Port */
        for (i=0; i<(sizeof(rtl838x_intPhy_0477B_perport)/sizeof(confcode_rv_t)); i++)
        {
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, \
            RTK_MII_MAXPAGE8380, (int)rtl838x_intPhy_0477B_perport[i].reg, rtl838x_intPhy_0477B_perport[i].val);
        }

        /*Disable broadcast ID*/
        ioal_mem32_write(unit, 0x0120, 0xff0010);
        osal_time_mdelay(1);
    }
    else if((val == RTL8218B_INT_MP_A_CHIP_ID) || (val == RTL8218B_CHIP_ID) || (val == RTL8218B_INT_MP_B_CHIP_ID))
    {
        /*MP Chip internal PHY AND external 8218b MP have the same RLNO: 6276,but at last internal phy will be 6275*/
        /*note that the definition of RTL8218B_INT_MP_CHIP_ID should be 0x6275 in B-cut, mantis#0012977*/
        if((rtl8218_phy0_macid == 0) || (rtl8218_phy0_macid == 16))
        {
            /*CHIP Version*/
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1b, 0x4);
            RTK_MII_READ(unit,rtl8218_phy0_macid, 0xfff, 0x1c, &cut_id);

            /* Per-Chip */
            if(0 == cut_id)
            {
                for (i=0; i<(sizeof(rtl8218B_6276A_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid + rtl8218B_6276A_rtl8380_perchip[i].phy, \
                    RTK_MII_MAXPAGE8380, (int)rtl8218B_6276A_rtl8380_perchip[i].reg, rtl8218B_6276A_rtl8380_perchip[i].val);
                }
            }
            else
            {
                for (i=0; i<(sizeof(rtl8218B_6276B_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid + rtl8218B_6276B_rtl8380_perchip[i].phy, \
                    RTK_MII_MAXPAGE8380, (int)rtl8218B_6276B_rtl8380_perchip[i].reg, rtl8218B_6276B_rtl8380_perchip[i].val);
                }
            }

            /*Enable Phy*/
            for(port_id = 0; port_id < 8; port_id++)
            {
                RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
                RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x00, 0x1140);
            }
            osal_time_mdelay(100);

            /*Patch request*/
            for(port_id = 0; port_id < 8; port_id++)
            {
                RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
            }
            osal_time_mdelay(300);

            /*Patch ready check*/
            for(port_id = 0; port_id < 8; port_id++)
            {
                for(loop = 0; loop < 100; loop++)
                {
                    RTK_MII_READ(unit,rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                    if(val & 0x40)break;
                }

                if(loop >= 100)
                {
                    osal_printf("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                    return;
                }
            }

            /*Use broadcast ID method to patch phy*/
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x16, 0xff00+HWP_PHY_ADDR(unit,rtl8218_phy0_macid));
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
            osal_time_mdelay(1);


            /* Per-Port */
            if(0 == cut_id)
            {
                for (i=0; i<(sizeof(rtl8218B_6276A_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid, \
                    RTK_MII_MAXPAGE8380, (int)rtl8218B_6276A_rtl8380_perport[i].reg, rtl8218B_6276A_rtl8380_perport[i].val);
                }
            }
            else
            {
                for (i=0; i<(sizeof(rtl8218B_6276B_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid, \
                    RTK_MII_MAXPAGE8380, (int)rtl8218B_6276B_rtl8380_perport[i].reg, rtl8218B_6276B_rtl8380_perport[i].val);
                }
            }

            /*Disable broadcast ID*/
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x16, 0+HWP_PHY_ADDR(unit,rtl8218_phy0_macid));
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
            osal_time_mdelay(1);
        }
        else if(rtl8218_phy0_macid == 8)
        {
            if(val == RTL8218B_INT_MP_A_CHIP_ID)
            {
                /*Enable Phy*/
                ioal_mem32_read(unit, 0xa100, &val);
                val |= 1 << 15;
                ioal_mem32_write(unit, 0xa100, val);
                osal_time_mdelay(100);

                /*Patch request*/
                for(port_id = 0; port_id < 8; port_id++)
                {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
                }
                osal_time_mdelay(300);

                /*Patch ready check*/
                for(port_id = 0; port_id < 8; port_id++)
                {
                    for(loop = 0; loop < 100; loop++)
                    {
                        RTK_MII_READ(unit,rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                        if(val & 0x40)break;
                    }

                    if(loop >= 100)
                    {
                        osal_printf("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                        return;
                    }
                }

                /*phy per-port patch*/
                for(port_id = 0; port_id < 8; port_id++)
                {
                    /* Per-Port */
                    for (i=0; i<(sizeof(rtl838x_6275A_intPhy_perport)/sizeof(confcode_rv_t)); i++)
                    {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, \
                            RTK_MII_MAXPAGE8380, (int)rtl838x_6275A_intPhy_perport[i].reg, rtl838x_6275A_intPhy_perport[i].val);
                    }
                }
            }
            else if(val == RTL8218B_INT_MP_B_CHIP_ID)
            {
                /*Enable Phy*/
                ioal_mem32_read(unit, 0xa100, &val);
                val |= 1 << 15;
                ioal_mem32_write(unit, 0xa100, val);
                osal_time_mdelay(100);

                /*Patch request*/
                for(port_id = 0; port_id < 8; port_id++)
                {
                   RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                   RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
                }
                osal_time_mdelay(300);

                /*Patch ready check*/
                for(port_id = 0; port_id < 8; port_id++)
                {
                    for(loop = 0; loop < 100; loop++)
                    {
                        RTK_MII_READ(unit,rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                        if(val & 0x40)break;
                    }

                    if(loop >= 100)
                    {
                        osal_printf("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                        return;
                    }
                }

                /*phy per-port patch*/
                for(port_id = 0; port_id < 8; port_id++)
                {
                    /* Per-Port */
                    for (i=0; i<(sizeof(rtl838x_6275B_intPhy_perport)/sizeof(confcode_rv_t)); i++)
                    {
                    RTK_MII_WRITE(unit,rtl8218_phy0_macid+port_id, \
                            RTK_MII_MAXPAGE8380, (int)rtl838x_6275B_intPhy_perport[i].reg, rtl838x_6275B_intPhy_perport[i].val);
                    }
                }
            }
            else
            {
                /*Do nothing*/
            }
        }
        else
        {
            /*right now nothing to do*/
        }
    }

    return;
} /* end of rtl8218b_rtl8380_config */

void rtl8218fb_rtl8380_config(uint32 unit, uint8 phy_base_macId)
{
    int rtl8218fb_phy0_macid = phy_base_macId;
    unsigned int val, cut_id;
    int i, port_id, base_id;
    int loop;

    base_id = rtl8218fb_phy0_macid;

    /*Check the 8218FB internal chip ID*/
    RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 31, 27, 0x0002);
    RTK_MII_READ(unit,rtl8218fb_phy0_macid, 31, 28, &val);


    if(val != 0x6276)
    {
        osal_printf("### Error chip ID = 0x%08x ###\n", val);
        return;
    }

     /*Check the 8218B internal cut ID*/
     RTK_MII_WRITE(unit,base_id, 31, 27, 0x0004);
     RTK_MII_READ(unit,base_id, 31, 28, &cut_id);

    /*Current not check CUT-ID,all use version-c patch*/

    /* Per-Chip */
    for (i=0; i<(sizeof(rtl8218fB_revB_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
    {
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid + rtl8218fB_revB_rtl8380_perchip[i].phy, \
            RTK_MII_MAXPAGE8380, (int)rtl8218fB_revB_rtl8380_perchip[i].reg, rtl8218fB_revB_rtl8380_perchip[i].val);
    }

    /************************************** 8 UTP **************************************/
     /*Force Copper*/
    for(port_id = 0; port_id < 8; port_id++)
     {
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid+port_id, 0xfff, 0x1e, 0x0001);
    }

     /*Enable Phy*/
     for(port_id = 0; port_id < 8; port_id++)
     {
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid+port_id, 0xfff, 0x00, 0x1140);
    }
    osal_time_mdelay(100);

    /*Patch request*/
   for(port_id = 0; port_id < 8; port_id++)
   {
       RTK_MII_WRITE(unit,rtl8218fb_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
       RTK_MII_WRITE(unit,rtl8218fb_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
   }
    osal_time_mdelay(300);

   /*Patch ready check*/
   for(port_id = 0; port_id < 8; port_id++)
   {
        for(loop = 0; loop < 100; loop++)
        {
            RTK_MII_READ(unit,rtl8218fb_phy0_macid + port_id, 0xb80, 0x10, &val);
            if(val & 0x40)break;
        }

        if(loop >= 100)
        {
            osal_printf("18fb phy port %d not ready!\n",port_id+rtl8218fb_phy0_macid);
            return;
        }
   }

   /*Use broadcast ID method to pathc internal phy*/
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x16, 0xff00+rtl8218fb_phy0_macid);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);
   osal_time_mdelay(1);


    /* Per-Port - UTP*/
    for(port_id = 0; port_id < 8; port_id++)
    {
                for (i=0; i<(sizeof(rtl8218fB_revB_rtl8380_utp_perport)/sizeof(confcode_rv_t)); i++)
        {
            RTK_MII_WRITE(unit,rtl8218fb_phy0_macid + port_id, \
                    RTK_MII_MAXPAGE8380, (int)rtl8218fB_revB_rtl8380_utp_perport[i].reg, rtl8218fB_revB_rtl8380_utp_perport[i].val);
         }
    }

   /*Disable broadcast ID*/
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x16, 0+rtl8218fb_phy0_macid);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);


    /************************************** 4 Fiber  **************************************/
   /*Use broadcast ID method to pathc internal phy*/
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x16, 0xf000+rtl8218fb_phy0_macid+4);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);
   osal_time_mdelay(1);

    /* Per-Port - Fiber*/
     for (i=0; i<(sizeof(rtl8218fB_revB_rtl8380_fiber_perport)/sizeof(confcode_rv_t)); i++)
    {
        RTK_MII_WRITE(unit,rtl8218fb_phy0_macid + 4, \
                RTK_MII_MAXPAGE8380, (int)rtl8218fB_revB_rtl8380_fiber_perport[i].reg, rtl8218fB_revB_rtl8380_fiber_perport[i].val);
     }

   /*Disable broadcast ID*/
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x16, 0+rtl8218fb_phy0_macid+4);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   RTK_MII_WRITE(unit,rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);


    return;
} /* end of rtl8218b_rtl8380_config */


void rtl8214fc_rtl8380_config(uint32 unit, uint8 phyid)
{
    rtk_port_t rtl8214_phy0_macid = phyid;
    unsigned int val, cut_id;
    int i;
    rtk_port_t port_id, base_id;
    int loop;

    base_id = rtl8214_phy0_macid;

    /*Switch to Copper address*/
    RTK_MII_WRITE(unit,base_id, 0, 30, 0x0001);

    RTK_MII_WRITE(unit,base_id, 0, 31, 0x0a42);

    /*Check the 8214B internal chip ID*/
    RTK_MII_WRITE(unit,base_id, 31, 27, 0x0002);
    RTK_MII_READ(unit,base_id, 31, 28, &val);
    if((val != RTL8214FC_MP_CHIP_ID))
    {
        PRINT_PHY_INITIAL_MESSGE("Error chip ID", val);
        return;
    }

    /*Check the 8214FC internal cut ID*/
    RTK_MII_WRITE(unit,base_id, RTK_MII_MAXPAGE8380, 27, 0x0004);
    RTK_MII_READ(unit,base_id, RTK_MII_MAXPAGE8380, 28, &cut_id);

    /*Back to Auto address*/
    RTK_MII_WRITE(unit,base_id, 0, 30, 0x0000);

    if(val == RTL8214FC_MP_CHIP_ID)
    {
        if(cut_id == RTL8214FC_TCA_CUT_A)
        {
            /* Per-Chip */
            for (i=0; i<(sizeof(construct_rtl8214fc_6276A_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214_phy0_macid + construct_rtl8214fc_6276A_rtl8380_perchip[i].phy, \
                    RTK_MII_MAXPAGE8380, (int)construct_rtl8214fc_6276A_rtl8380_perchip[i].reg, construct_rtl8214fc_6276A_rtl8380_perchip[i].val);
            }
        }
        else
        {
            uint32 page_temp;
            uint32 val_temp;

            page_temp = 0;

            /* Per-Chip */
            for (i=0; i<(sizeof(construct_rtl8214fc_6276B_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
            {
                /*Fix 14FC PHYID Fixed bug*/
                if(construct_rtl8214fc_6276B_rtl8380_perchip[i].reg == 0x1f)
                    page_temp = construct_rtl8214fc_6276B_rtl8380_perchip[i].val;

                if((construct_rtl8214fc_6276B_rtl8380_perchip[i].reg == 0x13) && (page_temp == 0x260))
                {
                    RTK_MII_READ(unit,rtl8214_phy0_macid + construct_rtl8214fc_6276B_rtl8380_perchip[i].phy, 0x260, 0x13, &val_temp);
                    val_temp = (val_temp & 0x1f00) | (construct_rtl8214fc_6276B_rtl8380_perchip[i].val & 0xe0ff);
                    RTK_MII_WRITE(unit,rtl8214_phy0_macid + construct_rtl8214fc_6276B_rtl8380_perchip[i].phy, \
                            RTK_MII_MAXPAGE8380, (int)construct_rtl8214fc_6276B_rtl8380_perchip[i].reg, val_temp);
                }
                else
                {
                    RTK_MII_WRITE(unit,rtl8214_phy0_macid + construct_rtl8214fc_6276B_rtl8380_perchip[i].phy, \
                    RTK_MII_MAXPAGE8380, (int)construct_rtl8214fc_6276B_rtl8380_perchip[i].reg, construct_rtl8214fc_6276B_rtl8380_perchip[i].val);
                }
            }


            /*Force Copper*/
            for(port_id = 0; port_id < 4; port_id++)
            {
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x1e, 0x0001);
            }

            /*Enable Phy*/
            for(port_id = 0; port_id < 4; port_id++)
            {
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x00, 0x1140);
            }
            osal_time_mdelay(100);

            /*Disable AutoSending Featrue*/
            for(port_id = 0; port_id < 4; port_id++)
            {
                for(loop = 0; loop < 100; loop++)
                {
                    RTK_MII_READ(unit,rtl8214_phy0_macid + port_id, 0x0a42, 0x10, &val);
                    if((val & 0x7)>=3)break;
                }

                if(loop >= 100)
                {
                    osal_printf("Disable Port %d AutoSending Featrue Not OK!\n",port_id+rtl8214_phy0_macid);
                    return;
                }
            }


            /*Patch request*/
            for(port_id = 0; port_id < 4; port_id++)
            {
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
            }
            osal_time_mdelay(300);

            /*Patch ready check*/
            for(port_id = 0; port_id < 4; port_id++)
            {
                for(loop = 0; loop < 100; loop++)
                {
                    RTK_MII_READ(unit,rtl8214_phy0_macid + port_id, 0xb80, 0x10, &val);
                    if(val & 0x40)break;
                }

                if(loop >= 100)
                {
                    osal_printf("Internal phy port %u not ready!\n",port_id+rtl8214_phy0_macid);
                    return;
                }
            }

            /*Use broadcast ID method to patch phy*/
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x001d, 0x0008);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x001f, 0x266);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x16, 0xff00+HWP_PHY_ADDR(unit,rtl8214_phy0_macid));
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x1d, 0x0000);
            osal_time_mdelay(1);

            /* Per-Port */
            for (i=0; i<(sizeof(construct_rtl8214fc_6276B_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214_phy0_macid, \
                RTK_MII_MAXPAGE8380, (int)construct_rtl8214fc_6276B_rtl8380_perport[i].reg, construct_rtl8214fc_6276B_rtl8380_perport[i].val);
            }

            /*Disable broadcast ID*/
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x001d, 0x0008);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x001f, 0x266);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x16, 0+HWP_PHY_ADDR(unit,rtl8214_phy0_macid));
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x1f, 0x0000);
            RTK_MII_WRITE(unit,rtl8214_phy0_macid, 0xfff, 0x1d, 0x0000);
            osal_time_mdelay(1);

            /*Enable Auto*/
            for(port_id = 0; port_id < 4; port_id++)
            {
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
                RTK_MII_WRITE(unit,rtl8214_phy0_macid+port_id, 0xfff, 0x1e, 0x0000);
            }

        }
    }

    return;
} /* end of rtl8214fc_rtl8380_config */


#endif//CONFIG_SDK_RTL8380

#if (defined(CONFIG_SDK_RTL9300) && defined(CONFIG_SDK_RTL8218B))
/* Function Name:
 *      rtl8218b_rtl9300_config
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8218b_rtl9300_config(uint32 unit, uint8 phy_base_macId)
{
    uint32 portnum = 8;
    uint32 i = 0;
    uint32 j = 0;
    uint32 temp = 0;
    uint32 patchready = 0;
    uint16 regdata;
    uint8  regaddr;
    uint32 value;

    for (i = 0; i < sizeof(rtl8218b_rtl9300_perchip) / sizeof(confcode_rv_t); i++)
    {
        regaddr = rtl8218b_rtl9300_perchip[i].reg;
        regdata = rtl8218b_rtl9300_perchip[i].val;
        RTK_MII_WRITE(unit, phy_base_macId, 0xfff, regaddr, regdata);
    }

    for (i=0; i<portnum; i++)
    {
        /* phy power on */
        RTK_MII_READ(unit, phy_base_macId + i, 0, 0, &value);
        RTK_MII_WRITE(unit, phy_base_macId + i, 0, 0, 0x1140);

        /*#patch request*/
        RTK_MII_WRITE(unit, phy_base_macId+i, 0xb82, 0x10, 0x0010);

        for (temp = 0; temp < 1000; temp++)
        {
            RTK_MII_READ(unit, phy_base_macId + i, 0xb80, 0x10, &patchready);
            if(patchready & 0x40)
            {
                break;
            }
        }
        if (temp >= 1000)
        {
            RTK_MII_READ(unit, phy_base_macId + i, 0x0b80, 0x10, &patchready);
            osal_printf("8218B phy port %d not ready! patchready:0x%x\n", phy_base_macId+i, patchready);
            return;
        }
        for (j = 0; j < sizeof(rtl8218b_rtl9300_perport) / sizeof(confcode_rv_t); j++)
        {
            regaddr = rtl8218b_rtl9300_perport[j].reg;
            regdata = rtl8218b_rtl9300_perport[j].val;
            RTK_MII_WRITE(unit, phy_base_macId + i, 0xfff, regaddr, regdata);
        }

        RTK_MII_WRITE(unit, phy_base_macId + i, 0, 0, value);
    }

}
#endif

#if defined(CONFIG_SDK_RTL8218D)
/* Function Name:
 *      _rtl8218d_serdesMode_get
 * Description:
 *      Get SerDes mode of the PHY
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218d
 * Output:
 *      pMode - serdes mode
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAIED
 * Note:
 *      None
 */
int32
_rtl8218d_serdesMode_get(uint32 unit, uint8 phy_base_macId, rt_serdesMode_t *pMode)
{
    uint32      data, blk;

    RTK_MII_READ(unit, phy_base_macId, 0, 30, &blk);
    RTK_MII_WRITE(unit, phy_base_macId, 0, 30, 8);
    RTK_MII_READ(unit, phy_base_macId, 0x260, 18, &data);
    RTK_MII_WRITE(unit, phy_base_macId, 0, 30, blk);

    switch (data & 0xF0)
    {
      case 0xD0:
        *pMode = RTK_MII_QSGMII;
        break;
      case 0xB0:
        *pMode = RTK_MII_XSGMII;
        break;
      default:
        RTK_DBG_PRINT(0, "unit %u 8218D phy port %u serdes mode: %X unknown type.\n", unit, (uint32)phy_base_macId, (data & 0xF0));
        return RT_ERR_FAILED;
    }
    return RT_ERR_OK;
}

#if defined(CONFIG_SDK_RTL8380)

/* Function Name:
 *      _rtl8218d_rtl8380_config
 * Description:
 *      Configuration code for RTL8218d.
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218d
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void _rtl8218d_rtl8380_config(uint32 unit, uint8 phy_base_macId)
{
    uint32 i = 0;

    for (i = 0; i < sizeof(rtl8218d_bT_qsgmii_rtl8380_perchip)/sizeof(confcode_prv_t); i++)
    {
        RTK_MII_WRITE(unit, phy_base_macId + rtl8218d_bT_qsgmii_rtl8380_perchip[i].phy,\
            0xfff,  (int)rtl8218d_bT_qsgmii_rtl8380_perchip[i].reg, rtl8218d_bT_qsgmii_rtl8380_perchip[i].val);
    }

}
#endif /* CONFIG_SDK_RTL8380 */

#if defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310)

/* Function Name:
 *      rtl8218d_rtl9300_config
 * Description:
 *      Configuration code for RTL8218d.
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218d
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void _rtl8218d_rtl9300_config(uint32 unit, uint8 phy_base_macId)
{
    uint32 i = 0;
    int32   ret;
    rt_serdesMode_t mode;

    if ((ret = _rtl8218d_serdesMode_get(unit, phy_base_macId, &mode)) != RT_ERR_OK)
        return;

    if(RTK_MII_QSGMII == mode)
    {
        for (i=0; i<(sizeof(rtl8218d_bT_qsgmii_rtl9300_perchip)/sizeof(confcode_prv_t)); i++)
        {
            RTK_MII_WRITE(unit,phy_base_macId + rtl8218d_bT_qsgmii_rtl9300_perchip[i].phy, \
                HAL_MIIM_PAGE_ID_MAX(unit), (int)rtl8218d_bT_qsgmii_rtl9300_perchip[i].reg, rtl8218d_bT_qsgmii_rtl9300_perchip[i].val);
        }
    }
    if(RTK_MII_XSGMII == mode)
    {
        for (i=0; i<(sizeof(rtl8218d_bT_xsgmii_rtl9300_perchip)/sizeof(confcode_prv_t)); i++)
        {
            RTK_MII_WRITE(unit,phy_base_macId + rtl8218d_bT_xsgmii_rtl9300_perchip[i].phy, \
                HAL_MIIM_PAGE_ID_MAX(unit), (int)rtl8218d_bT_xsgmii_rtl9300_perchip[i].reg, rtl8218d_bT_xsgmii_rtl9300_perchip[i].val);
        }
    }
}
#endif /* CONFIG_SDK_RTL9300 */

#if defined(CONFIG_SDK_RTL8390)
/* Function Name:
 *      _rtl8218d_rtl8390_config
 * Description:
 *      Configuration code for RTL8218d.
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218d
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
_rtl8218d_rtl8390_config(uint32 unit, uint8 phy_base_macId)
{

    int     i;

    RT_LOG(LOG_TRACE, (MOD_HAL|MOD_PORT), "U%u,P%u", unit, phy_base_macId);

    for (i = 0; i < sizeof(rtl8218d_aT_qsgmii_8390_perchip)/sizeof(confcode_rv_t); i++)
    {
        //RT_LOG(LOG_TRACE, (MOD_HAL|MOD_PORT), "0x%X 0x%04X", rtl8218d_aT_qsgmii_8390_perchip[i].reg, rtl8218d_aT_qsgmii_8390_perchip[i].val);

        RTK_MII_WRITE(unit, phy_base_macId, RTK_MII_MAXPAGE8390,
                        (int)rtl8218d_aT_qsgmii_8390_perchip[i].reg,
                        (int)rtl8218d_aT_qsgmii_8390_perchip[i].val);
    }

}
#endif /* CONFIG_SDK_RTL8218D */

void rtl8218d_config(uint32 unit, uint8 phy_base_macId)
{
    uint32 portnum = 8;
    uint32 i = 0;
    uint32 j = 0;
    uint32 value;
    rtk_enable_t    enable;

    if (phy_8218d_efuseSwLoadEnable_get(unit, phy_base_macId, &enable) == RT_ERR_OK)
    {
        if (enable == ENABLED)
        {
            phy_8218d_efuseSw_load(unit, phy_base_macId);
        }
    }


#if defined(CONFIG_SDK_RTL8380)
    if (HWP_8330_FAMILY_ID(unit) || HWP_8380_FAMILY_ID(unit))
    {
        _rtl8218d_rtl8380_config(unit,phy_base_macId);
    }
#endif


#if defined(CONFIG_SDK_RTL8390)
    if (HWP_8390_50_FAMILY(unit))
    {
        _rtl8218d_rtl8390_config(unit,phy_base_macId);
    }
#endif

#if defined(CONFIG_SDK_RTL9300) || defined(CONFIG_SDK_RTL9310)
    if(HWP_9300_FAMILY_ID(unit) || HWP_9310_FAMILY_ID(unit))
    {
        _rtl8218d_rtl9300_config(unit,phy_base_macId);
    }
#endif

    for (i=0; i<portnum; i++)
    {
        RTK_MII_READ(unit, phy_base_macId + i, HAL_MIIM_PAGE_ID_MAX(unit),0x0, &value);

        for (j = 0; j < sizeof(rtl8218d_bT_1000FM_INRX_perport) / sizeof(confcode_rv_t); j++)
        {
            RTK_MII_WRITE(unit, phy_base_macId + i, HAL_MIIM_PAGE_ID_MAX(unit),
                rtl8218d_bT_1000FM_INRX_perport[j].reg, rtl8218d_bT_1000FM_INRX_perport[j].val);
        }

        for (j = 0; j < sizeof(rtl8218d_bT_1000FS_INRX_perport) / sizeof(confcode_rv_t); j++)
        {
            RTK_MII_WRITE(unit, phy_base_macId + i, HAL_MIIM_PAGE_ID_MAX(unit),
                rtl8218d_bT_1000FS_INRX_perport[j].reg, rtl8218d_bT_1000FS_INRX_perport[j].val);
        }

        for (j = 0; j < sizeof(rtl8218d_bT_100_INRX_perport) / sizeof(confcode_rv_t); j++)
        {
            RTK_MII_WRITE(unit, phy_base_macId + i, HAL_MIIM_PAGE_ID_MAX(unit),
                rtl8218d_bT_100_INRX_perport[j].reg, rtl8218d_bT_100_INRX_perport[j].val);
        }

        for (j = 0; j < sizeof(rtl8218d_bT_green_perport) / sizeof(confcode_rv_t); j++)
        {
            RTK_MII_WRITE(unit, phy_base_macId + i, HAL_MIIM_PAGE_ID_MAX(unit),
                rtl8218d_bT_green_perport[j].reg, rtl8218d_bT_green_perport[j].val);
        }

        RTK_MII_WRITE(unit, phy_base_macId + i, HAL_MIIM_PAGE_ID_MAX(unit), 0x0, value);

    }
}
#endif

/*
 * Function Declaration
 */
/* Function Name:
 *      rtl8218_config
 * Description:
 *      Configuration code for RTL8218.
 * Input:
 *      phyId - the phyid of PHY0 of the RTL8218
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void rtl8208_config(uint32 unit, uint8 phyid, int max_page)
{
    rtk_port_t rtl8208_phy0_macid = phyid;
    unsigned int val;
    int i, port_id;

    /*Check the 8208 internal chip ID*/
    /*Page0 Reg3   dft:0xc882  bit[9:4] modle number:0x8  [3:0] revision number:0x2 */
    RTK_MII_READ(unit,phyid, 0, 3, &val);
    val = (val >> 4) & 0x3f;

    if(val != RTL8208_CHIP_ID)
    {
        PRINT_PHY_INITIAL_MESSGE("Error chip ID",val);
        return;
    }

    PRINT_PHY_INITIAL_MESSGE("RTL8208", rtl8208_phy0_macid);

    /* Per-Chip */
    for (i=0; i<(sizeof(rtl8208_perchip)/sizeof(confcode_prv_t)); i++)
    {
        RTK_MII_WRITE(unit,rtl8208_phy0_macid + rtl8208_perchip[i].phy, \
        max_page, (int)rtl8208_perchip[i].reg, rtl8208_perchip[i].val);
    }

    /* Per-Port */
    for(port_id = 0; port_id < 8; port_id++)
    {
        for (i=0; i<(sizeof(rtl8208_perport)/sizeof(confcode_rv_t)); i++)
        {
            RTK_MII_WRITE(unit,rtl8208_phy0_macid + port_id, \
            max_page, (int)rtl8208_perport[i].reg, rtl8208_perport[i].val);
        }
    }

    return;
} /* end of rtl8208_config */
void rtl8208_rtl8380_config(uint32 unit, uint8 phyid)
{
    rtl8208_config(unit, phyid, RTK_MII_MAXPAGE8380);
}

void rtl8208_rtl8390_config(uint32 unit, uint8 phyid)
{
    rtl8208_config(unit, phyid, RTK_MII_MAXPAGE8390);
}




/* Function Name:
 *      rtl8214fb_config
 * Description:
 *      Configuration code for RTL8214FB.
 * Input:
 *      phyId - the phyid of PHY0 of the RTL8214FB
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
static void rtl8214fb_config(uint32 unit, uint8 phyid,int max_page)
{
    rtk_port_t rtl8214fb_phy0_macid = phyid;
    int i;
    unsigned int val, forceReg;
    unsigned int version, data;
    int is_fb, is_12b = 0;

    RTK_MII_READ(unit,rtl8214fb_phy0_macid,  10, 18, &version);

    is_fb = version & 0xF;
    version = (version >> 8) & 0xF;

    if (is_fb == 0)
    {
        RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+3, 8, 18, 0x93f0);
        RTK_MII_READ(unit,rtl8214fb_phy0_macid+3, 8, 19, &data);
        if (((data & 0xF) == 0xE) || ((data & 0xF) == 0x8))
            is_12b = 1;
        else
            is_12b = 0;
    }

    if(version == 0)
    {
        /* Configuration code, 2010-12-22 version for version B */
        if (is_fb)
            PRINT_PHY_INITIAL_MESSGE("RTL8214FB(Ver.B)", rtl8214fb_phy0_macid);
        else if (is_12b)
            PRINT_PHY_INITIAL_MESSGE("RTL8212B(Ver.B)", rtl8214fb_phy0_macid);
        else
            PRINT_PHY_INITIAL_MESSGE("RTL8214B(Ver.B)", rtl8214fb_phy0_macid);

        /* RTL8214FB Revision B Per-Chip */
        for (i=0; i<(sizeof(rtl8214fb_perchip_revB)/sizeof(confcode_prv_t)); i++)
        {
            rtk_portmask_t pm;
            osal_memset(&pm, 0, sizeof(rtk_portmask_t));
            RTK_PORTMASK_PORT_SET(pm, (rtl8214fb_phy0_macid + rtl8214fb_perchip_revB[i].phy));
            RTK_MII_PORTMASK_WRITE(unit, pm, max_page, \
                (int)rtl8214fb_perchip_revB[i].reg, (int)rtl8214fb_perchip_revB[i].val);
        }
    }/*end Version B*/
    else//for forward compatiblility use, so delete:::::::::: if (version == 2)
    {
        /* Configuration code, 2011-04-21 version for version C */
        if (is_fb)
            PRINT_PHY_INITIAL_MESSGE("RTL8214FB(Ver.C)", rtl8214fb_phy0_macid);
        else if (is_12b)
            PRINT_PHY_INITIAL_MESSGE("RTL8212B(Ver.C)", rtl8214fb_phy0_macid);
        else
            PRINT_PHY_INITIAL_MESSGE("RTL8214B(Ver.C)", rtl8214fb_phy0_macid);

        /* RTL8214FB Revision C Per-Chip */
#if defined(CONFIG_SDK_RTL8390)
        if (HWP_8390_50_FAMILY(unit))
        {
            for (i=0; i<(sizeof(rtl8214fb_rtl8390_perchip_revC_1)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + rtl8214fb_rtl8390_perchip_revC_1[i].phy, \
                        max_page,
                        (int)rtl8214fb_rtl8390_perchip_revC_1[i].reg,
                        (int)rtl8214fb_rtl8390_perchip_revC_1[i].val);
            }

            osal_time_mdelay(100);

            for (i=0; i<(sizeof(rtl8214fb_rtl8390_perchip_revC_2)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + rtl8214fb_rtl8390_perchip_revC_2[i].phy, \
                        max_page,
                        (int)rtl8214fb_rtl8390_perchip_revC_2[i].reg,
                        (int)rtl8214fb_rtl8390_perchip_revC_2[i].val);
            }

            osal_time_mdelay(100);

            for (i=0; i<(sizeof(rtl8214fb_rtl8390_perchip_revC_3)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + rtl8214fb_rtl8390_perchip_revC_3[i].phy, \
                        max_page,
                        (int)rtl8214fb_rtl8390_perchip_revC_3[i].reg,
                        (int)rtl8214fb_rtl8390_perchip_revC_3[i].val);
            }

            /* The configuration must be keep for 839x/5x, which isn't in the patch */
            /* Disable serdes pre-emphasis */
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid,
                    max_page, 0x1f, 0xf);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid,
                    max_page, 0x1e, 0xD);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid,
                    max_page, 0x18, 0x8088);

            //"Auto Mode Standard Register ..."
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 1,
                    max_page, 31, 8);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 1,
                    max_page, 16, 0x0000);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 1,
                    max_page, 31, 8);
            osal_time_mdelay(1000);

            //"Reset Ser-Des ..."
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 31, 8);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 28, 0xFF00);
            osal_time_mdelay(100);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 28, 0x0);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 31, 8);

            osal_time_mdelay(1000);
        }
        else
#endif
#if defined(CONFIG_SDK_RTL8380)
        if (HWP_8380_30_FAMILY(unit))
        {
            for (i=0; i<(sizeof(rtl8214fb_rtl8380_perchip_revC_1)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + rtl8214fb_rtl8380_perchip_revC_1[i].phy, \
                        max_page,
                        (int)rtl8214fb_rtl8380_perchip_revC_1[i].reg,
                        (int)rtl8214fb_rtl8380_perchip_revC_1[i].val);
            }

            osal_time_mdelay(100);

            for (i=0; i<(sizeof(rtl8214fb_rtl8380_perchip_revC_2)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + rtl8214fb_rtl8380_perchip_revC_2[i].phy, \
                        max_page,
                        (int)rtl8214fb_rtl8380_perchip_revC_2[i].reg,
                        (int)rtl8214fb_rtl8380_perchip_revC_2[i].val);
            }

            osal_time_mdelay(100);

            for (i=0; i<(sizeof(rtl8214fb_rtl8380_perchip_revC_3)/sizeof(confcode_prv_t)); i++)
            {
                RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + rtl8214fb_rtl8380_perchip_revC_3[i].phy, \
                        max_page,
                        (int)rtl8214fb_rtl8380_perchip_revC_3[i].reg,
                        (int)rtl8214fb_rtl8380_perchip_revC_3[i].val);
            }

            //"Auto Mode Standard Register ..."
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 1,
                    max_page, 31, 8);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 1,
                    max_page, 16, 0x0000);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 1,
                    max_page, 31, 8);
            osal_time_mdelay(1000);

            //"Reset Ser-Des ..."
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 31, 8);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 28, 0xFF00);
            osal_time_mdelay(100);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 28, 0x0);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid + 3,
                    max_page, 31, 8);

            osal_time_mdelay(1000);
        }
        else
#endif
        {
            /* else condition for the chips that do not need to configure */
        }


        /* Write the PHY 0-3, Page 8, Register 30 bit[7:6] = 0b00 */
        for (i=0; i<4; i++)
        {
            RTK_MII_READ(unit,rtl8214fb_phy0_macid+i, 8, 30, &val);
            val &= ~(1<<7);
            val &= ~(1<<6);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 8, 30, val);
        }

        /* power down PHY in copper & fiber media */
        for (i=0; i<4; i++)
        {
            RTK_MII_READ(unit,rtl8214fb_phy0_macid+i, 8, 30, &forceReg);    /* store the original register value */

            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 8, 30, (forceReg & ~(0x3<<12)) | (0x3<<12));
            RTK_MII_READ(unit,rtl8214fb_phy0_macid+i, 0, 0, &val);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 0, 0, (val | (0x1 << 11)));

            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 8, 30, (forceReg & ~(0x3<<12)) | (0x2<<12));
            RTK_MII_READ(unit,rtl8214fb_phy0_macid+i, 0, 0, &val);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 0, 0, (val | (0x1 << 11)));

            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 8, 30, forceReg);   /* restore the original register value */
        }

        /* power down the serdes 1 for RTL8212B */
        if (is_fb == 0 && is_12b == 1)
        {
            RTK_MII_READ(unit,rtl8214fb_phy0_macid+3, 8, 28, &val);
            val |= 0xc000;
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+3, 8, 28, val);
        }

        /* Configure option when 1000-X Nway is failure, try force mode */
        for (i=0; i<4; i++)
        {
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 8, 31, 0x000F);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 15, 30, 0x0018);
            RTK_MII_WRITE(unit,rtl8214fb_phy0_macid+i, 15, 25, 0x074D);
        }
    }
    return;
} /* end of rtl8214fb_config */

void rtl8214fb_rtl8380_config(uint32 unit, uint8 phyid)
{
    rtl8214fb_config(unit, phyid, RTK_MII_MAXPAGE8380);
}

void rtl8214fb_rtl8390_config(uint32 unit, uint8 phyid)
{
    rtl8214fb_config(unit, phyid, RTK_MII_MAXPAGE8390);
}

/* Function Name:
 *      construct_rtl8214fb_phyPowerOn
 * Description:
 *
 * Input:
 *      portid - Port number (0~28)
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void construct_rtl8214fb_phyPowerOn(uint32 unit, rtk_port_t portid)
{

    unsigned int forceReg;
    unsigned int val;

    CNSTRT_PRINT("%s()\n",__FUNCTION__);

    RTK_MII_READ(unit,portid, 8, 30, &forceReg);      /* store the original register value */
    RTK_MII_WRITE(unit,portid, 8, 30, (forceReg & ~(0x3<<12)) | (0x3<<12));
    RTK_MII_READ(unit,portid, 0, 0, &val);
    RTK_MII_WRITE(unit,portid, 0, 0, (val & ~(0x1 << 11)));

    RTK_MII_WRITE(unit,portid, 8, 30, (forceReg & ~(0x3<<12)) | (0x2<<12));
    RTK_MII_READ(unit,portid, 0, 0, &val);
    RTK_MII_WRITE(unit,portid, 0, 0, (val & ~(0x1 << 11)));

    RTK_MII_WRITE(unit,portid, 8, 30, forceReg);       /* restore the original register value */

    osal_time_mdelay(1000);
    return;
} /* end of construct_rtl8214fb_phyPowerOn */

/* Function Name:
 *      construct_rtl8214fb_phyPowerOff
 * Description:
 *      Get meida of port.
 * Input:
 *      portid - Port number (0~28)
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void construct_rtl8214fb_phyPowerOff(uint32 unit, rtk_port_t portid)
{
    unsigned int forceReg;
    unsigned int val;

    CNSTRT_PRINT("%s()\n",__FUNCTION__);

    RTK_MII_READ(unit,portid, 8, 30, &forceReg);      /* store the original register value */
    RTK_MII_WRITE(unit,portid, 8, 30, (forceReg & ~(0x3<<12)) | (0x3<<12));
    RTK_MII_READ(unit,portid, 0, 0, &val);
    RTK_MII_WRITE(unit,portid, 0, 0, (val | (0x1 << 11)));

    RTK_MII_WRITE(unit,portid, 8, 30, (forceReg & ~(0x3<<12)) | (0x2<<12));
    RTK_MII_READ(unit,portid, 0, 0, &val);
    RTK_MII_WRITE(unit,portid, 0, 0, (val | (0x1 << 11)));

    RTK_MII_WRITE(unit,portid, 8, 30, forceReg);       /* restore the original register value */
    return;
} /* end of construct_rtl8214fb_phyPowerOff */
#if defined(CONFIG_SDK_RTL8214C)
/* Function Name:
 *      _rtl8214c_indirect_read
 * Description:
 *      Get PHY register.
 * Input:
 *      portid - Port number (0~51)
 *      page   - PHY page (0~127)
 *      reg    - PHY register (0~31)
 *      val    - data to write
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
int _rtl8214c_indirect_read(uint32 unit, rtk_port_t port, int page, int reg, uint32 *val)
{
    unsigned int addr, data;

    /* bit [10:5] page; bit [4:0] register */
    addr = (page << 5) | reg;
    RTK_MII_WRITE(unit, port, 0, 29, 0x6601);
    RTK_MII_WRITE(unit, port, 0, 30, addr);

    RTK_MII_WRITE(unit, port, 0, 29, 0x6600);
    RTK_MII_WRITE(unit, port, 0, 30, 0x80);

    do
    {
        RTK_MII_WRITE(unit, port, 0, 29, 0x6600);
        RTK_MII_READ(unit, port, 0, 30, &data);
    } while (((data >> 8) & 0x1) != 0);

    RTK_MII_WRITE(unit, port, 0, 29, 0x6602);
    RTK_MII_READ(unit, port, 0, 30, val);

    return 0;
}   /* end of _rtl8214c_indirect_read */

/* Function Name:
 *      _rtl8214c_indirect_write
 * Description:
 *      Set PHY register.
 * Input:
 *      portid - Port number (0~51)
 *      page   - PHY page (0~127)
 *      reg    - PHY register (0~31)
 *      val    - data to write
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
int _rtl8214c_indirect_write(uint32 unit, rtk_port_t port, int page, int reg, uint32 val)
{
    unsigned int addr, data;

    /* bit [10:5] page; bit [4:0] register */
    addr = (page << 5) | reg;
    RTK_MII_WRITE(unit, port, 0, 29, 0x6601);
    RTK_MII_WRITE(unit, port, 0, 30, addr);

    RTK_MII_WRITE(unit, port, 0, 29, 0x6602);
    RTK_MII_WRITE(unit, port, 0, 30, val);

    RTK_MII_WRITE(unit, port, 0, 29, 0x6600);
    RTK_MII_WRITE(unit, port, 0, 30, 0xC0);

    do
    {
        RTK_MII_WRITE(unit, port, 0, 29, 0x6600);
        RTK_MII_READ(unit, port, 0, 30, &data);
    } while (((data >> 8) & 0x1) != 0);

    return 0;
}   /* end of _rtl8214c_indirect_write */

#if defined(CONFIG_SDK_RTL8380)
static void _rtl8214c_serdes_rtl8380_config(uint32 unit, rtk_port_t phy0_macid)
{
    _rtl8214c_indirect_write(unit, phy0_macid, 0x2a, 0, 0x84D2);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x2a, 1, 0xF995);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x2a, 2, 0x3DA1);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x2a, 3, 0x3960);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x2a, 4, 0x9728);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x2a, 5, 0xf83f);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x21, 3, 0x9D85);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x21, 4, 0xD810);

    _rtl8214c_indirect_write(unit, phy0_macid, 0x21, 2, 0xb682);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x21, 2, 0xb602);
    _rtl8214c_indirect_write(unit, phy0_macid, 0, 3, 0x7146);
    _rtl8214c_indirect_write(unit, phy0_macid, 0, 3, 0x7106);
}   /* end of _rtl8214c_serdes_rtl8380_config */
#endif  /* end of #if defined(CONFIG_SDK_RTL8380) */
static void _rtl8214c_serdes_config(uint32 unit, rtk_port_t phy0_macid)
{
    _rtl8214c_indirect_write(unit, phy0_macid, 0x2a, 2, 0x2da1);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x21, 2, 0xb682);
    _rtl8214c_indirect_write(unit, phy0_macid, 0x21, 2, 0xb602);
    _rtl8214c_indirect_write(unit, phy0_macid, 0, 3, 0x7146);
    _rtl8214c_indirect_write(unit, phy0_macid, 0, 3, 0x7106);
}   /* end of _rtl8214c_serdes_config */

/* Function Name:
 *      rtl8214c_config
 * Description:
 *      Configuration code for RTL8214c.
 * Input:
 *      macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8214c_config(uint32 unit, uint8 macId)
{
    rtk_port_t phy0_macid = HWP_PHY_BASE_MACID(unit, macId);
    uint32  val;
    uint8   sdsId;

    CNSTRT_PRINT("%s()\n", __FUNCTION__);

    #if defined(CONFIG_SDK_RTL8380)
    if (HWP_8380_30_FAMILY(unit))
    {
        _rtl8214c_serdes_rtl8380_config(unit, phy0_macid);
    }
    else
    #endif  /* end of #if defined(CONFIG_SDK_RTL8380) */
    {
        _rtl8214c_serdes_config(unit, phy0_macid);
    }

    sdsId = HWP_PORT_SDSID(unit, phy0_macid);

    _rtl8214c_indirect_read(unit, phy0_macid, 0, 0, &val);

    if (SERDES_POLARITY_CHANGE == HWP_SDS_RX_POLARITY(unit, sdsId))
    {
        val |= (1 << 9);
    }

    if (SERDES_POLARITY_CHANGE == HWP_SDS_TX_POLARITY(unit, sdsId))
    {
        val |= (1 << 8);
    }

    _rtl8214c_indirect_write(unit, phy0_macid, 0, 0, val);

    return;
} /* end of rtl8214c_config */
#endif  /* end of defined(CONFIG_SDK_RTL8214C) */

#if defined(CONFIG_SDK_RTL8295R)
/* Function Name:
 *      rtl8295r_config_patchParam_cust
 * Description:
 *      This fundion is used for customizing the TX related parameters for 8431 and DAC-Long of your demo board.
 *      It is called by rtl8295r_config_patchParam_init.
 *      By default, the patch parameters are using the defaults that defined above.
 *      If your demo board require different parameters,
 *      You can declare your own parameters and assign it using PHY_8295R_PATCH_PARAM_INIT.
 *      The patch is per-port base. That is, you can assign different paramters for each port.
 *      Please refer to the sample code here in the function.
 * Input:
 *      unit    - unit ID
 *      baseMacId    - base port ID of the PHY
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      We don't suggest put your parameters into the rtl8295r_oooo_xxxx_default array directly,
 *      it would easily get into trouble when you need to upgrade/patch SDK.
 */
void rtl8295r_config_patchParam_cust(uint32 unit, uint8 baseMacId)
{

#if defined(CONFIG_SDK_RTL8390)
    if (HWP_8390_50_FAMILY(unit))
    {
        if ((sizeof(rtl8295r_8390_s18431_myParam) != sizeof(rtl8295r_8390_s18431_default))
            || osal_memcmp(&rtl8295r_8390_s18431_default, &rtl8295r_8390_s18431_myParam, sizeof(rtl8295r_8390_s18431_myParam)) != 0)
        {
            PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S18431, rtl8295r_8390_s18431_myParam, phy_8295_pageRegVal_t);
        }
        if ((sizeof(rtl8295r_8390_s1dacLong_myParam) != sizeof(rtl8295r_8390_s1dacLong_default))
            || osal_memcmp(&rtl8295r_8390_s1dacLong_default, &rtl8295r_8390_s1dacLong_myParam, sizeof(rtl8295r_8390_s1dacLong_myParam)) != 0)
        {
           PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S1DACLONG, rtl8295r_8390_s1dacLong_myParam, phy_8295_pageRegVal_t);
        }

        phy_8295r_rxCaliConf_set(unit, &rtl8295r_8390_rxCaliConf_myParam);
    }
#endif

#if defined(CONFIG_SDK_RTL9300)
    if (HWP_9300_FAMILY_ID(unit))
    {
        if ((sizeof(rtl8295r_9300_s18431_myParam) != sizeof(rtl8295r_9300_s18431_default))
            || osal_memcmp(&rtl8295r_9300_s18431_default, &rtl8295r_9300_s18431_myParam, sizeof(rtl8295r_9300_s18431_myParam)) != 0)
        {
            PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S18431, rtl8295r_9300_s18431_myParam, phy_8295_pageRegVal_t);
        }
        if ((sizeof(rtl8295r_9300_s1dacLong_myParam) != sizeof(rtl8295r_9300_s1dacLong_default))
            || osal_memcmp(&rtl8295r_9300_s1dacLong_default, &rtl8295r_9300_s1dacLong_myParam, sizeof(rtl8295r_9300_s1dacLong_myParam)) != 0)
        {
           PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S1DACLONG, rtl8295r_9300_s1dacLong_myParam, phy_8295_pageRegVal_t);
        }

        phy_8295r_rxCaliConf_set(unit, &rtl8295r_9300_rxCaliConf_myParam);
    }

#endif
}

/* Function Name:
 *      rtl8295r_config_patchParam_init
 * Description:
 *      Inital patch database
 * Input:
 *      unit    - unit ID
 *      baseMacId    - base port ID of the PHY
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8295r_config_patchParam_init(uint32 unit, uint8 baseMacId)
{
#if defined(CONFIG_SDK_RTL8390)
    if (HWP_8390_50_FAMILY(unit))
    {
        /* for 8431 */
        PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S18431,    rtl8295r_8390_s18431_default,    phy_8295_pageRegVal_t);
        /* for DAC-Long */
        PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S1DACLONG, rtl8295r_8390_s1dacLong_default, phy_8295_pageRegVal_t);
        /* for RX-Calibration */
        phy_8295r_rxCaliConf_set(unit, &rtl8295r_8390_rxCaliConf_default);
    }
#endif
#if defined(CONFIG_SDK_RTL9300)
    if(HWP_9300_FAMILY_ID(unit))
    {
        /* for 8431 */
        PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S18431,    rtl8295r_9300_s18431_default,    phy_8295_pageRegVal_t);
        /* for DAC-Long */
        PHY_8295R_PATCH_PARAM_INIT(unit, baseMacId, PHY_8295_PATCH_TYPE_TXATTR_S1DACLONG, rtl8295r_9300_s1dacLong_default, phy_8295_pageRegVal_t);
        /* for RX-Calibration */
        phy_8295r_rxCaliConf_set(unit, &rtl8295r_9300_rxCaliConf_default);
    }
#endif

    /* for customer customization */
    rtl8295r_config_patchParam_cust(unit, baseMacId);

}
#endif

#if defined(CONFIG_SDK_RTL8295R)
/* Function Name:
 *      rtl8295R_config
 * Description:
 *      Configuration code for RTL8295R.
 * Input:
 *      unit    - unit ID
 *      baseMacId    - base port ID
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
rtl8295R_config(uint32 unit, uint8 baseMacId)
{

    rtl8295r_config_patchParam_init(unit, baseMacId);

    phy_8295r_patch_set(unit, baseMacId);

}
#endif /* CONFIG_SDK_RTL8295R */

#if defined(CONFIG_SDK_RTL8214QF)
void
rtl8214QF_config(uint32 unit, uint8 baseMacId)
{

    phy_8214qf_patch_set(unit, baseMacId);

}
#endif /* CONFIG_SDK_RTL8214QF */
