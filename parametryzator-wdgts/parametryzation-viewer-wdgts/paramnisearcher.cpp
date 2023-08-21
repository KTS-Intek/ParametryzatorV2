#include "paramnisearcher.h"

#include <QDebug>

#include <QDesktopServices>

///[!] parametryzator-base
#include "src/datafilters/helpers/paremetryzatordata2userreadable.h"
#include "parametryzator-src/parametryzatornisearcher.h"

///[!] type-converter
#include "src/base/convertatype.h"




ParamNISearcher::ParamNISearcher(GuiHelper *gHelper, QWidget *parent) : ReferenceWidgetClassGui(gHelper, parent)
{
  hidePbAdd();
}

QStringList ParamNISearcher::getHeader()
{
    QStringList l;
    l.append(tr("Date"));
    l.append(tr("S/N"));
    l.append(tr("NI"));
    l.append(tr("Directory"));
    l.append(tr("Path"));
    return l;
}

QDir ParamNISearcher::getObjectDirectory(const QString &objectname)
{
    const QString path2workdir = lastsett.mainsett.value("lePath2pdfDir").toString();
//    const QString objectname = lastsett.grpsett.value("leConsumerAddress").toString();

    QDir objectdir(QString("%1/%2").arg(path2workdir).arg(objectname));
    return objectdir;
}

void ParamNISearcher::clearPage()
{
    StandardItemModelHelper::setModelHorizontalHeaderItems(model, getHeader());
    emit setHeaderUpdated(true);
    proxy_model->resetSorting();
}

void ParamNISearcher::initPage()
{
//    updateInfoAboutRow(-1);

    gHelper->getDateTimeMask();
    createModels(SETT_FILTERS_FREE_7);
    clearPage();

    connectMessageSignal();
    connect(lastTv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onLastTvCustomContextMenuRequestedExt(QPoint)));// SLOT(ontvTable_customContextMenuRequested(QPoint)));
    allow2useDefaultContextMenu(true);



    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(333);

    connect(t, SIGNAL(timeout()), this, SLOT(refreshTableExtraData()));
    connect(this, SIGNAL(refreshTableExtraDataLater(int)), t, SLOT(start(int)));

    connect(lastTv, &QTableView::doubleClicked, this, &ParamNISearcher::onLastTvClicked);

    ParametryzatorNISearcher *searcher = new ParametryzatorNISearcher;
    QThread *thread = new QThread;

    thread->setObjectName("ParametryzatorNISearcher");
    searcher->moveToThread(thread);
    connect(this, SIGNAL(kickOffAllObjects()), searcher, SLOT(stopAllAndKickOff()));
    connect(this, SIGNAL(kickOffAllObjects()), searcher, SLOT(stopAllForced()), Qt::DirectConnection);
    connect(searcher, SIGNAL(searchstarted()), this, SLOT(clearPage()));
    connect(searcher, &ParametryzatorNISearcher::addThisNI, this, &ParamNISearcher::addThisNI);

    connect(searcher, &ParametryzatorNISearcher::searchfinished, [=](int counter){
        qDebug() << "countger " << counter;
        this->setEnabled(true);
        resizeLastTv2content();
    });

    connect(this, &ParamNISearcher::startSearcher, searcher, &ParametryzatorNISearcher::startSearcher);
    thread->start();


}

void ParamNISearcher::refreshTableExtraDataLaterDefMsec()
{
    emit refreshTableExtraDataLater(1111);

}

void ParamNISearcher::setPageSettpPrintingGrp(QVariantHash sett)
{
    lastsett.grpsett = sett;
    refreshTableExtraDataLaterDefMsec();

}

void ParamNISearcher::setPageSettpPrintingMain(QVariantHash lastPageSett)
{
    lastsett.mainsett = lastPageSett;
    refreshTableExtraDataLaterDefMsec();
}

void ParamNISearcher::setTheseNis(QStringList nis)
{
    lastsett.lastnis = nis;
    if(!isEnabled()){
        refreshTableExtraDataLaterDefMsec();
        return;
    }

    emit refreshTableExtraDataLater(111);

}

void ParamNISearcher::addThisNI(qint64 msecsep, QString sn, QString ni, QString dirname, QString fullpath2dir)
{
    qDebug() << "addThisNI " << msecsep << sn << ni << dirname << fullpath2dir;

    QStringList listrow;
    listrow.append(QDateTime::fromMSecsSinceEpoch(msecsep).toLocalTime().toString(gHelper->getDateTimeMask()));
    listrow.append(sn);
    listrow.append(ni);
    listrow.append(dirname);
    listrow.append(fullpath2dir);
    StandardItemModelHelper::append2model(listrow, model);
}


void ParamNISearcher::refreshTableExtraData()
{

    if(lastsett.lastnis.isEmpty())
        return;

    if(!isEnabled()){
        refreshTableExtraDataLaterDefMsec();

        return;//wait a moment
    }



    setEnabled(false);

//    if(lastsett.lastworkingdir.isEmpty())
    lastsett.lastworkingdir = lastsett.mainsett.value("lePath2pdfDir").toString();


    emit startSearcher(lastsett.lastnis, lastsett.lastworkingdir, lastsett.mainsett);// .value("lePath2pdfDir").toString());


}

void ParamNISearcher::onLastTvClicked(const QModelIndex &index)
{

    const QString urlprefix =
    #ifdef Q_OS_WIN
            "file:///"
        #else
"file://"
    #endif
            ;


    if(index.column() == 4){
        //try to open pdf file

        const QString indextext = index.data(Qt::DisplayRole).toString();

        if(!indextext.isEmpty()){
            const bool r = QDesktopServices::openUrl(QUrl(urlprefix + indextext));
            if(r)
                return;
        }
    }

    //open the directory
    const int srcrow = proxy_model->mapToSource(index).row();
    if(srcrow < 0)
        return;

    const QString path2dir = QString("%1/%2").arg(lastsett.lastworkingdir).arg(model->item(srcrow, 3)->text());


    QDesktopServices::openUrl(QUrl(urlprefix + path2dir));
}

void ParamNISearcher::onLastTvCustomContextMenuRequestedExt(const QPoint &pos)
{
    QAction *actupdateinfo = new QAction(QIcon(":/katynko/svg/view-refresh.svg"), tr("Update info"));
    connect(actupdateinfo, SIGNAL(triggered(bool)), this, SLOT(refreshTableExtraDataLaterDefMsec()));


    QList<QAction*> laCustom ;
    laCustom.append(actupdateinfo);


    createCustomMenu4tv(pos, (GuiHelper::ShowReset|GuiHelper::ShowExport|GuiHelper::ShowOnlyCopy|GuiHelper::DeleteSelected), accessibleName(), laCustom);

}
