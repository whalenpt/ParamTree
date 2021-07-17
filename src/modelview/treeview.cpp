#include "paramtreeview.h"
#include "paramtreemodel.h"
#include "paramitemdelegate.h"
#include <QAction>

ParamTreeView::ParamTreeView(ParamTreeModel* model,QWidget* parent) :
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

    m_delegate = new ParamItemDelegate(this);
    setItemDelegateForColumn(1,m_delegate);
}
