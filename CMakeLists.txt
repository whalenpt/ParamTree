cmake_minimum_required(VERSION 3.5)
project(ParamTree VERSION 0.0.1)
option(PARAMTREE_DEMOS "Build ParamTree demos" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    if(PARAMTREE_DEMOS)
        add_subdirectory(demos)
    endif()
endif()




