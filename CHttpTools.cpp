////////////////////////////////////////////////////////////////////////////////
///// Global include
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <future>
#include <chrono>

////////////////////////////////////////////////////////////////////////////////
///// Local include
////////////////////////////////////////////////////////////////////////////////
#include "CHttpTools.h"

namespace VDC
{

const int SOCKET_ERROR = -1,     ///< the socket error code
          MAX_DATA_SIZE = 65536, ///< input buffer
          PORT = 80;             ///< default port

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CHttpTools::CHttpTools()
{
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CHttpTools::~CHttpTools()
{
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CHttpTools::getCurrentIPs(std::vector<std::string> & outIPs)
{
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);
    outIPs.resize(INET_ADDRSTRLEN+1);
    char addressBuffer[INET_ADDRSTRLEN+1];
    outIPs.clear();

    while (ifAddrStruct!=NULL)
    {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET && strcmp(ifAddrStruct->ifa_name, "lo0")!=0)
        { // check it is IP4 and not lo0
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            outIPs.push_back(std::string(inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN)));
        } else {}
        ifAddrStruct=ifAddrStruct->ifa_next;
    } // while
} // getCurrentIPs

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CHttpTools::sendData(int inSockfd, const char* inStrToSend, ...) const
{
    va_list ap;
    va_start  (ap, inStrToSend);
    char *pTxt = NULL;
    vasprintf(&pTxt, inStrToSend, ap);
    if (!pTxt)
    {
        std::cerr<<"Allocation memory to send message"<<std::endl;
        return false;
    } else {}
    va_end (ap);
    std::string txtToSend(pTxt);
    free(pTxt);

    int retour = send(inSockfd,txtToSend.c_str(),sizeof(char)*txtToSend.size(),0 );

//std::cout<<"Sent: '"<<txtToSend<<"'"<<std::endl;

    if (retour == SOCKET_ERROR)
    {
        std::cerr<<"send message error"<<std::endl;
        return false;
    } else {}
    return true;
} // sendData

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CHttpTools::getDataFile(int inSockfd, const std::string& inURLFileToGet, std::string& outReceivedData, bool inWithHTTPHeader, const std::string& inType, const std::string& inCookie, const std::string& inRequestMethod, const std::string& inContent) const
{
    std::string hostStr;
    std::string relativeFile(inURLFileToGet);
    std::string::size_type endOfHost = inURLFileToGet.find('/');
    if (endOfHost != std::string::npos)
    {
        hostStr = inURLFileToGet.substr(0, endOfHost);
        relativeFile = inURLFileToGet.substr(endOfHost);
    } else {}
    sendData(inSockfd,"%s %s HTTP/1.0\r\n",inRequestMethod.c_str(), relativeFile.c_str());
    if (!hostStr.empty())
    {
        sendData(inSockfd,"Host: %s\r\n", hostStr.c_str());
    } else {}
    sendData(inSockfd,"From: someuser@gmail.com\r\n");
    sendData(inSockfd,"User-Agent: HTTPTool/1.0\r\n");
    if (!inCookie.empty())
    {
    	sendData(inSockfd,"Cookie: %s\r\n", inCookie.c_str());
    } else {}
    sendData(inSockfd,"Content-Type: %s\r\n", inType.c_str());
    if (!inContent.empty())
    {
        sendData(inSockfd,"Content-Length: %d\r\n", inContent.size());
        sendData(inSockfd,"\r\n");
        sendData(inSockfd,"%s\r\n", inContent.c_str());
    } else {}

    sendData(inSockfd,"\r\n");

    outReceivedData.clear();
    char buf[MAX_DATA_SIZE];
    int numbytes=recv(inSockfd, buf, MAX_DATA_SIZE, 0);
    while ( numbytes > 0)
    {
        buf[numbytes] = '\0';
        outReceivedData += buf;
        numbytes=recv(inSockfd, buf, MAX_DATA_SIZE, 0);
    }

    // erase header if needed
    if (!outReceivedData.empty() && !inWithHTTPHeader)
    {
        if (outReceivedData.compare(0, 5, "HTTP/") == 0)
        {
            /// HTTP header detected, erase it !
            /// HTTP header is stopped by an empty line => we must erase until the empty line, that is "\r\n\r\n"
            std::string patternEndOfHeader="\r\n\r\n";
            std::size_t endHeaderidx = outReceivedData.find(patternEndOfHeader);
            outReceivedData.erase(0, endHeaderidx+patternEndOfHeader.size());
        }
        else
        {
            /// no HTTP header, nothing to do
        }
    } else {}

    return (!outReceivedData.empty());
} // getDataFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CHttpTools::getDataFile(const std::string& inURLFileToGet, std::string& outReceivedData, bool inWithHTTPHeader, const std::string& inType, const std::string& inCookie, const std::string& inRequestMethod, const std::string& inContent) const
{
    bool retVal = false;

    std::string URLFileToGetUpCase(inURLFileToGet),
                ipToConnect(inURLFileToGet),
                fileToGet,
                fullURL;

    std::transform(URLFileToGetUpCase.begin(), URLFileToGetUpCase.end(), URLFileToGetUpCase.begin(), (int(*)(int))std::toupper);
    size_t idStrToFind = URLFileToGetUpCase.find("HTTP://");
    if (idStrToFind != std::string::npos)
    {
        ipToConnect.erase(0,idStrToFind+7);
    } else {}

    idStrToFind = ipToConnect.find('/');
    if (idStrToFind != std::string::npos)
    {
        fileToGet = ipToConnect.substr(idStrToFind);
        ipToConnect.erase(idStrToFind);
    } else {}

    fullURL = "http://" + ipToConnect + fileToGet;

    if (!ipToConnect.empty() && !fileToGet.empty())
    {
        int sockfd;  
        struct hostent *he;
        struct sockaddr_in their_addr; /* Adresse de celui qui se connecte */

        if ((he=gethostbyname(ipToConnect.c_str())) == NULL)
        {  /* Info de l'hote */
            herror("gethostbyname");
            return false;
        } else {}

        if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        {
            std::cerr<<"socket not created to download '"<<inURLFileToGet<<"'"<<std::endl;
            return false;
        } else {}

        their_addr.sin_family = AF_INET;  /* host byte order */
        their_addr.sin_port = htons(PORT);/* short, network byte order */
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        bzero(&(their_addr.sin_zero), 8); /* zero pour le reste de struct */

        if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
        {
            std::cerr<<"Connection to '"<<ipToConnect<<"' : "<<PORT<<" not possible to download '"<<inURLFileToGet<<"'"<<std::endl;
            close(sockfd);
            return false;
        } else {}

        retVal = getDataFile(sockfd, inURLFileToGet, outReceivedData, inWithHTTPHeader, inType, inCookie, inRequestMethod, inContent);

        close(sockfd);
    } else {}

    return retVal;
} // getDataFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string CHttpTools::getIpFromHostName(const std::string& inHostName)
{
    std::string ipStr;
    struct hostent *pHe = gethostbyname( inHostName.c_str() );
    if (pHe)
    {
        struct in_addr **pAddrList = (struct in_addr **) pHe->h_addr_list;
        /// get the first one
        if (pAddrList && pAddrList[0])
        {
            ipStr = inet_ntoa(*pAddrList[0]);
        } else {}
    } else {}

    return ipStr;
} // getIpFromHostName

} // namespace VDC
