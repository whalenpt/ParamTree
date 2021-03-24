#include "paramtreewidget.h"
#include "paramtreemodel.h"
#include "paramgroupbox.h"
#include <QVBoxLayout>
#include <QStringList>

ParamTreeWidget::ParamTreeWidget(ParamTreeModel* model,const QStringList& key,
                             Depth depth,QWidget* parent) :
    QWidget(parent),
    m_model(model),
    m_key(key),
    m_depth(depth),
    m_group_widget(nullptr)
{
    m_layout = new QVBoxLayout;
    m_group_widget = new ParamGroupBox(model,key,depth);
    m_layout->addWidget(m_group_widget);

    connect(m_model,&QAbstractItemModel::modelReset,this,
            &ParamTreeWidget::reloadWidgets);

    connect(m_model,&QAbstractItemModel::rowsAboutToBeRemoved,this,
            &ParamTreeWidget::removeWidgets);

    connect(m_model,&QAbstractItemModel::rowsInserted,this,
            &ParamTreeWidget::insertWidgets);

    setLayout(m_layout);
}

void ParamTreeWidget::setTitle(const QString& str)
{
    m_group_widget->setTitle(str);
}

void ParamTreeWidget::removeWidgets(const QModelIndex& parent,int first,int last)
{
    // If the first index is greater than or equal to the last then removeRows
    // of the QAbstractItemModel was called with a count of 0.
    if(first > last)
        return;

    ParamGroupBox* parent_group = m_group_widget->getGroup(parent);
    if(parent_group){
        QModelIndex index(m_model->index(first,0,parent));
        parent_group->removeItem(index);
    }
}

void ParamTreeWidget::insertWidgets(const QModelIndex& parent,int start,int end)
{
    if(start > end)
        return;

    ParamGroupBox* parent_group = m_group_widget->getGroup(parent);
    if(parent_group){
        QModelIndex index = m_model->index(start,0,parent);
        parent_group->addItem(index);
    }
}

void ParamTreeWidget::reloadWidgets()
{
    if(m_group_widget != nullptr){
        m_layout->removeWidget(m_group_widget);
        delete m_group_widget;
        m_group_widget = nullptr;
    }

    if(m_model->hasItem(m_key)){
        m_group_widget = new ParamGroupBox(m_model,m_key,m_depth);
        m_layout->addWidget(m_group_widget);
    }
}



