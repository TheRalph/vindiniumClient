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
#include <string>
#include <vector>
#include <list>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CPosition.h"
#include "CHero.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
namespace Json
{
    class Value;
} // namespace Json

namespace MOBE
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

typedef std::list<int> path_t;

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
        * @brief Get path from the current hero position to go to a given cellId
        * @param inHeroId the hero id to work with
        * @param inCellId the cellId to go to
        * @param inAvoidHeros set to true to find a path and try to not meet other heros
        * @param outPath path from the current hero position to go to a given cellId
        * @return true is a path has been found
        * 
        * Hero cellId is not in the path, destination cellId is the last element of the path
        */
        bool getPathTo(const int inHeroId, const int inCellId, bool inAvoidHeros, path_t& outPath) const;

        /**
        * @brief Get number of cells from the current hero and a position to reach (shortest path)
        * @param inHeroId the hero id to work with
        * @param inPosition the position to use
        * @param inAvoidHeros set to true to find a path and try to not meet other heros
        * @return the number of cells to cross to reach the given position
        * 
        * A negative value means, the destination can not be reached
        * call of CGame::getDistanceTo(const int inCellId)
        */
        int getDistanceTo(const int inHeroId, const CPosition& inPosition, bool inAvoidHeros) const;

        /**
        * @brief Get number of cells from the current hero and a cellId to reach (shortest path)
        * @param inHeroId the hero id to work with
        * @param inCellId the cellId to use
        * @param inAvoidHeros set to true to find a path and try to not meet other heros
        * @return the number of cells to cross to reach the given cellId
        * 
        * A negative value means, the destination can not be reached
        */
        int getDistanceTo(const int inHeroId, const int inCellId, bool inAvoidHeros) const;

        /**
        * @brief Get the opponent id with the max mine count
        * @param outOpponentIdWithMaxMineCount the id of the opponent with the max mine count. If no opponent is found outOpponentIdWithMaxMineCount = -1
        * @param outMaxMineCount the mine count of the opponent found. If no opponent is found outMaxMineCount = 0
        * @return true if an opponent has been found
        */
        bool getOpponentIdWithMaxMineCount(int &outOpponentIdWithMaxMineCount, int &outMaxMineCount) const;

        /**
        * @brief Get the cellid and the distance of the closest opponent
        * @param outOpponentId the id of the closest opponent. If no opponent is found outOpponentCellId = -1
        * @param outOpponentDistance the distance of the closest opponent. If no opponent is found outOpponentDistance = -1
        * @return true if an opponent has been found
        */
        bool getClosestOpponent(int &outOpponentId, int &outOpponentDistance) const;

        /**
        * @brief Get the closest opponent path
        * @param outOpponentId the id of the closest opponent. If no opponent is found outOpponentCellId = -1
        * @param outClosestOpponentPath the path of the closest opponent. If no opponent is found outClosestOpponentPath is empty
        * @return true if an opponent path has been found
        */
        bool getClosestOpponentPath(int &outOpponentId, path_t &outClosestOpponentPath) const;

        /**
        * @brief Get the cellId and the path to reach the closest tavern
        * @param inHeroId the hero to work with
        * @param outTavernCellId the cellId of the tavern found. If no tavern is found outTavernCellId = -1
        * @param outClosestTavernPath the path to reach the tavern found. If no tavern is found outClosestTavernPath is empty
        * @param inAvoidHeros set to true to find a path and try to not meet other heros. Default is true
        * @return true is a tavern has been found
        */
        bool getClosestTavernPath(const int inHeroId, int &outTavernCellId, path_t &outClosestTavernPath, bool inAvoidHeros = true) const;

        /**
        * @brief Get the cellId and the path to reach the closest tavern
        * @param inHeroId the hero to work with
        * @param outTavernCellId the cellId of the tavern found. If no tavern is found outTavernCellId = -1
        * @param outBestDist the distance to the tavern found.  If no tavern is found outBestDist = -1
        * @param inAvoidHeros set to true to find a path and try to not meet other heros. Default is true
        * @return true is a tavern has been found
        */
        bool getClosestTavernDistance(const int inHeroId, int &outTavernCellId, int& outBestDist, bool inAvoidHeros = true) const;

        /**
        * @brief Get the cellId and the distance of the closest gold mine the current hero do not own
        * @param outGoldMineCellId the cellId of the gold mine found. If no gold mine is found outGoldMineCellId = -1
        * @param outGoldMineDistance the distance of the gold mine found. If no gold mine is found outGoldMineDistance = -1
        * @param inAvoidHeros set to true to find a path and try to not meet other heros. Default is true
        * @return true is a gold mine has been found
        */
        bool getClosestGoldMineCellIdMyHeroDoNotControl(int &outGoldMineCellId, int& outGoldMineDistance, bool inAvoidHeros = true) const;

        /**
        * @brief Get the cellId and the path to reach the closest gold mine the current hero do not own
        * @param outGoldMineCellId the cellId of the gold mine found. If no gold mine is found outGoldMineCellId = -1
        * @param outClosestGoldMinePath the path to reach the gold mine found. If no gold mine is found outClosestGoldMinePath is empty
        * @param inAvoidHeros set to true to find a path and try to not meet other heros. Default is true
        * @return true is a gold mine has been found
        * 
        * Call of CGame::getClosestGoldMineCellIdMyHeroDoNotControl(int &outGoldMineCellId, int& outGoldMineDistance)
        */
        bool getClosestGoldMineMyHeroDoNotControlPath(int &outGoldMineCellId, path_t &outClosestGoldMinePath, bool inAvoidHeros = true) const;

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
        * @brief Return a const reference to an hero
        * @param inHeroId the id of the hero to get (the hero id starts from 1)
        * @return a const reference to an hero
        */
        inline const CHero& getHero(const int inHeroId) const { return m_heros.at(inHeroId-1); }

        /**
        * @brief Return a reference to an hero
        * @param inHeroId the id of the hero to get (the hero id starts from 1)
        * @return a reference to an hero
        */
        inline CHero& getHero(const int inHeroId) { return m_heros.at(inHeroId-1); }

        /**
        * @brief Return the current hero to play
        * @return the current hero to play
        */
        inline const CHero& getMyHero() const { return getHero(m_myHeroId); }

        /**
        * @brief Return the id of the current hero to play
        * @return the id of the current hero to play
        */
        inline const int getMyHeroId() const { return m_myHeroId; }

        /**
        * @brief Return the distance map of a given hero (read only)
        * @param inHeroId The hero id to get the distance map from
        * @return the distance map of a given hero (read only)
        */
        inline const std::vector<int>& getBoardDistancesOfHero(const int inHeroId) const { return m_boardDistances.at(inHeroId-1);}

        /**
        * @brief Return the distance map of a given hero (read/write)
        * @param inHeroId The hero id to get the distance map from
        * @return the distance map of a given hero (read/write)
        */
        inline std::vector<int>& getBoardDistancesOfHero(const int inHeroId) { return m_boardDistances.at(inHeroId-1);}

        /**
        * @brief Return a vector with links to the opponent heros
        * @return a vector with links to the opponent heros
        */
        inline const std::vector<int>& getOpponentHeroIds() const { return m_opponentHeroIds; }

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
        * @brief Return the number of mines
        * @return the number of mines
        */
        inline int getNbMines() const { return m_goldMineCellIdsList.size(); }

        /**
        * @brief Return the number of taverns
        * @return the number of taverns
        */
        inline int getNbTaverns() const { return m_tavernCellIdsList.size(); }

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
        * @brief return the abscissa in the 1D board representation of a 2D position
        * @param inPosition the position to use
        * @return the abscissa in the 1D board representation of a 2D position
        */
        inline int get1DCoordOnBoard(const CPosition& inPosition) const { return get1DCoordOnBoard(inPosition.getX(), inPosition.getY()); }

        /**
        * @brief return the abscissa in the 1D board representation of a 2D position
        * @param inX the horizontal coordinate
        * @param inY the vertical coordinate
        * @return the abscissa in the 1D board representation of a 2D position
        */
        inline int get1DCoordOnBoard(const int inX, const int inY) const { return ((inX>=0 && inY>=0)? inX+inY*m_boardEdgeSize:-1); }

        /**
        * @brief return the 2D position in the board
        * @param in1DPosition the position to use
        * @return the 2D position in the board
        */
        inline CPosition get2DCoordOnBoard(const int in1DPosition) const { return CPosition( in1DPosition%m_boardEdgeSize, in1DPosition/m_boardEdgeSize); }

        /**
        * @brief return the edge size of the board
        * @return the edge size of the board
        */
        inline int getBoardEdgeSize() const { return m_boardEdgeSize; }

        /**
        * @brief return the board with static objects
        * @return the board with static objects
        */
        inline const std::vector<MOBE::E_BOARD_OBJECTS>& getStaticBoard() const { return m_staticBoard; }

        /**
        * @brief Print in the standard output the current game data
        */
        void print() const;

        /**
        * @brief print into the standard output the given path
        * @param inPath the path to print
        * @param inComment the comment to print
        */
        void printPath( const path_t& inPath, const std::string& inComment ) const;

        /**
        * @brief Print in the standard output the current game board and distance map
        */
        void printBoard() const;

        /**
        * @brief Dump the board string, data and distance map into a file. Usefull for debug !
        * @param inPathFileToSave the path of the file to dump the borad string into
        * @return true if the dump has been done successfully
        */
        bool dumpBoardData(const std::string& inPathFileToSave) const;

    private:
        /**
        * @brief Initialize the board of static objects
        * @param inJsonValues the json data describing the game
        */
        void initStaticBoard(const Json::Value& inJsonValues);

        /**
        * @brief Update the current board with the static board and the moving objects
        * 
        * Current hero is not in the current board, only opponents are
        */
        void updateCurrentBoard();

        /**
        * @brief Update the board distances definition according to heros positions
        * @param inBoard The borad to compute the distance map from
        * @param inCellId The cell id to create the distance map from
        * @param outBoardDistance The resulting distance map
        */
        void updateBoardDistances(const std::vector<E_BOARD_OBJECTS>& inBoard, const int inCellId, std::vector<int> &outBoardDistance);

        /**
        * @brief get the smallest distance from the 4-connected cells in the distance map of the given position
        * @param inHeroId the hero id to work with
        * @param inPosition the position to use
        * @param inAvoidHeros set to true to find a path and try to not meet other heros
        * @param outBestDist the best distance found
        * @param outBestDistCellId the cellId in the distance map where the best distance has been found
        * 
        * -1 value in the distance map is a not reachable cell, so -1 values are not processed
        * if many solutions are available, only the last one is returned according the following 4-connected cell order:  0,+1 # +1,0 # 0,-1 # -1,0
        */
        bool getSmallerNeighborValueInDistanceMap(const int inHeroId, const CPosition& inPosition, bool inAvoidHeros, int &outBestDist, int &outBestDistCellId) const;

        /**
        * @brief get the smallest distance from the 4-connected cells in the distance map of the given cellId
        * @param inHeroId the hero id to work with
        * @param inCellId the cellId to use
        * @param inAvoidHeros set to true to find a path and try to not meet other heros
        * @param outBestDist the best distance found
        * @param outBestDistCellId the cellId in the distance map where the best distance has been found
        * 
        * -1 value in the distance map is a not reachable cell, so -1 values are not processed
        * if many solutions are available, only the last one is returned according the following 4-connected cell order:  0,+1 # +1,0 # 0,-1 # -1,0
        * 
        * call of CGame::getSmallerNeighborValueInDistanceMap(const CPosition& inPosition, int &outBestDist, int &outBestDistCellId)
        */
        bool getSmallerNeighborValueInDistanceMap(const int inHeroId, const int inCellId, bool inAvoidHeros, int &outBestDist, int &outBestDistCellId) const;

    private:
        std::string m_id;
        int m_nbPlayers;
        int m_turn;
        int m_maxTurns;
        std::vector<CHero> m_heros;
        int m_boardEdgeSize;
        std::string m_boardStr;
        std::string m_jsonStr;
        bool m_isFinished;
        int m_myHeroId; ///< the id of the current hero to play. Use m_myHeroId-1 to find it in m_heros vector
        std::vector<int> m_opponentHeroIds;

        /// board data
        std::vector<int> m_tavernCellIdsList; ///< the list of tavern cellId (position in 1D buffer)
        std::vector<int> m_goldMineCellIdsList; /// the list of gold mine cellId (position in 1D buffer)
        std::vector<int> m_unownedGoldMineCellIdsList; /// the list of gold mine cellId (position in 1D buffer) which are owned by no one
        std::vector<std::vector<int>> m_boardDistances; ///< all the distances from each hero to each board cell
        std::vector<int> m_boardDistancesAvoidHeros; ///< all the distances from my hero to each board cell and security area
        std::vector<E_BOARD_OBJECTS> m_staticBoard; ///< board defined at the beginning of the game with only static objects
        std::vector<E_BOARD_OBJECTS> m_currentBoard; ///< current board with other heros
        std::vector<E_BOARD_OBJECTS> m_currentBoardAvoidHeros; ///< current board with other heros and security area
}; // class CGame

} // namespace MOBE
