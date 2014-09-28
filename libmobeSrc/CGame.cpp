////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <iomanip>
#include <list>
#include <json/json.h>

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

    m_heros.reserve(G_NUMBER_OF_PLAYERS);
    for (auto &hero : inJsonValues["heroes"])
    {
        m_heros.emplace_back(hero);
        m_nbPlayers++;
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
    m_tavernPositionsList.clear();
    m_goldMinePositionsList.clear();
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
                    m_tavernPositionsList.emplace_back(j/2, i);
                    m_staticBoard.emplace_back(E_TAVERN);
                    break;
                case '$': // Gold mine
                    m_goldMinePositionsList.emplace_back(j/2, i);
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

    const int nbCellsInBoard = localBoard.size();
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
            const int localCellid = get1DCoordOnBoard(connectedCell);
            if ( localCellid >= 0 &&
                 localCellid < nbCellsInBoard &&
                 localBoard.at(localCellid) == E_NO_OBJECT &&
                 m_boardDistances.at(localCellid) == -1
               )
            {
                cellsToProcess.push_back(localCellid);
                cellsToProcess.push_back(baseDistance+1);
                m_boardDistances.at(localCellid) = -2;
            } else {}
        } // for
    } // while
} // updateBoardDistances

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::update(const Json::Value& inJsonValues)
{
    m_isFinished = inJsonValues["finished"].asBool();
    m_turn = inJsonValues["turn"].asInt();

    /// heros come always in the same order !
    int i = 0;
    for (auto &hero : inJsonValues["heroes"])
    {
        m_heros.at(i++).update(hero);
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
int CGame::getDistanceTo(const CPosition& inPosition)
{
    const int cellId = get1DCoordOnBoard(inPosition);
    int distance = m_boardDistances.at(cellId);

    /// check if it is an hero, a mine or a tavern
    if (distance < 0)
    {
        const E_BOARD_OBJECTS object = m_currentBoard.at(cellId);
        if (object != E_IMPASSABLE_WOOD)
        {
            const std::vector<CPosition> the4Connecteds = inPosition.get4Connected();
            int bestDist = m_boardEdgeSize*2;
            for (const CPosition& neighbor : the4Connecteds)
            {
                const int neighborId = get1DCoordOnBoard(neighbor);
                const int neighborDist = m_boardDistances.at(neighborId);
                if (neighborDist >= 0 && neighborDist < bestDist)
                {
                    bestDist = neighborDist;
                } else {}
            } // for
            // if a neighbor can be reached, update the distance
            if (bestDist < m_boardEdgeSize*2)
            {
                distance = 1 + bestDist;
            } else {}
        } else {}
    } else {}

    return distance;
} // getDistanceTo

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

    for (CPosition &tavernPos : m_tavernPositionsList)
    {
        std::cout<<"Tavern position="<<tavernPos.getX()<<" , "<<tavernPos.getY()<<std::endl;
    } // for

    for (CPosition &goldMinePos : m_goldMinePositionsList)
    {
        std::cout<<"Gold Mine position="<<goldMinePos.getX()<<" , "<<goldMinePos.getY()<<std::endl;
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
