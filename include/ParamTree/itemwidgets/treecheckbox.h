#ifndef TREECHECKBOX_H
#define TREECHECKBOX_H

#include <QCheckBox>
#include <QModelIndex>
class QString;

namespace paramtree{
class TreeModel;
class TreeItem;

class TreeCheckBox : public QCheckBox
{
public:
    TreeCheckBox(TreeModel* model,const TreeItem& item,QWidget* parent = nullptr);
    QString name() const;

public slots:
    void updateModel(int val);

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    TreeModel* m_model;
    QModelIndex m_index;
};

}

#endif // CHECKBOX_H
