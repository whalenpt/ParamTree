#ifndef TREELINEEDIT_H
#define TREELINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QModelIndex>


namespace paramtree{

class TreeModel;
class TreeItem;

class TreeLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    TreeLineEdit(TreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;
    QString value() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    TreeModel* m_model;
    QModelIndex m_index;
};

}

#endif // PARAMLINEEDIT_H
