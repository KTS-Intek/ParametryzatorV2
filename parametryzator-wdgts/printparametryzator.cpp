#include "printparametryzator.h"
#include "ui_printparametryzator.h"

#include <QDesktopServices>
#include <QPrinterInfo>
#include <QCompleter>

#include "parametryzation-viewer-wdgts/tariffsettviewer.h"

///[!] parameteryzator-base
#include "src/datafilters/helpers/paremetryzatordata2userreadable.h"
#include "parametryzator-src/parametryzatornisearcher.h"


///[!] printer-widgets
#include "src/printer/printerhelper.h"


///[!] widgets-shared
#include "gui-src/printinghelper.h"


//--------------------------------------------------------------

PrintParametryzator::PrintParametryzator(GuiHelper *gHelper, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintParametryzator)
{
    ui->setupUi(this);
    printtv = new PrintTvPreview(gHelper, this);
    connect(printtv, &PrintTvPreview::pageEndInit, this, &PrintParametryzator::createWidgets);
    ui->vl4wdgt->addWidget(printtv);
    connect(printtv, &PrintTvPreview::udpateInfoAboutCurrentRow, this, &PrintParametryzator::udpateInfoAboutCurrentRow);

    ui->pbCurrentEdit->hide();//I don't want to add this feature at this moment

    ui->tabWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
}

//--------------------------------------------------------------

PrintParametryzator::~PrintParametryzator()
{
    delete ui;
}

//--------------------------------------------------------------

QVariantHash PrintParametryzator::getPageSett()
{
    QVariantHash h;

    h.insert("leConsumerAddress", ui->leConsumerAddress->text());

    h.insert("sbDocNumber", ui->sbDocNumber->value() );
    h.insert("sbPrecisionGroup", ui->sbPrecisionGroup->value());

    h.insert("cbxCurrentCoefitient", ui->cbxCurrentCoefitient->isChecked());
    h.insert("sbCurrentNumerator", ui->sbCurrentNumerator->value());
    h.insert("sbCurrentDenominator", ui->sbCurrentDenominator->value());

    h.insert("cbxVoltageCoefitient", ui->cbxVoltageCoefitient->isChecked());
    h.insert("sbVoltageNumerator", ui->sbVoltageNumerator->value());
    h.insert("sbVoltageDenominator", ui->sbVoltageDenominator->value());

    h.insert("cbxPrintProfile", ui->cbxPrintProfile->currentText());

    h.insert("splitter", ui->splitter->saveState());
    h.insert("cbxPrinter", ui->cbxPrinter->currentText());

    lastpagesett.lastGrpSett = h;
    return h;
}

//--------------------------------------------------------------

void PrintParametryzator::setCurrentCbx(QComboBox *cbx, const QString &currtxt)
{
    //    ui->cbxModel->setCurrentText(lastmodel);
    if(!currtxt.isEmpty()){
        const int indx = cbx->findText(currtxt);
        if(indx >= 0){
            cbx->setCurrentIndex(indx);
            return;
        }
        cbx->setCurrentText(currtxt);
        return;
    }
    if(cbx->count() > 0)
        cbx->setCurrentIndex(0);
}

//--------------------------------------------------------------

bool PrintParametryzator::canStartPrintingSmart(const int &mode, const bool &isPreview)
{
    QStringList messsages;
    const bool canprint = canStartPrinting(mode, isPreview, messsages);
    if(!canprint){
        emit printtv->gHelper->showMessageSlot(messsages.join("<br>"));

    }
    return canprint;
}

//--------------------------------------------------------------

bool PrintParametryzator::canStartPrinting(const int &mode, const bool &isPreview, QStringList &messages)
{

    //#define PARAM_PRINT_MODE_VISIBLE    2
    //#define PARAM_PRINT_MODE_SELECTED   1
    //#define PARAM_PRINT_MODE_CURRENTRW  0

    if(!isPreview && ui->cbxPrinter->currentIndex() < 0){
        messages.append(tr("There is no any printer"));
        ui->tbUpdatePrint->animateClick();
        return false;
    }



    QStringList nis;
    QList<int> l;


    if(mode == PARAM_PRINT_MODE_CURRENTRW){//ignore, because it takes data from PreviewWdgt
        if(!ui->leNI->text().simplified().trimmed().isEmpty()){
             nis.append(ui->leNI->text().simplified().trimmed());
             l.append(ui->cbxIgnoreDuplicates->isChecked() ? 0 : printtv->findNisInObjectDir(nis.first()));
        }else{
            printtv->updateInfoAboutCurrentRow();
        }
    }else{
        l = printtv->getDuplicates(nis, mode);
    }

    if(l.isEmpty()){//there is nothing to print

        messages.append(tr("There is nothing to pring"));
    }else{
        for(int i = 0, imax = l.size(); i < imax; i++){
            if(l.at(i) > 0){
                messages.append(tr("NI '%1' already exists [%2]").arg(nis.at(i)).arg(l.at(i)));
            }
        }
    }

    return messages.isEmpty();
}

