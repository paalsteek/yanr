#include "sqlitefeedstorageengine.h"

#include <QDir>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QDebug>

SQLiteFeedStorageEngine::SQLiteFeedStorageEngine()
    : _db()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");

    QString path(QDir::home().path());
    path.append(QDir::separator()).append(".local");
    path.append(QDir::separator()).append("share");
    path.append(QDir::separator()).append("yanr");
    QDir dbdir(path);
    if (!dbdir.exists())
        dbdir.mkpath(path);
    path.append(QDir::separator()).append("feeds.sqlite");
    path = QDir::toNativeSeparators(path);
    _db.setDatabaseName(path);

    _db.open();

    QStringList tables = _db.tables();
    if ( ! tables.contains("meta") )
    {
        QSqlQuery query("CREATE TABLE meta(id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR[30], url CHAR[100]);", _db);
        qDebug() << query.exec() << query.lastError();
    }
}

SQLiteFeedStorageEngine::~SQLiteFeedStorageEngine()
{
    _db.close();
}

int SQLiteFeedStorageEngine::addFeed(Feed* feed)
{
    QSqlQuery query(_db);
    query.prepare("SELECT COUNT(name) FROM meta WHERE name LIKE :name;");
    query.bindValue(":name", feed->getTitle());
    if ( !query.exec() )
    {
        qDebug() << query.lastError();
        return -1;
    }
    while (query.next()) {
        qDebug() << query.value(0);
        if ( query.value(0).toInt() > 0 )
            return -1; //TODO: throw something
    }

    query.prepare("INSERT INTO meta(name, url) VALUES (:name, :url);");
    query.bindValue(":name", feed->getTitle());
    query.bindValue(":url", feed->getUrl());
    if ( !query.exec() )
    {
        qDebug() << query.lastError();
        return -1;
    }

    return query.lastInsertId().toInt();
}
