#include "tabwidget/treetabwidget.h"
#include "tabwidget/tabpage.h"
#include "modelview/treemodel.h"
#include <QVBoxLayout>
#include <QStringList>
#include <QDebug>

namespace paramtree{

TreeTabWidget::TreeTabWidget(TreeModel* model,const QStringList& key,QWidget* parent) :
    QWidget(parent),
    m_model(model),
    m_key(key),
    m_tab_page(nullptr)
{
    m_layout = new QVBoxLayout;
    m_tab_page = new TabPage(m_model,m_key);
    m_layout->addWidget(m_tab_page);
    QSpacerItem* vspacer = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    m_layout->addItem(vspacer);

    connect(m_model,&QAbstractItemModel::modelReset,this,
            &TreeTabWidget::reloadWidgets);

    connect(m_model,&QAbstractItemModel::rowsAboutToBeRemoved,this,
            &TreeTabWidget::removeWidgets);

    connect(m_model,&QAbstractItemModel::rowsInserted,this,
            &TreeTabWidget::insertWidgets);

    setLayout(m_layout);
}

void TreeTabWidget::removeWidgets(const QModelIndex& parent,int first,int last)
{
  // If the first index is greater than or equal to the last then removeRows
  // of the QAbstractItemModel was called with a count of 0.
    if(first > last)
        return;

    TabPage* parent_tab = m_tab_page->tabPage(parent);
    if(parent_tab){
        QModelIndex index(m_model->index(first,0,parent));
        parent_tab->removeItem(index);
    }
}

void TreeTabWidget::insertWidgets(const QModelIndex& parent,int start,int end)
{
    if(start > end)
        return;

    TabPage* parent_tab = m_tab_page->tabPage(parent);
    if(parent_tab){
        QModelIndex index = m_model->index(start,0,parent);
        parent_tab->addItem(index);
    }
}

void TreeTabWidget::reloadWidgets()
{
    if(m_tab_page != nullptr){
        m_layout->removeWidget(m_tab_page);
        delete m_tab_page;
        m_tab_page = nullptr;
    }
    if(m_model->hasItem(m_key)){
        m_tab_page = new TabPage(m_model,m_key);
        m_layout->addWidget(m_tab_page);
    }
}

}


