/*! Copyright(c) 2008-2010 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 *\file		flashUtil.c
 *\brief		
 *\details	
 *
 *\author	Cai Peifeng
 *\version	
 *\date		07Apr10
 *
 *\warning	
 *
 *\history \arg	07Apr10, Cai Peifeng, Create the file
 */

/**************************************************************************************************/
/*                                      CONFIGURATIONS                                            */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      INCLUDE_FILES                                             */
/**************************************************************************************************/

#include<tplink/flashUtil.h>
#include<tplink/flashDrvLib.h>
#include<tplink/md5_tp.h>
#include<tplink/flashAppFsImage.h>
#include<common.h>

/**************************************************************************************************/
/*                                      DEFINES                                                   */
/**************************************************************************************************/
#define MESSAGE(fmt,...)

/**************************************************************************************************/
/*                                      TYPES                                                     */
/**************************************************************************************************/
typedef struct IMG_MTD_PARTS
{
    int nUbootSize;
    int nKernel1Offset;
    int nKernel1Size;
    int nKernel2Offset;
    int nKernel2Size;
    int nUsrImg1Offset;
    int nUsrImg1Size;
    int nUsrImg2Offset;
    int nUsrImg2Size;
    int nUsrAppOffset;
    int nUsrAppSize;
    int nSysParaOffset;
    int nSysParaSize;
}IMG_MTD_PARTS;
/**************************************************************************************************/
/*                                      EXTERN_PROTOTYPES                                         */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      LOCAL_PROTOTYPES                                          */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      VARIABLES                                                 */
/**************************************************************************************************/

unsigned char md5key[16] = 
{
	0x1c,0x29,0x1a,0xae,0x2c,0x75,0xc2,0xfd,
	0x5c,0x10,0xe6,0x82,0x7f,0x61,0x7c,0x6b
};
/**************************************************************************************************/
/*                                      LOCAL_FUNCTIONS                                           */
/**************************************************************************************************/
#if 0
/*!
 *\fn			unsigned char *sysGetMd5Key()
 *\brief   		get the md5 key		
 *
 *\param[in]   	N/A
 *\param[out]  	N/A
 *
 *\return  		the md5 key
 *\retval  
 *
 *\note		  
 */
 static unsigned char *sysGetMd5Key(void)
 {
	 return md5key;
 }
#endif
/**************************************************************************************************/
/*                                      PUBLIC_FUNCTIONS                                          */
/**************************************************************************************************/

/*!
 *\fn			STATUS lockFlash()
 *\brief   		lock the flash
 *
 *\param[in]   	N/A
 *\param[out]  	N/A
 *
 *\return  		the error code 
 *\retval  		OK		success
 *\retval		ERROR	fail
 *
 *\note		  
 */
 STATUS lockFlash(void)
 {
 
	 flashProtectOn();
	
	 return OK; 
 }
	 

/*!
 *\fn			STATUS unlockFlash()
 *\brief   		unlock the flash
 *
 *\param[in]   	N/A
 *\param[out]  	N/A
 *
 *\return  		error code
 *\retval  		OK		success
 *\retval		ERROR	fail
 *
 *\note		  
 */
STATUS unlockFlash()
{
	flashProtectOff();
	return OK;
}
extern int flashBlkWrite(int firstSector, char *buff, int offset, unsigned int count);
extern int flashBlkRead(int firstSector, char *buff, int offset,unsigned int count);

UINT32 l_writeFlash(char *source,unsigned int size, unsigned int flashOffset,int prompt)
{
	int rv;
	int start; 		 /*开始扇区*/
	int offset;		 /*扇区偏移量*/
	int count; 		 /*扇区写的大小*/
	int buf_len;	 /*缓冲区长度*/

	buf_len = size;

	if(source == NULL)
	{
		return ERROR;
	}

	flashProbe();
	
	start = flashOffset / FLASH_SECTOR_SIZE;
	offset = flashOffset % FLASH_SECTOR_SIZE;

	if( FLASH_SECTOR_SIZE - offset < size)
	{
		count = FLASH_SECTOR_SIZE - offset;
	}
	else
	{
		count = size;
	}

	flashErase(start, (size + FLASH_SECTOR_SIZE) /  FLASH_SECTOR_SIZE);

	while( buf_len > 0)
	{
		rv = flashBlkWrite( start, source, offset, count);
	
		if (OK == rv)
	 	{
			if(true == prompt)
		 		printf("######################");
	 	}
	 	else
	 	{
		 	return rv;
	 	}
	 	source += count;
	 	buf_len -= count;

	 	if( buf_len >= FLASH_SECTOR_SIZE )
		 	count = FLASH_SECTOR_SIZE;
	 	else
		 	count = buf_len;
	 	start++;
	 	offset = 0;
	}

	return OK;
}


