/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9310 board
 *
 * Feature : RTL9310 board database
 *
 */



/*
 * 12*10G mode
 */
static hwp_swDescp_t rtl9313_12xge_sim_swDescp = {

    .chip_id                    = RTL9313_CHIP_ID,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_SIM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .swcore_pcie_bus_number     = HWP_NOT_USED,
    .nic_supported              = FALSE,
    .cascade_role               = HWP_CASCADE_NONE,

    .port.descp = {
        { .mac_id =  0, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(2),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id =  8, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(3),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 16, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(4),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(5),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(6),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(7),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(8),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(9),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 52, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(10),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 53, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(11),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 54, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(12),      .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 55, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(13),      .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE,        .led_c = HWP_NONE, .led_f = 3,       .led_layout = SINGLE_SET,},
        { .mac_id = 56, .attr = HWP_CPU,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE,  },
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_HIGH,
        .led_if_sel = LED_IF_SEL_SINGLE_COLOR_SCAN,
        .led_definition_set[3].led[0] = 0x0A01,
        .led_definition_set[3].led[1] = 0x0A20,
        .led_definition_set[3].led[2] = HWP_LED_END,      /* None */
    },/* led.descp */

    .serdes.descp = {
        { .sds_id = 2, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 3, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 4, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 5, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 6, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 7, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 8, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 9, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 10, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 11, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 12, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = 13, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = HWP_END },
    }   /* .phy.descp */
};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9313_12xge_sim = {

    .identifier.name        = "RTL9313_12XGE_SIM",
    .identifier.id          = HWP_RTL9313_12XGE_SIM,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9313_12xge_sim_swDescp,
    }

};
