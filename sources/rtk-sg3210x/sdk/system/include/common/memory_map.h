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
 * $Revision: 100385 $
 * $Date: 2019-09-23 16:15:30 +0800 (周一, 23 9月 2019) $
 *
 * Purpose : IOAL Layer Init Module
 *
 * Feature : IOAL Init Functions
 *
 */

/*
 * Include Files
 */
#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__


/*
 * Memeory Definition
 */

/* Size */
#define SWCORE_MEM_SIZE                 0x10000
#define SWCORE_ARM_MEM_SIZE             0x01000000
#define RTL9330_SWCORE_MEM_SIZE         0x300000
#define SOC_MEM_SIZE                    0x153FFF    //MIPS
#define SOC_ARM_MEM_SIZE                0x02000000

#define SRAM_MEM_SIZE_128M              0x20000



#ifdef CONFIG_SDK_DRIVER_NIC_USER_MODE
#define DRAM_MEM_SIZE                   ioal_lowMem_size
#define DMA_MEM_SIZE                    ioal_dma_size  /* for NIC driver, L2NTFY driver DMA usage, for our Linux User Space example code */
#else
#define DRAM_MEM_SIZE                   0x10000000
#endif

/* Physical */
/* >>> MIPS <<< */
#define SWCORE_PHYS_BASE                0x1B000000
#define SOC_PHYS_BASE                   0x18000000
#define SRAM_PHYS_BASE                  0x1F000000
#define DRAM_PHYS_BASE                  0x00000000
#define DMA_PHYS_BASE                   (DRAM_PHYS_BASE + DRAM_MEM_SIZE)
/* >>> ARM <<< */
#define SWCORE_ARM_PHYS_BASE            0x02000000
#define SOC_ARM_PHYS_BASE               0x00000000
#define SRAM_ARM_PHYS_BASE              0x00100000
#define DRAM_ARM_PHYS_BASE              0x10000000
#define DMA_ARM_PHYS_BASE               (DRAM_PHYS_BASE + DRAM_MEM_SIZE)

/* Virtual */
/* >>> MIPS <<< */
#define SWCORE_VIRT_BASE                0xBB000000
#define SOC_VIRT_BASE                   0xB8000000
#define SRAM_VIRT_BASE                  0xBF000000
#define DRAM_CACHE_VIRT_BASE            0x80000000
#define DRAM_UNCACHE_VIRT_BASE          0xA0000000

/* >>> ARM <<< */
/* TODO: remove? */
//#define SWCORE_ARM_VIRT_BASE            SWCORE_ARM_PHYS_BASE        //0x0200_0000~0x02FF_FFFF(16MB)
//#define SOC_ARM_VIRT_BASE               SOC_ARM_PHYS_BASE           //0x0100_0000~0x0100_0FFF(4KB)
//#define SRAM_ARM_VIRT_BASE              SRAM_ARM_PHYS_BASE          //0x0010_0000~0x0012_FFFF(192KB)


#endif  /* __MEMORY_MAP_H__ */

