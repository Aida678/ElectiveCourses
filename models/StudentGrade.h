#ifndef STUDENTGRADE_H
#define STUDENTGRADE_H

#include <QString>

class StudentGrade
{
public:
    StudentGrade();
    StudentGrade(int studentId, const QString& lastName, const QString& firstName,
                 const QString& middleName, int finalGrade);

    int studentId() const { return m_studentId; }
    QString fullName() const;
    int finalGrade() const { return m_finalGrade; }

private:
    int m_studentId;
    QString m_lastName;
    QString m_firstName;
    QString m_middleName;
    int m_finalGrade;
};

#endif // STUDENTGRADE_H