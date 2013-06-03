#ifndef FEEDSTREEITEM_H
#define FEEDSTREEITEM_H

#include <QList>
#include <QVariant>

#include "feed.h"

class FeedsTreeItem
{
public:
    FeedsTreeItem(const QList<QVariant> &data, FeedsTreeItem *parent = 0);

    void appendChild(FeedsTreeItem *child);

    FeedsTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setFeed(Feed* feed);
    bool hasFeed();
    Feed* getFeed();
    int row() const;
    FeedsTreeItem *parent();

private:
    QList<FeedsTreeItem*> _childItems;
    QList<QVariant> _itemData;
    Feed *_feed;
    FeedsTreeItem *_parentItem;
};

#endif // FEEDSTREEITEM_H
