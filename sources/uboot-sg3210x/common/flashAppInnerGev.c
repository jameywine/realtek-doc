/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashAppGev.c
* version    :   V1.0
* description:   环境变量相关接口.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/18/2013
*
* history    :
* 01   06/18/2013  LiuZenglin     Create.
*   环境变量以NAME VALUE的形式存储，环境变量区共128K-12B(valid-key, type, length)，
*   初始值为全FF，最开始512B为bootrom参数，接着是254B的byte图
*   ，接着是246BYTE的预留字段，剩余127K全部用于保存环境变量，
*   每条环境变量占512B，其中NAME和VALUE各占256B，环境变量区总共可以保存254条环境变量。
*
*   默认环境变量有:
* 
序号 NAME               DEFAULT VALUE                   DESCRIPTION
1	START_NO_CONFIG	    0	                            表示启动时是否读取配置，
                                                        用于实现启动时忽略配置的功能
2	START_CONFIG	    flash:startup_config.cfg	    主配置文件
3	BACKUP_CONFIG	    flash:backup_config.cfg	        备份配置文件
4	START_IMAGE	        flash:image1.bin	            主镜像
5	BACKUP_IMAGE	    flash:image2.bin	            备份镜像
6	FL_VER	            0x10000                         表明当前Flash版本号为V1.0.0
7	FL_SIZE_BOOT	    0x100000	                    bootrom区的大小
8	FL_OFFSET_FS	    0x100000	                    文件系统区的偏移地址
9	FL_SIZE_FS	        0x1c00000(32M) 0xc00000(16M)    文件系统区的大小    (FL_SIZE-4M)
*更新环境变量策略:
*  为了使环境变量区更加效率，减少擦除次数
*  采用byte位域图来指示后续254个环境变量的使用情况:
* 0  --- invalid
* 1  --- valid
* ff --- blank
* 当添加或者修改环境变量后，检查是否还有BLANK空间(位图为0xff)如果没有则擦除整个
* sector，保留bootrom参数的512B、环境变量个数，其它部分全部重新初始化，包括位域图，由
* gevSetBack()实现
******************************************************************************/
#include<common.h>
#include<vsprintf.h>
#include <tplink/flashApp.h>
#include <tplink/flashAppInner.h>
#include <tplink/flashAppFsImage.h>
#include <tplink/flashAppInnerGev.h>
#include "malloc.h"

/* 环境变量区各项布局 by liuzenglin, 15Aug12 */
/* GEV总共占128K-12B */
/* 512B */
#define ENV_BOOTPARAM_OFFSET    0                   /* 最开始为bootrom参数 */
#define ENV_BOOTPARAM_LENGTH    (BOOT_PARAM_SIZE)
/* 500B */
#define ENV_BMP_OFFSET          (ENV_BOOTPARAM_OFFSET + ENV_BOOTPARAM_LENGTH)
//#define ENV_BMP_LENGTH          254     /* 存储后面254个环境变量的状态 */
#define ENV_BMP_LENGTH          126 
#define ENV_RESERVE_OFFSET      (ENV_BMP_OFFSET + ENV_BMP_LENGTH)
//#define ENV_RESERVE_LENGTH      246     /* 保留字段，后续还剩127K */
#define ENV_RESERVE_LENGTH 374
/* 127K */
#define ENV_CONTENT_OFFSET        (ENV_RESERVE_OFFSET + ENV_RESERVE_LENGTH)
//#define ENV_CONTENT_LENGTH        (127*1024)     /* 保留字段，后续还剩127K */
#define ENV_CONTENT_LENGTH      (64*1024-ENV_CONTENT_OFFSET-12) /* 保留字段剩余127K实际用的很少 */

typedef enum _GEV_ITEM_TYPE
{
    GEV_ITEM_BOOTPARAM = 0,
    GEV_ITEM_BMP,
    GEV_ITEM_RESERVE,
    GEV_ITEM_VALUE,
    GEV_ITEM_BMP_AND_VALUE, /* 同时操作除bootrom参数之外的所有内容,用于_gevSetBack */
    GEV_ITEM_TYPE_NUM
}GEV_ITEM_TYPE;

#define IS_VALID_GEV_ITEM(git)  ((git >= 0) && (git < GEV_ITEM_TYPE_NUM))
typedef struct _GEV_MAP
{
    GEV_ITEM_TYPE git;
    int offset;
    int length;
}GEV_MAP;

LOCAL GEV_MAP gevMap[GEV_ITEM_TYPE_NUM] = {
{GEV_ITEM_BOOTPARAM,ENV_BOOTPARAM_OFFSET,ENV_BOOTPARAM_LENGTH},
{GEV_ITEM_BMP,      ENV_BMP_OFFSET,      ENV_BMP_LENGTH},
{GEV_ITEM_RESERVE,  ENV_RESERVE_OFFSET,  ENV_RESERVE_LENGTH},
{GEV_ITEM_VALUE,    ENV_CONTENT_OFFSET,  ENV_CONTENT_LENGTH},
{GEV_ITEM_BMP_AND_VALUE,    ENV_BMP_OFFSET,  ENV_BMP_LENGTH+ENV_RESERVE_LENGTH+ENV_CONTENT_LENGTH},
};

/* 后续全部是环境变量区 */
#define ENV_ITEM_LENGTH         (ENV_ITEM_NAME_LEN+ENV_ITEM_VALUE_LEN)    /* 每一条存放空间为512B */
//#define ENV_ITEM_MAX_NUM        (254)
#define ENV_ITEM_MAX_NUM        (126)
#define ENV_ITEM_NAME_OFFSET(x) (ENV_ITEM_LENGTH * (x))
#define ENV_ITEM_VALUE_OFFSET(x) (ENV_ITEM_LENGTH * (x) + ENV_ITEM_NAME_LEN)
/* 环境变量区定义完成 */

