//tetsc do gita
#include "projects_classes.h"

int main() {
  
    // Na razie dla sprawdzenia czy podstawa dziala
    Project p1(
        "System CRM",
        { "C++", "SQLite", "Qt" },
        "W trakcie",
        120.5,
        "https://github.com/uzytkownik/crm"
    );

    TeamProject tp1(
        "System rezerwacji",
        { "C++", "MySQL", "Qt" },
        "Zakończony",
        250.0,
        "https://github.com/uzytkownik/rezerwacje",
        { "Anna Kowalska", "Jan Nowak", "Tomasz Zieliński" },
        "Frontend, komunikacja z bazą danych"
    );

 
    cout << "\n--- Informacje o projekcie indywidualnym ---\n";
    p1.printInfo();

    cout << "\n--- Informacje o projekcie zespołowym ---\n";
    tp1.printInfo();

    return 0;
}
