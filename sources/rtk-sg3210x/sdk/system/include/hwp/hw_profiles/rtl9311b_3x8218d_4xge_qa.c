/*
 * Copyright(c) Realtek Semiconductor Corporation, 2021
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9311BM board
 *
 * Feature : RTL9311BM board database
 *
 */


static hwp_swDescp_t rtl9311b_3x8218d_4xge_qa_swDescp = {

    .chip_id                    = HWP_CHIP_AUTOPROBE,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .swcore_pcie_bus_number     = HWP_NOT_USED,
    .nic_supported              = TRUE,
    .cascade_role               = HWP_CASCADE_NONE,

    .port.descp = {
        { .mac_id =  0, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 0,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  1, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 1,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  2, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 2,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  3, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 3,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  4, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 4,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  5, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 5,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  6, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 6,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  7, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(0),              .phy_idx = 0,        .smi = 0,        .phy_addr = 7,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  8, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 8,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  9, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 9,        .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 10, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 10,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 11, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 11,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 12, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 12,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 13, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 13,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 14, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 14,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 15, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(1),              .phy_idx = 1,        .smi = 1,        .phy_addr = 15,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 16, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 16,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 17, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 17,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 18, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 18,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 19, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 19,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 20, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 20,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 21, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 21,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 22, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 22,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 23, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds = HWP_1SDS(2),              .phy_idx = 2,        .smi = 2,        .phy_addr = 23,       .led_c = 0,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(3),              .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = 3,       .led_layout = DOUBLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 25, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(4),              .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = 3,       .led_layout = DOUBLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 26, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(5),              .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = 3,       .led_layout = DOUBLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 27, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(6),              .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = 3,       .led_layout = DOUBLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 28, .attr = HWP_CPU,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0,},
        { .mac_id = RTK_OOB_IDX_TO_PORT(0), .attr = HWP_OOB, .eth = HWP_GE, .medi = HWP_COPPER,  .sds = HWP_1SDS(RTK_OOB_IDX_TO_SDS(0)), .phy_idx = HWP_NONE, .smi = RTK_OOB_IDX_TO_SMI(0), .phy_addr =  0, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0, },
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        .led_signal_invert = FALSE,
        .led_definition_set[0].led[0] = 0x0B80,
        .led_definition_set[0].led[1] = 0x0A20,
        .led_definition_set[0].led[2] = 0x0,
        .led_definition_set[0].led[3] = HWP_LED_END,    /* None */
        .led_definition_set[3].led[0] = 0x0A01,
        .led_definition_set[3].led[1] = 0x0A20,
        .led_definition_set[3].led[2] = 0x0,
        .led_definition_set[3].led[3] = HWP_LED_END,    /* None */
    },/* led.descp */

    .serdes.descp = {
        { .sds_id = 0, .mode = RTK_MII_XSGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 1, .mode = RTK_MII_XSGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 2, .mode = RTK_MII_XSGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 3, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 4, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 5, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 6, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = RTK_OOB_IDX_TO_SDS(0),  .mode = RTK_MII_SGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_RTL8218D, .mac_id = 0,  .phy_max = 8, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [1] = { .chip = RTK_PHYTYPE_RTL8218D, .mac_id = 8,  .phy_max = 8, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [2] = { .chip = RTK_PHYTYPE_RTL8218D, .mac_id = 16, .phy_max = 8, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [3] = { .chip = RTK_PHYTYPE_RTL8213B, .mac_id = RTK_OOB_IDX_TO_PORT(0), .phy_max = 1, .interruptSocGpioDev = 1, .interruptSocGpioPin = 3, },
        [4] = { .chip = HWP_END },
    }   /* .phy.descp */
};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9311b_3x8218d_4xge_qa = {
    .identifier.name        = "RTL9311B_3x8218D_4XGE_QA",
    .identifier.id          = HWP_RTL9311B_3x8218D_4XGE_QA,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9311b_3x8218d_4xge_qa_swDescp,
    }
};
