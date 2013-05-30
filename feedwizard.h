#ifndef FEEDWIZARD_H
#define FEEDWIZARD_H

#include <QWizard>

#include "feed.h"

namespace Ui {
class FeedWizard;
}

class FeedWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit FeedWizard(QWidget *parent = 0);
    ~FeedWizard();

    Feed* getFeed();
    void setFeed(Feed* feed);

signals:
    void newFeed(Feed*);
    
private:
    Ui::FeedWizard *_ui;
    Feed* _feed;
};

#endif // FEEDWIZARD_H
