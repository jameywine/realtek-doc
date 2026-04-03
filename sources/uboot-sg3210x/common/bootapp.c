/*! Copyright(c) 2008-2010 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 *\file		bootapp.c
 *\brief		
 *\details	
 *
 *\author	Cai Peifeng
 *\version	
 *\date		02Sep10
 *
 *\warning	
 *
 *\history \arg	02Sep10, Cai Peifeng, Create the file
 */
/**********************************************************************/
/*                          CONFIGURATIONS                            */
/**********************************************************************/
/**********************************************************************/
/*                          INCLUDE_FILES                             */
/**********************************************************************/

/*from uboot*/
#include<common.h>
#include<linux/ctype.h>
#include<vsprintf.h>
#include<linux/time.h>
#include<rtc.h>
#include<jffs2/jffs2.h>

/*our files*/
#include<tplink/flashDrvLib.h>
#include<tplink/flashAppFsImage.h>
#include <tplink/flashAppInner.h>
#include <tplink/flashConfig.h>
#include <tplink/flashAppInnerProfile.h>
#include<tplink/flashUtil.h>
#include <uboot/cmd/uboot_cmd.h>
#include <private/drv/gpio/gpio.h>
#include "dt_common.h"


//#include <rtk/drv/gpio/gpio.h>


/**********************************************************************/
/*                           DEFINES                                  */
/**********************************************************************/
/* 从 ad层 同步 供动态识别机型 */
typedef enum
{
	AD_REALTEK_TL_SG3210XHP_M2_V3 = 76,
	AD_REALTEK_TL_SG3210X_M2 = 78,
	AD_REALTEK_TL_SG3218XP_M2 = 79,
	AD_REALTEK_S5500_8MHP2XF = 89,
	AD_REALTEK_SG2210XMP_M2 = 92,
	AD_NULL_END
}combo_platform_info;


#ifndef bool
#define bool int
#endif

#define TFTP_LOAD_ADDR		0x81000000
//#define UBOOT_DEBUG
#define BUF_LINE			255		/*!< the max characters of a line*/
#define CMD_LEN				32

#define BOOT_ADDR_LEN		30
#define BOOT_FILE_LEN		160	/* max chars in file name */
#define BOOT_USR_LEN		20	/* max chars in user name */
#define BOOT_PASSWORD_LEN	20	/* max chars in password */		
#define MAC_ADDRESS_LENGTH	6
#define BOOT_VERSION		"1.0.0"


#define MAC_ADDRESS_LENGTH	6
#define IP_ADDRESS_LENGTH	17


#define SQUASHFS_HEADER_MAGIC_BH  0x6873
#define SQUASHFS_HEADER_MAGIC_TH   0x7173
/* 判断是否输入数字 */
#define IS_NOT_DIGITAL_LINE(line)    (_isDigitalString(line) == 0)    

#define ERR_NOT_DIGITAL_PRINT()      printf("Input string contains illegal characters.\r\n")

#define IS_NOT_NEW_LINE(line)		(strlen(line) != 0) 

/* 直接RAM启动配置选项 */
 #define TP_KERNEL_IMAGE_NAME   "uImage.img"
 #define TP_KERNEL_IMAGE_DL_ADDR 0x81000000
 #define TP_KERNEL_IMAGE_MAX_SIZE 8*1024*1024 /*8M*/


 #define TP_USER_IMAGE_NAME   "usrimage.jffs2"


 
 /*   
 	@def   : TP_USER_IMAGE_DL_ADDR
 	@brief : 将usrimage.jffs2下载到RAM的地址
	linux配置文件linux-3.6.5-flash.config(buildroot\board\broadcom\hurricane2目录下)的
	配置项CONFIG_TPLINK_ROOFS_LOAD_ADDR需要与该值对应，且为RAM的偏移地址:
	CONFIG_TPLINK_ROOFS_LOAD_ADDR = TP_USER_IMAGE_DL_ADDR - RAM的基地址(0x60000000)
 */
 #define TP_USER_IMAGE_DL_ADDR  0x83000000 

 /*
	@def   : TP_USER_IMAGE_MAX_SIZE
	@brief : usrimage.jffs2的最大大小
 */
 #define TP_USER_IMAGE_MAX_SIZE 10*1024*1024  /*10M*/


/**********************************************************************/
/*                            TYPES                                   */
/**********************************************************************/

typedef struct
{
	char ipAddr[BOOT_ADDR_LEN];			/*!< the ip of the switch*/
	char ipMask[BOOT_ADDR_LEN];			/*!< the mask of the switch*/
	char routeAddr[BOOT_ADDR_LEN];		/*!< the route of the switch*/
	char hostAddr[BOOT_ADDR_LEN];		/*!< the tftp host address*/
	char fileName[BOOT_FILE_LEN];		/*!< the file that to be download*/
	int flags;							/*!< the current mode*/
}BOOT_CONFIG;

