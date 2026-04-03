#include <common/rt_type.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>
#include <osal/lib.h>
#include <osal/memory.h>
#include <osal/time.h>
#include <hwp/hw_profile.h>
#include <hal/phy/phy_common.h>
#include <hal/phy/phydef.h>
#include <private/drv/oob/phy/oob_phy_rtl8213b.h>
#include <private/drv/oob/phy/oob_phy_rtl8213b_reg.h>



static rtk_switch_halCtrl_t *halCtrl = NULL;
static init_state_t    init_state = INIT_NOT_COMPLETED;


drv_oob_phyInfo_t oob_phy_rtl8213b_info =
{
    .eth_type = HWP_GE,
    .is_c45 = FALSE,
    .support_poll_std_reg = FALSE,
};

static rtk_switch_halCtrl_t rtl8213b_hal_Ctrl =
{
    /* Switch Chip */
    CHIP_RTL8364B,

    /* Logical to Physical */
    {0xFF, 1, 0xFF, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     6, 7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },

    /* Physical to Logical */
    {UNDEFINE_PORT, UTP_PORT1, UNDEFINE_PORT, UTP_PORT3, UNDEFINE_PORT, UNDEFINE_PORT, EXT_PORT0, EXT_PORT1,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT},

    /* Port Type */
    {UNKNOWN_PORT, UTP_PORT, UNKNOWN_PORT, UTP_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     EXT_PORT, EXT_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT},

    /* PTP port */
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0 },

    /* Valid port mask */
    ( (0x1 << UTP_PORT1) | (0x1 << UTP_PORT3) | (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Valid UTP port mask */
    ( (0x1 << UTP_PORT1) | (0x1 << UTP_PORT3) ),

    /* Valid EXT port mask */
    ( (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Valid CPU port mask */
    0x00,

    /* Minimum physical port number */
    0,

    /* Maxmum physical port number */
    7,

    /* Physical port mask */
    0xCA,

    /* Combo Logical port ID */
    4,

    /* HSG Logical portmask */
    (0x1 << EXT_PORT0),

    /* SGMII Logical portmask */
    ( (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Max Meter ID */
    32,

    /* MAX LUT Address Number */
    2112,

    /* Trunk Group Mask */
    0x01,

    /* Packet buffer page number */
    512
};

static rtk_switch_halCtrl_t rtl8363sc_vb_hal_Ctrl =
{
    /* Switch Chip */
    CHIP_RTL8363SC_VB,

    /* Logical to Physical */
    {0xFF, 0xFF, 1, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     6, 7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },

    /* Physical to Logical */
    {UNDEFINE_PORT, UTP_PORT2, UNDEFINE_PORT, UTP_PORT3, UNDEFINE_PORT, UNDEFINE_PORT, EXT_PORT0, EXT_PORT1,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT},

    /* Port Type */
    {UNKNOWN_PORT, UNKNOWN_PORT, UTP_PORT, UTP_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     EXT_PORT, EXT_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT},

    /* PTP port */
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0 },

    /* Valid port mask */
    ( (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) | (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Valid UTP port mask */
    ( (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) ),

    /* Valid EXT port mask */
    ( (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Valid CPU port mask */
    0x00,

    /* Minimum physical port number */
    0,

    /* Maxmum physical port number */
    7,

    /* Physical port mask */
    0xCA,

    /* Combo Logical port ID */
    4,

    /* HSG Logical portmask */
    (0x1 << EXT_PORT0),

    /* SGMII Logical portmask */
    ( (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Max Meter ID */
    32,

    /* MAX LUT Address Number */
    2112,

    /* Trunk Group Mask */
    0x01,

    /* Packet buffer page number */
    512
};

uint8 Sgmii_Init[SGMII_INIT_SIZE] = {
0x02,0x03,0xA9,0xE4,0xF5,0xA8,
0xD2,0xAF,0x22,0x00,0x00,0x02,0x04,0x35,
0xC5,0xF0,0xF8,0xA3,0xE0,0x28,0xF0,0xC5,
0xF0,0xF8,0xE5,0x82,0x15,0x82,0x70,0x02,
0x15,0x83,0xE0,0x38,0xF0,0x22,0x75,0xF0,
0x08,0x75,0x82,0x00,0xEF,0x2F,0xFF,0xEE,
0x33,0xFE,0xCD,0x33,0xCD,0xCC,0x33,0xCC,
0xC5,0x82,0x33,0xC5,0x82,0x9B,0xED,0x9A,
0xEC,0x99,0xE5,0x82,0x98,0x40,0x0C,0xF5,
0x82,0xEE,0x9B,0xFE,0xED,0x9A,0xFD,0xEC,
0x99,0xFC,0x0F,0xD5,0xF0,0xD6,0xE4,0xCE,
0xFB,0xE4,0xCD,0xFA,0xE4,0xCC,0xF9,0xA8,
0x82,0x22,0xB8,0x00,0xC1,0xB9,0x00,0x59,
0xBA,0x00,0x2D,0xEC,0x8B,0xF0,0x84,0xCF,
0xCE,0xCD,0xFC,0xE5,0xF0,0xCB,0xF9,0x78,
0x18,0xEF,0x2F,0xFF,0xEE,0x33,0xFE,0xED,
0x33,0xFD,0xEC,0x33,0xFC,0xEB,0x33,0xFB,
0x10,0xD7,0x03,0x99,0x40,0x04,0xEB,0x99,
0xFB,0x0F,0xD8,0xE5,0xE4,0xF9,0xFA,0x22,
0x78,0x18,0xEF,0x2F,0xFF,0xEE,0x33,0xFE,
0xED,0x33,0xFD,0xEC,0x33,0xFC,0xC9,0x33,
0xC9,0x10,0xD7,0x05,0x9B,0xE9,0x9A,0x40,
0x07,0xEC,0x9B,0xFC,0xE9,0x9A,0xF9,0x0F,
0xD8,0xE0,0xE4,0xC9,0xFA,0xE4,0xCC,0xFB,
0x22,0x75,0xF0,0x10,0xEF,0x2F,0xFF,0xEE,
0x33,0xFE,0xED,0x33,0xFD,0xCC,0x33,0xCC,
0xC8,0x33,0xC8,0x10,0xD7,0x07,0x9B,0xEC,
0x9A,0xE8,0x99,0x40,0x0A,0xED,0x9B,0xFD,
0xEC,0x9A,0xFC,0xE8,0x99,0xF8,0x0F,0xD5,
0xF0,0xDA,0xE4,0xCD,0xFB,0xE4,0xCC,0xFA,
0xE4,0xC8,0xF9,0x22,0xEB,0x9F,0xF5,0xF0,
0xEA,0x9E,0x42,0xF0,0xE9,0x9D,0x42,0xF0,
0xE8,0x9C,0x45,0xF0,0x22,0xE0,0xFC,0xA3,
0xE0,0xFD,0xA3,0xE0,0xFE,0xA3,0xE0,0xFF,
0x22,0xE0,0xF8,0xA3,0xE0,0xF9,0xA3,0xE0,
0xFA,0xA3,0xE0,0xFB,0x22,0xEC,0xF0,0xA3,
0xED,0xF0,0xA3,0xEE,0xF0,0xA3,0xEF,0xF0,
0x22,0xE4,0x90,0x06,0x28,0xF0,0xFD,0x7C,
0x01,0x7F,0x3F,0x7E,0x1D,0x12,0x04,0x93,
0x7D,0x40,0x7C,0x00,0x7F,0x36,0x7E,0x13,
0x12,0x04,0x93,0xE4,0xFF,0xFE,0xFD,0x80,
0x25,0xE4,0x7F,0xFF,0x7E,0xFF,0xFD,0xFC,
0x90,0x06,0x24,0x12,0x01,0x0F,0xC3,0x12,
0x00,0xF2,0x50,0x1B,0x90,0x06,0x24,0x12,
0x01,0x03,0xEF,0x24,0x01,0xFF,0xE4,0x3E,
0xFE,0xE4,0x3D,0xFD,0xE4,0x3C,0xFC,0x90,
0x06,0x24,0x12,0x01,0x1B,0x80,0xD2,0xE4,
0xF5,0xA8,0xD2,0xAF,0x7D,0x1F,0xFC,0x7F,
0x49,0x7E,0x13,0x12,0x04,0x93,0x12,0x04,
0xBA,0x7D,0xFE,0x7C,0x00,0x7F,0xAA,0x7E,
0x12,0x12,0x04,0x93,0x7D,0x41,0x7C,0x00,
0x7F,0x36,0x7E,0x13,0x12,0x04,0x93,0xE4,
0xFF,0xFE,0xFD,0x80,0x25,0xE4,0x7F,0x20,
0x7E,0x4E,0xFD,0xFC,0x90,0x06,0x24,0x12,
0x01,0x0F,0xC3,0x12,0x00,0xF2,0x50,0x1B,
0x90,0x06,0x24,0x12,0x01,0x03,0xEF,0x24,
0x01,0xFF,0xE4,0x3E,0xFE,0xE4,0x3D,0xFD,
0xE4,0x3C,0xFC,0x90,0x06,0x24,0x12,0x01,
0x1B,0x80,0xD2,0xC2,0x00,0xC2,0x01,0xD2,
0xA9,0xD2,0x8C,0x7D,0x3D,0x7C,0x00,0x7F,
0x01,0x7E,0x66,0x12,0x04,0x93,0x7D,0x80,
0x7C,0x00,0x7F,0x00,0x7E,0x66,0x12,0x04,
0x93,0x7F,0x02,0x7E,0x66,0x12,0x04,0x6F,
0x7F,0x02,0x7E,0x66,0x12,0x04,0x6F,0xEF,
0x30,0xE4,0x07,0xE4,0x90,0x06,0x28,0xF0,
0x80,0xD1,0x90,0x06,0x28,0xE0,0x70,0x12,
0x12,0x03,0x03,0x90,0x06,0x28,0x74,0x01,
0xF0,0xE4,0x90,0x06,0x2B,0xF0,0xA3,0xF0,
0x80,0xB9,0xC3,0x90,0x06,0x2C,0xE0,0x94,
0x62,0x90,0x06,0x2B,0xE0,0x94,0x00,0x40,
0xAA,0xE4,0xF0,0xA3,0xF0,0x12,0x03,0x03,
0x90,0x06,0x28,0x74,0x01,0xF0,0x80,0x9B,
0x75,0x0F,0x80,0x75,0x0E,0x7E,0x75,0x0D,
0xAA,0x75,0x0C,0x83,0xE4,0xF5,0x10,0x7F,
0x36,0x7E,0x13,0x12,0x04,0x6F,0xEE,0xC4,
0xF8,0x54,0xF0,0xC8,0xEF,0xC4,0x54,0x0F,
0x48,0x54,0x07,0xFB,0x7A,0x00,0xEA,0x70,
0x4A,0xEB,0x14,0x60,0x1C,0x14,0x60,0x27,
0x24,0xFE,0x60,0x31,0x14,0x60,0x3C,0x24,
0x05,0x70,0x38,0x75,0x0B,0x00,0x75,0x0A,
0xC2,0x75,0x09,0xEB,0x75,0x08,0x0B,0x80,
0x36,0x75,0x0B,0x40,0x75,0x0A,0x59,0x75,
0x09,0x73,0x75,0x08,0x07,0x80,0x28,0x75,
0x0B,0x00,0x75,0x0A,0xE1,0x75,0x09,0xF5,
0x75,0x08,0x05,0x80,0x1A,0x75,0x0B,0xA0,
0x75,0x0A,0xAC,0x75,0x09,0xB9,0x75,0x08,
0x03,0x80,0x0C,0x75,0x0B,0x00,0x75,0x0A,
0x62,0x75,0x09,0x3D,0x75,0x08,0x01,0x75,
0x89,0x11,0xE4,0x7B,0x60,0x7A,0x09,0xF9,
0xF8,0xAF,0x0B,0xAE,0x0A,0xAD,0x09,0xAC,
0x08,0x12,0x00,0x60,0xAA,0x06,0xAB,0x07,
0xC3,0xE4,0x9B,0xFB,0xE4,0x9A,0xFA,0x78,
0x17,0xF6,0xAF,0x03,0xEF,0x08,0xF6,0x18,
0xE6,0xF5,0x8C,0x08,0xE6,0xF5,0x8A,0x74,
0x0D,0x2B,0xFB,0xE4,0x3A,0x18,0xF6,0xAF,
0x03,0xEF,0x08,0xF6,0x75,0x88,0x10,0x53,
0x8E,0xC7,0xD2,0xA9,0x22,0x7D,0x02,0x7C,
0x00,0x7F,0x4A,0x7E,0x13,0x12,0x04,0x93,
0x7D,0x46,0x7C,0x71,0x7F,0x02,0x7E,0x66,
0x12,0x04,0x93,0x7D,0x03,0x7C,0x00,0x7F,
0x01,0x7E,0x66,0x12,0x04,0x93,0x7D,0xC0,
0x7C,0x00,0x7F,0x00,0x7E,0x66,0x12,0x04,
0x93,0xE4,0xFF,0xFE,0x0F,0xBF,0x00,0x01,
0x0E,0xEF,0x64,0x64,0x4E,0x70,0xF5,0x7D,
0x04,0x7C,0x00,0x7F,0x02,0x7E,0x66,0x12,
0x04,0x93,0x7D,0x00,0x7C,0x04,0x7F,0x01,
0x7E,0x66,0x12,0x04,0x93,0x7D,0xC0,0x7C,
0x00,0x7F,0x00,0x7E,0x66,0x12,0x04,0x93,
0xE4,0xFD,0xFC,0x7F,0x02,0x7E,0x66,0x12,
0x04,0x93,0x7D,0x00,0x7C,0x04,0x7F,0x01,
0x7E,0x66,0x12,0x04,0x93,0x7D,0xC0,0x7C,
0x00,0x7F,0x00,0x7E,0x66,0x12,0x04,0x93,
0xE4,0xFD,0xFC,0x7F,0x4A,0x7E,0x13,0x12,
0x04,0x93,0x7D,0x06,0x7C,0x71,0x7F,0x02,
0x7E,0x66,0x12,0x04,0x93,0x7D,0x03,0x7C,
0x00,0x7F,0x01,0x7E,0x66,0x12,0x04,0x93,
0x7D,0xC0,0x7C,0x00,0x7F,0x00,0x7E,0x66,
0x02,0x04,0x93,0x78,0x7F,0xE4,0xF6,0xD8,
0xFD,0x75,0x81,0x3C,0x02,0x03,0xF0,0x02,
0x01,0x27,0xE4,0x93,0xA3,0xF8,0xE4,0x93,
0xA3,0x40,0x03,0xF6,0x80,0x01,0xF2,0x08,
0xDF,0xF4,0x80,0x29,0xE4,0x93,0xA3,0xF8,
0x54,0x07,0x24,0x0C,0xC8,0xC3,0x33,0xC4,
0x54,0x0F,0x44,0x20,0xC8,0x83,0x40,0x04,
0xF4,0x56,0x80,0x01,0x46,0xF6,0xDF,0xE4,
0x80,0x0B,0x01,0x02,0x04,0x08,0x10,0x20,
0x40,0x80,0x90,0x04,0xAF,0xE4,0x7E,0x01,
0x93,0x60,0xBC,0xA3,0xFF,0x54,0x3F,0x30,
0xE5,0x09,0x54,0x1F,0xFE,0xE4,0x93,0xA3,
0x60,0x01,0x0E,0xCF,0x54,0xC0,0x25,0xE0,
0x60,0xA8,0x40,0xB8,0xE4,0x93,0xA3,0xFA,
0xE4,0x93,0xA3,0xF8,0xE4,0x93,0xA3,0xC8,
0xC5,0x82,0xC8,0xCA,0xC5,0x83,0xCA,0xF0,
0xA3,0xC8,0xC5,0x82,0xC8,0xCA,0xC5,0x83,
0xCA,0xDF,0xE9,0xDE,0xE7,0x80,0xBE,0xC0,
0xE0,0xC0,0xF0,0xC0,0x83,0xC0,0x82,0xC0,
0xD0,0x75,0xD0,0x00,0xC0,0x00,0x78,0x17,
0xE6,0xF5,0x8C,0x78,0x18,0xE6,0xF5,0x8A,
0x90,0x06,0x29,0xE4,0x75,0xF0,0x01,0x12,
0x00,0x0E,0x90,0x06,0x2B,0xE4,0x75,0xF0,
0x01,0x12,0x00,0x0E,0xD0,0x00,0xD0,0xD0,
0xD0,0x82,0xD0,0x83,0xD0,0xF0,0xD0,0xE0,
0x32,0xC2,0xAF,0xAD,0x07,0xAC,0x06,0x8C,
0xA2,0x8D,0xA3,0x75,0xA0,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xAE,0xA1,0xBE,0x00,0xF0,0xAE,0xA6,
0xAF,0xA7,0xD2,0xAF,0x22,0xC2,0xAF,0xAB,
0x07,0xAA,0x06,0x8A,0xA2,0x8B,0xA3,0x8C,
0xA4,0x8D,0xA5,0x75,0xA0,0x03,0x00,0x00,
0x00,0xAA,0xA1,0xBA,0x00,0xF8,0xD2,0xAF,
0x22,0x42,0x06,0x2B,0x00,0x00,0x42,0x06,
0x29,0x00,0x00,0x00,0x12,0x04,0xC3,0x12,
0x02,0x3E,0x02,0x00,0x03,0xE4,0xF5,0x8E,
0x22};

int32 rtl8367c_smi_read(uint32 unit, rtk_port_t oob_port, uint32 mAddrs, uint32 *rData)
{
    if(mAddrs > 0xFFFF)
        return RT_ERR_INPUT;

    if(rData == NULL)
        return RT_ERR_NULL_POINTER;

    /* Write address control code to register 31 */
    MDC_MDIO_WRITE(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);
    osal_time_udelay(1);
    /* Write address to register 23 */
    MDC_MDIO_WRITE(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_ADDRESS_REG, mAddrs);
    osal_time_udelay(1);
    /* Write read control code to register 21 */
    MDC_MDIO_WRITE(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_CTRL1_REG, MDC_MDIO_READ_OP);
    osal_time_udelay(1);
    /* Read data from register 25 */
    MDC_MDIO_READ(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_DATA_READ_REG, rData);
    osal_time_udelay(1);
    return RT_ERR_OK;

}

int32 rtl8367c_smi_write(uint32 unit, rtk_port_t oob_port, uint32 mAddrs, uint32 rData)
{
    if(mAddrs > 0xFFFF)
        return RT_ERR_INPUT;

    if(rData > 0xFFFF)
        return RT_ERR_INPUT;

    /* Write address control code to register 31 */
    MDC_MDIO_WRITE(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);
    osal_time_udelay(1);
    /* Write address to register 23 */
    MDC_MDIO_WRITE(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_ADDRESS_REG, mAddrs);
    osal_time_udelay(1);
    /* Write data to register 24 */
    MDC_MDIO_WRITE(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_DATA_WRITE_REG, rData);
    osal_time_udelay(1);
    /* Write data control code to register 21 */
    MDC_MDIO_WRITE(unit, HWP_PORT_SMI(unit, oob_port), MDC_MDIO_PREAMBLE_LEN, MDC_MDIO_PHY_ID, MDC_MDIO_CTRL1_REG, MDC_MDIO_WRITE_OP);
    osal_time_udelay(1);
    return RT_ERR_OK;

}

/* Function Name:
 *      rtl8367c_getAsicRegBits
 * Description:
 *      Get bits value of a specified register
 * Input:
 *      reg     - register's address
 *      bits    - bits mask for setting
 *      value   - bits value for setting
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_INPUT    - Invalid input parameter
 * Note:
 *      None
 */
int32 rtl8367c_getAsicRegBits(uint32 unit, rtk_port_t oob_port, uint32 reg, uint32 bits, uint32 *pValue)
{
    uint32 regData;
    int32 retVal;
    uint32 bitsShift;

    if(bits>= (1<<RTL8367C_REGBITLENGTH) )
        return RT_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= RTL8367C_REGBITLENGTH)
            return RT_ERR_INPUT;
    }

    retVal = rtl8367c_smi_read(unit, oob_port, reg, &regData);
    if(retVal != RT_ERR_OK) return RT_ERR_SMI;

    *pValue = (regData & bits) >> bitsShift;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicRegBits
 * Description:
 *      Set bits value of a specified register
 * Input:
 *      reg     - register's address
 *      bits    - bits mask for setting
 *      value   - bits value for setting
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_INPUT    - Invalid input parameter
 * Note:
 *      Set bits of a specified register to value. Both bits and value are be treated as bit-mask
 */
int32 rtl8367c_setAsicRegBits(uint32 unit, rtk_port_t oob_port, uint32 reg, uint32 bits, uint32 value)
{
    uint32 regData;
    int32 retVal;
    uint32 bitsShift;
    uint32 valueShifted;

    if(bits >= (1 << RTL8367C_REGBITLENGTH) )
        return RT_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= RTL8367C_REGBITLENGTH)
            return RT_ERR_INPUT;
    }
    valueShifted = value << bitsShift;

    if(valueShifted > RTL8367C_REGDATAMAX)
    {
        return RT_ERR_INPUT;
    }

    retVal = rtl8367c_smi_read(unit, oob_port, reg, &regData);
    if(retVal != RT_ERR_OK)
    {
        return RT_ERR_SMI;
    }

    regData = regData & (~bits);
    regData = regData | (valueShifted & bits);

    retVal = rtl8367c_smi_write(unit, oob_port, reg, regData);
    if(retVal != RT_ERR_OK)
    {
        return RT_ERR_SMI;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getAsicRegBit
 * Description:
 *      Get a bit value of a specified register
 * Input:
 *      reg     - register's address
 *      bit     - bit location
 *      value   - value to get.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_INPUT    - Invalid input parameter
 * Note:
 *      None
 */
int32 rtl8367c_getAsicRegBit(uint32 unit, rtk_port_t oob_port, uint32 reg, uint32 bit, uint32 *pValue)
{

    uint32 regData;
    int32 retVal;

    retVal = rtl8367c_smi_read(unit, oob_port, reg, &regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    *pValue = (regData & (0x1 << bit)) >> bit;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicRegBit
 * Description:
 *      Set a bit value of a specified register
 * Input:
 *      reg     - register's address
 *      bit     - bit location
 *      value   - value to set. It can be value 0 or 1.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_INPUT    - Invalid input parameter
 * Note:
 *      Set a bit of a specified register to 1 or 0.
 */
int32 rtl8367c_setAsicRegBit(uint32 unit, rtk_port_t oob_port, uint32 reg, uint32 bit, uint32 value)
{
    uint32 regData;
    int32 retVal;

    if(bit >= RTL8367C_REGBITLENGTH)
        return RT_ERR_INPUT;

    retVal = rtl8367c_smi_read(unit, oob_port, reg, &regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    if(value)
        regData = regData | (1 << bit);
    else
        regData = regData & (~(1 << bit));

    retVal = rtl8367c_smi_write(unit, oob_port, reg, regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getAsicReg
 * Description:
 *      Get content of asic register
 * Input:
 *      reg     - register's address
 *      value   - Value setting to register
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 * Note:
 *      Value 0x0000 will be returned for ASIC un-mapping address
 */
int32 rtl8367c_getAsicReg(uint32 unit, rtk_port_t oob_port, uint32 reg, uint32 *pValue)
{
    uint32 regData;
    int32 retVal;

    retVal = rtl8367c_smi_read(unit, oob_port, reg, &regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    *pValue = regData;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicReg
 * Description:
 *      Set content of asic register
 * Input:
 *      reg     - register's address
 *      value   - Value setting to register
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 * Note:
 *      The value will be set to ASIC mapping address only and it is always return RT_ERR_OK while setting un-mapping address registers
 */
int32 rtl8367c_setAsicReg(uint32 unit, rtk_port_t oob_port, uint32 reg, uint32 value)
{
    int32 retVal;

    retVal = rtl8367c_smi_write(unit, oob_port, reg, value);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicPHYOCPReg
 * Description:
 *      Set PHY OCP registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      ocpAddr - OCP address
 *      ocpData - Writing data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PHY_REG_ID       - invalid PHY address
 *      RT_ERR_PHY_ID           - invalid PHY no
 *      RT_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
int32 rtl8367c_setAsicPHYOCPReg(uint32 unit, rtk_port_t oob_port, uint32 phyNo, uint32 ocpAddr, uint32 ocpData )
{
    int32 retVal;
    uint32 regAddr;
    uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;

    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);
    if((retVal = rtl8367c_setAsicRegBits(unit, oob_port, RTL8367C_REG_GPHY_OCP_MSB_0, RTL8367C_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != RT_ERR_OK)
        return retVal;

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);
    regAddr = RTL8367C_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << RTL8367C_PHY_OFFSET) | ocpAddr5_1;
    if((retVal = rtl8367c_setAsicReg(unit, oob_port, regAddr, ocpData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getAsicPHYOCPReg
 * Description:
 *      Get PHY OCP registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      ocpAddr - PHY address
 *      pRegData - read data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PHY_REG_ID       - invalid PHY address
 *      RT_ERR_PHY_ID           - invalid PHY no
 *      RT_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
int32 rtl8367c_getAsicPHYOCPReg(uint32 unit, rtk_port_t oob_port, uint32 phyNo, uint32 ocpAddr, uint32 *pRegData )
{
    int32 retVal;
    uint32 regAddr;
    uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;
    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);
    if((retVal = rtl8367c_setAsicRegBits(unit, oob_port, RTL8367C_REG_GPHY_OCP_MSB_0, RTL8367C_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != RT_ERR_OK)
        return retVal;

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);
    regAddr = RTL8367C_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << RTL8367C_PHY_OFFSET) | ocpAddr5_1;
    if((retVal = rtl8367c_getAsicReg(unit, oob_port, regAddr, pRegData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicPHYReg
 * Description:
 *      Set PHY registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      phyAddr - PHY address (0~31)
 *      phyData - Writing data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PHY_REG_ID       - invalid PHY address
 *      RT_ERR_PHY_ID           - invalid PHY no
 *      RT_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
int32 rtl8367c_setAsicPHYReg(uint32 unit, rtk_port_t oob_port, uint32 phyNo, uint32 phyAddr, uint32 phyData )
{
    uint32 ocp_addr;

    if(phyAddr > RTL8367C_PHY_REGNOMAX)
        return RT_ERR_PHY_REG_ID;

    ocp_addr = 0xa400 + phyAddr*2;

    return rtl8367c_setAsicPHYOCPReg(unit, oob_port, phyNo, ocp_addr, phyData);
}

/* Function Name:
 *      rtl8367c_getAsicPHYReg
 * Description:
 *      Get PHY registers
 * Input:
 *      phyNo   - Physical port number (0~7)
 *      phyAddr - PHY address (0~31)
 *      pRegData - Writing data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_PHY_REG_ID       - invalid PHY address
 *      RT_ERR_PHY_ID           - invalid PHY no
 *      RT_ERR_BUSYWAIT_TIMEOUT - PHY access busy
 * Note:
 *      None
 */
int32 rtl8367c_getAsicPHYReg(uint32 unit, rtk_port_t oob_port, uint32 phyNo, uint32 phyAddr, uint32 *pRegData )
{
    uint32 ocp_addr;

    if(phyAddr > RTL8367C_PHY_REGNOMAX)
    {
        return RT_ERR_PHY_REG_ID;
    }

    ocp_addr = 0xa400 + phyAddr*2;

    return rtl8367c_getAsicPHYOCPReg(unit, oob_port, phyNo, ocp_addr, pRegData);
}

/* Function Name:
 *      rtl8367c_setAsicPortEgressRateIfg
 * Description:
 *      Set per-port egress rate calculate include/exclude IFG
 * Input:
 *      ifg     - 1:include IFG 0:exclude IFG
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK   - Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
int32 rtl8367c_setAsicPortEgressRateIfg(uint32 unit, rtk_port_t oob_port, uint32 ifg)
{
    int32 retVal;

    retVal = rtl8367c_setAsicRegBit(unit, oob_port, RTL8367C_REG_SCHEDULE_WFQ_CTRL, RTL8367C_SCHEDULE_WFQ_CTRL_OFFSET, ifg);

    return retVal;
}

/* Function Name:
 *      rtl8367c_setAsicLutIpLookupMethod
 * Description:
 *      Set Lut IP lookup hash with DIP or {DIP,SIP} pair
 * Input:
 *      type - 1: When DIP can be found in IPMC_GROUP_TABLE, use DIP+SIP Hash, otherwise, use DIP+(SIP=0.0.0.0) Hash.
 *             0: When DIP can be found in IPMC_GROUP_TABLE, use DIP+(SIP=0.0.0.0) Hash, otherwise use DIP+SIP Hash.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK   - Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
int32 rtl8367c_setAsicLutIpLookupMethod(uint32 unit, rtk_port_t oob_port, uint32 type)
{
    return rtl8367c_setAsicRegBit(unit, oob_port, RTL8367C_REG_LUT_CFG, RTL8367C_LUT_IPMC_LOOKUP_OP_OFFSET, type);
}

/* Function Name:
 *      rtl8367c_setAsicRma
 * Description:
 *      Set reserved multicast address for CPU trapping
 * Input:
 *      index     - reserved multicast LSB byte, 0x00~0x2F is available value
 *      pRmacfg     - type of RMA for trapping frame type setting
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK         - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_RMA_ADDR - Invalid RMA address index
 * Note:
 *      None
 */
int32 rtl8367c_setAsicRma(uint32 unit, rtk_port_t oob_port, uint32 index, rtl8367c_rma_t* pRmacfg)
{
    uint32 regData = 0;
    int32 retVal;

    if(index > RTL8367C_RMAMAX)
        return RT_ERR_RMA_ADDR;

    regData |= (pRmacfg->portiso_leaky & 0x0001);
    regData |= ((pRmacfg->vlan_leaky & 0x0001) << 1);
    regData |= ((pRmacfg->keep_format & 0x0001) << 2);
    regData |= ((pRmacfg->trap_priority & 0x0007) << 3);
    regData |= ((pRmacfg->discard_storm_filter & 0x0001) << 6);
    regData |= ((pRmacfg->operation & 0x0003) << 7);

    if( (index >= 0x4 && index <= 0x7) || (index >= 0x9 && index <= 0x0C) || (0x0F == index))
        index = 0x04;
    else if((index >= 0x13 && index <= 0x17) || (0x19 == index) || (index >= 0x1B && index <= 0x1f))
        index = 0x13;
    else if(index >= 0x22 && index <= 0x2F)
        index = 0x22;

    retVal = rtl8367c_setAsicRegBits(unit, oob_port, RTL8367C_REG_RMA_CTRL00, RTL8367C_TRAP_PRIORITY_MASK, pRmacfg->trap_priority);
    if(retVal != RT_ERR_OK)
        return retVal;

    return rtl8367c_setAsicReg(unit, oob_port, RTL8367C_REG_RMA_CTRL00+index, regData);
}

/* Function Name:
 *      rtl8367c_setAsicPortMirrorIsolationTxLeaky
 * Description:
 *      Set the mirror function of Isolation TX leaky
 * Input:
 *      enabled     - 1: enabled, 0: disabled
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 * Note:
 *      None
 */
int32 rtl8367c_setAsicPortMirrorIsolationTxLeaky(uint32 unit, rtk_port_t oob_port, uint32 enabled)
{
    return rtl8367c_setAsicRegBit(unit, oob_port, RTL8367C_REG_MIRROR_CTRL2, RTL8367C_MIRROR_TX_ISOLATION_LEAKY_OFFSET, enabled);
}

/* Function Name:
 *      rtl8367c_getAiscSdsReg
 * Description:
 *      Get Serdes registers
 * Input:
 *      sdsId   - sdsid (0~1)
 *      sdsReg - reg address (0~31)
 *      sdsPage - Writing data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success

 * Note:
 *      None
 */
int32 rtl8367c_getAsicSdsReg(uint32 unit, rtk_port_t oob_port, uint32 sdsId, uint32 sdsReg, uint32 sdsPage, uint32 *value)
{
    uint32 retVal, busy;

    if( (retVal = rtl8367c_setAsicReg(unit, oob_port, RTL8367C_REG_SDS_INDACS_ADR, (sdsPage<<5) | sdsReg)) != RT_ERR_OK)
        return retVal;

    if( (retVal = rtl8367c_setAsicReg(unit, oob_port, RTL8367C_REG_SDS_INDACS_CMD, 0x0080|sdsId)) != RT_ERR_OK)
        return retVal;

    while(1)
    {
        if ((retVal = rtl8367c_getAsicReg(unit, oob_port, RTL8367C_REG_SDS_INDACS_CMD, &busy))!=RT_ERR_OK)
            return retVal;

        if ((busy & 0x100) == 0)
            break;
    }

    if ((retVal = rtl8367c_getAsicReg(unit, oob_port, RTL8367C_REG_SDS_INDACS_DATA, value))!=RT_ERR_OK)
            return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicSdsReg
 * Description:
 *      Set Serdes registers
 * Input:
 *      sdsId   - sdsid (0~1)
 *      sdsReg - reg address (0~31)
 *      sdsPage - Writing data
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success

 * Note:
 *      None
 */
int32 rtl8367c_setAsicSdsReg(uint32 unit, rtk_port_t oob_port, uint32 sdsId, uint32 sdsReg, uint32 sdsPage,  uint32 value)
{
    uint32 retVal;

    if( (retVal = rtl8367c_setAsicReg(unit, oob_port, RTL8367C_REG_SDS_INDACS_DATA, value)) != RT_ERR_OK)
        return retVal;

    if( (retVal = rtl8367c_setAsicReg(unit, oob_port, RTL8367C_REG_SDS_INDACS_ADR, (sdsPage<<5) | sdsReg)) != RT_ERR_OK)
        return retVal;

    if( (retVal = rtl8367c_setAsicReg(unit, oob_port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0|sdsId)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;

}

/* Function Name:
 *      rtk_switch_initialState_get
 * Description:
 *      Get initial status
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      INIT_COMPLETED     - Initialized
 *      INIT_NOT_COMPLETED - Uninitialized
 * Note:
 *
 */
init_state_t rtk_switch_initialState_get(void)
{
    return init_state;
}

/* Function Name:
 *      rtk_switch_logicalPortCheck
 * Description:
 *      Check logical port ID.
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is correct
 *      RT_ERR_FAILED   - Port ID is not correct
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
int32 rtk_switch_logicalPortCheck(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if(halCtrl->l2p_port[logicalPort] == 0xFF)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_switch_isUtpPort
 * Description:
 *      Check is logical port a UTP port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a UTP port
 *      RT_ERR_FAILED   - Port ID is not a UTP port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
int32 rtk_switch_isUtpPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
    {
        return RT_ERR_NOT_INIT;
    }

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
    {
        return RT_ERR_FAILED;
    }



    if(halCtrl->log_port_type[logicalPort] == UTP_PORT)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

/* Function Name:
 *      rtk_switch_isExtPort
 * Description:
 *      Check is logical port a Extension port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a EXT port
 *      RT_ERR_FAILED   - Port ID is not a EXT port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
int32 rtk_switch_isExtPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if(halCtrl->log_port_type[logicalPort] == EXT_PORT)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

/* Function Name:
 *      rtk_switch_isHsgPort
 * Description:
 *      Check is logical port a HSG port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a HSG port
 *      RT_ERR_FAILED   - Port ID is not a HSG port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
int32 rtk_switch_isHsgPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if( ((0x01 << logicalPort) & halCtrl->hsg_logical_portmask) != 0)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

/* Function Name:
 *      rtk_switch_isSgmiiPort
 * Description:
 *      Check is logical port a SGMII port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a SGMII port
 *      RT_ERR_FAILED   - Port ID is not a SGMII port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
int32 rtk_switch_isSgmiiPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if( ((0x01 << logicalPort) & halCtrl->sg_logical_portmask) != 0)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

/* Function Name:
 *      rtk_switch_isCPUPort
 * Description:
 *      Check is logical port a CPU port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a CPU port
 *      RT_ERR_FAILED   - Port ID is not a CPU port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
int32 rtk_switch_isCPUPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if( ((0x01 << logicalPort) & halCtrl->valid_cpu_portmask) != 0)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

/* Function Name:
 *      rtk_switch_isPtpPort
 * Description:
 *      Check is logical port a PTP port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Port ID is a PTP port
 *      RT_ERR_FAILED   - Port ID is not a PTP port
 *      RT_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
int32 rtk_switch_isPtpPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if(halCtrl->ptp_port[logicalPort] == 1)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

/* Function Name:
 *      rtk_switch_port_L2P_get
 * Description:
 *      Get physical port ID
 * Input:
 *      logicalPort       - logical port ID
 * Output:
 *      None
 * Return:
 *      Physical port ID
 * Note:
 *
 */
int32 rtk_switch_port_L2P_get(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return UNDEFINE_PHY_PORT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return UNDEFINE_PHY_PORT;

    return (halCtrl->l2p_port[logicalPort]);
}

int32 rtl8367c_setAsicPortIngressBandwidth(uint32 unit, rtk_port_t oob_port, uint32 port, uint32 bandwidth, uint32 preifg, uint32 enableFC)
{
    int32 retVal;
    uint32 regData;
    uint32 regAddr;

    /* Invalid input parameter */
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(bandwidth > RTL8367C_QOS_GRANULARTY_MAX)
        return RT_ERR_OUT_OF_RANGE;

    regAddr = RTL8367C_INGRESSBW_PORT_RATE_LSB_REG(port);
    regData = bandwidth & RTL8367C_QOS_GRANULARTY_LSB_MASK;
    retVal = rtl8367c_setAsicReg(unit, oob_port, regAddr, regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr += 1;
    regData = (bandwidth & RTL8367C_QOS_GRANULARTY_MSB_MASK) >> RTL8367C_QOS_GRANULARTY_MSB_OFFSET;
    retVal = rtl8367c_setAsicRegBits(unit, oob_port, regAddr, RTL8367C_INGRESSBW_PORT0_RATE_CTRL1_INGRESSBW_RATE16_MASK, regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_PORT_MISC_CFG_REG(port);
    retVal = rtl8367c_setAsicRegBit(unit, oob_port, regAddr, RTL8367C_PORT0_MISC_CFG_INGRESSBW_IFG_OFFSET, preifg);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_PORT_MISC_CFG_REG(port);
    retVal = rtl8367c_setAsicRegBit(unit, oob_port, regAddr, RTL8367C_PORT0_MISC_CFG_INGRESSBW_FLOWCTRL_OFFSET, enableFC);
    if(retVal != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicPortEgressRate
 * Description:
 *      Set per-port egress rate
 * Input:
 *      port        - Physical port number (0~10)
 *      rate        - Egress rate
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number
 *      RT_ERR_QOS_EBW_RATE - Invalid bandwidth/rate
 * Note:
 *      None
 */
int32 rtl8367c_setAsicPortEgressRate(uint32 unit, rtk_port_t oob_port, uint32 port, uint32 rate)
{
    int32 retVal;
    uint32 regAddr, regData;

    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    if(rate > RTL8367C_QOS_GRANULARTY_MAX)
        return RT_ERR_QOS_EBW_RATE;

    regAddr = RTL8367C_PORT_EGRESSBW_LSB_REG(port);
    regData = RTL8367C_QOS_GRANULARTY_LSB_MASK & rate;

    retVal = rtl8367c_setAsicReg(unit, oob_port, regAddr, regData);

    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_PORT_EGRESSBW_MSB_REG(port);
    regData = (RTL8367C_QOS_GRANULARTY_MSB_MASK & rate) >> RTL8367C_QOS_GRANULARTY_MSB_OFFSET;

    retVal = rtl8367c_setAsicRegBits(unit, oob_port, regAddr, RTL8367C_PORT6_EGRESSBW_CTRL1_MASK, regData);

    return retVal;
}

/*
@func uint32 | rtl8367c_getAsicEee100M | Get 100M eee enable/disable.
@parm uint32 | port | The port number.
@parm uint32* | enabled | 1: enabled, 0: disabled.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@rvalue RT_ERR_INPUT | Invalid input parameter.
@comm
    This API get the 100M EEE function.
*/
int32 rtl8367c_getAsicEee100M(uint32 unit, rtk_port_t oob_port, uint32 port, uint32 *enable)
{
    int32   retVal;
    uint32  regData;

    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if((retVal = rtl8367c_getAsicPHYOCPReg(unit, oob_port, port, EEE_OCP_PHY_ADDR, &regData)) != RT_ERR_OK)
        return retVal;

    *enable = (regData & (0x0001 << 1)) ? ENABLED : DISABLED;
    return RT_ERR_OK;
}

/*
@func uint32 | rtl8367c_setAsicEee100M | Set eee force mode function enable/disable.
@parm uint32 | port | The port number.
@parm uint32 | enabled | 1: enabled, 0: disabled.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@rvalue RT_ERR_INPUT | Invalid input parameter.
@comm
    This API set the 100M EEE enable function.

*/
int32 rtl8367c_setAsicEee100M(uint32 unit, rtk_port_t oob_port, uint32 port, uint32 enable)
{
    int32   retVal;
    uint32  regData;

    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if (enable > 1)
        return RT_ERR_INPUT;

    if((retVal = rtl8367c_getAsicPHYOCPReg(unit, oob_port, port, EEE_OCP_PHY_ADDR, &regData)) != RT_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 1);
    else
        regData &= ~(0x0001 << 1);

    if((retVal = rtl8367c_setAsicPHYOCPReg(unit, oob_port, port, EEE_OCP_PHY_ADDR, regData)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, oob_port, RTL8367C_PORT_EEE_CFG_REG(port), &regData)) != RT_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 11);
    else
        regData &= ~(0x0001 << 11);

    if((retVal = rtl8367c_setAsicReg(unit, oob_port, RTL8367C_PORT_EEE_CFG_REG(port),regData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/*
@func uint32 | rtl8367c_setAsicEeeGiga | Set eee force mode function enable/disable.
@parm uint32 | port | The port number.
@parm uint32 | enabled | 1: enabled, 0: disabled.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@rvalue RT_ERR_INPUT | Invalid input parameter.
@comm
    This API set the 100M EEE enable function.

*/
int32 rtl8367c_setAsicEeeGiga(uint32 unit, rtk_port_t oob_port, uint32 port, uint32 enable)
{
    int32   retVal;
    uint32   regData;

    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if (enable > 1)
        return RT_ERR_INPUT;

    if((retVal = rtl8367c_getAsicPHYOCPReg(unit, oob_port, port, EEE_OCP_PHY_ADDR, &regData)) != RT_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 2);
    else
        regData &= ~(0x0001 << 2);

    if((retVal = rtl8367c_setAsicPHYOCPReg(unit, oob_port, port, EEE_OCP_PHY_ADDR, regData)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, oob_port, RTL8367C_PORT_EEE_CFG_REG(port), &regData)) != RT_ERR_OK)
        return retVal;

    if(enable)
        regData |= (0x0001 << 10);
    else
        regData &= ~(0x0001 << 10);

    if((retVal = rtl8367c_setAsicReg(unit, oob_port, RTL8367C_PORT_EEE_CFG_REG(port),regData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/*
@func uint32 | rtl8367c_getAsicEeeGiga | Get 100M eee enable/disable.
@parm uint32 | port | The port number.
@parm uint32* | enabled | 1: enabled, 0: disabled.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@rvalue RT_ERR_INPUT | Invalid input parameter.
@comm
    This API get the 100M EEE function.
*/
int32 rtl8367c_getAsicEeeGiga(uint32 unit, rtk_port_t oob_port, uint32 port, uint32 *enable)
{
    int32   retVal;
    uint32  regData;

    if(port >= RTL8367C_PORTNO)
    {
        return RT_ERR_PORT_ID;
    }

    if((retVal = rtl8367c_getAsicPHYOCPReg(unit, oob_port, port, EEE_OCP_PHY_ADDR, &regData)) != RT_ERR_OK)
        return retVal;

    *enable = (regData & (0x0001 << 2)) ? ENABLED : DISABLED;
    return RT_ERR_OK;
}

int32 phy_8213b_AutoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable){
    int32   retVal;
    uint32  phyData;

    /* get value from CHIP*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    if (phyData & (1 << 12))
    {
        *pEnable = ENABLED;
    }
    else
    {
        *pEnable = DISABLED;
    }

    return RT_ERR_OK;
}

int32 phy_8213b_AutoNegoEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable){
    int32       retVal;
    uint32      phyData;
    uint32      phyEnMsk0;
    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* get value from CHIP*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    phyEnMsk0 = 0;
    phyEnMsk0 = ((enable << AutoNegotiationEnable_OFFSET) | (1 << RestartAutoNegotiation_OFFSET));
    phyData = (phyData & ~(AutoNegotiationEnable_MASK | RestartAutoNegotiation_MASK)) | phyEnMsk0;

    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

int32 phy_8213b_AutoNegoAbility_get( uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility){
    int32       retVal;
    uint32      phyData0;
    uint32      phyData4;
    uint32      phyData9;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();


    if(NULL == pAbility)
        return RT_ERR_NULL_POINTER;


    /*Control register setting and restart auto*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return retVal;

    /*Auto-Negotiation control register setting*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_AN_ADVERTISEMENT_REG, &phyData4)) != RT_ERR_OK)
        return retVal;

    /*1000 BASE-T control register setting*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_1000_BASET_CONTROL_REG, &phyData9)) != RT_ERR_OK)
        return retVal;

    if (phyData9 & (1 << 9))
        pAbility->Full_1000 = 1;
    else
        pAbility->Full_1000 = 0;

    if (phyData4 & (1 << 11))
        pAbility->AsyFC = 1;
    else
        pAbility->AsyFC = 0;

    if (phyData4 & (1 << 10))
        pAbility->FC = 1;
    else
        pAbility->FC = 0;


    if (phyData4 & (1 << 8))
        pAbility->Full_100 = 1;
    else
        pAbility->Full_100 = 0;

    if (phyData4 & (1 << 7))
        pAbility->Half_100 = 1;
    else
        pAbility->Half_100 = 0;

    if (phyData4 & (1 << 6))
        pAbility->Full_10 = 1;
    else
        pAbility->Full_10 = 0;

    if (phyData4 & (1 << 5))
        pAbility->Half_10 = 1;
    else
        pAbility->Half_10 = 0;

    return RT_ERR_OK;
}

int32 phy_8213b_AutoNegoAbility_set( uint32 unit, rtk_port_t port, rtk_port_phy_ability_t *pAbility){
    int32           retVal;
    uint32          phyData;
    uint32          phyEnMsk0;
    uint32          phyEnMsk4;
    uint32          phyEnMsk9;
    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if(NULL == pAbility)
        return RT_ERR_NULL_POINTER;

    phyEnMsk0 = 0;
    phyEnMsk4 = 0;
    phyEnMsk9 = 0;

    if (1 == pAbility->Half_10)
    {
        /*10BASE-TX half duplex capable in reg 4.5*/
        phyEnMsk4 = phyEnMsk4 | (1 << 5);

        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));
    }

    if (1 == pAbility->Full_10)
    {
        /*10BASE-TX full duplex capable in reg 4.6*/
        phyEnMsk4 = phyEnMsk4 | (1 << 6);
        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));

        /*Full duplex mode in reg 0.8*/
        phyEnMsk0 = phyEnMsk0 | (1 << 8);

    }

    if (1 == pAbility->Half_100)
    {
        /*100BASE-TX half duplex capable in reg 4.7*/
        phyEnMsk4 = phyEnMsk4 | (1 << 7);
        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 | (1 << 13);
    }


    if (1 == pAbility->Full_100)
    {
        /*100BASE-TX full duplex capable in reg 4.8*/
        phyEnMsk4 = phyEnMsk4 | (1 << 8);
        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 | (1 << 13);
        /*Full duplex mode in reg 0.8*/
        phyEnMsk0 = phyEnMsk0 | (1 << 8);
    }


    if (1 == pAbility->Full_1000)
    {
        /*1000 BASE-T FULL duplex capable setting in reg 9.9*/
        phyEnMsk9 = phyEnMsk9 | (1 << 9);

        /*Speed selection [1:0] */
        /* 11=Reserved*/
        /* 10= 1000Mpbs*/
        /* 01= 100Mpbs*/
        /* 00= 10Mpbs*/
        phyEnMsk0 = phyEnMsk0 | (1 << 6);
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));


        /*Auto-Negotiation setting in reg 0.12*/
        phyEnMsk0 = phyEnMsk0 | (1 << 12);

     }


    /*Auto-Negotiation setting in reg 0.12*/
    phyEnMsk0 = phyEnMsk0 | (1 << 12);


    if (1 == pAbility->AsyFC)
    {
        /*Asymetric flow control in reg 4.11*/
        phyEnMsk4 = phyEnMsk4 | (1 << 11);
    }
    if (1 == pAbility->FC)
    {
        /*Flow control in reg 4.10*/
        phyEnMsk4 = phyEnMsk4 | (1 << 10);
    }

    /*1000 BASE-T control register setting*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_1000_BASET_CONTROL_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x0200)) | phyEnMsk9 ;

    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_1000_BASET_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;

    /*Auto-Negotiation control register setting*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_AN_ADVERTISEMENT_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_AN_ADVERTISEMENT_REG, phyData)) != RT_ERR_OK)
        return retVal;

    /*Control register setting and restart auto*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x3140)) | phyEnMsk0;
    /*If have auto-negotiation capable, then restart auto negotiation*/
    phyData = phyData | (1 << 9);

    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

int32 phy_8213b_Enable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable){
    int32     retVal;
    uint32    data;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, &data)) != RT_ERR_OK)
        return retVal;

    if ( (data & 0x0800) == 0x0800)
    {
        *pEnable = DISABLED;
    }
    else
    {
        *pEnable = ENABLED;
    }

    return RT_ERR_OK;
}

int32 phy_8213b_Enable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable){
    int32     retVal;
    uint32     data;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if (enable >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, &data)) != RT_ERR_OK)
        return retVal;

    if (ENABLED == enable)
    {
        data &= 0xF7FF;
        data |= 0x0200;
    }
    else if (DISABLED == enable)
    {
        data |= 0x0800;
    }

    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, data)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

int32 phy_8213b_Speed_get(uint32 unit, rtk_port_t port, rtk_port_speed_t *pSpeed){
    int32     retVal;
    uint32    phyData;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if((NULL == pSpeed))
        return RT_ERR_NULL_POINTER;

    /*Get PHY resolved register*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_RESOLVED_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    /*check link status*/
    if (phyData & (1<<2))
    {
        /*check link speed*/
        *pSpeed = (phyData&0x0030) >> 4;
    }
    else
    {
        *pSpeed = 0;
    }

    return RT_ERR_OK;
}

int32 phy_8213b_Speed_set(uint32 unit, rtk_port_t port, rtk_port_speed_t speed){
    int32          retVal;
    uint32         phyData;
    uint32         phyEnMsk0;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    phyEnMsk0 = 0;

    /*Speed selection [1:0] */
    /* 11=Reserved*/
    /* 10= 1000Mpbs*/
    /* 01= 100Mpbs*/
    /* 00= 10Mpbs*/
    switch (speed)
    {
    case PORT_SPEED_10M:
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));
        break;
    case PORT_SPEED_100M:
        phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
        phyEnMsk0 = phyEnMsk0 | (1 << 13);
        break;
    case PORT_SPEED_1000M:
        phyEnMsk0 = phyEnMsk0 | (1 << 6);
        phyEnMsk0 = phyEnMsk0 & (~(1 << 13));
    default:
        break;
    }

    /*Control register setting and power off/on*/
    phyData = phyEnMsk0 & (~(1 << 12));
    phyData |= (1 << 11);   /* power down PHY, bit 11 should be set to 1 */
    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;

    phyData = phyData & (~(1 << 11));   /* power on PHY, bit 11 should be set to 0*/
    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

int32 phy_8213b_Duplex_get(uint32 unit, rtk_port_t port, rtk_port_duplex_t *pDuplex){
    int32     retVal;
    uint32    phyData;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if((NULL == pDuplex) )
        return RT_ERR_NULL_POINTER;

    /*Get PHY resolved register*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_RESOLVED_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    /*check link status*/
    if (phyData & (1<<2))
    {
        /*check link duplex*/
        *pDuplex = (phyData&0x0008) >> 3;
    }
    else
    {
        *pDuplex = PORT_HALF_DUPLEX;
    }

    return RT_ERR_OK;
}

int32 phy_8213b_Duplex_set(uint32 unit, rtk_port_t port, rtk_port_duplex_t duplex){
    int32          retVal;
    uint32         phyData;
    uint32         phyEnMsk0;
    uint32         phyEnMsk4;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    phyEnMsk0 = 0;
    phyEnMsk4 = 0;

    switch (duplex)
    {
    case PORT_HALF_DUPLEX:
        /*half duplex capable in reg 4.5*/
        phyEnMsk4 = phyEnMsk4 | (1 << 5);
        break;
    case PORT_FULL_DUPLEX:
        /*full duplex capable in reg 4.6*/
        phyEnMsk4 = phyEnMsk4 | (1 << 6);
        /*Full duplex mode in reg 0.8*/
        phyEnMsk0 = phyEnMsk0 | (1 << 8);
        break;
    default:
        break;
    }

    /*Auto-Negotiation control register setting*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_AN_ADVERTISEMENT_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_AN_ADVERTISEMENT_REG, phyData)) != RT_ERR_OK)
        return retVal;
    /*Control register setting and power off/on*/
    phyData = phyEnMsk0 & (~(1 << 12));
    phyData |= (1 << 11);   /* power down PHY, bit 11 should be set to 1 */
    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;
    phyData = phyData & (~(1 << 11));   /* power on PHY, bit 11 should be set to 0*/
    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

int32 phy_8213b_Link_get(uint32 unit, rtk_port_t port, rtk_port_linkStatus_t *pLinkStatus){
    int32     retVal;
    uint32    phyData;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if( (NULL == pLinkStatus))
        return RT_ERR_NULL_POINTER;

    /*Get PHY resolved register*/
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, rtk_switch_port_L2P_get(UTP_PORT3), PHY_RESOLVED_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    /*check link status*/
    if (phyData & (1<<2))
    {
        *pLinkStatus = 1;
    }
    else
    {
        *pLinkStatus = 0;
    }

    return RT_ERR_OK;
}

int32 phy_8213b_EeeEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable){
    int32     retVal;
    uint32    regData1, regData2;
    uint32    phy_port;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    phy_port = rtk_switch_port_L2P_get(UTP_PORT3);

    if ((retVal = rtl8367c_getAsicEee100M(unit, port, phy_port,&regData1))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367c_getAsicEeeGiga(unit, port, phy_port,&regData2))!=RT_ERR_OK)
        return retVal;

    if (regData1==1&&regData2==1)
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return RT_ERR_OK;
}

int32 phy_8213b_EeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable){
    int32       retVal;
    uint32      regData;
    uint32      phy_port;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if (enable>=RTK_ENABLE_END)
        return RT_ERR_INPUT;

    phy_port = rtk_switch_port_L2P_get(UTP_PORT3);

    if ((retVal = rtl8367c_setAsicEee100M(unit, port, phy_port,enable))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367c_setAsicEeeGiga(unit, port, phy_port,enable))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, phy_port, RTL8367C_PHY_PAGE_ADDRESS, 0))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtl8367c_getAsicPHYReg(unit, port, phy_port, 0, &regData))!=RT_ERR_OK)
        return retVal;
    regData |= 0x0200;
    if ((retVal = rtl8367c_setAsicPHYReg(unit, port, phy_port, 0, regData))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

int32
_phy_8367c_chipId_chk(uint32 unit, rtk_port_t port, uint32 model_id)
{
    int32 retVal;
    uint32 chip_id = 0, chip_ver = 0, reg_model_id = 0;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1300, &chip_id)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1301, &chip_ver)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    reg_model_id = (chip_ver & RTL8367C_MODEL_ID_MASK) >> RTL8367C_MODEL_ID_OFFSET;
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:unit=%u port=%u chipID=%X chip_ver=%X reg_model_id=%x\n", __FUNCTION__, __LINE__, unit, port, chip_id, chip_ver, reg_model_id);
    if (chip_id == 0x6511 && reg_model_id == model_id)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

int32 phy_8213b_Identify(uint32 unit, rtk_port_t port, uint32 model_id, uint32 rev_id)
{
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:unit=%u port=%u\n", __FUNCTION__, __LINE__, unit, port);

    if (_phy_8367c_chipId_chk(unit, port, RTL8213B_MODEL_ID) == RT_ERR_OK)
    {
        if (halCtrl == NULL)
        {
            halCtrl = &rtl8213b_hal_Ctrl;
        }
        return RT_ERR_OK;
    }

    return RT_ERR_FAILED;
}

int32 phy_8363sc_vb_Identify(uint32 unit, rtk_port_t port, uint32 model_id, uint32 rev_id)
{
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:unit=%u port=%u\n", __FUNCTION__, __LINE__, unit, port);

    if (_phy_8367c_chipId_chk(unit, port, RTL8363SC_VB_MODEL_ID) == RT_ERR_OK)
    {
        if (halCtrl == NULL)
        {
            halCtrl = &rtl8363sc_vb_hal_Ctrl;
        }
        return RT_ERR_OK;
    }

    return RT_ERR_FAILED;
}

int32 phy_8213b_Patch_set(uint32 unit, rtk_port_t port){
    return 0;
}

int32 phy_8213b_IntrStatus_get(uint32 unit, rtk_port_t port, rtk_phy_intr_t phyIntr, rtk_phy_intrStatusVal_t *pStatus){
    return 0;
}

int32 phy_8213b_IntrMask_get(uint32 unit, rtk_port_t port, rtk_phy_intr_t phyIntr, uint32 *pMask){
    return 0;
}

int32 phy_8213b_IntrMask_set(uint32 unit, rtk_port_t port, rtk_phy_intr_t phyIntr, uint32 mask){
    return 0;
}

int32 phy_8213b_phyInfo_get(drv_oob_phyInfo_t *pPhyInfo)
{
    osal_memcpy(pPhyInfo, &oob_phy_rtl8213b_info, sizeof(drv_oob_phyInfo_t));
    return RT_ERR_OK;
}

static int32
_phy_8213b_led_cfg(uint32 unit, rtk_port_t port, uint32 led, uint32 led_cfg)
{
    int32 retVal;
    uint32  data, val, link_only;
    uint32  led_cfg_offset[] = {RTL8367C_LED0_CFG_OFFSET, RTL8367C_LED1_CFG_OFFSET, RTL8367C_LED2_CFG_OFFSET}; /* RTL8367C_REG_LED_CONFIGURATION */
    uint32  led_cfg_mask[] = {RTL8367C_LED0_CFG_MASK, RTL8367C_LED1_CFG_MASK, RTL8367C_LED2_CFG_MASK};         /* RTL8367C_REG_LED_CONFIGURATION */
    uint32  led_data_ctrl_reg[] = {RTL8367C_REG_LED0_DATA_CTRL, RTL8367C_REG_LED1_DATA_CTRL, RTL8367C_REG_LED2_DATA_CTRL};


    RT_LOG(LOG_DEBUG, MOD_OOB, "%s,%u: unit=%u port=%u, led %u led_cfg=0x%x\n", __FUNCTION__, __LINE__, unit, port, led, led_cfg);

    link_only = FALSE;
    if (led_cfg == (RTK_PHY_CTRL_LED_MODE_1G | RTK_PHY_CTRL_LED_MODE_100M | RTK_PHY_CTRL_LED_MODE_10M))
    {
        /* as intr pin when connecting to 9311B OOB */
        //0010:Link/Act
        val = 0x2;
        link_only = TRUE;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_1G | RTK_PHY_CTRL_LED_MODE_100M | RTK_PHY_CTRL_LED_MODE_10M | RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //0010:Link/Act
        val = 0x2;
    }
    else if (led_cfg == RTK_PHY_CTRL_LED_MODE_DUPLEX)
    {
        //0001:Dup/Col
        val = 0x1;
    }
    else if (led_cfg == RTK_PHY_CTRL_LED_MODE_1G)
    {
        //0011:Spd1000
        val = 0x3;
    }
    else if (led_cfg == RTK_PHY_CTRL_LED_MODE_100M)
    {
        //0100:Spd100
        val = 0x4;
    }
    else if (led_cfg == RTK_PHY_CTRL_LED_MODE_10M)
    {
        //0101:Spd10
        val = 0x5;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_1G | RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //0110:Spd1000/Act
        val = 0x6;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_100M | RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //0111:Spd100/Act
        val = 0x7;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_10M | RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //1000:Spd10/Act
        val = 0x8;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_100M | RTK_PHY_CTRL_LED_MODE_10M | RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //1001:Spd100(10)/Act
        val = 0x9;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_1G | RTK_PHY_CTRL_LED_MODE_100M | RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //1100:Spd1000(100)/Act
        val = 0xC;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_1G | RTK_PHY_CTRL_LED_MODE_10M | RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //1101:Spd1000(10)/Act
        val = 0xD;
    }
    else if (led_cfg == RTK_PHY_CTRL_LED_MODE_MASTER)
    {
        //1110:Master
        val = 0xE;
    }
    else if (led_cfg == (RTK_PHY_CTRL_LED_MODE_TX_ACT | RTK_PHY_CTRL_LED_MODE_RX_ACT))
    {
        //1111:1'b0/Act
        val = 0xF;
    }
    else if (led_cfg == 0)
    {
        //LED off
        val = 0x0;
    }
    else
    {
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s,%u: unit=%u port=%u, led %u FAIL!\n", __FUNCTION__, __LINE__, unit, port, led);
        return RT_ERR_FAILED;
    }

    /* LED config */
    if((retVal = rtl8367c_getAsicReg(unit, port, RTL8367C_REG_LED_CONFIGURATION, &data)) != RT_ERR_OK)
        return retVal;
    data = (data & ~led_cfg_mask[led]) | (val << led_cfg_offset[led]);      /* Link/Act */
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s,%u: unit=%u port=%u, led %u val=0x%x reg %x data=0x%x (link_only=%u)\n", __FUNCTION__, __LINE__, unit, port,
            led, val, RTL8367C_REG_LED_CONFIGURATION, data, link_only);
    if((retVal = rtl8367c_getAsicReg(unit, port, RTL8367C_REG_LED_CONFIGURATION, &data)) != RT_ERR_OK)
        return retVal;


    /* LEDn Link/Act set to Link Only (as for intr-pin) */
    if((retVal = rtl8367c_getAsicReg(unit, port, led_data_ctrl_reg[led], &data)) != RT_ERR_OK)
        return retVal;
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s,%u: unit=%u port=%u, led %u led_cfg=0x%x, reg:0x%x data=0x%x\n", __FUNCTION__, __LINE__, unit, port,
            led, led_cfg, led_data_ctrl_reg[led], data);
    data = (data & ~RTL8367C_CFG_DATA_LED0_SEL_MASK) | (1 << RTL8367C_CFG_DATA_LED0_SEL_OFFSET);
    data = (data & ~RTL8367C_CFG_DATA_LED0_ACT_MASK) | (0 << RTL8367C_CFG_DATA_LED0_ACT_OFFSET);
    data = (data & ~RTL8367C_CFG_DATA_LED0_SPD_MASK) | (0xF << RTL8367C_CFG_DATA_LED0_SPD_OFFSET);
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s,%u: unit=%u port=%u, data set 0x%x\n", __FUNCTION__, __LINE__, unit, port, data);

   if((retVal = rtl8367c_setAsicReg(unit, port, led_data_ctrl_reg[led], data)) != RT_ERR_OK)
       return retVal;

    return RT_ERR_OK;

}

static int32
_phy_8213b_led_init(uint32 unit, rtk_port_t port)
{
    uint32 mode;
    uint32 led_cfg;

    if ((mode = HWP_LED_COPR_SET(unit, port)) != HWP_NONE)
    {
        led_cfg = HWP_LED_MOD(unit, mode, 0);
        _phy_8213b_led_cfg(unit, port, 0, led_cfg);
        led_cfg = HWP_LED_MOD(unit, mode, 1);
        _phy_8213b_led_cfg(unit, port, 1, led_cfg);
        led_cfg = HWP_LED_MOD(unit, mode, 2);
        _phy_8213b_led_cfg(unit, port, 2, led_cfg);
    }
    else
    {
        /* initial a default */
        _phy_8213b_led_cfg(unit, port, 0, (RTK_PHY_CTRL_LED_MODE_1G | RTK_PHY_CTRL_LED_MODE_100M | RTK_PHY_CTRL_LED_MODE_10M)); /* Link */
        _phy_8213b_led_cfg(unit, port, 1, RTK_PHY_CTRL_LED_MODE_1G);
        _phy_8213b_led_cfg(unit, port, 2, RTK_PHY_CTRL_LED_MODE_100M);
    }

#if 0 /* debug get register */
    {
        uint32 data, data0, data1, data2;
        data = data0 = data1 = data2 = 0;
        rtl8367c_getAsicReg(unit, port, RTL8367C_REG_LED_CONFIGURATION, &data);
        rtl8367c_getAsicReg(unit, port, RTL8367C_REG_LED0_DATA_CTRL, &data0);
        rtl8367c_getAsicReg(unit, port, RTL8367C_REG_LED1_DATA_CTRL, &data1);
        rtl8367c_getAsicReg(unit, port, RTL8367C_REG_LED2_DATA_CTRL, &data2);
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s,%u: unit=%u port=%u, LED_CFG=%x LED0=%x LED1=%x LED2=%x\n", __FUNCTION__, __LINE__, unit, port,
                data, data0, data1, data2);
    }
#endif

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicPortForceLinkExt
 * Description:
 *      Set external interface force linking configuration
 * Input:
 *      id          - external interface id (0~2)
 *      portAbility - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
int32 rtl8367c_setAsicPortForceLinkExt(uint32 unit, rtk_port_t port, uint32 id, rtl8367c_port_ability_t *pPortAbility)
{
    uint32 retVal, regValue, regValue2, type, sgmiibit, hisgmiibit;
    uint32 reg_data = 0;
    uint32 i = 0;

    reg_data |= pPortAbility->forcemode << 12;
    reg_data |= pPortAbility->mstfault << 9;
    reg_data |= pPortAbility->mstmode << 8;
    reg_data |= pPortAbility->nway << 7;
    reg_data |= pPortAbility->txpause << 6;
    reg_data |= pPortAbility->rxpause << 5;
    reg_data |= pPortAbility->link << 4;
    reg_data |= pPortAbility->duplex << 2;
    reg_data |= pPortAbility->speed;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;
    /*get chip ID */
    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if (1 == type)
    {
        if(1 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(unit, port, RTL8367C_REG_REG_TO_ECO4, &regValue)) != RT_ERR_OK)
                return retVal;

            if((regValue & (0x0001 << 5)) && (regValue & (0x0001 << 7)))
            {
                return RT_ERR_OK;
            }

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;
        }

        if(0 == id || 1 == id)
            return rtl8367c_setAsicReg(unit, port, RTL8367C_REG_DIGITAL_INTERFACE0_FORCE + id, reg_data);
        else
            return rtl8367c_setAsicReg(unit, port, RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, reg_data);
    }
    else if (2 == type)
    {
        if (1 == id)
        {
             if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 2, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1311, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 6, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                return retVal;

            if (pPortAbility->link == 1)
            {
                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, 0)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1311, 0x3, 2)) != RT_ERR_OK)
                    return retVal;
            }


            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;
        }
        else if (2 == id)
        {
            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 2, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c4, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 4, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 6, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                return retVal;

            if (pPortAbility->link == 1)
            {
                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 4, 0)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 4, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c4, 0x3, 2)) != RT_ERR_OK)
                    return retVal;
            }

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1dc1, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1dc1, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1dc1, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1dc1, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1dc1, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;
        }

    }
    else if(3 == type)
    {
        if(1 == id)
        {
            if((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d11, 6, &sgmiibit)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d11, 11, &hisgmiibit)) != RT_ERR_OK)
                return retVal;

            if ((sgmiibit == 1) || (hisgmiibit == 1))
            {
                /*for 1000x/100fx/1000x_100fx, param has to be set to serdes registers*/
                if((retVal = rtl8367c_getAsicReg(unit, port, 0x1d41, &regValue)) != RT_ERR_OK)
                    return retVal;


                if((regValue & 0xa0) == 0xa0)
                {

                    if((retVal = rtl8367c_getAsicRegBits(unit, port, 0x1d95, 0x1f00, &regValue2)) != RT_ERR_OK)
                        return retVal;

                     /*1000X*/
                    if(regValue2 == 0x4)
                    {
#if 0
                        /* new_cfg_sds_mode:reset mode */
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                            return retVal;
#endif
                        /* Enable new sds mode config */
                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0x9000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFDFFF;
                        reg_data |= 0x40;
                        if(pPortAbility->forcemode)
                            reg_data &= 0xffffefff;
                        else
                            reg_data |= 0x1000;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;

                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data &= (~0x80);

                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &= (~0x100);

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /*new_cfg_sds_mode=1000x*/
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                            return retVal;

                    }
                    else if(regValue2 == 0x5)
                    {
#if 0
                        /*100FX*/
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                            return retVal;
#endif

                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0xB000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFFFBF;
                        reg_data |= 0x2000;
                        reg_data &= 0xffffefff;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data &= (~0x80);
                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &= (~0x100);
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;
                       /* new_cfg_sds_mode=1000x */
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                            return retVal;

                    }
                    else if(regValue2 == 0x7)
                    {
#if 0
                        /*100FX*/
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                            return retVal;
#endif
                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0xB000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFFFBF;
                        reg_data |= 0x2000;
                        reg_data &= 0xffffefff;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data &= 0xffffff7f;
                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &= 0xfffffeff;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0x9000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFDFFF;
                        reg_data |= 0x40;
                        if(pPortAbility->forcemode)
                            reg_data &= 0xffffefff;
                        else
                            reg_data |= 0x1000;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data &= (~0x80);
                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &=(~0x100);
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /*sds_mode:*/
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                            return retVal;

                    }

                    /*disable force ability   ---      */
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x137c, 12, 0)) != RT_ERR_OK)
                        return retVal;
                    return RT_ERR_OK;

                }

                /* new_cfg_sds_mode */
                if((retVal = rtl8367c_getAsicRegBits(unit, port, 0x1d95, 0x1f00, &regValue2)) != RT_ERR_OK)
                    return retVal;
                if(regValue2 == 0x2)
                {
#if 0
                    /*SGMII*/
                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
#endif
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    for(i=0;i<0xfff; i++);

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x2)) != RT_ERR_OK)
                        return retVal;

                    for(i=0;i<0xfff; i++);

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                        return retVal;

                    /*disable force ability   ---      */
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x137c, 12, 0)) != RT_ERR_OK)
                        return retVal;
                    return RT_ERR_OK;
                }
                else if(regValue2 == 0x12)
                {
#if 0
                    /*HiSGMII*/
                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
#endif
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    for(i=0;i<0xfff; i++);

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x12)) != RT_ERR_OK)
                        return retVal;

                    for(i=0;i<0xfff; i++);

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 0x1)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                        return retVal;

                    /*disable force ability   ---      */
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x137c, 12, 0)) != RT_ERR_OK)
                        return retVal;
                    return RT_ERR_OK;

                }
            }
            else
            {
                if((retVal = rtl8367c_getAsicRegBits(unit, port, 0x1d3d, 10, &regValue2)) != RT_ERR_OK)
                    return retVal;
                if (regValue2 == 0)
                {
                    /*ext1_force_ablty*/
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 2, pPortAbility->duplex)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1311, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, pPortAbility->link)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 6, pPortAbility->txpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                        return retVal;

                    /*force mode for ext1*/
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                        return retVal;

                    if (pPortAbility->link == 1)
                    {
                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, 0)) != RT_ERR_OK)
                            return retVal;

                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, 1)) != RT_ERR_OK)
                            return retVal;
                    }
                    else
                    {
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1311, 0x3, 2)) != RT_ERR_OK)
                            return retVal;
                    }

                    /*disable force ability   ---      */
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x137c, 12, 0)) != RT_ERR_OK)
                        return retVal;
                    return RT_ERR_OK;
                }
            }


        }
        else if (2 == id)
        {

            if((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d95, 0, &sgmiibit)) != RT_ERR_OK)
                    return retVal;
            if (sgmiibit == 1)
            {
                /*for 1000x/100fx/1000x_100fx, param has to bet set to serdes registers*/
                if((retVal = rtl8367c_getAsicReg(unit, port, 0x1d95, &regValue)) != RT_ERR_OK)
                    return retVal;
                /*cfg_mac7_sel_sgmii=1 & cfg_mac7_fib =1*/
                if((regValue & 0x3) == 0x3)
                {
                    if((retVal = rtl8367c_getAsicRegBits(unit, port, 0x1d95, 0x1f00, &regValue2)) != RT_ERR_OK)
                        return retVal;

                    if(regValue2 == 0x4)
                    {
                        /*1000X*/
#if 0
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                            return retVal;
#endif
                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0x9000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFDFFF;
                        reg_data |= 0x40;
                        if(pPortAbility->forcemode)
                            reg_data &= 0xffffefff;
                        else
                            reg_data |= 0x1000;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data &= 0xffffff7f;
                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &= 0xfffffeff;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                            return retVal;

                    }
                    else if(regValue2 == 0x5)
                    {
                        /*100FX*/
#if 0
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                            return retVal;
#endif
                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0xB000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFFFBF;
                        reg_data |= 0x2000;
                        reg_data &= 0xffffefff;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data &= 0xffffff7f;
                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &= 0xfffffeff;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                            return retVal;

                    }
                    else if(regValue2 == 0x7)
                    {
                        /*100FX*/
#if 0
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                            return retVal;
#endif
                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0xB000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFFFBF;
                        reg_data |= 0x2000;
                        reg_data &= 0xffffefff;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data  &= 0xffffff7f;
                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &= 0xfffffeff;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

			   /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFF0FFF;
                        reg_data |= 0x9000;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFDFFF;
                        reg_data |= 0x40;
                        if(pPortAbility->forcemode)
                            reg_data &= 0xffffefff;
                        else
                            reg_data |= 0x1000;

                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,0,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                        /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 2, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        if (pPortAbility->txpause)
                            reg_data |= 0x80;
                        else
                            reg_data &= 0xffffff7f;
                        if (pPortAbility->rxpause)
                            reg_data |= 0x100;
                        else
                            reg_data &= 0xfffffeff;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,2, reg_data)) != RT_ERR_OK)
                            return retVal;

                         /* 0 4 0  bit 12  set 0*/
                        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &reg_data)) != RT_ERR_OK)
                            return retVal;
                        reg_data &= 0xFFFFEFFF;
                        if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,4,0, reg_data)) != RT_ERR_OK)
                            return retVal;

                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                            return retVal;

                    }

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x137d, 12, 0)) != RT_ERR_OK)
                        return retVal;
                    return RT_ERR_OK;

                }

                if((retVal = rtl8367c_getAsicRegBits(unit, port, 0x1d95, 0x1f00, &regValue2)) != RT_ERR_OK)
                        return retVal;
                if(regValue2 == 0x2)
                {
                    /*SGMII*/
#if 0
                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
#endif
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    for(i=0;i<0xfff; i++);

                    /* 0 2 0  bit 8-9  nway*/
                    if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    reg_data &= 0xfffffcff;
                    if (pPortAbility->nway)
                        reg_data &= 0xfffffcff;
                    else
                        reg_data |= 0x100;
                    if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,2,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x2)) != RT_ERR_OK)
                        return retVal;

                    for(i=0;i<0xfff; i++);

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x137d, 12, 0)) != RT_ERR_OK)
                        return retVal;
                    return RT_ERR_OK;
                }
            }
            else
            {

                /*ext2_force_ablty*/
                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 2, pPortAbility->duplex)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c4, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 4, pPortAbility->link)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 6, pPortAbility->txpause)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                    return retVal;

                /*force mode for ext2*/
                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                    return retVal;

                if (pPortAbility->link == 1)
                {
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 4, 0)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c4, 4, 1)) != RT_ERR_OK)
                        return retVal;
                }
                else
                {
                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c4, 0x3, 2)) != RT_ERR_OK)
                        return retVal;
                }


                if((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d3d, 10, &reg_data)) != RT_ERR_OK)
                        return retVal;
                if(reg_data == 1)
                {
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 2, pPortAbility->duplex)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1311, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, pPortAbility->link)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 6, pPortAbility->txpause)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                        return retVal;

                    /*force mode for ext1*/
                    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                        return retVal;

                    if (pPortAbility->link == 1)
                    {
                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, 0)) != RT_ERR_OK)
                            return retVal;

                        if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1311, 4, 1)) != RT_ERR_OK)
                            return retVal;
                    }
                    else
                    {
                        if((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1311, 0x3, 2)) != RT_ERR_OK)
                            return retVal;
                    }
                }


            }

            /*disable force ability   ---      */
            if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x137d, 12, 0)) != RT_ERR_OK)
                return retVal;
        }
