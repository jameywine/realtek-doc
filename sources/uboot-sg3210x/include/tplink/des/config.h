/* $Header: /usr/cvsroot/target/h/wrn/wm/common/config.h,v 1.3 2003/01/16 18:20:11 josh Exp $ */


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
 * $Log: config.h,v $
 * Revision 1.3  2003/01/16 18:20:11  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:09  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.9  2001/01/19 22:21:27  paul
 * Update copyright.
 *
 * Revision 1.8  2000/03/17 00:16:35  meister
 * Update copyright message
 *
 * Revision 1.7  1998/02/25 04:43:25  sra
 * Update copyrights.
 *
 * Revision 1.6  1998/02/05 22:37:35  josh
 * Changing references to types.h so we include common/h/types.h instead
 *
 * Revision 1.5  1997/08/21 17:23:42  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 1.4  1997/08/14 16:04:38  lowell
 * boilerplate
 *
 */

/* [clearcase]
modification history
-------------------
*/


#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

/*
 * Grab the standard top-level includes.
 */

#ifndef EPILOGUE_INSTALL_H
#include <tplink/des/install.h>
#endif

#ifndef EPILOGUE_TYPES_H
#include <tplink/des/types.h>
#endif

/*
 * Load the preamble portion of the customer's common.h file.
 */

#define CONFIG_PREAMBLE 1
#include <common.h>
#undef CONFIG_PREAMBLE

#endif /* COMMON_CONFIG_H */
