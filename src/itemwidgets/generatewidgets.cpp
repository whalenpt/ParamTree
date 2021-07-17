#include "modelview/treemodel.h"
#include "modelview/treeitem.h"
#include "itemwidgets/itemwidget.h"
#include "itemwidgets/generatewidgets.h"
#include "itemwidgets/treecombobox.h"
#include "itemwidgets/treescilineedit.h"
#include "itemwidgets/treespinbox.h"
#include "itemwidgets/treelineedit.h"
#include "itemwidgets/treedblspinbox.h"
#include "itemwidgets/treecheckbox.h"

namespace paramtree{

QWidget* generateParamWidget(TreeModel* model,const TreeItem& item)
{
    QWidget* widget;
    if(item.dtype() == TreeItem::DataType::COMBO)
        widget = new TreeComboBox(model,item);
    else if(item.dtype() == TreeItem::DataType::BOOL)
        widget = new TreeCheckBox(model,item);
    else if(item.dtype() == TreeItem::DataType::SCIENTIFIC)
        widget = new TreeSciLineEdit(model,item);
    else if(item.dtype() == TreeItem::DataType::VAR){
        QMetaType var_type = item.value().metaType();
        if(var_type.id() == QMetaType::Int)
            widget = new TreeSpinBox(model,item);
        else if(var_type.id() == QMetaType::QString)
            widget = new TreeLineEdit(model,item);
        else if(var_type.id() == QMetaType::Double)
            widget = new TreeDblSpinBox(model,item);
        else if(var_type.id() == QMetaType::LongLong)
            widget = new TreeSpinBox(model,item);
        else
            widget = new TreeLineEdit(model,item);
    } else
        widget = new TreeLineEdit(model,item);
    return widget;
}

ItemWidget* generateItemWidget(TreeModel* model,const TreeItem& item)
{
    return new ItemWidget(item,generateParamWidget(model,item));
}

}

