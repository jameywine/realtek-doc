/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9300 FPGA board
 *
 * Feature : RTL9302C  8x2.5G Copper RTL9302C*1(RTL9302C + 2*GPY2xx)
 *
 */

static hwp_swDescp_t rtl9302c_2xgpy2xx_swDescp = {

    .chip_id                    = RTL9302C_CHIP_ID_16X2_5G, /*RTL9302C_CHIP_ID,*/
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .nic_supported              = TRUE,

    .port.descp = {
        { .mac_id =  0, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 0,        .phy_idx = 0,        .smi = 0,        .phy_addr =   4,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id =  1, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 0,        .phy_idx = 0,        .smi = 0,        .phy_addr =   5,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id =  2, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 0,        .phy_idx = 0,        .smi = 0,        .phy_addr =   6,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id =  3, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 0,        .phy_idx = 0,        .smi = 0,        .phy_addr =   7,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id =  8, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 1,        .phy_idx = 1,        .smi = 0,        .phy_addr =   8,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id =  9, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 1,        .phy_idx = 1,        .smi = 0,        .phy_addr =   9,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id = 10, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 1,        .phy_idx = 1,        .smi = 0,        .phy_addr =  10,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id = 11, .attr = HWP_ETH, .eth = HWP_2_5GE, .medi = HWP_COPPER, .sds_idx = 1,        .phy_idx = 1,        .smi = 0,        .phy_addr =  11,       .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id = 24, .attr = HWP_ETH, .eth = HWP_XGE,   .medi = HWP_SERDES, .sds_idx = 2,        .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,  .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id = 25, .attr = HWP_ETH, .eth = HWP_XGE,   .medi = HWP_SERDES, .sds_idx = 3,        .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,  .led_c = 0,        .led_f = 0,        .led_layout = SINGLE_SET,},
        { .mac_id = 28, .attr = HWP_CPU, .eth = HWP_NONE,  .medi = HWP_NONE,   .sds_idx = HWP_NONE, .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr =  HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,},
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        /* For 2.5G Port */
        .led_definition_set[0].led[0] = 0x208,        /* 2.5G link*/
        .led_definition_set[0].led[1] = 0x2A0,        /* 100M 1000M link*/
        .led_definition_set[0].led[2] = 0xAA8,        /* 2.5G/1000M/100M act */
        .led_definition_set[0].led[3] = HWP_LED_END,  /* None */
        .led_definition_set[0].led[4] = HWP_LED_END,  /* None */

    },/* led.descp */

    .serdes.descp = {
        [0] = { .sds_id = 2, .mode = RTK_MII_USXGMII_10GQXGMII, .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [1] = { .sds_id = 3, .mode = RTK_MII_USXGMII_10GQXGMII, .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [2] = { .sds_id = 6, .mode = RTK_MII_10GR,              .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [3] = { .sds_id = 7, .mode = RTK_MII_10GR,              .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [4] = { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_GPY2xx ,  .mac_id =  0, .phy_max = 4 },
        [1] = { .chip = RTK_PHYTYPE_GPY2xx ,  .mac_id =  8, .phy_max = 4 },
        [2] = { .chip = HWP_END },
    },   /* .phy.descp */

};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9302c_2xgpy2xx = {

    .identifier.name        = "RTL9302C_2xGPY2xx",
    .identifier.id          = HWP_RTL9302C_2xGPY2xx,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9302c_2xgpy2xx_swDescp,
    }

};

