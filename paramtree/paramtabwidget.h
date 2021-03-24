#ifndef PARAMTABWIDGET_H
#define PARAMTABWIDGET_H

#include <QWidget>
#include "ParamTree_global.h"
#include "tabpage.h"
class ParamTreeModel;
class QVBoxLayout;

class PARAMTREE_EXPORT ParamTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParamTabWidget(ParamTreeModel* model,const QStringList& key = QStringList(),
                             QWidget* parent = nullptr);

public slots:
    void reloadWidgets();
    void removeWidgets(const QModelIndex& parent,int first,int last);
    void insertWidgets(const QModelIndex& parent,int start,int end);

private:
    ParamTreeModel* m_model;
    QStringList m_key;
    QVBoxLayout* m_layout;
    TabPage* m_tab_page;
};

#endif // ParamTabWidget_H
