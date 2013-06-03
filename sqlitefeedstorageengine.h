#ifndef SQLITEFEEDSTORAGEENGINE_H
#define SQLITEFEEDSTORAGEENGINE_H

#include <QMap>
#include <QtSql/QSqlDatabase>

#include "feedstorageengine.h"

class SQLiteFeedStorageEngine : public FeedStorageEngine
{
public:
    SQLiteFeedStorageEngine();
    ~SQLiteFeedStorageEngine();
    virtual int addFeed(Feed* feed);
    virtual QList<Feed*> getFeeds();

private:
    QSqlDatabase _db;
};

#endif // SQLITEFEEDSTORAGEENGINE_H
