#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "ParamTree/ParamTree_global.h"
#include "ParamTree/modelview/treeitem.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QSettings>
#include <memory>

namespace paramtree{

//const QString ROOT_NAME = "Tree";
void writeNode(QXmlStreamWriter& writer,const TreeItem& item);
void writeAuxMap(QXmlStreamWriter& writer,const QMap<QString,QVariant>& map);
TreeItem* readNode(QXmlStreamReader& reader);
void readAuxMap(QXmlStreamReader& reader,TreeItem* item);

class PARAMTREE_EXPORT TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Role {KEY = Qt::UserRole+1,STRINGKEY,RANGE,DATATYPE,AUXMAP};
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel() override {}

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // returns index with column = 0 corresponding to the name of the name/value parameter
    QModelIndex addItem(TreeItem* item,const QModelIndex& parent = QModelIndex());
    QModelIndex addItem(std::unique_ptr<TreeItem> item,const QModelIndex& parent = QModelIndex());

    const TreeItem& getItem(const QString& key,const QModelIndex& parent = QModelIndex()) const;
    const TreeItem& getItem(const QStringList& key) const;
    const TreeItem& getItem(const QModelIndex& index) const;
    const TreeItem& getRootItem() const;
    bool contains(const QStringList& key) const;
    bool hasItem(const QStringList& key) const;

    // Returns index
    QModelIndex getIndex(const TreeItem& item) const;
    QModelIndex getIndex(const QStringList& key) const;
    QModelIndex getIndex(const QString& key,const QModelIndex& parent = QModelIndex()) const;

    // Returns index with column = 1 corresponding to the value
    QModelIndex getValIndex(const TreeItem& item) const;
    QModelIndex getValIndex(const QStringList& key) const;

    // Returns index with the column set to 1
    QModelIndex getValIndex(const QModelIndex& index) const;
    QModelIndex getNameIndex(const QModelIndex& index) const;
    QStringList getKey(const QModelIndex& index) const;

    void boolLink(const QModelIndex& index1,const QModelIndex& index2);
    bool save(const QString& filename);
    bool load(const QString& filename);
    void clear();

    void saveToSettings(QSettings& settings, const TreeItem& item) const;
    void saveToSettings(QSettings& settings,const QModelIndex& index) const;
    QVariant readFromSettings(QSettings& settings,const QStringList& key);

private:
    std::unique_ptr<TreeItem> m_root_item;
    QSettings* m_settings;

    TreeItem* itemForIndex(const QModelIndex& index) const;
    QModelIndex indexForPath(const QModelIndex& parent,const QStringList& path) const;

    void writeTreeItem(QXmlStreamWriter& writer,const TreeItem* item);
    void readTreeItems(QXmlStreamReader& reader,TreeItem* item);
};

}


#endif // PARAMTREEMODEL_H