int gev_debug = 1;

#define DBG(arg...) do{if(0){printf("%s,%d:",__FUNCTION__, __LINE__);printf(arg);}}while(0)

typedef struct _gev_s
{
    unsigned char   name[ENV_ITEM_NAME_LEN];
    unsigned char   value[ENV_ITEM_VALUE_LEN];
    int   index;                      /* 用于记录存储与FLASH中第几个环境变量，从0开始 */
}gev_s;

/* 启动时软件会获取环境变量，保存到内存中 by liuzenglin, 18Jun13 */
typedef struct ENV_NODE_S
{
    gev_s  gev;
    struct ENV_NODE_S *next;
} ENV_NODE_T;
LOCAL ENV_NODE_T* pEnvHead = NULL; /* 用于存储环境变量 */
LOCAL int  flashGevInited = FALSE;

/* 设置默认的环境变量 */
/*{
    {TP_SWITCH_START_NO_CONFIG, "0",                            1},
    {TP_SWITCH_START_CONFIG,    TP_SWITCH_CONFIG_NAME_1,     1},
    {TP_SWITCH_BACKUP_CONFIG,   TP_SWITCH_CONFIG_NAME_2,   1},
    {TP_SWITCH_START_IMAGE,     TP_SWITCH_IMAGE_NAME_1,         1},
    {TP_SWITCH_BACKUP_IMAGE,    TP_SWITCH_IMAGE_NAME_2,         1},
    {TP_SWITCH_FL_VER,          "0x10000",                      1},
    {TP_SWITCH_FL_SIZE_BOOT,    "0x100000",                     1},
    {TP_SWITCH_FL_OFFSET_FS,    "0x100000",                     1},
    {TP_SWITCH_FL_SIZE_FS,      "0x1c00000"| "0xc00000",                    1},    T3700存在16Mflash
};*/
/* 返回动态申请内存，需要手动释放 */
char* getDftGevContent(int *otGevSize)
{
    unsigned char* gev = NULL;
    unsigned char* pbm = NULL;
    unsigned char* value = NULL;
    int  gevSize = flashGetSectorSize() -12;
    char strFsDftSize[ENV_ITEM_VALUE_LEN] = {0};

    *otGevSize = gevSize;

    //gev = (unsigned char*)calloc(1, gevSize);
    gev = (unsigned char*)BUF_ADDR_NOFREE;
    if (gev == NULL)
    {
        DBG("Fatal error: Failed to allocate memory.\r\n");
        return NULL;
    }

    memset(gev, 0xff, gevSize);
    pbm = gev+gevMap[GEV_ITEM_BMP].offset;
    value = gev+gevMap[GEV_ITEM_VALUE].offset;
    
    if (ERROR == flashGetDftGevFsSize(strFsDftSize))
    {
        printf("Fatal error: Failed to get default fs size.\r\n");
        //free(gev);
        return NULL;
    }
    DBG("Default fs size is %s\r\n", strFsDftSize);

    pbm[0]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(0)), TP_SWITCH_START_CONFIG, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(0)), TP_SWITCH_CONFIG_NAME_1, ENV_ITEM_VALUE_LEN);
    pbm[1]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(1)), TP_SWITCH_BACKUP_CONFIG, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(1)), TP_SWITCH_CONFIG_NAME_2, ENV_ITEM_VALUE_LEN);
    pbm[2]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(2)), TP_SWITCH_START_IMAGE, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(2)), TP_SWITCH_IMAGE_NAME_1, ENV_ITEM_VALUE_LEN);
    pbm[3]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(3)), TP_SWITCH_BACKUP_IMAGE, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(3)), TP_SWITCH_IMAGE_NAME_2, ENV_ITEM_VALUE_LEN);
    pbm[4]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(4)), TP_SWITCH_FL_VER, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(4)), "0x10000", ENV_ITEM_VALUE_LEN);
    pbm[5]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(5)), TP_SWITCH_FL_SIZE_BOOT, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(5)), "0x100000", ENV_ITEM_VALUE_LEN);
    pbm[6]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(6)), TP_SWITCH_FL_OFFSET_KERNEL, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(6)), "0x100000", ENV_ITEM_VALUE_LEN);
    pbm[7]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(7)), TP_SWITCH_FL_SIZE_KERNEL, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(7)), "0x600000", ENV_ITEM_VALUE_LEN);
    pbm[8]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(8)), TP_SWITCH_FL_OFFSET_USRIMG1, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(8)), "0x700000", ENV_ITEM_VALUE_LEN);
    pbm[9]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(9)), TP_SWITCH_FL_SIZE_USRIMG1, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(9)), "0xa00000", ENV_ITEM_VALUE_LEN);
    pbm[10]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(10)), TP_SWITCH_FL_OFFSET_USRIMG2, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(10)), "0x1100000", ENV_ITEM_VALUE_LEN);
    pbm[11]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(11)), TP_SWITCH_FL_SIZE_USRIMG2, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(11)), "0xa00000", ENV_ITEM_VALUE_LEN);
    pbm[12]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(12)), TP_SWITCH_FL_OFFSET_USRAPP, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(12)), "0x1b00000", ENV_ITEM_VALUE_LEN);
    pbm[13]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(13)), TP_SWITCH_FL_SIZE_USRAPP, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(13)), "0x400000", ENV_ITEM_VALUE_LEN);
    pbm[14]=0x1;
    strncpy((char *)(value+ENV_ITEM_NAME_OFFSET(14)), TP_SWITCH_START_NO_CONFIG, ENV_ITEM_NAME_LEN);
    strncpy((char *)(value+ENV_ITEM_VALUE_OFFSET(14)), "0", ENV_ITEM_VALUE_LEN);


    return (char *)gev;
}

