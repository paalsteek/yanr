#include "feed.h"

#include <QtCore/QDebug>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

Feed::Feed(QString url) :
    QObject(NULL),
    _nam(new QNetworkAccessManager)
{
    _nam->get(QNetworkRequest(QUrl(url)));
    connect(_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseFeed(QNetworkReply*)));
}

void Feed::parseFeed(QNetworkReply *reply)
{
    if ( reply->error() )
        return;

    //QByteArray body = reply->readAll();
    QDomDocument doc;
    doc.setContent(reply);
    QDomElement root = doc.documentElement();
    if ( root.isNull() )
    {
        qDebug() << "Error parsing feed";
        return;
    }
    qDebug() << root.tagName();
    if ( root.tagName() == "feed" )
    {
        _type = FEED_ATOM;
        parseAtom(&doc);
    }
    else if ( root.tagName() == "rss" )
    {
        _type = FEED_RSS;
        qDebug() << "RSS not yet supported";
    }
    else
    {
        _type = FEED_UNKNOWN;
        qDebug() << "Unknown feed format";
    }
    emit updated();
}

void Feed::parseAtom(QDomDocument *doc)
{
    QDomNode titleNode = doc->documentElement().elementsByTagName("title").item(0);
    if ( titleNode.isNull() )
        qDebug() << "No title found!";
    else
    {
        _title = titleNode.firstChild().nodeValue();
        qDebug() << "Title:" << _title;
    }
    QDomNodeList entries = root.elementsByTagName("entry");
    qDebug() << entries.size();
}

QString Feed::getTitle()
{
    return _title;
}

FeedType Feed::getType()
{
    return _type;
}

QString Feed::getTypeString()
{
    switch ( _type )
    {
    case FEED_UNKNOWN:
        return "Unknown";
    case FEED_RSS:
        return "RSS";
    case FEED_ATOM:
        return "Atom";
    }
}
