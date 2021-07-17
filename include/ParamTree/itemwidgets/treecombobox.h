#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H

#include "modelview/treemodel.h"
#include "modelview/treeitem.h"
#include <QComboBox>
#include <QModelIndex>
#include <QString>

namespace paramtree{

class TreeComboBox : public QComboBox
{
    Q_OBJECT

public:
    TreeComboBox(TreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    TreeModel* m_model;
    QModelIndex m_index;
};

}

#endif // COMBOBOX_H