UINT32 l_readFlash(char *source, unsigned int size, unsigned int flashOffset)
{
	int rv;
	int start; 		 /*开始扇区*/
	int offset;		 /*扇区偏移量*/
	int count; 		 /*扇区写的大小*/
	int buf_len;	 /*缓冲区长度*/

	buf_len = size;
	
	if(source == NULL)
	{
		return ERROR;
	}

	flashProbe();

	start = flashOffset / FLASH_SECTOR_SIZE;
	offset = flashOffset % FLASH_SECTOR_SIZE;


	if( FLASH_SECTOR_SIZE - offset < size)
	{
		count = FLASH_SECTOR_SIZE - offset;
	}
	else
	{
		count = size;
	}

    rv = flashBlkRead( start, source, offset, size);
    
    if (OK != rv)
    {
        return ERROR;
    }

	while( buf_len > 0)
	{
		rv = flashBlkRead( start,source,offset,count);

		if (OK != rv)
	 	{
			return ERROR;
		}
	 	source += count;
	 	buf_len -= count;

	 	if( buf_len >= FLASH_SECTOR_SIZE )
		 	count = FLASH_SECTOR_SIZE;
	 	else
		 	count = buf_len;
	 	start++;
	 	offset = 0;
	}

	return OK;
}


UINT32 l_writeFlash_noErase(char *source,unsigned int size, unsigned int flashOffset,int prompt)
{
	int rv;
	int start; 		 /*开始扇区*/
	int offset;		 /*扇区偏移量*/
	int count; 		 /*扇区写的大小*/
	int buf_len;	 /*缓冲区长度*/

	buf_len = size;

	if(source == NULL)
	{
		return ERROR;
	}

	flashProbe();
	
	start = flashOffset / FLASH_SECTOR_SIZE;
	offset = flashOffset % FLASH_SECTOR_SIZE;

	if( FLASH_SECTOR_SIZE - offset < size)
	{
		count = FLASH_SECTOR_SIZE - offset;
	}
	else
	{
		count = size;
	}

	//flashErase(start, (size + FLASH_SECTOR_SIZE) /  FLASH_SECTOR_SIZE);

	while( buf_len > 0)
	{
		rv = flashBlkWrite( start, source, offset, count);
	
		if (OK == rv)
	 	{
			if(true == prompt)
		 		printf("######################");
	 	}
	 	else
	 	{
		 	return rv;
	 	}
	 	source += count;
	 	buf_len -= count;

	 	if( buf_len >= FLASH_SECTOR_SIZE )
		 	count = FLASH_SECTOR_SIZE;
	 	else
		 	count = buf_len;
	 	start++;
	 	offset = 0;
	}

	return OK;
}

extern BOOL resaveStartupIndex(void);
extern int getStartUpIndex(void);
extern int sysGetFlAddr(char * key);
extern int sysGetMtdpartsInfo(char * key);

int sysFixKernel(char * pImageBuff, int len)
{
    int nRet = ERROR;
    
    if(NULL == pImageBuff)
    {
        return ERROR;
    }

    int KernelOffset = sysGetFlAddr("sys");

     if(len > 0)
     {
            /*write kernel uImage.img*/
            MESSAGE("[update]begin update kernel[flashoffset:%x imgsize:%x]....\n", KernelOffset, len);
            flashErase(KernelOffset/FLASH_SECTOR_SIZE, len/FLASH_SECTOR_SIZE);
            nRet = l_writeFlash_noErase(pImageBuff, len, KernelOffset, true);
            if(ERROR == nRet)
            {
                MESSAGE("[update]update kernel fail, please check!\n");
                return FALSE;
            }
            MESSAGE("[update]update kernel OK....\n");
     }

     return OK;
}

