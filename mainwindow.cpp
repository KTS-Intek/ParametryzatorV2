#include "mainwindow.h"
#include "ui_mainwindow.h"


///[!] widgets-shared
#include "gui-src/stackwidgethelper.h"
#include "main-pgs/optionswdgt.h"
#include "template-pgs/smplptewdgt.h"
#include "zbyrator-src/zbyrifacesett.h"



///[!] parametryzator-v2
#include "parametryzator-wdgts/parametryzationprofileswdgt.h"
#include "parametryzator-wdgts/paramertyzatorconfigtabwdgt.h"
#include "parametryzator-wdgts/getparametryzatorhistory.h"
#include "parametryzator-wdgts/printparametryzator.h"
#include "parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzatorprintsettingswdgt.h"
#include "parametryzator-wdgts/humanaccessdialog.h"
#include "parametryzator-wdgts/parampollwdgtdad.h"
#include "parametryzator-wdgts/paramnisearcherwdgt.h"


///[!] parametryzator-base
#include "parametryzator-src/parametryzatorprocessmanager.h"



///[!] zbyrator-base
#include "src/zbyrator-v2/metermanager.h"




//------------------------------------------------------------------------------------

MainWindow::MainWindow(const QFont &font4log, const int &defFontPointSize, QWidget *parent)
    :  MainIfaceMedium(font4log, defFontPointSize, parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

//------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    saveMainAppLog(ui->actionApplication_log->isChecked());
    delete ui;
}



//------------------------------------------------------------------------------------

void MainWindow::openIfaceSett()
{
    onShowThisWdiget(STACK_PAGE_IFACE);

}

//------------------------------------------------------------------------------------

void MainWindow::showLastWdgt()
{
    onShowThisWdiget(lastWdgtTag);

}

//------------------------------------------------------------------------------------

void MainWindow::onRequest2pollThese(QStringList nis, quint8 metertype)
{

//    Q_UNUSED(nis);
//    Q_UNUSED(metertype);
    onShowThisWdiget(STACK_PAGE_CONFIG);

    emit onRequest2pollThese2wdgt(nis, metertype);

//    emit showMessCritical(tr("this function is not ready"));

}

//------------------------------------------------------------------------------------

void MainWindow::gimmeMyLastSettingsPrint()
{
    //      h.insert("sbDocNumber", SettLoader::loadSett(SETT_OPTIONS_LEOUTINDX).toInt());// ui->leOutIndx->setValue(SettLoader::loadSett(SETT_OPTIONS_LEOUTINDX).toInt() );

    gimmeMyLastSettingsPrintHtmlProfiles();//it must be before group settings

    emit setPrintingSettings(SettLoader::loadSett(SETT_PARAMETRYZATOR_PRINTONE).toHash());


    emit setPageSettpPrintingGrp(SettLoader::loadSett(SETT_PARAMETRYZATOR_PRINTGRP).toHash());


    gimmeMyLastSettingsPrintMain();


}

//------------------------------------------------------------------------------------

void MainWindow::setLastPageSettPrint(QVariantHash pagesett)
{
    SettLoader::saveSett(SETT_PARAMETRYZATOR_PRINTGRP, pagesett);
}

//------------------------------------------------------------------------------------

void MainWindow::gimmeMyLastSettingsPrintMain()
{

    QVariantHash h = SettLoader::loadSett(SETT_PARAMETRYZATOR_PRINTMN).toHash();
    if(h.value("lePath2pdfDir").toString().isEmpty())
        h.insert("lePath2pdfDir", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));


    emit setPageSettpPrintingMain(h);

}

//------------------------------------------------------------------------------------

void MainWindow::gimmeMyLastSettingsPrintHtmlProfiles()
{
    emit setPageSettpPrintingHtml(SettLoader::loadSett(SETT_PARAMETRYZATOR_PROFILES).toHash());

}

//------------------------------------------------------------------------------------

void MainWindow::showMessageIneedHuman(QString message, QString deviceni)
{
//    QDialog *d = new QDialog(this);
    HumanAccessDialog *d = new HumanAccessDialog(message, this);
    d->setObjectName(QString("deviceni-%1").arg(deviceni));
    connect(this, SIGNAL(closeMessageIneedHuman()), d, SLOT(deleteLater()));

    d->exec();

    emit resetMakeApause();

}

//------------------------------------------------------------------------------------

