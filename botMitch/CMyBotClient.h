#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "../CHttpTools.h"
#include "../CClient.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
namespace VDC
{
    class CGame;
    class CHero;
} // namespace VDC

namespace VDC
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Client for web game 'Vindinium' (http://vindinium.org/)
 */
class CMyBotClient : public CClient
{
    public:
        /**
        * @brief Constructor of Vindinium client
        * @param inVindiniumKey the key to be connected to the web server
        * @param inVindiniumUrl the Vindinium web server url to be used
        */
        CMyBotClient(const std::string& inVindiniumKey, const std::string& inVindiniumUrl = "vindinium.org") : CClient(inVindiniumKey, inVindiniumUrl) {}

        /**
        * @brief Destructor of Vindinium client
        */
        virtual ~CMyBotClient(){}

        /**
        * @brief Compute the next move according to the current Game status
        * @param inGame the current Game
        * @return the action to do
        * 
        * The default behavior is random!
        * This function must be overloaded with the needed AI
        */
        virtual E_VINDINIUM_ACTIONS playAI(const CGame& inGame);

}; // class CVindiniumClient

} // namespace VDC