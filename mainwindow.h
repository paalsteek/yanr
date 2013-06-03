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

private:
    Ui::MainWindow *_ui;
    FeedStorageEngine *_storage;
};

#endif // MAINWINDOW_H