typedef struct _cmdStruct
{
	bool	(*cmdFunc)(void);				/*命令回调函数*/
	char*	description;				/*功能描述*/
}cmdStruct;
/**********************************************************************/
/*                            FUNCTIONS                               */
/**********************************************************************/

void printBootromLogo(void);
bool cmdHelp(void);
bool cmdReboot(void);
bool cmdReset(void);
bool cmdStart(void);
bool cmdActivateBackupImage(void);
bool cmdShowImage(void);
bool cmdPwdRecovery(void);

/**********************************************************************/
/*                            VARIABLES                               */
/**********************************************************************/

/*!
  var				bootCommand
   \brief			the comand list
   \description	
   \range		
   \usage		
*/
cmdStruct bootCommand[] = 
{
    {cmdHelp,					"- Print this boot menu"},
    {cmdReboot, 				"- Reboot"},
    {cmdReset,					"- Reset"},
    {cmdStart,					"- Start"},
    {cmdActivateBackupImage,	"- Activate Backup Image"},
    {cmdShowImage,              "- Display image(s) info"},	
    {cmdPwdRecovery,            "- Password recovery"},
	{NULL,NULL}
};


static char* imageName[2] = 
{
	"image1.bin",
	"image2.bin"	
};	

static int cmdNumber = sizeof(bootCommand) / sizeof(cmdStruct) - 1;

static int g_ExitMenuFlag = 0;


/**********************************************************************/
/*                            EXTERN_PROTOTYPES                       */
/**********************************************************************/

extern int readline_into_buffer(const char *const prompt, 
								char *buffer);


extern IPaddr_t string_to_ip(const char *s);

extern void ip_to_string(IPaddr_t x, char *s);

extern void to_tm(int tim, struct rtc_time * tm);

extern int sysGetFlAddr(char * key);
extern int AutoFixKernel(void);
extern int sysSetActiveInfo(int nRegion);
extern int sysSetActiveInfo(int nRegion);
extern ulong getenv_ulong(const char *name, int base, ulong default_val);
extern int   flashAppInit(void);
extern int sysSetActiveInfoWithOutFlash(int nRegion, int baudrate);
extern int sysGetMtdpartsInfo(char * key);
extern int sysFixKernel(char * pImageBuff, int len);


/**********************************************************************/
/*                            LOCAL_PROTOTYPES                        */
/**********************************************************************/
/**********************************************************************/
/*                            LOCAL_FUNCTIONS                         */
/**********************************************************************/

#define is_digit(c)	((c) >= '0' && (c) <= '9')
#if 0
static int skip_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}
#endif


static int _isDigitalString(const char* str)
{
    int len = 0;
    char *temp = NULL;

    if ((str == NULL) ||
       ((len = strlen(str)) == 0))
    {
        return 0;
    }

    temp = (char *)str;
    while (*temp != '\0')
    {
        if (((*temp) < '0') ||
           ((*temp) > '9'))
        {
            return 0;
        }
        temp++;
    }
    return 1;
}

/******************************************************************************
*
*
* skipSpace - advance pointer past white space
*
* Increments the string pointer passed as a parameter to the next
* non-white-space character in the string.
*/
void skipSpace(char **strptr)
{
	char *pLine;
	int len;
	
	if( NULL == strptr || NULL == *strptr)
	{
		return;
	}
	
	/*skip the last space*/
	len = strlen( *strptr);

	pLine = *strptr + len - 1;

	while( pLine >= *strptr && (*pLine == ' ') )
	{
		*pLine = EOS;
		pLine --;
	}

	/*skip the first space*/
	while( isspace( (UINT32) ** strptr) )
		++ *strptr;	
}


/*!
 *\fn			bool cmdReadLine(char *param)
 *\brief   		read a line
 *
 *\param[in]   
 *\param[out]  
 *
 *\return  
 *\retval  
 *
 *\note		  
 */
static bool cmdReadLine(char *param, const char *const prompt)
{
	int len = 0;

	if( NULL == param)
	{
		return FALSE;
	}
	
	/*read string*/
	len = readline_into_buffer(prompt, param);

	if( 0 == len)
	{
		return FALSE;
	}
	else
	{
		param[len] = '\0';
	}

	/*turn the line feed to end*/
	if(param[strlen(param)-1] == 0xa)
	{
		param[strlen(param)-1] = EOS;
	}

	return TRUE;
}

#if 0
/*!
   \function	cmdGetName(char *param,char *pName)	
   \brief		
   \descrition	get the commander name of the string.
   \param[in]	param:the string that to be parsse.
   \param[out]	pName:store the commander name.
   \return		TRUE:success;FALSE:some error has occur.
   \calls		
   \others		
*/
static bool cmdGetName(char *param, char *pName)
{
	int len;
	int i;
	
	if(NULL == param || NULL == pName)
	{
		return FALSE;
	}

	len = strlen(param);
	
	if( 0 == len)
	{
		return FALSE;
	}
	
	for(i=0; i < len; i++)
	{
		if( i > CMD_LEN)
		{
			return FALSE;
		}
		if(isspace((int)param[i]) )
		{
			break;
		}
	}
	
	strncpy(pName,param,i);
	pName[i] = '\0';
	return TRUE;	
}

