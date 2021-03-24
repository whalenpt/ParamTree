#include "paramtreemodel.h"
#include "paramtreeitem.h"
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include <QByteArray>

const QString NAME_HEADER("Param");
const QString VALUE_HEADER("Value");
const QString ROOT_NAME("PARAMTREEMODEL");

const QChar XML_SEPERATOR('|');
const QString FILE_FORMAT("PARAMTREE");
const QString VERSION_NUM("1.1");

ParamTreeModel::ParamTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    root_item = new TreeItem(ROOT_NAME);
}

ParamTreeModel::~ParamTreeModel()
{
    delete root_item;
}

TreeItem* ParamTreeModel::itemForIndex(const QModelIndex& index) const
{
    if(index.isValid()){
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if(item)
            return item;
    }
    return root_item;
}

QVariant ParamTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if(section == 0)
            return NAME_HEADER;
        else if(section == 1)
            return VALUE_HEADER;
    }
    return QVariant();
}

QModelIndex ParamTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();
    TreeItem* parent_item = itemForIndex(parent);
    TreeItem* child_item = parent_item->child(row);
    if(child_item)
        return createIndex(row,column,child_item);
    return QModelIndex();
}

QModelIndex ParamTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    TreeItem* child_item = itemForIndex(index);
    TreeItem* parent_item = child_item->parent();
    if(parent_item == root_item)
        return QModelIndex();
    return createIndex(parent_item->childNumber(),0,parent_item);
}

int ParamTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem* item = itemForIndex(parent);
    return item->childCount();
}

int ParamTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}

QVariant ParamTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem* item = itemForIndex(index);
    switch(role){
        case Qt::DisplayRole:
        case Qt::EditRole: {
            int col = index.column();
            if(col == 0){
                return item->name();
            }
            else if(col == 1){
                return item->value();
            }
            break;
        }
        case Role::KEY: {
           // first item in pathkey is the root name, this is not part of the QStringList key
           return QVariant(item->pathkey().mid(1));
        }
        case Role::STRINGKEY: {
            return item->pathkey().mid(1).join("/");
        }
        case Role::DATATYPE: {
            return static_cast<int>(item->dtype());
        }
        case Role::RANGE: {
            return item->aux("RANGE");
        }
        case Role::AUXMAP: {
            return item->auxMap();
        }
    }
    return QVariant();
}


bool ParamTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch(role){
        case Qt::EditRole:
        case Qt::DisplayRole:
        {
            if (data(index, role) != value) {
                TreeItem* item = itemForIndex(index);
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
            break;
        }

        case Role::RANGE: {
            TreeItem* item = itemForIndex(index);
            item->setAux("RANGE",value);
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags ParamTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if(index.column() == 1 && itemForIndex(index)->isLeaf())
         return Qt::ItemIsEditable | QAbstractItemModel::flags(index);

     return QAbstractItemModel::flags(index);
}

bool ParamTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    TreeItem* parent_item = itemForIndex(parent);
    bool success;

    beginInsertRows(parent, row, row + count - 1);
    success = parent_item->insertChildren(row,count);
    endInsertRows();

    return success;
}

bool ParamTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parent_item = itemForIndex(parent);
    bool success = true;
    beginRemoveRows(parent,row,row+count - 1);
    success = parent_item->removeChildren(row,count);
    endRemoveRows();
    return success;
}



QModelIndex ParamTreeModel::addItem(const TreeItem& item,const QModelIndex& parent_index)
{
    TreeItem* parent = itemForIndex(parent_index);
    int position = parent->childCount();
    beginInsertRows(parent_index,position,position);
    parent->insertItem(item,position);
    endInsertRows();
    return index(position,0,parent_index);
}

QModelIndex ParamTreeModel::getIndex(const QString& name,const QModelIndex& parent) const
{
    return indexForPath(parent,QStringList() << name);
}

const TreeItem& ParamTreeModel::getItem(const QString& name,const QModelIndex& parent) const
{
    QModelIndex index = indexForPath(parent,QStringList() << name);
    return *itemForIndex(index);
}

