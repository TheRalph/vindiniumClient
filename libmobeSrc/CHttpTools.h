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

#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations

namespace MOBE
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Tools to send/get http requests
 */
class CHttpTools
{
    public:
        /**
        * @brief Constructor of http tools
        */
        CHttpTools();

        /**
        * @brief Destructor of http tools
        */
        ~CHttpTools();

        /**
        * @brief Get the list of the current machine IPs
        * @param outIPs the list of the current machine IPs
        */
        void getCurrentIPs(std::vector<std::string> & outIPs);

        /**
        * @brief Get data from a given URL
        * @param inURLFileToGet the url to get data from
        * @param outReceivedData the string to fill with the data
        * @param inWithHTTPHeader if true then the outReceivedData string will contains the http header. if false, the header is removed
        * @param inType the mime type of data to use (Ex: application/x-www-form-urlencoded for POST request). Default is "text/html"
        * @param inCookie a string containing the cookie to use. Default is empty.
        * @param inRequestMethod the request method to use : 'GET' or 'POST'. Default is 'GET'
        * @param inContent a content to add in the request. Mainly used fot POST request.
        * @return true if the operation has been done successfully, false otherwise
        */
        bool getDataFile(const std::string& inURLFileToGet, std::string& outReceivedData, bool inWithHTTPHeader = true, const std::string& inType="text/html", const std::string& inCookie="", const std::string& inRequestMethod = "GET", const std::string& inContent = "");

        /**
        * @brief Get the IP adress using an host name (dns name)
        * @param inHostName the host name to find the ip
        * @return the ip of the given host name
        */
        std::string getIpFromHostName(const std::string& inHostName);

    private:
        /**
        * @brief Send data to the server using a given socket
        * @param inSockfd the socket to use
        * @param inStrToSend the string to send using fprintf format
        * @return true if the operation has been done successfully, false otherwise
        */
        bool sendData(int inSockfd, const char* inStrToSend, ...) const;

        /**
        * @brief Get data from a given socket
        * @param inSockfd the socket to use
        * @param inURLFileToGet the url to get data from
        * @param outReceivedData the string to fill with the data
        * @param inWithHTTPHeader if true then the outReceivedData string will contains the http header. if false, the header is removed
        * @param inType the mime type of data to use (Ex: application/x-www-form-urlencoded for POST request). Default is "text/html"
        * @param inCookie a string containing the cookie to use. Default is empty.
        * @param inRequestMethod the request method to use : 'GET' or 'POST'. Default is 'GET'
        * @param inContent a content to add in the request. Mainly used fot POST request.
        * @return true if the operation has been done successfully, false otherwise
        */
        bool getDataFile(int inSockfd, const std::string& inURLFileToGet, std::string& outReceivedData, bool inWithHTTPHeader = true, const std::string& inType="text/html", const std::string& inCookie="", const std::string& inRequestMethod = "GET", const std::string& inContent = "") const;
        
    private:
        std::map<std::string, std::string> m_ipsCache;
}; // class CHttpTools

} // namespace MOBE
