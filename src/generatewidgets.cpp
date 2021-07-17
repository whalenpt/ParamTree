#include "generatewidgets.h"
#include "itemwidget.h"
#include "paramtreemodel.h"
#include "paramtreeitem.h"
#include "paramcombobox.h"
#include "paramscientificlineedit.h"
#include "paramspinbox.h"
#include "paramlineedit.h"
#include "paramdoublespinbox.h"
#include "paramcheckbox.h"

QWidget* generateParamWidget(ParamTreeModel* model,const TreeItem& item)
{
    QWidget* widget;
    if(item.dtype() == TreeItem::DataType::COMBO)
        widget = new ParamComboBox(model,item);
    else if(item.dtype() == TreeItem::DataType::BOOL)
        widget = new ParamCheckBox(model,item);
    else if(item.dtype() == TreeItem::DataType::SCIENTIFIC)
        widget = new ParamScientificLineEdit(model,item);
    else if(item.dtype() == TreeItem::DataType::VAR){
        QMetaType var_type = item.value().metaType();
        if(var_type.id() == QMetaType::Int)
            widget = new ParamSpinBox(model,item);
        else if(var_type.id() == QMetaType::QString)
            widget = new ParamLineEdit(model,item);
        else if(var_type.id() == QMetaType::Double)
            widget = new ParamDoubleSpinBox(model,item);
        else if(var_type.id() == QMetaType::LongLong)
            widget = new ParamSpinBox(model,item);
        else
            widget = new ParamLineEdit(model,item);
    } else
        widget = new ParamLineEdit(model,item);
    return widget;
}

ItemWidget* generateItemWidget(ParamTreeModel* model,const TreeItem& item)
{
    return new ItemWidget(item,generateParamWidget(model,item));
}
