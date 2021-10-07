#ifndef PARAMERTYZATORCONFIGTABWDGT_H
#define PARAMERTYZATORCONFIGTABWDGT_H

#include "lamp-conf-shared/tabwidget4config.h"

///[!] parametryzator-base
#include "parametryzator-src/parametryzatorviewmedium.h"


class ParamertyzatorConfigTabWdgt : public TabWidget4config
{
    Q_OBJECT
public:
    explicit ParamertyzatorConfigTabWdgt(GuiHelper *gHelper, QWidget *parent = nullptr);
    ParametryzatorViewMedium *viewmedium;

    struct ThisPageSettState{
        bool isConnectedAddressChange;


        ThisPageSettState() : isConnectedAddressChange(false) {}

    } lsettstate;

    void createStartPage();

    QStringList getEnrg4yourCode(const int &code);

//    void checkEnableQrMode(DbDataForm *f);


    void operationIsAboutToStart();

    QList<QAction*> getLaAdditional(const bool &youHaveApower, const int &selItems, const int &pollcode, QTableView *tparent, const QString &fobjname);

    void thisRowClicked(const int &srcrow, const int &proxyrow, const int &code, QTableView *tparent);

    void onTabCreated(const int &pollcode, DbDataForm *f );

signals:
    //to paramMedium
    void onReloadParametryzatorProfiles();


    //from paramMedium
    void setPageSettParametryzatorProfile(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader);


    void startPrintingTariffReports(MPrintTableOut printout, QVector<QVariantHash> printdata);


//to startpage
    void onRequest2pollThese2wdgt(QStringList nis, quint8 metertype);
    void setPageSettpPrintingGrp(QVariantHash lastPageSett);
    void setPageSettpPrintingMain(QVariantHash mainPrintSett);

    //from startpage
    void gimmeMainAndGroupSettings();
//to print page
    void setNewConsumerAddress(QString address);

    //from print page
    void onConsumerAddressChanged(QString address, QString workingdirectory);


    void openWorkingDirectorySettings();


    void ndtNi(QString oneni);


    void checkPrintingWdgt();

    void setConsumerAddressFromWriteTariff(QString address);


    //to the poll tabs
        void onUCLastReceivedDeviceRecordsQuickCollectChanged(UCLastReceivedDeviceRecords records);//connect it to GUI

//to paramMedium
        void onPollStartedV2(UCSelectFromDB select, QString yourSenderName, QStringList listHeaderDb, quint8 deviceType);

public slots:

    void createProfileViewer(QString title);

    void onParamViewerReady();

    void onLastTvClicked(const QModelIndex &index);

};

#endif // PARAMERTYZATORCONFIGTABWDGT_H
