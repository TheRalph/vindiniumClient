////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <json/json.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CGame.h"
#include "CHero.h"

namespace VDC
{

const int G_NUMBER_OF_PLAYERS = 4;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CGame::CGame(const Json::Value& inJsonValues)
: m_id(""),
  m_nbPlayers(0),
  m_turn(-1),
  m_maxTurns(-1),
  m_boardEdgeSize(-1),
  m_isFinished(false)
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
    m_board = inJsonValues["board"]["tiles"].asString();

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

    const char *pChar = m_board.data();
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
                    break;
                case '#': // Impassable wood
                    break;
                case '$': // Gold mine
                    if (pChar[0] == '-')
                    {
                        // TODO: fill free gold mine
                    }
                    else
                    {
                        // TODO: fill players gold mine
                    } // else
                    break;
                case '@': // Hero, nothing to do
                default:
                    break;
            } // switch
            pChar+=2;
        } // for
    } // for
} // parseBoard

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
    const char *pChar = m_board.data();
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
} // print

} // namespace VDC