/*!
   \function	cmdGetNextArg(char **param,char *pName,int *pEnd)	
   \brief		get the next argument
   \descrition	
   \param[in]	param:the string that to be parse
   \param[out]	pName:store the next argument
   \param[out]  pEnd:the param has the next argument or not.
   				FALSE:has the next argument;TRUE:not has the next argument.
   \return		FALSE:some error has occur.TRUE:success
   \calls		
   \others		
*/
static int cmdGetNextArg(char **param, char *pName, int *pEnd)
{
	int len;
	int i;
	char *pFirst;

	if( NULL == param || NULL == *param || NULL == pEnd)
	{
		return FALSE;
	}
	
	skipSpace(param);

	if (**param == EOS)
	{
		*pEnd = TRUE;
		return TRUE;
	}
	
	len = strlen(*param);
	
	pFirst = *param;
	
	for(i=0; i < len; i++)
	{
		if( i > BUF_LINE)
		{
			printf("The param length must less than 255.\r\n");
			*pEnd = FALSE;
			return FALSE;
		}
		if(isspace((int)**param))
		{
			break;
		}
		(*param)++;					
	}

	if( NULL != pName)
	{
		strncpy(pName,pFirst,i);
		pName[i] = '\0';
	}

	*pEnd = FALSE;
	
	return TRUE;	
}
#endif

/*!
 *\fn			void cmdParseCmd(char *param)
 *\brief   		parse the command and run the comand
 *
 *\param[in]   
 *\param[out]  
 *
 *\return  
 *\retval  
 *
 *\note		  
 */
void cmdParseCmd(char *param)
{
	
    int index = 0;
    int cmdNumber = 0;

    if (NULL == param)
    {
        return;
    }

    if (strlen(param) == 0)
    {
        return;
    }

    if (strncmp(param, "?", BUF_LINE) == 0)
    {
        cmdHelp();
        return;
    }

    cmdNumber = sizeof(bootCommand) / sizeof(cmdStruct) - 1;

    if (IS_NOT_DIGITAL_LINE(param))
    {
        ERR_NOT_DIGITAL_PRINT();
        return;
	}

    index = simple_strtoul(param, NULL, 10);

	if (index < 0 || index >= cmdNumber)
	{
		printf("Invalid boot option.\r\n");
	}	
    if ((index >= 0) && (index < cmdNumber) && bootCommand[index].cmdFunc != NULL)
    {
        bootCommand[index].cmdFunc();
    }
    return;	

}




/*!
 *\fn			bool ftpGetImage(int fSocket,char **pData,int *fileLength, int maxSize)
 *\brief   
 *
 *\param[in]   
 *\param[out]  
 *
 *\return  
 *\retval  
 *
 *\note		  
 */
static bool ftpGetImage(char **pData, char* pFileName,int *fileLength, int maxSize)
{
	char *pBuf;				
    char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;

    if (NULL == pFileName || NULL == fileLength)
    {
        return FALSE;
    }    
    #if 1
    sprintf(command, "rtk network on");
    run_command(command, flag);
    #endif
    pBuf = (char *)TFTP_LOAD_ADDR;
	memset(command,0,CONFIG_SYS_CBSIZE);
//    sprintf(command, "tftpboot 0x%0x %s 0x%0x 0x%0x", (unsigned int)pBuf, pFileName, (unsigned int)fileLength, maxSize);
    sprintf(command, "tftpboot 0x%0x %s", (unsigned int)pBuf, pFileName);
    printf("%s:%s,0x%x\n",__FUNCTION__,command,(unsigned int)*fileLength);

    run_command(command, flag);
    *fileLength = (size_t)NetBootFileXferSize;
    printf("%s:%s,0x%x\n",__FUNCTION__,command,(unsigned int)*fileLength);

	*pData = pBuf;
	return TRUE;

}

static void sysReboot(void)
{
    char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0; 
    sprintf(command, "reset");
    run_command(command, flag); 
}


#if 0
/*!
   \function	helpIfconfig()
   \brief		show the ifconfig help.
   \descrition
   \param[in]	N/A
   \param[out]	N/A
   \return
   \calls
   \others
*/
static void helpIfconfig(void)
{
	printf("Usage (ifConfig): Parameters: [[<option> <value> ...]]\r\n");
	printf("With no parameters, all current interfaces are listed.\r\n");
	printf("Value options are:\r\n");
	printf("\tip <address>		- IP address associated with interface\r\n");
	printf("\tmask <value>		- Subnet mask associated with interface\r\n");
	printf("\tgateway <adress>	- Default gateway\r\n");
}
#endif

