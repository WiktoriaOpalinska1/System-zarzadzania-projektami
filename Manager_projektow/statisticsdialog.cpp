#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

StatisticsDialog::StatisticsDialog(const QStringList& stats, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog){
    ui->setupUi(this);
    ui->listWidget->addItems(stats);
}

StatisticsDialog::~StatisticsDialog(){
    delete ui;
}