//--------------------------------------------------------------

bool PrintParametryzator::printAreport(const bool &usepreview)
{
    const bool isPdfMode = (ui->cbxPrinter->currentIndex() < 1 && !usepreview);

    bool rez = false;
    QStringList messages;

    if(isPdfMode){
        if(lastpagesett.lastpdffilename.isEmpty())
            lastpagesett.lastpdffilename = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

        const QString fileName = QFileDialog::getSaveFileName(this, tr("Save pdf as..."), QString("%1/%2.pdf").arg(lastpagesett.lastpdffilename).arg(ui->leNI->text()));
        if(fileName.isEmpty())
            return false;

        QFileInfo fi(fileName);
        lastpagesett.lastpdffilename = fi.absoluteDir().path();


        rez = print2pdfFile(messages, fileName, true);



    }else{
        const Print2file::Import2fileSett importSett = PrintingHelper::getImport2fileSett("-", QStringList(), QStringList());//just load saved settings

        if(usepreview){
            //    static bool print2dev(QString &mess, QWidget *parent, const QString &html, const QFont &font, const Import2fileSett &sett);
            QString message;
            const QString lastprintername = PrintingHelper::print2devName(message, this, ui->txtPreview->toHtml(), ui->txtPreview->font(), importSett);
            rez = !lastprintername.isEmpty();

            if(rez && ui->cbxPrinter->findText(lastprintername) > 0){
                ui->cbxPrinter->setCurrentIndex(ui->cbxPrinter->findText(lastprintername));
            }
            if(!message.isEmpty())
                messages.append(message);


        }else{
            rez = PrintingHelper::print2devViaObject(messages, ui->txtPreview, importSett, ui->cbxPrinter->currentText());
        }

    }

    if(!messages.isEmpty()){
        printtv->gHelper->showMessageSlot(messages.join("<br>"));

    }

    if(!rez && messages.isEmpty()){
        if(!usepreview)//it is normal for preview mode, to exit with rez=false
            printtv->gHelper->showMessageSlot(tr("Something is wrong with printing system"));
        return false;
    }



    if(rez){
        const QString ni = ui->leNI->text().simplified().trimmed();
        const int docNumber = ui->sbDocNumber->value();


        messages.clear();
        if(print2pdf(messages)){
            emit onReportReady(ni); //remove this row
            ui->sbDocNumber->setValue(docNumber + 1);
        }

        if(!messages.isEmpty())
            printtv->gHelper->showMessageSlot(messages.join("<br>"));

    }

    return true;

}

//--------------------------------------------------------------

bool PrintParametryzator::print2pdf(QStringList &messages)
{

    QString fileName = lastpagesett.lastbasepdffilename;

    const QString leReplaceNIrule4fileFind = lastpagesett.lastMainPrintSett.value("leReplaceNIrule4fileFind").toString();
    const QString leReplaceNIrule4fileReplace = lastpagesett.lastMainPrintSett.value("leReplaceNIrule4fileReplace").toString();
    fileName = ParametryzatorNISearcher::replaceNiChar(fileName, leReplaceNIrule4fileFind, leReplaceNIrule4fileReplace);

    fileName.remove(QRegExp(QString::fromUtf8("*|<>\":?")));

    fileName = QString("%1/%2").arg(lastpagesett.lastMainPrintSett.value("lePath2pdfDir").toString()).arg(fileName);

    if(fileName.left(4).toLower() != ".pdf")
        fileName.append(".pdf");

    return print2pdfFile(messages, fileName, false);
}

//--------------------------------------------------------------

bool PrintParametryzator::print2pdfFile(QStringList &messages, const QString &path2pdf, const bool &openthen)
{
    QString lastPdfFileName = path2pdf;

    QFileInfo fi(path2pdf);
    if(fi.exists()){

        if(lastPdfFileName.right(4).toLower() == ".pdf")
            lastPdfFileName.chop(4);

        const QString lastPdfFileNameBase = lastPdfFileName;

        for(int i = 2; i < 100000; i++){
            lastPdfFileName = QString("%1_%2.pdf").arg(lastPdfFileNameBase).arg(i);
            fi.setFile(lastPdfFileName);
            if(!fi.exists())
                break;
            lastPdfFileName.clear();
        }
    }

    if(lastPdfFileName.isEmpty()){
        messages.append(tr("Bad filename"));
        return false;
    }

    return save2pdf(messages, lastPdfFileName, openthen);

}


