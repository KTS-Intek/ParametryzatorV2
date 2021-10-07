#ifndef GETPARAMETRYZATORHISTORY_H
#define GETPARAMETRYZATORHISTORY_H


///[!] widgets-meters
#include "dataconcetrator-pgs/src/databasewdgt4quickcollect.h"


///[!] parametryzator-base
#include "parametryzator-src/parametryzatorviewmedium.h"

class GetParametryzatorHistory : public DatabaseWdgt4QuickCollect
{
    Q_OBJECT
public:
    explicit GetParametryzatorHistory(GuiHelper *gHelper, QWidget *parent = nullptr);

    ParametryzatorViewMedium *viewmedium;

    bool lastIsATariffPollCode;



    MyPollCodeList getLvIconsAndTexts(const int &version);//device types

    OneDevicePollCodes getDeviceSelectSett4adev(const int &devtype);



    static MTableFullHouse getThisTvTariffSettExt(QTableView *tv, const QModelIndex &index);// const int &code, const bool &onlycurrentrow);


    void onThisDbDataFormV3IsGoingToBeAdded(DbDataFormV3 *w);

//    void connEnDisableButtons();

signals:
//    void data2dbMediumExt(quint16 command, QVariant varData);



    void startPrintingTariffReports(MPrintTableOut printout, QVector<QVariantHash> printdata);





public slots:
//    void data2dbMediumSlot(quint16 command, QVariant varData);

    void createProfileViewer(QString title);

    void onParamViewerReady();


    void onGetUCDataBaseData(UCSelectFromDB select, QString senderName);



    void onThisWidgetIsAddedAndReady(DbDataFormSmplV2 *w);


    void onLastTvClicked(const QModelIndex &index);

};

#endif // GETPARAMETRYZATORHISTORY_H
