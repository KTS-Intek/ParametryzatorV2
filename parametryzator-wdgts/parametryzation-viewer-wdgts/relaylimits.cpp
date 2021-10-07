#include "relaylimits.h"
#include "ui_relaylimits.h"

//----------------------------------------------------------------------------

RelayLimits::RelayLimits(const bool &editable, QWidget *parent) :
    QWidget(parent), isEditable(editable),
    ui(new Ui::RelayLimits)
{
    ui->setupUi(this);
    createWidgets();
}

//----------------------------------------------------------------------------

RelayLimits::~RelayLimits()
{
    delete ui;
}

//----------------------------------------------------------------------------

QVariantHash RelayLimits::getDefSettHash()
{
    QVariantHash h;


    h.insert("vltgmax", 265.0); //V
    h.insert("vltgmin", 156.0); //V

    h.insert("crrntmax", 120.0);//A

    h.insert("pt1max", 31800);//W
    h.insert("pt2max", 31800);
    h.insert("pt3max", 31800);
    h.insert("pt4max", 31800);

    h.insert("qt1max", 31800);
    h.insert("qt2max", 31800);
    h.insert("qt3max", 31800);
    h.insert("qt4max", 31800);

    h.insert("pwravrgivl", 60);//secs

    h.insert("frqmax", 55);
    h.insert("frqmin", 45);

    h.insert("pfcmin", 0.0);

    h.insert("pwrcalcmode", 1);//0 -  instantenous, 1 - average

    return h;
}

//----------------------------------------------------------------------------

QVariantHash RelayLimits::getPageSett()
{
    QVariantHash h;


    h.insert("vltgmax", ui->sbVoltageMax->value()); //V
    h.insert("vltgmin", ui->sbVoltageMin->value()); //V

    h.insert("crrntmax", ui->sbCurrentMax->value());//A

    h.insert("pt1max", ui->sbPT1max->value());//W
    h.insert("pt2max", ui->sbPT2max->value());
    h.insert("pt3max", ui->sbPT3max->value());
    h.insert("pt4max", ui->sbPT4max->value());

    h.insert("qt1max", ui->sbQT1max->value());
    h.insert("qt2max", ui->sbQT2max->value());
    h.insert("qt3max", ui->sbQT3max->value());
    h.insert("qt4max", ui->sbQT4max->value());

    h.insert("pwravrgivl", ui->sbPwrAvrgnIval->value());//secs

    h.insert("frqmax", ui->sbFreqMax->value());
    h.insert("frqmin", ui->sbFreqMin->value());

    h.insert("pfcmin", ui->sbPwrFacMin->value());

    h.insert("pwrcalcmode", qMax(0, ui->cbxPowerMode->currentIndex()));//0 -  instantenous, 1 - average

    return h;
}

//----------------------------------------------------------------------------

void RelayLimits::setPageSett(const QVariantHash &relaylimits)
{
    setThisSett(relaylimits);
}

//----------------------------------------------------------------------------

void RelayLimits::applyDefaultSett()
{
    setThisSett(getDefSettHash());
}

//----------------------------------------------------------------------------

void RelayLimits::createWidgets()
{
    ui->sbVoltageMax->setEnabled(isEditable);//(relaylimits.value("vltgmax").toDouble());
    ui->sbVoltageMin->setEnabled(isEditable);//(relaylimits.value("vltgmin").toDouble());

    ui->sbCurrentMax->setEnabled(isEditable);//(relaylimits.value("crrntmax").toDouble());

    ui->sbPT1max->setEnabled(isEditable);//(relaylimits.value("pt1max").toInt());
    ui->sbPT2max->setEnabled(isEditable);//(relaylimits.value("pt2max").toInt());
    ui->sbPT3max->setEnabled(isEditable);//(relaylimits.value("pt3max").toInt());
    ui->sbPT4max->setEnabled(isEditable);//(relaylimits.value("pt4max").toInt());

    ui->sbQT1max->setEnabled(isEditable);//(relaylimits.value("qt1max").toInt());
    ui->sbQT2max->setEnabled(isEditable);//(relaylimits.value("qt2max").toInt());
    ui->sbQT3max->setEnabled(isEditable);//(relaylimits.value("qt3max").toInt());
    ui->sbQT4max->setEnabled(isEditable);//(relaylimits.value("qt4max").toInt());

    ui->sbPwrAvrgnIval->setEnabled(isEditable);//(relaylimits.value("pwravrgivl").toInt());

    ui->sbFreqMax->setEnabled(isEditable);//(relaylimits.value("frqmax").toInt());
    ui->sbFreqMin->setEnabled(isEditable);//(relaylimits.value("frqmin").toInt());

    ui->sbPwrFacMin->setEnabled(isEditable);//(relaylimits.value("pfcmin").toDouble());

    ui->cbxPowerMode->setEnabled(isEditable);//(relaylimits.value("pwrcalcmode").toInt());//0 -  instantenous, 1 - average

    applyDefaultSett();
}

//----------------------------------------------------------------------------

void RelayLimits::setThisSett(const QVariantHash &relaylimits)
{
    ui->sbVoltageMax->setValue(relaylimits.value("vltgmax").toDouble());
    ui->sbVoltageMin->setValue(relaylimits.value("vltgmin").toDouble());

    ui->sbCurrentMax->setValue(relaylimits.value("crrntmax").toDouble());

    ui->sbPT1max->setValue(relaylimits.value("pt1max").toInt());
    ui->sbPT2max->setValue(relaylimits.value("pt2max").toInt());
    ui->sbPT3max->setValue(relaylimits.value("pt3max").toInt());
    ui->sbPT4max->setValue(relaylimits.value("pt4max").toInt());

    ui->sbQT1max->setValue(relaylimits.value("qt1max").toInt());
    ui->sbQT2max->setValue(relaylimits.value("qt2max").toInt());
    ui->sbQT3max->setValue(relaylimits.value("qt3max").toInt());
    ui->sbQT4max->setValue(relaylimits.value("qt4max").toInt());

    ui->sbPwrAvrgnIval->setValue(relaylimits.value("pwravrgivl").toInt());

    ui->sbFreqMax->setValue(relaylimits.value("frqmax").toInt());
    ui->sbFreqMin->setValue(relaylimits.value("frqmin").toInt());

    ui->sbPwrFacMin->setValue(relaylimits.value("pfcmin").toDouble());

    ui->cbxPowerMode->setCurrentIndex(relaylimits.value("pwrcalcmode").toInt());//0 -  instantenous, 1 - average
}

//----------------------------------------------------------------------------
