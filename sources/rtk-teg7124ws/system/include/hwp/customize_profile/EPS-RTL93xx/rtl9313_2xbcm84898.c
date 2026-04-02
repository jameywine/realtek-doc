/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9310 board
 *
 * Feature : RTL9310 board database
 *
 */


static hwp_swDescp_t rtl9313_2xbcm84898_swDescp = {

    .chip_id                    = RTL9313_CHIP_ID,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .nic_supported              = TRUE,

    .port.descp = {
        { .mac_id =  0, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 0, .phy_idx = 0,        .smi = 0,        .phy_addr =  0,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 0 },
        { .mac_id =  8, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 1, .phy_idx = 0,        .smi = 0,        .phy_addr =  1,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 1 },
        { .mac_id = 16, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 2, .phy_idx = 0,        .smi = 0,        .phy_addr =  2,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 0 },
        { .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 3, .phy_idx = 0,        .smi = 0,        .phy_addr =  3,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 1 },
        { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 4, .phy_idx = 0,        .smi = 0,        .phy_addr =  4,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 0 },
        { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 5, .phy_idx = 0,        .smi = 0,        .phy_addr =  5,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 1 },
        { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 6, .phy_idx = 0,        .smi = 0,        .phy_addr =  6,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 0 },
        { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_COPPER, .sds_idx = 7, .phy_idx = 0,        .smi = 0,        .phy_addr =  7,        .led_c = 0,        .led_f = 0,       .led_layout = SINGLE_SET,      .phy_mdi_pin_swap = 1 },
        { .mac_id = 52, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds_idx = 8,  .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,  .led_c = 1,        .led_f = 1,       .led_layout = SINGLE_SET,     .phy_mdi_pin_swap = 0 },
        { .mac_id = 53, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds_idx = 9,  .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,  .led_c = 1,        .led_f = 1,       .led_layout = SINGLE_SET,     .phy_mdi_pin_swap = 0 },
        { .mac_id = 54, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds_idx = 10, .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,  .led_c = 1,        .led_f = 1,       .led_layout = SINGLE_SET,     .phy_mdi_pin_swap = 0 },
        { .mac_id = 55, .attr = HWP_ETH,   .eth = HWP_XGE,   .medi = HWP_SERDES, .sds_idx = 11, .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE,  .led_c = 1,        .led_f = 1,       .led_layout = SINGLE_SET,     .phy_mdi_pin_swap = 0 },
        { .mac_id = 56, .attr = HWP_CPU,   .eth = HWP_NONE,  .medi = HWP_NONE,   .sds_idx = HWP_NONE, .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr =  HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE, .phy_mdi_pin_swap = 0 },
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        /* For Coper Port*/
        .led_definition_set[0].led[0] = 0x20B,        /* 10G 5G 2.5Glink*/
        .led_definition_set[0].led[1] = 0x2A0,        /* 100M 1000M 2.5G 5G link*/
        .led_definition_set[0].led[2] = 0xBAB,        /* 10G/5G/2.5G/1000M/100M/10M act */
        .led_definition_set[0].led[3] = HWP_LED_END,  /* None */
        .led_definition_set[0].led[4] = HWP_LED_END,  /* None */
        /* For Fiber Port*/
        .led_definition_set[1].led[0] = 0x201,        /* 10G link*/
        .led_definition_set[1].led[1] = 0x220,        /* 1000M link*/
        .led_definition_set[1].led[2] = 0xBAB,        /* 10G/5G/2.5G/1000M/100M/10M act */
        .led_definition_set[1].led[3] = HWP_LED_END,  /* None */
        .led_definition_set[1].led[4] = HWP_LED_END,  /* None */

    },/* led.descp */   


    .serdes.descp = {
        [0] =  { .sds_id = 2, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [1] =  { .sds_id = 3, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [2] =  { .sds_id = 4, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [3] =  { .sds_id = 5, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [4] =  { .sds_id = 6, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [5] =  { .sds_id = 7, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [6] =  { .sds_id = 8, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [7] =  { .sds_id = 9, .mode = RTK_MII_USXGMII_10GSXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },

        [8] =  { .sds_id = 10, .mode = RTK_MII_10GR,              .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        [9] =  { .sds_id = 11, .mode = RTK_MII_10GR,              .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        [10] = { .sds_id = 12, .mode = RTK_MII_10GR,              .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        [11] = { .sds_id = 13, .mode = RTK_MII_10GR,              .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },

        [12] = { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_CUST1 , .mac_id = 0,  .phy_max = 50 },
        [1] = { .chip = HWP_END },
    },   /* .phy.descp */

};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9313_2xbcm84898 = {

    .identifier.name        = "RTL9313_2xBCM84898",
    .identifier.id          = HWP_RTL9313_2xBCM84898,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9313_2xbcm84898_swDescp,
    }

};
