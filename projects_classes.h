#ifndef PROJECTS_CLASSES_H
#define PROJECT_CLASSES_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Project {
protected:
    string name;
    vector<string> technologies;
    string status;
    float workTime;
    string repositoryLink;

public:
    // Konstruktor
    Project(const string& name,
        const vector<string>& technologies,
        const string& status,
        float workTime,
        const string& repositoryLink);

    // Gettery
    string getName() const;
    vector< string> getTechnologies() const;
    string getStatus() const;
    float getWorkTime() const;
    string getRepositoryLink() const;

    // Settery
    void setName(const string& name);
    void setTechnologies(const vector<string>& technologies);
    void setStatus(const string& status);
    void setWorkTime(float time);
    void setRepositoryLink(const string& link);

    // Pozosta³e funkcje
    void printInfo() const;

};

class TeamProject : public Project {
private:
    vector<string> collaborators;
    string responsibilities;

public:
    // Konstruktor
    TeamProject(const string& name,
        const vector<string>& technologies,
        const string& status,
        float workTime,
        const string& repositoryLink,
        const vector<string>& collaborators,
        const string& responsibilities);

    // Gettery
    vector<string> getCollaborators() const;
    string getResponsibilities() const;

    // Settery
    void setCollaborators(const vector<string>& collaborators);
    void setResponsibilities(const string& responsibilities);


};


#endif // PROJECTS_CLASSES_H

