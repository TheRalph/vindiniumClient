////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <iomanip>
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

    initStaticBoard();
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CGame::~CGame()
{
    // Nothing to do
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::initStaticBoard()
{
    m_tavernCellIdsList.clear();
    m_goldMineCellIdsList.clear();
    m_staticBoard.clear();
    m_staticBoard.reserve(m_boardEdgeSize*m_boardEdgeSize);

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

    updateCurrentBoard();
    updateBoardDistances();
} // initStaticBoard

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::updateBoardDistances()
{
    std::vector<E_BOARD_OBJECTS> localBoard(m_currentBoard);
    m_boardDistances.clear();
    m_boardDistances.resize(m_staticBoard.size(), -1);

    const int currentHeroPosId = get1DCoordOnBoard(getMyHero().getPosition());
    /// update board distances from first empty cell
    std::list<int> cellsToProcess;
    cellsToProcess.push_back(currentHeroPosId);
    cellsToProcess.push_back(0);

    while (!cellsToProcess.empty())
    {
        const int cellId = cellsToProcess.front();
        cellsToProcess.pop_front();
        const int baseDistance = cellsToProcess.front();
        cellsToProcess.pop_front();

        // set the current cell value
        m_boardDistances.at(cellId) = baseDistance;

        // stack the next cells to manage
        const CPosition cellPosition( get2DCoordOnBoard(cellId) );
        const std::vector<CPosition> the4Connecteds = cellPosition.get4Connected();
        for (const CPosition &connectedCell : the4Connecteds)
        {
            if ( connectedCell.getX() >= 0 && connectedCell.getX() < m_boardEdgeSize &&
                 connectedCell.getY() >= 0 && connectedCell.getY() < m_boardEdgeSize )
            {
                const int localCellid = get1DCoordOnBoard(connectedCell);
                if ( localBoard.at(localCellid) == E_NO_OBJECT &&
                     m_boardDistances.at(localCellid) == -1 )
                {
                    cellsToProcess.push_back(localCellid);
                    cellsToProcess.push_back(baseDistance+1);
                    m_boardDistances.at(localCellid) = -2;
                } else {}
            } else {}
        } // for
    } // while

#if 0
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
    for (int i = 0, cellId = 0; i<m_boardEdgeSize; i++)
    {
        for (int j = 0; j<m_boardEdgeSize; j++)
        {
            std::cout<<std::setw(4)<<m_boardDistances.at(cellId++);
        } // for
        std::cout<<std::endl;
    } // for
#endif
} // updateBoardDistances

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::update(const Json::Value& inJsonValues)
{
    m_isFinished = inJsonValues["finished"].asBool();
    m_turn = inJsonValues["turn"].asInt();
    m_boardStr = inJsonValues["board"]["tiles"].asString();

    /// heros come always in the same order !
    int i = 0;
    for (auto &heroJSon : inJsonValues["heroes"])
    {
        CHero &hero = m_heros.at(i++);
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
            const unsigned int heroId = owner-(int)'1';
            if (heroId >= 0 && heroId < m_heros.size())
            {
                CHero &hero = m_heros.at(heroId);
                hero.getOwnedGoldMineCellIds().emplace_back(goldMineCellId);
            } else
            {
                std::cerr<<"unknown hero with id="<<heroId+1<<std::endl;
            }
        } // else
    } // for

    updateCurrentBoard();
    updateBoardDistances();
} // update

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::updateCurrentBoard()
{
    /// update heros positions on current board
    m_currentBoard = m_staticBoard;
    for (CHero &hero : m_heros)
    {
        if (hero.getId() != getMyHeroId())
        {
            m_currentBoard.at(get1DCoordOnBoard(hero.getPosition())) = E_HERO;
        } else {}
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
bool CGame::getClosestTavernPath(int &outTavernCellId, path_t &outClosestTavernPath) const
{
    outClosestTavernPath.clear();
    outTavernCellId = -1;
    int minTavernDistance = std::numeric_limits<int>::max();
    for (const int& tavernCellId : m_tavernCellIdsList)
    {
        const int tavernDistance = getDistanceTo( tavernCellId );
        if ( tavernDistance >= 0 && tavernDistance < minTavernDistance)
        {
            minTavernDistance = tavernDistance;
            outTavernCellId = tavernCellId;
        } else {}
    } // for

    bool pathFound = false;
    if (outTavernCellId >= 0)
    {
        pathFound = getPathTo(outTavernCellId, outClosestTavernPath);
    } else {}

    return pathFound;
} // getClosestTavernPath

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getClosestGoldMineMyHeroDoNotControlPath(int &outGoldMineCellId, path_t &outClosestGoldMinePath) const
{
    outClosestGoldMinePath.clear();
    outGoldMineCellId = -1;
    int minGoldMineDistance = std::numeric_limits<int>::max();

    /// first the mines owned by no one
    for (const int& goldMineCellId : m_unownedGoldMineCellIdsList)
    {
        const int goldMineDistance = getDistanceTo( goldMineCellId );
        if ( goldMineDistance >= 0 && goldMineDistance < minGoldMineDistance)
        {
            minGoldMineDistance = goldMineDistance;
            outGoldMineCellId = goldMineCellId;
        } else {}
    } // for

    /// second mines owned by other heros
    for (const int& heroId : m_opponentHeroIds)
    {
        const CHero& hero = getHero(heroId);
        for (const int& goldMineCellId : hero.getOwnedGoldMineCellIds())
        {
            const int goldMineDistance = getDistanceTo( goldMineCellId );
            if ( goldMineDistance < minGoldMineDistance)
            {
                minGoldMineDistance = goldMineDistance;
                outGoldMineCellId = goldMineCellId;
            } else {}
        } // for
    } // for

    bool pathFound = false;
    if (outGoldMineCellId >= 0)
    {
        pathFound = getPathTo(outGoldMineCellId, outClosestGoldMinePath);
    } else {}

    return pathFound;
} // getClosestTavernPath

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getSmallerNeighborValueInDistanceMap(const int inCellId, int &outBestDist, int &outBestDistCellId) const
{
    const CPosition cellPos = get2DCoordOnBoard(inCellId);
    return getSmallerNeighborValueInDistanceMap(cellPos, outBestDist, outBestDistCellId);
} // getSmallerNeighborValueInDistanceMap

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getSmallerNeighborValueInDistanceMap(const CPosition& inPosition, int &outBestDist, int &outBestDistCellId) const
{
    outBestDist = std::numeric_limits<int>::max();
    outBestDistCellId = -1;
    const std::vector<CPosition> the4Connecteds = inPosition.get4Connected();
    for (const CPosition& neighborPos : the4Connecteds)
    {
        if ( neighborPos.getX() >= 0 && neighborPos.getX() < m_boardEdgeSize &&
             neighborPos.getY() >= 0 && neighborPos.getY() < m_boardEdgeSize )
        {
            const int neighborId = get1DCoordOnBoard(neighborPos);
            const int neighborDist = m_boardDistances.at(neighborId);

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
int CGame::getDistanceTo(const CPosition& inPosition) const
{
    const int cellId = get1DCoordOnBoard(inPosition);
    return getDistanceTo(cellId);
} // getDistanceTo

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int CGame::getDistanceTo(const int inCellId) const
{
    int distance = m_boardDistances.at(inCellId);

    /// check if it is an hero, a mine or a tavern
    if (distance < 0)
    {
        const E_BOARD_OBJECTS object = m_currentBoard.at(inCellId);
        if (object != E_IMPASSABLE_WOOD)
        {
            int bestDist = 0;
            int bestDistCellId = 0;
             // if a neighbor can be reached, update the distance by 1 to reach it !
            if (getSmallerNeighborValueInDistanceMap(inCellId, bestDist, bestDistCellId))
            {
                distance = 1 + bestDist;
            } else {}
        } else {}
    } else {}

    return distance;
} // getDistanceTo

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CGame::getPathTo(const int inCellId, path_t& outPath) const
{
    outPath.clear();
    int cellId = inCellId;
    int distance = getDistanceTo(cellId);

//CPosition pos(get2DCoordOnBoard(cellId));
//std::cout<<pos.getX()<<" "<<pos.getY()<<" distance="<<distance<<std::endl;
    /// check if at least a path is available
    while (distance > 0)
    {
        // add the current cellId
        outPath.emplace_front(cellId);

        // the neighbor must be reachable because we know a distance exists
        getSmallerNeighborValueInDistanceMap(cellId, distance, cellId);
//std::cout<<"distance="<<distance<<std::endl;
    } // while

    return !outPath.empty();
} // getPathTo

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::print()
{
    std::cout<<"Game Id='"<<m_id<<"'"<<std::endl;
    std::cout<<"Turn="<<m_turn<<std::endl;
    std::cout<<"MaxTurns="<<m_maxTurns<<std::endl;
    for (auto &hero:m_heros)
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
    for (int i = 0, cellId = 0; i<m_boardEdgeSize; i++)
    {
        for (int j = 0; j<m_boardEdgeSize; j++)
        {
            std::cout<<std::setw(4)<<m_boardDistances.at(cellId++);
        } // for
        std::cout<<std::endl;
    } // for
} // print

} // namespace MOBE
