#include "humanaccessdialog.h"
#include "ui_humanaccessdialog.h"

HumanAccessDialog::HumanAccessDialog(const QString &message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HumanAccessDialog)
{
    ui->setupUi(this);
    ui->lblText->setText(message);

    setModal(true);
    QTimer::singleShot(11, this, SLOT(createObjects()));
}

HumanAccessDialog::~HumanAccessDialog()
{
    delete ui;
}

void HumanAccessDialog::onTmrTo()
{

    if(tmrCounter > 0){
        ui->lblIcon->setPixmap(QPixmap( (tmrCounter%2) ? ":/katynko/dialog-svg/dialog-error.svg" : ":/katynko/dialog-svg/dialog-warning.svg"));
        tmrCounter--;
        ui->lblTime->setText(tr("<h2>%1 [sec]</h2>").arg(tmrCounter));
        return;
    }
    ui->lblIcon->setPixmap(QPixmap(":/katynko/dialog-svg/dialog-question.svg" ));

    ui->lblTime->setText(tr("Accepted"));

    QTimer::singleShot(444, this, SLOT(on_buttonBox_accepted()));
}

void HumanAccessDialog::createObjects()
{
    const int oneh = ui->buttonBox->height();
    const int minl = oneh * 4;
    const int maxl = oneh * 5;
    ui->lblIcon->setMinimumSize(minl, minl);
    ui->lblIcon->setMaximumSize(maxl, maxl);



    tmrCounter = 15;
    QTimer *t = new QTimer(this);
    t->setSingleShot(false);
    t->setInterval(1000);

    connect(t, SIGNAL(timeout()), this ,SLOT(onTmrTo()));


    t->start();

    if(ui->lblText->text().isEmpty())
        tmrCounter = 0;

    onTmrTo();
}

void HumanAccessDialog::on_buttonBox_accepted()
{
    accept();
    deleteLater();
}



void HumanAccessDialog::on_buttonBox_rejected()
{
    reject();
    deleteLater();
}
