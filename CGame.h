#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CPosition.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
namespace Json
{
    class Value;
} // namespace Json

namespace VDC
{
    class CHero;
} // namespace VDC

namespace VDC
{

/**
 * @brief Definition of possible objects in the board
 */
enum E_BOARD_OBJECTS
{
    E_NO_OBJECT = 0,
    E_IMPASSABLE_WOOD,
    E_GOLD_MINE,
    E_TAVERN,
    E_HERO,
    NB_BOARD_OBJECTS
}; // enum E_BOARD_OBJECTS

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Client for web game 'Vindinium' (http://vindinium.org/)
 */
class CGame
{
    public:
        /**
        * @brief Constructor of Vindinium game
        * @param inJsonValues the json data describing the game
        * @param inMyHeroId the id of the hero to play
        */
        CGame(const Json::Value& inJsonValues, const int inMyHeroId);

        /**
        * @brief Destructor of Vindinium game
        */
        virtual ~CGame();

        /**
        * @brief Update the current game with new json data
        * @param inJsonValues the json data describing the game
        */
        void update(const Json::Value& inJsonValues);

        /**
        * @brief Return the game id
        * @return the game id
        */
        inline std::string getId() const { return m_id; }

        /**
        * @brief Return true if the game is finished, false otherwise
        * @return true if the game is finished, false otherwise
        */
        inline bool isFinished() const { return m_isFinished; }

        /**
        * @brief Return a reference to an hero
        * @param inHeroId the id of the hero to get
        * @return a reference to an hero
        */
        inline CHero& getHero(const int inHeroId) { return m_heros.at(inHeroId); }

        /**
        * @brief Return the current hero to play
        * @return the current hero to play
        */
        inline CHero& getMyHero() { return getHero(m_myHeroId-1); }

        /**
        * @brief Return the id of the current hero to play
        * @return the id of the current hero to play
        */
        inline int getMyHeroId() { return m_myHeroId; }

        /**
        * @brief Return the current number of turns of game (is a multiple of the number of player).
        * @return the current turn
        * 
        * Use CGame::getTrueTurn to get the true number of turns of the hero
        */
        inline int getTurn() const { return m_turn; }

        /**
        * @brief Return the true current number of turns
        * @return the true current number of turns
        */
        inline int getTrueTurn() const { return m_turn/m_nbPlayers; }

        /**
        * @brief Return the max number of turns (is a multiple of the number of player).
        * @return the max turn (is a multiple of the number of player).
        * 
        * Use CGame::getTrueMaxTurn to get the true max number of turns of the hero
        */
        inline int getMaxTurn() const { return m_maxTurns; }

        /**
        * @brief Return the true max number of turns
        * @return the true max number of turns
        */
        inline int getTrueMaxTurn() const { return m_maxTurns/m_nbPlayers; }

        /**
        * @brief Return the number of players of the game
        * @return the number of players of the game
        */
        inline int getNbPlayers() const { return m_nbPlayers; }

        /**
        * @brief Print in the standard output the current game data
        */
        void print();

    private:
        /**
        * @brief Fill the list of objects
        */
        void parseBoard();

        /**
        * @brief return the abscissa in the 1D board representation of a 2D position
        * @param inPosition the position to use
        * @return the abscissa in the 1D board representation of a 2D position
        */
        inline int get1DCoordOnBoard(const CPosition& inPosition) { return ((inPosition.getX()>=0 && inPosition.getY()>=0)? inPosition.getX()+inPosition.getY()*m_boardEdgeSize:-1); }

        /**
        * @brief return the 2D position in the board
        * @param in1DPosition the position to use
        * @return the 2D position in the board
        */
        inline CPosition get2DCoordOnBoard(const int in1DPosition) { return CPosition( in1DPosition%m_boardEdgeSize, in1DPosition/m_boardEdgeSize); }

        /**
        * @brief Update the board distances definition according to heros positions
        */
        void updateBoardDistances();

    private:
        std::string m_id;
        int m_nbPlayers;
        int m_turn;
        int m_maxTurns;
        std::vector<CHero> m_heros;
        int m_boardEdgeSize;
        std::string m_boardStr;
        bool m_isFinished;
        int m_myHeroId; ///< the id of the current hero to play. Use m_myHeroId-1 to find it in m_heros vector

        /// board data
        std::vector<CPosition> m_tavernPositionsList; ///< the list of tavern positions
        std::vector<CPosition> m_goldMinePositionsList; /// the list of gold mine positions
        std::vector<int> m_boardDistances; ///< all the distances from each board cell to the top-left corner of the board
        std::vector<E_BOARD_OBJECTS> m_board; ///< board defined at the beginning of the game
}; // class CGame

} // namespace VDC