/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9310 FPGA board
 *
 * Feature : RTL9310 FPGA board database
 *
 */


hwp_swDescp_t rtl9310_fpga_l3_swDescp = {

    .chip_id                    = RTL9310_CHIP_ID,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .nic_supported              = TRUE,

    .port.descp = {
        { .mac_id =  0, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_COPPER, .sds_idx = 0,        .phy_idx = 0,        .smi = HWP_NONE, .phy_addr =  0,       .led_c = 0,       .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = 52, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_COPPER, .sds_idx = 0,        .phy_idx = 1,        .smi = HWP_NONE, .phy_addr =  0,       .led_c = 0,       .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = 53, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_COPPER, .sds_idx = 0,        .phy_idx = 2,        .smi = HWP_NONE, .phy_addr =  0,       .led_c = 0,       .led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = 56, .attr = HWP_CPU,   .eth = HWP_NONE, .medi = HWP_NONE,   .sds_idx = HWP_NONE, .phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE,.led_f = HWP_NONE,.led_layout = HWP_NONE,},
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_HIGH,
        .led_if_sel = LED_IF_SEL_BI_COLOR_SCAN,
        .led_definition_set[0].led[0] = 0xF,        /* 10/100Mbps link/act */
        .led_definition_set[0].led[1] = 0xA,        /* 1000Mbps link/act */
        .led_definition_set[0].led[2] = 0x6,        /* duplex mode */
        .led_definition_set[0].led[3] = HWP_LED_END,    /* None */
    },/* led.descp */

    .serdes.descp = {
        [0] = { .sds_id = 0, .mode = RTK_MII_QSGMII,       .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL},
        [1] = { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] =   { .chip = RTK_PHYTYPE_RTL8218B, .mac_id = 0,   .phy_max = 1 },
        [1] =   { .chip = RTK_PHYTYPE_RTL8218B, .mac_id = 52,  .phy_max = 1 },
        [2] =   { .chip = RTK_PHYTYPE_RTL8218B, .mac_id = 53,  .phy_max = 1 },
        [3] =   { .chip = HWP_END },
    }   /* .phy.descp */
};

/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9310_fpga_l3 = {

    .identifier.name        = "RTL9310_FPGA_L3",
    .identifier.id          = HWP_RTL9310_FPGA_L3,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9310_fpga_l3_swDescp,
    }

};


