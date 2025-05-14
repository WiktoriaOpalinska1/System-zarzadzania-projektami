#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BusinessLogic/managerprojektow.h"

#include <QString>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ManagerProjektow manager;

    QString allProjects;

    for (auto* p : manager.getProjekty()) {
        allProjects += QString::fromStdString(p->toString());
        allProjects += "\n-------------------\n";
    }

    ui->textEdit->setText(allProjects);
}

MainWindow::~MainWindow() {
    delete ui;
}
