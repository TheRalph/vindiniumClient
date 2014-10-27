#=============================================================================
# Copyright (c) 2014, Raphaël La Greca <raphael.la.greca+vindinium@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  * Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

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
#include <sstream>
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////////
///// Local include
////////////////////////////////////////////////////////////////////////////////
#include "CHttpTools.h"

namespace MOBE
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

    std::stringstream sstr;
    sstr<<inRequestMethod<<" "<<relativeFile<<" HTTP/1.0\r\n";
    if (!hostStr.empty())
    {
        sstr<<"Host: "<<hostStr<<"\r\n";
    } else {}
    sstr<<"From: someuser@gmail.com\r\n";
    sstr<<"User-Agent: HTTPTool/1.0\r\n";
    if (!inCookie.empty())
    {
        sstr<<"Cookie: "<<inCookie<<"\r\n";
    } else {}
    sstr<<"Content-Type: "<<inType<<"\r\n";
    if (!inContent.empty())
    {
        sstr<<"Content-Length: "<<inContent.size()<<"\r\n";
        sstr<<"\r\n";
        sstr<<inContent<<"\r\n";
    } else {}
    sstr<<"\r\n";
    sendData(inSockfd, sstr.str().c_str());

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
bool CHttpTools::getDataFile(const std::string& inURLFileToGet, std::string& outReceivedData, bool inWithHTTPHeader, const std::string& inType, const std::string& inCookie, const std::string& inRequestMethod, const std::string& inContent)
{
    bool retVal = false;

    std::string URLFileToGetUpCase(inURLFileToGet),
                urlToConnect(inURLFileToGet),
                fileToGet,
                ipToConnect;

    std::transform(URLFileToGetUpCase.begin(), URLFileToGetUpCase.end(), URLFileToGetUpCase.begin(), (int(*)(int))std::toupper);
    size_t idStrToFind = URLFileToGetUpCase.find("HTTP://");
    if (idStrToFind != std::string::npos)
    {
        urlToConnect.erase(0,idStrToFind+7);
    } else {}

    idStrToFind = urlToConnect.find('/');
    if (idStrToFind != std::string::npos)
    {
        fileToGet = urlToConnect.substr(idStrToFind);
        urlToConnect.erase(idStrToFind);
    } else {}

    if (!urlToConnect.empty() && !fileToGet.empty())
    {
        int sockfd;  

        std::map<std::string,std::string>::iterator itIP = m_ipsCache.find(urlToConnect);
        if (itIP != m_ipsCache.end())
        {
            ipToConnect = itIP->second;
        }
        else
        {
            struct hostent *he;
            if ((he=gethostbyname(urlToConnect.c_str())) == NULL)
            {
                herror("gethostbyname");
                return false;
            }
            else
            {
                ipToConnect = he->h_addr;
                m_ipsCache[urlToConnect] = ipToConnect;
            } // else
        } // else

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            std::cerr<<"socket not created to download '"<<inURLFileToGet<<"'"<<std::endl;
            return false;
        } else {}

        struct sockaddr_in their_addr;
        their_addr.sin_family = AF_INET;  /* host byte order */
        their_addr.sin_port = htons(PORT);/* short, network byte order */
        their_addr.sin_addr = *((struct in_addr *)ipToConnect.c_str());
        bzero(&(their_addr.sin_zero), 8); /* zero pour le reste de struct */

        if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
        {
            std::cerr<<"Connection to '"<<urlToConnect<<"' : "<<PORT<<" not possible to download '"<<inURLFileToGet<<"'"<<std::endl;
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

} // namespace MOBE