//--------------------------------------------------------------

bool PrintParametryzator::save2pdf(QStringList &messages, const QString &pdfFileName, const bool &openthen)
{
    QFileInfo fi(pdfFileName);
    QDir dir(fi.absoluteDir());
    if(!dir.exists()){
        dir.mkpath(fi.absoluteDir().path());
        reconfigureCompleter4addr();
    }

    qDebug() << "pdf dir " << fi.absoluteDir().path();
    qDebug() << "pdf file " << fi.absoluteFilePath();


//    PrintingHelper::print2devHtmlString();

    QPrinter printer(QPrinter::HighResolution);
    const PrintingHelper::Import2fileSett importSett = PrintingHelper::getImport2fileSett(pdfFileName, QStringList(), QStringList());
    PrintingHelper::setupPrinterDevice(printer, importSett);
    printer.setOutputFileName(importSett.fileName);


    ui->txtPreview->print(&printer);

    fi.setFile(pdfFileName);
    if(!fi.exists()){
        messages.append(tr("It is not able to save the PDF file"));
        return false;
    }

    if(openthen){
#ifdef Q_OS_WIN

        QDesktopServices::openUrl(QUrl("file:///" + pdfFileName));

#else
        QDesktopServices::openUrl(QUrl("file://" + pdfFileName));
#endif
    }
    return true;
}


//--------------------------------------------------------------

void PrintParametryzator::clearThePage()
{
    ui->rbResetDate->animateClick();
    ui->tabWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);

    if(!ui->pbGroupSkeletonPreview->isChecked())
        ui->pbGroupSkeletonPreview->animateClick();
    ui->sbPrecisionGroup->setValue(1.0);
    ui->sbCurrentDenominator->setValue(1);
    ui->sbCurrentNumerator->setValue(1);
    ui->sbVoltageDenominator->setValue(1);
    ui->sbVoltageNumerator->setValue(1);

    ui->tbUpdateAddresses->animateClick();

}

//--------------------------------------------------------------

void PrintParametryzator::setPageSettpPrintingGrp(QVariantHash lastPageSett)
{
    lastpagesett.lastGrpSett = lastPageSett;

    ui->leConsumerAddress->setText(lastPageSett.value("leConsumerAddress").toString());

    ui->sbDocNumber->setValue(lastPageSett.value("sbDocNumber", 1).toInt() );
    ui->sbPrecisionGroup->setValue(lastPageSett.value("sbPrecisionGroup", 1.0).toDouble());

    ui->cbxCurrentCoefitient->setChecked(lastPageSett.value("cbxCurrentCoefitient", false).toBool());
    ui->sbCurrentNumerator->setValue(lastPageSett.value("sbCurrentNumerator", 1).toInt());
    ui->sbCurrentDenominator->setValue(lastPageSett.value("sbCurrentDenominator", 1).toInt());

    ui->cbxVoltageCoefitient->setChecked(lastPageSett.value("cbxVoltageCoefitient", false).toBool());
    ui->sbVoltageNumerator->setValue( lastPageSett.value("sbVoltageNumerator", 1).toInt());
    ui->sbVoltageDenominator->setValue(lastPageSett.value("sbVoltageDenominator", 1).toInt());


    const int txtindx = ui->cbxPrintProfile->findText(lastPageSett.value("cbxPrintProfile").toString());

    if(txtindx < 0)
        ui->cbxPrintProfile->setCurrentIndex(0);


    on_cbxCurrentCoefitient_clicked(ui->cbxCurrentCoefitient->isChecked());
    on_cbxVoltageCoefitient_clicked(ui->cbxVoltageCoefitient->isChecked());

    ui->splitter->restoreState(lastPageSett.value("splitter").toByteArray());

    ui->cbxPrinter->clear();
    ui->cbxPrinter->addItem(lastPageSett.value("cbxPrinter").toString());
    ui->cbxPrinter->setCurrentIndex(0);//it makes it a default printer
    ui->tbUpdatePrint->animateClick();

    ui->rbResetDate->animateClick();

    printtv->setPageSettpPrintingGrp(lastPageSett);
    emit updateHtmlPreviewLater();

}

//--------------------------------------------------------------

