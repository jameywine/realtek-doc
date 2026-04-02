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
 * $Revision: 83222 $
 * $Date: 2017-11-08 13:43:47 +0800 (Wed, 08 Nov 2017) $
 *
 * Purpose : Define the PHY hardware related function
 *
 * Feature : SDK PHY reset IOAL
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#include <common/rt_error.h>
#include <common/debug/rt_log.h>

#include <hwp/hw_profile.h>
#include <osal/lib.h>
#include <common/util/rt_util_system.h>
#ifdef CONFIG_SDK_HW_PROFILE_PROBE_GPIO
/* SENAO Support GPIO board*/
#include <private/drv/swcore/chip_probe.h>
#include <hwp/hw_profiles/hwp_senao_gpio_board.c>
#endif
#ifdef __BOOTLOADER__
#include <command.h>
#endif

/*
 * Symbol Definition
 */


/*
 * Macro Definition
 */

/*
 * Data Declaration
 */



/*
 * Function Declaration
 */


#if !defined(CONFIG_SDK_EXTERNAL_CPU)
#ifdef CONFIG_SDK_HW_PROFILE_PROBE_GPIO
/* Function Name:
 *      hwp_gpio_customer_identifier_get
 * Description:
 *      Get profile identifier from specific GPIO pins and treats then as integer.
 *      This is used to decide what hardware board the code is running on.
 *      Customer should modify this content of the API for thier design.
 * Input:
 *      None
 * Output:
 *      identifier.id
 * Return:
 *      RT_ERR_OK       - Got
 *      RT_ERR_FAILED   - Not found
 * Note:
 *      None
 */
