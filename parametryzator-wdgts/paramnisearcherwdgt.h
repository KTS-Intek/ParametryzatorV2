#ifndef PARAMNISEARCHERWDGT_H
#define PARAMNISEARCHERWDGT_H

#include <QWidget>

#include "parametryzation-viewer-wdgts/paramnisearcher.h"




///[!] widgets-shared
#include "template-pgs/devicehistoryselector.h"
#include "template-pgs/selectstartoperationpage.h"


namespace Ui {
class ParamNISearcherWdgt;
}

class ParamNISearcherWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit ParamNISearcherWdgt(GuiHelper *gHelper, QWidget *parent = nullptr);
    ~ParamNISearcherWdgt();

    QMap<QString, SelectStartOperationPage::DeviceHistory> ni2history;


    QStringList lastnis;

    void createWidgets(GuiHelper *gHelper);

    ParamNISearcher *searcherwdgt;

    QStringList getnis();

signals:
    void gimmeSettings();


    void setTheseNis(QStringList nis);


    void setPageSettpPrintingGrp(QVariantHash sett);
    void setPageSettpPrintingMain(QVariantHash lastPageSett);
    void openWorkingDirectorySettings();


    //local
    void killOldHistory();
    void sendPageSett(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader);
    void kickOffAllObjects();


private slots:

    void sendNis();


    void on_tbNiHistory_clicked();

private:
    Ui::ParamNISearcherWdgt *ui;
};

#endif // PARAMNISEARCHERWDGT_H
