cmake_minimum_required(VERSION 3.5)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(This optics)

set(PROJECT_SOURCES
	main.cpp
	mainwindow.cpp
	mainwindow.h
	opticstree.cpp
	opticstree.h
	mainwindow.ui
)

add_executable(${This} ${PROJECT_SOURCES})

target_include_directories(${This} 
	PUBLIC ${CMAKE_SOURCE_DIR}/paramtree
)

target_link_libraries(${This}
        PRIVATE Qt6::Widgets
        PRIVATE paramtree
)




