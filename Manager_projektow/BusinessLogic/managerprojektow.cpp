#include "managerprojektow.h"
#include <iostream>
#include <algorithm>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Project::Project(const string& name, const vector<string>& technologies, const string& status,
                 float workTime, const string& repositoryLink) {
    this->name = name;
    this->technologies = technologies;
    this->status = status;
    this->workTime = workTime;
    this->repositoryLink = repositoryLink;
}

string Project::getName() const { return name; }

vector<string> Project::getTechnologies() const { return technologies; }

string Project::getStatus() const { return status; }

float Project::getWorkTime() const { return workTime; }

string Project::getRepositoryLink() const { return repositoryLink; }

void Project::setName(const string& name) { this->name = name; }

void Project::setTechnologies(const vector<string>& technologies) { this->technologies = technologies; }

void Project::setStatus(const string& status) { this->status = status; }

void Project::setWorkTime(float time) { this->workTime = time; }

void Project::setRepositoryLink(const string& link) { this->repositoryLink = link; }

void Project::printInfo() const {
    cout << "=== Projekt ===" << endl;
    cout << "Nazwa: " << name << endl;
    cout << "Technologie: ";
    for (const auto& tech : technologies) {
        cout << tech << " ";
    }
    cout << endl;
    cout << "Stan: " << status << endl;
    cout << "Czas pracy: " << workTime << " h" << endl;
    cout << "Repozytorium: " << repositoryLink << endl;
}

string Project::toString() const {
    std::string result = "Nazwa: " + name + "\n";
    result += "Technologie: ";
    for (const auto& tech : technologies) {
        result += tech + " ";
    }
    result += "\nStan: " + status + "\n";
    result += "Czas pracy: " + std::to_string(workTime) + " h\n";
    result += "Repozytorium: " + repositoryLink + "\n";
    return result;
}

TeamProject::TeamProject(const string& name, const vector<string>& technologies, const string& status, float workTime,
                         const string& repositoryLink, const vector<string>& collaborators, const string& responsibilities)
    : Project(name, technologies, status, workTime, repositoryLink){
    this->collaborators = collaborators;
    this->responsibilities = responsibilities;
}

const vector<string>& TeamProject::getCollaborators() const { return collaborators; }

const string& TeamProject::getResponsibilities() const { return responsibilities; }

void TeamProject::setCollaborators(const vector<string>& collaborators) { this->collaborators = collaborators; }

void TeamProject::setResponsibilities(const string& responsibilities) { this->responsibilities = responsibilities; }

ManagerProjektow::ManagerProjektow() {
    projekty.push_back(new Project("CRM", {"C++", "Qt"}, "W trakcie", 100, "https://github.com/projekt/crm"));
    projekty.push_back(new TeamProject("Rezerwacje", {"C++", "Qt", "MySQL"}, "Zakończony", 200,
                                       "https://github.com/projekt/rezerwacje",
                                       {"Jan", "Anna"}, "Backend + GUI"));
}

const vector<Project*>& ManagerProjektow::getProjekty() const { return projekty; }

ManagerProjektow::~ManagerProjektow() {
    for (auto* p : projekty)
        delete p;
}

void ManagerProjektow::addProject(Project* p) { projekty.push_back(p); }

void ManagerProjektow::removeProject(Project* p) {
    auto it = std::find(projekty.begin(), projekty.end(), p);
    if (it != projekty.end()) {
        delete *it;  // opcjonalne, tylko jeśli odpowiadasz za dealokację
        projekty.erase(it);
    }
}

bool ManagerProjektow::saveToFile(const QString& filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QJsonArray array;
    for (const auto* p : projekty)
        array.append(p->toJson());

    QJsonDocument doc(array);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool ManagerProjektow::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
        return false;

    QJsonArray array = doc.array();

    for (auto* p : projekty)
        delete p;
    projekty.clear();

    for (const QJsonValue& val : array) {
        QJsonObject obj = val.toObject();
        Project* p = nullptr;

        if (obj["type"].toString() == "TeamProject")
            p = new TeamProject("", {}, "", 0, "", {}, "");
        else
            p = new Project("", {}, "", 0, "");

        p->fromJson(obj);
        projekty.push_back(p);
    }

    return true;
}

QJsonObject Project::toJson() const {
    QJsonObject obj;
    obj["type"] = "Project";
    obj["name"] = QString::fromStdString(name);
    obj["status"] = QString::fromStdString(status);
    obj["workTime"] = workTime;
    obj["repository"] = QString::fromStdString(repositoryLink);

    QJsonArray techs;
    for (const auto& t : technologies)
        techs.append(QString::fromStdString(t));
    obj["technologies"] = techs;

    return obj;
}

void Project::fromJson(const QJsonObject& obj) {
    name = obj["name"].toString().toStdString();
    status = obj["status"].toString().toStdString();
    workTime = static_cast<float>(obj["workTime"].toDouble());
    repositoryLink = obj["repository"].toString().toStdString();

    technologies.clear();
    for (const auto& val : obj["technologies"].toArray())
        technologies.push_back(val.toString().toStdString());
}

QJsonObject TeamProject::toJson() const {
    QJsonObject obj = Project::toJson();
    obj["type"] = "TeamProject";

    QJsonArray collabs;
    for (const auto& c : collaborators)
        collabs.append(QString::fromStdString(c));
    obj["collaborators"] = collabs;
    obj["responsibilities"] = QString::fromStdString(responsibilities);

    return obj;
}

void TeamProject::fromJson(const QJsonObject& obj) {
    Project::fromJson(obj);

    collaborators.clear();
    for (const auto& val : obj["collaborators"].toArray())
        collaborators.push_back(val.toString().toStdString());

    responsibilities = obj["responsibilities"].toString().toStdString();
}
