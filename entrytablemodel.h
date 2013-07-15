#ifndef ENTRYTABLEMODEL_H
#define ENTRYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QHash>

#include "feedentry.h"

class EntryTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit EntryTableModel(QObject *parent = 0, const QList<FeedEntry*>* entries = 0);
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    void addEntries(QList<FeedEntry*>* entries);
    QList<FeedEntry *> getEntries();
    
signals:
    
public slots:
    void markEntryRead(const QModelIndex &index);
    void markEntryUnread(const QModelIndex &index);
    
private:
    QHash<QString, FeedEntry*> _entries;
};

#endif // ENTRYTABLEMODEL_H
