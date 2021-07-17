#ifndef PARAMSPINBOX_H
#define PARAMSPINBOX_H


#include <QObject>
#include <QSpinBox>
#include <QModelIndex>

class ParamTreeModel;
class TreeItem;

class ParamSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    ParamSpinBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    ParamTreeModel* m_model;
    QModelIndex m_index;
};

#endif // PARAMSPINBOX_H
