#ifndef PARAMCOMBOBOX_H
#define PARAMCOMBOBOX_H

#include "paramtreemodel.h"
#include "paramtreeitem.h"
#include <QComboBox>
#include <QModelIndex>
#include <QString>

class ParamComboBox : public QComboBox
{
    Q_OBJECT

public:
    ParamComboBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    ParamTreeModel* m_model;
    QModelIndex m_index;
};

#endif // PARAMCOMBOBOX_H
