/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashAppProfile.h
* version    :   V1.0
* description:   profile接口的头文件.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/18/2013
*
* history    :
* 01   06/18/2013  LiuZenglin     Create.
*
******************************************************************************/
#ifndef __FLASH_APP_PROFILE_H__
#define __FLASH_APP_PROFILE_H__

//#include <tplink/typedefs.h>
#include <tplink/flashConfig.h>

/* OEM */
STATUS loadItems(void);
STATUS getCorporation(char *corporation);
STATUS getDevice(char *device);
STATUS getDescription(char *description);
STATUS getWebEn(char *webEn);
STATUS getWebCn(char *webCn);
STATUS getPrompt(char *prompt);
STATUS getCpyright(char *cpyright);
STATUS getLocation(char *location);
STATUS getOptionOemItem(UINT32 index, char* strOemItem, UINT32 len);

/* profile常规项 */
STATUS sysGetMacFromFlash(unsigned char *s_pro);
STATUS sysSetMacToFlash(unsigned char *s_pro);
STATUS sysGetFlashSign(char *s_pro,int len);
STATUS sysSetFlashSign(char *s_pro,int len);
STATUS sysGetFlashPubLicKey(char *s_pro,int len);
STATUS sysSetFlashPublicKey(char *s_pro,int len);
STATUS sysGetProductId(unsigned char *productId);
STATUS sysSetProductId(unsigned char *productId);
UINT32 sysFirmwareCheckProduct(const unsigned char *pAppBuf);

#endif /* __FLASH_APP_PROFILE_H__ */