void PrintParametryzator::setPageSettpPrintingMain(QVariantHash lastPageSett)
{
    lastpagesett.lastMainPrintSett = lastPageSett;

    ui->lblPath2pdf->setText(lastPageSett.value("lePath2pdfDir").toString());

    //    h.insert("pteMeterProg2meterType", QString("MTX1A MTX 1A10\n"
    //                                               "MTX3R MTX 3R30\n"
    //                                               "CE303 CE303\n"
    //                                               "CE301 CE301\n"
    //                                               "CE301 CE303-U(A)\n"
    //                                               "CE102 CE102\n"
    //                                               "102 CE102-U\n"
    //                                               "301v12 CE303 UA\n"
    //                                               "G1B GAMA100\n"
    //                                               "G3B GAMA300"));const QStringList pteMeterProg2meterType = QString(mainpramssett.value("pteMeterProg2meterType").toString()).replace("\r", "\n").split("\n", QString::SkipEmptyParts);
    lastpagesett.meterprot2sett = ParametryzatorPrintHelper::getOneMetersInfoHash(lastPageSett);



    QList<QString> lk = lastpagesett.meterprot2sett.keys();
    std::sort(lk.begin(), lk.end());

    QStringList amperes;
    QStringList voltages;
    QStringList manufactures;
    const QString lastmodel = ui->cbxModel->currentText();
    const QString lastampers = ui->cbxCurrent->currentText();
    const QString lastvolta = ui->cbxVoltage->currentText();
    const QString lastmanuf = ui->cbxManuf->currentText();

    ui->cbxManuf->clear();
    ui->cbxVoltage->clear();
    ui->cbxCurrent->clear();
    ui->cbxModel->clear();

    for(int i = 0, imax = lk.size(); i < imax; i++){
        const QString meterprog = lk.at(i);
        const MeterProg2info oneproginfo =  lastpagesett.meterprot2sett.value(meterprog);

        ui->cbxModel->addItem(oneproginfo.metermodelhuman, meterprog);

        if(!amperes.contains(oneproginfo.ampers)){
            amperes.append(oneproginfo.ampers);
            ui->cbxCurrent->addItem(oneproginfo.ampers);
        }

        if(!voltages.contains(oneproginfo.voltage)){
            voltages.append(oneproginfo.voltage);
            ui->cbxVoltage->addItem(oneproginfo.voltage);
        }


        if(!manufactures.contains(oneproginfo.manuf)){
            manufactures.append(oneproginfo.manuf);
            ui->cbxManuf->addItem(oneproginfo.manuf);
        }

    }

    setCurrentCbx(ui->cbxModel, lastmodel);
    setCurrentCbx(ui->cbxCurrent, lastampers);
    setCurrentCbx(ui->cbxVoltage, lastvolta);
    setCurrentCbx(ui->cbxManuf, lastmanuf);


    printtv->setPageSettpPrintingMain(lastPageSett);

    updateleConsumerAddressDest();
    emit updateHtmlPreviewLater();
}

//--------------------------------------------------------------

void PrintParametryzator::setPageSettpPrintingHtml(QVariantHash lastPageSett)
{
    lastpagesett.lastHtmlUserProfiles = lastPageSett.value("userprofiles").toHash();

    lastpagesett.lastHtmlDefaultProfile = lastPageSett.value("standard").toString();

    const QString cbxprofiletxt = ui->cbxPrintProfile->currentText();
    const QString currdata = ui->cbxPrintProfile->currentData().toString();


    QList<QString> lk = lastpagesett.lastHtmlUserProfiles.keys();
    std::sort(lk.begin(), lk.end());




    //            htmlEditorStandardPte->setPteText(lastPageSett.value("standard").toString());
    //            if(htmlEditorStandardPte->getPte()->toPlainText().isEmpty())
    //                loadDefaultHtml4thisEditor(htmlEditorStandardPte->getPte());

    //            QVariantHash h = lastPageSett.value("userprofiles").toHash();
    //            if(!h.isEmpty()){
    //                QList<QString> lk = h.keys();
    //                std::sort(lk.begin(), lk.end());
    //                for(int i = 0, imax = lk.size(); i < imax; i++){

    //                    const QString profilename = lk.at(i);
    //                    const QString profiletext = h.value(profilename).toString();

    //                    createUserHtmlPte(profilename, profiletext);




    //                }
    //            }

    ui->cbxPrintProfile->clear();
    ui->cbxPrintProfile->setCurrentIndex(-1);

    ui->cbxPrintProfile->addItem(tr("Standard"), lastpagesett.lastHtmlDefaultProfile);

    for(int i = 0, imax = lk.size(); i < imax; i++){
        ui->cbxPrintProfile->addItem(lk.at(i), lastpagesett.lastHtmlUserProfiles.value(lk.at(i)).toString());
    }
    int indx = ui->cbxPrintProfile->findText(cbxprofiletxt);
    if(indx < 0)
        indx = ui->cbxPrintProfile->findData(currdata);
    if(indx < 0)
        indx = 0;//Standard
    ui->cbxPrintProfile->setCurrentIndex(indx);

    emit updateHtmlPreviewLater();

}

