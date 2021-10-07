#include "parametryzationprofileviewer.h"
#include "ui_parametryzationprofileviewer.h"
#include <QTimer>

//--------------------------------------------------------------------------------------------

ParametryzationProfileViewer::ParametryzationProfileViewer(const bool &editable, GuiHelper *gHelper, QWidget *parent) :
    QWidget(parent), gHelper(gHelper), isEditable(editable),
    ui(new Ui::ParametryzationProfileViewer)
{
    ui->setupUi(this);
    createWidgets();
}

//--------------------------------------------------------------------------------------------

ParametryzationProfileViewer::~ParametryzationProfileViewer()
{
    delete ui;
}

QVariantHash ParametryzationProfileViewer::getPageSett(QString &profname)
{
    profname = ui->lineEdit->text();
    if(!isEditable){
        return lastSettHash;
    }


    QVariantHash hash;

    if(model->item(0,0)->checkState() == Qt::Checked)
        hash.insert("display", dispw->getPageSett());// ui->pteDisplay->toPlainText());

    if(model->item(1, 0)->checkState() == Qt::Checked)
        hash.insert("dst", dstw->getPageSett());// ui->cbxDst->currentData() );

    if(model->item(2, 0)->checkState() == Qt::Checked)
        hash.insert("relaylimits", rellimw->getPageSett());

    if(model->item(3, 0)->checkState() == Qt::Checked)
        hash.insert("relay", relsettw->getPageSett());// ui->pteRelay->toPlainText());

    if(model->item(4, 0)->checkState() == Qt::Checked)
        hash.insert("relay-test", reltestw->getPageSett());// ui->pteRelay_3->toPlainText());

    if(model->item(5, 0)->checkState() == Qt::Checked)
        hash.insert("tariff", tariffw->getPageSett());// ui->cbxTariff->currentData() );

    return hash;
}

//--------------------------------------------------------------------------------------------

void ParametryzationProfileViewer::sendMeYourSett()
{
    QString profname;
    QVariantHash h = getPageSett(profname);

    emit setThisSett(profname, h);


}

//--------------------------------------------------------------------------------------------

void ParametryzationProfileViewer::setThisPagesSett(QString profilename, QVariantHash setthash)
{
    ui->lineEdit->setText(profilename);
    lastSettHash = setthash;

    const QString lastactiveitem = ui->lvOptions->currentIndex().data(Qt::DisplayRole).toString();

    if(!isEditable || (setthash.contains("display") && isEditable ))
        dispw->setPageSett(setthash.value("display").toString());

    if(!isEditable || (setthash.contains("dst") && isEditable ))
    dstw->setPageSett(setthash.value("dst").toHash());

    if(!isEditable || (setthash.contains("relaylimits") && isEditable ))
    rellimw->setPageSett(setthash.value("relaylimits").toHash());

    if(!isEditable || (setthash.contains("relay") && isEditable ))
    relsettw->setPageSett(setthash.value("relay").toString());

    if(!isEditable || (setthash.contains("relay-test") && isEditable ))
    reltestw->setPageSett(setthash.value("relay-test").toString());

    if(!isEditable || (setthash.contains("tariff") && isEditable ))
    tariffw->setPageSett(setthash.value("tariff").toHash());

    const QStringList lk = QString("display dst relaylimits relay relay-test tariff").split(" ", QString::SkipEmptyParts);


    int lastactiveitemindx = -1;

    for(int i = 0, imax = lk.size(); i < imax; i++){
        if(setthash.contains(lk.at(i))){
            if(isEditable){
                model->item(i,0)->setCheckState(Qt::Checked);
            }else{
                model->item(i,0)->setEnabled(true);
            }
            if(model->item(i,0)->text() == lastactiveitem)
                lastactiveitemindx = i;
        }else{
            if(isEditable){
                model->item(i,0)->setCheckState(Qt::Unchecked);
                if(model->item(i,0)->text() == lastactiveitem)
                    lastactiveitemindx = i;
            }else{
                model->item(i,0)->setEnabled(false);
            }
        }
    }



    if(proxymodel->rowCount() > 0){

        if(lastactiveitemindx >= 0)
            lastactiveitemindx = proxymodel->mapFromSource(model->index(lastactiveitemindx, 0)).row();


        if(lastactiveitemindx < 0)
            lastactiveitemindx = 0;


        ui->lvOptions->setCurrentIndex(proxymodel->index(lastactiveitemindx,0));



    }else{
        lastactiveitemindx = -1;
    }

    if(lastactiveitemindx < 0)
        ui->stackedWidget->setCurrentIndex(-1);







}

