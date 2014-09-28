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
