/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashAppProfile.c
* version    :   V1.0
* description:   环境变量相关接口.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/18/2013
*
* history    :
* 01   06/18/2013  LiuZenglin     Create.
******************************************************************************/

#include<common.h>
#include<vsprintf.h>
#include <tplink/flashApp.h>
#include <tplink/flashAppInner.h>
#include <tplink/flashAppInnerProfile.h>


typedef enum _PROFILE_ITEM_TYPE
{
    PROFILE_ITEM_MAC = 0,
    PROFILE_ITEM_SIGN,
    PROFILE_ITEM_PUBLICKEY,
    PROFILE_ITEM_PRODUCTID,
    PROFILE_ITEM_TYPE_NUM
}PROFILE_ITEM_TYPE;

#define IS_VALID_PROFILE_ITEM(pit)  ((pit >= 0) && (pit < PROFILE_ITEM_TYPE_NUM))
typedef struct _PROFILE_MAP
{
    PROFILE_ITEM_TYPE pit;
    int offset;
    int length;
}PROFILE_MAP;

/* profile中内容的布局 by liuzenglin, 15Aug12 */
/* profile中各字段的定义 */
#define PROFILE_MAC_OFFSET          0
#define PROFILE_MAC_LENGTH                  6
#define PROFILE_SIGN_OFFSET         (PROFILE_MAC_OFFSET + PROFILE_MAC_LENGTH)
#define PROFILE_SIGN_LENGTH                 128
#define PROFILE_PUBLICKEY_OFFSET    (PROFILE_SIGN_OFFSET + PROFILE_SIGN_LENGTH)
#define PROFILE_PUBLICKEY_LENGTH        250
#define PROFILE_PRODUCTID_OFFSET    (PROFILE_PUBLICKEY_OFFSET + PROFILE_PUBLICKEY_LENGTH)
#define PROFILE_PRODUCTID_LENGTH        16

LOCAL PROFILE_MAP profileMap[PROFILE_ITEM_TYPE_NUM] = {
{PROFILE_ITEM_MAC,       PROFILE_MAC_OFFSET,         PROFILE_MAC_LENGTH},
{PROFILE_ITEM_SIGN,      PROFILE_SIGN_OFFSET,        PROFILE_SIGN_LENGTH},
{PROFILE_ITEM_PUBLICKEY, PROFILE_PUBLICKEY_OFFSET,   PROFILE_PUBLICKEY_LENGTH},
{PROFILE_ITEM_PRODUCTID, PROFILE_PRODUCTID_OFFSET,   PROFILE_PRODUCTID_LENGTH},
};

/* 为了兼容V2.1版本的profile */
#define PROFILE_BOOTPARAM_LENGTH    324
/* OEM defines，在profile中 */
#define OEM_ITEM_OFFSET_1     (PROFILE_PRODUCTID_OFFSET + PROFILE_PRODUCTID_LENGTH)
#define OEM_ITEM_OFFSET_2     (PROFILE_PRODUCTID_OFFSET + PROFILE_PRODUCTID_LENGTH + PROFILE_BOOTPARAM_LENGTH)
//static int oemItemOffset = OEM_ITEM_OFFSET_1;   /* 为了兼容两种profile, 定义变量 */
#define OEM_ITEM_OFFSET       oemItemOffset
#define OEM_VALID_SIZE      4               /* OEM项的开始之前的四字节用于标识有效性 */
#define OEM_VALID_WORD      0xaa55aa55     /* 与OEM profile 一致 */
#define OEM_ITEM_SIZE       4               /* 前四字节保存长度 */
#define OEM_ITEM_LEN        124             /* 每个条目占Flash中124字节，与长度共同构成128字节 */
#define OEM_ITEM_NUM        8               /* OEM必选项数 */
#define OEM_OPTION_ITEM_OFFSET(x) (OEM_ITEM_OFFSET + OEM_VALID_SIZE + \
                        ((OEM_ITEM_NUM+x) * (OEM_ITEM_SIZE + OEM_ITEM_LEN)))
/* profile定义完成 */
extern void free(void*);

int profile_debug = 0;

