/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : Define board profile structure, API, and macros.
 *
 * Feature :
 *
 */

#ifndef __HW_PROFILE_H__
#define __HW_PROFILE_H__

/*
 * Include Files
 */
#include <common/type.h>
#include <hal/chipdef/chip.h>
#include <hal/phy/phydef.h>
#include <private/drv/swcore/chip_probe.h>

/*
 * Symbol Definition
 */



/* profile identifier */
#define HWP_CHIP_AUTOPROBE                              0xFFFFFF00
#define HWP_MAX_PROFILE                                 10     /* how many profiles the code would use */
#define HWP_PROFILE_NAME_LEN_MAX                        44     /* hardware profile name max length (if using string) */
#define HWP_NONE                                        (0xFF)
#define HWP_END                                         (0xFF)
#define HWP_LED_END                                     (0xFFFFFFFF)
#define HWP_NOT_USED                                    (0xFF)
#define HWP_TBD                                         99
#define HWP_VALUE_NO_INIT                               (-1)
#define HWP_LOCALUNIT_EMPTY                             0
#define HWP_CASCADE_MASTER                              0x1
#define HWP_CASCADE_SLAVE                               0x2
#define HWP_CASCADE_NONE                                0x0


#define HWP_1SDS(s1)                                    { s1, HWP_NONE, HWP_NONE, HWP_NONE }
#define HWP_2SDS(s1,s2)                                 { s1, s2, HWP_NONE, HWP_NONE }
#define HWP_3SDS(s1,s2,s3)                              { s1, s2, s3, HWP_NONE }
#define HWP_4SDS(s1,s2,s3,s4)                           { s1, s2, s3, s4 }
#define SDS_NONE()                                      { HWP_NONE, HWP_NONE, HWP_NONE, HWP_NONE }

/*
 * Data Type Definition
 */
/* hardware profile ID, it can be either by "string" or by "integer" */
typedef enum hwp_id_type_e
{
    HWP_ID_TYPE_STRING = 0,
    HWP_ID_TYPE_INTEGER,
    HWP_ID_TYPE_END
}hwp_id_type_t;

typedef enum hwp_id_e
{
    HWP_RTL833X_DEFAULT                             = 8330000,
    HWP_RTL8330M_INTPHY_8212B_DEMO                  = 8330001,
    HWP_RTL8330M_INTPHY_8212B_SGMII_DEMO,
    HWP_RTL8330M_INTPHY_2FIB_1G_DEMO,
    HWP_RTL8332M_8208L_INTPHY_8208L_8214C_DEMO,
    HWP_RTL8332M_8208L_INTPHY_8208L_8214B_DEMO,
    HWP_RTL835X_DEFAULT                             = 8350000,
    HWP_RTL8351M                                    = 8350001,
    HWP_RTL8353M_QA,
    HWP_RTL8353M_DEMO,
    HWP_RTL8353M_8214B_DEMO,
    HWP_RTL838X_DEFAULT                             = 8380000,
    HWP_RTL8380M_INTPHY_2FIB_1G_DEMO                = 8380001,
    HWP_RTL8382M_8218B_INTPHY_8218FB_DEMO,
    HWP_RTL8382M_8218B_INTPHY_8218B_8214FC_DEMO,
    HWP_RTL8382M_8218D_INTPHY_8218D_8214FC_DEMO,
    HWP_RTL8382M_8218B_INTPHY_8218B_8214B_DEMO,
    HWP_RTL8382M_8218B_INTPHY_8218B_2FIB_1G_DEMO,
    HWP_RTL8382M_8218D_INTPHY_8218D_2FIB_1G_DEMO,
    HWP_RTL8382M_8218E_INTPHY_8218E_2FIB_1G_DEMO,
    HWP_RTL8381M_INTPHY_8214FC_2FIB_1G_DEMO,
    HWP_RTL8381M_INTPHY_8214C_2FIB_1G_DEMO,
    HWP_RTL8380H_INTPHY_DEMO,
    HWP_RTL839X_DEFAULT                             = 8390000,
    HWP_RTL8391M_DEMO                               = 8390001,
    HWP_RTL8391M_14C_DEMO,
    HWP_RTL8392M_DEMO,
    HWP_RTL8393M_DEMO,
    HWP_RTL8396M2_DEMO,
    HWP_RTL8396M_8214QF_8295R_ES_DEMO,
    HWP_RTL8396M_8218B_8214QF_8295R_DEMO,
    HWP_RTL8396M_8218B_8214QF_8295R_C45_DEMO,
    HWP_RTL8396M_8214QF_8295R_C45_DEMO,
    HWP_RTL8393M_8218D,
    HWP_RTL8393M_8218E,
    HWP_RTL9301_DEFAULT                             = 9301000,
    HWP_RTL9303_DEFAULT                             = 9303000,
    HWP_RTL9301_2x8214QF_4XGE                       = 9300001,
    HWP_RTL9301_3x8218B_4XGE,
    HWP_RTL9301_8218B_4XGE,
    HWP_RTL9301_8218B_4XGE_CASCADE,
    HWP_RTL9301_2x8214QF_4XGE_CASCADE,
    HWP_RTL9301_14QF_4XGE_18B_4XGE_CASCADE,
    HWP_RTL9301_3x8218D_4XGE,
    HWP_RTL9301_6x8214QF_4XGE,
    HWP_RTL9301_2x8214FC_4x8214QF_4XGE,
    HWP_RTL9301_6x8218D_2x8295R_CASCADE,
    HWP_RTL9301_2x8218B_4x8218D_2x8295R_CASCADE,
    HWP_RTL9302B_2x8218D_2xCUST1_4XGE,
    HWP_RTL9302C_4xCUST1,
    HWP_RTL9303_2xCUST1,
    HWP_RTL9303_8XGE,
    HWP_RTL9302D_6x8224QF_2XGE,
    HWP_RTL9302D_6x8224_2XGE,
    HWP_RTL9302C_SG3210XHP_M2_V3,
    HWP_RTL9302C_SG3210X_M2,
    HWP_RTL9302C_SG3218XP_M2,
    HWP_RTL9302C_SG2210XMP_M2,
    HWP_RTL9302B_2x8218E_2x8224QF_4XGE,
    HWP_RTL9303_8x8226,
    HWP_RTL9301_3x8218D_2x8226CARD_2XGE,
    HWP_RTL9303_8x2_5G,
    HWP_RTL9303_2x8264,
    HWP_RTL9303_2x8264_8264_8264,
    HWP_RTL9303_2x8264_8261_8264,
    HWP_RTL9301_3x8218E_4XGE,
    HWP_RTL9303_2x8254L_DEMO,
    HWP_RTL9303_6x8254L_6xSPI,
    HWP_RTL9310_DEFAULT                             = 9310000,
    HWP_RTL9311_6x8218D_6XGE,
    HWP_RTL9311_6x8218D_6XGE_PCI,
    HWP_RTL9311_6x8218D_6XGE_DEMO,
    HWP_RTL9311_6x8218E_6XGE_DEMO,
    HWP_RTL9311_6x8218D_6XGE1G,
    HWP_RTL9311_6x8218D_6xFib1g100,
    HWP_RTL9311E,
    HWP_RTL9311R,
    HWP_RTL9311R_PCI,
    HWP_RTL9311_USERMODELKM_PCI_DEFAULT,
    HWP_RTL9313_12XGE,
    HWP_RTL9313_12XGE_PCI,
    HWP_RTL9313_12XGE1G,
    HWP_RTL9313_12xFib1g100,
    HWP_RTL9313_2x8264_2x8261_2XGE_DEMO,
    HWP_RTL9313_2x8264_2x8261_2XGE_DEMO_B,
    HWP_RTL9312_24X2P5G_6XGE,
    HWP_RTL9313_2x8254L_2x8261I_2XGE_DEMO,
    HWP_RTL9313_2x8264B_2x8261N_2XGE_AB,
    HWP_RTL9313_2x8264B_2x8261N_2XGE_D,
    HWP_RTL9313_2x8264B_2x8261N_2XGE_GH,
    HWP_RTL9313_10x8261N,
    HWP_RTL9313_6X8224_4XGE,
    HWP_RTL9313_6X8224_4XGE_3428X_M2,
    HWP_RTL9311BM_3x8218D_4XGE,
    HWP_RTL9311BM_6x8218D_4XGE_CASCADE,
    HWP_RTL9311BM_3x8218D_4XGE_QA,
    HWP_RTL9311BM_3x8218E_4XGE,
    HWP_RTL9311BM_6x8218E_4XGE_CASCADE,
    HWP_RTL9311BM_3x8218E_4XGE_QA,
    HWP_RTL9311BM_3x8218E_4XGE_PCIE,
    HWP_RTL9311B_3x8218D_4XGE,
    HWP_RTL9311B_6x8218D_4XGE_CASCADE,
    HWP_RTL9311B_3x8218D_4XGE_QA,
    HWP_RTL9311B_3x8218E_4XGE,
    HWP_RTL9311B_6x8218E_4XGE_CASCADE,
    HWP_RTL9311B_3x8218E_4XGE_QA,
    HWP_RTL9311B_3x8218E_4XGE_PCIE,
    HWP_RTL8392B_QA,
    HWP_RTL9311_2x8214QF_1x8224QF_2x8295R,
    HWP_RTL9330                                     = 9330000,
    HWP_RTL_PHY_ONLY_DEFAULT,
}hwp_id_t;

