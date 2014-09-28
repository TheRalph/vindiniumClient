project(vindiniumClient)
cmake_minimum_required(VERSION 2.8)
#set (CMAKE_VERBOSE_MAKEFILE ON)

macro( config_project PROJNAME LIBNAME )
    include_directories( ${${LIBNAME}_INCLUDE_DIRS} )
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARIES} ${${LIBNAME}_LIBS} )
endmacro()

find_package(PkgConfig REQUIRED)
macro( config_project_by_pkgconfig PROJNAME LIBNAME )
    pkg_check_modules(PKG_${LIBNAME} ${LIBNAME})
    link_directories( ${PKG_${LIBNAME}_LIBRARY_DIRS} )
    include_directories( ${PKG_${LIBNAME}_INCLUDE_DIRS} )
    target_link_libraries(${PROJNAME} ${PKG_${LIBNAME}_LIBRARIES} )
endmacro()

### g++ options
add_definitions(-std=c++0x)
add_definitions(-Wall)
add_definitions(-O3)
add_definitions(-msse)
add_definitions(-msse2)
add_definitions(-msse3)
add_definitions(-ffast-math)
add_definitions(-DNDEBUG)

SET( libmobeFolderSrc ${CMAKE_SOURCE_DIR}/libmobeSrc )
SET( behaviorModulesFolderSrc ${CMAKE_SOURCE_DIR}/behaviorModulesSrc )

include_directories( ${libmobeFolderSrc} )

################################################################################
### libvindinium
SET( libmobeSRC
    ${libmobeFolderSrc}/CClient.cpp
    ${libmobeFolderSrc}/CClient.h
    ${libmobeFolderSrc}/CHttpTools.h
    ${libmobeFolderSrc}/CHttpTools.cpp
    ${libmobeFolderSrc}/CHero.cpp
    ${libmobeFolderSrc}/CHero.h
    ${libmobeFolderSrc}/CGame.cpp
    ${libmobeFolderSrc}/CGame.h
    ${libmobeFolderSrc}/CPosition.h
    ${libmobeFolderSrc}/IBehaviorModule.h
    ${libmobeFolderSrc}/CBehaviorMgr.cpp
    ${libmobeFolderSrc}/CBehaviorMgr.h
    ${libmobeFolderSrc}/CTicTac.h
    ${libmobeFolderSrc}/CTicTac.cpp
)
add_library(mobe SHARED ${libmobeSRC})
target_link_libraries(mobe -ldl )
set_target_properties(mobe PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

#--- lib jsoncpp
config_project_by_pkgconfig( mobe jsoncpp )

################################################################################
### vindiniumLauncher
add_executable(vindiniumLauncher vindiniumLauncher.cpp)
target_link_libraries( vindiniumLauncher mobe )
set_target_properties(vindiniumLauncher PROPERTIES
         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

################################################################################
### Behavior modules

### random behavior
SET( moduleRandomSRC 
    ${behaviorModulesFolderSrc}/random/CRandomBehaviorModule.cpp
    ${behaviorModulesFolderSrc}/random/CRandomBehaviorModule.h
    ${libmobeFolderSrc}/IBehaviorModule.h
)
add_library(random SHARED ${moduleRandomSRC})
target_link_libraries( random mobe )
set_target_properties(random PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/behaviorModules)

### Mitch behavior (botMitch)
SET( moduleBotMitchSRC
    ${behaviorModulesFolderSrc}/botMitch/CBotMitchBehaviorModule.cpp
    ${behaviorModulesFolderSrc}/botMitch/CBotMitchBehaviorModule.h
    ${libmobeFolderSrc}/IBehaviorModule.h
)
add_library(botMitch SHARED ${moduleBotMitchSRC})
target_link_libraries( botMitch mobe )
set_target_properties(botMitch PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/behaviorModules)