static int32
hwp_gpio_customer_identifier_get(hwp_identifier_t *identifier)
{
    int unit = HWP_DEFHWP_UNIT_ID;
    uint32 family_id, chip_id, chip_rev;
    static uint32 pin_value;
#if EXT_GPIO_TEST
    static uint32 advance_pin_value;
#endif
    int  chipid = 0;
#ifdef __BOOTLOADER__   
    char *DefaultBoardModel; 
    char *DefaultMagicNum; 
    char BoardModel[64];
    char magicnumber[64];
    int  EnvironmentValueChange = 0;
    hwp_senao_gpio_board_t board_93XX_tmp[HWP_SENAO_GPIO_MAX_NUM];
    
    char gpio_string[HWP_PROFILE_NAME_LEN_MAX];
    uint32 Force_pin_value=0xFEFE;
    uint32 Force_advance_pin_value=0xFEFE;
    uint8 HexValue;
    int32 Ret;

#if EXT_GPIO_TEST
    if (getenv ("gpioext") != NULL)  {
      memset(gpio_string,0,sizeof(gpio_string));
      rt_util_flashEnv_get("gpioext",  gpio_string, HWP_PROFILE_NAME_LEN_MAX);
    
      Ret = rt_util_ascii2hex((char *)&gpio_string[0],&HexValue);
      if (Ret ==  RT_ERR_OK)  {
        Force_advance_pin_value = HexValue;
        Ret = rt_util_ascii2hex((char *)&gpio_string[1],&HexValue);
        if (Ret ==  RT_ERR_OK)  {
          Force_advance_pin_value = Force_advance_pin_value << 4;
          Force_advance_pin_value += HexValue;        
        }
      } 
      RT_INIT_MSG("    Get Software_gpio_ext_pin 0x%x\n", Force_advance_pin_value);
    }
    //#endif
#endif

    if (getenv ("gpioint") != NULL)  {
      memset(gpio_string,0,sizeof(gpio_string));
      rt_util_flashEnv_get("gpioint",  gpio_string, HWP_PROFILE_NAME_LEN_MAX);

      Ret = rt_util_ascii2hex((char *)&gpio_string[0],&HexValue);
      if (Ret ==  RT_ERR_OK)  {
        Force_pin_value = HexValue;
        Ret = rt_util_ascii2hex((char *)&gpio_string[1],&HexValue);
        if (Ret ==  RT_ERR_OK)  {
          Force_pin_value = Force_pin_value << 4;
          Force_pin_value += HexValue;
        }
      } 
      RT_INIT_MSG("    Get Software_gpio_int_pin 0x%x\n", Force_pin_value);
    }
	
    memset(BoardModel,0,sizeof(BoardModel));
    memset(magicnumber,0,sizeof(magicnumber));
#endif
    rt_util_boardID_GPIO_init();
    while(1)
    {
        rt_util_boardID_GPIO_get(&pin_value);
        RT_LOG(LOG_INFO, (MOD_INIT), "\nhwp_gpio_customer_identifier_get, GPIO = %d",pin_value);
        identifier->id = pin_value;
        break;
    }

    /*Customers implement this API by GPIO pin*/
    drv_swcore_cid_get(unit, &chip_id, &chip_rev);
    family_id = FAMILY_ID(chip_id);

    switch(family_id)
    {
        case RTL9300_FAMILY_ID:
            osal_memcpy(board_93XX_tmp, senao_customer_gpio_board_info.board_930X, sizeof(senao_customer_gpio_board_info.board_930X));
        break;
        case RTL9310_FAMILY_ID:
            osal_memcpy(board_93XX_tmp, senao_customer_gpio_board_info.board_931X, sizeof(senao_customer_gpio_board_info.board_931X));
        break;
        default:
            RT_LOG(LOG_INFO, (MOD_INIT), "\n Family_id ID = %x not Support,please add",family_id);
            return RT_ERR_FAILED;
        break;
    }

    if (Force_pin_value != 0xFEFE)  {
      pin_value = Force_pin_value;
    }
    for(chipid = 0 ; chipid < HWP_SENAO_GPIO_MAX_NUM; chipid++)
    {
        if (board_93XX_tmp[chipid].gpio_id == pin_value || board_93XX_tmp[chipid].gpio_id == HWP_END)
        {
#if EXT_GPIO_TEST
            if (board_93XX_tmp[chipid].gpio_id == HWP_SENAO_INTERNAL_GPIO_ID || board_93XX_tmp[chipid].gpio_id == HWP_SENAO_EXTERNAL_GPIO_ID )
            {
                if (board_93XX_tmp[chipid].gpio_id == HWP_SENAO_INTERNAL_GPIO_ID)
                    rt_util_boardID_ADVGPIO_get(&advance_pin_value, HWP_SENAO_INTERNAL_GPIO, family_id);
                else
                    rt_util_boardID_ADVGPIO_get(&advance_pin_value, HWP_SENAO_EXTERNAL_GPIO, family_id);

                if (Force_advance_pin_value != 0xFEFE)  {
                  advance_pin_value = Force_advance_pin_value;
                }

                RT_LOG(LOG_INFO, (MOD_INIT), "\nhwp_gpio_customer_identifier_get, ADVANCE GPIO = %d",advance_pin_value);

                if(board_93XX_tmp[chipid].advance_gpio_id == advance_pin_value )
                {
                    identifier->id = board_93XX_tmp[chipid].id;
        #ifdef __BOOTLOADER__
                    strncpy(BoardModel,  board_93XX_tmp[chipid].BoardModel,  strlen(board_93XX_tmp[chipid].BoardModel));
                    strncpy(magicnumber, board_93XX_tmp[chipid].magicnumber, strlen(board_93XX_tmp[chipid].magicnumber));
        #endif
                    break;
                }
            }else
#endif
            {
                identifier->id = board_93XX_tmp[chipid].id;
    #ifdef __BOOTLOADER__
                strncpy(BoardModel,  board_93XX_tmp[chipid].BoardModel,  strlen(board_93XX_tmp[chipid].BoardModel));
                strncpy(magicnumber, board_93XX_tmp[chipid].magicnumber, strlen(board_93XX_tmp[chipid].magicnumber));
    #endif
                break;
            }                    
        }
    }
    if (chipid < HWP_SENAO_GPIO_MAX_NUM) {
        if (board_93XX_tmp[chipid].gpio_id != HWP_END) 
            RT_INIT_MSG("  Hardware-profile gpio ID =%d probe(gpio:int=0x%x, ext=0x%x)\n", chipid, pin_value, advance_pin_value);
        else
            RT_INIT_MSG("  Hardware-profile gpio ID =%d can not probe(gpio:int=0x%x, ext=0x%x), use defaulu profile, please check Hardware-profile\n", chipid, pin_value, advance_pin_value);
    } else
        RT_INIT_MSG("  Hardware-profile gpio ID =%d can not probe(gpio:int=0x%x, ext=0x%x), please check Hardware-profile\n", chipid, pin_value, advance_pin_value);


#ifdef __BOOTLOADER__
#ifdef CONFIG_SENAO_GPIO_INIT
    rt_util_GPIO_init(identifier->id);
#endif

    DefaultBoardModel = getenv ("boardmodel");
    if(strlen(BoardModel) > 1 && strcmp(DefaultBoardModel,BoardModel) != 0 )
    {
        setenv ("boardmodel", BoardModel);
        EnvironmentValueChange =1;
    }

    DefaultMagicNum = getenv ("magic_number");
    if(strlen(magicnumber) > 1 && strcmp(DefaultMagicNum,magicnumber) != 0 )
    {
        setenv ("magic_number", magicnumber);
        EnvironmentValueChange =1;
    }
    if(EnvironmentValueChange)
    {
        saveenv();
    }
#endif
    return RT_ERR_OK;
}


