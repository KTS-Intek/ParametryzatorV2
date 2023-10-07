#include "relaysettviewer.h"
#include "ui_relaysettviewer.h"


///[!] type-converter
#include "src/base/convertatype.h"


///[!] parametryzator-base
#include "src/datafilters/helpers/paremetryzatordata2userreadable.h"

/*
//timeouts
TOnB=0  вкл по кнопці (сек)
TOnAI=1 автоматично увімкнути після перевантаженню по струму  (сек) хв
TOnAP=1 автоматично увімкнути після перевантаженню по потужності (сек) хв
TOnAU=5  автоматично увімкнути по якісній напрузі (сек)
TOnM=5  вкл після магнітного поля (сек)
TOffM=5         вимк із-за магнітне поле (сек)
TOffId=0        вимк із-за диф струму (сек)
TOffU=5         вимк із-за неякісної напруги (сек)
TOffI=5         вимк із-за перевищення струму (сек)
TOffP=5         вимк із-за перевищення потужності (сек)
TOffCf=5        вимк із-за коефіцієнта потужності (сек)



//увімкнення
OnR     - ф-ція вкл реле
OnC     - команда із центру
OnB     - по кнопці
OnT1    - тариф 1
OnT2    - тариф 2
OnT3    - тариф 3
OnT4    - тариф 4
OnU     - якісна напруга
OnCf    - коеф потужності
OnT     - по таймауту
OnPrpd  - сальдо (передоплата)
OnM     - вкл магнітне поле
OnId    - увм після різниці струмів


//вимкнення
OffR    - вимк ф-цію вимк реле
OffC    - вимк ф-цію вимк реле по команді
OffT1   - вимк по тарифу 1
OffT2   - вимк по тарифу 2
OffT3   - вимк по тарифу 3
OffT4   - вимк по тарифу 4
OffI    - вимк по струму
OffU    - вимк по неяк напрузі
OffId   - вимк по диф струму
OffPT1  - вимк по перевищенню активної потужності по тарифу 1
OffPT2  - вимк по перевищенню активної потужності по тарифу 2
OffPT3  - вимк по перевищенню активної потужності по тарифу 3
OffPT4  - вимк по перевищенню активної потужності по тарифу 4
OffQT1  - вимк по перевищенню реактивної потужності по тарифу 1
OffQT2  - вимк по перевищенню реактивної потужності по тарифу 2
OffQT3  - вимк по перевищенню реактивної потужності по тарифу 3
OffQT4  - вимк по перевищенню реактивної потужності по тарифу 4
OffCf   - вимк по низькому косинус фі
OffPrpd  - сальдо (передоплата)
OffPrpdFrnd  - сальдо (передоплата) по дружньому
OffM    - вимк по магнітному полю
OffPd   - вимк по потужності різної полярності




*/


//-----------------------------------------------------------------------------

RelaySettViewer::RelaySettViewer(const bool &editable, QWidget *parent) :
    QWidget(parent), isEditable(editable),
    ui(new Ui::RelaySettViewer)
{
    ui->setupUi(this);
    createWidgets();
}

//-----------------------------------------------------------------------------

RelaySettViewer::~RelaySettViewer()
{
    delete ui;
}

QString RelaySettViewer::getPageSett()
{
    QStringList list = listwdgt->getApplyVarListAsStr();


    QHash<QString,QString> hash;// = ConvertAtype::strHashFromQslSmpl(l, "=");



    hash.insert("TOnB", QString::number(ui->sbTimeoutOnBttn->value()));
    hash.insert("TOnAI", QString::number(ui->sbTimeoutOnCurrent->value()));
    hash.insert("TOnAP", QString::number(ui->sbTimeoutOnPwr->value()));
    hash.insert("TOnAU", QString::number(ui->sbTimeoutOnVltg->value()));
    hash.insert("TOnM", QString::number(ui->sbTimeoutOnMagnet->value()));

    hash.insert("TOffId", QString::number(ui->sbTimeoutOffIdiff->value()));
    hash.insert("TOffI", QString::number(ui->sbTimeoutOffCurrent->value()));
    hash.insert("TOffP", QString::number(ui->sbTimeoutOffPwr->value()));
    hash.insert("TOffU", QString::number(ui->sbTimeoutOffVltg->value()));
    hash.insert("TOffM", QString::number(ui->sbTimeoutOffMagnet->value()));
    hash.insert("TOffCf", QString::number(ui->sbTimeoutOffPwrCoefitient->value()));
    hash.insert("TOffPd", QString::number(ui->sbTimeoutOffPdiff->value()));



    list.append(ConvertAtype::qslFromHash(hash, "="));
    return list.join(" ");
}

//-----------------------------------------------------------------------------

TwoListWdgtPairList RelaySettViewer::getAllListItems()
{
    TwoListWdgtPairList list;

    const QHash<QString,QString> hash = ParemetryzatorData2userReadable::getHashRealayOptions(false);
    QList<QString> lk = hash.keys();
    std::sort(lk.begin(), lk.end());

    for(int i = 0, imax = lk.size(); i < imax; i++)
        list.append(TwoListWdgtPairItem(hash.value(lk.at(i)), lk.at(i)));


    return list;
}

//-----------------------------------------------------------------------------

