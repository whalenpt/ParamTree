#include "tabwidget/tabpage.h"
#include "tabwidget/tabwidget.h"
#include "modelview/treemodel.h"
#include "modelview/treeitem.h"
#include "itemwidgets/itemwidget.h"
#include "itemwidgets/generatewidgets.h"

#include <QString>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QMetaType>
#include <QWidget>
#include <QLayout>
#include <QStringList>
#include <QDebug>
#include <QSpacerItem>

namespace paramtree{

TabPage::TabPage(TreeModel* model,const QStringList& key,
                 TabWidget *parent_tabwidget) :
    QWidget(parent_tabwidget),
    m_model(model),
    m_key(key),
    m_tab_widget(nullptr),
    m_parent_tabwidget(parent_tabwidget),
    m_leaf_layout(new QVBoxLayout),
    m_layout(new QVBoxLayout)
{
    QWidget::setStyleSheet("QLabel,QLineEdit,QSpinBox,QComboBox {font-size: 14pt}");
    generateLayout();
    // ownership of layout transfered to this object
    QWidget::setLayout(m_layout);
}

void TabPage::generateLayout()
{
    // Check that the model has the key, otherwise return
    if(!m_model->hasItem(m_key))
        return;

    m_layout->addLayout(m_leaf_layout);
    const TreeItem& parent_item = m_model->getItem(m_key);
    for(unsigned int i = 0; i < parent_item.childCount(); i++){
        const TreeItem& child = parent_item.getItem(i);
        addItem(child);
    }

    QSpacerItem* vspacer = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    m_layout->addItem(vspacer);
}

void TabPage::addItem(const TreeItem& item)
{
    QModelIndex index = m_model->getIndex(item);
    addItem(index);
}

void TabPage::addItem(const QModelIndex& index)
{
    if(!index.isValid())
        return;
    const TreeItem& item = m_model->getItem(index);
    QStringList key = m_model->data(index,TreeModel::Role::KEY).toStringList();

    if(item.isBranch()) {
        TabPage* tab_page = new TabPage(m_model,key);
        if(!m_tab_widget){
            m_tab_widget = new TabWidget;
            m_layout->addWidget(m_tab_widget);
        }
        m_tab_widget->addTab(key,tab_page);
    }
    else if(item.isLeaf()){
        ItemWidget* item_widget = generateItemWidget(m_model,item);
        m_leafs.insert(key,item_widget);
        m_leaf_layout->addWidget(item_widget);
    }
}

void TabPage::removeItem(const QModelIndex& index)
{
    if(!index.isValid())
        return;
    QStringList key = m_model->data(index,TreeModel::Role::KEY).toStringList();
    if(m_leafs.contains(key)){
        ItemWidget* widget = m_leafs.take(key);
        m_leaf_layout->removeWidget(widget);
        delete widget;
    }
    if(m_tab_widget){
        if(m_tab_widget->contains(key))
            m_tab_widget->removeTab(key);
        if(!m_tab_widget->count()){
            m_layout->removeWidget(m_tab_widget);
            delete m_tab_widget;
            m_tab_widget = nullptr;
        }
    }
}

TabPage* TabPage::tabPage(const QModelIndex& index)
{
    // Find key of the corresponding index in the model and use this to find the tabPage
    QStringList key = m_model->data(index,TreeModel::Role::KEY).toStringList();
    return tabPage(key);
}


// Note that the key is the model key but the TabPage can contain a
// subset of the model data if the parent key is not the root. In this case
// we return a nullptr if the TabPage does not contain the data corresponding to the key
TabPage* TabPage::tabPage(const QStringList& key)
{
    // check if key is the parent key of the TabPage
    if(key == m_key)
        return this;
    // check if key is shorter than or equal to the length of the parent (m_key)
    if(key.count() <= m_key.count())
        return nullptr;

    // key is longer than the parent key and hence might be a branch of the parent
    // check that the first strings in the key correspond to the parent key
    if(key.mid(0,m_key.count()) != m_key)
        return nullptr;

//    // returns nullptr if TabWidget cannot find key.
    return m_tab_widget->tabPage(key);
}

}