#if 0
        if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
            return retVal;
#endif
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getAsicPortForceLinkExt
 * Description:
 *      Get external interface force linking configuration
 * Input:
 *      id          - external interface id (0~1)
 *      pPortAbility - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
int32 rtl8367c_getAsicPortForceLinkExt(uint32 unit, rtk_port_t port, uint32 id, rtl8367c_port_ability_t *pPortAbility)
{
    uint32  reg_data, regValue, type;
    uint32  sgmiiSel;
    uint32  hsgmiiSel;
    uint32  Mode;
    int32       retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if (1 == type)
    {
        if(1 == id)
        {
            if((retVal = rtl8367c_getAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, &sgmiiSel)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, &hsgmiiSel)) != RT_ERR_OK)
                return retVal;

            if( (sgmiiSel == 1) || (hsgmiiSel == 1) )
            {
                memset(pPortAbility, 0x00, sizeof(rtl8367c_port_ability_t));
                pPortAbility->forcemode = 1;

                if((retVal = rtl8367c_getAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->duplex = reg_data;

                if((retVal = rtl8367c_getAsicRegBits(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->speed = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->link = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->txpause = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->rxpause = reg_data;

                return RT_ERR_OK;
            }
        }

        if(0 == id || 1 == id)
            retVal = rtl8367c_getAsicReg(unit, port, RTL8367C_REG_DIGITAL_INTERFACE0_FORCE+id, &reg_data);
        else
            retVal = rtl8367c_getAsicReg(unit, port, RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, &reg_data);

        if(retVal != RT_ERR_OK)
            return retVal;

        pPortAbility->forcemode = (reg_data >> 12) & 0x0001;
        pPortAbility->mstfault  = (reg_data >> 9) & 0x0001;
        pPortAbility->mstmode   = (reg_data >> 8) & 0x0001;
        pPortAbility->nway      = (reg_data >> 7) & 0x0001;
        pPortAbility->txpause   = (reg_data >> 6) & 0x0001;
        pPortAbility->rxpause   = (reg_data >> 5) & 0x0001;
        pPortAbility->link      = (reg_data >> 4) & 0x0001;
        pPortAbility->duplex    = (reg_data >> 2) & 0x0001;
        pPortAbility->speed     = reg_data & 0x0003;
    }
    else if (2 == type)
    {
        if (id == 1)
        {
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1311, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
        else if (2 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x13c4, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
    }
    else if (3 == type)
    {
        if (id == 1)
        {

            if((retVal = rtl8367c_getAsicPortExtMode(unit, port, id, &Mode))!=RT_ERR_OK)
                return retVal;
            if(Mode < EXT_SGMII)
            {

                if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1311, &reg_data))!=RT_ERR_OK)
                    return retVal;

                pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
            else if(Mode < EXT_1000X_100FX)
            {
                if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d11, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 10) & 1;
                pPortAbility->link = (reg_data >> 9) & 1;
                pPortAbility->speed = (reg_data >> 7) & 3;
                pPortAbility->rxpause = (reg_data >> 14) & 1;
                pPortAbility->txpause = (reg_data >> 13) & 1;
            }
            else if(Mode < EXT_RGMII_2)
            {
                if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1358, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }

        }
        else if (2 == id)
        {
            if((retVal = rtl8367c_getAsicPortExtMode(unit, port, id, &Mode))!=RT_ERR_OK)
                return retVal;
            if(Mode < EXT_SGMII)
            {

                if ((retVal = rtl8367c_getAsicReg(unit, port, 0x13c4, &reg_data))!=RT_ERR_OK)
                    return retVal;

                pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
            else if(Mode < EXT_1000X_100FX)
            {
                if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d11, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 10) & 1;
                pPortAbility->link = (reg_data >> 9) & 1;
                pPortAbility->speed = (reg_data >> 7) & 3;
                pPortAbility->rxpause = (reg_data >> 14) & 1;
                pPortAbility->txpause = (reg_data >> 13) & 1;
            }
            else if(Mode < EXT_RGMII_2)
            {
                if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1359, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
            else if(Mode < EXT_END)
            {

                if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1311, &reg_data))!=RT_ERR_OK)
                    return retVal;

                pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
        }
    }
    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicPortExtMode
 * Description:
 *      Set external interface mode configuration
 * Input:
 *      id      - external interface id (0~2)
 *      mode    - external interface mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
int32 rtl8367c_setAsicPortExtMode(uint32 unit, rtk_port_t port, uint32 id, uint32 mode)
{
    int32   retVal;
    uint32 i, regValue, type, option,reg_data;
    uint32 idx;
    uint32 redData[][2] =   { {0x04D7, 0x0480}, {0xF994, 0x0481}, {0x21A2, 0x0482}, {0x6960, 0x0483}, {0x9728, 0x0484}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x83F2, 0x002E} };
    uint32 redDataSB[][2] = { {0x04D7, 0x0480}, {0xF994, 0x0481}, {0x2420, 0x0482}, {0x6960, 0x0483}, {0x9728, 0x0484}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x83F2, 0x002E} };
    uint32 redData1[][2] =  { {0x82F1, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    uint32 redData5[][2] =  { {0x82F1, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    uint32 redData6[][2] =  { {0x82F1, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    uint32 redData8[][2] =  { {0x82F1, 0x0500}, {0xF995, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    uint32 redData9[][2] =  { {0x82F1, 0x0500}, {0xF995, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    uint32 redDataHB[][2] = { {0x82F0, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x7960, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:unit=%u port=%u\n", __FUNCTION__, __LINE__, unit, port);
    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:unit=%u port=%u type=%d\n", __FUNCTION__, __LINE__, unit, port, type);

    if (1==type)
    {
        if((mode == EXT_1000X_100FX) || (mode == EXT_1000X) || (mode == EXT_100FX))
        {
            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_REG_TO_ECO4, 5, 1)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_REG_TO_ECO4, 7, 1)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_CHIP_RESET, RTL8367C_DW8051_RST_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_MISCELLANEOUS_CONFIGURE0, RTL8367C_DW8051_EN_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_ACS_IROM_ENABLE_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_IROM_MSB_OFFSET, 0)) != RT_ERR_OK)
                return retVal;

            // if(mode == EXT_1000X_100FX)
            // {
            //     for(idx = 0; idx < FIBER2_AUTO_INIT_SIZE; idx++)
            //     {
            //         if ((retVal = rtl8367c_setAsicReg(unit, port, 0xE000 + idx, (uint32)Fiber2_Auto[idx])) != RT_ERR_OK)
            //             return retVal;
            //     }
            // }

            // if(mode == EXT_1000X)
            // {
            //     for(idx = 0; idx < FIBER2_1G_INIT_SIZE; idx++)
            //     {
            //         if ((retVal = rtl8367c_setAsicReg(unit, port, 0xE000 + idx, (uint32)Fiber2_1G[idx])) != RT_ERR_OK)
            //             return retVal;
            //     }
            // }

            // if(mode == EXT_100FX)
            // {
            //     for(idx = 0; idx < FIBER2_100M_INIT_SIZE; idx++)
            //     {
            //         if ((retVal = rtl8367c_setAsicReg(unit, port, 0xE000 + idx, (uint32)Fiber2_100M[idx])) != RT_ERR_OK)
            //             return retVal;
            //     }
            // }

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_IROM_MSB_OFFSET, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_ACS_IROM_ENABLE_OFFSET, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_CHIP_RESET, RTL8367C_DW8051_RST_OFFSET, 0)) != RT_ERR_OK)
                return retVal;
        }

        if(mode == EXT_GMII)
        {
            if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_EXT0_RGMXF, RTL8367C_EXT0_RGTX_INV_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_EXT1_RGMXF, RTL8367C_EXT1_RGTX_INV_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_EXT_TXC_DLY, RTL8367C_EXT1_GMII_TX_DELAY_MASK, 5)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_EXT_TXC_DLY, RTL8367C_EXT0_GMII_TX_DELAY_MASK, 6)) != RT_ERR_OK)
                return retVal;
        }

        /* Serdes reset */
        if( (mode == EXT_TMII_MAC) || (mode == EXT_TMII_PHY) )
        {
            if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_BYPASS_LINE_RATE, id, 1)) != RT_ERR_OK)
                return retVal;
        }
        else
        {
            if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_BYPASS_LINE_RATE, id, 0)) != RT_ERR_OK)
                return retVal;
        }

        if( (mode == EXT_SGMII) || (mode == EXT_HSGMII) )
        {
            if(id != 1)
                return RT_ERR_PORT_ID;

            if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C0, 0x0249)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicReg(unit, port, 0x13C1, &option)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C0, 0x0000)) != RT_ERR_OK)
                return retVal;
        }

        if(mode == EXT_SGMII)
        {
            if(option == 0)
            {
                for(i = 0; i <= 7; i++)
                {
                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redData[i][0])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redData[i][1])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;
                }
            }
            else
            {
                for(i = 0; i <= 7; i++)
                {
                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redDataSB[i][0])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redDataSB[i][1])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;
                }
            }
        }

        if(mode == EXT_HSGMII)
        {
            if(option == 0)
            {
                if( (retVal = rtl8367c_setAsicReg(unit, port, 0x13c2, 0x0249)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicReg(unit, port, 0x1301, &regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, 0x13c2, 0x0000)) != RT_ERR_OK)
                    return retVal;

                if ( ((regValue & 0x00F0) >> 4) == 0x0001)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redData1[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redData1[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0005)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redData5[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redData5[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0006)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redData6[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redData6[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0008)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redData8[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redData8[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0009)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redData9[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redData9[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
            }
            else
            {
                for(i = 0; i <= 8; i++)
                {
                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, redDataHB[i][0])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, redDataHB[i][1])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;
                }
            }
        }

        /* Only one ext port should care SGMII setting */
        if(id == 1)
        {

            if(mode == EXT_SGMII)
            {
                if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 1)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode == EXT_HSGMII)
            {
                if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {

                if((mode != EXT_1000X_100FX) && (mode != EXT_1000X) && (mode != EXT_100FX))
                {
                    if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != RT_ERR_OK)
                        return retVal;
                }
            }
        }

        if(0 == id || 1 == id)
        {
            if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_DIGITAL_INTERFACE_SELECT, RTL8367C_SELECT_GMII_0_MASK << (id * RTL8367C_SELECT_GMII_1_OFFSET), mode)) != RT_ERR_OK)
                return retVal;
        }
        else
        {
            if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1, RTL8367C_SELECT_GMII_2_MASK, mode)) != RT_ERR_OK)
                return retVal;
        }

        /* Serdes not reset */
        if( (mode == EXT_SGMII) || (mode == EXT_HSGMII) )
        {
            if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x7106)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, 0x0003)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                return retVal;
        }

        if( (mode == EXT_SGMII) || (mode == EXT_HSGMII) )
        {
            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_CHIP_RESET, RTL8367C_DW8051_RST_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_MISCELLANEOUS_CONFIGURE0, RTL8367C_DW8051_EN_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_ACS_IROM_ENABLE_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_IROM_MSB_OFFSET, 0)) != RT_ERR_OK)
                return retVal;

            for(idx = 0; idx < SGMII_INIT_SIZE; idx++)
            {
                if ((retVal = rtl8367c_setAsicReg(unit, port, 0xE000 + idx, (uint32)Sgmii_Init[idx])) != RT_ERR_OK)
                    return retVal;
            }

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_IROM_MSB_OFFSET, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_ACS_IROM_ENABLE_OFFSET, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_CHIP_RESET, RTL8367C_DW8051_RST_OFFSET, 0)) != RT_ERR_OK)
                return retVal;
        }
    }
    else if (2 == type)
    {

        if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_CHIP_RESET, RTL8367C_DW8051_RST_OFFSET, 1)) != RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_MISCELLANEOUS_CONFIGURE0, RTL8367C_DW8051_EN_OFFSET, 1)) != RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_ACS_IROM_ENABLE_OFFSET, 1)) != RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_IROM_MSB_OFFSET, 0)) != RT_ERR_OK)
            return retVal;

        // for(idx = 0; idx < FIBER1_2_INIT_SIZE; idx++)
        // {
        //     if ((retVal = rtl8367c_setAsicReg(unit, port, 0xE000 + idx, (uint32)Fiber1_2_Init[idx])) != RT_ERR_OK)
        //         return retVal;
        // }


        if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_IROM_MSB_OFFSET, 0)) != RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_DW8051_RDY, RTL8367C_ACS_IROM_ENABLE_OFFSET, 0)) != RT_ERR_OK)
            return retVal;

        if ((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_CHIP_RESET, RTL8367C_DW8051_RST_OFFSET, 0)) != RT_ERR_OK)
            return retVal;


        if( (mode == EXT_TMII_MAC) || (mode == EXT_TMII_PHY) )
        {
            if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_BYPASS_LINE_RATE, id+2, 1)) != RT_ERR_OK)
                return retVal;
        }
        else
        {
            if( (retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_BYPASS_LINE_RATE, id+2, 0)) != RT_ERR_OK)
                return retVal;
        }


        if (id == 1)
        {
            if(mode == EXT_HSGMII)
                return RT_ERR_PORT_ID;

            if (mode == EXT_SGMII)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 14, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X || mode == EXT_100FX || mode == EXT_1000X_100FX)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 14, 0)) != RT_ERR_OK)
                    return retVal;


                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x6210, 11, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, mode)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 14, 0)) != RT_ERR_OK)
                    return retVal;
            }

            if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 0x1f)) != RT_ERR_OK)
                return retVal;
        }
        else if(id == 2)
        {
            if (mode == EXT_HSGMII)
            {
                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x130, 7)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x39f, 7)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x3fa, 7)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x130, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x39f, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x3fa, 4)) != RT_ERR_OK)
                    return retVal;

            }


            if (mode == EXT_SGMII)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 6, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 7, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_HSGMII)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 6, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 7, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X || mode == EXT_100FX || mode == EXT_1000X_100FX)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 6, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 7, 0)) != RT_ERR_OK)
                    return retVal;


                if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x6200, 11, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, mode)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 6, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d92, 7, 0)) != RT_ERR_OK)
                    return retVal;
            }

            if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 0x1f)) != RT_ERR_OK)
                return retVal;
        }


        if (mode == EXT_RGMII)
        {

            if ((retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_PARA_LED_IO_EN3, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_PARA_LED_IO_EN1, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_PARA_LED_IO_EN2, 0)) != RT_ERR_OK)
                return retVal;


            if (id == 1)
            {

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1303, 9, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1303, 6, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1303, 4, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1303, 1, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1307, 3, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13f9, 0x38, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1307, 0x7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1304, 0x7000, 4)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13f9, 0x700, 4)) != RT_ERR_OK)
                    return retVal;
            }
            else if (id == 2)
            {

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1303, 10, 1)) != RT_ERR_OK)
                    return retVal;

                /*drving 1*/
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13e2, 2, 1)) != RT_ERR_OK)
                    return retVal;

                /*drving 1*/
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13e2, 1, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13e2, 0, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x13c5, 3, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13f9, 0x1c0, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c5, 0x7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13e2, 0x1c0, 4)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13e2, 0x38, 4)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else if (mode == EXT_SGMII)
        {
            if (id == 1)
            {
                /*sds 1     reg 1    page 0x21     write value  0xec91*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0xec91)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5) | 1)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C1)) != RT_ERR_OK)
                    return retVal;

                /*sds 1     reg 5    page 0x24     write value  0x5825*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x5825)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x24<<5) | 5)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 2)) != RT_ERR_OK)
                    return retVal;

                /*?????????????????*/

            }
            else if (id == 2)
            {
                /*sds 0     reg 0    page 0x28     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x28<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 0    page 0x24     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x24<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 5    page 0x21     write value  0x8dc3*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x8dc3)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5) | 5)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 2)) != RT_ERR_OK)
                    return retVal;

                /*?????????????????*/
            }
        }
        else if (mode == EXT_HSGMII)
        {
            if (id == 2)
            {
                /*sds 0     reg 0    page 0x28     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x28<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 0    page 0x24     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x24<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 5    page 0x21     write value  0x8dc3*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x8dc3)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5) | 5)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;


                /* optimizing HISGMII performance while RGMII used & */
                /*sds 0     reg 9     page 0x21     write value 0x3931*/
                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_DATA, 0x3931)) != RT_ERR_OK)
                        return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5)|9) ) != RT_ERR_OK)
                        return retVal;

                if( (retVal = rtl8367c_setAsicReg(unit, port, RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 0x12)) != RT_ERR_OK)
                    return retVal;

                /*?????????????????*/
            }
        }
        else if (mode == EXT_1000X)
        {
            if (id == 1)
            {

                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 1, 0x21, 0xec91)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 5, 0x24, 0x5825)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 4)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds1 1000M*/
                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0x9000;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 1, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFdFFF;
                regValue |= 0x40;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;


                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 4)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x6000, 0)) != RT_ERR_OK)
                    return retVal;

            }
            else if (id == 2)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 0x28, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 0x24, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 5, 0x21, 0x8dc3)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 4)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds0 1000M*/
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 0x1f)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0x9000;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFDFFF;
                regValue |= 0x40;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 4)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0xe0, 0)) != RT_ERR_OK)
                    return retVal;

            }
        }
        else if (mode == EXT_100FX)
        {
            if (id == 1)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 1, 0x21, 0xec91)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 5, 0x24, 0x5825)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 5)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds1 100M*/
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0xb000;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 1, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFFFBF;
                regValue |= 0x2000;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;
