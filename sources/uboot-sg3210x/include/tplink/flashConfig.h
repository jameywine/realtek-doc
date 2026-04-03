/*!Copyright (c) 2010-2011 TP-LINK Technologies CO.,LTD.
 *All rights reserved.
 *
 *\file     flashConfig.h
 *\brief    保存flash的基本配置.
 *
 *\author   黄江华 <huangjianghua@tp-link.net>
 *\version  1.0.0
 *\date     03/28/2014
 *
 *\history  \arg 1.0.0 03/28/2014, 黄江华, Create file.
 */

#ifndef _FLASH_CONFIG_H_
#define _FLASH_CONFIG_H_

/**********************************************************************/
/*                          CONFIGURATIONS                            */
/**********************************************************************/
/**********************************************************************/
/*                          INCLUDE_FILES                             */
/**********************************************************************/

#include<tplink/flashDrvLib.h>


/**********************************************************************/
/*                           DEFINES                                  */
/**********************************************************************/

#ifdef FLASH_SECTOR_SIZE
#undef FLASH_SECTOR_SIZE
#endif
#define FLASH_SECTOR_SIZE 				(flashGetSectorSize())

#ifdef FLASH_SECTOR_NUM
#undef FLASH_SECTOR_NUM
#endif
#define FLASH_SECTOR_NUM          		(flashGetSectorCount())


//#define FLASH_SIZE                  	(FLASH_SECTOR_SIZE * FLASH_SECTOR_NUM)
#define FLASH_SIZE                  4*16*512*1024

#define FLASH_BASE_ADDRESS				0
#define FLASH_SECTOR_ADDRESS(sector) 	(FLASH_BASE_ADDRESS + (sector) * FLASH_SECTOR_SIZE)


#define STATUS			int
#define ERROR			0
#define OK				1
#define BOOL 			int
#define TRUE 			1
#define	FALSE			0
#define bool 			int
#define true 			1
#define	false			0
//#define NULL			0
#define EOS 	   		'\0'

#undef LOCAL
#define LOCAL


/**********************************************************************/
/*                            TYPES                                   */
/**********************************************************************/

typedef unsigned char	UINT8;
typedef unsigned short	UINT16; 
typedef unsigned int 	UINT32;
typedef unsigned long	UINT64;
typedef int 			INT32;
typedef unsigned char	UCHAR;
typedef unsigned long	ULONG;

/**********************************************************************/
/*                            VARIABLES                               */
/**********************************************************************/
/**********************************************************************/
/*                            FUNCTIONS                               */
/**********************************************************************/
/**********************************************************************/
/*                            EXTERN_PROTOTYPES                       */
/**********************************************************************/
/**********************************************************************/
/*                            LOCAL_PROTOTYPES                        */
/**********************************************************************/
/**********************************************************************/
/*                            LOCAL_FUNCTIONS                         */
/**********************************************************************/
/**********************************************************************/
/*                            PUBLIC_FUNCTIONS                        */
/**********************************************************************/
/**********************************************************************/
/*                            GLOBAL_FUNCTIONS                        */
/**********************************************************************/

#endif/*_FLASH_CONFIG_H_*/


