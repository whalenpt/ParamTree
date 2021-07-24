

#include "opticstree.h"
#include <ParamTree/modelview/treemodel.h>
#include <ParamTree/TreeItem>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <memory>

namespace optics{

using namespace paramtree;

const QStringList INPUT_T_KEY{"INPUT PULSE","T"};
const QStringList INPUT_R_KEY{"INPUT PULSE","R"};
const QStringList INPUT_XY_KEY{"INPUT PULSE","XY"};
const QStringList PLASMA_KEY{"MEDIUM","PLASMA"};
const QStringList PLASMA_GEN_KEY{"MEDIUM","PlasmaGeneration"};
const QStringList INPUT_T_SUPERGAUSSM_KEY{"INPUT PULSE","T","m"};

variant_map default_map;

void generateTree(TreeModel* model)
{
    generateDefaultMap();
    auto sim_name = std::make_unique<TreeItem>("Simulation Name",default_map["Simulation Name"]);
    auto cd = std::make_unique<TreeItem>("Coordinate Dependency",default_map["Coordinate Dependency"],TreeItem::DataType::COMBO);
    cd->setAux("RANGE",QStringList() << "RT" << "T");

    auto distance = std::make_unique<TreeItem>("Travel distance",default_map["Travel distance"],TreeItem::DataType::SCIENTIFIC);
    auto num_threads = std::make_unique<TreeItem>("Number of threads",default_map["Number of threads"]);
    num_threads->setAux("MINIMUM",1);
    num_threads->setAux("MAXIMUM",16);

    model->addItem(std::move(sim_name));
    model->addItem(std::move(cd));
    model->addItem(std::move(distance));
    model->addItem(std::move(num_threads));

    auto input_group = std::make_unique<TreeItem>("INPUT PULSE");
    auto intensity = std::make_unique<TreeItem>("Intensity",default_map["Intensity"],TreeItem::DataType::SCIENTIFIC);
    input_group->addItem(std::move(intensity));
    input_group->addItem(std::make_unique<TreeItem>("Carrier Wavelength",default_map["Carrier Wavelength"],TreeItem::DataType::SCIENTIFIC));
    model->addItem(std::move(input_group));

    if(model->getItem("Coordinate Dependency").value().toString() == "RT"){
        loadInputT(model);
        loadInputR(model);
    } else if(model->getItem("Coordinate Dependency").value().toString() == "T")
        loadInputT(model);
    else if(model->getItem("Coordinate Dependency").value().toString() == "R")
        loadInputR(model);

    auto medium = std::make_unique<TreeItem>("MEDIUM");
    medium->addItem(std::make_unique<TreeItem>("Max Wavelength",default_map["Max Wavelength"],TreeItem::DataType::SCIENTIFIC));
    medium->addItem(std::make_unique<TreeItem>("Min Frequency",default_map["Min Frequency"],TreeItem::DataType::SCIENTIFIC));

    auto n0 = std::make_unique<TreeItem>("n0",default_map["n0"]);
    n0->setAux("STEP SIZE",0.01);
    medium->addItem(std::move(n0));
    medium->addItem(std::make_unique<TreeItem>("n2",default_map["n2"],TreeItem::DataType::SCIENTIFIC));
    auto plas_gen = std::make_unique<TreeItem>("PlasmaGeneration",default_map["PlasmaGeneration"],TreeItem::DataType::BOOL);
    medium->addItem(std::move(plas_gen));

    auto plasma = std::make_unique<TreeItem>("PLASMA");
    plasma->addItem(std::make_unique<TreeItem>("rhoN",default_map["rhoN"],TreeItem::DataType::SCIENTIFIC));
    plasma->addItem(std::make_unique<TreeItem>("MultiphotonK",default_map["MultiphotonK"]));
    plasma->addItem(std::make_unique<TreeItem>("SigmaK",default_map["SigmaK"],TreeItem::DataType::SCIENTIFIC));
    plasma->addItem(std::make_unique<TreeItem>("CollisionTime",default_map["CollisionTime"],TreeItem::DataType::SCIENTIFIC));
    plasma->addItem(std::make_unique<TreeItem>("Ui",default_map["Ui"]));
    medium->addItem(std::move(plasma));
    model->addItem(std::move(medium));

    QModelIndex plasmagen_index = model->getIndex(PLASMA_GEN_KEY);
    model->boolLink(plasmagen_index,PLASMA_KEY);
}

void generateDefaultMap()
{
    default_map["Simulation Name"] = QVariant("No Name");
    default_map["Coordinate Dependency"] = QVariant("RT");
    default_map["Travel distance"] = QVariant(2.0e-2);
    default_map["Number of threads"] = QVariant(4);

    default_map["Intensity"] = 5.5e14;
    default_map["Carrier Wavelength"] = 800.0e-9;
    default_map["Pulse Width"] = 20.0e-15;
    default_map["T_Shape"] = "gauss";

    default_map["Beam Width"] = 50.0e-6;
    default_map["R_Shape"] = "gauss";

    default_map["Max Wavelength"] = 4000.0e-9;
    default_map["Min Frequency"] =200.0e-9;
    default_map["n0"] = 1.334;
    default_map["n2"] =  4.1e-20;

    default_map["PlasmaGeneration"] = true;
    default_map["rhoN"] = 6.68e28;
    default_map["MultiphotonK"] = QVariant(5);
    default_map["SigmaK"] = 1.2e-72;
    default_map["CollisionTime"] = 3.0e-15;
    default_map["Ui"] = 6.5;
}


void loadPlasma(TreeModel* model){

    QSettings settings;
    QStringList rho_key,multiK_key,sigK_key,coltime_key,ui_key;
    rho_key << PLASMA_KEY << "rhoN";
    multiK_key << PLASMA_KEY << "MultiphotonK";
    sigK_key << PLASMA_KEY << "SigmaK";
    coltime_key << PLASMA_KEY << "CollisionTime";
    ui_key << PLASMA_KEY << "Ui";

    auto plasma = std::make_unique<TreeItem>("PLASMA");
    plasma->addItem(std::make_unique<TreeItem>("rhoN",model->readFromSettings(settings,rho_key),
                            TreeItem::DataType::SCIENTIFIC));
    plasma->addItem(std::make_unique<TreeItem>("MultiphotonK",model->readFromSettings(settings,multiK_key)));
    plasma->addItem(std::make_unique<TreeItem>("SigmaK",model->readFromSettings(settings,sigK_key),
                            TreeItem::DataType::SCIENTIFIC));
    plasma->addItem(std::make_unique<TreeItem>("CollisionTime",model->readFromSettings(settings,coltime_key),
                            TreeItem::DataType::SCIENTIFIC));
    plasma->addItem(std::make_unique<TreeItem>("Ui",model->readFromSettings(settings,ui_key)));
    model->addItem(std::move(plasma),model->getIndex("MEDIUM"));
}

void updatePlasma(const TreeItem& item,TreeModel* model)
{
    if(item.value().toBool()){
        if(!model->hasItem(PLASMA_KEY)){
            loadPlasma(model);
        }
    } else {
        if(model->hasItem(PLASMA_KEY)){
            QModelIndex index = model->getIndex(PLASMA_KEY);
            QSettings settings;
            model->saveToSettings(settings,index);
            model->removeRows(index.row(),1,index.parent());
        }
    }
}

void updateShapeT(const TreeItem& item,TreeModel* model)
{
    QString val = item.value().toString();
    if(val == "supergauss"){
        if(!model->hasItem(INPUT_T_SUPERGAUSSM_KEY)){
            loadSuperGaussT(model);
        }
    } else{
        if(model->hasItem(INPUT_T_SUPERGAUSSM_KEY)){
            QModelIndex index = model->getIndex(INPUT_T_SUPERGAUSSM_KEY);
            QSettings settings;
            model->saveToSettings(settings,index);
            model->removeRows(index.row(),1,index.parent());
        }
    }
}

void loadSuperGaussT(TreeModel* model)
{
    QSettings settings;
    auto superGaussM = std::make_unique<TreeItem>("m",model->readFromSettings(settings,INPUT_T_SUPERGAUSSM_KEY).toInt());
    superGaussM->setAux("MINIMUM",1);
    superGaussM->setAux("MAXIMUM",9);
    model->addItem(std::move(superGaussM),model->getIndex(INPUT_T_KEY));
}

//QSettings settings("PTW software","ParamTreeWidgetTest");
void updateCD(const TreeItem& item,TreeModel* model)
{
    QString cd_str = item.value().toString();
    if(cd_str == "T"){
        if(model->hasItem(INPUT_R_KEY)){
            QModelIndex index = model->getIndex(INPUT_R_KEY);
            QSettings settings;
            model->saveToSettings(settings,index);
            model->removeRows(index.row(),1,index.parent());
        }
        if(!model->hasItem(INPUT_T_KEY))
            loadInputT(model);
    } else if(cd_str == "RT"){
        if(!model->hasItem(INPUT_T_KEY))
            loadInputT(model);
        if(!model->hasItem(INPUT_R_KEY))
            loadInputR(model);
    }
}

void loadInputT(TreeModel* model)
{
    QSettings settings;
    QVariant pw_var(model->readFromSettings(settings,QStringList() << INPUT_T_KEY << "Pulse Width"));
    QVariant shape_var(model->readFromSettings(settings,QStringList() << INPUT_T_KEY << "Shape"));

    auto tinput = std::make_unique<TreeItem>("T");
    if(pw_var.isNull())
        pw_var = default_map["Pulse Width"];
    if(shape_var.isNull())
        shape_var = default_map["T_Shape"];

    auto pw = std::make_unique<TreeItem>("Pulse Width",pw_var,TreeItem::DataType::SCIENTIFIC);
    auto shape = std::make_unique<TreeItem>("Shape",shape_var,TreeItem::DataType::COMBO);
    shape->setAux("RANGE",QStringList() << "gauss" << "bessel" << "airy");

    tinput->addItem(std::move(shape));
    tinput->addItem(std::move(pw));
    model->addItem(std::move(tinput),model->getIndex("INPUT PULSE"));
}


void loadInputR(TreeModel* model){

    QSettings settings;
    QVariant bw_var(model->readFromSettings(settings,QStringList() << INPUT_R_KEY << "Beam Width"));
    QVariant shape_var(model->readFromSettings(settings,QStringList() << INPUT_R_KEY << "Shape"));
    auto rinput = std::make_unique<TreeItem>("R");

    if(bw_var.isNull())
        bw_var = default_map["Beam Width"];
    if(shape_var.isNull())
        shape_var = default_map["R_Shape"];

    auto bw = std::make_unique<TreeItem>("Beam Width",bw_var,TreeItem::DataType::SCIENTIFIC);
    auto beam_shape = std::make_unique<TreeItem>("Shape",shape_var,TreeItem::DataType::COMBO);
    beam_shape->setAux("RANGE",QStringList() << "gauss" << "bessel" << "airy");
    rinput->addItem(std::move(beam_shape));
    rinput->addItem(std::move(bw));
    model->addItem(std::move(rinput),model->getIndex("INPUT PULSE"));
}




}
