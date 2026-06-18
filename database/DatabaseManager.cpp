#include "DatabaseManager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager manager;
    return manager;
}

bool DatabaseManager::connect(const QString& host, const QString& dbName,
                              const QString& user, const QString& password,
                              int port)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    // ВНИМАНИЕ: здесь мы принудительно задаём имя с кавычками, чтобы сохранить регистр
    m_db.setDatabaseName("ElectiveCoursesDB");   // ← заменили dbName на эту строку
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (!m_db.open()) {
        qCritical() << "Ошибка подключения:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "Подключение к БД успешно!";
    return true;
}

void DatabaseManager::disconnect()
{
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Соединение закрыто.";
    }
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}