#if 1
static image_header_t *image_get_kernel(ulong img_addr, int verify)
{
	image_header_t *hdr = (image_header_t *)img_addr;

	if (!image_check_magic(hdr)) {
		/*puts("Bad Magic Number\n");*/
		//bootstage_error(BOOTSTAGE_ID_CHECK_MAGIC);
		return NULL;
	}
	//bootstage_mark(BOOTSTAGE_ID_CHECK_HEADER);

	if (!image_check_hcrc(hdr)) {
		/*puts("Bad Header Checksum\n");*/
		//bootstage_error(BOOTSTAGE_ID_CHECK_HEADER);
		return NULL;
	}

	//bootstage_mark(BOOTSTAGE_ID_CHECK_CHECKSUM);
	/*image_print_contents(hdr);*/

	/*if (verify) {
		puts("   Verifying Checksum ... ");
		if (!image_check_dcrc(hdr)) {
			printf("Bad Data CRC\n");
			bootstage_error(BOOTSTAGE_ID_CHECK_CHECKSUM);
			return NULL;
		}
		puts("OK\n");
	}*/
	//bootstage_mark(BOOTSTAGE_ID_CHECK_ARCH);

	if (!image_check_target_arch(hdr)) {
		/*printf("Unsupported Architecture 0x%x\n", image_get_arch(hdr));*/
		//bootstage_error(BOOTSTAGE_ID_CHECK_ARCH);
		return NULL;
	}
	return hdr;
}


static image_header_t* getImageHeadInfo(ulong img_addr, ulong* os_data, ulong* os_len)
{
	
	image_header_t	*hdr = NULL;
	
	/* copy from dataflash if needed */
	img_addr = genimg_get_image(img_addr);

	
	//bootstage_mark(BOOTSTAGE_ID_CHECK_MAGIC);

	/* check image type, for FIT images get FIT kernel node */
	*os_data = *os_len = 0;
	switch (genimg_get_format((void *)img_addr)) {
		case IMAGE_FORMAT_LEGACY:
			/*printf("## Booting kernel from Legacy Image at %08lx ...\n",
					img_addr);*/
			hdr = image_get_kernel(img_addr, 0);
			if (!hdr)
				return NULL;
			//bootstage_mark(BOOTSTAGE_ID_CHECK_IMAGETYPE);

			/* get os_data and os_len */
			switch (image_get_type(hdr)) {
			case IH_TYPE_KERNEL:
			case IH_TYPE_KERNEL_NOLOAD:
				*os_data = image_get_data(hdr);
				*os_len = image_get_data_size(hdr);
				break;
			case IH_TYPE_MULTI:
				image_multi_getimg(hdr, 0, os_data, os_len);
				break;
			case IH_TYPE_STANDALONE:
				*os_data = image_get_data(hdr);
				*os_len = image_get_data_size(hdr);
				break;
			default:
				/*printf("Wrong Image Type for\n");*/
				//bootstage_error(BOOTSTAGE_ID_CHECK_IMAGETYPE);
				return NULL;
			}
		//bootstage_mark(BOOTSTAGE_ID_DECOMP_IMAGE);
		break;
	default:
		/*printf("Wrong Image Format\n");*/
		//bootstage_error(BOOTSTAGE_ID_FIT_KERNEL_INFO);
		return NULL;
	}

	return hdr;

}

#endif

static bool imageFileExist(ulong nRegion)
{
    image_header_t* phdr = NULL;
    ulong os_data = 0;
    ulong os_len = 0;
    int img_addr = 0;
    //int kernel_addr = 0;
    //int i = 0;
    struct jffs2_unknown_node head;
	struct jffs2_unknown_node crcnode;
	uint32_t crc;
    int nRet = ERROR;


    phdr = getImageHeadInfo(IMAGE_ADDR, &os_data, &os_len);

    if (NULL == phdr)
    {
        nRet = AutoFixKernel();
        if(ERROR == nRet)
        {
            return FALSE;
        }        
    }

    img_addr = (1 == nRegion)? (sysGetFlAddr("usrimg1")):(sysGetFlAddr("usrimg2"));
    memset(&head, 0, sizeof(head));

    l_readFlash((char *)&head, sizeof(struct jffs2_unknown_node), img_addr);

	/*check magic number*/
	if (head.magic != JFFS2_MAGIC_BITMASK)
	{
		//printf("wrong magic number %d\n", head.magic);
		
		
		if (SQUASHFS_HEADER_MAGIC_BH == head.magic
		&& SQUASHFS_HEADER_MAGIC_TH == head.nodetype)
		{
			return TRUE;
		}
		
		return FALSE;
	}	
	else
	{
	
	/* OK, now check for node validity and CRC */
	crcnode.magic = JFFS2_MAGIC_BITMASK;
	crcnode.nodetype = head.nodetype;
	crcnode.totlen = head.totlen;
	crc = crc32_no_comp(0, (uchar *)&crcnode, sizeof(crcnode)-4);

	if (head.hdr_crc != crc) {
		//printf("head.hdr %d, crc %d\n", head.hdr_crc, crc);
		return FALSE;
	}
	}	


    return TRUE;
    
}


