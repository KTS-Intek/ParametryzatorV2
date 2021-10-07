#include "parametryzatorstartpage.h"


///[!] parametryzator-v2
#include "parametryzator-wdgts/switchrelaywdgt.h"
#include "parametryzator-wdgts/parametryzationprofileswdgt.h"
#include "parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzationprofileswdgttopper.h"


///[!] guisett-shared-core
#include "src/nongui/settloader.h"


///[!] tasks-shared
#include "src/shared/embeemodemglobalparams.h"



///[!] lamp-conf-shared
#include "lamp-conf-shared/nodediscoveryparams.h"
#include "lamp-conf-shared/writeembeesett.h"


///[!] widgets-shared
#include "zbyrator-src/zombieexhangewdgt.h"



#include "definedpollcodes.h"


//--------------------------------------------------------------------------------------------

ParametryzatorStartPage::ParametryzatorStartPage(GuiHelper *gHelper, QWidget *parent)
    : SelectStartOperationPage(gHelper, parent)
{
    dispsett.lblCounterDisp = tr("NIs %1");
    dispsett.lineSepar = " ";

    connect(this, &ParametryzatorStartPage::createWidget4thisTag, this, &ParametryzatorStartPage::createOneWdgt);
    connect(this, &ParametryzatorStartPage::pbStartClicked, this, &ParametryzatorStartPage::onPbStart_clicked);

    cbxMeterModels = new QComboBox(this);

//    QPushButton *testnibttn = new QPushButton(tr("Check NIs"), this);
//    connect(testnibttn, SIGNAL(clicked(bool)), this, SLOT(startNiChecker()));

//    getNiLineLayout()->insertWidget(1, testnibttn);
    getNiLineLayout()->insertWidget(1, cbxMeterModels);


    setMeterModels(QString("Auto MTX CE102 CE303").split(" "));

    cbxMeterModels->setCurrentIndex(1);//MTX


    connect(this, &ParametryzatorStartPage::onHighLevelCommandsObject, cbxMeterModels, &QComboBox::setEnabled);

    connect(this, &ParametryzatorStartPage::onConsumerAddressChanged, [=](QString address, QString workingdirectory){
       lastprintsett.lastConsumerAddress = address;
       lastprintsett.lastWorkingDirectory = workingdirectory;
    });


}

//--------------------------------------------------------------------------------------------

MyPollCodeList ParametryzatorStartPage::getLvOperations()
{
    MyPollCodeList list = getLvOperations4emb();

    list.append(ParametryzatorMedium::getLvOperations4parametryzator());

    return list;
}

//--------------------------------------------------------------------------------------------

QString ParametryzatorStartPage::getText4line()
{
    return SettLoader::loadSett(SETT_LAMPCONF_LASTNIS).toStringList().join(dispsett.lineSepar);

}

//--------------------------------------------------------------------------------------------

QStringList ParametryzatorStartPage::getNiListWithModels()
{
    const QString model = (cbxMeterModels->currentIndex() < 1) ? "" : cbxMeterModels->currentText();
    if(model.isEmpty())
        return QStringList();

    const  QStringList nis = getNiList();
    QStringList out;
    for(int i = 0, imax = nis.size(); i < imax; i++)
        out.append(QString("%1&%2").arg(model).arg(nis.at(i)));
    return out;
}


//--------------------------------------------------------------------------------------------

void ParametryzatorStartPage::setActiveJsonLine(QString jsonline, QVariantMap advancedsett)
{
    lastJsonLine = jsonline;
    lastWriteManufAdvacedSett = advancedsett;
}

//--------------------------------------------------------------------------------------------

