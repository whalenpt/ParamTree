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

The easiest way to build the library and run the examples is to use QtCreator:
<ul>
<li> Open QtCreator </li>
<li> Select Open File or Project from the File Menu </li>
<li> Navigate to the ParamTree folder downloaded from github and open the CMakeLists.txt file </li>
<li> Build the project to create a paramtree library file (shared) </li> 
<li> Run a demo subdirectory to see examples of how the library works </li>
</ul>

To build the ParamTree library from the terminal (Linux/MacOS) use
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

A similar procedure can be done for from the Windows Powershell with a few additional
options to the cmake command
```Powershell
cd ParamTree
cmake -S . -B build-mingw -DCMAKE_PREFIX_PATH=<Path_To_Qt_Folder\Qt_Version_#\mingw_version>
			  -DCMAKE_CXX_COMPILER=<Path_To_Qt_Folder\Tools\mingw_version\bin\g++.exe>
			  -G "Unix Makefiles"
cd build-mingw 
make -j4
```
To run demos on the Windows build, the executables need to be able to find the Qt DLLs (for
instance by adding the folder `<Path_To_Qt_Folder\Qt_Version_#\mingw_version\bin>` to the
windows environment PATH variable) and the generated paramtree DLL (for instance by
copying these to the directory of the demo or also adding it to the path). Alternatively,
cmake without the "Unix Makefiles" compiler flag can create a Visual Studio project file and
the library can be created in that setting as well. 

# License #
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.
The Qt code used in the paramtree library is available with an LGPLv3 license 
but check [Qt.LICENSE](./Qt.LICENSE) for more information on reuse restrictions and
limitations.

# Contact #
Patrick Whalen - whalenpt@gmail.com













































