#include "paramlineedit.h"
#include "paramtreemodel.h"
#include <QAbstractItemModel>

ParamLineEdit::ParamLineEdit(ParamTreeModel* model,const TreeItem& item,QWidget* parent) :
    QLineEdit(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    QLineEdit::setText(m_model->data(m_index,Qt::DisplayRole).toString());
    connect(this,&QLineEdit::textChanged,[this] (const QString& text) {
        m_model->setData(m_index,text,Qt::EditRole);
    });

    connect(m_model,&QAbstractItemModel::dataChanged,this,&ParamLineEdit::setEditorData);
}

void ParamLineEdit::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        QString text = m_index.data(Qt::DisplayRole).toString();
        QLineEdit::setText(text);
    }
}

QString ParamLineEdit::name() const
{
    return m_name;
}

QString ParamLineEdit::value() const
{
    return QLineEdit::text();
}

