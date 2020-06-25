#include "apolloclient.h"
#include <curl/curl.h>

ApolloClient::ApolloClient()
{
    m_tokenEncrypted=0;
    m_switchHttps=0;
}
/**
 *  tokenEncrypted : 是否对token进行加密；1 标识加密 ；其他不加密
 *  switchHttps    : 是否进行https访问
 * */
ApolloClient::ApolloClient(int tokenEncrypted,int switchHttps)
{
    m_tokenEncrypted=tokenEncrypted;
    m_switchHttps=switchHttps;
}


ApolloClient::~ApolloClient()
{

}

void ApolloClient::InitClientInfo(string aIpinfo,string appId,string clusterName,\
        string namespaceName,string token)
{
    this->m_apolloIpInfo=aIpinfo;
    this->m_appId=appId;
    this->m_clusterName=clusterName;
    this->m_namespaceName=namespaceName;
    this->m_token=token;
}

string ApolloClient::GetPropertyInfoNoCache(string apolloIpInfo,string appId,string clusterName,string namespaceName,string token,int switchHttps)
{
    CURL *curl= curl_easy_init();
    if (!curl)
    {
        std::cout<<"curl init error"<<std::endl;
        return "";
    }
    CURLcode res;
    
    //init get curldest
    char urlDest[256];
    //"http://%s/configs/%s/%s/%s
    string http="http";
    if (switchHttps==1)
    {
        http="https";
    }
    sprintf(urlDest,APOLO_CONFIG_NOCACHE_URL,http.c_str(),apolloIpInfo.c_str(),\
    appId.c_str(),clusterName.c_str(),namespaceName.c_str());
    MyPrintfInfo("GetUrlDest: %s",urlDest); 

    //init header 
    char rstr[MAXCONFIGNUM];
    memset(&rstr, 0, MAXCONFIGNUM);
    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, CONTENT_HEADER);
    if (token!="")
    {
        char signatureStr[256];
        time_t now=getNowTime();
        //now=1592817449880;
        sprintf(signatureStr,"%ld\n/configs/%s/%s/%s",now,appId.c_str(),
        clusterName.c_str(),namespaceName.c_str());
        
        unsigned char digest[128];
        memset(&rstr, 0, 128);
        signature(digest,token.c_str(),token.length() ,signatureStr,strlen(signatureStr));
        //std::cout<<"digest len="<<strlen(digest)<<std::endl;

        //tokenstr and timestampstr
        char tokenstr[256];
        sprintf(tokenstr,"Authorization: Apollo %s:%s",appId.c_str(),digest);
        char timestampstr[128];
        sprintf(timestampstr,"Timestamp: %ld",now);
        MyPrintfDebug("[cppApollo] timestamp setHeader %s" ,timestampstr);
        MyPrintfDebug("[cppApollo] token setHeader: %s",tokenstr);

        //add curl_slist
        headers= curl_slist_append(headers,tokenstr);
        headers = curl_slist_append(headers, timestampstr);
        //delete[] digest;
    }

    if (switchHttps==1)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);//不使用证书
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    curl_easy_setopt(curl, CURLOPT_URL, &urlDest);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rstr);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);
    if (res!=CURLE_OK){
        MyPrintfError("[cppApollo]curl error:",curl_easy_strerror(res));
    }
    else
    {
        MyPrintfDebug("[cppApollo]curl ok: %s",rstr);
    }
    curl_easy_cleanup(curl);
    std::cout<<"[cppApollo]curl end"<<std::endl;
    return string(rstr);
}


void ApolloClient::GetUrlDestDefault(string &url)
{
    char urldest[256];
    //"http://%s/configs/%s/%s/%s
    string http="http";
    if(m_switchHttps==1)
    {
        http="https";
    }
    sprintf(urldest,APOLO_CONFIG_NOCACHE_URL,http.c_str(),this->m_apolloIpInfo.c_str(),\
    this->m_appId.c_str(),this->m_clusterName.c_str(),this->m_namespaceName.c_str());
    std::cout<<"[cppApollo]GetUrlDest:"<<urldest<<std::endl;
    url=string(urldest);
}

Properties ApolloClient::GetClientInfo(string apolloInfo,string appId,string clusterName,string namespaceName,string token)
{
    string newtoken=TokenChange(token);
    string rstr=GetPropertyInfoNoCache(apolloInfo,appId,clusterName,namespaceName,newtoken,m_switchHttps);
    //std::cout<<rstr<<std::endl;
    Properties ret;
    try 

    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(rstr,root);
        if(root.isMember("configurations"))
        {
            Json::Value c=root["configurations"];
            Json::Value::Members mem = c.getMemberNames();
	        Json::Value::Members::iterator it;
            for (it = mem.begin(); it != mem.end(); it++)
	        {
                ret[*it]=c[*it].asString();
            }      
        }
        else
        {
            MyPrintfError("error: not configurations :%s",rstr.c_str());
        }
        
    }
    catch(...)
    {
        MyPrintfError("error:parase json : %s",rstr.c_str());
    }
    return ret;
}

Properties ApolloClient::GetClientInfoDefault()
{
    return GetClientInfo(m_apolloIpInfo,m_appId,m_clusterName,m_namespaceName,m_token); 
}

string ApolloClient::TokenChange(string token)
{
    if (m_tokenEncrypted!=1)
    {
        return token;
    }
    else
    {
        MyPrintfDebug("base64tokenis %s",token.c_str());
        char* buffer=NULL;
        base64Decode(token.c_str(),token.length(),&buffer);
        unsigned char* outtoken=NULL;
    
        AES_CBC_Decrypto(buffer,strlen(buffer),&outtoken);
        string newtoken(outtoken);
        MyPrintfDebug("new token is %s",newtoken.c_str());

        free(outtoken);
        free(buffer);
        //free(decryptedMessage);
        return newtoken;
    }
}