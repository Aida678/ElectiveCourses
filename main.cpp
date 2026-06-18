#include "views/LoginWindow.h"
#include "database/DatabaseManager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager::instance().connect("127.0.0.1", "ElectiveCoursesDB", "postgres", "Haroro14", 5433);

    LoginWindow login;
    login.show();
    return a.exec();
}