//--------------------------------------------------------------

void PrintParametryzator::createWidgets()
{

    connect(ui->leConsumerAddress, SIGNAL(textChanged(QString)), this, SLOT(updateleConsumerAddressDest()));



    connect(ui->tbUpdateAddresses, SIGNAL(clicked(bool)), this, SLOT(reconfigureCompleter4addr()));


    connect(this, &PrintParametryzator::startPrintingTariffReports, printtv, &PrintTvPreview::startPrintingTariffReports);
    connect(this, &PrintParametryzator::onReportReady, printtv, &PrintTvPreview::onReportReady);//remove a row, and go to the next, update preview
    connect(ui->tbOpenProfilesEditor, SIGNAL(clicked(bool)), this, SIGNAL(gimmeMyLastSettingsPrintHtmlProfiles()));

    clearThePage();

    ui->cbxTariffProgram->clear();
    tariffline2cbxindex.clear();


    if(true){
        const QVariantHash h = TariffSettViewer::defVal4tariff();


        QList<QString> lk = h.keys();

        std::sort(lk.begin(), lk.end());

        for(int i = 0, imax = lk.size(); i < imax; i++){
            ui->cbxTariffProgram->addItem(lk.at(i), h.value(lk.at(i)));


            const QString strtarr = TariffSettViewer::tariffHash2line(h.value(lk.at(i)).toHash());
            if(!strtarr.isEmpty() && !tariffline2cbxindex.contains(strtarr))
                tariffline2cbxindex.insert(strtarr, i);
        }

    }



    if(true){
        //send grp settings 2 printtv
        QTimer *t = new QTimer(this);
        t->setInterval(111);
        t->setSingleShot(true);


        connect(ui->cbxPrintProfile, SIGNAL(currentIndexChanged(int)), t, SLOT(start()));

        connect(ui->dteDocDate, SIGNAL(dateChanged(QDate)), t, SLOT(start()));
        connect(ui->sbDocNumber, SIGNAL(valueChanged(int)), t, SLOT(start()));
        connect(ui->leConsumerAddress, SIGNAL(textChanged(QString)), t, SLOT(start()));

        connect(ui->sbPrecisionGroup, SIGNAL(valueChanged(double)), t, SLOT(start()));
        connect(ui->sbCurrentDenominator, SIGNAL(valueChanged(int)), t, SLOT(start()));
        connect(ui->sbCurrentNumerator, SIGNAL(valueChanged(int)), t, SLOT(start()));
        connect(ui->sbVoltageDenominator, SIGNAL(valueChanged(int)), t, SLOT(start()));
        connect(ui->sbVoltageNumerator, SIGNAL(valueChanged(int)), t, SLOT(start()));

        connect(ui->cbxCurrentCoefitient, SIGNAL(clicked(bool)), t, SLOT(start()));
        connect(ui->cbxVoltageCoefitient, SIGNAL(clicked(bool)), t, SLOT(start()));

        connect(t, SIGNAL(timeout()), this, SLOT(onGroupSettingsChanged()));
    }

    if(true){
        //send one meter settings 2 printtv
        QTimer *t = new QTimer(this);
        t->setInterval(111);
        t->setSingleShot(true);

        connect(ui->cbxModel, SIGNAL(currentTextChanged(QString)), t, SLOT(start()));
        connect(ui->leSn, SIGNAL(textChanged(QString)), t, SLOT(start()));
        connect(ui->leNI, SIGNAL(textChanged(QString)), t, SLOT(start()));

        connect(ui->cbxVoltage, SIGNAL(currentTextChanged(QString)), t, SLOT(start()));
        connect(ui->cbxCurrent, SIGNAL(currentTextChanged(QString)), t, SLOT(start()));
        connect(ui->cbxTariffProgram, SIGNAL(currentTextChanged(QString)), t, SLOT(start()));
        connect(ui->cbxManuf, SIGNAL(currentTextChanged(QString)), t, SLOT(start()));

        connect(ui->leConsumerAddress, SIGNAL(textChanged(QString)), t, SLOT(start()));


        connect(this, SIGNAL(updateHtmlPreviewLater()), t, SLOT(start()));

        connect(t, SIGNAL(timeout()), this, SLOT(updateHtmlPreview()));

    }
    emit gimmeMyLastSettings();


    PrinterHelper *ph = new PrinterHelper(ui->cbxPrinter, this);
    connect(ui->tbUpdatePrint, SIGNAL(clicked(bool)), ph, SLOT(request2update()));
    ph->request2update();


    connect(ui->tbSetupPrinting, SIGNAL(clicked(bool)), this, SIGNAL(openPrintSettings()));
    connect(printtv, &PrintTvPreview::ask2printSelected, this, &PrintParametryzator::ask2printSelected);
    connect(printtv, &PrintTvPreview::ask2PreviewPrintSelected, this, &PrintParametryzator::ask2PreviewPrintSelected);


    if(true){
        QTimer *t = new QTimer(this);
        t->setInterval(333);
        t->setSingleShot(true);

        connect(ui->cbxIgnoreDuplicates, SIGNAL(clicked(bool)),  this, SLOT(checkLeNiLater()));

        connect(ui->leNI, SIGNAL(textChanged(QString)), t, SLOT(start()));
        connect(ui->leConsumerAddress, SIGNAL(textChanged(QString)), t, SLOT(start()));

        connect(this, SIGNAL(onPrintingOperationFinished()), t, SLOT(start()));

        connect(t, SIGNAL(timeout()), this, SLOT(checkLeNiLater()));
        t->start();
    }
}