STATUS resetEnv(void)
{
    char *gev = NULL;
    int gevSize = 0;

    gev = getDftGevContent(&gevSize);
    if (gev == NULL)
    {
        printf("Get default content failed.\r\n");
        return ERROR;
    }
    
    if (ERROR == flashAppInnerSet(FLASH_INNER_ITEM_GEV, gev, gevSize, TRUE))
    {
        printf("Fata error: Failed to set gev to default.\r\n");
        //free(gev);
        return ERROR;
    }
    
    /* 设置关于FLASH布局之后的环境变量后需要重新更新FLASH布局 */
    if (ERROR == flashAppInit())
    {
        printf("Fatal error: Failed to init flash app.\r\n");
        //free(gev);
        return ERROR;
    }

    //free(gev);
    DBG("Reset gev success.\r\n");
    return OK;
}

extern void free(void*);

/* 默认buff长度与gev中项的长度一致 by liuzenglin, 22Jun13 */
LOCAL int _getGevItem(GEV_ITEM_TYPE git, unsigned char* buff, int len)
{
    int length = 0;
    unsigned char* gevBuff = flashAppInnerGet(FLASH_INNER_ITEM_GEV, &length);

    if (NULL == gevBuff)
    {
        DBG("Failed to get gev at first read, and we will reset gev.\r\n");
        if (ERROR == resetEnv())
        {
            printf("Failed to reset gev.\r\n");
            return ERROR;
        }

        /* 再读取一次 */
        gevBuff = flashAppInnerGet(FLASH_INNER_ITEM_GEV, &length);
        if (NULL == gevBuff)
        {
            printf("Fatal error: Failed to get gev at second read.\r\n");
            return ERROR;
        }
    }

    if (! IS_VALID_GEV_ITEM(git))
    {
        DBG("Unkown git, git=%d\r\n", git);
        free(gevBuff);
        return ERROR;
    } 

    if(NULL == buff)
    {
        DBG("Null buff, buff=0x%p\r\n", buff);
        free(gevBuff);
        return ERROR;
    }
    memcpy(buff, gevBuff+gevMap[git].offset, len);

    DBG("Get gev item git=%#x,len=%d, gevMap len=%d.\r\n", 
        git, len, gevMap[git].length);

    free(gevBuff);

    return OK;    
}

/* 默认buff长度与gev中项的长度一致 by liuzenglin, 22Jun13 */
LOCAL int _setGevItem(GEV_ITEM_TYPE git, const char* buff, int len)
{
    int length = 0;
    unsigned char* gevBuff = flashAppInnerGet(FLASH_INNER_ITEM_GEV, &length);

    if (NULL == gevBuff)
    {
        printf("Failed to get gev.\r\n");
        return ERROR;
    }

    if (! IS_VALID_GEV_ITEM(git))
    {
        DBG("Unkown git, git=%d\r\n", git);
        free(gevBuff);
        return ERROR;
    }

    if(NULL == buff)
    {
        DBG("Null buff, buff=0x%p\r\n", buff);
        free(gevBuff);
        return ERROR;
    }

    DBG("Set gev item git=%#x,len=%d, gevMap len=%d.\r\n", 
        git, len, gevMap[git].length);

    memcpy(gevBuff+gevMap[git].offset, buff, gevMap[git].length);

    if (ERROR == flashAppInnerSet(FLASH_INNER_ITEM_GEV, (char *)gevBuff, length, FALSE))
    {
        printf("Fatal error: Failed to set gev.\r\n");
        free(gevBuff);
        return ERROR;
    }

    free(gevBuff);

    return OK;

}


/* 当环境变量区没有可用的空白块时重新设置回去 */
LOCAL int _gevSetBack(void)
{
    unsigned char* pbm = NULL;
    unsigned char* content = NULL;
    unsigned char* buff = NULL;
    ENV_NODE_T* pRead = pEnvHead;
    int i = 0;
    int index = 0;

    //buff = (unsigned char*)calloc(1, gevMap[GEV_ITEM_BMP_AND_VALUE].length);
    buff = (unsigned char *)BUF_ADDR;
    if (buff == NULL)
    {
        printf("Fatal error: failed to allocate memory, size=%#x.\r\n", gevMap[GEV_ITEM_BMP_AND_VALUE].length);
        return ERROR;
    }

    memset(buff, 0xff, gevMap[GEV_ITEM_BMP_AND_VALUE].length);

    pbm = (unsigned char*)buff; /* 偏移地址相同 */

    if (ERROR == _getGevItem(GEV_ITEM_BMP, pbm, gevMap[GEV_ITEM_BMP].length))
    {
        //free(buff);
        printf("Fatal error: Failed to get gev bmp.\r\n");
        return ERROR;
    }

    for (i = 0; i < ENV_ITEM_MAX_NUM; i++)
    {
        if (pbm[i] == 0xff)
        {
            DBG("There are some blank gev, i=%d, and do not set back gev.\r\n", i);
            //free(buff);
            return OK;
        }
    }

    DBG("There is not any blank gev, and set back gev.\r\n");

    memset(pbm, 0xff, ENV_BMP_LENGTH);

    content = (unsigned char*)(buff+gevMap[GEV_ITEM_VALUE].offset-gevMap[GEV_ITEM_BMP_AND_VALUE].offset);

    index = 0;
    while (pRead != NULL)
    {
        pbm[index] = 0x1;
        strncpy((char *)(content+ENV_ITEM_NAME_OFFSET(index)),(const char *) pRead->gev.name, ENV_ITEM_NAME_LEN);
        strncpy((char *)(content+ENV_ITEM_VALUE_OFFSET(index)),(const char *)  pRead->gev.value, ENV_ITEM_VALUE_LEN);
        DBG("\t%-2d, %s=%s\r\n", pRead->gev.index, pRead->gev.name, pRead->gev.value);
        pRead = pRead->next;
        index++;
    }
    
    if (ERROR == _setGevItem(GEV_ITEM_BMP_AND_VALUE, (const char *)buff, gevMap[GEV_ITEM_BMP_AND_VALUE].length))
    {
        //free(buff);
        printf("Fatal error: Failed to set gev pbm and value.\r\n");
        return ERROR;
    }
    flashGevInited = FALSE; /* 初始化链表需要重新更新 */

    //free(buff);
    DBG("Gev setback ok, index=%d.\r\n", index);
    return OK; 
}

