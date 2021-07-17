#ifndef PARAMGROUPWIDGET_H
#define PARAMGROUPWIDGET_H

#include "ParamTree_global.h"
#include "auxsmallclass.h"
#include <QWidget>

class ParamTreeModel;
class QVBoxLayout;
class GroupBoxWidget;

class PARAMTREE_EXPORT ParamGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParamGroupWidget(ParamTreeModel* model,const QStringList& key = QStringList(),
                             Depth depth = Depth::COMPLETE,
                             QWidget* parent = nullptr);
    void setTitle(const QString& str);
    void setSpacing(int val);

public slots:
    void reloadWidgets();
    void removeWidgets(const QModelIndex& parent,int first,int last);
    void insertWidgets(const QModelIndex& parent,int start,int end);

private:
    ParamTreeModel* m_model;
    QStringList m_key;
    Depth m_depth;
    QVBoxLayout* m_layout;
    GroupBoxWidget* m_group_widget;
};

#endif // PARAMGROUPWIDGET_H