/* Function Name:
 *      _ioal_param_hwp_gpio_identifier_get
 * Description:
 *      Get profile identifier from specific GPIO pins and treats then as integer.
 *      This is used to decide what hardware board the code is running on.
 *      Customer should modify this content of the API for thier design.
 * Input:
 *      None
 * Output:
 *      identifier.id
 * Return:
 *      RT_ERR_OK       - Got
 *      RT_ERR_FAILED   - Not found
 * Note:
 *      None
 */
static int32
_ioal_param_hwp_gpio_identifier_get(hwp_identifier_t *identifier)
{
    if(hwp_gpio_customer_identifier_get(identifier) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, MOD_INIT, "Could not get hardware board ID!");
        return RT_ERR_FAILED;
    }
    return RT_ERR_OK;
}
#endif
#endif /* #if !defined(CONFIG_SDK_EXTERNAL_CPU) */


#if !defined(CONFIG_SDK_EXTERNAL_CPU)
/* Function Name:
 *      _ioal_param_hwp_type_get
 * Description:
 *      Get profile identifier type from system. either a string or a integer.
 *      This is used to decide what hardware board the code is running on.
 * Input:
 *      None
 * Output:
 *      hwp_identifier_t *identifier
 * Return:
 *      None
 * Note:
 *      None
 */
static void
_ioal_param_hwp_type_get(hwp_identifier_t *identifier)
{
    char type[HWP_PROFILE_NAME_LEN_MAX];
    char *pType = type;

    osal_memset(type, 0, HWP_PROFILE_NAME_LEN_MAX);

    /*Default type is string.*/
    identifier->type = HWP_ID_TYPE_STRING;

#ifdef CONFIG_SDK_HW_PROFILE_PROBE_UBOOT_ENV
    if (RT_ERR_OK != rt_util_flashEnv_get("idType", pType, HWP_PROFILE_NAME_LEN_MAX))
    {
        return;
    }
#endif

    if ((osal_strncmp((const char*)pType, "integer", HWP_PROFILE_NAME_LEN_MAX)) != 0)
    {
        return;
    }
    identifier->type = HWP_ID_TYPE_INTEGER;

    return;
}
#ifdef CONFIG_SDK_HW_PROFILE_PROBE_UBOOT_ENV
/* Function Name:
 *      _ioal_param_hwp_boardIdString_to_int
 * Description:
 *      transfer boardId string to integer
 * Input:
 *      None
 * Output:
 *      hwp_identifier_t *identifier
 * Return:
 *      None
 * Note:
 *      None
 */
static int32
_ioal_param_hwp_boardIdString_to_int(char *board_id, int *p_id_int)
{
    int     index;
    char    val;
    const   char base = 10;

    *p_id_int = 0;
    for(index = 0; index < HWP_PROFILE_NAME_LEN_MAX; index++)
    {
        if(RT_ERR_OK == rt_util_ascii2dec((char *)&board_id[index],(uint8*)&val))
        {
            *p_id_int = *p_id_int * base + val;
        }
        else
        {
            break;
        }
    }
    return RT_ERR_OK;
}
#endif /*CONFIG_SDK_HW_PROFILE_PROBE_UBOOT_ENV*/
#endif /* #if !defined(CONFIG_SDK_EXTERNAL_CPU) */

/* Function Name:
 *      ioal_param_hwpIdKey_get
 * Description:
 *      Each h/w board has its own ID (we call hardware profile ID), either integer or string.
 *      Please put your own code here according to your hardware design.
 *
 *      Typical design may be (1)store hardware profile ID in EEPROM, (2) read from GPIO PINs, (3) or store in NVRAM, whatever.
 *
 *      How-To-implement:
 *          (a) Fill identifier->type to be HWP_ID_TYPE_STRING or HWP_ID_TYPE_INTEGER, if your board ID is by string or integer
 *          (b) Put the ID to identifier->name, if type is HWP_ID_TYPE_STRING, OR
 *              put the ID to identifier->id, if type is HWP_ID_TYPE_INTEGER
 *
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
ioal_param_hwpIdKey_get(hwp_identifier_t *identifier)
{
#if defined(CONFIG_SDK_EXTERNAL_CPU)

    identifier->type = HWP_ID_TYPE_STRING;


    if(identifier->type == HWP_ID_TYPE_STRING)
    {
//        osal_strcpy(identifier->name, "RTL9313_12XGE_PCI");
//        osal_strcpy(identifier->name, "RTL9311_6x8218D_6XGE_PCI");
        osal_strcpy(identifier->name, "RTL9311R_PCI");
    }
    else
    {
        identifier->id = HWP_RTL9313_12XGE;
    }

    return RT_ERR_OK;

#else

    int32   ret;

    _ioal_param_hwp_type_get(identifier);

    /* Get profile name,
     * according to board design, can be stored in EEPROM, FLASH, GPIO pins, or any others
     */

