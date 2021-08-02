

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
const QStringList INPUT_T_SUPERGAUSSN_KEY{"INPUT PULSE","T","n"};
const QStringList INPUT_T_SHAPE_KEY{"INPUT PULSE","T","Shape"};
const QStringList CD_KEY{"Coordinate Dependency"};

variant_map default_map;

void generateTree(TreeModel* model)
{
    generateDefaultMap();
    auto sim_name = std::make_unique<TreeItem>("Simulation Name",default_map["Simulation Name"]);
    auto cd = std::make_unique<TreeItem>("Coordinate Dependency",default_map["Coordinate Dependency"],TreeItem::DataType::COMBO);
    cd->setAux("RANGE",QStringList() << "RT" << "T");

    auto distance = std::make_unique<TreeItem>("Travel distance",default_map["Travel distance"],TreeItem::DataType::SCIENTIFIC);
    auto num_threads = std::make_unique<TreeItem>("Number of threads",default_map["Number of threads"]);
    num_threads->setAux("MIN",1);
    num_threads->setAux("MAX",16);

    model->addItem(std::move(sim_name));
    model->addItem(std::move(cd));
    model->addItem(std::move(distance));
    model->addItem(std::move(num_threads));

    auto input_group = std::make_unique<TreeItem>("INPUT PULSE");
    auto intensity = std::make_unique<TreeItem>("Intensity",default_map["Intensity"],TreeItem::DataType::SCIENTIFIC);
    input_group->addItem(std::move(intensity));
    input_group->addItem(std::make_unique<TreeItem>("Carrier Wavelength",default_map["Carrier Wavelength"],TreeItem::DataType::SCIENTIFIC));
    model->addItem(std::move(input_group));

    initInputT(model);
    initInputR(model);

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

    //auto secondPlasArg = std::make_unique<TreeItem>("Plasma Name","Multiphoton");
    //medium->addItem(std::move(secondPlasArg));

    // Can add TreeItem objects instead of pointers 
    auto secondPlasArg = TreeItem("Plasma Name","Multiphoton");
    // Note this will COPY the contents of the scondPlasArg TreeItem into the parent TreeItem labeled medium
    medium->addItem(secondPlasArg);
    model->addItem(std::move(medium));

    model->boolLink(PLASMA_GEN_KEY,PLASMA_KEY);
    model->boolLink(PLASMA_GEN_KEY,QStringList() << "MEDIUM" << "Plasma Name");
    model->comboLink(CD_KEY,INPUT_R_KEY,"RT");
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

void initInputT(TreeModel* model)
{
    // can create an object tinput instead of unique_ptr 
    auto tinput = TreeItem("T");
    auto pw = std::make_unique<TreeItem>("Pulse Width",default_map["Pulse Width"],TreeItem::DataType::SCIENTIFIC);
    auto shape = std::make_unique<TreeItem>("Shape",default_map["T_Shape"],TreeItem::DataType::COMBO);
    shape->setAux("RANGE",QStringList() << "gauss" << "bessel" << "airy" << "supergauss");

    auto superGaussM = std::make_unique<TreeItem>("m",1);
    superGaussM->setAux("MIN",1);
    superGaussM->setAux("MAX",9);
    auto superGaussN = std::make_unique<TreeItem>("n",2);

    tinput.addItem(std::move(shape));
    tinput.addItem(std::move(pw));
    tinput.addItem(std::move(superGaussM));
    tinput.addItem(std::move(superGaussN));

    // tinput is COPIED into the model! Don't track the tinput object (access it through the model)
    model->addItem(tinput,model->getIndex("INPUT PULSE"));
    model->comboLink(INPUT_T_SHAPE_KEY,INPUT_T_SUPERGAUSSM_KEY,"supergauss");
    model->comboLink(INPUT_T_SHAPE_KEY,INPUT_T_SUPERGAUSSN_KEY,"supergauss");
}

void initInputR(TreeModel* model)
{
    auto rinput = std::make_unique<TreeItem>("R");
    auto bw = std::make_unique<TreeItem>("Beam Width",default_map["Beam Width"],TreeItem::DataType::SCIENTIFIC);
    auto beam_shape = std::make_unique<TreeItem>("Shape",default_map["R_Shape"],TreeItem::DataType::COMBO);
    beam_shape->setAux("RANGE",QStringList() << "gauss" << "bessel" << "airy");
    rinput->addItem(std::move(beam_shape));
    rinput->addItem(std::move(bw));
    model->addItem(std::move(rinput),model->getIndex("INPUT PULSE"));
}

}


