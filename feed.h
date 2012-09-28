#ifndef FEED_H
#define FEED_H

#include <QtCore/QString>

#include <QtNetwork/QNetworkAccessManager>

#include <QtXml/QDomDocument>

enum {
    FEED_UNKNOWN = 0,
    FEED_RSS,
    FEED_ATOM
} typedef FeedType;

class Feed : public
        QObject
{
    Q_OBJECT

public:
    Feed(QString url);
    QString getTitle();
    FeedType getType();
    QString getTypeString();

signals:
    void updated();

private slots:
    void parseFeed(QNetworkReply *reply);

private:
    void parseAtom(QDomDocument *doc);
    QNetworkAccessManager *_nam;
    QString _title;
    FeedType _type;
};

#endif // FEED_H
