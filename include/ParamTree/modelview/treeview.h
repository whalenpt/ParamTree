#ifndef PARAMTREEVIEW_H
#define PARAMTREEVIEW_H

#include "ParamTree_global.h"
#include <QTreeView>

class ParamTreeModel;
class ParamItemDelegate;

class PARAMTREE_EXPORT ParamTreeView : public QTreeView
{
public:
    ParamTreeView(ParamTreeModel* model,QWidget* parent = nullptr);

private:
    ParamTreeModel* m_model;
    ParamItemDelegate* m_delegate;
};

#endif // PARAMTREEVIEW_H
