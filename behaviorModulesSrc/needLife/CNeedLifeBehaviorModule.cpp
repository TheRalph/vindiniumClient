////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdlib.h>
#include <iostream>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CNeedLifeBehaviorModule.h"

namespace MOBE
{

CREATE_C_SYMBOLS( CNeedLifeBehaviorModule )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CNeedLifeBehaviorModule::CNeedLifeBehaviorModule()
{
    m_version = 0;
    m_name    = "needLife";
    m_comment = "try to get life first of all!";

    srand(time(NULL)); // init random
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CNeedLifeBehaviorModule::~CNeedLifeBehaviorModule()
{
    // Nothing
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CNeedLifeBehaviorModule::playBehavior(const CGame& inGame)
{
    E_BEHAVIOR_ACTIONS nextAction = E_ACTION_STAY;

    path_t closestTavernPath;
    int closestTavernCellId = -1;

    if (inGame.getClosestTavernPath(closestTavernCellId, closestTavernPath))
    {
//         inGame.printPath(closestTavernPath, m_name);

        const CHero &myHero = inGame.getMyHero();
        const CPosition nextPosition(inGame.get2DCoordOnBoard(*closestTavernPath.cbegin()));
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