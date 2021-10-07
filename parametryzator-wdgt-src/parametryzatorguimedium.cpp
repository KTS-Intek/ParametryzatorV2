#include "parametryzatorguimedium.h"

#include <QTimer>
#include <QDateTime>


///[!] guisett-shared-core
#include "src/nongui/settloader.h"

///[!] guisett-shared
#include "zbyrator-src/src/startexchangehelper.h"


///[!] parametryzator-base
#include "parametryzator-src/parametryzatordatabasemedium.h"

///[!] Matilda-IO
#include "matilda-bbb-src/shared/pathsresolver.h"



///[!] zbyrator-shared
#include "src/meter/relaystatehelper.h"
#include "src/zbyrator-v2/quickpollhelper.h"

#include "matildaprotocolcommands.h"

//-------------------------------------------------------------------------

ParametryzatorGuiMedium::ParametryzatorGuiMedium(UCDeviceTreeWatcher *ucDeviceTreeW, QObject *parent) : GuiIfaceMedium(parent)
{
    this->ucDeviceTreeW = ucDeviceTreeW;

    ucDeviceTreeW->setConnectionState(UCCONNECT_STATE_CONNECTIONREADY);
    ucDeviceTreeW->setAccessLevel(MTD_USER_ADMIN);
    ucDeviceTreeW->setProtocolVersion(MATILDA_PROTOCOL_VERSION);

    setPbWriteDis(true);
    createDataCalculator();
    createDatabaseMedium();
    //    createLocalSocketObject();
}

void ParametryzatorGuiMedium::onPollStartedSmpl(quint8 pollCode, QStringList listEnrg, QString dateMask, int dotPos, bool allowDate2utc)
{
    UCSelectFromDB select;

    select.pollCode = pollCode;
    select.timeRange.msecFrom = QDateTime::currentMSecsSinceEpoch();
    select.keyValues = listEnrg;
//    select.selectionTag = ?

    emit onPollStarted(select, "start", listEnrg, (pollCode < POLL_CODE_MODEM_OPEREATIONS_END) ? UC_METER_EMB_MODEM : UC_METER_ELECTRICITY);

}

//-------------------------------------------------------------------------

void ParametryzatorGuiMedium::onReloadParametryzatorProfiles()
{
    const QVariantHash h = SettLoader::loadSett(SETT_PARAMETRYZATOR_TRFPRFLS).toHash();
    QList<QString> lk = h.keys();
    std::sort(lk.begin(), lk.end());

    MyListStringList listRows;

    QVariantMap mapdata;
    for(int i = 0, imax = lk.size(); i < imax; i++){
        QStringList l;
        l.append(lk.at(i));
        mapdata.insert(QString("%1;0").arg(i), h.value(lk.at(i)));
        listRows.append(l);
    }

    const QStringList header = tr("Profile").split("\r\n");

    emit setPageSettParametryzatorProfile(listRows, mapdata,
                                          header,
                                          header, true);

}

//-------------------------------------------------------------------------

void ParametryzatorGuiMedium::onParametryzatorProfileTableContentChanged(QString profname, QVariantHash profile)
{
    QVariantHash h = SettLoader::loadSett(SETT_PARAMETRYZATOR_TRFPRFLS).toHash();
    h.insert(profname, profile );
    SettLoader::saveSett(SETT_PARAMETRYZATOR_TRFPRFLS, h);
    QTimer::singleShot(111, this, SLOT(onReloadParametryzatorProfiles()));
}

//-------------------------------------------------------------------------

void ParametryzatorGuiMedium::onDeleteParametryzatorProfileTable(QStringList deleteThese)
{
    QVariantHash h = SettLoader::loadSett(SETT_PARAMETRYZATOR_TRFPRFLS).toHash();
    for(int i = 0, imax = deleteThese.size(); i < imax; i++)
        h.remove(deleteThese.at(i));
    SettLoader::saveSett(SETT_PARAMETRYZATOR_TRFPRFLS, h);
    QTimer::singleShot(111, this, SLOT(onReloadParametryzatorProfiles()));
}

//-------------------------------------------------------------------------

void ParametryzatorGuiMedium::onTaskCanceled(quint8 pollCode, QString ni, qint64 dtFinished, quint8 rez)
{

    if(rez == 0) // RD_EXCHANGE_DONE)
        return;
    const QString stts = StartExchangeHelper::getStts4rez(rez);

    emit appendAppLog(tr("%3: NI: %1, one task removed, rezult is %2").arg(ni).arg(stts).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));

//    const QDateTime dtLocal = QDateTime::fromMSecsSinceEpoch(dtFinished).toLocalTime();

    MyListHashString data;
    QHash<QString,QString> hh;
    hh.insert("msec", QString::number(dtFinished));
    hh.insert("stts", tr("canceled"));

    if(pollCode < POLL_CODE_MODEM_OPEREATIONS_END_EXT)
        hh.insert("device_ni", ni);

    data.append(hh);




    emit appendMeterData(ni, "", data);
}

