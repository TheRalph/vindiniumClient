////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdlib.h>
#include <iostream>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CAggressiveBehaviorModule.h"

namespace MOBE
{

CREATE_C_SYMBOLS( CAggressiveBehaviorModule )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CAggressiveBehaviorModule::CAggressiveBehaviorModule()
{
    m_version = 0;
    m_name    = "aggressive";
    m_comment = "try to kill opponents with max mine count first of all!";

    srand(time(NULL)); // init random
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CAggressiveBehaviorModule::~CAggressiveBehaviorModule()
{
    // Nothing
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CAggressiveBehaviorModule::playBehavior(const CGame& inGame)
{
    E_BEHAVIOR_ACTIONS nextAction = E_ACTION_STAY;

    int opponentIdWithMaxMineCount = -1;
    int maxMineCount = 0;

    if (inGame.getOpponentIdWithMaxMineCount(opponentIdWithMaxMineCount, maxMineCount))
    {
        const CHero &targetHero = inGame.getHero(opponentIdWithMaxMineCount);
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