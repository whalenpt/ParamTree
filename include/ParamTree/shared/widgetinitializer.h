#ifndef WIDGETINITIALIZER_H
#define WIDGETINITIALIZER_H

class QSpinBox;
class QModelIndex;
class QDoubleSpinBox;
//class ScientificLineEdit;

namespace paramtree{

namespace widgetinitializer{
    void initializeSpinBox(QSpinBox* widget,const QModelIndex& index);
    void initializeSpinBox(QDoubleSpinBox* widget,const QModelIndex& index);
//    void initializeWidget(ScientificLineEdit* widget,const QModelIndex& index);
};

}

#endif // WIDGETINITIALIZERS_H
