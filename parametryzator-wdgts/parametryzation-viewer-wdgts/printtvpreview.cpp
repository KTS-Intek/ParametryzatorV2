#include "printtvpreview.h"

///[!] parametryzator-base
#include "src/datafilters/helpers/paremetryzatordata2userreadable.h"
#include "parametryzator-src/parametryzatornisearcher.h"


///[!] type-converter
#include "src/base/convertatype.h"


//--------------------------------------------------------------------

PrintTvPreview::PrintTvPreview(GuiHelper *gHelper, QWidget *parent) : ReferenceWidgetClassGui(gHelper, parent)
{
    hidePbAdd();
}


int PrintTvPreview::findNisInObjectDir(const QString &ni)
{
    return ParametryzatorNISearcher::findNisInObjectDir(ni, lastsett.grpsett.value("leConsumerAddress").toString(), lastsett.mainsett).size();
}

//--------------------------------------------------------------------

QStringList PrintTvPreview::getSelectedNIs()
{
    return TableViewHelper::getSelectedRowsText(lastTv, getNICol());
}

int PrintTvPreview::getNICol()
{
    return 2;
}

//--------------------------------------------------------------------

QStringList PrintTvPreview::getHeader()
{
    QStringList l;
    l.append(tr("Duplicates"));
    l.append(tr("Model"));
    l.append(tr("NI"));
    l.append(tr("S/N"));
    l.append(tr("Voltage"));
    l.append(tr("Current"));
    l.append(tr("Tariff program"));
    l.append(tr("Manufacturer"));
    return l;
}

//--------------------------------------------------------------------

QList<int> PrintTvPreview::getDuplicates(QStringList &nis, const int &mode)
{

    QStringList listrowstext;
    switch(mode){
    case PARAM_PRINT_MODE_SELECTED:{
        listrowstext = TableViewHelper::getSelectedRowsText(lastTv, 0);
        nis = TableViewHelper::getSelectedRowsText(lastTv, getNICol());

        break;} //selected

    case PARAM_PRINT_MODE_VISIBLE:{ //visible
        listrowstext = TableViewHelper::getRowsText(lastTv, 0);
        nis = TableViewHelper::getRowsText(lastTv, getNICol()); //getSelectedNIs();

        break;}
        //PARAM_PRINT_MODE_CURRENTRW
    default:{  //current row
        const int row = proxy_model->mapToSource(lastTv->currentIndex()).row();
        if(row >= 0){
            listrowstext.append(model->item(row, 0)->text());
            nis.append(model->item(row, getNICol())->text());

        }
        break;}
    }

    bool ok;
    emit refreshTableExtraDataLater();
    return ConvertAtype::strList2intListExt(listrowstext, true, ok);

    //    return r;
}

//--------------------------------------------------------------------

void PrintTvPreview::clearPage()
{
    StandardItemModelHelper::setModelHorizontalHeaderItems(model, getHeader());
    emit setHeaderUpdated(true);
    proxy_model->resetSorting();

}

//--------------------------------------------------------------------

void PrintTvPreview::initPage()
{
    updateInfoAboutRow(-1);

    gHelper->getDateTimeMask();
    createModels(SETT_FILTERS_FREE_5);
    clearPage();

    connectMessageSignal();
    connect(lastTv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onLastTvCustomContextMenuRequestedExt(QPoint)));// SLOT(ontvTable_customContextMenuRequested(QPoint)));
    allow2useDefaultContextMenu(false);


    connect(this, SIGNAL(delActionWatcher()), this, SLOT(updateInfoAboutCurrentRow()));
    connect(this, SIGNAL(delActionWatcher()), this, SIGNAL(refreshTableExtraDataLater()));

    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(333);

    connect(t, SIGNAL(timeout()), this, SLOT(refreshTableExtraData()));
    connect(this, SIGNAL(refreshTableExtraDataLater()), t, SLOT(start()));



    connect(lastTv, &QTableView::clicked, this, &PrintTvPreview::onLastTvClicked);

}

void PrintTvPreview::onReportReady(QString ni)
{
    const int srcrow = StandardItemModelHelper::findThisCellText(getNICol(), ni, model);
    if(srcrow >= 0){
        model->removeRow(srcrow);
    }

    if(proxy_model->rowCount() > 0){
        lastTv->setCurrentIndex(proxy_model->index(0,0));
        updateInfoAboutCurrentRow();//it must do it immediately
    }


}

