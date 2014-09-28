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
    const CHero &myHero = inGame.getMyHero();
    const std::vector<int>& opponentHeroIds = inGame.getOpponentHeroIds();

    int opponentIdWithMaxMineCount = -1;
    int maxMineCount = 0;
    for (const int &opponentHeroId : opponentHeroIds)
    {
        const CHero& opponentHero = inGame.getHero(opponentHeroId-1);
        const int opponentHeroMineCount = opponentHero.getMineCount();
        if ( opponentHeroMineCount > maxMineCount)
        {
            maxMineCount = opponentHeroMineCount;
            opponentIdWithMaxMineCount = opponentHeroId;
        } else {}
    } // for

    E_BEHAVIOR_ACTIONS nextAction = E_ACTION_STAY;
    int targetId = -1;
    if (opponentIdWithMaxMineCount > 0)
    {
        targetId = opponentIdWithMaxMineCount - 1;
        const CHero &targetHero = inGame.getHero(targetId);
        const CPosition targetHeroPos(targetHero.getPosition());
        const int targetHeroCellId = inGame.get1DCoordOnBoard(targetHeroPos);

        std::forward_list<int> targetHeroPath = inGame.getPathTo(targetHeroCellId);
        if (!targetHeroPath.empty())
        {
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