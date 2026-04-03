#ifndef __RTK_BUMBLEBEE_REGFIELD_LIST_H__
#define __RTK_BUMBLEBEE_REGFIELD_LIST_H__

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#ifdef __BOOTLOADER__
#include <hal/chipdef/bumblebee/rtk_bumblebee_uboot_feature_def.h>
#else   /* __BOOTLOADER__ */
#include <hal/chipdef/bumblebee/rtk_bumblebee_feature_def.h>
#endif  /* __BOOTLOADER__ */

#if defined(CONFIG_SDK_CHIP_FEATURE_CHIP_INFORMATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CHIP_INFO_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MODEL_NAME_INFO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CHIP_INFO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SOC_OTP_REG_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CHIP_INFORMATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_RESET)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RST_GLB_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RST_GLB_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CHIP_RST_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WDOG_RST_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BONDING_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SOC_RST_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DDR_PWD15N_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PCIE_PHY_PIN_RST_EN_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RESET */
#if defined(CONFIG_SDK_CHIP_FEATURE_PLL___BIAS)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BG_POR_TOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BG_POR_PLL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t XTAL_TOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_GLB_COM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_GLB_COM_CPU_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_TOP_1588_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_TOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_GLB_SIG_CPU_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_GLB_SIG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_CPU_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_CPU_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_CPU_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_CPU_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_CPU_MISC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_SYS_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_SYS_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_SYS_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_SYS_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_SYS_MISC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_LXB_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_LXB_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_LXB_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_LXB_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_LXB_MISC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_PCIE_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_PCIE_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_PCIE_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_PCIE_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_PCIE_MISC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_1588_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_1588_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_1588_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_1588_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PLL_1588_MISC_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PLL___BIAS */
#if defined(CONFIG_SDK_CHIP_FEATURE_BIST___BISR)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_MBIST_CTRL_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_MBIST_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_MBIST_STA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_MBISD_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN_2P1C_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_MARGIN7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_INIT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_INIT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_INIT2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_INIT3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_INIT4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_INIT5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_TCAM_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_MEM_MARGIN1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_MEM_MARGIN_2P1C_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_MEM_MARGIN2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_MEM_INIT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_MEM_INIT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_CAM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_NIC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_NIC_LOOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_NIC_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_NIC_RESUME_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_NIC_GROUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OBM_MEM_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_MEM_INIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM_RESUME_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM_GLB_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM0_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM1_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM2_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM3_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM4_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM5_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM6_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM7_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM8_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM9_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM10_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM11_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM12_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM13_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM14_STATUS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_OBM15_STATU_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_START0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_START1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_START2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_START3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_START4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_START5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_START6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RESUME0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RESUME1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RESUME2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RESUME3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RESUME4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RESUME5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RESUME6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS8_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS9_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS50_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS52_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS53_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS54_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS55_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS56_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS57_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS58_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS59_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS60_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS61_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS62_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS63_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS64_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS65_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS66_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS67_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS68_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS69_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS70_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS71_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS72_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS73_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS74_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS75_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS76_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS77_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS78_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS79_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS80_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS81_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS82_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS83_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS84_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS85_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS86_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS87_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS88_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS89_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS90_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS91_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS92_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS93_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS94_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS170_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS95_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS96_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS97_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS98_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS99_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS100_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS101_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS102_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS103_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS104_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS105_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS106_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS107_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS108_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS109_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS110_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS111_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS112_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS113_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS114_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS115_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS116_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS117_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS118_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS119_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS120_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS121_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS122_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS123_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS124_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS125_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS126_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS127_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS128_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS129_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS130_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS131_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS132_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS133_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS134_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS135_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS136_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS137_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS138_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS139_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS140_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS141_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS142_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS143_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS144_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS145_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS146_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS147_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS148_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS149_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_START0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_RESUME0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS8_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS9_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_MEM_INIT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_MEM_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_MEM_MARGIN1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_CTRL_MEM_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_CTRL_MEM_INIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MIB_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MIB_RESUME_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MIB_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_PKT_GEN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_PKT_GEN_LOOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_PKT_GEN_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_PKT_GEN_RESUME_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_PKT_GEN_GROUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EGR_GLB_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_START1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_RESUME1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_START0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_START1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_RESUME0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_RESUME1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_GLB_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS8_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_PKT_DATA_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS50_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS52_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS53_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS54_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS55_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS56_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS57_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS58_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS59_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_STS60_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_BISR_TEST_RESULT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_FAILED_PAGE_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_TM_FAILED_PAGE_CMD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MEM_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00M_S01_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00M_S01_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00M_S23_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00M_S23_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01M_S45_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01M_S45_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01M_S67_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01M_S67_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02M_S89_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02M_S89_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02M_S1011_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02M_S1011_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03M_S1213_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03M_S1213_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03M_S1415_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03M_S1415_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10M_S16_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10M_S16_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10M_S16_43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10M_S16_44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10M_S16_45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10M_S16_48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10M_S16_49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11M_S20_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11M_S20_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11M_S20_43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11M_S20_44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11M_S20_45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11M_S20_48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11M_S20_49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20M_S24_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20M_S24_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20M_S24_43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20M_S24_44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20M_S24_45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20M_S24_48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20M_S24_49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21M_S28_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21M_S28_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21M_S28_43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21M_S28_44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21M_S28_45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21M_S28_48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21M_S28_49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22M_S32_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22M_S32_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22M_S32_43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22M_S32_44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22M_S32_45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22M_S32_48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22M_S32_49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23M_S36_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23M_S36_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23M_S36_43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23M_S36_44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23M_S36_45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23M_S36_48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23M_S36_49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS150_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS151_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS152_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS153_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS154_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS155_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS156_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS157_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS158_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS159_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS160_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS161_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS162_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS163_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS164_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS165_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS166_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS176_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS177_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS178_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS179_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS180_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS181_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS182_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS183_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS184_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS185_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS186_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS187_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS188_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS189_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS190_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS167_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS173_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS168_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS169_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS174_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS175_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS171_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_IPP_STS172_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_MEM_MARGIN3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_START2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_RESUME2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_START3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_RESUME3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS43_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS44_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS45_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS48_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS49_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS50_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS52_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS53_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS54_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS55_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS56_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS57_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS58_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS59_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS60_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS61_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS62_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS63_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS64_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS65_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS66_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS67_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS68_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS69_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS70_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS71_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS72_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS73_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS74_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS75_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS76_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS77_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS78_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS79_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS80_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS81_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS82_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS83_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS84_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS85_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS86_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS87_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS88_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_EPP_STS89_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO0_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO1_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO2_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO3_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO4_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO5_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO6_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO7_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO8_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO9_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO10_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO11_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO12_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO13_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO14_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO15_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO0_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO1_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO2_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO3_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO4_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO5_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO6_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO7_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO8_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO9_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO10_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO11_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO12_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO13_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO14_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MBIST_MAC_FIFO15_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO0_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO1_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO2_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO3_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO4_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO5_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO6_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO7_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO8_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO9_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO10_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO11_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO12_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO13_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO14_MARGIN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO15_MARGIN_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_BIST___BISR */
#if defined(CONFIG_SDK_CHIP_FEATURE_INTERFACE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t I2C_SLV_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t I2C_MST_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t I2C_MST_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t I2C_MST_MEMADDR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t I2C_MST_DATA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPI_SLV_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPI_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPI_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPI_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPI_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPI_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GPIO_DRV_STRENGTH_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GPIO_SLEW_RATE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IO_DRV_STRENGTH_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IO_SLEW_RATE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IO_SMIT_TRIG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SD_DRV_STRENGTH_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SD_SLEW_RATE_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INTERFACE */
#if defined(CONFIG_SDK_CHIP_FEATURE_LED)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_NUM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET3_LED3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET3_LED2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET3_LED1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET3_LED0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET2_LED3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET2_LED2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET2_LED1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET2_LED0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET1_LED3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET1_LED2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET1_LED1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET1_LED0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET0_LED3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET0_LED2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET0_LED1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_SET0_LED0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_COPR_SET_SEL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_FIB_SET_SEL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_COPR_MASK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_FIB_MASK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_COMBO_MASK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SW_LED_LOAD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_SW_EN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_PORT_SW_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_INDRT_ACCESS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_100G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_100G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_100G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_50G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_50G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_50G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_40G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_40G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_40G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_25G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_25G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_25G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_TP5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_TP5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_TP5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_TP2P5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_TP2P5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_TP2P5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_2P5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_2P5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_2P5G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_TP1G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_TP1G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_TP1G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_1G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_1G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_1G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_500M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_500M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_500M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_100M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_100M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_100M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV1_10M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV2_10M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_LOAD_LV3_10M_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_P_LOAD_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_P_LOAD_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_GLB_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_GLB_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_TRIG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DIR_CTRL_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_DATA_CTRL_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_INDRT_ACCESS_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LED */
#if defined(CONFIG_SDK_CHIP_FEATURE_INTERRUPT)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IIMR_PORT_LINK_STS_CHG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EIMR_PORT_LINK_STS_CHG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IIMR_REMOTE_INTR_STS_UPD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EIMR_REMOTE_INTR_STS_UPD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_SERDES_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_MISC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_CCM_INT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_EXT_GPIO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_THERMAL_METER_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_AUTO_RECOVERY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_AUTO_RECOVERY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_SDS_INBAND_PHYEVENT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_RLFD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_SERDES_RXIDLE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_DBGO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMR_MAC_AUTO_REC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_SRC_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGLB_SRC_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EGLB_SRC_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IISR_PORT_LINK_STS_CHG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EISR_PORT_LINK_STS_CHG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IISR_RMT_INTR_STS_UPD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EISR_RMT_INTR_STS_UPD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_SERDES_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_MISC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_CCM_INT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_EXT_GPIO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_GPIO_INTR_MODE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_THERMAL_METER_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_AUTO_RECOVERY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_AUTO_RECOVERY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_SDS_INBAND_PHYEVENT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_RLFD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_SERDES_RXIDLE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_DBGO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ISR_MAC_AUTO_REC_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INTERRUPT */
#if defined(CONFIG_SDK_CHIP_FEATURE_HW_MISC_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_ACCESS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_WDATA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_RDATA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_CHK_RSLT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VOL_CTRL_RESIS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PCIE_EPHY_PARAM_REG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PCIE_MISC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PCIE_MDX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PCIE_MDX_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PCIE_EPHY_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SECURE_BOOT_RST_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL8_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL9_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BIR_CTRL15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SECURE_BOOT_KEY_REG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_DEBUG_SELECT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_DEBUG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_DEBUG_PIN_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VOLT_PROB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VOLT_PROB_RESULT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL0_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL1_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL2_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL3_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL4_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT0_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT1_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT2_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT3_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT4_TM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL0_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL1_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL2_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL3_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL4_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT0_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT1_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT2_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT3_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT4_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL0_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL1_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL2_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL3_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL4_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT0_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT1_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT2_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT3_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT4_SDS_10G_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL0_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL1_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL2_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL3_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL4_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT0_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT1_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT2_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT3_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT4_SDS_25G_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL0_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL1_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL2_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL3_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL4_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT0_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT1_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT2_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT3_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT4_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_ACCESS_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_INI_MARG_RD_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SECURE_BOOT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_AUTOLOAD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_INI_MARG_RD_ERR_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_INI_MARG_RD_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL0_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL1_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL2_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL3_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTRL4_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT0_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT1_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT2_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT3_SDS_25G_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_RESULT4_SDS_25G_0_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_HW_MISC_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_SOFT_ERROR_RECOVERY)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_RXMAC_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_OSB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_IPP_PAR_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_IPP_PAR_ERR1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_IPP_PAR_ERR2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_IPP_PAR_ERR3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_IPP_ECC1BIT_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_IPP_ECC2BIT_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_IPP_PAR_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_IPP_PAR_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_IPP_PAR_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_IPP_PAR_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_IPP_PAR_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_IPP_PAR_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_IPP_PAR_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_IPP_PAR_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_IPP_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_IPP_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_IPP_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_IPP_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IVC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_VACL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IVCA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_PCPDEI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_DSCP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_PIEKSEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IVC_VLAN_RANGE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IVAPF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IVPPRE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IVP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IL3IF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IL3IFPF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_VLANMB_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SERV_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MSTI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_SRS_L2S0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_SRS_L2S1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_L2BCAM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SFT32K0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SFT32K1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SFT16K0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SFT16K1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_L3HOST0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_L3HOST1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_L3NET0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_L3NET1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_L3HOSTT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_RMEPRX_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MYMAC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_BCD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_L2_LEVEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_BCD_FID_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_RPF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_LAG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_PORTISO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SEQ_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SHPMSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_DFPMSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_DSTBCD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_SEQRCV_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_INDVRCV_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MCPMSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IPPEVP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_L3MTU_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EL3IF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_NHOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_PORTGRP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_ECMPMBR1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_ECMPMBR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_ECMPGRP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_DFPCP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_GIP2LAG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MPLSPW_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_TNLDECAP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_TNLDECAPACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IACLACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_IACLKEYSEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_PBMAP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_PHM_DBG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IACLMTR_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_SERVMTR_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_SERVMTRACT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_TM_PAR_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_TM_ECC1BIT_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_TM_ECC2BIT_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_TM_PAR_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_TM_ECC1BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_TM_ECC2BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_TM_PAR_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_TM_ECC1BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_TM_ECC2BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EREPL_DPM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EREPL_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_LSQ_SHA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_HSQ_SHA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_LSSN_SHA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_HSSN_SHA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_LSQ_BKT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_LSQ_BKT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_HSQ_BKT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_HSQ_BKT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_LSSN_BKT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_LSSN_BKT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_HSSN_BKT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_HSSN_BKT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MCABS_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MCABS_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MCABS_FREE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_MIRABS_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_CPUABS_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_CPUABS_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_CPUABS_FREE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_WRED_CONFIG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_TDM_CONFIG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_CNG_PB_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_PKT_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_PKT_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_PKT_FREE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_PKT_DESC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_PG_DESC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_COPY_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_UCQ_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_MCQ_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_MCQ_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_MCQ_FREE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_AVG_QSIZE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_PORT_WEIGHT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SN_WEIGHT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_PKT_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_PKT_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_PKT_FREE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_PKT_DESC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_PG_DESC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_COPY_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_UCQ_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_MCQ_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_MCQ_LINK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_MCQ_FREE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_AVG_QSIZE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_AVG_QSIZE2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_PORT_WEIGHT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SN_WEIGHT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_EPP_PAR_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_EPP_PAR_ERR1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_EPP_ECC1BIT_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_EPP_ECC2BIT_ERR1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_EPP_PAR_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_EPP_PAR_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_EPP_ECC1BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_EPP_ECC2BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_EPP_PAR_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_EPP_PAR_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_EPP_ECC1BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_EPP_ECC2BIT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_PCP_DEI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_UL_PCP_DEI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_DSCP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_VP_SH_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_BCD_FID_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_BCD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EVA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_UNTAG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_MPLS_PW_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_MPLS_LSP_TC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_MPLS_PW_TC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_GIP2LAG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_ETAG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EVCS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EVC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_TNL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_MBR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_NHOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_MSTI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EVP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EL3IF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_DBG_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_SERV_MAP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EACL_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EACL_KEY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_EPP_EACL_METR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_EACL_METR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_SERV_CNT_7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_PW_SEQ_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC1BIT_STS_EACL_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_EACL_METR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_SERV_CNT_7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_PW_SEQ_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECC2BIT_STS_EACL_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_IMR_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_IMR_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_ISR_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_ISR_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_FRC_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_FRC_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_ECC1BIT_STS_RX_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_ECC2BIT_STS_RX_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_ECC1BIT_STS_TX_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_NIC_ECC2BIT_STS_TX_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM0_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM1_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM2_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM3_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM4_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM5_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM6_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM7_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM8_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM9_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM10_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM11_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM12_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM13_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM14_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM15_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK0_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK1_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK2_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK3_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK4_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK5_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK6_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK7_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK8_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK9_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK10_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK11_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK12_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK13_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK14_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK15_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE0_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE1_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE2_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE3_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE4_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE5_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE6_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE7_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE8_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE9_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE10_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE11_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE12_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE13_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE14_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE15_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM0_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM1_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM2_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM3_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM4_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM5_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM6_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM7_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM8_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM9_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM10_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM11_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM12_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM13_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM14_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_OBM15_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK0_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK1_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK2_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK3_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK4_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK5_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK6_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK7_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK8_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK9_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK10_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK11_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK12_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK13_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK14_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_LINK15_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE0_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE1_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE2_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE3_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE4_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE5_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE6_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE7_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE8_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE9_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE10_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE11_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE12_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE13_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE14_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OB_FREE15_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_OSB_OBM_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_OSB_LINK_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_OSB_FREE_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_OSB_OBM_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_OSB_LINK_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_OSB_FREE_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_OSB_OBM_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_OSB_LINK_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_OSB_FREE_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_OSB_OBM_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_OSB_LINK_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_OSB_FREE_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_OSB_OBM_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_OSB_LINK_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_OSB_FREE_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_OSB_OBM_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_OSB_LINK_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_OSB_FREE_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MIB_ECC1BIT_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MIB_ECC2BIT_ERR0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_MIB_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_MIB_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MIB_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MIB_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STAT_PORT_CNT_0_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STAT_PORT_CNT_0_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STAT_PORT_CNT_1_ECC1BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STAT_PORT_CNT_1_ECC2BIT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PKT_GEN_IMR_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PKT_GEN_IMR_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PKT_GEN_ISR_ECC1BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PKT_GEN_ISR_ECC2BIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PKT_GEN_FRC_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PKT_GEN_FRC_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PKT_GEN_ECC1BIT_STS_L2NTFY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_PJT_GEN_ECC2BIT_STS_L2NTFY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_PKT_GEN_PAR_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_IMR_PKT_GEN_PAR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_PKT_GEN_PAR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_STAT_CNT2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_STS_STAT_CNT3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LSQ_BKT0_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LSQ_BKT1_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t HSQ_BKT0_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t HSQ_BKT1_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LSSN_BKT0_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LSSN_BKT1_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t HSSN_BKT0_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t HSSN_BKT1_CLR_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_EPP_ECC1BIT_ERR2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_EPP_ECC2BIT_ERR3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO0_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO1_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO2_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO3_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO4_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO5_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO6_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO7_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO8_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO9_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO10_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO11_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO12_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO13_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO14_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO15_ECC1BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO0_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO1_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO2_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO3_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO4_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO5_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO6_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO7_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO8_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO9_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO10_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO11_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO12_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO13_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO14_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_FRC_MAC_FIFO15_ECC2BIT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_0_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_1_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_2_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_3_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_4_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_5_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_6_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_7_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_8_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_9_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_10_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_11_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_12_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_13_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_14_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SER_ISR_MAC_FIFO_15_ECC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO0_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO1_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO2_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO3_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO4_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO5_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO6_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO7_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO8_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO9_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO10_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO11_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO12_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO13_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO14_EMPTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO15_EMPTY_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SOFT_ERROR_RECOVERY */
#if defined(CONFIG_SDK_CHIP_FEATURE_NIC___DMA)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_CPU_QID2RING_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_RING_CPU_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_RING_CPU_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_BASE_DESC_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_CUR_DESC_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_BASE_DESC_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_CUR_DESC_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_RX_RUNOUT_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_RX_DONE_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_EXT_RX_RUNOUT_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_EXT_RX_DONE_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_TX_DONE_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_EXT_TX_DONE_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_INTR_EXT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_HOL_RX_RING_SUPR_AMOUNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_HOL_RX_RING_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_BUS_ADDR_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_EXT_BUS_ADDR_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_PHYSICAL_ADDR_SHIFT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_EXT_PHYSICAL_ADDR_SHIFT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_CLOSE_TIMEOUT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_PREFETCH_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_INTR_MITIGATION_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_INTR_MITIGATION_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_LOCAL_RING_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_RING_SIZE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_CPU_PTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_HW_PTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_RING_SIZE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_CPU_PTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_HW_PTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TX_RING_CONGEST_SRC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_ERR_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_SOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_EOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_SOP_CNT_EXT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_EOP_CNT_EXT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_SOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_EOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_SOP_CNT_EXT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_EOP_CNT_EXT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_BYTE_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_BYTE_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_RX_BYTE_CNT_EXT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_TX_BYTE_CNT_EXT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_IND_PREF_BUF_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PDMA_IND_PREF_BUF_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INT_AXI_MAS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INT_AXI_MAS_TO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INT_AXI_MAS_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INT_AXI_MAS_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_AXI_MAS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_AXI_MAS_TO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_AXI_MAS_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXT_AXI_MAS_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WR_CH_ID_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RD_CH_ID_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WR_ARB_SLOT_A_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WR_ARB_SLOT_B_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WR_ARB_SLOT_C_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WR_ARB_SLOT_D_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WR_ARB_MODE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RD_ARB_SLOT_A_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RD_ARB_SLOT_B_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RD_ARB_SLOT_C_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RD_ARB_SLOT_D_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RD_ARB_MODE_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_NIC___DMA */
#if defined(CONFIG_SDK_CHIP_FEATURE_TABLE_ACCESS)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_DMA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_DMA_START_IDX_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_DMA_NUM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_DMA_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_8_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_8_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_9_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_9_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_CTRL_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TBL_ACCESS_DATA_10_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_TABLE_ACCESS */
#if defined(CONFIG_SDK_CHIP_FEATURE_MAC_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_INDRT_ACCESS_CTRL_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_INDRT_ACCESS_BC_PHYID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_INDRT_ACCESS_MMD_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FIB_UNIDIR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_DBG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_SEL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_SEL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_SEL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_SEL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_SEL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_SEL5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_TSET_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDXDBG_READ_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBGMUX_SEL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBGMUX_SEL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBGMUX_SEL_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_DT_7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_OUT_CUR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_DBG_OUT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_ABLTY_GET_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PORT_POLLING_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PORT_ADDR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PORT_POLLING_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_GLB_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_GLB_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PRVTE0_POLLING_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PRVTE1_POLLING_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_BYPASS_ABLTY_LOCK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_MDC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TX_IDLE_TMR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LINK_DELAY_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDIO_FREE_CNT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_GPHY_RLFD_POLLING_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_RLFD_POLLING_BIT_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_10GPHY_RLFD_POLLING_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_GPHY_POLLING_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_10GPHY_POLLING_SEL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_10GPHY_POLLING_SEL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET0_10GPHY_POLLING_SEL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET1_10GPHY_POLLING_SEL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET2_10GPHY_POLLING_SEL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET3_10GPHY_POLLING_SEL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET0_10GPHY_POLLING_SEL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET1_10GPHY_POLLING_SEL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET2_10GPHY_POLLING_SEL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET3_10GPHY_POLLING_SEL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET0_10GPHY_POLLING_SEL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET1_10GPHY_POLLING_SEL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET2_10GPHY_POLLING_SEL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_SET3_10GPHY_POLLING_SEL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK0_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK1_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK2_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK3_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK_CHK4_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK0_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK0_MMD_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK1_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK1_MMD_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK2_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK2_MMD_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK3_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK3_MMD_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK4_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK4_MMD_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK0_DATA_GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK1_DATA_GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK2_DATA_GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK3_DATA_GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK4_DATA_GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK0_DATA_10GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK1_DATA_10GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK2_DATA_10GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK3_DATA_10GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK4_DATA_10GPHY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK0_RESULT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK1_RESULT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK2_RESULT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK3_RESULT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK4_RESULT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK0_RESULT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK1_RESULT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK2_RESULT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK3_RESULT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMI_PHY_REG_CHK4_RESULT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_FORCE_MODE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RLFD_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RLFD_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_LINK_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_LINK_MEDIA_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_LINK_SPD_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_LINK_DUP_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_TX_PAUSE_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_RX_PAUSE_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_ABLTY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_MSTR_SLV_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FEFI_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FORCE_MAC_SPD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPD_FORCE_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_SPD_CODE_CONFIG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MDX_SPD_CODE_CONFIG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INBAND_SPD_CODE_CONFIG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INBAND_SPD_CODE_CONFIG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SDS_ABLTY_SPD_CODE_CONFIG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SDS_ABLTY_SPD_CODE_CONFIG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_FORCE_MODE_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_GLOBAL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t HALF_CHG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_GLOBAL_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_GLOBAL_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_FRONT_PORT_CPU_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CPU_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_PADDING_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SDS_REG_ACC_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SDS_REG_ACC_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_ADDR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SDS_BCST_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SDS_BCST_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SDS_BCST_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_CPU_TRUNCATE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_CPU_TAG_SWAP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_CPU_CRC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_PORT_RX_MAX_LEN_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_PORT_RX_MAX_LEN_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_PORT_TX_MAX_LEN_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_PORT_TX_MAX_LEN_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_L2_CPU_MAX_LEN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CPU_L2_HDR1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CPU_L2_HDR2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP0_1_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP2_3_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP4_5_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP6_7_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP8_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP9_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP10_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP11_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP12_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP13_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP14_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_GROUP15_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP00_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP01_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP02_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP1_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP2_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP3_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP40_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_TYP41_DBG_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MISC_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_MSC_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S01_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_S23_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_MSC_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S45_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_S67_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_MSC_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S89_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_S1011_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_MSC_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1213_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_S1415_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_MSC_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_S16_51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_MSC_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_S20_51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_52_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_53_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_S24_54_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_52_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_53_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_S28_54_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_52_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_53_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_S32_54_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_03_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_MSCC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_MSC_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_MSC_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_04_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_05_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_06_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_07_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_08_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_09_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_15_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_16_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_17_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_18_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_19_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_20_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_21_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_22_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_23_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_24_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_25_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_26_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_27_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_28_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_29_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_30_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_31_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_32_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_33_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_34_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_35_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_36_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_37_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_38_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_39_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_40_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_41_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_42_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_46_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_47_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_51_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_52_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_53_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_S36_54_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_PTP_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_MSC_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBGSEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBGSEL_PORT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBGSEL_SYS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBGSEL_250_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBGSEL_DT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBGSEL_START_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_DT_7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_OUT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_00_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_01_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DBG_02_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT00_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT01_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT02_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT03_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT10_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT11_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT20_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT21_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT22_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MACT23_DBG_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_TX_TDM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_RX_TDM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM7_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM8_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM9_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM10_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM11_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM12_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM13_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM14_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_FIFO_TDM15_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MAC_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_POWER_SAVING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_PORT_TX_RX_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_MISC_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_MISC_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_MISC_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_MINIFG_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_MINIFG_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_MINIFG_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_WAIT_RX_INACTIVE_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_MULTIWAKE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_TIMER_100M_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_TIMER_GELITE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_TIMER_GIGA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_TIMER_2P5G_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_TIMER_5G_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_TIMER_2P5G_5G_LITE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_TX_TIMER_10G_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EEE_PORT_TX_RX_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PS_SLOW_SYSCLK_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_POWER_SAVING */
#if defined(CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_TAG_ETPID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_TAG_ITPID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_TAG_OTPID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_TPID_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_AFT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_IGR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_EGR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_EGR_TPID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_PVID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_EGR_FLTR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_KEEP_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_802_1Q_VLAN___QINQ */
#if defined(CONFIG_SDK_CHIP_FEATURE__IEEE802_1V__PROTOCOL_BASED_VLAN)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PPB_VLAN_SET_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_PPB_VLAN_SET_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE__IEEE802_1V__PROTOCOL_BASED_VLAN */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_IVC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_PORT_EVC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_IVC_MIS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VLAN_EVC_MIS_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_TRANSLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_VLAN_CROSS_CONNECT)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VXC_GLB_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VLAN_CROSS_CONNECT */
#if defined(CONFIG_SDK_CHIP_FEATURE_RMA)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_MIRROR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_SMAC_LRN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_MGN_LRN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_PORT_BPDU_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_PORT_PTP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_PORT_LLDP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_PORT_EAPOL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_PORT_OAM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_USR_DEF_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RMA */
#if defined(CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LAG_HASH_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LAG_HASH_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LAG_HASH_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LAG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LAG_SHIFT_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LAG_SHIFT_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LOCAL_LAG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_LAG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_LAG_DUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_LAG_MBR_NUM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_LAG_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LINK_AGGREGATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ST_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ST_CTRL_EPP_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SPANNING_TREE */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_ISO_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_ISOLATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_KT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_AGE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_AGE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_TRK_AGE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_SALRN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_NEW_SA_SALRN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_NEW_SA_FWD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_DYN_MV_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_DYN_MV_LRN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_STT_MV_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_MV_FORBID_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_TRK_MV_FORBID_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_MV_FORBID_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_SABLK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_DABLK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_UC_LM_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_MC_LM_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_CONSTRT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_CONSTRT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_PORT_CONSTRT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_PORT_CONSTRT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_TRK_CONSTRT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_TRK_CONSTRT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_VLAN_CONSTRT_ENTRY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_VLAN_CONSTRT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_VLAN_CONSTRT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_CONSTRT_PORT_CNT_DBG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_CONSTRT_TRK_CNT_DBG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_CONSTRT_SYS_CNT_DBG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_CONSTRT_VLAN_CNT_DBG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_TBL_FLUSH_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_SRC_P_FLTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_SA_ACT_REF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_FRER_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_PORT_LRN_LEVEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_TRK_LRN_LEVEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_LRN_LEVEL_PF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_CAM_ENTRY_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_SRAM_FIFO_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_SFT_FIFO_CNT_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MAC_FORWARDING_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_L2_ENTRY_NOTIFICATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_PKT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_PKT_BP_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_PKT_BP_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_PKT_TIMEOUT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_PKT_MAGIC_NUM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_PKT_FIFO_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_INTR_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_EXT_INTR_MSK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_INTR_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_DBG_PKT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_DBG_EVENT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_DBG_BYTE_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L2_NTFY_BP_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_SRAM_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_L2_ENTRY_NOTIFICATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_L2_MISC_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_IGR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PORT_EGR_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_L2_MISC_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_STORM_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_IPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_IPORT_LB_RATE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_IPORT_LB_HIGH_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_IPORT_LB_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_IPORT_LB_STATE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_ADMIT_PROTO_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_STORM_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_INGRESS_BANDWIDTH_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_ADMIT_PROTO_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_IPORT_HIGH_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_IPORT_EXTRA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_IPORT_LOW_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_IPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_IPORT_LB_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IGBR_IPORT_LB_STATE_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INGRESS_BANDWIDTH_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_EGRESS_BANDWIDTH_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EGBW_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EGBW_PORT_FIX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EGBW_CPU_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EGBW_CPU_Q_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_EGRESS_BANDWIDTH_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_METER_MARKER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t METER_LB_EXCEED_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EMETER_LB_EXCEED_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t METER_ERROR_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t METER_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EMETER_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IMETER_CPEN_TKN_EXCEED_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_DBG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_IACL_MTR_DBG_DAT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_IACL_MTR_DBG_DAT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_IACL_MTR_DBG_DAT2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_IACL_MTR_DBG_DAT3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_IACL_MTR_DBG_DAT4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_IACL_MTR_DBG_DAT5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_SERV_MTR_DBG_DAT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_SERV_MTR_DBG_DAT1_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_METER_MARKER */
#if defined(CONFIG_SDK_CHIP_FEATURE_SERVICE_METER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SMETER_CPEN_TKN_EXCEED_STS_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SERVICE_METER */
#if defined(CONFIG_SDK_CHIP_FEATURE_OVERSUBSCRIPTION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PROTO_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PARSER_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_OTPID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_ITPID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_EVTPID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PCP_MAP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DSCP_MAP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PG2OSBPRI_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PORT_FC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PORT_PFC_CTRL_HI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PORT_PFC_CTRL_LO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_FC_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_FC_GRP_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_FIFO_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_TDM_RX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_TDM_RX_MG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_TDM_TX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_TDM_TX_MG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_MEM_GRP_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_MEM_PORT_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_MEM_FIFO_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_TDM_STOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_MEM_BYPASS_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_OB2IPP_SOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_OB2IPP_EOP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_PORT_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_OVERSUBSCRIPTION */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLOWCONTROL___BACKPRESSURE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PKTBUF_MAX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_TH_ADJUST_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_HOL_PRVNT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_MAC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_PG2ISG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_FORCE_DROP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_CPU_FORCE_DROP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_GEN_FORCE_DROP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ISG_SHARE_SIZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ISG_SIZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ISG_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MC_MAX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MIR_MAX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_CPU_MAX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MC_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MIR_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_CPU_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MC_SHARE_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MIR_SHARE_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_CPU_SHARE_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_MC_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_MIR_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_CPU_PG_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_MC_Q_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_MIR_Q_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_CPU_Q_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_CPU_ABS_Q_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_CASD_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_CASD_MAC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_CASD_CNTR_MAP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_GLB_IA_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_GLB_EA_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_SHR_HR_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_SHR_ISG_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ISG_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MC_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MIR_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_CPU_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MC_SHR_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_MIR_SHR_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_ABS_CPU_SHR_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_CNTR_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_MC_CNTR_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_MIR_CNTR_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FC_PORT_ABS_CPU_CNTR_RST_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLOWCONTROL___BACKPRESSURE */
#if defined(CONFIG_SDK_CHIP_FEATURE_CONGESTION_AVOIDANCE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ADM_OPT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_GBL_DBG_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_UCQ2ESG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_MCQ2ESG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_MIRQ2ESG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_CTRLQ2ESG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_CPU_Q2ESG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_GEN_PORT_Q2ESG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_SIZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_UCQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_MCQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_MIRQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_CTRLQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_CPUQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_UCQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_MCQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_MIRQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_CTRLQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_CPUQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_GEN_PORT_Q_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_LL_MAX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_LL_SIZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_LL_MCQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_LL_MIRQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_LL_CTRLQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_LL_CPUQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_LL_MCQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_LL_MIRQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_LL_CTRLQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_LL_CPUQ_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_CNTR_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_PORT_LL_CNTR_RST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_LL_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_ESG_LL_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_CPUQ_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_LL_CPUQ_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_GEN_PORT_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t E2E_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t E2E_TIMER_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SC_PORT_TIMER_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SC_DRAIN_OUT_THR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WRED_SAMPLE_TIME_PROFILE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CC_LL_FREE_CNTR_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CONGESTION_AVOIDANCE */
#if defined(CONFIG_SDK_CHIP_FEATURE_PFC)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_TIMER_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_MAC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_GEN_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_PCP2PG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_PRE_INTPRI2PG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_PG2PEV_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_PEV2TXQ_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_PG_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_BKP_TXQ_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_TX_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PFC_PORT_RX_CNTR_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PFC */
#if defined(CONFIG_SDK_CHIP_FEATURE_ECN_MARKING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t WRED_INT_ECN_RMK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ECN_TO_INT_ECN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INT_ECN_TO_ECN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DP_TO_INT_ECN_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ECN_MARKING */
#if defined(CONFIG_SDK_CHIP_FEATURE_INGRESS_PRIORITY_DECISION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QOS_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QOS_PORT_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_INGRESS_PRIORITY_DECISION */
#if defined(CONFIG_SDK_CHIP_FEATURE_REMARKING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMK_PORT_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_REMARKING */
#if defined(CONFIG_SDK_CHIP_FEATURE_SCHEDULER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_PORT_ALGO_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_PORT_SNODE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_PORT_UCQ_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_PORT_MCQ_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_PORT_MIRQ_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_PORT_CTRLQ_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCH_CPU_Q_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SCHEDULER */
#if defined(CONFIG_SDK_CHIP_FEATURE_QUEUE_MANAGEMENT)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_INTPRI2MCQID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_INTPRI2UCQID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_STK_INTPRI2UCQID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_STK_XFER_INTPRI2UCQID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_RSN2CPUQID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_FLAG2CPUQID_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_FLAG2CPUQID_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t QM_EPP_RSN2CPUQID_CTRL_0_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_QUEUE_MANAGEMENT */
#if defined(CONFIG_SDK_CHIP_FEATURE_TSN)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PREEMPT_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PREEMPT_PORT_QUEUE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PREEMPT_DISCARD_KEEP_CNTR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FRER_RTAG_PSR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FRER_RTAG_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FRER_AGE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FRER_INDV_RECOV_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FRER_INDV_RECOV_CTRL2_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_TSN */
#if defined(CONFIG_SDK_CHIP_FEATURE_RATE_MONITOR)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RM_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RM_R_ON_THD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RM_R_OFF_THD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RM_Y_ON_THD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RM_Y_OFF_THD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RM_PKT_THD_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RATE_MONITOR */
#if defined(CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L3_GLB_IPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L3_GLB_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L3_HDR_CHK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L3_HOST_LU_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L3_MTU_CHK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L3_NAPT_IP_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t L3_NAPT_IP_NETWORK_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_LAYER_3_ROUTING */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_AUX_OPER_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_PORT_IGR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_PORT_EGR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_TNL_DECAP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_TNL_PROTO_PF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_TERM_DIP_ONLY_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_DECAP_LU_MIS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_TNL_ENCAP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_ENCAP_IP_HDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_ENCAP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_ENCAP_GTP1_SEQ_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_ENCAP_GTP2_SEQ_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_ENCAP_GTP_EXT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TNL_ENCAP_QOS_PF_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_TUNNEL */
#if defined(CONFIG_SDK_CHIP_FEATURE_VXLAN)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VXLAN_GPE_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VXLAN */
#if defined(CONFIG_SDK_CHIP_FEATURE_L3_MPLS)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MPLS_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MPLS_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MPLS_TC_MAP_DP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MPLS_TC_MAP_INTPRI_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_L3_MPLS */
#if defined(CONFIG_SDK_CHIP_FEATURE_SRV6)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SR6_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SR6_PORT_IGR_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SRV6 */
#if defined(CONFIG_SDK_CHIP_FEATURE_STATISTIC_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_CNT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_CNT_FLEX_SET_0_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_CNT_FLEX_SET_1_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_CNT_FLEX_SET_2_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_CNT_FLEX_SET_3_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_CNT_E_Q_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_STATISTIC_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_STATISTIC_COUNTER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_SYS_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_GEN_PORT_E_Q_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STAT_CPU_PORT_E_Q_CNT_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_STATISTIC_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_SERVICE_COUNTER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_SRV_STAT_MODE_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SERVICE_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_DEBUG_COUNTER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_RXMAC_PORT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_OSB_PORT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_TM_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_TM_LINK_STS_PORT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_TM_SPEC_CGST_PORT_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_LB_PORT_PKTBUF_FULL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_LB_PORT_MC_ABSORB_FULL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_LB_PORT_CPU_ABSORB_FULL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_LB_PORT_MIR_ABSORB_FULL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_GEN_PORT_PKTBUF_FULL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_GEN_PORT_CPU_ABSORB_FULL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_GEN_PORT_MIR_ABSORB_FULL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MGMT_FRAME_ERR_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ETM_OPTION_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_DEBUG_COUNTER */
#if defined(CONFIG_SDK_CHIP_FEATURE_MIRRORING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MTP_CTRL_DUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_SPM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_DPM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_RX_SAMPLE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_TX_SAMPLE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIR_SAMPLE_BAD_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MIRRORING */
#if defined(CONFIG_SDK_CHIP_FEATURE_ERSPAN)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ENCAP_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ERSPAN_SEQ_NUM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ERSPAN_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ERSPAN_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ERSPAN_SGT_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ERSPAN */
#if defined(CONFIG_SDK_CHIP_FEATURE_SFLOW)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SAMPLE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SAMPLE_TRUNC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SFLOW_IGR_EN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SFLOW_EGR_EN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SFLOW_IGR_THR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SFLOW_EGR_THR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RAND_SCALE_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SFLOW */
#if defined(CONFIG_SDK_CHIP_FEATURE_IPFIX)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPFIX_THR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPFIX_IACL_PKT_WRAP_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPFIX_IACL_BYTE_WRAP_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPFIX_EACL_PKT_WRAP_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPFIX_EACL_BYTE_WRAP_STS_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IPFIX */
#if defined(CONFIG_SDK_CHIP_FEATURE_DISCARD_MONITOR)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_SYS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_IPP_QUEUE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_EPP_QUEUE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RX_ERR_QUEUE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_TRUNC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_0_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_1_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_2_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_RSN_CTRL_3_EPP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DM_SYS_CTRL_EPP_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_DISCARD_MONITOR */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLOW_IDENTIFICATION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_IPP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_EPP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_IPP_SEL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_EPP_SEL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_IPP_OFST_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_EPP_OFST_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_IPP_SEL_PAIR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_IPP_OFST_PAIR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_EPP_SEL_PAIR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FI_EPP_OFST_PAIR_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLOW_IDENTIFICATION */
#if defined(CONFIG_SDK_CHIP_FEATURE_PIE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_VBLK_LUT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_VBLK_GRP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_VBLK_LID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_IBLK_LUT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_IBLK_GRP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_IBLK_LID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_EBLK_LUT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_EBLK_GRP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_EBLK_LID_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_VBLK_AND1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_IBLK_AND1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_EBLK_AND1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_VBLK_AND2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_IBLK_AND2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_EBLK_AND2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_FIELD_SEL_OFST_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PIE_FIELD_SEL_FMT_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PIE */
#if defined(CONFIG_SDK_CHIP_FEATURE_ACL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IACL_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VACL_PORT_LOOKUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IACL_PORT_LOOKUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EACL_PORT_LOOKUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_VBLK_MAC_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_IBLK_MAC_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_EBLK_MAC_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IACL_CNT_MODE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EACL_CNT_MODE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_IBLK_DOMAIN_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_IBLK_SRCIF_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_EBLK_EGRIF_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_VBLK_ENTRY_HIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_IBLK_ENTRY_HIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACL_EBLK_ENTRY_HIT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EACL_LATENCY_THR_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EACL_LATENCY_THR_CTRL1_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ACL */
#if defined(CONFIG_SDK_CHIP_FEATURE_RANGE_CHECK__PKT_LEN_L4PORT_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RNG_CHK_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_RANGE_CHECK__PKT_LEN_L4PORT_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_ATTACK_PREVENTION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_PORT_EN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_ARP_INVLD_PORT_ACT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_IPV6_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_ICMP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_TCP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_SMURF_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ATK_PRVNT_CNT_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ATTACK_PREVENTION */
#if defined(CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SEC_IP_MAC_BIND_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SEC_PORT_IP_MAC_BIND_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_IP_MAC_BIDING */
#if defined(CONFIG_SDK_CHIP_FEATURE_OAM)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OAM_IPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OAM_EPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OAM_GLB_DYING_GASP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OAM_PORT_DYING_GASP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DYING_GASP_POLARITY_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OAM_EGR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OAM_PORT_RELEASE_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_OAM */
#if defined(CONFIG_SDK_CHIP_FEATURE_CFM)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CCM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CFM_OP_PF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPORT_MAC_ADDR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CCM_TX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CCM_INT_STS_LIST_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_CFM */
#if defined(CONFIG_SDK_CHIP_FEATURE_BFD)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BFD_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ACH_TYPE_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_BFD */
#if defined(CONFIG_SDK_CHIP_FEATURE_PTP__PRECISION_TIME_PROTOCOL_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_FREQ_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_FREQ_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_RW_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_RW_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_RW_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_RW_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_RW_CTRL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_RW_CTRL5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CLK_OUTPUT_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CLK_OUTPUT_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CLK_OUTPUT_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CLK_OUTPUT_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CLK_OUTPUT_CTRL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_OP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PPS_OUT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TX_TIME_FIFO_R_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TX_TIME_FIFO_R_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PTP_INTR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYS_TIME_CLK_SRC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PPS_IN_LATCH_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PPS_IN_LATCH_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PPS_IN_LATCH_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TOD_OUT_DATA_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TOD_OUT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TOD_SARP_GPS_WEEK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TOD_SARP_UTC_SEC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TOD_UART_SETTING_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PTP_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PTP_MAC_PORT_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PTP__PRECISION_TIME_PROTOCOL_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_SYNCHRONOUS_ETHERNET)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYNCE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYNC_LOCK_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYNC_LOCK_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYNC_OUTPUT_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SYNC_OUTPUT_CTRL1_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SYNCHRONOUS_ETHERNET */
#if defined(CONFIG_SDK_CHIP_FEATURE_FLEXIBLE_PIPELINE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY_MAC_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY0_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY1_GRP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY0_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY1_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY2_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY3_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY4_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY5_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY6_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY_MODE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY0_MSK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY1_MSK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY_LUT_MISS_ACT0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY_LUT_MISS_ACT1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY_TCP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY_DOMAIN_SEL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FLEX_KEY_SRCIF_SEL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_FLEXIBLE_PIPELINE */
#if defined(CONFIG_SDK_CHIP_FEATURE_UNIT_CONTROL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_GBL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_PORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_DEV_PORT_MAP_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_DEV_PORT_MAP_CTRL_DUP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_NON_UNICAST_BLOCK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_CASD_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STK_SHRINK_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STKOE_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STKOE_RX_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STKOE_DMAC_SEL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STKOE_DMAC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STKOE_SMAC_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STKOE_VLAN_SEL_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STKOE_VLAN_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_UNIT_CONTROL */
#if defined(CONFIG_SDK_CHIP_FEATURE_REMOTE_ACCESS)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_ACCESS_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_ACCESS_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_ACCESS_CFG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_ACCESS_SEM_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_ACCESS_SEM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_IDMA_TIMEOUT_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_REMOTE_ACCESS */
#if defined(CONFIG_SDK_CHIP_FEATURE_REMOTE_INTERRUPT)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_INTR_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_INTR_INFO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMT_INTR_DATA_LINK_STS_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_REMOTE_INTERRUPT */
#if defined(CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BPE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BPE_ING_IPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BPE_QOS_IPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BPE_EGR_IPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BPE_EGR_EPORT_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PORT_EXTENSION */
#if defined(CONFIG_SDK_CHIP_FEATURE_MGMT_FRAME)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MGMT_UNKN_TYPE_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MGMT_FRAME */
#if defined(CONFIG_SDK_CHIP_FEATURE_VP__VIRTUAL_PORT_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t VP_IPORT_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_VP__VIRTUAL_PORT_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_BCD__BROADCAST_DOMAIN_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_BCD_PF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t BCD_GLB_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_BCD__BROADCAST_DOMAIN_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_PARSER)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PARSER_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PARSER */
#if defined(CONFIG_SDK_CHIP_FEATURE_SHARED_FORWARDING_TABLE)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SFT_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SHARED_FORWARDING_TABLE */
#if defined(CONFIG_SDK_CHIP_FEATURE_PACKET_GENERATOR)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_SLOT_CTRL_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_SLOT_CTRL_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_RX_SOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_RX_EOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_TX_SOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_TX_EOP_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_RX_BYTE_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PGEN_TX_BYTE_CNT_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PACKET_GENERATOR */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_IPT)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_IPT_IN_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_IPT2IFD_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_IPT */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_IFD)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_IFD2FWL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_IFD */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_FWD)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_FWL2IACL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_FWD */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_IACL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_IACL2PSEL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_IACL */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_PSEL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_PSEL_IF_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_PSEL_PPH_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_PSEL */
#if defined(CONFIG_SDK_CHIP_FEATURE_PHM)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_PHM_ITMH_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_PHM_IPPH_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_PHM */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_EPT)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EPT_ETMH_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EPT_EPPH_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EPT2EHD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EPT2EACL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EPT2PMOD_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_EPT */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_EHD)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EHD2EACL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_EHD */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_EACL)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EACL2PMOD_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_EACL_TM_ER_IF_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_EACL */
#if defined(CONFIG_SDK_CHIP_FEATURE_MD_PMOD)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MD_PMOD2TX_MAC_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_MD_PMOD */
#if defined(CONFIG_SDK_CHIP_FEATURE_DEBUGGING__ALE__LOOPBACK__DROP_MECHANISM__FC_AND_QM_)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXTGPIO_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXTGPIO_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXTGPIO_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXTGPIO_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXTGPIO_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXTGPIO_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_IFD_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_FWL_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_IACL_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_PSEL_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_PHM_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_EPT_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_EHD_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_EACL_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_PMOD_MD_FREEZE_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_TM_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_TM_EPORT_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EXPR_EPP_BUFCNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PREEMPT_EPP_BUFCNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t DBG_IPT_FIFO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_TRI_CORE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_CTL_CORE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_MASK_CORE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_CMP_CORE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_VAL_CORE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DBG_DATA_CORE_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t REGIF_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t REGIF_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t REGIF_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t REGIF_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t REGIF_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t REGIF_DBG_DATA_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_TRI_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_CTL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_MASK_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_CMP_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_VAL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_DBG_DATA_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_DEBUGGING__ALE__LOOPBACK__DROP_MECHANISM__FC_AND_QM_ */
#if defined(CONFIG_SDK_CHIP_FEATURE_SMART_PACKET_GENERATOR)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_GLB_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_TX_GRP_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_TX_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_TX_GRP_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_TX_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PB_IDX_INFO_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_PKT_CNT_H_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_PKT_CNT_L_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM0_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM0_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM0_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM0_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM0_CTRL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM0_CTRL5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM0_CTRL6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM1_CTRL0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM1_CTRL1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM1_CTRL2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM1_CTRL3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM1_CTRL4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM1_CTRL5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_STREAM1_CTRL6_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SPG_PORT_IPG_CTRL_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_SMART_PACKET_GENERATOR */
#if defined(CONFIG_SDK_CHIP_FEATURE_AUTO_RECOVERY)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LD_TX_DSC_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LD_TX_DSC_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TX_DSC_CHK_TMR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_PKT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_PKT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PKT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_PKT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_PKT_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_PKT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OSB_PKT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_PKT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_PKT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_PKT_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RX_FIFO_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RX_FIFO_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SW_Q_RST_CNT_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TRIG_AUTO_RECOVER_CTRL_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t AUTO_RECOVER_EVENT_FLAG_STS_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t AUTO_RECOVER_EVENT_FLAG_ERR_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_AUTO_REC_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_AUTO_REC1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SW_PORT_RST_STS_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_AUTO_RECOVERY */
#if defined(CONFIG_SDK_CHIP_FEATURE_ECO)
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_CTRL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_CTRL_DUMY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_CTRL_DUMY_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_CTRL_DUMY_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_CTRL_DUMY_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_CTRL_DUMY_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DUMY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DUMY_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DUMY_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DUMY_4_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DUMY_5_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DUMY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EPP_DUMY_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t NIC_DUMY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OBM_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OBM_DUMY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OBM_DUMY_2_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OBM_DUMY_3_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPT_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_TNL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_ATK_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PHM_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IBW_CTL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t STORM_CTL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PSEL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t FWL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IFD_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_LAG_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_CFM_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MCST_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MEM_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_MBIST_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_TAB_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DBG_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_DCNT_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IPP_GLB_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t RMA_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t IACL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t EACL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MIB_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t LED_CTRL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t GLB_REG_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t INTR_REG_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ADM_IA_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ADM_EA_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_MEM_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_MBIST_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t TM_CTL_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ITM_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t ETM_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t SCHED_DMY_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t PKT_GEN_DUMY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t CGMII_PORT_DEBUG_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t MAC_DEBUG_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_CTRL_DUMY_0_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_CTRL_DUMY_1_RTL9330_FIELDS[];
#endif
#if defined(CONFIG_SDK_RTL9330)
extern rtk_regField_t OTP_CTRL_DUMY_2_RTL9330_FIELDS[];
#endif
#endif   /* CONFIG_SDK_CHIP_FEATURE_ECO */



#endif    /* __RTK_BUMBLEBEE_REGFIELD_LIST_H__ */
