#include<common.h>
#include<vsprintf.h>
#include <tplink/des/des.h>

#if 0
/*!
   \function	des_encode	
   \brief		use the des_cbc algorithm to encry the in_buf data.
   				it need the 8 byte key and the 8 byte vector to encry
   				the source data.it is commended to keep the in_buff
   				the same as out_buff.
   \descrition	
   \param[in]	key:the encry key
   \param[in]	sec_blk:the initial vector that the cbc needed
   \param[in]	in_buf:the source data
   \param[in]	in_len:the length of the source data
   \param[out]	out_buf:contain the data after encry.
   \return		
   \calls		
   \others		
*/

static int
des_encode(bits8_t   *key,
			bits8_t   *sec_blk,
		    bits8_t   *in_buf,
		    bits32_t in_len,
		    bits8_t   *out_buf)
{
	DES_CTX ctx;

	descbc_init_context(&ctx, key, 0);
	descbc_iv_context(&ctx, sec_blk);
	descbc_crypt(&ctx, in_buf, out_buf, in_len);

	return(0);
}
#endif
/*!
   \function	des_decode	
   \brief		use the des_cbc algorithm to decrypt the in_buf data.
   				it need the 8 byte key and the 8 byte vector to decrypt
   				the source data.it is commended to keep the in_buff
   				the same as out_buff.
   \descrition	
   \param[in]	key:the decrypt key
   \param[in]	sec_blk:the initial vector that the cbc needed
   \param[in]	in_buf:the source data
   \param[in]	in_len:the length of the source data
   \param[out]	out_buf:contain the data after encry.
   \return		
   \calls		
   \others		
*/
static int
des_decode(bits8_t   *key,
			bits8_t   *sec_blk,
		    bits8_t   *in_buf,
		    bits32_t  in_len,
		    bits8_t   *out_buf)
{
	DES_CTX ctx;
	
	descbc_init_context(&ctx, key, 1);
	descbc_iv_context(&ctx, sec_blk);
	descbc_crypt(&ctx, in_buf, out_buf, in_len);

	return(0);
}

/*!	
\description	: define the key for the des_cbc.
*/
unsigned char des_key[] = 
{
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07
};

/*!	
\description	: define the vector for the des_cbc
*/
unsigned char des_iv[] = 
{
  0xf5,0x10,0x10,0x73,0x6e,0xfb,0xab,0xb2
};
/*!
   \function	sysDesDecode(unsigned char *in_buf,int in_len,unsigned char *out_buf)	
   \brief		
   \descrition	use the des_cbc the decode the in_buf.
   \param[in]	in_buf:the source data
   \param[in]	in_len:the length of the in_buf
   \param[out]	out_buf:contain the data after decode.
   \return		
   \calls		
   \others		
*/
int sysDesDecode(unsigned char *in_buf,int in_len,unsigned char *out_buf)
{

	if( NULL == in_buf || NULL == out_buf)
	{
		return 0;
	}
	des_decode(des_key,des_iv,in_buf,in_len,out_buf);

	return 1;	
}
