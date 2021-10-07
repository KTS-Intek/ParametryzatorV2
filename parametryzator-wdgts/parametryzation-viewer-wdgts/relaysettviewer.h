#ifndef RELAYSETTVIEWER_H
#define RELAYSETTVIEWER_H

#include <QWidget>


///[!] widgets-shared
#include "template-pgs/twolistswdgt.h"


namespace Ui {
class RelaySettViewer;
}

class RelaySettViewer : public QWidget
{
    Q_OBJECT

public:
    explicit RelaySettViewer(const bool &editable, QWidget *parent = nullptr);
    ~RelaySettViewer();
    bool isEditable;

    QString getPageSett();


    TwoListWdgtPairList getAllListItems();

    TwoListWdgtPairList getDefListItems();

    TwoListsWdgt *listwdgt;


signals:
    void onWdgtIsReady();


public slots:
    void setPageSett(const QString &relay);

    void applyDefaultSett();



private slots:
    void createWidgets();



private:
    Ui::RelaySettViewer *ui;
};

#endif // RELAYSETTVIEWER_H