void MainWindow::initPage()
{
    lastToolBar = ui->mainToolBar;
    lastWdgtTag = 0xFF;

    createGuiHelper(ui->stackedWidget, true);

    guiHelper->ucDeviceTreeW = new UCDeviceTreeWatcher(true, true, this);

    createStackWidgetHelper();

    paramMedium = new ParametryzatorGuiMedium(guiHelper->ucDeviceTreeW, this);

    connect(paramMedium, &ParametryzatorGuiMedium::setPbReadEnableDisable, guiHelper, &GuiHelper::setPbReadEnableDisableSlot);

//    connect(paramMedium, &ParametryzatorGuiMedium::pbStopAnimateClick, guiHelper, &GuiHelper::pbStopAnimateClick);
    guiHelper->setObjectName("MainWindow");
    connect(paramMedium, &ParametryzatorGuiMedium::appendAppLog        , this, &MainWindow::appendShowMessagePlain );
    //to show iface log in app log
    connect(paramMedium, &ParametryzatorGuiMedium::ifaceLogStrFromZbyrator, this, &MainWindow::appendShowMessagePlain);
    connect(paramMedium, &ParametryzatorGuiMedium::ifaceLogStrFromDA, this, &MainWindow::appendShowMessagePlain);

    guiHelper->managerEnDisBttn.pbReadDis = guiHelper->managerEnDisBttn.pbWriteDis = false;
//    connect(this, &MainWindow::reloadSettings2ucEmulator, paramMedium, &ParametryzatorGuiMedium::reloadSettings);
    connect(guiHelper, &GuiHelper::setPbWriteEnableDisable, paramMedium, &ParametryzatorGuiMedium::setPbWriteDis);
    paramMedium->setPbWriteDis(guiHelper->managerEnDisBttn.pbWriteDis);

    connect(guiHelper, &GuiHelper::setDateMask, paramMedium, &ParametryzatorGuiMedium::setDateMask);
    connect(guiHelper, &GuiHelper::setDotPos, paramMedium, &ParametryzatorGuiMedium::setDotPos);


    connect(paramMedium, SIGNAL(setPollSaveSettings(quint16,quint16,bool,bool,bool,qint32,qint32,qint32,bool)), paramMedium, SIGNAL(reloadIfaceChannels()));




    createActions();
    onShowThisWdiget(STACK_PAGE_CONFIG);

    createAppOutLog();

    QTimer::singleShot(1, this, SLOT(createParametryzatorTaskManager()));


    if(guiHelper->ucDeviceTreeW->getProtocolVersion() < MATILDA_PROTOCOL_VERSION_V7)
        guiHelper->ucDeviceTreeW->setProtocolVersion(MATILDA_PROTOCOL_VERSION_V7);//it is needed by this widget


    lastIfaceLoader4mainwindow = paramMedium->ifaceLoaderPrimary;

    connect(this, &MainWindow::onRequest2GetDataThese, [=]{
       onShowThisWdiget(STACK_PAGE_HISTORY);
    });



}

//------------------------------------------------------------------------------------

void MainWindow::onActClicked()
{
    QAction *a = qobject_cast<QAction *>(QObject::sender());
    bool ok;
    const int tag = a->data().toInt(&ok);
    if(ok)
        onShowThisWdiget(tag);
}

//------------------------------------------------------------------------------------

void MainWindow::onShowThisWdiget(const int &tag)
{
    if(tag != STACK_PAGE_IFACE)
        lastWdgtTag = tag;

    if(lastWdgtTag != tag)
        guiHelper->closeYourPopupsSlot();


    if(!readyWdgts.contains(tag) && !createThisWdiget(tag))
        return;

    ui->stackedWidget->setCurrentWidget(readyWdgts.value(tag));

    MatildaConfWidget *w = currentMatildaWidget();
    if(w)
        w->pageActivated();

    emit addWdgt2history();

}

//------------------------------------------------------------------------------------

void MainWindow::on_actionGlobal_triggered()
{

    addWdgt2stackWdgt(new OptionsWdgt(guiHelper,this), WDGT_TYPE_OPTIONS, false, tr("Options"), ":/katynko/svg/applications-system.svg");

}

//------------------------------------------------------------------------------------

