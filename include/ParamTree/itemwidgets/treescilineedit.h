#ifndef TREESCILINEEDIT_H
#define TREESCILINEEDIT_H

#include <QLineEdit>
#include <QString>
#include <QModelIndex>
#include "modelview/treemodel.h"
#include "modelview/treeitem.h"
#include "shared/scientificlineedit.h"

class QWidget;

namespace paramtree{

class TreeSciLineEdit : public ScientificLineEdit
{
    Q_OBJECT

public:

    TreeSciLineEdit(TreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    TreeModel* m_model;
    QModelIndex m_index;
};

}




#endif // SCIENTIFICLINEEDIT_H
