#ifndef DISPLAYSETTVIEWER_H
#define DISPLAYSETTVIEWER_H

#include <QWidget>


///[!] widgets-shared
#include "template-pgs/twolistswdgt.h"


namespace Ui {
class DisplaySettViewer;
}

class DisplaySettViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DisplaySettViewer(const bool &editable, QWidget *parent = nullptr);
    ~DisplaySettViewer();
    bool isEditable;

    QString getPageSett();


    TwoListWdgtPairList getAllListItems();

    TwoListWdgtPairList getDefListItems();

    TwoListsWdgt *listwdgt;


signals:
    void onWdgtIsReady();


public slots:
    void setPageSett(const QString &display);

    void applyDefaultSett();



private slots:
    void createWidgets();




private:
    Ui::DisplaySettViewer *ui;

    QVariantHash lastSettHash;

};

#endif // DISPLAYSETTVIEWER_H
