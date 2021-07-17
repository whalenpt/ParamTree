#include "groupboxwidget.h"
#include "paramtreemodel.h"
#include "paramtreeitem.h"
#include "itemwidget.h"
#include "generatewidgets.h"

#include <QString>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMetaType>
#include <QWidget>
#include <QLayout>
#include <QStringList>
#include <QDebug>

GroupBoxWidget::GroupBoxWidget(ParamTreeModel* model,const QStringList& key,
                                  Depth depth,QWidget *parent) :
    QGroupBox(parent),
    m_model(model),
    m_key(key),
    m_depth(depth),
    m_layout(new QVBoxLayout)
{
//    QWidget::setStyleSheet("QGroupBox {font-weight: bold;"
//                  "font-size: 14pt}");
    setStyleSheet("QGroupBox {font-weight: bold;"
                  "font-size: 14pt}");

    const TreeItem& item = m_model->getItem(m_key);
    if(!item.isRoot())
        QGroupBox::setTitle(item.name());
    generateLayout();

    // ownership of layout transfered to this object
    QGroupBox::setLayout(m_layout);
}

void GroupBoxWidget::setSpacing(int val)
{
    m_layout->setSpacing(val);
}

void GroupBoxWidget::generateLayout()
{
    m_leafs.clear();
    m_branches.clear();

    // Check that the model has the key, otherwise return
    if(!m_model->hasItem(m_key))
        return;

    const TreeItem& item = m_model->getItem(m_key);
    for(unsigned int i = 0; i < item.childCount(); i++){
        const TreeItem& child = item.getItem(i);
        addItem(child);
    }
}

void GroupBoxWidget::addItem(const TreeItem& item)
{
    QModelIndex index = m_model->getIndex(item);
    addItem(index);
}

void GroupBoxWidget::addItem(const QModelIndex& index)
{
    if(!index.isValid())
        return;

    const TreeItem& item = m_model->getItem(index);
    QStringList key = m_model->data(index,ParamTreeModel::Role::KEY).toStringList();

    if(item.isBranch() && m_depth == Depth::COMPLETE){
        // m_layout does not take ownership of group_widget
        GroupBoxWidget* group_widget = new GroupBoxWidget(m_model,key,m_depth);
        m_layout->addWidget(group_widget);
        m_branches.insert(key,group_widget);
    } else if(item.isLeaf()){
        ItemWidget* item_widget = generateItemWidget(m_model,item);
        m_layout->addWidget(item_widget);
        m_leafs.insert(key,item_widget);
    }
}

GroupBoxWidget* GroupBoxWidget::getGroup(const QModelIndex& index)
{
    QStringList key = m_model->data(index,ParamTreeModel::Role::KEY).toStringList();
    return getGroup(key);
}


void GroupBoxWidget::removeItem(const QModelIndex& index)
{
    if(!index.isValid())
        return;
    QStringList key = m_model->data(index,ParamTreeModel::Role::KEY).toStringList();

        // check child branches
    if(m_branches.contains(key)) {
        GroupBoxWidget* group = m_branches.take(key);
        delete group;
    }
    if(m_leafs.contains(key)){
        // check leafs
        ItemWidget* widget = m_leafs.take(key);
        delete widget;
    }
}

void GroupBoxWidget::removeItem(const TreeItem& item)
{
    QModelIndex index = m_model->getIndex(item);
    removeItem(index);
}


GroupBoxWidget* GroupBoxWidget::getGroup(const QStringList& key)
{
    if(key == m_key)
        return this;
    if(m_branches.contains(key)) {
        return m_branches.value(key);
    } else {
        // check branches of the children
        QHash<QStringList,GroupBoxWidget*>::iterator iter = m_branches.begin();
        while(iter != m_branches.end()){
            GroupBoxWidget* group = iter.value()->getGroup(key);
            if(group != nullptr)
                return group;
            iter++;
        }
    }
    return nullptr;
}

