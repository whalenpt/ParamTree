#ifndef PARAMSCIENTIFICLINEEDIT_H
#define PARAMSCIENTIFICLINEEDIT_H


#include "scientificlineedit.h"
#include <QWidget>
#include <QModelIndex>

class ParamTreeModel;
class TreeItem;

class ParamScientificLineEdit : public ScientificLineEdit
{
    Q_OBJECT

public:

    ParamScientificLineEdit(ParamTreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);
//    void setModelData();

private:
    QString m_name;
    ParamTreeModel* m_model;
    QModelIndex m_index;
};


#endif // PARAMSCIENTIFICLINEEDIT_H
