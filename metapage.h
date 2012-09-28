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

private slots:
    void fillForm();
    
private:
    Ui::MetaPage *_ui;
    Feed *_feed;
};

#endif // METAPAGE_H
