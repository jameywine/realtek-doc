/*
 * this DES implementation derived from Phil Karns public domain DES.
 * thanks, Phil.
 */

/* 
 * $Log: des_key.c,v $
 * Revision 1.3  2003/01/16 18:18:53  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/06 22:35:34  tneale
 * Fixed for newest file structure
 *
 * Revision 1.1.1.1  2001/11/05 17:48:38  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:21:26  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:16:22  meister
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


/* 
 * Portable C code to create DES key schedules from user-provided keys
 */

#include <tplink/des/config.h>
#include <tplink/des/des.h>
#include <tplink/des/glue.h>

/* Key schedule-related tables from FIPS-46 */

/* permuted choice table (key) */
static bits8_t pc1[] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,

	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

/* number left rotations of pc1 */
static bits8_t totrot[] = {
	1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28
};

/* permuted choice key (table) */
static bits8_t pc2[] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

/* End of DES-defined tables */

/* bit 0 is left-most in byte */
static sbits16_t bytebit[] = {
	0200,0100,040,020,010,04,02,01
};


/* Generate key schedule for encryption or decryption
 * depending on the value of "decrypt"
 */

/* 0 = encrypt, 1 = decrypt */
void deskey (DES_KS k, bits8_t *key, int decrypt)
{
	bits8_t pc1m[56];		/* place to modify pc1 into */
	bits8_t pcr[56];		/* place to rotate pc1 into */
	register int i,j,l;
	int m;
	bits8_t ks[8];

	for (j=0; j<56; j++) {		/* convert pc1 to bits of key */
		l=pc1[j]-1;		/* integer bit location	 */
		m = l & 07;		/* find bit		 */
		pc1m[j]=(key[l>>3] &	/* find which key byte l is in */
			bytebit[m])	/* and which bit of that byte */
			? 1 : 0;	/* and store 1-bit result */
	}
	for (i=0; i<16; i++) {		/* key chunk for each iteration */
		MEMSET(ks, 0, sizeof(ks));	/* Clear key schedule */
		for (j=0; j<56; j++)	/* rotate pc1 the right amount */
			pcr[j] = pc1m[(l=j+totrot[decrypt? 15-i : i])<(j<28? 28 : 56) ? l: l-28];
			/* rotate left and right halves independently */
		for (j=0; j<48; j++){	/* select bits individually */
			/* check bit that goes to ks[j] */
			if (pcr[pc2[j]-1]){
				/* mask it in if it's there */
				l= j % 6;
				ks[j/6] |= bytebit[l] >> 2;
			}
		}
		/* Now convert to packed odd/even interleaved form */
		k[i][0] = ((sbits32_t)ks[0] << 24)
		 | ((sbits32_t)ks[2] << 16)
		 | ((sbits32_t)ks[4] << 8)
		 | ((sbits32_t)ks[6]);
		k[i][1] = ((sbits32_t)ks[1] << 24)
		 | ((sbits32_t)ks[3] << 16)
		 | ((sbits32_t)ks[5] << 8)
		 | ((sbits32_t)ks[7]);
	}
}

#if INSTALL_COMMON_3DES

/* Generate key schedule for triple DES in E-D-E (or D-E-D) mode.
 *
 * The key argument is taken to be 24 bytes. The first 8 bytes are K1
 * for the first stage, the second 8 bytes are K2 for the middle stage
 * and the third 8 bytes are K3 for the last stage.
 *
 *    DES3_KS k         key schedule to be initialized
 *    bits8_t *key	192 bits (24 bytes) of key (will use only 168) 
 *    int decrypt 	0 = encrypt, 1 = decrypt 
 */
void des3key (DES3_KS k, bits8_t *key, int decrypt)
{
	if (!decrypt)
        {   deskey(&k[0],&key[0],0);
            deskey(&k[16],&key[8],1);
	    deskey(&k[32],&key[16],0);
	} 
        else 
        {   deskey(&k[32],&key[0],1);
            deskey(&k[16],&key[8],0);
            deskey(&k[0],&key[16],1);
	}
}

#endif /* INSTALL_COMMON_3DES */

