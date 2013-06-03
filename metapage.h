#ifndef METAPAGE_H
#define METAPAGE_H

#include <QWizardPage>

#include "feed.h"

namespace Ui {
class MetaPage;
}

class MetaPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit MetaPage(QWidget *parent = 0);
    ~MetaPage();
    void initializePage();
    virtual bool isComplete() const;

private slots:
    void fillForm();
    
private:
    Ui::MetaPage *_ui;
    Feed *_feed;
    bool _complete;
};

#endif // METAPAGE_H
