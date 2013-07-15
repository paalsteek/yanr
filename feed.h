#ifndef FEED_H
#define FEED_H

#include <QtCore/QString>

#include <QtNetwork/QNetworkAccessManager>

#include <QtXml/QDomDocument>

#include <QAbstractTableModel>

#include "feedentry.h"
#include "entrytablemodel.h"

enum {
    FEED_UNKNOWN = 0,
    FEED_RSS,
    FEED_ATOM
} typedef FeedType;

//TODO: refactor to a feeds superclass and seperate subclasses for the different feed types
//this will probably introduce a factory class
class Feed : public
        QObject
{
    Q_OBJECT

public:
    Feed(QString url, QString title = "", FeedType type = FEED_UNKNOWN, QList<FeedEntry *>* entries = 0);
    void setId(int feedId);
    int getId();
    QString getTitle();
    QString getUrl();
    FeedType getType();
    QAbstractTableModel* getEntries();
    int getTotalCount();
    int getUnreadCount();

    static QString typeToString(FeedType type);
    static FeedType stringToType(QString typeString) __attribute__ ((deprecated));

    bool error();
    QString errorString();

signals:
    void updated();
    void entryAdded(FeedEntry*);
    void cancelRequests();

public slots:
    void abort();

private slots:
    void parseFeed(QNetworkReply *reply);

private:
    void parseAtom(QDomDocument *doc);
    QNetworkAccessManager *_nam;
    QString _title;
    QString _url;
    FeedType _type;
    EntryTableModel* _entries;
    bool _error;
    QString _errorString;
    int _id;
};

#endif // FEED_H