typedef enum operationType_e
{
    HWP_OPERATION_AND = 0,
    HWP_OPERATION_OR,
    HWP_OPERATION_END
}operationType_t;

typedef struct hwp_identifier_s
{
    hwp_id_type_t   type;
    char    name[HWP_PROFILE_NAME_LEN_MAX];
    int     id;
}hwp_identifier_t;


typedef enum rt_port_attri_id_e
{
    HWP_ETH_ID = 0,
    HWP_CASCADE_ID,
    HWP_CPU_ID,
    HWP_SC_ID,
    HWP_OOB_ID,
    HWP_ATTRI_ID_END,
}rt_port_attri_id_t;

typedef enum rt_port_ethType_id_e
{
    HWP_FE_ID = 0,
    HWP_GE_ID,
    HWP_2_5GE_ID,
    HWP_5GE_ID,
    HWP_XGE_ID,
    HWP_25GE_ID,
    HWP_40GE_ID,
    HWP_50GE_ID,
    HWP_100GE_ID,
    HWP_SXGE_ID,
    HWP_ETHTYPE_ID_END,
}rt_port_ethType_id_t;

typedef enum rt_port_medium_id_e
{
    HWP_COPPER_ID = 0,
    HWP_FIBER_ID,
    HWP_COMBO_ID,
    HWP_SERDES_ID,
    HWP_MEDIUM_ID_END,
}rt_port_medium_id_t;


/* port attribute */
typedef enum rt_port_attri_e
{
    HWP_ETH             = (0x1 << HWP_ETH_ID),        /* Ethernet port */
    HWP_CASCADE         = (0x1 << HWP_CASCADE_ID),    /* Cadcade port (the interconnection between two chips cadcaded on a board) */
    HWP_CPU             = (0x1 << HWP_CPU_ID),        /* CPU port */
    HWP_SC              = (0x1 << HWP_SC_ID),         /* there is SerDes Converter connecting to the port */
    HWP_OOB             = (0x1 << HWP_OOB_ID),         /* there is SerDes Converter connecting to the port */
    HWP_ATTRI_END       = (0x1 << HWP_ATTRI_ID_END),  /* Attri end*/
}rt_port_attri_t;


/* port ethernet type */
typedef enum rt_port_ethType_e
{
    HWP_FE             = (0x1 << HWP_FE_ID),          /* Fast ethernet port */
    HWP_GE             = (0x1 << HWP_GE_ID),          /* Giga ethernet port */
    HWP_2_5GE          = (0x1 << HWP_2_5GE_ID),       /* 2.5 Giga ethernet port */
    HWP_5GE            = (0x1 << HWP_5GE_ID),         /* 5 Giga ethernet port */
    HWP_XGE            = (0x1 << HWP_XGE_ID),         /* 10 Giga ethernet port */
    HWP_25GE           = (0x1 << HWP_25GE_ID),        /* 25 Giga ethernet port */
    HWP_40GE           = (0x1 << HWP_40GE_ID),        /* 40 Giga ethernet port */
    HWP_50GE           = (0x1 << HWP_50GE_ID),        /* 50 Giga ethernet port */
    HWP_100GE          = (0x1 << HWP_100GE_ID),     /* 100 Giga ethernet port */
    HWP_SXGE           = (0x1 << HWP_SXGE_ID),        /* Cascade Stack port */
    HWP_ETHTYPE_END    = (0x1 << HWP_ETHTYPE_ID_END), /* Ether type end*/
}rt_port_ethType_t;


/* port medium type */
typedef enum rt_port_medium_e
{
    HWP_COPPER         = (0x1 << HWP_COPPER_ID),      /* port's PHY media is copper */
    HWP_FIBER          = (0x1 << HWP_FIBER_ID),       /* port's PHY media is fiber */
    HWP_COMBO          = (0x1 << HWP_COMBO_ID),       /* port's PHY media is combo */
    HWP_SERDES         = (0x1 << HWP_SERDES_ID),      /* port media is serdes */
    HWP_MEDIUM_END     = (0x1 << HWP_MEDIUM_ID_END),  /* hwp medium end  */
}rt_port_medium_t;


/* Port Count information */
typedef struct hwp_portCount_s
{
    uint32              count;      /* Total number of ports */
    int32               max;        /* Maximum mac ID. */
    int32               min;        /* Minimum mac ID. */
}hwp_portCount_t;


/* chip location. */
typedef enum hwp_location_e
{
    HWP_LOCATE_LOCAL = 0,               /* chip is on local board */
    HWP_LOCATE_REMOTE,                  /* chip is on a remote board (especially for stacking system) */
    HWP_LOCATE_END
}hwp_location_t;


/* switch core register access method (from main CPU) */
typedef enum hwp_swRegAccMethod_e
{
    HWP_SW_ACC_NONE = 0,                /* not able to access register */
    HWP_SW_ACC_MEM,                     /* the switch core registers are "Memory Map" to main CPU, for example, by PCIe bus of external CPU, or embedded CPU  */
    HWP_SW_ACC_SPI,                     /* the switch core registers are accessed through SPI interface by main CPU */
    HWP_SW_ACC_PCIe,                    /* the switch core registers are accessed through PCIe by main CPU */
    HWP_SW_ACC_I2C,                     /* the switch core registers are accessed through I2C interface by main CPU */
    HWP_SW_ACC_SIM,                     /* the switch core registers are accessed through simulated virtual memory by local CPU */
    HWP_SW_ACC_END
}hwp_swRegAccMethod_t;


/* serdes polarity */
typedef enum hwp_serdes_polarity_e
{
    SERDES_POLARITY_NORMAL = 0,
    SERDES_POLARITY_CHANGE = 1,
} hwp_serdes_polarity_t;


typedef enum hwp_cpuArch_e
{
    HWP_CPU_ARCH_MIPS = 0x6,
    HWP_CPU_ARCH_ARM,
    HWP_CPU_ARCH_END,
} hwp_cpuArch_t;

/* port description */
typedef struct hwp_portDescp_e
{
    uint8   mac_id;                     /* Physical MAC ID */
    uint8   phy_idx;                    /* phy index number or HWP_NONE */
    uint8   smi;                        /* which set of SMI interface */
    uint8   phy_addr;                   /* phy address */
    uint8   sds[RTK_MAX_SDS_PER_PORT];  /* serdes ID of the port. It index to sds_id of hwp_serdesDescp_t table. set to SDS_NONE() if there is no SerDes of the port */
    uint16  eth;                        /* port ethernet type */
    uint8   attr;                       /* port attribute */
    uint8   medi;                       /* port medium */
    uint8   sc_idx;                     /* index to serdes converter. this file is meanful only when HWP_SC bit is set in .attr  */
    uint8   led_c;                      /* copper port led definition selection */
    uint8   led_f;                      /* fibber port led definition selection */
    uint8   led_layout;                 /* choose led layout of the combo port*/
    uint8   phy_mdi_pin_swap:1;         /* PHY's MDI pins which connects to ICM. 1: Swap the pins(pair ABCD to DCBA); 0: swap is asigned by strap pin. */
    uint8   phy_mdi_pair_swap;          /* PHY's MDI pins which connects to ICM. A bitmap, bit[0] for swap pair A polarity; bit[1] for swap pair B polarity; bit[2] for swap pair C polarity; bit[3] for swap pair D polarity; */
}hwp_portDescp_t;


