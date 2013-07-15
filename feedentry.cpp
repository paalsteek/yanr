#include "feedentry.h"

FeedEntry::FeedEntry(QString id)
    : _id(id)
    , _read(false)
{
}

QString FeedEntry::getId()
{
    return _id;
}

QString FeedEntry::getTitle()
{
    return _title;
}

void FeedEntry::setTitle(QString title)
{
    _title = title;
}

QString FeedEntry::getUrl()
{
    return _url;
}

void FeedEntry::setUrl(QString url)
{
    _url = url;
}

QDate FeedEntry::getDate()
{
    return _date;
}

void FeedEntry::setDate(QDate date)
{
    _date = date;
}

QString FeedEntry::getAuthor()
{
    return _author;
}

void FeedEntry::setAuthor(QString author)
{
    _author = author;
}

QString FeedEntry::getSummary()
{
    return _summary;
}

void FeedEntry::setSummary(QString summary)
{
    _summary = summary;
}

bool FeedEntry::isRead()
{
    return _read;
}

void FeedEntry::setRead(bool read)
{
    _read = read;
}
