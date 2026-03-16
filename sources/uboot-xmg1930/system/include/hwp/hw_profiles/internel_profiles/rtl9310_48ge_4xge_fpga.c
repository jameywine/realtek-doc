/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9310 FPGA board
 *
 * Feature : RTL9310 FPGA board database
 *
 */


static hwp_swDescp_t rtl9310_48ge_4xge_fpga_swDescp = {

    .chip_id                    = RTL9310_CHIP_ID,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .nic_supported              = TRUE,

    .port.descp = {
        { .mac_id = 0,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  0,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 1,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  0,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 2,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  0,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 3,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  0,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 4,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  1,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 5,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  1,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 6,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  1,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 7,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  1,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 8,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  2,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 9,  .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  2,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 10, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  2,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 11, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  2,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 12, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  3,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 13, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  3,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 14, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  3,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 15, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  3,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 16, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  4,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 17, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  4,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 18, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  4,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 19, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  4,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 20, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  5,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 21, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  5,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 22, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  5,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 23, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  5,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 24, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  6,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 25, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  6,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 26, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  6,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 27, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  6,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 28, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  7,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 29, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  7,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 30, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  7,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 31, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  7,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 32, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  8,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 33, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  8,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 34, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  8,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 35, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  8,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 36, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  9,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 37, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  9,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 38, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  9,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 39, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx =  9,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 40, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 10,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 41, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 10,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 42, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 10,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 43, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 10,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 44, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 11,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 45, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 11,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 46, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 11,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 47, .attr = HWP_ETH,           .eth = HWP_GE,   .medi = HWP_NONE,   .sds_idx = 11,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 48, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 12,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 49, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 13,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 50, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 14,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 51, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 15,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = 0,        .led_f = 0,       .led_layout = HWP_NONE,},
        { .mac_id = 52, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 16,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = 53, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 18,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = 54, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 20,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = 55, .attr = HWP_ETH,           .eth = HWP_XGE,  .medi = HWP_NONE,   .sds_idx = 22,       .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = 56, .attr = HWP_CPU,           .eth = HWP_NONE, .medi = HWP_NONE,   .sds_idx = HWP_NONE, .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_HIGH,
        .led_if_sel = LED_IF_SEL_BI_COLOR_SCAN,
        .led_definition_set[0].led[0] = 0xF,    /* 10/100Mbps link/act */
        .led_definition_set[0].led[1] = 0xA,    /* 1000Mbps link/act */
        .led_definition_set[0].led[2] = 0x6,    /* duplex mode */
        .led_definition_set[0].led[3] = HWP_LED_END,    /* duplex mode */
    },/* led.descp */

    .serdes.descp = {
        [0]  = { .sds_id = 0,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [1]  = { .sds_id = 1,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [2]  = { .sds_id = 2,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [3]  = { .sds_id = 3,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [4]  = { .sds_id = 4,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [5]  = { .sds_id = 5,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [6]  = { .sds_id = 6,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [7]  = { .sds_id = 7,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [8]  = { .sds_id = 8,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [9]  = { .sds_id = 9,  .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [10] = { .sds_id = 10, .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [11] = { .sds_id = 11, .mode = RTK_MII_QSGMII,     .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [12] = { .sds_id = 12, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [13] = { .sds_id = 13, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [14] = { .sds_id = 14, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [15] = { .sds_id = 15, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [16] = { .sds_id = 16, .mode = RTK_MII_10GR,       .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [17] = { .sds_id = 17, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [18] = { .sds_id = 18, .mode = RTK_MII_10GR,       .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [19] = { .sds_id = 19, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [20] = { .sds_id = 20, .mode = RTK_MII_10GR,       .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [21] = { .sds_id = 21, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [22] = { .sds_id = 22, .mode = RTK_MII_10GR,       .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [23] = { .sds_id = 23, .mode = RTK_MII_DISABLE,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = HWP_END },
    }   /* .phy.descp */
};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9310_48ge_4xge_fpga = {

    .identifier.name        = "RTL9310_48GE_4XGE_FPGA",
    .identifier.id          = HWP_RTL9310_48GE_4XGE_FPGA,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9310_48ge_4xge_fpga_swDescp,
    }

};


