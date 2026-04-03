/*********************************************************************
* Filename:   aes.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding AES implementation.
*********************************************************************/

#ifndef AES_H
#define AES_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define AES_BLOCK_SIZE 16               // AES operates on 16 bytes at a time

/**************************** DATA TYPES ****************************/
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned char BYTE;            // 8-bit byte
typedef unsigned int WORD;             // 32-bit word, change to "long" for 16-bit machines

/*********************** FUNCTION DECLARATIONS **********************/
///////////////////
// AES
///////////////////


/*----------------------------------------------------------------------------
* AES_Encrypt
*
* Wrapper function as required by the SA Builder. Encrypts a single
* data block using AES.
*
* pInput
*     Pointer to a 16 byte buffer with the plain data.
*
* pOutput
*     Pointer to a 16 bytes buffer with received the encrypted data.
*
* pKey
*     Pointer to the encryption key.
*
* nKeyLen
*     The length of the encryption key in bytes.
*     This must be a value if 16, 24 or 32.
*
* Return Value:
*     No return value.
*/
void
AES_Encrypt(
        const uint8_t * const pInput,
        uint8_t * const pOutput,
        const uint8_t * const pKey,
        unsigned int nKeyLen);

#endif   // AES_H