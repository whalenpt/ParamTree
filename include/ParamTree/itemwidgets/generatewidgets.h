#ifndef GENERATEWIDGETS_H
#define GENERATEWIDGETS_H

class QWidget;

namespace paramtree{

class TreeModel;
class TreeItem;
class ItemWidget;
QWidget* generateParamWidget(TreeModel* model,const TreeItem& item);
ItemWidget* generateItemWidget(TreeModel* model,const TreeItem& item);

}


#endif // GENERATEWIDGETS_H
