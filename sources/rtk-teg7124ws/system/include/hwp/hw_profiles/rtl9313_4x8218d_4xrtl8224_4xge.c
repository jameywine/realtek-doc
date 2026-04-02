/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL9310 board
 *
 * Feature : RTL9310 board database
 *
 */


static hwp_swDescp_t rtl9313_4x8218d_4xrtl8224_4xge_swDescp = {
    .chip_id                    = RTL9313_CHIP_ID,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .nic_supported              = TRUE,

    .port.descp = {
        { .mac_id =  0, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 0,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  1, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 1,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  2, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 2,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  3, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 3,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  4, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 4,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  5, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 5,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  6, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 6,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  7, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 0,       .phy_idx = 0,        .smi = 0,        .phy_addr = 7,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  8, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 8,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id =  9, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 9,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 10, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 10,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 11, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 11,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 12, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 12,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 13, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 13,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 14, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 14,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 15, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 1,       .phy_idx = 1,        .smi = 0,        .phy_addr = 15,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 16, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 16,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 17, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 17,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 18, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 18,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 19, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 19,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 20, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 20,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 21, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 21,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 22, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 22,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 23, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 2,       .phy_idx = 2,        .smi = 0,        .phy_addr = 23,       .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        
        { .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 0,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 25, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 1,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 26, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 2,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 27, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 3,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 28, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 4,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 29, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 5,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 30, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 6,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
        { .mac_id = 31, .attr = HWP_ETH,   .eth = HWP_GE,   .medi = HWP_COPPER,  .sds_idx = 3,       .phy_idx = 3,        .smi = 2,        .phy_addr = 7,        .led_c = 0,        .led_f = 0       ,.led_layout = SINGLE_SET,},
#if 0  // BCM phy0-sdrsed6-addr=8
        { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 20,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 21,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 22,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 23,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 16,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 17,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 18,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 19,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
#else   // RTL8224 phy[]0-3]-sdrsed[6-9]-addr=[0-3, 4-7, 8-11, 12-15]
  #if 0 // P33-36, 41-46 link down
        { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        
        { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        
        { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        
        { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
        { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
        { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
  #else
        #if 0 // P33-34,41-42 link down, 35-36, up
        { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},

        { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        
        { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        
        { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
        #else 
          #if 0 // try . u-boot_20221213_1407_690_hwp.bin, [p33-36 ping OK], P41-42 down , p37-40, 43-48 link up          
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET,},
          #else
          #if 0  // try u-boot_20221213_1542_692_hwp2.bin, [p33-36 ping OK], p33-48 link up
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0  // [boot_20221213_1724_692_hwp3.bin] try p37-40 change phy_mdi_pin_swap=1/0 , 
            // p33-36 no diff
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p37-40 change(reversed) phy_mdi_pin_swap=1/0, link=37-40
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 4,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
            // p41-44 change(reversed) phy_mdi_pin_swap=1/0, down=41-44
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 5,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif          

          #if 0  // [u-boot_20221213_1858_692_hwp4.bin] try  ping:33-36,P48, U:33-36,37-40,41,44,45,48  D:42,43,46,47
          // p33-36 no diff
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p37-40 change(backet) phy_mdi_pin_swap=1/0, phy_idx=5 
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p41-44 change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=6
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=7
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif          

          #if 0  // [u-boot_20221213_2002_692_hwp5.bin] try  ping:33-36,P47-48, U:33-36,37,42,43,46-48,  D:38,39,40,41,44,45
          // p33-36 no diff
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p37-40 change(backet) phy_mdi_pin_swap=1/0, phy_idx=5 
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p41-44 change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=6
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
            // change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=7
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0  // [u-boot_20221214_0951_692_hwp6.bin] try  ping:33-36,P47-48, U:33-36,37,42,43,46-48,  D:38,39,40,41,44,45
          // p33-36
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p37-40
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p41-44
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
            // p45-48
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif
  
          #if 0  // [u-u-boot_20221215_1844_700_net_macid_mdf] try FAE reback-1
          // ping: P33-38,      U:39-48
          // p33-36 no diff
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40 change(backet) phy_mdi_pin_swap=1/0, phy_idx=5 
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p41-44 change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=6
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=7
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0  // [u-boot_20221215_2006_700_net_macid_mdf-2.bin] try FAE reback-2 ,mac_id & sds_idx modify
          // ping: P33,35,37,      U:39,41,43,45,
          // p33-36 no diff
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40 change(backet) phy_mdi_pin_swap=1/0, phy_idx=5 
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p41-44 change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=6
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=7
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0  // [u-boot_20221215_2103_700_net_macid_mdf-3.bin] try FAE reback-3 ,mac_id & sds_idx modify
          // D:P45     , U_led_off :46-48
          // U,Ping: P33-38 , 
          // U_Ping_led_shtL:P39+2,P40+2, P41-44+4
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40 change(backet) phy_mdi_pin_swap=1/0, phy_idx=5 
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 6,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 6,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p41-44 change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=6
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 6,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 6,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=7
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 8,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 9,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 7,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0  // [u-boot_20221215_1018_700_net_macid_mdf-4.bin] try FAE reback-4 ,mac_id & sds_idx modify#4
          // linkDown          : P41-44,
          // linkUp(led_shift) : P45~46(-6), P47~48(-4)
          // linkUp_Ping       : P33-38, 
          // linkUp_Ping(led_shift):P39(+2),P40(+2),
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40 change(backet) phy_mdi_pin_swap=1/0, phy_idx=5 
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p41-44 change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=6
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=7
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0  // [u-boot_20221215_1201_700_net_macid_mdf-5.bin] try FAE reback-5 ,phy_mdi_pin_swap modify#5
          // linkDown          : P41-44,
          // linkUp(led_shift) : P45~46(-6), P47~48(-4)
          // linkUp_Ping       : P33-38, 
          // linkUp_Ping(led_shift):P39(+2),P40(+2),
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40 change(backet) phy_mdi_pin_swap=1/0, phy_idx=5 
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          
            // p41-44 change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=6
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // change(other) phy_mdi_pin_swap=1/0/1/0, phy_idx=7
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0
          // [u-boot_20221216_1358_700_net_phy_descp_mdf-6.bin] try FAE reback-6 , .phy.descp[6-7].mac_id = 44->48,48->44- modify#4 -> #6,#7
          // [u-boot_20221216_1502_700_net_phy_descp_mdf-7.bin] try FAE reback-7 , .phy.descp[7].mac_id = 44/4->42/8 modify#6 = #7
          // [u-boot_20221216_1554_700_net_phy_descp_mdf-8.bin] try FAE reback-8 , .phy.descp[7].phy_max = 42/8->42/4  #8
          // #4
          // linkDown          : P41-44,
          // linkUp(led_shift) : P45~46(-6), P47~48(-4)
          // linkUp_Ping       : P33-38, 
          // linkUp_Ping(led_shift):P39-40(+2)
          // #6 ---- modify -----
          
          // linkDown          : p45-48
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39-40(+2),P41-44(+4),
          // #7 ---- modify -----
          
          // linkDown          : P45,46
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2),P41~44(+4),P47~48(-4)
          // #8 ---- modify -----same #7
          
          // linkDown          : P45,46
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2),P41~44(+4),P47~48(-4)


            // p33-36
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p41-44
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p45-48
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif


          #if 0
          // [u-boot_20221216_1554_700_net_phy_descp_mdf-9.bin],swap mac_id= 順著sds與內定macid排       #9
          // #8 ---- modify
          // linkDown          : P45,46
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2),P41~44(+4),P47~48(-4)
          // #9 ---- modify
          // linkDown          : P45,46
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2),P41~44(+4),P47~48(-4)

          // p33-36
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          
            // p41-44
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

          // p45-48
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0  
          // [u-boot_20221216_1554_700_net_phy_descp_mdf-9.bin],swap phy_mdi_pin_swap[42/43]=0->1 #9 -> 10
          // #9 ---- modify
          // linkDown          : P45,46
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2),P41~44(+4),P47~48(-4)
          // #10 ---- modify  => result=>all port can traffic
          // linkDown          : 
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2),P41~44(+4),P45~48(-4)

          // p33-36
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          
            // p41-44
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

          // p45-48
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 0
          // [u-boot_20221216_1554_700_net_phy_descp_mdf-9.bin],swap phy_mdi_pin_swap[44/45]=1->0 #10 -> 11
          // #10 ---- modify  => result=>all port can traffic
          // linkDown          : 
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2),P41~44(+4),P45~48(-4)
          // #11 ---- modify  => result=>
          // linkDown          :  
          // linkUp(led_shift) : 
          // linkUp_Ping       : P33-38
          // linkUp_Ping(led_shift):P39~40(+2), P41~44(+4),P45~48(-4)

          // p33-36
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p41-44
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

          // p45-48
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1      ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1      ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 1,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif

          #if 1  // Starry@20230104 for Patch-704 [Correct MDI inverse operation]
          // p33-36
          { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 0,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 1,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 2,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 4,       .phy_idx = 4,        .smi = 1,        .phy_addr = 3,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p37-40
          { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 4,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 5,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 6,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 5,       .phy_idx = 5,        .smi = 1,        .phy_addr = 7,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          
            // p41-44
          { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 8,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 9,        .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 10,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 6,       .phy_idx = 6,        .smi = 1,        .phy_addr = 11,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},

          // p45-48
          { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 12,        .led_c = 1,        .led_f = 1      ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 13,        .led_c = 1,        .led_f = 1      ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 14,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_2_5GE,   .medi = HWP_COPPER,  .sds_idx = 7,       .phy_idx = 7,        .smi = 1,        .phy_addr = 15,       .led_c = 1,        .led_f = 1       ,.led_layout = SINGLE_SET, .phy_mdi_pin_swap = 0,},
          #endif


          { .mac_id = 52, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds_idx = 8,       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 0,        .led_c = 2, .led_f = 2,       .led_layout = SINGLE_SET,},
          { .mac_id = 53, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds_idx = 9,       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 1,        .led_c = 2, .led_f = 2,       .led_layout = SINGLE_SET,},
          { .mac_id = 54, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds_idx = 10,       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 2,        .led_c = 2, .led_f = 2,       .led_layout = SINGLE_SET,},
          { .mac_id = 55, .attr = HWP_ETH,   .eth = HWP_XGE,  .medi = HWP_SERDES,  .sds_idx = 11,       .phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = 3,        .led_c = 2, .led_f = 2,       .led_layout = SINGLE_SET,},
          
          { .mac_id = 56, .attr = HWP_CPU,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds_idx = HWP_NONE,.phy_idx = HWP_NONE, .smi = HWP_NONE, .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE,.led_layout = HWP_NONE, },
          { .mac_id = HWP_END },
        
          #endif
        #endif
  #endif
#endif  
   
    },  /* port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_LOW,
        .led_if_sel = LED_IF_SEL_SERIAL,
        /* For GE Coper Port*/
        .led_definition_set[0].led[0] = 0x220,        /* 1000M link*/
        .led_definition_set[0].led[1] = 0x280,        /* 100M link*/
        .led_definition_set[0].led[2] = 0xBAB,        /* 10G/5G/2.5G/1000M/100M/10M act */
        .led_definition_set[0].led[3] = HWP_LED_END,  /* None */
        .led_definition_set[0].led[4] = HWP_LED_END,  /* None */
        /* For 2_5GE Coper Port*/
        .led_definition_set[1].led[0] = 0x20B,        /* 2.5G 5G 10G link*/
        .led_definition_set[1].led[1] = 0x2A0,        /* 100M 1000M link*/
        .led_definition_set[1].led[2] = 0xBAB,        /* 10G/5G/2.5G/1000M/100M/10M act */
        .led_definition_set[1].led[3] = HWP_LED_END,  /* None */
        .led_definition_set[1].led[4] = HWP_LED_END,  /* None */
        /* For Fiber Port*/
        .led_definition_set[2].led[0] = 0x201,        /* 10G link*/
        .led_definition_set[2].led[1] = 0x220,        /* 1000M link*/
        .led_definition_set[2].led[2] = 0xBAB,        /* 10G/5G/2.5G/1000M/100M/10M act */
        .led_definition_set[2].led[3] = HWP_LED_END,  /* None */
        .led_definition_set[2].led[4] = HWP_LED_END,  /* None */
    },/* led.descp */

    .serdes.descp = {
        [0] =  { .sds_id = 2, .mode = RTK_MII_XSGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [1] =  { .sds_id = 3, .mode = RTK_MII_XSGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [2] =  { .sds_id = 4, .mode = RTK_MII_XSGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [3] =  { .sds_id = 5, .mode = RTK_MII_XSGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        
        [4] =  { .sds_id = 6, .mode = RTK_MII_USXGMII_10GQXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [5] =  { .sds_id = 7, .mode = RTK_MII_USXGMII_10GQXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [6] =  { .sds_id = 8, .mode = RTK_MII_USXGMII_10GQXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        [7] =  { .sds_id = 9, .mode = RTK_MII_USXGMII_10GQXGMII,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        
        [8] =  { .sds_id = 10, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        [9] =  { .sds_id = 11, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        [10] = { .sds_id = 12, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        [11] = { .sds_id = 13, .mode = RTK_MII_10GR,  .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_CHANGE },
        [12] = { .sds_id = HWP_END },
    }, /* serdes.descp */

    .phy.descp = {
        [0] = { .chip = RTK_PHYTYPE_RTL8218D, .mac_id = 0, .phy_max = 8 },
        [1] = { .chip = RTK_PHYTYPE_RTL8218D, .mac_id = 8, .phy_max = 8 },
        [2] = { .chip = RTK_PHYTYPE_RTL8218D, .mac_id = 16,.phy_max = 8 },
        [3] = { .chip = RTK_PHYTYPE_RTL8218D, .mac_id = 24,.phy_max = 8 },
        
        #if 0
        // [u-boot_20221214_0951_692_hwp6.bin]
        [4] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 32,.phy_max = 4 },
        [5] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 36,.phy_max = 4 },
        [6] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 40,.phy_max = 4 },
        [7] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 44,.phy_max = 4 },
        #endif
        
        #if 0
        // [u-boot_20221215_1844_700_net_macid_mdf], try FAE reback-1
        // [u-boot_20221215_2006_700_net_macid_mdf-2.bin] try FAE reback-2 ,mac_id & sds_idx modify#2
        // [u-boot_20221215_2103_700_net_macid_mdf-3.bin] try FAE reback-3 ,mac_id & sds_idx modify#3
        // [u-boot_20221215_1018_700_net_macid_mdf-4.bin] try FAE reback-4 ,mac_id & sds_idx modify#4
        [4] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 32,.phy_max = 4 },
        [5] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 40,.phy_max = 4 },
        [6] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 44,.phy_max = 4 },
        [7] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 48,.phy_max = 4 },
        #endif

        #if 0
        // [u-boot_20221216_1358_700_net_phy_descp_mdf-6.bin] try FAE reback-6 , .phy.descp[6-7].mac_id = 44->48,48->44- modify#4 -> #6
        [4] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 32,.phy_max = 4 },
        [5] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 40,.phy_max = 4 },
        [6] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 48,.phy_max = 4 },
        [7] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 44,.phy_max = 4 },
        #endif

        #if 0
        // [u-boot_20221216_1502_700_net_phy_descp_mdf-7.bin] try FAE reback-7 , .phy.descp[7].mac_id = 42/4->42/8 modify#6 = #7
        [4] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 32,.phy_max = 4 },
        [5] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 40,.phy_max = 4 },
        [6] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 48,.phy_max = 4 },
        [7] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 42,.phy_max = 8 },
        #endif

        #if 1
        // [u-boot_20221216_1554_700_net_phy_descp_mdf-8.bin] try FAE reback-8 , .phy.descp[7].phy_max = 42/8->42/4  #8
        [4] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 32,.phy_max = 4 },
        [5] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 40,.phy_max = 4 },
        [6] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 48,.phy_max = 4 },
        [7] = { .chip = RTK_PHYTYPE_RTL8224, .mac_id = 42,.phy_max = 4 },
        #endif


        // [u--7.bin] try FAE reback-6 , .phy.descp[7].mac_id = 42/8 modify#6 = #7

        
        [8] = { .chip = HWP_END },        

    }   /* .phy.descp */
};


/*
 * hardware profile
 */
    static hwp_hwProfile_t rtl9313_4x8218d_4xrtl8224_4xge = {
    .identifier.name        = "RTL9313_4x8218D_4x8224_4XGE",
    .identifier.id          = HWP_RTL9313_4x8218D_4x8224_4XGE,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 1,
    .swDescp = {
        [0]                 = &rtl9313_4x8218d_4xrtl8224_4xge_swDescp,
    }

};