LOCAL void _freeEnvItems(void)      /* 释放链表 */
{
    ENV_NODE_T* pFree = pEnvHead;
    ENV_NODE_T* pTemp = NULL;
    while (pFree != NULL)
    {
        pTemp = pFree->next;
        free(pFree);
        pFree = pTemp;
    }
    pEnvHead = NULL;
    return;
}

/* 初始化时判断必需要的环境变量是否存在，不存在则写入 */
STATUS _checkEnvIsValid(void)
{
    char envValue[ENV_ITEM_VALUE_LEN] = {0};

    /* 启动和备份镜像名称只能为TP_SWITCH_IMAGE_NAME_1或TP_SWITCH_IMAGE_NAME_2 */
    if (ERROR == getEnvItem(TP_SWITCH_START_IMAGE, envValue) ||
        ((strncmp(envValue, TP_SWITCH_IMAGE_NAME_1, FN_MAX_LENGTH) != 0) && 
        (strncmp(envValue, TP_SWITCH_IMAGE_NAME_2, FN_MAX_LENGTH) != 0)) ||
        ERROR == getEnvItem(TP_SWITCH_BACKUP_IMAGE, envValue) ||
        ((strncmp(envValue, TP_SWITCH_IMAGE_NAME_1, FN_MAX_LENGTH) != 0) && 
        (strncmp(envValue, TP_SWITCH_IMAGE_NAME_2, FN_MAX_LENGTH) != 0)) ||
        (ERROR == getEnvItem(TP_SWITCH_START_CONFIG, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_BACKUP_CONFIG, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_VER, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_SIZE_BOOT, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_OFFSET_KERNEL, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_SIZE_KERNEL, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_OFFSET_USRIMG1, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_SIZE_USRIMG1, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_OFFSET_USRIMG2, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_SIZE_USRIMG2, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_OFFSET_USRAPP, envValue)) ||
        (ERROR == getEnvItem(TP_SWITCH_FL_SIZE_USRAPP, envValue)) )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************
* fn            : _initReadGev()
* author        : LiuZenglin <LiuZenglin@tp-link.net>
* decription    : 读取Flash中环境变量到内存中，构造单链表保存
* param[in]     : N/A
* param[out]    : N/A
*
* return        : N/A
* retval        : N/A
* note          : 如果读取条目失败，则失败条目之后的变量
*****************************************************************************/
STATUS _initReadGev(void)    /* copy items for flash to ram */
{
    UINT32 nEnvNum = 0;
    int i = 0;
    ENV_NODE_T* pCur = NULL;    /* 当前可用链表节点 */
    ENV_NODE_T* pParent = NULL;
    unsigned char* pbm = NULL;
    unsigned char* content = NULL;    

    if (NULL != pEnvHead)   /* 如果已经初始化过，则释放后再次获取 */
    {
        DBG("Reinit the environment list.\r\n");
        _freeEnvItems();
    }

    //pbm = (unsigned char*)calloc(1, ENV_BMP_LENGTH);
    pbm = (unsigned char *)BUF_ADDR;
    if (pbm == NULL)
    {
        flashGevInited = TRUE;
        printf("Fatal error: Failed to allocate memory, size=%#x.\r\n", ENV_BMP_LENGTH);
        return ERROR;
    }

    if (ERROR == _getGevItem(GEV_ITEM_BMP, pbm, ENV_BMP_LENGTH))
    {
        //free(pbm);
        flashGevInited = TRUE;
        printf("Fatal error: Failed to get gev bmp.\r\n");
        return ERROR;
    }

    //content = (unsigned char*)calloc(1, ENV_CONTENT_LENGTH);
    content = (unsigned char *)(BUF_ADDR + ENV_BMP_LENGTH);
    if (content == NULL)
    {
        //free(pbm);
        flashGevInited = TRUE;
        printf("Fatal error: Failed to allocate memory, size=%#x.\r\n", ENV_CONTENT_LENGTH);
        return ERROR;
    }

    if (ERROR == _getGevItem(GEV_ITEM_VALUE, content, ENV_CONTENT_LENGTH))
    {
        //free(pbm);
        //free(content);
        flashGevInited = TRUE;
        printf("Fatal error: Failed to get gev value.\r\n");
        return ERROR;
    }

    /* 读取环境变量 */
    for (i = 0; i < ENV_ITEM_MAX_NUM; i++)
    {
        DBG("ENV %d is %s.\r\n", i, 
            (pbm[i]==0)? "not active":((pbm[i]==1)?"active":((pbm[i]==0xff)?"blank":"unkown")));
        if (pbm[i] != 0x1)
        {
            continue;
        }

        if ((strlen((const char *)(content+ENV_ITEM_NAME_OFFSET(i))) == 0) ||
            (strlen((const char *)(content+ENV_ITEM_VALUE_OFFSET(i))) == 0))
        {
            DBG("Gev index=%d length is 0, and will be ignored.\r\n", i);
            continue;
        }

        pCur = (ENV_NODE_T*)calloc(1, sizeof(ENV_NODE_T));
        if (NULL == pCur)
        {
            DBG("Failed to allocate ENV_NODE_T.\r\n");
            _freeEnvItems();
            flashGevInited = TRUE;
            //free(pbm);
            //free(content);
            return ERROR;
        }
        memset(pCur->gev.name, 0, ENV_ITEM_NAME_LEN);
        memset(pCur->gev.value, 0, ENV_ITEM_VALUE_LEN);
        pCur->gev.index = i;
        pCur->next = NULL;

        strncpy((char*)(pCur->gev.name), (const char *)(content+ENV_ITEM_NAME_OFFSET(i)), ENV_ITEM_NAME_LEN);
        strncpy((char*)(pCur->gev.value),(const char *)(content+ENV_ITEM_VALUE_OFFSET(i)), ENV_ITEM_VALUE_LEN);

        nEnvNum++;
        DBG("%d index=%d %s=%s.\r\n", nEnvNum, i, pCur->gev.name, pCur->gev.value);
        if (nEnvNum == 1)
        {
            pEnvHead = pCur;
            pParent  = pEnvHead;
            pCur = NULL;
        }
        else
        {
            pParent->next = pCur;
            pParent = pCur;
            pCur = NULL;
        }
    }

    flashGevInited = TRUE;
    //free(pbm);
    //free(content);

    return OK;
}

int flashAppInnerGevInit(void)
{
    if (ERROR == _initReadGev())
    {
        printf("Failed to read gev to memory.\r\n");
        return ERROR;
    }
    
    /* 检查环境变量的合法性，默认配置文件都在 */
    if (FALSE == _checkEnvIsValid())
    {
        printf("Gev is not valid, and The gev will be reset.\r\n");
        if (ERROR == resetEnv())
        {
            printf("Failed to reset gev.\r\n");
            return ERROR;
        }

        if (ERROR == _initReadGev())
        {
            printf("Failed to read gev to memory.\r\n");
            return ERROR;
        }
    }

    return OK;
}

/*****************************************************************************
* fn            : setEnvItem()
* author        : LiuZenglin <LiuZenglin@tp-link.net>
* decription    : 添加或修改条目，同时更新Flash和内存中内容
* param[in]     : N/A
* param[out]    : N/A
*
* return        : N/A
* retval        : N/A
* note          : Flash中int全部以大端存储
*****************************************************************************/
STATUS setEnvItem(const char* name, const char* value)
{
    ENV_NODE_T* pModify = pEnvHead;
    ENV_NODE_T* pEnd = NULL;   /* 用于记录最后一个节点 */
    UINT8 buffer[ENV_ITEM_LENGTH] = {0};   /* Flash中条目大小，用于写Flash条目 */
    UINT32 nNameLen = 0;
    UINT32 nValueLen = 0;
    int    index  =-1;         /* 表明当前要修改的环境变量所在的序号, 如果新建则保持为-1 */
    UINT32 counter = 0;       /* 用于表示当前Flash中的环境变量的数目 */
    UINT8 newNode = 0;     /* 用于标记是否新添加节点 */
    int i =0;
    unsigned char* pbm = NULL;
    int valueOffset = 0;

    if (flashGevInited == FALSE)
    {
        if (ERROR == flashAppInnerGevInit())
        {
            DBG("Fatal error: Failed to init the gev.\r\n");
            return ERROR;
        }
    }

    memset(buffer, 0x0, ENV_ITEM_LENGTH);
    nNameLen = strlen(name);
    nValueLen = strlen(value);

    if ((NULL == name) || (NULL == value)
        || (nNameLen >= ENV_ITEM_NAME_LEN)
        || (nValueLen >= ENV_ITEM_VALUE_LEN)
        || (nNameLen <= 0)
        || (nValueLen <= 0))
    {
        DBG("%s, invalid param.\r\n", __FUNCTION__);
        return ERROR;
    }

    strncpy((char *)buffer, name, ENV_ITEM_NAME_LEN);
    strncpy((char *)(buffer + ENV_ITEM_NAME_LEN), value, ENV_ITEM_VALUE_LEN);

    pModify = pEnvHead;
    /* 查找节点 */
    while (NULL != pModify)
    {
        if (0 == strncmp((char *)pModify->gev.name, name, ENV_ITEM_NAME_LEN))   /* 修改 */
        {
            DBG("Modify gev item, name=%s, old_value=%s, new_value=%s, index=%d\r\n", 
                name, pModify->gev.value, value, pModify->gev.index);
            index = pModify->gev.index;
            if (0 == strncmp((char *)pModify->gev.value, value, ENV_ITEM_VALUE_LEN))    /* 该节点不用修改 */
            {
                return OK;
            }
            break;
        }
        else
        {
            pEnd = pModify;
            pModify = pModify->next;
            counter++;
        }
    }

    /* 如果找不到，并且条目数未满，则添加节点 */
    if (NULL == pModify)   /* 新建变量 */
    {
        DBG("Write a new gev item, name=%s, new_value=%s\r\n", 
            name, value);
        /* 如果已经满了，则提示出错 */
        if  (counter >= ENV_ITEM_MAX_NUM)
        {
            printf("Warning: Failed to set %s=%s, %d items at most.\r\n",
                   name, value, ENV_ITEM_MAX_NUM);
            return OK;
        }

        pModify = (ENV_NODE_T*)calloc(1, sizeof(ENV_NODE_T));
        if (NULL == pModify)
        {
            DBG("Failed to allocate ENV_NODE_T.\r\n");
            return ERROR;
        }
        pModify->next = NULL;
        memset(pModify->gev.name, 0, ENV_ITEM_NAME_LEN);
        strncpy((char *)pModify->gev.name, name, ENV_ITEM_NAME_LEN);

        if (0 == counter) /* 为空链表 */
        {
            /* 初始化链表 */
            pEnvHead = pModify;
        }
        else
        {
            /* 添加节点到链表中 */
            pEnd->next = pModify;
        }
        newNode = 1;
    }
    memset(pModify->gev.value, 0, ENV_ITEM_VALUE_LEN);
    strncpy((char *)pModify->gev.value, value, ENV_ITEM_VALUE_LEN);

    /* write flash */
    DBG("gev counter = %d, index = %d\r\n", counter, index);
    //pbm = (unsigned char*)calloc(1, ENV_BMP_LENGTH);
    pbm = (unsigned char*)BUF_ADDR;
    if (pbm == NULL)
    {
        printf("Fatal error: Failed to allocate memory, size=%#x.\r\n", ENV_BMP_LENGTH);
        return ERROR;
    }

    if (ERROR == _getGevItem(GEV_ITEM_BMP, pbm, ENV_BMP_LENGTH))
    {
        //free(pbm);
        printf("Fatal error: Failed to get gev bmp.\r\n");
        return ERROR;
    }

    for (i = 0; i < ENV_ITEM_MAX_NUM; i++)
    {
        /* 找到没有使用的环境变量 */
        if (pbm[i] == 0xff)
        {
            DBG("Find a blank gev space, index=%d\r\n", i);
            pbm[i] = 0x1;
            /* 如果是修改，则需要将原环境变量置为invalid状态 */
            if (newNode != 1)
            {
                pbm[index] = 0;
            }
            pModify->gev.index = i;

            if (ERROR == flashAppInnerGetOffset(FLASH_INNER_ITEM_GEV, &valueOffset))
            {
                //free(pbm);
                printf("Fatal error: Failed to set gev content offset.\r\n");
                return ERROR; 
            }

            /* 先写内容，再写BMP，避免因为断电而出现非字符串的环境变量 */
            if (ERROR == writeFlash((char *)buffer, ENV_ITEM_LENGTH, 
                valueOffset+gevMap[GEV_ITEM_VALUE].offset+ENV_ITEM_NAME_OFFSET(i), FALSE))
            {
                //free(pbm);
                printf("Fatal error: Failed to set gev content.\r\n");
                return ERROR;                
            }
            if (ERROR == writeFlash((char *)pbm, ENV_BMP_LENGTH, valueOffset+gevMap[GEV_ITEM_BMP].offset, FALSE))
            {
                //free(pbm);
                printf("Fatal error: Failed to set gev pbm.\r\n");
                return ERROR;
            }

            /* 检查是否还有多余的空白块 */
            if (ERROR == _gevSetBack())
            {
                //free(pbm);
                printf("Fatal error: Failed to setback gev.\r\n");
                return ERROR;
            }

            /* 设置关于FLASH布局之后的环境变量后需要重新更新FLASH布局 */
            if (ERROR == flashAppInit())
            {
                printf("Fatal error: Failed to init flash app.\r\n");
                //free(pbm);
                return ERROR;
            }

            //free(pbm);
            DBG("Set env ok,index=%d %s=%s\r\n", i, name, value);
            return OK;
        }
    }

    //free(pbm);
    /* 由于每次设置之后都会检查是否有BLANK的gev，故不可能找不到BLANK gev */
    printf("Fatal error: Can't find a blank gev.\r\n");
    return ERROR;
}


/*****************************************************************************
* fn            : copyEnvItem()
* author        : LiuZenglin <LiuZenglin@tp-link.net>
* decription    : set gev named dstName with the same value of gev srcName 
* param[in]     : N/A
* param[out]    : N/A
*
* return        : N/A
* retval        : N/A
* note          : N/A
*****************************************************************************/
STATUS copyEnvItem(const char* dstName, const char* srcName)
{
    char gevValue[256] = {0};
    
    if ((NULL == dstName) ||
        (NULL == srcName))
    {
        DBG("Fatal error: dstName is 0x%p, srcName is 0x%p\r\n", dstName, srcName);
        return ERROR;
    }

    if (flashGevInited == FALSE)
    {
        if (ERROR == flashAppInnerGevInit())
        {
            DBG("Fatal error: Failed to init the gev.\r\n");
            return ERROR;
        }
    }

    if (ERROR == getEnvItem(srcName, gevValue))
    {
        DBG("Failed to get gev %s when copy gev.\r\n", srcName);
        return ERROR;
    }

    if (ERROR == setEnvItem(dstName, gevValue))
    {
        DBG("Failed to set gev %s when copy gev.\r\n", dstName);
        return ERROR;
    }

    DBG("Copy env ok,dst gev=%s src gev=%s\r\n", dstName, srcName);
    return OK;
}

/*****************************************************************************
* fn            : getEnvItem()
* author        : LiuZenglin <LiuZenglin@tp-link.net>
* decription    : 获取环境变量，只查找内存中链表
* param[in]     : name环境变量的值
* param[out]    : value, 获取失败value全部为0
*
* return        : N/A
* retval        : N/A
* note          : 该函数运行时，假设内存环境变量与Flash中环境变量一致
*****************************************************************************/
STATUS getEnvItem(const char* name, char* value)
{

    ENV_NODE_T* pRead = pEnvHead;

    if (flashGevInited == FALSE)
    {
        if (ERROR == flashAppInnerGevInit())
        {
            DBG("Fatal error: Failed to init the gev.\r\n");
            return ERROR;
        }
    }

    /* 清空返回buffer */
    memset(value, 0, ENV_ITEM_VALUE_LEN);

    pRead = pEnvHead;

    /* 遍历链表 */
    while (pRead != NULL)
    {
        if (0 == strncmp((char *)pRead->gev.name, name, ENV_ITEM_NAME_LEN))   /* 修改 */
        {
            DBG("Find a gev, index=%d.\r\n", pRead->gev.index);
            strncpy(value, (const char *)pRead->gev.value, ENV_ITEM_VALUE_LEN);
            break;
        }
        else
        {
            pRead = pRead->next;
        }
    }


    if (pRead == NULL)
    {
        DBG("Warning: Environment variable %s doesn't exist.\r\n", name);
        return ERROR;
    }
    else
    {
        DBG("%s=%s\r\n", name, value);
        return OK;
    }
}

/*****************************************************************************
* fn            : delEnvItem()
* author        : LiuZenglin <LiuZenglin@tp-link.net>
* decription    : 删除环境变量，包括Flash和链表
* param[in]     : name,环境变量名称
* param[out]    : N/A
*
* return        : N/A
* retval        : N/A
* note          : N/A
*****************************************************************************/
STATUS delEnvItem(const char* name)
{
    ENV_NODE_T* pTemp = pEnvHead;
    ENV_NODE_T* pDel = NULL;
    UINT8 buffer[ENV_ITEM_LENGTH] = {0};
  //  UINT32 nNameLen = 0;
    UINT32 index = 0;
    unsigned char* pbm = NULL;
    int valueOffset = 0;

    if (flashGevInited == FALSE)
    {
        if (ERROR == flashAppInnerGevInit())
        {
            DBG("Fatal error: Failed to init the gev.\r\n");
            return ERROR;
        }
    }
    pTemp = pEnvHead;

    memset(buffer, 0xff, ENV_ITEM_LENGTH);
   // nNameLen = strlen(name);

    if (NULL == name)
    {
        DBG("Null ptr param.\r\n");
        return ERROR;
    }

    /* 空表 */
    if (pEnvHead == NULL)
    {
        printf("Warning: Environment variable %s doesn't exist.\r\n", name);
        return OK;
    }

    /* 删除头节点 */
    if (0 == strncmp((char *)pEnvHead->gev.name, name, ENV_ITEM_NAME_LEN))
    {
        pTemp = pEnvHead->next;
        free(pEnvHead);
        pEnvHead = pTemp;
        index = 0;
    }
    else
    {
        pTemp = pEnvHead;
        pDel = pTemp->next;

        while (NULL != pDel)
        {
            if (0 == strncmp((char *)pDel->gev.name, name, ENV_ITEM_NAME_LEN))   /* 找到 */
            {
                break;
            }
            else
            {
                pTemp = pDel;
                pDel = pDel->next;
                index++;
            }
        }

        if (NULL == pDel)       /* 不存在该变量 */
        {
            printf("Warning: Environment variable %s doesn't exist.\r\n", name);
            return OK;
        }

        /* 删除节点 */
        pTemp->next = pDel->next;
        index = pDel->gev.index;
        free(pDel);
    }

    /* write flash */
    //pbm = (unsigned char*)calloc(1, ENV_BMP_LENGTH);
    pbm = (unsigned char *)BUF_ADDR;
    if (pbm == NULL)
    {
        printf("Fatal error: Failed to allocate memory, size=%#x.\r\n", ENV_BMP_LENGTH);
        return ERROR;
    }

    if (ERROR == _getGevItem(GEV_ITEM_BMP, pbm, ENV_BMP_LENGTH))
    {
        //free(pbm);
        printf("Fatal error: Failed to get gev bmp.\r\n");
        return ERROR;
    }
    /* 把相应条目设置为INVALID状态 */
    pbm[index] = 0;
    DBG("Set gev invalid, index=%d\r\n", index);
    if (ERROR == flashAppInnerGetOffset(FLASH_INNER_ITEM_GEV, &valueOffset))
    {
        //free(pbm);
        printf("Fatal error: Failed to set gev content offset.\r\n");
        return ERROR; 
    }
            
    //if (ERROR == _setGevItem(GEV_ITEM_BMP, pbm, ENV_BMP_LENGTH))
    if (ERROR == writeFlash((char *)pbm, ENV_BMP_LENGTH, valueOffset+gevMap[GEV_ITEM_BMP].offset, FALSE))
    {
        //free(pbm);
        printf("Fatal error: Failed to set gev pbm.\r\n");
        return ERROR;
    }

    //free(pbm);

    return OK;
}

/* 用于重设环境变量区，删除非必须的环境变量，被函数flashAppInnerReset调用 */
STATUS updateGevItem(const char*flBootSize, const char*flFsOffset, const char*flFsSize, const char*flVer)
{
    ENV_NODE_T* pDel = pEnvHead;

    /* 删除非必须的节点 */
    pDel = pEnvHead;
    while (pDel != NULL)
    {
        DBG("Try to clear gev, %s=%s\r\n", pDel->gev.name, pDel->gev.value);
        /*  共九项 */
        if ((0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_START_IMAGE, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_BACKUP_IMAGE, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_START_NO_CONFIG, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_START_CONFIG, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_BACKUP_CONFIG, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_OFFSET_KERNEL, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_SIZE_BOOT, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_SIZE_KERNEL, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_OFFSET_USRIMG1, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_SIZE_USRIMG1, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_OFFSET_USRIMG2, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_SIZE_USRIMG2, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_OFFSET_USRAPP, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_SIZE_USRAPP, ENV_ITEM_NAME_LEN)) &&
            (0 != strncmp((const char *)pDel->gev.name, TP_SWITCH_FL_VER, ENV_ITEM_NAME_LEN)))
        {
            DBG("Remove gev, %s=%s\r\n", pDel->gev.name, pDel->gev.value);
            pDel = pDel->next;      /* 在删除之前记录下一个节点的位置 */
            if (ERROR == delEnvItem((const char *)pDel->gev.name))
            {
                printf("Fatal error: Failed to remove gev, %s=%s, index=%d\r\n",
                    pDel->gev.name, pDel->gev.value, pDel->gev.index);
                return ERROR;
            }
        }
        else
        {
            pDel = pDel->next;
        }
    }
    
    /* 重设必须的环境变量 
    if ((ERROR == setEnvItem(TP_SWITCH_FL_VER, flVer)) ||
        (ERROR == setEnvItem(TP_SWITCH_FL_SIZE_BOOT, flBootSize)) ||
        (ERROR == setEnvItem(TP_SWITCH_FL_SIZE_KERNEL, flFsSize)) ||
        (ERROR == setEnvItem(TP_SWITCH_FL_OFFSET_KERNEL, flFsOffset)))
    {
        printf("Fatal error: Failed to update gev.\r\n");
        return ERROR;
    }*/

    return OK;
}

