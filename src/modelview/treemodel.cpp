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
const QString VISIBLE_TREE("VISIBLETREE");

const QString LINK_ITEMS("LINKITEMS");
const QString BOOLLINK_ITEMS("BOOLLINKITEMS");
const QString BOOLLINKS("BOOLLINKS");
const QString BOOLHIDDEN("BOOLHIDDEN");
const QString BOOLLINK_NODE("BOOLLINK_NODE");
const QString BOOLHIDDEN_NODE("BOOLHIDDENNODE");

const QString COMBOLINK_ITEMS("COMBOLINKITEMS");
const QString COMBOLINKS("COMBOLINKS");
const QString COMBOHIDDEN("COMBOHIDDEN");
const QString COMBOLINK_NODE("COMBOLINKNODE");
const QString COMBOHIDDEN_NODE("COMBOHIDDENNODE");
const QString VERSION_NUM("0.3");

void streamDiagnostic(QXmlStreamReader& reader)
{
    qDebug() << reader.name();
    qDebug() << reader.tokenString();
    qDebug() << reader.attributes().value("NAME");
}

void streamHiddenDiagnostic(QXmlStreamReader& reader)
{
    qDebug() << reader.name();
    qDebug() << reader.tokenString();
    qDebug() << reader.attributes().value("KEY");
}

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent),
    m_root_item(std::make_unique<TreeItem>(ROOT_NAME))
{
    connect(this,&QAbstractItemModel::dataChanged,this,&TreeModel::linkUpdate);
}

void TreeModel::boolLink(const QStringList& linkkey,const QStringList& key,int position)
{
    QModelIndex val_index = getValIndex(linkkey);
    TreeItem* boolItem = itemForIndex(val_index);
    if(boolItem->dtype() != TreeItem::DataType::BOOL){
        throw std::invalid_argument("TreeModel::boolLink error, index must be\
                a reference to a TreeItem with DataType of BOOL"); 
    }
    if(!hasItem(key) && !hiddenItem(key))
        throw std::invalid_argument("TreeModel::boolLink error, key is not found in model."); 

    if(position < 0)
        position = getIndex(key).row();
    m_bool_links.insert(std::make_pair(linkkey,std::make_pair(key,position)));
    boolLinkUpdate(val_index);
}

void TreeModel::comboLink(const QStringList& linkkey,const QStringList& key,\
        const QString& combo_str,int position)
{
    QModelIndex val_index = getValIndex(linkkey);
    TreeItem* comboItem = itemForIndex(val_index);
    if(comboItem->dtype() != TreeItem::DataType::COMBO){
        throw std::invalid_argument("TreeModel::comboLink error, index must be\
                a reference to a TreeItem with DataType of COMBO"); 
    }

    if(!hasItem(key) && !hiddenItem(key))
        throw std::invalid_argument("TreeModel::comboLink error, key is not found in model."); 

    if(position < 0)
        position = getIndex(key).row();
    m_combo_links.insert(std::make_pair(linkkey,std::make_tuple(key,combo_str,position)));
    comboLinkUpdate(val_index);
}

