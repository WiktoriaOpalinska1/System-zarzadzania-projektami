#include "addprojectdialog.h"
#include "ui_addprojectdialog.h"

AddProjectDialog::AddProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProjectDialog)
{
    ui->setupUi(this);
    ui->collaboratorsLabel->setVisible(false);
    ui->collaboratorsLineEdit->setVisible(false);
    ui->responsibilitiesLabel->setVisible(false);
    ui->responsibilitiesLineEdit->setVisible(false);
    ui->statusComboBox->addItems({"Zakończony", "W trakcie", "Wstrzymany", "Planowany"});
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
    return ui->statusComboBox->currentText();
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

bool AddProjectDialog::isTeamProject() const {
    return ui->teamProjectCheckBox->isChecked();
}
QStringList AddProjectDialog::getCollaborators() const {
    return ui->collaboratorsLineEdit->text().split(",", Qt::SkipEmptyParts);
}

QString AddProjectDialog::getResponsibilities() const {
    return ui->responsibilitiesLineEdit->text();
}

void AddProjectDialog::on_teamProjectCheckBox_toggled(bool checked)
{
    ui->collaboratorsLabel->setVisible(checked);
    ui->collaboratorsLineEdit->setVisible(checked);
    ui->responsibilitiesLabel->setVisible(checked);
    ui->responsibilitiesLineEdit->setVisible(checked);
}
