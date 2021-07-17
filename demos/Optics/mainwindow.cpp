

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opticstree.h"

#include <ParamTree/TreeModel>
#include <ParamTree/TreeView>
#include <ParamTree/TreeTabWidget>
#include <ParamTree/TreeWidget>

#include <QSettings>
#include <QVBoxLayout>
#include <QSize>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QAbstractItemModel>
#include <QStringList>
#include <QStackedWidget>

const QString FILENAME_SETTING("MainWindow/Filename");
const QString WINDOWSIZE_SETTING("MainWindow/Size");
const QString WINDOWPOS_SETTING("MainWindow/Pos");

using namespace paramtree;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_filename(),
    m_top_layout(new QVBoxLayout),
    m_input_layout(new QVBoxLayout),
    m_medium_layout(new QVBoxLayout)
{
    ui->setupUi(this);
    m_model = new TreeModel(this);
    m_treeview = new TreeView(m_model);

    // Add the tree view to the ui
    QVBoxLayout* tree_layout = new QVBoxLayout;
    tree_layout->addWidget(m_treeview);
    // ui->treeWidget takes ownership of tree_layout
    // (so no parent necessary when instantiating tree_layout)
    ui->treeWidget->setLayout(tree_layout);
    // m_treeview parent now ui->treeWidget

    QAction* expand_action = new QAction("Expand all");
    QAction* collapse_action = new QAction("Collapse all");
    m_treeview->addAction(expand_action);
    m_treeview->addAction(collapse_action);
    m_treeview->setContextMenuPolicy(Qt::ActionsContextMenu);


    setWindowTitle("Parameter Tree");

    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionExit->setShortcut(QKeySequence::Quit);
    connect(ui->actionExit,&QAction::triggered,this,&QMainWindow::close);
    connect(ui->actionSaveAs,&QAction::triggered,this,&MainWindow::fileSaveAs);
    connect(ui->actionSave,&QAction::triggered,this,&MainWindow::fileSave);
    connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::fileOpen);
    connect(ui->exitButton,&QPushButton::clicked,ui->actionExit,&QAction::triggered);
    connect(expand_action,&QAction::triggered,this,&MainWindow::expandAll);
    connect(collapse_action,&QAction::triggered,this,&MainWindow::collapseAll);
    connect(m_model,&QAbstractItemModel::dataChanged,this,&MainWindow::dataChanged);
    connect(m_model,&QAbstractItemModel::rowsInserted,this,&MainWindow::expandAll);

    readSettings();
    optics::generateTree(m_model);
//    if(!m_filename.isEmpty())
//        m_model->load(m_filename);
//    else
//        optics::generateTree(m_model);

    expandAll();
    QVBoxLayout* vbox = new QVBoxLayout;
    TreeTabWidget* tab_widget = new TreeTabWidget(m_model);
    TreeWidget* tree_widget = new TreeWidget(m_model);

    vbox->addWidget(tab_widget);
    ui->tabWidget->setLayout(vbox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::fileSave()
{
    if(m_filename.isEmpty())
        return fileSaveAs();
    return m_model->save(m_filename);
}

bool MainWindow::fileSaveAs()
{
    m_filename = QFileDialog::getSaveFileName(this,
                                      tr("%1 - Save As").arg(QApplication::applicationName()));
    if(m_filename.isEmpty())
        return false;
    return fileSave();
}

bool MainWindow::fileOpen()
{

    QString dirpath(".");
    if(!m_filename.isEmpty()){
        QFileInfo info(m_filename);
        dirpath = info.absolutePath();
    }
    QString filename = QFileDialog::getOpenFileName(this,
                          tr("%1 - Open").arg(QApplication::applicationName()),
                                                    dirpath);
    if(filename.isEmpty())
        return false;
    if(m_model->load(filename)){
        m_filename = filename;
        expandAll();
        return true;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue(WINDOWSIZE_SETTING,size());
    settings.setValue(WINDOWPOS_SETTING,pos());
    settings.setValue(FILENAME_SETTING,m_filename);
}

void MainWindow::readSettings()
{
    QSettings settings;
    resize(settings.value(WINDOWSIZE_SETTING,QSize(600,600)).toSize());
    move(settings.value(WINDOWPOS_SETTING,QPoint(300,300)).toPoint());
    m_filename = settings.value(FILENAME_SETTING).toString();
}

void MainWindow::expandAll()
{
    m_treeview->expandAll();
    for(int j = 0; j < m_model->columnCount(); j++)
        m_treeview->resizeColumnToContents(j);
}

void MainWindow::collapseAll()
{
    m_treeview->collapseAll();
    for(int j = 0; j < m_model->columnCount(); j++)
        m_treeview->resizeColumnToContents(j);
}

void MainWindow::dataChanged(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/)
{
    QStringList key = m_model->data(topLeft,TreeModel::Role::KEY).toStringList();
    const TreeItem& item = m_model->getItem(topLeft);
    if(key == (QStringList() << "Coordinate Dependency")){
        optics::updateCD(item,m_model);
    } else if(key == (QStringList() << "MEDIUM" << "PlasmaGeneration")){
        optics::updatePlasma(item,m_model);
    } else if (key == (QStringList() << "INPUT PULSE" << "T" << "Shape")) {
        optics::updateShapeT(item,m_model);
    }
}