/* serdes description */
typedef struct hwp_serdesDescp_e
{
    uint8   sds_id;                     /* Physical Serdes ID */
    uint8   mode:6;
    uint8   rx_polarity:1;
    uint8   tx_polarity:1;
}hwp_serdesDescp_t;


/* serdes converter description */
typedef struct hwp_scDescp_e
{
    uint32  chip;
    uint8   smi;                        /* which set of SMI interface */
    uint8   phy_addr;                   /* phy address */
    uint8   rx_polarity:1;
    uint8   tx_polarity:1;
}hwp_scDescp_t;

/* phy description */
typedef struct hwp_phyDescp_e
{
    uint32  chip;                       /* phy_type_t */
    uint8   phy_max;
    uint8   mac_id;
    uint8   interruptSocGpioDev;    /* which SoC GPIO Device connect to PHY interrupt source. Use drv_generalCtrlGpio_devId_t enum. Set to HWP_NONE if no pin is connected  */
    uint8   interruptSocGpioPin;    /* which SoC GPIO PIN connect to PHY interrupt source. Set to 0 if no pin is connected  */
}hwp_phyDescp_t;


/* LED interface */
typedef enum led_if_sel_e
{
    LED_IF_SEL_NONE,
    LED_IF_SEL_SERIAL,
    LED_IF_SEL_SINGLE_COLOR_SCAN,
    LED_IF_SEL_BI_COLOR_SCAN,
}hwp_ledIfSel_t;

typedef enum hwp_ledActive_sel_e
{
    LED_ACTIVE_HIGH,
    LED_ACTIVE_LOW,
}hwp_ledActive_sel_t;


/* LED layout select*/
typedef enum led_layout_sel_e
{
    SINGLE_SET,
    DOUBLE_SET,
}hwp_ledLayoutSel_t;


/* LED description */
typedef struct hwp_ledDescp_s
{
    hwp_ledIfSel_t led_if_sel;
    hwp_ledActive_sel_t led_active;
    uint32 led_signal_invert;        /* LED signal stream invert, only for serial LED if. */

    struct {
        uint32 led[RTK_MAX_LED_PER_PORT];
    } led_definition_set[RTK_MAX_LED_MOD];

}hwp_ledDescp_t;


/* Switch description structure */
typedef struct hwp_swDescp_s
{
    uint32                  chip_id;                /* chip model (will check with probe result), or HWP_CHIP_AUTOPROBE(auto probe chip ID)  */
    uint8                   swcore_supported;       /* if the switch/MAC function is used. TRUE: used; FALSE: not used */
    hwp_swRegAccMethod_t    swcore_access_method;   /* how the switch/MAC registers were accessed */
    uint8                   swcore_spi_chip_select; /* if through SPI, which chip select of master CPU connected to this switch/MAC unit */
    uint8                   swcore_pcie_bus_number; /* if through PCI-E, which bus number connected to this switch/MAC unit */
    uint8                   nic_supported;        /* if support NIC function (packet Tx/Rx to/from CPU) */
    uint8                   cascade_role;           /* HWP_CASCADE_MASTER, HWP_CASCADE_SLAVE, or HWP_CASCADE_NONE */

    /*
     * following information is necessary only when "swcore_supported = TRUE"
     */
    struct {
        uint8               count;
        hwp_portDescp_t     descp[RTK_MAX_PORT_PER_UNIT];
    } port;

    /* serdes of the MAC */
    struct {
        uint8               count;
        hwp_serdesDescp_t   descp[RTK_MAX_SDS_PER_UNIT];
    } serdes;

    /* serdes converter */
    struct {
        uint8               count;
        hwp_scDescp_t       descp[RTK_MAX_SC_PER_UNIT];
    } sc;

    /* phy */
    struct {
        uint8               count;
        hwp_phyDescp_t      descp[RTK_MAX_PHY_PER_UNIT];
    } phy;

    /* LED */
    struct {
        hwp_ledDescp_t      descp;
    } led;

}hwp_swDescp_t;


typedef struct hwp_socDescp_s
{
    uint8           swDescp_index;      /* which swDescp the embedded CPU belong to, valid only if using embedded CPU */
    uint8           slaveInterruptPin;  /* which GPIO PIN connected to slave interrupt source, especially for two-chip cascade mode usage */
}hwp_socDescp_t;


/*
 * hardware profile structure.
 */
typedef struct hwp_hwProfile_s
{
    hwp_identifier_t        identifier;                             /* identifier of this hardware profile */
    hwp_socDescp_t          soc;                                    /* SoC (CPU/Peripheral) description */

    uint32                  sw_count;                               /* Total number of switch on the board */
    hwp_swDescp_t           *swDescp[RTK_MAX_NUM_OF_UNIT_LOCAL];    /* switch description */

    void                    *parsed_info;

}hwp_hwProfile_t;


/*
 * parsed information
 */
typedef struct hwp_unitInfo_s
{
    uint32                  hwp_chip_id;
    uint32                  hwp_chip_revision;
    uint32                  hwp_maxMacId;
    uint32                  hwp_minMacId;
    uint32                  hwp_maxMacId_w_oob;
    hwp_portDescp_t         *hwp_macID2PortDescp[RTK_MAX_PORT_PER_UNIT];
    uint8                   hwp_macID2SerdesID[RTK_MAX_PORT_PER_UNIT];
    uint8                   hwp_multiSdsBaseSds[RTK_MAX_SDS_PER_UNIT];      /* base sds Id of multi-serdes-port's serdes */
    uint32                  hwp_portCpuMacId;
    hwp_portCount_t         hwp_portCntEther;
    hwp_portCount_t         hwp_portCntCascade;
    hwp_portCount_t         hwp_portCntFe;
    hwp_portCount_t         hwp_portCntGe;
    hwp_portCount_t         hwp_portCnt2_5ge;
    hwp_portCount_t         hwp_portCnt5ge;
    hwp_portCount_t         hwp_portCntXge;
    hwp_portCount_t         hwp_portCntSXge;
    hwp_portCount_t         hwp_portCntCopper;
    hwp_portCount_t         hwp_portCntFiber;
    hwp_portCount_t         hwp_portCntCombo;
    hwp_portCount_t         hwp_portCntSerdes;
    hwp_portCount_t         hwp_portCntOob;
    rtk_portmask_t          hwp_attriPortMask[HWP_ATTRI_ID_END];
    rtk_portmask_t          hwp_ethTypePortMask[HWP_ETHTYPE_ID_END];
    rtk_portmask_t          hwp_mediumPortMask[HWP_MEDIUM_ID_END];
    rtk_portmask_t          hwp_allPortMask;
    uint8                   hwp_macID2phyAddr[RTK_MAX_PORT_PER_UNIT];
    phy_type_t              hwp_macID2phyType[RTK_MAX_PORT_PER_UNIT];
    phy_type_t              hwp_sdsID2phyType[RTK_MAX_SDS_PER_UNIT];
    uint8                   hwp_sdsID2MacID[RTK_MAX_SDS_PER_UNIT];
    hwp_serdesDescp_t       *hwp_sdsID2SerdesDescp[RTK_MAX_SDS_PER_UNIT];
    hwp_scDescp_t           *hwp_macID2SCDescp[RTK_MAX_PORT_PER_UNIT];
    uint8                   hwp_scBaseMacID[RTK_MAX_PORT_PER_UNIT];
    uint8                   hwp_phyBaseMacID[RTK_MAX_PORT_PER_UNIT];
    uint32                  hwp_ledNum[RTK_MAX_PORT_PER_UNIT];
    hwp_swDescp_t           *swDescp;
    void                    *parsed_info;
    uint32                  flag;

}hwp_unitInfo_t;

