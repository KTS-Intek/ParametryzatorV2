#include "dstsettviewer.h"
#include "ui_dstsettviewer.h"

DSTSettViewer::DSTSettViewer(const bool &editable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DSTSettViewer), isEditable(editable)
{
    ui->setupUi(this);
    ui->rbDisableDST->setEnabled(isEditable);
    ui->rbEnableDst->setEnabled(isEditable);
}

DSTSettViewer::~DSTSettViewer()
{
    delete ui;
}

QVariantHash DSTSettViewer::getPageSett()
{
    QVariantHash h;
    if(ui->rbEnableDst->isChecked()){
        h.insert("DST", true);
        h.insert("ST", "3,0,3,1");
        h.insert("NT", "10,0,4,1");
        h.insert("comment", ui->rbEnableDst->text());

    }else{
        h.insert("DST", false);
        h.insert("ST", "3,0,3,1");
        h.insert("NT", "10,0,4,1");
        h.insert("comment", ui->rbDisableDST->text());

    }
    return h;
}

void DSTSettViewer::setPageSett(const QVariantHash &hash)
{
    ui->rbEnableDst->setChecked(hash.value("DST").toBool());
    ui->rbDisableDST->setChecked(!ui->rbEnableDst->isChecked());
}

void DSTSettViewer::applyDefaultSett()
{
    setPageSett(QVariantHash());
}