void MainWindow::createParametryzatorTaskManager()
{
    createSqliteServer();

    MeterManager *zbyrator = new MeterManager("prm", true, ZbyrConnSett());

    QThread *thread = new QThread;
    thread->setObjectName("MeterManager");
    zbyrator->moveToThread(thread);

#ifdef ALLOW_HUMAN_STOPS
    connect(this, &MainWindow::resetMakeApause, zbyrator, &MeterManager::resetMakeApause, Qt::DirectConnection);
    connect(zbyrator, &MeterManager::showMessageIneedHuman, this, &MainWindow::showMessageIneedHuman);
    connect(zbyrator, &MeterManager::closeMessageIneedHuman, this, &MainWindow::closeMessageIneedHuman);
#endif

    connect(this, &MainWindow::receivedKillSignal, zbyrator, &MeterManager::kickOffAllObjects);
    connect(zbyrator, SIGNAL(destroyed(QObject*)), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(thread, SIGNAL(started()), zbyrator, SLOT(onThreadStarted()));

    connect(paramMedium, SIGNAL(command4devStr(quint16,QString))    , zbyrator, SIGNAL(command4devStr(quint16,QString)) );
    connect(paramMedium, SIGNAL(command4dev(quint16,QVariantMap)), zbyrator, SIGNAL(command4dev(quint16,QVariantMap)) );

//    connect(paramMedium, &ParametryzatorGuiMedium::onConfigChanged     , zbyrator, &MeterManager::onConfigChanged      );

    connect(paramMedium, &ParametryzatorGuiMedium::setThisIfaceSett    , zbyrator, &MeterManager::setThisIfaceSett     );
    connect(paramMedium, SIGNAL(setPollSaveSettings(quint16,quint16,bool,bool,bool,qint32,qint32,qint32,bool)), zbyrator, SLOT(reloadPollSettings()));
    connect(paramMedium, &ParametryzatorGuiMedium::reloadIfaceChannels, zbyrator, &MeterManager::reloadIfaceChannels); // SIGNAL(setPollSaveSettings(quint16,quint16,bool,bool,bool,qint32,qint32,qint32,bool)), zbyrator, SLOT(reloadIfaceChannels()));

    connect(paramMedium, &ParametryzatorGuiMedium::giveMeYourCache     , zbyrator, &MeterManager::giveMeYourCache      );
    connect(paramMedium, &ParametryzatorGuiMedium::killUconTasks       , zbyrator, &MeterManager::killUconsTasks       );
    connect(paramMedium, &ParametryzatorGuiMedium::setIgnoreCycles     , zbyrator, &MeterManager::setIgnoreCycles      );
    connect(paramMedium, &ParametryzatorGuiMedium::setOnlyGlobalConnection     , zbyrator, &MeterManager::setOnlyGlobalConnection      );

    connect(paramMedium, &ParametryzatorGuiMedium::reloadSettings, zbyrator, &MeterManager::reloadSettings);

    connect(paramMedium, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)) );

    connect(zbyrator, &MeterManager::ifaceLogStr                , paramMedium, &ParametryzatorGuiMedium::ifaceLogStr               );
    connect(zbyrator, &MeterManager::ifaceLogStrNonBuf    , paramMedium, &ParametryzatorGuiMedium::ifaceLogStrFromZbyrator               );

    connect(zbyrator, &MeterManager::appendMeterData            , paramMedium, &ParametryzatorGuiMedium::appendMeterData           );
    connect(zbyrator, &MeterManager::onConnectionStateChanged   , paramMedium, &ParametryzatorGuiMedium::onConnectionStateChanged  );
    connect(zbyrator, &MeterManager::onUconStartPoll            , paramMedium, &ParametryzatorGuiMedium::onUconStartPoll           );
    connect(zbyrator, &MeterManager::onReadWriteOperation       , paramMedium, &ParametryzatorGuiMedium::onReadWriteOperation      );

//    connect(zbyrator, &MeterManager::onStatisticChanged         , paramMedium, &ParametryzatorGuiMedium::onStatisticChanged        );
//    connect(zbyrator, &MeterManager::onAllStatHash              , paramMedium, &ParametryzatorGuiMedium::onAllStatHash             );
    connect(zbyrator, &MeterManager::onTaskTableChanged         , paramMedium, &ParametryzatorGuiMedium::onTaskTableChanged        );
    connect(zbyrator, &MeterManager::onTaskCanceled             , paramMedium, &ParametryzatorGuiMedium::onTaskCanceled            );
    connect(zbyrator, &MeterManager::meterRelayStatus           , paramMedium, &ParametryzatorGuiMedium::meterRelayStatus          );
