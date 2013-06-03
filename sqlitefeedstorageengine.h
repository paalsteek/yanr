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

private:
    QSqlDatabase _db;
};

#endif // SQLITEFEEDSTORAGEENGINE_H