int getStartUpIndex(void)
{
    char value[ENV_ITEM_VALUE_LEN] = {0};

    getEnvItem(TP_SWITCH_START_IMAGE, value);
	
    /*
    char * pArea = NULL;
    
    pArea = getenv("bootarea");

    if((pArea[0] != '1') && (pArea[0] != '2'))
    {
        return -1;
    }
	
    return pArea[0] - '1' + 1;
    */
    if(strcmp(value, TP_SWITCH_IMAGE_NAME_1) == 0)
    {
        return 1;
    }
    else if(strcmp(value, TP_SWITCH_IMAGE_NAME_2) == 0)
    {
        return 2;
    }
    else
    {
        return 1;//默认1区启动
    }
}

void setStartUpIndex(int index)
{
    char tmp[BUF_LINE] = {0};

    if (1 != index && 2 != index)
    {
    return;
    }

    if(1 == index)
    {
        setEnvItem(TP_SWITCH_START_IMAGE, TP_SWITCH_IMAGE_NAME_1);
        setEnvItem(TP_SWITCH_BACKUP_IMAGE, TP_SWITCH_IMAGE_NAME_2);
    }
    else
    {
        setEnvItem(TP_SWITCH_START_IMAGE, TP_SWITCH_IMAGE_NAME_2);
        setEnvItem(TP_SWITCH_BACKUP_IMAGE, TP_SWITCH_IMAGE_NAME_1);
    }

    sysSetActiveInfo(index);
    sprintf(tmp, "%d", index);
    setenv("bootarea", tmp);	
    saveenv();
 
}	


BOOL resaveStartupIndex(void)
{
    //char line[BUF_LINE] = {0};
    //ulong img_addr = 0;

    if(1 == getStartUpIndex())
    {
        setStartUpIndex(1);
    }
    else
    {
        setStartUpIndex(2);
    }
    

    return TRUE;
}

	
/**********************************************************************/
/*                            PUBLIC_FUNCTIONS                        */
/**********************************************************************/



//extern int rtc_get(struct rtc_time *tmp);

/*!
 *\fn			void printBootromLogo()
 *\brief   		printf bootrom logo
 *
 *\param[in]   	N/A
 *\param[out]  	N/A
 *
 *\return  		N/A
 *\retval  		N/A
 *
 *\note		  
 */
void printBootromLogo()
{
   
    char buf[100];

	char* time = __DATE__;

	int year = 0;

	int i = 0;


	for (i = strlen(time) - 1; i > 0 ; i--)
	{
		if (time[i] == 32)
		{
			break;
		}	
	}	

	year = simple_strtoul(&time[i + 1], NULL, 10);

    sprintf(buf, "Create Date: %s - %s\n", __DATE__, __TIME__);

    printf("\t\t%s","**********************************************\n");
    printf("\t\t%s%s%s","*         TP-Link  BOOTUTIL(v",BOOT_VERSION,")          *\n");
    printf("\t\t%s","**********************************************\n");
    printf("\t\t%s%d%s","Copyright (c) ", year, " TP-Link Corporation Limited    \n");
    printf("\t\t%s\r\n",buf);


}  


/*!
 *\fn			bool cmdHelp(char *param)
 *\brief   
 *
 *\param[in]   
 *\param[out]  
 *
 *\return  
 *\retval  
 *
 *\note		  
 */
 
bool cmdHelp(void)
{
	int i = 0;	
	
	printf("   Boot Menu\r\n");
	
	for (i = 0; i < cmdNumber; i++)
	{
		printf("%-2d %s\n", i, bootCommand[i].description);
	}
    
	printf("\r\nEnter your choice(0-%d)\r\n", cmdNumber-1);
	
	printf ("\n");

	return TRUE;
}


bool cmdReboot(void)
{
    char line[BUF_LINE] = {0};

    printf("Are you sure to reboot the device?[Y/N]:");
    cmdReadLine(line, NULL);
	
    if (tolower(line[0]) != 'y')
    {
        return TRUE;
    }

    printf("rebooting...\r\n");
    mdelay(1000);    /* pause a second */
    sysReboot();
    return TRUE;
}


bool cmdReset(void)
{
    char line[BUF_LINE] = {0};

    printf("Are you sure to reset the device?[Y/N]:");
  	cmdReadLine(line, NULL);

    if (tolower(line[0]) != 'y')
    {
        return TRUE;
    }
    setEnvItem(TP_SWITCH_START_CONFIG, "NULL");
    printf("resetting...\r\n");
    mdelay(1000);    /* pause a second */
    sysReboot();
    
    return TRUE;
}

