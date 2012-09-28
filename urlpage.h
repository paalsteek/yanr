#ifndef URLPAGE_H
#define URLPAGE_H

#include <QWizardPage>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>

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
    
private:
    Ui::UrlPage *_ui;
    bool _complete;
};

#endif // URLPAGE_H