void PrintTvPreview::activateRowWithThisNI(QString ni)
{
    update();

    const int niCol = getNICol();

    if(TableViewHelper::selectRowWithThisCell(lastTv, ni, niCol))
        updateInfoAboutCurrentRow();
    else{
        qDebug() << "!find ni" << ni;
        const QList<int> lindxes = TableViewHelper::getVisibleSourceRows(lastTv, false);
        for(int i = 0, imax = lindxes.size(); i < imax; i++)
            qDebug() << i << lindxes.at(i) << model->item(lindxes.at(i), niCol)->text();
        updateInfoAboutRow(-1);
    }

}

//--------------------------------------------------------------------

void PrintTvPreview::setPageSettpPrintingGrp(QVariantHash sett)
{
    lastsett.grpsett = sett;

    emit refreshTableExtraDataLater();


}

void PrintTvPreview::setPageSettpPrintingMain(QVariantHash lastPageSett)
{
    lastsett.mainsett = lastPageSett;

    emit refreshTableExtraDataLater();
}



//--------------------------------------------------------------------

void PrintTvPreview::startPrintingTariffReports(MPrintTableOut printout, QVector<QVariantHash> printdata)
{
    //    lastsett.printdata = printdata;
    //    lastsett.printout = printout;
    //    MPrintTableOut printout = lastsett.printout;
    //    const QVector<QVariantHash> printdata = lastsett.printdata;

    lastsett.printdata.clear();
    lastsett.printout.clear();
    lastsett.ni2meterprog.clear();

    //remove duplicates
    QStringList readynis;
    for(int i = 0, imax = printout.size() ; i < imax && !printout.isEmpty(); i++ ){
        const QStringList l = printout.takeLast();
        const QVariantHash h = printdata.takeLast();

        if(l.isEmpty())
            continue;

        if(readynis.contains(l.at(2)))
            continue;
        readynis.append(l.at(2));
        lastsett.printdata.prepend(h);
        lastsett.printout.prepend(l);
    }


    updateTable();

}

//--------------------------------------------------------------------

void PrintTvPreview::updateTable()
{
    const MPrintTableOut printout = lastsett.printout;
    const QVector<QVariantHash> printdata = lastsett.printdata;

    int currentrow = -1;
    const int currow = proxy_model->mapToSource(lastTv->currentIndex()).row();
    QString currni;
    const int niCol = getNICol();

    if(currow >= 0)
        currni = model->item(currow, niCol)->text();

    StandardItemModelHelper::setModelHorizontalHeaderItems(model, getHeader());


    //Model, S/N , NI, MeterProg



    for(int i = 0, imax = printout.size() ; i < imax; i++ ){
        const QStringList l = printout.at(i);

        if(!currni.isEmpty() && currni == l.at(2)){
            currentrow = i;
            currni.clear();
        }

        QList<QStandardItem*> li;
        li.append(new QStandardItem("-"));
        for(int c = 0; c < 3; c++)
            li.append(new QStandardItem(l.at(c)));//model S/N NI
        li.append(new QStandardItem("-"));
        li.append(new QStandardItem("-"));

        lastsett.ni2meterprog.insert(l.at(1), l.at(3));// printdata.at(i).value("meter_prog").toString());
        int t;
        li.append(new QStandardItem(ParemetryzatorData2userReadable::jsonTariff2prettyStrH(printdata.at(i).value("tariff").toHash(), t).join(", ") ));//tariff program
        li.append(new QStandardItem("-"));
        model->appendRow(li);
    }



    resizeLastTv2content();


    setHasDataFromRemoteDevice(UCDataState(QDateTime::currentDateTimeUtc(),true));
    emit refreshTableExtraDataLater();

    if(currentrow < 0 && proxy_model->rowCount() > 0)
        currentrow = 0;

    if(currentrow >= 0){
        lastTv->setCurrentIndex(proxy_model->mapFromSource(model->index(currentrow, niCol)));
    }

    QTimer::singleShot(1111, this, SLOT(updateInfoAboutCurrentRow()));
}

//--------------------------------------------------------------------

