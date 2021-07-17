#include "scientificlineedit.h"
#include <QDoubleValidator>
#include <QDebug>
#include <string>
#include <sstream>
#include <iomanip>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

ScientificLineEdit::ScientificLineEdit(QWidget* parent) :
    QLineEdit(parent)
{
    QDoubleValidator* dv = new QDoubleValidator(this);
    dv->setNotation(QDoubleValidator::ScientificNotation);
    QLineEdit::setValidator(dv);
}

void ScientificLineEdit::setValue(double value)
{

    QString str_num(QString::number(value,'e',12));
    QLineEdit::setText(formatNumericString(str_num));
}

QString formatNumericString(const QString& str)
{
    QRegularExpression re("0*e");
    QString formated_str(str);
    formated_str.replace(re,QString("e"));
    if(formated_str.contains(".e",Qt::CaseInsensitive)){
        int decimal_position = formated_str.indexOf('.');
        formated_str.insert(decimal_position+1,'0');
    }
    return formated_str;
}

double ScientificLineEdit::value() const
{
    return QLineEdit::text().toDouble();
}

