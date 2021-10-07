#ifndef PARAMETRYZATIONPROFILESWDGTTOPPER_H
#define PARAMETRYZATIONPROFILESWDGTTOPPER_H

#include <QWidget>
#include <QStringList>

#include <QVariantHash>

namespace Ui {
class ParametryzationProfilesWdgtTopper;
}

class ParametryzationProfilesWdgtTopper : public QWidget
{
    Q_OBJECT

public:
    explicit ParametryzationProfilesWdgtTopper(QWidget *parent = nullptr);
    ~ParametryzationProfilesWdgtTopper();



    struct ParamLastSett
    {
        QString workigndir;
        QStringList lastnis;

        QVariantHash mainsett;

        QHash<QString, int> hNi2Counter;
        bool isSearching;

        ParamLastSett() : isSearching(false) {}
    } lsett;

signals:
    void openNiChecker(QString dir, QString workingdirectory);

    void openWorkingDirectorySettings();


    void setNewConsumerAddress(QString address);

    void setConsumerAddressFromWriteTariff(QString address);

    void startTmrCalcNiWarns();

    void gimmeMainSettHash();

    void kickOffAllObjects();


    void startSearcher(QStringList nis, QString workingdirectory, QVariantHash mainsett);

public slots:
    void onConsumerAddressChanged(QString address, QString workingdirectory);


    void sendCurrentConsumerAddress();

    void onNiListUpdated(QStringList nis);

    void recalculateNiWarningsLater();

    void recalculateNiWarnings();


    void addThisNI(qint64 msecsep, QString sn, QString ni, QString dirname, QString fullpath2dir);
    void searchfinished(int counter);

private slots:
    void on_lblWarnings_linkActivated(const QString &link);


    void createAllConnections();

//    void onTextChanged(QString text);



    void on_lblWorkingDirectory_linkActivated(const QString &link);

    void on_tbUpdateCompleter_clicked();

    void on_tbNIchecker_clicked();

private:
    Ui::ParametryzationProfilesWdgtTopper *ui;
};

#endif // PARAMETRYZATIONPROFILESWDGTTOPPER_H
