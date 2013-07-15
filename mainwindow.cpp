#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include <QSortFilterProxyModel>

#include "feedwizard.h"
#include "feedstreemodel.h"

#include "entrytablemodel.h"

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
    connect(_ui->actionUpdate_All_Feeds, SIGNAL(triggered()), this, SLOT(updateFeeds()));
    connect(_ui->actionMark_feed_as_read, SIGNAL(triggered()), this, SLOT(markSelectedFeedsRead()));
    connect(_ui->actionMark_feed_as_unread, SIGNAL(triggered()), this, SLOT(markSelectedFeedsUnread()));
    connect(_ui->actionToggle_entries_read, SIGNAL(triggered()), this, SLOT(toggleSelectedEntriesRead()));
    connect(_ui->actionMark_feed_as_read, SIGNAL(triggered()), this, SLOT(markSelectedFeedsRead()));
    connect(_ui->treeView, SIGNAL(activated(QModelIndex)), this, SLOT(openFeed(QModelIndex)));
    connect(_ui->treeView, SIGNAL(activated(QModelIndex)), this, SLOT(enableUpdateFeed()));
    connect(_ui->tableView, SIGNAL(activated(QModelIndex)), this, SLOT(openEntry(QModelIndex)));
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
    feed->setId(res);
    qDebug() << "addFeed:" << res;
    if ( res >= 0 )
    {
        /*QModelIndex index =*/ model->addFeed(feed);
        _ui->treeView->reset();
    }
}

void MainWindow::openFeed(QModelIndex index)
{
    FeedsTreeItem* item = static_cast<FeedsTreeItem*>(index.internalPointer());

    int feedId = item->getFeed()->getId();

    if (feedId > 0)
    {
        QAbstractTableModel* m = item->getFeed()->getEntries();
        //_ui->tableView->setModel(_storage->getItems(feedId)); //TODO: feed->getEntries();
        QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
        proxy->setSourceModel(m);
        _ui->tableView->setModel(proxy);
    }
    else
        qDebug() << "Feed returned invalid Id";
}

void MainWindow::openEntry(QModelIndex index)
{
    FeedEntry entry = _ui->tableView->model()->data(index, 255).value<FeedEntry>();

    qDebug() << "Summary:" << entry.getSummary();
    _ui->textBrowser->setText(entry.getSummary());
}

void MainWindow::updateFeeds()
{
    qDebug() << _ui->treeView->model()->rowCount() << "x" << _ui->treeView->model()->columnCount();
    for ( int x = 0; x < _ui->treeView->model()->rowCount(); x++ )
    {
        FeedsTreeItem* item;
        if ( (item = ((FeedsTreeItem*) _ui->treeView->model()->index(x, 0).internalPointer())) )
        {
            QNetworkAccessManager* nam = new QNetworkAccessManager();
            QNetworkRequest req(QUrl(item->getFeed()->getUrl()));
            nam->get(req);
            connect(nam, SIGNAL(finished(QNetworkReply*)), item->getFeed(), SLOT(parseFeed(QNetworkReply*)) );
        }
    }
}

void MainWindow::enableUpdateFeed()
{
    _ui->actionUpdate_Feed->setEnabled(true);
}

void MainWindow::markSelectedEntriesRead()
{
    qDebug() << "abc";
}

void MainWindow::markSelectedEntriesUnread()
{
    qDebug() << "123";
}

void MainWindow::markSelectedFeedsRead()
{
    qDebug() << "fabc";
}

void MainWindow::markSelectedFeedsUnread()
{
    qDebug() << "f123";
}

void MainWindow::toggleSelectedEntriesRead()
{
    qDebug() << "et";
}
