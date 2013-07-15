#ifndef SQLITEFEEDSTORAGEENGINE_H
#define SQLITEFEEDSTORAGEENGINE_H

#include <QMap>
#include <QtSql/QSqlDatabase>

#include "feedstorageengine.h"

class SQLiteFeedStorageEngine : public FeedStorageEngine
{
    Q_OBJECT

public:
    SQLiteFeedStorageEngine();
    ~SQLiteFeedStorageEngine();
    virtual int addFeed(Feed* feed);
    virtual QList<Feed*> getFeeds();
    virtual QAbstractTableModel* getItems(int feedId);

public slots:
    virtual void addEntry(FeedEntry* entry, int feedId = -1);

private:
    QSqlDatabase _db;
};

#endif // SQLITEFEEDSTORAGEENGINE_H