//    connect(zbyrator, &MeterManager::meterRelayStatus           , paramMedium, &ParametryzatorGuiMedium::add2fileMeterRelayStatus  );
    connect(zbyrator, &MeterManager::meterDateTimeDstStatus     , paramMedium, &ParametryzatorGuiMedium::meterDateTimeDstStatus    );

    connect(zbyrator, &MeterManager::appendAppOut, this, &MainWindow::appendShowMessagePlain);

    connect(zbyrator, &MeterManager::command2extensionClient, paramMedium, &ParametryzatorGuiMedium::command2extensionClient   );
//    connect(zbyrator, &MeterManager::onAboutZigBee          , paramMedium, &ParametryzatorGuiMedium::onAboutZigBee      );
    connect(zbyrator, &MeterManager::relayStatusChanged     , paramMedium, &ParametryzatorGuiMedium::relayStatusChanged        );


    connect(zbyrator, &MeterManager::onConnectionStateChanged, guiHelper, &GuiHelper::setPbWriteEnableDisableSlot);// ReadEnableDisableSlot);

    connect(zbyrator, &MeterManager::requestToSwitchIgnoreCycles, paramMedium, &ParametryzatorGuiMedium::requestToSwitchIgnoreCycles);


//    connect(guiHelper, SIGNAL(mWrite2RemoteDev(quint16,QVariant,QWidget*)), paramMedium, SLOT(mWrite2RemoteDev(quint16,QVariant)));


    QTimer::singleShot(1111, thread, SLOT(start()) );
}

//------------------------------------------------------------------------------------

bool MainWindow::createThisWdiget(const int &tag)
{
    QWidget *w = nullptr;

     switch(tag){
     case STACK_PAGE_CONFIG   : w = createSTACK_PAGE_CONFIG()   ; break;
     case STACK_PAGE_IFACE    : w = createSTACK_PAGE_IFACE()    ; break;
     case STACK_PAGE_PROFILES : w = createSTACK_PAGE_PROFILES() ; break;
     case STACK_PAGE_HISTORY  : w = createSTACK_PAGE_HISTORY()  ; break;
     case STACK_PAGE_PRINTING : w = createSTACK_PAGE_PRINTING() ; break;
     case STACK_PAGE_LOG      : w = createSTACK_PAGE_LOG()      ; break;
     case STACK_PAGE_NISRCHR  : w = createSTACK_PAGE_NISRCHR()  ; break;

     }

     if(w == nullptr)
         return false;

     if(!readyWdgts.contains(tag)){
         const QString realPageName = QString::number(tag);
         w->setAccessibleName(realPageName);

         QWidget *sa = StackWidgetHelper::addWdgtWithScrollArea(this, w, realPageName);


         readyWdgts.insert(tag, sa);
         ui->stackedWidget->addWidget(sa);
     }else
         w->deleteLater();
     return true;
}

//------------------------------------------------------------------------------------

void MainWindow::createActions()
{
    ui->mainToolBar->addAction(addAction2bar(tr("Config"), ":/katynko/svg/go-home.svg", STACK_PAGE_CONFIG));
    ui->mainToolBar->addAction(addAction2bar(tr("Interface"), ":/katynko/svg/network-disconnect.svg", STACK_PAGE_IFACE));
    ui->mainToolBar->addAction(addAction2bar(tr("Profiles"), ":/katynko/svg2/configure.svg", STACK_PAGE_PROFILES));
    ui->mainToolBar->addAction(addAction2bar(tr("History"), ":/katynko/svg/format-list-ordered.svg", STACK_PAGE_HISTORY));
    ui->mainToolBar->addAction(addAction2bar(tr("Printing"), ":/katynko/svg/document-print.svg", STACK_PAGE_PRINTING));
    ui->mainToolBar->addAction(addAction2bar(tr("Search"), ":/katynko/svg/edit-find.svg", STACK_PAGE_NISRCHR));

    ui->mainToolBar->addAction(addAction2bar(tr("Log"), ":/katynko/svg/view-list-text.svg", STACK_PAGE_LOG));


//#define STACK_PAGE_CONFIG   0
//#define STACK_PAGE_IFACE    1
//#define STACK_PAGE_PROFILES 2
//#define STACK_PAGE_HISTORY  3
//#define STACK_PAGE_PRINTING 4
//#define STACK_PAGE_LOG      5


}

//------------------------------------------------------------------------------------

void MainWindow::createAppOutLog()
{
    setupAppLogAction(ui->actionApplication_log, ui->widget4log, true);//use font for log

}

