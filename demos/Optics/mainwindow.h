#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <ParamTree/TreeModel>
#include <ParamTree/TreeView>

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

private:
    Ui::MainWindow *ui;
    paramtree::TreeModel* m_model;
    paramtree::TreeView* m_treeview;
    QString m_filename;

    QVBoxLayout* m_top_layout;
    QVBoxLayout* m_input_layout;
    QVBoxLayout* m_medium_layout;

    void readSettings();
    void writeSettings();

};

#endif // MAINWINDOW_H
