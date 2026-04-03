/* $Header: /usr/cvsroot/target/h/wrn/wm/common/snprintf.h,v 1.3 2003/01/16 18:20:13 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1988-1997 Epilogue Technology Corporation.
 *  Copyright 1999 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snprintf.h,v $
 * Revision 1.3  2003/01/16 18:20:13  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:11  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:21:29  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/17 00:16:40  meister
 * Update copyright message
 *
 * Revision 1.3  2000/03/09 17:15:42  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.2  1999/05/21 14:46:33  sra
 * Include <stdarg.h>.
 *
 * Revision 1.1  1999/05/20 20:28:14  sra
 * Add etc_snprintf() and etc_vsnprintf().
 *
 */

/* [clearcase]
modification history
-------------------
*/

#ifndef COMMON_SNPRINTF_H

#ifndef EPILOGUE_TYPES_H
#ifdef __cplusplus
extern"C" {
#endif

#include <tplink/des/types.h>
#endif

/*
 * If va_arg exists, it's a macro (can't be a function).
 */
#ifndef va_arg
#include <stdarg.h>
#endif

int etc_snprintf (char *str, size_t count, const char *fmt, ...);
int etc_vsnprintf (char *str, size_t count, const char *fmt, va_list arg);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_SNPRINTF_H */
