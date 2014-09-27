#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>
#include <vector>
#include <dlfcn.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CGame.h"
#include "CBehaviorMgr.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// C symbols functions to create and destroy the instance
#define CREATE_C_SYMBOLS(inBehaviorModuleClassName)                           \
extern "C" inBehaviorModuleClassName* createBehavior()                        \
{                                                                             \
    return new inBehaviorModuleClassName();                                   \
}                                                                             \
extern "C" void destroyBehavior(inBehaviorModuleClassName* pinBehaviorModule) \
{                                                                             \
    if (pinBehaviorModule)                                                    \
    {                                                                         \
        delete pinBehaviorModule;                                             \
    } else {}                                                                 \
}

namespace BEEN
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Interface Behavior module class for BEhavior ENgine
 */
class IBehaviorModule
{
    public:

        /**
        * @brief Constructor by default
        */
        IBehaviorModule() : m_version(0),
                            m_name(""),
                            m_pHandle(NULL),
                            m_pBehaviorMgr(NULL) {}

        /**
        * @brief Destructor
        */
        virtual ~IBehaviorModule() {}

        /**
        * @brief Compute the next action according to the current Game status
        * @param inGame the current Game
        * @return the action to do
        * 
        * The default behavior is random!
        * This function must be overloaded with the needed behavior
        */
        virtual E_GAME_ACTIONS playBehavior(const CGame& inGame) = 0;

        /**
        * @brief set the handle of the loaded module
        * @param inpHandle the handle to set with
        */
        inline void setHandle(void * inpHandle) { m_pHandle = inpHandle; }

        /**
        * @brief return the handle of the loaded module
        * @return the handle of the loaded module
        */
        inline void *getHandle() const { return m_pHandle; }

        /**
        * @brief set the handle of the loaded module
        * @param inpHandle the handle to set with
        */
        inline void setBehaviorMgr(CBehaviorMgr* inpBehaviorMgr) { m_pBehaviorMgr = inpBehaviorMgr; }

        /**
        * @brief return the version of the loaded module
        * @return the version of the loaded module
        */
        inline const int getVersion() const { return m_version; }

        /**
        * @brief return the name of the loaded module
        * @return the name of the loaded module
        */
        inline const std::string& getName() const { return m_name; }

    protected:
        int m_version;       ///< the version of the module api
        std::string m_name;  ///< the name of the mmodule
        void *m_pHandle;     ///< the Handle of the loaded module
        CBehaviorMgr *m_pBehaviorMgr; ///< the behavior manager to use other behavior. WARNING: loop of death can appear if 2 behaviors call each other!
}; // class IBehaviorModule

} // namespace BEEN