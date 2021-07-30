#include "modelview/treemodel.h"
#include "modelview/treeitem.h"
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include <QByteArray>
#include <stdexcept>

namespace paramtree{

const QString NAME_HEADER("Param");
const QString VALUE_HEADER("Value");
const QString ROOT_NAME("PARAMTREEMODEL");

const QChar XML_SEPERATOR('|');
const QString FILE_FORMAT("PARAMTREE");
const QString VERSION_NUM("1.1");

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent),
    m_root_item(std::make_unique<TreeItem>(ROOT_NAME))
{
    connect(this,&QAbstractItemModel::dataChanged,this,&TreeModel::linkUpdate);
}

void TreeModel::boolLink(const QModelIndex& index,const QStringList& key,int position)
{
    TreeItem* boolItem = itemForIndex(index);
    if(boolItem->dtype() != TreeItem::DataType::BOOL){
        throw std::invalid_argument("TreeModel::boolLink error, index must be\
                a reference to a TreeItem with DataType of BOOL"); 
    }
    if(!hasItem(key) && !hiddenItem(key))
        throw std::invalid_argument("TreeModel::boolLink error, key is not found in model."); 

    QModelIndex val_index = getValIndex(index);
    if(position < 0)
        position = getIndex(key).row();
    m_bool_links.insert(std::make_pair(val_index,std::make_pair(key,position)));
    boolLinkUpdate(val_index);
}

void TreeModel::comboLink(const QModelIndex& index,const QStringList& key,\
        const QString& combo_str,int position)
{
    TreeItem* comboItem = itemForIndex(index);
    if(comboItem->dtype() != TreeItem::DataType::COMBO){
        throw std::invalid_argument("TreeModel::comboLink error, index must be\
                a reference to a TreeItem with DataType of COMBO"); 
    }

    if(!hasItem(key) && !hiddenItem(key))
        throw std::invalid_argument("TreeModel::comboLink error, key is not found in model."); 

    QModelIndex val_index = getValIndex(index);
    if(position < 0)
        position = getIndex(key).row();
    m_combo_links.insert(std::make_pair(val_index,std::make_tuple(key,combo_str,position)));
    comboLinkUpdate(val_index);
}

void TreeModel::boolLinkUpdate(const QModelIndex& index)
{
//    qDebug() << "BOOL LINK UPDATE CALLED";
    auto link_itr1 = m_bool_links.lower_bound(index);
    auto link_itr2 = m_bool_links.upper_bound(index);
    while(link_itr1 != link_itr2){
        bool link = getItem(index).value().toBool();
        if(link_itr1->first == index){
            auto& [key,row] = link_itr1->second;
            if(!link && hasItem(key)){
//                qDebug() << "NEED TO ERASE FROM TREE!";
                QModelIndex keyindex = getIndex(key);
                TreeItem* parent = itemForIndex(keyindex.parent());
                beginRemoveRows(keyindex.parent(),keyindex.row(),keyindex.row());
                std::unique_ptr<TreeItem> item = parent->pluckChild(keyindex.row());
                m_bool_links_map.insert(std::pair(key,std::pair(std::move(item),row))); 
                endRemoveRows();
            }
            else if(link && !hasItem(key)){
//                qDebug() << "NEED TO ADD TO TREE!";
                auto itr = m_bool_links_map.find(key);
                if(itr != m_bool_links_map.end()){
                    insertItem(std::move(itr->second.first),itr->second.second,getIndex(key.first(key.size()-1)));
                    m_bool_links_map.erase(itr);
                }
            }
        }
        link_itr1++;
    }
}

