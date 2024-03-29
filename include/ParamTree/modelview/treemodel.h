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
#include <utility>
#include <tuple>

namespace paramtree{

//const QString ROOT_NAME = "Tree";
void writeNode(QXmlStreamWriter& writer,const TreeItem& item);
void writeAuxMap(QXmlStreamWriter& writer,const QMap<QString,QVariant>& map);
void readAuxMap(QXmlStreamReader& reader,TreeItem* item);
void streamDiagnostic(QXmlStreamReader& reader);
void streamHiddenDiagnostic(QXmlStreamReader& reader);
QXmlStreamReader::TokenType eatStreamCharacters(QXmlStreamReader& reader);

class PARAMTREE_EXPORT TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Role {KEY = Qt::UserRole+1,STRINGKEY,RANGE,DATATYPE,AUXMAP,MIN,MAX};
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
//    QModelIndex addItem(TreeItem* item,const QModelIndex& parent = QModelIndex());
    QModelIndex addItem(std::unique_ptr<TreeItem> item,const QModelIndex& parent = QModelIndex());
    QModelIndex addItem(const TreeItem& item,const QModelIndex& parent = QModelIndex());
    QModelIndex insertItem(std::unique_ptr<TreeItem> item,unsigned int position,const QModelIndex& parent = QModelIndex());
    QModelIndex insertItem(const TreeItem& item,unsigned int position,const QModelIndex& parent = QModelIndex());

    const TreeItem& getItem(const QString& key,const QModelIndex& parent = QModelIndex()) const;
    const TreeItem& getItem(const QStringList& key) const;
    const TreeItem& getItem(const QModelIndex& index) const;
    const TreeItem& getRootItem() const;
    bool hasItem(const QStringList& key) const;
    bool hiddenItem(const QStringList& key) const;

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

    void boolLink(const QStringList& linkey,const QStringList& key,int position = -1);
    void comboLink(const QStringList& linkey,const QStringList& key,const QString& combo_str,int position=-1);

    bool save(const QString& filename);
    bool load(const QString& filename);
    void clear();
    void reset();

    void saveToSettings(QSettings& settings, const TreeItem& item) const;
    void saveToSettings(QSettings& settings,const QModelIndex& index) const;
    QVariant readFromSettings(QSettings& settings,const QStringList& key);

private slots:
    void linkUpdate(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/);

private:
    void boolLinkUpdate(const QModelIndex& index);
    void comboLinkUpdate(const QModelIndex& index);

    std::unique_ptr<TreeItem> m_root_item;
    QSettings* m_settings;

    std::multimap<QStringList,std::pair<QStringList,int>> m_bool_links;
    std::map<QStringList,std::pair<std::unique_ptr<TreeItem>,int>> m_bool_links_map;

    std::multimap<QStringList,std::tuple<QStringList,QString,int>> m_combo_links;
    std::map<QStringList,std::pair<std::unique_ptr<TreeItem>,int>> m_combo_links_map;

    TreeItem* itemForIndex(const QModelIndex& index) const;
    QModelIndex indexForPath(const QModelIndex& parent,const QStringList& path) const;

    void writeTreeItem(QXmlStreamWriter& writer,const TreeItem* item);
    std::unique_ptr<TreeItem> readTreeItem(QXmlStreamReader& reader);
    void writeLinkItems(QXmlStreamWriter& writer);
    void writeBoolLinkItems(QXmlStreamWriter& writer);
    void writeComboLinkItems(QXmlStreamWriter& writer);

    void readLinkItems(QXmlStreamReader& reader);
    void readBoolLinkItems(QXmlStreamReader& reader);
    void readComboLinkItems(QXmlStreamReader& reader);
};

}


#endif // TREEMODEL_H