#define HWP_UNITMAPFLAG_LOCALCHIP  0x1                             /* the unit is on local board */
typedef struct hwp_unitMap_s
{
    hwp_unitInfo_t          *pUnitInfo;
    uint32                  flag;
}hwp_unitMap_t;

typedef struct hwp_parsedInfo_s
{
    hwp_hwProfile_t             *hwp;
    uint32                      hwp_localUnitCnt;           /* Total number of chip on this board */
    uint32                      hwp_embeddedCpuUnitId;      /* The unit ID of running CPU */
    uint32                      hwp_cascadeSlaveUnitID;     /* Slave unit ID of cascade board */
    uint32                      hwp_cascade_mode;           /* TRUE: native cascade mode support, now only support two 9300 cascade which one CPU as master and stacking ports cascaded) */
    hwp_unitInfo_t              unitInfo[RTK_MAX_NUM_OF_UNIT_LOCAL];
}hwp_parsedInfo_t;


/* per-unit hwp detached info */
typedef struct hwp_detachedInfo_s
{
    uint8                   removedMacId[RTK_MAX_PORT_PER_UNIT];
} hwp_detachedInfo_t;

typedef enum hwp_attachType_e
{
    HWP_ATTACH_TYPE_HWP = 0,
    HWP_ATTACH_TYPE_MAC,
    HWP_ATTACH_TYPE_PHY,
    HWP_ATTACH_TYPE_END
} hwp_attachType_t;

typedef union rtk_attachInfo_s
{
    /*
     * whole hwp info
     */
    struct
    {
        hwp_hwProfile_t     *hwp;
    } hwp;

    /*
     * per-chip mac info,
     * subset of hwp_swDescp_t
     */
    struct
    {
        hwp_swRegAccMethod_t    swcore_access_method;   /* how the switch/MAC registers were accessed */

        struct {
            hwp_portDescp_t     descp[RTK_MAX_PORT_PER_UNIT];
        } port;

        struct {
            hwp_serdesDescp_t   descp[RTK_MAX_SDS_PER_UNIT];
        } serdes;

        struct {
            hwp_ledDescp_t      descp;
        } led;
    } mac;

    /*
     * per-chip phy info,
     * subset of hwp_swDescp_t's phyDescp and phy connected portDescp/serdesDescp
     */
    struct
    {
        uint32  model;
        uint8   phy_max;

        uint8   medi;                           /* rt_port_medium_t */
        uint8   phy_addr[RTK_MAX_PORT_PER_PHY];

        uint8   sds_mode;                       /* rt_serdesMode_t */
    } phy;
} rtk_attachInfo_t;


/*
 * Data Declaration
 */
extern hwp_portDescp_t          portDescpEmpty;
extern hwp_serdesDescp_t        serdesDescpEmpty;
extern hwp_swDescp_t            swDescpEmpty;
extern hwp_hwProfile_t          hwpEmpty;
extern hwp_unitInfo_t           unitInfoEmpty;
extern hwp_parsedInfo_t         parsedInfoEmpty;
extern hwp_hwProfile_t          *hwp_myHwProfile;
extern hwp_unitMap_t            unitMapStruct[];
extern uint32                   hwp_useDefHwp;
extern uint32                   hwp_init_status;
extern int                      hwp_ExternalCpu;
extern uint32                   hwp_cpuArch;

/*
 * Symbol Definition
 */
#define HWP_NOT_INIT_YET()                      (hwp_init_status != INIT_COMPLETED)
#define UNITMAP(unit)                           (unitMapStruct[unit].pUnitInfo)
#define HWP_USEDEFHWP()                         (hwp_useDefHwp)

/* <<< profile >>> */
#define HWP_PARSED_INFO(unit)                   ((hwp_parsedInfo_t *)UNITMAP(unit)->parsed_info)
#define HWP_HWPROFILE(unit)                     (HWP_PARSED_INFO(unit)->hwp)
#define HWP_IDENTIFIER_ID()                     (hwp_myHwProfile->identifier.id)
#define HWP_IDENTIFIER_NAME()                   (hwp_myHwProfile->identifier.name)


/* <<< CPU information >>> */
#define HWP_CPU_EMBEDDED()                      (!hwp_ExternalCpu)
#define HWP_CPU_ARCH()                          (hwp_cpuArch)
#define HWP_MY_CHIP_ID()                        (UNITMAP(((hwp_parsedInfo_t *)hwp_myHwProfile->parsed_info)->hwp_embeddedCpuUnitId)->hwp_chip_id)
#define HWP_MY_CHIP_FAMILY_ID()                 (UNITMAP(((hwp_parsedInfo_t *)hwp_myHwProfile->parsed_info)->hwp_embeddedCpuUnitId)->hwp_chip_family_id) /* e.g. RTL8350_FAMILY_ID, RTL8390_FAMILY_ID, ... */
#define HWP_MY_UNIT_ID_BY_HWP(pHwp)             (((hwp_parsedInfo_t *)pHwp->parsed_info)->hwp_embeddedCpuUnitId)
#define HWP_MY_UNIT_ID()                        (HWP_MY_UNIT_ID_BY_HWP(hwp_myHwProfile))
#define HWP_IS_CPU_UNIT(unit)                   (((hwp_parsedInfo_t *)hwp_myHwProfile->parsed_info)->hwp_embeddedCpuUnitId==unit) /* check if the CPU of this unit is the main CPU of the board*/
#define HWP_CASCADE_SLAVE_UNIT_ID()             (((hwp_parsedInfo_t *)hwp_myHwProfile->parsed_info)->hwp_cascadeSlaveUnitID)
#define HWP_CASCADE_MODE()                      (((hwp_parsedInfo_t *)hwp_myHwProfile->parsed_info)->hwp_cascade_mode)
#define HWP_CASCADE_SLAVE_INTR_PIN()            (hwp_myHwProfile->soc.slaveInterruptPin)

/* <<< switch core information >>> */
#define HWP_SWITCH_DESCP(unit)                  (UNITMAP(unit)->swDescp)
#define HWP_SWCORE_SUPPORT(unit)                (HWP_SWITCH_DESCP(unit)->swcore_supported)
#define HWP_SWCORE_ACC_METHOD(unit)             (HWP_SWITCH_DESCP(unit)->swcore_access_method)
#define HWP_SWCORE_SPI_CHIP_SEL(unit)           (HWP_SWITCH_DESCP(unit)->swcore_spi_chip_select)
#define HWP_SWCORE_PCIE_BUS(unit)               (HWP_SWITCH_DESCP(unit)->swcore_pcie_bus_number)
#define HWP_NIC_SUPPORT(unit)                   (HWP_SWITCH_DESCP(unit)->nic_supported)

/* <<< unit ID information >>> */
#define HWP_SW_COUNT(unit)                      (HWP_HWPROFILE(unit)->sw_count)
#define HWP_UNIT_VALID_IN_RANGE(unit)           ((unit)<RTK_MAX_NUM_OF_UNIT) /* if the unit in the range: 0~(RTK_MAX_NUM_OF_UNIT-1) */
#define HWP_UNIT_VALID_LOCAL(unit)              (HWP_UNIT_VALID_IN_RANGE(unit)&&(unitMapStruct[unit].flag&HWP_UNITMAPFLAG_LOCALCHIP))
#define HWP_UNIT_VALID_STACK(unit)              (HWP_UNIT_VALID_IN_RANGE(unit)&&(UNITMAP(unit) != &unitInfoEmpty))
/* traverse all units in a hardware profile */
#define HWP_UNIT_TRAVS_BY_HWP(unit, pHwp)       for (unit = 0; unit < pHwp->sw_count; unit++)
/* Traverse all chips on local board */
#define HWP_UNIT_TRAVS_LOCAL(unit)              HWP_UNIT_TRAVS_BY_HWP(unit, hwp_myHwProfile)
/* Traverse all chips in whole stacking system, include the ones on local board */
#define HWP_UNIT_TRAVS_STACK(unit)              for (unit = 0; unit < RTK_MAX_NUM_OF_UNIT; unit++) \
                                                    if (HWP_UNIT_VALID_STACK(unit))


