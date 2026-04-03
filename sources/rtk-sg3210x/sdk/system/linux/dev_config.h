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
 * Purpose : BSP APIs definition.
 *
 * Feature : device configure API
 *
 */

/*
 * Include Files
 */
#include <common/rt_type.h>

/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
typedef struct rtk_dev_s
{
    int32 dev_id;
    char *pName;
    void *fIsr;
    void *pIsr_param;
    int32 irq;
} rtk_dev_t;

enum
{
    RTK_DEV_TC0 = 0,
    RTK_DEV_TC1,
    RTK_DEV_TC2,
    RTK_DEV_TC3,
    RTK_DEV_TC4,
    RTK_DEV_TC5,
    RTK_DEV_TC6,
    RTK_DEV_WDT_PH1TO,
    RTK_DEV_WDT_PH2TO,
    RTK_DEV_RXI_300,
    RTK_DEV_DRAM_BUS_INTR,
    RTK_DEV_DFI_INTR,
    RTK_DEV_DFI_ECC_INTR,
    RTK_DEV_SPIF_INTR_R,
    RTK_DEV_UART0,
    RTK_DEV_UART1,
    RTK_DEV_UART2,
    RTK_DEV_GPIO_ABCD,
    RTK_DEV_LINK_CHG,                           /* RTK_DEV_TC4_DELAY_INT */
    RTK_DEV_TC5_DELAY_INT,
    RTK_DEV_TC6_DELAY_INT,
    RTK_DEV_LXSTO,
    RTK_DEV_LXMTO,
    RTK_DEV_BTG,
    RTK_DEV_PARALLEL_NAND,
    RTK_DEV_SPI_NAND,
    RTK_DEV_ECC,
    RTK_DEV_USB_H2,
    RTK_DEV_USB_H3,
    RTK_DEV_PCIE_RC_MSI,
    RTK_DEV_PCIE_RC_INT_LINK_REQ_RST,
    RTK_DEV_PCIE_RC_INT_A,
    RTK_DEV_PCIE_RC_INT_B,
    RTK_DEV_PCIE_RC_INT_C,
    RTK_DEV_PCIE_RC_INT_D,
    RTK_DEV_PCIE_EP_INT,
    RTK_DEV_PCIE_EP_INT_LINK_REQ_RST,
    RTK_DEV_INT_GMAC_INTR,
    RTK_DEV_SRAM_ECC_INTR,
    RTK_DEV_TM_INTR,
    RTK_DEV_EMMC,
    RTK_DEV_NIC_RX_DONE_0,
    RTK_DEV_NIC_RX_DONE_1,
    RTK_DEV_NIC_RX_DONE_2,
    RTK_DEV_NIC_RX_DONE_3,
    RTK_DEV_NIC_RX_DONE_4,
    RTK_DEV_NIC_RX_DONE_5,
    RTK_DEV_NIC_RX_DONE_6,
    RTK_DEV_NIC_RX_DONE_7,
    RTK_DEV_NIC_RX_DONE_8,
    RTK_DEV_NIC_RX_DONE_9,
    RTK_DEV_NIC_RX_DONE_10,
    RTK_DEV_NIC_RX_DONE_11,
    RTK_DEV_NIC_RX_RUNOUT_0,
    RTK_DEV_NIC_RX_RUNOUT_1,
    RTK_DEV_NIC_RX_RUNOUT_2,
    RTK_DEV_NIC_RX_RUNOUT_3,
    RTK_DEV_NIC_RX_RUNOUT_4,
    RTK_DEV_NIC_RX_RUNOUT_5,
    RTK_DEV_NIC_RX_RUNOUT_6,
    RTK_DEV_NIC_RX_RUNOUT_7,
    RTK_DEV_NIC_RX_RUNOUT_8,
    RTK_DEV_NIC_RX_RUNOUT_9,
    RTK_DEV_NIC_RX_RUNOUT_10,
    RTK_DEV_NIC_RX_RUNOUT_11,
    RTK_DEV_NIC_TX_0,
    RTK_DEV_NIC_TX_1,
    RTK_DEV_NIC_TX_2,
    RTK_DEV_NIC_TX_3,
    RTK_DEV_OAM_DYGASP,                         /* RTK_DEV_IGLB_OAM_DYGASP */
    RTK_DEV_IGLB_RLFD,
    RTK_DEV_SWCORE,                             /* RTK_DEV_IGLB_MISC */
    RTK_DEV_L2_NTFY_LOCAL_NTFY_BUF_RUN_OUT,
    RTK_DEV_L2_NTFY_RX_RUN_OUT,
    RTK_DEV_L2_NTFY_RX_DONE,
    RTK_DEV_AXI_MASTER,
    RTK_DEV_GPIO_A,
    RTK_DEV_GPIO_B,
    RTK_DEV_GPIO_C,
    RTK_DEV_GPIO_D,
    RTK_DEV_CPU_ERRIRQ_0,
    RTK_DEV_CPU_ERRIRQ_1,
    RTK_DEV_CPU_ERRIRQ_2,
    RTK_DEV_CPU_ERRIRQ_3,
    RTK_DEV_CPU_ERRIRQ_4,
    RTK_DEV_CPU_FAULTIRQ_0,
    RTK_DEV_CPU_FAULTIRQ_1,
    RTK_DEV_CPU_FAULTIRQ_2,
    RTK_DEV_CPU_FAULTIRQ_3,
    RTK_DEV_CPU_FAULTIRQ_4,
    RTK_DEV_CPU_CLUSTERPMUIRQ,
    RTK_DEV_CPU_CNTHVIRQ_0,
    RTK_DEV_CPU_CNTHVIRQ_1,
    RTK_DEV_CPU_CNTHVIRQ_2,
    RTK_DEV_CPU_CNTHVIRQ_3,
    RTK_DEV_OCPTO,
    RTK_DEV_HLXTO,
    RTK_DEV_SLXTO,
    RTK_DEV_NIC,
    RTK_DEV_REG_SWCORE,
    RTK_DEV_REG_SOC,
    RTK_DEV_MAX
};

#define RTK_DEV_PCIE_BASE   RTK_DEV_MAX
#define RTK_DEV_IDX_FOR_MSI(_ep, _msi_id)    ( RTK_DEV_PCIE_BASE + (_ep * (RTK_MAX_NUM_OF_PCIE_EP + 1)) + _msi_id )
#define RTK_DEV_IRQ_FOR_MSI(_ep, _msi_id)    ( rtk_dev[RTK_DEV_IDX_FOR_MSI(_ep, _msi_id)].irq )
extern rtk_dev_t rtk_dev[RTK_DEV_MAX + (RTK_MAX_NUM_OF_PCIE_EP * RTK_MAX_NUM_OF_PCI_MSI)];

/*
 * Function Declaration
 */
#if !defined(CONFIG_SDK_EXTERNAL_CPU)
extern int32 rtk_bspDev_irq_get(uint32 device_id, uint32 *irq_num, int32 *is_updated);
#endif

extern void dev_config_init(uint32 dev_id, uint32 irq_num, char *pName);

