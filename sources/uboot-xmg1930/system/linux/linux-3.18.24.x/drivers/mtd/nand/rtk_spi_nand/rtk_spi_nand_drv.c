/******************************************************************************
 * $Id: luan_spi_nand.c,v 1.0 2016/06/13   Exp $
 * drivers/mtd/nand/rtk_spi_nand_drv.c
 * Overview: Realtek NAND Flash Controller Driver
 * Copyright (c) 2016 Realtek Semiconductor Corp. All Rights Reserved.
 * Modification History:
 *
 *******************************************************************************/

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>
#include <mtd/mtd-abi.h>
#include "rtk_spi_nand_drv.h"
#include <soc.h>
#include <linux/delay.h>

//id (* hook_restart_func)(void) = NULL;

//id rtk_hook_restart_function(void (*func)(void))
typedef spi_nand_flash_info_t * (*pf_t)(void);
typedef int (*ir_t)(spi_nand_flash_info_t *);


pf_t spi_nand_chip_probe_func[] =
{   probe_gd_spi_nand_chip,
    probe_mxic_spi_nand_chip,
    probe_toshiba_spi_nand_chip,
    probe_winbond_spi_nand_chip
};

ir_t spi_nand_chip_init_rest_func[] =
{   gd_init_rest,
    mxic_init_rest,
    toshiba_init_rest,
    winbond_init_rest
};

/* Add by Managemented Switch Project for CS1 MUX setting */
/* Set NAND controller CS0 to PIN CS1 */
void
rtk_luna_cs_pin_set(void)
{
    unsigned int reg_v;

    reg_v = REGISTER_REG32(PAD_PSR0);
    reg_v &= ~(PAD_PSR0_CS1_MUX_SEL_MASK);
    reg_v |= ((0x2)<<PAD_PSR0_CS1_MUX_SEL_OFF);
    REGISTER_REG32(PAD_PSR0) = reg_v;
    mdelay(500);
}

spi_nand_flash_info_t * probe_spi_nand_chip_func(void)
{
    unsigned int    loop_idx;
    void *flash_info = VZERO;

    for(loop_idx = 0; loop_idx < 4; loop_idx++)
    {
        flash_info = (void *)spi_nand_chip_probe_func[loop_idx]();
        if(flash_info != VZERO)
            return flash_info;
    }
    return flash_info;
}

int probe_spi_nand_chip_init_rest(spi_nand_flash_info_t * spi_nand_info)
{
    unsigned int    loop_idx;
    int             result;

    for(loop_idx = 0; loop_idx < 4; loop_idx++)
    {
        result = spi_nand_chip_init_rest_func[loop_idx](spi_nand_info);
        if(result == 1)
            return result;
    }
    return result;
}


