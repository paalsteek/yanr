#ifndef FEEDWIZARD_H
#define FEEDWIZARD_H

#include <QWizard>

namespace Ui {
class FeedWizard;
}

class FeedWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit FeedWizard(QWidget *parent = 0);
    ~FeedWizard();
    
private:
    Ui::FeedWizard *_ui;
};

#endif // FEEDWIZARD_H