/* <<< LED information >>> */
#define HWP_PORT_LED_NUM(unit, mac_id)          (UNITMAP(unit)->hwp_ledNum[mac_id])
#define HWP_LED_IF(unit)                        (UNITMAP(unit)->swDescp->led.descp.led_if_sel)
#define HWP_LED_MOD(unit, mode, led_num)        (UNITMAP(unit)->swDescp->led.descp.led_definition_set[mode].led[led_num])
#define HWP_LED_COPR_SET(unit,mac_id)           (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->led_c)
#define HWP_LED_FIB_SET(unit,mac_id)            (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->led_f)
#define HWP_LED_LAYOUT(unit,mac_id)             (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->led_layout)
#define HWP_LED_ACTIVE(unit)                    (UNITMAP(unit)->swDescp->led.descp.led_active)
#define HWP_LED_SIGNAL_INVERT(unit)             (UNITMAP(unit)->swDescp->led.descp.led_signal_invert)


/* <<< port infomration >>> */
#define HWP_PORT_EXIST(unit,mac_id)             (((mac_id) <= RTK_MAX_PORT_ID_PER_UNIT) && (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]!=&portDescpEmpty) && (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]!=NULL) \
                                                  && !HWP_OOB_PORT(unit, mac_id))
#define HWP_PORT_EXIST_W_OOB(unit,mac_id)       (((mac_id) <= RTK_MAX_PORT_ID_PER_UNIT) && (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]!=&portDescpEmpty) && (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]!=NULL))
#define HWP_PMSK_EXIST(unit,portMask)           (hwp_portmaskValid_Chk(unit, portMask) == RT_ERR_OK)
#define HWP_PMSK_EXIST_ETHTYPE(unit,portMask, Type, opType)  (hwp_portmaskEthTypeValid_Chk(unit, portMask, Type, opType) == RT_ERR_OK)
#define HWP_PMSK_EXIST_ATTRI(unit,portMask, attri, opType)   (hwp_portmaskAttriValid_Chk(unit, portMask, attri, opType) == RT_ERR_OK)
#define HWP_PMSK_EXIST_MEDIUM(unit,portMask, medium, opType) (hwp_portmaskMediumValid_Chk(unit, portMask, medium, opType) == RT_ERR_OK)
#define HWP_PORT_IN_RANGE(unit,mac_id)          (HWP_UNIT_VALID_IN_RANGE(unit) && ((mac_id) <= RTK_MAX_PORT_ID_PER_UNIT))
#define HWP_PORT_COUNT(unit)                    (HWP_SWITCH_DESCP(unit)->port.count)                        /* total number of ports, include CPU, Stacking ports,... etc */
#define HWP_PHY_BASE_MACID(unit, port)          (UNITMAP(unit)->hwp_phyBaseMacID[port])
#define HWP_PHY_BASE_MACID_BY_IDX(unit,_phy)    (HWP_SWITCH_DESCP(unit)->phy.descp[_phy].mac_id)
#define HWP_PHY_PORTID(unit, port)              (port - (uint32)HWP_PHY_BASE_MACID(unit, port))
#define HWP_PHY_BASE_PHY_MAX(unit, port)        (HWP_SWITCH_DESCP(unit)->phy.descp[(UNITMAP(unit)->hwp_macID2PortDescp[port]->phy_idx)].phy_max)
#define HWP_PHY_MAX_PORT_NUM_BY_IDX(unit, phyIdx)   (HWP_SWITCH_DESCP(unit)->phy.descp[phyIdx].phy_max)
#define HWP_PORT_ATTRI(unit,mac_id)             (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->attr)
#define HWP_PORT_ETH(unit,mac_id)               (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->eth)
#define HWP_PORT_MEDI(unit,mac_id)              (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->medi)
#define HWP_PORT_SMI(unit,mac_id)               (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->smi)
#define HWP_PORT_PHY_IDX(unit, mac_id)          (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->phy_idx)
#define HWP_PHY_INTR_GPIO_DEV(unit, port)       (HWP_SWITCH_DESCP(unit)->phy.descp[(UNITMAP(unit)->hwp_macID2PortDescp[port]->phy_idx)].interruptSocGpioDev)
#define HWP_PHY_INTR_GPIO_PIN(unit, port)       (HWP_SWITCH_DESCP(unit)->phy.descp[(UNITMAP(unit)->hwp_macID2PortDescp[port]->phy_idx)].interruptSocGpioPin)

#define HWP_PORT_PHY_MDI_PIN_SWAP(unit, mac_id)         (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->phy_mdi_pin_swap)
#define HWP_PORT_PHY_MDI_PAIR_SWAP(unit, mac_id)        (UNITMAP(unit)->hwp_macID2PortDescp[mac_id]->phy_mdi_pair_swap)

#define HWP_CPU_MACID(unit)                     (UNITMAP(unit)->hwp_portCpuMacId)
#define HWP_IS_CPU_PORT(unit, mac_id)           ((HWP_PORT_ATTRI(unit, mac_id) & HWP_CPU)? 1:0)

#define HWP_PORT_TRAV_MACID_MAX(max)            ((max <= RTK_MAX_PORT_ID_PER_UNIT) ? max : RTK_MAX_PORT_ID_PER_UNIT)

#define HWP_OOB_PORT(unit, mac_id)              (HWP_PORT_ATTRI(unit, mac_id) & HWP_OOB)
#define HWP_OOB_PORT_COUNT(unit)                (UNITMAP(unit)->hwp_portCntOob.count)
#define HWP_OOB_PORT_TRAVS(unit, mac_id)         for (mac_id = UNITMAP(unit)->hwp_portCntOob.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntOob.max); mac_id++) \
                                                    if((mac_id <= RTK_MAX_PORT_ID_PER_UNIT) && HWP_OOB_PORT(unit, mac_id))

#define HWP_ETHER_PORT(unit, mac_id)            (HWP_PORT_ATTRI(unit, mac_id) & HWP_ETH)
#define HWP_ETHER_PORT_COUNT(unit)              (UNITMAP(unit)->hwp_portCntEther.count)
#define HWP_ETHER_PORT_TRAVS(unit, mac_id)      for (mac_id = UNITMAP(unit)->hwp_portCntEther.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntEther.max); mac_id++) \
                                                    if((mac_id <= RTK_MAX_PORT_ID_PER_UNIT) && HWP_ETHER_PORT(unit, mac_id))

#define HWP_UPLINK_PORT(unit, mac_id)           (hwp_portInUplink_check(RTK_CHIP_FAMILY_ID(unit), mac_id) == RT_ERR_OK)
#define HWP_UPLINK_PORT_COUNT(unit)             (hwp_portInUplink_count(RTK_CHIP_FAMILY_ID(unit)))
#define HWP_UPLINK_PORT_TRAVS(unit, mac_id)     for (mac_id = UNITMAP(unit)->hwp_portCntEther.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntEther.max); mac_id++) \
                                                    if(HWP_UPLINK_PORT(unit, mac_id))

#define HWP_CASCADE_PORT(unit, mac_id)           (HWP_PORT_ATTRI(unit, mac_id) & HWP_CASCADE)
#define HWP_CASCADE_PORT_COUNT(unit)            (UNITMAP(unit)->hwp_portCntCascade.count)
#define HWP_CASCADE_PORT_TRAVS(unit, mac_id)     for (mac_id = UNITMAP(unit)->hwp_portCntCascade.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntCascade.max); mac_id++) \
                                                    if(HWP_CASCADE_PORT(unit, mac_id))

#define HWP_FE_PORT(unit, mac_id)               (HWP_PORT_ETH(unit, mac_id) == HWP_FE)
#define HWP_FE_PORT_COUNT(unit)                 (UNITMAP(unit)->hwp_portCntFe.count)
#define HWP_FE_PORT_TRAVS(unit, mac_id)         for (mac_id = UNITMAP(unit)->hwp_portCntFe.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntFe.max); mac_id++) \
                                                    if(HWP_FE_PORT(unit, mac_id))

