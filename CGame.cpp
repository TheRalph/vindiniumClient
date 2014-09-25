////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <iomanip>
#include <queue>
#include <json/json.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CGame.h"
#include "CHero.h"
#include "CTicTac.h"

namespace VDC
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

    parseBoard();
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CGame::~CGame()
{
    // Nothing to do
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::parseBoard()
{
    m_tavernPositionsList.clear();
    m_goldMinePositionsList.clear();
    m_board.clear();
    m_board.reserve(m_boardEdgeSize*m_boardEdgeSize);

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
                    m_board.push_back(E_TAVERN);
                    break;
                case '$': // Gold mine
                    m_goldMinePositionsList.emplace_back(j/2, i);
                    m_board.push_back(E_GOLD_MINE);
                    break;
                case '@': // Hero, nothing to do
//                    m_board.push_back(E_HERO); ///<======= WARNING: probably to not manae heros here!
//                    break;
                case ' ': // empty cell
                    m_board.push_back(E_NO_OBJECT);
                    break;
                case '#': // Impassable wood
                default: // by default Impassable wood
                    m_board.push_back(E_IMPASSABLE_WOOD);
                    break;
            } // switch
            pChar+=2;
        } // for
    } // for

    updateBoardDistances();
} // parseBoard

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CGame::updateBoardDistances()
{
    std::vector<E_BOARD_OBJECTS> localBoard(m_board);
    m_boardDistances.clear();
    m_boardDistances.resize(m_board.size(), -1);

    /// update heros positions
    for (CHero &hero : m_heros)
    {
        if (hero.getId() != getMyHeroId())
        {
            localBoard.at(get1DCoordOnBoard(hero.getPosition())) = E_HERO;
        } else {}
    } // for

    const int nbCellsInBoard = localBoard.size();
    const int currentHeroPosId = get1DCoordOnBoard(getMyHero().getPosition());
    /// update board distances from first empty cell
    std::queue<int> m_cellsToProcess;
    m_cellsToProcess.emplace(currentHeroPosId);
    m_cellsToProcess.emplace(0);

    while (!m_cellsToProcess.empty())
    {
        const int cellId = m_cellsToProcess.front();
        m_cellsToProcess.pop();
        const int baseDistance = m_cellsToProcess.front();
        m_cellsToProcess.pop();

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
                 m_board.at(localCellid) == E_NO_OBJECT &&
                 m_boardDistances.at(localCellid) < 0
               )
            {
                m_cellsToProcess.emplace(localCellid);
                m_cellsToProcess.emplace(baseDistance+1);
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

    updateBoardDistances();
} // update

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

    std::cout<<"Board:"<<std::endl;
    for (int i = 0, cellId = 0; i<m_boardEdgeSize; i++)
    {
        for (int j = 0; j<m_boardEdgeSize; j++)
        {
            std::cout<<std::setw(4)<<m_board.at(cellId++);
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

} // namespace VDC
