#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CHttpTools.h"

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

/**
 * @brief Vindinium managed directions
 */
enum E_VINDINIUM_ACTIONS
{
    E_VINDINIUM_STAY = 0, ///< Stay here
    E_VINDINIUM_NORTH,    ///< Go to the North
    E_VINDINIUM_SOUTH,    ///< Go to the South
    E_VINDINIUM_EAST,     ///< Go to the East
    E_VINDINIUM_WEST,     ///< Go to the West
    NB_VINDINIUM_ACTIONS  ///< number of managed actions
}; // enum E_VINDINIUM_ACTIONS

static const std::string G_VINDINIUM_ACTIONS_DICTIONARY[NB_VINDINIUM_ACTIONS]={
    "Stay",
    "North",
    "South",
    "East",
    "West"
}; ///< Dictionary of Vindinium actions

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
        * @param inVindiniumUrl the Vindinium web serser url to be used
        */
        CClient(const std::string& inVindiniumKey, const std::string& inVindiniumUrl = "vindinium.org");

        /**
        * @brief Destructor of Vindinium client
        */
        virtual ~CClient();

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
        * @param inMode the mode to use in the new game
        * @return true if the game has been successfully started, false otherwise
        */
        bool startGame(const E_VINDINIUM_MODE inMode);

        /**
        * @brief Compute the next move according to the current Game status
        * @param inGame the current Game
        * @param inHero the current Hero
        * @return the action to do
        * 
        * The default behavior is random!
        */
        virtual E_VINDINIUM_ACTIONS playAI(const CGame& inGame, const CHero& inHero);

    private:
        CHttpTools m_httpTools;
        std::string m_key;
        std::string m_svrHostName;
        std::string m_svrIp;
}; // class CVindiniumClient

} // namespace VDC