#define HWP_GE_PORT(unit, mac_id)               (HWP_PORT_ETH(unit, mac_id) == HWP_GE)
#define HWP_GE_PORT_COUNT(unit)                 (UNITMAP(unit)->hwp_portCntGe.count)
#define HWP_GE_PORT_TRAVS(unit, mac_id)         for (mac_id = UNITMAP(unit)->hwp_portCntGe.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntGe.max); mac_id++) \
                                                    if(HWP_GE_PORT(unit, mac_id))
#define HWP_2_5GE_PORT(unit, mac_id)            (HWP_PORT_ETH(unit, mac_id) == HWP_2_5GE)
#define HWP_2_5GE_PORT_COUNT(unit)              (UNITMAP(unit)->hwp_portCnt2_5ge.count)
#define HWP_2_5GE_PORT_TRAVS(unit, mac_id)      for (mac_id = UNITMAP(unit)->hwp_portCnt2_5ge.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCnt2_5ge.max); mac_id++) \
                                                   if(HWP_2_5GE_PORT(unit, mac_id))

#define HWP_5GE_PORT(unit, mac_id)            (HWP_PORT_ETH(unit, mac_id) == HWP_5GE)
#define HWP_5GE_PORT_COUNT(unit)              (UNITMAP(unit)->hwp_portCnt5ge.count)
#define HWP_5GE_PORT_TRAVS(unit, mac_id)      for (mac_id = UNITMAP(unit)->hwp_portCnt2_5ge.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCnt5ge.max); mac_id++) \
                                                   if(HWP_5GE_PORT(unit, mac_id))

#define HWP_10GE_PORT(unit, mac_id)             (HWP_PORT_ETH(unit, mac_id) == HWP_XGE)
#define HWP_10GE_PORT_COUNT(unit)               (UNITMAP(unit)->hwp_portCntXge.count)
#define HWP_10GE_PORT_TRAVS(unit, mac_id)       for (mac_id = UNITMAP(unit)->hwp_portCntXge.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntXge.max); mac_id++) \
                                                    if((mac_id <= RTK_MAX_PORT_ID_PER_UNIT) && HWP_10GE_PORT(unit, mac_id))

#define HWP_SXGE_PORT(unit, mac_id)             (HWP_PORT_ETH(unit, mac_id) == HWP_SXGE)
#define HWP_SXGE_PORT_COUNT(unit)               (UNITMAP(unit)->hwp_portCntSXge.count)
#define HWP_SXGE_PORT_TRAVS(unit, mac_id)       for (mac_id = UNITMAP(unit)->hwp_portCntSXge.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntSXge.max); mac_id++) \
                                                    if(HWP_SXGE_PORT(unit, mac_id))

#define HWP_COPPER_PORT(unit, mac_id)           ((HWP_PORT_MEDI(unit, mac_id)!=HWP_NONE) && HWP_PORT_MEDI(unit, mac_id) & HWP_COPPER)
#define HWP_COPPER_PORT_COUNT(unit)             (UNITMAP(unit)->hwp_portCntCopper.count)
#define HWP_COPPER_PORT_TRAVS(unit, mac_id)     for (mac_id = UNITMAP(unit)->hwp_portCntCopper.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntCopper.max); mac_id++) \
                                                    if(HWP_COPPER_PORT(unit, mac_id))

#define HWP_FIBER_PORT(unit, mac_id)            ((HWP_PORT_MEDI(unit, mac_id)!=HWP_NONE) && HWP_PORT_MEDI(unit, mac_id) & HWP_FIBER)
#define HWP_FIBER_PORT_COUNT(unit)              (UNITMAP(unit)->hwp_portCntFiber.count)
#define HWP_FIBER_PORT_TRAVS(unit, mac_id)      for (mac_id = UNITMAP(unit)->hwp_portCntFiber.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntFiber.max); mac_id++) \
                                                    if(HWP_FIBER_PORT(unit, mac_id))

#define HWP_COMBO_PORT(unit, mac_id)            ((HWP_PORT_MEDI(unit, mac_id)!=HWP_NONE) && HWP_PORT_MEDI(unit, mac_id) & HWP_COMBO)
#define HWP_COMBO_PORT_COUNT(unit)              (UNITMAP(unit)->hwp_portCntCombo.count)
#define HWP_COMBO_PORT_TRAVS(unit, mac_id)      for (mac_id = UNITMAP(unit)->hwp_portCntCombo.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntCombo.max); mac_id++) \
                                                    if(HWP_COMBO_PORT(unit, mac_id))

#define HWP_SERDES_PORT(unit, mac_id)           (((mac_id) <= RTK_MAX_PORT_ID_PER_UNIT) && (HWP_PORT_MEDI(unit, mac_id)!=HWP_NONE) && HWP_PORT_MEDI(unit, mac_id) & HWP_SERDES)
#define HWP_SERDES_PORT_COUNT(unit)             (UNITMAP(unit)->hwp_portCntSerdes.count)
#define HWP_SERDES_PORT_TRAVS(unit, mac_id)     for (mac_id = UNITMAP(unit)->hwp_portCntSerdes.min; mac_id <= HWP_PORT_TRAV_MACID_MAX(UNITMAP(unit)->hwp_portCntSerdes.max); mac_id++) \
                                                    if(HWP_SERDES_PORT(unit, mac_id))

#define HWP_10GE_SERDES_PORT(unit, mac_id)      (HWP_10GE_PORT(unit, mac_id)&&HWP_SERDES_PORT(unit, mac_id))

#define HWP_GET_ATTRI_PORTMASK(unit, attri_id, dstPortmask)     \
                                                do{\
                                                    if(attri_id < HWP_ATTRI_ID_END)\
                                                        dstPortmask = UNITMAP(unit)->hwp_attriPortMask[attri_id];\
                                                    else\
                                                        dstPortmask = UNITMAP(unit)->hwp_attriPortMask[0];\
                                                }while(0)

#define HWP_GET_ETHTYPE_PORTMASK(unit, ethType_id, dstPortmask) \
                                                do{\
                                                    if(ethType_id < HWP_ETHTYPE_ID_END)\
                                                        dstPortmask = UNITMAP(unit)->hwp_ethTypePortMask[ethType_id];\
                                                    else\
                                                        dstPortmask = UNITMAP(unit)->hwp_ethTypePortMask[0];\
                                                }while(0)

#define HWP_GET_MEDIUM_PORTMASK(unit, medium_id, dstPortmask)\
                                                do{\
                                                    if(medium_id < HWP_MEDIUM_ID_END)\
                                                        dstPortmask = UNITMAP(unit)->hwp_mediumPortMask[medium_id];\
                                                    else\
                                                        dstPortmask = UNITMAP(unit)->hwp_mediumPortMask[0];\
                                                }while(0)

#define HWP_GET_ALL_PORTMASK(unit, dstPortmask)\
                                                do{\
                                                    dstPortmask = UNITMAP(unit)->hwp_allPortMask;\
                                                }while(0)

/* Traverse all ports of the specified unit */
#define HWP_PORT_TRAVS(unit, mac_id)            for (mac_id = UNITMAP(unit)->hwp_minMacId; mac_id <= UNITMAP(unit)->hwp_maxMacId; mac_id++) \
                                                    if(HWP_PORT_EXIST(unit, mac_id))

#define HWP_PORT_TRAVS_W_OOB(unit, mac_id)      for (mac_id = UNITMAP(unit)->hwp_minMacId; mac_id <= UNITMAP(unit)->hwp_maxMacId_w_oob; mac_id++) \
                                                    if(HWP_PORT_EXIST_W_OOB(unit, mac_id))

/* Traverse all ports except CPU of the specified unit */
#define HWP_PORT_TRAVS_EXCEPT_CPU(unit, mac_id) for (mac_id = UNITMAP(unit)->hwp_minMacId; mac_id <= UNITMAP(unit)->hwp_maxMacId; mac_id++) \
                                                    if (HWP_PORT_EXIST(unit, mac_id) && !HWP_IS_CPU_PORT(unit, mac_id))

