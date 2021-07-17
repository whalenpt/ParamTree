#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>
#include <QString>
class TreeItem;

class ItemWidget : public QWidget
{
public:
    ItemWidget(const TreeItem& item,QWidget* widget);
private:
    QString m_name;
};


#endif // ITEMWIDGET_H
