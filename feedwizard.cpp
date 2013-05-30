#include "feedwizard.h"
#include "ui_feedwizard.h"

#include <QDebug>

FeedWizard::FeedWizard(QWidget *parent) :
    QWizard(parent),
    _ui(new Ui::FeedWizard)
{
    _ui->setupUi(this);
}

FeedWizard::~FeedWizard()
{
    delete _ui;
}
