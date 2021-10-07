#ifndef TARIFFSETTVIEWER_H
#define TARIFFSETTVIEWER_H

#include <QWidget>

#include <QtCore>

///[!] widgets-shared
#include "template-pgs/smplptewdgt.h"

namespace Ui {
class TariffSettViewer;
}

class TariffSettViewer : public QWidget
{
    Q_OBJECT

public:
    explicit TariffSettViewer(const bool &editable, GuiHelper *gHelper, QWidget *parent = nullptr);
    ~TariffSettViewer();
    GuiHelper *gHelper;

    bool isEditable;

    static void findTariffIndex(const QVariantHash &hash, QComboBox *cbxTariffProgram, QMap<QString, int> &tariffline2cbxindex);


    QMap<QString, int> tariffline2cbxindex;
    static QString tariffHash2line(const QVariantHash &h);



    static QVariantHash defVal4tariff();


    QVariantHash getPageSett();





signals:
    void onWdgtIsReady();


    void setTariffProgramPte(const QString &lines);

public slots:
    void setPageSett(QVariantHash hash);

    void applyDefaultSett();


    void showCurrentTariffProgram(const int &indx);

private slots:
    void createWidgets();

private:
    Ui::TariffSettViewer *ui;
};

#endif // TARIFFSETTVIEWER_H
