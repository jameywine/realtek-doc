/*
 * different modes for DES use. ECB, CBC
 */

/*
 * $Log: desmodes.c,v $
 * Revision 1.3  2003/01/16 18:18:54  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/06 22:35:34  tneale
 * Fixed for newest file structure
 *
 * Revision 1.1.1.1  2001/11/05 17:48:39  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:21:26  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:16:23  meister
 * Update copyright message
 *
 * Revision 1.1  1998/06/24 13:29:12  josh
 * DES files have been moved to new directory to handle export
 * restrictions more cleanly
 *
 */

/* [clearcase]
modification history
-------------------
01a,11dec03,job  fix copyright statements
*/


#include <tplink/des/config.h>
#include <tplink/des/bug.h>
#include <tplink/des/des.h>
#include <tplink/des/glue.h>

void desecb_block_encrypt (DES_CTX *ctx, bits8_t *in, bits8_t *out)
{    des (ctx->keysched, in, out);
}     

void descbc_block_encrypt (DES_CTX *ctx, bits8_t *in, bits8_t *out)
{
     int i = 8;
     bits8_t *cp = ctx->iv;
     bits8_t *inp = in;

#ifdef IPSEC_DES_DEBUG
     PRINTF ("descbc_block_encrypt: input %2x %2x %2x %2x %2x %2x %2x %2x\n", 
             (unsigned char) in [0], (unsigned char) in [1],              
             (unsigned char) in [2], (unsigned char) in [3],
             (unsigned char) in [4], (unsigned char) in [5],
             (unsigned char) in [6], (unsigned char) in [7]);
#endif
     while (i--)
            *cp++ ^= *inp++;

#ifdef IPSEC_DES_DEBUG
     PRINTF ("descbc_block_encrypt: xiv %2x %2x %2x %2x %2x %2x %2x %2x\n", 
             (unsigned char) ctx->iv [0], (unsigned char) ctx->iv [1],     
             (unsigned char) ctx->iv [2], (unsigned char) ctx->iv [3],
             (unsigned char) ctx->iv [4], (unsigned char) ctx->iv [5],
             (unsigned char) ctx->iv [6], (unsigned char) ctx->iv [7]);
#endif

     des (ctx->keysched, ctx->iv, out);

#ifdef IPSEC_DES_DEBUG
     PRINTF ("descbc_block_encrypt: out %2x %2x %2x %2x %2x %2x %2x %2x\n", 
             (unsigned char) out [0], (unsigned char) out [1],     
             (unsigned char) out [2], (unsigned char) out [3],
             (unsigned char) out [4], (unsigned char) out [5],
             (unsigned char) out [6], (unsigned char) out [7]);
#endif
     MEMCPY(ctx->iv, out, 8);
}                 

void descbc_block_decrypt (DES_CTX *ctx, bits8_t *in, bits8_t *out)
{
     int i = 8;

     bits8_t *cp = ctx->iv;      /* == C[i-1] from last iteration, or IV */
     bits8_t *outp = out;
     bits8_t nextiv [8];

     /* save current input C[i] to be next blocks IV */

     MEMCPY(nextiv, in, 8);

     des (ctx->keysched, in, out);

     while (i--)
            *outp++ ^= *cp++;       /* P[i] = D(C[i]) XOR C[i-1]    */
     MEMCPY(ctx->iv, nextiv, 8);    /* save C[i] for next iteration */
}                 

#if INSTALL_COMMON_ESP_3DES
void des3ecb_block_encrypt (DES3_CTX *ctx, bits8_t *in, bits8_t *out)
{    des3 (ctx->keysched, in, out);
}     

void des3cbc_block_encrypt (DES3_CTX *ctx, bits8_t *in, bits8_t *out)
{
     int i = 8;
     bits8_t *cp = ctx->iv;
     bits8_t *inp = in;
     while (i--)
            *cp++ ^= *inp++;

     des3 (ctx->keysched, ctx->iv, out);
     MEMCPY(ctx->iv, out, 8);
}                 

void des3cbc_block_decrypt (DES3_CTX *ctx, bits8_t *in, bits8_t *out)
{
     int i = 8;

     bits8_t *cp = ctx->iv;      /* == C[i-1] from last iteration, or IV */
     bits8_t *outp = out;

     des3 (ctx->keysched, in, out);

     while (i--)
            *outp++ ^= *cp++;   /* P[i] = D(C[i]) XOR C[i-1]    */
     MEMCPY(ctx->iv, in, 8);    /* save C[i] for next iteration */
}                 

#endif /* INSTALL_COMMON_ESP_3DES */

/* algtable context glue */

void descbc_init_context (void *ctx, bits8_t *key, int decrypt)
{    
     deskey ( ((DES_CTX *) ctx)->keysched, key, decrypt);
     ((DES_CTX *) ctx)->decrypt = decrypt;
}

void descbc_iv_context (void *ctx, bits8_t *iv)
{
     MEMCPY( ((DES_CTX *)ctx)->iv, iv, 8);
}

void descbc_crypt (void *ctx, bits8_t *in, bits8_t *out, bits32_t n)
{
     int decrypt = ((DES_CTX *) ctx)->decrypt;

     while (n >= 8)
     {      if (decrypt) 
                descbc_block_decrypt (((DES_CTX *) ctx), in, out);
            else
                descbc_block_encrypt (((DES_CTX *) ctx), in, out);
            in += 8; 
            out += 8;
            n -= 8;
     }
}

#if INSTALL_COMMON_ESP_3DES

void des3cbc_init_context (void *ctx, bits8_t *key, int decrypt)
{    
     des3key ( ((DES3_CTX *) ctx)->keysched, key, decrypt);
     ((DES3_CTX *) ctx)->decrypt = decrypt;
}

void des3cbc_iv_context (void *ctx, bits8_t *iv)
{
     MEMCPY( ((DES3_CTX *)ctx)->iv, iv, 8);
}

void des3cbc_crypt (void *ctx, bits8_t *in, bits8_t *out, bits32_t n)
{
     int decrypt = ((DES3_CTX *) ctx)->decrypt;

     while (n >= 8)
     {      if (decrypt) 
                des3cbc_block_decrypt (((DES3_CTX *) ctx), in, out);
            else
                des3cbc_block_encrypt (((DES3_CTX *) ctx), in, out);
            in += 8; 
            out += 8;
            n -= 8;
     }
}

#endif /* INSTALL_COMMON_ESP_3DES */
     






