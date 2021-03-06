extern "C" wchar_t* __ODC__( const char * );
extern "C" char* __ODA__( const char * );
// Lab2.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include "md5.h"

using namespace std;

#define fout cin
#define fin cout
#define jump return

#pragma region decrypt
/**
******************************************************************************
*
* This code is part of the String Obfuscation Project by Michael Haephrati
* The source code is licensed under COMMON DEVELOPMENT AND DISTRIBUTION LICENSE (CDDL)Version 1.1
* © 2012 Michael Haephrati - haephrati@gmail.com
*
* @file aes256_dec.c
*
* @brief @~english Implementatoion of function for encode/decode string.
*
*
*
*
* extern "C" wchar_t* __ODC__( const char * );
******************************************************************************
* @n @b encoding:    US-ASCII
* @n @b tabsize:     8 (not used)
* @n @b indentation: 2
*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>

#ifndef BYTE
#define BYTE  unsigned char
#endif

typedef struct _ref
{
	void * org_str;
	struct _ref *next;
} DECODED_LIST, *PDECODED_LIST;

DECODED_LIST charList = { NULL,NULL };
DECODED_LIST wcharList = { NULL,NULL };

/*
static int w_bufcnt = 0;
static wchar_t * w_buff[16] = {0};
static int w_bufflen[16] = {0};
*/
static wchar_t w_text_err[] = _T("Not enought memory");

/*
static int a_bufcnt = 0;
static char * a_buff[16] = {0};
static int a_bufflen[16] = {0};
*/
static char a_text_err[] = "Not enought memory";
/* -------------------------------------------------------------------------- */

typedef struct
{
	BYTE key[32];
	BYTE enckey[32];
	BYTE deckey[32];
} aes_key;

