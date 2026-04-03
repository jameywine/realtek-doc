/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashAppInner.c
* version    :   V1.0
* description:   定义内部区的布局和操作接口.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/21/2013
*
* history    :
* 01   06/21/2013  LiuZenglin     Create.
* 内部区内容采取采用TLV的格式存储，管理内部区条目，每个条目的长度独自1个或几个
* sector，如profile条目总共占128K bytes格式如下:
* 4B    4B   4B     value_len   pad_len
* ----- ---- ------ ---------   -------
* valid type length value       FF
* 
* 本文件中所有的offset都是指针对FLASH头地址的偏移
* 内部区中有效条目后面必须跟一个为全FF的sector，避免读取时发生误判
******************************************************************************/
#include<common.h>
#include<vsprintf.h>
#include <tplink/flashApp.h>
#include <tplink/flashAppInner.h>
#include <tplink/flashAppInnerGev.h>
#include "malloc.h"


int flash_app_inner_debug = 1;

#define DBG(arg...) do{if(0){printf("%s,%d:",__FUNCTION__, __LINE__);printf(arg);}}while(0)

#define IF_FAIL_RET(expression) \
	do{\
		int __rv;\
		__rv = (expression);\
		if ( ERR_NO_ERROR != __rv ){\
			DBG("E - %d %s line %d\n",__rv,#expression,__LINE__);\
			return __rv;\
		}\
	}while(0)

#define IF_FAIL_DONE(ret,expression) \
	do{\
		ret = (expression);\
		if ( ERR_NO_ERROR != ret ){\
			DBG("E - %d %s line %d\n",ret,#expression,__LINE__);\
			goto done;\
		}\
	}while(0)
        
#define IF_FAIL_PRINT(expression) \
            do{\
                int __rv;\
                __rv = (expression);\
                if ( ERR_NO_ERROR != __rv ){\
                    DBG("E - %d %s line %d\n",__rv,#expression,__LINE__);\
                }\
            }while(0)

/* 写入时和读出时采用同样字节序 */
#define FLASH_INNER_ITEM_VALID_KEY  (0xAA55AA55)
#define FLASH_INNER_DFT_SIZE        (0x300000)
#define FLASH_INNER_ITEM_KEY_SIZE        (4)
#define FLASH_INNER_ITEM_TYPE_SIZE       (4)
#define FLASH_INNER_ITEM_LENGTH_SIZE     (4)
#define FLASH_INNER_ITEM_VALUE_OFFSET    (12)
#define FLASH_FIRST_INNER_ITEM      (FLASH_SIZE)    /* 第一个条目从最后开始 */
/* 根据值的长度求条目的长度 */
#define FLASH_INNER_ITEM_LENGTH(length)      (ROUND_SECTOR_UP(length+FLASH_INNER_ITEM_VALUE_OFFSET))

#define GET_FLASH_INNER_ITEM_VALID_KEY(offset, key) \
    readFlash((char*)key, FLASH_INNER_ITEM_KEY_SIZE, ROUND_ADDR_UP(offset-FLASH_INNER_ITEM_KEY_SIZE))
#define SET_FLASH_INNER_ITEM_VALID_KEY(offset, key) \
    writeFlash((char*)key, FLASH_INNER_ITEM_KEY_SIZE, ROUND_ADDR_UP(offset-FLASH_INNER_ITEM_KEY_SIZE), 0)
#define GET_FLASH_INNER_ITEM_TYPE(offset, type) \
    readFlash((char*)type, FLASH_INNER_ITEM_TYPE_SIZE, \
    ROUND_ADDR_UP(offset-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE))
#define SET_FLASH_INNER_ITEM_TYPE(offset, type) \
    writeFlash((char*)type, FLASH_INNER_ITEM_TYPE_SIZE, \
    ROUND_ADDR_UP(offset-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE), 0)
#define GET_FLASH_INNER_ITEM_LENGTH(offset, length) \
    readFlash((char*)length, FLASH_INNER_ITEM_LENGTH_SIZE, \
    ROUND_ADDR_UP(offset-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE))
#define SET_FLASH_INNER_ITEM_LENGTH(offset, length) \
    writeFlash((char*)length, FLASH_INNER_ITEM_LENGTH_SIZE, \
    ROUND_ADDR_UP(offset-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE), 0)

/* 采用链表的形式保存内部区的内容，在读取内部区的时初始化fii_head */
typedef struct _FLASH_INNER_ITEM
{
    FLASH_INNER_ITEM_TYPE type;
    int                   value_offset;     /* 值的偏移量 */
    int                   length;
    unsigned char*        pBuf;             /* 删除内部项时使用，初始化时为NULL */
}FLASH_INNER_ITEM;

typedef struct _FII_NODE
{
    FLASH_INNER_ITEM    flInnerItem;
    struct _FII_NODE*   next;
}FII_NODE;

/* 记录内部区的边界偏移，用于刷新内部区 */
extern void free(void*);

LOCAL int flashInnerEdgeOffset = 0;
LOCAL FII_NODE *fii_head;
LOCAL int flashAppInnerInited = FALSE;
LOCAL void l_freeFlashInnerItems(void)      /* 释放链表 */
{
    FII_NODE* pFree = fii_head;
    FII_NODE* pTemp = NULL;
    while (pFree != NULL)
    {
        pTemp = pFree->next;
        if (pFree->flInnerItem.pBuf != NULL)
        {
            free(pFree->flInnerItem.pBuf);
        }
        free(pFree);
        pFree = pTemp;
    }
    fii_head = NULL;
    return;
}

/* 读取并初始化fii_list，只依赖驱动 */
int flashAppInnerInit_patch(void)
{
    const int valid_key = FLASH_INNER_ITEM_VALID_KEY;
    int key = 0;
    int item_offset = FLASH_FIRST_INNER_ITEM;   /* 内部区条目的起始位置 */
    int value_offset = 0;                       /* 值字段的起始位置 */
    int type = 0;
    int length = 0;
    FII_NODE* pCur = NULL;    /* 当前可用链表节点 */
    FII_NODE* pParent = NULL;
    int count = 0;

    if (fii_head != NULL)
    {
        DBG("Reinit the inner item list.\r\n");
        l_freeFlashInnerItems();
    }

    flashInnerEdgeOffset = FLASH_FIRST_INNER_ITEM;

    GET_FLASH_INNER_ITEM_VALID_KEY(item_offset, &key);
    DBG("count=%d, key=%#x, item_offset=%#x\r\n", count, key, item_offset);
    while (key == valid_key)
    {
        GET_FLASH_INNER_ITEM_TYPE(item_offset, &type);
        //type = ntohl(type);
        GET_FLASH_INNER_ITEM_LENGTH(item_offset, &length);
        //length = ntohl(length);

        /* 更新内部长度变量 */
        value_offset = ROUND_ADDR_DOWN(item_offset - FLASH_INNER_ITEM_VALUE_OFFSET - length);

        DBG("%d Type %#x, item_offset %#x, length %#x, value_offset %#x\r\n", 
            count, type, item_offset, length, value_offset);
        pCur = (FII_NODE*)calloc(1, sizeof(FII_NODE));
        if (pCur == NULL)
        {
            printf("Fatal error: Failed to allocate memory.\r\n");
            l_freeFlashInnerItems();
            flashAppInnerInited = TRUE;
            return ERROR;
        }

        pCur->flInnerItem.length = length;
        pCur->flInnerItem.type   = type;
        pCur->flInnerItem.value_offset = value_offset;
        pCur->flInnerItem.pBuf = NULL;      /* 保留 */
        pCur->next = NULL;

        if (count == 0)
        {
            fii_head = pCur;
            pParent  = fii_head;
            pCur = NULL;
        }
        else
        {
            pParent->next = pCur;
            pParent = pCur;
            pCur = NULL;
        }
        count++;

        /* 值字段的起始位置也是下一个条目的开始处 */
        item_offset = ROUND_SECTOR_DOWN(value_offset); /* 内部区条目的边界都应该是sector对齐 */
        flashInnerEdgeOffset = item_offset;
        GET_FLASH_INNER_ITEM_VALID_KEY(item_offset, &key);
        DBG("count=%d, key=%#x, item_offset=%#x\r\n", count, key, item_offset);
    }

    flashAppInnerInited = TRUE;
    return OK;
}

void flashAppInnerEnum(int *sn,int *license)
{
	*sn = FLASH_INNER_SN;
	*license = FLASH_INNER_LICENSE;
}
/* 意外掉电时，可能出现gev区域被擦除，使得profile区域和其它区域丢失。
 * 保证GEV项保存为最后一个内部区条目，不修改bootrom的前提下都不会丢失其它区域 */
int flashAppInnerInit(void)
{
    int offset = 0;
    int sectorSize = flashGetSectorSize();
    char* pbuf = NULL;
    int key,type = 0;
    unsigned int length,size = 0;
    const int valid_key = FLASH_INNER_ITEM_VALID_KEY;
    int temp = 0;
    int valid_edge = FLASH_FIRST_INNER_ITEM;
    int gev_edge = -1;

    /* 保证GEV项在最后一个内部区条目*/
    GET_FLASH_INNER_ITEM_VALID_KEY(FLASH_FIRST_INNER_ITEM, &key);
    GET_FLASH_INNER_ITEM_TYPE(FLASH_FIRST_INNER_ITEM, &type);
    GET_FLASH_INNER_ITEM_LENGTH(FLASH_FIRST_INNER_ITEM, &length);
    DBG("Inner items precheck: key=%#x, type=%#x, length=%#x, item_offset=%#x\r\n", 
        key, type, length, FLASH_FIRST_INNER_ITEM);
    while ((key == valid_key) && (offset < FLASH_INNER_DFT_SIZE) && (length > 0))
    {
        if (type == FLASH_INNER_ITEM_GEV)
        {
            gev_edge = FLASH_FIRST_INNER_ITEM - offset - ROUND_SECTOR_UP(length);
        }
        valid_edge = FLASH_FIRST_INNER_ITEM - offset - ROUND_SECTOR_UP(length);
        offset = offset + ROUND_SECTOR_UP(length);


        GET_FLASH_INNER_ITEM_VALID_KEY(FLASH_FIRST_INNER_ITEM-offset, &key);
        GET_FLASH_INNER_ITEM_TYPE(FLASH_FIRST_INNER_ITEM-offset, &type);
        GET_FLASH_INNER_ITEM_LENGTH(FLASH_FIRST_INNER_ITEM-offset, &length);
        DBG("Inner items precheck: key=%#x, type=%#x, length=%#x, item_offset=%#x\r\n", 
                key, type, length, FLASH_FIRST_INNER_ITEM-offset);
    }
    


    DBG("valid edge is %#x, gev edge is %#x\r\n", valid_edge, gev_edge);
    if (valid_edge == gev_edge)   /* GEV是最后一个内部项 */
    {
        DBG("Gev is stored in the last inner item\r\n");
    }
    else
    {
        /* gev不存在，则在最后一个内部条目处写入默认gev */
        if (gev_edge == -1)
        {
            gev_edge = valid_edge - sectorSize;
            flashErase(gev_edge/sectorSize, 1);
            if (FLASH_FIRST_INNER_ITEM - gev_edge > FLASH_INNER_DFT_SIZE)
            {
                printf("Fatal error: inner area is full, valid_edge is %#x\r\n", valid_edge);
                return ERROR;
            }

			
            pbuf = getDftGevContent(&temp);
            if (pbuf == NULL)
            {
                printf("Fatal error: get default gev content failed.\r\n");
                return ERROR;
            }
					
           size = temp;

            /* 设置长度 */
			printf("SET_FLASH_INNER_ITEM_LENGTH  start............. \n");
            SET_FLASH_INNER_ITEM_LENGTH(valid_edge, &temp); 
			printf("SET_FLASH_INNER_ITEM_LENGTH  ok.............. \n");
            temp = FLASH_INNER_ITEM_GEV;
            SET_FLASH_INNER_ITEM_TYPE(valid_edge, &temp);  
            SET_FLASH_INNER_ITEM_VALID_KEY(valid_edge, &valid_key); 
		
        }
        else    /* 存在，则轮换 */
        {

            size = gev_edge-valid_edge+sectorSize;
            //pbuf = (char*)calloc(size, 1);
         
            pbuf = (char *)BUF_ADDR;
            if (pbuf == NULL)
            {
                printf("Fatal error: alloc memory failed.\r\n");
                return ERROR;
            }
            readFlash(pbuf, sectorSize, gev_edge);
            readFlash(pbuf+sectorSize, size-sectorSize, valid_edge);

            gev_edge = valid_edge;
		
        }

        /* 写入内部区 */
        DBG("pbuf=0x%p, size=%#x, gev_edge=%#x\r\n", pbuf, size, gev_edge);
        writeFlash(pbuf, size, gev_edge, FALSE);

        //free(pbuf);
        pbuf = NULL;
    }

    return flashAppInnerInit_patch();
}

int flashAppInnerSet(FLASH_INNER_ITEM_TYPE type, char* buff, int len, int prompt)
{
    FII_NODE *pCur = NULL;
   // FII_NODE *pParent = NULL;
    unsigned char * pbuf1 = NULL;   /* 读出来的值 */
    unsigned char * ptemp = NULL;
    unsigned char * pbuf2 = NULL;   /* 生成将要写入的值 */
    int oldSize = 0;                /* 重写之前内部区的长度 */
    int newSize = 0;                /* 重写时内部区的长度 */
    int itemStartOffset = 0;        /* 重写内部区条目的起始偏移 */
    int itemNextStartOffset = 0;    /* 重写内部区条目的下一条的起始偏移 */
    int itemOldLength = 0;          /* 重写内部区条目的原长度 */
    int itemNewLength = 0;          /* 重写内部区条目的新长度 */
    int tempSize = 0;
    int sectorSize = FLASH_SECTOR_SIZE;
    const int valid_key = FLASH_INNER_ITEM_VALID_KEY;

    if ((NULL == buff) || (0 == len))
    {
        printf("Fatal error: buff=0x%p, len=%#x", buff, len);
        return ERROR;
    }

    if (flashAppInnerInited == FALSE)
    {
        if (ERROR == flashAppInnerInit())
        {
            printf("Fatal error: Failed to init inner area structure.\r\n");
            return ERROR;
        }
    }

   // pParent = fii_head;
    pCur = fii_head;

    while (pCur != NULL)
    {
        DBG("flashAppInnerSet: type=%#x, pCur->flInnerItem.type=%#x\r\n",
            type, pCur->flInnerItem.type);
        if (pCur->flInnerItem.type == type)
        {
            DBG("Modify the inner item, type %d, length %d, value_offset %d.\r\n",
                type, len, pCur->flInnerItem.value_offset);

            DBG("Rewrite the inner area, new length is %#x, old length is %#x\r\n",
                len, pCur->flInnerItem.length);
            /* flash驱动中不会重写相同内容，所以都更新整个内部区 */
            goto REFRESH_INNER_AREA;
            break;
        }
        else
        {
           // pParent = pCur;
            pCur = pCur->next;
        }
    }

    if (pCur == NULL)
    {
        DBG("Write a new item to flash inner, type %d, length %d.\r\n", type, len);
    }

    itemNewLength = FLASH_INNER_ITEM_LENGTH(len);
    oldSize = FLASH_FIRST_INNER_ITEM - flashInnerEdgeOffset;
    newSize = oldSize + itemNewLength;
    itemStartOffset = FLASH_FIRST_INNER_ITEM - oldSize;

    DBG("itemnewlen=%#x,itemstartoffset=%#x,oldsize=%#x,newsize=%#x\r\n",
        itemNewLength,itemStartOffset,oldSize,newSize+ sectorSize);

    /* 由于是新写入一个条目，为了避免历史信息的影响，把后续一个sector写成全FF */
    //pbuf1 = (unsigned char*)calloc(1, newSize+sectorSize);
    pbuf1 = (unsigned char*)BUF_ADDR;
    memset(pbuf1, 0, (newSize+sectorSize));
    if (NULL == pbuf1)
    {
        printf("Fatal error: Failed to allocate memory for write inner area.\r\n");
        return ERROR;
    }
    memset(pbuf1, 0xff, newSize+sectorSize);
    ptemp = pbuf1 + sectorSize;
    
    if (0 != oldSize)   /* 内部区不为空 */
    {
        /* 读取最后一项gev */
        if (ERROR == readFlash((char *)ptemp, sectorSize, itemStartOffset))
        {
            //free(pbuf1);
            printf("Fatal error: Failed to read inner area.\r\n");
            return ERROR;        
        }

        if (ERROR == readFlash((char *)(ptemp + sectorSize + itemNewLength), oldSize-sectorSize, itemStartOffset+sectorSize))
        {
            //free(pbuf1);
            printf("Fatal error: Failed to read inner area.\r\n");
            return ERROR;        
        }

        ptemp = ptemp+sectorSize+itemNewLength; /* 新item的偏移 */
    }
    else    /* 空内部区 */
    {
        /* 不需要读取任何变量 */
        ptemp = ptemp+itemNewLength;
    }

    /* 写入KEY到buffer */
    memcpy(ptemp-FLASH_INNER_ITEM_KEY_SIZE, (unsigned char*)(&valid_key), FLASH_INNER_ITEM_KEY_SIZE);

    /* 写入TYPE到buffer */
    //type = ntohl(type);
    memcpy(ptemp-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE, 
        (unsigned char*)(&type), FLASH_INNER_ITEM_TYPE_SIZE);

    /* 写入LENGTH到buffer */
    //int wlen = ntohl(len);
    memcpy(ptemp-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE, 
        (unsigned char*)(&len), FLASH_INNER_ITEM_LENGTH_SIZE);
    /* 写入VALUE到buffer */
    memcpy(ptemp-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE-len,
        buff, len);

    /* 写入新条目时，必须保证后面一个sector为全FF */
    flashInnerEdgeOffset = FLASH_FIRST_INNER_ITEM-newSize;
    flashErase((flashInnerEdgeOffset-sectorSize)/sectorSize, (newSize+sectorSize)/sectorSize);
    if (ERROR == writeFlash((char*)pbuf1, newSize+sectorSize, flashInnerEdgeOffset-sectorSize, prompt))
    {
        //free(pbuf1);
        printf("Fatal error: Failed to write inner area.\r\n");
        return ERROR;
    }    
    DBG("\r\nSet inner item ok, type=%d startOffset=%#x length=%d flashInnerEdgeOffset=%#x.\r\n", 
        type, itemStartOffset, len, flashInnerEdgeOffset);

    //free(pbuf1);
    flashAppInnerInited = FALSE;    /* 需要重新初始化 */

    return OK;

REFRESH_INNER_AREA:
    itemOldLength = FLASH_INNER_ITEM_LENGTH(pCur->flInnerItem.length);
    itemNewLength = FLASH_INNER_ITEM_LENGTH(len);
    itemNextStartOffset = ROUND_SECTOR_DOWN(pCur->flInnerItem.value_offset);
    itemStartOffset = itemNextStartOffset + itemOldLength;
    oldSize = FLASH_FIRST_INNER_ITEM - flashInnerEdgeOffset;
    newSize = oldSize - itemOldLength + itemNewLength;
    DBG("itemoldlen=%#x,itemnewlen=%#x,itemstartoffset=%#x,itemnextstartoffset=%#x,oldsize=%#x,newsize=%#x\r\n",
        itemOldLength,itemNewLength,itemStartOffset,itemNextStartOffset,oldSize,newSize);
    //pbuf1 = (unsigned char*)calloc(1, oldSize);
    pbuf1 = (unsigned char*)BUF_ADDR;
    if (NULL == pbuf1)
    {
        printf("Fatal error: Failed to allocate memory for write inner area, size=%#x.\r\n", oldSize);
        return ERROR;
    }
    //pbuf2 = (unsigned char*)calloc(1, newSize);
    pbuf2 = (unsigned char*)(BUF_ADDR + oldSize);
    if (NULL == pbuf2)
    {
        //free(pbuf1);
        printf("Fatal error: Failed to allocate memory for write inner area, size=%#x.\r\n", newSize);
        return ERROR;
    }

    if (ERROR == readFlash((char *)pbuf1, oldSize, flashInnerEdgeOffset))
    {
        //free(pbuf1);
        //free(pbuf2);
        printf("Fatal error: Failed to read inner area.\r\n");
        return ERROR;        
    }
    memset(pbuf2, 0xff, newSize);

    /* 把需要修改的内部项的前面内容拷贝到新地址处 */
    tempSize = itemNextStartOffset-flashInnerEdgeOffset;
    memcpy(pbuf2 , pbuf1, tempSize);

    /* KEY和TYPE在合并后部分时写入，保持不变 */
    /* 更新长度 */
    //len = ntohl(len);
    memcpy(pbuf2+tempSize+itemNewLength\
    -FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE,\
    (unsigned char*)(&len), FLASH_INNER_ITEM_LENGTH_SIZE);
    /* 拷贝新内容到指定地址 */
    memcpy(pbuf2+tempSize+itemNewLength\
    -FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE-len,\
    buff, len);

    /* 如果为PROFILE，需要保留MAC地址和数字签名 */
    if (pCur->flInnerItem.type == FLASH_INNER_ITEM_PROFILE)
    {
       
        /* MAC和RSA为前面的6+128个字符 */
        memcpy(pbuf2+tempSize+itemNewLength\
    -FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE-len,\
        pbuf1+tempSize+itemOldLength\
    -FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE-len,\
        6+128);
    }


    /* 把需要修改的内部项的后面内容拷贝到新地址处 */
    /* 包括TYPE和key */
    memcpy(pbuf2+tempSize+itemNewLength-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE , 
    pbuf1+tempSize+itemOldLength-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE, 
        FLASH_FIRST_INNER_ITEM-itemStartOffset+FLASH_INNER_ITEM_KEY_SIZE+FLASH_INNER_ITEM_TYPE_SIZE);

    flashInnerEdgeOffset = FLASH_FIRST_INNER_ITEM-newSize;

    /* 如果条目变小，则在后面填充0xff, 使用buf1中内容写入 */
    if (newSize < oldSize)
    {
        unsigned char* temp = pbuf1;
        memset(pbuf1, 0xff, oldSize);
        memcpy(pbuf1, pbuf2, newSize);
        /* 交换指针的值，便于后续统一处理 */
        pbuf1 = pbuf2;
        pbuf2 = temp;
        newSize = oldSize;
    }
    //free(pbuf1);

    flashErase((FLASH_FIRST_INNER_ITEM-newSize)/sectorSize, newSize/sectorSize);
    if (ERROR == writeFlash((char*)pbuf2, newSize, FLASH_FIRST_INNER_ITEM-newSize, prompt))
    {
        //free(pbuf2);
        printf("Fatal error: Failed to write inner area.\r\n");
        return ERROR;
    }

    DBG("Set inner item ok, type=%d length=%d, flashInnerEdgeOffset=%#x.\r\n", 
        type, len, flashInnerEdgeOffset);

    //free(pbuf2);

    /* 当条目长度不一致时需要重新获取内部区的参数 */
    if (len != pCur->flInnerItem.length)
    {
        flashAppInnerInited = FALSE;
    }

    return OK;
}

/* 删除内部条目 */
int flashAppInnerUnset(FLASH_INNER_ITEM_TYPE type, int prompt)
{
    FII_NODE *pCur = NULL;
    FII_NODE *pParent = NULL;
    FII_NODE *pTempParent = NULL;
    FII_NODE *pTemp = NULL;
    unsigned int valid_key = 0;
    int len = 0;
    int  innerItemSize = 0;
    int  innerItemWriteOffset = 0;          /* 写flash的偏移地址 */
    unsigned char* innerItemBuf = 0;

    if (flashAppInnerInited == FALSE)
    {
        if (ERROR == flashAppInnerInit())
        {
            printf("Fatal error: Failed to init inner area structure.\r\n");
            return ERROR;
        }
    }

    /* 遍历所有节点 */
    pCur = fii_head;
    pParent = fii_head;
    while (pCur != NULL)
    {
        DBG("Unset inner item, type=%#x, pCur type=%#x\r\n", type, pCur->flInnerItem.type);
        if (pCur->flInnerItem.type == type)
        {
            break;
        }
        else
        {
            pParent = pCur;
            pCur = pCur->next;
        }
    }

    if (pCur == NULL)
    {
        DBG("Inner item is not exist, type=%#x.\r\n", type);
    }
    else
    {
        DBG("Remove inner item, type=%#x inner_item_len=%#x value_offset=%#x.\r\n",
            pCur->flInnerItem.type, pCur->flInnerItem.length, pCur->flInnerItem.value_offset);
        /* 此时已经确认内部项存在了，需要删除该项 */
        pTempParent = pParent;
        innerItemWriteOffset = (pCur == fii_head)? \
            FLASH_FIRST_INNER_ITEM : ROUND_SECTOR_DOWN(pParent->flInnerItem.value_offset);
        pTemp = pCur;   /* 保存要删除的节点 */

        /* 遍历后面所有节点，并读取相应值 */
        pParent = pCur;
        pCur = pCur->next;
        while (pCur != NULL)
        {
            DBG("Get inner item in unsetting, type=%#x\r\n", pCur->flInnerItem.type);

            /* 这部分内存在写入FLASH之后会释放 */
            pCur->flInnerItem.pBuf = (unsigned char*)calloc(1, pCur->flInnerItem.length);
            if (pCur->flInnerItem.pBuf == NULL)
            {
                printf("Fatal error: Failed to allocate memory, size=%#x.\r\n", pCur->flInnerItem.length);
                return ERROR;
            }
            readFlash((char *)pCur->flInnerItem.pBuf, pCur->flInnerItem.length, pCur->flInnerItem.value_offset);

            pParent = pCur;
            pCur = pCur->next;
        }

        DBG("Remove inner item, refresh flash.\r\n");

        /* 需要重新初始化 */
        flashAppInnerInited = FALSE;
        pParent = pTempParent;
        pCur = pTemp->next;

        /* 如果没有后续条目，则设置当前块为全0XFF */
        if (pCur == NULL)
        {
            //innerItemBuf = (unsigned char*)calloc(1, FLASH_SECTOR_SIZE);
            innerItemBuf = (unsigned char *)BUF_ADDR;
            if (NULL == innerItemBuf)
            {
                printf("Fatal error: Failed to allocate memory for write inner area.\r\n");
                return ERROR;
            }
            memset(innerItemBuf, 0xff, FLASH_SECTOR_SIZE);

            DBG("Clear current item.\r\n");                
            if (ERROR == writeFlash((char *)innerItemBuf, FLASH_SECTOR_SIZE, innerItemWriteOffset-FLASH_SECTOR_SIZE, TRUE))
            {
                printf("\r\nError: Failed to write flash, innerItemBuf=0x%p, innerItemSize=%#x, innerItemStartOffset=%#x\r\n",
                    innerItemBuf, innerItemSize, innerItemWriteOffset);
            }
            DBG("\r\n");
        }
        else
        {
            /* 把后面的条目写入FLASH */
            while (pCur != NULL)
            {
                if (NULL != pCur->flInnerItem.pBuf)
                {
                    innerItemSize = FLASH_INNER_ITEM_LENGTH(pCur->flInnerItem.length);
                    //innerItemBuf = (unsigned char*)calloc(1, innerItemSize+FLASH_SECTOR_SIZE);
                    innerItemBuf = (unsigned char *)BUF_ADDR;
                    if (NULL == innerItemBuf)
                    {
                        printf("Fatal error: Failed to allocate memory for write inner area.\r\n");
                        return ERROR;
                    }

                    /* 更新写入的偏移地址 */
                    innerItemWriteOffset -= innerItemSize;
                    
                    memset(innerItemBuf, 0xff, innerItemSize+FLASH_SECTOR_SIZE);

                    /* 写入KEY到buffer */
                    valid_key = FLASH_INNER_ITEM_VALID_KEY;
                    memcpy(innerItemBuf+FLASH_SECTOR_SIZE+innerItemSize-FLASH_INNER_ITEM_KEY_SIZE, (unsigned char*)(&valid_key), FLASH_INNER_ITEM_KEY_SIZE);
                    /* 写入TYPE到buffer */
                    //type = ntohl(pCur->flInnerItem.type);
                    memcpy(innerItemBuf+FLASH_SECTOR_SIZE+innerItemSize-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE, 
                        (unsigned char*)(&type), FLASH_INNER_ITEM_TYPE_SIZE);
                    /* 写入LENGTH到buffer */
                    //len = ntohl(pCur->flInnerItem.length);
                    memcpy(innerItemBuf+FLASH_SECTOR_SIZE+innerItemSize-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE, 
                        (unsigned char*)(&len), FLASH_INNER_ITEM_LENGTH_SIZE);
                    /* 写入VALUE到buffer */
                    memcpy(innerItemBuf+FLASH_SECTOR_SIZE+innerItemSize-FLASH_INNER_ITEM_KEY_SIZE-FLASH_INNER_ITEM_TYPE_SIZE-FLASH_INNER_ITEM_LENGTH_SIZE-pCur->flInnerItem.length,
                        pCur->flInnerItem.pBuf, pCur->flInnerItem.length);
                    DBG("innerItemSize=%#x,value_length=%#x,innerItemStartOffset=%#x\r\n",
                        innerItemSize,pCur->flInnerItem.length,innerItemWriteOffset);                
                    if (ERROR == writeFlash((char *)innerItemBuf, innerItemSize+FLASH_SECTOR_SIZE, innerItemWriteOffset-FLASH_SECTOR_SIZE, TRUE))
                    {
                        printf("\r\nError: Failed to write flash, innerItemBuf=0x%p, innerItemSize=%#x, innerItemStartOffset=%#x\r\n",
                            innerItemBuf, innerItemSize, innerItemWriteOffset);
                    }

                    free(pCur->flInnerItem.pBuf);   /* 释放读取出来的内存 */
                    pCur->flInnerItem.pBuf = NULL;
                }
                pParent = pCur;
                pCur = pCur->next;
            }
        }
        DBG("\r\nRemove inner item done.\r\n");
    }
    return OK;
}

/* 获取内部条目，长度由TLV指定 */
/* 返回动态申请的内存，上层使用完之后释放, length为返回buf的长度 */
unsigned char* flashAppInnerGet(FLASH_INNER_ITEM_TYPE type, int *length)
{
    FII_NODE *pCur = NULL;
//    FII_NODE *pParent = NULL;
    unsigned char* buf = NULL;

    if (flashAppInnerInited == FALSE)
    {
        if (ERROR == flashAppInnerInit())
        {
            printf("Fatal error: Failed to init inner area structure.\r\n");
            return NULL;
        }
    }

    /* 遍历所有节点 */
    pCur = fii_head;
  //  pParent = fii_head;
    while (pCur != NULL)
    {
        DBG("Get inner item, type=%#x, pCur type=%#x\r\n", type, pCur->flInnerItem.type);
        if (pCur->flInnerItem.type == type)
        {
            *length = pCur->flInnerItem.length;
            buf = (unsigned char*)calloc(1, *length);
            if (buf == NULL)
            {
                printf("Fatal error: Failed to allocate memory, size=%#x.\r\n", *length);
                return NULL;
            }
            readFlash((char *)buf, *length, pCur->flInnerItem.value_offset);
            break;
        }
        else
        {
       //    pParent = pCur;
            pCur = pCur->next;
        }
    }

    if (pCur == NULL)
    {
        DBG("Failed to get inner item, type=%#x.\r\n", type);
        return NULL;
    }
    else
    {
        DBG("Get inner item ok, type=%#x inner_item_len=%#x value_offset=%#x.\r\n",
            pCur->flInnerItem.type, pCur->flInnerItem.length, pCur->flInnerItem.value_offset);
        return buf;
    }
}

/* 返回内部项在FLASH中的偏移地址 */
int flashAppInnerGetOffset(FLASH_INNER_ITEM_TYPE type, int *value_offset)
{
    FII_NODE *pCur = NULL;
//    FII_NODE *pParent = NULL;

    if (flashAppInnerInited == FALSE)
    {
        if (ERROR == flashAppInnerInit())
        {
            printf("Fatal error: Failed to init inner area structure.\r\n");
            return ERROR;
        }
    }
    *value_offset = 0;

    /* 遍历所有节点 */
    pCur = fii_head;
  //  pParent = fii_head;
    while (pCur != NULL)
    {
        DBG("Get inner item, type=%#x, pCur type=%#x\r\n", type, pCur->flInnerItem.type);
        if (pCur->flInnerItem.type == type)
        {
            *value_offset = pCur->flInnerItem.value_offset;
            break;
        }
        else
        {
         //   pParent = pCur;
            pCur = pCur->next;
        }
    }

    if (pCur == NULL)
    {
        DBG("Failed to get inner item value_offset, type=%#x.\r\n", type);
        return ERROR;
    }
    else
    {
        DBG("Get inner item value_offset ok, type=%#x inner_item_len=%#x value_offset=%#x.\r\n",
            pCur->flInnerItem.type, pCur->flInnerItem.length, pCur->flInnerItem.value_offset);
        return OK;
    }

}

/* 复位内部区，删除除profile和gev之外的其它所有项 */
int flashAppInnerReset(const char*flBootSize, const char*flFsOffset, const char*flFsSize, const char*flVer)
{
    FII_NODE *pCur = fii_head;

    if (flashAppInnerInited == FALSE)
    {
        if (ERROR == flashAppInnerInit())
        {
            printf("Fatal error: Failed to init inner area structure.\r\n");
            return ERROR;
        }
    }

    if ((flBootSize == NULL) ||
        (flFsOffset == NULL) ||
        (flFsSize == NULL) ||
        (flVer == NULL))
    {
        printf("Fatal error: Bad parameter, flBootSize=0x%p, flFsOffset=0x%p, flFsSize=0x%p, flVer=0x%p\r\n",
            flBootSize, flFsOffset, flFsSize, flVer);
        return ERROR;
    }
    DBG("Reset inner item with: flBootSize=%s, flFsOffset=%s, flFsSize=%s, flVer=%s\r\n",
            flBootSize, flFsOffset, flFsSize, flVer);

    while (pCur != NULL)
    {
        DBG("Try to reset inner item, type=%#x, value_offset=%#x, length=%#x\r\n", 
            pCur->flInnerItem.type, pCur->flInnerItem.value_offset, pCur->flInnerItem.length);
        if (pCur->flInnerItem.type == FLASH_INNER_ITEM_PROFILE) /* 不删除profile */
        {
            pCur = pCur->next;
            continue;
        }
        else if (pCur->flInnerItem.type == FLASH_INNER_ITEM_GEV)
        {
            /* 先删除GEV中非必须项 */
            if (ERROR == updateGevItem(flBootSize, flFsOffset, flFsSize, flVer))
            {
                printf("Fatal error: Failed to clear gev.\r\n");
                return ERROR;
            }

            DBG("Update gev ok.\r\n");
            pCur = pCur->next;
            continue;      
        }
        else    /* 删除其它内部项 */
        {
            if (ERROR == flashAppInnerUnset(pCur->flInnerItem.type, FALSE))
            {
                printf("Fatal error: Failed to remove inner item, type=%#x, value_offset=%#x, length=%#x\r\n", 
            pCur->flInnerItem.type, pCur->flInnerItem.value_offset, pCur->flInnerItem.length);
                return ERROR;                
            }
            DBG("Remove innner item ok, type=%#x, value_offset=%#x, length=%#x\r\n", 
            pCur->flInnerItem.type, pCur->flInnerItem.value_offset, pCur->flInnerItem.length);

            /* 经过unset之后，原链表需要重新, 递归调用 */
            if (ERROR == flashAppInnerReset(flBootSize, flFsOffset, flFsSize, flVer))
            {
                DBG("Failed to recurse.\r\n");
                return ERROR;
            }

            return OK;
        }
    }

    return OK;   
}


