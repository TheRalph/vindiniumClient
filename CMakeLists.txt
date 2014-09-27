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

include_directories( ${CMAKE_SOURCE_DIR} )

### g++ options
add_definitions(-std=c++0x)
add_definitions(-Wall)
add_definitions(-O3)
add_definitions(-msse)
add_definitions(-msse2)
add_definitions(-msse3)
add_definitions(-ffast-math)
add_definitions(-DNDEBUG)

################################################################################
### libvindinium
SET( libvindiniumSRC CClient.cpp CClient.h CHttpTools.h CHttpTools.cpp CHero.cpp CHero.h CGame.cpp CGame.h CPosition.h IBehaviorModule.h CBehaviorMgr.cpp CBehaviorMgr.h CTicTac.h CTicTac.cpp)
add_library(vindinium SHARED ${libvindiniumSRC})
target_link_libraries(vindinium -ldl )
set_target_properties(vindinium PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

#--- lib jsoncpp
config_project_by_pkgconfig( vindinium jsoncpp )

################################################################################
### vindiniumLauncher
add_executable(vindiniumLauncher vindiniumLauncher.cpp)
target_link_libraries( vindiniumLauncher vindinium )
set_target_properties(vindiniumLauncher PROPERTIES
         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

################################################################################
### Behavior modules

### random behavior
SET( moduleRandomSRC behaviorModulesSrc/random/CRandomBehaviorModule.cpp behaviorModulesSrc/random/CRandomBehaviorModule.h IBehaviorModule.h )
add_library(random SHARED ${moduleRandomSRC})
target_link_libraries( random vindinium )
set_target_properties(random PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/behaviorModules)

### Mitch behavior (botMitch)
SET( moduleBotMitchSRC behaviorModulesSrc/botMitch/CBotMitchBehaviorModule.cpp behaviorModulesSrc/botMitch/CBotMitchBehaviorModule.h IBehaviorModule.h )
add_library(botMitch SHARED ${moduleBotMitchSRC})
target_link_libraries( botMitch vindinium )
set_target_properties(botMitch PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/behaviorModules)