#ifndef DSTSETTVIEWER_H
#define DSTSETTVIEWER_H

#include <QWidget>

namespace Ui {
class DSTSettViewer;
}

class DSTSettViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DSTSettViewer(const bool &editable, QWidget *parent = nullptr);
    ~DSTSettViewer();
    bool isEditable;

    QVariantHash getPageSett();

public slots:
    void setPageSett(const QVariantHash &hash);

    void applyDefaultSett();

private:
    Ui::DSTSettViewer *ui;
};

#endif // DSTSETTVIEWER_H
