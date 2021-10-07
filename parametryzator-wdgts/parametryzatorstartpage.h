#ifndef PARAMETRYZATORSTARTPAGE_H
#define PARAMETRYZATORSTARTPAGE_H

///[!] widgets-shared
#include "template-pgs/selectstartoperationpage.h"



///[!] parametryzator-base
#include "parametryzator-src/parametryzatormedium.h"



#include "parametryzation-viewer-wdgts/printtvpreview.h"



#include "definedpollcodes.h"



class ParametryzatorStartPage : public SelectStartOperationPage
{
    Q_OBJECT
public:
    explicit ParametryzatorStartPage(GuiHelper *gHelper, QWidget *parent = nullptr);

    QComboBox *cbxMeterModels;

    struct ParamLastPrintingSett{
        QVariantHash mainhash;
        QVariantHash grphash;

        QString lastDirectoryPath;
        QStringList shownis;

        QString lastConsumerAddress;
        QString lastWorkingDirectory;


    } lastprintsett;


    MyPollCodeList getLvOperations();

    QString getText4line();

    QStringList getNiListWithModels();

signals:
    void giveMeLastAtndParams();


    void gimmeYourLastTariffProgram();


//    void giveMePowerParams();

//    void giveMeSensorsParams();



    //to paramMedium
    void onReloadParametryzatorProfiles();

    //from paramMedium
    void setPageSettParametryzatorProfile(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader);




    void gimmeMainAndGroupSettings();

    void setNewConsumerAddress(QString address);
    void onConsumerAddressChanged(QString address, QString workingdirectory);

    void setConsumerAddressFromWriteTariff(QString address);


    void openWorkingDirectorySettings();


    void checkPrintingWdgt();
    void kickOffAllObjects();

    void gimmeZombieCommands();


public slots:


    void setActiveJsonLine(QString jsonline, QVariantMap advancedsett);

//    void setNetParams(int channel, int id);



//    void oneLampParamPageSettChanged(quint8 grp, quint8 pstart, quint8 pna, quint16 tna, QString ni);//if ni isEmpty - do not change



    void createOneWdgt(const int &tag);

//    void dimmingStartOperation(quint16 pollcode, QVariantMap params);

//    void setSensorsParams(QVariantMap map);

    void onPbStart_clicked();


    void setMeterModels(QStringList metermodels);



    void onRequest2pollThese2wdgt(QStringList nis, quint8 metertype);

    //    void openNiChecker(QString dir, QString workingdirectory);

    void startNiChecker(QString dir, QString workingdirectory);


    void addData2theTable();
    void addData2theTableExt(PrintTvPreview *printtv);

    void setPageSettParametryzatorProfileSlot(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader);


private:
    bool canStartTheOperation(const int &pollCode, const QString &name);
    QVariantMap tryStartTheOperation(int pollCode);


    struct RelaySwitchParam
    {
        int relayMode;
        RelaySwitchParam() : relayMode(RELAY_READ) {}
    } lRelaySwitchMode;


    struct TariffParams
    {
        QVariantHash profile;
        QString profname;

        TariffParams() {}
    } lastTariffProgramSett;


    QString lastZombieCommands;


    QString lastJsonLine;
    QVariantMap lastWriteManufAdvacedSett;


    QStringList lastCommand2aModem;
    QVariantMap lastNtdParam;


};

#endif // PARAMETRYZATORSTARTPAGE_H
