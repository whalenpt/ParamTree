#ifndef TREESPINBOX_H
#define TREESPINBOX_H

#include <QObject>
#include <QSpinBox>
#include <QModelIndex>

namespace paramtree{

class TreeModel;
class TreeItem;

class TreeSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    TreeSpinBox(TreeModel* model,const TreeItem& item,QWidget* parent=nullptr);
    QString name() const;

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    TreeModel* m_model;
    QModelIndex m_index;
};

}

#endif // PARAMSPINBOX_H
