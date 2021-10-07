#ifndef RELAYTESTSETTVIEWER_H
#define RELAYTESTSETTVIEWER_H

#include <QWidget>


///[!] widgets-shared
#include "template-pgs/twolistswdgt.h"



namespace Ui {
class RelayTestSettViewer;
}

class RelayTestSettViewer : public QWidget
{
    Q_OBJECT

public:
    explicit RelayTestSettViewer(const bool &editable, QWidget *parent = nullptr);
    ~RelayTestSettViewer();

    bool isEditable;

    QString getPageSett();

    TwoListWdgtPairList getAllListItems();

    TwoListWdgtPairList getDefListItems();

    TwoListsWdgt *listwdgt;

signals:
    void onWdgtIsReady();


public slots:
    void setPageSett(const QString &relay_test);

    void applyDefaultSett();



private slots:
    void createWidgets();



private:
    Ui::RelayTestSettViewer *ui;


};

#endif // RELAYTESTSETTVIEWER_H
