

#include "itemwidgets/treescilineedit.h"
#include "shared/scientificlineedit.h"
#include <QDoubleValidator>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QAbstractItemModel>
#include <string>
#include <sstream>
#include <iomanip>

namespace paramtree{

TreeSciLineEdit::TreeSciLineEdit(TreeModel* model,const TreeItem& item,QWidget* parent) :
    ScientificLineEdit(parent),
    m_name(item.name()),
    m_model(model),
    m_index(model->getValIndex(item))
{
    ScientificLineEdit::setValue(m_model->data(m_index,Qt::DisplayRole).toDouble());
    connect(this,&QLineEdit::editingFinished, [this] {
       m_model->setData(m_index,ScientificLineEdit::value(),Qt::EditRole);
    });
    connect(m_model,&QAbstractItemModel::dataChanged,this,&TreeSciLineEdit::setEditorData);
}

void TreeSciLineEdit::setEditorData(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    if(m_index == topLeft){
        double value = m_index.data(Qt::DisplayRole).toDouble();
        ScientificLineEdit::setValue(value);
    }
}

}