//--------------------------------------------------------------

void PrintParametryzator::savePageSett()
{
    emit setLastPageSett(getPageSett());
}

//--------------------------------------------------------------

void PrintParametryzator::onGroupSettingsChanged()
{
    printtv->setPageSettpPrintingGrp(getPageSett());
    updateHtmlPreview();
}

//--------------------------------------------------------------

void PrintParametryzator::updateHtmlPreview()
{

    //    QVariantHash lastHtmlUserProfiles;
    //    QString lastHtmlDefaultProfile;

    QString inputString = (ui->cbxPrintProfile->currentIndex() < 1) ?
                lastpagesett.lastHtmlDefaultProfile :
                lastpagesett.lastHtmlUserProfiles.value(ui->cbxPrintProfile->currentText()).toString();

    if(inputString.isEmpty()){//get the
        QFile file(":/def/default.html");
        file.open(QFile::ReadOnly);
        inputString = file.readAll();
        file.close();
    }
    /*
 * QString ParametryzatorPrintHelper::replaceKeysWithValue(
 * const ParametryzatorPrintInputParams &params,
 * const ParametryzatorPrintInputMeterInfo &meterinfo,
 * const ParametryzatorPrintInputMeterInfoHuman &meterhuman,
 * const QString &inputString)
*/



    //    static ParametryzatorPrintInputMeterInfoHuman onemeterprot2sett = ParametryzatorPrintHelper::getInfoHumanFromVariantHashExt(const QString &meterversion, const MeterProg2MeterHumanData &hashpramssett);

    const ParametryzatorPrintInputParams params = ParametryzatorPrintHelper::paramsFromHash(lastpagesett.lastGrpSett, lastpagesett.lastMainPrintSett);
    ParametryzatorPrintInputMeterInfo meterinfo;

    meterinfo.metermodel = ui->cbxModel->currentText();
    meterinfo.meterNI = ui->leNI->text();
    meterinfo.meterSN = ui->leSn->text();



    //relay ???
    meterinfo.relayinfo = ParemetryzatorData2userReadable::getPrettyRelayLineSimplified(lastpagesett.lastmeterdata.value("relay").toString()).join(". ");

    int t;
    meterinfo.meterTariffSchedule = ParemetryzatorData2userReadable::jsonTariff2prettyStrH(ui->cbxTariffProgram->currentData().toHash(), t).join("<br>");//tariff program
    meterinfo.meterTariffCount = QString::number(t);



    ParametryzatorPrintInputMeterInfoHuman meterhuman;
    meterhuman.cbxCurrent = ui->cbxCurrent->currentText();
    meterhuman.lastManufName = ui->cbxManuf->currentText();
    meterhuman.meterVolta = ui->cbxVoltage->currentText();
    const QString meterprog = ui->cbxModel->currentData().toString();
    //    const MeterProg2info oneproginfo =  lastpagesett.meterprot2sett.value(meterprog);

    meterhuman.meterFrom = lastpagesett.meterprot2sett.value(meterprog).city;// "need to setup32";
    meterhuman.meterTypeH =  ParametryzatorPrintHelper::phaseCountFromVoltage(meterhuman.meterVolta);// "need to setup33";




    ui->txtPreview->setHtml(ParametryzatorPrintHelper::replaceKeysWithValue(

                                params,
                                meterinfo,
                                meterhuman,
                                inputString));
//to prepary for printing
    lastpagesett.lastbasepdffilename = ParametryzatorPrintHelper::replaceKeysWithValue(

                params,
                meterinfo,
                meterhuman,
                 lastpagesett.lastMainPrintSett.value("lePdfFileName").toString());
}

//--------------------------------------------------------------

