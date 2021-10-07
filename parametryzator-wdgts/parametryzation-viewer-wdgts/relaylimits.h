#ifndef RELAYLIMITS_H
#define RELAYLIMITS_H

#include <QWidget>

namespace Ui {
class RelayLimits;
}

class RelayLimits : public QWidget
{
    Q_OBJECT

public:
    explicit RelayLimits(const bool &editable, QWidget *parent = nullptr);
    ~RelayLimits();

     bool isEditable;

    QVariantHash getDefSettHash();

    QVariantHash getPageSett();


signals:
    void onWdgtIsReady();


public slots:
    void setPageSett(const QVariantHash &relaylimits);

    void applyDefaultSett();


private slots:
    void createWidgets();

    void setThisSett(const QVariantHash &relaylimits);

private:
    Ui::RelayLimits *ui;
};

#endif // RELAYLIMITS_H
