/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Revision: 78657 $
 * $Date: 2017-05-11 18:28:49 +0800 (Thu, 11 May 2017) $
 *
 */
#ifndef __BOOTLOADER__
#include <linux/module.h>
#endif
#include <osal/lib.h>
#include <common/rt_type.h>
#include <flash/flash_partition.h>

rtk_partition_entry_t rtk_partition_table[RTK_MTD_PARTITION_NUM] = {
    [FLASH_INDEX_LOADER] = {
        .name = "uboot",
        .offset = LOADER_OFFSET,
        .size = LOADER_SIZE,
    },
    [FLASH_INDEX_LOADER_BDINFO] = {
        .name = "BDINFO",
        .offset = BDINFO_OFFSET,
        .size = BDINFO_SIZE,
    },
    [FLASH_INDEX_SYSINFO] = {
        .name = "sys",
        .offset = SYSINFO_OFFSET,
        .size = SYSINFO_SIZE,
    },
    [FLASH_INDEX_JFFS2_CFG] = {
        .name = "usrimg1",
        .offset = JFFS2_CFG_OFFSET,
        .size = JFFS2_CFG_SIZE,
    },
    [FLASH_INDEX_JFFS2_LOG] = {
        .name = "usrimg2",
        .offset = JFFS2_LOG_OFFSET,
        .size = JFFS2_LOG_SIZE,
    },
    [FLASH_INDEX_KERNEL] = {
        .name = "usrappfs",
        .offset = KERNEL_OFFSET,
        .size = USRAPP_SIZE,
    },
#if defined(CONFIG_DUAL_IMAGE)
    [FLASH_INDEX_KERNEL2] = {
        .name = "para",
        .offset = PARA_OFFSET,
        .size = PARA_SIZE,
    }
#endif /* CONFIG_DUAL_IMAGE */
};

unsigned int rtk_flash_partition_size_get(unsigned int partition_idx)
{
    unsigned int	size;

    switch(partition_idx){
        case FLASH_INDEX_LOADER:			/* LOADER */
        case FLASH_INDEX_LOADER_BDINFO: 	/* LOADER_BDINFO */
        case FLASH_INDEX_SYSINFO:			/* SYSINFO */
        case FLASH_INDEX_JFFS2_CFG: 		/* JFFS2_CFG */
        case FLASH_INDEX_JFFS2_LOG: 		/* JFFS2_LOG */
        case FLASH_INDEX_KERNEL:			/* KERNEL */
#if defined(CONFIG_DUAL_IMAGE)
        case FLASH_INDEX_KERNEL2:			/* KERNEL2 */
#endif
            size = rtk_partition_table[partition_idx].size;
            break;
        default:
            size = 0;
            break;
    }
    return size;
}

unsigned int rtk_flash_partition_offset_get(unsigned int partition_idx)
{
    unsigned int	offset;
    switch(partition_idx){
        case FLASH_INDEX_LOADER:			/* LOADER */
        case FLASH_INDEX_LOADER_BDINFO: 	/* LOADER_BDINFO */
        case FLASH_INDEX_SYSINFO:			/* SYSINFO */
        case FLASH_INDEX_JFFS2_CFG: 		/* JFFS2_CFG */
        case FLASH_INDEX_JFFS2_LOG: 		/* JFFS2_LOG */
        case FLASH_INDEX_KERNEL:			/* KERNEL */
#if defined(CONFIG_DUAL_IMAGE)
        case FLASH_INDEX_KERNEL2:			/* KERNEL2 */
#endif
            offset = rtk_partition_table[partition_idx].offset;
            break;
        default:
            offset = 0;
            break;
    }
    return offset;
}

int rtk_flash_partition_table_get(unsigned int total_size, rtk_partition_entry_t *partition_table) {
    #if defined(CONFIG_DUAL_IMAGE)
        #ifdef CONFIG_TARGET_ARCH_MIPS
          //  rtk_partition_table[FLASH_INDEX_KERNEL].size = (total_size - (unsigned int)KERNEL_OFFSET)/2 ;
          //  rtk_partition_table[FLASH_INDEX_KERNEL2].size = (total_size - (unsigned int)KERNEL_OFFSET)/2 ;
          //  rtk_partition_table[FLASH_INDEX_KERNEL2].offset = rtk_partition_table[FLASH_INDEX_KERNEL].size + rtk_partition_table[FLASH_INDEX_KERNEL].offset;
        #endif
        #ifdef CONFIG_TARGET_ARCH_ARM
            if ((total_size - KERNEL_OFFSET) >= 2 * KERNEL_SIZE ) {
                /* size enough for dual image partition */
                rtk_partition_table[FLASH_INDEX_KERNEL].size = KERNEL_SIZE ;
                rtk_partition_table[FLASH_INDEX_KERNEL2].size = KERNEL_SIZE ;
            }
            else if((total_size - KERNEL_OFFSET) >= KERNEL_SIZE ){
                /* size enough for single image partition only */
                rtk_partition_table[FLASH_INDEX_KERNEL].size = KERNEL_SIZE ;
            } else {
                /* available size is smaller than definition */
                rtk_partition_table[FLASH_INDEX_KERNEL].size = (total_size - KERNEL_OFFSET) ;
            }
            rtk_partition_table[FLASH_INDEX_KERNEL2].offset = rtk_partition_table[FLASH_INDEX_KERNEL].size + rtk_partition_table[FLASH_INDEX_KERNEL].offset;
        #endif
    #else
        rtk_partition_table[FLASH_INDEX_KERNEL].size = total_size - KERNEL_OFFSET;
    #endif
    memcpy(partition_table, rtk_partition_table, sizeof(rtk_partition_entry_t)*RTK_MTD_PARTITION_NUM);
    return 0;
}

#ifndef __BOOTLOADER__
EXPORT_SYMBOL(rtk_flash_partition_offset_get);
EXPORT_SYMBOL(rtk_flash_partition_size_get);
EXPORT_SYMBOL(rtk_flash_partition_table_get);
#endif