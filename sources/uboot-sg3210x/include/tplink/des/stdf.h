/* $Header: /usr/cvsroot/target/h/wrn/wm/common/stdf.h,v 1.3 2003/01/16 18:20:13 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 2000-2001 Wind River Systems, Inc.
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: stdf.h,v $
 * Revision 1.3  2003/01/16 18:20:13  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:13:33  meister
 * rework pathnames
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/02/09 20:13:33  paul
 * Added our own version of strncpy(), just in case the user doesn't have one.
 *
 * Revision 1.6  2001/01/19 22:21:29  paul
 * Update copyright.
 *
 * Revision 1.5  2000/03/17 00:16:41  meister
 * Update copyright message
 *
 * Revision 1.4  2000/03/09 17:15:42  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.3  1999/04/09 20:33:47  wes
 * re-constify.  let's see what, if anything, blows up
 *
 * Revision 1.2  1998/07/03 20:19:20  sra
 * Add etc_memcmp().
 *
 * Revision 1.1  1998/06/03 20:10:34  sar
 * Added some common string functions.  stdf.h has the externs for them
 * while glue.h will use them if the user installs them and doesn't
 * specify other functions to use.
 *
 */

/* [clearcase]
modification history
-------------------
*/


#ifndef COMMON_STDF_H
#define COMMON_STDF_H

#ifdef __cplusplus
extern"C" {
#endif

/* pick up size_t (via oemtypes.h and sttdef.h) */

#ifndef EPILOGUE_TYPES_H
#include <tplink/des/types.h>
#endif

long          etc_strtol   (char *string, char **endptr, int radix);
unsigned long etc_strtoul  (char *string, char **endptr, int radix);
int           etc_stricmp  (const char *s, const char *t);
int           etc_strnicmp (const char *s, const char *t, size_t len);
int           etc_memcmp   (const void *x, const void *y, size_t len);
char *        etc_strncpy  (char *dst, const char *src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_STDF_H */
