#include "entrytablemodel.h"

#include <QDebug>

#include <QFont>
#include <QSize>

EntryTableModel::EntryTableModel(QObject *parent, const QList<FeedEntry *> *entries)
    : QAbstractTableModel(parent)
{
    if ( entries )
    {
        //qDebug() << "SegFault:" << entries;
        foreach(FeedEntry* e, *entries)
        {
            _entries.insert(e->getId(), e);
        }
    }
}

int EntryTableModel::rowCount(const QModelIndex &parent) const
{
    //qDebug() << "rowCount" << _entries.size();
    return _entries.size();
}

int EntryTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant EntryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant::Invalid;
    //qDebug() << "data(" << index << "," << role << ")";
    if ( role == Qt::DisplayRole )
    {
        if (index.column() == 0)
            return _entries.values()[index.row()]->getTitle();
        else
            return _entries.values()[index.row()]->getDate();
    } else if ( role == Qt::FontRole )
    {
        QFont font;
        font.setBold(!_entries.values()[index.row()]->isRead());
        return font;
    } else if ( role == 255 )
    {
        return QVariant::fromValue(*(_entries.values()[index.row()]));
    }
    return QVariant();
}

QVariant EntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //qDebug() << "headerData(" << section << "," << orientation << "," << role << ")";
    if( role == Qt::DisplayRole )
    {
        if ( orientation == Qt::Horizontal )
            if ( section == 0 )
                return "Title";
            else
                return "Date";
        else
            return section + 1;
    }
    return QVariant();

}

Qt::ItemFlags EntryTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void EntryTableModel::addEntries(QList<FeedEntry *> *entries)
{
    foreach(FeedEntry* e, *entries)
    {
        qDebug() << "addEntries:" << e->getId();
        _entries.insert(e->getId(), e);
    }
}

QList<FeedEntry*> EntryTableModel::getEntries()
{
    return _entries.values();
}

void EntryTableModel::markEntryRead(const QModelIndex &index)
{
    _entries.values()[index.row()]->setRead(true);
}

void EntryTableModel::markEntryUnread(const QModelIndex &index)
{
    _entries.values()[index.row()]->setRead(false);
}
