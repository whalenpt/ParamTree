#ifndef ParamGroupBox_H
#define ParamGroupBox_H

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

class ParamGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit ParamGroupBox(ParamTreeModel* model,const QStringList& key,
                              Depth depth = Depth::COMPLETE,QWidget *parent = nullptr);
    int leafCount() const;
    int branchCount() const;
    int itemCount() const;
    void addItem(const QModelIndex& index);
    void removeItem(const QModelIndex& index);
    ParamGroupBox* getGroup(const QModelIndex& index);

    const QStringList& key() {return m_key;}

private:
    ParamTreeModel* m_model;
    QStringList m_key;
    Depth m_depth;
    QVBoxLayout* m_layout;
    void generateLayout();
    QHash<QStringList,ItemWidget*> m_leafs;
    QHash<QStringList,ParamGroupBox*> m_branches;
    ParamGroupBox* getGroup(const QStringList& key);
    void addItem(const TreeItem& item);
    void removeItem(const TreeItem& item);

};



#endif // ParamGroupBox_H
