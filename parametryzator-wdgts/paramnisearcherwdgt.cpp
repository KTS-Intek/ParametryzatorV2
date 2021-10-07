#include "paramnisearcherwdgt.h"
#include "ui_paramnisearcherwdgt.h"



//--------------------------------------------------------------------------------------------

ParamNISearcherWdgt::ParamNISearcherWdgt(GuiHelper *gHelper, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamNISearcherWdgt)
{
    ui->setupUi(this);

    createWidgets(gHelper);
}

//--------------------------------------------------------------------------------------------

ParamNISearcherWdgt::~ParamNISearcherWdgt()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------

void ParamNISearcherWdgt::createWidgets(GuiHelper *gHelper)
{
    searcherwdgt = new ParamNISearcher(gHelper, ui->stackedWidget->currentWidget());

    ui->vl4wdgt->addWidget(searcherwdgt);

    QTimer *nitimer = new QTimer(this);
    nitimer->setSingleShot(true);
    nitimer->setInterval(555);

    connect(ui->leNIs, SIGNAL(textChanged(QString)), nitimer, SLOT(start()));
    connect(nitimer, SIGNAL(timeout()), this, SLOT(sendNis()));

    connect(this, &ParamNISearcherWdgt::kickOffAllObjects, searcherwdgt, &ParamNISearcher::kickOffAllObjects);

    connect(searcherwdgt, SIGNAL(pageEndInit()), nitimer, SLOT(start()));

    connect(searcherwdgt, &ParamNISearcher::pageEndInit, this, &ParamNISearcherWdgt::gimmeSettings);
    connect(ui->tbChangeProjectDir, SIGNAL(clicked(bool)), this, SIGNAL(openWorkingDirectorySettings()));

    connect(this, &ParamNISearcherWdgt::setPageSettpPrintingGrp, searcherwdgt, &ParamNISearcher::setPageSettpPrintingGrp);
    connect(this, &ParamNISearcherWdgt::setPageSettpPrintingMain, searcherwdgt, &ParamNISearcher::setPageSettpPrintingMain);

    connect(this, &ParamNISearcherWdgt::setTheseNis, searcherwdgt, &ParamNISearcher::setTheseNis);

    connect(this, &ParamNISearcherWdgt::setPageSettpPrintingMain, [=](QVariantHash mainsett){
       ui->lblProjectDir->setText(mainsett.value("lePath2pdfDir").toString());
    });


}

//--------------------------------------------------------------------------------------------

QStringList ParamNISearcherWdgt::getnis()
{
    QStringList l = ui->leNIs->text().simplified().trimmed().split(" ", QString::SkipEmptyParts);
    l.removeDuplicates();
    return l;
}

//--------------------------------------------------------------------------------------------

void ParamNISearcherWdgt::sendNis()
{
    const QStringList nil = getnis();
    if(nil == lastnis)
        return;
    lastnis = nil;

    const qint64 currdtmsec = QDateTime::currentMSecsSinceEpoch();
    for(int i = 0, imax = nil.size(); i < imax; i++){
        if(ni2history.contains(nil.at(i)))
            continue;
        SelectStartOperationPage::DeviceHistory d;
        d.msec = currdtmsec;
        d.operationname = "search";
        ni2history.insert(nil.at(i), d);
    }

    ui->lblNIs->setText(tr("NIs - %1").arg(nil.size()));
    emit setTheseNis(lastnis);
}

//--------------------------------------------------------------------------------------------


void ParamNISearcherWdgt::on_tbNiHistory_clicked()
{
    emit killOldHistory();
    DeviceHistorySelector *w = new DeviceHistorySelector(searcherwdgt->gHelper, 0);
//    connect(w, SIGNAL(onPageCanReceiveData()), this, SLOT(sendHistory()));
    connect(w, &DeviceHistorySelector::onPageCanReceiveData, [=]{
        const QStringList lkni = getnis();
        const QString mask = searcherwdgt->gHelper->getDateTimeMask();


        MyListStringList listRows;
    //    QStringList headerH, const QStringList &header, const bool &hasHeader
        for(int i = 0, imax = lkni.size(); i < imax; i++){
            const SelectStartOperationPage::DeviceHistory d = ni2history.value(lkni.at(i));
            QStringList l;
            l.append(QDateTime::fromMSecsSinceEpoch(d.msec).toString(mask));
            l.append(lkni.at(i));
            l.append(d.operationname);
            listRows.append(l);
        }

        emit sendPageSett(listRows,QVariantMap(),tr("Date and time;NI;Operation").split(";"), QString("msec;NI;operation").split(";"),true);

    });

    connect(this, SIGNAL(killOldHistory()), w, SLOT(deleteLater()));
//    connect(w, SIGNAL(mainColRows(QStringList,bool)), this, SLOT(onMainColRows(QStringList,bool)));
    connect(w, &DeviceHistorySelector::mainColRows, [=](QStringList list, bool appendMode){
        QStringList old = appendMode ? getnis() : QStringList();
        old.append(list);
        old.removeDuplicates();
        ui->leNIs->setText(old.join(" "));
    });

    connect(this, SIGNAL(sendPageSett(MyListStringList,QVariantMap,QStringList,QStringList,bool)), w, SLOT(setPageSett(MyListStringList,QVariantMap,QStringList,QStringList,bool)));

    w->setKeyColumn(1);

    w->setAccessibleName("delete");

    ui->stackedWidget->addWidget(w);
    ui->stackedWidget->setCurrentWidget(w);
}
