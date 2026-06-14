#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool connect(const QString& host, const QString& dbName,
                 const QString& user, const QString& password,
                 int port = 5432);

    void disconnect();

    QSqlDatabase database() const { return m_db; }
    bool isOpen() const { return m_db.isOpen(); }
    QString lastError() const { return m_db.lastError().text(); }

private:
    DatabaseManager() = default;
    ~DatabaseManager() { disconnect(); }

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
