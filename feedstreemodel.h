#ifndef FEEDSTREEMODEL_H
#define FEEDSTREEMODEL_H

#include <QAbstractItemModel>

#include "feedstreeitem.h"

class FeedsTreeModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    FeedsTreeModel(QObject* parent);
    ~FeedsTreeModel();

    QModelIndex addFeed(Feed *feed);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    FeedsTreeItem* _rootItem;
};

#endif // FEEDSTREEMODEL_H