#if 0
                if( (retVal = rtl8367c_setAsicReg(unit, port, 0x6214, 0x1a0)) != RT_ERR_OK)
                    return retVal;
#endif
                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 5)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x6000, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if (id == 2)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 0x28, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 0x24, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 5, 0x21, 0x8dc3)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 5)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds0 100M*/
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 0x1f)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0xb000;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFFFBF;
                regValue |= 0x2000;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 5)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0xe0, 0)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else if (mode == EXT_1000X_100FX)
        {
            if (id == 1)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 1, 0x21, 0xec91)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 5, 0x24, 0x5825)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 13, 0, 0x4616)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 1, 1, 0, 0xf20)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f00, 7)) != RT_ERR_OK)
                    return retVal;
            }
            else if (id == 2)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 0x28, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 0, 0x24, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 5, 0x21, 0x8dc3)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 13, 0, 0x4616)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 1, 0, 0xf20)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d92, 0x1f, 7)) != RT_ERR_OK)
                    return retVal;
            }
        }

    }
    else if (3 == type)
    {

        /*restore patch, by designer. patch Tx FIFO issue, when not HSGMII 2.5G mode
         #sds0, page 1, reg 1, bit4=0*/
        if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 1, 1, &regValue)) != RT_ERR_OK)
            return retVal;
        regValue &= 0xFFFFFFEF;
        if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 1, 1, regValue)) != RT_ERR_OK)
            return retVal;

        /*set for mac 6*/
        if (1 == id)
        {

            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x137c, 0x1000)) != RT_ERR_OK)
                    return retVal;

            if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d9d, 6, &reg_data)) != RT_ERR_OK)
                    return retVal;
            while(reg_data == 0)
            {
                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d9d, 6, &reg_data)) != RT_ERR_OK)
                    return retVal;
            }

            if (mode == EXT_SGMII)
            {

                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d3d, 10, &reg_data)) != RT_ERR_OK)
                    return retVal;
                if(reg_data == 0)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 0)) != RT_ERR_OK)
                        return retVal;
                }

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 1, 0)) != RT_ERR_OK)
                    return retVal;




                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 1, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 0, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 9, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;



                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x2)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 2, 0)) != RT_ERR_OK)
                    return retVal;


            }
            else if (mode == EXT_HSGMII)
            {

                /*restore patch, by designer. patch Tx FIFO issue, when  HSGMII 2.5G mode
                 #sds0, page 1, reg 1, bit4=1*/
                if( (retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 1, 1, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue |= 0x10;
                if( (retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 1, 1, regValue)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d3d, 10, &reg_data)) != RT_ERR_OK)
                    return retVal;
                if(reg_data == 0)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 0)) != RT_ERR_OK)
                        return retVal;
                }

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 1, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 1, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 0, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 9, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0xd0,7)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x399, 7)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x3fa, 7)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x12)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 2, 0)) != RT_ERR_OK)
                    return retVal;

            }
            else if(mode == EXT_1000X)
            {

                if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &reg_data)) != RT_ERR_OK)
                    return retVal;
                reg_data &= 0xFFFFFCFF;
                if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,2,0, reg_data)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d3d, 10, &reg_data)) != RT_ERR_OK)
                    return retVal;
                if(reg_data == 0)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 0)) != RT_ERR_OK)
                        return retVal;
                }

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 1, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13eb, 0x15bb)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13e7, 0xc)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode == EXT_100FX)
            {

                if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &reg_data)) != RT_ERR_OK)
                    return retVal;
                reg_data &= 0xFFFFFCFF;
                if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,2,0, reg_data)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d3d, 10, &reg_data)) != RT_ERR_OK)
                    return retVal;
                if(reg_data == 0)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 0)) != RT_ERR_OK)
                        return retVal;
                }

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 1, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13eb, 0x15bb)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13e7, 0xc)) != RT_ERR_OK)
                    return retVal;



                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;



                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode == EXT_1000X_100FX)
            {
                /* 0 2 0  bit 8~9  set 0, force n-way*/
                if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &reg_data)) != RT_ERR_OK)
                    return retVal;
                reg_data &= 0xFFFFFCFF;
                if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,2,0, reg_data)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d3d, 10, &reg_data)) != RT_ERR_OK)
                    return retVal;
                if(reg_data == 0)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 0)) != RT_ERR_OK)
                        return retVal;
                }

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 1, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13eb, 0x15bb)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13e7, 0xc)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode < EXT_SGMII)
            {
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d3d, 10, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 1, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                if (mode < EXT_GMII)
                {
                    /* set mac6 mode*/
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, mode)) != RT_ERR_OK)
                        return retVal;
                }
                else if(mode == EXT_RMII_MAC)
                {

                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 7)) != RT_ERR_OK)
                        return retVal;
                }
                else if(mode == EXT_RMII_PHY)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, 8)) != RT_ERR_OK)
                        return retVal;
                }

                if ((mode == EXT_TMII_MAC) || (mode == EXT_TMII_PHY))
                {
                    if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 1, 1)) != RT_ERR_OK)
                        return retVal;
                }
            }

        }
        else if (2 == id)
        {

            /*force port7 linkdown*/
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x137d, 0x1000)) != RT_ERR_OK)
                    return retVal;

            if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d9d, 7, &reg_data)) != RT_ERR_OK)
                    return retVal;
            while(reg_data == 0)
            {
                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d9d, 7, &reg_data)) != RT_ERR_OK)
                    return retVal;
            }

            if (mode == EXT_SGMII)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf,0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13c4, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 0, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x2)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 2, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13c4, 0)) != RT_ERR_OK)
                    return retVal;


                if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &reg_data)) != RT_ERR_OK)
                    return retVal;
                reg_data &= 0xFFFFFCFF;
                if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,2,0, reg_data)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 3)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                    return retVal;

            }
            else if (mode == EXT_100FX)
            {

                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13c4, 0)) != RT_ERR_OK)
                    return retVal;


                if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &reg_data)) != RT_ERR_OK)
                    return retVal;
                reg_data &= 0xFFFFFCFF;
                if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,2,0, reg_data)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 3)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X_100FX)
            {
                /*  disable mac7 MII/TMM/RMII/GMII/RGMII mode, mode_ext2 = disable  */
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x13c4, 0)) != RT_ERR_OK)
                    return retVal;


                if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &reg_data)) != RT_ERR_OK)
                    return retVal;
                reg_data &= 0xFFFFFCFF;
                if((retVal = rtl8367c_setAsicSdsReg(unit, port, 0,2,0, reg_data)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 3)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode < EXT_SGMII)
            {
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d3d, 10, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicReg(unit, port, 0x1d95, 0x1f00)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13c3, 0xf, mode)) != RT_ERR_OK)
                    return retVal;

                if ((mode == EXT_TMII_MAC) || (mode == EXT_TMII_PHY))
                {
                    if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 1)) != RT_ERR_OK)
                        return retVal;
                }

            }
            else if ((mode < EXT_END) && (mode > EXT_100FX))
            {
                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x13C3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 0)) != RT_ERR_OK)
                    return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d3d, 10, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x1d11, 11, &reg_data)) != RT_ERR_OK)
                    return retVal;
                if(reg_data == 0)
                {
                    if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1d11, 6, 1)) != RT_ERR_OK)
                        return retVal;
                }


                if (mode < EXT_RMII_MAC_2)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, (mode-13))) != RT_ERR_OK)
                        return retVal;
                }
                else
                {
                    if ((retVal = rtl8367c_setAsicRegBits(unit, port, 0x1305, 0xf0, (mode-12))) != RT_ERR_OK)
                        return retVal;
                }

                if ((mode == EXT_TMII_MAC_2) || (mode == EXT_TMII_PHY_2))
                {
                    if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x3f7, 2, 1)) != RT_ERR_OK)
                        return retVal;
                }
            }

        }

    }
    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getAsicPortExtMode
 * Description:
 *      Get external interface mode configuration
 * Input:
 *      id      - external interface id (0~1)
 *      pMode   - external interface mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
