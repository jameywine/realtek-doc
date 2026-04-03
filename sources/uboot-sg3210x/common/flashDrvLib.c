/*!Copyright (c) 2010-2011 TP-LINK Technologies CO.,LTD.
 *All rights reserved.
 *
 *\file     flashDrvLib.c
 *\brief    flash操作的接口.
 *
 *\author   黄江华 <huangjianghua@tp-link.net>
 *\version  1.0.0
 *\date     03/31/2014
 *
 *\history  \arg 1.0.0 03/31/2014, 黄江华, Create file.
 */


/**********************************************************************/
/*                          CONFIGURATIONS                            */
/**********************************************************************/
/**********************************************************************/
/*                          INCLUDE_FILES                             */
/**********************************************************************/

#include<common.h>
#include<vsprintf.h>
#include<spi_flash.h>
#include<tplink/flashDrvLib.h>
#include<tplink/flashConfig.h>


/**********************************************************************/
/*                           DEFINES                                  */
/**********************************************************************/
#ifdef CONFIG_SYS_CBSIZE
#undef CONFIG_SYS_CBSIZE
#define CONFIG_SYS_CBSIZE		256
#endif

/**********************************************************************/
/*                            TYPES                                   */
/**********************************************************************/
/**********************************************************************/
/*                            VARIABLES                               */
/**********************************************************************/

static int  flashDevSectorSize = 0;
static int  flashSectorCount = 0;
static bool flashInit = -1;

/**********************************************************************/
/*                            FUNCTIONS                               */
/**********************************************************************/
/**********************************************************************/
/*                            EXTERN_PROTOTYPES                       */
/**********************************************************************/

extern int run_command(const char *cmd, int flag);


//extern int spi_flash_get_sector_size(void);


//extern int spi_flash_get_sector_num(void);


/**********************************************************************/
/*                            LOCAL_PROTOTYPES                        */
/**********************************************************************/
/**********************************************************************/
/*                            LOCAL_FUNCTIONS                         */
/**********************************************************************/
/**********************************************************************/
/*                            PUBLIC_FUNCTIONS                        */
/**********************************************************************/


int flashProbe(void)
{
	char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;
	sprintf(command, "sf probe 0x0");
	run_command(command, flag);
	return OK;
	
}	

int	flashDrvLibInit(void)
{   
	flashDevSectorSize = 4096 * 16;
    flashSectorCount = 512;
    //flashDevSectorSize = spi_flash_get_sector_size();
    //flashSectorCount = spi_flash_get_sector_num();
	/*printf("sector num %d, sector size %d\n", flashSectorCount, flashDevSectorSize);*/
    flashInit = 1;
    
    return OK;
}    

int	flashGetSectorCount(void)
{
    return flashSectorCount;
}    

int	flashGetSectorSize(void)
{
    return flashDevSectorSize;
}    

int	flashDiagnostic(void)
{
    return OK;
}    


int flashEraseBank(int firstSector, int nSectors)
{
    
	ULONG addrFirst = FLASH_SECTOR_ADDRESS(firstSector);  
    int count = flashDevSectorSize * nSectors;
	char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;

    if (-1 == flashInit)
    {
        return ERROR;
    }    

    if (0 == nSectors)
	{
      	return OK;
    } 

	if (firstSector < 0 || firstSector + nSectors > flashSectorCount) 
    {
        printf("flashEraseBank(): Illegal parms %d, %d\n", firstSector, nSectors);
        return ERROR;
    }

	sprintf(command, "sf erase 0x%0x 0x%0x", (unsigned int)addrFirst, count);
	run_command(command, flag);

    return OK;
}


int flashErase(int firstSector, unsigned int count)
{
    ULONG addrFirst = FLASH_SECTOR_ADDRESS(firstSector);
    char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;

    if (-1 == flashInit)
    {
        return ERROR;
    } 


	if (firstSector < 0 || firstSector >= flashSectorCount) 
    {
		printf("flashBlkWrite(): Sector %d invalid\n", firstSector);
		return (ERROR);
	}
       
	/* 
	 * Count must be within range and must be a long word multiple, as
	 * we always program long words.
	*/
    
	if ((count < 0) 
    || (count > (flashSectorCount - firstSector)))
    {
		printf("flashErase(): Count 0x%x invalid\n", count);
		return (ERROR);
	}
    

	sprintf(command, "sf erase 0x%0lx 0x%0x", addrFirst, count * flashDevSectorSize);
	run_command(command, flag);
    return OK;
}




int flashBlkWrite(int firstSector, char *buff, int offset, unsigned int count)
{
    ULONG addrFirst = FLASH_SECTOR_ADDRESS(firstSector) + offset;
    char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;

    if (-1 == flashInit)
    {
        return ERROR;
    } 

	if (firstSector < 0 || firstSector >  flashSectorCount) 
    {
		printf("flashBlkWrite(): Sector %d invalid\n", firstSector);
		return (ERROR);
	}
       
	/* 
	 * Count must be within range and must be a long word multiple, as
	 * we always program long words.
	*/
    
	if ((count < 0) 
    || (count > ((flashSectorCount - firstSector) * flashDevSectorSize - offset))) 
    {
		printf("flashBlkWrite(): Count 0x%x invalid\n", count);
		return (ERROR);
	}
	sprintf(command, "sf write 0x%p 0x%0lx 0x%0x", buff , addrFirst, count);
    run_command(command, flag);
    return OK;
}

int flashBlkRead(int firstSector, char *buff, int offset,unsigned int count)
{
    
    ULONG addrFirst = FLASH_SECTOR_ADDRESS(firstSector) + offset;
    char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;


    if (-1 == flashInit)
    {
        return ERROR;
    }  

    if (firstSector < 0 || firstSector >  flashSectorCount) 
    {
        printf("flashBlkRead(): Sector %d  %d invalid\n", firstSector, flashSectorCount);
        return (ERROR);
    }

    if (count < 0 || (count > ((flashSectorCount - firstSector) * flashDevSectorSize - offset))) 
    {
        printf("flashBlkRead(): Count 0x%x invalid\n", count);
        return (ERROR);
    }
	sprintf(command, "sf read 0x%p 0x%0lx 0x%0x", buff, addrFirst, count);
    run_command(command, flag);
    return OK;

}


int flashSyncFilesystem(void)
{
    return OK;
}

int flashProtectOn(void)
{
  	char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;
    
    if (-1 == flashInit)
    {
        return ERROR;
    } 
    
    sprintf(command, "protect on all");
    run_command(command, flag);
    return OK;
}    

int flashProtectOff(void)
{
  	char command[CONFIG_SYS_CBSIZE] = {0};
    int flag = 0;

    if (-1 == flashInit)
    {
        return ERROR;
    } 
    
    sprintf(command, "protect off all");
    run_command(command, flag);
    return OK;

}    


/**********************************************************************/
/*                            GLOBAL_FUNCTIONS                        */
/**********************************************************************/




