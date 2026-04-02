/* SENAO Board PHY Infomation structure */
#include <hwp/hw_profile.h>
#define HWP_BOARD_STRING_LEN_MAX                    64
#define HWP_MAGIC_NUMBER_STRING_LEN_MAX             16
#define MODEL_RTL8218_COMPATIBLE_NUM                3
typedef struct hwp_board_phyid_s
{
    char            board_hwp[HWP_BOARD_STRING_LEN_MAX];
    unsigned int    phyID1;
    unsigned int    phyID2;
} hwp_board_phyid_t;

typedef struct hwp_board_s
{
    char            MagicNumber[HWP_MAGIC_NUMBER_STRING_LEN_MAX];
    hwp_board_phyid_t    BoardModel[PHY_CHIP_RTL8218_NAME_END];
    int                         IdentifyPort; /* Accroding to which PHY to be identified */
} hwp_board_t;


static hwp_board_t board_info_rtl8218_series[MODEL_RTL8218_COMPATIBLE_NUM] = {
};