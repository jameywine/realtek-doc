/* $Header: /usr/cvsroot/target/h/wrn/wm/common/glue.h,v 1.3 2003/01/16 18:20:12 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/* 
 * $Log: glue.h,v $
 * Revision 1.3  2003/01/16 18:20:12  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:09  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.11  2001/01/19 22:21:28  paul
 * Update copyright.
 *
 * Revision 1.10  2000/03/17 00:16:37  meister
 * Update copyright message
 *
 * Revision 1.9  2000/03/09 19:09:22  josh
 * changes so we don't shoot ourselves in the foot if we're building
 * Emissary or Liaison
 *
 * Revision 1.8  1999/05/21 16:28:06  sra
 * Fix snprintf() installation test.
 *
 * Revision 1.7  1999/05/20 22:14:50  wes
 * If we're using the common snprintf, include its declaration here
 *
 * Revision 1.6  1999/05/20 21:05:54  sra
 * Add SNPRINTF and VSNPRINTF.
 *
 * Revision 1.5  1998/07/03 20:19:22  sra
 * Add etc_memcmp().
 *
 * Revision 1.4  1998/06/05 17:14:11  meister
 * STRNICMP takes 3 args, not two.
 *
 * Revision 1.3  1998/06/03 20:10:33  sar
 * Added some common string functions.  stdf.h has the externs for them
 * while glue.h will use them if the user installs them and doesn't
 * specify other functions to use.
 *
 * Revision 1.2  1998/02/25 04:43:25  sra
 * Update copyrights.
 *
 * Revision 1.1  1997/08/21 17:23:43  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 */

/* [clearcase]
modification history
-------------------
*/


#ifndef COMMON_GLUE_H
#define COMMON_GLUE_H

/*
 * Load the postamble portion of the customer's common.h file.
 */

#include <common.h>
#include <tplink/des/stdf.h>

#if INSTALL_COMMON_SNPRINTF
#include <tplink/des/snprintf.h>
#endif

/*
 * Default definitions for macros that stand in for ANSI functions.
 * These may need to be fine-tuned after we've run some compilation tests.
 */

#ifndef MEMCHR
#define MEMCHR(x,y,z)		memchr(x,y,z)
#endif

#ifndef MEMCMP
#if INSTALL_COMMON_MEMCMP
#define MEMCMP(x,y,z)		etc_memcmp(x,y,z)
#else
#define MEMCMP(x,y,z)		memcmp(x,y,z)
#endif
#endif

#ifndef MEMCPY
#define MEMCPY(x,y,z)		memcpy(x,y,z)
#endif

#ifndef MEMSET
#define MEMSET(x,y,z)		memset(x,y,z)
#endif

#ifndef STRCMP
#define STRCMP(x,y)		strcmp(x,y)
#endif

#ifndef STRCPY
#define STRCPY(x,y)		strcpy(x,y)
#endif

#ifndef STRLEN
#define STRLEN(x)		strlen(x)
#endif

#ifndef STRNCMP
#define STRNCMP(x,y,z)		strncmp(x,y,z)
#endif

#ifndef STRTOL
#if (INSTALL_COMMON_STRTOL)
#define STRTOL(S, E, R)		etc_strtol(S, E, R)
#else
#define STRTOL(S, E, R)		strtol(S, E, R)
#endif
#endif

#ifndef STRTOUL
#if (INSTALL_COMMON_STRTOUL)
#define STRTOUL(S, E, R)	etc_strtoul(S, E, R)
#else
#define STRTOUL(S, E, R)	strtoul(S, E, R)
#endif
#endif

/*
 * These two aren't ANSI functions, but their ANSI equivilents have a
 * serious design flaw: there's no way to specify the length of the
 * output buffer, so careless use of the ANSI function can ruin your
 * whole day.  Get a clue, ANSI.
 *
 * We provide implementations of these in the common library.
 */

#ifndef SNPRINTF
#if INSTALL_COMMON_SNPRINTF
#define SNPRINTF		etc_snprintf
#else
#define SNPRINTF		snprintf
#endif
#endif

#ifndef VSNPRINTF
#if INSTALL_COMMON_SNPRINTF
#define VSNPRINTF		etc_vsnprintf
#else
#define VSNPRINTF		vsnprintf
#endif
#endif

/*
 * These two aren't ANSI functions, but most systems have equivilents.
 * They're case-insensitive versions of the corresponding ANSI functions.
 * We provide implementations of these in the common library.  */

#ifndef STRICMP
#if (INSTALL_COMMON_STRICMP)
#define	STRICMP(x, y)		etc_stricmp(x, y)
#else
#define	STRICMP(x, y)		strcasecmp(x, y)
#endif
#endif

#ifndef STRNICMP
#if (INSTALL_COMMON_STRNICMP)
#define	STRNICMP(x, y, z)	etc_strnicmp(x, y, z)
#else
#define	STRNICMP(x, y, z)	strncasecmp(x, y, z)
#endif
#endif

#endif /* COMMON_GLUE_H */

