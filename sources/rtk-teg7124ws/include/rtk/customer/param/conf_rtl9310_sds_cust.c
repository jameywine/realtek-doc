
#if defined(CONFIG_SDK_RTL9310)

/* HWP_RTL9313_4x8218D_4x8224_4XGE -S */
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s10 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 0x8,  /*post_amp*/ 0x9, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
    
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s11 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 0x8,  /*post_amp*/ 0x9, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
    
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s12 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 12,  /*post_amp*/ 0x9, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
   
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s13 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 13,  /*post_amp*/ 10, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};


sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s10 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s10,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
    
sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s11 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s11,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s12 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s12,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s13 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_4x8218D_4x8224_4XGE_s13,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};

sds_conf_per_unit_t sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE = {
    .sds_cfg = {
            [0] = NULL,
            [1] = NULL,
            [2] = NULL,
            [3] = NULL,
            [4] = NULL,
            [5] = NULL,
            [6] = NULL,
            [7] = NULL,
            [8] = NULL,
            [9] = NULL,
            [10] = &sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s10,
            [11] = &sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s11,
            [12] = &sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s12,
            [13] = &sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE_s13,
        },
};
/* HWP_RTL9313_4x8218D_4x8224_4XGE -E */

/* HWP_RTL9313_2x8264B_4XGE -S */
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s10 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 2,   /*main_amp*/ 8,  /*post_amp*/ 0x0, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x1,  /*post_en*/ 0x0,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
    
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s11 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 8,  /*post_amp*/ 0x1, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
    
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s12 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 9,  /*post_amp*/ 0x0, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x0,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
   
sds_tx_param_t rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s13 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 9,  /*post_amp*/ 0, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x0,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};


sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s10 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s10,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
    
sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s11 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s11,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s12 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s12,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
sds_conf_per_sds_t sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s13 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9310_fiber10g_Param_HWP_RTL9313_2x8264B_4XGE_s13,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};

sds_conf_per_unit_t sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE = {
    .sds_cfg = {
            [0] = NULL,
            [1] = NULL,
            [2] = NULL,
            [3] = NULL,
            [4] = NULL,
            [5] = NULL,
            [6] = NULL,
            [7] = NULL,
            [8] = NULL,
            [9] = NULL,
            [10] = &sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s10,
            [11] = &sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s11,
            [12] = &sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s12,
            [13] = &sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE_s13,
        },
};
/* HWP_RTL9313_2x8264B_4XGE -E */


sds_conf_t conf_rtl9310_cust_board_list_HWP_RTL9313_4x8218D_4x8224_4XGE = { 
    HWP_RTL9313_4x8218D_4x8224_4XGE,
    { &sds_cfg_rtl9310_HWP_RTL9313_4x8218D_4x8224_4XGE,
      NULL},
};

sds_conf_t conf_rtl9310_cust_board_list_HWP_RTL9313_2x8264B_4XGE = { 
    HWP_RTL9313_2x8264B_4XGE,
    { &sds_cfg_rtl9310_HWP_RTL9313_2x8264B_4XGE,
      NULL},
};

/*  board list for customer's 9310 config */
sds_conf_t  *conf_rtl9310_cust_board_list[] = {
        &conf_rtl9310_cust_board_list_HWP_RTL9313_4x8218D_4x8224_4XGE,
        &conf_rtl9310_cust_board_list_HWP_RTL9313_2x8264B_4XGE,
        NULL,
};

#endif


