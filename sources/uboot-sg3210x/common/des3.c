/*
 * this DES implementation derived from Phil Karns public domain DES.
 * thanks, Phil.
 */

/* 
 * $Log: des3.c,v $
 * Revision 1.3  2003/01/16 18:18:53  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/06 22:35:33  tneale
 * Fixed for newest file structure
 *
 * Revision 1.1.1.1  2001/11/05 17:48:38  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:21:25  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:16:21  meister
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


#include <tplink/des.h>

/* Combined SP lookup table, linked in
 * For best results, ensure that this is aligned on a 32-bit boundary;
 * Borland C++ 3.1 doesn't guarantee this!
 */
extern bits32_t Spbox[8][64];		/* Combined S and P boxes */

/* Primitive function F.
 * Input is r, subkey array in keys, output is XORed into l.
 * Each round consumes eight 6-bit subkeys, one for
 * each of the 8 S-boxes, 2 longs for each round.
 * Each long contains four 6-bit subkeys, each taking up a byte.
 * The first long contains, from high to low end, the subkeys for
 * S-boxes 1, 3, 5 & 7; the second contains the subkeys for S-boxes
 * 2, 4, 6 & 8 (using the origin-1 S-box numbering in the standard,
 * not the origin-0 numbering used elsewhere in this code)
 * See comments elsewhere about the pre-rotated values of r and Spbox.
 */
#define	F(l,r,key){\
	work = ((r >> 4) | (r << 28)) ^ key[0];\
	l ^= Spbox[6][(int) (work & 0x3f)];\
	l ^= Spbox[4][(int) ((work >> 8) & 0x3f)];\
	l ^= Spbox[2][(int) ((work >> 16) & 0x3f)];\
	l ^= Spbox[0][(int) ((work >> 24) & 0x3f)];\
	work = r ^ key[1];\
	l ^= Spbox[7][(int) (work & 0x3f)];\
	l ^= Spbox[5][(int) ((work >> 8) & 0x3f)];\
	l ^= Spbox[3][(int) ((work >> 16) & 0x3f)];\
	l ^= Spbox[1][(int) ((work >> 24) & 0x3f)];\
}
/* Encrypt or decrypt a block of data in ECB mode */
void des3(DES3_KS ks, bits8_t *block, bits8_t *out)
{
	bits32_t left,right,work;
	
	/* Read input block and place in left/right in big-endian order */
	left = ((bits32_t)block[0] << 24)
	 | ((bits32_t)block[1] << 16)
	 | ((bits32_t)block[2] << 8)
	 | (bits32_t)block[3];
	right = ((bits32_t)block[4] << 24)
	 | ((bits32_t)block[5] << 16)
	 | ((bits32_t)block[6] << 8)
	 | (bits32_t)block[7];

	/* Hoey's clever initial permutation algorithm, from Outerbridge
	 * (see Schneier p 478)	
	 *
	 * The convention here is the same as Outerbridge: rotate each
	 * register left by 1 bit, i.e., so that "left" contains permuted
	 * input bits 2, 3, 4, ... 1 and "right" contains 33, 34, 35, ... 32	
	 * (using origin-1 numbering as in the FIPS). This allows us to avoid
	 * one of the two rotates that would otherwise be required in each of
	 * the 16 rounds.
	 */
	work = ((left >> 4) ^ right) & 0x0f0f0f0fL;
	right ^= work;
	left ^= work << 4;
	work = ((left >> 16) ^ right) & 0xffff;
	right ^= work;
	left ^= work << 16;
	work = ((right >> 2) ^ left) & 0x33333333L;
	left ^= work;
	right ^= (work << 2);
	work = ((right >> 8) ^ left) & 0xff00ffL;
	left ^= work;
	right ^= (work << 8);
	right = (right << 1) | (right >> 31);
	work = (left ^ right) & 0xaaaaaaaaL;
	left ^= work;
	right ^= work;
	left = (left << 1) | (left >> 31);

	/* First key */
	F(left,right,ks[0]);
	F(right,left,ks[1]);
	F(left,right,ks[2]);
	F(right,left,ks[3]);
	F(left,right,ks[4]);
	F(right,left,ks[5]);
	F(left,right,ks[6]);
	F(right,left,ks[7]);
	F(left,right,ks[8]);
	F(right,left,ks[9]);
	F(left,right,ks[10]);
	F(right,left,ks[11]);
	F(left,right,ks[12]);
	F(right,left,ks[13]);
	F(left,right,ks[14]);
	F(right,left,ks[15]);

	/* Second key (must be created in opposite mode to first key) */
	F(right,left,ks[16]);
	F(left,right,ks[17]);
	F(right,left,ks[18]);
	F(left,right,ks[19]);
	F(right,left,ks[20]);
	F(left,right,ks[21]);
	F(right,left,ks[22]);
	F(left,right,ks[23]);
	F(right,left,ks[24]);
	F(left,right,ks[25]);
	F(right,left,ks[26]);
	F(left,right,ks[27]);
	F(right,left,ks[28]);
	F(left,right,ks[29]);
	F(right,left,ks[30]);
	F(left,right,ks[31]);

	/* First key again */
	F(left,right,ks[32]);
	F(right,left,ks[33]);
	F(left,right,ks[34]);
	F(right,left,ks[35]);
	F(left,right,ks[36]);
	F(right,left,ks[37]);
	F(left,right,ks[38]);
	F(right,left,ks[39]);
	F(left,right,ks[40]);
	F(right,left,ks[41]);
	F(left,right,ks[42]);
	F(right,left,ks[43]);
	F(left,right,ks[44]);
	F(right,left,ks[45]);
	F(left,right,ks[46]);
	F(right,left,ks[47]);

	/* Inverse permutation, also from Hoey via Outerbridge and Schneier */
	right = (right << 31) | (right >> 1);
	work = (left ^ right) & 0xaaaaaaaaL;
	left ^= work;

	right ^= work;
	left = (left >> 1) | (left  << 31);
	work = ((left >> 8) ^ right) & 0xff00ffL;
	right ^= work;
	left ^= work << 8;
	work = ((left >> 2) ^ right) & 0x33333333L;
	right ^= work;
	left ^= work << 2;
	work = ((right >> 16) ^ left) & 0xffffL;
	left ^= work;
	right ^= work << 16;
	work = ((right >> 4) ^ left) & 0x0f0f0f0fL;
	left ^= work;
	right ^= work << 4;

	/* Put the block back into the user's buffer with final swap */
	out[0] = (bits8_t) (right >> 24);
	out[1] = (bits8_t) (right >> 16);
	out[2] = (bits8_t) (right >> 8);
	out[3] = (bits8_t) (right);
	out[4] = (bits8_t) (left >> 24);
	out[5] = (bits8_t) (left >> 16);
	out[6] = (bits8_t) (left >> 8);
	out[7] = (bits8_t) (left);
}






