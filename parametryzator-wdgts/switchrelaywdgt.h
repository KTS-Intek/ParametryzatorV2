#ifndef SWITCHRELAYWDGT_H
#define SWITCHRELAYWDGT_H

#include <QWidget>
#include "definedpollcodes.h"

namespace Ui {
class SwitchRelayWdgt;
}

class SwitchRelayWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchRelayWdgt(QWidget *parent = nullptr);
    ~SwitchRelayWdgt();


signals:
    void pageSettChanged(int relaySwitch);


public slots:
    void sendCurrentMode();


private:
    Ui::SwitchRelayWdgt *ui;

    int getRelayIndx();
};

#endif // SWITCHRELAYWDGT_H
