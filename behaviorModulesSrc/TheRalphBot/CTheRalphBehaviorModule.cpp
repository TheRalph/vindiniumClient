////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdlib.h>
#include <iostream>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CTheRalphBehaviorModule.h"

namespace MOBE
{

CREATE_C_SYMBOLS( CTheRalphBehaviorModule )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CTheRalphBehaviorModule::CTheRalphBehaviorModule()
: m_isInitialized(false)
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

        m_isInitialized = (m_pNeedLife && m_pAggressive && m_pConquerGoldMine);
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
        if (myHero.getLife() < 0.51*myHero.getMaxLife())
        {
            std::cout<<"NeedLife "<<std::endl;
            nextAction = m_pNeedLife->playBehavior(inGame);
        }
        else
        {
            int opponentIdWithMaxMineCount = 0;
            int maxMineCount = 0;
            if (inGame.getOpponentIdWithMaxMineCount(opponentIdWithMaxMineCount, maxMineCount) &&
                maxMineCount > 2*myHero.getMineCount() )
            {
                std::cout<<"Aggressive "<<std::endl;
                nextAction = m_pAggressive->playBehavior(inGame);
            }
            else
            {
                std::cout<<"ConquerGoldMine "<<std::endl;
                nextAction = m_pConquerGoldMine->playBehavior(inGame);
            } // else
        } // else
    } else {}
    return nextAction;
} // playBehavior

} // namespace MOBE