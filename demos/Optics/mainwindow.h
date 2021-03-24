#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QVBoxLayout>
#include "paramtreemodel.h"
#include "paramtreewidget.h"
#include "paramtreeview.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void expandAll();
    void collapseAll();
    bool fileSave();
    bool fileSaveAs();
    bool fileOpen();
    void dataChanged(const QModelIndex& topLeft,const QModelIndex& /*bottomRight*/);

private:
    Ui::MainWindow *ui;
    ParamTreeModel* m_model;
    ParamTreeView* m_treeview;
    QString m_filename;

    QVBoxLayout* m_top_layout;
    QVBoxLayout* m_input_layout;
    QVBoxLayout* m_medium_layout;

//    ParamTreeWidget* m_input_group;
//    ParamTreeWidget* m_medium_group;

    void readSettings();
    void writeSettings();
    void connectWidgets();

};

#endif // MAINWINDOW_H
