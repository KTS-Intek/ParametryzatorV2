#include "getparametryzatorhistory.h"


#include "parametryzationprofileviewer.h"


///[!] dataprocessing
#include "src/matilda/devicepollcodeselectorhelper.h"


///[!] widgets-shared
#include "dataconcentrator-pgs/dbdataform.h"




//---------------------------------------------------------------------------

GetParametryzatorHistory::GetParametryzatorHistory(GuiHelper *gHelper, QWidget *parent) :
    DatabaseWdgt4QuickCollect(gHelper, parent)
{
    lastIsATariffPollCode = false;
    viewmedium = new ParametryzatorViewMedium(gHelper, false, this);

    connect(viewmedium, &ParametryzatorViewMedium::startPrintingTariffReports, this, &GetParametryzatorHistory::startPrintingTariffReports);
    connect(viewmedium, &ParametryzatorViewMedium::createProfileViewer, this, &GetParametryzatorHistory::createProfileViewer);

    connect(gHelper->ucDeviceTreeW, &UCDeviceTreeWatcher::onGetUCDataBaseData, this, &GetParametryzatorHistory::onGetUCDataBaseData);
//    connect(this, &GetParametryzatorHistory::data2dbMedium, this, &GetParametryzatorHistory::data2dbMediumSlot);

}

//---------------------------------------------------------------------------

MyPollCodeList GetParametryzatorHistory::getLvIconsAndTexts(const int &version)
{
    Q_UNUSED(version);

    MyPollCodeList l;

    l.append(PollCodeName2data(tr("Electricity meter"), UC_METER_ELECTRICITY , ":/katynko/svg4/sc_symbolshapes.lightning.svg"    ));
    //do not use to select from DB, it can make you a problem because of meter journal
//    l.append(PollCodeName2data(tr("EMB Modem")        , UC_METER_EMB_MODEM    , ":/katynko/svg3/relay-load-command.svg"    ));


    l.append(PollCodeName2data(tr("Other")  , UC_METER_UNKNOWN     , ":/katynko/svg4/sc_flowchartshapes.flowchart-summing-junction.svg" ));

    return l;
}

//---------------------------------------------------------------------------

OneDevicePollCodes GetParametryzatorHistory::getDeviceSelectSett4adev(const int &devtype)
{
    OneDevicePollCodes codes;

    bool isUnknownDevType = false;
    switch(devtype){
    case UC_METER_ELECTRICITY   : codes = DevicePollCodeSelectorHelper::getElectricityMetersSelectSett4parametryzator(); break;

//    case UC_METER_EMB_MODEM     : codes = DevicePollCodeSelectorHelper::getEMBSelectSett()                  ; break;

//    case UC_METER_WATER         : codes = DevicePollCodeSelectorHelper::getWaterMetersSelectSett()        ; break;
//    case UC_METER_FF_RELAY      : codes = DevicePollCodeSelectorHelper::getRelayDevicesSelectSett()       ; break;
//    case UC_METER_FF_GROUP      : codes = DevicePollCodeSelectorHelper::getLCUgroupsSelectSett()          ; break;
//    case UC_METER_FF_LAMP       : codes = DevicePollCodeSelectorHelper::getLCUSelectSett()                ; break;
    case UC_METER_UNKNOWN       : codes = DevicePollCodeSelectorHelper::getAllDevicesSelectSett()         ; break;
    default: isUnknownDevType = true; break;
    }

    if(!isUnknownDevType)
        return codes;

    codes.currentPollCode = 0;
    codes.pollcodesetts.insert(0, OnePollCodeSett());
    codes.pollcodesnames.append(PollCodeName2data("name", 0, "iconstr"));
//    codes.datetimesett = SelectDateTimeSett() is is already ready for using

    return codes;
}


//---------------------------------------------------------------------------

//void GetParametryzatorHistory::thisRowClicked(const int &srcrow, const int &proxyrow, const int &code, QTableView *tparent)
//{
//    Q_UNUSED(srcrow);
//    Q_UNUSED(proxyrow);
//    viewmedium->onShowClickedSett(code, tparent);
//}

MTableFullHouse GetParametryzatorHistory::getThisTvTariffSettExt(QTableView *tv, const QModelIndex &index)
{
    auto *proxy = TableViewHelper::proxyModelFromTv(tv);

    QList<int> srcRows;
    srcRows.append(proxy->mapToSource(index).row());

    QStandardItemModel *model = TableViewHelper::modelFromTv(tv);

    QMap<int, QList<int> > mapColumns2data;
    mapColumns2data.insert(1, QList<int>() << NPRINTABLE_USER);

    return StandardItemModelHelper::getModelDataExt(model, srcRows, mapColumns2data);
}


//MTableFullHouse GetParametryzatorHistory::getThisTvTariffSettExt(QTableView *tv, const int &code, const bool &onlycurrentrow)
//{
//    Q_UNUSED(code);




//    MySortFilterProxyModel *proxy = 0;
//    bool everythingisfine = false;
//    if(model){
//        proxy = TableViewHelper::proxyModelFromTv(tv);
//        if(proxy){
//            everythingisfine = true;
//        }
//    }
//    if(!everythingisfine)
//        return MTableFullHouse();

