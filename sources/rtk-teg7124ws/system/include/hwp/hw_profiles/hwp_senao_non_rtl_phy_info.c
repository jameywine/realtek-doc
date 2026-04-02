/*
 * Copyright (C) 2020 SENAO
 * All Rights Reserved.
 * SENAO
 */

/* SENAO non Realtek PHY Infomation structure */
#define HWP_SENAO_MAX_NON_RTL_PHY_NUM        10
#define HWP_SENAO_MAX_NON_RTL_SERDES_NUM     16
#define HWP_SENAO_MAX_NON_RTL_PORT_NUM       64
#define HWP_SENAO_MODEL_STRING_LEN_MAX       32

typedef struct phyp_senao_serdes_mapping_s
{
    uint32                        serdes_id;
    uint32                        mdio_addr;
} phyp_senao_serdes_mapping_t;

typedef struct phyp_senao_port_mapping_s
{
    uint32       unit;
    rtk_port_t   mac_id;
    uint32       phy_id;
    uint32       force_auto;
    int32        speed;
} phyp_senao_port_mapping_t;

typedef struct phyp_senao_phy_info_s
{
    uint32                       serdes_num;
    phyp_senao_serdes_mapping_t  serdes_mapping[HWP_SENAO_MAX_NON_RTL_SERDES_NUM];
    uint32                       port_num;
    phyp_senao_port_mapping_t    port_mapping[HWP_SENAO_MAX_NON_RTL_PORT_NUM];
} phyp_senao_phy_info_t;

typedef enum phy_senao_board_type_e
{
    RTL9302C_BCM50998_BOARD_TYPE_1 = 0, /*RTL9302C_BCM50998_8x2_5G_4FibXGE*/
    RTL9313_BCM84898_BOARD_TYPE_1,      /*RTL9313_BCM84898_8XGE_4FibXGE*/
    RTL9313_BCM50998_BOARD_TYPE_1,      /*RTL9313_4x8218D_2xBCM50998E_4XGE*/
    RTL9302B_BCM50998_BOARD_TYPE_1,     /*RTL9302B_2x8218D_BCM50998E_4XGE*/
    RTL9302C_GPY2xx_BOARD_TYPE_1,       /*RTL9302C_1xGPY2xx_1x8261I*/
    // Starry@20210326 add for GPY241
    RTL9302C_GPY2xx_BOARD_TYPE_2,        /*RTL9302C_2xGPY2xx_2XGE*/
    RTL9302C_GPY2xx_BOARD_TYPE_3,       /*RTL9302C_2xGPY2xx_2x8261I*/
    RTL9302C_GPY2xx_BOARD_TYPE_4,       /*RTL9302C_1xGPY2xx*/
    RTL9302C_GPY2xx_BOARD_TYPE_5,       /*RTL9302C_2xGPY2xx*/
    RHY_BOARD_TYPE_END
} phy_senao_board_type_t;


