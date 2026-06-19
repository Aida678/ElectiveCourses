#ifndef ELECTIVECONTROLLER_H
#define ELECTIVECONTROLLER_H

#include <QObject>
#include "repositories/ElectiveRepository.h"
#include "models/Elective.h"
#include "models/StudentGrade.h"

class MainWindow;
class AuthController;

class ElectiveController : public QObject
{
    Q_OBJECT

public:
    explicit ElectiveController(MainWindow* view, AuthController* auth, QObject *parent = nullptr);

public slots:
    void loadElectives();
    void addElective(const Elective& elective);
    void updateElective(const Elective& elective);
    void deleteElective(int id);
    void selectElective(int row);

    QList<Elective> getAllElectives() const;
    QList<StudentGrade> getStudentGradesForElective(int electiveId);

private:
    MainWindow* m_view;
    AuthController* m_auth;
    ElectiveRepository m_repo;
    QList<Elective> m_electives;
};

#endif // ELECTIVECONTROLLER_H