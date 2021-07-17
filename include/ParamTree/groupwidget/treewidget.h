#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "ParamTree/ParamTree_global.h"
#include "ParamTree/shared/auxsmallclass.h"
#include "ParamTree/groupwidget/treegroupbox.h"
#include "ParamTree/modelview/treemodel.h"
#include <QWidget>

class QVBoxLayout;

namespace paramtree{

class PARAMTREE_EXPORT TreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidget(TreeModel* model,const QStringList& key = QStringList(),
                             Depth depth = Depth::COMPLETE,
                             QWidget* parent = nullptr);
    void setTitle(const QString& str);
    void setSpacing(int val);

public slots:
    void reloadWidgets();
    void removeWidgets(const QModelIndex& parent,int first,int last);
    void insertWidgets(const QModelIndex& parent,int start,int end);

private:
    TreeModel* m_model;
    QStringList m_key;
    Depth m_depth;
    QVBoxLayout* m_layout;
    TreeGroupBox* m_group_widget;
};

}

#endif // PARAMGROUPWIDGET_H
