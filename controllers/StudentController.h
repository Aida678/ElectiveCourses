#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include <QObject>
#include "repositories/StudentRepository.h"

class MainWindow;

class StudentController : public QObject
{
    Q_OBJECT

public:
    explicit StudentController(MainWindow* view, QObject *parent = nullptr);

public slots:
    void loadStudents();
    void addStudent(const Student& student);
    void updateStudent(const Student& student);
    void deleteStudent(int id);
    void selectStudent(int row);

private:
    MainWindow* m_view;
    StudentRepository m_repo;
    QList<Student> m_students;
};

#endif // STUDENTCONTROLLER_H
