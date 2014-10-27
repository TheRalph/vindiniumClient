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
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <json/json.h>
#include <list>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CGame.h"
#include "CHero.h"

namespace MOBE
{

#define DEBUG_BOARD 0
const int G_NUMBER_OF_PLAYERS = 4;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CGame::CGame(const Json::Value& inJsonValues, const int inMyHeroId)
: m_id(""),
  m_nbPlayers(0),
  m_turn(-1),
  m_maxTurns(-1),
  m_boardEdgeSize(-1),
  m_isFinished(false),
  m_myHeroId(inMyHeroId)
{
    m_id = inJsonValues["id"].asString();
    m_turn = inJsonValues["turn"].asInt();
    m_maxTurns = inJsonValues["maxTurns"].asInt();

    m_opponentHeroIds.reserve(G_NUMBER_OF_PLAYERS-1);
    m_heros.reserve(G_NUMBER_OF_PLAYERS);
    for (auto &hero : inJsonValues["heroes"])
    {
        m_heros.emplace_back(hero);
        m_nbPlayers++;
        CHero &currentHero = m_heros.back();
        currentHero.setMaxLife(currentHero.getLife());
        if (currentHero.getId() != m_myHeroId)
        {
            m_opponentHeroIds.push_back(currentHero.getId());
        } else {}
    } // for

    if (m_nbPlayers != G_NUMBER_OF_PLAYERS)
    {
        std::cerr<<"WARNING: Number of player detected is "<<m_nbPlayers<<" but "<<G_NUMBER_OF_PLAYERS<<" was expected"<<std::endl;
    } else {}

    m_boardEdgeSize = inJsonValues["board"]["size"].asInt();
    m_boardStr = inJsonValues["board"]["tiles"].asString();

    m_isFinished = inJsonValues["finished"].asBool();

    m_jsonStr = inJsonValues.toStyledString();

    initStaticBoard(inJsonValues);
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CGame::~CGame()
{
    // Nothing to do
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::initStaticBoard(const Json::Value& inJsonValues)
{
    m_tavernCellIdsList.clear();
    m_goldMineCellIdsList.clear();
    m_staticBoard.clear();
    m_staticBoard.reserve(m_boardEdgeSize*m_boardEdgeSize);
    m_boardDistances.clear();
    m_boardDistances.resize(m_nbPlayers);

    /// build static board
    const char *pChar = m_boardStr.data();
    for (int i = 0; i<m_boardEdgeSize; i++)
    {
        for (int j = 0; j<m_boardEdgeSize*2; j+=2)
        {
            // ## Impassable wood
            // @1 Hero number 1
            // [] Tavern
            // $- Gold mine (neutral)
            // $1 Gold mine (belonging to hero 1)
            switch (pChar[0])
            {
                case '[': // Tavern
                    m_tavernCellIdsList.emplace_back( get1DCoordOnBoard(j/2, i) );
                    m_staticBoard.emplace_back(E_TAVERN);
                    break;
                case '$': // Gold mine
                    m_goldMineCellIdsList.emplace_back( get1DCoordOnBoard(j/2, i) );
                    m_unownedGoldMineCellIdsList.emplace_back( m_goldMineCellIdsList.back() ); /// mines are owned by no one at beginning
                    m_staticBoard.emplace_back(E_GOLD_MINE);
                    break;
                case ' ': // empty cell
                    m_staticBoard.emplace_back(E_NO_OBJECT);
                    break;
                case '@': // Hero
                    m_staticBoard.emplace_back(E_NO_OBJECT); /// heros are moving, they must not be into static board
                    break;
                case '#': // Impassable wood
                default: // by default Impassable wood
                    m_staticBoard.emplace_back(E_IMPASSABLE_WOOD);
                    break;
            } // switch
            pChar+=2;
        } // for
    } // for

    update(inJsonValues);
} // initStaticBoard

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::updateBoardDistances(const std::vector<E_BOARD_OBJECTS>& inBoard, const int inCellId, std::vector<int> &outBoardDistance)
{
    outBoardDistance.clear();
    outBoardDistance.resize(inBoard.size(), -1);

    /// update board distances from first empty cell
    std::list<int> cellsToProcess;
    cellsToProcess.push_back(inCellId);
    cellsToProcess.push_back(0);

    while (!cellsToProcess.empty())
    {
        const int cellId = cellsToProcess.front();
        cellsToProcess.pop_front();
        const int baseDistance = cellsToProcess.front();
        cellsToProcess.pop_front();

        // set the current cell value
        outBoardDistance.at(cellId) = baseDistance;

        // stack the next cells to manage
        const CPosition cellPosition( get2DCoordOnBoard(cellId) );
        const std::vector<CPosition> the4Connecteds = cellPosition.get4Connected();
        for (const CPosition &connectedCell : the4Connecteds)
        {
            if ( connectedCell.getX() >= 0 && connectedCell.getX() < m_boardEdgeSize &&
                 connectedCell.getY() >= 0 && connectedCell.getY() < m_boardEdgeSize )
            {
                const int localCellid = get1DCoordOnBoard(connectedCell);
                if ( inBoard.at(localCellid) == E_NO_OBJECT &&
                     outBoardDistance.at(localCellid) == -1 )
                {
                    cellsToProcess.push_back(localCellid);
                    cellsToProcess.push_back(baseDistance+1);
                    outBoardDistance.at(localCellid) = -2;
                } else {}
            } else {}
        } // for
    } // while

#if (DEBUG_BOARD == 1)
    printBoard();
#endif
} // updateBoardDistances

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::update(const Json::Value& inJsonValues)
{
    m_isFinished = inJsonValues["finished"].asBool();
    m_turn = inJsonValues["turn"].asInt();
    m_boardStr = inJsonValues["board"]["tiles"].asString();
    m_jsonStr = inJsonValues.toStyledString();

    for (auto &heroJSon : inJsonValues["heroes"])
    {
        CHero &hero = getHero(heroJSon["id"].asInt());
        hero.update(heroJSon);
        hero.getOwnedGoldMineCellIds().clear();
    } // for

    m_unownedGoldMineCellIdsList.clear();
    for ( const int& goldMineCellId : m_goldMineCellIdsList )
    {
        CPosition goldMinePosition(get2DCoordOnBoard(goldMineCellId));
        char owner = m_boardStr.at(goldMinePosition.getY()*m_boardEdgeSize*2 + goldMinePosition.getX()*2+1);
        if (owner == '-')
        {
            // no owner
            m_unownedGoldMineCellIdsList.emplace_back(goldMineCellId);
        }
        else
        {
            const unsigned int heroId = owner-(int)'0';
            if (heroId > 0 && heroId <= m_heros.size())
            {
                CHero &hero = getHero(heroId);
                hero.getOwnedGoldMineCellIds().emplace_back(goldMineCellId);
            } else
            {
                std::cerr<<"unknown hero with id="<<heroId<<std::endl;
            }
        } // else
    } // for

    updateCurrentBoard();
    int heroId = 1;
    for (std::vector<int>& boardDistance : m_boardDistances )
    {
        updateBoardDistances(m_currentBoard, get1DCoordOnBoard(getHero(heroId).getPosition()), boardDistance);
        heroId++;
    } // for
    updateBoardDistances(m_currentBoardAvoidHeros, get1DCoordOnBoard(getMyHero().getPosition()), m_boardDistancesAvoidHeros);
} // update

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::updateCurrentBoard()
{
    /// update heros positions on current board
    m_currentBoard = m_staticBoard;
    m_currentBoardAvoidHeros = m_staticBoard;
    for (CHero &hero : m_heros)
    {
        const CPosition& heroPos = hero.getPosition();
        const int heroCellId = get1DCoordOnBoard(heroPos);
        m_currentBoard.at(heroCellId) = E_HERO;
        m_currentBoardAvoidHeros.at(heroCellId) = E_HERO;
#if 0
        const std::vector<CPosition> security(heroPos.get8Connected());
        for (const CPosition& posSecurity : security)
        {
            if ( posSecurity.getX() >= 0 && posSecurity.getX() < m_boardEdgeSize &&
                 posSecurity.getY() >= 0 && posSecurity.getY() < m_boardEdgeSize )
            {
                m_currentBoardAvoidHeros.at(get1DCoordOnBoard(posSecurity)) = E_IMPASSABLE_WOOD;
            } else {}
        } // for
#endif
    } // for
} // updateCurrentBoard

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getOpponentIdWithMaxMineCount(int &outOpponentIdWithMaxMineCount, int &outMaxMineCount) const
{
    outOpponentIdWithMaxMineCount = -1;
    outMaxMineCount = 0;
    for (const int &opponentHeroId : m_opponentHeroIds)
    {
        const CHero& opponentHero = getHero(opponentHeroId);
        const int opponentHeroMineCount = opponentHero.getMineCount();
        if ( opponentHeroMineCount > outMaxMineCount )
        {
            outMaxMineCount = opponentHeroMineCount;
            outOpponentIdWithMaxMineCount = opponentHeroId;
        } else {}
    } // for

    return (outOpponentIdWithMaxMineCount >= 0);
} // getHeroIdWithMaxMineCount

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getClosestOpponent(int &outOpponentId, int &outOpponentDistance) const
{
    outOpponentId = -1;
    outOpponentDistance = std::numeric_limits<int>::max();
    for (const int &opponentHeroId : m_opponentHeroIds)
    {
        const CHero &opponent = getHero(opponentHeroId);
        if (!opponent.isCrashed())
        {
            const int opponentCellId = get1DCoordOnBoard(opponent.getPosition());
            const int opponentDistance = getDistanceTo( getMyHeroId(), opponentCellId, false );
            if ( opponentDistance >= 0 && opponentDistance < outOpponentDistance)
            {
                outOpponentDistance = opponentDistance;
                outOpponentId = opponentHeroId;
            } else {}
        } else {}
    } // for
    bool retVal = (outOpponentId >= 0);
    if (!retVal)
    {
        outOpponentDistance = -1;
    } else {}
    return retVal;
} // getClosestOpponent

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getClosestOpponentPath(int &outOpponentId, path_t &outClosestOpponentPath) const
{
    outClosestOpponentPath.clear();
    outOpponentId = -1;
    int minOpponentDistance = -1;

    bool pathFound = false;
    if (getClosestOpponent(outOpponentId, minOpponentDistance))
    {
        const int opponentCellId = get1DCoordOnBoard(getHero(outOpponentId).getPosition());
        pathFound = getPathTo(getMyHeroId(), opponentCellId, false, outClosestOpponentPath);
    } else {}

    return pathFound;
} // getClosestOpponentPath

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getClosestTavernDistance(const int inHeroId, int &outTavernCellId, int& outBestDist, bool inAvoidHeros) const
{
    outTavernCellId = -1;
    outBestDist = std::numeric_limits<int>::max();
    for (const int& tavernCellId : m_tavernCellIdsList)
    {
        const int tavernDistance = getDistanceTo(inHeroId, tavernCellId, inAvoidHeros );
        if ( tavernDistance >= 0 && tavernDistance < outBestDist)
        {
            outBestDist = tavernDistance;
            outTavernCellId = tavernCellId;
        } else {}
    } // for
    bool retVal = true;
    if (outTavernCellId < 0)
    {
        retVal = false;
        outBestDist = -1;
    } else {}
    return retVal;
} // getClosestTavernDistance

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getClosestTavernPath(const int inHeroId, int &outTavernCellId, path_t &outClosestTavernPath, bool inAvoidHeros) const
{
    int minDist = std::numeric_limits<int>::max();
    outClosestTavernPath.clear();
    bool pathFound = false;
    if (getClosestTavernDistance(inHeroId, outTavernCellId, minDist, inAvoidHeros))
    {
        pathFound = getPathTo(inHeroId, outTavernCellId, inAvoidHeros, outClosestTavernPath);
    } else {}

    return pathFound;
} // getClosestTavernPath

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getClosestGoldMineCellIdMyHeroDoNotControl(int &outGoldMineCellId, int &outGoldMineDistance, bool inAvoidHeros) const
{
    outGoldMineCellId = -1;
    outGoldMineDistance = std::numeric_limits<int>::max()-1;
    /// first the mines owned by no one
    for (const int& goldMineCellId : m_unownedGoldMineCellIdsList)
    {
        const int goldMineDistance = getDistanceTo( getMyHeroId(), goldMineCellId, inAvoidHeros);
        if ( goldMineDistance >= 0 && goldMineDistance < outGoldMineDistance)
        {
            outGoldMineDistance = goldMineDistance;
            outGoldMineCellId = goldMineCellId;
        } else {}
    } // for

    /// second mines owned by other heros
    for (const int& heroId : m_opponentHeroIds)
    {
        const CHero& hero = getHero(heroId);
        for (const int& goldMineCellId : hero.getOwnedGoldMineCellIds())
        {
            const int goldMineDistance = getDistanceTo( getMyHeroId(), goldMineCellId, inAvoidHeros);
            if ( goldMineDistance >= 0 && goldMineDistance < outGoldMineDistance)
            {
                outGoldMineDistance = goldMineDistance;
                outGoldMineCellId = goldMineCellId;
            } else {}
        } // for
    } // for
    bool retVal = (outGoldMineCellId >= 0);
    if (!retVal)
    {
        outGoldMineDistance = -1;
    } else {}
    return retVal;
} // getClosestGoldMineCellIdMyHeroDoNotControl

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getClosestGoldMineMyHeroDoNotControlPath(int &outGoldMineCellId, path_t &outClosestGoldMinePath, bool inAvoidHeros) const
{
    outClosestGoldMinePath.clear();
    outGoldMineCellId = -1;
    int minGoldMineDistance = std::numeric_limits<int>::max()-1;

    bool pathFound = false;
    if (getClosestGoldMineCellIdMyHeroDoNotControl(outGoldMineCellId, minGoldMineDistance, inAvoidHeros))
    {
        pathFound = getPathTo(getMyHeroId(), outGoldMineCellId, inAvoidHeros, outClosestGoldMinePath);
    } else {}

    return pathFound;
} // getClosestTavernPath

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getSmallerNeighborValueInDistanceMap(const int inHeroId, const int inCellId, bool inAvoidHeros, int &outBestDist, int &outBestDistCellId) const
{
    const CPosition cellPos = get2DCoordOnBoard(inCellId);
    return getSmallerNeighborValueInDistanceMap(inHeroId, cellPos, inAvoidHeros, outBestDist, outBestDistCellId);
} // getSmallerNeighborValueInDistanceMap

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getSmallerNeighborValueInDistanceMap(const int inHeroId, const CPosition& inPosition, bool inAvoidHeros, int &outBestDist, int &outBestDistCellId) const
{
    const std::vector <int> &boardDistance = ((inAvoidHeros)? m_boardDistancesAvoidHeros:getBoardDistancesOfHero(inHeroId));

    outBestDist = std::numeric_limits<int>::max();
    outBestDistCellId = -1;
    const std::vector<CPosition> the4Connecteds = inPosition.get4Connected();
    for (const CPosition& neighborPos : the4Connecteds)
    {
        if ( neighborPos.getX() >= 0 && neighborPos.getX() < m_boardEdgeSize &&
             neighborPos.getY() >= 0 && neighborPos.getY() < m_boardEdgeSize )
        {
            const int neighborId = get1DCoordOnBoard(neighborPos);
            const int neighborDist = boardDistance.at(neighborId);

            if (neighborDist >= 0 && neighborDist < outBestDist)
            {
                outBestDist = neighborDist;
                outBestDistCellId = neighborId;
            } else {}
        } else {}
    } // for
    return (outBestDistCellId >= 0);
} // getSmallerNeighborValueInDistanceMap

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int CGame::getDistanceTo(const int inHeroId, const CPosition& inPosition, bool inAvoidHeros) const
{
    const int cellId = get1DCoordOnBoard(inPosition);
    return getDistanceTo(inHeroId, cellId, inAvoidHeros);
} // getDistanceTo

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int CGame::getDistanceTo(const int inHeroId, const int inCellId, bool inAvoidHeros) const
{
    const std::vector <int> &boardDistance = ((inAvoidHeros)? m_boardDistancesAvoidHeros:getBoardDistancesOfHero(inHeroId));
    const std::vector <MOBE::E_BOARD_OBJECTS> &board = ((inAvoidHeros)? m_currentBoardAvoidHeros:m_currentBoard);

    int distance = boardDistance.at(inCellId);

    /// check if it is an hero, a mine or a tavern
    if (distance < 0)
    {
        const E_BOARD_OBJECTS object = board.at(inCellId);
        if (object != E_IMPASSABLE_WOOD)
        {
            int bestDist = 0;
            int bestDistCellId = 0;
             // if a neighbor can be reached, update the distance by 1 to reach it !
            if (getSmallerNeighborValueInDistanceMap(inHeroId, inCellId, inAvoidHeros, bestDist, bestDistCellId))
            {
                distance = 1 + bestDist;
            } else {}
        } else {}
    } else {}

    return distance;
} // getDistanceTo

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getPathTo(const int inHeroId, const int inCellId, bool inAvoidHeros, path_t& outPath) const
{
    outPath.clear();
    int cellId = inCellId;
    int distance = getDistanceTo(inHeroId, cellId, inAvoidHeros);

//CPosition pos(get2DCoordOnBoard(cellId));
//std::cout<<pos.getX()<<" "<<pos.getY()<<" distance="<<distance<<std::endl;
    /// check if at least a path is available
    while (distance > 0)
    {
        // add the current cellId
        outPath.emplace_front(cellId);

        // the neighbor must be reachable because we know a distance exists
        getSmallerNeighborValueInDistanceMap(inHeroId, cellId, inAvoidHeros, distance, cellId);
//std::cout<<"distance="<<distance<<std::endl;
    } // while

    return !outPath.empty();
} // getPathTo

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::print() const
{
    std::cout<<"Game Id='"<<m_id<<"'"<<std::endl;
    std::cout<<"Turn="<<m_turn<<std::endl;
    std::cout<<"MaxTurns="<<m_maxTurns<<std::endl;
    for (const CHero &hero:m_heros)
    {
        hero.print();
    } // for
    std::cout<<"Board edge size="<<m_boardEdgeSize<<std::endl;
    std::cout<<"Board="<<std::endl;
    const char *pChar = m_boardStr.data();
    for (int i = 0; i<m_boardEdgeSize; i++)
    {
        for (int j = 0; j<m_boardEdgeSize*2; j++)
        {
            std::cout<<(*pChar++);
        } // for
        std::cout<<std::endl;
    } // for

    std::cout<<"IsFinished="<<(m_isFinished? "true":"false")<<std::endl;

    for (const int tavenCellId : m_tavernCellIdsList)
    {
        const CPosition tavernPosition(get2DCoordOnBoard(tavenCellId));
        std::cout<<"Tavern position="<<tavernPosition.getX()<<" , "<<tavernPosition.getY()<<std::endl;
    } // for

    for (const int goldMineCellId : m_goldMineCellIdsList)
    {
        const CPosition goldMinePosition(get2DCoordOnBoard(goldMineCellId));
        std::cout<<"Gold Mine position="<<goldMinePosition.getX()<<" , "<<goldMinePosition.getY()<<std::endl;
    } // for

    printBoard();
} // print

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::printPath( const path_t& inPath, const std::string& inComment ) const
{
    std::cout<<inComment;

    for ( const int cellId : inPath )
    {
        const CPosition cellPos = get2DCoordOnBoard(cellId);
        std::cout<<" ( "<<cellPos.getX()<<","<<cellPos.getY()<<" )" ;
    } // for

    std::cout<<std::endl;
} // printPath

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::printBoard() const
{
    std::cout<<"Current board:"<<std::endl;
    for (int i = 0, cellId = 0; i<m_boardEdgeSize; i++)
    {
        for (int j = 0; j<m_boardEdgeSize; j++)
        {
            std::cout<<std::setw(4)<<m_currentBoard.at(cellId++);
        } // for
        std::cout<<std::endl;
    } // for

    std::cout<<"Distances:"<<std::endl;
    for (int heroId = 1; heroId <= getNbPlayers(); heroId++)
    {
        std::cout<<getHero(heroId).getName()<<std::endl;
        for (int i = 0, cellId = 0; i<m_boardEdgeSize; i++)
        {
            for (int j = 0; j<m_boardEdgeSize; j++)
            {
                std::cout<<std::setw(4)<<m_boardDistances.at(heroId-1).at(cellId++);
            } // for
            std::cout<<std::endl;
        } // for
    } // for
} // printBoard

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::dumpBoardData(const std::string& inPathFileToSave) const
{
    bool retVal = false;
    std::ofstream file(inPathFileToSave);
    if (file)
    {
        file<<m_jsonStr;
        retVal = file.good();
    } else {}
    return retVal;
} // dumpBoardData

} // namespace MOBE