void ParametryzatorStartPage::createOneWdgt(const int &tag)
{
    QWidget *w = 0;

    switch(tag){

    case POLL_CODE_ZOMBIE_EXCHANGE:{
        ZombieExhangeWdgt *p = new ZombieExhangeWdgt(gHelper, this);
        connect(this, &ParametryzatorStartPage::gimmeZombieCommands, p, &ZombieExhangeWdgt::gimmeYourData);
        connect(p, &ZombieExhangeWdgt::checkAddToCommands, [=](QString cname, QString cdata){
            if(cname.isEmpty())
                cname.append("test value");
            lastZombieCommands = cdata;
        });
        w = p;
        break; }

    case POLL_CODE_WRITE_MODEM_CONFIG:{
        WriteEmbeeSett *p = new WriteEmbeeSett(SettLoader::loadSett(SETT_LAMPCONF_LASTNETPARAM).toMap(), this);
        connect(p, &WriteEmbeeSett::setPageSett, [=](QVariantMap param){
            SettLoader::saveSett(SETT_LAMPCONF_LASTNETPARAM, param);
        });

        connect(p, &WriteEmbeeSett::setCommand2aModem, [=](QStringList list){
            lastCommand2aModem = list;
        });
        w = p;
        break;}

    case POLL_CODE_NODE_DISCOVERY:{

        AtndOneModeSettStruct deffsett;//it has all necessary settings

        NodeDiscoveryParams *p = new NodeDiscoveryParams(true, tr("Existing devices"),  deffsett, SettLoader::loadSett(SETT_LAMPCONF_LASTNDTPARAM).toMap(), this);
        connect(p, &NodeDiscoveryParams::setLastNDTparams, [=](QVariantMap map){
            SettLoader::saveSett(SETT_LAMPCONF_LASTNDTPARAM, map);
            lastNtdParam = map;
        });

        connect(this, &ParametryzatorStartPage::giveMeLastAtndParams, p, &NodeDiscoveryParams::sendMeYourSettings);
        w = p;
        break;}




    case POLL_CODE_WRITE_TARIFF:{

        ParametryzationProfilesWdgt *v = new ParametryzationProfilesWdgt(false, gHelper, this);
        connect(this, &ParametryzatorStartPage::gimmeYourLastTariffProgram, v, &ParametryzationProfilesWdgt::sendMeYourLastSett);
        connect(v, &ParametryzationProfilesWdgt::lastSettFromViewer, [=](QString lastprofiletext, QVariantHash paramsett){
            lastTariffProgramSett.profile = paramsett;
            lastTariffProgramSett.profname = lastprofiletext;
        }); // this, &ParametryzatorStartPage::setLastTariffSett);


        connect(v, &ParametryzationProfilesWdgt::onReloadParametryzatorProfiles, this, &ParametryzatorStartPage::onReloadParametryzatorProfiles);
        connect(this, &ParametryzatorStartPage::setPageSettParametryzatorProfile, v, &ParametryzationProfilesWdgt::setPageSettParametryzatorProfile);


        QStackedWidget *sw = v->topStackedWidget();//


        sw->setVisible(true);

        ParametryzationProfilesWdgtTopper *tw = new ParametryzationProfilesWdgtTopper(v);
        sw->addWidget(tw);
        sw->setCurrentWidget(tw);

        connect(tw, &ParametryzationProfilesWdgtTopper::openWorkingDirectorySettings, this, &ParametryzatorStartPage::openWorkingDirectorySettings);
        connect(this, &ParametryzatorStartPage::onConsumerAddressChanged, tw, &ParametryzationProfilesWdgtTopper::onConsumerAddressChanged);
        connect(tw, &ParametryzationProfilesWdgtTopper::setNewConsumerAddress, this, &ParametryzatorStartPage::setNewConsumerAddress);
        connect(tw, &ParametryzationProfilesWdgtTopper::setConsumerAddressFromWriteTariff, this, &ParametryzatorStartPage::setConsumerAddressFromWriteTariff);

        connect(tw, &ParametryzationProfilesWdgtTopper::openNiChecker, this, &ParametryzatorStartPage::startNiChecker);
        connect(this, &ParametryzatorStartPage::onNiListUpdated, tw, &ParametryzationProfilesWdgtTopper::onNiListUpdated);

        connect(tw, &ParametryzationProfilesWdgtTopper::gimmeMainSettHash, [=]{
            tw->lsett.mainsett = lastprintsett.mainhash;
        });
        connect(this, &ParametryzatorStartPage::kickOffAllObjects, tw, &ParametryzationProfilesWdgtTopper::kickOffAllObjects);




//        connect(w, &ParametryzationProfilesWdgt::onDeleteParametryzatorProfileTable, paramMedium, &ParametryzatorGuiMedium::onDeleteParametryzatorProfileTable);
//        connect(w, &ParametryzationProfilesWdgt::onParametryzatorProfileTableContentChanged, paramMedium, &ParametryzatorGuiMedium::onParametryzatorProfileTableContentChanged);



//        ParametryzationProfileViewer *v = new ParametryzationProfileViewer(false, gHelper, this);
//        connect(this, &ParametryzatorStartPage::gimmeYourLastTariffProgram, v, &ParametryzationProfileViewer::sendMeYourSett);
//        connect(v, &ParametryzationProfileViewer::setThisSett, this, &ParametryzatorStartPage::setLastTariffSett);
        w = v;
//tw->onConsumerAddressChanged();
        if(!lastprintsett.lastWorkingDirectory.isEmpty())
            emit onConsumerAddressChanged(lastprintsett.lastConsumerAddress, lastprintsett.lastWorkingDirectory);

        tw->onNiListUpdated(getNiList());

        QTimer::singleShot(111, this, SIGNAL(checkPrintingWdgt()));
        break;
    }

    case POLL_CODE_RELAY_OPERATIONS:{

        SwitchRelayWdgt *p = new SwitchRelayWdgt(this);
        connect(p, &SwitchRelayWdgt::pageSettChanged, [=](int switchMode){
            lRelaySwitchMode.relayMode = switchMode;
        });
        w = p;
        break;}

    case POLL_CODE_READ_MODEM_CONFIG:
    case POLL_CODE_RESET_MODEM:
    case POLL_CODE_FACTORY_SETTINGS:
    case POLL_CODE_RESET_NETWORK:

    case POLL_CODE_CCT: //just create an empty widget

//    case POLL_CODE_FF_READ_LAMP:
//    case POLL_CODE_FF_LED_VISUALISE:
//    case POLL_CODE_FF_RESET_EMB_IN_ALAMP:
//    case POLL_CODE_FF_READ_LAMP_MANUF:

    default: w = new QWidget(this); break;
    }

    addThisWidget2stack(w, tag);
}

