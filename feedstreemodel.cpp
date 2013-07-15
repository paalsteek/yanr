#include "feedstreemodel.h"

#include <QDebug>

FeedsTreeModel::FeedsTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Title" << "Unread" << "Total";
    _rootItem = new FeedsTreeItem(rootData);
}

FeedsTreeModel::~FeedsTreeModel()
{
    delete _rootItem;
}

QModelIndex FeedsTreeModel::addFeed(Feed *feed)
{
    QList<QVariant> itemData;
    itemData << feed->getTitle() << feed->getUnreadCount() << feed->getTotalCount();
    FeedsTreeItem *feedItem = new FeedsTreeItem(itemData, _rootItem);
    feedItem->setFeed(feed);
    _rootItem->appendChild(feedItem);
    return createIndex(_rootItem->childCount()-1, 0, feedItem);
}

QVariant FeedsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    FeedsTreeItem *item = static_cast<FeedsTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags FeedsTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant FeedsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex FeedsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FeedsTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<FeedsTreeItem*>(parent.internalPointer());

    FeedsTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FeedsTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FeedsTreeItem *childItem = static_cast<FeedsTreeItem*>(index.internalPointer());
    FeedsTreeItem *parentItem = childItem->parent();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FeedsTreeModel::rowCount(const QModelIndex &parent) const
{
    FeedsTreeItem *parentItem;

    if ( parent.column() > 0 )
        return 0;

    if ( !parent.isValid() )
        parentItem = _rootItem;
    else
        parentItem = static_cast<FeedsTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FeedsTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<FeedsTreeItem*>(parent.internalPointer())->columnCount();
    else
        return _rootItem->columnCount();
}
