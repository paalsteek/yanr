#include "metapage.h"
#include "ui_metapage.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QDebug>

#include "feedwizard.h"

MetaPage::MetaPage(QWidget *parent) :
    QWizardPage(parent),
    _ui(new Ui::MetaPage),
    _feed(NULL),
    _complete(false)
{
    _ui->setupUi(this);
}

void MetaPage::initializePage()
{
    QString url = wizard()->field("url").toString();
    QRegExp srx("^(http|https|ftp)://.*$");
    if ( !srx.exactMatch(url) )
        url = "http://" + url;

    _ui->urlEdit->setText(url);
    _ui->nameEdit->setText("");
    _ui->nameEdit->setEnabled(false);
    _ui->typeEdit->setText("");
    _ui->typeEdit->setEnabled(false);

    _complete = false;

    _feed = new Feed(_ui->urlEdit->text());
    ((FeedWizard*) this->wizard())->setFeed(_feed);

    connect(_feed, SIGNAL(updated()), this, SLOT(fillForm()));
}

bool MetaPage::isComplete() const
{
    return _complete;
}

MetaPage::~MetaPage()
{
    delete _ui;
}

void MetaPage::fillForm()
{
    if ( sender() && ((Feed*) sender())->error() )
    {
        qDebug() << "Error in Feed:" << ((Feed*) sender())->errorString();
    } else {
        _ui->nameEdit->setText(_feed->getTitle());
        _ui->nameEdit->setEnabled(true);
        _ui->typeEdit->setText(Feed::typeToString(_feed->getType()));
        _ui->typeEdit->setEnabled(true);
        ((FeedWizard*) this->wizard())->setFeed(_feed);
        _complete = true;
        emit completeChanged();
    }
}
