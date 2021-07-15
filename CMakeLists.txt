cmake_minimum_required(VERSION 3.5)
project(ParamTree LANGUAGES CXX)
option(PARAMTREE_STATIC "Build ParamTree as static (ON) or shared library (OFF)" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT PARAMTREE_STATIC)
    set(BUILD_SHARED_LIBS ON)
    message(STATUS "Building shared ParamTree library")
else()
    set(BUILD_SHARED_LIBS OFF)
    message(STATUS "Building static ParamTree library")
endif()


add_subdirectory(paramtree)
add_subdirectory(demos)