int sysLoadSoftWare(char * pImageBuff, int len)
{
    int nRet = ERROR;
	
	int flashSectionCount = 0;
    
    if(NULL == pImageBuff || len <= 0)
    {
        return ERROR;
    }
	
	printf("Start erasing flash\r\n");
	
	flashSectionCount = flashGetSectorCount();
	
	if (flashSectionCount <= 0)
	{
		printf("Unexpect Error with Flash Driver\r\n");
		return ERROR;
	}
	
	if (OK != flashErase(0, flashSectionCount - 1))
	{
		printf("Error ocurr when erasing flash\r\n");
		return ERROR;
	}
	printf("Erase flash OK\r\n");

	printf("Start writing img to flash\r\n");
	
	nRet = l_writeFlash_noErase(pImageBuff, len, 0, true);
	if(ERROR == nRet)
	{
		printf("Error ocurr when writing img to flash\r\n");
		return FALSE;
	}
	printf("write img OK\r\n");

    return OK;
}

/*!
 *\fn			int sysGetMtdpartsInfo(char * key)
 *\brief   		Get mtd parts info
 *
 *\param[in]   	N/A
 *\param[out]  	N/A
 *
 *\return  		error code
 *\retval  		OK		success
 *\retval		ERROR	fail
 *
 *\note		  
 */
int sysGetMtdpartsInfo(char * key)
{
    /*系统参数没有调通前的临时代码*/
    char * pBootArgs = NULL;
    char * pCur = NULL;
    char * pPre = NULL;
    int nUnit = 0;
    int nLen = 0;
    char caValue[16];

    memset(caValue, 0, sizeof(caValue));
    
    if(NULL == key)
    {
        return ERROR;
    }
    /*bootargs "mtdparts=spi1.0:896K(boot),128K(env),7M(sys1),7M(sys2),6M(usrimge1),6M(usrimg2),2M(usrappfs),3M(para)"*/
    pBootArgs = getenv("bootargs");
    if(NULL == pBootArgs)
    {
        return ERROR;
    }

    pCur = strstr(pBootArgs, key);
    if(NULL == pCur)
    {
        return ERROR;
    }
    
    pCur -= 2;/*寻址到单位*/
    if(*pCur == 'M')
    {
        nUnit = 0x100000;
    }
    else if(*pCur == 'K')
    {
        nUnit = 0x400;
    }
    
    pPre = pCur;
    
    while(pPre && (*pPre != ',' && *pPre != ':'))
    {
        pPre--;
        if(strstr(pPre, "mtdparts") != NULL)
        {
            break;/*防止无限寻址*/
        }
    }

    if(*pPre != ',' && *pPre != ':')
    {
        return ERROR;
    }

    pPre = pPre+1;
    nLen = pCur - pPre;

    if(nLen <=0)
    {
        return ERROR;
    }

    memcpy(caValue, pPre, nLen);



    return simple_strtoul(caValue, NULL, 10) * nUnit;
       
}

int sysGetFlAddr(char * key)
{
    /*系统参数没有调通前的临时代码*/
    if(NULL == key)
    {
        return ERROR;
    }

    int kerneloffset = 0;
    int usrimg1offset = 0;
    int usrimg2offset = 0;
    int usrappoffset = 0;
    int paraoffset = 0;

    kerneloffset = sysGetMtdpartsInfo("boot")+sysGetMtdpartsInfo("env");
    usrimg1offset = kerneloffset + sysGetMtdpartsInfo("sys");
    usrimg2offset = usrimg1offset + sysGetMtdpartsInfo("usrimg1");
    usrappoffset =  usrimg2offset + sysGetMtdpartsInfo("usrimg2");
    paraoffset = usrappoffset + sysGetMtdpartsInfo("usrapp");

    if(strcmp(key, "sys") == 0)
    {
        return  kerneloffset;
    }
    else if(strcmp(key, "usrimg1") == 0)
    {
        return usrimg1offset;
    }
    else if(strcmp(key, "usrimg2") == 0)
    {
        return usrimg2offset;
    }
    else if(strcmp(key, "usrapp") == 0)
    {
        return usrappoffset;
    }
    else if(strcmp(key, "para") == 0)
    {
        return paraoffset;
    }
    else
    {
        MESSAGE("error input\n");
    }


    return ERROR;
}

