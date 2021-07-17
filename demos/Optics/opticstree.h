#ifndef OPTICSTREE_H
#define OPTICSTREE_H

#include <map>
#include <string>
#include <QVariant>


namespace paramtree{
    class TreeModel;
    class TreeItem;
}

namespace optics{

    using namespace paramtree;

    void generateTree(TreeModel* model);
    void updateCD(const TreeItem& item,TreeModel* model);
    void updatePlasma(const TreeItem& item,TreeModel* model);
    void updateShapeT(const TreeItem& item,TreeModel* model);
    void loadInputR(TreeModel* model);
    void loadInputT(TreeModel* model);
    void loadPlasma(TreeModel* model);
    void loadSuperGaussT(TreeModel* model);
}

namespace optics{
    using variant_map = std::map<std::string,QVariant>;
    void generateDefaultMap();
}

#endif // OPTICSTREE_H