/*****************************************************************************
* fn            : printEnvItems()
* author        : LiuZenglin <LiuZenglin@tp-link.net>
* decription    : 输出所有环境变量
* param[in]     : N/A
* param[out]    : N/A
*
* return        : N/A
* retval        : N/A
* note          : N/A
*****************************************************************************/
void printEnvItems(void)
{
    ENV_NODE_T* pRead = pEnvHead;
    UINT32 i = 0;

    if (flashGevInited == FALSE)
    {
        if (ERROR == flashAppInnerGevInit())
        {
            DBG("Fatal error: Failed to init the gev.\r\n");
            return;
        }
    }
    pRead = pEnvHead;

    printf("Global environment variables:\r\n");
    while (pRead != NULL)
    {
        i++;
        printf("\t%-2d, %s=%s\r\n", i, pRead->gev.name, pRead->gev.value);
        pRead = pRead->next;
    }
    printf("\r\n");
    return;
}

/*!
 *\fn                   STATUS sysGetBootFromFlash(char *s_pro)
 *\brief                get the mac from flash
 *
 *\param[in]    N/A
 *\param[out]   s_pro   store the mac address
 *
 *\return               error code
 *\retval               OK              success
 *\retval               ERROR   fail
 *
 *\note
 */
STATUS sysGetBootFromFlash(BOOT_PARAM *bootParam)
{

    int valueOffset = 0;

    if(NULL == bootParam)
    {
        return ERROR;
    }

    if (ERROR == flashAppInnerGetOffset(FLASH_INNER_ITEM_GEV, &valueOffset))
    {
        printf("Fatal error: Failed to set gev content offset.\r\n");
        return ERROR; 
    }

    /* if (ERROR == _getGevItem(GEV_ITEM_BOOTPARAM, (unsigned char*)(bootParam->padding), gevMap[GEV_ITEM_BOOTPARAM].length)) */
    if (ERROR == readFlash((char*)(bootParam->padding), 
        gevMap[GEV_ITEM_BOOTPARAM].length, valueOffset+gevMap[GEV_ITEM_BOOTPARAM].offset))
    {
        printf("Failed to get boot param.\r\n");
        return ERROR;
    }

    return OK;
}