int sysSetActiveInfo(int nRegion)
{  
    char env[256];
    memset(env, 0, sizeof(env));

    CHECKREGION(nRegion);

    int flKernelSize = sysGetMtdpartsInfo("sys");
    int flUsrImg1Size = sysGetMtdpartsInfo("usrimg1");
    int flUsrImg2Size = sysGetMtdpartsInfo("usrimg2");
    int flUsrAppSize = sysGetMtdpartsInfo("usrappfs");
    int flBootSize = sysGetMtdpartsInfo("boot");
    int flEnvSIze = sysGetMtdpartsInfo("env");
    int flParaSize = FLASH_SIZE - flKernelSize -flUsrImg1Size -flUsrImg2Size- flUsrAppSize - flBootSize -flEnvSIze;

    if(1 == nRegion)
    {//需与uboot变量配置一致
        sprintf(env, "console=ttyS0,38400n8 mem=240M  mtdparts=spi_flash:896K(boot),128K(env),%dK(sys),%dK(usrimg1@main),%dK(usrimg2),%dK(usrappfs),%dK(para)", 
                            flKernelSize/0x400,
                            flUsrImg1Size/0x400, 
                            flUsrImg2Size/0x400, 
                            flUsrAppSize/0x400,
                            flParaSize/0x400);
    }
    else
    {//需与uboot变量配置一致
        sprintf(env, "console=ttyS0,38400n8 mem=240M  mtdparts=spi_flash:896K(boot),128K(env),%dK(sys),%dK(usrimg1),%dK(usrimg2@main),%dK(usrappfs),%dK(para)", 
                            flKernelSize/0x400,
                            flUsrImg1Size/0x400, 
                            flUsrImg2Size/0x400, 
                            flUsrAppSize/0x400,
                            flParaSize/0x400);
    }

    setenv("bootargs",env);
    saveenv();

    return OK;
}

int sysSetActiveInfoWithOutFlash(int nRegion, int baudrate)
{  
    char env[256];
    memset(env, 0, sizeof(env));
    CHECKREGION(nRegion);
    int flKernelSize = sysGetMtdpartsInfo("sys");
    int flUsrImg1Size = sysGetMtdpartsInfo("usrimg1");
    int flUsrImg2Size = sysGetMtdpartsInfo("usrimg2");
    int flUsrAppSize = sysGetMtdpartsInfo("usrappfs");
    int flBootSize = sysGetMtdpartsInfo("boot");
    int flEnvSIze = sysGetMtdpartsInfo("env");
    int flParaSize = FLASH_SIZE - flKernelSize -flUsrImg1Size -flUsrImg2Size- flUsrAppSize - flBootSize -flEnvSIze;
    if(1 == nRegion)
    {//需与uboot变量配置一致
        sprintf(env, "console=ttyS0,%dn8 mem=240M  mtdparts=rtk_norsf_g3:896K(boot),128K(env),%dK(sys),%dK(usrimg1@main),%dK(usrimg2),%dK(usrappfs),%dK(para)", 
                            baudrate,
							flKernelSize/0x400,
                            flUsrImg1Size/0x400, 
                            flUsrImg2Size/0x400, 
                            flUsrAppSize/0x400,
                            flParaSize/0x400);
    }
    else
    {//需与uboot变量配置一致
        sprintf(env, "console=ttyS0,%dn8 mem=240M  mtdparts=rtk_norsf_g3:896K(boot),128K(env),%dK(sys),%dK(usrimg1),%dK(usrimg2@main),%dK(usrappfs),%dK(para)", 
                            baudrate,
							flKernelSize/0x400,
                            flUsrImg1Size/0x400, 
                            flUsrImg2Size/0x400, 
                            flUsrAppSize/0x400,
                            flParaSize/0x400);
    }
    setenv("bootargs",env);/*无saveenv*/
    return OK;
}






