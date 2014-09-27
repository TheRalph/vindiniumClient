#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CHttpTools.h"
#include "CGame.h"
#include "CBehaviorMgr.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
namespace BEEN
{
    class CHero;
} // namespace BEEN

namespace BEEN
{


/**
 * @brief Vindinium managed modes
 */
enum E_VINDINIUM_MODE
{
    E_VINDINIUM_TRAINING_MODE = 0, ///< to train the IA on specific maps
    E_VINDINIUM_ARENA_MODE,        ///< to play against other players
    NB_VINDINIUM_MODE              ///< number of managed modes
}; // enum E_VINDINIUM_MODE

static const std::string G_VINDINIUM_MODE_DICTIONARY[NB_VINDINIUM_MODE]={
    "Vindinium training mode",
    "Vindinium arena mode"
}; ///< Dictionary of Vindinium modes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Client for web game 'Vindinium' (http://vindinium.org/)
 */
class CClient
{
    public:
        /**
        * @brief Constructor of Vindinium client
        * @param inVindiniumKey the key to be connected to the web server
        * @param inVindiniumUrl the Vindinium web server url to be used
        * @param inBehaviorName the name of the behavior to use
        * @param inNavigator the navigator to start on start game. By default no navigator is started
        */
        CClient(const std::string& inVindiniumKey, const std::string& inVindiniumUrl = "vindinium.org", const std::string& inBehaviorName = "random", const std::string& inNavigator = "");

        /**
        * @brief Destructor of Vindinium client
        */
        virtual ~CClient();

        /**
        * @brief Set the navigator to start on start game. By default no navigator is started
        * @param inNavigator the navigator to start on start game. By default no navigator is started
        */
        void setNavigator(const std::string& inNavigator);

        /**
        * @brief Set the current active behavior to use
        * @param inBehaviorName the name of the behavior to use
        */
        void setActiveBehavior(const std::string& inBehaviorName);

        /**
        * @brief Return the Ip of the current server
        */
        std::string getServerIp() const;

        /**
        * @brief Return the Host name of the current server
        */
        std::string getServerHostName() const;

        /**
        * @brief Return the current Vindinium identification key
        */
        std::string getKey() const;

        /**
        * @brief Start a game on the current Vindinium server
        * @param inMode the mode to use in the new game, default is arena
        * @param inNbTurns only used in training mode. This is the number of turns to play. Default is -1 that is the default server number of turns
        * @param inMap only used in training mode. This is the map to use to play (m1 ... m6). Default is empty that is random server map
        * @return true if the game has been successfully started, false otherwise
        */
        bool startGame(const E_VINDINIUM_MODE inMode = E_VINDINIUM_ARENA_MODE, const int inNbTurns = -1, const std::string& inMap = "");

        /**
        * @brief Compute the next move according to the current Game status
        * @param inGame the current Game
        * @return the action to do
        * 
        * The default behavior is random!
        * This function must be overloaded with the needed AI
        */
        virtual E_GAME_ACTIONS playAI(const CGame& inGame);

    protected:
        /**
        * @brief Constructor by default of Vindinium client. Only used by childrens
        * @param inVindiniumKey the key to be connected to the web server
        * @param inVindiniumUrl the Vindinium web server url to be used
        */
        CClient() {}

    private:
        CHttpTools m_httpTools;
        std::string m_key;
        std::string m_svrHostName;
        std::string m_svrIp;
        std::string m_navigator;
        CBehaviorMgr m_behaviorMgr;
        IBehaviorModule *m_pActiveBehavior;
}; // class CVindiniumClient

} // namespace BEEN