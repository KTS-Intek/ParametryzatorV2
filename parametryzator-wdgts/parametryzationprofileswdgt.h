#ifndef PARAMETRYZATIONPROFILESWDGT_H
#define PARAMETRYZATIONPROFILESWDGT_H

#include "gui-src/referencewidgetclass.h"

#include "parametryzationprofileviewer.h"
#include "parametryzationprofileeditor.h"


namespace Ui {
class ParametryzationProfilesWdgt;
}

class ParametryzationProfilesWdgt : public ReferenceWidgetClass
{
    Q_OBJECT

public:
    explicit ParametryzationProfilesWdgt(const bool &editable, GuiHelper *gHelper, QWidget *parent = nullptr);
    ~ParametryzationProfilesWdgt();

    QStackedWidget *topStackedWidget();

    bool isEditableMode();

signals:

    //to paramMedium
    void onReloadParametryzatorProfiles();

    void onParametryzatorProfileTableContentChanged(QString profname, QVariantHash profile);

    void onDeleteParametryzatorProfileTable(QStringList deleteThese);



    //to params viewer
    void showThisProfile(QString profname, QVariantHash profile);

    void sendMeYourLastSett();
//from params viewer
    void lastSettFromViewer(QString profname, QVariantHash profile);


//editor
    void startEditThisProfile(QString profname, QVariantHash profile);

    void hideEditor();


public slots:

    //from paramMedium
    void setPageSettParametryzatorProfile(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader);



    void addThisProfile(QString profname, QVariantHash profile);




private slots:

    void initPage();



    void on_tvTable_clicked(const QModelIndex &index);

    void on_tvTable_customContextMenuRequested(const QPoint &pos);


    void onSettingsChangedSlot();

    void on_tvTable_doubleClicked(const QModelIndex &index);

private:
    Ui::ParametryzationProfilesWdgt *ui;

    void createEditor();


    QStringList listLastProfNames;



};

#endif // PARAMETRYZATIONPROFILESWDGT_H
