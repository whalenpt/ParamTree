#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "ParamTree/ParamTree_global.h"
#include "ParamTree/modelview/itemdelegate.h"
#include "ParamTree/modelview/treemodel.h"
#include <QTreeView>

namespace paramtree{

class PARAMTREE_EXPORT TreeView : public QTreeView
{
public:
    TreeView(TreeModel* model,QWidget* parent = nullptr);

private:
    TreeModel* m_model;
    ItemDelegate* m_delegate;
};

}

#endif // PARAMTREEVIEW_H
