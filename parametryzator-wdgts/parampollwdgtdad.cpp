#include "parampollwdgtdad.h"
#include "ui_parampollwdgtdad.h"


#include "src/nongui/settloader.h"
//#include "zbyrator-src/settloader4matildaemulator.h"
#include "src/matilda/settloader4matilda.h"

///[!] matilda-bbb-serverside-shared
#include "matilda-bbb-src/decoders/decodematildaprotocolv1.h"

///[!] matilda-bbb-clientside
//#include "src/matilda-conf/classmanagerv1.h"

ParamPollWdgtDad::ParamPollWdgtDad(GuiHelper *gHelper, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamPollWdgtDad)
{
    ui->setupUi(this);

    pollWdgt = new PollWdgt(gHelper, this);
    onReloadPageSettings();

//    connect(pollWdgt, SIGNAL(onPageCanReceiveData()), this, SLOT(onReloadPageSettings()));
    connect(pollWdgt, SIGNAL(onSettingsChanged()), this, SLOT(onSavePageSettings()));


    ignoreSave = true;

    ui->verticalLayout_2->addWidget(pollWdgt);
    ui->lblSaved->hide();
    pollWdgt->showOnlyTheseKeys(getOnlyKeys());
}

ParamPollWdgtDad::~ParamPollWdgtDad()
{
    saveAllForced();
    delete ui;
}

void ParamPollWdgtDad::on_pushButton_clicked()
{
    deleteLater();
}


QStringList ParamPollWdgtDad::getOnlyKeys()
{
    //    h.insert("mr"   , ui->sbMetrRetry->value()      );
    //    h.insert("mrfa" , ui->sbMetrRetryFA->value()    );
    //    h.insert("pw"   , ui->sbWati4Poll->value()      );
    //    h.insert("ha"   , ui->cbHardAddrsn->isChecked() );
    //    h.insert("frwrd", ui->cbEnblForward->isChecked());
    //    h.insert("af"   , ui->cbEnblAutoForward->isChecked());

    //    h.insert("w4e"  , ui->cbW4E->isChecked()        );
    //    h.insert("w4eRb", ui->sbW4ERtrBf->value()       );
    //    h.insert("w4eRa", ui->sbW4ErA->value()          );

    //    h.insert("tc"   , ui->cbTimeCorrection->isChecked() );
    //    h.insert("td"   , ui->sbMaxSecDiff->value()     );

    //    h.insert("enms", ui->cbxSortingOfTheMeterList->currentIndex());

    //    //v6
    //    if(ui->cbxLogRecording->isEnabled())
    //        h.insert("enLoggn", ui->cbxLogRecording->isChecked());

    //    //v7
    //    if(ui->cbxDefaultPollChannel->isEnabled())
    //        h.insert("dpllch", ui->cbxDefaultPollChannel->currentData().toInt());
    return QString("mr mrfa ha enLoggn ").split(" ", QString::SkipEmptyParts);
}


void ParamPollWdgtDad::onReloadPageSettings()
{
    const QVariantHash h = DecodeMatildaProtocolV1::readCOMMAND_READ_POLL_SETT().toHash();// SettLoader::loadSett(SETT_ZBRTR_POLLPAGE).toHash();
    ignoreSave = true;

    UCMeterPollSettings meterPollSettings;
    meterPollSettings.validator.dtlastupdate = QDateTime::currentDateTimeUtc();// = getTemplateValidatorAllDataIsReceived(QVariant(h));
    meterPollSettings.retryGeneral          = h.value("mr"    ).toInt();
    meterPollSettings.retryFA               = h.value("mrfa"  ).toInt();
    meterPollSettings.hardAddressing        = h.value("ha"    ).toBool();
    meterPollSettings.writeDeviceExchange = h.value("enLoggn").toBool();

    pollWdgt->gHelper->ucDeviceTreeW->setUCMeterPollSettings(meterPollSettings);

//     pollWdgt->setPageSett( h.isEmpty() ? SettLoader4matildaEmulator::defaultPollWdgtSett() : h );
    QTimer::singleShot(333, this, SLOT(unlockIgnore()));
}

void ParamPollWdgtDad::onSavePageSettings()
{
    if(ignoreSave)
        return;
    const QVariantHash h = DecodeMatildaProtocolV1::readCOMMAND_READ_POLL_SETT().toHash();// SettLoader::loadSett(SETT_ZBRTR_POLLPAGE).toHash();

    QVariantHash hh;// = pollWdgt->getPageSett(ok, mess, false);

    const auto settings = pollWdgt->getPageSettings();
    hh.insert("mr"   , settings.retryGeneral); // ui->sbMetrRetry->value()      );
    hh.insert("mrfa" , settings.retryFA );// ui->sbMetrRetryFA->value()    );
    hh.insert("ha"   , settings.hardAddressing );//ui->cbHardAddrsn->isChecked() );
    hh.insert("enLoggn", settings.writeDeviceExchange );//ui->cbxLogRecording->isChecked());





    if(h != hh){
//        SettLoader::saveSett(SETT_ZBRTR_POLLPAGE, hh);

        const QStringList onlyk = getOnlyKeys();
        QVariantHash outhash;
        for(int i = 0, imax = onlyk.size(); i < imax; i++)
            outhash.insert(onlyk.at(i), hh.value(onlyk.at(i)));

        DecodeMatildaProtocolV1::writeCOMMAND_WRITE_POLL_SETT(outhash);
        ui->lblSaved->show();
        QTimer::singleShot(3333, ui->lblSaved, SLOT(hide()));
    }

}

void ParamPollWdgtDad::unlockIgnore()
{
    ignoreSave = false;
}

void ParamPollWdgtDad::saveAllForced()
{
    unlockIgnore();
    onSavePageSettings();
}
