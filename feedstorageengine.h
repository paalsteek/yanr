#ifndef FEEDSTORAGEENGINE_H
#define FEEDSTORAGEENGINE_H

#include "feed.h"

#include <QAbstractTableModel>

class FeedStorageEngine : public QObject
{
    Q_OBJECT

public:
    virtual int addFeed(Feed* feed) = 0;
    virtual QList<Feed*> getFeeds() = 0;
    virtual QAbstractTableModel* getItems(int feedId) = 0;

public slots:
    virtual void addEntry(FeedEntry* entry, int feedId = -1) = 0;
};

#endif // FEEDSTORAGEENGINE_H