//--------------------------------------------------------------------------------------------

void ParametryzatorStartPage::onPbStart_clicked()
{
    const int tag = getCurrentItemTag();
    if(canStartTheOperation(tag, getCurrentItemText())){
        qDebug() << "can";
        setPbStartEnabled(false);
    }else{
        qDebug() << "can't";
    }
}

//--------------------------------------------------------------------------------------------


void ParametryzatorStartPage::setMeterModels(QStringList metermodels)
{
    const QString currentmetermodel = cbxMeterModels->currentText();
    cbxMeterModels->clear();

    if(metermodels.isEmpty())
        return;

    cbxMeterModels->addItems(metermodels);
    if(metermodels.contains(currentmetermodel))
        cbxMeterModels->setCurrentIndex(cbxMeterModels->findText(currentmetermodel));
    else
        cbxMeterModels->setCurrentIndex(0);
}

//--------------------------------------------------------------------------------------------

void ParametryzatorStartPage::onRequest2pollThese2wdgt(QStringList nis, quint8 metertype)
{
    Q_UNUSED(metertype);
    setTheseNIs(nis);
}

//--------------------------------------------------------------------------------------------

void ParametryzatorStartPage::startNiChecker(QString dir, QString workingdirectory)
{
    if(workingdirectory.isEmpty()){
        emit gimmeMainAndGroupSettings();

    workingdirectory = lastprintsett.mainhash.value("lePath2pdfDir").toString();

    }
    const QStringList nis = getNiList();
    if(nis.isEmpty()){
        gHelper->showMessageSlot(tr("There is nothing to check"));
        return;
    }

    if(lastprintsett.lastDirectoryPath.isEmpty()){
        lastprintsett.lastDirectoryPath = QString("%1/%2")
                .arg(workingdirectory)
                .arg(dir);// lastprintsett.grphash.value("leConsumerAddress").toString());

        QDir dirr(lastprintsett.lastDirectoryPath);
        if(!dirr.exists()){
            dirr.cdUp();
            if(dirr.exists()){
                lastprintsett.lastDirectoryPath = dirr.absolutePath();
            }else{
                lastprintsett.lastDirectoryPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
            }
        }
    }
const QString dirpath = QString("%1/%2")
        .arg(workingdirectory)
        .arg(dir);
//    const QString dirpath = QFileDialog::getExistingDirectory(this, tr("Select a directory"), lastprintsett.lastDirectoryPath);
    if(workingdirectory.isEmpty() || dir.isEmpty())
        return;
    lastprintsett.lastDirectoryPath = dirpath;

    lastprintsett.shownis = nis;

    QDir dirr(dirpath);

    PrintTvPreview *printtv = new PrintTvPreview(gHelper, 0);
    printtv->setWindowTitle(tr("Check NIs - %1").arg(dirr.dirName()));
    printtv->show();
    printtv->lastsett.lockPrinting = true;

    connect(printtv, &PrintTvPreview::pageEndInit, this, &ParametryzatorStartPage::addData2theTable);


    QTimer *tmrUpdateLater = new QTimer(printtv);
    tmrUpdateLater->setInterval(1111);
    tmrUpdateLater->setSingleShot(true);

    connect(cbxMeterModels, SIGNAL(currentIndexChanged(int)), tmrUpdateLater, SLOT(start()));
    connect(this, SIGNAL(onNiTextChanged()), tmrUpdateLater, SLOT(start()));
    connect(tmrUpdateLater, &QTimer::timeout, [=]{
        lastprintsett.shownis = getNiList();

        addData2theTableExt(printtv);

    });




}

void ParametryzatorStartPage::addData2theTable()
{
    PrintTvPreview *printtv = qobject_cast<PrintTvPreview *>(QObject::sender());
   addData2theTableExt(printtv);
}

