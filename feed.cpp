#include "feed.h"

#include <QtCore/QDebug>
#include <QtCore/QList>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

Feed::Feed(QString url, QString title, QString type) :
    QObject(NULL),
    _nam(new QNetworkAccessManager),
    _title(title),
    _url(url),
    _type(Feed::stringToType(type)),
    _error(false),
    _errorString("")
{
    if ( title.isEmpty() )
        _nam->get(QNetworkRequest(QUrl(_url)));

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
    else if ( root.tagName() == "html" )
    {
        qDebug() << "HTML found!";
        QDomNodeList linkNodes = doc.documentElement().elementsByTagName("link");
        QList< QPair<QString, QUrl> > feeds;
        for( unsigned int i = 0; i < linkNodes.length(); i++ )
        {
            qDebug() << "link tag found!";
            QDomElement linkElement = linkNodes.item(i).toElement();
            if ( linkElement.attribute("type") == "application/rss+xml" || linkElement.attribute("type") == "application/atom+xml" )
            {
                qDebug() << "feed link tag found!";
                QUrl url(linkElement.attribute("href"));
                if ( url.isRelative() )
                    feeds.append(QPair<QString, QUrl>(linkElement.attribute("title"), QUrl(reply->url().scheme() + "://" + reply->url().host() + url.toString())));
                else
                    feeds.append(QPair<QString, QUrl>(linkElement.attribute("title"), url));
            }
        }
        if ( feeds.length() > 0 )
        {
            qDebug() << "Feeds found:" << feeds;
            _url = feeds.begin()->second.toString();
            qDebug() << "Selected feed:" << _url;
            _nam->get(QNetworkRequest(QUrl(_url)));
            return;
        } else {
            _error = true;
            _errorString = "Not a valid feed.";
        }
    }
    else
    {
        _type = FEED_UNKNOWN;
        _error = true;
        _errorString = "Unknown feed format.";
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
    QDomNodeList entries = doc->elementsByTagName("entry");
    qDebug() << entries.size();
    for ( unsigned int i = 0; i < entries.length(); i++ )
    {
        QDomNode entry = entries.item(i);
        qDebug() << "Entry" << i << "has" << entry.childNodes().length() << "children.";
    }
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
    return Feed::typeToString(_type);
}

QString Feed::getUrl()
{
    return _url;
}

bool Feed::error()
{
    return _error;
}

QString Feed::errorString()
{
    QString ret = "";
    if (_error)
        ret = _errorString;

    return ret;
}

QString Feed::typeToString(FeedType type)
{
    QString result;
    switch ( type )
    {
    case FEED_UNKNOWN:
        result =  "Unknown";
    case FEED_RSS:
        result = "RSS";
    case FEED_ATOM:
        result = "Atom";
    }
    return result;
}

FeedType Feed::stringToType(QString typeString)
{
    FeedType type;
    if ( typeString.compare("RSS") == 0 )
        type = FEED_RSS;
    else if ( typeString.compare("Atom") == 0 )
        type = FEED_ATOM;
    else
        type = FEED_UNKNOWN;

    return type;
}