//    QList<int> srcrows2get;
//    if(onlycurrentrow){
//        srcrows2get.append(proxy->mapToSource(tv->currentIndex()).row());
//    }else{
//        srcrows2get = TableViewHelper::getSelectedRows(tv, 0, false);
//    }

//    MTableFullHouse out;
//    const bool hasRelayTest = (code == POLL_CODE_WRITE_TARIFF);

//    for(int i = 0, imax = srcrows2get.size(); i < imax; i++){
//        const int row = srcrows2get.at(i);
//        table.append(StandardItemModelHelper::getRowText(row, -1, model));

//        QVariantHash profile;
//        //dst data is col6
//        const QVariantHash dst = ParemetryzatorData2userReadable::hashFromJSONlineBase64(model->item(row,6)->data().toString());
//        if(!dst.isEmpty())
//            profile.insert("dst", dst);
//        //tariff program is col7
//        const QVariantHash tariff = ParemetryzatorData2userReadable::hashFromJSONlineBase64(model->item(row,7)->data().toString());
//        if(!tariff.isEmpty())
//            profile.insert("tariff", tariff);
//        //display is col8
//        const QString display = model->item(row,8)->data().toString();
//        if(!display.isEmpty() && display != "-")
//            profile.insert("display", display);

//        //relay is col9
//        const QString relay = model->item(row,9)->data().toString();
//        if(!relay.isEmpty() && relay != "-")
//            profile.insert("relay", relay);

//        const int thelastcolumn = hasRelayTest ? 12 : 11;
//        if(hasRelayTest){
//            const QString relaytest = model->item(row,10)->data().toString();
//            if(!relaytest.isEmpty() && relaytest != "-")
//                profile.insert("relay-test", relaytest);
//        }

//        const QVariantHash relaylimits = ParemetryzatorData2userReadable::hashFromJSONline(model->item(row,thelastcolumn)->data().toString()).value("relaylimits").toHash();
//        if(!relaylimits.isEmpty())
//            profile.insert("relaylimits", relaylimits);
//        out.append(profile);

//    }
    //    return out;
//}

void GetParametryzatorHistory::onThisDbDataFormV3IsGoingToBeAdded(DbDataFormV3 *w)
{
    connect(w, &DbDataFormV3::onThisWidgetIsAddedAndReady, this, &GetParametryzatorHistory::onThisWidgetIsAddedAndReady);
}


//---------------------------------------------------------------------------

//void GetParametryzatorHistory::data2dbMediumSlot(quint16 command, QVariant varData)
//{
//    QVariantHash h = varData.toHash();
////    const int code = h.value("code").toInt();
////    if(code == POLL_CODE_WRITE_TARIFF){
////        h.insert("g", true);
////    }

//    emit data2dbMediumExt(command, QVariant(h));
//}

void GetParametryzatorHistory::createProfileViewer(QString title)
{

    ParametryzationProfileViewer *viewer = new ParametryzationProfileViewer(false, gHelper);
    connect(viewmedium, &ParametryzatorViewMedium::showThisProfile, viewer, &ParametryzationProfileViewer::setThisPagesSett);


    connect(viewer, &ParametryzationProfileViewer::onWidgetsReady, this, &GetParametryzatorHistory::onParamViewerReady);


//    viewer->show();
    viewer->setWindowTitle(title);
    viewer->setWindowIcon(this->windowIcon());


    QTimer::singleShot(111, viewer, SLOT(show()));

    QTimer::singleShot(222, viewer, SLOT(raise()));

    //    qDebug() << "onShowSett " << ldata.first() << tabletext.first();
}

void GetParametryzatorHistory::onParamViewerReady()
{
    ParametryzationProfileViewer *viewer = qobject_cast<ParametryzationProfileViewer*>(QObject::sender());

    viewmedium->onReady2showLastProfile();

    if(viewer){
        disconnect(viewmedium, &ParametryzatorViewMedium::showThisProfile, viewer, &ParametryzationProfileViewer::setThisPagesSett);
    }
}

void GetParametryzatorHistory::onGetUCDataBaseData(UCSelectFromDB select, QString senderName)
{
    QString mynamee = accessibleName();
    if(senderName != mynamee)
        return;
    lastIsATariffPollCode = (select.pollCode == POLL_CODE_WRITE_TARIFF || select.pollCode == POLL_CODE_READ_TARIFF);
}

void GetParametryzatorHistory::onThisWidgetIsAddedAndReady(DbDataFormSmplV2 *w)
{
    //it emits after pageEndInit

    if(!lastIsATariffPollCode)
        return;




    w->setSpecialCustomMenu(viewmedium->getActionl());

    connect(w, &DbDataFormSmplV2::onSpecialCustomActionClickedData, viewmedium, &ParametryzatorViewMedium::onSpecialCustomActionClickedData);

    connect(w->lastTv, &QTableView::doubleClicked, this, &GetParametryzatorHistory::onLastTvClicked);

}


void GetParametryzatorHistory::onLastTvClicked(const QModelIndex &index)
{
    QTableView *lastTv = qobject_cast<QTableView *>(QObject::sender());
    if(lastTv){
        viewmedium->onShowClickedSett(getThisTvTariffSettExt(lastTv, index));
    }
}