void TreeModel::boolLinkUpdate(const QModelIndex& index)
{
//    qDebug() << "BOOL LINK UPDATE CALLED";
    QStringList linkkey = getKey(index);
    auto link_itr1 = m_bool_links.lower_bound(linkkey);
    auto link_itr2 = m_bool_links.upper_bound(linkkey);
    while(link_itr1 != link_itr2){
        bool link = getItem(index).value().toBool();
        if(link_itr1->first == linkkey){
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
    QStringList linkkey = getKey(index);
    auto linkdel_itr = m_combo_links.lower_bound(linkkey);
    auto linkend_itr = m_combo_links.upper_bound(linkkey);
    while(linkdel_itr != linkend_itr){
        if(linkdel_itr->first == linkkey){
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
    auto linkadd_itr = m_combo_links.lower_bound(linkkey);
    while(linkadd_itr != linkend_itr){
        if(linkadd_itr->first == linkkey){
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

    writer.writeStartElement(VISIBLE_TREE);
    writeTreeItem(writer,m_root_item.get());
    writer.writeEndElement();

    writer.writeStartElement(LINK_ITEMS);
    writeLinkItems(writer);
    writer.writeEndElement();

    writer.writeEndElement();
    writer.writeEndDocument();
    return true;
}

void TreeModel::writeTreeItem(QXmlStreamWriter& writer,const TreeItem* item)
{
    if(item == nullptr)
        return;
    writer.writeStartElement("TREENODE");
    writeNode(writer,*item);
    for(const auto& child : item->m_child_items)
        writeTreeItem(writer,child.get());
    writer.writeEndElement();
}


void writeNode(QXmlStreamWriter& writer,const TreeItem& item)
{
    writer.writeAttribute("NAME",item.name());
    writer.writeAttribute("TYPENAME",item.value().typeName());
    writer.writeAttribute("NODETYPE",QString::number(static_cast<int>(item.dtype())));
    writer.writeAttribute("VALUE",item.value().toString());
    if(!item.auxMap().empty())
        writeAuxMap(writer,item.auxMap());
}

void writeAuxMap(QXmlStreamWriter& writer,const QMap<QString,QVariant>& map)
{
    writer.writeStartElement("AUXMAP");
    QMapIterator<QString,QVariant> aux_iter(map);
    while(aux_iter.hasNext()){
        aux_iter.next();
        writer.writeStartElement("AUXPARAM");
        writer.writeAttribute("KEY",aux_iter.key());
        QVariant val(aux_iter.value());
        QMetaType meta_type(val.metaType());
        writer.writeAttribute("TYPENAME",meta_type.name());
        if(meta_type.id() == QMetaType::QStringList)
            writer.writeAttribute("VALUE",val.toStringList().join(XML_SEPERATOR));
        else if(meta_type.id() == QMetaType::QVariantList){
            QVariantList val_list = val.toList();
            if(!val_list.isEmpty()){
                QMetaType val_meta_type = val_list.at(0).metaType();
                for(const auto& item : val_list){
                    if(item.metaType() != val_meta_type){
                        throw std::invalid_argument("writeAuxMap requires AUXPARAM QVariantLists\
                                to all be of the same QVariant type.");
                    }
                }
                writer.writeAttribute("SUBTYPENAME",val_meta_type.name());
            } else
                writer.writeAttribute("SUBTYPENAME","void");
            writer.writeAttribute("VALUE",val.toStringList().join(XML_SEPERATOR));
        }
        else
            writer.writeAttribute("VALUE",val.toString());

        writer.writeEndElement();
    }
    writer.writeEndElement();
}

void readAuxMap(QXmlStreamReader& reader,TreeItem* item)
{
    eatStreamCharacters(reader);
    // Advance Reader from AUXMAP start element to AUXPARAM start element
    while(reader.isStartElement() && reader.name() == QString("AUXPARAM")){
        QXmlStreamAttributes attributes(reader.attributes());
        QString key(attributes.value("KEY").toString());
        QVariant val = QVariant(attributes.value("VALUE").toString());
        QString valtype = attributes.value("TYPENAME").toString();
        if(valtype == "QStringList"){
            item->setAux(key,val.toString().split(XML_SEPERATOR));
        } else if(valtype == "QVariantList"){
            QString subvaltype = attributes.value("SUBTYPENAME").toString();
            QMetaType meta_type = QMetaType::fromName(subvaltype.toUtf8());
            QStringList stringlist = val.toString().split(XML_SEPERATOR);
            QList<QVariant> varlist;
            for(const auto& item : stringlist){
                QVariant var(item);
                var.convert(meta_type);
                varlist.push_back(var);
            }
            item->setAux(key,varlist);
        }
        else{
            QMetaType meta_type = QMetaType::fromName(valtype.toUtf8());
            val.convert(meta_type);
            item->setAux(key,val);
        }
        // Read to end element for AUXPARAM
        //while(reader.tokenType() != QXmlStreamReader::EndElement)
        while(!reader.isEndElement())
            reader.readNext();
        // Read until a new start element or the end element for AUXMAP
        while(reader.readNext()){
            if(reader.tokenType() == QXmlStreamReader::EndElement && (reader.name() == QString("AUXMAP")))
                break;
            if(reader.tokenType() == QXmlStreamReader::StartElement)
                break;
        }
    }
    eatStreamCharacters(reader);
    // Reader at end of TREENODE
}


void TreeModel::writeLinkItems(QXmlStreamWriter& writer)
{
    writeBoolLinkItems(writer);
    writeComboLinkItems(writer);
}

void TreeModel::writeBoolLinkItems(QXmlStreamWriter& writer)
{
    writer.writeStartElement(BOOLLINK_ITEMS);

    writer.writeStartElement(BOOLLINKS);
    for(auto const& [key,val] : m_bool_links){
        writer.writeStartElement(BOOLLINK_NODE);
        const auto& item = getItem(key);
        writer.writeAttribute("LINKKEY",item.pathkey(false).join(XML_SEPERATOR));
        writer.writeAttribute("CONNECTEDKEY",val.first.join(XML_SEPERATOR));
        writer.writeAttribute("ROW",QString::number(val.second));
        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeStartElement(BOOLHIDDEN);
    for(auto const& [key,val] : m_bool_links_map){
        writer.writeStartElement(BOOLHIDDEN_NODE);
        writer.writeAttribute("KEY",key.join(XML_SEPERATOR));
        writer.writeAttribute("ROW",QString::number(val.second));
        writeTreeItem(writer,val.first.get());
        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeEndElement();
}

void TreeModel::writeComboLinkItems(QXmlStreamWriter& writer)
{
    writer.writeStartElement(COMBOLINK_ITEMS);

    writer.writeStartElement(COMBOLINKS);
    for(auto const& [key,val] : m_combo_links){
        writer.writeStartElement(COMBOLINK_NODE);
        const auto& item = getItem(key);
        writer.writeAttribute("LINKKEY",item.pathkey(false).join(XML_SEPERATOR));
        writer.writeAttribute("CONNECTEDKEY",std::get<0>(val).join(XML_SEPERATOR));
        writer.writeAttribute("COMBOSTR",std::get<1>(val));
        writer.writeAttribute("ROW",QString::number(std::get<2>(val)));
        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeStartElement(COMBOHIDDEN);
    for(auto const& [key,val] : m_combo_links_map){
        writer.writeStartElement(COMBOHIDDEN_NODE);
        writer.writeAttribute("KEY",key.join(XML_SEPERATOR));
        writer.writeAttribute("ROW",QString::number(val.second));
        writeTreeItem(writer,val.first.get());
        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeEndElement();
}

bool TreeModel::load(const QString& filename)
{
    if(filename.isEmpty())
        return false;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    qDebug() << "CLEAR CURRENT TREE";
    clear();

    QXmlStreamReader reader(&file);
    //Check file type has TreeModel data in it (in the current version)
    reader.readNextStartElement();
    qDebug() << "CHECK FILE FORMAT";
    if(reader.name() != FILE_FORMAT)
        return false;
    if(reader.attributes().value("VERSION") != VERSION_NUM)
        return false;

    eatStreamCharacters(reader);
    qDebug() << "CHECK VISIBLE TREE";
    if(!reader.isStartElement() || reader.name() != VISIBLE_TREE)
        return false;
    eatStreamCharacters(reader);

    qDebug() << "READ VISIBLE TREE";
    m_root_item = readTreeItem(reader);
    // Check that reader is at end of the visible tree
    if(!reader.isEndElement() || reader.name()!=VISIBLE_TREE)
        return false;
    qDebug() << "END OF VISIBLE TREE";

    eatStreamCharacters(reader);
    readLinkItems(reader);

    qDebug() << "RESET MODEL";
    beginResetModel();
    endResetModel();
    return true;
}

void TreeModel::readLinkItems(QXmlStreamReader& reader)
{
    streamDiagnostic(reader);
    // Confirm that file is at beggining of saved LINK_ITEMS
    if(!reader.isStartElement() || reader.name() != LINK_ITEMS)
        return;

    eatStreamCharacters(reader);
    // Return if their are no LINK_ITEMS and nothing further needs to be done
    if(reader.isEndElement() && reader.name() == LINK_ITEMS)
        return;

    while(reader.isStartElement()){
        if(reader.name() == BOOLLINK_ITEMS)
            readBoolLinkItems(reader);
        else if(reader.name() == COMBOLINK_ITEMS)
            readComboLinkItems(reader);
        else
            break;
        eatStreamCharacters(reader);
    }
    if(!reader.isEndElement() || reader.name() != LINK_ITEMS)
        return;
}

void TreeModel::readBoolLinkItems(QXmlStreamReader& reader)
{
    // Confirm that reader is at beggining of BOOLLINK_ITEMS
    if(!reader.isStartElement() || reader.name() != BOOLLINK_ITEMS)
        return;
    eatStreamCharacters(reader);

    // Check for bool links
    if(!reader.isStartElement() || reader.name() != BOOLLINKS){
        while(eatStreamCharacters(reader) != QXmlStreamReader::Invalid){
            if(reader.isEndElement() && reader.name() == BOOLLINK_ITEMS)
                return;
        }
    }
    eatStreamCharacters(reader);
    while(reader.isStartElement() && reader.name() == BOOLLINK_NODE)
    {
        qDebug() << "Read bool link";
        QXmlStreamAttributes attributes(reader.attributes());
        QStringList linkkey = attributes.value("LINKKEY").toString().split(XML_SEPERATOR);
        QStringList connectkey = attributes.value("CONNECTEDKEY").toString().split(XML_SEPERATOR);
        int row = attributes.value("ROW").toInt();
        m_bool_links.insert(std::make_pair(linkkey,std::make_pair(connectkey,row)));

        eatStreamCharacters(reader);
        if(!reader.isEndElement() || reader.name() != BOOLLINK_NODE)
            return;
        eatStreamCharacters(reader);
    }

    if(!reader.isEndElement() || reader.name() != BOOLLINKS){
        qDebug() << "Failed to read BOOLLINKS correctly.";
        return;
    }
    eatStreamCharacters(reader);

    // Check for bool hidden 
    if(!reader.isStartElement() || reader.name() != BOOLHIDDEN){
        while(!reader.isEndElement() && reader.name() != BOOLLINK_ITEMS)
            eatStreamCharacters(reader);
        qDebug() << "No BOOLHIDDEN found.";
        return;
    }
    eatStreamCharacters(reader);
    while(reader.isStartElement() && reader.name() == BOOLHIDDEN_NODE)
    {
        qDebug() << "Read bool hidden";
        QXmlStreamAttributes attributes(reader.attributes());
        QStringList key = attributes.value("KEY").toString().split(XML_SEPERATOR);
        int row = attributes.value("ROW").toInt();
        eatStreamCharacters(reader);
        std::unique_ptr<TreeItem> item = readTreeItem(reader);
        m_bool_links_map.insert(std::make_pair(key,std::make_pair(std::move(item),row)));
        if(!reader.isEndElement() || reader.name() != BOOLHIDDEN_NODE)
            return;
        eatStreamCharacters(reader);
    }

    if(!reader.isEndElement() || reader.name() != BOOLHIDDEN){
        qDebug() << "Failed to read BOOLHIDDEN correctly.";
        return;
    }
    eatStreamCharacters(reader);
    // Confirm that reader is at end of BOOLLINK_ITEMS
    if(!reader.isEndElement() || reader.name() != BOOLLINK_ITEMS){
        qDebug() << "Failed to read BOOLLINKS_ITEMS correctly.";
    }
}

void TreeModel::readComboLinkItems(QXmlStreamReader& reader)
{
    // Confirm that reader is at beggining of COMBOLINK_ITEMS
    if(!reader.isStartElement() || reader.name() != COMBOLINK_ITEMS)
        return;
    eatStreamCharacters(reader);

    // Check for combo links
    if(!reader.isStartElement() || reader.name() != COMBOLINKS){
        while(eatStreamCharacters(reader) != QXmlStreamReader::Invalid){
            if(reader.isEndElement() && reader.name() == COMBOLINK_ITEMS)
                return;
        }
    }
    eatStreamCharacters(reader);
    while(reader.isStartElement() && reader.name() == COMBOLINK_NODE)
    {
        qDebug() << "Read combo link";
        QXmlStreamAttributes attributes(reader.attributes());
        QStringList linkkey = attributes.value("LINKKEY").toString().split(XML_SEPERATOR);
        QStringList connectkey = attributes.value("CONNECTEDKEY").toString().split(XML_SEPERATOR);
        QString combo_str = attributes.value("COMBOSTR").toString();
        int row = attributes.value("ROW").toInt();
        m_combo_links.insert(std::make_pair(linkkey,std::make_tuple(connectkey,combo_str,row)));

        eatStreamCharacters(reader);
        if(!reader.isEndElement() || reader.name() != COMBOLINK_NODE)
            return;
        eatStreamCharacters(reader);
    }

    if(!reader.isEndElement() || reader.name() != COMBOLINKS){
        qDebug() << "Failed to read COMBOLINKS correctly.";
        return;
    }
    eatStreamCharacters(reader);

    // Check for bool hidden 
    if(!reader.isStartElement() || reader.name() != COMBOHIDDEN){
        while(!reader.isEndElement() && reader.name() != COMBOLINK_ITEMS)
            eatStreamCharacters(reader);
        qDebug() << "No COMBOHIDDEN found.";
        return;
    }

    eatStreamCharacters(reader);
    while(reader.isStartElement() && reader.name() == COMBOHIDDEN_NODE)
    {
        qDebug() << "Read combo hidden";

        QXmlStreamAttributes attributes(reader.attributes());
        QStringList key = attributes.value("KEY").toString().split(XML_SEPERATOR);
        int row = attributes.value("ROW").toInt();
        eatStreamCharacters(reader);
        std::unique_ptr<TreeItem> item = readTreeItem(reader);
        m_combo_links_map.insert(std::make_pair(key,std::make_pair(std::move(item),row)));

        if(!reader.isEndElement() || reader.name() != COMBOHIDDEN_NODE){
            qDebug() << "Reader not at end element of combohidden_node!";
            return;
        }
        eatStreamCharacters(reader);
    }

    if(!reader.isEndElement() || reader.name() != COMBOHIDDEN){
        qDebug() << "Failed to read COMBOHIDDEN correctly.";
        return;
    }
    eatStreamCharacters(reader);

    // Confirm that reader is at end of COMBOLINK_ITEMS
    if(!reader.isEndElement() || reader.name() != COMBOLINK_ITEMS){
        qDebug() << "Failed to read COMBOLINK_ITEMS correctly.";
    }
}


std::unique_ptr<TreeItem> TreeModel::readTreeItem(QXmlStreamReader& reader)
{ 
    // Check that reader is at the start element for a tree node
    if(!reader.isStartElement() || reader.name() != QString("TREENODE"))
        return nullptr;

    QXmlStreamAttributes attributes(reader.attributes());
    QString name = attributes.value("NAME").toString();
    QByteArray typestr(attributes.value("TYPENAME").toString().toUtf8());
    QString dtypestr = attributes.value("NODETYPE").toString();
    QString valuestr = attributes.value("VALUE").toString();
    QMetaType meta_type = QMetaType::fromName(typestr);
    QVariant value(valuestr);
    value.convert(meta_type);
    TreeItem::DataType dtype(static_cast<TreeItem::DataType>(dtypestr.toInt()));
    std::unique_ptr<TreeItem> item  = std::make_unique<TreeItem>(name,value,dtype);
    QXmlStreamReader::TokenType ttype = eatStreamCharacters(reader);
    if(ttype == QXmlStreamReader::EndElement && reader.name() == QString("TREENODE")){
        eatStreamCharacters(reader);
        return item;
    }
    if(ttype == QXmlStreamReader::StartElement && (reader.name() == QString("AUXMAP"))){
        readAuxMap(reader,item.get());
        // Reader at end of TREENODE
        eatStreamCharacters(reader);
        // streamDiagnostic(reader);
        return item;
    }
    while(reader.tokenType() == QXmlStreamReader::StartElement){
        std::unique_ptr<TreeItem> child = readTreeItem(reader);
        child->m_parent = item.get();
        item->m_child_items.push_back(std::move(child));
    } 
    eatStreamCharacters(reader);
    return item;
}

QXmlStreamReader::TokenType eatStreamCharacters(QXmlStreamReader& reader)
{
    while(reader.readNext()){
        if(reader.tokenType() == QXmlStreamReader::StartElement ||
                reader.tokenType() == QXmlStreamReader::EndElement ||
                reader.tokenType() == QXmlStreamReader::Invalid)
            return reader.tokenType();
    }
    return reader.tokenType();
}


void TreeModel::clear()
{
    beginResetModel();
    m_root_item.reset();
    m_root_item = std::make_unique<TreeItem>(ROOT_NAME);
    m_bool_links.clear();
    m_bool_links_map.clear();
    m_combo_links.clear();
    m_combo_links_map.clear();
    endResetModel();
}

}






