////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdlib.h>
#include <iostream>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CConquerGoldMineBehaviorModule.h"

namespace MOBE
{

CREATE_C_SYMBOLS( CConquerGoldMineBehaviorModule )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CConquerGoldMineBehaviorModule::CConquerGoldMineBehaviorModule()
{
    m_version = 0;
    m_name    = "conquerGoldMine";
    m_comment = "try to get new gold mine first of all!";

    srand(time(NULL)); // init random
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CConquerGoldMineBehaviorModule::~CConquerGoldMineBehaviorModule()
{
    // Nothing
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CConquerGoldMineBehaviorModule::playBehavior(const CGame& inGame)
{
    E_BEHAVIOR_ACTIONS nextAction = E_ACTION_STAY;

    path_t closestGoldMinePath;
    int closestGoldMineCellId = -1;

    if (inGame.getClosestGoldMineMyHeroDoNotControlPath(closestGoldMineCellId, closestGoldMinePath))
    {
        const CHero &myHero = inGame.getMyHero();
        const CPosition nextPosition(inGame.get2DCoordOnBoard(*closestGoldMinePath.cbegin()));
        int deltaX = nextPosition.getX() - myHero.getPosition().getX();
        int deltaY = nextPosition.getY() - myHero.getPosition().getY();

        if      (deltaX ==  1) nextAction = E_ACTION_MOVE_EAST;
        else if (deltaX == -1) nextAction = E_ACTION_MOVE_WEST;
        else if (deltaY ==  1) nextAction = E_ACTION_MOVE_SOUTH;
        else if (deltaY == -1) nextAction = E_ACTION_MOVE_NORTH;
    } else
    {
        // WAIT
    } // else

    return nextAction;
} // playBehavior

} // namespace MOBE