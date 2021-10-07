#ifndef PARAMETRYZATIONPROFILEEDITOR_H
#define PARAMETRYZATIONPROFILEEDITOR_H


///[!] widgets-shared
#include "gui-src/wdgt/referenceaddeditwdgt.h"


#include "parametryzationprofileviewer.h"

class ParametryzationProfileEditor : public ReferenceAddEditWdgt
{
    Q_OBJECT
public:
    explicit ParametryzationProfileEditor(GuiHelper *gHelper, QWidget *parent = nullptr);

    GuiHelper *gHelper;

    struct OneParametryzationProfileSett
    {

        QVariantHash sett;
        QString lastprofilename;

        OneParametryzationProfileSett() {}
    };
    OneParametryzationProfileSett getCurrentPageSett();

signals:
    void saveThisSett(QString profname, QVariantHash profile);


public slots:
    void createWidgets(int refheight);

    void onTbClearPressed();

    void onTbUndoPressed();

    void clearThePage();


    void onAddClick();

    void onAddHideClick();


    void setPageSett(OneParametryzationProfileSett sett);

    void setPageSettExt(QString profname, QVariantHash profile);


    void startEdit(QString profname, QVariantHash profile);

    void startAdding();

private:
    ParametryzationProfileViewer *settw;

    struct ThisPageSett
    {
       OneParametryzationProfileSett sett;
        bool hasSett;
        ThisPageSett() : hasSett(false) {}


    } pgsett;
};

#endif // PARAMETRYZATIONPROFILEEDITOR_H
