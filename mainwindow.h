#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "feed.h"
#include "sqlitefeedstorageengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected slots:
    void addFeed();
    void newFeed();
    void openFeed(QModelIndex index);
    void openEntry(QModelIndex index);
    void updateFeeds();
    void enableUpdateFeed();
    void markSelectedEntriesRead();
    void markSelectedEntriesUnread();
    void toggleSelectedEntriesRead();
    void markSelectedFeedsRead();
    void markSelectedFeedsUnread();

signals:
    void markEntryRead(QModelIndex index);
    void markEntryUnread(QModelIndex index);

private:
    Ui::MainWindow *_ui;
    FeedStorageEngine *_storage;
};

#endif // MAINWINDOW_H
