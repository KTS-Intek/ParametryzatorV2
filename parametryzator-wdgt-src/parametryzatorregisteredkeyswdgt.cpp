#include "parametryzatorregisteredkeyswdgt.h"

ParametryzatorRegisteredKeysWdgt::ParametryzatorRegisteredKeysWdgt(GuiHelper *gHelper, QWidget *parent)
    : ReferenceWidgetClassGui(gHelper, parent)
{
    hidePbAdd();
}

void ParametryzatorRegisteredKeysWdgt::initPage()
{
    createModels(SETT_FILTERS_FREE_6);
    clearPage();

    connectMessageSignal();
    connect(lastTv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onLastTvCustomContextMenuRequested(QPoint)));// SLOT(ontvTable_customContextMenuRequested(QPoint)));


    allow2useDefaultContextMenu(true);

    QTimer::singleShot(1111, this, SLOT(showRegisteredKeys()));

}

void ParametryzatorRegisteredKeysWdgt::showRegisteredKeys()
{
    QHash<QString,QString> h;

    h.insert("$pteAddrs", tr("This company address"));
    h.insert("$outDate1", tr("Act date"));
    h.insert("$outIndx1", tr("Act number (right part)"));
    h.insert("$leConsumerAddressSmpl", tr("Consumer city (it can be trimmed)"));//
    h.insert("$cbxCurrent", tr("Maximum Meter Current"));
    h.insert("$sbPrecision", tr("Meter precision"));

    h.insert("$sbCoefitientU", tr("Voltage transformers coefitients"));
    h.insert("$sbCoefitientI", tr("Current transformers coefitients"));
    h.insert("$aboutRelay", tr("Text info about relays"));
    h.insert("$outDate3", tr("Act date (format ddMMyy)"));
    h.insert("$outIndxL", tr("Act date (left part)"));
    h.insert("$dd", tr("Current day {01-dd}"));
    h.insert("$MM", tr("Current month {01-MM}"));
    h.insert("$yy", tr("Current year {00-99}"));
    h.insert("$yyyy", tr("Current year {0000-yyyy}"));


    h.insert("$meterTypeH", tr("Phase count"));
    h.insert("$meterTypeM", tr("Meter model"));
    h.insert("$meterTypeE", tr("Meter manufacturer"));
    h.insert("$meterFrom", tr("Meter origin"));
    h.insert("$meterVolta", tr("Meter voltage"));
    h.insert("$meterSN", tr("Meter S/N"));
    h.insert("$meterNI", tr("Meter NI"));
    h.insert("$leConsumerAddress", tr("Consumer city (full name)"));//

    QList<QString> lk = h.keys();
    std::sort(lk.begin(), lk.end());

    MPrintTableOut table;
    for(int i = 0, imax = lk.size(); i < imax; i++){
        QStringList rowl;
        rowl.append(lk.at(i));
        rowl.append(h.value(lk.at(i)));

        table.append(rowl);
    }    
    setTableData4amodelExt6woValidator(table, tr("Key;Value").split(";"));

}
