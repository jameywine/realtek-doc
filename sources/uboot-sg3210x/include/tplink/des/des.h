/* $Header: /usr/cvsroot/target/h/wrn/wm/common/des.h,v 1.3 2003/01/16 18:20:11 josh Exp $ */

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
 *  All rights reserved.
 ****************************************************************************/

/*
 * this DES implementation derived from Phil Karns public domain DES.
 */

/* [clearcase]
modification history
-------------------
*/


#ifndef	EPILOGUE_DES_H
#define EPILOGUE_DES_H

#ifdef __cplusplus
extern"C" {
#endif

#ifndef EPILOGUE_TYPES_H
#include <tplink/des/types.h>
#endif
#ifndef EPILOGUE_INSTALL_H
#include <tplink/des/install.h>
#endif

typedef bits32_t DES_KS[16][2];	        /* Single-key DES key schedule */
typedef bits32_t DES3_KS[48][2];	/* Triple-DES key schedule */

typedef struct 
{  int decrypt;
   DES_KS keysched;
   bits8_t iv [8];
}  DES_CTX;

#if INSTALL_COMMON_3DES

typedef struct 
{  int decrypt;
   DES3_KS keysched;
   bits8_t iv [8];
}  DES3_CTX;

#endif /* INSTALL_COMMON_3DES */

#ifndef _EXCLUDE_PROTOTYPES_

int  des_is_real(void);
void deskey   (DES_KS, bits8_t *,int);
void des      (DES_KS, bits8_t *, bits8_t *);
void desecb_block_encrypt (DES_CTX *ctx, bits8_t *in, bits8_t *out);
void descbc_block_decrypt (DES_CTX *ctx, bits8_t *in, bits8_t *out);
void descbc_block_encrypt (DES_CTX *ctx, bits8_t *in, bits8_t *out);
void descbc_init_context (void *ctx, bits8_t *key, int decrypt);
void descbc_iv_context (void *ctx, bits8_t *iv);
void descbc_crypt (void *ctx, bits8_t *in, bits8_t *out, bits32_t n);

#if INSTALL_COMMON_3DES
void des3key  (DES3_KS, bits8_t *,int);
void des3     (DES3_KS, bits8_t *, bits8_t *);
void des3cbc_block_decrypt (DES3_CTX *ctx, bits8_t *in, bits8_t *out);
void des3cbc_block_encrypt (DES3_CTX *ctx, bits8_t *in, bits8_t *out);
void des3cbc_init_context (void *ctx, bits8_t *key, int decrypt);
void des3cbc_iv_context (void *ctx, bits8_t *iv);
void des3cbc_crypt (void *ctx, bits8_t *in, bits8_t *out, bits32_t n);
#endif /* INSTALL_COMMON_3DES */

#endif /* ifndef _EXCLUDE_PROTOTYPES_ */

#ifdef __cplusplus
}
#endif

#endif /* EPILOGUE_DES_H */



