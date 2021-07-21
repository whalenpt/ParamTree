#include "modelview/itemdelegate.h"
#include "modelview/treeitem.h"
#include "shared/scientificlineedit.h"
#include "shared/widgetinitializer.h"
#include <stdexcept>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QList>
#include <QApplication>
#include <QDebug>

namespace paramtree{

ItemDelegate::ItemDelegate(QWidget* parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    const TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    if(item->dtype() == TreeItem::DataType::COMBO){
        QStringList range;
        if(item->hasAux("RANGE")){
            QVariant rangevar = item->aux("RANGE");
            if(!rangevar.canConvert<QStringList>())
                throw std::runtime_error("ItemDelegate::createEditor error.");
            range = rangevar.toStringList();
        }
        m_combo_box = new QComboBox(parent);
        if(!range.isEmpty())
            m_combo_box->addItems(range);
        m_combo_box->setStyleSheet("border: none");
        connect(m_combo_box,SIGNAL(currentIndexChanged(int)),this,SLOT(setComboData(int)));
        return m_combo_box;
    } else if(item->dtype() == TreeItem::DataType::SCIENTIFIC){
        ScientificLineEdit* editor = new ScientificLineEdit(parent);
        return editor;
    }
    else if(item->dtype() == TreeItem::DataType::VAR){
        QMetaType var_type = item->value().metaType();
        if(var_type.id() == QMetaType::Int){
            QSpinBox* editor = new QSpinBox(parent);
            widgetinitializer::initializeSpinBox(editor,index);
            return editor;
        } else if(var_type.id() == QMetaType::QString){
            return new QLineEdit(parent);
        } else if(var_type.id() == QMetaType::Double) {
            QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
            widgetinitializer::initializeSpinBox(editor,index);
            return editor;
        } else {
            return QStyledItemDelegate::createEditor(parent,option,index);
        }
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
           const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
}

void ItemDelegate::setEditorData(QWidget* editor,const QModelIndex& index) const
{
    const TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    if(item->dtype() == TreeItem::DataType::COMBO){
        QString text = index.data(Qt::DisplayRole).toString();
        (static_cast<QComboBox*>(editor))->setCurrentText(text);
        return;
    } else if(item->dtype() == TreeItem::DataType::SCIENTIFIC){
        double val = index.data(Qt::DisplayRole).toDouble();
        (static_cast<ScientificLineEdit*>(editor))->setValue(val);
        return;
    } else if(item->dtype() == TreeItem::DataType::VAR){
        QMetaType var_type = item->value().metaType();
        if(var_type.id() == QMetaType::Int){
            int val = index.data(Qt::DisplayRole).toInt();
            (static_cast<QSpinBox*>(editor))->setValue(val);
            return;
        } else if(var_type.id() == QMetaType::QString){
            QString val = index.data(Qt::DisplayRole).toString();
            (static_cast<QLineEdit*>(editor))->setText(val);
            return;
        } else if(var_type.id() == QMetaType::Double) {
            double val = index.data(Qt::DisplayRole).toDouble();
            (static_cast<QDoubleSpinBox*>(editor))->setValue(val);
            return;
        } else {
            return QStyledItemDelegate::setEditorData(editor,index);
        }
    }
    QStyledItemDelegate::setEditorData(editor,index);
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    if(item->dtype() == TreeItem::DataType::COMBO){
        model->setData(index,static_cast<QComboBox*>(editor)->currentText(),Qt::EditRole);
        return;
    } else if(item->dtype() == TreeItem::DataType::SCIENTIFIC) {
        model->setData(index,static_cast<ScientificLineEdit*>(editor)->value(),Qt::EditRole);
        return;
    }
    else if(item->dtype() == TreeItem::DataType::VAR){
        QMetaType var_type = item->value().metaType();
        if(var_type.id() == QMetaType::Int){
            model->setData(index,static_cast<QSpinBox*>(editor)->value(),Qt::EditRole);
        } else if(var_type.id() == QMetaType::QString){
            model->setData(index,static_cast<QLineEdit*>(editor)->text(),Qt::EditRole);
        } else if(var_type.id() == QMetaType::Double) {
            model->setData(index,static_cast<QDoubleSpinBox*>(editor)->value(),Qt::EditRole);
        } else {
            QStyledItemDelegate::setModelData(editor,model,index);
        }
        return;
    }
    QStyledItemDelegate::setModelData(editor,model,index);
}

void ItemDelegate::updateEditorGeometry(QWidget* editor,const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor,option,index);
}

void ItemDelegate::setComboData(int /*val*/)
{
    emit commitData(m_combo_box);
}



}