#ifdef CONFIG_SDK_HW_PROFILE_PROBE_GPIO
    RT_INIT_MSG("  hwp probe gpio \n");
    if((ret = _ioal_param_hwp_gpio_identifier_get(identifier)) != RT_ERR_OK)
    {
        RT_ERR(RT_ERR_FAILED, MOD_INIT, "Could not get hardware board ID!");
        return RT_ERR_FAILED;
    }
    identifier->type = HWP_ID_TYPE_INTEGER;

#elif defined CONFIG_SDK_HW_PROFILE_PROBE_UBOOT_ENV
    RT_INIT_MSG("  hwp probe uboot env \n");
    if(identifier->type != HWP_ID_TYPE_INTEGER)
    {
        if (RT_ERR_OK != (ret = rt_util_flashEnv_get("boardmodel",  identifier->name, HWP_PROFILE_NAME_LEN_MAX)))
            return RT_ERR_FAILED;
    }
    else
    {
        char    board_id[HWP_PROFILE_NAME_LEN_MAX];

       if (RT_ERR_OK != (ret = rt_util_flashEnv_get("boardid",  board_id, HWP_PROFILE_NAME_LEN_MAX)))
            return RT_ERR_FAILED;

        if (_ioal_param_hwp_boardIdString_to_int(board_id, &identifier->id) != RT_ERR_OK)
            return RT_ERR_FAILED;
    }
#endif /* end of CONFIG_SDK_HW_PROFILE_PROBE_UBOOT_ENV */

    return ret;
#endif /* #if defined(CONFIG_SDK_EXTERNAL_CPU) */
}


/* Function Name:
 *      ioal_param_ledInitFlag_get
 * Description:
 *      Port's link/speed/... LED can be controled by the switch chip through RTL8231.
 *      SDK will initial related registers according to hardware profile's LED config.
 *
 *      You can skip the LED initial by setting *ledModeInitSkip = LEDMODEINITSKIP_YES
 *      Set *ledModeInitSkip = LEDMODEINITSKIP_NO, SDK will do the intitial.
 * Input:
 *      None
 * Output:
 *      ledModeInitSkip   - LEDMODEINITSKIP_NO or LEDMODEINITSKIP_YES.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *    None
 */
int32
ioal_param_ledInitFlag_get(uint8 *ledModeInitSkip)
{
#if defined(CONFIG_SDK_EXTERNAL_CPU)

    *ledModeInitSkip = LEDMODEINITSKIP_NO;
    return RT_ERR_OK;

#else

    *ledModeInitSkip = LEDMODEINITSKIP_NO;
    return rt_util_ledInitFlag_get(ledModeInitSkip);

#endif /* #if defined(CONFIG_SDK_EXTERNAL_CPU) */
}


/* Function Name:
 *      ioal_param_sysMac_get
 * Description:
 *      Get System MAC address.
 * Input:
 *      pMacAddr   - mac address buffer
 * Output:
 *      pMacAddr   - mac address
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *    None
 */
int32
ioal_param_sysMac_get(rtk_mac_t *pMacAddr)
{
#if defined(CONFIG_SDK_EXTERNAL_CPU)

    pMacAddr->octet[5] = 0x00;
    pMacAddr->octet[4] = 0xE0;
    pMacAddr->octet[3] = 0x4C;
    pMacAddr->octet[2] = 0x00;
    pMacAddr->octet[1] = 0x00;
    pMacAddr->octet[0] = 0x00;
    return RT_ERR_OK;

#else
    return rt_util_sysMac_get(pMacAddr);
#endif
}



/* Function Name:
 *      ioal_param_phyXfmrRj45Impd_get
 * Description:
 *      impedance between transformer and RJ45 a.k.a copper MDI differential pair impedance for CS patch.
 *      This API will be called by RTL8218D chip initial flow.
 *      The initial value of some registers will be different according to the different impedance level on the board.
 *
 *      Consult your HW what enum in rtk_phy_xfmrRj45Impd_t shall be used for your board.
 *      Different board may have to return differnt value in this API.
 * Input:
 *      unit        - unit id
 * Output:
 *      pImpd - enum value of impedance level.
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *    Apply for PHY RTL8218D
 */
int32
ioal_param_phyXfmrRj45Impd_get(uint32 unit, rtk_phy_xfmrRj45Impd_t *pImpd)
{

    *pImpd = RTK_PHY_XFMRRJ45IMPD_100_OHM;

    return RT_ERR_OK;
}




