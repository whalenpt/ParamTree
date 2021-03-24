#ifndef GROUPBOXWIDGET_H
#define GROUPBOXWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QHash>
#include <QString>
#include <QStringList>
#include "itemwidget.h"
#include "auxsmallclass.h"

class QVBoxLayout;
class ParamTreeModel;
class TreeItem;
class QLayout;

class GroupBoxWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit GroupBoxWidget(ParamTreeModel* model,const QStringList& key,
                              Depth depth = Depth::COMPLETE,QWidget *parent = nullptr);
    void addItem(const QModelIndex& index);
    void removeItem(const QModelIndex& index);
    GroupBoxWidget* getGroup(const QModelIndex& index);
    const QStringList& key() {return m_key;}
    void setSpacing(int val);

private:
    ParamTreeModel* m_model;
    QStringList m_key;
    Depth m_depth;
    QVBoxLayout* m_layout;
    void generateLayout();
    QHash<QStringList,ItemWidget*> m_leafs;
    QHash<QStringList,GroupBoxWidget*> m_branches;
    GroupBoxWidget* getGroup(const QStringList& key);
    void addItem(const TreeItem& item);
    void removeItem(const TreeItem& item);

};

#endif // GROUPBOXWIDGET_H
