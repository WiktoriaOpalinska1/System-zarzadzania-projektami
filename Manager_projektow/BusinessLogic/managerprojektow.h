#ifndef MANAGERPROJEKTOW_H
#define MANAGERPROJEKTOW_H

#include <qobject.h>
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
    Project(const string& name,
            const vector<string>& technologies,
            const string& status,
            float workTime,
            const string& repositoryLink);

    string getName() const;
    vector< string> getTechnologies() const;
    string getStatus() const;
    float getWorkTime() const;
    string getRepositoryLink() const;

    void setName(const string& name);
    void setTechnologies(const vector<string>& technologies);
    void setStatus(const string& status);
    void setWorkTime(float time);
    void setRepositoryLink(const string& link);

    void printInfo() const;
    string toString() const;
    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject& obj);
    virtual bool isTeamProject() const { return false; }

    virtual ~Project() = default;
};

class TeamProject : public Project {
private:
    vector<string> collaborators;
    string responsibilities;

public:
    TeamProject(const string& name,
                const vector<string>& technologies,
                const string& status,
                float workTime,
                const string& repositoryLink,
                const vector<string>& collaborators,
                const string& responsibilities);

    const vector<string>& getCollaborators() const;
    const string& getResponsibilities() const;

    void setCollaborators(const vector<string>& collaborators);
    void setResponsibilities(const string& responsibilities);

    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject& obj);
    bool isTeamProject() const override { return true; }
};

class ManagerProjektow {
private:
    vector<Project*> projekty;
    vector<string> contributors;
    string responsibilities;

public:
    ManagerProjektow();

    const vector<Project*>& getProjekty() const;
    void addProject(Project* p);
    void removeProject(Project* p);
    bool saveToFile(const QString& filename) const;
    bool loadFromFile(const QString& filename);

    ~ManagerProjektow();
};

#endif // MANAGERPROJEKTOW_H
