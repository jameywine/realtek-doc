/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : If the RTOS (Real-time Operation System) is not supported those kind
 *           of library, you can use the library functions.
 *
 * Feature : 1)Library for OS independent part
 *
 */

#ifndef __OSAL_LIB_H__
#define __OSAL_LIB_H__


/*
 * Include Files
 */
#if defined(__BOOTLOADER__) || defined(__ATF__)
  #if defined(__ATF__)
    #include <string.h>
  #else
    #include <common.h>
  #endif
  #define osal_strncpy  strncpy
  #define osal_memset   memset
  #define osal_memcpy   memcpy
  #define osal_memcmp   memcmp
  #define osal_strcmp   strcmp
  #define osal_strncmp  strncmp
  #define osal_strcpy   strcpy
  #define osal_strlen   strlen
#elif defined(CONFIG_SDK_KERNEL_LINUX)
  #if defined(__KERNEL__)
    #include <linux/ctype.h>    /* for Kernel Space */
    #include <linux/kernel.h>
    #include <linux/string.h>
  #else
    #include <ctype.h>          /* for User Space */
    #include <string.h>
  #endif

  #define osal_strlen   strlen
  #define osal_strcmp   strcmp
  #define osal_strcpy   strcpy
  #define osal_strncpy  strncpy
  #define osal_strcat   strcat
  #define osal_strchr   strchr
  #define osal_memset   memset
  #define osal_memcpy   memcpy
  #define osal_memcmp   memcmp
  #define osal_strdup   strdup
  #define osal_strncmp  strncmp
  #define osal_strstr   strstr
  #define osal_strtok   strtok
  #define osal_strtok_r   strtok_r
  #define osal_toupper  toupper

#else
extern int   osal_strlen(const char *s);
extern int   osal_strcmp(const char *s1, const char *s2);
extern char *osal_strcpy(char *dst, const char *src);
extern char *osal_strncpy(char *dst, const char *src, int len);
extern char *osal_strcat(char *dst, const char *src);
extern char *osal_strchr(const char *str, int c);
extern void *osal_memset(void *dst, int val, int len);
extern void *osal_memcpy(void *dst, const void *src, int len);
extern int   osal_memcmp(const void *mem1, const void *mem2, int len);
extern char *osal_strdup(const char *s);
extern int   osal_strncmp(const char *str1, const char *str2, int len);
extern char *osal_strstr(const char *haystack, const char *needle);
extern char *osal_strtok(char *str, const char *delim);
extern char *osal_strtok_r(char *str, const char *delim, char **saveptr);
extern int   osal_toupper(int c);
#endif


#endif /* __OSAL_LIB_H__ */