int32 rtl8367c_getAsicPortExtMode(uint32 unit, rtk_port_t port, uint32 id, uint32 *pMode)
{
    int32   retVal;
    uint32 regData, regValue, type;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }


    if (1 == type)
    {
        if(0 == id || 1 == id)
            return rtl8367c_getAsicRegBits(unit, port, RTL8367C_REG_DIGITAL_INTERFACE_SELECT, RTL8367C_SELECT_GMII_0_MASK << (id * RTL8367C_SELECT_GMII_1_OFFSET), pMode);
        else
           return rtl8367c_getAsicRegBits(unit, port, RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1, RTL8367C_SELECT_GMII_2_MASK, pMode);
    }
    else if (2 == type)
    {
        if (1 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d92, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData & 0x4000)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            else if (((regData >> 8) & 0x1f) == 4)
            {
                *pMode = EXT_1000X;
                return RT_ERR_OK;
            }
            else if (((regData >> 8) & 0x1f) == 5)
            {
                *pMode = EXT_100FX;
                return RT_ERR_OK;
            }
            else if (((regData >> 8) & 0x1f) == 7)
            {
                *pMode = EXT_1000X_100FX;
                return RT_ERR_OK;
            }

            return rtl8367c_getAsicRegBits(unit, port, 0x1305, 0xf0, pMode);
        }
        else if (2 == id)
        {
#if 0
            if ((retVal = rtl8367c_getAsicRegBit(0x1d92, 6, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData == 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            if ((retVal = rtl8367c_getAsicRegBit(0x1d92, 7, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData == 1)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
#endif
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d92, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData & 0x40)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else if (regData & 0x80)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 4)
            {
                *pMode = EXT_1000X;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 5)
            {
                *pMode = EXT_100FX;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 7)
            {
                *pMode = EXT_1000X_100FX;
                return RT_ERR_OK;
            }

            return rtl8367c_getAsicRegBits(unit, port, 0x13c3, 0xf, pMode);
        }
    }
    else if(3 == type)
    {
        if (1 == id)
        {
            /* SDS_CFG_NEW */
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d95, &regData))!=RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d41, &regValue))!=RT_ERR_OK)
                return retVal;


            if((regValue & 0xa0)  == 0xa0 )
            {

                regData = regData >> 8;
                if((regData & 0x1f) == 4)
                {
                    *pMode = EXT_1000X;
                     return RT_ERR_OK;
                }
                else if((regData & 0x1f) == 5)
                {
                    *pMode = EXT_100FX;
                     return RT_ERR_OK;
                }
                else if((regData & 0x1f) == 7)
                {
                    *pMode = EXT_1000X_100FX;
                     return RT_ERR_OK;
                }

            }


            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d11, &regData))!=RT_ERR_OK)
                return retVal;

            /* check cfg_mac6_sel_sgmii */
            if((regData >> 6) & 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else if((regData >> 11) & 1)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
            else
            {
                /* check port6 MAC mode */
                if ((retVal = rtl8367c_getAsicRegBits(unit, port, 0x1305, 0xf0, &regData))!=RT_ERR_OK)
                    return retVal;

                *pMode = regData;
                return RT_ERR_OK;
            }
        }
        else if (2 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x1d95, &regData))!=RT_ERR_OK)
                return retVal;


            if(((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x4))
            {
                *pMode = EXT_1000X;
                    return RT_ERR_OK;
            }
            else if (((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x5))
            {
                *pMode = EXT_100FX;
                    return RT_ERR_OK;
            }
            else if (((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x7))
            {
                *pMode = EXT_1000X_100FX;
                    return RT_ERR_OK;
            }
            else if(regData & 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else
            {

                if ((retVal = rtl8367c_getAsicRegBits(unit, port, 0x13c3, 0xf, &regData))!=RT_ERR_OK)
                    return retVal;

                *pMode = regData;

                return RT_ERR_OK;
            }
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_rtl8367c_port_macForceLinkExt_set
 * Description:
 *      Set external interface force linking configuration.
 * Input:
 *      port            - external port ID
 *      mode            - external interface mode
 *      pPortability    - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set external interface force mode properties.
 *      The external interface can be set to:
 *      - MODE_EXT_DISABLE,
 *      - MODE_EXT_RGMII,
 *      - MODE_EXT_MII_MAC,
 *      - MODE_EXT_MII_PHY,
 *      - MODE_EXT_TMII_MAC,
 *      - MODE_EXT_TMII_PHY,
 *      - MODE_EXT_GMII,
 *      - MODE_EXT_RMII_MAC,
 *      - MODE_EXT_RMII_PHY,
 *      - MODE_EXT_SGMII,
 *      - MODE_EXT_HSGMII,
 *      - MODE_EXT_1000X_100FX,
 *      - MODE_EXT_1000X,
 *      - MODE_EXT_100FX,
 */
int32 dal_rtl8367c_port_macForceLinkExt_set(uint32 unit, rtk_port_t port, rtl8367c_mode_ext_t mode, rtl8367c_port_mac_ability_t *pPortability)
{
    int32 retVal;
    rtl8367c_port_ability_t ability;
    uint32 ext_id;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();
    ext_id = EXT_PORT0 - 15;
    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:unit=%u port=%u ext_id=%u\n", __FUNCTION__, __LINE__, unit, port, ext_id);

    if ((retVal = rtl8367c_setAsicPortExtMode(unit, port, ext_id, mode)) != RT_ERR_OK)
        return retVal;


    if ((retVal = rtl8367c_getAsicPortForceLinkExt(unit, port, ext_id, &ability)) != RT_ERR_OK)
        return retVal;

    ability.forcemode = pPortability->forcemode;
    ability.speed     = (mode == MODE_EXT_HSGMII) ? PORT_SPEED_1000M : pPortability->speed;
    ability.duplex    = pPortability->duplex;
    ability.link      = pPortability->link;
    ability.nway      = pPortability->nway;
    ability.txpause   = pPortability->txpause;
    ability.rxpause   = pPortability->rxpause;

    if ((retVal = rtl8367c_setAsicPortForceLinkExt(unit, port, ext_id, &ability)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setSgmiiNway
 * Description:
 *      Set SGMII Nway
 * Input:
 *      ext_id      - EXT ID
 *      state       - SGMII Nway state
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK                   - Success
 *      RT_ERR_SMI                  - SMI access error
 * Note:
 *      None.
 */
int32 rtl8367c_setSgmiiNway(uint32 unit, rtk_port_t port, uint32 ext_id, uint32 state)
{
    uint32 retVal, regValue, type, running = 0, retVal2;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 0;
            break;
        case 0x0652:
        case 0x6368:
            type = 1;
            break;
        case 0x0801:
        case 0x6511:
            type = 2;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if(type == 0)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x130c, 5, &running))!=RT_ERR_OK)
                return retVal;

            if(running == 1)
            {
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x130c, 5, 0))!=RT_ERR_OK)
                    return retVal;
            }

            retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x0002);

            if(retVal == RT_ERR_OK)
                retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x0080);

            if(retVal == RT_ERR_OK)
                retVal = rtl8367c_getAsicReg(unit, port, 0x6602, &regValue);

            if(retVal == RT_ERR_OK)
            {
                if(state)
                      regValue |= 0x0200;
                else
                      regValue &= ~0x0200;

                regValue |= 0x0100;
            }

            if(retVal == RT_ERR_OK)
                retVal = rtl8367c_setAsicReg(unit, port, 0x6602, regValue);

            if(retVal == RT_ERR_OK)
                retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x0002);

            if(retVal == RT_ERR_OK)
                retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x00C0);

            if(running == 1)
            {
                if ((retVal2 = rtl8367c_setAsicRegBit(unit, port, 0x130c, 5, 1))!=RT_ERR_OK)
                    return retVal2;
            }

            if(retVal != RT_ERR_OK)
                return retVal;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 1)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x0002))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x0081))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x6602, &regValue))!=RT_ERR_OK)
                   return retVal;

            if(state)
                  regValue |= 0x0200;
            else
                  regValue &= ~0x0200;

            regValue |= 0x0100;

            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6602, regValue))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x0002))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x00C1))!=RT_ERR_OK)
                   return retVal;
        }
        else if (2 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x0002))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x0080))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            if(state)
                regValue |= 0x0200;
            else
                regValue &= ~0x0200;

            regValue |= 0x0100;

            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6602, regValue))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x0002))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x00C0))!=RT_ERR_OK)
                return retVal;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 2)
    {
        if ((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 2, 0, &regValue))!=RT_ERR_OK)
            return retVal;

        if(state & 1)
            regValue &= ~0x100;
        else
            regValue |= 0x100;

        if ((retVal = rtl8367c_setAsicSdsReg(unit, port, 0, 2, 0, regValue))!=RT_ERR_OK)
            return retVal;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_rtl8367c_port_sgmiiNway_set
 * Description:
 *      Configure SGMII/HSGMII port Nway state
 * Input:
 *      port        - Port ID
 *      state       - Nway state
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK                   - OK
 *      RT_ERR_FAILED               - Failed
 *      RT_ERR_SMI                  - SMI access error
 *      RT_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API configure SGMII/HSGMII port Nway state
 */
