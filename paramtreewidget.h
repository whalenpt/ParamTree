#ifndef PARAMTREEWIDGET_H
#define PARAMTREEWIDGET_H

#include <QWidget>
#include "paramgroupbox.h"
#include "auxsmallclass.h"
class ParamTreeModel;
class QVBoxLayout;

class ParamTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParamTreeWidget(ParamTreeModel* model,const QStringList& key = QStringList(),
                             Depth depth = Depth::COMPLETE,
                             QWidget* parent = nullptr);
    void setTitle(const QString& str);

public slots:
    void reloadWidgets();
    void removeWidgets(const QModelIndex& parent,int first,int last);
    void insertWidgets(const QModelIndex& parent,int start,int end);

private:
    ParamTreeModel* m_model;
    QStringList m_key;
    Depth m_depth;
    QVBoxLayout* m_layout;
    ParamGroupBox* m_group_widget;
};


#endif // PARAMTREEWIDGET_H
