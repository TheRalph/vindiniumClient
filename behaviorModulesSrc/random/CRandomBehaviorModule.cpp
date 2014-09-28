////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdlib.h>
#include <time.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CRandomBehaviorModule.h"

namespace MOBE
{

CREATE_C_SYMBOLS( CRandomBehaviorModule )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CRandomBehaviorModule::CRandomBehaviorModule()
{
    m_version = 0;
    m_name    = "random";
    m_comment = "a random displacement behavior.",

    srand(time(NULL)); // init random
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CRandomBehaviorModule::~CRandomBehaviorModule()
{
    // Nothing
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CRandomBehaviorModule::playBehavior(const CGame& inGame)
{
    return (E_BEHAVIOR_ACTIONS)(rand()%NB_BEHAVIOR_ACTIONS);
} // playBehavior

} // namespace MOBE