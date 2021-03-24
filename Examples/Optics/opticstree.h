#ifndef OPTICSTREE_H
#define OPTICSTREE_H

#include <map>
#include <string>
#include <QVariant>

class ParamTreeModel;
class TreeItem;


namespace optics{

    void generateTree(ParamTreeModel* model);
    void updateCD(const TreeItem& item,ParamTreeModel* model);
    void updatePlasma(const TreeItem& item,ParamTreeModel* model);
    void updateShapeT(const TreeItem& item,ParamTreeModel* model);
    void loadInputR(ParamTreeModel* model);
    void loadInputT(ParamTreeModel* model);
    void loadPlasma(ParamTreeModel* model);
    void loadSuperGaussT(ParamTreeModel* model);
}

namespace optics{
    using variant_map = std::map<std::string,QVariant>;
    void generateDefaultMap();
}

#endif // OPTICSTREE_H
