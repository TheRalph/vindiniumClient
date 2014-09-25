project(vindiniumClient)
cmake_minimum_required(VERSION 2.8)
set (CMAKE_VERBOSE_MAKEFILE ON)

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

SET( libvindiniumSRC CClient.cpp CClient.h CHttpTools.h CHttpTools.cpp CHero.cpp CHero.h CGame.cpp CGame.h CPosition.h CTicTac.h CTicTac.cpp )
add_library(vindinium SHARED ${libvindiniumSRC})
set_target_properties(vindinium PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

#--- lib jsoncpp
config_project_by_pkgconfig( vindinium jsoncpp )

### vindiniumTest
add_executable(vindiniumTest vindiniumTest.cpp)
target_link_libraries( vindiniumTest vindinium )
set_target_properties(vindiniumTest PROPERTIES
         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
