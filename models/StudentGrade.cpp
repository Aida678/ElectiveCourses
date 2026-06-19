#include "StudentGrade.h"

StudentGrade::StudentGrade()
    : m_studentId(0), m_finalGrade(0)
{
}

StudentGrade::StudentGrade(int studentId, const QString& lastName, const QString& firstName,
                           const QString& middleName, int finalGrade)
    : m_studentId(studentId)
    , m_lastName(lastName)
    , m_firstName(firstName)
    , m_middleName(middleName)
    , m_finalGrade(finalGrade)
{
}

QString StudentGrade::fullName() const
{
    return m_lastName + " " + m_firstName + " " + m_middleName;
}