#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "IBehaviorModule.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations

namespace MOBE
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Random Behavior module class for BEhavior ENgine
 */
class CTheRalphBehaviorModule : public IBehaviorModule
{
    public:

        /**
        * @brief Constructor by default
        */
        CTheRalphBehaviorModule();

        /**
        * @brief Destructor
        */
        virtual ~CTheRalphBehaviorModule();

        /**
        * @brief Compute the next action according to the current Game status
        * @param inGame the current Game
        * @return the action to do
        */
        E_BEHAVIOR_ACTIONS playBehavior(const CGame& inGame);

    private:
        /**
        * @brief Initialize the currebt module by loading other behaviors
        * @return true if the initialization is ok, false otherwise
        */
        bool initialize();

    private:
        bool m_isInitialized;
        IBehaviorModule* m_pNeedLife;
        IBehaviorModule* m_pAggressive;
        IBehaviorModule* m_pConquerGoldMine;
}; // class CTheRalphBehaviorModule

} // namespace MOBE