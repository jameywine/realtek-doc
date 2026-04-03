/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9303 board
 *
 * Feature : RTL9302de 2XCUST1 board database
 *
 */

/*
 *  8*10G mode, has 8 ports: 8, 9, 10, 11, 12, 13, 14 ,15
 */
static hwp_swDescp_t rtl9302d_1x8224_qa_swDescp = {

    .chip_id                    = HWP_CHIP_AUTOPROBE,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .swcore_pcie_bus_number     = HWP_NOT_USED,
    .nic_supported              = TRUE,
    .cascade_role               = HWP_CASCADE_NONE,

    .port.descp = {
        { .mac_id =  8, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_COPPER,   .sds = HWP_1SDS(4), .phy_idx = 0, .smi = 1, .phy_addr =  8,  .led_c = 0,  .led_f = 0,  .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id =  9, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_COPPER,   .sds = HWP_1SDS(4), .phy_idx = 0, .smi = 1, .phy_addr =  9,  .led_c = 0,  .led_f = 0,  .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 10, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_COPPER,   .sds = HWP_1SDS(4), .phy_idx = 0, .smi = 1, .phy_addr =  10, .led_c = 0,  .led_f = 0,  .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 11, .attr = HWP_ETH,   .eth = HWP_2_5GE,  .medi = HWP_COPPER,   .sds = HWP_1SDS(4), .phy_idx = 0, .smi = 1, .phy_addr =  11, .led_c = 0,  .led_f = 0,  .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 28, .attr = HWP_CPU,   .eth = HWP_NONE,   .medi = HWP_NONE,     .sds = SDS_NONE(),  .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr =  HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE, .phy_mdi_pin_swap = 0,  },
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        .led_definition_set[0].led[0] = 0xA01,        /* 10Gbps link/act */
        .led_definition_set[0].led[1] = 0xA20,        /* 1000Mbps link/act */
        .led_definition_set[0].led[2] = 0xA02,        /* 5Gbps link/act */
        .led_definition_set[0].led[3] = 0xA08,        /* 2.5Gbps link/act */
        .led_definition_set[0].led[4] = HWP_LED_END,      /* None */
    },/* led.descp */

    .serdes.descp = {
        { .sds_id = 0, .mode = RTK_MII_DISABLE,             .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 1, .mode = RTK_MII_DISABLE,             .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 2, .mode = RTK_MII_DISABLE,             .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 3, .mode = RTK_MII_DISABLE,             .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 4, .mode = RTK_MII_USXGMII_10GQXGMII,   .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 6, .mode = RTK_MII_USXGMII_10GQXGMII,   .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = HWP_END },

    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 8,  .phy_max = 4, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [1] = { .chip = HWP_END },
    }   /* .phy.descp */
};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9302d_1x8224_qa = {
    .identifier.name        = "RTL9302D_1x8224_QA",
    .identifier.id          = HWP_RTL9302D_1X8224_QA,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9302d_1x8224_qa_swDescp,
    }

};
