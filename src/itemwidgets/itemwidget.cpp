#include "itemwidgets/itemwidget.h"
#include "modelview/treeitem.h"
#include "modelview/treemodel.h"
#include <QHBoxLayout>
#include <QLabel>


namespace paramtree{

const int MIN_WIDGET_WIDTH = 40;
//const int MAX_WIDGET_WIDTH = 240;
const int MIN_LABEL_WIDTH = 30;
const int WIDGET_HEIGHT = 20;
const int LABEL_WIDGET_SPACING = 100;

ItemWidget::ItemWidget(const TreeItem& item,QWidget* widget) :
    m_name(item.name())
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0,4,0,4);
    hbox->setSizeConstraint(QLayout::SetMinimumSize);

    widget->setMinimumSize(QSize(MIN_WIDGET_WIDTH,WIDGET_HEIGHT));
//    widget->setMaximumSize(QSize(MAX_WIDGET_WIDTH,WIDGET_HEIGHT));

    QLabel* label = new QLabel(m_name + ":");
    label->setMinimumSize(QSize(MIN_LABEL_WIDTH,WIDGET_HEIGHT));
    label->setMaximumHeight(WIDGET_HEIGHT);

    hbox->addWidget(label);
    hbox->addSpacing(LABEL_WIDGET_SPACING);
    hbox->addWidget(widget);
    setLayout(hbox);
}

}


