/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashAppFsImage.h
* version    :   V1.0
* description:   定义解析镜像的接口.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/25/2013
*
* history    :
* 01   06/25/2013  LiuZenglin     Create.
*
******************************************************************************/
#ifndef __FLASH_APP_FS_IMAGE_H__
#define __FLASH_APP_FS_IMAGE_H__

#include <tplink/flashAppInnerGev.h>
//#include <tplink/typedefs.h>
#include <tplink/flashConfig.h>

#define MD5_CHECKSUM_OFFSET		0		/*!< the offset of the md5 key (add by caipeifeng:2010-1-8 14:41:25)*/
#define MD5_DIGEST_LEN			16		/*!< length of MD5 digest result  (add by Cai Peifeng:2010-4-7 13:45:1) */
#define FLASH_PRODUCT_LENGTH 	16		/*!< the length of the product (add by caipeifeng:2010-1-8 14:41:9)*/
#define TP_IMAGE_NAME_SIZE			200
#define TP_IMAGE_HEADER_SIZE			512
/* 镜像头部中存储的三个环境变量长度 */
#define FL_SIZE_LEN                16  /* "0x100000" */
#define FL_VER_LEN                      16   /* "0x10000" */

#define strtoul                     simple_strtoul   
#define TOLOWER(x) ((x) | 0x20)   

typedef struct
{
    char            md5[MD5_DIGEST_LEN];
    unsigned char   productId[FLASH_PRODUCT_LENGTH];
    char	        imagename[TP_IMAGE_NAME_SIZE];
    UINT32          usrImageSize;   /*用户镜像大小*/
    UINT32          uImageSize;      /*内核镜像大小*/
    UINT32          bootromSize;        /* 可选项，bootromSize == 0表示镜像不包括bootrom */
    UINT32          compressMethod;     /* 压缩算法 1 - LZMA */
    char            flBootSize[FL_SIZE_LEN];   /* BOOTROM区的大小 */
    char            flKernelOffset[FL_SIZE_LEN];   /*内核的起始地址*/
    char            flKernelSize[FL_SIZE_LEN];       /* 内核的大小 */
    char            flUsrImg1Offset[FL_SIZE_LEN];   /* 用户镜像的起始地址 */
    char            flUsrImg1Size[FL_SIZE_LEN];       /* 用户镜像的大小 */
    char            flUsrImg2Offset[FL_SIZE_LEN];   /* 用户镜像的起始地址 */
    char            flUsrImg2Size[FL_SIZE_LEN];       /* 用户镜像的大小 */
    char            flUsrAppOffset[FL_SIZE_LEN];   /* 用户app文件系统起始地址 */
    char            flUsrAppSize[FL_SIZE_LEN];       /* 用户app文件系统大小 */
    char            flVer[FL_VER_LEN];              /* Flash布局版本号 */
}_TP_IMAGE_HEADER;

typedef union
{
	UINT8			 padding[TP_IMAGE_HEADER_SIZE];	
	_TP_IMAGE_HEADER content;  	  
}TP_IMAGE_HEADER;

typedef struct _INNER_ITEM_INFO_NODE
{
    int type;
    int length;
    int offset;
    struct _INNER_ITEM_INFO_NODE* next;
}INNER_ITEM_INFO_NODE;

typedef enum _IMAGE_STATUS_T
{
    IMAGE_STATUS_NOT_EXIST = 0,
    IMAGE_STATUS_EXIST_OK,
    IMAGE_STATUS_EXIST_ERROR,
    IMAGE_STATUS_NUM
}IMAGE_STATUS_T;

typedef struct _IMAGE_INFO
{
    char imageName[BOOT_FILE_LEN];  /* image1.bin image2.bin */
    bool isValid;                   /* 表明当前结构中出ImageName外的其它值是否有效 */
    IMAGE_STATUS_T ist;             /* image status */
    char imagename[TP_IMAGE_NAME_SIZE];
    /* 下列偏移地址为内容在解密后的镜像中的偏移地址 */
    int  usrImage_offset;
    int  usrImage_len;
    int  uImage_offset;
    int  uImage_len;
    int  bootrom_offset;
    int  bootrom_len;       /* 如果镜像中不存在bootrom，则该项为0 */
    /* 内部区内容 */
    int  innerItemNumber;
    /* 环境变量项必须存在，初始化时如果不存在，会初始化为V1.0.0版本 */
    int  gevType;                        /* 环境变量在内部区中的type */
    char flBootSize[FL_SIZE_LEN];   /* BOOTROM区的大小 */
    char flKernelOffset[FL_SIZE_LEN];   /*内核的起始地址*/
    char flKernelSize[FL_SIZE_LEN];       /* 内核的大小 */
    char flUsrImg1Offset[FL_SIZE_LEN];   /* 用户镜像的起始地址 */
    char flUsrImg1Size[FL_SIZE_LEN];       /* 用户镜像的大小 */
    char flUsrImg2Offset[FL_SIZE_LEN];   /* 用户镜像的起始地址 */
    char flUsrImg2Size[FL_SIZE_LEN];       /* 用户镜像的大小 */
    char flUsrAppOffset[FL_SIZE_LEN];   /* 用户app文件系统起始地址 */
    char flUsrAppSize[FL_SIZE_LEN];       /* 用户app文件系统大小 */
    char flVer[FL_VER_LEN];              /* Flash布局版本号 */
    /* 其余内部项，动态 */
    INNER_ITEM_INFO_NODE *innerItemHead;
} IMAGE_INFO;

/* 启动镜像时，刷新FLASH，完成版本切换用镜像中switch.tp和webImage.z更新文件系统 */
int refreshFlash(const IMAGE_INFO *imageInfo, const char* pBuff,  int nRegion);

/* 解析传入的镜像buff，buff长度为length, 获取镜像结构 */
/* 输出镜像信息pImageInfo */
int sysParseImage(const unsigned char* pImageBuff, int length, IMAGE_INFO *pImageInfo);

STATUS sysShowImage(const IMAGE_INFO *pImageInfo);

typedef struct _IMAGE_PRINT_INFO
{
    char imageName[BOOT_FILE_LEN];  /* image1.bin image2.bin */
    IMAGE_STATUS_T ist;             /* image status */
    char imagename[TP_IMAGE_NAME_SIZE];
    /* 下列偏移地址为内容在解密后的镜像中的偏移地址 */
    /* 下列偏移地址为内容在解密后的镜像中的偏移地址 */
    int  usrImage_offset;
    int  usrImage_len;
    int  uImage_offset;
    int  uImage_len;
    int  bootrom_offset;
    int  bootrom_len;       /* 如果镜像中不存在bootrom，则该项为0 */
    /* 内部区内容 */
    int  innerItemNumber;
    char flVer[FL_VER_LEN];              /* Flash布局版本号 */
}IMAGE_PRINT_INFO;

int sysGetImagePrintInfo(const unsigned char* pImageBuff, int length, IMAGE_PRINT_INFO *pImagePrintInfo);

STATUS sysShowImageByPrintInfo(const IMAGE_PRINT_INFO *pImagePrintInfo);

//STATUS sysUpgradeFileCheck(SWITCH_FILE_ATTRIBUTE sfa, unsigned char *pAppBuf, unsigned int nFileBytes);

//STATUS sysUpgradeFileApply(SWITCH_FILE_ATTRIBUTE sfa, unsigned char *pAppBuf, unsigned int nFileBytes);

#endif /* __FLASH_APP_FS_IMAGE_H__ */