//------------------------------------------------------------------------------------

void MainWindow::createSqliteServer()
{
    ParametryzatorProcessManager *m = new ParametryzatorProcessManager;
    QThread *t = new QThread;

    t->setObjectName("ParametryzatorProcessManagerz");
    m->moveToThread(t);

//    connect(this, &MainWindow::destroyed, m, &ZbyratorProcessManager::deleteLater);
    connect(this, &MainWindow::receivedKillSignal, m, &ParametryzatorProcessManager::kickOffAllObjects);

    connect(m, SIGNAL(destroyed(QObject*)), t, SLOT(quit()) );

    connect(t, &QThread::finished, t, &QThread::deleteLater);

    connect(t, SIGNAL(started()), m, SLOT(onThreadStarted()) );

    QTimer::singleShot(1111, t, SLOT(start()));
}

//------------------------------------------------------------------------------------

QWidget *MainWindow::createSTACK_PAGE_CONFIG()
{
    ParamertyzatorConfigTabWdgt *w = new ParamertyzatorConfigTabWdgt(guiHelper, this);



    connect(paramMedium, &ParametryzatorGuiMedium::onIfaceSett, w, &ParamertyzatorConfigTabWdgt::onIfaceChanged);
    connect(paramMedium, &ParametryzatorGuiMedium::setIfaceSett, w, &ParamertyzatorConfigTabWdgt::onIfaceChanged);
    connect(paramMedium, &ParametryzatorGuiMedium::onPollCodeChanged, w, &ParamertyzatorConfigTabWdgt::onPollCodeChanged);



//    connect(paramMedium, SIGNAL(appendData2model(QVariantHash)), w, SLOT(setPageSett(QVariantHash)) );
    connect(paramMedium, &ParametryzatorGuiMedium::onUCLastReceivedDeviceRecordsQuickCollectChanged, w, &ParamertyzatorConfigTabWdgt::onUCLastReceivedDeviceRecordsQuickCollectChanged);
    connect(w, &ParamertyzatorConfigTabWdgt::onPollStartedV2, paramMedium, &ParametryzatorGuiMedium::onPollStarted);


    connect(w, &ParamertyzatorConfigTabWdgt::onPageCanReceiveData , paramMedium, &ParametryzatorGuiMedium::sendMeIfaceSett           );
    connect(w, SIGNAL(startOperation(quint16,QVariantMap)), paramMedium, SLOT(command4devSlot(quint16,QVariantMap)));// &ConfigTabWdgt::startOperation       , paramMedium, &ParametryzatorGuiMedium::o            );
    connect(w, &ParamertyzatorConfigTabWdgt::onPollStarted, paramMedium, &ParametryzatorGuiMedium::onPollStartedSmpl);
    connect(w, &ParamertyzatorConfigTabWdgt::stopAll              , paramMedium, &ParametryzatorGuiMedium::killUconTasks);

    connect(w, &ParamertyzatorConfigTabWdgt::onPageCanReceiveData , this, &MainWindow::initDone       );
    connect(w, &ParamertyzatorConfigTabWdgt::openIfaceSett        , this, &MainWindow::openIfaceSett  );

    connect(paramMedium, SIGNAL(onReadWriteOperation(bool)), w, SIGNAL(onReadWriteOperation(bool)) );
    connect(paramMedium, &ParametryzatorGuiMedium::ifaceLogStrFromZbyrator, w, &ParamertyzatorConfigTabWdgt::appendLogLines);

    connect(paramMedium, &ParametryzatorGuiMedium::ndtNi, w, &ParamertyzatorConfigTabWdgt::ndtNi);


    connect(paramMedium, &ParametryzatorGuiMedium::setPageSettParametryzatorProfile, w, &ParamertyzatorConfigTabWdgt::setPageSettParametryzatorProfile);
    connect(w, &ParamertyzatorConfigTabWdgt::onReloadParametryzatorProfiles, paramMedium, &ParametryzatorGuiMedium::onReloadParametryzatorProfiles);;

    connect(this, &MainWindow::onRequest2pollThese2wdgt, w, &ParamertyzatorConfigTabWdgt::onRequest2pollThese2wdgt);

    connect(w, &ParamertyzatorConfigTabWdgt::onRequest2GetDataThese, this, &MainWindow::onRequest2GetDataThese);//get date

    connect(w, &ParamertyzatorConfigTabWdgt::gimmeMainAndGroupSettings, this, &MainWindow::gimmeMyLastSettingsPrint);
    connect(this, &MainWindow::setPageSettpPrintingGrp, w, &ParamertyzatorConfigTabWdgt::setPageSettpPrintingGrp);
    connect(this, &MainWindow::setPageSettpPrintingMain, w, &ParamertyzatorConfigTabWdgt::setPageSettpPrintingMain);

    connect(w, &ParamertyzatorConfigTabWdgt::startPrintingTariffReports, this, &MainWindow::startPrintingTariffReports);

    //void startOperation(quint16 pollcode, QVariantMap params)


    connect(w, SIGNAL(startOperation(quint16,QVariantMap)), w, SIGNAL(checkPrintingWdgt()));

    connect(w, &ParamertyzatorConfigTabWdgt::checkPrintingWdgt, [=]{
        if(!readyWdgts.contains(STACK_PAGE_HISTORY))
            createThisWdiget(STACK_PAGE_HISTORY);
        if(!readyWdgts.contains(STACK_PAGE_PRINTING))
            createThisWdiget(STACK_PAGE_PRINTING);

    });

    connect(w, &ParamertyzatorConfigTabWdgt::setNewConsumerAddress, this, &MainWindow::setNewConsumerAddress);
    connect(this, &MainWindow::onConsumerAddressChanged, w, &ParamertyzatorConfigTabWdgt::onConsumerAddressChanged);
    connect(w, &ParamertyzatorConfigTabWdgt::openWorkingDirectorySettings, this, &MainWindow::on_actionPrint_triggered);

    connect(this, &MainWindow::receivedKillSignal, w, &ParamertyzatorConfigTabWdgt::kickOffAllObjects);

    return w;


}

