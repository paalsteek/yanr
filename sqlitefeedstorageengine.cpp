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
        QSqlQuery query("CREATE TABLE meta(id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR[30], url CHAR[100], type INTEGER);", _db);
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
    query.prepare("SELECT COUNT(*) FROM meta WHERE name LIKE :name;");
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

    query.prepare("INSERT INTO meta(name, url, type) VALUES (:name, :url, :type);");
    query.bindValue(":name", feed->getTitle());
    query.bindValue(":url", feed->getUrl());
    query.bindValue(":type", feed->getType());
    if ( !query.exec() )
    {
        qDebug() << query.lastError();
        return -1;
    }

    return query.lastInsertId().toInt();
}

QList<Feed*> SQLiteFeedStorageEngine::getFeeds()
{
    QList<Feed*> feedlist;
    QSqlQuery query;
    query.exec("SELECT * FROM meta;");
    while ( query.next() )
    {
        Feed* feed = new Feed(query.value("url").toString(), query.value("name").toString(), (FeedType) query.value("type").toInt());
        feedlist.append(feed);
    }
    return feedlist;
}
