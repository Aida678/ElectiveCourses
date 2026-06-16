#include "UserRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>

UserRepository::UserRepository()
{
}

User UserRepository::findById(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return User();

    QSqlQuery query(db);
    query.prepare("SELECT id, email, password_hash FROM users WHERE id = ?");
    query.bindValue(0, id);

    if (query.exec() && query.next()) {
        return User::fromRecord(query.record());
    }
    return User();
}

User UserRepository::findByEmail(const QString& email)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return User();

    QSqlQuery query(db);
    query.prepare("SELECT id, email, password_hash FROM users WHERE email = ?");
    query.bindValue(0, email);

    if (query.exec() && query.next()) {
        return User::fromRecord(query.record());
    }
    return User();
}

int UserRepository::insert(const User& user)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (email, password_hash) VALUES (?, ?) RETURNING id");
    query.bindValue(0, user.email());
    query.bindValue(1, user.passwordHash());

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool UserRepository::update(const User& user)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen() || user.id() == 0) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE users SET email=?, password_hash=? WHERE id=?");
    query.bindValue(0, user.email());
    query.bindValue(1, user.passwordHash());
    query.bindValue(2, user.id());

    return query.exec();
}

bool UserRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE id = ?");
    query.bindValue(0, id);
    return query.exec();
}

bool UserRepository::checkPassword(const QString& email, const QString& password)
{
    User user = findByEmail(email);
    if (user.id() == 0) return false;

    QString hash = QString::fromUtf8(
        QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()
        );

    return user.passwordHash() == hash;
}