void PrintTvPreview::refreshTableExtraData()
{
    const MeterProg2MeterHumanData hashpramssett = ParametryzatorPrintHelper::getOneMetersInfoHash(lastsett.mainsett);// const QVariantHash &mainpramssett);
    //    const ParametryzatorPrintInputMeterInfoHuman metercachedinfo = ParametryzatorPrintHelper::getInfoHumanFromVariantHashExt(const QVariantHash &metersetthash, const MeterProg2MeterHumanData &hashpramssett)




    const int currrow = proxy_model->mapToSource(lastTv->currentIndex()).row();

    const QList<int> lcols = QList<int>() << 0 << 1 << 4 << 5 << 7;
    const int niCol = getNICol();
    for(int i = 0, imax = model->rowCount(), jmax = lcols.size(); i < imax; i++){


        const QString ni = model->item(i, niCol)->text();
        const ParametryzatorPrintInputMeterInfoHuman metercachedinfo = ParametryzatorPrintHelper::getInfoHumanFromVariantHashExt(lastsett.ni2meterprog.value(ni), hashpramssett);

        QStringList l;
        l.append(QString::number(findNisInObjectDir(ni)));
        l.append(metercachedinfo.metermodelhuman);
        l.append(metercachedinfo.meterVolta);
        l.append(metercachedinfo.cbxCurrent);
        l.append(metercachedinfo.lastManufName);


        bool anythingchanged = false;

        for(int j = 0; j < jmax; j++){
            const int col = lcols.at(j);

            if(model->item(i, col)->text() != l.at(j)){
                model->item(i,col)->setText(l.at(j));
                if(!anythingchanged)
                    anythingchanged = true;
            }
        }
        if(i == currrow && anythingchanged){            
            updateInfoAboutRow(currrow);
        }

    }

    resizeLastTv2content();

}

void PrintTvPreview::updateInfoAboutCurrentRow()
{
    onLastTvClicked(lastTv->currentIndex());
}

//--------------------------------------------------------------------

void PrintTvPreview::updateInfoAboutRow(const int &srcrow)
{
    if(srcrow < 0){
        emit udpateInfoAboutCurrentRow(0, "", "", "", "", "", "", QVariantHash(), "");

    }else{
        emit udpateInfoAboutCurrentRow(model->item(srcrow, 0)->text().toInt(),
                                       model->item(srcrow, 1)->text(),

                                       model->item(srcrow, 3)->text(),//sn
                                       model->item(srcrow, 2)->text(),//ni

                                       model->item(srcrow, 4)->text(),
                                       model->item(srcrow, 5)->text(),
                                       model->item(srcrow, 6)->text(),
                                       lastsett.printdata.at(srcrow), //  lastsett.ni2tariffjsn.value(model->item(srcrow, 3)->text()),
                                       model->item(srcrow, 7)->text());

    }

}

//--------------------------------------------------------------------

void PrintTvPreview::onLastTvClicked(const QModelIndex &index)
{
    updateInfoAboutRow(proxy_model->mapToSource(index).row());
}

void PrintTvPreview::onLastTvCustomContextMenuRequestedExt(const QPoint &pos)
{
    const bool hasSelectedItms = !lastTv->selectionModel()->selectedRows().isEmpty();


    QAction *actprintselected = new QAction(QIcon(":/katynko/printing/document-print-direct-nobackground.svg"), tr("Print selected"));
    actprintselected->setEnabled(hasSelectedItms);
    connect(actprintselected, SIGNAL(triggered(bool)), this, SIGNAL(ask2printSelected()));
    connect(actprintselected, SIGNAL(triggered(bool)), this, SIGNAL(refreshTableExtraDataLater()));

    QAction *actpreviewselected = new QAction(QIcon(":/katynko/svg/document-print.svg"), tr("Preview selected"));
    actpreviewselected->setEnabled(hasSelectedItms);
    connect(actpreviewselected, SIGNAL(triggered(bool)), this, SIGNAL(ask2PreviewPrintSelected()));
    connect(actpreviewselected, SIGNAL(triggered(bool)), this, SIGNAL(refreshTableExtraDataLater()));


    QAction *actupdateinfo = new QAction(QIcon(":/katynko/svg/view-refresh.svg"), tr("Update info"));
    connect(actupdateinfo, SIGNAL(triggered(bool)), this, SIGNAL(refreshTableExtraDataLater()));


    QList<QAction*> laCustom ;
     if(!lastsett.lockPrinting){
    laCustom.append(actprintselected);
    laCustom.append(actpreviewselected);
    }
    laCustom.append(actupdateinfo);


    createCustomMenu4tv(pos, (GuiHelper::ShowReset|GuiHelper::ShowExport|GuiHelper::ShowOnlyCopy|GuiHelper::DeleteSelected), accessibleName(), laCustom);

}

//--------------------------------------------------------------------
