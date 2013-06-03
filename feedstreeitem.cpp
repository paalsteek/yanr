#include "feedstreeitem.h"

FeedsTreeItem::FeedsTreeItem(const QList<QVariant> &data, FeedsTreeItem *parent)
    : _childItems()
    , _itemData(data)
    , _feed(NULL)
    , _parentItem(parent)
{
}

void FeedsTreeItem::appendChild(FeedsTreeItem *child)
{
    _childItems.append(child);
}

FeedsTreeItem* FeedsTreeItem::child(int row)
{
    return _childItems.value(row);
}

int FeedsTreeItem::childCount() const
{
    return _childItems.count();
}

int FeedsTreeItem::columnCount() const
{
    return _itemData.count();
}

QVariant FeedsTreeItem::data(int column) const
{
    return _itemData.value(column);
}

void FeedsTreeItem::setFeed(Feed *feed)
{
    _feed = feed;
}

bool FeedsTreeItem::hasFeed()
{
    return (_feed != NULL);
}

Feed* FeedsTreeItem::getFeed()
{
    return _feed;
}

int FeedsTreeItem::row() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<FeedsTreeItem*>(this));

    return 0;
}

FeedsTreeItem* FeedsTreeItem::parent()
{
    return _parentItem;
}
