#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addprojectdialog.h"
#include "statisticsdialog.h"
#include "BusinessLogic/managerprojektow.h"

#include <QString>
#include <QTextEdit>
#include <algorithm>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

const QString SAVE_FILENAME = "projekty.json";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    managerProjektow.loadFromFile(SAVE_FILENAME);
    showProjectsInTable(managerProjektow.getProjekty());

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
    connect(ui->deleteProjectButton, &QPushButton::clicked, this, &MainWindow::on_deleteProjectButton_clicked);


}

MainWindow::~MainWindow() {
    delete ui;
    managerProjektow.saveToFile(SAVE_FILENAME);

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

void MainWindow::on_deleteProjectButton_clicked() {
    int selectedRow = ui->projectTableWidget->currentRow();
    if (selectedRow >= 0 && selectedRow < static_cast<int>(managerProjektow.getProjekty().size())) {
        auto projekty = managerProjektow.getProjekty();
        Project* toRemove = projekty[selectedRow];

        managerProjektow.removeProject(toRemove);
        showProjectsInTable(managerProjektow.getProjekty());
    }
}

void MainWindow::on_showStatsButton_clicked() {
    const auto& projekty = managerProjektow.getProjekty();

    int totalProjects = projekty.size();
    float totalTime = 0.0f;
    int teamCount = 0;
    std::map<std::string, int> technologyCount;
    std::map<std::string, int> statusCount;

    for (auto* p : projekty) {
        totalTime += p->getWorkTime();
        for (const auto& tech : p->getTechnologies()) {
            technologyCount[tech]++;
        }
        statusCount[p->getStatus()]++;
        if (dynamic_cast<TeamProject*>(p)) {
            teamCount++;
        }
    }

    float averageTime = totalProjects > 0 ? totalTime / totalProjects : 0;
    int individualCount = totalProjects - teamCount;

    std::vector<std::pair<std::string, int>> sortedTechs(technologyCount.begin(), technologyCount.end());
    std::sort(sortedTechs.begin(), sortedTechs.end(), [](const auto& a, const auto& b) {
        return b.second > a.second;
    });

    QString topTech;
    for (int i = 0; i < std::min(3, static_cast<int>(sortedTechs.size())); ++i) {
        topTech += QString::fromStdString(sortedTechs[i].first);
        if (i < std::min(3, static_cast<int>(sortedTechs.size())) - 1)
            topTech += ", ";
    }

    QStringList stats;
    stats << "Liczba projektów: " + QString::number(totalProjects);
    stats << "Łączny czas pracy: " + QString::number(totalTime) + " h";
    stats << "Średni czas pracy: " + QString::number(averageTime, 'f', 2) + " h";
    stats << "Projekty zespołowe: " + QString::number(teamCount);
    stats << "Projekty indywidualne: " + QString::number(individualCount);
    stats << "Najpopularniejsze technologie: " + topTech;
    stats << "Statusy:";

    for (const auto& [status, count] : statusCount) {
        stats << " - " + QString::fromStdString(status) + ": " + QString::number(count);
    }

    StatisticsDialog dialog(stats, this);
    dialog.exec();  // pokaż okno statystyk
}


// Pomocnicza funkcja do łączenia std::vector<std::string> na QString
QString join(const std::vector<std::string>& vec, const QString& sep) {
    QStringList list;
    for (const auto& s : vec)
        list << QString::fromStdString(s);
    return list.join(sep);
}

void MainWindow::on_generatePdfButton_clicked() {
    const auto& projekty = managerProjektow.getProjekty();

    int totalProjects = projekty.size();
    float totalTime = 0.0f;
    int teamCount = 0;
    std::map<std::string, int> technologyCount;
    std::map<std::string, int> statusCount;

    for (auto* p : projekty) {
        totalTime += p->getWorkTime();
        for (const auto& tech : p->getTechnologies()) {
            technologyCount[tech]++;
        }
        statusCount[p->getStatus()]++;
        if (dynamic_cast<TeamProject*>(p)) {
            teamCount++;
        }
    }

    float averageTime = totalProjects > 0 ? totalTime / totalProjects : 0;
    int individualCount = totalProjects - teamCount;

    std::vector<std::pair<std::string, int>> sortedTechs(technologyCount.begin(), technologyCount.end());
    std::sort(sortedTechs.begin(), sortedTechs.end(), [](const auto& a, const auto& b) {
        return b.second > a.second;
    });

    QString topTech;
    for (int i = 0; i < std::min(3, static_cast<int>(sortedTechs.size())); ++i) {
        topTech += QString::fromStdString(sortedTechs[i].first);
        if (i < std::min(3, static_cast<int>(sortedTechs.size())) - 1)
            topTech += ", ";
    }

    QStringList stats;
    stats << "<b>Liczba projektów:</b> " + QString::number(totalProjects);
    stats << "<b>Łączny czas pracy:</b> " + QString::number(totalTime) + " h";
    stats << "<b>Średni czas pracy:</b> " + QString::number(averageTime, 'f', 2) + " h";
    stats << "<b>Projekty zespołowe:</b> " + QString::number(teamCount);
    stats << "<b>Projekty indywidualne:</b> " + QString::number(individualCount);
    stats << "<b>Najpopularniejsze technologie:</b> " + topTech;
    stats << "<b>Statusy:</b>";
    for (const auto& [status, count] : statusCount) {
        stats << " - " + QString::fromStdString(status) + ": " + QString::number(count);
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Zapisz PDF", "", "PDF files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(96);
    QPainter painter(&writer);
    painter.setFont(QFont("Times", 10));

    const int margin = 50;
    const int rowHeight = 25;
    int y = margin;

    // ========= NAGŁÓWEK =========
    painter.setFont(QFont("Times", 16, QFont::Bold));
    QString headerText = "Projekty programistyczne";
    int headerWidth = painter.fontMetrics().horizontalAdvance(headerText);
    int pageWidth = writer.width();
    painter.drawText((pageWidth - headerWidth) / 2, y, headerText);

    y += rowHeight + 30;
    painter.setFont(QFont("Times", 10, QFont::Bold));
    painter.drawText(margin, y, "Nazwa");
    painter.drawText(margin + 120, y, "Technologie");
    painter.drawText(margin + 260, y, "Status");
    painter.drawText(margin + 370, y, "Czas (h)");
    painter.drawText(margin + 450, y, "Repozytorium");

    y += rowHeight;
    painter.setFont(QFont("Times", 10));

    // ========= TABELA PROJEKTÓW =========
    for (const auto* p : projekty) {
        QString name = QString::fromStdString(p->getName());
        QStringList techList;
        for (const auto& tech : p->getTechnologies()) {
            techList << QString::fromStdString(tech);
        }
        QString techs = techList.join(", ");
        QString status = QString::fromStdString(p->getStatus());
        QString time = QString::number(p->getWorkTime());
        QString repo = QString::fromStdString(p->getRepositoryLink());
        painter.drawText(margin, y, name);
        painter.drawText(margin + 120, y, techs);
        QString statusIcon;
        QColor statusColor = Qt::black;

        if (status == "Zakończony") {
            statusIcon = "✅ ";
            statusColor = QColor(0, 128, 0);
        } else if (status == "W trakcie") {
            statusIcon = "🔄 ";
            statusColor = QColor(218, 165, 32);
        } else if (status == "Wstrzymany") {
            statusIcon = "⛔ ";
            statusColor = Qt::red;
        } else {
            statusIcon = "❔ ";
            statusColor = Qt::darkGray;
        }

        painter.setPen(statusColor);
        painter.drawText(margin + 260, y, statusIcon + status);
        painter.setPen(Qt::black);
        painter.drawText(margin + 370, y, time);
        painter.drawText(margin + 450, y, repo);

        y += rowHeight;
    }

    // ========= STATYSTYKI =========
    y += 30;
    painter.setFont(QFont("Times", 14, QFont::Bold));
    QString statsHeader = "Ogólne statystyki";
    int statsHeaderWidth = painter.fontMetrics().horizontalAdvance(statsHeader);
    painter.drawText((pageWidth - statsHeaderWidth) / 2, y, statsHeader);
    y += rowHeight;


    // Wyświetlanie statystyk
    painter.setFont(QFont("Times", 10));
    for (const QString& line : stats) {
        if (line.startsWith("<b>")) {
            QTextDocument doc;
            doc.setHtml(line);
            painter.save();
            painter.translate(margin, y);
            doc.drawContents(&painter);
            painter.restore();
            y += doc.size().height();
        } else if (line.startsWith(" - ")) {
            painter.drawText(margin + 20, y+10, line);
            y += rowHeight;
        } else {
            painter.drawText(margin, y, line);
            y += rowHeight;
        }
    }

    painter.end();
}