void TreeModel::comboLinkUpdate(const QModelIndex& index)
{

    auto linkdel_itr = m_combo_links.lower_bound(index);
    auto linkend_itr = m_combo_links.upper_bound(index);
    while(linkdel_itr != linkend_itr){
        if(linkdel_itr->first == index){
            QString linkval = getItem(index).value().toString();
            auto& [key,combostr,row] = linkdel_itr->second;
            if((combostr != linkval) && hasItem(key)){
//                qDebug() << "NEED TO ERASE FROM TREE!";
                QModelIndex keyindex = getIndex(key);
                TreeItem* parent = itemForIndex(keyindex.parent());
                beginRemoveRows(keyindex.parent(),keyindex.row(),keyindex.row());
                std::unique_ptr<TreeItem> item = parent->pluckChild(keyindex.row());
                m_combo_links_map.insert(std::pair(key,std::pair(std::move(item),row))); 
                endRemoveRows();
            }
        }
        linkdel_itr++;
    }
    auto linkadd_itr = m_combo_links.lower_bound(index);
    while(linkadd_itr != linkend_itr){
        if(linkadd_itr->first == index){
            QString linkval = getItem(index).value().toString();
            auto& [key,combostr,row] = linkadd_itr->second;
            if((combostr == linkval) && !hasItem(key)){
//                qDebug() << "NEED TO ERASE FROM TREE!";
                auto itr = m_combo_links_map.find(key);
                if(itr != m_combo_links_map.end()){
                    insertItem(std::move(itr->second.first),itr->second.second,getIndex(key.first(key.size()-1)));
                    m_combo_links_map.erase(itr);
                }
            }
        }
        linkadd_itr++;
    }
}

void TreeModel::linkUpdate(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    boolLinkUpdate(topLeft);
    comboLinkUpdate(topLeft);
}

TreeItem* TreeModel::itemForIndex(const QModelIndex& index) const
{
    if(index.isValid()){
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if(item)
            return item;
    }
    return m_root_item.get();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if(section == 0)
            return NAME_HEADER;
        else if(section == 1)
            return VALUE_HEADER;
    }
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();
    TreeItem* parent_item = itemForIndex(parent);
    TreeItem* child_item = parent_item->child(row);
    if(child_item)
        return createIndex(row,column,child_item);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    TreeItem* child_item = itemForIndex(index);
    TreeItem* parent_item = child_item->parent();
    if(parent_item == m_root_item.get())
        return QModelIndex();
    return createIndex(parent_item->childNumber(),0,parent_item);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem* item = itemForIndex(parent);
    return item->childCount();
}

int TreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem* item = itemForIndex(index);
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
        int col = index.column();
        if(col == 0)
            return item->name();
        else if(col == 1){
            if(item->dtype() == TreeItem::DataType::BOOL)
                return QVariant();
            return item->value();
        }
    } else if (role == Role::KEY){
         // first item in pathkey is the root name if argument is not false 
        return QVariant(item->pathkey(false));
    } else if (role == Role::STRINGKEY) 
        return item->pathkey(false).join("/");
    else if (role == Role::DATATYPE) 
        return static_cast<int>(item->dtype());
    else if (role == Role::RANGE)
        return item->aux("RANGE");
    else if (role == Role::MIN)
        return item->aux("MIN");
    else if (role == Role::MAX)
        return item->aux("MAX");
    else if (role == Role::AUXMAP)
        return item->auxMap();
    else if (role == Qt::CheckStateRole && index.column() == 1 && \
            item->dtype() == TreeItem::DataType::BOOL){
        if(item->value().toBool())
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }
        
    return QVariant();
}


bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole || role == Qt::DisplayRole){
        if (data(index, role) != value) {
            TreeItem* item = itemForIndex(index);
            if(item->dtype() == TreeItem::DataType::BOOL)
                return false;
            int col = index.column();
            if(col == 0)
                item->setName(value.toString());
            else if(col == 1){
                item->setValue(value);
            }
            else
                return false;

            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    } else if (role == Role::RANGE) {
        TreeItem* item = itemForIndex(index);
        item->setAux("RANGE",value);
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    } else if (role == Qt::CheckStateRole && 
            index.column() == 1 && 
            static_cast<TreeItem::DataType>( data(index,Role::DATATYPE).toInt()) == TreeItem::DataType::BOOL) 
    {
        if (data(index, role) != value) {
            TreeItem* item = itemForIndex(index);
            if(static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked)
                item->setValue(true);
            else
                item->setValue(false);
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if(index.column() == 1 && itemForIndex(index)->isLeaf()){
        if(static_cast<TreeItem::DataType>(data(index,Role::DATATYPE).toInt()) == \
                TreeItem::DataType::BOOL){
            return Qt::ItemIsUserCheckable | QAbstractItemModel::flags(index); 
        }
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    return QAbstractItemModel::flags(index);
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    TreeItem* parent_item = itemForIndex(parent);
    bool success;

    beginInsertRows(parent, row, row + count - 1);
    success = parent_item->insertChildren(row,count);
    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parent_item = itemForIndex(parent);
    bool success = true;
    beginRemoveRows(parent,row,row+count - 1);
    success = parent_item->removeChildren(row,count);
    endRemoveRows();
    return success;
}

QModelIndex TreeModel::addItem(std::unique_ptr<TreeItem> item,const QModelIndex& parent_index)
{
    TreeItem* parent = itemForIndex(parent_index);
    int position = parent->childCount();
    beginInsertRows(parent_index,position,position);
    parent->insertItem(std::move(item),position);
    endInsertRows();
    return index(position,0,parent_index);
}


QModelIndex TreeModel::addItem(const TreeItem& item,const QModelIndex& parent_index)
{
    TreeItem* parent = itemForIndex(parent_index);
    int position = parent->childCount();
    beginInsertRows(parent_index,position,position);
    parent->insertItem(item,position);
    endInsertRows();
    return index(position,0,parent_index);
}

QModelIndex TreeModel::insertItem(std::unique_ptr<TreeItem> item,unsigned int position,const QModelIndex& parent_index)
{
    TreeItem* parent = itemForIndex(parent_index);
    if(position > parent->childCount())
        position = parent->childCount();
    beginInsertRows(parent_index,position,position);
    parent->insertItem(std::move(item),position);
    endInsertRows();
    return index(position,0,parent_index);
}


QModelIndex TreeModel::insertItem(const TreeItem& item,unsigned int position,const QModelIndex& parent_index)
{
    TreeItem* parent = itemForIndex(parent_index);
    if(position > parent->childCount())
        position = parent->childCount();
    beginInsertRows(parent_index,position,position);
    parent->insertItem(item,position);
    endInsertRows();
    return index(position,0,parent_index);
}

QModelIndex TreeModel::getIndex(const QString& name,const QModelIndex& parent) const
{
    return indexForPath(parent,QStringList() << name);
}

const TreeItem& TreeModel::getItem(const QString& name,const QModelIndex& parent) const
{
    QModelIndex index = indexForPath(parent,QStringList() << name);
    return *itemForIndex(index);
}

const TreeItem& TreeModel::getItem(const QStringList& key) const
{
    QModelIndex index = indexForPath(QModelIndex(),key);
    return *itemForIndex(index);
}

bool TreeModel::hasItem(const QStringList& key) const
{
    // Empty key corresponds to root index
    if(key.isEmpty())
        return true;

    // Non-empty key, we need to find a valid index
    QModelIndex index = indexForPath(QModelIndex(),key);
    if(index.isValid())
        return true;

    return false;
}

bool TreeModel::hiddenItem(const QStringList& key) const
{
    // Empty key corresponds to root index
    if(key.isEmpty())
        return true;
    if(m_bool_links_map.find(key) != m_bool_links_map.end())
        return true;
    if(m_combo_links_map.find(key) != m_combo_links_map.end())
        return true;
    return false;
}


const TreeItem& TreeModel::getItem(const QModelIndex &index) const
{
    return *itemForIndex(index);
}

const TreeItem& TreeModel::getRootItem() const
{
    return *m_root_item;
}

QModelIndex TreeModel::getValIndex(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    if(index.column() == 1)
        return index;

    return TreeModel::index(index.row(),1,index.parent());
}


QModelIndex TreeModel::getNameIndex(const QModelIndex& index) const
{
    if(!index.isValid())
         return QModelIndex();

    if(index.column() == 0)
         return index;

    return TreeModel::index(index.row(),0,index.parent());
}



QModelIndex TreeModel::getIndex(const TreeItem& item) const
{
    QStringList pathkey = item.pathkey();
    // Check that item has a root consistent with the model
    if(pathkey.at(0) !=  ROOT_NAME)
        return QModelIndex();

    return indexForPath(QModelIndex(), pathkey.mid(1));
}

QModelIndex TreeModel::getIndex(const QStringList& key) const
{
    return indexForPath(QModelIndex(),key);
}

QModelIndex TreeModel::getValIndex(const TreeItem& item) const
{
    return getValIndex(getIndex(item));
}

QModelIndex TreeModel::getValIndex(const QStringList& key) const
{
    return getValIndex(getIndex(key));
}

// Settings warning: QVariant type not necessarily saved properly (int->qlonglong)
void TreeModel::saveToSettings(QSettings& settings, const TreeItem& item) const
{
    if(item.isLeaf()){
        QModelIndex index = getIndex(item);
        if(!index.isValid())
            return;
        QString settings_key = data(index,Role::STRINGKEY).toString();
        settings.setValue(settings_key,item.value());
    } else{
        for(const auto& child : item.m_child_items)
            saveToSettings(settings,*child);
    }
}

void TreeModel::saveToSettings(QSettings& settings,
                                const QModelIndex& index) const
{
    if(!index.isValid())
        return;
    saveToSettings(settings,*itemForIndex(index));
}

QVariant TreeModel::readFromSettings(QSettings& settings,const QStringList& key)
{
    if(key.isEmpty())
        return QVariant();
    QString settings_key = key.join("/");
    if(settings.contains(settings_key))
        return settings.value(settings_key);
    else
        return QVariant();
}


// Returns index of parameter name (column 0)
QModelIndex TreeModel::indexForPath(const QModelIndex &parent,
                                    const QStringList &path) const
{
    if (path.isEmpty())
        return QModelIndex();

    for (int row = 0; row < rowCount(parent); ++row) {
        QModelIndex thisIndex = index(row, 0, parent);
        if (data(thisIndex).toString() == path.at(0)) {
            if (path.count() == 1)
                return thisIndex;
            thisIndex = indexForPath(thisIndex, path.mid(1));
            if (thisIndex.isValid())
                return thisIndex;
        }
    }
    return QModelIndex();
}

QStringList TreeModel::getKey(const QModelIndex& index) const
{
    QStringList key;
    key.push_front(getNameIndex(index).data().toString());
    QModelIndex parent_index(index.parent());
    while(parent_index.isValid()){
        key.push_front(parent_index.data().toString());
        parent_index = parent_index.parent();
    }
    return key;
}

bool TreeModel::save(const QString& filepath)
{
    QFile file(filepath);
    // If save fails its up to user to deal with it
    if(!file.open(QFile::WriteOnly))
        return false;
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(FILE_FORMAT);
    writer.writeAttribute("VERSION",VERSION_NUM);

    writeTreeItem(writer,m_root_item.get());
    writer.writeEndElement();
    writer.writeEndDocument();
    return true;
}

void TreeModel::writeTreeItem(QXmlStreamWriter& writer,const TreeItem* item)
{
    if(item != m_root_item.get()) {
        writer.writeStartElement("TREENODE");
        writeNode(writer,*item);
    }
    for(const auto& child : item->m_child_items){
        writeTreeItem(writer,child.get());
    }
    if(item != m_root_item.get())
        writer.writeEndElement();
}

void writeNode(QXmlStreamWriter& writer,const TreeItem& item)
{
    writer.writeAttribute("NAME",item.name());
    writer.writeAttribute("TYPENAME",item.value().typeName());
    writer.writeAttribute("NODETYPE",QString::number(static_cast<int>(item.dtype())));
    writer.writeAttribute("VALUE",item.value().toString());

    writer.writeStartElement("AUXMAP");
    writeAuxMap(writer,item.auxMap());
    writer.writeEndElement();
}

void writeAuxMap(QXmlStreamWriter& writer,const QMap<QString,QVariant>& map)
{
    QMapIterator<QString,QVariant> aux_iter(map);
    while(aux_iter.hasNext()){
        aux_iter.next();
        writer.writeStartElement("AUXPARAM");
        writer.writeAttribute("KEY",aux_iter.key());

        QVariant val(aux_iter.value());
        writer.writeAttribute("TYPENAME",val.typeName());
        if(val.metaType().id() == QMetaType::QStringList)
            writer.writeAttribute("VALUE",val.toStringList().join(XML_SEPERATOR));
        else
            writer.writeAttribute("VALUE",val.toString());

        writer.writeEndElement();
    }
}


bool TreeModel::load(const QString& filename)
{
    if(filename.isEmpty())
        return false;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    clear();

    QXmlStreamReader reader(&file);
    //Check file type has TreeModel data in it (in the current version)
    reader.readNextStartElement();
    if(reader.name() != FILE_FORMAT)
        return false;
    if(reader.attributes().value("VERSION") != VERSION_NUM)
        return false;

    readTreeItems(reader,m_root_item.get());
    beginResetModel();
    endResetModel();

    return true;
}


void TreeModel::readTreeItems(QXmlStreamReader& reader,TreeItem* item)
{
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isStartElement()){
            std::unique_ptr<TreeItem> child(readNode(reader));
            child->m_parent = item;
            item->m_child_items.push_back(std::move(child));
            item = child.get();
        } else if(reader.isEndElement()){
            item = item->m_parent;
        }
    }
}

TreeItem* readNode(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attributes(reader.attributes());

    QString name = attributes.value("NAME").toString();
    QByteArray typestr(attributes.value("TYPENAME").toString().toUtf8());
    QString dtypestr = attributes.value("NODETYPE").toString();
    QString valuestr = attributes.value("VALUE").toString();

    QMetaType meta_type = QMetaType::fromName(typestr);
    QVariant value(valuestr);
    value.convert(meta_type);
    TreeItem::DataType dtype(static_cast<TreeItem::DataType>(dtypestr.toInt()));
    TreeItem* child = new TreeItem(name,value,dtype);

    reader.readNextStartElement();
    readAuxMap(reader,child);
    return child;
}

void readAuxMap(QXmlStreamReader& reader,TreeItem* item)
{
    while(!reader.isEndElement()){
        reader.readNext();
        if(reader.isStartElement() && (reader.name() == QString("AUXPARAM"))){
            QXmlStreamAttributes attributes(reader.attributes());
            QString key(attributes.value("KEY").toString());
            QVariant val = QVariant(attributes.value("VALUE").toString());
            QString valtype = attributes.value("TYPENAME").toString();
            if(valtype == "QStringList"){
                item->setAux(key,val.toString().split(XML_SEPERATOR));
            } else{
                QMetaType meta_type = QMetaType::fromName(valtype.toUtf8());
                val.convert(meta_type);
                item->setAux(key,val);
            }

            while(!reader.isEndElement())
                reader.readNext();
            reader.readNext();
        }
    }
}

void TreeModel::clear()
{
    m_root_item.reset();
    m_root_item = std::make_unique<TreeItem>(ROOT_NAME);
    beginResetModel();
    endResetModel();
}

}