#define HWP_PORT_TRAVS_SAME_PHY(unit, basePort, phy_id, mac_id) for (mac_id = UNITMAP(unit)->hwp_minMacId; mac_id <= UNITMAP(unit)->hwp_maxMacId; mac_id++) \
                                                                    if ((HWP_PORT_EXIST(unit, mac_id) && !HWP_SERDES_PORT(unit, basePort) && (HWP_PORT_PHY_IDX(unit, mac_id) == phy_id)) \
                                                                        || (HWP_PORT_EXIST(unit, mac_id) && HWP_SERDES_PORT(unit, basePort) && (mac_id == basePort)))


/* <<< PHY information >>> */
#define HWP_PHY_EXIST(unit,mac_id)              (((mac_id) <= RTK_MAX_PORT_ID_PER_UNIT) && (UNITMAP(unit)->hwp_macID2PortDescp[mac_id] != NULL) && (HWP_PORT_PHY_IDX(unit, mac_id) != HWP_NONE))
#define HWP_PHY_ADDR(unit,mac_id)               (UNITMAP(unit)->hwp_macID2phyAddr[mac_id])
#define HWP_PHY_MODEL_BY_PHY(unit,phy_idx)      (HWP_SWITCH_DESCP(unit)->phy.descp[phy_idx].chip)
#define HWP_PHY_MODEL_BY_PORT(unit,mac_id)      (UNITMAP(unit)->hwp_macID2phyType[mac_id])
#define HWP_PHY_COUNT(unit)                     (HWP_SWITCH_DESCP(unit)->phy.count)
/* Traverse all phy of the specified unit */
#define HWP_PHY_TRAVS(unit, phy_num)            for (phy_num = 0; phy_num < HWP_SWITCH_DESCP(unit)->phy.count; phy_num++) \
                                                    if (HWP_SWITCH_DESCP(unit)->phy.descp[phy_num].mac_id < RTK_MAX_NUM_OF_SW_PORT)

#define HWP_PHY_TRAVS_W_OOB(unit, phy_num)      for (phy_num = 0; phy_num < HWP_SWITCH_DESCP(unit)->phy.count; phy_num++)


/* <<< SERDES information >>> */
#define HWP_SDS_EXIST(unit, sdsID)              (((sdsID) < RTK_MAX_SDS_PER_UNIT) && (UNITMAP(unit)->hwp_sdsID2SerdesDescp[sdsID] != &serdesDescpEmpty) && (UNITMAP(unit)->hwp_sdsID2SerdesDescp[sdsID] != NULL))
#define HWP_SDS_MODE(unit, sdsID)               (UNITMAP(unit)->hwp_sdsID2SerdesDescp[sdsID]->mode)
#define HWP_SDS_RX_POLARITY(unit, sdsID)        (UNITMAP(unit)->hwp_sdsID2SerdesDescp[sdsID]->rx_polarity)
#define HWP_SDS_TX_POLARITY(unit, sdsID)        (UNITMAP(unit)->hwp_sdsID2SerdesDescp[sdsID]->tx_polarity)
#define HWP_SDS_ID2PHYMODEL(unit, sdsID)        (UNITMAP(unit)->hwp_sdsID2phyType[sdsID])
#define HWP_SDS_ID2MACID(unit, sdsID)           (UNITMAP(unit)->hwp_sdsID2MacID[sdsID])
#define HWP_PORT_SDSID(unit, mac_id)            (UNITMAP(unit)->hwp_macID2SerdesID[mac_id])
#define HWP_MULTI_SDS_BASESDS(unit, sdsID)      (UNITMAP(unit)->hwp_multiSdsBaseSds[sdsID])

#define HWP_SDS_TRAVS(unit, sdsID)              for (sdsID = 0; sdsID < RTK_MAX_NUM_OF_SW_SDS; sdsID++)  \
                                                    if (HWP_SDS_EXIST(unit, sdsID))

#define HWP_PORT_SDS_TRAVS(unit, mac_id, sdsID)   for (sdsID=UNITMAP(unit)->hwp_macID2SerdesID[mac_id]; sdsID < RTK_MAX_NUM_OF_SW_SDS; sdsID++)    \
                                                    if (UNITMAP(unit)->hwp_sdsID2MacID[sdsId] == mac_id)

#define HWP_SDS_TRAVS_W_OOB(unit, sdsID)        for (sdsID = 0; sdsID < RTK_MAX_SDS_PER_UNIT; sdsID++)  \
                                                    if (HWP_SDS_EXIST(unit, sdsID))

#define HWP_OOB_SDS_TRAV(unit, sdsID)           for (sdsID = RTK_OOB_SDS_START; sdsID < RTK_OOB_SDS_END; sdsID++)  \
                                                    if (HWP_SDS_EXIST(unit, sdsID))


#define HWP_SC_EXIST(unit,mac_id)               (((mac_id) <= RTK_MAX_PORT_ID_PER_UNIT) && (UNITMAP(unit)->hwp_macID2SCDescp[mac_id]!=&scDescpEmpty) && (UNITMAP(unit)->hwp_macID2SCDescp[mac_id]!=NULL))
#define HWP_SC_TYPE(unit,mac_id)                (UNITMAP(unit)->hwp_macID2SCDescp[mac_id]->chip)
#define HWP_SC_SMI(unit,mac_id)                 (UNITMAP(unit)->hwp_macID2SCDescp[mac_id]->smi)
#define HWP_SC_ADDR(unit,mac_id)                (UNITMAP(unit)->hwp_macID2SCDescp[mac_id]->phy_addr)
#define HWP_SC_TX_POLAR(unit,mac_id)            (UNITMAP(unit)->hwp_macID2SCDescp[mac_id]->tx_polarity)
#define HWP_SC_RX_POLAR(unit,mac_id)            (UNITMAP(unit)->hwp_macID2SCDescp[mac_id]->rx_polarity)

/* for Unit-Test only */
#define HWP_UNITTEST_PORT_MACID_MIN(unit)       (UNITMAP(unit)->hwp_minMacId)
#define HWP_UNITTEST_PORT_MACID_MAX(unit)       (UNITMAP(unit)->hwp_maxMacId)
#define HWP_UNITTEST_ETHER_PORT_MACID_MIN(unit) (UNITMAP(unit)->hwp_portCntEther.min)
#define HWP_UNITTEST_ETHER_PORT_MACID_MAX(unit) (UNITMAP(unit)->hwp_portCntEther.max)


/* phy_index */
#define HWP_PHY_PORT_TRAV(unit, basePort, i, pMac_id)   for (i = 0; i < RTK_MAX_PORT_PER_PHY; i++) \
                                                            if (hwp_get_port_by_baseport_offset(unit, (uint8)basePort, (uint8)i, pMac_id) == RT_ERR_OK)


/* Function Name:
 *      hwp_identifier_get
 * Description:
 *      Get profile identifier from system. either a string or a integer.
 *      This is used to decide what hardware board the code is running on.
 * Input:
 *      None
 * Output:
 *      hwp_identifier_t *identifier
 * Return:
 *      RT_ERR_OK       - Got
 *      RT_ERR_FAILED   - Not found
 * Note:
 *      None
 */
int32
hwp_identifier_get(hwp_identifier_t *identifier);


/* Function Name:
 *      hwp_chipInfo_update
 * Description:
 *      Get chip revision and update to hardware profile database.
 * Input:
 *      unit                - unit id
 * Output:
 *      hwp_hwProfile_t     - Pointer of profile description struct.
 * Return:
 *      RT_ERR_OK         - if success
 *      RT_ERR_FAILED   -if failed
 * Note:
 *      None
 */
int32
hwp_chipInfo_update(uint32 unit);


/* Function Name:
 *      hwp_forceProfile
 * Description:
 *      Set the hardware profile of this board. Using the function will
 *      take priority over auto-probe.
 * Input:
 *      id1     - Hardware profile identifier
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Set finished
 * Note:
 *      None
 */
int32
hwp_forceProfile(hwp_identifier_t *id1);


/* Function Name:
 *      hwp_probe
 * Description:
 *      Probe what hardware profile this board belongs to
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Profile found
 *      RT_ERR_FAILED   - Not found
 * Note:
 *      None
 */
int32
hwp_probe(void);


/* Function Name:
 *      hwp_profileList_show
 * Description:
 *      Show available hardware profile list
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      None
 *
 * Note:
 *      None
 */
