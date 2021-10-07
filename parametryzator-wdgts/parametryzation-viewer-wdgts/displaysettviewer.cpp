#include "displaysettviewer.h"
#include "ui_displaysettviewer.h"


///[!] type-converter
#include "src/base/convertatype.h"


///[!] parametryzator-base
#include "src/datafilters/helpers/paremetryzatordata2userreadable.h"

//---------------------------------------------------------------------------------

DisplaySettViewer::DisplaySettViewer(const bool &editable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplaySettViewer), isEditable(editable)
{
    ui->setupUi(this);
    createWidgets();
}

//---------------------------------------------------------------------------------

DisplaySettViewer::~DisplaySettViewer()
{
    delete ui;
}

//---------------------------------------------------------------------------------

QString DisplaySettViewer::getPageSett()
{
    QStringList list = listwdgt->getApplyVarListAsStr();


    QHash<QString,QString> hash;// = ConvertAtype::strHashFromQslSmpl(l, "=");



    hash.insert("auto", QString::number(int(ui->cbxEnableAutoScroll->isChecked())));
    hash.insert("sec", QString::number(ui->sbAutoscrollSec->value()));
    hash.insert("disp", QString::number(int(ui->cbxEnableDisplay->isChecked())));

    list.append(ConvertAtype::qslFromHash(hash, "="));
    return list.join(" ");
}

//---------------------------------------------------------------------------------

TwoListWdgtPairList DisplaySettViewer::getAllListItems()
{

    /*        h.insert("relay", "TPb=0,TAOnIP=1,TAOnU=5,TAOnU=5,TOnM=5,TOffM=5,TOffId=0,TOffU=5,TOffI=5,TOffP=5,TOffCf=5,OnU,OnT,OnM,OnR,OnC,OffU,OffId,OffM,OffR,OffC");

    TxA+ //по тарифам де 0 <= x <= max tariff, по Активній експорт
T0A- //по всім тарифам, T0
T0R+
T0R-
Ty // якщо використати y, то замість суми по всім тарифам буде відображатись сума по задіяним тарифам якщо це можливо

//зрізи    не використовую
P+
P-
Q+
Q-

//Напруга
VI струм нейтралі
VIA
VIB
VIC

VU  напруга нейтралі
VUA
VUB
VUC

VP // миттєва потужність споживана по всім фазам (нейтралі)
VPA // миттєва потужність по фазі А
VPB
VPC

VQ+
VQ+A
VQ+B
VQ+C

VQ-
VQ-A
VQ-B
VQ-C

cosF  сумарний кос фі
cosFA
cosFB
cosFC


U - напруга
Idiff - струм дифф
IdiffT - струм дифф час

//загальне
test // тест дисплею
date //дата
time //час
auto=  //авто прокрутка 0 - ні, все інше так
disp=   0 off, 1 on
sec=7  //період автопроктрутки*/



    TwoListWdgtPairList list;

    const QHash<QString,QString> hash = ParemetryzatorData2userReadable::getHashDisplayOptions();
    QList<QString> lk = hash.keys();
    std::sort(lk.begin(), lk.end());

    for(int i = 0, imax = lk.size(); i < imax; i++)
        list.append(TwoListWdgtPairItem(hash.value(lk.at(i)), lk.at(i)));


    return list;



}

//---------------------------------------------------------------------------------

TwoListWdgtPairList DisplaySettViewer::getDefListItems()
{
    QStringList listdefkeys;


    if(true){
        //tariff energy section
        //test vprog T0A+ T1A+ T2A+ T3A+ T4A+
        const QStringList enrgl = QString("A+").split(" ", QString::SkipEmptyParts);

        for(int e = 0, emax = enrgl.size(); e < emax; e++){
            listdefkeys.append(QString("T%1%2").arg(0).arg(enrgl.at(e)));
            for(int i = 1; i < 3; i++){
                listdefkeys.append(QString("T%1%2").arg(i).arg(enrgl.at(e)));
            }
        }
        listdefkeys.append("U");
        listdefkeys.append("test");
        listdefkeys.append("date");
        listdefkeys.append("time");

    }

    return TwoListsWdgt::generateListFromKeys(listdefkeys, getAllListItems());

}

//---------------------------------------------------------------------------------

void DisplaySettViewer::setPageSett(const QString &display)
{
    //QStringList l = QString("T1A+ T2A+ VP U test date time auto=1 sec=7 disp=1").split(" ", QString::SkipEmptyParts);
    const QStringList l = display.split(" ", QString::SkipEmptyParts);


    listwdgt->setTheseListsPairsStrL(l,  getAllListItems());


    const QHash<QString,QString> hash = ConvertAtype::strHashFromQslSmpl(l, "=");


    ui->cbxEnableAutoScroll->setChecked(hash.value("auto").toInt() > 0);
    ui->sbAutoscrollSec->setValue(hash.value("sec").toInt());
    ui->cbxEnableDisplay->setChecked(hash.value("disp").toInt() > 0);
}

//---------------------------------------------------------------------------------

void DisplaySettViewer::applyDefaultSett()
{
    ui->cbxEnableAutoScroll->setChecked(true);
    ui->sbAutoscrollSec->setValue(7);
    ui->cbxEnableDisplay->setChecked(true);
    listwdgt->setTheseDefListsPairs(getDefListItems() , getAllListItems());
}

//---------------------------------------------------------------------------------

void DisplaySettViewer::createWidgets()
{
    listwdgt = new TwoListsWdgt(!isEditable, true, this);//editable, unique values only
    ui->vl4wdgt->addWidget(listwdgt);

    ui->cbxEnableAutoScroll->setEnabled(isEditable);//true);
    ui->sbAutoscrollSec->setEnabled(isEditable);//7);
    ui->cbxEnableDisplay->setEnabled(isEditable);//true);
    applyDefaultSett();
}

//---------------------------------------------------------------------------------
