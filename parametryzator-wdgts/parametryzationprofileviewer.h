#ifndef PARAMETRYZATIONPROFILEVIEWER_H
#define PARAMETRYZATIONPROFILEVIEWER_H

#include <QWidget>


#include "parametryzation-viewer-wdgts/displaysettviewer.h"
#include "parametryzation-viewer-wdgts/dstsettviewer.h"
#include "parametryzation-viewer-wdgts/relaylimits.h"
#include "parametryzation-viewer-wdgts/relaysettviewer.h"
#include "parametryzation-viewer-wdgts/relaytestsettviewer.h"
#include "parametryzation-viewer-wdgts/tariffsettviewer.h"


///[!] widgets-shared
#include "gui-src/mysortfilterproxymodel.h"

namespace Ui {
class ParametryzationProfileViewer;
}

class ParametryzationProfileViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ParametryzationProfileViewer(const bool &editable, GuiHelper *gHelper, QWidget *parent = nullptr);
    ~ParametryzationProfileViewer();


    GuiHelper *gHelper;
    bool isEditable;

    QVariantHash lastSettHash;

    QVariantHash getPageSett(QString &profname);

signals:
    void setThisSett(QString lastprofiletext, QVariantHash paramsett);

    void onWidgetsReady();



public slots:
    void sendMeYourSett();

    void setThisPagesSett(QString profilename, QVariantHash setthash);

    void resetAll2defSett();

    void createWidgets();

    void addWidget(QWidget *w, QString title, bool enable);

    void updateCurrentRowWdgt();

    void resetProfileName();

private slots:
    void on_lvOptions_clicked(const QModelIndex &index);



private:
    Ui::ParametryzationProfileViewer *ui;

    DisplaySettViewer *dispw;
    DSTSettViewer *dstw;
    RelayLimits *rellimw;
    RelaySettViewer *relsettw;
    RelayTestSettViewer *reltestw;
    TariffSettViewer *tariffw;

    QStandardItemModel *model;
    MySortFilterProxyModel *proxymodel;

    QString lastactiveitemtext;

};

#endif // PARAMETRYZATIONPROFILEVIEWER_H
