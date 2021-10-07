#ifndef PARAMNISEARCHER_H
#define PARAMNISEARCHER_H

///[!] widgets-shared
#include "gui-src/wdgt/referencewidgetclassgui.h"


///[!] parametryzator-base
#include "parametryzator-src/parametryzatorprinthelper.h"


class ParamNISearcher : public ReferenceWidgetClassGui
{
    Q_OBJECT
public:
    explicit ParamNISearcher(GuiHelper *gHelper, QWidget *parent = nullptr);


    struct PageLastSett
    {
        QVariantHash grpsett;
        QVariantHash mainsett;

        MPrintTableOut printout;

        QStringList lastnis;

        QString lastworkingdir;

        bool lockPrinting;
        PageLastSett() : lockPrinting(false) {}
    } lastsett;


    QStringList getHeader();

//    QList<int> getDuplicates(QStringList &nis, const int &mode);
    QDir getObjectDirectory(const QString &objectname);

signals:
    void refreshTableExtraDataLater(int msec );

    void kickOffAllObjects();

    void startSearcher(QStringList nis, QString workingdirectory, QVariantHash mainsett);

    void stopSearch();


public slots:
    void clearPage();

    void initPage();
    void refreshTableExtraDataLaterDefMsec( );



    void setPageSettpPrintingGrp(QVariantHash sett);
    void setPageSettpPrintingMain(QVariantHash lastPageSett);

    void setTheseNis(QStringList nis);
    void addThisNI(qint64 msecsep, QString sn, QString ni, QString dirname, QString fullpath2dir);



//    void startPrintingTariffReports(MPrintTableOut printout, QVector<QVariantHash> printdata);



    void refreshTableExtraData();


    void onLastTvClicked(const QModelIndex &index);

    void onLastTvCustomContextMenuRequestedExt(const QPoint &pos);

};



#endif // PARAMNISEARCHER_H