int32 dal_rtl8367c_port_sgmiiNway_set(uint32 unit, rtk_port_t port, rtk_enable_t state)
{
    uint32 ext_id;

     /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if(state >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    ext_id = EXT_PORT0 - 15;
    return rtl8367c_setSgmiiNway(unit, port, ext_id, (uint32)state);
}

/* Function Name:
 *      rtl8367c_getSdsLinkStatus
 * Description:
 *      Get SGMII status
 * Input:
 *      id  - EXT ID
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK                   - Success
 *      RT_ERR_SMI                  - SMI access error
 * Note:
 *      None.
 */
int32 rtl8367c_getSdsLinkStatus(uint32 unit, rtk_port_t port, uint32 ext_id, uint32 *pSignalDetect, uint32 *pSync, uint32 *pLink)
{
    uint32 retVal, regValue, type, running = 0, retVal2;


    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(unit, port, 0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 0;
            break;
        case 0x0652:
        case 0x6368:
            type = 1;
            break;
        case 0x0801:
        case 0x6511:
            type = 2;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if(type == 0)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_getAsicRegBit(unit, port, 0x130c, 5, &running))!=RT_ERR_OK)
                return retVal;

            if(running == 1)
            {
                if ((retVal = rtl8367c_setAsicRegBit(unit, port, 0x130c, 5, 0))!=RT_ERR_OK)
                    return retVal;
            }

            retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x003D);

            if(retVal == RT_ERR_OK)
                retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x0080);

            if(retVal == RT_ERR_OK)
                retVal = rtl8367c_getAsicReg(unit, port, 0x6602, &regValue);

            if(running == 1)
            {
                if ((retVal2 = rtl8367c_setAsicRegBit(unit, port, 0x130c, 5, 1))!=RT_ERR_OK)
                    return retVal2;
            }

            if(retVal != RT_ERR_OK)
                return retVal;

            *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
            *pSync = (regValue & 0x0001) ? 1 : 0;
            *pLink = (regValue & 0x0010) ? 1 : 0;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 1)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x003D))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x0081))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
            *pSync = (regValue & 0x0001) ? 1 : 0;
            *pLink = (regValue & 0x0010) ? 1 : 0;
        }
        else if (2 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6601, 0x003D))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(unit, port, 0x6600, 0x0080))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(unit, port, 0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
            *pSync = (regValue & 0x0001) ? 1 : 0;
            *pLink = (regValue & 0x0010) ? 1 : 0;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 2)
    {
        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 30, 1, &regValue)) != RT_ERR_OK)
            return retVal;
        if((retVal = rtl8367c_getAsicSdsReg(unit, port, 0, 30, 1, &regValue)) != RT_ERR_OK)
            return retVal;
        *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
        *pSync = (regValue & 0x0001) ? 1 : 0;
        *pLink = (regValue & 0x0010) ? 1 : 0;
        RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%d signal_detect=%u, sync=%u, link=%u\n", __FUNCTION__, __LINE__, *pSignalDetect, *pSync, *pLink);
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_rtl8367c_port_sgmiiLinkStatus_get
 * Description:
 *      Get SGMII status
 * Input:
 *      port        - Port ID
 * Output:
 *      pSignalDetect   - Signal detect
 *      pSync           - Sync
 *      pLink           - Link
 * Return:
 *      RT_ERR_OK                   - OK
 *      RT_ERR_FAILED               - Failed
 *      RT_ERR_SMI                  - SMI access error
 *      RT_ERR_PORT_ID              - Invalid port ID.
 * Note:
 *      The API can reset Serdes
 */