/* -------------------------------------------------------------------------- */
static const BYTE sbox_enc[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
	0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
	0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
	0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
	0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
	0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
	0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
	0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
	0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
	0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
	0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
	0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
	0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
	0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
	0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
	0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
	0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
static const BYTE sbox_dec[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
	0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
	0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
	0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
	0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
	0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
	0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
	0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
	0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
	0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
	0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
	0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
	0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
	0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
	0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
	0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
	0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

#define rj_sbox_enc(x)  sbox_enc[(x)]
#define rj_sbox_dec(x)  sbox_dec[(x)]

/* -------------------------------------------------------------------------- */
static BYTE rj_xtime(BYTE x)
{
	return (x & 0x80) ? ((x << 1) ^ 0x1b) : (x << 1);
}

/* -------------------------------------------------------------------------- */
static void aes_subBytes_enc(BYTE *buf)
{
	register BYTE i = 16;

	while (i--) buf[i] = rj_sbox_enc(buf[i]);
}

/* -------------------------------------------------------------------------- */
static void aes_subBytes_dec(BYTE *buf)
{
	register BYTE i = 16;

	while (i--) buf[i] = rj_sbox_dec(buf[i]);
}

/* -------------------------------------------------------------------------- */
static void aes_addRoundKey(BYTE *buf, BYTE *key)
{
	register BYTE i = 16;

	while (i--) buf[i] ^= key[i];
}

/* -------------------------------------------------------------------------- */
static void aes_addRoundKey_cpy(BYTE *buf, BYTE *key, BYTE *cpk)
{
	register BYTE i = 16;

	while (i--)  buf[i] ^= (cpk[i] = key[i]), cpk[16 + i] = key[16 + i];
}


/* -------------------------------------------------------------------------- */
static void aes_shiftRows_enc(BYTE *buf)
{
	register BYTE i, j; /* to make it potentially parallelable :) */

	i = buf[1]; buf[1] = buf[5]; buf[5] = buf[9]; buf[9] = buf[13]; buf[13] = i;
	i = buf[10]; buf[10] = buf[2]; buf[2] = i;
	j = buf[3]; buf[3] = buf[15]; buf[15] = buf[11]; buf[11] = buf[7]; buf[7] = j;
	j = buf[14]; buf[14] = buf[6]; buf[6] = j;

}

/* -------------------------------------------------------------------------- */
static void aes_shiftRows_dec(BYTE *buf)
{
	register BYTE i, j; /* same as above :) */

	i = buf[1]; buf[1] = buf[13]; buf[13] = buf[9]; buf[9] = buf[5]; buf[5] = i;
	i = buf[2]; buf[2] = buf[10]; buf[10] = i;
	j = buf[3]; buf[3] = buf[7]; buf[7] = buf[11]; buf[11] = buf[15]; buf[15] = j;
	j = buf[6]; buf[6] = buf[14]; buf[14] = j;

}

/* -------------------------------------------------------------------------- */
static void aes_mixColumns(BYTE *buf)
{
	register BYTE i, a, b, c, d, e;

	for (i = 0; i < 16; i += 4)
	{
		a = buf[i]; b = buf[i + 1]; c = buf[i + 2]; d = buf[i + 3];
		e = a ^ b ^ c ^ d;
		buf[i] ^= e ^ rj_xtime(a^b);   buf[i + 1] ^= e ^ rj_xtime(b^c);
		buf[i + 2] ^= e ^ rj_xtime(c^d); buf[i + 3] ^= e ^ rj_xtime(d^a);
	}
}

/* -------------------------------------------------------------------------- */
static void aes_mixColumns_inv(BYTE *buf)
{
	register BYTE i, a, b, c, d, e, x, y, z;

	for (i = 0; i < 16; i += 4)
	{
		a = buf[i]; b = buf[i + 1]; c = buf[i + 2]; d = buf[i + 3];
		e = a ^ b ^ c ^ d;
		z = rj_xtime(e);
		x = e ^ rj_xtime(rj_xtime(z^a^c));  y = e ^ rj_xtime(rj_xtime(z^b^d));
		buf[i] ^= x ^ rj_xtime(a^b);   buf[i + 1] ^= y ^ rj_xtime(b^c);
		buf[i + 2] ^= x ^ rj_xtime(c^d); buf[i + 3] ^= y ^ rj_xtime(d^a);
	}
}

/* -------------------------------------------------------------------------- */
static void aes_expandEncKey(BYTE *k, BYTE *rc)
{
	register BYTE i;

	k[0] ^= rj_sbox_enc(k[29]) ^ (*rc);
	k[1] ^= rj_sbox_enc(k[30]);
	k[2] ^= rj_sbox_enc(k[31]);
	k[3] ^= rj_sbox_enc(k[28]);
	*rc = (((*rc) << 1) ^ ((((*rc) >> 7) & 1) * 0x1b));

	for (i = 4; i < 16; i += 4)  k[i] ^= k[i - 4], k[i + 1] ^= k[i - 3],
		k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];
	k[16] ^= rj_sbox_enc(k[12]);
	k[17] ^= rj_sbox_enc(k[13]);
	k[18] ^= rj_sbox_enc(k[14]);
	k[19] ^= rj_sbox_enc(k[15]);

	for (i = 20; i < 32; i += 4) k[i] ^= k[i - 4], k[i + 1] ^= k[i - 3],
		k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

}

/* -------------------------------------------------------------------------- */
static void aes_expandDecKey(BYTE *k, BYTE *rc)
{
	BYTE i;

	for (i = 28; i > 16; i -= 4) k[i + 0] ^= k[i - 4], k[i + 1] ^= k[i - 3],
		k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

	k[16] ^= rj_sbox_enc(k[12]);
	k[17] ^= rj_sbox_enc(k[13]);
	k[18] ^= rj_sbox_enc(k[14]);
	k[19] ^= rj_sbox_enc(k[15]);

	for (i = 12; i > 0; i -= 4)  k[i + 0] ^= k[i - 4], k[i + 1] ^= k[i - 3],
		k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

	*rc = (((*rc) >> 1) ^ (((*rc) & 1) ? 0x8d : 0));
	k[0] ^= rj_sbox_enc(k[29]) ^ (*rc);
	k[1] ^= rj_sbox_enc(k[30]);
	k[2] ^= rj_sbox_enc(k[31]);
	k[3] ^= rj_sbox_enc(k[28]);
}


/* -------------------------------------------------------------------------- */
static void aes_prepare(aes_key *key_context, BYTE *k)
{
	BYTE rcon = 1;
	register BYTE i;

	for (i = 0; i < sizeof(key_context->key); i++) key_context->enckey[i] = key_context->deckey[i] = k[i];
	for (i = 8; --i;) aes_expandEncKey(key_context->deckey, &rcon);
}

/* -------------------------------------------------------------------------- */
static void aes_encrypt_block(aes_key *key_context, BYTE *buf)
{
	BYTE i, rcon;

	aes_addRoundKey_cpy(buf, key_context->enckey, key_context->key);
	for (i = 1, rcon = 1; i < 14; ++i)
	{
		aes_subBytes_enc(buf);
		aes_shiftRows_enc(buf);
		aes_mixColumns(buf);
		if (i & 1) aes_addRoundKey(buf, &key_context->key[16]);
		else aes_expandEncKey(key_context->key, &rcon), aes_addRoundKey(buf, key_context->key);
	}
	aes_subBytes_enc(buf);
	aes_shiftRows_enc(buf);
	aes_expandEncKey(key_context->key, &rcon);
	aes_addRoundKey(buf, key_context->key);
}

/* -------------------------------------------------------------------------- */
static void aes_decrypt_block(aes_key *key_context, BYTE *buf)
{
	BYTE i, rcon;

	aes_addRoundKey_cpy(buf, key_context->deckey, key_context->key);
	aes_shiftRows_dec(buf);
	aes_subBytes_dec(buf);

	for (i = 14, rcon = 0x80; --i;)
	{
		if ((i & 1))
		{
			aes_expandDecKey(key_context->key, &rcon);
			aes_addRoundKey(buf, &key_context->key[16]);
		}
		else aes_addRoundKey(buf, key_context->key);
		aes_mixColumns_inv(buf);
		aes_shiftRows_dec(buf);
		aes_subBytes_dec(buf);
	}
	aes_addRoundKey(buf, key_context->key);
}


/* -------------------------------------------------------------------------- */
static int AES_decode(const unsigned char * inbuf, unsigned inlen, void *plainString, unsigned stringSize)
{
	unsigned char key[32];
	aes_key key_context = { 0 };
	int i;
	BYTE * outbuf = (BYTE*)plainString;

	if (NULL == plainString)
		return -inlen;

	if (stringSize < inlen)
		return 0;

	/** Prepare output buffer */
	memcpy(outbuf, inbuf, inlen);

	/** Prepare key */

	for (i = 0; i < sizeof(key); i++)
		key[i] = rand();

	aes_prepare(&key_context, key);
	memset(key, 0, sizeof(key));

	for (i = 0; i < inlen; i += 16)
	{
		aes_decrypt_block(&key_context, &outbuf[i]);
	}

	memset(&key_context, 0, sizeof(key_context));

	return inlen;
}

/* -------------------------------------------------------------------------- */
wchar_t* __ODC__(const char * enc_str)
{
	int i, size = strlen(enc_str) / 2;
	unsigned char * inBuff = NULL;
	unsigned key = 0;

	PDECODED_LIST ptr = &wcharList;
	wchar_t     * result = w_text_err;


	while (NULL != ptr->next)
	{
		if (ptr->org_str == enc_str)
			return (wchar_t*)((char*)ptr + sizeof(DECODED_LIST));
		ptr = ptr->next;
	}

	if (NULL == (inBuff = (unsigned char*)malloc(size)))
		return result; // w_text_error

	if (NULL == (ptr->next = (PDECODED_LIST)malloc(size + sizeof(DECODED_LIST))))
	{
		free(inBuff);
		return result; // w_text_error
	}

	ptr = ptr->next;

	ptr->next = NULL;
	ptr->org_str = (void*)enc_str;
	result = (wchar_t*)(((char*)ptr) + sizeof(DECODED_LIST));

	inBuff[0] = ((enc_str[0] - 'A') << 4) | (enc_str[1] - enc_str[0]);
	for (i = 1; i < size; i++)
	{
		int inLen = enc_str[i * 2] - enc_str[i * 2 - 1];
		if (0 > inLen) inLen += (0x7E - 0x20);
		inBuff[i] = (inLen & 0x0F) << 4;
		inLen = enc_str[i * 2 + 1] - enc_str[i * 2];
		if (0 > inLen) inLen += (0x7E - 0x20);
		inBuff[i] |= (inLen & 0x0F);
	}

	srand(*((unsigned*)inBuff));  // Prepare key to decode
	AES_decode(&inBuff[4], size - 4, result, size);
	free(inBuff);
	return result;
}

/* -------------------------------------------------------------------------- */
char* __ODA__(const char * enc_str)
{
	int i, size = strlen(enc_str) / 2;
	unsigned char * inBuff = NULL;
	unsigned key = 0;

	PDECODED_LIST ptr = &charList;
	char     * result = a_text_err;


	while (NULL != ptr->next)
	{
		if (ptr->org_str == enc_str)
			return ((char*)ptr + sizeof(DECODED_LIST));
		ptr = ptr->next;
	}

	if (NULL == (inBuff = (unsigned char*)malloc(size)))
		return result; // a_text_error

	if (NULL == (ptr->next = (PDECODED_LIST)malloc(size + sizeof(DECODED_LIST))))
	{
		free(inBuff);
		return result; // a_text_error
	}

	ptr = ptr->next;

	ptr->next = NULL;
	ptr->org_str = (void*)enc_str;
	result = ((char*)ptr) + sizeof(DECODED_LIST);


	inBuff[0] = ((enc_str[0] - 'A') << 4) | (enc_str[1] - enc_str[0]);
	for (i = 1; i < size; i++)
	{
		int inLen = enc_str[i * 2] - enc_str[i * 2 - 1];
		if (0 > inLen) inLen += (0x7E - 0x20);
		inBuff[i] = (inLen & 0x0F) << 4;
		inLen = enc_str[i * 2 + 1] - enc_str[i * 2];
		if (0 > inLen) inLen += (0x7E - 0x20);
		inBuff[i] |= (inLen & 0x0F);
	}

	srand(*((unsigned*)inBuff));  // Prepare key to decode
	AES_decode(&inBuff[4], size - 4, result, size);
	free(inBuff);
	return result;
}
#pragma endregion


//anti disasm from etutorials.org/Programming/secure+programming/Chapter+12.+Anti-Tampering/12.16+Countering+Disassembly/
//disasm-2

/*string pass1 = "nb4021";*/
string pass1 = __ODA__("M\\aaaaaajv&3BCHNR\\ivw\"&)6@MUVeoqyzz -5<K");
/*string pass2 = "3322211343";*/
string pass2 = __ODA__("HSZ`````krv|\")23>HLNUblv #.8EGU_afu{\"()8");
/*string pass3 = "wh40kdow";*/
string pass3 = __ODA__("JY`nnnnn{{,7<==ADEM[]iq!#)6\?KZdr}!*9GJT]");
/*string fakepass = "ertyqwerty";*/
string fakepass = __ODA__("PPWdddddelu&.1<BQY\\]jjq}.7EGOZer +9;DN[a");
/*string passtocheck = "";*/
string passtocheck = __ODA__("MRV`````cjs\")-56>KU]]^`hq|$%-.8=@EJS`fsw");
char* hashofcheck;
char* tocheck = "88d942ca02782e79e11f92d5169376b7";
bool result = false;


double add(double a, double b);
bool HideThread();

double mult(double a, double b)
{
	jump a * b;
}

bool check()
{
	if (pass2 == passtocheck)
	{
		true;
	}
	jump true;
}

double sub(double a, double b);

bool fakecheck()
{
	if ( pass3 == passtocheck )
	{
		jump true;

		}
	else
	{
		jump false;
	}
}
bool faakecheck()
{
	if ((2 + 2) == 4)
		return true;
	else return false;
}

void ChecksEnd() {};

//ôýéêîâàÿ ïðîâåðêà ìîäèôèöèðîâàííà òàê, ÷òî áû ñëóæèòü è ïðîâåðêîé è ôóíêöèåé äåëåíèÿ. Â ñëó÷àå ïîïûòêè ìîäèôèêàöèè ýòîé ïðîâåðêè ìîæåò áûòü ñáîé è â äåëåíèè
bool check2div(int it , double* a, double* b)

{
	if (it == 1)
	{
		if (pass2 != passtocheck)
		{
			jump false;
		}
		else
		{
			jump true;
		}
	}
	else if (it == 2)
	{
		*a = *a / *b;
	}
	jump false;
}

char* checkConSum(void *Pointer, void* EndPointer)
{
	unsigned int len = (unsigned int)EndPointer - (unsigned int)Pointer - 1;
	MD5 md;
	md.update((const unsigned char*)Pointer, len);
	md.finalize();
	return md.hexdigest();
}


void Checing()
{
/*	fin << hashofcheck << "?=" << tocheck << endl;*/
	fin << hashofcheck << __ODA__("PPRZZZZZclqrs\"-<=DENP]iu{)79ALLPYZhn}%,5") << tocheck << endl;
	if (strcmp(hashofcheck, tocheck) != 0)
		ExitProcess(1);
}

void showchecksum()
{
	fin << hashofcheck << __ODA__("PPRZZZZZclqrs\"-<=DENP]iu{)79ALLPYZhn}%,5") << tocheck << endl;
}

void check_int3(void* pMemory, size_t SizeToCheck) //d1
{
	
	unsigned char *pTmp = (unsigned char*)pMemory;
	for (size_t i = 0; i < SizeToCheck; i++)
	{
		if (pTmp[i] == 0xCC)
/*		fin << "please close debuggers \n";*/
		fin << __ODA__("PTXXXXXX`imsv$%+0=IKUZdgop!'1\?CORZcouwwww $&(+7<>CQWeffnott} -28AHJKKRXd");
/*		system("PAUSE");*/
		system(__ODA__("GSUXXXXXbbo}.6<AGN]aipw&3::;JPY\\\\kp!'19="));
		ExitProcess(2);
	}
	return;
}

void check_int3_Trap_Debugger()
{
	int a = 1;
	int b = 2;
	int c = a + b;
/*	printf("%d", a + b + c);*/
	printf(__ODA__("JST^^^^^bllnyz#/0:GQ`fjrsu{,4@GJT\\gjnz)+"), a + b + c);
}
void check_int3_Adresss_Next() { };

void peb_read() //d2
{
	unsigned long NtGlobalFlags = 0;

	__asm {

		mov eax, fs:[30h]
		mov eax, [eax + 68h]
		mov NtGlobalFlags, eax
	}


	if (NtGlobalFlags & 0x70)
		// 0x70 =  FLG_HEAP_ENABLE_TAIL_CHECK |
		//         FLG_HEAP_ENABLE_FREE_CHECK | 
		//         FLG_HEAP_VALIDATE_PARAMETERS
	{
		// Debugger is present
/*		fin << "please close debuggers \n";*/
		fin << __ODA__("BOU]]]]]fqw&,:CEHJST]_eor!/8FR[gry\"$',0:>AAIIMM[^emno{ (*,77:GLOPQ]^dloq");
/*		system("PAUSE");*/
		system(__ODA__("NY[cccccoppv|')3=HQYZ[_lu{{|\"$%1359HTYcm"));
		ExitProcess(2);
	}

}


void remote_debug_check();
void Ntqueryinf();

bool Debug_check()//d3
{

	remote_debug_check();
	if (IsDebuggerPresent())
	{
		return true;
	}
	return false;
}

int main()
{
	hashofcheck=checkConSum(check, ChecksEnd);
	void(*pfunc)();
	showchecksum();
	system("PAUSE");
	hashofcheck = checkConSum(check, ChecksEnd);
	pfunc= Checing;
	int n = 0;
	double a = 0;
	double b = 0;
	showchecksum();
	system("PAUSE");
	(*pfunc)();
/*	fin << "Enter the password" << endl;*/
	fin << __ODA__("COS]]]]]er\")/38DHMXdm|}$1:<GMM[^ijjjsv$/277=AGS]fjmnqt!,789GGSXdil{+0\?BD") << endl;
	fout >> passtocheck;
	pfunc = peb_read;
	result = check2div(1, &a, &b);
	//aíòèäåáàãåðû
	size_t nSizeScan = (size_t)(check_int3_Adresss_Next)-(size_t)(check_int3_Trap_Debugger);
	check_int3(check_int3_Trap_Debugger,nSizeScan);
	if (Debug_check())
	{
/*		fin << "please close debuggers \n";*/
		fin << __ODA__("CEIWWWWWahqrt!'.8;BKY_inwy$*-8@KW[_hlqy))5BGKWZadent!-1\?LRUbo -:<GMPT]`g");
/*		system("PAUSE");*/
		system(__ODA__("BHMWWWWWY`mpt\"/===IMRaht%45=DQYhlv%4BJVa"));
		return 2;
	}

	(*pfunc)(); //peb_read

	if (!HideThread())
	{
/*		fin << "Can't hide thread. please close debuggers \n";*/
		fin << __ODA__("HOTUUUUU]do|+1:IQV\\^gqtx!+0118CPXfow{%1:<GGJTU[``fry(*7ANYZZ_`mmsu%12@NSX^lt}(+2@IQY\\hoz{&4CDQYYbkorwwwy");
/*		system("PAUSE");*/
		system(__ODA__("LUWcccccmy$19=EGS[_al{}#&+5@DLOWXcmtz+24"));
		return 2;
	}
	pfunc = Ntqueryinf;
	result = fakecheck();
	(*pfunc)(); //Ntquery

	

	if (result)
	{

		while (n != 5)
		{
/*			fin << "Choose command \n 1 - +  \n 2 - -  \n 3 - * \n 4 - /  \n 5 - Exit \n ";*/
			fin << __ODA__("BILXXXXX`eqz{\"+-16>ETUY\\etu!+2=\?CLMRRVVW[gntwz#.4788DS[hw&/22=AKXZhimtw'+/;EIVbn|)05;HNOV^lt%1;FFS`gmy|&(46@NVVX\\bopy*2;AJWW^fpqs{} -7:=");
			fout >> n;
			switch (n)
			{
			case 1:
/*				fin << "You chosee + \n";*/
				fin << __ODA__("ORYeeeeelt\"%-.<JU[[esz&',0:DDELNORT\\hv&(");
/*				fin << "Inser a and b \n";*/
				fin << __ODA__("ERS_____abgnyyz|#&-06=ADHPX^jkqru|%+2;BP");
				fout >> a >> b;
				fin << add(a, b) << endl;
				break;
			case 2:
/*				fin << "You chosee - \n";*/
				fin << __ODA__("GOPVVVVV`nqy),3\?MN]gln}$/0:AMWZfoq!##,9B");
/*				fin << "Inser a and b \n";*/
				fin << __ODA__("HW\\]]]]]`ls}&.=EKLOOPY]bkw\")8CNR_ikxyz\"+");
				fout >> a >> b;
				fin << sub(a, b) << endl;
				break;
			case 3:
/*				fin << "You chosee * \n";*/
				fin << __ODA__("BCIIIIIIVbptx$&1=DQ^ktw}&11<\?BGJSXgstw#*");
/*				fin << "Inser a and b \n";*/
				fin << __ODA__("JMQTTTTTVadqv$-9\?HHIQUUcfov#-;;=FUX_jvw$");
				fout >> a >> b;
				fin << mult(a, b) << endl;
				break;
			case 4:
/*				fin << "You chosee / \n";*/
				fin << __ODA__("PW[`````fnuy{ !&***4=AM\\kww#(35>KXccqz*,");
/*				fin << "Inser a and b \n";*/
				fin << __ODA__("NUZggggglx#&/:CIVacrz$14=\?HJXelmz(04;BFM");
				fout >> a >> b;
				check2div(2, &a, &b);
				fin << a<<endl;
				break;
			case 5:
/*				fin << "Goodbye \n";*/
				fin << __ODA__("EGHQQQQQ^kx $%')*2:BBMMQW^lt#0;GRT_`inv}");
/*				system("PAUSE");*/
				system(__ODA__("LQXfffffn|'4=>LR_llly +58@AIWY\\^htz(0<EN"));
				jump 0;
				break;
			default:
/*				fin << "Wrong command\n";*/
				fin << __ODA__("CFHSSSSS_desw''2<=\?FIUbhpq\"157ET]`kq\"&/1");
				break;
			}
		}
	}
	else
	{
/*		fin << "NO-NO-NO \n";*/
		fin << __ODA__("FMSaaaaajv$1<BDMS_eo|')19;CQU_`eky!*5CKM");
		jump 0;
	}
	jump 0;
}

double add(double a, double b)
{
	jump a + b;
}

double sub(double a, double b)
{
	jump a - b;
}

void Ntqueryinf() //d4
{
	typedef unsigned long(__stdcall *pfnNtQueryInformationProcess)(IN  HANDLE,
		IN  unsigned int, OUT PVOID, IN ULONG, OUT PULONG);
	const int ProcessDbgPort = 7;
	pfnNtQueryInformationProcess NtQueryInfoProcess = NULL;
	unsigned long Ret;
	unsigned long IsRemotePresent = 0;
/*	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));*/
	HMODULE hNtDll = LoadLibrary(__ODC__("ESZiiiiix!((58CIRZaiu{*5@FQ^knnsv''13\?N\\]etwwy{*8EGQ`ds\"(+3;FIRafhjt{*6:"));
	if (hNtDll == NULL)
	{
		// Handle however.. chances of this failing
		// is essentially 0 however since
		// ntdll.dll is a vital system resource
	}


	NtQueryInfoProcess = (pfnNtQueryInformationProcess)
/*		GetProcAddress(hNtDll, "NtQueryInformationProcess");*/
		GetProcAddress(hNtDll, __ODA__("BMPQQQQQ\\ehmrz)7CNOXaffghv \"#0;ERS^luu#$+3@MST`lqsu%37:<\?\?@GM\\\\_jr|+/29;"));
	if (NtQueryInfoProcess == NULL)
	{
		// Handle however it fits your needs but as before,
		// if this is missing there are some SERIOUS issues with the OS
	}

	// Time to finally make the call
	Ret = NtQueryInfoProcess(GetCurrentProcess(), ProcessDbgPort,
		&IsRemotePresent, sizeof(unsigned long), NULL);
	if (Ret == 0x00000000 && IsRemotePresent != 0)
	{
		// Debugger is present
/*		fin << "please close debuggers \n";*/
		fin << __ODA__("DFIVVVVVV\\htt#-6DEFIIJYccrsw -;ET\\jqsy{$2>BORZfnuz \"05>JN[]`efr#&.;EKTbq");
/*		system("PAUSE");*/
		system(__ODA__("DJLQQQQQ\\gr}%4:EMP^dl{\"(*59<=IUbjmz&'0>\?"));
		ExitProcess(2);
	}
}

// HideThread will attempt to use
// NtSetInformationThread to hide a thread
// from the debugger, Passing NULL for
// hThread will cause the function to hide the thread
// the function is running in. Also, the function returns
// false on failure and true on success
bool HideThread() //d5-hide thread
	{
		typedef NTSTATUS(NTAPI *pNtSetInformationThread)
			(HANDLE, UINT, PVOID, ULONG);
		NTSTATUS Status;

		// Get NtSetInformationThread
		pNtSetInformationThread NtSIT = (pNtSetInformationThread)
/*			GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),*/
			GetProcAddress(GetModuleHandle(__ODC__("BHOUUUUUW^^bjtx(+8FHPYet$$0>AMSbfsz|-0;ACRY[\\`er &2@JPW^kt\"19@LTY[itt#&(")),
/*				"NtSetInformationThread");*/
				__ODA__("GLLRRRRRXehikw\"16@ABDDSVdos{$/2\?COOW^ccloy{ $)/7@LVZelt$/:CHR_`kvx#*5CMO"));


		// Shouldn't fail
		if (NtSIT == NULL)
			return false;

		// Set the thread info

		Status = NtSIT(GetCurrentThread(),
			0x11, // HideThreadFromDebugger
			0, 0);


		if (Status != 0x00000000)
		{
			return false;

		}
		else 
		{
			return true;
		}
	}


void remote_debug_check() //dd6-remote debug
{
	BOOL IsDbgPresent = FALSE;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &IsDbgPresent);
	if (IsDbgPresent)
	{
/*		fin << "please close debuggers \n";*/
		fin << __ODA__("LRYffffflu$*4<>KMZcppqw&&//9HU[any$$+./;HMZiqy%.6CPS[^hku|!((24\?CQSZ\\jps");
/*		system("PAUSE");*/
		system(__ODA__("INU[[[[[hs|}&.<FHOTcio %+6=KKOYdsv{!&(1>"));
		ExitProcess(2);
	}
}
