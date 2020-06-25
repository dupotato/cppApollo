#ifndef _APOLOCLIENT_H_
#define _APOLOCLIENT_H_

#include "iostream"
#include "common.h"
#include "json/json.h"
#include <map>
using namespace std;

#define APOLO_CONFIG_NOCACHE_URL "%s://%s/configs/%s/%s/%s"
#define CONTENT_HEADER "Content-Type:application/json;charset=UTF-8"
typedef std::map<std::string,std::string> Properties;
class ApolloClient 
{
    public:
        ApolloClient();
        ApolloClient(int tokenEncrypted,int switchHttps);
        ~ApolloClient();
        ApolloClient(string apolloIpInfo,string appId,string clusterName,string namespaceName,string token);
    public:
        void InitClientInfo(string aIpinfo,string appId,string clusterName,\
        string namespaceName,string token);

        string GetPropertyInfoNoCache(string apolloIpInfo,string appId,string clusterName,string namespaceName,string token,int switchHttps);
        void GetUrlDestDefault(string &url);
        string TokenChange(string token);
        Properties GetClientInfoDefault();
        Properties GetClientInfo(string apolloInfo,string appId,string clusterName,string namespaceName,string token);
    private:
        string m_apolloIpInfo;
        string m_appId;
        string m_clusterName;
        string m_namespaceName;
        string m_token;
        int m_tokenEncrypted;
        int m_switchHttps;
};
#endif //_APOLOCLIENT_
