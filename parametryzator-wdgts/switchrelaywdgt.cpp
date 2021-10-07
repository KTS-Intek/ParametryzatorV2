#include "switchrelaywdgt.h"
#include "ui_switchrelaywdgt.h"
#include <QTimer>


SwitchRelayWdgt::SwitchRelayWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchRelayWdgt)
{
    ui->setupUi(this);

    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(111);

    connect(ui->rbMainOff, SIGNAL(clicked(bool)), t, SLOT(start()));
    connect(ui->rbMainOn, SIGNAL(clicked(bool)), t, SLOT(start()));
    connect(ui->rbRead, SIGNAL(clicked(bool)), t, SLOT(start()));
    connect(ui->rbSecondOff, SIGNAL(clicked(bool)), t, SLOT(start()));
    connect(ui->rbSecondOn, SIGNAL(clicked(bool)), t, SLOT(start()));

    connect(t, SIGNAL(timeout()), this, SLOT(sendCurrentMode()));

    QTimer::singleShot(111, t, SLOT(start()));

}

SwitchRelayWdgt::~SwitchRelayWdgt()
{
    delete ui;
}

void SwitchRelayWdgt::sendCurrentMode()
{
    emit pageSettChanged(getRelayIndx());
}

int SwitchRelayWdgt::getRelayIndx()
{
    if(ui->rbRead->isChecked())
        return RELAY_READ;

    if(ui->rbMainOff->isChecked())
        return RELAY_LOAD_OFF;

    if(ui->rbMainOn->isChecked())
        return RELAY_LOAD_ON;

    if(ui->rbSecondOff->isChecked())
        return RELAY2_LOAD_OFF;

    if(ui->rbSecondOn->isChecked())
        return RELAY2_LOAD_ON;

    return RELAY_READ;


}