//------------------------------------------------------------------------------------

QWidget *MainWindow::createSTACK_PAGE_IFACE()
{

    ZbyrIfaceSett * w = new ZbyrIfaceSett(false, tr("Communication settings"),guiHelper, this);

    connect(paramMedium, SIGNAL(setIfaceSett(QVariantHash)), w, SLOT(setPageSett(QVariantHash)) );
//    connect(paramMedium, SIGNAL(setTcpClientCompliter(QStringList)), w, SLOT(setTcpClientCompliter(QStringList)));
    connect(paramMedium, &ParametryzatorGuiMedium::setTcpClientCompliter, w, &ZbyrIfaceSett::setTcpClientCompliter);

    connect(w, SIGNAL(setNewSettings(QVariantHash)), paramMedium, SLOT(setNewSettings(QVariantHash)));
    connect(w, SIGNAL(sendMeIfaceSett()), paramMedium, SLOT(sendMeIfaceSett()) );
    connect(w, SIGNAL(setNewSettings(QVariantHash)), this, SLOT(showLastWdgt()) );
    connect(w, SIGNAL(closeThisPage())              , this, SLOT(showLastWdgt()));


    connect(w, SIGNAL(sendMeTheTcpHistory(QLineEdit*))      , paramMedium, SLOT(openTcpServerDlg(QLineEdit*)) );
    connect(w, SIGNAL(sendMeTheM2mHistory(QLineEdit*))      , paramMedium, SLOT(openM2mDlg(QLineEdit*)));
    connect(w, SIGNAL(openEditMacProfileWdgt(bool,QLineEdit*)), this, SLOT(openEditMacProfileWdgt(bool,QLineEdit*)));


    connect(w, &ZbyrIfaceSett::onPageActivated, w, &ZbyrIfaceSett::gimmeSerialPorts);

    return w;
}

//------------------------------------------------------------------------------------

QWidget *MainWindow::createSTACK_PAGE_PROFILES()
{
    ParametryzationProfilesWdgt *w = new ParametryzationProfilesWdgt(true, guiHelper, this);

    connect(paramMedium, &ParametryzatorGuiMedium::setPageSettParametryzatorProfile, w, &ParametryzationProfilesWdgt::setPageSettParametryzatorProfile);
    connect(w, &ParametryzationProfilesWdgt::onReloadParametryzatorProfiles, paramMedium, &ParametryzatorGuiMedium::onReloadParametryzatorProfiles);;
    connect(w, &ParametryzationProfilesWdgt::onDeleteParametryzatorProfileTable, paramMedium, &ParametryzatorGuiMedium::onDeleteParametryzatorProfileTable);
    connect(w, &ParametryzationProfilesWdgt::onParametryzatorProfileTableContentChanged, paramMedium, &ParametryzatorGuiMedium::onParametryzatorProfileTableContentChanged);

    return w;

}