const TreeItem& ParamTreeModel::getItem(const QStringList& key) const
{
    QModelIndex index = indexForPath(QModelIndex(),key);
    return *itemForIndex(index);
}

bool ParamTreeModel::hasItem(const QStringList& key) const
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

bool ParamTreeModel::contains(const QStringList& key) const
{
    return hasItem(key);
}

const TreeItem& ParamTreeModel::getItem(const QModelIndex &index) const
{
    return *itemForIndex(index);
}

const TreeItem& ParamTreeModel::getRootItem() const
{
    return *root_item;
}

QModelIndex ParamTreeModel::getValIndex(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    if(index.column() == 1)
        return index;

    return ParamTreeModel::index(index.row(),1,index.parent());
}


QModelIndex ParamTreeModel::getNameIndex(const QModelIndex& index) const
{
    if(!index.isValid())
         return QModelIndex();

    if(index.column() == 0)
         return index;

    return ParamTreeModel::index(index.row(),0,index.parent());
}



QModelIndex ParamTreeModel::getIndex(const TreeItem& item) const
{
    QStringList pathkey = item.pathkey();

    // Check that item has a root consistent with the model
    if(pathkey.at(0) !=  ROOT_NAME)
        return QModelIndex();

    return indexForPath(QModelIndex(), pathkey.mid(1));
}

QModelIndex ParamTreeModel::getIndex(const QStringList& key) const
{
    return indexForPath(QModelIndex(),key);
}

QModelIndex ParamTreeModel::getValIndex(const TreeItem& item) const
{
    return getValIndex(getIndex(item));
}

QModelIndex ParamTreeModel::getValIndex(const QStringList& key) const
{
    return getValIndex(getIndex(key));
}

// Settings warning: QVariant type not necessarily saved properly (int->qlonglong)
void ParamTreeModel::saveToSettings(QSettings& settings, const TreeItem& item) const
{
    if(item.isLeaf()){
        QModelIndex index = getIndex(item);
        if(!index.isValid())
            return;
        QString settings_key = data(index,Role::STRINGKEY).toString();
        settings.setValue(settings_key,item.value());
    } else{
        for(auto child : item.child_items)
            saveToSettings(settings,*child);
    }
}

void ParamTreeModel::saveToSettings(QSettings& settings,
                                const QModelIndex& index) const
{
    if(!index.isValid())
        return;
    saveToSettings(settings,*itemForIndex(index));
}

QVariant ParamTreeModel::readFromSettings(QSettings& settings,const QStringList& key)
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
QModelIndex ParamTreeModel::indexForPath(const QModelIndex &parent,
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

QStringList ParamTreeModel::getKey(const QModelIndex& index) const
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

bool ParamTreeModel::save(const QString& filepath)
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

    writeTreeItem(writer,root_item);
    writer.writeEndElement();
    writer.writeEndDocument();
    return true;
}

void ParamTreeModel::writeTreeItem(QXmlStreamWriter& writer,const TreeItem* item)
{
    if(item != root_item) {
        writer.writeStartElement("TREENODE");
        writeNode(writer,*item);
    }
    for(auto child : item->child_items){
        writeTreeItem(writer,child);
    }
    if(item != root_item)
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


bool ParamTreeModel::load(const QString& filename)
{
    if(filename.isEmpty())
        return false;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    clear();

    QXmlStreamReader reader(&file);
    //Check file type has ParamTreeModel data in it (in the current version)
    reader.readNextStartElement();
    if(reader.name() != FILE_FORMAT)
        return false;
    if(reader.attributes().value("VERSION") != VERSION_NUM)
        return false;

    readTreeItems(reader,root_item);
    beginResetModel();
    endResetModel();

    return true;
}


void ParamTreeModel::readTreeItems(QXmlStreamReader& reader,TreeItem* item)
{
    while(!reader.atEnd()){
        reader.readNext();
        if(reader.isStartElement()){
            TreeItem* child = readNode(reader);
            child->m_parent = item;
            item->child_items.push_back(child);
            item = child;
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

void ParamTreeModel::clear()
{
    delete root_item;
    root_item = new TreeItem(ROOT_NAME);
    beginResetModel();
    endResetModel();
}