TwoListWdgtPairList RelaySettViewer::getDefListItems()
{
    QStringList listdefkeys;
    listdefkeys.append("OnU");
    listdefkeys.append("OnT");
    listdefkeys.append("OnM");
    listdefkeys.append("OnR");
    listdefkeys.append("OnC");
    listdefkeys.append("OffU");
    listdefkeys.append("OffId");
    listdefkeys.append("OffM");
    listdefkeys.append("OffR");
    listdefkeys.append("OffC");
    listdefkeys.append("OnId");

    return TwoListsWdgt::generateListFromKeys(listdefkeys, getAllListItems());
}

//-----------------------------------------------------------------------------

void RelaySettViewer::setPageSett(const QString &relay)
{
    const QStringList l = relay.split(" ", QString::SkipEmptyParts);


    qDebug() << "RelaySettViewer::setPageSett relay=" << l;

    listwdgt->setTheseListsPairsStrL(l,  getAllListItems());


    const QHash<QString,QString> hash = ConvertAtype::strHashFromQslSmpl(l, "=");


    ui->sbTimeoutOnBttn->setValue(hash.value("TOnB").toInt());
    ui->sbTimeoutOnCurrent->setValue(hash.value("TOnAI").toInt());
    ui->sbTimeoutOnPwr->setValue(hash.value("TOnAP", QString::number(ui->sbTimeoutOnPwr->value())).toInt());
    ui->sbTimeoutOnVltg->setValue(hash.value("TOnAU").toInt());
    ui->sbTimeoutOnMagnet->setValue(hash.value("TOnM").toInt());

    ui->sbTimeoutOffIdiff->setValue(hash.value("TOffId").toInt());
    ui->sbTimeoutOffCurrent->setValue(hash.value("TOffI").toInt());
    ui->sbTimeoutOffPwr->setValue(hash.value("TOffP").toInt());
    ui->sbTimeoutOffVltg->setValue(hash.value("TOffU").toInt());
    ui->sbTimeoutOffMagnet->setValue(hash.value("TOffM").toInt());
    ui->sbTimeoutOffPwrCoefitient->setValue(hash.value("TOffCf").toInt());
    ui->sbTimeoutOffPdiff->setValue(hash.value("TOffPd").toInt());

    /*
    //timeouts
    TOnB=0  вкл по кнопці (сек)
    TOnAI=1 автоматично увімкнути після перевантаженню по струму  (сек) хв
    TOnAP=1 автоматично увімкнути після перевантаженню по потужності (сек) хв
    TOnAU=5  автоматично увімкнути по якісній напрузі (сек)
    TOnM=5  вкл після магнітного поля (сек)
    TOffM=5         вимк із-за магнітне поле (сек)
    TOffId=0        вимк із-за диф струму (сек)
    TOffU=5         вимк із-за неякісної напруги (сек)
    TOffI=5         вимк із-за перевищення струму (сек)
    TOffP=5         вимк із-за перевищення потужності (сек)
    TOffCf=5        вимк із-за коефіцієнта потужності (сек)

*/

}

//-----------------------------------------------------------------------------

void RelaySettViewer::applyDefaultSett()
{
//    ui->cbxEnableAutoScroll->setChecked(true);
//    ui->sbAutoscrollSec->setValue(7);
//    ui->cbxEnableDisplay->setChecked(true);
    //TPb=0 TAOnIP=1 TAOnU=5 TAOnU=5 TOnM=5 TOffM=5 TOffId=0 TOffU=5 TOffI=5 TOffP=5 TOffCf=5

    ui->sbTimeoutOnBttn->setValue(0);
    ui->sbTimeoutOnCurrent->setValue(60); //1 minute
    ui->sbTimeoutOnPwr->setValue(60); //1 minute
    ui->sbTimeoutOnVltg->setValue(5);
    ui->sbTimeoutOnMagnet->setValue(5);

    ui->sbTimeoutOffIdiff->setValue(0);
    ui->sbTimeoutOffCurrent->setValue(5);
    ui->sbTimeoutOffPwr->setValue(5);
    ui->sbTimeoutOffVltg->setValue(5);
    ui->sbTimeoutOffMagnet->setValue(5);
    ui->sbTimeoutOffPwrCoefitient->setValue(5);
    ui->sbTimeoutOffPdiff->setValue(0);


    listwdgt->setTheseDefListsPairs(getDefListItems() , getAllListItems());
}

//-----------------------------------------------------------------------------

void RelaySettViewer::createWidgets()
{
    listwdgt = new TwoListsWdgt(!isEditable, true, this);//editable, unique values only
//    ui->vl4wdgt->addWidget(listwdgt);
    ui->vl4wdgt->insertWidget(0, listwdgt);

    ui->sbTimeoutOnBttn->setEnabled(isEditable);//(0);
    ui->sbTimeoutOnCurrent->setEnabled(isEditable);//(60); //1 minute
    ui->sbTimeoutOnPwr->setEnabled(isEditable);//(60); //1 minute
    ui->sbTimeoutOnVltg->setEnabled(isEditable);//(5);
    ui->sbTimeoutOnMagnet->setEnabled(isEditable);//(5);

    ui->sbTimeoutOffIdiff->setEnabled(isEditable);//(0);
    ui->sbTimeoutOffCurrent->setEnabled(isEditable);//(5);
    ui->sbTimeoutOffPwr->setEnabled(isEditable);//(5);
    ui->sbTimeoutOffVltg->setEnabled(isEditable);//(5);
    ui->sbTimeoutOffMagnet->setEnabled(isEditable);//(5);
    ui->sbTimeoutOffPwrCoefitient->setEnabled(isEditable);//(5);
    ui->sbTimeoutOffPdiff->setEnabled(isEditable);//(0);


    applyDefaultSett();
}

//-----------------------------------------------------------------------------
