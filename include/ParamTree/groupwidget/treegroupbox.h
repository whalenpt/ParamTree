#ifndef TREEGROUPBOX_H
#define TREEGROUPBOX_H

#include <QWidget>
#include <QGroupBox>
#include <QHash>
#include <QString>
#include <QStringList>
#include "ParamTree/itemwidgets/itemwidget.h"
#include "ParamTree/shared/auxsmallclass.h"

class QVBoxLayout;
class QLayout;

namespace paramtree{

class TreeModel;
class TreeItem;

class TreeGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit TreeGroupBox(TreeModel* model,const QStringList& key,
                              Depth depth = Depth::COMPLETE,QWidget *parent = nullptr);
    void addItem(const QModelIndex& index);
    void removeItem(const QModelIndex& index);
    TreeGroupBox* getGroup(const QModelIndex& index);
    const QStringList& key() {return m_key;}
    void setSpacing(int val);

private:
    TreeModel* m_model;
    QStringList m_key;
    Depth m_depth;
    QVBoxLayout* m_layout;
    void generateLayout();
    QHash<QStringList,ItemWidget*> m_leafs;
    QHash<QStringList,TreeGroupBox*> m_branches;
    TreeGroupBox* getGroup(const QStringList& key);
    void addItem(const TreeItem& item);
    void removeItem(const TreeItem& item);
};

}

#endif // GROUPBOXWIDGET_H
