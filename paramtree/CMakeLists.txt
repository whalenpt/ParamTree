cmake_minimum_required(VERSION 3.5)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_LIST 
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

set(HEADER_LIST ParamTree_global.h
	auxsmallclass.h
	generatewidgets.h
	groupboxwidget.h
	itemwidget.h
	paramcheckbox.h
	paramcombobox.h
	paramdoublespinbox.h
	paramgroupbox.h
	paramgroupwidget.h
	paramitemdelegate.h
	paramlineedit.h
	paramscientificlineedit.h
	paramspinbox.h
	paramtabwidget.h
	paramtree.h
	paramtreeitem.h
	paramtreemodel.h
	paramtreeview.h
	paramtreewidget.h
	scientificlineedit.h
	tabpage.h
	tabwidget.h
	widgetinitializers.h
)

add_library(paramtree SHARED ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(paramtree
    PRIVATE Qt6::Core
    PRIVATE Qt6::Widgets)
target_compile_definitions(paramtree PRIVATE PARAMTREE_LIBRARY)





