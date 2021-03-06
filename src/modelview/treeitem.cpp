#include "modelview/treeitem.h"
#include "modelview/treemodel.h"
#include <QVariant>
#include <QDebug>
#include <stdexcept>

namespace paramtree{

TreeItem::TreeItem(const QString& name,const QVariant& val,DataType dt,
                   const AuxMap& aux_map) :
    m_name(name),
    m_val(val),
    m_dtype(dt),
    m_aux_map(aux_map),
    m_child_items(),
    m_parent(nullptr)
{
}

TreeItem::TreeItem(const TreeItem& item) :
    m_name(item.m_name),
    m_val(item.m_val),
    m_dtype(item.m_dtype),
    m_aux_map(item.m_aux_map),
    m_child_items(),
    m_parent(item.m_parent)
{
    for(const auto& child : item.m_child_items){
        std::unique_ptr<TreeItem> child_ptr = std::make_unique<TreeItem>(*child.get());
        child_ptr->m_parent = this;
        m_child_items.push_back(std::move(child_ptr));
    }
}

TreeItem& TreeItem::operator=(const TreeItem& item)
{
    m_name = item.m_name;
    m_val = item.m_val;
    m_dtype = item.m_dtype;
    m_aux_map = item.m_aux_map;
    m_parent = item.m_parent;
    m_child_items.clear();
    for(const auto& child : item.m_child_items){
        std::unique_ptr<TreeItem> child_ptr = std::make_unique<TreeItem>(*child.get());
        child_ptr->m_parent = this;
        m_child_items.push_back(std::move(child_ptr));
    }
    return *this;
}

TreeItem::~TreeItem()
{
    m_child_items.clear();
}

TreeItem* TreeItem::parent()
{
    return m_parent;
}

TreeItem* TreeItem::child(unsigned int number)
{
    if(number < childCount())
        return m_child_items.at(number).get();
    return nullptr;
}

unsigned int TreeItem::childCount() const
{
    return m_child_items.size();
}

unsigned int TreeItem::childNumber() const
{
    if(m_parent){
        for(unsigned int i = 0; i < m_parent->childCount(); i++){
            if(m_parent->m_child_items.at(i).get() == this)
                return i;
        }
    }
    return 0;
}

QString TreeItem::name() const
{
    return m_name;
}

QVariant TreeItem::value() const
{
    return m_val;
}

TreeItem::DataType TreeItem::dtype() const
{
    return m_dtype;
}


bool TreeItem::isBranch() const
{
    if(m_dtype == DataType::BRANCH)
        return true;
    return (childCount() > 0 ? true : false);
}

bool TreeItem::isLeaf() const
{
    return !isBranch();
}


void TreeItem::setName(const QString &name)
{
    m_name = name;
}

void TreeItem::setValue(const QVariant &value)
{
    m_val = value;
}

bool TreeItem::insertItem(std::unique_ptr<TreeItem> item,unsigned int position)
{
    if(position > m_child_items.size())
        return false;
    item->m_parent = this;
    m_child_items.insert(m_child_items.begin()+position,std::move(item));
    return true;
}

bool TreeItem::insertItem(const TreeItem& item,unsigned int position)
{
    std::unique_ptr<TreeItem> item_ptr = std::make_unique<TreeItem>(item);
    return insertItem(std::move(item_ptr),position);
}

void TreeItem::addItem(std::unique_ptr<TreeItem> item)
{
    item->m_parent = this;
    m_child_items.push_back(std::move(item));
}

void TreeItem::addItem(const TreeItem& item)
{
    std::unique_ptr<TreeItem> item_ptr = std::make_unique<TreeItem>(item);
    addItem(std::move(item_ptr));
}

bool TreeItem::insertChildren(unsigned int position,unsigned int count)
{
    if(count < 1)
        return false;
    if(position > m_child_items.size())
        return false;
    for(unsigned int i = 0; i < count; i++){
        auto item = std::make_unique<TreeItem>("",QVariant());
        item->m_parent = this;
        m_child_items.insert(m_child_items.begin()+position,std::move(item));
    }
    return true;
}

std::unique_ptr<TreeItem> TreeItem::pluckChild(unsigned int position)
{
    if (position > m_child_items.size())
        return nullptr;
    std::unique_ptr<TreeItem> item = std::move(m_child_items.at(position));
    m_child_items.erase(m_child_items.begin()+position,m_child_items.begin()+position+1);
    return item;
}

bool TreeItem::removeChildren(unsigned int position, unsigned int count)
{
    if(count < 1)
        return false;
    if (position + count > m_child_items.size())
        return false;
    m_child_items.erase(m_child_items.begin()+position,m_child_items.begin()+position+count);
    return true;
}

const TreeItem& TreeItem::getItem(const QString &name) const
{
    for(unsigned int i = 0; i < m_child_items.size(); i++) {
        if(m_child_items.at(i)->name() == name)
            return *m_child_items.at(i);
    }
    throw TreeKeyException(name);
}

const TreeItem& TreeItem::getItem(unsigned int position) const
{
    return *m_child_items.at(position);
}

const std::vector<TreeItem*> TreeItem::getLeafs() const
{
    std::vector<TreeItem*> leaf_vector;
    for(unsigned int i = 0; i < m_child_items.size(); i++) {
        if(m_child_items.at(i)->isLeaf())
            leaf_vector.push_back(m_child_items.at(i).get());
    }
    return leaf_vector;
}

const std::vector<TreeItem*> TreeItem::getBranches() const
{
    std::vector<TreeItem*> branch_vector;
    for(unsigned int i = 0; i < m_child_items.size(); i++) {
        if(m_child_items.at(i)->isBranch())
            branch_vector.push_back(m_child_items.at(i).get());
    }
    return branch_vector;
}

bool TreeItem::isRoot() const
{
    if(m_parent == nullptr)
        return true;
    else
        return false;
}

QStringList TreeItem::pathkey(bool include_root) const
{
    QStringList path(m_name);
    TreeItem* parent = m_parent;
    while (parent){
        path.prepend(parent->name());
        parent = parent->parent();
    }
    if(include_root)
        return path;
    else
        return path.mid(1);
}

void TreeItem::setAux(const QString& aux_name,const QVariant& aux_val)
{
    m_aux_map.insert(aux_name,aux_val);
}

void TreeItem::setAux(const QString& aux_name,const QVector<QVariant>& aux_val)
{
    m_aux_map.insert(aux_name,QVariant::fromValue(aux_val));
}

QVariant TreeItem::aux(const QString& aux_name) const
{
    // Returns QVariant() if aux_name is not in the map

    if(hasAux(aux_name))
        return m_aux_map.value(aux_name);
    return QVariant();
//    throw std::runtime_error("TreeItem::getAux exception: failed to recognize \
//                              aux_name " + aux_name.toStdString());
}

bool TreeItem::hasAux(const QString& aux_key) const
{
    if(m_aux_map.contains(aux_key))
        return true;
    return false;
}

const AuxMap& TreeItem::auxMap() const
{
    return m_aux_map;
}

}
































