#ifndef HUMANACCESSDIALOG_H
#define HUMANACCESSDIALOG_H

#include <QDialog>
#include <QTime>
#include <QTimer>

namespace Ui {
class HumanAccessDialog;
}

class HumanAccessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HumanAccessDialog(const QString &message, QWidget *parent = nullptr);
    ~HumanAccessDialog();


    int tmrCounter ;

public slots:
    void onTmrTo();

    void createObjects();


private slots:
    void on_buttonBox_accepted();


    void on_buttonBox_rejected();

private:
    Ui::HumanAccessDialog *ui;
};

#endif // HUMANACCESSDIALOG_H
