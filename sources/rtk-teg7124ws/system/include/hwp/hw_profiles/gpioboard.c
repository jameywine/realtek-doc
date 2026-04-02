#include <hwp/hw_profile.h>
#include <hwp/hw_profiles/hwp_senao_gpio_board.c>

#define ISS_PORT_SPEED_CAP_10M      0x01
#define ISS_PORT_SPEED_CAP_100M     0x02
#define ISS_PORT_SPEED_CAP_1G       0x04
#define ISS_PORT_SPEED_CAP_2500M    0x08
#define ISS_PORT_SPEED_CAP_5G       0x10
#define ISS_PORT_SPEED_CAP_10G      0x20

typedef struct
{
    int   dev;
    int   port;
    int   SpeedCap;
} tRTKCfaIndexMap;

typedef struct port_info_params {
    int                       id;
    const tRTKCfaIndexMap           *UbootToSdkPortMap;
}t_port_info_params;

int GpioboardInfoInit (void);
void PortInfoInit (void);
void GpioboardIdGet (int *);
int PortMapSdkPortGet (int);
extern const tRTKCfaIndexMap PortMap_RTL9301_3x8218D_4XGE[4];
extern const tRTKCfaIndexMap PortMap_RTL9311_6x8218D_6XGE_DEMO[4];
extern const tRTKCfaIndexMap PortMap_RTL9302C_2xBCM50998[4];
extern const tRTKCfaIndexMap PortMap_RTL9313_2xBCM84898[4];


const tRTKCfaIndexMap PortMap_RTL9301_3x8218D_4XGE[4] =
{
    /* 25 */ {0,24, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 26 */ {0,25, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 27 */ {0,26, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 28 */ {0,27, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G}
};

const tRTKCfaIndexMap PortMap_RTL9311_6x8218D_6XGE_DEMO[4] =
{
    /* 49 */ {0,48, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 50 */ {0,50, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 51 */ {0,52, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 52 */ {0,53, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G}
};

const tRTKCfaIndexMap PortMap_RTL9302C_2xBCM50998[4] =
{/*need to check HW spec*/
    /*  9 */ {0, 24, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 10 */ {0, 25, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 11 */ {0, 26, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 12 */ {0, 27, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G}
};

const tRTKCfaIndexMap PortMap_RTL9313_2xBCM84898[4] =
{
    /*  9 */ {0, 52, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 10 */ {0, 53, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 11 */ {0, 54, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G},
    /* 12 */ {0, 55, ISS_PORT_SPEED_CAP_1G|ISS_PORT_SPEED_CAP_10G}
};


t_port_info_params port_Info;

static const struct port_info_params port_info_table[] = {
    {
        .id = HWP_RTL9301_3x8218D_4XGE,
        .UbootToSdkPortMap = PortMap_RTL9301_3x8218D_4XGE,
    },
    {
        .id = HWP_RTL9302C_2xBCM50998,
        .UbootToSdkPortMap = PortMap_RTL9302C_2xBCM50998,
    },
    {
        .id = HWP_RTL9311_6x8218D_6XGE_DEMO,
        .UbootToSdkPortMap = PortMap_RTL9311_6x8218D_6XGE_DEMO,
    },
    {
        .id = HWP_RTL9313_2xBCM84898,
        .UbootToSdkPortMap = PortMap_RTL9313_2xBCM84898,
    },
};

hwp_senao_gpio_board_t gpioboard_Info;

/****************************************************************************/
/*                                                                          */
/*    Function Name      : GpioboardInfoInit                                */
/*                                                                          */
/*    Description        : This function is used to Gpio board related      */
/*                         info init.                                       */
/*                                                                          */
/*    Input(s)           : None.                                            */
/*                                                                          */
/*    Output(s)          : None.                                            */
/*                                                                          */
/*    Returns            : Board id.                                        */
/****************************************************************************/
int GpioboardInfoInit()
{
    int chipid = 0;
    int BoardId = 2;
    char *BoardMagicNumber;

    BoardMagicNumber = getenv ("magic_number");

    for(chipid = 0 ; chipid < HWP_SENAO_GPIO_MAX_NUM; chipid++)
    {
        if(strcmp(BoardMagicNumber,senao_customer_gpio_board_info.board_930X[chipid].magicnumber) == 0)
        {
            BoardId = 0;
            gpioboard_Info.gpio_id = senao_customer_gpio_board_info.board_930X[chipid].gpio_id;
            gpioboard_Info.id = senao_customer_gpio_board_info.board_930X[chipid].id;
            strcpy(gpioboard_Info.BoardModel, senao_customer_gpio_board_info.board_930X[chipid].BoardModel);
            strcpy(gpioboard_Info.magicnumber, senao_customer_gpio_board_info.board_930X[chipid].magicnumber);
            break;
        }
        else if(strcmp(BoardMagicNumber,senao_customer_gpio_board_info.board_931X[chipid].magicnumber) == 0)
        {
            BoardId = 1;
            gpioboard_Info.gpio_id = senao_customer_gpio_board_info.board_931X[chipid].gpio_id;
            gpioboard_Info.id = senao_customer_gpio_board_info.board_931X[chipid].id;
            strcpy(gpioboard_Info.BoardModel, senao_customer_gpio_board_info.board_931X[chipid].BoardModel);
            strcpy(gpioboard_Info.magicnumber, senao_customer_gpio_board_info.board_931X[chipid].magicnumber);
            break;
        }
    }
    return BoardId;
}

/****************************************************************************/
/*                                                                          */
/*    Function Name      : PortInfoInit                                     */
/*                                                                          */
/*    Description        : This function is used to Port related info init. */
/*                                                                          */
/*    Input(s)           : None.                                            */
/*                                                                          */
/*    Output(s)          : None.                                            */
/*                                                                          */
/*    Returns            : true/false                                       */
/****************************************************************************/
void PortInfoInit()
{
    int id = 0;
    int MatchIdx = 0;

    GpioboardIdGet(&id);

    for(MatchIdx = 0; MatchIdx < sizeof(port_info_table); MatchIdx++)
    {
        if(id == port_info_table[MatchIdx].id)
        {
            port_Info.id = port_info_table[MatchIdx].id;
            port_Info.UbootToSdkPortMap = port_info_table[MatchIdx].UbootToSdkPortMap;
            break;
        }
    }
}


/****************************************************************************/
/*                                                                          */
/*    Function Name      : GpioboardIdGet                                   */
/*                                                                          */
/*    Description        : This function is used to get id of Gpio board    */
/*                                                                          */
/*    Input(s)           : None.                                            */
/*                                                                          */
/*    Output(s)          : None.                                            */
/*                                                                          */
/*    Returns            : true/false                                       */
/****************************************************************************/
void GpioboardIdGet (int *id)
{
    *id = gpioboard_Info.id;
}

/****************************************************************************/
/*                                                                          */
/*    Function Name      : PortMapSdkPortGet                                */
/*                                                                          */
/*    Description        : This function is used to get Max port number that*/
/*                         support fiber port                               */
/*                                                                          */
/*    Input(s)           : None.                                            */
/*                                                                          */
/*    Output(s)          : None.                                            */
/*                                                                          */
/*    Returns            : SFP Max port number                              */
/****************************************************************************/
int PortMapSdkPortGet(int PortIdx)
{
    const tRTKCfaIndexMap   *UbootToSdkPort = NULL;

    UbootToSdkPort = port_Info.UbootToSdkPortMap;

    return UbootToSdkPort[PortIdx].port;
}

