#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addprojectdialog.h"
#include "BusinessLogic/managerprojektow.h"

#include <QString>
#include <QTextEdit>
#include <algorithm>

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

    auto projekty = managerProjektow.getProjekty();
    showProjectsInTable(projekty);
    ui->projectTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortButtonClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);
    connect(ui->searchFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSearchFilterChanged);
    ui->searchLineEdit->setPlaceholderText("Wpisz dowolny szukany fragment...");




}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSortButtonClicked() {
    auto projekty = managerProjektow.getProjekty();

    std::sort(projekty.begin(), projekty.end(), [this](Project* a, Project* b) {
        return (ui->sortComboBox->currentText() == "A-Z")
        ? a->getName() < b->getName()
                       : a->getName() > b->getName();
    });

    showProjectsInTable(projekty);  // ← zamiast wypisywania do QTextEdit
}

void MainWindow::showProjectsInTable(const std::vector<Project*>& projekty) {
    ui->projectTableWidget->clear();
    ui->projectTableWidget->setColumnCount(7);
    ui->projectTableWidget->setRowCount(static_cast<int>(projekty.size()));

    ui->projectTableWidget->setHorizontalHeaderLabels({
        "Nazwa", "Technologie", "Stan", "Czas pracy", "Repozytorium", "Współautorzy", "Obowiązki"
    });

    for (int row = 0; row < projekty.size(); ++row) {
        Project* p = projekty[row];

        ui->projectTableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(p->getName())));

        QString techList;
        for (const auto& t : p->getTechnologies())
            techList += QString::fromStdString(t) + ", ";
        if (!techList.isEmpty()) techList.chop(2);
        ui->projectTableWidget->setItem(row, 1, new QTableWidgetItem(techList));

        ui->projectTableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(p->getStatus())));

        ui->projectTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(p->getWorkTime())));

        ui->projectTableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(p->getRepositoryLink())));

        TeamProject* team = dynamic_cast<TeamProject*>(p);
        if (team) {
            QString authors;
            for (const auto& a : team->getCollaborators())
                authors += QString::fromStdString(a) + ", ";
            if (!authors.isEmpty()) authors.chop(2);
            ui->projectTableWidget->setItem(row, 5, new QTableWidgetItem(authors));

            ui->projectTableWidget->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(team->getResponsibilities())));
        } else {

            ui->projectTableWidget->setItem(row, 5, new QTableWidgetItem("-"));
            ui->projectTableWidget->setItem(row, 6, new QTableWidgetItem("-"));
        }
    }
}
void MainWindow::on_addProjectButton_clicked()
{
    AddProjectDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        auto name = dialog.getName().toStdString();
        QStringList techList = dialog.getTechnologies();
        std::vector<std::string> technologies;
        for (const QString& tech : techList) {
            technologies.push_back(tech.trimmed().toStdString());
        }

        auto status = dialog.getStatus().toStdString();
        auto time = dialog.getWorkTime();
        auto repo = dialog.getRepository().toStdString();

        Project* newProject = nullptr;

        if (dialog.isTeamProject()) {
            std::vector<std::string> collaborators;
            for (const auto& c : dialog.getCollaborators())
                collaborators.push_back(c.trimmed().toStdString());

            newProject = new TeamProject(
                name, technologies, status, time, repo,
                collaborators,
                dialog.getResponsibilities().toStdString()
                );
        } else {
            newProject = new Project(name, technologies, status, time, repo);
        }

        managerProjektow.addProject(newProject);
        showProjectsInTable(managerProjektow.getProjekty());
    }
}
void MainWindow::onSearchTextChanged(const QString&)
{
    QString searchText = ui->searchLineEdit->text().trimmed().toLower();
    QString filterType = ui->searchFilterComboBox->currentText();

    std::vector<Project*> filtered;
    for (auto* p : managerProjektow.getProjekty()) {
        QString name = QString::fromStdString(p->getName()).toLower();
        QString status = QString::fromStdString(p->getStatus()).toLower();
        QString repo = QString::fromStdString(p->getRepositoryLink()).toLower();

        QString techList;
        for (const auto& t : p->getTechnologies())
            techList += QString::fromStdString(t).toLower() + " ";

        QString authors;
        if (TeamProject* tp = dynamic_cast<TeamProject*>(p)) {
            for (const auto& a : tp->getCollaborators())
                authors += QString::fromStdString(a).toLower() + " ";
        }

        bool match = false;

        if (filterType == "Nazwa")
            match = name.contains(searchText);
        else if (filterType == "Technologie")
            match = techList.contains(searchText);
        else if (filterType == "Status")
            match = status.contains(searchText);
        else if (filterType == "Repozytorium")
            match = repo.contains(searchText);
        else if (filterType == "Współautorzy")
            match = authors.contains(searchText);
        else  // Wszystko
            match = name.contains(searchText) || techList.contains(searchText) ||
                    status.contains(searchText) || repo.contains(searchText) ||
                    authors.contains(searchText);

        if (match)
            filtered.push_back(p);
    }

    showProjectsInTable(filtered);
}
void MainWindow::onSearchFilterChanged(int index)
{
    QString placeholder;

    switch (index) {
    case 1: placeholder = "Wpisz nazwę projektu..."; break;
    case 2: placeholder = "Wpisz technologię..."; break;
    case 3: placeholder = "Wpisz status projektu..."; break;
    case 4: placeholder = "Wpisz link do repozytorium..."; break;
    case 5: placeholder = "Wpisz współautora..."; break;
    default: placeholder = "Wpisz dowolny szukany fragment..."; break;
    }

    ui->searchLineEdit->setPlaceholderText(placeholder);
}