void PrintParametryzator::udpateInfoAboutCurrentRow(int duplicates, QString model, QString sn, QString ni, QString voltage, QString current, QString tariffp, QVariantHash meterdata, QString manuf)
{
    setCurrentCbx(ui->cbxModel, model);
    ui->leSn->setText(sn);
    ui->leNI->setText(ni);

    setCurrentCbx(ui->cbxVoltage, voltage);
    setCurrentCbx(ui->cbxCurrent, current);


    lastpagesett.lastmeterdata = meterdata;



    setCurrentCbx(ui->cbxManuf, manuf);

    const bool canprint = !meterdata.isEmpty();

    ui->pbPrintGroup->setEnabled(canprint);
    ui->pbPreviewGroup->setEnabled(canprint);

//    ui->pbCurrentPreview->setEnabled(canprint);
//    ui->pbCurrentPrint->setEnabled( canprint);


    if(!tariffp.isEmpty())
        TariffSettViewer::findTariffIndex(meterdata.value("tariff").toHash(), ui->cbxTariffProgram, tariffline2cbxindex);

    ui->cbxIgnoreDuplicates->setToolTip(tr("NI %1, duplicates %2").arg(ni).arg(duplicates));

    ui->leNI->setStyleSheet( (duplicates > 0) ? "background-color: rgb(255, 117, 119);" : "" );

    if(ui->cbxIgnoreDuplicates->isChecked())
        duplicates = 0;
    ui->wdgtCurrentMeter->setEnabled((duplicates < 1) );
    updateHtmlPreview();
}

//--------------------------------------------------------------

void PrintParametryzator::ask2printSelected()
{
    if(!canStartPrintingSmart(PARAM_PRINT_MODE_SELECTED, false))
        return;

    const QStringList nis = printtv->getSelectedNIs();

     const int rowmax = nis.size();
     bool wasAgoodResult = false;
     for(int i = 1; i <= rowmax; i++){
         if(!ui->wdgtCurrentMeter->isEnabled())
             break;
         if(printAreport(false)){
             if(i < rowmax)
                 printtv->activateRowWithThisNI(nis.at(i));
             wasAgoodResult = true;

         }
     }
     if(wasAgoodResult)
         savePageSett();
emit onPrintingOperationFinished();

}

//--------------------------------------------------------------

void PrintParametryzator::ask2PreviewPrintSelected()
{
    if(!canStartPrintingSmart(PARAM_PRINT_MODE_SELECTED, true))
        return;

   const QStringList nis = printtv->getSelectedNIs();

    const int rowmax = nis.size();

    for(int i = 1; i <= rowmax; i++){
        if(!ui->wdgtCurrentMeter->isEnabled())
            break;
        if(printAreport(true)){
            if(i < rowmax)
                printtv->activateRowWithThisNI(nis.at(i));
             savePageSett();

        }
    }
    emit onPrintingOperationFinished();

}

//--------------------------------------------------------------

void PrintParametryzator::checkLeNiLater()
{
    //    if(ui->leNI->styleSheet().isEmpty())
    //        return;



    const QString arg1 = ui->leNI->text().simplified().trimmed();

    if(arg1.isEmpty()){
        ui->wdgtCurrentMeter->setEnabled(false);
        if(!ui->leNI->styleSheet().isEmpty())
            ui->leNI->setStyleSheet("");
        return;
    }

    const int duplicates = printtv->findNisInObjectDir(arg1);
    ui->leNI->setStyleSheet( (duplicates > 0) ? "background-color: rgb(255, 117, 119);" : "");//  );

    //    if(duplicates < 1){
    //        if(!printtv->lastsett.ni2meterprog.contains(arg1))
    //            ui->leNI->setStyleSheet("background-color: rgb(235, 255, 161);");//the table doesn't contain it
    //    }

    ui->wdgtCurrentMeter->setEnabled((duplicates < 1) || ui->cbxIgnoreDuplicates->isChecked());


    //check if NI is unique
}

//--------------------------------------------------------------

void PrintParametryzator::updateleConsumerAddressDest()
{
    ui->leConsumerAddressDest->setText(QString("<a href=\"%1/%2\">%1/%2").arg(ui->lblPath2pdf->text()).arg(ui->leConsumerAddress->text()));


    emit onConsumerAddressChanged(ui->leConsumerAddress->text(), ui->lblPath2pdf->text());
}

//--------------------------------------------------------------

void PrintParametryzator::setNewConsumerAddress(QString address)
{
    if(address.isEmpty())
        return;
    ui->leConsumerAddress->setText(address);
    QTimer::singleShot(11, this, SLOT(savePageSett()));
}

//--------------------------------------------------------------

void PrintParametryzator::on_pbGroupNext_clicked()
{
    savePageSett();
    ui->tabWidget->setCurrentIndex(1);
    ui->stackedWidget->setCurrentIndex(1);

}

