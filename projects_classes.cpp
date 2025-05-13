#include "projects_classes.h"

// ============ Projects ============
// Konstruktor
Project::Project(const string& name, const vector<string>& technologies, const string& status,
                float workTime, const string& repositoryLink) {
    this->name = name;
    this->technologies = technologies;
    this->status = status;
    this->workTime = workTime;
    this->repositoryLink = repositoryLink;
}

// Gettery
string Project::getName() const {
    return name;
}

vector<string> Project::getTechnologies() const {
    return technologies;
}

string Project::getStatus() const {
    return status;
}

float Project::getWorkTime() const {
    return workTime;
}

string Project::getRepositoryLink() const {
    return repositoryLink;
}

// Settery
void Project::setName(const string& name) {
    this->name = name;
}

void Project::setTechnologies(const vector<string>& technologies) {
    this->technologies = technologies;
}

void Project::setStatus(const string& status) {
    this->status = status;
}

void Project::setWorkTime(float time) {
    this->workTime = time;
}

void Project::setRepositoryLink(const string& link) {
    this->repositoryLink = link;
}

// Pozostale funkcje
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

// ============ Team Projects ============
// Konstruktor
TeamProject::TeamProject(const string& name, const vector<string>& technologies, const string& status, float workTime,
    const string& repositoryLink, const vector<string>& collaborators, const string& responsibilities)
    : Project(name, technologies, status, workTime, repositoryLink){
    this->collaborators = collaborators;
    this->responsibilities = responsibilities;
}

// Gettery
vector<string> TeamProject::getCollaborators() const {
    return collaborators;
}

string TeamProject::getResponsibilities() const {
    return responsibilities;
}

// Settery
void TeamProject::setCollaborators(const vector<string>& collaborators) {
    this->collaborators = collaborators;
}

void TeamProject::setResponsibilities(const string& responsibilities) {
    this->responsibilities = responsibilities;
}