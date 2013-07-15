#include "sqlitefeedstorageengine.h"

#include <QDir>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

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
        QSqlQuery query("CREATE TABLE meta(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, url TEXT, type INTEGER);", _db);
        qDebug() << query.exec() << query.lastError();
    }

    if ( ! tables.contains("feeds"))
    {
        QSqlQuery query("CREATE TABLE feeds(id TEXT PRIMARY KEY, title TEXT, author TEXT, date INTEGER, url TEXT, summary TEXT, feed INTEGER, FOREIGN KEY(feed) REFERENCES meta(id));");
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
    int feedId = query.lastInsertId().toInt();

    foreach(FeedEntry* e, ((EntryTableModel*) feed->getEntries())->getEntries())
        addEntry(e, feedId);

    connect(feed, SIGNAL(entryAdded(FeedEntry*)), this, SLOT(addEntry(FeedEntry*)));

    return feedId;
}

QList<FeedEntry*>* loadEntriesFromDb(int feedId)
{
    qDebug() << "loading entries for feed" << feedId;
    QSqlQuery query;
    query.prepare("SELECT * FROM feeds WHERE feed = :feedId;");
    query.bindValue(":feedId", feedId);
    if ( !query.exec() )
    {
        qDebug() << query.lastError();
        return NULL;
    }

    QList<FeedEntry*>* entryList = new QList<FeedEntry*>();
    while ( query.next() )
    {
        qDebug() << "new feed entry" << query.value("id").toString();
        FeedEntry* entry = new FeedEntry(query.value("id").toString());
        entry->setTitle(query.value("title").toString());
        entry->setAuthor(query.value("author").toString());
        entry->setDate(QDateTime::fromTime_t(query.value("date").toInt()).date());
        entry->setUrl(query.value("url").toString());
        entry->setSummary(query.value("summary").toString());
        entryList->append(entry);
    }
    return entryList;
}

QList<Feed *> SQLiteFeedStorageEngine::getFeeds()
{
    QList<Feed*> feedlist;
    QSqlQuery query;
    query.exec("SELECT * FROM meta;");
    while ( query.next() )
    {
        Feed* feed = new Feed(query.value("url").toString(), query.value("name").toString(), (FeedType) query.value("type").toInt(), loadEntriesFromDb(query.value("id").toInt()));
        feed->setId(query.value("id").toInt());
        feedlist.append(feed);
        connect(feed, SIGNAL(entryAdded(FeedEntry*)), this, SLOT(addEntry(FeedEntry*)));
    }
    return feedlist;
}

QAbstractTableModel* SQLiteFeedStorageEngine::getItems(int feedId)
{
    qDebug() << "getItems" << feedId;
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(_db);
    query.prepare("SELECT title, date FROM feeds WHERE feed = :feedId;");
    query.bindValue(":feedId", feedId);
    query.exec();
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, "Title");
    model->setHeaderData(1, Qt::Horizontal, "Date");

    return model;
}

void SQLiteFeedStorageEngine::addEntry(FeedEntry *entry, int feedId)
{
    if ( feedId < 0 )
    {
        if ( !sender() )
        {
            qDebug() << "when called directly, feedId has to be passed";
            return;
        } else if (!((Feed*) sender()))
        {
            qDebug() << "sender() must be of type Feed*";
            return;
        } else {
            feedId = ((Feed*) sender())->getId();
        }
    }

    qDebug() << "date:" << QDateTime(entry->getDate()).toTime_t();

    QSqlQuery query(_db);
    query.prepare("INSERT INTO feeds VALUES (:id, :title, :author, :date, :url, :summary, :feed)");
    query.bindValue(":id", entry->getId());
    query.bindValue(":title", entry->getTitle());
    query.bindValue(":author", entry->getAuthor());
    query.bindValue(":date", QDateTime(entry->getDate()).toTime_t());
    query.bindValue(":url", entry->getUrl());
    query.bindValue(":summary", entry->getSummary());
    query.bindValue(":feed", feedId);
    if ( !query.exec() )
    {
        qDebug() << "insert failed";
    }
}
