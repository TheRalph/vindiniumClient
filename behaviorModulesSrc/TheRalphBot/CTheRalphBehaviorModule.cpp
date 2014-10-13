////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CTheRalphBehaviorModule.h"

namespace MOBE
{

CREATE_C_SYMBOLS( CTheRalphBehaviorModule )

enum E_THE_RALPH_BOT_STATES
{
    E_CONQUER_GOLD_MINES = 0,
    E_NEED_LIFE,
    E_AGGRESSIVE,
    E_AGGRESSIVE_CLOSE,
    NB_THE_RALPH_BOT_STATES
}; // enum E_THE_RALPH_BOT_STATES

static const std::string G_THE_RALPH_BOT_STATES_DICTIONARY[NB_THE_RALPH_BOT_STATES]={
    "CONQUER_GOLD_MINES",
    "NEED_LIFE",
    "AGGRESSIVE",
    "AGGRESSIVE_CLOSE"
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CTheRalphBehaviorModule::CTheRalphBehaviorModule()
: m_isInitialized(false),
  m_status(E_CONQUER_GOLD_MINES)
{
    m_version = 0;
    m_name    = "TheRalphBot";
    m_comment = "The Ralph AI";

    srand(time(NULL)); // init random
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CTheRalphBehaviorModule::~CTheRalphBehaviorModule()
{
    // Nothing
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CTheRalphBehaviorModule::initialize()
{
    if (m_pBehaviorMgr && !m_isInitialized)
    {
        m_pNeedLife = m_pBehaviorMgr->getBehavior("needLife");
        if (!m_pNeedLife)
        {
            std::cerr<<"Can not load behavior 'needLife'"<<std::endl;
        } else {}

        m_pAggressive = m_pBehaviorMgr->getBehavior("aggressive");
        if (!m_pAggressive)
        {
            std::cerr<<"Can not load behavior 'aggressive'"<<std::endl;
        } else {}

        m_pConquerGoldMine = m_pBehaviorMgr->getBehavior("conquerGoldMine");
        if (!m_pConquerGoldMine)
        {
            std::cerr<<"Can not load behavior 'conquerGoldMine'"<<std::endl;
        } else {}

        m_pAggressiveClose = m_pBehaviorMgr->getBehavior("aggressiveClose");
        if (!m_pAggressiveClose)
        {
            std::cerr<<"Can not load behavior 'aggressiveClose'"<<std::endl;
        } else {}

        m_isInitialized = (m_pNeedLife && m_pAggressive && m_pConquerGoldMine && m_pAggressiveClose);
    } else {}
    return m_isInitialized;
} // initialize

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CTheRalphBehaviorModule::playBehavior(const CGame& inGame)
{
    E_BEHAVIOR_ACTIONS nextAction = E_ACTION_STAY;

    if (initialize())
    {
        const CHero &myHero = inGame.getMyHero();
        int closestOpponentDistance = -1;
        int closestOpponentId = -1;
        const bool isClosestOpponentAvailable = inGame.getClosestOpponent(closestOpponentId, closestOpponentDistance);
        int closestOpponentClosestTavernId = -1;
        int closestOpponentClosestTavernDistance = -1;;
        const bool closestOpponentTavernAccess = inGame.getClosestTavernDistance(closestOpponentId, closestOpponentClosestTavernId, closestOpponentClosestTavernDistance);

        if ( isClosestOpponentAvailable   &&
             closestOpponentDistance <= 1 &&
             (closestOpponentClosestTavernDistance > 1 || !closestOpponentTavernAccess)
           )
        {
            m_status = E_AGGRESSIVE_CLOSE;
        }
        else if ( myHero.getMineCount() == inGame.getNbMines()                          ||
             ( m_status != E_NEED_LIFE && myHero.getLife() < 0.25*myHero.getMaxLife() ) ||
             ( m_status == E_NEED_LIFE && myHero.getLife() < 0.98*myHero.getMaxLife() ) )
        {
            m_status = E_NEED_LIFE;
        }
        else
        {
            int closestGoldMineDistance = -1;
            int closestGoldMineCellId = -1;
//            int opponentIdWithMaxMineCount = -1;
//            int MaxMineCountOfOpponent = -1;

            const bool isGoldMineAvailable = inGame.getClosestGoldMineCellIdMyHeroDoNotControl(closestGoldMineCellId, closestGoldMineDistance);
//            const bool isOpponentWithMaxCountAvailable = inGame.getOpponentIdWithMaxMineCount(opponentIdWithMaxMineCount, MaxMineCountOfOpponent);

            const CHero *pClosestHero = (isClosestOpponentAvailable? &inGame.getHero(closestOpponentId):nullptr);
//            const CHero *pHeroWithMaxGoldMine = (isOpponentWithMaxCountAvailable? &inGame.getHero(opponentIdWithMaxMineCount):nullptr);
            if ( isClosestOpponentAvailable && isGoldMineAvailable )
            {
                if ( myHero.getLife() > 0.30*myHero.getMaxLife()
//                     && pClosestHero->getMineCount() > 0
                     && pClosestHero->getLife() < myHero.getLife()
                     && closestGoldMineDistance >= closestOpponentDistance )
                {
                    m_status = E_AGGRESSIVE_CLOSE;
                }
                else
                {
                    m_status = E_CONQUER_GOLD_MINES;
                } // else
            }
            else if ( isGoldMineAvailable )
            {
                m_status = E_CONQUER_GOLD_MINES;
            }
            else if ( isClosestOpponentAvailable
//                      && pClosestHero->getMineCount() > 0
                      && pClosestHero->getLife() < myHero.getLife()
                      && myHero.getLife() > 0.30*myHero.getMaxLife()
                    )
            {
                m_status = E_AGGRESSIVE_CLOSE;
            }
            else
            {
                m_status = E_NEED_LIFE;
            } // else
        } // else

        /// manage motion
        switch (m_status)
        {
            case E_NEED_LIFE:
                nextAction = m_pNeedLife->playBehavior(inGame);
                break;
            case E_CONQUER_GOLD_MINES:
                nextAction = m_pConquerGoldMine->playBehavior(inGame);
                break;
            case E_AGGRESSIVE:
                nextAction = m_pAggressive->playBehavior(inGame);
                break;
            case E_AGGRESSIVE_CLOSE:
                nextAction = m_pAggressiveClose->playBehavior(inGame);
                break;
            case NB_THE_RALPH_BOT_STATES:
            default:
//                m_status = E_NEED_LIFE;
                nextAction = E_ACTION_STAY;
                break;
        }; // switch

        if (nextAction == E_ACTION_STAY)
        {
            std::stringstream fileName;
//            inGame.printBoard();
            fileName<<"./dump/"<<m_name<<"_"<<G_THE_RALPH_BOT_STATES_DICTIONARY[m_status]<<"_"<<inGame.getTrueTurn()<<"_"<<myHero.getId()<<".json";
            if (inGame.dumpBoardData(fileName.str()))
            {
                std::cout<<"'"<<fileName<<"' dump OK"<<std::endl;
            }
            else
            {
                std::cerr<<"'"<<fileName<<"' dump NOT OK"<<std::endl;
            }
        } else {}

    } else {}
    return nextAction;
} // playBehavior

} // namespace MOBE
