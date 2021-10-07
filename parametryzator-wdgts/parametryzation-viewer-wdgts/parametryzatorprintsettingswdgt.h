#ifndef PARAMETRYZATORPRINTSETTINGSWDGT_H
#define PARAMETRYZATORPRINTSETTINGSWDGT_H

#include <QWidget>


///[!] widgets-shared
#include "template-pgs/smplptewdgt.h"


namespace Ui {
class ParametryzatorPrintSettingsWdgt;
}

class ParametryzatorPrintSettingsWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit ParametryzatorPrintSettingsWdgt(GuiHelper *gHelper, QWidget *parent = nullptr);
    ~ParametryzatorPrintSettingsWdgt();

    static QVariantHash getDefaultPrintMainSett() ;



    QPlainTextEdit *getActivePteInStack();

    QString getHtmlTextFromActivePteInStack(const bool &replaceKeys);
    QString replaceKeysWithValue(const QString &inputString);


    struct LastPringSett
    {
        QVariantHash lastPageSettMain;
        QVariantHash lastPageSettHtml;

        LastPringSett() {}
    } lastsett;

signals:
    void gimmeMyLastSettingsPrintMain();//get saved settings to the first page only

    void gimmeMyLastSettingsPrintHtmlProfiles();

    //to mainwindow
    void setLastPageSettPrintMain(QVariantHash pagesett); //save settings

    void setLastPageSettPrintHtml(QVariantHash pagesett); //save settings

    void restartTmrUpdateHtmlPreview();

public slots:
    void setPageSettpPrintingMain(QVariantHash lastPageSett);


    void applyPageSettpPrintingMain(QVariantHash lastPageSett);


    void setPageSettpPrintingHtml(QVariantHash lastPageSett);


    void clearPage();

    void createWidgets();


    void updateHtmlPreview();

    void addThisProfile(QString name);


private slots:

    void on_tbPath2pdfDir_clicked();

    void on_pbSaveAsDef_clicked();

    void on_listWidget_clicked(const QModelIndex &index);


    void on_tbAddProfile4print_clicked();




    void on_tbRemoveProfile4print_clicked();

    void on_tbShowTxtBrwsrPrev_clicked(bool checked);


    void on_pbDefault_clicked();

    void on_cbxPrintProfiles_currentIndexChanged(int index);

    void on_pbLoadDefHtml_clicked();

    void on_pbSaveHtml_clicked();

    void on_tbPrintPreview_clicked();

    void on_tbPrintPdf_clicked();

private:
    Ui::ParametryzatorPrintSettingsWdgt *ui;

    SmplPteWdgt *createUserHtmlPte(const QString &profilename, const QString &profiletext);

    void loadDefaultHtml4thisEditor(QPlainTextEdit *pte );

    SmplPteWdgt *htmlEditorStandardPte;
};

#endif // PARAMETRYZATORPRINTSETTINGSWDGT_H
