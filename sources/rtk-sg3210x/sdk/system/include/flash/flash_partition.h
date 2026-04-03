/*
 * Copyright (C) 2009-2015 Realtek Semiconductor Corp.
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
 * $Revision: 58581 $
 * $Date: 2015-05-12 15:02:48 +0800 (Tue, 12 May 2015) $
 *
 */

#ifndef _RTK_FLASH_PARTITION_H
#define _RTK_FLASH_PARTITION_H
#ifndef __UBOOT__
#include <common/rt_autoconf.h>
#endif /*__UBOOT__*/

#ifndef FLASH_BASE
#define FLASH_BASE 0xBD000000
#endif

#define SPI_FLASH_BASE 0xb4000000

#define UNIT_SIZE 65536 // 64Kb

#define FLASH_SIZE_8MB		(0x00800000U)
#define FLASH_SIZE_16MB		(0x01000000U)
#define FLASH_SIZE_32MB		(0x02000000U)
#define FLASH_SIZE_64MB		(0x04000000U)
#define FLASH_SIZE_128MB	(0x08000000U)

#define SIZE_IDX_NOT_FOUND	0xffffffff

#ifdef CONFIG_DUAL_IMAGE
#define RTK_MTD_PARTITION_NUM		7
#else
#define RTK_MTD_PARTITION_NUM		6
#endif


#ifdef CONFIG_TARGET_ARCH_MIPS
#define LOADER_OFFSET           0x00000000U
#define LOADER_SIZE             (UNIT_SIZE*14)
#define BDINFO_OFFSET           (LOADER_OFFSET+LOADER_SIZE)
#define BDINFO_SIZE             (UNIT_SIZE*2)
#define SYSINFO_OFFSET          (BDINFO_OFFSET+BDINFO_SIZE)
#define SYSINFO_SIZE            (UNIT_SIZE*16*6)
#define JFFS2_CFG_OFFSET        (SYSINFO_OFFSET+SYSINFO_SIZE)
#define JFFS2_CFG_SIZE          (UNIT_SIZE*16*10)
#define JFFS2_LOG_OFFSET        (JFFS2_CFG_OFFSET+JFFS2_CFG_SIZE)
#define JFFS2_LOG_SIZE          (UNIT_SIZE*16*10)
#define KERNEL_OFFSET           (JFFS2_LOG_OFFSET+JFFS2_LOG_SIZE)
#define USRAPP_SIZE             (UNIT_SIZE*16*4)

#define PARA_SIZE               (UNIT_SIZE*16)
#define PARA_OFFSET             (UNIT_SIZE*16*31)

#endif

#ifdef CONFIG_TARGET_ARCH_ARM
#define LOADER_OFFSET           0x00000000U
#define LOADER_SIZE             (UNIT_SIZE*32)
#define BDINFO_OFFSET           (LOADER_OFFSET+LOADER_SIZE)
#define BDINFO_SIZE             (UNIT_SIZE*2)
#define SYSINFO_OFFSET          (BDINFO_OFFSET+BDINFO_SIZE)
#define SYSINFO_SIZE            (UNIT_SIZE*2)
#define JFFS2_CFG_OFFSET        (SYSINFO_OFFSET+SYSINFO_SIZE)
#define JFFS2_CFG_SIZE          (UNIT_SIZE*16)
#define JFFS2_LOG_OFFSET        (JFFS2_CFG_OFFSET+JFFS2_CFG_SIZE)
#define JFFS2_LOG_SIZE          (UNIT_SIZE*16)
#define KERNEL_OFFSET           (JFFS2_LOG_OFFSET+JFFS2_LOG_SIZE)
#define KERNEL_SIZE             ((FLASH_SIZE_32MB-KERNEL_OFFSET)/2) //Use 32MByte as default support
#endif

#define LOADER_BDINFO_SIZE BDINFO_SIZE

typedef struct rtk_partition_entry_s {
    const char *name;
    unsigned int offset;
    unsigned int size;
}rtk_partition_entry_t;

typedef enum rtk_flash_partition_idx_e
{
    FLASH_INDEX_LOADER = 0,
    FLASH_INDEX_LOADER_BDINFO,
    FLASH_INDEX_SYSINFO,
    FLASH_INDEX_JFFS2_CFG,
    FLASH_INDEX_JFFS2_LOG,
    FLASH_INDEX_KERNEL,
#ifdef CONFIG_DUAL_IMAGE
    FLASH_INDEX_KERNEL2,
#endif
    FLASH_INDEX_END,
}rtk_flash_partition_idx_t;

extern unsigned int rtk_flash_partition_offset_get(unsigned int partition_idx);
extern unsigned int rtk_flash_partition_size_get(unsigned int partition_idx);
extern int rtk_flash_partition_table_get(unsigned int total_size ,rtk_partition_entry_t *partition_table);

#if defined(__UBOOT__)
#define LOADER_BDINFO_START (unsigned int)flash_partition_addr_ret(FLASH_INDEX_LOADER_BDINFO)
#define SYSINFO_START		(unsigned int)flash_partition_addr_ret(FLASH_INDEX_SYSINFO)

extern unsigned int flash_partition_size_ret(rtk_flash_partition_idx_t partition);
extern unsigned int flash_partition_addr_ret(rtk_flash_partition_idx_t partition);
#endif

#endif /*_RTK_FLASH_PARTITION_H*/