bool cmdStart(void)
{
    #if 0
    char command[256];
    int nRet = 0;

    memset(command, 0, sizeof(command));
    sprintf(command, "bootm 0xb4100000");
    nRet = run_command(command, 0);
    if(1 == nRet )
    {
        printf("The kernel has been damaged!\nbegin to fix kernel...\n");

        return FALSE;
    }
    printf("%s,line%d \n\r",__FUNCTION__,__LINE__);
    return TRUE;
    #else

//	printf("cmdStart ....\n");
    //ulong img_addr = 0;
    char command[CONFIG_SYS_CBSIZE];
    char value[ENV_ITEM_VALUE_LEN] = {0};
    char tmp[BUF_LINE] = {0};
    int nRet = 0;
	int baudrate = 0;
	
	baudrate = getenv_ulong("baudrate", 10, 38400);

    
    flashAppInit();
    getEnvItem(TP_SWITCH_START_CONFIG, value);
    if(strcmp(value, "NULL") == 0)
    {
        // char * configarea = NULL;
        // configarea = getenv("config_area");
        // if(strcmp(configarea, "null") != 0)
        {
            setenv("config_area", "null");
            saveenv();           
        }
         setEnvItem(TP_SWITCH_START_CONFIG, TP_SWITCH_CONFIG_NAME_1);
    }
    else
    {
        char * configarea = NULL;
        configarea = getenv("config_area");
        if(strcmp(configarea, TP_SWITCH_CONFIG_NAME_1) != 0)
        {
            setenv("config_area", TP_SWITCH_CONFIG_NAME_1);
            saveenv();
        }
    }  

    if (FALSE == imageFileExist(getStartUpIndex()))
    {
        printf("The Startup Image does not exist\r\n");
        //return FALSE;
    }

    /*启动前需要检查更新启动区域*/
    int index = getStartUpIndex();
   // int curArea = 1;
   // char * pArea = NULL;
   // pArea = getenv("bootarea");

   // curArea = pArea[0] - '1' + 1;

    sysSetActiveInfoWithOutFlash(index, baudrate);/*去掉不必要的save操作*/
    sprintf(tmp, "%d", index);
    setenv("bootarea", tmp);	


    printf("\nBegin to startup system, please wait a moment...\n");

    memset(command, 0, sizeof(command));
    sprintf(command, "bootm 0x%x", IMAGE_ADDR);

    nRet = run_command(command, 0);
    if(1 == nRet )
    {
        nRet = AutoFixKernel();
        if(ERROR == nRet)
        {
            return FALSE;
        }
    }
    
    return TRUE;
    #endif
}



bool cmdActivateBackupImage(void)
{
    char line[BUF_LINE] = {0};
    ulong nRegion = 1;

    	
    if (1 == getStartUpIndex())
    {
    	    nRegion = 2;
    }
    else
    {
    	    nRegion = 1;
    	
    }	

    if (FALSE == imageFileExist(nRegion))
    {
        	printf("The Backup Image does not exist in system\r\n");
        	return FALSE;
    }
    
    /* 设置全局变量 */
    printf("Are you sure to set the Backup Image as Startup Image?[Y/N]:");
	line[0] = 0;
    cmdReadLine(line, NULL); 
    if (tolower(line[0]) == 'y')
    {
		if(1 == getStartUpIndex())
		{
			setStartUpIndex(2);
		}
		else
		{
			setStartUpIndex(1);
		}
				 
        /* 显示文件列表 */
        cmdShowImage();
		
    }
    printf("\r\n");
    return TRUE;
}

bool cmdShowImage(void)
{ 
    #if 1
	//image_header_t* phdr = NULL;
	int index = 0;

	ulong img_len = 0;
	//uint32_t time = 0;
       char cImgName[16] = {0};
       char * pImgName = "NULL";

	char* startUpLabel = "(*)"; 
	char* backUpLabel = "(b)";

	printf("Images in system:\r\n");
	printf("index  Attribute  Size        Filename\r\n");
	printf("-----  ---------  ----------  ---------------------\r\n");

	for (index = 0; index < sizeof(imageName)/ sizeof(char*); index ++)
	{
            char* label = NULL;
            if (FALSE == imageFileExist(index + 1))
            {
                img_len = 0;
                pImgName = "NULL";
            }
            else
            {
                sprintf(cImgName, "usrimg%d", (index+1));  
                img_len = (int)sysGetMtdpartsInfo((char *)cImgName);
                pImgName = imageName[index];
            }
            if (index + 1 == getStartUpIndex())
            {
                label = startUpLabel;
            }
            else
            {
                label = backUpLabel;
            }	

            printf("%-5d  %-9s  %-10d  %-21s\r\n",
								index + 1, label, (int)img_len,
								pImgName);
	}
	
    printf("-----  ---------  ----------  ---------------------\r\n");
    printf("\r\n");
    printf("(*) - with the Startup attribute\r\n");
    printf("(b) - with the Backup attribute\r\n");	
    #endif
	return TRUE;    
    
}

bool cmdPwdRecovery(void)
{
    UINT8 gevValue[512] = {0};
    int pwdEnable = 0;
    char line[BUF_LINE] = {0};

    printf("This will delete all the previously created accounts. Continue?[Y/N]:");
    cmdReadLine(line, NULL);
	
    if (tolower(line[0]) != 'y')
    {
        return TRUE;
    }
    
    /* 密码恢复功能开关判断 */
    if (ERROR == getEnvItem(TP_SWITCH_PWD_RECOVERY, (char *)gevValue))
    {
        printf("Fatal error: Failed to get password recovery status.\r\n");
        return FALSE;
    }
    pwdEnable = simple_strtoul((const char *)gevValue, NULL, 10);
    if (pwdEnable == 0)
    {
        printf("Password Recovery function is disabled.\r\n");
        return TRUE;
    }

    /* 设置密码恢复标志位 */
    if (ERROR == setEnvItem(TP_SWITCH_PWD_RECOVERY_FLAG, "1"))
    {
        printf("Fatal error: Failed to set password recovery flag.\r\n");
        return FALSE;
    }
    printf("Operation OK!\r\n");
    
    return TRUE;
}
 