void ParametryzatorGuiMedium::createDatabaseMedium()
{
    ParametryzatorDatabaseMedium *m = new ParametryzatorDatabaseMedium(ucDeviceTreeW);

    QThread *t = new QThread;
    m->moveToThread(t);

    connect(this, &ParametryzatorGuiMedium::killAllObjects, m, &ParametryzatorDatabaseMedium::kickOffObject);


    connect(m, SIGNAL(destroyed(QObject*)), t, SLOT(quit()) );
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));

    connect(t, SIGNAL(started()), m, SLOT(onThreadStarted()) );


//    connect(this, SIGNAL(data2dbMedium(quint16,QVariant)), m, SLOT(data2matilda4inCMD(quint16,QVariant)));
    connect(this, SIGNAL(stopReadDatabase()), m, SLOT(stopOperationSlot()) );

//    connect(this, &ParametryzatorGuiMedium::setDateMask, m, &ParametryzatorDatabaseMedium::setDateMask);
//    connect(this, &ParametryzatorGuiMedium::setDotPos, m, &ParametryzatorDatabaseMedium::setDotPos);

    connect(m, SIGNAL(setPbReadEnableDisable(bool)), this, SIGNAL(setPbReadEnableDisable(bool)));

//    connect(m, SIGNAL(appendDataDatabase(QVariantHash)), this, SIGNAL(appendDataDatabase(QVariantHash)));

    connect(m, SIGNAL(setLblWaitTxtDatabase(QString)), this, SIGNAL(setLblWaitTxtDatabase(QString)));


    t->start();
}

void ParametryzatorGuiMedium::setPbWriteDis(bool disabled)
{
    pbWriteDis = disabled;

}

void ParametryzatorGuiMedium::meterRelayStatus(QString ni, QDateTime dtLocal, quint8 mainstts, quint8 secondarystts)
{
    ///process and send to GUI
    qDebug() << "meterRelayStatus " << ni << dtLocal << mainstts << secondarystts;

}

void ParametryzatorGuiMedium::meterDateTimeDstStatus(QString ni, QDateTime dtLocal, QString stts)
{
    qDebug() << "meterDateTimeDstStatus " << ni << dtLocal << stts ;

}

void ParametryzatorGuiMedium::relayStatusChanged(QVariantMap map)
{
    qDebug() << "relayStatusChanged " << map;

}

void ParametryzatorGuiMedium::mWrite2RemoteDev(quint16 command, QVariant dataVar)
{//do I need this method?
    switch(command){
    case COMMAND_WRITE_ELMTRRELAY_OPERATION:{
        if(pbWriteDis)
            return;

        const QStringList listni = dataVar.toHash().value("nis").toStringList();
        if(listni.isEmpty()){
            emit showMessage(tr("no meters"));
            return;
        }
        const int operation = dataVar.toHash().value("mode").toInt();
        QString mess;
        const QVariantMap map = QuickPollHelper::createPollMap4relay(listni, operation, mess);

        if(map.isEmpty())
            return;

//        setLastPageId("Relay");

        command4devSlot(POLL_CODE_RELAY_OPERATIONS, map);
//        lrelay.hasRequestFromMeterList = true;
        break;}//realy operations, main relay
    case COMMAND_READ_ELMTRRELAY_TABLE:{
//        lrelay.hasRequestFromMeterList = true;
//        emit startTmrUpdateRelayStatuses(111);

        break;} //send last realy statuses
    }

    qDebug() << "onElMeterRelayChanged mWrite2RemoteDev " <<  command;
}

//-------------------------------------------------------------------------

void ParametryzatorGuiMedium::createDataCalculator()
{

    //for test only
    ParametryzatorDataCalculation *c = new ParametryzatorDataCalculation;

    QThread *t = new QThread;
    c->moveToThread(t);
    t->setObjectName("ParametryzatorDataCalculation");

    connect(this, SIGNAL(destroyed(QObject*)), t, SLOT(quit()) );
    connect(t, SIGNAL(started()), c, SLOT(onThreadStarted()) );

    connect(this, &ParametryzatorGuiMedium::appendMeterData , c, &ParametryzatorDataCalculation::appendMeterDataSmart  );
    connect(this, &ParametryzatorGuiMedium::onPollStarted   , c, &ParametryzatorDataCalculation::onPollStarted    );
    connect(this, &ParametryzatorGuiMedium::onUconStartPoll , c, &ParametryzatorDataCalculation::onUconStartPoll  );

    connect(this, &ParametryzatorGuiMedium::onMeterPollCancelled, c, &ParametryzatorDataCalculation::onMeterPollCancelled);

    connect(c, &ParametryzatorDataCalculation::onUCLastReceivedDeviceRecordsQuickCollectChanged, this, &ParametryzatorGuiMedium::onUCLastReceivedDeviceRecordsQuickCollectChanged);


//    connect(c, &ParametryzatorDataCalculation::appendData2modelProcessed, this, &ParametryzatorGuiMedium::appendData2model);

    connect(c, &ParametryzatorDataCalculation::setLblWaitTxt   , this, &ParametryzatorGuiMedium::setLblWaitTxt);

    t->start();

}

//-------------------------------------------------------------------------
