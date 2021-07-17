#ifndef SCIENTIFICLINEEDIT_H
#define SCIENTIFICLINEEDIT_H

#include <QLineEdit>
#include <QString>
#include <QModelIndex>

class QWidget;

namespace paramtree{

QString formatNumericString(const QString& str);

class ScientificLineEdit : public QLineEdit
{
public:
    ScientificLineEdit(QWidget* parent = nullptr);
    void setValue(double value);
    double value() const;
};


}

#endif // SCIENTIFICLINEEDIT_H