/**********************************************************************/
/*                            GLOBAL_FUNCTIONS                        */
/**********************************************************************/

void menu_shell(void)
{
    char readBuf[BUF_LINE];
    int status;
    char *pLine;

	/*check reset_dsable flag, if reset_disable, remove 'reset' cmd*/
	/* debug uboot将这一段注释掉，否则会影响进rtk的shell */
	#ifndef UBOOT_DEBUG
	char str[10];
	memset(str,0,10);
	getEnvItem(TP_SWITCH_RESET_DISABLE, str);
	if (simple_strtoul(str, NULL, 10))
	{
		int i=0;
		for (i=2;i<cmdNumber;i++)
		{
			memcpy(&bootCommand[i],&bootCommand[i+1],sizeof(cmdStruct));
		}
		cmdNumber--;
	}
	#endif

    cmdHelp();
#ifdef CONFIG_SYS_PROMPT
#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT	"tplink> "  
#endif

#if 1            
    g_ExitMenuFlag = 0;
    while(!g_ExitMenuFlag)
#else
    while(1)
#endif
    {	
        /*!< read the input command (add by Cai Peifeng:2010-9-3 10:35:16)*/
        readBuf[0] = 0;
        status = cmdReadLine(readBuf, CONFIG_SYS_PROMPT);

        if( FALSE == status)
        {
            continue;
        }
        /*skip the space*/
        pLine = readBuf;
        skipSpace(&pLine);

        /*handle the command*/
        cmdParseCmd(pLine);
    }

}

#define INNER_CONFIG_STRING(x) 			#x
#define USB_RJ45_CONFIG					INNER_CONFIG_STRING(usb_rj45)
#define COMBO_INFO_CONFIG				INNER_CONFIG_STRING(combo_info)

/* usb_rj45_config = 0 机型没有usb，= 1 机型有usb */
static int usb_rj45_config = 0;

/* usb_rj45_config = 0禁用usb串口输入，>0使能usb串口输入 */
static int usb_rj45_enable = 0;

static unsigned int usb_rj45_en_pin = 0;
static unsigned int usb_rj45_insert_pin = 0;


char* tpBoardInfoGet(void)
{
	
	int rv = 0;
	unsigned int combo_info = AD_NULL_END;
	char value[256] = {0};
	static char  combo_str[64]={0};

	flashAppInit();

	if((rv = getEnvItem(COMBO_INFO_CONFIG,value)) == OK)
	{
		combo_info= simple_strtoul(value, NULL, 16);
		
		switch (combo_info)
		{
		case AD_REALTEK_TL_SG3210XHP_M2_V3:
		case AD_REALTEK_S5500_8MHP2XF:
			strcpy(combo_str,"RTL9302C_SG3210XHP_M2_V3");
			break;
		case AD_REALTEK_TL_SG3210X_M2:
			strcpy(combo_str,"RTL9302C_SG3210X_M2");
			break;
		case AD_REALTEK_TL_SG3218XP_M2:
			strcpy(combo_str,"RTL9302C_SG3218XP_M2");
			break;
		case AD_REALTEK_SG2210XMP_M2:
			strcpy(combo_str,"RTL9302C_SG2210XMP_M2");
			break;
		default:
			return NULL;		
			break;
		}
		
	}
	else
	{
		return NULL;
	}

	return combo_str;
}



int tpBoardUSBConsoleEn(unsigned int data)
{
	rtk_intGpio_pin_set(usb_rj45_en_pin, data);
	return 0;
}



int tpBoardUSBConsoleInsertCheck(void)
{
	unsigned int bitData = 0;

	rtk_intGpio_pin_get(usb_rj45_insert_pin, &bitData);
	
	if(bitData)
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}

void tpBoardUSBConsoleInit(void)
{
	static int initFlag = 0;
    char value[256] = {0};
	int rv = 0;
	unsigned int combo_info = AD_NULL_END;

	if(initFlag == 0)
	{
		initFlag = 1;

		if((rv = getEnvItem(COMBO_INFO_CONFIG,value)) == OK)
		{
			combo_info= simple_strtoul(value, NULL, 16);
			
			switch (combo_info)
			{
			case AD_REALTEK_TL_SG3210XHP_M2_V3:
			case AD_REALTEK_S5500_8MHP2XF:
			case AD_REALTEK_TL_SG3210X_M2:
			case AD_REALTEK_SG2210XMP_M2:
				usb_rj45_config = 1;
				usb_rj45_insert_pin = 20;
				usb_rj45_en_pin = 6;
				break;
			case AD_REALTEK_TL_SG3218XP_M2:
				usb_rj45_config = 1;
				usb_rj45_insert_pin = 20;
				usb_rj45_en_pin = 23;
				break;
			default:
				usb_rj45_config = 0;		
				break;
			}
			
		}	

		if((rv = getEnvItem(USB_RJ45_CONFIG,value)) == OK)
		{
			usb_rj45_enable = simple_strtoul(value, NULL, 16);
		}
		else
		{
			usb_rj45_enable = 1;
		}
	    

	}
}


