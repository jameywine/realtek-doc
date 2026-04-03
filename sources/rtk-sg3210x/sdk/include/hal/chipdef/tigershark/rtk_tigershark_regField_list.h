#ifndef __RTK_TIGERSHARK_REGFIELD_LIST_H__
#define __RTK_TIGERSHARK_REGFIELD_LIST_H__

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#ifdef __BOOTLOADER__
#include <hal/chipdef/tigershark/rtk_tigershark_uboot_feature_def.h>
#else   /* __BOOTLOADER__ */
#include <hal/chipdef/tigershark/rtk_tigershark_feature_def.h>
#endif  /* __BOOTLOADER__ */

#if defined(CONFIG_SDK_CHIP_FEATURE_CHIP_INFORMATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CHIP_INFO_DMY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MODEL_NAME_INFO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CHIP_MODE_INFO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CHIP_INFO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SYSTEM_CLK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFG_DMY_CHIP_INFO_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFG_DMY_CHIP_INFO_1_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CHIP_INFORMATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_RESET)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RST_GLB_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RST_GLB_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CHIP_RST_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WDOG_RST_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RTT_CONTROL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CEN0X_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CEN1X_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CEN2X_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CEN3X_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CEN4X_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BONDING_STS_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RESET */
#if defined(CONFIG_SDK_CHIP_FEATURE_PLL___BIAS)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_PLL_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_PLL_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LXB_PLL_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LXB_PLL_SSC_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LXB_PLL_MISC_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LXB_PLL_MISC_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_PLL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_PLL_SSC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_PLL_MISC_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_PLL_MISC_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MISC_PLL_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MISC_PLL_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MISC_PLL_SSC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MISC_PLL_MISC_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MISC_PLL_MISC_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_CPU_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_CPU_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_CPU_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_CPU_CTRL3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_CPU_MISC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_DBG_OUT_MISC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_DBG_OUT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_DUMMY_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PLL___BIAS */
#if defined(CONFIG_SDK_CHIP_FEATURE_BIST___BISR)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG00_RXFIFO_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG00_RXINFO_1G_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG00_RXINFO_1G_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG00_TXFIFO_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG01_RXFIFO_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG01_RXINFO_1G_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG01_RXINFO_1G_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG01_TXFIFO_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG02_RXFIFO_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG02_RXINFO_1G_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG02_RXINFO_1G_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG02_TXFIFO_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG03_RXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG03_TXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG04_RXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG04_TXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG05_RXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG05_TXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG06_RXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG06_TXFIFO_TG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG07_RXFIFO_SPC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG07_RXINFO_SPC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG07_TXFIFO_SPC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PG07_L2MSG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_RESET_00_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_RESET_01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_RESUME_00_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_RESUME_01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_LOOP_MODE_00_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_LOOP_MODE_01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_DYN_READ_EN_00_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MAC_DYN_READ_EN_01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_TXFIFO_DVSE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_TXFIFO_DVS_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_TXFIFO_DVS_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_TXFIFO_DVS_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_TXFIFO_DVS_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXFIFO_DVSE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXFIFO_DVS_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXFIFO_DVS_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXFIFO_DVS_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXFIFO_DVS_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXINFO_DVSE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXINFO_DVS_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXINFO_DVS_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXINFO_DVS_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RXINFO_DVS_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_MODE_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_MODE_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_MODE_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_MODE_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_MODE_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_MODE_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_MODE_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_MODE_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_RESUME_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_RESUME_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_RESUME_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_RESUME_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DYN_READ_EN_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DYN_READ_EN_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DYN_READ_EN_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DYN_READ_EN_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_LOOP_MODE_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_LOOP_MODE_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_LOOP_MODE_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_PAUSE_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_PAUSE_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_PAUSE_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_PAUSE_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_DONE_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_DONE_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_DONE_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_DONE_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_DONE_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_DONE_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_DONE_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_DONE_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_FAIL_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_FAIL_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_FAIL_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_FAIL_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_FAIL_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_FAIL_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_FAIL_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DRF_BIST_FAIL_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_GRP_EN_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_GRP_EN_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_GRP_EN_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_GRP_EN_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_RESET_GROUP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_RESET_GROUP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_RESET_GROUP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_RESET_GROUP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG2_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG2_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG15_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG15_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG16_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG16_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG17_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG17_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG18_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG19_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG20_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG21_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG21_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG22_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG23_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG24_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG25_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG26_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG27_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG28_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG29_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG30_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG31_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG32_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG33_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG34_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG35_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG36_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG37_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG38_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_CFG_BCAM_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE31_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE23_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE24_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE25_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE26_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE27_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE28_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE29_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE30_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE22_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE21_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_ALE20_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PRETC_A_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PRETC_B_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PRETC_C_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ALE_INIT_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ALE_INIT_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_MBIST_CTRL_DMY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_MBIST_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_MBIST_STA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_MBISD_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_MBISD_CFG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MIB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MIB_START_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MIB_RESUME_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MIB_GLB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MIB00_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_MIB01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR_BIST_START_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR_BIST_START_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR_DRF_TEST_RESUME_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR_DRF_TEST_RESUME_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR_GLB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR02_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR04_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR06_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR07_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR08_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR18_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR19_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR20_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_EGR_BISR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_EGR_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_EGR_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_EGR_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_EGR_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR_LOOP_DYNREAD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR_DVSE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR_DVS0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR_DVS1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR_START_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR_RESUME_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR_GLB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR01_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_PAR02_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ENCAP_INIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_EGR_INIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_MIB_INIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_INIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR_BIST_START_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR_DRF_TEST_RESUME_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR_GLB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIGR_RESULT_01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIGR_RESULT_23_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR01_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR01_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR02_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR03_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR04_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR05_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR06_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR06_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR06_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_IGR06_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_IGR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_IGR_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_IGR_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_IGR_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_16_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_17_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_PKT_ENCAP_18_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_16_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_17_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_18_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_19_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_20_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_21_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_22_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_23_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_24_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_25_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_26_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_27_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_28_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_29_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_30_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_31_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_32_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_33_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_34_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_35_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_36_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MEM_ACL_37_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_BIST___BISR */
#if defined(CONFIG_SDK_CHIP_FEATURE_INTERFACE)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t INTERFACE_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_SLV_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST_IF_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST_IF_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST1_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST1_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST1_MEMADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST1_DATA_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST2_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST2_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST2_MEMADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t I2C_MST2_DATA_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPI_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPI_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPI_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPI_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPI_ADDR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_SLV_TIMEOUT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GPIO_DRV_STRENGTH_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GPIO_SLEW_RATE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GPIO_SMIT_TRIG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BOND_STRAP_DRV_STRENGTH_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BOND_STRAP_SLEW_RATE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BOND_STRAP_SMIT_TRIG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IO_DRV_STRENGTH_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IO_SLEW_RATE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IO_SMIT_TRIG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_DBG_MUTEX_W_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_DBG_MUTEX_R_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INTERFACE */
#if defined(CONFIG_SDK_CHIP_FEATURE_LED)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_GLB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_NUM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_3_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_3_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_2_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_2_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_1_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_1_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_0_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_SET_0_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_COPR_SET_SEL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_FIB_SET_SEL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_COPR_MASK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_FIB_MASK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_COMBO_MASK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_LED_LOAD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_SW_EN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_PORT_SW_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_INDRT_ACCESS_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_10G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_10G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_10G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_5G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_5G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_5G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_2P5G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_2P5G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_2P5G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_TP1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_TP1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_TP1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_1G_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_500M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_500M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_500M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_100M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_100M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_100M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV1_10M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV2_10M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_LOAD_LV3_10M_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_P_LOAD_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_GLB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_READY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_INDRT_ACCESS_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LED */
#if defined(CONFIG_SDK_CHIP_FEATURE_INTERRUPT)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_SW_INT_MODE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_PORT_LINK_STS_CHG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_SERDES_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_MISC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_EXT_GPIO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_THERMAL_METER_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_AUTO_RECOVERY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_SERDES_UPD_PHYSTS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_RLFD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_SERDES_RXIDLE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_DBGO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IMR_ECC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_GLB_SRC_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_PORT_LINK_STS_CHG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_SERDES_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_MISC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_EXT_GPIO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_GPIO_INTR_MODE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_THERMAL_METER_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_AUTO_RECOVERY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_SERDES_UPD_PHYSTS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_STAT_TRIGGER_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_RLFD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_SERDES_RXIDLE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_DBGO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_ECC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_MASK_FOR_PARITY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_FOR_PARITY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_MASK_FOR_PARITY_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_FOR_PARITY_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_MASK_FOR_PARITY_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_FOR_PARITY_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_MASK_FOR_ECC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_GLB_STATUS_FOR_ECC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ISR_IMR_GLB_INGR_SRAM_ECC_SRC_STS_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INTERRUPT */
#if defined(CONFIG_SDK_CHIP_FEATURE_HW_MISC_)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_ACCESS_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_IND_WD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_IND_RD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_IND_CMD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_CP_MISC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_INI_MARG_RD_ERR_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_INI_MARG_RD_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OTP_AUTOLOAD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PCIE_EPHY_PARAM_REG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PCIE_AUTO_LOAD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SECURE_BOOT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIR_CTRL15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SECURE_BOOT_KEY_REG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SOC_SW_REG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SOC_OTP_REG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SOC_SW_REG_RO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TEST_MDX_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TEST_MDX_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_SENR_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_SENR_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_SENR_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_SENR_CTRL_ALE_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_SENR_CTRL_ALE_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_16_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_IN_17_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_16_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SPD_DATA_OUT_17_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_RESULT_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_RESULT_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_RESULT_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_RESULT_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM0_RESULT_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_RESULT_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_RESULT_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_RESULT_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_RESULT_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM1_RESULT_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_RESULT_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_RESULT_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_RESULT_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_RESULT_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM2_RESULT_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_DEBUG_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_DEBUG_PIN_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BOND_DBG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STRAP_DBG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VOLT_PROB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VOLT_PROB_RESULT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VOLT_PROB_FIR_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_HW_MISC_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_NIC___DMA)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_CPU_QID2RING_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_RING_CPU_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_SW2NIC_TRUNCATE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_NIC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_NIC_LOOP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_NIC_START_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_NIC_RESUME_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_NIC_GROUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MBIST_NIC_DYN_RD_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_TEST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SER_NIC_IMR_ECC1BIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SER_NIC_IMR_ECC2BIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SER_NIC_ISR_ECC1BIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SER_NIC_ISR_ECC2BIT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_RING_CPU_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_BASE_DESC_ADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_CUR_DESC_ADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_BASE_DESC_ADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_CUR_DESC_ADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_RX_RUNOUT_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_RX_DONE_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_EXT_RX_RUNOUT_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_EXT_RX_DONE_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_TX_DONE_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_EXT_TX_DONE_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_INTR_EXT_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_HOL_RX_RING_SUPP_AMOUNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_HOL_RX_RING_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_PHYSICAL_ADDR_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_EXT_PHYSICAL_ADDR_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_CLOSE_TIMEOUT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_PREFETCH_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_INTR_MITIGATION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_INTR_MITIGATION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_LOCAL_RING_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_RING_SIZE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_CPU_PTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_HW_PTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_RING_SIZE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_CPU_PTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_HW_PTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_ERR_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_SOP_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_EOP_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_SOP_CNT_EXT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_RX_EOP_CNT_EXT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_SOP_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_EOP_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_SOP_CNT_EXT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_TX_EOP_CNT_EXT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t INT_AXI_MAS_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t INT_AXI_MAS_TO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t INT_AXI_MAS_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t INT_AXI_MAS_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_AXI_MAS_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_AXI_MAS_TO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_AXI_MAS_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EXT_AXI_MAS_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WR_CH_ID_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RD_CH_ID_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WR_ARB_SLOT_A_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WR_ARB_SLOT_B_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WR_ARB_SLOT_C_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WR_ARB_SLOT_D_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WR_ARB_MODE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RD_ARB_SLOT_A_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RD_ARB_SLOT_B_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RD_ARB_SLOT_C_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RD_ARB_SLOT_D_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RD_ARB_MODE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_L2_NTFY_INTR_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_L2_NTFY_INTR_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_EXT_L2_NTFY_INTR_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PDMA_EXT_L2_NTFY_INTR_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMA_IF_PKT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_DEBUG_NIC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_BITMASK_NIC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NIC_SRAM_ACCESS_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NIC_SRAM_ACCESS_CTRL1_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_NIC___DMA */
#if defined(CONFIG_SDK_CHIP_FEATURE_APPLICATION_TRAP)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRAP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRAP_ARP_GRAT_PORT_ACT_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_APPLICATION_TRAP */
#if defined(CONFIG_SDK_CHIP_FEATURE_TABLE_ACCESS)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_L2_METHOD_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_DATA_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_DATA_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_DATA_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_DATA_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_CTRL_5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_DATA_5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_HSA_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TBL_ACCESS_HSA_DATA_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_TABLE_ACCESS */
#if defined(CONFIG_SDK_CHIP_FEATURE_MAC_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_GLOBAL_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_GLOBAL_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_TGPORT_PRMB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_TGPORT_PRMB_DBG0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_TGPORT_PRMB_DBG1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_GLOBAL_CTRL3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_GLOBAL_CTRL4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TG_MAC_TX_IPG_DEBUG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_PORT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_PADDING_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG_DEBUG_RESET_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG0_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG1_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG2_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG3_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG4_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG5_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG6_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PG7_RX_CELL_COL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_ADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_CPU_CRC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_INDRT_ACCESS_BC_PHYID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_INDRT_ACCESS_MMD_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_DBG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDXDBG_SEL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDXDBG_SEL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDXDBG_SEL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDXDBG_SEL3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDXDBG_READ_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_GLB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_PORT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HALF_CHG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PORT_ADDR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PORT_POLLING_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_ABLTY_GET_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_GLB_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_GLB_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_GLB_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PRVTE0_POLLING_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PRVTE1_POLLING_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_BYPASS_ABLTY_LOCK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PORT_POLLING_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TX_IDLE_TMR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LINK_DELAY_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDIO_FREE_CNT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDIO_FREE_CNT_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_GPHY_RLFD_POLLING_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_RLFD_POLLING_BIT_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_10GPHY_RLFD_POLLING_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_GPHY_POLLING_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_10GPHY_POLLING_SEL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_10GPHY_POLLING_SEL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_10GPHY_POLLING_SEL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_10GPHY_POLLING_SEL3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_10GPHY_POLLING_SEL4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK0_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK1_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK2_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK3_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK4_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK0_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK0_MMD_ADDR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK1_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK1_MMD_ADDR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK2_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK2_MMD_ADDR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK3_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK3_MMD_ADDR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK4_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK4_MMD_ADDR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK0_DATA_GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK1_DATA_GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK2_DATA_GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK3_DATA_GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK4_DATA_GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK0_DATA_10GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK1_DATA_10GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK2_DATA_10GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK3_DATA_10GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK4_DATA_10GPHY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK0_RESULT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK1_RESULT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK2_RESULT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK3_RESULT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK4_RESULT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK0_RESULT_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK1_RESULT_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK2_RESULT_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK3_RESULT_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SMI_PHY_REG_CHK4_RESULT_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_FORCE_MODE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RLFD_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RLFD_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_LINK_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_LINK_MEDIA_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_LINK_SPD_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_LINK_DUP_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_TX_PAUSE_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_RX_PAUSE_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_EEE_ABLTY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_MSTR_SLV_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FEFI_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_MAC_CONTROL_MISC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PER_PORT_MAC_RSTB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PER_PORT_TX_RSTB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PER_PORT_RX_RSTB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_PORT_MAX_LEN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_L2_CPU_MAX_LEN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FIB_UNIDIR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_DBG_SEL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_ABLTY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RXDV_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TX_IDLE_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PHY_UNIDIR_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MACRX_DUPDET_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRC_RXDV_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRC_TX_IDLE_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRC_PHY_UNIDIR_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRC_MACRX_DUPDET_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRCV_RXDV_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRCV_TX_IDLE_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRCV_PHY_UNIDIR_L_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FRCV_MACRX_DUPDET_L_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MAC_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_PHY___SERDES)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_MODE_SEL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_MODE_SEL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SERDES_INDRT_ACCESS_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SERDES_INDRT_DATA_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_SUB_INDEX_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_OUI_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_VERSION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_OUI_TGR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_VERSION_TGR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_INTF_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_DBG_DEL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_INTF_OUT0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_INTF_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_INTF_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_DBG_DEL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SDS_INTF_OUT1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_ADR_JUMP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_CODE_MOVE_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_CODE_MOVE_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_CODE_MOVE_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_MON_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_MON_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_MON_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_MON_STATUS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_MON_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P00_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P00_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P01_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P02_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P02_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P03_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P03_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P04_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P04_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P05_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P05_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P06_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P06_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P07_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P07_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P08_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P08_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P09_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P09_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_HIT_P15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BREAK_TARG_P15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRCA_VAL_I0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRCA_VAL_I1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRCA_VAL_I2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRCB_VAL_I0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRCB_VAL_I1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRCB_VAL_I2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_MON_SRAM_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_MON_SRAM_STATUS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_ROM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_RAM_CTRL_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_RAM_WDT_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_RAM_RDT_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_COCI_SEL_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_CO_MON_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_CI_LSB_MON_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_CI_MSB_MON_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_ECO_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SDS_RG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BROM_MISR_DATAOUT_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BROM_S0_STATUS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_BROM_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRAM_S0_STATUS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NCTL_SRAM_S0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIST_DESKEW_BUF_STATUS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t BIST_DESKEW_BUF_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PHY___SERDES */
#if defined(CONFIG_SDK_CHIP_FEATURE_POWER_SAVING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_PORT_TX_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_PORT_RX_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_MISC_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_MISC_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_MINIFG_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_MINIFG_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_MINIFG_CTRL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_WAIT_RX_INACTIVE_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_MULTIWAKE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_TIMER_100M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_TIMER_GELITE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_TIMER_GIGA_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_TIMER_2P5G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_TIMER_5G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_TIMER_10G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_TX_TIMER_2P5G_5G_10G_LITE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_PORT_TX_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EEE_PORT_RX_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PS_GROUP_PORT_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PS_SOC_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_POWER_SAVING */
#if defined(CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_TAG_TPID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_TAG_TPID_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_ETAG_TPID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_OTAG_TPID_CMP_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_ITAG_TPID_CMP_MSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_ETAG_TPID_CMP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_AFT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_IGR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_FWD_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_APP_PKT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_IGR_FLTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_EGR_FLTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_TAG_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_PROFILE)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PROFILE_SET_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_PROFILE */
#if defined(CONFIG_SDK_CHIP_FEATURE__IEEE802_1V__PROTOCOL_BASED_VLAN)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PPB_VLAN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PPB_VLAN_SET_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_PPB_VLAN_SET_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_PPB_DBL_VLAN_SET_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE__IEEE802_1V__PROTOCOL_BASED_VLAN */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_RANGE_CHECK)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_IGR_VID_RNG_CHK_SET_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_IGR_VID_RNG_CHK_SET_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_IGR_VID_RNG_CHK_SET_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_IGR_VID_RNG_CHK_SET_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_EGR_VID_RNG_CHK_SET_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_EGR_VID_RNG_CHK_SET_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_EGR_VID_RNG_CHK_SET_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_EGR_VID_RNG_CHK_SET_3_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_RANGE_CHECK */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_IVC_BLK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_L2TBL_CNVT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_L2TBL_CNVT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_TRUNK_L2TBL_CNVT_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_TRUNK_L2TBL_CNVT_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_TRUNK_L2TBL_CNVT_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_TRUNK_L2TBL_CNVT_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_IVC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_PORT_EVC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_IVC_ENTRY_INDICATION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_EVC_ENTRY_INDICATION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_IVC_ENTRY_INDICATION_AGGR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VLAN_EVC_ENTRY_INDICATION_AGGR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_ENCAP_MISC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_ENCAP_EVC_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_IVC_MISC_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_RMA)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_MIRROR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_SMAC_LRN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_MGN_LRN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_PORT_BPDU_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_PORT_PTP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_PORT_LLDP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_PORT_EAPOL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_FLD_PMSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_BPDU_FLD_PMSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_EAPOL_FLD_PMSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_LLDP_FLD_PMSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_USR_DEF_FLD_PMSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMA_USR_DEF_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RMA */
#if defined(CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_ID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_MBR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_HASH_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_SHFT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_LOCAL_TBL_REFRESH_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_LOCAL_TBL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRK_STK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LOCAL_PORT_TRK_MAP_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ST_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PORT_ISO_RESTRICT_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PORT_ISO_VB_ISO_PMSK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PORT_ISO_VB_EGR_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_AGE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_AGE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_TRK_AGE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_SALRN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_NEW_SA_FWD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_DYN_MV_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_DYN_MV_LRN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_STT_MV_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_STT_MV_LRN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_TRK_STT_MV_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_TRK_STT_MV_LRN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_GLB_STT_PORT_MV_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_GLB_STT_PORT_MV_LRN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_GLB_NOT_LRN_UP_AGE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_MV_FORBID_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_TRK_MV_FORBID_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_MV_FORBID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_SABLK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_DABLK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_UNKN_UC_FLD_PMSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_BC_FLD_PMSK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_UC_LM_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_L2_MC_LM_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_IP4_MC_LM_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_PORT_IP6_MC_LM_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_CONSTRT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_CONSTRT_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_PORT_CONSTRT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_PORT_CONSTRT_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_TRK_CONSTRT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_TRK_CONSTRT_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_VLAN_CONSTRT_ENTRY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_LRN_VLAN_CONSTRT_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_VLAN_CONSTRT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_CONSTRT_PORT_CNT_DBG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_CONSTRT_TRK_CNT_DBG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_CONSTRT_SYS_CNT_DBG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_CONSTRT_VLAN_CNT_DBG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_TBL_FLUSH_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_SRC_P_FLTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_SA_ACT_REF_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_HASH_FULL_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_CAM_ENTRY_STS_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_L2_ENTRY_NOTIFICATION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_BP_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_BP_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_TIMEOUT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_ITAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_TYPE_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_CTRL_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_MAGIC_NUM_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_FIFO_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_NIC_TIMEOUT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_PKT_QUE_ID_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_NIC_MSG_CNT_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_DBG_PKT_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_DBG_EVENT_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_BP_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_DBG_SEL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_DBG_SEL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_NTFY_LAST_EVENT_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_L2_ENTRY_NOTIFICATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_STORM_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_LB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_LB_PPS_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_LB_PROTO_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_UC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_UC_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_UC_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_MC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_MC_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_MC_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_BC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_BC_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_BC_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_DHCP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_DHCP_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_DHCP_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_BPDU_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_BPDU_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_BPDU_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_IGMP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_IGMP_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_IGMP_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_ARP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_ARP_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STORM_PORT_PROTO_ARP_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_STORM_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_INGRESS_BANDWIDTH_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_LB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_FC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_Q_DROP_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_Q_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_Q_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_Q_LB_EXCEED_FLAG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PORT_SCHED_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_10M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_100M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_500M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_1G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_1250M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_2500M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_5G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_10G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_RATE_20G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_PAGE_CNT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_Q_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_ADMIT_Q_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_DROP_Q_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t IGBW_WFQ_LB_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INGRESS_BANDWIDTH_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_EGRESS_BANDWIDTH_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_ENCAP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_CPU_PPS_LB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_LB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_MAX_LB_RST_SET0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_MAX_LB_RST_SET1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_CPU_Q_MAX_LB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_CPU_Q_MAX_LB_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_ASSURED_LB_RST_SET0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_ASSURED_LB_RST_SET1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_10M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_100M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_500M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_1G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_1250M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_2500M_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_5G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_10G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_RATE_20G_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_MAX_LB_CTRL_SET0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_MAX_LB_CTRL_SET1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_ASSURED_LB_CTRL_SET0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGBW_PORT_Q_ASSURED_LB_CTRL_SET1_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_EGRESS_BANDWIDTH_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_METER_MARKER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_GLB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_LB_EXCEED_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_LB_GLB_EXCEED_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_CNTR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_CNTR_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_GREEN_CNTR_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_YELLOW_CNTR_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_RED_CNTR_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_TOTAL_CNTR_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_BYTE_TB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t METER_PKT_TB_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_METER_MARKER */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLOWCONTROL___BACKPRESSURE)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_ACT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_GLB_SYS_UTIL_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_GLB_DROP_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_GLB_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_GLB_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_GLB_FCOFF_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_GLB_FCOFF_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_JUMBO_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_JUMBO_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_JUMBO_FCOFF_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_JUMBO_FCOFF_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_JUMBO_THR_ADJUST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_JUMBO_THR_ADJUST_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_FCOFF_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_FCOFF_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_GUAR_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_THR_SET_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_ON_L2_NOTIFY_PORT_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_DUMMY_GUAR_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_EGR_DROP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_HOL_PRVNT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_Q_EGR_FORCE_DROP_CTRL_SET0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_Q_EGR_FORCE_DROP_CTRL_SET1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_CPU_Q_EGR_FORCE_DROP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_Q_EGR_DROP_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_CPU_Q_EGR_DROP_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_EGR_DROP_THR_SET_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_REPCT_FCOFF_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_GLB_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PB_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_CPU_Q_EGR_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_ALE_NON_REPCT_Q_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_ALE_REPCT_Q_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_REPCT_FCON_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_REPCT_FCOFF_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_REPCT_FCOFF_DROP_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_HSA_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_CTRL_GUAR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CASCADE_PORT_DROP_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CASCADE_PORT_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CASCADE_PORT_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CASCADE_PORT_FCOFF_HI_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CASCADE_PORT_FCOFF_LO_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_ON_REMOTE_PORT_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_ON_REMOTE_L2_NOTIFY_PORT_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_OFF_REMOTE_PORT_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_OFF_REMOTE_L2_NOTIFY_PORT_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_PORT_GUAR_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_PORT_THR_SET_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_L2_NTFY_PORT_GUAR_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_TX_PAUSE_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_CONGEST_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_SPECIAL_PAUSE_DMAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_SPECIAL_PAUSE_SMAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CASCADE_PORT_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_PORT_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_L2NTFY_PORT_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_PORT_PAGE_CNT_ERROR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_CASCADE_PORT_PAGE_CNT_ERROR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_L2NTFY_PORT_PAGE_CNT_ERROR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_PORT_PAGE_CNT_ERROR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_REMOTE_L2NTFY_PORT_PAGE_CNT_ERROR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETE_FC_PAUSE_FRAME_PORT_CONGEST_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_EGR_PAGE_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_Q_EGR_PAGE_CNT_SET0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t FC_PORT_Q_EGR_PAGE_CNT_SET1_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLOWCONTROL___BACKPRESSURE */
#if defined(CONFIG_SDK_CHIP_FEATURE_CONGESTION_AVOIDANCE)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SWRED_Q_DROP_RATE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SWRED_Q_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SWRED_DROP_CNTR_PIDX_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SWRED_DROP_CNTR_CIDX_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SWRED_DROP_CNTR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SWRED_DROP_CNTR_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SC_PORT_TIMER_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SC_PORT_TIMER_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SC_DRAIN_OUT_THR_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CONGESTION_AVOIDANCE */
#if defined(CONFIG_SDK_CHIP_FEATURE_INGRESS_PRIORITY_DECISION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_PORT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_REMAP_PORT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_REMAP_IPRI_CFI0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_REMAP_IPRI_CFI1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_REMAP_OPRI_DEI0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_REMAP_OPRI_DEI1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_REMAP_DSCP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_REMAP_1BR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_PORT_TBL_IDX_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PRI_SEL_TBL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DP_SEL_REMAP_ITAG_CFI0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DP_SEL_REMAP_ITAG_CFI1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DP_SEL_REMAP_OTAG_DEI0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DP_SEL_REMAP_OTAG_DEI1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DP_SEL_REMAP_DSCP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DP_SEL_PORT_TBL_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INGRESS_PRIORITY_DECISION */
#if defined(CONFIG_SDK_CHIP_FEATURE_SCHEDULING___QUEUE_MANAGEMENT)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_INTPRI2QID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_CPUQID2QID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_CPUQID2XGSQID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_FLAG2CPUQID_CTRL_0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_FLAG2CPUQID_CTRL_1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_FLAG2CPUQID_CTRL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SCHED_PORT_Q_CTRL_SET0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SCHED_PORT_Q_CTRL_SET1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SCHED_CPU_Q_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SCHED_PORT_ALGO_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SCHED_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t WFQ_LB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t QM_FLAG2CPUQID_CTRL_2_DUP_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SCHEDULING___QUEUE_MANAGEMENT */
#if defined(CONFIG_SDK_CHIP_FEATURE_REMARKING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_PORT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_INTPRI2IPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_IPRI2IPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_OPRI2IPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_DSCP2IPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_INTPRI2OPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_IPRI2OPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_OPRI2OPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_DSCP2OPRI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_INTPRI2DEI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_DP2DEI_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_INTPRI2DSCP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_IPRI2DSCP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_OPRI2DSCP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_DSCP2DSCP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RMK_DPINTPRI2DSCP_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_REMARKING */
#if defined(CONFIG_SDK_CHIP_FEATURE_802_1QAV)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AVB_PORT_CLASS_A_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AVB_PORT_CLASS_B_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AVB_PORT_CLASS_A_EN_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AVB_PORT_CLASS_B_EN_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AVB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AVB_CTRL_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AVB_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_802_1QAV */
#if defined(CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_IP_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_HOST_TBL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_IPUC_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_IP6UC_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_IPMC_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_IP6MC_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_PORT_IP_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_PORT_IP6_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_INTF_IP_MTU_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_INTF_IP6_MTU_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_ENTRY_MV_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_ENTRY_MV_PARAM_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_ECMP_HASH_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_MONT_CNTR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_MONT_CNTR_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_HW_LU_KEY_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_HW_LU_KEY_SIP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_HW_LU_KEY_DIP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L3_HW_LU_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_L3_MISC_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TUNNEL_IP_ADDR_CHK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TUNNEL_ROUTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TUNNEL_IP_IDENTIFICATION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TUNNEL_QOS_PROFILE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TUNNEL_VXLAN_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL */
#if defined(CONFIG_SDK_CHIP_FEATURE_MIB_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PORT_RST_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_CNT_SET1_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_CNT_SET0_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MIB_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_MIB_COUNTER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_BRIDGE_DOT1DTPLEARNEDENTRYDISCARDS_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MIB_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_DEBUG_COUNTER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR6_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR7_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR8_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR9_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR10_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR11_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR12_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR13_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR14_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR15_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR16_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR17_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR18_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR19_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR20_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR21_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR22_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR23_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR24_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR25_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR26_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR27_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR28_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR29_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR30_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR31_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR32_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR33_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR34_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR35_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR36_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR37_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR38_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR39_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR40_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR41_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR42_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR43_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR44_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR45_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR46_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR47_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR48_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR49_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR50_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR51_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR52_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR53_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR54_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR55_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR56_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR57_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR58_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR59_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR60_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR61_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR62_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR63_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR64_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR65_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR66_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR67_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR68_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR69_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR70_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR71_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR72_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR73_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR74_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR75_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR76_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR77_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR78_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR79_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_CNTR80_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STAT_PRVTE_DBG_ENCAP_CNTR_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_DEBUG_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_MIRRORING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_SPM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_DPM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_SAMPLE_RATE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_QID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MIRRORING */
#if defined(CONFIG_SDK_CHIP_FEATURE_RSPAN)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_RSPAN_VLAN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_RSPAN_TX_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_RSPAN_RX_TAG_RM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_RSPAN_VLAN_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_RSPAN_TX_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIR_RSPAN_RX_TAG_RM_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RSPAN */
#if defined(CONFIG_SDK_CHIP_FEATURE_SFLOW)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SFLOW_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SFLOW_PORT_RATE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SFLOW_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SFLOW */
#if defined(CONFIG_SDK_CHIP_FEATURE_PIE)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_BLK_LOOKUP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_BLK_PHASE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_BLK_GROUP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_BLK_TMPLTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_TMPLTE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_MV_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_MV_LEN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_CLR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_TAG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_RULE_HIT_INDICATION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_GLB_HIT_INDICATION_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_MISC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_FIELD_SELTOR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_ENCAP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PIE_MISC2_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PIE */
#if defined(CONFIG_SDK_CHIP_FEATURE_ACL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ACL_PORT_LOOKUP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ACL_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ACL */
#if defined(CONFIG_SDK_CHIP_FEATURE_RANGE_CHECK__PKT_LEN_L4PORT_)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RNG_CHK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RNG_CHK_IP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RNG_CHK_IP_RNG_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RANGE_CHECK__PKT_LEN_L4PORT_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_ATTACK_PREVENTION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_PORT_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_ACT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_IPV6_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_ICMP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_TCP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_SMURF_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ATK_PRVNT_ARP_INVLD_PORT_ACT_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ATTACK_PREVENTION */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SEC_IP_MAC_BIND_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SEC_PORT_IP_MAC_BIND_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING */
#if defined(CONFIG_SDK_CHIP_FEATURE_OAM)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OAM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OAM_PORT_ACT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OAM_GLB_DYING_GASP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OAM_GLB_DYING_GASP_TRIGGER_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OAM_PORT_DYING_GASP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t OAM_PORT_DYING_GASP_RELEASE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DYING_GASP_POLARITY_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_OAM */
#if defined(CONFIG_SDK_CHIP_FEATURE_CFM)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFM_RX_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFM_RX_CCM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFM_RX_LB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFM_RX_LT_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_RX_LIFETIME_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_RX_INST_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_RX_INST_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_TX_TAG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_TX_INST_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_TX_INST_MEM_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_TX_INST_TRK_PRESENT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CCM_TX_INST_PKT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFM_TX_CNGST_DROP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_PORT_EN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_RX_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_TX_DLY_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_RX_TIME_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_CLK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_TIME_FREQ_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_TIME_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ETH_DM_TIME_CTRL_SEC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_GEN_RANDOM_PAYLOAD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_GEN_TOGGLE_PAYLOAD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_GEN_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_GEN_DPM_SET_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CFM */
#if defined(CONFIG_SDK_CHIP_FEATURE_L2_TUNNEL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t L2_TUNNEL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t VXLAN_PASSENGER_QOS_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_L2_TUNNEL */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_ETAG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_ETAG_MAC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_ETAG_MAC_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_PORT_ETAG_MAC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_ETAG_RMK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_PORT_ETAG_IGR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_PORT_ETAG_EGR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PE_PORT_PCID_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION */
#if defined(CONFIG_SDK_CHIP_FEATURE_UNIT_CONTROL)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_GBL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_PORT_ID_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_DEV_PORT_MAP_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_NON_UNICAST_BLOCK_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_DBG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_CASCADE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_CASCADE_TRK_HASH_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_CASCADE_TRK_HASH_PORT_SEL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_CASCADE_INT_OUTPUT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_GBL_CTRL_ENCAP_DUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_PORT_ID_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_CASCADE_CTRL_DUP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_DBGMUX_SEL_2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t STK_DBGMUX_SEL_3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DCNT_RSVD_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_UNIT_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_PARSER)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARSER_CTRL_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PARSER */
#if defined(CONFIG_SDK_CHIP_FEATURE_ECC_PARITY)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_INT_MASK_MASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_INT_MASK_ISR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_PARITY_INT_INFO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_PARITY_INT_INFO_DUP0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_PARITY_INT_INFO_DUP1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_PARITY_INT_INFO_DUP2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t REG_PARITY_INT_INFO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_REG_DEBUG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_REG_BITMASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_DEBUG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_BITMASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_DEBUG_DUP0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_BITMASK_DUP0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_DEBUG_DUP1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_BITMASK_DUP1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_DEBUG_DUP2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_BITMASK_DUP2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_DEBUG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_BITMASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_ECC_INT_MASK_ISR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_DEBUG_DUP0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_BITMASK_DUP0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_DEBUG_DUP4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_BITMASK_DUP4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_PARITY_INT_INFO_DUP3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ACLLOG_PARITY_INT_MASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ACLACT_PARITY_INT_MASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ACLLOG_PARITY_INT_ISR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ACLACT_PARITY_INT_ISR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_DEBUG_DUP3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_BITMASK_DUP3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_PARITY_INT_INFO_DUP4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_DEBUG_DUP4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_BITMASK_DUP4_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_DEBUG_DUP3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_BITMASK_DUP3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ENCAP_SRAM_PARITY_INT_MASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ENCAP_SRAM_PARITY_INT_ISR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ENCAP_ECC_MULTIBIT_INT_MASK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ENCAP_ECC_MULTIBIT_INT_ISR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_DEBUG_EN_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_BITMASK_MAC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_DEBUG_EN_PAR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ECC_SRAM_BITMASK_PAR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_DEBUG_DUP5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PARITY_SRAM_BITMASK_DUP5_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SRAM_PARITY_INT_INFO_DUP5_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ECC_PARITY */
#if defined(CONFIG_SDK_CHIP_FEATURE_PARSER_HSB)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSB_DATA_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PARSER_HSB */
#if defined(CONFIG_SDK_CHIP_FEATURE_HSM)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSM_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSM_FWD_DATA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSM_POST_DATA_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_HSM */
#if defined(CONFIG_SDK_CHIP_FEATURE_MODIFIER_HSA)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSA_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSA_DBG_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t HSA_DATA_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MODIFIER_HSA */
#if defined(CONFIG_SDK_CHIP_FEATURE_DEBUGGING__ALE__LOOPBACK__DROP_MECHANISM__FC_AND_QM_)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PER_PORT_MAC_DEBUG0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PER_PORT_MAC_DEBUG1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PER_PORT_MAC_DEBUG2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PAR_DBG_TRI_CTL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_TRI_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CTL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_MASK0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CMP0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_VAL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_DATA0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CTL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_MASK1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CMP1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_VAL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_DATA1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CTL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_MASK2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CMP2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_VAL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_DATA2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CTL3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_MASK3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_CMP3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_VAL3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_DATA3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_ADR3_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_ADR2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_ADR1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_ADR0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_SEL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_SEL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_SEL2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DBG_SEL3_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_DEBUGGING__ALE__LOOPBACK__DROP_MECHANISM__FC_AND_QM_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_AUTO_RECOVERY)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RX_PORT_DSC_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RX_PORT_DSC_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_Q_RST_SYS_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_Q_RST_P_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_Q_RST_REMOTE_P_THR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TX_DSC_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TX_DSC_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TX_DSC_CHK_TMR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RX_FIFO_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RX_FIFO_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LINK_LIST_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LINK_LIST_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RET_EMPTY_PKT_BUF_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t RET_EMPTY_PKT_BUF_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SW_Q_RST_CNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TRIG_AUTO_RECOVER_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AUTO_RECOVER_EVENT_FLAG_STS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t AUTO_RECOVER_EVENT_FLAG_ERR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t SYS_DSC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_DSC_CHK_TMR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LINK_LIST_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PB_ACCESS_CTRL0_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PB_ACCESS_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PB_ACCESS_CTRL2_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_AUTO_RECOVERY */
#if defined(CONFIG_SDK_CHIP_FEATURE_ECO)
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t TM_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PWRON_REG_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_PARSER_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t CFM_GEN_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t LED_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t NIC_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PLL_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MDX_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t GLB_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t EGR_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ACL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_ENCAP_MISC_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_ENCAP_RSVD1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PKT_ENCAP_RSVD2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t INGR_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MAC_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t PER_PORT_MAC_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t MIB_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t REG_IF_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_PISO_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_LUEN_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_TM_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_MIRROR_CTL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_BIST_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_SFLOW_CTL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_TABLE_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_MIB_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_RMA_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_VLAN_ISO_CTL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_PRE_MISC_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_INGPRI_CTL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_VLAN_CFG_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_VLAN_PROFILE_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_IVC_MISC_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_PSTORM_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_CFM_CTL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_Q_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_HSB_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_HSM_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_HSA_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_UNSTORM_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_TRUNK_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_VLAN_PROTOCOL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_L2_CTRL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_L2_CTRL_RSVD1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_L2_CTRL_RSVD2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_OF_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_DCNT_RSVD1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_IBW_CTL_RSVD_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_L3_MISC_CTRL1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_L3_CTRL_RSVD1_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t ALE_L3_CTRL_RSVD2_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_BIST_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_TM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ENCAP_QOS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ENCAP_BR_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_PWRON_REG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_PKT_PARSER_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_CFM_GEN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_PLL_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY__MDX_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_GLB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_SOC_REG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_EGR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_REG_IF_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_EFUSE_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ACL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_PKT_ENCAP_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_INGR_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_MAC_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_MIB_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_GLB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_GLB_PORT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_PISO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_LUEN_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_TM_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_SFLOW_CTL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_TABLE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_MIB_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_RMA_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_VLAN_ISO_CTL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_PRE_MISC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_INGPRI_CTL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_VLAN_CFG_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_VLAN_PROFILE_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_IVC_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_PSTORM_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_CFM_CTL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_Q_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_MIRROR_CTL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_UNSTORM_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_TRUNK_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_VLAN_PROTOCOL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_L2_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_OF_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_DCNT_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_IBW_CTL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_ALE_L3_CTRL_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_SDS_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_IO_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_AXI_MASTER_RTL9311B_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9311B)
extern rtk_regField_t DMY_DBG_GLB_RTL9311B_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ECO */



#endif    /* __RTK_TIGERSHARK_REGFIELD_LIST_H__ */
