//=============================================================================
// Copyright (c) 2014, Raphaël La Greca <raphael.la.greca+vindinium@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
//  * Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=============================================================================

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
