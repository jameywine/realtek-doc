/* $Header: /usr/cvsroot/target/h/wrn/wm/common/types.h,v 1.2 2003/01/16 18:20:14 josh Exp $ */

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
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: types.h,v $
 * Revision 1.2  2003/01/16 18:20:14  josh
 * directory structure shifting
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.9  2001/01/19 22:21:29  paul
 * Update copyright.
 *
 * Revision 1.8  2000/03/17 00:16:42  meister
 * Update copyright message
 *
 * Revision 1.7  2000/03/13 21:21:46  paul
 * Removed some code that we are no longer working on.
 *
 * Revision 1.4  1999/05/12 21:41:33  niqbal
 * Changes for Redefinition protection
 *
 * Revision 1.3  1998/03/11 22:40:02  josh
 * epilogue_char_t is now a char instead of bits8_t
 *
 * Revision 1.2  1998/02/25 04:43:27  sra
 * Update copyrights.
 *
 * Revision 1.1  1998/02/05 22:19:28  josh
 * moving commonly used include files into common/h from top-level directory
 *
 * Revision 1.9  1997/04/20 01:32:00  alan
 * Add ldb_int{8,16,32}_t types for the benefit of layout/ldbglue.h.
 *
 * Revision 1.8  1997/03/20 06:58:18  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.7  1997/02/25 10:57:03  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 1.6  1996/10/28 22:52:57  sar
 * correction added epilogue_size_t and epilogue_char_t
 *
 * Revision 1.5  1996/10/28  22:51:08  sar
 * Added epilogue_types_h epilogue_char_h
 *
 * Revision 1.4  1993/04/21  15:39:23  dab
 * No longer have cookie_t and renamed cardinal_t to fastint_t.
 *
 * Revision 1.3  1993/03/20  19:09:52  dab
 * changed pvoid_t to ptr_t
 *
 * Revision 1.2  1993/02/26  16:54:21  dab
 * Changed htypes.h to porttype.h
 *
 * Revision 1.1  1993/02/19  22:28:01  dab
 * Initial revision
 *
 */


/* [clearcase]
modification history
-------------------
*/


#ifndef EPILOGUE_TYPES_H
#define EPILOGUE_TYPES_H

/* <porttype.h> is defined in the hardware specific part of the port.
 * It must supply a typedef for size_t since we can't do that one
 * portably.  It can override any typedef here by defining
 * __TYPES_HAVE_<type-name>_t_.
 */
//#include <oemtypes.h>

/* Internal data types
 */
#ifndef __TYPES_HAVE_bits8_t_
#define __TYPES_HAVE_bits8_t_ 1
typedef unsigned char   bits8_t; /* 8  bits */
#endif
#ifndef __TYPES_HAVE_bits16_t_
#define __TYPES_HAVE_bits16_t_ 1
typedef unsigned short  bits16_t; /* 16 bits */
#endif
#ifndef __TYPES_HAVE_bits32_t_
#define __TYPES_HAVE_bits32_t_ 1
typedef unsigned long   bits32_t; /* 32 bits */
#endif

#ifndef __TYPES_HAVE_sbits8_t_
#define __TYPES_HAVE_sbits8_t_ 1
typedef signed char     sbits8_t; /* signed 8 bits */
#endif
#ifndef __TYPES_HAVE_sbits16_t_
#define __TYPES_HAVE_sbits16_t_ 1
typedef signed short    sbits16_t; /* signed 16 bits */
#endif
#ifndef __TYPES_HAVE_sbits32_t_
#define __TYPES_HAVE_sbits32_t_ 1
typedef signed long     sbits32_t; /* signed 32 bits */
#endif

#ifndef __TYPES_HAVE_inaddr_t_
#define __TYPES_HAVE_inaddr_t_ 1
typedef bits32_t        inaddr_t; /* IP address */
#endif

#ifndef __TYPES_HAVE_ptr_t_
#define __TYPES_HAVE_ptr_t_ 1
typedef void *          ptr_t;  /* generic pointer */
#endif

#ifndef __TYPES_HAVE_fastint_t_
#define __TYPES_HAVE_fastint_t_ 1
typedef int             fastint_t; /* a loop counter (should be at least 15 bits) */
#endif
#ifndef __TYPES_HAVE_boolean_t_
#define __TYPES_HAVE_boolean_t_ 1
typedef int             boolean_t; /* an efficient boolean variable */
#endif

/* These next three types are needed by the layout glue macros.
 * These are the unsigned integer types that your C implementation
 * normally converts 8, 16 and 32 bit quantities into in expressions.
 * The following typedefs are correct for most C implementations
 * (with 16 or 32 bit integers), but may need to be changed in sufficiently
 * unusual environments.
 */
#ifndef __TYPES_HAVE_ldb_int8_t_
typedef unsigned int ldb_int8_t;        /* 8  bits */
#endif
#ifndef __TYPES_HAVE_ldb_int16_t_
typedef unsigned int ldb_int16_t;       /* 16 bits */
#endif
#ifndef __TYPES_HAVE_ldb_int32_t_
typedef unsigned long ldb_int32_t;      /* 32 bits */
#endif

/* yes, we know these two types are not what they should be
   they are here to make some of the code happy and will be
   updated when that code is fixed. */
#ifndef __TYPES_HAVE_epilogue_size_t_
typedef sbits16_t epilogue_size_t;
#endif
#ifndef __TYPES_HAVE_epilogue_char_t_
typedef char epilogue_char_t;
#endif 

#if !defined(NO_PP)
#define __(x) x
#else
#define __(x) ()
#endif

#endif /* EPILOGUE_TYPES_H */
