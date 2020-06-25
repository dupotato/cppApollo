#include "common.h"

void signature(unsigned char* digest,unsigned char* key,int keylen,char* data,int datalen)
{
    unsigned char result[20];
    unsigned int len=20;

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    MyPrintfDebug("hmac key:%s",key);
    MyPrintfDebug("hmac data:%s",data);
    HMAC_Init_ex(&ctx,key,keylen,EVP_sha1(),NULL);
    HMAC_Update(&ctx,(unsigned char*)data,datalen);
    HMAC_Final(&ctx,result,&len);
    //for (int i = 0; i != len; i++)
    //    printf("%02x", (unsigned int)result[i]);
    char* base64buffer=base64_encode(result,len);
    strcpy(digest,base64buffer);
    free(base64buffer);
    MyPrintfDebug("signature: %s",digest);

}

char* base64_encode(const unsigned char *in, int32_t len)
{
    BIO     *bmem, *b64;
    BUF_MEM *b_ptr;
 
    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, in, len);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &b_ptr);
 
    char *buff = (char *)calloc(1, b_ptr->length);
    if (buff == NULL) {
        printf("alloc memory fail\n");
        return NULL;
    }
 
    memcpy(buff, b_ptr->data, b_ptr->length - 1);
    buff[b_ptr->length - 1] = 0;
 
    BIO_free_all(b64);
 
    return buff;
}


size_t write_memory_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    if (strlen((char *)stream) + strlen((char *)ptr) > MAXCONFIGNUM-1) return 0;
    size_t len = size * nmemb;
    char* streamChar=(char*)stream;
    strcat(streamChar,(char*)ptr);
    return len;
}

time_t getNowTime()
{
    struct timeval tv;
 
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000 + tv.tv_usec/1000);

}


int AES_CBC_Decrypto(unsigned char* encryptedMessage,int enlen,unsigned char** decryptedMessage)
{
    Crypto crypto;
    //key length cannot beyond 16
    char key[8]="ccodccod";

    //填充 0x7
    unsigned char * aesKey=new unsigned char[16];
    memset(aesKey,0x7,16);
    //strcpy(aesKey32,inputkey.c_str());
    strncpy(aesKey,key,8);
    unsigned char iv[16]="0123456789abcdef";
    
    //set key and iv
    crypto.setAesKey(aesKey,16);
    crypto.setAesIv(iv,16);

    //aes 128 decode
    unsigned char* outMessage=NULL;
    int decryptedlen=crypto.aesDecrypt(encryptedMessage,enlen,&outMessage);
    if (decryptedlen==-1)
    {
        MyPrintfError("decrypt error:%s",encryptedMessage);
        return -1;
    }
    *decryptedMessage=new unsigned char[decryptedlen+1];
    strncpy(*decryptedMessage,outMessage,decryptedlen);
    (*decryptedMessage)[decryptedlen]='\0';

    MyPrintfDebug("decryptdMessage %s;lenis %d",*decryptedMessage,decryptedlen);
    delete [] aesKey;
    return 0;
}

char *base64_decode(unsigned char *in, int32_t len)
{
    BIO     *b64, *bmem;
    char *buffer = (char *)calloc(1, len);
    if (buffer == NULL) {
        printf("alloc memory fail\n");
        return NULL;
    }
 
    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new_mem_buf(in, len);
    bmem = BIO_push(b64, bmem);
 
    BIO_read(bmem, buffer, len);
 
    BIO_free_all(bmem);
 
    return buffer;
}

int base64Decode(const char *b64message, const size_t length, unsigned char **buffer) {
  int decodedLength = calcDecodeLength(b64message, length);
  *buffer = (unsigned char*)malloc(decodedLength + 1);

  if(*buffer == NULL) {
    MyPrintfError("Failed to allocate memory");
    return -1;
  }

  BIO *bio = BIO_new_mem_buf(b64message, -1);
  BIO *b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);
  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

  decodedLength = BIO_read(bio, *buffer, strlen(b64message));
  (*buffer)[decodedLength] = '\0';

  BIO_free_all(bio);
  //printf("xxxxxxxx%s",*buffer);
  return decodedLength;
}

int calcDecodeLength(const char *b64input, const size_t length) {
  unsigned int padding = 0;

  // Check for trailing '=''s as padding
  if(b64input[length - 1] == '=' && b64input[length - 2] == '=') {
    padding = 2;
  } else if (b64input[length - 1] == '=') {
    padding = 1;
  }

  return (int)length * 0.75 - padding;
}

void MyPrintfDebug(const char *cmd, ...)  
{  
    #ifdef _DEBUG_
    printf("[debug][cppApollo]%s %s ", __DATE__, __TIME__);  
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args,cmd); //使args指向可变参数的第一个参数  
    vprintf(cmd,args);  //必须用vprintf等带V的  
    va_end(args);       //结束可变参数的获取
    printf("\n"); 
    #endif
}  

void MyPrintfInfo(const char *cmd, ...)  
{  
    printf("[info][cppApollo]%s %s ", __DATE__, __TIME__);  
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args,cmd); //使args指向可变参数的第一个参数  
    vprintf(cmd,args);  //必须用vprintf等带V的  
    va_end(args);       //结束可变参数的获取
    printf("\n"); 
}  


void MyPrintfError(const char *cmd, ...)  
{  
    printf("[err][cppApollo]%s %s ", __DATE__, __TIME__);  
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args,cmd); //使args指向可变参数的第一个参数  
    vprintf(cmd,args);  //必须用vprintf等带V的  
    va_end(args);       //结束可变参数的获取
    printf("\n"); 
}  