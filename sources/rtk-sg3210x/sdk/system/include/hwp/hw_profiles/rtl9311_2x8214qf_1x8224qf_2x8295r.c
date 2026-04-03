/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9300 FPGA board
 *
 * Feature : RTL9300 FPGA board database
 *
 */



/*
 * 12*10G mode
 */
static hwp_swDescp_t rtl9311_2x8214qf_1x8224qf_2x8295r_swDescp = {

    .chip_id                    = HWP_CHIP_AUTOPROBE,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .swcore_pcie_bus_number     = HWP_NOT_USED,
    .nic_supported              = TRUE,
    .cascade_role               = HWP_CASCADE_NONE,


    .port.descp = {
        { .mac_id =  0, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 0,        .smi = 0,        .phy_addr = 0,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  1, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 0,        .smi = 0,        .phy_addr = 1,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  2, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 0,        .smi = 0,        .phy_addr = 2,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  3, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 0,        .smi = 0,        .phy_addr = 3,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  4, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 1,        .smi = 0,        .phy_addr = 4,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  5, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 1,        .smi = 0,        .phy_addr = 5,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  6, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 1,        .smi = 0,        .phy_addr = 6,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  7, .attr = HWP_ETH,   .eth = HWP_GE,     .medi = HWP_FIBER,  .sds = HWP_1SDS(2),   .phy_idx = 1,        .smi = 0,        .phy_addr = 7,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  8, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_FIBER,  .sds = HWP_1SDS(3),   .phy_idx = 2,        .smi = 1,        .phy_addr = 8,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id =  9, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_FIBER,  .sds = HWP_1SDS(3),   .phy_idx = 2,        .smi = 1,        .phy_addr = 9,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 12, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_FIBER,  .sds = HWP_1SDS(3),   .phy_idx = 2,        .smi = 1,        .phy_addr = 10,         .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 13, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_FIBER,  .sds = HWP_1SDS(3),   .phy_idx = 2,        .smi = 1,        .phy_addr = 11,         .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

        { .mac_id = 16, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(4),   .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(5),   .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(6),   .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(7),   .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(8),   .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(9),   .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 52, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(10),  .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 53, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_SERDES, .sds = HWP_1SDS(11),  .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 54, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_FIBER,  .sds = HWP_1SDS(12),  .phy_idx = 3,        .smi = 3,        .phy_addr = 0,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 55, .attr = HWP_ETH,   .eth = HWP_XGE,    .medi = HWP_FIBER,  .sds = HWP_1SDS(13),  .phy_idx = 4,        .smi = 3,        .phy_addr = 1,          .led_c = HWP_NONE, .led_f = 1,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 56, .attr = HWP_CPU,   .eth = HWP_NONE,   .medi = HWP_NONE,   .sds = SDS_NONE(),    .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,   .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE, .phy_mdi_pin_swap = 0,  },
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        .led_signal_invert = FALSE,
		  /* 2.5G Copper Port LED */
        .led_definition_set[0].led[0] = 0x0A08,           /* 2.5G Link Active */
        .led_definition_set[0].led[1] = 0x0AA0,           /* 1G/100M Link Active */
        .led_definition_set[0].led[2] = HWP_LED_END,      /* None */
		  /* 10G Fiber Port LED */
        .led_definition_set[1].led[0] = 0x0A01,           /* 10G Link Active */
        .led_definition_set[1].led[1] = 0x0A20,           /* 1G Link Active */
        .led_definition_set[1].led[2] = HWP_LED_END,      /* None */
    },/* led.descp */

    .serdes.descp = {
        { .sds_id = 2,  .mode = RTK_MII_XSGMII,             .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 3,  .mode = RTK_MII_USXGMII_10GQXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 4,  .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 5,  .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 6,  .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 7,  .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 8,  .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 9,  .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 10, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 11, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 12, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 13, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_RTL8214QF, .mac_id = 0,  .phy_max = 4, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [1] = { .chip = RTK_PHYTYPE_RTL8214QF, .mac_id = 4,  .phy_max = 4, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [2] = { .chip = RTK_PHYTYPE_RTL8224QF, .mac_id = 8,  .phy_max = 4, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [3] = { .chip = RTK_PHYTYPE_RTL8295R,  .mac_id = 54, .phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE  },
        [4] = { .chip = RTK_PHYTYPE_RTL8295R,  .mac_id = 55, .phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE  },
        [5] = { .chip = HWP_END },

    }   /* .phy.descp */
};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9311_2x8214qf_1x8224qf_2x8295r = {

    .identifier.name        = "RTL9311_2x8214QF_1x8224QF_2x8295R",
    .identifier.id          = HWP_RTL9311_2x8214QF_1x8224QF_2x8295R,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9311_2x8214qf_1x8224qf_2x8295r_swDescp,
    }

};
