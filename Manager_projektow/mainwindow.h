#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BusinessLogic/managerprojektow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSortButtonClicked();
    void showProjectsInTable(const std::vector<Project*>& projekty);

    void on_addProjectButton_clicked();
    void onSearchTextChanged(const QString& text);
    void onSearchFilterChanged(int index);
    void on_deleteProjectButton_clicked();
    void on_showStatsButton_clicked();
    void on_generatePdfButton_clicked();

private:
    Ui::MainWindow *ui;
    ManagerProjektow managerProjektow;
};

QString join(const std::vector<std::string>& vec, const QString& sep);

#endif // MAINWINDOW_H
