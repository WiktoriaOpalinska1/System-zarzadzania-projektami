#ifndef ADDPROJECTDIALOG_H
#define ADDPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class AddProjectDialog;
}

class AddProjectDialog : public QDialog{
    Q_OBJECT

public:
    explicit AddProjectDialog(QWidget *parent = nullptr);
    ~AddProjectDialog();

    QString getName() const;
    QStringList getTechnologies() const;
    QString getStatus() const;
    float getWorkTime() const;
    QString getRepository() const;
    bool isTeamProject() const;
    QStringList getCollaborators() const;
    QString getResponsibilities() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_teamProjectCheckBox_toggled(bool checked);

private:
    Ui::AddProjectDialog *ui;
};

#endif // ADDPROJECTDIALOG_H
