#ifndef GENERATEWIDGETS_H
#define GENERATEWIDGETS_H

class ParamTreeModel;
class TreeItem;
class ItemWidget;
class QWidget;

QWidget* generateParamWidget(ParamTreeModel* model,const TreeItem& item);
ItemWidget* generateItemWidget(ParamTreeModel* model,const TreeItem& item);


#endif // GENERATEWIDGETS_H
