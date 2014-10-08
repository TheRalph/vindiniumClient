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

        path_t targetHeroPath;
        if (inGame.getPathTo(targetHeroCellId, false, targetHeroPath))
        {
//             inGame.printPath(targetHeroPath, m_name);

            const CHero &myHero = inGame.getMyHero();
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