#ifndef PARAMTREEITEM_H
#define PARAMTREEITEM_H

#include "ParamTree/ParamTree_global.h"
#include <exception>
#include <QVariant>
#include <QMap>
#include <QStringList>
#include <QModelIndex>



namespace paramtree{

class TreeKeyException : public std::invalid_argument
{
public:
    explicit TreeKeyException(const QString& key) : std::invalid_argument(\
            QString("The key " + key + " was not found in the TreeItem.").toStdString()) {}
};

using AuxMap = QMap<QString,QVariant>;

class PARAMTREE_EXPORT TreeItem
{
public:
    enum class DataType {VAR,BRANCH,COMBO,SCIENTIFIC,BOOL};
    explicit TreeItem(const QString& name = "",const QVariant& val = QVariant(),
                      DataType dt = DataType::VAR,
                      const AuxMap& aux_map = AuxMap());
    explicit TreeItem(const TreeItem& item);
    ~TreeItem();
    friend class TreeModel;

    unsigned int childCount() const;
    unsigned int childNumber() const;

    QString name() const;
    QVariant value() const;
    DataType dtype() const;
    const AuxMap& auxMap() const;

    bool hasAux(const QString& aux_key) const;
    QVariant aux(const QString& aux_name) const;

    bool isRoot() const;
    bool isLeaf() const;
    bool isBranch() const;
    void setName(const QString& name);
    void setValue(const QVariant& value);
    void setAux(const QString& aux_name,const QVariant& aux_val);
    void setAux(const QString& aux_name,const QVector<QVariant>& aux_val);

    void addItem(const TreeItem& item);
    bool insertItem(const TreeItem& item,unsigned int position);

    const TreeItem& getItem(const QString& name) const;
    const TreeItem& getItem(unsigned int position) const;
    const std::vector<TreeItem*> getLeafs() const;
    const std::vector<TreeItem*> getBranches() const;

private:
    TreeItem* parent();
    TreeItem* child(unsigned int number);

    void addChild(TreeItem* item);
    bool insertChild(TreeItem* item,unsigned int position);
    bool insertChildren(unsigned int position,unsigned int count = 1);
    bool removeChildren(unsigned int position, unsigned int count = 1);
    QStringList pathkey() const;

    QString m_name;
    QVariant m_val;
    DataType m_dtype;
    AuxMap m_aux_map;
    std::vector<TreeItem*> child_items;
    TreeItem* m_parent;
};


}

#endif // TREEITEM_H
