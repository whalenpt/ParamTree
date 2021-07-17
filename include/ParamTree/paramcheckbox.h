#ifndef PARAMCHECKBOX_H
#define PARAMCHECKBOX_H

#include <QCheckBox>
#include <QModelIndex>
class ParamTreeModel;
class TreeItem;
class QString;

class ParamCheckBox : public QCheckBox
{
public:
    ParamCheckBox(ParamTreeModel* model,const TreeItem& item,QWidget* parent = nullptr);
    QString name() const;

public slots:
    void updateModel(int val);

private slots:
    void setEditorData(const QModelIndex& topLeft,const QModelIndex& bottomRight);

private:
    QString m_name;
    ParamTreeModel* m_model;
    QModelIndex m_index;
};

#endif // PARAMCHECKBOX_H
