project(vindiniumClient)
cmake_minimum_required(VERSION 2.8)
#set (CMAKE_VERBOSE_MAKEFILE ON)

include(${CMAKE_SOURCE_DIR}/cmake/OptimizeForArchitecture.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/FindSSE.cmake)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.7
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
        message(FATAL_ERROR "g++ version must be at least 4.7!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with g++.")
endif()

macro( config_project PROJNAME LIBNAME )
    include_directories( ${${LIBNAME}_INCLUDE_DIRS} )
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARIES} ${${LIBNAME}_LIBS} )
endmacro()

macro( add_behavior_module BEHAVIOR_MODULE_NAME BEHAVIOR_MODULE_SRC_NAME )
    SET( moduleSRC 
        ${behaviorModulesFolderSrc}/${BEHAVIOR_MODULE_NAME}/${BEHAVIOR_MODULE_SRC_NAME}.cpp
        ${behaviorModulesFolderSrc}/${BEHAVIOR_MODULE_NAME}/${BEHAVIOR_MODULE_SRC_NAME}.h
        ${libmobeFolderSrc}/IBehaviorModule.h
    )
    add_library(${BEHAVIOR_MODULE_NAME} SHARED ${moduleSRC})
    target_link_libraries( ${BEHAVIOR_MODULE_NAME} mobe )
    set_target_properties(${BEHAVIOR_MODULE_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/behaviorModules)
endmacro()

find_package(PkgConfig REQUIRED)
macro( config_project_by_pkgconfig PROJNAME LIBNAME )
    pkg_check_modules(PKG_${LIBNAME} ${LIBNAME})
    link_directories( ${PKG_${LIBNAME}_LIBRARY_DIRS} )
    include_directories( ${PKG_${LIBNAME}_INCLUDE_DIRS} )
    target_link_libraries(${PROJNAME} ${PKG_${LIBNAME}_LIBRARIES} )
endmacro()

### g++ options
add_definitions(-std=c++11)
add_definitions(-Wall)
#add_definitions(-msse)
#add_definitions(-msse2)
#add_definitions(-msse3)
add_definitions(-ffast-math)
add_definitions(-O3)
add_definitions(-DNDEBUG)
#add_definitions(-O0)
#add_definitions(-ggdb)

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
    ${libmobeFolderSrc}/CDuration.h
)
add_library(mobe SHARED ${libmobeSRC})
target_link_libraries(mobe -ldl )
set_target_properties(mobe PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

#--- lib jsoncpp
config_project_by_pkgconfig( mobe jsoncpp )

#--- utests
add_executable(testPathTo ${libmobeFolderSrc}/utest/testPathTo.cpp)
target_link_libraries( testPathTo mobe )
set_target_properties(testPathTo PROPERTIES
         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

################################################################################
### vindiniumLauncher
add_executable(vindiniumLauncher vindiniumLauncher.cpp)
target_link_libraries( vindiniumLauncher mobe )
set_target_properties(vindiniumLauncher PROPERTIES
         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

################################################################################
### Behavior modules

### random behavior
add_behavior_module( random CRandomBehaviorModule )

### aggressive behavior
add_behavior_module( aggressive CAggressiveBehaviorModule )

### aggressive close behavior
add_behavior_module( aggressiveClose CAggressiveCloseBehaviorModule )

### need life behavior
add_behavior_module( needLife CNeedLifeBehaviorModule )

add_behavior_module( conquerGoldMine CConquerGoldMineBehaviorModule )

### Mitch behavior (botMitch)
add_behavior_module( botMitch CBotMitchBehaviorModule )

### The Ralph Bot
add_behavior_module( TheRalphBot CTheRalphBehaviorModule )