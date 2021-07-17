#include "paramgroupbox.h"
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

ParamGroupBox::ParamGroupBox(ParamTreeModel* model,const QStringList& key,
                                  Depth depth,QWidget *parent) :
    QGroupBox(parent),
    m_model(model),
    m_key(key),
    m_depth(depth),
    m_layout(new QVBoxLayout)
{
    QWidget::setStyleSheet("QGroupBox {font-weight: bold;"
                  "font-size: 14pt}");

    const TreeItem& item = m_model->getItem(m_key);
    if(!item.isRoot())
        QGroupBox::setTitle(item.name());

    generateLayout();
    m_layout->setSizeConstraint(QLayout::SetMinimumSize);

    // ownership of layout transfered to this object
    QGroupBox::setLayout(m_layout);
}

void ParamGroupBox::generateLayout()
{
    m_leafs.clear();
    m_branches.clear();

    // Check that the model has the key, otherwise return
    if(!m_model->hasItem(m_key))
        return;
    m_layout->setSpacing(0);

    const TreeItem& item = m_model->getItem(m_key);
    for(unsigned int i = 0; i < item.childCount(); i++){
        const TreeItem& child = item.getItem(i);
        addItem(child);
    }
}

void ParamGroupBox::addItem(const TreeItem& item)
{
    QModelIndex index = m_model->getIndex(item);
    addItem(index);
}

void ParamGroupBox::addItem(const QModelIndex& index)
{
    const TreeItem& item = m_model->getItem(index);
    QStringList key = m_model->data(index,ParamTreeModel::Role::KEY).toStringList();

    if(item.isBranch() && m_depth == Depth::COMPLETE){
        // m_layout does not take ownership of group_widget
        ParamGroupBox* group_widget = new ParamGroupBox(m_model,key,m_depth);
        m_layout->addWidget(group_widget);
        m_branches.insert(key,group_widget);
    } else if(item.isLeaf()){
        ItemWidget* item_widget = generateItemWidget(m_model,item);
        m_layout->addWidget(item_widget);
        m_leafs.insert(key,item_widget);
    }
}

ParamGroupBox* ParamGroupBox::getGroup(const QModelIndex& index)
{
    QStringList key = m_model->data(index,ParamTreeModel::Role::KEY).toStringList();
    return getGroup(key);
}


void ParamGroupBox::removeItem(const QModelIndex& index)
{
    if(!index.isValid())
        return;
    QStringList key = m_model->data(index,ParamTreeModel::Role::KEY).toStringList();

        // check child branches
    if(m_branches.contains(key)) {
        ParamGroupBox* group = m_branches.take(key);
        delete group;
    }
    if(m_leafs.contains(key)){
        // check leafs
        ItemWidget* widget = m_leafs.take(key);
        delete widget;
    }
}

void ParamGroupBox::removeItem(const TreeItem& item)
{
    QModelIndex index = m_model->getIndex(item);
    removeItem(index);
}


int ParamGroupBox::leafCount() const
{
    return static_cast<int>(m_leafs.size());
}

int ParamGroupBox::branchCount() const
{
    return static_cast<int>(m_branches.size());
}

int ParamGroupBox::itemCount() const
{
    int count = leafCount();
    for(auto branch : m_branches){
        count += branch->leafCount();
    }
    return count;
}

ParamGroupBox* ParamGroupBox::getGroup(const QStringList& key)
{
    if(key == m_key)
        return this;
    if(m_branches.contains(key)) {
        return m_branches.value(key);
    } else {
        // check branches of the children
        QHash<QStringList,ParamGroupBox*>::iterator iter = m_branches.begin();
        while(iter != m_branches.end()){
            ParamGroupBox* group = iter.value()->getGroup(key);
            if(group != nullptr)
                return group;
            iter++;
        }
    }
    return nullptr;
}




