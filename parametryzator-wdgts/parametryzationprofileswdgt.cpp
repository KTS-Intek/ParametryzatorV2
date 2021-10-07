#include "parametryzationprofileswdgt.h"
#include "ui_parametryzationprofileswdgt.h"


///[!] guisett-shared-core
#include "src/nongui/settloader.h"
#include "src/nongui/showmessagehelpercore.h"



//---------------------------------------------------------------------------

ParametryzationProfilesWdgt::ParametryzationProfilesWdgt(const bool &editable, GuiHelper *gHelper, QWidget *parent) :
    ReferenceWidgetClass(gHelper, parent),
    ui(new Ui::ParametryzationProfilesWdgt)
{
    ui->setupUi(this);
    ui->pbAdd->setEnabled(false);
    if(!editable)        
        ui->stackedWidget->hide();
    ui->tvTable->setEnabled(false);
}

//---------------------------------------------------------------------------

ParametryzationProfilesWdgt::~ParametryzationProfilesWdgt()
{
    delete ui;
}

//---------------------------------------------------------------------------

QStackedWidget *ParametryzationProfilesWdgt::topStackedWidget()
{
    return ui->stackedWidget;
}

//---------------------------------------------------------------------------

bool ParametryzationProfilesWdgt::isEditableMode()
{
    return (ui->stackedWidget->isVisible() && ui->stackedWidget->currentIndex() == 0);
}

//---------------------------------------------------------------------------

void ParametryzationProfilesWdgt::setPageSettParametryzatorProfile(MyListStringList listRows, QVariantMap col2data, QStringList headerH, QStringList header, bool hasHeader)
{
    if(!ui->tvTable->isEnabled())
        return;//do not add data before all items are created

    const QString currprofile = TableViewHelper::getCellValueOfcurrentRow(ui->tvTable, 0);

    StandardItemModelHelper::append2model(listRows, col2data, headerH, header, hasHeader, model);

    if(currprofile.isEmpty()){
        if(proxy_model->rowCount() > 0){
            lastTv->setCurrentIndex(proxy_model->index(0,0));
            on_tvTable_clicked(proxy_model->index(0,0));
        }
    }else
        TableViewHelper::selectRowWithThisCell(ui->tvTable, currprofile, 0);


    setHasDataFromRemoteDeviceAndResizeLastTv();
}


//---------------------------------------------------------------------------

void ParametryzationProfilesWdgt::addThisProfile(QString profname, QVariantHash profile)
{

    QStringList messages;
    if(profname.isEmpty())
        messages.append(tr("Bad profile name"));

    if(profile.isEmpty())
        messages.append(tr("There is no any useful data in the profile"));

    if(!messages.isEmpty()){


        emit hideEditor();
        gHelper->showMessageSlot(messages.join("<br>"));
        return;
    }

    emit onParametryzatorProfileTableContentChanged(profname, profile);

}


//---------------------------------------------------------------------------



void ParametryzationProfilesWdgt::initPage()
{
    setupObjects(ui->horizontalLayout_2, ui->tvTable, nullptr, nullptr, ui->leFilter, SETT_FILTERS_FREE_4);
    connect(this, &ParametryzationProfilesWdgt::openContextMenu, this, &ParametryzationProfilesWdgt::on_tvTable_customContextMenuRequested);
    StandardItemModelHelper::setModelHorizontalHeaderItems(model, QStringList());


    ParametryzationProfileViewer *viewer = new ParametryzationProfileViewer(false, gHelper, this);
    connect(this, &ParametryzationProfilesWdgt::showThisProfile, viewer, &ParametryzationProfileViewer::setThisPagesSett);
    connect(this, &ParametryzationProfilesWdgt::sendMeYourLastSett, viewer, &ParametryzationProfileViewer::sendMeYourSett);

    connect(viewer, &ParametryzationProfileViewer::setThisSett, this, &ParametryzationProfilesWdgt::lastSettFromViewer);

    ui->scrollArea->setWidget(viewer);


    if(isEditableMode()){
        ui->pbAdd->setEnabled(true);

        QTimer *t = new QTimer(this);
        t->setInterval(333);
        t->setSingleShot(true);
        connect(this, SIGNAL(onSettingsChanged()), t, SLOT(start()));
        connect(t, SIGNAL(timeout()), this, SLOT(onSettingsChangedSlot()));

        createEditor();
    }

    ui->tvTable->setEnabled(true);

    QTimer::singleShot(111, this, SIGNAL(onReloadParametryzatorProfiles()));

}

//---------------------------------------------------------------------------

void ParametryzationProfilesWdgt::on_tvTable_clicked(const QModelIndex &index)
{
    const int row = proxy_model->mapToSource(index).row();
    if(row < 0)
        return;

    emit showThisProfile(model->item(row,0)->text(), model->item(row,0)->data().toHash());

}

//---------------------------------------------------------------------------

void ParametryzationProfilesWdgt::on_tvTable_customContextMenuRequested(const QPoint &pos)
{
    listLastProfNames = StandardItemModelHelper::getColText(0, -1, model);
    connect(gHelper, SIGNAL(onModelChanged(int)), this, SLOT(onSettingsChangedSlot()));
//    gHelper->createCustomMenu(pos, ui->tvTable, isEditableMode() ?
//                                   (GuiHelper::ShowReset|GuiHelper::ShowExport|GuiHelper::ShowCopy|GuiHelper::DeleteSelected)
//                                :  (GuiHelper::ShowReset|GuiHelper::ShowExport|GuiHelper::ShowCopy),
//                              CLBRD_SMPL_PRXTBL, ShowMessHelperCore::matildaFileName(windowTitle()));
    disconnect(gHelper, SIGNAL(onModelChanged(int)), this, SLOT(onSettingsChangedSlot()));


}

//---------------------------------------------------------------------------

void ParametryzationProfilesWdgt::onSettingsChangedSlot()
{
    if(!isEditableMode() || !ui->tvTable->isEnabled())
        return;

    const QStringList listCurrProfNames = StandardItemModelHelper::getColText(0, -1, model);

    QStringList l2del;
    for(int i = 0, imax = listLastProfNames.size(); i < imax; i++){
        if(!listCurrProfNames.contains(listLastProfNames.at(i)))
            l2del.append(listLastProfNames.at(i));
    }
  emit hideEditor();
    emit onDeleteParametryzatorProfileTable(l2del);
}

//---------------------------------------------------------------------------

void ParametryzationProfilesWdgt::createEditor()
{
    ParametryzationProfileEditor *editor = new ParametryzationProfileEditor(gHelper, this);

    connect(this, &ParametryzationProfilesWdgt::startEditThisProfile, editor, &ParametryzationProfileEditor::startEdit);
    connect(ui->pbAdd, SIGNAL(clicked(bool)), editor, SLOT(startAdding()));
    connect(editor, &ParametryzationProfileEditor::saveThisSett, this, &ParametryzationProfilesWdgt::addThisProfile);

    connect(gHelper, SIGNAL(closeYourPopups()), editor, SLOT(smartHide())) ;

    connect(this, &ParametryzationProfilesWdgt::hideEditor, editor, &ParametryzationProfileEditor::hide);

    editor->setWindowTitle(tr("Editor"));
}

//---------------------------------------------------------------------------

void ParametryzationProfilesWdgt::on_tvTable_doubleClicked(const QModelIndex &index)
{
    const int row = proxy_model->mapToSource(index).row();
    if(row < 0)
        return;

    emit startEditThisProfile(model->item(row,0)->text(), model->item(row,0)->data().toHash());
}
