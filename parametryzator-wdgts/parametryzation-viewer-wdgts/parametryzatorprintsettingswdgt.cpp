#include "parametryzatorprintsettingswdgt.h"
#include "ui_parametryzatorprintsettingswdgt.h"

#include <QFileDialog>
#include <QMessageBox>


///[!] parametryzator-v2
#include "parametryzator-wdgt-src/parametryzatorregisteredkeyswdgt.h"


///[!] widgets-shared
#include "template-pgs/lineeditdialog.h"


///[!] parametryzator-base
#include "parametryzator-src/parametryzatorprinthelper.h"


///[!] printer-widgets
#include "src/printer/printerhelper.h"


///[!] widgets-shared
#include "gui-src/printinghelper.h"

//-------------------------------------------------------------------------------------------

ParametryzatorPrintSettingsWdgt::ParametryzatorPrintSettingsWdgt(GuiHelper *gHelper, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametryzatorPrintSettingsWdgt)
{
    ui->setupUi(this);
    htmlEditorStandardPte = new SmplPteWdgt("", false, false, gHelper, false, this);
    connect(htmlEditorStandardPte->getPte(), SIGNAL(textChanged()), this, SIGNAL(restartTmrUpdateHtmlPreview()));

    connect(htmlEditorStandardPte, &SmplPteWdgt::pageEndInit, this, &ParametryzatorPrintSettingsWdgt::createWidgets);

}

//-------------------------------------------------------------------------------------------

ParametryzatorPrintSettingsWdgt::~ParametryzatorPrintSettingsWdgt()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------

QVariantHash ParametryzatorPrintSettingsWdgt::getDefaultPrintMainSett()
{
    QVariantHash h ;//= lastsett.lastPageSettMain;

    h.insert("pteAddrs", QString("Україна, м. Київ\n"
                                 "вул. Бориспільська, 7\n"
                                 "тел. +38-068-358-9149\n"
                                 "info@kts-intek.com\n"
                                 "http://kts-intek.com.ua\n"
//                                 "https://kts-intek.com\n"
//                                 "https://www.facebook.com/ktsintek"
                                 ));

    h.insert("lePath2pdfDir", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) );
    h.insert("lePdfFileName", QString("$leConsumerAddressSmpl/$meterSN_сад_тов_$meterNI"));
    h.insert("leCutAddrs2", QString("\\([0-9]+\\)"));
    h.insert("pteMeterProg2meterType", QString("MTX1A MTX 1A10\n"
                                               "MTX3R MTX 3R30\n"
//                                               "CE303 CE303\n"
//                                               "CE301 CE301\n"
//                                               "CE301 CE303-U(A)\n"
//                                               "CE102 CE102\n"
                                               "102 CE102-U\n"
                                               "301v12 CE303 UA\n"
                                               "G1B GAMA100\n"
                                               "G3B GAMA300"));

    //pteMeterModel2VoltageManufCity
    h.insert("pteMeterModel2VoltageManufCity", QString("MTX1A 1x220 5(60) Телетек м.Одеса\n"
                                                       "MTX3R 3x220 5(100) Телетек м.Одеса\n"
//                                                       "CE303 3x230 5(100) Енергоміра м.Харків\n"
//                                                       "CE301 3x230 5(100) Енергоміра м.Харків\n"
                                                       "102 1x230 5(60) Енергоміра м.Харків\n"
                                                       "301v12 3x230 5(100) Енергоміра м.Харків\n"
                                                       "G1B 1x230 5(80) ELGAMA-ELEKTRONIKA м.Вільнюс\n"
                                                       "G3B 3x230 5(100) ELGAMA-ELEKTRONIKA м.Вільнюс"));

    h.insert("leFindNIRegExp", QString("_$lastMeterNI[_.]"));//this is much better _$lastMeterNI_|$lastMeterNI\\."));
    h.insert("leReplaceNIrule4fileFind", QString(":"));
    h.insert("leReplaceNIrule4fileReplace", QString(";"));
    return h;
}

//-------------------------------------------------------------------------------------------

QPlainTextEdit *ParametryzatorPrintSettingsWdgt::getActivePteInStack()
{
    SmplPteWdgt *w = qobject_cast<SmplPteWdgt*>(ui->sw4htmlEditor->currentWidget());
    if(w){
        return w->getPte();
    }
    return 0;

}
//-------------------------------------------------------------------------------------------
QString ParametryzatorPrintSettingsWdgt::getHtmlTextFromActivePteInStack(const bool &replaceKeys)
{
    QPlainTextEdit *pte = getActivePteInStack();
    if(pte)
        return replaceKeys ? replaceKeysWithValue(pte->toPlainText()) : pte->toPlainText();
    return  tr("Error. The object is not found").toHtmlEscaped();
}

