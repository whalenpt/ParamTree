# ParamTree #

ParamTree is a library that enables storing and modifying of variables in a tree
structure using the Qt framework. Designed primarily for scientific applications,
its main purpose is to be able to view and change parameters seamlessly at multiple locations
in an application GUI.

Core functionality is provided in the ParamTreeModel class which is a subclass
of QAbstractItemModel. The ParamTreeView and ParamTabWidget classes are for displaying and editing
the values stored in the ParamTreeModel.

# Dependencies #

Library links with Qt6 modules
<ul>
<li> Qt6::Core </li>
<li> Qt6::Widgets </li>
</ul>

Build is done using CMake

# Installation #

If not installed, get and install <a href = https://doc.qt.io/qt-6/gettingstarted.html>Qt6 </a>
and download <a href = https://cmake.org/download/>CMake </a>.

To build the ParamTree library from the terminal use
```bash
git clone https://github.com/whalenpt/ParamTree.git
cd ParamTree
cmake -S . -B build -DCMAKE_PREFIX_PATH=<Path_To_Qt_Folder/Qt_Version_#/compiler_name>
cd build
make -j4
```
The paramtree library file is under the paramtree subdirectory of build. Examples
are in the demos subdirectory. For instance, to run the Optics demo change directory
into demos/Optics and run the executable named Optics.

Alternatively we can use QtCreator to build the library and run the examples, 
<ul>
<li> Open QtCreator </li>
<li> Select Open File or Project from the File Menu </li>
<li> Navigate to the ParamTree folder downloaded from github and open the CMakeLists.txt file </li>
<li> Build the project to create a paramtree library file (shared) </li> 
<li> Run a demo subdirectory to see examples of how the library works </li>
</ul>

# License #
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.
The Qt code used in the paramtree library is available with an LGPLv3 license 
but check [Qt.LICENSE](./Qt.LICENSE) for more information on reuse restrictions and
limitations.

# Contact #
Patrick Whalen - whalenpt@gmail.com













































