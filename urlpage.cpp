#include "urlpage.h"
#include "ui_urlpage.h"

#include <QtCore/QDebug>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

UrlPage::UrlPage(QWidget *parent) :
    QWizardPage(parent),
    _ui(new Ui::UrlPage),
    _complete(false)
{
    _ui->setupUi(this);

    connect(_ui->urlEdit, SIGNAL(textEdited(QString)), this, SLOT(checkUrl(QString)));

    registerField("url", _ui->urlEdit);
}

UrlPage::~UrlPage()
{
    delete _ui;
}

bool UrlPage::isComplete() const
{
    qDebug() << "isComplete";
    return _complete;
}

void UrlPage::checkUrl(QString url)
{
    _complete = false;
    this->completeChanged();
    qDebug() << "Checking" << url;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->head(QNetworkRequest(QUrl(url)));
    connect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authRequired(QNetworkReply*,QAuthenticator*)));
    connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslError(QNetworkReply*,QList<QSslError>)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(checkFinished(QNetworkReply*)));
}

void UrlPage::authRequired(QNetworkReply *reply, QAuthenticator *auth)
{
    qDebug() << "authRequired for" << reply->url();
}

void UrlPage::sslError(QNetworkReply *reply, QList<QSslError> errors)
{
    qDebug() << "sslError while checking" << reply->url();
    for ( QList<QSslError>::Iterator it = errors.begin(); it != errors.end(); it++)
    {
        qDebug() << it->errorString();
    }
}

void UrlPage::checkFinished(QNetworkReply *reply)
{
    qDebug() << "checkFinished";
    if ( reply->error() )
        qDebug() << "Check failed:" << "GET" << reply->url().toString() << ":" << reply->errorString();
    else
    {
        qDebug() << "Check successful:" << "GET" << reply->url().toString() << ":" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        _complete = true;
        this->completeChanged();
    }
}
