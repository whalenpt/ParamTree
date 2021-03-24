#include "paramitemdelegate.h"
#include "paramtreeitem.h"
#include "scientificlineedit.h"
#include "widgetinitializers.h"
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QList>
#include <QApplication>
#include <QDebug>

ParamItemDelegate::ParamItemDelegate(QWidget* parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* ParamItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    const TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    if(item->dtype() == TreeItem::DataType::COMBO){
        QVariant range = item->aux("RANGE");
        if(!range.canConvert<QStringList>())
            throw std::runtime_error("ParamItemDelegate::createEditor error.");
        combo_box = new QComboBox(parent);
        combo_box->addItems(range.toStringList());
        combo_box->setStyleSheet("border: none");
        connect(combo_box,SIGNAL(currentIndexChanged(int)),this,SLOT(setComboData(int)));
        return combo_box;
    } else if(item->dtype() == TreeItem::DataType::BOOL){
        m_bool_box = new QComboBox(parent);
        m_bool_box->addItems(QStringList() << "True" << "False");
        m_bool_box->setStyleSheet("border: none");
        connect(m_bool_box,SIGNAL(currentIndexChanged(int)),this,SLOT(setBoolData(int)));
        return m_bool_box;
    }
    else if(item->dtype() == TreeItem::DataType::SCIENTIFIC){
        ScientificLineEdit* editor = new ScientificLineEdit(parent);
        return editor;
    }
    else if(item->dtype() == TreeItem::DataType::VAR){
        QMetaType var_type = item->value().metaType();
        if(var_type.id() == QMetaType::Int){
            QSpinBox* editor = new QSpinBox(parent);
            ParamWidgetInitializer::initializeSpinBox(editor,index);
            return editor;
        } else if(var_type.id() == QMetaType::QString){
            return new QLineEdit(parent);
        } else if(var_type.id() == QMetaType::Double) {
            QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
            ParamWidgetInitializer::initializeSpinBox(editor,index);
            return editor;
        } else {
            return QStyledItemDelegate::createEditor(parent,option,index);
        }
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

void ParamItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
           const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
}

void ParamItemDelegate::setEditorData(QWidget* editor,const QModelIndex& index) const
{
    const TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    if(item->dtype() == TreeItem::DataType::COMBO){
        QString text = index.data(Qt::DisplayRole).toString();
        (static_cast<QComboBox*>(editor))->setCurrentText(text);
        return;
    } else if(item->dtype() == TreeItem::DataType::BOOL){
        static_cast<QComboBox*>(editor)->setCurrentText(\
                    index.data(Qt::DisplayRole).toBool() ? "true" : "false");
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

void ParamItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    if(item->dtype() == TreeItem::DataType::COMBO){
        model->setData(index,static_cast<QComboBox*>(editor)->currentText(),Qt::EditRole);
        return;
    } else if(item->dtype() == TreeItem::DataType::BOOL){
        QString val(static_cast<QComboBox*>(editor)->currentText());
        QVariant var(val == "true" ? QVariant(true) : QVariant(false));
        model->setData(index,var,Qt::EditRole);
    }
    else if(item->dtype() == TreeItem::DataType::SCIENTIFIC) {
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

void ParamItemDelegate::updateEditorGeometry(QWidget* editor,const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor,option,index);
}

void ParamItemDelegate::setComboData(int /*val*/)
{
    emit commitData(combo_box);
}

void ParamItemDelegate::setBoolData(int /*val*/)
{
    emit commitData(m_bool_box);
}