//--------------------------------------------------------------------------------------------


void ParametryzationProfileViewer::resetAll2defSett()
{
    dispw->applyDefaultSett();

    dstw->applyDefaultSett();

    rellimw->applyDefaultSett();

    relsettw->applyDefaultSett();

    reltestw->applyDefaultSett();

    tariffw->applyDefaultSett();
}
//--------------------------------------------------------------------------------------------


void ParametryzationProfileViewer::createWidgets()
{
    model = new QStandardItemModel(this);
    proxymodel = new MySortFilterProxyModel(this);

    proxymodel->setSourceModel(model);
    connect(ui->leFilter, SIGNAL(textChanged(QString)), proxymodel, SLOT(setNewFileterStr(QString)));

    ui->lvOptions->setModel(proxymodel);

    dispw = new DisplaySettViewer(isEditable, this);
    addWidget(dispw, tr("Display"), true);

    dstw = new DSTSettViewer(isEditable, this);
    addWidget(dstw, tr("DST"), true);

    rellimw = new RelayLimits(isEditable, this);
    addWidget(rellimw, tr("Relay limits"), true);


    relsettw = new RelaySettViewer(isEditable, this);
    addWidget(relsettw, tr("Relay settings"), true);

    reltestw = new RelayTestSettViewer(isEditable, this);
    addWidget(reltestw, tr("Relay test"), true);


    tariffw = new TariffSettViewer(isEditable, gHelper, this);
    addWidget(tariffw, tr("Tariff program"), true);


    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(111);

    connect(ui->leFilter, SIGNAL(textChanged(QString)), t, SLOT(start()));
    connect(t, SIGNAL(timeout()), this, SLOT(updateCurrentRowWdgt()));

    QTimer::singleShot(11, this, SIGNAL(onWidgetsReady()));

    ui->lineEdit->setReadOnly(!isEditable);

    ui->stackedWidget->setCurrentIndex(0);
    ui->leFilter->clear();

    ui->lvOptions->setCurrentIndex(proxymodel->mapFromSource(model->index(0,0)));

}

//--------------------------------------------------------------------------------------------

void ParametryzationProfileViewer::addWidget(QWidget *w, QString title, bool enable)
{
    QStandardItem *item = new QStandardItem(title);

    if(isEditable){
        item->setCheckable(true);
        item->setCheckState(enable ? Qt::Checked : Qt::Unchecked);
    }

    model->appendRow(item);

    QScrollArea *sa = new QScrollArea(this);
    sa->setWidget(w);
    sa->setWindowTitle(title);

    w->setWindowTitle(title);
    ui->stackedWidget->addWidget(sa);
}

//--------------------------------------------------------------------------------------------

void ParametryzationProfileViewer::updateCurrentRowWdgt()
{
    on_lvOptions_clicked(ui->lvOptions->currentIndex());
}

void ParametryzationProfileViewer::resetProfileName()
{
    ui->lineEdit->clear();
}

//--------------------------------------------------------------------------------------------

void ParametryzationProfileViewer::on_lvOptions_clicked(const QModelIndex &index)
{
    const int indx = proxymodel->mapToSource(index).row();
    if(indx < 0)
        return;

    ui->stackedWidget->setCurrentIndex(indx);
}


//--------------------------------------------------------------------------------------------
