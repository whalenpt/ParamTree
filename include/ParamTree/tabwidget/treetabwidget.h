#ifndef TREETABWIDGET_H
#define TREETABWIDGET_H

#include <QWidget>
#include "ParamTree/ParamTree_global.h"
#include "ParamTree/tabwidget/tabpage.h"
#include "ParamTree/modelview/treemodel.h"

class QVBoxLayout;

namespace paramtree{

class PARAMTREE_EXPORT TreeTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeTabWidget(TreeModel* model,const QStringList& key = QStringList(),
                             QWidget* parent = nullptr);

public slots:
    void reloadWidgets();
    void removeWidgets(const QModelIndex& parent,int first,int last);
    void insertWidgets(const QModelIndex& parent,int start,int end);

private:
    TreeModel* m_model;
    QStringList m_key;
    QVBoxLayout* m_layout;
    TabPage* m_tab_page;
};

}

#endif // TreeTabWidget_H
