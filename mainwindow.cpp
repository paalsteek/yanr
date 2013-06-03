#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "feedwizard.h"
#include "feedstreemodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _storage(new SQLiteFeedStorageEngine())
{
    _ui->setupUi(this);

    FeedsTreeModel *model = new FeedsTreeModel(this);

    QList<Feed*> feeds = _storage->getFeeds();

    foreach(Feed* feed, feeds)
    {
        model->addFeed(feed);
    }

    _ui->treeView->setModel(model);

    connect(_ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(_ui->actionAdd_Feed, SIGNAL(triggered()), this, SLOT(addFeed()));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::addFeed()
{
    FeedWizard *wizard = new FeedWizard();

    connect(wizard, SIGNAL(accepted()), this, SLOT(newFeed()));
    wizard->show();
}

void MainWindow::newFeed()
{
    Feed* feed = ((FeedWizard*) sender())->getFeed();

    FeedsTreeModel *model = static_cast<FeedsTreeModel*>(_ui->treeView->model());

    int res = _storage->addFeed(feed);
    qDebug() << "addFeed:" << res;
    if ( res >= 0 )
    {
        /*QModelIndex index =*/ model->addFeed(feed);
        _ui->treeView->reset();
    }
}
