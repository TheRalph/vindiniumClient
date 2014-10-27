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
#include <stdlib.h>
#include <iostream>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CAggressiveCloseBehaviorModule.h"

namespace MOBE
{

CREATE_C_SYMBOLS( CAggressiveCloseBehaviorModule )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CAggressiveCloseBehaviorModule::CAggressiveCloseBehaviorModule()
{
    m_version = 0;
    m_name    = "aggressiveClose";
    m_comment = "try to kill the closest opponent first of all!";

    srand(time(NULL)); // init random
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CAggressiveCloseBehaviorModule::~CAggressiveCloseBehaviorModule()
{
    // Nothing
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CAggressiveCloseBehaviorModule::playBehavior(const CGame& inGame)
{
    E_BEHAVIOR_ACTIONS nextAction = E_ACTION_STAY;

    int closestOpponentId = -1;
    int opponentDistance = 0;
    if (inGame.getClosestOpponent(closestOpponentId, opponentDistance))
    {
        const CHero &targetHero = inGame.getHero(closestOpponentId);
        const CPosition targetHeroPos(targetHero.getPosition());
        const int targetHeroCellId = inGame.get1DCoordOnBoard(targetHeroPos);

        const CHero &myHero = inGame.getMyHero();
        path_t targetHeroPath;
        if (inGame.getPathTo(myHero.getId(), targetHeroCellId, false, targetHeroPath))
        {
//             inGame.printPath(targetHeroPath, m_name);

            const CPosition nextPosition(inGame.get2DCoordOnBoard(*targetHeroPath.cbegin()));
            int deltaX = nextPosition.getX() - myHero.getPosition().getX();
            int deltaY = nextPosition.getY() - myHero.getPosition().getY();

            if      (deltaX ==  1) nextAction = E_ACTION_MOVE_EAST;
            else if (deltaX == -1) nextAction = E_ACTION_MOVE_WEST;
            else if (deltaY ==  1) nextAction = E_ACTION_MOVE_SOUTH;
            else if (deltaY == -1) nextAction = E_ACTION_MOVE_NORTH;
        } else {}
    } else
    {
        // WAIT
    } // else

    return nextAction;
} // playBehavior

} // namespace MOBE