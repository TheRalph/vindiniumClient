////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <dlfcn.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CBehaviorMgr.h"
#include "IBehaviorModule.h"

namespace MOBE
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CBehaviorMgr::CBehaviorMgr()
{
    // nothing
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CBehaviorMgr::CBehaviorMgr(const std::string& inBehaviorModulesFolder)
{
    const int nbLoadedModules = loadBehaviorModuleFolder(inBehaviorModulesFolder);
    std::cout<<"Nb loaded modules="<<nbLoadedModules<<std::endl;
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CBehaviorMgr::~CBehaviorMgr()
{
    clearBehaviors();
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CBehaviorMgr::clearBehaviors()
{
    for (auto &behaviorIt : m_behaviors)
    {
        IBehaviorModule* pBehavior = behaviorIt.second;
        if (pBehavior)
        {
            void *pHandle = pBehavior->getHandle();
            destroy_t* destroyBehavior = (destroy_t*)dlsym(pHandle,"destroyBehavior");
            if (!destroyBehavior)
            {
                std::cerr<<"Can not find destroy function from '"<<pBehavior->getName()<<"': "<<dlerror()<<std::endl;
            }
            else
            {
                void *pHandle = pBehavior->getHandle();
                destroyBehavior(pBehavior);
                if (pHandle)
                {
                    ::dlclose(pHandle);
                } else {}
            } // else
        } else {}
    } // for
    m_behaviors.clear();
} // clearBehaviors

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int CBehaviorMgr::loadBehaviorModules(bool inUnloadExistingModules)
{
    if (inUnloadExistingModules)
    {
        clearBehaviors();
    } else {}

    int nbLoadedModules = 0;
    for (const std::string &behaviorsPath : G_MODULES_PATH_LIST)
    {
        nbLoadedModules += loadBehaviorModuleFolder(behaviorsPath);
    } // for
    return nbLoadedModules;
} // loadBehaviorModules

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CBehaviorMgr::loadBehaviorModule(const std::string& inBehaviorModulePath)
{
    bool moduleLoaded = false;

    if (inBehaviorModulePath.empty())
    {
        std::cerr<<"The given behavior module path to load is empty"<<std::endl;
    }
    else
    {
        void *pHandle = ::dlopen (inBehaviorModulePath.c_str(), RTLD_NOW);
        if (!pHandle)
        {
            std::cerr<<"Can not load module '"<<inBehaviorModulePath<<"': "<<dlerror()<<std::endl;
        }
        else
        {
            create_t* createBehavior = (create_t*)dlsym(pHandle,"createBehavior");

            if (!createBehavior)
            {
                std::cerr<<"Can not find symbol 'createBehavior' on loaded module '"<<inBehaviorModulePath<<"': "<<dlerror()<<std::endl;
            }
            else
            {
                IBehaviorModule *pBehavior = createBehavior();
                if (!pBehavior)
                {
                    std::cerr<<"Can not create behavior module from '"<<inBehaviorModulePath<<"': "<<dlerror()<<std::endl;
                }
                else
                {
                    pBehavior->setHandle(pHandle);
                    pBehavior->setBehaviorMgr(this);
                    m_behaviors[pBehavior->getName()] = pBehavior;
                    moduleLoaded = true;
                } // else
            } // else
        } // else
    } // else

    return moduleLoaded;
} // loadBehaviorModule

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int CBehaviorMgr::loadBehaviorModuleFolder(const std::string& inBehaviorModuleFolder, bool inUnloadExistingModules)
{
    int nbLoadedModules = 0;

    std::vector<std::string> soFilesList;

    if (inBehaviorModuleFolder.empty())
    {
        std::cerr<<"The given behavior folder name to load is empty"<<std::endl;
    }
    else
    {
        DIR *pDir = ::opendir(inBehaviorModuleFolder.c_str());

        if (pDir)
        {
            if (inUnloadExistingModules)
            {
                clearBehaviors();
            } else {}

            struct dirent * pDp = NULL;
            while ((pDp = ::readdir(pDir)) != NULL)
            {
                if (::strcmp(pDp->d_name, ".") == 0 || ::strcmp(pDp->d_name, "..") == 0)
                {
                    continue;
                } else {}
                const std::string currentFileName(pDp->d_name);

                /// only get .so files
                const char * pExt = NULL;
                std::string::size_type idOfPoint = currentFileName.find_last_of('.');
                if (idOfPoint != std::string::npos)
                {
                    pExt = &currentFileName.c_str()[idOfPoint+1];
                } else {}

                const std::string extension(pExt? pExt:"");
                if (extension == "so")
                {
                    soFilesList.push_back(inBehaviorModuleFolder+"/"+currentFileName);
                } else {}
            } // while
            closedir(pDir);
        } else {}

        if (!soFilesList.empty())
        {
            for (std::string &soFilePath : soFilesList)
            {
                if (loadBehaviorModule(soFilePath))
                {
                    nbLoadedModules++;
                } else {}
            } // for
        } else {}
    } // else

    return nbLoadedModules;
} // loadBehaviorModuleFolder

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
IBehaviorModule* CBehaviorMgr::getBehavior(const std::string& inBehaviorName)
{
    IBehaviorModule* pBehavior = nullptr;
    auto behaviorIt = m_behaviors.find(inBehaviorName);
    if (behaviorIt != m_behaviors.end())
    {
        pBehavior = behaviorIt->second;
    } else {}

    return pBehavior;
} // getBehavior

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const behaviorsMap_t& CBehaviorMgr::getBehaviors() const
{
    return m_behaviors;
} // getBehaviors

} // namespace MOBE