#ifdef __HWP_INIT_C__
static phyp_senao_phy_info_t senao_phy_info[HWP_SENAO_MAX_NON_RTL_PHY_NUM] = {
        /*RTL9302C_BCM50998_8x2_5G_4FibXGE*/
        [0] = {
                  .serdes_num=2,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 2,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = 3,       .mdio_addr = 5      },
                                    [2]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=8,
                  .port_mapping={
                                    [0] = { .unit = 0,       .mac_id = 0,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1] = { .unit = 0,       .mac_id = 1,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2] = { .unit = 0,       .mac_id = 2,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3] = { .unit = 0,       .mac_id = 3,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4] = { .unit = 0,       .mac_id = 8,         .phy_id = 4,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [5] = { .unit = 0,       .mac_id = 9,         .phy_id = 5,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [6] = { .unit = 0,       .mac_id = 10,        .phy_id = 6,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [7] = { .unit = 0,       .mac_id = 11,        .phy_id = 7,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [8] = { .unit = HWP_END, .mac_id = HWP_END,   .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        /*RTL9313_BCM84898_8XGE_4FibXGE*/
        [1] = {
                  .serdes_num=8,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 2,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = 3,       .mdio_addr = 8      },
                                    [2]={ .serdes_id = 6,       .mdio_addr = 16     },  
                                    [3]={ .serdes_id = 7,       .mdio_addr = 24     },
                                    [4]={ .serdes_id = 10,      .mdio_addr = 32     },  
                                    [5]={ .serdes_id = 11,      .mdio_addr = 40     },
                                    [6]={ .serdes_id = 14,      .mdio_addr = 48     },
                                    [7]={ .serdes_id = 15,      .mdio_addr = 50     },
                                    [8]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=8,
                  .port_mapping={
                                    [0] = { .unit = 0,       .mac_id = 0,          .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1] = { .unit = 0,       .mac_id = 8,          .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2] = { .unit = 0,       .mac_id = 16,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3] = { .unit = 0,       .mac_id = 24,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4] = { .unit = 0,       .mac_id = 32,         .phy_id = 4,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [5] = { .unit = 0,       .mac_id = 40,         .phy_id = 5,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [6] = { .unit = 0,       .mac_id = 48,         .phy_id = 6,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [7] = { .unit = 0,       .mac_id = 50,         .phy_id = 7,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [8] = { .unit = HWP_END, .mac_id = HWP_END,    .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        /*RTL9313_4x8218D_2xBCM50998E_4XGE*/
        [2] = {
                  .serdes_num=4,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 6,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = 7,       .mdio_addr = 8      },
                                    [2]={ .serdes_id = 8,       .mdio_addr = 16     },
                                    [3]={ .serdes_id = 9,       .mdio_addr = 24     },
                                    [4]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=16,
                  .port_mapping={
                                    [0]  = { .unit = 0,       .mac_id = 32,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1]  = { .unit = 0,       .mac_id = 33,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2]  = { .unit = 0,       .mac_id = 36,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3]  = { .unit = 0,       .mac_id = 37,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4]  = { .unit = 0,       .mac_id = 40,         .phy_id = 4,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [5]  = { .unit = 0,       .mac_id = 41,         .phy_id = 5,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [6]  = { .unit = 0,       .mac_id = 42,         .phy_id = 12,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [7]  = { .unit = 0,       .mac_id = 43,         .phy_id = 13,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [8]  = { .unit = 0,       .mac_id = 44,         .phy_id = 6,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [9]  = { .unit = 0,       .mac_id = 45,         .phy_id = 7,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [10] = { .unit = 0,       .mac_id = 46,         .phy_id = 14,        .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [11] = { .unit = 0,       .mac_id = 47,         .phy_id = 15,        .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [12] = { .unit = 0,       .mac_id = 48,         .phy_id = 8,        .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [13] = { .unit = 0,       .mac_id = 49,         .phy_id = 9,        .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [14] = { .unit = 0,       .mac_id = 50,         .phy_id = 10,        .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [15] = { .unit = 0,       .mac_id = 51,         .phy_id = 11,        .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [16] = { .unit = HWP_END, .mac_id = HWP_END,    .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        /*RTL9302B_2x8218D_BCM50998E_4XGE*/
        [3] = {
                  .serdes_num=2,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 4,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = 5,       .mdio_addr = 8      },
                                    [2]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=8,
                  .port_mapping={
                                    [0]  = { .unit = 0,       .mac_id = 16,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1]  = { .unit = 0,       .mac_id = 17,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2]  = { .unit = 0,       .mac_id = 18,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3]  = { .unit = 0,       .mac_id = 19,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4]  = { .unit = 0,       .mac_id = 20,         .phy_id = 4,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [5]  = { .unit = 0,       .mac_id = 21,         .phy_id = 5,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [6]  = { .unit = 0,       .mac_id = 22,         .phy_id = 6,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [7]  = { .unit = 0,       .mac_id = 23,         .phy_id = 7,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [8] = { .unit = HWP_END, .mac_id = HWP_END,    .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        /*RTL9302C_1xGPY2xx_1x8261I*/
        [4] = {
                  .serdes_num=1,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 2,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=4,
                  .port_mapping={
                                    [0] = { .unit = 0,       .mac_id = 0,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1] = { .unit = 0,       .mac_id = 1,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2] = { .unit = 0,       .mac_id = 2,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3] = { .unit = 0,       .mac_id = 3,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4] = { .unit = HWP_END, .mac_id = HWP_END,   .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        // Starry@20210326
        /*RTL9302 2xGPY2xx__2XGE*/
        [5] = {
                  .serdes_num=2,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 2,       .mdio_addr = 0x18      }, 
                                    [1]={ .serdes_id = 3,       .mdio_addr = 0x1C      }, 
                                    [2]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=8,
                  .port_mapping={
                                    [0] = { .unit = 0,       .mac_id = 0,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1] = { .unit = 0,       .mac_id = 1,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2] = { .unit = 0,       .mac_id = 2,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3] = { .unit = 0,       .mac_id = 3,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4] = { .unit = 0,       .mac_id = 8,         .phy_id = 8,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [5] = { .unit = 0,       .mac_id = 9,         .phy_id = 9,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [6] = { .unit = 0,       .mac_id = 10,        .phy_id = 10,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [7] = { .unit = 0,       .mac_id = 11,        .phy_id = 11,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [8] = { .unit = HWP_END, .mac_id = HWP_END,   .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        /*RTL9302C_2xGPY2xx_2x8261I*/
        [6] = {
                  .serdes_num=2,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 2,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = 3,       .mdio_addr = 8      },
                                    [2]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=8,
                  .port_mapping={
                                    [0] = { .unit = 0,       .mac_id = 0,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1] = { .unit = 0,       .mac_id = 1,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2] = { .unit = 0,       .mac_id = 2,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3] = { .unit = 0,       .mac_id = 3,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4] = { .unit = 0,       .mac_id = 8,         .phy_id = 4,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [5] = { .unit = 0,       .mac_id = 9,         .phy_id = 5,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [6] = { .unit = 0,       .mac_id = 10,        .phy_id = 6,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [7] = { .unit = 0,       .mac_id = 11,        .phy_id = 7,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [8] = { .unit = HWP_END, .mac_id = HWP_END,   .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        /*RTL9302C_1xGPY2xx*/
        [7] = {
                  .serdes_num=1,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 2,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=4,
                  .port_mapping={
                                    [0] = { .unit = 0,       .mac_id = 0,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1] = { .unit = 0,       .mac_id = 1,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2] = { .unit = 0,       .mac_id = 2,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3] = { .unit = 0,       .mac_id = 3,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4] = { .unit = HWP_END, .mac_id = HWP_END,   .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
        /*RTL9302C_2xGPY2xx*/
        [8] = {
                  .serdes_num=2,
                  .serdes_mapping={
                                    [0]={ .serdes_id = 2,       .mdio_addr = 0      },
                                    [1]={ .serdes_id = 3,       .mdio_addr = 8      },
                                    [2]={ .serdes_id = HWP_END, .mdio_addr = HWP_END}
                  },
                  .port_num=8,
                  .port_mapping={
                                    [0] = { .unit = 0,       .mac_id = 0,         .phy_id = 0,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [1] = { .unit = 0,       .mac_id = 1,         .phy_id = 1,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [2] = { .unit = 0,       .mac_id = 2,         .phy_id = 2,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [3] = { .unit = 0,       .mac_id = 3,         .phy_id = 3,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [4] = { .unit = 0,       .mac_id = 8,         .phy_id = 4,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [5] = { .unit = 0,       .mac_id = 9,         .phy_id = 5,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [6] = { .unit = 0,       .mac_id = 10,        .phy_id = 6,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [7] = { .unit = 0,       .mac_id = 11,        .phy_id = 7,         .force_auto = 0,         .speed = PORT_SPEED_100M},
                                    [8] = { .unit = HWP_END, .mac_id = HWP_END,   .phy_id = HWP_END,   .force_auto = HWP_END,   .speed = HWP_END}
                  },
              },
};
#else
extern phyp_senao_phy_info_t *senao_non_rtl_phy_info;
extern uint32 phy_board_type;
#endif /*__HWP_INIT_C__*/
