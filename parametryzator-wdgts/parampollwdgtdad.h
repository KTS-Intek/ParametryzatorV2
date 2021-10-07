#ifndef PARAMPOLLWDGTDAD_H
#define PARAMPOLLWDGTDAD_H


#include <QWidget>
#include "dataconcetrator-pgs/pollwdgt.h"

namespace Ui {
class ParamPollWdgtDad;
}

class ParamPollWdgtDad : public QWidget
{
    Q_OBJECT

public:
    explicit ParamPollWdgtDad(GuiHelper *gHelper, QWidget *parent = nullptr);
    ~ParamPollWdgtDad();


    QStringList getOnlyKeys();

    PollWdgt *pollWdgt;

public slots:
    void onReloadPageSettings();

    void onSavePageSettings();

    void unlockIgnore();

    void saveAllForced();


private slots:
    void on_pushButton_clicked();

private:
    Ui::ParamPollWdgtDad *ui;

    bool ignoreSave;


};

#endif // PARAMPOLLWDGTDAD_H
