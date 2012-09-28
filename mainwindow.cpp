#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "feedwizard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

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

    connect(wizard, SIGNAL(accepted()), this, SLOT(repaint()));
    wizard->show();
}
