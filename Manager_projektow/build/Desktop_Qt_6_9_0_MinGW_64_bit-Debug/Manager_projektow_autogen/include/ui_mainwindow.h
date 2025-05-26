/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QComboBox *sortComboBox;
    QPushButton *addProjectButton;
    QTableWidget *projectTableWidget;
    QWidget *widget_3;
    QWidget *widget;
    QPushButton *sortButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(776, 631);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        sortComboBox = new QComboBox(centralwidget);
        sortComboBox->addItem(QString());
        sortComboBox->addItem(QString());
        sortComboBox->setObjectName("sortComboBox");

        gridLayout->addWidget(sortComboBox, 0, 1, 1, 1);

        addProjectButton = new QPushButton(centralwidget);
        addProjectButton->setObjectName("addProjectButton");

        gridLayout->addWidget(addProjectButton, 0, 2, 1, 1);

        projectTableWidget = new QTableWidget(centralwidget);
        projectTableWidget->setObjectName("projectTableWidget");

        gridLayout->addWidget(projectTableWidget, 1, 0, 1, 5);

        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName("widget_3");

        gridLayout->addWidget(widget_3, 0, 3, 1, 1);

        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");

        gridLayout->addWidget(widget, 0, 4, 1, 1);

        sortButton = new QPushButton(centralwidget);
        sortButton->setObjectName("sortButton");

        gridLayout->addWidget(sortButton, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 776, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        sortComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "A-Z", nullptr));
        sortComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Z-A", nullptr));

        addProjectButton->setText(QCoreApplication::translate("MainWindow", "Dodaj Projekt", nullptr));
        sortButton->setText(QCoreApplication::translate("MainWindow", "Sortuj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
