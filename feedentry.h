#ifndef FEEDENTRY_H
#define FEEDENTRY_H

#include <QtCore/QString>
#include <QtCore/QDate>

class FeedEntry
{
public:
    FeedEntry(QString id = "");
    QString getId();
    QString getTitle();
    void setTitle(QString title);
    QString getUrl();
    void setUrl(QString url);
    QDate getDate();
    void setDate(QDate date);
    QString getAuthor();
    void setAuthor(QString author);
    QString getSummary();
    void setSummary(QString summary);
    void setRead(bool read);
    bool isRead();

private:
    QString _id;
    QString _title;
    QString _url;
    QDate _date;
    QString _author;
    QString _summary;
    bool _read;
};

Q_DECLARE_METATYPE(FeedEntry)

#endif // FEEDENTRY_H