//------------------------------------------------------------------------------------

QWidget *MainWindow::createSTACK_PAGE_HISTORY()
{
    GetParametryzatorHistory *w = new GetParametryzatorHistory(guiHelper, this);

//data from db must go via ucDeviceTreeW

//    connect(w, SIGNAL(data2dbMediumExt(quint16,QVariant)), paramMedium, SIGNAL(data2dbMedium(quint16,QVariant)) );
    connect(w, &GetParametryzatorHistory::stopDbReading, paramMedium, &ParametryzatorGuiMedium::stopReadDatabase);

    connect(paramMedium, SIGNAL(setLblWaitTxtDatabase(QString)), w, SIGNAL(setLblWaitTxtDatabase(QString)));
//    connect(paramMedium, SIGNAL(appendDataDatabase(QVariantHash)), w, SLOT(setPageSett(QVariantHash)));


    connect(this, &MainWindow::onRequest2GetDataThese, w, &GetParametryzatorHistory::onRequest2pollThese);

    connect(w, &GetParametryzatorHistory::onRequest2GetDataTheseFromDb, this, &MainWindow::onRequest2GetDataThese);//get date
    connect(w, &GetParametryzatorHistory::onRequest2pollTheseFromDb, this, &MainWindow::onRequest2pollThese );//start a new poll

    connect(w, &GetParametryzatorHistory::startPrintingTariffReports, this, &MainWindow::startPrintingTariffReports);


    connect(w, &GetParametryzatorHistory::pageEndInit, [=]{

        if(!readyWdgts.contains(STACK_PAGE_PRINTING))
            createThisWdiget(STACK_PAGE_PRINTING);

    });


    return w;

}

//------------------------------------------------------------------------------------

QWidget *MainWindow::createSTACK_PAGE_PRINTING()
{
    PrintParametryzator *w = new PrintParametryzator(guiHelper, this);

    connect(w, &PrintParametryzator::gimmeMyLastSettings, this, &MainWindow::gimmeMyLastSettingsPrint);


    connect(w, &PrintParametryzator::setLastPageSett, this, &MainWindow::setLastPageSettPrint);

    connect(this, &MainWindow::setPrintingSettings, w, &PrintParametryzator::setPrintingSettings);
    connect(this, &MainWindow::setPageSettpPrintingGrp, w, &PrintParametryzator::setPageSettpPrintingGrp);
    connect(this, &MainWindow::setPageSettpPrintingMain, w, &PrintParametryzator::setPageSettpPrintingMain);

    connect(this, &MainWindow::startPrintingTariffReports, w, &PrintParametryzator::startPrintingTariffReports);

    connect(this, &MainWindow::startPrintingTariffReports, [=]{
        onShowThisWdiget(STACK_PAGE_PRINTING);
    });


    connect(w, &PrintParametryzator::openPrintSettings, this, &MainWindow::on_actionPrint_triggered);

    connect(w, SIGNAL(gimmeMyLastSettingsPrintHtmlProfiles()), this, SLOT(gimmeMyLastSettingsPrintHtmlProfiles()));

    connect(this, &MainWindow::setPageSettpPrintingHtml, w, &PrintParametryzator::setPageSettpPrintingHtml);

    connect(this, &MainWindow::setNewConsumerAddress, w, &PrintParametryzator::setNewConsumerAddress);
    connect(w, &PrintParametryzator::onConsumerAddressChanged, this, &MainWindow::onConsumerAddressChanged);
    return w;
}

//------------------------------------------------------------------------------------

QWidget *MainWindow::createSTACK_PAGE_LOG()
{
    SmplPteWdgt *w = new SmplPteWdgt(tr("Log"), true, true, guiHelper, false, this);
//    connect(this, SIGNAL(appendShowMess(QString)), w, SLOT(appendHtml(QString)) );
//    connect(this, SIGNAL(appendShowMessPlain(QString)), w, SLOT(appendPteText(QString)));
//    connect(guiHelper, SIGNAL(appendShowMess(QString)), w, SLOT(appendHtml(QString)));

    connect(w, &SmplPteWdgt::giveMeYourCache, paramMedium, &ParametryzatorGuiMedium::giveMeYourCache);
    connect(paramMedium, &ParametryzatorGuiMedium::ifaceLogStr, w, &SmplPteWdgt::appendPteText);
    w->appendPteText("some test text");
    w->showClearButton(true);
    return w;
}

