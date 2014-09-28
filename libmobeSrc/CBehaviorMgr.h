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