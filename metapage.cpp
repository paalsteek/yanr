#include "metapage.h"
#include "ui_metapage.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

MetaPage::MetaPage(QWidget *parent) :
    QWizardPage(parent),
    _ui(new Ui::MetaPage),
    _feed(NULL)
{
    _ui->setupUi(this);
}

void MetaPage::initializePage()
{
    _ui->urlEdit->setText(wizard()->field("url").toString());

    _feed = new Feed(_ui->urlEdit->text());

    connect(_feed, SIGNAL(updated()), this, SLOT(fillForm()));
}

MetaPage::~MetaPage()
{
    delete _ui;
}

void MetaPage::fillForm()
{
    _ui->nameEdit->setText(_feed->getTitle());
    _ui->typeEdit->setText(_feed->getTypeString());
}
