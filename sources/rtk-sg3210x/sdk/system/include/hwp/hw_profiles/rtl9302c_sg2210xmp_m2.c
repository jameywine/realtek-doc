/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9302B board
 *
 * Feature : RTL9302C_2xRTL8284_2XGE demo board
 *
 */

static hwp_swDescp_t rtl9302c_sg2210xmp_m2_swDescp = {

    .chip_id                    = HWP_CHIP_AUTOPROBE,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .swcore_pcie_bus_number     = HWP_NOT_USED,
    .nic_supported              = TRUE,
    .cascade_role               = HWP_CASCADE_NONE,

    .port.descp = {
        { .mac_id = 0, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(2), .phy_idx = 0,        .smi = 0,        .phy_addr =  0,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 1, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(2), .phy_idx = 0,        .smi = 0,        .phy_addr =  1,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 2, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(2), .phy_idx = 0,        .smi = 0,        .phy_addr =  2,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 3, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(2), .phy_idx = 0,        .smi = 0,        .phy_addr =  3,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 8, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(3), .phy_idx = 1,        .smi = 1,        .phy_addr =  4,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 9, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(3), .phy_idx = 1,        .smi = 1,        .phy_addr =  5,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 10, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(3), .phy_idx = 1,        .smi = 1,        .phy_addr =  6,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 11, .attr = HWP_ETH,   .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds = HWP_1SDS(3), .phy_idx = 1,        .smi = 1,        .phy_addr =  7,        .led_c = 2,        .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds = HWP_1SDS(6), .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE,        .led_f = 3,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 25, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds = HWP_1SDS(7), .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE,        .led_f = 3,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 26, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds = HWP_1SDS(8), .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE,        .led_f = 3,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 27, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds = HWP_1SDS(9), .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE,        .led_f = 3,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 28, .attr = HWP_CPU,   .eth = HWP_NONE,  .medi = HWP_NONE,   .sds = SDS_NONE(), .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr =  HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE, .phy_mdi_pin_swap = 0,  },
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        .led_signal_invert = FALSE,
        .led_definition_set[0].led[0] = 0x0A01,        /*  2.5g_lite/2.5g link/act */
        .led_definition_set[0].led[1] = 0x0A20,        /* 10M/100M/500M/1G link/act */
        .led_definition_set[0].led[2] = HWP_LED_END,  /* None */
        .led_definition_set[1].led[0] = 0x0A01,        /* 10Gbps link */
        .led_definition_set[1].led[1] = 0x0A20,        /* 10Gbps link/act */
        .led_definition_set[1].led[2] = HWP_LED_END,  /* None */
    },/* led.descp */

    .serdes.descp = {
        { .sds_id = 2, .mode = RTK_MII_USXGMII_10GQXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 3, .mode = RTK_MII_USXGMII_10GQXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 6, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 7, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 8, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 9, .mode = RTK_MII_10GR,               .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 0,  .phy_max = 4, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [1] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 8,  .phy_max = 4, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [2] = { .chip = HWP_END },
    },   /* .phy.descp */

};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9302c_sg2210xmp_m2 = {

    .identifier.name        = "RTL9302C_SG2210XMP_M2",
    .identifier.id          = HWP_RTL9302C_SG2210XMP_M2,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9302c_sg2210xmp_m2_swDescp,
    }

};