int32 dal_rtl8367c_port_sgmiiLinkStatus_get(uint32 unit, rtk_port_t port, uint32 *pSignalDetect, uint32 *pSync, rtk_port_linkStatus_t *pLink)
{
    uint32 ext_id;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if(NULL == pSignalDetect)
        return RT_ERR_NULL_POINTER;

    if(NULL == pSync)
        return RT_ERR_NULL_POINTER;

    if(NULL == pLink)
        return RT_ERR_NULL_POINTER;

    ext_id = EXT_PORT0 - 15;
    return rtl8367c_getSdsLinkStatus(unit, port, ext_id, pSignalDetect, pSync, pLink);
}

int32 phy_8213b_init(uint32 unit, rtk_port_t port){
    int32  retVal;
    rtl8367c_rma_t rmaCfg;
    rtl8367c_port_mac_ability_t *mac_ability;
    rtk_port_phy_ability_t *pAbility;
    uint32 signal_detect, sync, link;
    uint32 *pSignal_detect, *pSync, *pLink;
    mac_ability = osal_alloc(sizeof(*mac_ability));
    pAbility = osal_alloc(sizeof(*pAbility));
    pSignal_detect = &signal_detect;
    pSync = &sync;
    pLink = &link;

    RT_LOG(LOG_DEBUG, MOD_OOB, "%s:%u:unit=%u port=%u\n", __FUNCTION__, __LINE__, unit, port);
    /* Find device */
    init_state = INIT_COMPLETED;

    /* Set Old max packet length to 16K */
    if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_MAX_LENGTH_LIMINT_IPG, RTL8367C_MAX_LENTH_CTRL_MASK, 3)) != RT_ERR_OK)
        goto FAIL_EXIT;

    if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_MAX_LEN_RX_TX, RTL8367C_MAX_LEN_RX_TX_MASK, 3)) != RT_ERR_OK)
        goto FAIL_EXIT;

    /* ACL Mode */
    if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_REG_ACL_ACCESS_MODE, RTL8367C_ACL_ACCESS_MODE_MASK, 1)) != RT_ERR_OK)
        goto FAIL_EXIT;

    /* Max rate */
    if((retVal = rtl8367c_setAsicPortIngressBandwidth(unit, port, rtk_switch_port_L2P_get(EXT_PORT0), RTL8367C_QOS_RATE_INPUT_MAX_HSG>>3, DISABLED, ENABLED)) != RT_ERR_OK)
        goto FAIL_EXIT;

    if ((retVal = rtl8367c_setAsicPortEgressRate(unit, port, rtk_switch_port_L2P_get(EXT_PORT0), RTL8367C_QOS_RATE_INPUT_MAX_HSG>>3)) != RT_ERR_OK)
        goto FAIL_EXIT;

    if ((retVal = rtl8367c_setAsicPortEgressRateIfg(unit, port, ENABLED)) != RT_ERR_OK)
        goto FAIL_EXIT;

    if((retVal = rtl8367c_setAsicReg(unit, port, 0x03fa, 0x0007)) != RT_ERR_OK)
        goto FAIL_EXIT;

    /* Change unknown DA to per port setting */
    if((retVal = rtl8367c_setAsicRegBits(unit, port, RTL8367C_PORT_SECURIT_CTRL_REG, RTL8367C_UNKNOWN_UNICAST_DA_BEHAVE_MASK, 3)) != RT_ERR_OK)
        goto FAIL_EXIT;

    /* LUT lookup OP = 1 */
    if ((retVal = rtl8367c_setAsicLutIpLookupMethod(unit, port, 1))!=RT_ERR_OK)
        goto FAIL_EXIT;

    /* Set RMA */
    rmaCfg.portiso_leaky = 0;
    rmaCfg.vlan_leaky = 0;
    rmaCfg.keep_format = 0;
    rmaCfg.trap_priority = 0;
    rmaCfg.discard_storm_filter = 0;
    rmaCfg.operation = 0;
    if ((retVal = rtl8367c_setAsicRma(unit, port, 2, &rmaCfg))!=RT_ERR_OK)
        goto FAIL_EXIT;

    /* Enable TX Mirror isolation leaky */
    if ((retVal = rtl8367c_setAsicPortMirrorIsolationTxLeaky(unit, port, ENABLED)) != RT_ERR_OK)
        goto FAIL_EXIT;

    /* INT EN */
    if((retVal = rtl8367c_setAsicRegBit(unit, port, RTL8367C_REG_IO_MISC_FUNC, RTL8367C_INT_EN_OFFSET, 1)) != RT_ERR_OK)
        goto FAIL_EXIT;



    mac_ability->forcemode = 1;
    mac_ability->speed = 2;
    mac_ability->duplex = 1;
    mac_ability->link = 1;
    mac_ability->nway = 0;
    mac_ability->txpause = 1;
    mac_ability->rxpause = 1;
    if((retVal = dal_rtl8367c_port_sgmiiNway_set(unit, port, ENABLED)) != RT_ERR_OK)
        goto FAIL_EXIT;
    if((retVal = dal_rtl8367c_port_macForceLinkExt_set(unit, port, MODE_EXT_SGMII, mac_ability)) != RT_ERR_OK)
        goto FAIL_EXIT;
    if((retVal = rtl8367c_getSdsLinkStatus(unit, port, 1, pSignal_detect, pSync, pLink)) != RT_ERR_OK)
        goto FAIL_EXIT;

    pAbility->Half_10 = 1;
    pAbility->Full_10 = 1;
    pAbility->Half_100 = 1;
    pAbility->Full_100 = 1;
    pAbility->Half_1000 = 1;
    pAbility->Full_1000 = 1;
    pAbility->FC = 1;
    // pAbility->AsyFC = 1;
    if((retVal = phy_8213b_AutoNegoAbility_set(unit, port, pAbility)) != RT_ERR_OK)
        goto FAIL_EXIT;

    // set P3 crc skip enable
    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x006e, 8, 0x1)) != RT_ERR_OK)
        goto FAIL_EXIT;
    // set P6 crc skip enable
    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x00ce, 8, 0x1)) != RT_ERR_OK)
        goto FAIL_EXIT;

    // set pause frame pass through
    // set no drop rx pause enable
    if((retVal = rtl8367c_setAsicRegBit(unit, port, 0x1201, 8, 0x1)) != RT_ERR_OK)
        goto FAIL_EXIT;
    // set Reserved Multicast Address forwarding
    if((retVal = rtl8367c_setAsicReg(unit, port, 0x801, 0x0)) != RT_ERR_OK)
        goto FAIL_EXIT;

    if((retVal = _phy_8213b_led_init(unit, port)) != RT_ERR_OK)
        goto FAIL_EXIT;

    return RT_ERR_OK;

    FAIL_EXIT:
        if (mac_ability != NULL)
            osal_free(mac_ability);
        if (pAbility != NULL)
            osal_free(pAbility);
        return retVal;
}


