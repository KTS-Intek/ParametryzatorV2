#include "relaytestsettviewer.h"
#include "ui_relaytestsettviewer.h"

///[!] parametryzator-base
#include "src/datafilters/helpers/paremetryzatordata2userreadable.h"

//-----------------------------------------------------------------------------------

RelayTestSettViewer::RelayTestSettViewer(const bool &editable, QWidget *parent) :
    QWidget(parent), isEditable(editable),
    ui(new Ui::RelayTestSettViewer)
{
    ui->setupUi(this);
    createWidgets();
}



//-----------------------------------------------------------------------------------

RelayTestSettViewer::~RelayTestSettViewer()
{
    delete ui;
}

//-----------------------------------------------------------------------------------

QString RelayTestSettViewer::getPageSett()
{
    QStringList list = listwdgt->getApplyVarListAsStr();

    return list.join(" ");
}

//-----------------------------------------------------------------------------------

TwoListWdgtPairList RelayTestSettViewer::getAllListItems()
{
    TwoListWdgtPairList list;

    const QHash<QString,QString> hash = ParemetryzatorData2userReadable::getHashRealayTestOptions();

    QList<QString> lk = hash.keys();
    std::sort(lk.begin(), lk.end());

    for(int i = 0, imax = lk.size(); i < imax; i++)
        list.append(TwoListWdgtPairItem(hash.value(lk.at(i)), lk.at(i)));

    return list;

}

//-----------------------------------------------------------------------------------

TwoListWdgtPairList RelayTestSettViewer::getDefListItems()
{
    const QStringList list = QString("warn on state off state on").split(" ", QString::SkipEmptyParts);
    return TwoListsWdgt::generateListFromKeys(list, getAllListItems());
}

//-----------------------------------------------------------------------------------

void RelayTestSettViewer::setPageSett(const QString &relay_test)
{
    const QStringList l = relay_test.split(" ", QString::SkipEmptyParts);

    const QHash<QString,QString> hashall = ParemetryzatorData2userReadable::getHashRealayTestOptions();
    const QHash<QString,QString> hashstate = ParemetryzatorData2userReadable::getHashRealayTestOptionsStates();
    TwoListWdgtPairList list;

    for(int i = 0, imax = l.size(); i < imax; i++){
        const QString key = l.at(i);
        if(key.startsWith("state-")){
            list.append(TwoListWdgtPairItem(hashstate.value(key), "state"));//preview mode only
        }else{
            list.append(TwoListWdgtPairItem(hashall.value(key), key));
        }
    }

//    const TwoListWdgtPairList list = TwoListsWdgt::generateListFromKeys(l, getAllListItems());

    listwdgt->setTheseListsPairs(list, getAllListItems());
}

//-----------------------------------------------------------------------------------

void RelayTestSettViewer::applyDefaultSett()
{
    listwdgt->setTheseDefListsPairs(getDefListItems() , getAllListItems());

}

//-----------------------------------------------------------------------------------

void RelayTestSettViewer::createWidgets()
{
    listwdgt = new TwoListsWdgt(!isEditable, false, this);//editable, unique values only
    ui->vl4wdgt->addWidget(listwdgt);


    applyDefaultSett();
}

//-----------------------------------------------------------------------------------
