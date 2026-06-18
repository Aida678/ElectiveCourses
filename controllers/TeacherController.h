#ifndef TEACHERCONTROLLER_H
#define TEACHERCONTROLLER_H

#include <QObject>
#include "repositories/TeacherRepository.h"
#include "models/Teacher.h"

class MainWindow; // forward declaration

class TeacherController : public QObject
{
    Q_OBJECT

public:
    explicit TeacherController(MainWindow* view, QObject *parent = nullptr);

public slots:
    void loadTeachers();
    void addTeacher(const Teacher& teacher);
    void updateTeacher(const Teacher& teacher);
    void deleteTeacher(int id);
    void selectTeacher(int row);

private:
    MainWindow* m_view;
    TeacherRepository m_repo;
    QList<Teacher> m_teachers;
};

#endif // TEACHERCONTROLLER_H
