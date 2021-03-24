#ifndef WIDGETINITIALIZERS_H
#define WIDGETINITIALIZERS_H

class QSpinBox;
class QModelIndex;
class QDoubleSpinBox;
//class ScientificLineEdit;

namespace ParamWidgetInitializer{
    void initializeSpinBox(QSpinBox* widget,const QModelIndex& index);
    void initializeSpinBox(QDoubleSpinBox* widget,const QModelIndex& index);
//    void initializeWidget(ScientificLineEdit* widget,const QModelIndex& index);
};


#endif // WIDGETINITIALIZERS_H
