#ifndef SCIENTIFICLINEEDIT_H
#define SCIENTIFICLINEEDIT_H

#include <QLineEdit>
#include <QString>

class ScientificLineEdit : public QLineEdit
{
public:
    ScientificLineEdit(QWidget* parent = nullptr);
    void setValue(double value);
    double value() const;
};

QString formatNumericString(const QString& str);

#endif // SCIENTIFICLINEEDIT_H
