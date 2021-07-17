#ifndef TABPAGE_H
#define TABPAGE_H

#include <QWidget>
#include <QHash>
#include <QString>
#include <QStringList>
#include "itemwidget.h"

class QVBoxLayout;
class ParamTreeModel;
class TreeItem;
class QLayout;
class TabWidget;

class TabPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabPage(ParamTreeModel* model,const QStringList& key,
                              TabWidget *parent = nullptr);
    int leafCount() const;
    int tabCount() const;
    void addItem(const QModelIndex& index);
    void removeItem(const QModelIndex& index);
    TabPage* tabPage(const QModelIndex& index);
    TabPage* tabPage(const QStringList& key);
    TabWidget* parentTabWidget() {return m_parent_tabwidget;}
    const QStringList& key() {return m_key;}

private:
    ParamTreeModel* m_model;
    QStringList m_key;
    TabWidget* m_tab_widget;
    TabWidget* m_parent_tabwidget;
    QVBoxLayout* m_leaf_layout;
    QVBoxLayout* m_layout;
    QHash<QStringList,ItemWidget*> m_leafs;

    void generateLayout();

    void addItem(const TreeItem& item);
    void removeItem(const TreeItem& item);
};



#endif // TabPage_H
