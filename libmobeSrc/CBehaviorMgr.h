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
#include <map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
namespace MOBE
{
    class IBehaviorModule;
} // namespace MOBE

namespace MOBE
{

static const std::vector<std::string> G_MODULES_PATH_LIST={
    "./behaviorModules",
    "../behaviorModules"
}; ///< List of possible folder of Modules

typedef std::map<std::string, IBehaviorModule*> behaviorsMap_t;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Behavior module mother class for BEhavior ENgine
 */
class CBehaviorMgr
{
    public:
        /**
        * @brief Constructor by default of CBehaviorMgr
        */
        CBehaviorMgr();

        /**
        * @brief Constructor of CBehaviorMgr
        */
        CBehaviorMgr(const std::string& inBehaviorModulesFolder);

        /**
        * @brief Destructor of CBehaviorMgr
        */
        virtual ~CBehaviorMgr();

        /**
        * @brief Clear the current behavior list
        */
        void clearBehaviors();

        /**
        * @brief Get behavior from its name
        * @param inBehaviorName the behavior name to load
        * @return return a behavior from the given name. If the brhavior is not found, NULL is returned.
        */
        IBehaviorModule* getBehavior(const std::string& inBehaviorName);

        /**
        * @brief Get the map of available behaviors
        * @return return the map of available behaviors
        */
        const behaviorsMap_t& getBehaviors() const;

        /**
        * @brief Load all available modules from G_MODULES_PATH_LIST
        * @return return the number of loaded modules
        */
        int loadBehaviorModules(bool inUnloadExistingModules = false);

        /**
        * @brief Load a behavior module
        * @param inBehaviorModulePath the path of the so file defining the behavior
        * @return return true if the behavior module has been loaded successfully, false otherwise
        */
        bool loadBehaviorModule(const std::string& inBehaviorModulePath);

        /**
        * @brief Load all behavior module so files from a folder
        * @param inBehaviorModuleFolder the folder path to load behavior module so files from
        * @return return the number of loaded modules
        */
        int loadBehaviorModuleFolder(const std::string& inBehaviorModuleFolder, bool inUnloadExistingModules = false);

    private:
        typedef IBehaviorModule* create_t();
        typedef void destroy_t(IBehaviorModule*);

        behaviorsMap_t m_behaviors; ///< the list of behaviors
}; // class CBehaviorMgr

} // namespace MOBE