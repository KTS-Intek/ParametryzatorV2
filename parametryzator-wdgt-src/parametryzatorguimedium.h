#ifndef PARAMETRYZATORGUIMEDIUM_H
#define PARAMETRYZATORGUIMEDIUM_H


///[!] widgets-shared
#include "gui-src/guiifacemedium.h"

///[!] guisett-shared-core
#include "src/ucdev/ucdevicetreewatcher.h"


///[!] parametryzator-base
#include "parametryzator-src/parametryzatordatacalculation.h"


#include "medium4modelsdatatypes.h"


class ParametryzatorGuiMedium : public GuiIfaceMedium
{
    Q_OBJECT
public:
    explicit ParametryzatorGuiMedium(UCDeviceTreeWatcher *ucDeviceTreeW, QObject *parent = nullptr);

    bool pbWriteDis;

    UCDeviceTreeWatcher *ucDeviceTreeW;

signals:
    void setPageSettParametryzatorProfile(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader);


//    void onPollStarted(quint8 pollCode, QStringList listEnrg, QString dateMask, int dotPos, bool allowDate2utc);
    void onPollStarted(UCSelectFromDB select, QString yourSenderName, QStringList listHeaderDb, quint8 deviceType);

//    void addData2sourcePages(quint8 pollCode, MTableFullHouse ftable, QStringList header, QStringList humanHeader);


    //to the poll tabs
        void onUCLastReceivedDeviceRecordsQuickCollectChanged(UCLastReceivedDeviceRecords records);//connect it to GUI


    void setLblWaitTxt(QString s);


    //Database and meterjournal
//    void data2dbMedium(quint16 command, QVariant varData); use ucDeviceTreeW
    void stopReadDatabase();

//    void appendDataDatabase(QVariantHash hash); use ucDeviceTreeW
    void setLblWaitTxtDatabase(QString s);

    void setDateMask(QString dateMask);
    void setDotPos(int dotPos);
    void setPbReadEnableDisable(bool disable);




    void reloadSettings();


    void killAllObjects();


    //to zbyrator
    void reloadIfaceChannels();



    //zbyrator ipc
    void command2extensionClient(quint16 command, QVariant dataVar);


public slots:
    void onPollStartedSmpl(quint8 pollCode, QStringList listEnrg, QString dateMask, int dotPos, bool allowDate2utc);

    void onReloadParametryzatorProfiles();

    void onParametryzatorProfileTableContentChanged(QString profname, QVariantHash profile);

    void onDeleteParametryzatorProfileTable(QStringList deleteThese);


    void onTaskCanceled(quint8 pollCode, QString ni, qint64 dtFinished, quint8 rez);

    void createDatabaseMedium();

    void setPbWriteDis(bool disabled);


    void meterRelayStatus(QString ni, QDateTime dtLocal, quint8 mainstts, quint8 secondarystts);

    void meterDateTimeDstStatus(QString ni, QDateTime dtLocal, QString stts);


    void relayStatusChanged(QVariantMap map);


    void mWrite2RemoteDev(quint16 command, QVariant dataVar);//from guihelper

private:

    void createDataCalculator();



};

#endif // PARAMETRYZATORGUIMEDIUM_H
