#ifndef URLPAGE_H
#define URLPAGE_H

#include <QWizardPage>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include <QtNetwork/QNetworkAccessManager>

namespace Ui {
class UrlPage;
}

class UrlPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit UrlPage(QWidget *parent = 0);
    ~UrlPage();
    virtual bool isComplete() const;

protected slots:
    void checkUrl(QString url);
    void authRequired(QNetworkReply *reply, QAuthenticator *auth);
    void sslError(QNetworkReply *reply,QList<QSslError> errors);
    void checkFinished(QNetworkReply*);

signals:
    void cancelRequests();
    
private:
    Ui::UrlPage *_ui;
    bool _complete;
    QNetworkAccessManager *_nam;

    static QString alphaSym;
    static QString digitSym;
    static QString hexdigExp;
    static QString unreservedExp;
    static QString subdelimsExp;
    static QString gendelimsExp;
    static QString reservedExp;
    static QString pctencodedExp;
    static QString pcharExp;
    static QString fragmentExp;
    static QString queryExp;
    static QString segmentExp;
    static QString segmentnzExp;
    static QString segmentnzncExp;
    static QString pathemptyExp;
    static QString pathrootlessExp;
    static QString pathnoschemeExp;
    static QString pathabsoluteExp;
    static QString pathabemptyExp;
    static QString pathExp;
    static QString portExp;
    static QString regnameExp;
    static QString decoctetExp;
    static QString ipv4addrExp;
    static QString h16Exp;
    static QString ls32Exp;
    static QString ipv6addrExp;
    static QString ipvfutureExp;
    static QString ipliteralExp;
    static QString hostExp;
    static QString userinfoExp;
    static QString authorityExp;
    static QString schemeExp;
    static QString hierpartExp;
    static QString uriExp;
    void testURIExp();
};

#endif // URLPAGE_H
