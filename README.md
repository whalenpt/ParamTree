# ParamTree #

ParamTree is a library that enables storing and modifying of variables in a tree
structure using the Qt framework. Designed primarily for scientific applications,
its main purpose is to be able to view and change parameters at multiple locations
in an application GUI.

Core functionality is provided in the ParamTreeModel class which is a subclass
of QAbstractItemModel: This is where variables are stored in a tree structure.
The ParamTreeView and ParamTabWidget classes are for displaying and editing
the values stored in the ParamTreeModel.

# Dependencies

Library links with Qt6 modules
<ul>
<li> Qt6::Core </li>
<li> Qt6::Widgets </li>







