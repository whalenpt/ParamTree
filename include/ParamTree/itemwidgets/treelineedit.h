#ifndef PARAMLINEEDIT_H
#define PARAMLINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QModelIndex>

class ParamTreeModel;
class TreeItem;

class ParamLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    ParamLineEdit(ParamTreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;
    QString value() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    ParamTreeModel* m_model;
    QModelIndex m_index;
};

#endif // PARAMLINEEDIT_H