void ParametryzatorStartPage::addData2theTableExt(PrintTvPreview *printtv)
{
    if(printtv){
        printtv->setPageSettpPrintingGrp(lastprintsett.grphash);

        printtv->setPageSettpPrintingMain(lastprintsett.mainhash);

        MPrintTableOut printout;
        QVector<QVariantHash> printdata;

        //model, S/N, ni, meterprog

        for(int i = 0, imax = lastprintsett.shownis.size(); i < imax; i++){
            const QString ni = lastprintsett.shownis.at(i);
            QStringList l;
            l.append(cbxMeterModels->currentText());// "UCM");
            l.append(QString("...%1").arg(ni));
            l.append(ni);
            l.append("unknown");
            printout.append(l);

            printdata.append(QVariantHash());

        }

        printtv->startPrintingTariffReports(printout, printdata);

    }
}

void ParametryzatorStartPage::setPageSettParametryzatorProfileSlot(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader)
{
    qDebug() << "setPageSettParametryzatorProfile" << headerH << listRows;

}


//--------------------------------------------------------------------------------------------

bool ParametryzatorStartPage::canStartTheOperation(const int &pollCode, const QString &name)
{
    const QVariantMap map = tryStartTheOperation(pollCode);
    if(map.isEmpty()){
        gHelper->showMessageSlot(tr("Not all fields are filled"));
        return false;

    }

    gHelper->updateSettDateMaskAndDotPos();
 //   metersListMedium->setLastPageId(accessibleName());

//    void startOperation(int pollcode, QVariantMap params, QString name, quint8 deviceType);

    emit startOperation(pollCode, map, name, UC_METER_ELECTRICITY);
    if(pollCode < POLL_CODE_MODEM_OPEREATIONS_END )
        return true;

    QStringList ns = map.value("-ns").toStringList();

    for(int i = 0, imax = ns.size() ; i < imax; i++)
        ns.replace(i, ns.at(i).split("&").last());

    const DeviceHistory h = DeviceHistory(QDateTime::currentMSecsSinceEpoch(), name);
    for(int i = 0, imax = ns.size(); i < imax; i++)
        ni2history.insert(ns.at(i), h);


    SettLoader::saveSett(SETT_LAMPCONF_LASTNIS, ns);

    return true;
}

//--------------------------------------------------------------------------------------------

QVariantMap ParametryzatorStartPage::tryStartTheOperation(int pollCode)
{
    const QStringList ns = (pollCode < POLL_CODE_MODEM_OPEREATIONS_END) ? EmbeeModemGlobalParams::getNi4taskList(pollCode) : getNiListWithModels();

    if(ns.isEmpty())
        return QVariantMap();
    QVariantMap map;
    map.insert("-ns", ns);


    switch(pollCode){

    case POLL_CODE_CCT:
        map.insert("-ns", getNiList());//without models
        break;//do not forget about NIs

    case POLL_CODE_ZOMBIE_EXCHANGE:{
        emit gimmeZombieCommands();
        map.insert("-operation", lastZombieCommands);
        break;}

    case POLL_CODE_WRITE_MODEM_CONFIG:{
        map.insert("-operation", lastCommand2aModem.join("\n"));
        break;}

    case POLL_CODE_NODE_DISCOVERY:{
        emit giveMeLastAtndParams();
        map.insert("-operation", QJsonDocument(QJsonObject::fromVariantMap(lastNtdParam)).toJson(QJsonDocument::Compact));
        break;}



    case POLL_CODE_READ_MODEM_CONFIG:
    case POLL_CODE_RESET_MODEM:
    case POLL_CODE_FACTORY_SETTINGS:
    case POLL_CODE_RESET_NETWORK:  break;


    case POLL_CODE_WRITE_TARIFF:{
        lastTariffProgramSett.profile.clear();
        emit gimmeYourLastTariffProgram();
        map.insert("-operation", QJsonDocument(QJsonObject::fromVariantHash(lastTariffProgramSett.profile)).toJson(QJsonDocument::Compact));

        if(lastTariffProgramSett.profile.isEmpty())
            map.clear();
        break;}

    case POLL_CODE_READ_TARIFF:
    case POLL_CODE_READ_DATE_TIME_DST:
    case POLL_CODE_WRITE_DATE_TIME:{
        //52 QMap(("-ns", QVariant(QStringList, ("7498")))("-operation", QVariant(QString, "0")))
        //53 QMap(("-ns", QVariant(QStringList, ("7498")))("-operation", QVariant(QString, "0")))
        map.insert("-operation", 0);
        break;}

    case POLL_CODE_RELAY_OPERATIONS:{
//59 QMap(("-ns", QVariant(QStringList, ("7498")))("-operation", QVariant(QString, "3")))
        map.insert("-operation", lRelaySwitchMode.relayMode);
        break;}




    }
    //map must contain -operation and -ns
    return map;
}

//--------------------------------------------------------------------------------------------
