/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashApp.h
* version    :   V1.0
* description:   定义Flash布局和操作接口.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/20/2013
*
* history    :
* 01   06/20/2013  LiuZenglin     Create.
*定义FLASH布局，和文件系统初始化参数
******************************************************************************/
#ifndef __FLASH_APP_H__
#define __FLASH_APP_H__

#include <tplink/flashDrvLib.h>
//#include <tplink/typedefs.h>
#include <tplink/flashConfig.h>
#ifdef FLASH_SECTOR_SIZE
#undef FLASH_SECTOR_SIZE
#endif
#define FLASH_SECTOR_SIZE (0x10000)

#define ROUNDUP(size, align)       (((int) (size) + (align - 1)) & ~(align - 1))
#define ROUNDDOWN(size, align)       ((int) (size) & ~(align - 1))

#define ROUND_SECTOR_UP(size)   ROUNDUP((size), (FLASH_SECTOR_SIZE))
#define ROUND_SECTOR_DOWN(size) ROUNDDOWN((size), (FLASH_SECTOR_SIZE))
/* 地址2对齐 */
#define ROUND_ADDR_UP(size)   size/* (((int) (size) + (2 - 1)) & ~(2 - 1)) */
#define ROUND_ADDR_DOWN(size) size/* ((int)(size) & ~(2 - 1)) */

#define FIRMWARE_MIN_SIZE     			0x180000  		/* 1.5M bytes */
#define FIRMWARE_MAX_SIZE				0x1000000		/* 8.5M bytes */
#define LICENSE_MAX_SIZE                0x020000        /* 128K bytes*/

extern unsigned int getConfigMaxSize(void);

#define CONFIG_MAX_SIZE		getConfigMaxSize()

#define BOOTROM_MAX_SIZE  	0x100000

#define BUF_ADDR		               0x88F00000	
#define BUF_ADDR_NOFREE		 0x89F00000	
#define BUF_IMAGE_ADDR               0x8aF00000


int   flashAppInit(void);

int flashGetBootSize(void);

int flashGetFsOffset(void);

int flashGetFsSize(void);

int flashGetFlVer(void);

/* 获取默认的文件系统大小的环境变量值 */
/* 因为T3700有16M和32M之分，16M为0xc00000，32M为0x1c00000 */
int  flashGetDftGevFsSize(char* pBuf);


STATUS tpRm(const char *    fileName    /* name of file to remove */);

UINT32 writeFlash(char *source, unsigned int size, unsigned int flashOffset, int prompt);

UINT32 readFlash(char *source, unsigned int size, unsigned int flashOffset);

STATUS writeBootrom(char * source, UINT32 size);

void flashAppDebugMode(BOOL mode);

STATUS lockFlash(void);
STATUS lockFs(void);    
STATUS unlockFlash(void);
STATUS unlockFs(void);

#endif /* __FLASH_APP_H__ */

