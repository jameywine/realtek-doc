#include <hwp/hw_profile.h>

#if defined(CONFIG_SDK_RTL9300)


/* SQX10-HWP_RTL9302C_2xGPY2xx_2XGE_1LED -S */
sds_tx_param_t rtl9303_80G_txParam_SQX10_s6 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/0x8, /*pre_amp*/0x0,   /*main_amp*/0x9,  /*post_amp*/0x1, /*post2_amp*/ 0x0,
                  /*pre_en*/0x0,  /*post_en*/0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
    
sds_tx_param_t rtl9303_80G_txParam_SQX10_s7 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp 1*/ 0x0, /* main_amp 9*/ 0x9, /*post_amp 1*/ 0x1,/*post2_amp*/ 0x0, 
                  /*pre_en*/ 0x0, /*post_en*/ 0x1,/*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};

sds_conf_per_sds_t sds_cfg_rtl9303_80G_SQX10_s6 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9303_80G_txParam_SQX10_s6,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
    
sds_conf_per_sds_t sds_cfg_rtl9303_80G_SQX10_s7 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9303_80G_txParam_SQX10_s7,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};

sds_conf_per_unit_t sds_cfg_rtl9303_80G_SQX10 = {
    .sds_cfg = {
            [0] = NULL,
            [1] = NULL,
            [2] = NULL,
            [3] = NULL,
            [4] = NULL,
            [5] = NULL,
            [6] = &sds_cfg_rtl9303_80G_SQX10_s6,
            [7] = &sds_cfg_rtl9303_80G_SQX10_s7,
            [8] = NULL,
            [9] = NULL,
        },
};
/* SQX10-HWP_RTL9302C_2xGPY2xx_2XGE_1LED -E */


/* SQX10SP & HWP_RTL9302C_2xGPY2xx_2XGE & DTS_RTL9302C_2xGPY2xx_2XGE -S */
sds_tx_param_t rtl9303_80G_txParam_SQX10SP_s6 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/0x8, /*pre_amp*/0x0,   /*main_amp*/0x9,  /*post_amp*/0x1, /*post2_amp*/ 0x0,
                  /*pre_en*/0x0,  /*post_en*/0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};
    
sds_tx_param_t rtl9303_80G_txParam_SQX10SP_s7 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp 1*/ 0x1, /* main_amp 9*/ 0x9, /*post_amp 1*/ 0x1,/*post2_amp*/ 0x0, 
                  /*pre_en*/ 0x1, /*post_en*/ 0x1,/*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};


sds_conf_per_sds_t sds_cfg_rtl9303_80G_SQX10SP_s6 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9303_80G_txParam_SQX10SP_s6,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};
    
sds_conf_per_sds_t sds_cfg_rtl9303_80G_SQX10SP_s7 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9303_80G_txParam_SQX10SP_s7,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};

sds_conf_per_unit_t sds_cfg_rtl9303_80G_SQX10SP = {
    .sds_cfg = {
            [0] = NULL,
            [1] = NULL,
            [2] = NULL,
            [3] = NULL,
            [4] = NULL,
            [5] = NULL,
            [6] = &sds_cfg_rtl9303_80G_SQX10SP_s6,
            [7] = &sds_cfg_rtl9303_80G_SQX10SP_s7,
            [8] = NULL,
            [9] = NULL,
        },
};
/* SQX10SP & HWP_RTL9302C_2xGPY2xx_2XGE & DTS_RTL9302C_2xGPY2xx_2XGE -E */

/* HWP_RTL9302b_2x8218D_2x8224_4XGE -S */
sds_tx_param_t rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s6 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x1,   /*main_amp*/ 0x9,  /*post_amp*/ 0x2, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x1,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};

sds_tx_param_t rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s7 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x1,   /*main_amp*/ 0x9,  /*post_amp*/ 0x4, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x1,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};

sds_tx_param_t rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s8 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 12,  /*post_amp*/ 0x4, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x0,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};

sds_tx_param_t rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s9 = {
    .eyeParam_valid = TRUE,
    .eyeParam =  {/*impedance*/ 0x8, /*pre_amp*/ 0x0,   /*main_amp*/ 10,  /*post_amp*/ 0x6, /*post2_amp*/ 0x0,
                  /*pre_en*/ 0x1,  /*post_en*/ 0x1,  /*post2_en*/ 0x0,
                 },
    .tx_param_misc = NULL,
};

sds_conf_per_sds_t sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s6 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s6,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};

sds_conf_per_sds_t sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s7 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s7,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};

sds_conf_per_sds_t sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s8 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s8,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};

sds_conf_per_sds_t sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s9 = {
    .sds_param = NULL,
    .fiber10g_8431_param = &rtl9300_fiber10g_Param_HWP_RTL9302b_2x8218D_2x8224_4XGE_s9,
    .dac50cm_tx_param = NULL,
    .dac100cm_tx_param =  NULL,
    .dac300cm_tx_param =  NULL,
    .dac500cm_tx_param =  NULL,
};


sds_conf_per_unit_t sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE = {
    .sds_cfg = {
            [0] = NULL,
            [1] = NULL,
            [2] = NULL,
            [3] = NULL,
            [4] = NULL,
            [5] = NULL,
            [6] = &sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s6,
            [7] = &sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s7,
            [8] = &sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s8,
            [9] = &sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE_s9,
        },
};
/* HWP_RTL9302b_2x8218D_2x8224_4XGE -E */


sds_conf_t conf_rtl9300_cust_board_list_SQX10SP = { 
    HWP_RTL9302C_2xGPY2xx_2XGE,
    { &sds_cfg_rtl9303_80G_SQX10SP,
      NULL},
};
    
sds_conf_t conf_rtl9300_cust_board_list_DAT_SQX10SP = {
    HWP_DTS_RTL9302C_2xGPY2xx_2XGE,
    { &sds_cfg_rtl9303_80G_SQX10SP,
      NULL},
};    

sds_conf_t conf_rtl9300_cust_board_list_SQX10 = { 
    HWP_RTL9302C_2xGPY2xx_2XGE_1LED,
    { &sds_cfg_rtl9303_80G_SQX10,
      NULL},
};

sds_conf_t conf_rtl9300_cust_board_list_HWP_RTL9302b_2x8218D_2x8224_4XGE = { 
    HWP_RTL9302B_2x8218D_2x8224_4XGE,
    { &sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE,
      NULL},
};

sds_conf_t conf_rtl9300_cust_board_list_HWP_RTL9302b_2x8218D_2x8224_4XGE_SNAP_LED = {
    HWP_RTL9302B_2x8218D_2x8224_4XGE_SNAP_LED,
    { &sds_cfg_rtl9300_HWP_RTL9302b_2x8218D_2x8224_4XGE,
      NULL},
};

/*  board list for customer's 9300 config */
sds_conf_t  *conf_rtl9300_cust_board_list[] = {
     &conf_rtl9300_cust_board_list_SQX10SP,
     &conf_rtl9300_cust_board_list_SQX10,
     &conf_rtl9300_cust_board_list_DAT_SQX10SP,
     &conf_rtl9300_cust_board_list_HWP_RTL9302b_2x8218D_2x8224_4XGE,
     &conf_rtl9300_cust_board_list_HWP_RTL9302b_2x8218D_2x8224_4XGE_SNAP_LED,
     NULL
};

#endif