//------------------------------------------------------------------------------------

QWidget *MainWindow::createSTACK_PAGE_NISRCHR()
{
    ParamNISearcherWdgt *w = new ParamNISearcherWdgt(guiHelper, this);
    connect(w, &ParamNISearcherWdgt::gimmeSettings, this, &MainWindow::gimmeMyLastSettingsPrint);
    connect(w, &ParamNISearcherWdgt::openWorkingDirectorySettings, this, &MainWindow::on_actionPrint_triggered);

    connect(this, &MainWindow::setPageSettpPrintingGrp, w, &ParamNISearcherWdgt::setPageSettpPrintingGrp );
    connect(this, &MainWindow::setPageSettpPrintingMain, w, &ParamNISearcherWdgt::setPageSettpPrintingMain);


    connect(this, &MainWindow::receivedKillSignal, w, &ParamNISearcherWdgt::kickOffAllObjects);
    connect(this, &MainWindow::startPrintingTariffReports, [=]{
        onShowThisWdiget(STACK_PAGE_PRINTING);
    });

    return w;
}


//------------------------------------------------------------------------------------

QAction *MainWindow::addAction2bar(const QString &text, const QString &icon, const int &wdgtTag)
{
    QAction *a = new QAction(QIcon(icon), text, this);
    a->setData(wdgtTag);
    connect(a, SIGNAL(triggered(bool)), this, SLOT(onActClicked()));
    return a;
}


//------------------------------------------------------------------------------------

void MainWindow::on_actionPrint_triggered()
{
    ParametryzatorPrintSettingsWdgt *w = new ParametryzatorPrintSettingsWdgt(guiHelper, this);

    connect(ui->actionPrint, SIGNAL(triggered(bool)), w, SLOT(deleteLater()));

    connect(w, SIGNAL(gimmeMyLastSettingsPrintMain()), this, SLOT(gimmeMyLastSettingsPrintMain()));
    connect(w, SIGNAL(gimmeMyLastSettingsPrintHtmlProfiles()), this, SLOT(gimmeMyLastSettingsPrintHtmlProfiles()));

    connect(this, &MainWindow::setPageSettpPrintingMain, w, &ParametryzatorPrintSettingsWdgt::setPageSettpPrintingMain);

    connect(this, &MainWindow::setPageSettpPrintingHtml, w, &ParametryzatorPrintSettingsWdgt::setPageSettpPrintingHtml);


    connect(w, &ParametryzatorPrintSettingsWdgt::setLastPageSettPrintMain, [=](const QVariantHash mainprintsett){
        SettLoader::saveSett(SETT_PARAMETRYZATOR_PRINTMN, mainprintsett);
        QTimer::singleShot(111, this, SLOT(gimmeMyLastSettingsPrintMain()) );
    });


    connect(w, &ParametryzatorPrintSettingsWdgt::setLastPageSettPrintHtml, [=](const QVariantHash &htmlsett){
        SettLoader::saveSett(SETT_PARAMETRYZATOR_PROFILES, htmlsett);
        QTimer::singleShot(111, this, SLOT(gimmeMyLastSettingsPrintHtmlProfiles()) );
    });
//    connect(w, SIGNAL(gimmeMyLastSettingsPrintHtmlProfiles()), this, SLOT(gimmeMyLastSettingsPrint()));

//    emit setPrintingSettings(SettLoader::loadSett(SETT_PARAMETRYZATOR_PRINTONE).toHash());
//    emit setPageSettpPrintingGrp(SettLoader::loadSett(SETT_PARAMETRYZATOR_PRINTGRP).toHash());
//    emit setPageSettpPrintingMain(h);


    ui->stackedWidget->addWidget(w);
    ui->stackedWidget->setCurrentWidget(w);
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), w, SLOT(deleteLater()));
    emit addWdgt2history();
}

void MainWindow::on_actionPoll_triggered()
{
//    onShowThisWdiget(STACK_PAGE_POLLSETT);

    ParamPollWdgtDad *w = new ParamPollWdgtDad(guiHelper,  this);

    connect(w, SIGNAL(destroyed(QObject*)), paramMedium, SIGNAL(reloadSettings()));

    addWdgt2stackWdgt(w, WDGT_TYPE_OPTIONS, false, ui->actionPoll->text(), ":/katynko/svg2/kt-queue-manager.svg");



}