//--------------------------------------------------------------

void PrintParametryzator::on_pbPrintGroup_clicked()
{
    if(!canStartPrintingSmart(PARAM_PRINT_MODE_VISIBLE, false))
        return;

    bool wasAgoodResult = false;
    const int rowmax = printtv->model->rowCount();
    for(int i = 0; i < rowmax; i++){
        if(!ui->wdgtCurrentMeter->isEnabled())
            break;
        if(printAreport(false))
            wasAgoodResult = true;
    }
    if(wasAgoodResult)
        savePageSett();

emit onPrintingOperationFinished();
}

//--------------------------------------------------------------

void PrintParametryzator::on_pbPreviewGroup_clicked()
{
    if(!canStartPrintingSmart(PARAM_PRINT_MODE_VISIBLE, true))
        return;


    const int rowmax = printtv->model->rowCount();
    for(int i = 0; i < rowmax; i++){
        if(!ui->wdgtCurrentMeter->isEnabled())
            break;
        if(printAreport(true))
            savePageSett();
    }


emit onPrintingOperationFinished();

}



//--------------------------------------------------------------

void PrintParametryzator::on_rbResetDate_clicked()
{
    ui->dteDocDate->setDate(QDate::currentDate());
}

//--------------------------------------------------------------

void PrintParametryzator::on_tbOpenProfilesEditor_clicked()
{
    emit openPrintProfileEditor(ui->cbxPrintProfile->currentText());
}

//--------------------------------------------------------------

void PrintParametryzator::on_pbGroupSkeletonPreview_clicked(bool checked)
{
    //    ui->swGroupSkeletonPreview->setCurrentIndex(int(checked));
    ui->wdgt4skeleton->setVisible(checked);
}

//--------------------------------------------------------------

void PrintParametryzator::on_pbCurrentPrint_clicked()
{
    if(!canStartPrintingSmart(PARAM_PRINT_MODE_CURRENTRW, false))
        return;

    if(printAreport(false))
        savePageSett();
emit onPrintingOperationFinished();
}

//--------------------------------------------------------------

void PrintParametryzator::on_pbCurrentPreview_clicked()
{
    if(!canStartPrintingSmart(PARAM_PRINT_MODE_CURRENTRW, true))
        return;
    if(printAreport(true))
        savePageSett();

emit onPrintingOperationFinished();
}



//--------------------------------------------------------------

void PrintParametryzator::on_pbCurrentEdit_clicked()
{
//it is blocked at this moment, I think I'll add it later or never
}

//--------------------------------------------------------------

void PrintParametryzator::on_cbxCurrentCoefitient_clicked(bool checked)
{
    ui->sbCurrentDenominator->setVisible(checked);
    ui->sbCurrentNumerator->setVisible(checked);
    ui->label_19->setVisible(checked);
}

//--------------------------------------------------------------

void PrintParametryzator::on_cbxVoltageCoefitient_clicked(bool checked)
{
    ui->sbVoltageDenominator->setVisible(checked);
    ui->sbVoltageNumerator->setVisible(checked);
    ui->label_20->setVisible(checked);
}

//--------------------------------------------------------------

void PrintParametryzator::reconfigureCompleter4addr()
{
    QDir dir ;
    dir.setPath(ui->lblPath2pdf->text());

    QStringList l = dir.entryList(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot);
    QCompleter *c = new QCompleter(l, this);
    c->setCaseSensitivity(Qt::CaseInsensitive);

    //    QFileSystemModel *m = new QFileSystemModel(c);
    //    m->setRootPath(ui->lePath2pdfDir->text());
    //    c->setModel(m);

    //    qDebug() << m->rootPath();
    ui->leConsumerAddress->setCompleter(c);


}

//--------------------------------------------------------------

void PrintParametryzator::on_lblPath2pdf_linkActivated(const QString &link)
{
    qDebug() << "link " << link;

}



void PrintParametryzator::on_tabWidget_tabBarClicked(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void PrintParametryzator::on_pbBack_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);

}

void PrintParametryzator::on_leConsumerAddressDest_linkActivated(const QString &link)
{
    QString openpath = link;
    QDir dir(link);
    if(!dir.exists()){
        dir.cdUp();
        if(dir.exists())
            openpath = dir.absolutePath();
    }

    qDebug() << "try to open link " << openpath << link;
#ifdef Q_OS_WIN
        QDesktopServices::openUrl(QUrl("file:///" + openpath, QUrl::TolerantMode));
#else
        QDesktopServices::openUrl(QUrl("file://" + openpath, QUrl::TolerantMode));
#endif
}
