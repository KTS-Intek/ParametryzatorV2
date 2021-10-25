#include "paramertyzatorconfigtabwdgt.h"


///[!] parametryzator-v2
#include "parametryzatorstartpage.h"
#include "parametryzationprofileviewer.h"

#include "parametryzator-wdgts/getparametryzatorhistory.h"

///[!] widgets-meters
#include "dataconcetrator-pgs/src/databasewdgtv3.h"

///[!] guisett-shared-core
//#include "src/nongui/pollcodesoperations.h"

///[!] guisett-shared-core
#include "src/nongui/tabkeys4lordofifaces.h"


#include "definedpollcodes.h"

ParamertyzatorConfigTabWdgt::ParamertyzatorConfigTabWdgt(GuiHelper *gHelper, QWidget *parent) :
    TabWidget4config(gHelper, parent)
{
    viewmedium = new ParametryzatorViewMedium(gHelper, true, this);

    connect(viewmedium, &ParametryzatorViewMedium::startPrintingTariffReports, this, &ParamertyzatorConfigTabWdgt::startPrintingTariffReports);
    connect(viewmedium, &ParametryzatorViewMedium::createProfileViewer, this, &ParamertyzatorConfigTabWdgt::createProfileViewer);

    connect(this, &ParamertyzatorConfigTabWdgt::setConsumerAddressFromWriteTariff, viewmedium, &ParametryzatorViewMedium::setConsumerAddressFromWriteTariff);
}

void ParamertyzatorConfigTabWdgt::createStartPage()
{
    ParametryzatorStartPage *w = new ParametryzatorStartPage(gHelper, this);
    addWidget2tab(w, ":/katynko/svg2/configure.svg", tr("Operations"));
    w->setAccessibleName(doNotClose());




    connect(this, SIGNAL(setPageSettParametryzatorProfile(MyListStringList,QVariantMap,QStringList,QStringList,bool)),
            w, SLOT(setPageSettParametryzatorProfileSlot(MyListStringList,QVariantMap,QStringList,QStringList,bool)));

    connect(w, SIGNAL(onPageCanReceiveData()), this, SIGNAL(onPageCanReceiveData()));

    connect(w, &ParametryzatorStartPage::startOperation, this, &ParamertyzatorConfigTabWdgt::onStartOperation);


    connect(this, &ParamertyzatorConfigTabWdgt::ndtNi, w, &ParametryzatorStartPage::ndtNi );


    connect(this, &ParamertyzatorConfigTabWdgt::setPageSettParametryzatorProfile, w, &ParametryzatorStartPage::setPageSettParametryzatorProfile);
    connect(w, &ParametryzatorStartPage::onReloadParametryzatorProfiles, this, &ParamertyzatorConfigTabWdgt::onReloadParametryzatorProfiles);

    connect(this, &ParamertyzatorConfigTabWdgt::onRequest2pollThese2wdgt, w, &ParametryzatorStartPage::onRequest2pollThese2wdgt);

    connect(this, &ParamertyzatorConfigTabWdgt::onRequest2pollThese2wdgt, [=]{
        tbwDevices->setCurrentWidget(w);
    });
//        w, &ParametryzatorStartPage::onRequest2pollThese2wdgt);


    //it is necessary for NI checker
    connect(w, &ParametryzatorStartPage::gimmeMainAndGroupSettings, this, &ParamertyzatorConfigTabWdgt::gimmeMainAndGroupSettings);
    connect(this, &ParamertyzatorConfigTabWdgt::setPageSettpPrintingGrp, [=](const QVariantHash &h){
        if(w->lastprintsett.grphash != h){
            w->lastprintsett.grphash = h;
            w->lastprintsett.lastDirectoryPath.clear();
        }
    });

    connect(this, &ParamertyzatorConfigTabWdgt::setPageSettpPrintingMain, [=](const QVariantHash &h){
        if(w->lastprintsett.mainhash != h){
            w->lastprintsett.mainhash = h;
            w->lastprintsett.lastDirectoryPath.clear();
        }
    });
    connect(w, &ParametryzatorStartPage::setConsumerAddressFromWriteTariff, this, &ParamertyzatorConfigTabWdgt::setConsumerAddressFromWriteTariff);

    connect(w, &ParametryzatorStartPage::setNewConsumerAddress, this, &ParamertyzatorConfigTabWdgt::setNewConsumerAddress);
    connect(this, &ParamertyzatorConfigTabWdgt::onConsumerAddressChanged, w, &ParametryzatorStartPage::onConsumerAddressChanged);
    connect(w, &ParametryzatorStartPage::openWorkingDirectorySettings, this, &ParamertyzatorConfigTabWdgt::openWorkingDirectorySettings);

    connect(w, &ParametryzatorStartPage::checkPrintingWdgt, this, &ParamertyzatorConfigTabWdgt::checkPrintingWdgt);


    connect(this, &ParamertyzatorConfigTabWdgt::kickOffAllObjects, w, &ParametryzatorStartPage::kickOffAllObjects);

}

