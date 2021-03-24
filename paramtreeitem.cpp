#include "paramtreeitem.h"
#include "paramtreemodel.h"
#include <QVariant>
#include <QDebug>

TreeItem::TreeItem(const QString& name,const QVariant& val,DataType dt,
                   const AuxMap& aux_map) :
    m_name(name),
    m_val(val),
    m_dtype(dt),
    m_aux_map(aux_map),
    m_parent(nullptr)
{
}

TreeItem::TreeItem(const TreeItem& item) :
    m_name(item.name()),
    m_val(item.value()),
    m_dtype(item.dtype()),
    m_aux_map(item.m_aux_map),
    child_items(),
    m_parent(nullptr)
{
    for(auto child : item.child_items){
        TreeItem* childptr = new TreeItem(*child);
        childptr->m_parent = this;
        child_items.push_back(childptr);
    }
}


TreeItem::~TreeItem()
{
    for(auto it = child_items.begin(); it != child_items.end(); it++)
    {
        delete *it;
    }
    child_items.clear();
}

TreeItem* TreeItem::parent()
{
    return m_parent;
}

TreeItem* TreeItem::child(unsigned int number)
{
    if(number < childCount())
        return child_items.at(number);
    return nullptr;
}

unsigned int TreeItem::childCount() const
{
    return child_items.size();
}

unsigned int TreeItem::childNumber() const
{
    if(m_parent){
        for(unsigned int i = 0; i < m_parent->childCount(); i++){
            if(m_parent->child_items.at(i) == this)
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

bool TreeItem::insertChild(TreeItem* item,unsigned int position)
{
    if(position > child_items.size())
        return false;
    item->m_parent = this;
    child_items.insert(child_items.begin()+position,item);
    return true;
}

void TreeItem::addChild(TreeItem* item)
{
    item->m_parent = this;
    child_items.push_back(item);
}

bool TreeItem::insertItem(const TreeItem& item,unsigned int position)
{
    if(position > child_items.size())
        return false;
    TreeItem* child = new TreeItem(item);
    child->m_parent = this;
    child_items.insert(child_items.begin()+position,child);
    return true;
}

void TreeItem::addItem(const TreeItem& item)
{
    TreeItem* child = new TreeItem(item);
    child->m_parent = this;
    child_items.push_back(child);
}

bool TreeItem::insertChildren(unsigned int position,unsigned int count)
{
    if(count < 1)
        return false;
    if(position > child_items.size())
        return false;
    for(unsigned int i = 0; i < count; i++){
        TreeItem* item = new TreeItem("",QVariant());
        item->m_parent = this;
        child_items.insert(child_items.begin()+position,item);
    }
    return true;
}

bool TreeItem::removeChildren(unsigned int position, unsigned int count)
{
    if(count < 1)
        return false;
    if (position + count > child_items.size())
        return false;
    for(auto it = child_items.begin()+position; it!=child_items.begin()+position+count; it++)
        delete *it;
    child_items.erase(child_items.begin()+position,child_items.begin()+position+count);
    return true;
}

const TreeItem& TreeItem::getItem(const QString &name) const
{
    for(unsigned int i = 0; i < child_items.size(); i++) {
        if(child_items.at(i)->name() == name)
            return *child_items.at(i);
    }
    throw TreeKeyException(name);
}

const TreeItem& TreeItem::getItem(unsigned int position) const
{
    return *child_items.at(position);
}

const std::vector<TreeItem*> TreeItem::getLeafs() const
{
    std::vector<TreeItem*> leaf_vector;
    for(unsigned int i = 0; i < child_items.size(); i++) {
        if(child_items.at(i)->isLeaf())
            leaf_vector.push_back(child_items.at(i));
    }
    return leaf_vector;
}

const std::vector<TreeItem*> TreeItem::getBranches() const
{
    std::vector<TreeItem*> branch_vector;
    for(unsigned int i = 0; i < child_items.size(); i++) {
        if(child_items.at(i)->isBranch())
            branch_vector.push_back(child_items.at(i));
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

QStringList TreeItem::pathkey() const
{
    QStringList path(m_name);
    TreeItem* parent = m_parent;
    while (parent){
        path.prepend(parent->name());
        parent = parent->parent();
    }
    return path;
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

    if(m_aux_map.contains(aux_name)){
        return m_aux_map.value(aux_name);
    }
    throw std::runtime_error("TreeItem::getAux exception: failed to recognize \
                              aux_name " + aux_name.toStdString());
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
