QString ParametryzatorPrintSettingsWdgt::replaceKeysWithValue(const QString &inputString)
{
    return ParametryzatorPrintHelper::replaceKeysWithFakeValue(lastsett.lastPageSettMain, inputString);
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::setPageSettpPrintingMain(QVariantHash lastPageSett)
{
    lastsett.lastPageSettMain = lastPageSett;

    applyPageSettpPrintingMain(lastPageSett);
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::applyPageSettpPrintingMain(QVariantHash lastPageSett)
{

    ui->pteAddrs->setPlainText(lastPageSett.value("pteAddrs").toString());

    ui->lePath2pdfDir->setText(lastPageSett.value("lePath2pdfDir").toString());

    ui->lePdfFileName->setText(lastPageSett.value("lePdfFileName").toString());


    ui->leCutAddrs2->setText(lastPageSett.value("leCutAddrs2").toString());

    ui->pteMeterProg2meterType->setPlainText(lastPageSett.value("pteMeterProg2meterType").toString());

    ui->pteMeterModel2VoltageManufCity->setPlainText(lastPageSett.value("pteMeterModel2VoltageManufCity").toString());


    ui->leFindNIRegExp->setText(lastPageSett.value("leFindNIRegExp").toString());


    ui->leReplaceNIrule4fileFind->setText(lastPageSett.value("leReplaceNIrule4fileFind").toString());
    ui->leReplaceNIrule4fileReplace->setText(lastPageSett.value("leReplaceNIrule4fileReplace").toString());

}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::setPageSettpPrintingHtml(QVariantHash lastPageSett)
{
    lastsett.lastPageSettHtml = lastPageSett;

    htmlEditorStandardPte->clearPage();
    ui->cbxPrintProfiles->clear();
    ui->cbxPrintProfiles->addItem(tr("Standard"));


    htmlEditorStandardPte->setPteText(lastPageSett.value("standard").toString());
    if(htmlEditorStandardPte->getPte()->toPlainText().isEmpty())
        loadDefaultHtml4thisEditor(htmlEditorStandardPte->getPte());

    for(int i = ui->sw4htmlEditor->count() - 1; i > 0; i--){
        QWidget *w = ui->sw4htmlEditor->widget(i);
        ui->sw4htmlEditor->removeWidget(w);
        w->deleteLater();

    }

    ui->cbxPrintProfiles->setCurrentIndex(0);
    ui->sw4htmlEditor->setCurrentIndex(0);
    qDebug() << "sw4htmlEditor " << ui->sw4htmlEditor->count();

    QVariantHash h = lastPageSett.value("userprofiles").toHash();
    if(!h.isEmpty()){
        QList<QString> lk = h.keys();
        std::sort(lk.begin(), lk.end());
        for(int i = 0, imax = lk.size(); i < imax; i++){

            const QString profilename = lk.at(i);
            const QString profiletext = h.value(profilename).toString();

            createUserHtmlPte(profilename, profiletext);




        }
    }


    ui->sbFontSizeHtml->setValue(lastPageSett.value("fonts", htmlEditorStandardPte->getPte()->font().pointSize()).toInt());
    QFont font = ui->txtBrwsr4prew->font();
    font.setPointSize(ui->sbFontSizeHtml->value());
    ui->txtBrwsr4prew->setFont( font);

    emit restartTmrUpdateHtmlPreview();
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::clearPage()
{

    ui->listWidget->setCurrentRow(0);
    on_listWidget_clicked(ui->listWidget->currentIndex());

    loadDefaultHtml4thisEditor(htmlEditorStandardPte->getPte());

    emit gimmeMyLastSettingsPrintMain();
    emit restartTmrUpdateHtmlPreview();
    emit gimmeMyLastSettingsPrintHtmlProfiles();


     on_cbxPrintProfiles_currentIndexChanged(0);

    if(!ui->tbShowTxtBrwsrPrev->isChecked())
        ui->tbShowTxtBrwsrPrev->animateClick();

    if(!ui->tbReplaceKeysInTxtBrwsr->isChecked())
        ui->tbReplaceKeysInTxtBrwsr->animateClick();
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::createWidgets()
{
    connect(ui->pbLoadSaved, SIGNAL(clicked(bool)), this, SIGNAL(gimmeMyLastSettingsPrintMain()) );
    connect(ui->pbLoadHtml, SIGNAL(clicked(bool)), this, SIGNAL(gimmeMyLastSettingsPrintHtmlProfiles()));

    connect(ui->tbReplaceKeysInTxtBrwsr, SIGNAL(clicked(bool)), this, SIGNAL(restartTmrUpdateHtmlPreview()) );
    connect(ui->cbxPrintProfiles, SIGNAL(currentIndexChanged(int)), this, SIGNAL(restartTmrUpdateHtmlPreview()));


    ui->sw4htmlEditor->addWidget(htmlEditorStandardPte);

    QTimer *tmr = new QTimer(this);
    tmr->setInterval(333);
    tmr->setSingleShot(true);
    connect(this, SIGNAL(restartTmrUpdateHtmlPreview()), tmr, SLOT(start()));
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateHtmlPreview()));

    clearPage();

    connect(ui->pbClose, SIGNAL(clicked(bool)), this, SLOT(deleteLater()));


    ParametryzatorRegisteredKeysWdgt *regkeys = new ParametryzatorRegisteredKeysWdgt(htmlEditorStandardPte->gHelper, this);
    ui->swSettingsPages->addWidget(regkeys);



}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::updateHtmlPreview()
{
    ui->txtBrwsr4prew->setHtml(getHtmlTextFromActivePteInStack(ui->tbReplaceKeysInTxtBrwsr->isChecked()));

}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::addThisProfile(QString name)
{
    ui->sw4htmlEditor->setCurrentWidget(createUserHtmlPte(name, QString()));
    ui->cbxPrintProfiles->setCurrentIndex(ui->cbxPrintProfiles->count() - 1);
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_tbPath2pdfDir_clicked()
{
    QString path2dir = QFileDialog::getExistingDirectory(this, tr("Select PDF Directory"), ui->lePath2pdfDir->text(),
                                                         QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(!path2dir.isEmpty())
        ui->lePath2pdfDir->setText(path2dir);
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_pbSaveAsDef_clicked()
{
    QVariantHash h = lastsett.lastPageSettMain;

    h.insert("pteAddrs", ui->pteAddrs->toPlainText());

    h.insert("lePath2pdfDir", ui->lePath2pdfDir->text() );
    h.insert("lePdfFileName", ui->lePdfFileName->text());
    h.insert("leCutAddrs2", ui->leCutAddrs2->text());
    h.insert("pteMeterProg2meterType", ui->pteMeterProg2meterType->toPlainText());
    h.insert("pteMeterModel2VoltageManufCity", ui->pteMeterModel2VoltageManufCity->toPlainText());
    h.insert("leFindNIRegExp", ui->leFindNIRegExp->text());
    h.insert("leReplaceNIrule4fileFind", ui->leReplaceNIrule4fileFind->text());
    h.insert("leReplaceNIrule4fileReplace", ui->leReplaceNIrule4fileReplace->text());
    lastsett.lastPageSettMain = h;

    emit setLastPageSettPrintMain(h);
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_listWidget_clicked(const QModelIndex &index)
{
    ui->swSettingsPages->setCurrentIndex(index.row());
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_tbAddProfile4print_clicked()
{
    LineEditDialog *d = new LineEditDialog("", tr("New profile"), tr("Profile name"), QStringList(), false, this);
    connect(d, &LineEditDialog::onAcceptedText, this, &ParametryzatorPrintSettingsWdgt::addThisProfile );
    d->exec();
    d->deleteLater();
}

//-------------------------------------------------------------------------------------------

SmplPteWdgt * ParametryzatorPrintSettingsWdgt::createUserHtmlPte(const QString &profilename, const QString &profiletext)
{
    SmplPteWdgt *pte = new SmplPteWdgt("", false, false, htmlEditorStandardPte->gHelper, false, this);

//    connect(this, &ParametryzatorPrintSettingsWdgt::gimmeMyLastSettingsPrintHtmlProfiles, pte, &SmplPteWdgt::deleteLater);//all exept the standard musb be deleted

    pte->setAccessibleName(QString(profilename.toLocal8Bit().toBase64()));
    pte->setPteText(profiletext);

    connect(pte->getPte(), SIGNAL(textChanged()), this, SIGNAL(restartTmrUpdateHtmlPreview()));



    ui->sw4htmlEditor->addWidget(pte);
    ui->cbxPrintProfiles->addItem(profilename);

    qDebug() << "ui->sw4htmlEditor->addWidget " << ui->sw4htmlEditor->count() << ui->cbxPrintProfiles->count();

    return pte;
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::loadDefaultHtml4thisEditor(QPlainTextEdit *pte)
{
    if(pte){
        QFile file(":/def/default.html");
        file.open(QFile::ReadOnly);
        pte->setPlainText(file.readAll());
        file.close();
    }
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_tbRemoveProfile4print_clicked()
{
    if(QMessageBox::question(this, tr("Delete"), tr("Do you really want to delte<br>'%1'?").arg(ui->cbxPrintProfiles->currentText())) == QMessageBox::Yes){
        if(ui->sw4htmlEditor->count() > 0){
            qDebug() << "sw current wdgt " << QByteArray::fromBase64(ui->sw4htmlEditor->currentWidget()->accessibleName().toUtf8()) << ui->cbxPrintProfiles->currentText();
            QWidget *w = ui->sw4htmlEditor->currentWidget();
            ui->sw4htmlEditor->removeWidget(w);
            w->deleteLater();

        }
        if(ui->cbxPrintProfiles->count() > 0)
            ui->cbxPrintProfiles->removeItem(ui->cbxPrintProfiles->currentIndex());
        ui->cbxPrintProfiles->setCurrentIndex(0);
    }
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_tbShowTxtBrwsrPrev_clicked(bool checked)
{
    ui->scrollArea_4->setVisible(checked);
    if(checked)
        updateHtmlPreview();
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_pbDefault_clicked()
{
    applyPageSettpPrintingMain(getDefaultPrintMainSett());
}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_cbxPrintProfiles_currentIndexChanged(int index)
{
    ui->tbRemoveProfile4print->setEnabled(index > 0);
    if(index >= 0)
        ui->sw4htmlEditor->setCurrentIndex(index);
//    ui->pbLoadDefHtml->setEnabled(index > 0);

}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_pbLoadDefHtml_clicked()
{
    loadDefaultHtml4thisEditor(getActivePteInStack());

}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_pbSaveHtml_clicked()
{
    QVariantHash settings;
    settings.insert("standard", htmlEditorStandardPte->getPte()->toPlainText());
    QVariantHash userprofiles;// = lastPageSett.value("userprofiles").toHash();

    for(int i = 1, imax = ui->sw4htmlEditor->count(); i < imax; i++){
        SmplPteWdgt *w = qobject_cast<SmplPteWdgt*>(ui->sw4htmlEditor->widget(i));
        if(w){
            const QString profielname = QByteArray::fromBase64(w->accessibleName().toUtf8());
            const QString htmltext = w->getPte()->toPlainText();

            qDebug() << "save html " << profielname << QByteArray::fromBase64(w->accessibleName().toUtf8());

            userprofiles.insert(profielname, htmltext);
        }
        //    pte->setAccessibleName(QString(profilename.toLocal8Bit().toBase64()));

    }
    settings.insert("userprofiles", userprofiles);
    settings.insert("fonts", ui->sbFontSizeHtml->size());

    emit setLastPageSettPrintHtml(settings);

}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_tbPrintPreview_clicked()
{
    const Print2file::Import2fileSett importSett = PrintingHelper::getImport2fileSett("-", QStringList(), QStringList());//just load saved settings


        //    static bool print2dev(QString &mess, QWidget *parent, const QString &html, const QFont &font, const Import2fileSett &sett);
    QString message;
    PrintingHelper::print2dev(message, this, ui->txtBrwsr4prew->toHtml(), ui->txtBrwsr4prew->font(), importSett);




}

//-------------------------------------------------------------------------------------------

void ParametryzatorPrintSettingsWdgt::on_tbPrintPdf_clicked()
{
    QString lastpdffilename;
    if(lastpdffilename.isEmpty())
        lastpdffilename = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save pdf as..."), QString("%1/%2.pdf").arg(lastpdffilename).arg("html_preview"));
    if(fileName.isEmpty())
        return ;

    QPrinter printer(QPrinter::HighResolution);
    const PrintingHelper::Import2fileSett importSett = PrintingHelper::getImport2fileSett(fileName, QStringList(), QStringList());
    PrintingHelper::setupPrinterDevice(printer, importSett);
    printer.setOutputFileName(importSett.fileName);

    ui->txtBrwsr4prew->print(&printer);

}

//-------------------------------------------------------------------------------------------
