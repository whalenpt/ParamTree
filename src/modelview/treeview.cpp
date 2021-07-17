#include "modelview/treeview.h"
#include "modelview/treemodel.h"
#include "modelview/itemdelegate.h"
#include <QAction>

namespace paramtree{

TreeView::TreeView(TreeModel* model,QWidget* parent) :
    QTreeView(parent),
    m_model(model)
{
    setModel(m_model);
    setFocusPolicy(Qt::NoFocus);

    setEditTriggers(QAbstractItemView::CurrentChanged);
//    setStyleSheet("selection-color: black;"
//                  "selection-background-color: white;"
//                  "alternate-background-color: red;"
//                  "border-color: green;");


    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    m_delegate = new ItemDelegate(this);
    setItemDelegateForColumn(1,m_delegate);
}

}


