#ifndef OPTICSTREE_H
#define OPTICSTREE_H

#include <map>
#include <string>
#include <QVariant>
#include <ParamTree/TreeModel>


namespace optics{
    using namespace paramtree;
    void generateTree(TreeModel* model);
    void initInputR(TreeModel* model);
    void initInputT(TreeModel* model);
    using variant_map = std::map<std::string,QVariant>;
    void generateDefaultMap();
}

#endif // OPTICSTREE_H
