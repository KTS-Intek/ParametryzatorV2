#ifndef PRINTTVPREVIEW_H
#define PRINTTVPREVIEW_H

///[!] widgets-shared
#include "gui-src/wdgt/referencewidgetclassgui.h"


///[!] parametryzator-base
#include "parametryzator-src/parametryzatorprinthelper.h"


#define PARAM_PRINT_MODE_VISIBLE    2
#define PARAM_PRINT_MODE_SELECTED   1
#define PARAM_PRINT_MODE_CURRENTRW  0

class PrintTvPreview : public ReferenceWidgetClassGui
{
    Q_OBJECT
public:
    explicit PrintTvPreview(GuiHelper *gHelper, QWidget *parent = nullptr);



    int findNisInObjectDir(const QString &ni);

    QStringList getSelectedNIs();

    struct PageLastSett
    {
        QVariantHash grpsett;
        QVariantHash mainsett;

        MPrintTableOut printout;

        QVector<QVariantHash> printdata;

        QHash<QString,QString> ni2meterprog;


        bool lockPrinting;
        PageLastSett() : lockPrinting(false) {}
    } lastsett;

    int getNICol();

    QStringList getHeader();

    QList<int> getDuplicates(QStringList &nis, const int &mode);

signals:
    void hasSelectedChanged(bool hasselected);


    void refreshTableExtraDataLater();


    void udpateInfoAboutCurrentRow(int duplicates, QString model, QString sn, QString ni, QString voltage, QString current, QString tariffp, QVariantHash meterdata, QString manuf);


    void ask2printSelected();
    void ask2PreviewPrintSelected();


public slots:
    void clearPage();

    void initPage();


    void onReportReady(QString ni);

    void activateRowWithThisNI(QString ni);

    void setPageSettpPrintingGrp(QVariantHash sett);
    void setPageSettpPrintingMain(QVariantHash lastPageSett);


    void startPrintingTariffReports(MPrintTableOut printout, QVector<QVariantHash> printdata);

    void updateTable();

    void refreshTableExtraData();

    void updateInfoAboutCurrentRow();

    void updateInfoAboutRow(const int &srcrow);

    void onLastTvClicked(const QModelIndex &index);

    void onLastTvCustomContextMenuRequestedExt(const QPoint &pos);

};

#endif // PRINTTVPREVIEW_H
