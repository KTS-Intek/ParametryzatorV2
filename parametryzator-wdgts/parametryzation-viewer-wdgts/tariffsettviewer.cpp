#include "tariffsettviewer.h"
#include "ui_tariffsettviewer.h"


///[!] parametryzator-base
#include "src/datafilters/helpers/paremetryzatordata2userreadable.h"


#include "src/base/convertatype.h"

//-------------------------------------------------------------------------------------

TariffSettViewer::TariffSettViewer(const bool &editable, GuiHelper *gHelper, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TariffSettViewer), gHelper(gHelper), isEditable(editable)
{
    ui->setupUi(this);
    createWidgets();
}

//-------------------------------------------------------------------------------------

TariffSettViewer::~TariffSettViewer()
{
    delete ui;
}

void TariffSettViewer::findTariffIndex(const QVariantHash &hash, QComboBox *cbxTariffProgram, QMap<QString, int> &tariffline2cbxindex)
{


    if(hash.isEmpty()){
        cbxTariffProgram->setCurrentIndex(-1);
        return;
    }

    int indx = cbxTariffProgram->findData(hash);
    if(indx < 0){

        QString comment = hash.value("comment").toString();
        if(!comment.isEmpty()){
            indx = cbxTariffProgram->findText(comment);

        }

        if(comment.isEmpty()){
            const QString tarrrlne = tariffHash2line(hash);
            if(!tarrrlne.isEmpty() && tariffline2cbxindex.contains(tarrrlne)){
                indx = tariffline2cbxindex.value(tarrrlne);
                comment = cbxTariffProgram->itemText(indx);
            }
        }


        if(!comment.isEmpty() && indx >= 0){
            cbxTariffProgram->setCurrentIndex(indx);
            return;
        }


        if(comment.isEmpty()){
            for(int i = 0; i < 1000; i++){
                comment = tr("Custom programm %1").arg(i);

                indx = cbxTariffProgram->findText(comment);
                if(indx < 0){
                    cbxTariffProgram->addItem(comment, hash);
                    const QString strtarr = tariffHash2line(hash);
                    if(!strtarr.isEmpty() && !tariffline2cbxindex.contains(strtarr))
                        tariffline2cbxindex.insert(strtarr, cbxTariffProgram->count() - 1);

                    break;
                }
            }
        }
        if(indx < 0 && !comment.isEmpty()){
            cbxTariffProgram->addItem(comment, hash);
            tariffline2cbxindex.insert(tariffHash2line(hash), cbxTariffProgram->count() - 1);

            cbxTariffProgram->setCurrentIndex(cbxTariffProgram->count() - 1);

        }//else is impossible, so if it ever happends ignore it

    }else{
        cbxTariffProgram->setCurrentIndex(indx);
    }
}

//-------------------------------------------------------------------------------------

QString TariffSettViewer::tariffHash2line(const QVariantHash &h)
{
    QList<QString> lk = h.keys();
    std::sort(lk.begin(), lk.end());
    QStringList out;



    for(int i = 0, imax = lk.size(); i < imax; i++){
        const QString key = lk.at(i);
        if(key.startsWith("Season_count") || key.startsWith("DR") || key.startsWith("S") || key.endsWith("SDR")){
            out.append(QString("%1\t%2").arg(key).arg(h.value(key).toString()));
        }
    }
    return out.join("\n");
}

//-------------------------------------------------------------------------------------