void
hwp_profileList_show(void);


/* Function Name:
 *      hwp_unit_attach
 * Description:
 *      Add unit into system
 * Input:
 *      type                - hwp attach type, i.e., whole hwp, per-chip mac, per-chip phy
 *      pIdentifier         - pointer of attach identifier which identifies where to attach
 *      pInfo               - point of attached info
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
hwp_unit_attach(hwp_attachType_t type, rtk_attachIdentifier_t *pIdentifier, rtk_attachInfo_t *pInfo);

/* Function Name:
 *      hwp_unit_detach
 * Description:
 *      Remove a unit from hardware profile database. Local unit should not be deleted.
 * Input:
 *      type                - hwp attach type, i.e., whole hwp, per-chip mac, per-chip phy
 *      pIdentifier         - pointer of attach identifier which identifies where to detach
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
hwp_unit_detach(hwp_attachType_t type, rtk_attachIdentifier_t *pIdentifier);

/* Function Name:
 *      hwp_init
 * Description:
 *      Initialize hardware profile related variables
 * Input:
 *      hwp             - hardware profile
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Init ok
 *      RT_ERR_FAILED   - Init failed
 * Note:
 *      None
 */
int32
hwp_init(hwp_hwProfile_t *hwp);


/* Function Name:
 *      hwp_portmaskAttriValid_Chk
 * Description:
 *      Check whether the input attribute of portmask is valid by chip.
 * Input:
 *      unit        - unit id
 *      portmask    - the portmask to be checked
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - valid portMask
 *      RT_ERR_FAILED - invalid portMask
 * Note:
 */
int32
hwp_portmaskAttriValid_Chk(uint32 unit, rtk_portmask_t *portmask, rt_port_attri_t PortMasktype, operationType_t type);

/* Function Name:
 *      hwp_portmaskEthTypeValid_Chk
 * Description:
 *      Check whether the input ether type of portmask is valid by chip.
 * Input:
 *      unit        - unit id
 *      portmask    - the portmask to be checked
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - valid portMask
 *      RT_ERR_FAILED - invalid portMask
 * Note:
 */
int32
hwp_portmaskEthTypeValid_Chk(uint32 unit, rtk_portmask_t *portmask, rt_port_ethType_t PortMasktype, operationType_t type);

/* Function Name:
 *      hwp_portmaskMediumValid_Chk
 * Description:
 *      Check whether the medium of portmask is valid by chip.
 * Input:
 *      unit        - unit id
 *      portmask    - the portmask to be checked
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - valid portMask
 *      RT_ERR_FAILED - invalid portMask
 * Note:
 */
int32
hwp_portmaskMediumValid_Chk(uint32 unit, rtk_portmask_t *portmask, rt_port_medium_t PortMasktype, operationType_t type);

/* Function Name:
 *      hwp_portmaskValid_Chk
 * Description:
 *      Check whether portmask is valid by chip.
 * Input:
 *      unit        - unit id
 *      portmask    - the portmask to be checked
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - valid portMask
 *      RT_ERR_FAILED - invalid portMask
 * Note:
 */
int32
hwp_portmaskValid_Chk(uint32 unit, rtk_portmask_t *portmask);

/* Function Name:
 *      hwp_profile_find
 * Description:
 *      Scan all hardware profile and get the matched one
 * Input:
 *      hwp_identifier_t *id        - the ID that try to match
 * Output:
 *      hwp_hwProfile_t **profile   - the matched the profile
 * Return:
 *      RT_ERR_OK       - Got
 *      RT_ERR_FAILED   - Not found
 * Note:
 *      None
 */
int32
hwp_profile_find(hwp_identifier_t *id, hwp_hwProfile_t **profile);

/* Function Name:
 *      hwp_unit_get_next
 * Description:
 *      Get first or next valid unit id information
 * Input:
 *      unit     - unit id.
 *                 unit is declared as integer here, so, if unit id is less than zero,
 *                 this API will return first available unit id.
 * Output:
 *      pUnitInfo - pointer to the unit information
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_NULL_POINTER     - input parameter may be null pointer
 *      RT_ERR_NOT_INIT         - The module is not initial
 *      RT_ERR_ENTRY_NOTFOUND   - There is no next entry according to input unit id.
 * Note:
 *      None
 */
int32
hwp_unit_get_next(int32 unit, int32 *pNextUnit);

/* Function Name:
 *      hwp_identifier_cmp
 * Description:
 *      Compare two identifiers. User/Customer could change the comparing method.
 *      Using string or integer type, depends on the "type" defined in id1.
 * Input:
 *      id1     -   the first hwp_identifier_t
 *      id2     -   the second hwp_identifier_t
 * Output:
 *      None
 * Return:
 *      TRUE    - same
 *      FALSE   - different
 * Note:
 *      None
 */
int32
hwp_identifier_cmp(hwp_identifier_t *id1, hwp_identifier_t *id2);


/* Function Name:
 *      hwp_parsedInfo_buildup
 * Description:
 *      Build up a new hwp_parsedInfo_t from a hardware profile
 * Input:
 *      hwp_hwProfile_t     - which hardware profile will be build
 * Output:
 *      hwp_parsedInfo_t *  - return the result
 * Return:
 *      RT_ERR_OK       - if success
 *      RT_ERR_FAILED   - if failed
 * Note:
 *      None
 */
int32
hwp_parsedInfo_buildup(hwp_parsedInfo_t **p, hwp_hwProfile_t *hwp);

/* Function Name:
 *      _hwp_unitInfo2unitIdx_get
 * Description:
 *      Give a hwp_unitInfo_t, and find out which local unit it belongs to
 * Input:
 *      *unit_info   - the pointer of hwp_unitInfo_t
 * Output:
 *      *id         - the local unit ID of this unit_info
 * Return:
 *      RT_ERR_OK       - if success
 *      RT_ERR_FAILED   - if failed
 * Note:
 *      None
 */
int32
_hwp_unitInfo2unitIdx_get(uint32 *id, hwp_unitInfo_t *unit_info);


/* Function Name:
 *      hwp_chipInfo_get
 * Description:
 *      Get chip info from hwp
 * Input:
 *      chipType            - rtk_chip_t
 *      unit                - unit id
 *      phy_idx             - per-unit phy slot index
 * Output:
 *      pInfo               - pointer of rtk_attachInfo_t
 * Return:
 *      RT_ERR_OK           - get ok
 *      RT_ERR_FAILED       - get failed
 * Note:
 *      None
 */
int32
hwp_chipInfo_get(rtk_chip_t chipType, rtk_attachIdentifier_t *pIdentifier, rtk_attachInfo_t *pInfo);

int32
hwp_swSdsPortInfo_get(rtk_chip_t chipType, uint32 unit, uint32 macId, rtk_attachInfo_t *pInfo);

/* Function Name:
 *      hwp_portInUplink_check
 * Description:
 *      Check a port number if in the range of UPLINK port
 *      (uplink port has a special definition here: the last 4 ports of 9300/9310)
 * Input:
 *      unit        - Family ID
 *      port        - port ID
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - in range
 *      RT_ERR_FAILED   - out of range
 * Note:
 *      None
 */
extern int32
hwp_portInUplink_check(uint32 family_id, uint32 port);

/* Function Name:
 *      _hwp_portInUplinkPortmask_check
 * Description:
 *      Check a portmask if all ports are in the range of UPLINK port
 * Input:
 *      family_id   - chip family ID
 *      portmask    - portmask for checking
 * Output:
 *      None
 * Return:
 *     RT_ERR_OK
 *     RT_ERR_FAILED
 * Note:
 *      None
 */
extern int32
hwp_portInUplinkPortmask_check(uint32 family_id, rtk_portmask_t *portmask);

/* Function Name:
 *      _hwp_portInUplink_count
 * Description:
 *      return number of uplink ports of the chip
 *      (uplink port has a special definition here: the last 4 ports of 9300/9310)
 * Input:
 *      family_id   - chip family ID
 * Output:
 *      None
 * Return:
 *      Number of uplink ports
 * Note:
 *      None
 */
extern int32
hwp_portInUplink_count(uint32 family_id);

#endif /* __HW_PROFILE_H__ */

