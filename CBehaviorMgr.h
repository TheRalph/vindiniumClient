#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>
#include <map>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
namespace BEEN
{
    class IBehaviorModule;
} // namespace BEEN

namespace BEEN
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Behavior module mother class for BEhavior ENgine
 */
class CBehaviorMgr
{
    public:
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

        std::map<std::string, IBehaviorModule*> m_behaviors; ///< the list of behaviors
}; // class CBehaviorMgr

} // namespace BEEN