QVariantHash TariffSettViewer::defVal4tariff()
{
    QVariantHash hash;
    if(true){
        QVariantHash h;
        h.insert("comment", tr("Один тариф"));
        h.insert("Season_count", 1);
        h.insert("S0", "1.1");
        h.insert("0SDR", "0");
        h.insert("DR0", "T1{00:00-24:00}");
        hash.insert(h.value("comment").toString(), h);
    }

    if(true){
        QVariantHash h;
        h.insert("comment", "Два тарифи для населення (Постанова НКРЕКП від 26.02.2015 № 221)");
        h.insert("link", "http://www.nerc.gov.ua/?id=14360");
        h.insert("Season_count", 1);
        h.insert("S0", "1.1");
        h.insert("0SDR", "0");
        h.insert("DR0", "T1{07:00-23:00}; T2{23:00-07:00}");
        hash.insert(h.value("comment").toString(), h);
    }

    if(true){
        QVariantHash h;
        h.insert("comment", "Три тарифи для населення (Постанова НКРЕКП від 26.02.2015 № 221)");
        h.insert("link", "http://www.nerc.gov.ua/?id=14360");
        h.insert("Season_count", 1);
        h.insert("S0", "1.1");
        h.insert("0SDR", "0");
        h.insert("DR0", "T1{08:00-11:00, 20:00-22:00}; T2{07:00-08:00, 11:00-20:00, 22:00-23:00}; T3{23:00-07:00}");
        hash.insert(h.value("comment").toString(), h);
    }


    if(true){
        QVariantHash h;
        h.insert("comment", "Два тарифи для юр. осіб (ДП НЕК \"Укренерго\" факсограма від 25.11.2014 р. № 06-1-1/13396)");
        h.insert("link", "http://www.oe.if.ua/tarif_jur.php?id=02214");
        h.insert("Season_count", 1);
        h.insert("S0", "1.1");
        h.insert("0SDR", "0");
        h.insert("DR0", "T1{07:00-23:00}; T2{23:00-07:00}");
        hash.insert(h.value("comment").toString(), h);
    }

    if(true){
        QVariantHash h;
        h.insert("comment", "Три тарифи для юр. осіб (ДП НЕК \"Укренерго\" факсограма від 25.11.2014 р. № 06-1-1/13396)");
        h.insert("link", "http://www.oe.if.ua/tarif_jur.php?id=02214");
        h.insert("Season_count", 3);
        h.insert("S0", "1.1,1.11");
        h.insert("S1", "1.3,1.9");
        h.insert("S2", "1.5");

        h.insert("0SDR", "0");
        h.insert("DR0", "T1{08:00-10:00, 17:00-21:00}; T2{06:00-08:00, 10:00-17:00, 21:00-23:00}; T3{23:00-06:00}");

        h.insert("1SDR", "1");
        h.insert("DR1", "T1{08:00-10:00, 18:00-22:00}; T2{06:00-08:00, 10:00-18:00, 22:00-23:00}; T3{23:00-06:00}");

        h.insert("2SDR", "2");
        h.insert("DR2", "T1{08:00-11:00, 20:00-23:00}; T2{07:00-08:00, 11:00-20:00, 23:00-24:00}; T3{00:00-07:00}");

        hash.insert(h.value("comment").toString(), h);
    }
    return hash;
}

//-------------------------------------------------------------------------------------

QVariantHash TariffSettViewer::getPageSett()
{
    return ui->cbxTariffProgram->currentData().toHash();
}



//-------------------------------------------------------------------------------------

void TariffSettViewer::setPageSett(QVariantHash hash)
{

    findTariffIndex(hash, ui->cbxTariffProgram, tariffline2cbxindex);
}

//-------------------------------------------------------------------------------------

void TariffSettViewer::applyDefaultSett()
{
    ui->cbxTariffProgram->setCurrentIndex(0);

}

//-------------------------------------------------------------------------------------

void TariffSettViewer::showCurrentTariffProgram(const int &indx)
{

    if(indx < 0){
        emit setTariffProgramPte(tr("no data"));
        return;
    }
    const QVariantHash h = ui->cbxTariffProgram->itemData(indx).toHash();

    int tariffcount;
    const QString lines = ParemetryzatorData2userReadable::jsonTariff2prettyStrH(h, tariffcount).join("<br>");

    emit setTariffProgramPte(lines);



}

//-------------------------------------------------------------------------------------

void TariffSettViewer::createWidgets()
{

    ui->cbxTariffProgram->clear();
    ui->cbxTariffProgram->setCurrentIndex(-1);
    tariffline2cbxindex.clear();

    connect(ui->cbxTariffProgram, SIGNAL(currentIndexChanged(int)), this, SLOT(showCurrentTariffProgram(int)));


    SmplPteWdgt *tariffwdgt = new SmplPteWdgt(tr("Tariff"), true, true, gHelper, false, this);
    ui->vl4pte->addWidget(tariffwdgt);

    connect(this, &TariffSettViewer::setTariffProgramPte, tariffwdgt, &SmplPteWdgt::setPteText);

    const QVariantHash h = defVal4tariff();


    QList<QString> lk = h.keys();

    std::sort(lk.begin(), lk.end());

    for(int i = 0, imax = lk.size(); i < imax; i++){
        ui->cbxTariffProgram->addItem(lk.at(i), h.value(lk.at(i)));


        const QString strtarr = tariffHash2line(h.value(lk.at(i)).toHash());
        if(!strtarr.isEmpty() && !tariffline2cbxindex.contains(strtarr))
            tariffline2cbxindex.insert(strtarr, i);
    }

    ui->cbxTariffProgram->setEnabled(isEditable);

    applyDefaultSett();
}

//-------------------------------------------------------------------------------------
