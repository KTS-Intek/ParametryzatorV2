#include "parametryzationprofileeditor.h"

//--------------------------------------------------------------------

ParametryzationProfileEditor::ParametryzationProfileEditor(GuiHelper *gHelper, QWidget *parent) :
    ReferenceAddEditWdgt("ParametryzationProfileEditor", false, parent), gHelper(gHelper)
{
    connect(this, SIGNAL(referenceHeight(int)), this, SLOT(createWidgets(int)));
    connect(this, SIGNAL(addClicked(bool)), this, SLOT(onAddClick()));
    connect(this, SIGNAL(addHideClicked(bool)), this, SLOT(onAddHideClick()));
}

ParametryzationProfileEditor::OneParametryzationProfileSett ParametryzationProfileEditor::getCurrentPageSett()
{
    OneParametryzationProfileSett sett;
    sett.sett = settw->getPageSett(sett.lastprofilename);
    return sett;
}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::createWidgets(int refheight)
{
    QWidget *w = new QWidget(this);
    settw = new ParametryzationProfileViewer(true, gHelper, w);

    QVBoxLayout *l = new QVBoxLayout(w);
    w->setLayout(l);
    l->addWidget(settw);

//    QWidget *spacer = new QWidget(this);
//    spacer->resize(5,5);
//    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
//    l->addWidget(spacer);

    setSaCentralWidgetAndRefHeight(w, 10, 600, refheight);
    clearThePage();
}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::onTbClearPressed()
{
    pgsett.sett = getCurrentPageSett();
    pgsett.hasSett = true;
    emit onHasUndoData(true);
    clearThePage();
}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::onTbUndoPressed()
{
    emit onHasUndoData(false);
    if(!pgsett.hasSett)
        return;
    setPageSett(pgsett.sett);
}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::clearThePage()
{
    setPageSett(OneParametryzationProfileSett());
    settw->resetAll2defSett();

}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::onAddClick()
{
    const OneParametryzationProfileSett sett = getCurrentPageSett();
    emit saveThisSett(sett.lastprofilename, sett.sett);
}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::onAddHideClick()
{
    onAddClick();
    hide();
}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::setPageSett(ParametryzationProfileEditor::OneParametryzationProfileSett sett)
{
    setPageSettExt(sett.lastprofilename, sett.sett);

}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::setPageSettExt(QString profname, QVariantHash profile)
{
    settw->setThisPagesSett(profname, profile);



}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::startEdit(QString profname, QVariantHash profile)
{
    setPageSettExt(profname, profile);
    showLater();
}

//--------------------------------------------------------------------

void ParametryzationProfileEditor::startAdding()
{
    showLater();
    bool ok;
    settw->resetProfileName();
    const QString linestr = gHelper->getClipboardContentJsonTxt(ok);
    if(ok && !linestr.isEmpty()){
        const QJsonObject j = QJsonDocument::fromJson(linestr.toUtf8()).object();
        if(j.isEmpty())
            return;

        if(j.contains("tariffsett") && j.contains("tariffname")){
            const QVariantHash sett = j.value("tariffsett").toObject().toVariantHash();
            const QString name = j.value("tariffname").toString();
            setPageSettExt(name, sett);
        }


    }

}

//--------------------------------------------------------------------
