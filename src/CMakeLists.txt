cmake_minimum_required(VERSION 3.5)
option(PARAMTREE_STATIC "Build ParamTree as static (ON) or shared library (OFF)" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

if(NOT PARAMTREE_STATIC)
    set(BUILD_SHARED_LIBS ON)
    message(STATUS "Building shared ParamTree library")
else()
    set(BUILD_SHARED_LIBS OFF)
    message(STATUS "Building static ParamTree library")
endif()


set(SOURCE_FILES
    itemwidgets/itemwidget.cpp
	itemwidgets/generatewidgets.cpp
	itemwidgets/treecheckbox.cpp
	itemwidgets/treecombobox.cpp
	itemwidgets/treedblspinbox.cpp
	itemwidgets/treelineedit.cpp
	itemwidgets/treescilineedit.cpp
	itemwidgets/treespinbox.cpp
	shared/widgetinitializer.cpp
	shared/scientificlineedit.cpp

	modelview/itemdelegate.cpp
	modelview/treeitem.cpp
	modelview/treemodel.cpp
	modelview/treeview.cpp
	groupwidget/treewidget.cpp
	groupwidget/treegroupbox.cpp
	tabwidget/tabpage.cpp
	tabwidget/tabwidget.cpp
	tabwidget/treetabwidget.cpp
)

set(INCL_DIR "${ParamTree_SOURCE_DIR}/include/ParamTree")
set(INCLUDE_FILES 
    "${INCL_DIR}/ParamTree_global.h"
	"${INCL_DIR}/itemwidgets/itemwidget.h"
	"${INCL_DIR}/itemwidgets/generatewidgets.h"
	"${INCL_DIR}/itemwidgets/treecheckbox.h"
	"${INCL_DIR}/itemwidgets/treecombobox.h"
	"${INCL_DIR}/itemwidgets/treelineedit.h"
	"${INCL_DIR}/itemwidgets/treespinbox.h"
	"${INCL_DIR}/itemwidgets/treescilineedit.h"
	"${INCL_DIR}/itemwidgets/treedblspinbox.h"
	"${INCL_DIR}/shared/widgetinitializer.h"
	"${INCL_DIR}/shared/scientificlineedit.h"
	"${INCL_DIR}/shared/auxsmallclass.h"
	"${INCL_DIR}/modelview/treeitem.h"
	"${INCL_DIR}/modelview/treemodel.h"
	"${INCL_DIR}/modelview/treeview.h"
	"${INCL_DIR}/modelview/itemdelegate.h"
	"${INCL_DIR}/groupwidget/treegroupbox.h"
	"${INCL_DIR}/groupwidget/treewidget.h"
	"${INCL_DIR}/tabwidget/tabpage.h"
	"${INCL_DIR}/tabwidget/tabwidget.h"
	"${INCL_DIR}/tabwidget/treetabwidget.h"
)

add_library(paramtree ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(paramtree
    PRIVATE
	$<BUILD_INTERFACE:${ParamTree_SOURCE_DIR}/include/ParamTree>
	$<INSTALL_INTERFACE:include/ParamTree>
)

target_include_directories(paramtree
    PUBLIC
	$<BUILD_INTERFACE:${ParamTree_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_link_libraries(paramtree
    PRIVATE Qt6::Core
    PRIVATE Qt6::Widgets)

target_compile_definitions(paramtree PRIVATE PARAMTREE_LIBRARY)





