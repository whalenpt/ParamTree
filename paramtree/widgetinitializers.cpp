#include "widgetinitializers.h"
#include "paramtreemodel.h"
#include <QVariant>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QModelIndex>
#include <QMap>
#include <QList>
#include <QDebug>
#include <algorithm>
#include <limits>

namespace ParamWidgetInitializer{

    void initializeSpinBox(QSpinBox* widget,const QModelIndex& index)
    {
        int start_val = index.data(Qt::DisplayRole).toInt();
        const AuxMap& aux_map = index.data(ParamTreeModel::Role::AUXMAP).toMap();
        if(aux_map.contains("RANGE")){
            QVariant range = aux_map.value("RANGE");
            if(range.canConvert<QVector<QVariant>>()){
                QVector<QVariant> v = range.value<QVector<QVariant>>();
                widget->setMinimum(v[0].toInt());
                widget->setMaximum(v[1].toInt());
            } else{
                widget->setMinimum(0);
                widget->setMaximum(std::max(10*start_val,100));
            }
        }
        if(aux_map.contains("STEP SIZE"))
            widget->setSingleStep(aux_map.value("STEP SIZE").toInt());
        widget->setValue(start_val);
    }

    void initializeSpinBox(QDoubleSpinBox* widget,const QModelIndex& index)
    {
        double start_val = index.data(Qt::DisplayRole).toDouble();
        const AuxMap& aux_map = index.data(ParamTreeModel::Role::AUXMAP).toMap();
        if(aux_map.contains("RANGE")){
            QVariant range = aux_map.value("RANGE");
            if(range.canConvert<QVector<QVariant>>()){
                QVector<QVariant> v = range.value<QVector<QVariant>>();
                widget->setMinimum(v[0].toDouble());
                widget->setMaximum(v[1].toDouble());
            } else{
                widget->setMinimum(0.0);
                widget->setMaximum(std::max(10.0*start_val,100.0));
            }
        }
        if(aux_map.contains("STEP SIZE"))
            widget->setSingleStep(aux_map.value("STEP SIZE").toDouble());
        widget->setValue(start_val);
    }


    //void initializeWidget(ScientificLineEdit* widget,const QModelIndex& index)
    //{
    //    double start_val = index.data(Qt::DisplayRole).toDouble();
    //    widget->setValue(start_val);
    //}
}
