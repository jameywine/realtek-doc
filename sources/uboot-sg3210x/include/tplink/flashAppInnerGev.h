/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashAppGev.h
* version    :   V1.0
* description:   环境变量接口的头文件.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/18/2013
*
* history    :
* 01   06/18/2013  LiuZenglin     Create.
*
******************************************************************************/
#ifndef __FLASH_APP_GEV_H__
#define __FLASH_APP_GEV_H__

//#include <tplink/typedefs.h>
#include <tplink/flashConfig.h>

#define ENV_ITEM_NAME_LEN       256     /* 无长度信息，不足256的以0xff填充 */
#define ENV_ITEM_VALUE_LEN      256     /* 无长度信息，不足256的以0xff填充 */

/* 默认的全局环境变量名称 */
#define TP_SWITCH_START_NO_CONFIG   "START_NO_CONFIG"
#define TP_SWITCH_START_CONFIG      "START_CONFIG"
#define TP_SWITCH_BACKUP_CONFIG     "BACKUP_CONFIG"
#define TP_SWITCH_START_IMAGE       "START_IMAGE"
#define TP_SWITCH_BACKUP_IMAGE      "BACKUP_IMAGE"
#define TP_SWITCH_FL_VER            "FL_VER"
#define TP_SWITCH_FL_SIZE_BOOT      "FL_SIZE_BOOT"
#define TP_SWITCH_FL_OFFSET_KERNEL      "FL_OFFSET_KERNEL" /*1区内核镜像起始地址，2区的不在系统参数区体系，通过计算可以获得*/
#define TP_SWITCH_FL_SIZE_KERNEL        "FL_SIZE_KERNEL"  /*1区内核镜像大小*/
#define TP_SWITCH_FL_OFFSET_USRIMG1      "FL_OFFSET_USRIMG1" /*1区用户镜像起始地址*/
#define TP_SWITCH_FL_SIZE_USRIMG1        "FL_SIZE_USRIMG1"/*1区用户镜像大小*/
#define TP_SWITCH_FL_OFFSET_USRIMG2      "FL_OFFSET_USRIMG2" /*2区用户镜像起始地址*/
#define TP_SWITCH_FL_SIZE_USRIMG2        "FL_SIZE_USRIMG2"/*2区用户镜像大小*/
#define TP_SWITCH_FL_OFFSET_USRAPP      "FL_OFFSET_USRAPP"
#define TP_SWITCH_FL_SIZE_USRAPP        "FL_SIZE_USRAPP"
#define TP_SWITCH_CURRENT_IMAGE     "CURRENT_IMAGE"     /* 系统启动是记录当前启动的镜像 */
#define TP_SWITCH_CURRENT_CONFIG    "CURRENT_CONFIG"    /* 系统启动是记录当前启动的配置文件 */

#define TP_SWITCH_PWD_RECOVERY        "PWD_RECOVERY"         /* 密码恢复开关 */
#define TP_SWITCH_PWD_RECOVERY_FLAG   "PWD_RECOVERY_FLAG"    /* 密码恢复标志 */
#define TP_SWITCH_RESET_DISABLE			"RESET_DISABLE"

#define TP_SWITCH_CONFIG_PREFIX     ".cfg"
#define TP_SWITCH_IMAGE_PREFIX      ".bin"

#define TP_SWITCH_CONFIG_NAME_1      "flash:config1.cfg"
#define TP_SWITCH_CONFIG_NAME_2    "flash:config2.cfg"
#define TP_SWITCH_CONFIG_NAME_3    "flash:config3.cfg"
#define TP_SWITCH_IMAGE_NAME_1          "flash:image1.bin"
#define TP_SWITCH_IMAGE_NAME_2          "flash:image2.bin"

#define BOOT_PARAM_SIZE         512
#define BOOT_ADDR_LEN		30
#define BOOT_FILE_LEN		160	/* max chars in file name */
#define BOOT_USR_LEN		20	/* max chars in user name */
#define BOOT_PASSWORD_LEN	20	/* max chars in password */

#define FN_MAX_LENGTH 160

typedef struct 
{
	char ipAddr[BOOT_ADDR_LEN];			/*!< the ip of the switch (add by Cai Peifeng:2010-9-3 13:56:4)*/
	char ipMask[BOOT_ADDR_LEN];			/*!< the mask of the switch (add by Cai Peifeng:2010-9-3 13:56:11)*/
	char routeAddr[BOOT_ADDR_LEN];		/*!< the route of the switch (add by Cai Peifeng:2010-9-3 13:56:18)*/
	char hostAddr[BOOT_ADDR_LEN];		/*!< the ftp host address (add by Cai Peifeng:2010-9-3 13:56:25)*/
	char userName[BOOT_USR_LEN];		/*!< the user name (add by Cai Peifeng:2010-9-3 13:56:38)*/
	char passWd[BOOT_PASSWORD_LEN];		/*!< the password of the user (add by Cai Peifeng:2010-9-3 13:56:46)*/
	char fileName[BOOT_FILE_LEN];		/*!< the file that to be download (add by Cai Peifeng:2010-9-3 13:56:54)*/
	int  flags;							/*!< the current mode (add by Cai Peifeng:2011-9-27 11:34:43)*/
}_BOOT_PARAM;

typedef union
{
	_BOOT_PARAM     content;
	UINT8		    padding[BOOT_PARAM_SIZE];	  	  
}BOOT_PARAM;

/* 返回动态申请内存，需要手动释放 */
char* getDftGevContent(int *otGevSize);
STATUS resetEnv(void);
STATUS flashAppInnerGevInit(void);    /* copy items for flash to ram */
STATUS setEnvItem(const char* name, const char* value);
STATUS copyEnvItem(const char* dstName, const char* srcName);
STATUS getEnvItem(const char* name, char* value);
STATUS delEnvItem(const char* name);
STATUS updateGevItem(const char*flBootSize, const char*flFsOffset, const char*flFsSize, const char*flVer);

STATUS sysGetBootFromFlash(BOOT_PARAM *bootParam);
STATUS sysSetBootToFlash(BOOT_PARAM *bootParam);

STATUS sysGetBootLineFromFlash(char *bootLine, int len);

#endif /* __FLASH_APP_GEV_H__ */