/*!
 *\fn           STATUS sysSetBootToFlash(char *s_pro,int len)
 *\brief        set the bootutil config to flash
 *
 *\param[in]
 *\param[out]
 *
 *\return
 *\retval
 *
 *\note
 */
STATUS sysSetBootToFlash(BOOT_PARAM *bootParam)
{
    int valueOffset = 0;

    if(NULL == bootParam)
    {
        return ERROR;
    }

    if (ERROR == flashAppInnerGetOffset(FLASH_INNER_ITEM_GEV, &valueOffset))
    {
        printf("Fatal error: Failed to set gev content offset.\r\n");
        return ERROR; 
    }

    /* if (ERROR == _setGevItem(GEV_ITEM_BOOTPARAM, (char*)(bootParam->padding), gevMap[GEV_ITEM_BOOTPARAM].length)) */
    if (ERROR == writeFlash((char*)(bootParam->padding), 
        gevMap[GEV_ITEM_BOOTPARAM].length, valueOffset+gevMap[GEV_ITEM_BOOTPARAM].offset, FALSE))
    {
        printf("Failed to write boot config to flash\r\n");
        return ERROR;
    } 

    return OK;
}

#if 0
UINT32 inet_addr(register char *inetString);
STATUS sysGetBootLineFromFlash(char *bootLine, int len)
{
    int rc = 0;
    BOOT_PARAM *bootParam = NULL;
    UINT32 mask = 0;

    /* BOOTLINE默认长度为240字节 */
    if ((NULL == bootLine) && (len < 240))
    {
        DBG("Error: Bad parameter, len = %d.\r\n", len);
        return ERROR;
    }

    bootParam = (BOOT_PARAM*)calloc(1, sizeof(BOOT_PARAM));
    if( NULL == bootParam)
    {
        printf("Error: Failed to allocate memory.\r\n");
        return ERROR;
    }

    rc = sysGetBootFromFlash(bootParam);
    if (rc == ERROR)
    {
        free(bootParam);
        printf("Error: Failed to read boot parameters.\r\n");
        return ERROR;
    }

    memset(bootLine, 0, len);
    mask = inet_addr(bootParam->content.ipMask);
    sprintf(bootLine,
            "sc(0,0)host:%s h=%s e=%s:%x g=%s u=%s pw=%s",
            bootParam->content.fileName,
            bootParam->content.hostAddr,
            bootParam->content.ipAddr,
            mask,
            bootParam->content.routeAddr,
            bootParam->content.userName,
            bootParam->content.passWd
           );

    free(bootParam);
    return rc;
}


void testSetEnv(int num)
{
    int i =0;
    char name[20] = {0};
    char value[20] = {0};
    if (num <= 0)
    {
        return;
    }

    for (i = 0; i < num; i++)
    {
        sprintf(name, "n%d", i);
        sprintf(value, "v%d", i);
        if (ERROR == setEnvItem(name, value))
        {
            printf("Failed to set %s=%s.\r\n", name, value);
        }
        else
        {
            printf("Set %s=%s ok.\r\n", name, value);
        }
        memset(name, 0, 20);
        memset(value, 0, 20);
    }
}

void testDelEnv(int num)
{
    int i =0;
    char name[20] = {0};
    char value[20] = {0};
    if (num <= 0)
    {
        return;
    }

    for (i = 0; i < num; i++)
    {
        sprintf(name, "n%d", i);
        sprintf(value, "v%d", i);
        if (ERROR == delEnvItem(name))
        {
            printf("Failed to delete %s=%s.\r\n", name, value);
        }
        memset(name, 0, 20);
        memset(value, 0, 20);
    }
}

#endif