#define DBG(arg...) do{if(0){printf("%s,%d:",__FUNCTION__, __LINE__);printf(arg);}}while(0)
#if 0
typedef struct oemItems
{
        UINT8 content[OEM_ITEM_LEN + 1];
        UINT32 size;
        UINT32 maxSize;         /* 根据ucGlobalConf.c中的全局变量大小而定,不包含结束符 */
} OEM_ITEMS;

typedef enum profile_oem_items
{
    PROFILE_CORPORATION = 0,
    PROFILE_PROMPT,
    PROFILE_DEVICE,
    PROFILE_LOCATION,
    PROFILE_WEBEN,
    PROFILE_WEBCN,
    PROFILE_COPYRIGHT,
    PROFILE_DESCRIPTION,
    PROFILE_END
} PROFILE_ITEMS;


LOCAL OEM_ITEMS gOemItems[OEM_ITEM_NUM] =
{
        {"TP-LINK", 7, 29},/* gCorporation */
        {"TP-LINK", 7, 29},/* bootConfig.c :tPrompt[30] */
        {"TL-SL5428", 9, 29},/* gDevModelName */
        {"SHENZHEN", 8, 29},/* sysDeault_Location */
        {"http://www.tp-link.com", 22, 29},/* sysDeault_Contact */
        {"http://www.tp-link.com.cn", 25, 29},      /* sysDeault_ContactEn */
        {"TP-LINK TECHNOLOGIES CO., LTD.", 30, 39}, /* gCompanyName */
        {"24-Port 10/100Mbps + 4-Port Gigabit L2 Managed Switch", 53, 99}/* gSysDescription */
};

LOCAL int oemItemsLoaded = FALSE;


STATUS loadItems(void)
{
    PROFILE_ITEMS profileItem = PROFILE_CORPORATION;
    int length = 0;
    unsigned char* profileBuff = flashAppInnerGet(FLASH_INNER_ITEM_PROFILE, &length);
    UINT32 oemTag1 = 0;
    UINT32 oemTag2 = 0;
    int offset = 0;
    UINT32 itemSize = 0;

    if (NULL == profileBuff)
    {
        printf("Failed to get profile.\r\n");
        oemItemsLoaded = TRUE;
        return ERROR;
    }

    /* 如果profile中不包括OEM项，则获取出来的长度小于OEM项的偏移 */
    /* 保持gOemItems的值不变，采用默认值 */
    if (length <= OEM_ITEM_OFFSET_1)
    {
        DBG("There is not any oem item in profile, length=%#x, oem_offset=%#x.\r\n",
            length, OEM_ITEM_OFFSET_1);
        oemItemsLoaded = TRUE;
        free(profileBuff);
        return OK;
    }

    oemTag1 = *(UINT32*)((unsigned char*)profileBuff + OEM_ITEM_OFFSET_1);
    oemTag2 = *(UINT32*)((unsigned char*)profileBuff + OEM_ITEM_OFFSET_2);
    
    DBG("OEM_VALID_WORD1 in flash is 0x%X\r\n", oemTag1);
    DBG("OEM_VALID_WORD2 in flash is 0x%X\r\n", oemTag2);

    if ((OEM_VALID_WORD != oemTag1) && (OEM_VALID_WORD != oemTag2))
    {
        printf("\nCurrent profile is not an OEM profile!\n");
        free(profileBuff);
        oemItemsLoaded = TRUE;
        return ERROR;
    }
    else
    {
        OEM_ITEM_OFFSET = ((OEM_VALID_WORD == oemTag1) ? OEM_ITEM_OFFSET_1 : OEM_ITEM_OFFSET_2);
    }

    for (profileItem = PROFILE_CORPORATION; profileItem < PROFILE_END; profileItem++)
    {
        offset = OEM_ITEM_OFFSET \
                 + OEM_VALID_SIZE + profileItem * (OEM_ITEM_SIZE + OEM_ITEM_LEN);
        
        memcpy((UINT8*)(&itemSize), profileBuff+offset, OEM_ITEM_SIZE);
        itemSize = ntohl(itemSize);
        if (itemSize > gOemItems[profileItem].maxSize)
        {
            DBG("\nInvalid OEM item length. size = %d\r\n", itemSize);
            DBG("\nFailed to load OEM items %d\r\n", profileItem);
            free(profileBuff);
            oemItemsLoaded = TRUE;
            return ERROR;
        }
        else
        {
            memset(gOemItems[profileItem].content, 0, gOemItems[profileItem].maxSize);
            gOemItems[profileItem].size = itemSize;
            if (itemSize != 0)
            {
                offset += OEM_ITEM_SIZE;
                memcpy(gOemItems[profileItem].content, profileBuff + offset, itemSize);
            }
        }
    }
    
    free(profileBuff);
    oemItemsLoaded = TRUE;
    return OK;
}

