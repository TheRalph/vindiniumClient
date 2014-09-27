////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <stdlib.h>
#include <time.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CBotMitchBehaviorModule.h"

namespace BEEN
{

CREATE_C_SYMBOLS( CBotMitchBehaviorModule )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CBotMitchBehaviorModule::CBotMitchBehaviorModule()
{
    m_version = 0;
    m_name    = "botMitch";
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CBotMitchBehaviorModule::~CBotMitchBehaviorModule()
{
    // Nothing
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_GAME_ACTIONS CBotMitchBehaviorModule::playBehavior(const CGame& inGame)
{
    return (E_GAME_ACTIONS)(3);//rand()%NB_VINDINIUM_ACTIONS);
} // playBehavior

} // namespace BEEN