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
 * 48 + 6*10G mode
 */
static hwp_swDescp_t rtl9330_swDescp = {

    .chip_id                    = HWP_CHIP_AUTOPROBE,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .swcore_pcie_bus_number     = HWP_NOT_USED,
    .nic_supported              = TRUE,
    .cascade_role               = HWP_CASCADE_NONE,

    .port.descp = {
		{ .mac_id = 0, .attr = HWP_ETH,   .eth = HWP_XGE,	.medi = HWP_COPPER,  .sds = HWP_1SDS(0),		 .phy_idx = 0,		  .smi = 0, 	   .phy_addr = 0,		 .led_c = 2,		.led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 4, .attr = HWP_ETH,   .eth = HWP_XGE,	.medi = HWP_COPPER,  .sds = HWP_1SDS(1),		 .phy_idx = 1,		  .smi = 0, 	   .phy_addr = 1,		 .led_c = 2,		.led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 8, .attr = HWP_ETH,   .eth = HWP_XGE,	.medi = HWP_COPPER,  .sds = HWP_1SDS(2),		 .phy_idx = 2,		  .smi = 0, 	   .phy_addr = 2,		 .led_c = 2,		.led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 12, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(3), 	  .phy_idx = 3, 	   .smi = 0,		.phy_addr = 3,		  .led_c = 2,		 .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 16, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(4), 	  .phy_idx = 4, 	   .smi = 0,		.phy_addr = 4,		  .led_c = 2,		 .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 17, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(5), 	  .phy_idx = 5, 	   .smi = 0,		.phy_addr = 5,		  .led_c = 2,		 .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 20, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(6), 	  .phy_idx = 6, 	   .smi = 1,		.phy_addr = 0,		  .led_c = 2,		 .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 21, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(7), 	  .phy_idx = 7, 	   .smi = 1,		.phy_addr = 1,		  .led_c = 2,		 .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 22, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(8), 	  .phy_idx = 8, 	   .smi = 1,		.phy_addr = 2,		  .led_c = 2,		 .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 23, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(9), 	  .phy_idx = 9, 	   .smi = 1,		.phy_addr = 3,		  .led_c = 2,		 .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(10),	   .phy_idx = 10,		 .smi = 1,		  .phy_addr = 4,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 25, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(11),	   .phy_idx = 11,		 .smi = 1,		  .phy_addr = 5,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 28, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(12),	   .phy_idx = 12,		 .smi = 2,		  .phy_addr = 0,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 29, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(13),	   .phy_idx = 13,		 .smi = 2,		  .phy_addr = 1,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 30, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(14),	   .phy_idx = 14,		 .smi = 2,		  .phy_addr = 2,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 31, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(15),	   .phy_idx = 15,		 .smi = 2,		  .phy_addr = 3,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(16),	   .phy_idx = 16,		 .smi = 2,		  .phy_addr = 4,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(17),	   .phy_idx = 17,		 .smi = 2,		  .phy_addr = 5,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(18),	   .phy_idx = 18,		 .smi = 3,		  .phy_addr = 0,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(19),	   .phy_idx = 19,		 .smi = 3,		  .phy_addr = 1,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(20),	   .phy_idx = 20,		 .smi = 3,		  .phy_addr = 2,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(21),	   .phy_idx = 21,		 .smi = 3,		  .phy_addr = 3,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(22),	   .phy_idx = 22,		 .smi = 3,		  .phy_addr = 4,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
		{ .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_XGE,	 .medi = HWP_COPPER,  .sds = HWP_1SDS(23),	   .phy_idx = 23,		 .smi = 3,		  .phy_addr = 5,		.led_c = 2, 	   .led_f = HWP_NONE,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

        { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_25GE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(28),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 0,        .led_c = HWP_NONE, .led_f = 2,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_25GE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(29),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 1,        .led_c = HWP_NONE, .led_f = 2,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_25GE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(30),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 2,        .led_c = HWP_NONE, .led_f = 2,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_25GE,  .medi = HWP_SERDES,  .sds = HWP_1SDS(31),       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 3,        .led_c = HWP_NONE, .led_f = 2,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_100GE,  .medi = HWP_SERDES, .sds = HWP_4SDS(32,33,34,35),      .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 4,        .led_c = HWP_NONE, .led_f = 2,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 52, .attr = HWP_ETH,   .eth = HWP_100GE,  .medi = HWP_SERDES, .sds = HWP_4SDS(36,37,38,39),      .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 5,        .led_c = HWP_NONE, .led_f = 2,       .led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 56, .attr = HWP_CPU,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE, .phy_mdi_pin_swap = 0,  },
        { .mac_id = HWP_END },
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        .led_signal_invert = FALSE,
        .led_definition_set[2].led[0] = 0x0280,
        .led_definition_set[2].led[1] = 0x0220,
        .led_definition_set[2].led[2] = 0x0BFF,
        .led_definition_set[2].led[3] = 0x0201,
        .led_definition_set[2].led[4] = HWP_LED_END,      /* None */
    },/* led.descp */

    .serdes.descp = {
        { .sds_id = 0, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 1, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 2, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 3, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 4, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 5, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 6, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 7, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 8, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 9, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 10, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 11, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 12, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 13, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 14, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 15, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 16, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 17, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 18, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 19, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 20, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 21, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 22, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 23, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
		
        { .sds_id = 28, .mode = RTK_MII_25GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 29, .mode = RTK_MII_25GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },		
        { .sds_id = 30, .mode = RTK_MII_25GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },		
        { .sds_id = 31, .mode = RTK_MII_25GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },		

        { .sds_id = 32, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 33, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },		
        { .sds_id = 34, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },		
        { .sds_id = 35, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },	

        { .sds_id = 36, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 37, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },		
        { .sds_id = 38, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },		
        { .sds_id = 39, .mode = RTK_MII_100GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },	
		
        { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 0, .phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [1] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 4, .phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [2] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 8,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [3] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 12,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [4] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 16,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [5] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 17,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [6] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 20, .phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [7] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 21, .phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [8] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 22,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [9] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 23,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [10] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 24,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [11] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 25,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[12] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 28,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[13] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 29,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[14] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 30,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[15] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 31,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[16] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 32,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[17] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 33,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[18] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 34,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[19] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 35,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[20] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 36,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[21] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 37,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[22] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 38,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
		[23] = { .chip = RTK_PHYTYPE_RTL8264, .mac_id = 39,.phy_max = 1, .interruptSocGpioDev = HWP_NONE, .interruptSocGpioPin = HWP_NONE },
        [24] = { .chip = HWP_END },
    }   /* .phy.descp */
};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl9330 = {
    .identifier.name        = "RTL9330",
    .identifier.id          = HWP_RTL9330,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9330_swDescp,
    }

};