STATUS _getItem(PROFILE_ITEMS pi, char* buff)
{
    if (NULL == buff)
    {
        printf("Null buff.\r\n");
        return ERROR;
    }

    if ((pi < 0) || (pi > PROFILE_END))
    {
        printf("Unkown oem item.\r\n");
        return ERROR;
    }

    if (oemItemsLoaded == FALSE)
    {
        if (ERROR == loadItems())
        {
            printf("Failed to load oem items.\r\n");
        }
        DBG("Load oem items.\r\n");
    }

    memcpy(buff, gOemItems[pi].content, gOemItems[pi].size + 1);
    return OK;   
}


STATUS getCorporation(char *corporation)
{
    if (ERROR == _getItem(PROFILE_CORPORATION, corporation))
    {
        printf("Failed to get oem corporation.\r\n");
        return FALSE;
    }
    return TRUE;
}
STATUS getDevice(char *device)
{
    if (ERROR == _getItem(PROFILE_DEVICE, device))
    {
        printf("Failed to get oem device.\r\n");
        return FALSE;
    }
    return TRUE;
}
STATUS getDescription(char *description)
{
    if (ERROR == _getItem(PROFILE_DESCRIPTION, description))
    {
        printf("Failed to get oem device.\r\n");
        return FALSE;
    }
    return TRUE;
}
STATUS getWebEn(char *webEn)
{
    if (ERROR == _getItem(PROFILE_WEBEN, webEn))
    {
        printf("Failed to get oem webEn.\r\n");
        return FALSE;
    }
    return TRUE;
}
STATUS getWebCn(char *webCn)
{
    if (ERROR == _getItem(PROFILE_WEBCN, webCn))
    {
        printf("Failed to get oem webCn.\r\n");
        return FALSE;
    }
    return TRUE;
}
STATUS getPrompt(char *prompt)
{
    if (ERROR == _getItem(PROFILE_PROMPT, prompt))
    {
        printf("Failed to get oem prompt.\r\n");
        return FALSE;
    }
    return TRUE;
}
STATUS getCpyright(char *cpyright)
{
    if (ERROR == _getItem(PROFILE_COPYRIGHT, cpyright))
    {
        printf("Failed to get oem cpyright.\r\n");
        return FALSE;
    }
    return TRUE;
}

