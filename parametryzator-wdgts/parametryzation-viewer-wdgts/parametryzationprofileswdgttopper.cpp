#include "parametryzationprofileswdgttopper.h"
#include "ui_parametryzationprofileswdgttopper.h"


#include <QDir>
#include <QDesktopServices>
#include <QCompleter>
#include <QDebug>
#include <QUrl>
#include <QTimer>
#include <QThread>

///[!] parametryzator-base
#include "parametryzator-src/parametryzatornisearcher.h"


//------------------------------------------------------------------------------------------------

ParametryzationProfilesWdgtTopper::ParametryzationProfilesWdgtTopper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametryzationProfilesWdgtTopper)
{
    ui->setupUi(this);

    createAllConnections();
}

//------------------------------------------------------------------------------------------------

ParametryzationProfilesWdgtTopper::~ParametryzationProfilesWdgtTopper()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::onConsumerAddressChanged(QString address, QString workingdirectory)
{
    if(!address.simplified().trimmed().isEmpty())
        ui->leConsumerAddress->setText(address);
//    ui->lblWorkingDirectory->setText(workingdirectory);

    if(lsett.workigndir != workingdirectory){
        lsett.workigndir = workingdirectory;
        recalculateNiWarningsLater();
    }

    ui->lblWorkingDirectory->setText(QString("<a href=\"%1\">%1").arg(workingdirectory));

    ui->tbUpdateCompleter->animateClick();
}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::sendCurrentConsumerAddress()
{
    const QString text = ui->leConsumerAddress->text();
    if(!text.isEmpty())
        emit setNewConsumerAddress(text);

}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::onNiListUpdated(QStringList nis)
{
    lsett.lastnis = nis;
    recalculateNiWarningsLater();
}

void ParametryzationProfilesWdgtTopper::recalculateNiWarningsLater()
{
    emit startTmrCalcNiWarns();

}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::recalculateNiWarnings()
{
    if(lsett.lastnis.isEmpty()){
        ui->lblWarnings->setText(tr("NI line is empty"));
        return;
    }

    if(lsett.isSearching){
        recalculateNiWarningsLater();
        return;//wait a moment
    }

    emit gimmeMainSettHash();


    lsett.isSearching = true;
    lsett.hNi2Counter.clear();

    QString lastworkingdir = lsett.mainsett.value("lePath2pdfDir").toString();

    emit startSearcher(lsett.lastnis, lastworkingdir , lsett.mainsett);

}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::addThisNI(qint64 msecsep, QString sn, QString ni, QString dirname, QString fullpath2dir)
{
    Q_UNUSED(msecsep);
    Q_UNUSED(sn);
    Q_UNUSED(dirname);
    Q_UNUSED(fullpath2dir);
    lsett.hNi2Counter.insert(ni, lsett.hNi2Counter.value(ni, 0) + 1);
}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::searchfinished(int counter)
{
    Q_UNUSED(counter);
  lsett.isSearching = false;


    const QList<QString> lk = lsett.hNi2Counter.keys();

    if(lk.isEmpty()){
        ui->lblWarnings->clear();
        return;
    }

    int warncounter = 0;
    for(int i = 0, imax = lk.size(); i < imax; i++){
        const int c = lsett.hNi2Counter.value(lk.at(i));
        if(c > 1){//1 - is ok, more bad
            warncounter++;
        }
    }
    QString txtmessage ;
    if(warncounter > 0){
        txtmessage = tr("There are some existing NIs. Press 'Check' to see");
        txtmessage = QString("<b>%1</b>").arg(txtmessage);
    }else{
        txtmessage = tr("All NIs are unique for this object");
    }
    ui->lblWarnings->setText(txtmessage);
}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::on_lblWarnings_linkActivated(const QString &link)
{
    Q_UNUSED(link);
    //just open a window
    on_tbNIchecker_clicked();
}


//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::createAllConnections()
{
    connect(ui->tbSetWorkingDir, SIGNAL(clicked(bool)), this, SIGNAL(openWorkingDirectorySettings()));


//    connect(ui->leConsumerAddress, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
    connect(ui->leConsumerAddress, SIGNAL(textChanged(QString)), this, SLOT(recalculateNiWarningsLater()));

    connect(ui->leConsumerAddress, SIGNAL(textChanged(QString)), this, SIGNAL(setConsumerAddressFromWriteTariff(QString)));


    QTimer *tmr = new QTimer(this);
    tmr->setInterval(999);
    tmr->setSingleShot(true);
    connect(this, SIGNAL(startTmrCalcNiWarns()), tmr, SLOT(start()));
    connect(tmr, SIGNAL(timeout()), this, SLOT(recalculateNiWarnings()));


    ParametryzatorNISearcher *searcher = new ParametryzatorNISearcher;
    QThread *thread = new QThread;

    thread->setObjectName("ParametryzatorNISearcherShadow");
    searcher->moveToThread(thread);
    connect(this, SIGNAL(kickOffAllObjects()), searcher, SLOT(stopAllAndKickOff()));
    connect(this, SIGNAL(kickOffAllObjects()), searcher, SLOT(stopAllForced()), Qt::DirectConnection);

    connect(searcher, &ParametryzatorNISearcher::searchstarted, [=]{

            ui->lblWarnings->clear();
    });

    connect(searcher, &ParametryzatorNISearcher::addThisNI, this, &ParametryzationProfilesWdgtTopper::addThisNI);

    connect(searcher, &ParametryzatorNISearcher::searchfinished, this, &ParametryzationProfilesWdgtTopper::searchfinished);

    connect(this, &ParametryzationProfilesWdgtTopper::startSearcher, searcher, &ParametryzatorNISearcher::startSearcher);
    thread->start();

}

//------------------------------------------------------------------------------------------------

//void ParametryzationProfilesWdgtTopper::onTextChanged(QString text)
//{

//    //check nis

//}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::on_lblWorkingDirectory_linkActivated(const QString &link)
{
    QString openpath = link;
    QDir dir(link);
    if(!dir.exists()){
        dir.cdUp();
        if(dir.exists())
            openpath = dir.absolutePath();
    }

    qDebug() << "try to open link " << openpath << link;
#ifdef Q_OS_WIN
        QDesktopServices::openUrl(QUrl("file:///" + openpath, QUrl::TolerantMode));
#else
        QDesktopServices::openUrl(QUrl("file://" + openpath, QUrl::TolerantMode));
#endif
}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::on_tbUpdateCompleter_clicked()
{
    QDir dir ;
    dir.setPath(lsett.workigndir);

    QStringList l = dir.entryList(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot);
    QCompleter *c = new QCompleter(l, this);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    ui->leConsumerAddress->setCompleter(c);

}

//------------------------------------------------------------------------------------------------

void ParametryzationProfilesWdgtTopper::on_tbNIchecker_clicked()
{
    emit openNiChecker(ui->leConsumerAddress->text(), lsett.workigndir);
}

//------------------------------------------------------------------------------------------------
