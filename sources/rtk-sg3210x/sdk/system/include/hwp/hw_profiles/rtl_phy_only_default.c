/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : define how hardware design
 *
 * Feature : default PHY-only hardware profile
 *
 */

static hwp_swDescp_t rtl_phy_only_default_swDescp = {

    .chip_id                    = THIRD_PARTY_MAC_CHIP_ID,
    .swcore_supported           = TRUE,
    .swcore_access_method       = HWP_SW_ACC_MEM,
    .swcore_spi_chip_select     = HWP_NOT_USED,
    .swcore_pcie_bus_number     = HWP_NOT_USED,
    .nic_supported              = TRUE,
    .cascade_role               = HWP_CASCADE_NONE,

    .port.descp =
    {
        { .mac_id =  0, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  1, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  2, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  3, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  4, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  5, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  6, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  7, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  8, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id =  9, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 10, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 11, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 12, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 13, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 14, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 15, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 16, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 17, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 18, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 19, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 20, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 21, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 22, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 23, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 24, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 25, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 26, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 27, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 28, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 29, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 30, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 31, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 32, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 33, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 34, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 35, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 36, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 37, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 38, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 39, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 40, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 41, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 42, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 43, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 44, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 45, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 46, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 47, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 48, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 49, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 50, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 51, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 52, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 53, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 54, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 55, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 56, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 57, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 58, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 59, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 60, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 61, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = 62, .attr = HWP_ETH,   .eth = HWP_NONE, .medi = HWP_NONE,    .sds = SDS_NONE(),.phy_idx = HWP_NONE, .smi = HWP_NONE,        .phy_addr = HWP_NONE, .led_c = HWP_NONE, .led_f = HWP_NONE, .led_layout = HWP_NONE,   .phy_mdi_pin_swap = 0, .phy_mdi_pair_swap = 0},
        { .mac_id = HWP_END },
    },  /* .port.descp */

    .led.descp = {
        .led_active = LED_ACTIVE_HIGH,
        .led_if_sel = LED_IF_SEL_SERIAL,
        .led_signal_invert = FALSE,
        .led_definition_set[0].led[0] = HWP_LED_END,
    },  /* .led.descp */

    .serdes.descp = {
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = 0, .mode = RTK_MII_10GR,    .rx_polarity = SERDES_POLARITY_NORMAL, .tx_polarity = SERDES_POLARITY_NORMAL },
        { .sds_id = HWP_END },
    },  /* .serdes.descp */

    .phy.descp =
    {
        [0] = { .chip = HWP_END },
    },  /* .phy.descp */
};


/*
 * hardware profile
 */
static hwp_hwProfile_t rtl_phy_only_default = {

    .identifier.name        = "RTL_PHY_ONLY_DEFAULT",
    .identifier.id          = HWP_RTL_PHY_ONLY_DEFAULT,

    .soc.swDescp_index      = 0,
    .soc.slaveInterruptPin  = HWP_NONE,

    .sw_count               = 0,
    .swDescp = {
        [0]                 = &rtl_phy_only_default_swDescp,
    }
};