STATUS getLocation(char *location)
{
    if (ERROR == _getItem(PROFILE_LOCATION, location))
    {
        printf("Failed to get oem location.\r\n");
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************************
* fn            : getOptionOemItem()
* author        : LiuZenglin <LiuZenglin@tp-link.net>
* decription    : 获取可选的OEM项
* param[in]     : index : 可选oem项的编号 1 ~ 50
* param[out]    : strOemItem:输出可选OEM项，最长124B，不包括停止符
* param[in]     : len   : strOemItem的最长长度，应该不大于125
*
* return        : TRUE:获取成功  FALSE:获取失败，直接返回不作处理
* retval        : N/A
* note          : N/A
*****************************************************************************/
STATUS getOptionOemItem(UINT32 index, char* strOemItem, UINT32 len)
{
    UINT32 offset = 0;
    UINT32 size = 0;
    UINT32 oemTag = 0;
    int    prfLen = 0;
    unsigned char* profileBuff = flashAppInnerGet(FLASH_INNER_ITEM_PROFILE, &prfLen);

    if (NULL == profileBuff)
    {
        printf("Failed to get profile.\r\n");
        return FALSE;
    }

    if ((index == 0) || (index > 50) ||
        (NULL == strOemItem) || (len == 0))
    {
        free(profileBuff);
        return FALSE;
    }

    /* 确定profile中是否预留了boot param字段 */
    if (oemItemsLoaded == FALSE)
    {
        if (ERROR == loadItems())
        {
            printf("Failed to load oem items.\r\n");
        }
        DBG("Load oem items.\r\n");
    }

    oemTag = *(UINT32*)((unsigned char*)profileBuff + OEM_ITEM_OFFSET);
    
    DBG("OEM_VALID_WORD in flash is 0x%X\r\n", oemTag);

    if (OEM_VALID_WORD != oemTag)
    {
        printf("\nCurrent profile is not an OEM profile!\n");
        free(profileBuff);
        return ERROR;
    }

    index--;    /* 转换成从0开始 */
    memset(strOemItem, 0, len);

    offset = OEM_OPTION_ITEM_OFFSET(index);

    memcpy((UINT8*)(&size), profileBuff+offset, OEM_ITEM_SIZE);
    size = ntohl(size);

    if ((0 == size) || (size > 124) )
    {
        DBG("\nInvalid option OEM item length. size = %d\r\n", size);
        free(profileBuff);
        return FALSE;
    }
    else
    {
        offset += OEM_ITEM_SIZE;
        memcpy((UINT8*)(strOemItem), profileBuff+offset, size);
        strOemItem[size] = '\0';
        free(profileBuff);
        return TRUE;
    }
}
#endif

/* 默认buff长度与profile中项的长度一致 by liuzenglin, 22Jun13 */
LOCAL int _getProfileItem(PROFILE_ITEM_TYPE pit, char* buff, int len)
{
    int length = 0;
    unsigned char* profileBuff = flashAppInnerGet(FLASH_INNER_ITEM_PROFILE, &length);

    DBG("Get profile item, pit=%#x, len=%#x, profileMap[pit].offset=%#x, profileMap[pit].length=%#x\r\n", 
        pit, len, profileMap[pit].offset, profileMap[pit].length);
    if (NULL == profileBuff)
    {
        printf("Failed to get profile.\r\n");
        return ERROR;
    }

    if (! IS_VALID_PROFILE_ITEM(pit))
    {
        DBG("Unkown pit, pit=%d\r\n", pit);
        free(profileBuff);
        return ERROR;
    } 

    if(NULL == buff)
    {
        DBG("Null buff, buff=0x%p\r\n", buff);
        free(profileBuff);
        return ERROR;
    }

    if (len != profileMap[pit].length)
    {
        //printf("len is %d,pit is %d,profile item length is not same.\r\n",len,pit);
    }

    memcpy(buff, profileBuff+profileMap[pit].offset, len);

    free(profileBuff);

    return OK;    
}

#if 0

/* 默认buff长度与profile中项的长度一致 by liuzenglin, 22Jun13 */
LOCAL int _setProfileItem(PROFILE_ITEM_TYPE pit, const char* buff, int len)
{
    int length = 0;
    unsigned char* profileBuff = flashAppInnerGet(FLASH_INNER_ITEM_PROFILE, &length);

    DBG("Set profile item, pit=%#x, len=%#x, profileMap[pit].offset=%#x, profileMap[pit].length=%#x\r\n", 
        pit, len, profileMap[pit].offset, profileMap[pit].length);

    if (NULL == profileBuff)
    {
        printf("Failed to get profile.\r\n");
        return ERROR;
    }

    if (! IS_VALID_PROFILE_ITEM(pit))
    {
        DBG("Unkown pit, pit=%d\r\n", pit);
        free(profileBuff);
        return ERROR;
    }

    if(NULL == buff)
    {
        DBG("Null buff, buff=%#x\r\n", (int)buff);
        free(profileBuff);
        return ERROR;
    }

    memcpy(profileBuff+profileMap[pit].offset, buff, profileMap[pit].length);

    if (ERROR == flashAppInnerSet(FLASH_INNER_ITEM_PROFILE, profileBuff, length, 0))
    {
        printf("Failed to set profile.\r\n");
        free(profileBuff);
        return ERROR;
    }

    free(profileBuff);

    return OK;

}
#endif


/*!
*\fn                    STATUS sysGetMacFromFlash(unsigned char *s_pro)
*\brief                 get the mac from flash
*
*\param[in]     N/A
*\param[out]    s_pro   store the mac address
*
*\return                error code
*\retval                OK              success
*\retval                ERROR   fail
*
*\note
*/

int sysGetMacFromFlash(unsigned char *s_pro)
{
    unsigned char tmpMac[PROFILE_MAC_LENGTH];

    if(NULL == s_pro)
    {
        return ERROR;
    }

    if (ERROR == _getProfileItem(PROFILE_ITEM_MAC, (char *)s_pro, profileMap[PROFILE_ITEM_MAC].length))
    {
        DBG("Failed to get mac from flash.\r\n");
        s_pro[0] = 0x00;
        s_pro[1] = 0x0A;
        s_pro[2] = 0xEB;
        s_pro[3] = 0x00;
        s_pro[4] = 0x13;
        s_pro[5] = 0x01;
        return OK;
    }
    
    memset(tmpMac, 0xff, sizeof(tmpMac));
    if( 0 == memcmp(s_pro, tmpMac, sizeof(tmpMac)) )
    {
        s_pro[0] = 0x00;
        s_pro[1] = 0x0A;
        s_pro[2] = 0xEB;
        s_pro[3] = 0x00;
        s_pro[4] = 0x13;
        s_pro[5] = 0x01;
    }
 #if 0
    else if( s_pro[0] & 0x01)
    {
        s_pro[0] = 0x00;
        s_pro[1] = 0x0A;
        s_pro[2] = 0xEB;
        s_pro[3] = 0x00;
        s_pro[4] = 0x13;
        s_pro[5] = 0x01;
    }
#endif
    return OK;
}



/*!
 *\fn                   STATUS sysGetProductId(char *productId)
 *\brief                get the product id from the flash
 *
 *\param[in]    N/A
 *\param[out]   productId       store the product id
 *
 *\return               error code
 *\retval               TRUE    success
 *\retval               FALSE   fail
 *
 *\note
 */
STATUS sysGetProductId(unsigned char *productId)
{
    if(NULL == productId)
    {
        return ERROR;
    }

    if (ERROR == _getProfileItem(PROFILE_ITEM_PRODUCTID, (char *)productId, profileMap[PROFILE_ITEM_PRODUCTID].length))
    {
        printf("Failed to get productID.\r\n");
        return ERROR;
    }    

    return OK;
}
#if 0
/*!
 *\fn                   STATUS sysGetFlashSign(char *s_pro)
 *\brief                get the digit sign from flash
 *
 *\param[in]    N/A
 *\param[out]   s_pro   store the digit sign
 *\return               error code
 *\retval               OK              success
 *\retval               ERROR   fail
 *
 *\note
 */
STATUS sysGetFlashSign(char *s_pro, int len)
{
    if(NULL == s_pro)
    {
        return ERROR;
    }

    if (ERROR == _getProfileItem(PROFILE_ITEM_SIGN, s_pro, len))
    {
        printf("Failed to get sign.\r\n");
        return ERROR;
    }    

    return OK;

}

/*!
 *\fn                   STATUS sysGetFlashPubLicKey(char *s_pro)
 *\brief                get the public key from flash
 *
 *\param[in]    N/A
 *\param[out]   s_pro   store the digit sign
 *\return               error code
 *\retval               OK              success
 *\retval               ERROR   fail
 *
 *\note
 */
STATUS sysGetFlashPubLicKey(char *s_pro, int len)
{
    if(NULL == s_pro)
    {
        return ERROR;
    }

    if (ERROR == _getProfileItem(PROFILE_ITEM_PUBLICKEY, s_pro, len))
    {
        printf("Failed to get publickey.\r\n");
        return ERROR;
    }    

    return OK;
}

/*!
 *\fn                   STATUS sysSetMacToFlash(unsigned char *s_pro)
 *\brief                set the mac address to flash
 *
 *\param[in]    s_pro   the pointer of the mac address
 *\param[out]   N/A
 *
 *\return               error code
 *\retval               ERROR   fail
 *\retval               OK              success
 *
 *\note mac地址需要直接写入，不能通过内部区的接口写，内部区接口会自动屏蔽mac和rsa的写
 */
STATUS sysSetMacToFlash(unsigned char *s_pro)
{
    int valueOffset = 0;
    if(NULL == s_pro)
    {
        return ERROR;
    }
    
    if (ERROR == flashAppInnerGetOffset(FLASH_INNER_ITEM_PROFILE, &valueOffset))
    {
        printf("Fatal error: Failed to set gev content offset.\r\n");
        return ERROR; 
    }

    if (ERROR == writeFlash(s_pro, 
        profileMap[PROFILE_ITEM_MAC].length, valueOffset+profileMap[PROFILE_ITEM_MAC].offset, FALSE))
    {
        printf("Failed to set mac.\r\n");
        return ERROR;
    }    

    return OK;
}


/*!
 *\fn                   STATUS sysSetProductId(char *productId)
 *\brief                get the product id from the flash
 *
 *\param[in]    N/A
 *\param[out]   productId       store the product id
 *
 *\return               error code
 *\retval               TRUE    success
 *\retval               FALSE   fail
 *
 *\note
 */
STATUS sysSetProductId(unsigned char *productId)
{
    if(NULL == productId)
    {
        return ERROR;
    }

    if (ERROR == _setProfileItem(PROFILE_ITEM_PRODUCTID, productId, profileMap[PROFILE_ITEM_PRODUCTID].length))
    {
        printf("Failed to set productid.\r\n");
        return ERROR;
    }    

    return OK;
}

/*!
 *\fn                   STATUS sysSetFlashPublicKey(char *s_pro,int len)
 *\brief                set the public key to flash
 *
 *\param[in]
 *\param[out]
 *
 *\return
 *\retval
 *
 *\note
 */
STATUS sysSetFlashPublicKey(char *s_pro, int len)
{
    if(NULL == s_pro)
    {
        return ERROR;
    }

    if (ERROR == _setProfileItem(PROFILE_ITEM_PUBLICKEY, s_pro, len))
    {
        printf("Failed to set publickey.\r\n");
        return ERROR;
    }    

    return OK;
}

/*!
 *\fn                   STATUS sysSetFlashSign(char *s_pro,int len)
 *\brief                set the flash digit sign to flash
 *
 *\param[in]
 *\param[out]
 *
 *\return
 *\retval
 *
 *\note sign需要直接写入，不能通过内部区的接口写，内部区接口会自动屏蔽mac和rsa的写
 */
STATUS sysSetFlashSign(char *s_pro, int len)
{
    int valueOffset = 0;

    if(NULL == s_pro)
    {
        return ERROR;
    }
    
    if (ERROR == flashAppInnerGetOffset(FLASH_INNER_ITEM_PROFILE, &valueOffset))
    {
        printf("Fatal error: Failed to get gev content offset.\r\n");
        return ERROR; 
    }

    if (ERROR == writeFlash(s_pro, 
        profileMap[PROFILE_ITEM_SIGN].length, valueOffset+profileMap[PROFILE_ITEM_SIGN].offset, FALSE))
    {
        printf("Failed to set sign .\r\n");
        return ERROR;
    }
    return OK;
}

#endif

/*!
 *\fn           LOCAL UINT32 sysFirmwareCheckProduct(const unsigned char *pAppBuf)
 *\brief        check the product version of the firmware file.
 *
 *\param[in]    pAppBuf  pointer to the buffer
 *\param[out]   N/A
 *
 *\return       the product version identical with the switch or not
 *\retval       OK    the product version identical with the switch
 *\retval       ERROR the product version different from the switch
 *
 *\note
 */
UINT32 sysFirmwareCheckProduct(const unsigned char *pAppBuf)
{
    unsigned char curProdId[PROFILE_PRODUCTID_LENGTH];

    if (ERROR == sysGetProductId(curProdId))
    {
        return ERROR;
    }
    if( memcmp(pAppBuf, curProdId, PROFILE_PRODUCTID_LENGTH) )
    {
        return ERROR;
    }

    return OK;
}


