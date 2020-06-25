#include <iostream>
#include "apolloclient.h"


 //token encrypted: 2vaC6OI25Mc7ohUpBbPcpd8zif1MgEBuI+d/OpVFBahhnW9UlV2vOooCofltY81X 
 //token::  d64d1458e5334921b5c35034cd13236b

// token encrypted
// http 
void useExample_1()
{
    ApolloClient* cc=new ApolloClient(1,0);
   
    Properties pp=cc->GetClientInfo("10.130.41.13:8080","1111","default","application","2vaC6OI25Mc7ohUpBbPcpd8zif1MgEBuI+d/OpVFBahhnW9UlV2vOooCofltY81X");
    Properties::iterator it=pp.begin();
    for (;it!=pp.end();++it)
    {
        std::cout<<it->first<<"\t"<<it->second<<std::endl;
    }
    delete cc;
    cc=NULL;
}

//token encrypted
//https
void useExample_2()
{
    ApolloClient* cc=new ApolloClient(1,1);
    string apolloIpInfo="10.130.29.88:8080";
    string appId="1111";
    string clusterName="default";
    string namespaceName="application";
    string token="2vaC6OI25Mc7ohUpBbPcpd8zif1MgEBuI+d/OpVFBahhnW9UlV2vOooCofltY81X";
    
    
    cc->InitClientInfo(apolloIpInfo,appId,clusterName,namespaceName,token);
    Properties pp=cc->GetClientInfoDefault();
    Properties::iterator it=pp.begin();
    for (;it!=pp.end();++it)
    {
        std::cout<<it->first<<"\t"<<it->second<<std::endl;
    }
    delete cc;
    cc=NULL;
}


//token not encrypted
//http 
void useExample_3()
{
    ApolloClient* cc=new ApolloClient(0,0);
    string apolloIpInfo="10.130.41.13:8080";
    string appId="1111";
    string clusterName="default";
    string namespaceName="application";
    string token="d64d1458e5334921b5c35034cd13236b";
    
    
    cc->InitClientInfo(apolloIpInfo,appId,clusterName,namespaceName,token);
    Properties pp=cc->GetClientInfoDefault();
    Properties::iterator it=pp.begin();
    for (;it!=pp.end();++it)
    {
        std::cout<<it->first<<"\t"<<it->second<<std::endl;
    }
    delete cc;
    cc=NULL;
}


int main()
{
    useExample_1();
    std::cout<<"\n===================\n";
    useExample_2();
    std::cout<<"\n===================\n";
    useExample_3();
}
