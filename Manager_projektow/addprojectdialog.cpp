#include "addprojectdialog.h"
#include "ui_addprojectdialog.h"

AddProjectDialog::AddProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProjectDialog)
{
    ui->setupUi(this);
}

AddProjectDialog::~AddProjectDialog()
{
    delete ui;
}

QString AddProjectDialog::getName() const {
    return ui->nameLineEdit->text();
}

QStringList AddProjectDialog::getTechnologies() const {
    return ui->technologiesLineEdit->text().split(",", Qt::SkipEmptyParts);
}

QString AddProjectDialog::getStatus() const {
    return ui->statusLineEdit->text();
}

float AddProjectDialog::getWorkTime() const {
    return static_cast<float>(ui->workTimeSpinBox->value());
}

QString AddProjectDialog::getRepository() const {
    return ui->repositoryLineEdit->text();
}

void AddProjectDialog::on_buttonBox_accepted()
{
    accept();
}


void AddProjectDialog::on_buttonBox_rejected()
{
    reject();
}

