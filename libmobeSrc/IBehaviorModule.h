//=============================================================================
// Copyright (c) 2014, Raphaël La Greca <raphael.la.greca+vindinium@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
//  * Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=============================================================================

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

namespace MOBE
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
                            m_comment(""),
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
        virtual E_BEHAVIOR_ACTIONS playBehavior(const CGame& inGame) = 0;

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
        * @brief return the comment of the current behavior
        * @return the comment of the current behavior
        */
        inline const std::string& getComment() const { return m_comment; }

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
        std::string m_comment; ///< a comment about the behavior
        void *m_pHandle;     ///< the Handle of the loaded module
        CBehaviorMgr *m_pBehaviorMgr; ///< the behavior manager to use other behavior. WARNING: loop of death can appear if 2 behaviors call each other!
}; // class IBehaviorModule

} // namespace MOBE