QStringList ParamertyzatorConfigTabWdgt::getEnrg4yourCode(const int &code)
{

    return ParametryzatorMedium::getEnrg4pollCodeWithJSONparametryzator(code);

}

void ParamertyzatorConfigTabWdgt::operationIsAboutToStart()
{
    //it does nothing, but do not change it
}

QList<QAction *> ParamertyzatorConfigTabWdgt::getLaAdditional(const bool &youHaveApower, const int &selItems, const int &pollcode, QTableView *tparent, const QString &fobjname)
{
    if(!lsettstate.isConnectedAddressChange){
        lsettstate.isConnectedAddressChange = true;
        connect(viewmedium, &ParametryzatorViewMedium::setConsumerAddressFromTheTab, this, &ParamertyzatorConfigTabWdgt::setNewConsumerAddress);
    }

    return QList<QAction *>();//viewmedium->getLaAdditional(youHaveApower, selItems, pollcode, tparent, fobjname);
}

void ParamertyzatorConfigTabWdgt::thisRowClicked(const int &srcrow, const int &proxyrow, const int &code, QTableView *tparent)
{
    Q_UNUSED(srcrow);
    Q_UNUSED(proxyrow);
//    viewmedium->onShowClickedSett(code, tparent);
}

void ParamertyzatorConfigTabWdgt::onTabCreated(const int &pollcode, DbDataForm *f)
{
    if(pollcode == POLL_CODE_WRITE_TARIFF)
        f->setObjectName(viewmedium->getObjName());

    const bool lastIsATariffPollCode = (pollcode == POLL_CODE_WRITE_TARIFF || pollcode == POLL_CODE_READ_TARIFF);

    connect(this, &ParamertyzatorConfigTabWdgt::onUCLastReceivedDeviceRecordsQuickCollectChanged, f, &DbDataForm::onUCLastReceivedDeviceRecordsChangedLocal);


    StartPollTabSettExt sett;
    f->myLastSelect.select.keyValues = (pollcode < POLL_CODE_MODEM_OPEREATIONS_END_EXT) ? TabKeys4lordOfIfaces::getListcolkeysLikeEnrg(pollcode) :
                                                                               getEnrg4yourCode(pollcode);
//    if(f->myLastSelect.select.keyValues.isEmpty())
    f->myLastSelect.select.keyValues.append("stts");//it must have at least one item

    sett.select = f->myLastSelect.select;


    sett.deviceType = f->myLastSelect.meterType;
    sett.ok = true;
    sett.txt = f->windowTitle();


    const MyLastSelectParamsV3 settext = DatabaseWdgtV3::fromStartPollTabSettExt(sett);

    f->setStreamParametersV3local(settext, lastWdgtAccssbltName);

    if(lastIsATariffPollCode){

        auto *w = f->dbDataFormMain;
        w->setSpecialCustomMenu(viewmedium->getActionl());
        connect(w, &DbDataFormSmplV2::onSpecialCustomActionClickedData, viewmedium, &ParametryzatorViewMedium::onSpecialCustomActionClickedData);

        if(w->getInitDone()){
            connect(w->lastTv, &QTableView::doubleClicked, this, &ParamertyzatorConfigTabWdgt::onLastTvClicked);
        }else{
            connect(w, &DbDataFormSmplV2::pageEndInit, [=]{
                connect(w->lastTv, &QTableView::doubleClicked, this, &ParamertyzatorConfigTabWdgt::onLastTvClicked);
            });
        }
    }



    emit onPollStartedV2(sett.select, lastWdgtAccssbltName, sett.select.keyValues, sett.deviceType);

}

void ParamertyzatorConfigTabWdgt::createProfileViewer(QString title)
{
    ParametryzationProfileViewer *viewer = new ParametryzationProfileViewer(false, gHelper);
    connect(viewmedium, &ParametryzatorViewMedium::showThisProfile, viewer, &ParametryzationProfileViewer::setThisPagesSett);


    connect(viewer, &ParametryzationProfileViewer::onWidgetsReady, this, &ParamertyzatorConfigTabWdgt::onParamViewerReady);

    viewer->show();
    viewer->setWindowTitle(title);
    viewer->setWindowIcon(this->windowIcon());
}

void ParamertyzatorConfigTabWdgt::onParamViewerReady()
{
    ParametryzationProfileViewer *viewer = qobject_cast<ParametryzationProfileViewer*>(QObject::sender());

    viewmedium->onReady2showLastProfile();

    if(viewer){
        disconnect(viewmedium, &ParametryzatorViewMedium::showThisProfile, viewer, &ParametryzationProfileViewer::setThisPagesSett);
    }
}

void ParamertyzatorConfigTabWdgt::onLastTvClicked(const QModelIndex &index)
{
    QTableView *lastTv = qobject_cast<QTableView *>(QObject::sender());
    if(lastTv){
        viewmedium->onShowClickedSett(GetParametryzatorHistory::getThisTvTariffSettExt(lastTv, index));
    }
}

//void ParamertyzatorConfigTabWdgt::checkEnableQrMode(DbDataForm *f)
//{
//    f->enabQrSettings(QR_DEVTYPE_EM, QList<int>() << -1 << -1 << 1);

//}
