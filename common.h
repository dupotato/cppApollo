/*************************************************************************
Author: potato
Mail: duerbin@126.com 
Created Time: 2020年06月22日 
************************************************************************/ 
#ifndef _APOLLOCLIENT_COMMON_H_
#define _APOLLOCLIENT_COMMON_H_

#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <sys/time.h>
#include "crypto.h"
//some function define
#define MAXCONFIGNUM 10000

/** apollo signature method  by java client
 *  digest ;create digest by key and data
 *  key ; apollo token
 */
void signature(unsigned char* digest,unsigned char* key,int keylen,char* data,int datalen);
/**
 * base64 encode;
 */
char* base64_encode(const unsigned char *in, int32_t len);


size_t write_memory_callback(void *ptr, size_t size, size_t nmemb, void *stream);

time_t getNowTime();

int AES_CBC_Decrypto(unsigned char* encryptedMessage,int enlen,unsigned char** decryptedMessage);
char *base64_decode(unsigned char *in, int32_t len);
int base64Decode(const char *b64message, const size_t length, unsigned char **buffer) ;
int calcDecodeLength(const char *b64input, const size_t length) ;


void MyPrintfDebug(const char *cmd, ...);
void MyPrintfInfo(const char *cmd, ...);
void MyPrintfError(const char *cmd, ...);
#endif //_APOLLOCLIENT_COMMON_H_
