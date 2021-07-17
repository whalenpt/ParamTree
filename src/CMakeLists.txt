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
	generatewidgets.cpp
	groupboxwidget.cpp
	itemwidget.cpp
	paramcheckbox.cpp
	paramcombobox.cpp
	paramdoublespinbox.cpp
	paramgroupbox.cpp
	paramgroupwidget.cpp
	paramitemdelegate.cpp
	paramlineedit.cpp
	paramscientificlineedit.cpp
	paramspinbox.cpp
	paramtabwidget.cpp
	paramtree.cpp
	paramtreeitem.cpp
	paramtreemodel.cpp
	paramtreeview.cpp
	paramtreewidget.cpp
	scientificlineedit.cpp
	tabpage.cpp
	tabwidget.cpp
	widgetinitializers.cpp
)

set(INCL_DIR "${ParamTree_SOURCE_DIR}/include/ParamTree")
set(INCLUDE_FILES 
    "${INCL_DIR}/ParamTree_global.h"
	"${INCL_DIR}/auxsmallclass.h"
	"${INCL_DIR}/generatewidgets.h"
	"${INCL_DIR}/groupboxwidget.h"
	"${INCL_DIR}/itemwidget.h"
	"${INCL_DIR}/paramcheckbox.h"
	"${INCL_DIR}/paramcombobox.h"
	"${INCL_DIR}/paramdoublespinbox.h"
	"${INCL_DIR}/paramgroupbox.h"
	"${INCL_DIR}/paramgroupwidget.h"
	"${INCL_DIR}/paramitemdelegate.h"
	"${INCL_DIR}/paramlineedit.h"
	"${INCL_DIR}/paramscientificlineedit.h"
	"${INCL_DIR}/paramspinbox.h"
	"${INCL_DIR}/paramtabwidget.h"
	"${INCL_DIR}/paramtree.h"
	"${INCL_DIR}/paramtreeitem.h"
	"${INCL_DIR}/paramtreemodel.h"
	"${INCL_DIR}/paramtreeview.h"
	"${INCL_DIR}/paramtreewidget.h"
	"${INCL_DIR}/scientificlineedit.h"
	"${INCL_DIR}/tabpage.h"
	"${INCL_DIR}/tabwidget.h"
	"${INCL_DIR}/widgetinitializers.h"
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





