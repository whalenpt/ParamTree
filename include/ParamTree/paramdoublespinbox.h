#ifndef PARAMDOUBLESPINBOX_H
#define PARAMDOUBLESPINBOX_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QModelIndex>

class ParamTreeModel;
class TreeItem;

class ParamDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    ParamDoubleSpinBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    ParamTreeModel* m_model;
    QModelIndex m_index;
};

#endif // PARAMDOUBLESPINBOX_H
