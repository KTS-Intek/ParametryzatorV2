#ifndef PRINTPARAMETRYZATOR_H
#define PRINTPARAMETRYZATOR_H

#include <QWidget>
#include <QDate>

#include "parametryzation-viewer-wdgts/printtvpreview.h"

namespace Ui {
class PrintParametryzator;
}

class PrintParametryzator : public QWidget
{
    Q_OBJECT

public:
    explicit PrintParametryzator(GuiHelper *gHelper, QWidget *parent = nullptr);
    ~PrintParametryzator();


    struct PrintParmPageSett
    {
        QVariantHash lastMainPrintSett;
        QVariantHash lastGrpSett;

        QVariantHash lastHtmlUserProfiles;
        QString lastHtmlDefaultProfile;

        //it contains data about last meter from printtv
        ParametryzatorPrintInputMeterInfo meterinfo;
        ParametryzatorPrintInputMeterInfoHuman meterhuman;
        QVariantHash lastmeterdata;


        MeterProg2MeterHumanData meterprot2sett;

        QString lastpdffilename;
        QString lastbasepdffilename;

        PrintParmPageSett() {}
    } lastpagesett;

    PrintTvPreview *printtv;
    QMap<QString, int> tariffline2cbxindex;

    QVariantHash getPageSett();


    void setCurrentCbx(QComboBox *cbx, const QString &currtxt);

    bool canStartPrintingSmart(const int &mode, const bool &isPreview);

    bool canStartPrinting(const int &mode, const bool &isPreview, QStringList &messages);

    bool printAreport(const bool &usepreview);

    bool print2pdf(QStringList &messages);
    bool print2pdfFile(QStringList &messages, const QString &path2pdf, const bool &openthen);

    bool save2pdf(QStringList &messages, const QString &pdfFileName, const bool &openthen);

signals:
    void openPrintProfileEditor(QString currentprofiletxt);

//to printtv
    void setPrintingSettings(QVariantHash sett);//after settings are reloaded or changed on the page

    //to mainwindow
    void setLastPageSett(QVariantHash pagesett); //save settings

    void gimmeMyLastSettings();//get saved settings here and to printtv

//only html
    void gimmeMyLastSettingsPrintHtmlProfiles();

    void startPrintingTariffReports(MPrintTableOut printout, QVector<QVariantHash> printdata);


    void updateHtmlPreviewLater();


    void openPrintSettings();


    void onReportReady(QString ni);


    void onPrintingOperationFinished();


    void onConsumerAddressChanged(QString address, QString workingdirectory);

public slots:
    void clearThePage();

    void setPageSettpPrintingGrp(QVariantHash lastPageSett);
    void setPageSettpPrintingMain(QVariantHash lastPageSett);

    void setPageSettpPrintingHtml(QVariantHash lastPageSett);

    void createWidgets();

    void savePageSett();


    void onGroupSettingsChanged();

    void updateHtmlPreview();


    void udpateInfoAboutCurrentRow(int duplicates, QString model, QString sn, QString ni, QString voltage, QString current, QString tariffp, QVariantHash meterdata, QString manuf);

    void ask2printSelected();
    void ask2PreviewPrintSelected();


    void checkLeNiLater();

    void updateleConsumerAddressDest();

    void setNewConsumerAddress(QString address);

private slots:
    void on_pbGroupNext_clicked();

    void on_pbPrintGroup_clicked();

    void on_pbPreviewGroup_clicked();


    void on_rbResetDate_clicked();

    void on_tbOpenProfilesEditor_clicked();

    void on_pbGroupSkeletonPreview_clicked(bool checked);

    void on_pbCurrentPrint_clicked();

    void on_pbCurrentPreview_clicked();


    void on_pbCurrentEdit_clicked();

    void on_cbxCurrentCoefitient_clicked(bool checked);

    void on_cbxVoltageCoefitient_clicked(bool checked);



    void reconfigureCompleter4addr();

    void on_lblPath2pdf_linkActivated(const QString &link);


    void on_tabWidget_tabBarClicked(int index);

    void on_pbBack_clicked();

    void on_leConsumerAddressDest_linkActivated(const QString &link);

private:
    Ui::PrintParametryzator *ui;
};

#endif // PRINTPARAMETRYZATOR_H
