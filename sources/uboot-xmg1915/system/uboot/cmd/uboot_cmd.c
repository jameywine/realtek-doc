/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : RTL83xx SOC and SWCORE commands for U-Boot.
 *
 * Feature :
 *
 */


/*
 * Include Files
 */
#include <config.h>
#include <common.h>
#include <command.h>
#include <common/util.h>
#include <common/type.h>
#include <common/rt_autoconf.h>
#include <uboot/cmd/uboot_cmd.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */


/* Function Name:
 *      do_rtk
 * Description:
 *      Main function of RTK commands.
 * Input:
 *      cmdtp, flag, argc, argv handled by the parser
 * Output:
 *      None
 * Return:
 *      0    - Command is executed.
 *      >=1  - Command syntax error or an error occur during execution of the function.
 * Note:
 *      None
 */
int do_rtk(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{


    uint32 port;
    uint32 led_index;
    uint32 id;

#ifdef CONFIG_SDK_DRIVER_GPIO
    uint pinNum;
    uint pinStatus;
    uint devAddress;
#endif /* CONFIG_SDK_DRIVER_GPIO */

    if (argc < 2)
    {
        goto usage;
    }


    /* misc init */
    if (0 == strcmp(argv[1], "init"))
    {
        uboot_sdk_init(UBOOT_SDK_INIT_PHASE_RTK);
        return 0;
    }

    if (0 == strcmp(argv[1], "rtcore-init"))        /* phase  */
    {
        uboot_sdk_init(UBOOT_SDK_INIT_PHASE_RTCORE);
        return 0;
    }


    if (0 == strcmp(argv[1], "network"))
    {
        if (0 == strcmp(argv[2], "on"))        /* network on */
        {
            printf("Enable network\n");
            rtk_network_on();

            return 0;
        }
        else if (0 == strcmp(argv[2], "off"))  /* network off */
        {
            printf("Disable network\n");
            rtk_network_off();

            return 0;
        }

        goto usage;
    }

    if (0 == strcmp(argv[1], "show"))
    {
        if (0 == strcmp(argv[2], "hw_profile_list"))
        {
            rtk_hwProfileList_show();

            return 0;
        }

        goto usage;

    }

    /* comboport */
    if (0 == strcmp(argv[1], "comboport"))
    {
        if (argc < 3)
            goto usage;

        if (0 == strcmp(argv[2], "copper"))        /* comboport copper */
        {
            printf("ComboPort: Copper mode\n");
            rtk_comboport_copper();

            return 0;
        }
        else if (0 == strcmp(argv[2], "fiber"))  /* comboport fiber */
        {
            printf("ComboPort: Fiber mode\n");
            rtk_comboport_fiber();

            return 0;
        }
#if defined(CONFIG_SDK_RTL8390)
        else if (0 == strcmp(argv[2], "auto"))
        {
            printf("ComboPort: Auto mode\n");
            rtk_comboport_auto();
            return 0;
        }
        else if (0 == strcmp(argv[2], "port"))  /* per port */
        {
            int portid;

            if (argc < 5)
                goto usage;

            portid = simple_strtoul(argv[3], NULL, 10);
            if (0 == strcmp(argv[4], "copper"))        /* comboport copper */
            {
                printf("ComboPort %d: Copper mode\n", portid);
                rtk_comboport_portcopper(portid);

                return 0;
            }
            else if (0 == strcmp(argv[4], "fiber"))  /* comboport fiber */
            {
                printf("ComboPort %d: Fiber mode\n", portid);
                rtk_comboport_portfiber(portid);

                return 0;
            }
        }
#endif
        goto usage;
    }


#if defined(CONFIG_SDK_RTL8390)
    if ((0 == strcmp(argv[1], "sfp-speed")) && (0 == strcmp(argv[2], "set")))
    {
        int port, speed;

        if (argc < 5)
            goto usage;

        port = simple_strtoul(argv[3], NULL, 10);
        speed = simple_strtoul(argv[4], NULL, 10);
        rtk_sfp_speed_set(port, speed);
        return 0;
    }
    else if ((0 == strcmp(argv[1], "sys-esd")))
    {
        if (argc < 3)
            goto usage;
        if (0 == strcmp(argv[2], "on"))
        {
            rtk_sysEsd_set(1);
        }
        else
        {
            rtk_sysEsd_set(0);
        }

        return 0;
    }
    else if ((0 == strcmp(argv[1], "parameter")) && (0 == strcmp(argv[2], "version")))
    {
        rtk_parameter_version_show();
        return 0;
    }
#endif/* CONFIG_SDK_RTL8390 */

    if(0 == strcmp(argv[1], "port-isolation"))
    {
        if(0 == strcmp(argv[2], "on"))
        {
            rtk_port_isolation_on();
            return 0;
        }
        else if(0 == strcmp(argv[2], "off"))
        {
            rtk_port_isolation_off();
            return 0;
        }
        goto usage;
    }
#ifdef CONFIG_EEE
        /* eee */
    if (0 == strcmp(argv[1], "eee"))
    {
        if (0 == strcmp(argv[2], "on"))        /* eee on */
        {
            printf("Enable EEE function\n");
            rtk_eee_on();

            return 0;
        }
        else if (0 == strcmp(argv[2], "off"))  /* eee off */
        {
            printf("Disable EEE function\n");
            rtk_eee_off();

            return 0;
        }

        goto usage;
    }
#endif

#ifdef CONFIG_SDK_DRIVER_GPIO
    /*get internal GPIO pin status*/
    if (strcmp(argv[1], "pinGet") == 0)
    {
        if (argc < 3)
        {
            printf("Usage: rtk pinGet <pinNum> \n pinNum: 0~7\n\n");
            return 1;
        }
        else
        {
            pinNum = simple_strtoul(argv[2], NULL, 10);

            if(0 != rtk_intGpio_pin_get(pinNum,&pinStatus))
            {
                printf("The pinNum (%d) is invalid.\n", pinNum);
                return 1;
            }
            printf("pin%d:\t%d\n\n",pinNum,pinStatus);
            return 0;
        }
    }

/*set internal GPIO pin status*/
    if (strcmp(argv[1], "pinSet") == 0)
    {
        if (argc < 4)
        {
            printf("Usage: rtk pinSet <pinNum> <status> \n pinNum: 0~7, status: 0/1\n\n");
            return 1;
        }
        else
        {
            pinNum = simple_strtoul(argv[2], NULL, 10);
            pinStatus = simple_strtoul(argv[3], NULL, 10);

            if(pinStatus > 1)
            {
                printf("The status (%d) is invalid.\n", pinStatus);
                return 1;
            }
            else
            {
                if(0 != rtk_intGpio_pin_set(pinNum, pinStatus))
                {
                    printf("The pinNum (%d) is invalid.\n", pinNum);
                    return 1;
                }
                printf("pin%d:\t%d\n\n",pinNum,pinStatus);
                return 0;
            }
        }
    }
#endif /* CONFIG_SDK_DRIVER_GPIO */

#ifdef CONFIG_SDK_DRIVER_GPIO
    if (strcmp(argv[1], "ext-devInit") == 0)
    {
        if (argc < 3)
        {
            printf("Usage: rtk ext-devInit <devAddress> \n devAddress: 0~31\n\n");
            return 1;
        }
        else
        {
            devAddress = simple_strtoul(argv[2], NULL, 10);

            if(0 != rtk_extGpio_device_init(devAddress))
            {
                printf("The generalCtrl GPIO device init failed.\n");
                return 1;
            }
            printf("devAddress:\t%d\n\n",devAddress);
            return 0;
        }
    }


    if (strcmp(argv[1], "ext-pinGet") == 0)
    {
        if (argc < 3)
        {
            printf("Usage: rtk ext-pinGet <pinNum> \n pinNum: 0~36\n\n");
            return 1;
        }
        else
        {
            pinNum = simple_strtoul(argv[2], NULL, 10);

            if(0 != rtk_extGpio_pin_get(pinNum, &pinStatus))
            {
                printf("The pinNum (%d) is invalid.\n", pinNum);
                return 1;
            }
            printf("pin%d:\t%d\n\n",pinNum,pinStatus);
            return 0;
        }
    }

    /*set external 8231 pin status*/
    if (strcmp(argv[1], "ext-pinSet") == 0)
    {
        if (argc < 4)
        {
            printf("Usage: rtk ext-pinSet <pinNum> <status> \n pinNum: 0~36, status: 0/1\n\n");
            return 1;
        }
        else
        {
            pinNum = simple_strtoul(argv[2], NULL, 10);
            pinStatus = simple_strtoul(argv[3], NULL, 10);

            if(pinStatus > 1)
            {
                printf("The status (%d) is invalid.\n", pinStatus);
                return 1;
            }
            else
            {
                if(0 != rtk_extGpio_pin_set(pinNum,pinStatus))
                {
                    printf("The pinNum (%d) is invalid.\n", pinNum);
                    return 1;
                }
                printf("pin%d:\t%d\n\n",pinNum,pinStatus);
                return 0;
            }
        }
    }
#endif /* CONFIG_SDK_DRIVER_GPIO */

#if defined(CONFIG_SDK_DRIVER_I2C)
    if (strcmp(argv[1], "i2c") == 0)
    {
        unsigned int i2c_dev_idx, reg;
        unsigned int clk_dev, clk_pin, data_dev, data_pin, data, freq, intf_id, rtl8231_address;

        if (argc < 3)
        {
            printf("i2c parameter error\n\n");
            goto usage;
        }

        if ((strcmp(argv[2], "init") == 0)&&(strcmp(argv[3], "sw") == 0))
        {
            int delay, chipid;
            if (argc < 12)
            {
                printf("i2c parameter error\n\n");
                goto usage;
            }
            i2c_dev_idx = simple_strtoul(argv[4], NULL, 10);
            clk_dev = simple_strtoul(argv[5], NULL, 10);
            clk_pin = simple_strtoul(argv[6], NULL, 10);
            data_dev = simple_strtoul(argv[7], NULL, 10);
            data_pin = simple_strtoul(argv[8], NULL, 10);
            chipid= simple_strtoul(argv[10], NULL, 10);
            delay = simple_strtoul(argv[11], NULL, 10);
			rtl8231_address = simple_strtoul(argv[12], NULL, 10);
            rtk_i2c_sw_init(i2c_dev_idx, clk_dev, clk_pin, data_dev, data_pin, argv[9], chipid, delay, rtl8231_address);
            return 0;
        }

        if ((strcmp(argv[2], "init") == 0)&&(strcmp(argv[3], "hw") == 0))
        {
            int chipid;
            if (argc < 9)
            {
                printf("i2c parameter error\n\n");
                goto usage;
            }
            i2c_dev_idx = simple_strtoul(argv[4], NULL, 10);
            intf_id = simple_strtoul(argv[5], NULL, 10);
            data_pin = simple_strtoul(argv[6], NULL, 10);
            chipid= simple_strtoul(argv[8], NULL, 10);
            freq = simple_strtoul(argv[9], NULL, 10);
            rtk_i2c_hw_init(i2c_dev_idx, data_pin, argv[7], chipid, freq, intf_id);
            return 0;
        }

        if (strcmp(argv[2], "read") == 0)
        {
            if (argc < 5)
            {
                printf("i2c parameter error\n\n");
                goto usage;
            }
            i2c_dev_idx = simple_strtoul(argv[3], NULL, 10);
            reg = simple_strtoul(argv[4], NULL, 10);
            rtk_i2c_read(i2c_dev_idx, reg);
            return 0;
        }
        if (strcmp(argv[2], "write") == 0)
        {
            if (argc < 6)
            {
                printf("i2c parameter error\n\n");
                return 1;
            }
            i2c_dev_idx = simple_strtoul(argv[3], NULL, 10);
            reg = simple_strtoul(argv[4], NULL, 10);
            data = simple_strtoul(argv[5], NULL, 10);
            rtk_i2c_write(reg, data, data);
            return 0;
        }
        goto usage;
    }
#endif/* defined(CONFIG_SDK_DRIVER_GPIO) */

    /* led test */
    if (strcmp(argv[1], "ledtest") == 0)
    {
        if (argc < 3)
        {
            rtk_ledTest_run();
            return 0;
        }
        if (argc != 4)
        {
            printf("Usage: cst ledtest [port] [led_index]\n\n");
            return 1;
        }
        port = simple_strtoul(argv[2], NULL, 10);
        led_index = simple_strtoul(argv[3], NULL, 10);
        rtk_swledtest_on(port,led_index);
        return 0;
    }
    /* phyreg get portid page reg */
    if(0 == strcmp(argv[1], "phyreg"))
    {
        if (0 == strcmp(argv[2], "get"))
        {
            rtk_phyreg_get(argv);
            return 1;
        }

        /* phyreg get portid page reg */
        if (0 == strcmp(argv[2], "dump-top"))
        {
            rtk_phyreg_dump_top(argv);
            return 1;
        }

        /* phyreg set */
        if (0 == strcmp(argv[2], "set"))
        {
            rtk_phyreg_set(argv);
            return 1;
        }

        /* phyreg setbymask */
        if (strcmp(argv[2], "setbymask") == 0)
        {
            rtk_phyreg_portmask_set(argv);
            return 1;
        }
        goto usage;
    }

#if defined(CONFIG_SDK_RTL8380)
    if (0 == strcmp(argv[1], "l2-testmode"))
    {
        if(0 == strcmp(argv[2], "on"))
        {
            rtk_l2testmode_on();
            return 0;
        }
        if(0 == strcmp(argv[2], "off"))
        {
            rtk_l2testmode_off();
            return 0;
        }
        goto usage;
    }
#endif

    /* testmode */
    if (strcmp(argv[1], "phytestmode") == 0)
    {
        if (argc < 4)
        {
            printf("Usage: testmode <unit> <mode> <port> <channel>\n channel: 1=A,2=B,3=C,4=D,0=None\n\n");
        }
        else
        {
            rtk_phy_testmode(argv);
        }

        return 1;
    }
    if (0 == strcmp(argv[1], "boardid"))
    {
        if (argc == 3)
        {
            id = simple_strtoul(argv[2], NULL, 10);
            rtk_boardId_set(id);
            return 0;
        }
        else
        {
            return rtk_boardId_get();
        }
    }

    if (0 == strcmp(argv[1], "boardmodel"))
    {
        if (argc == 3)
        {
            rtk_boardModel_set(argv);
            return 0;
        }
        else
        {
            return rtk_boardModel_get();
        }
    }

    if (0 == strcmp(argv[1], "10g"))
    {
        int order = 2;
        unsigned int port;

        port = simple_strtoul(argv[order++], NULL, 10);
        if (0 == strcmp(argv[order], "none"))
            rtk_10gMedia_none_set(port);
        else if (0 == strcmp(argv[order], "fiber10g"))
            rtk_10gMedia_fiber10g_set(port);
        else if (0 == strcmp(argv[order], "fiber1g"))
            rtk_10gMedia_fiber1g_set(port);
        else if (0 == strcmp(argv[order], "fiber100m"))
            rtk_10gMedia_fiber100m_set(port);
        else if (0 == strcmp(argv[order], "dac50cm"))
            rtk_10gMedia_dac50cm_set(port);
        else if (0 == strcmp(argv[order], "dac100cm"))
            rtk_10gMedia_dac100cm_set(port);
        else if (0 == strcmp(argv[order], "dac300cm"))
            rtk_10gMedia_dac300cm_set(port);
        else
        {
            goto usage;
        }
        return 0;
    }

usage:
    printf("Usage:\n%s\n", (char *)cmdtp->usage);
    return 1;
} /* end of do_rtk */
#if (ZyNOS) && (UBOOT_V2)
U_BOOT_CMD(
    rtk, 13, 0, do_rtk,
    "rtk     - Realtek commands\n",
    "rtk network on\n"
    "        - Enable the networking function\n"
    "rtk netowkr off\n"
    "        - Disable the networking function\n"
    "rtk testmode [mode] [port]\n"
    "        - Set default value for specific testing\n"
#ifdef CONFIG_SDK_DRIVER_GPIO
    "rtk ext-devInit [deviceAddress]\n"
    "        - set RTL8231 MDC address\n"
    "rtk ext-pinGet [pinNum]\n"
    "        - get external 8231 GPIO pin status\n"
    "rtk ext-pinSet [pinNum] [status]\n"
    "        - set external 8231 GPIO pin status\n"
#endif/* CONFIG_SDK_DRIVER_GPIO */
    "rtk i2c init sw [i2c_dev_id] [sck_dev] [sck_pin] [sda_dev] [sda_pin] "
    "[8/16 access type] [chipid] [delay] [rtl8231_address (for Ext-GPIO only)]\n"
    "        - create a i2c group and init\n"
    "rtk i2c init hw [i2c_dev_id] [intf_id 0~1 for HW] [sda_pin] "
    "[8/16 access type] [chipid] [freq 0~3]\n"
    "        - create a i2c group and init\n"
    "rtk i2c read [i2c_dev_id] [reg]\n"
    "rtk i2c write [i2c_dev_id] [reg] [data]\n"
#ifdef CONFIG_SDK_DRIVER_GPIO
    "rtk pinGet [pinNum]\n"
    "        - get internal GPIO pin status\n"
    "rtk pinSet [pinNum] [status]\n"
    "        - set internal GPIO pin status\n"
#endif/* CONFIG_SDK_DRIVER_GPIO */
    "rtk ledtest [port] [led_index]\n"
    "        - led test\n"
#ifdef CONFIG_EEE
    "rtk eee on\n"
    "        - enable eee function\n"
    "rtk eee off\n"
    "        - disable eee function\n"
#endif/* CONFIG_EEE */
    "rtk show hw_profile_list\n"
    "        - show the current all supported hw_profile list\n"
    "rtk phytestmode unit mode port channel\n"
    "        - Set PHY into test mode; channel: 1=A,2=B,3=C,4=D,0=None\n"
    "rtk boardid\n"
    "        - Get board model id\n"
    "rtk boardid id\n"
    "        - Set board model id\n"
    "rtk boardmodel\n"
    "        - Get board model\n"
    "rtk boardmode <str>model\n"
    "        - Set board model\n"
    "rtk 10g PORT [none | fiber10g | fiber1g | fiber100m | dac50cm | dac100cm | dac300cm]"
    "        - Set 10g port media\n"
);
#endif /* (ZyNOS) && (UBOOT_V2) */