void tpBoardUSBConsole(void)
{
	static int usb_rj45_status = -1;

	tpBoardUSBConsoleInit();

	if(usb_rj45_config)
	{
		if(usb_rj45_enable)
		{
			if(tpBoardUSBConsoleInsertCheck() != 0) /* USB INSERT */
			{
				if(usb_rj45_status != 1)
				{
					tpBoardUSBConsoleEn(1);
					usb_rj45_status = 1;
				}
			}
			else
			{
				if(usb_rj45_status != 0)
				{
					tpBoardUSBConsoleEn(0);
					usb_rj45_status = 0;
				}
			}
		}
		else
		{
			if(usb_rj45_status != 0)
			{
				tpBoardUSBConsoleEn(0);
				usb_rj45_status = 0;
			}
			
		}
	}
}


int abortboot(int bootdelay)
{
	int abort = 0;


	if (bootdelay >= 0)
		printf("\nHit any key to stop autoboot: %2d ", bootdelay);


	while ((bootdelay > 0) && (!abort)) {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i) {
			/*在hit loop中的动态切换轮询*/
			tpBoardUSBConsole();
			
			if (tstc()) {	/* we got a key press	*/
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/

				(void) getc();  /* consume input	*/

				break;
			}
			udelay(10000);
		}

		printf("\b\b\b%2d ", bootdelay);
	}

	putc('\n');

	return abort;
}

void AutoStart(void)
{
    if(!abortboot(3))
    {
        cmdStart();
    }
}

int AutoFixKernel(void)
{
    int nRet = OK;
    char *buffer = NULL;
    int maxSize = FLASH_SIZE;
    int recSize = 0;
    char downFile[BUF_LINE] = {0};
    char command[CONFIG_SYS_CBSIZE];

    printf("The kernel has been damaged!\nbegin to fix kernel...\n");
    printf("Please put the uimage.img into the tftpserver directory\n");
    printf("You can press CTRL-C to stop loading\r\n");

    sprintf(downFile, "%s:uImage.img", getenv("serverip"));

    /* 获取文件 */
    if( FALSE == ftpGetImage(&buffer, downFile, &recSize, maxSize))
    {
        printf("Download failed\r\n");
        return ERROR;
    }

    if (0 == recSize)
    {
        printf("Download failed\r\n");
        return ERROR;		
    }	

    nRet = sysFixKernel(buffer, recSize);
    if(ERROR == nRet)
    {
        printf("the kernel fixing is fail\n");
        return ERROR;
    }

    printf("The kernel has been repaired, begin to startup.\n");

    memset(command, 0, sizeof(command));
    sprintf(command, "bootm 0x%x", IMAGE_ADDR);
    nRet = run_command(command, 0);
    return OK;
}

void tpBoardGpioRestore(void)
{
	#if defined(CONFIG_RTL8380)
	char value[256] = {0};
	int rv = 0;
	unsigned int combo_info = AD_NULL_END;
	//int gpio_a3 = GPIO_ID(GPIO_PORT_A, 3);
	int gpio_a4 = GPIO_ID(GPIO_PORT_A, 4);
	int gpio_b6 = GPIO_ID(GPIO_PORT_B, 6);
	extern void rtl8380_intGpio_pindata_set(uint32 pin, uint32 bitData);
	extern void rtl8380_intGpio_DirSet(uint32 pin, drv_gpio_direction_t dir);

	flashAppInit();

	if((rv = getEnvItem(COMBO_INFO_CONFIG,value)) == OK)
	{
		combo_info= simple_strtoul(value, NULL, 16);
		switch (combo_info)
		{
		case AD_REALTEK_TL_SL2428P_V4:
		case AD_REALTEK_TL_SL2428P_V5:
		case AD_REALTEK_TL_SL2428P_V6:
			/* RTL8231 MDIO */
			break;
		case AD_REALTEK_TL_SG3428MP:
			/* usb insert */
			rtl8380_intGpio_DirSet(gpio_b6, GPIO_DIR_IN);
			break;
		case AD_REALTEK_TL_SG3428:
			/* usb enable */
			rtl8380_intGpio_pindata_set(gpio_b6, 0);
			break;
		case AD_REALTEK_TL_SG3210:
			/* sfp insert */
			rtl8380_intGpio_DirSet(gpio_a4, GPIO_DIR_IN);
			break;
		default:
			break;
		}
	}

	#endif
}


