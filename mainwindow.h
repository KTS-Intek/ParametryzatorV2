#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///[!] widgets-shared
#include "gui-src/wdgt/mainifacemedium.h"


#include "parametryzator-wdgt-src/parametryzatorguimedium.h"



#include "medium4modelsdatatypes.h"

/////[!] lamp-configurator
//#include "lamp-src/lampmedium.h"


#define STACK_PAGE_CONFIG   0
#define STACK_PAGE_IFACE    1
#define STACK_PAGE_PROFILES 2
#define STACK_PAGE_HISTORY  3
#define STACK_PAGE_PRINTING 4
#define STACK_PAGE_LOG      5
#define STACK_PAGE_NISRCHR  6


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public MainIfaceMedium
{
    Q_OBJECT

public:
    MainWindow(const QFont &font4log, const int &defFontPointSize, QWidget *parent = nullptr);
    ~MainWindow();

    ParametryzatorGuiMedium *paramMedium;


signals:
    void onRequest2GetDataThese(QStringList nis, quint8 metertype);
    void onRequest2pollThese2wdgt(QStringList nis, quint8 metertype);


    void setPrintingSettings(QVariantHash sett);

    void setPageSettpPrintingGrp(QVariantHash lastPageSett);
    void setPageSettpPrintingMain(QVariantHash mainPrintSett);

    void setPageSettpPrintingHtml(QVariantHash lastPageSett);

    void startPrintingTariffReports(MPrintTableOut printout, QVector<QVariantHash> printdata);


#ifdef ALLOW_HUMAN_STOPS

    void resetMakeApause();

    void closeMessageIneedHuman();

#endif

    void setNewConsumerAddress(QString address);
    void onConsumerAddressChanged(QString address, QString workingdirectory);

public slots:


    void openIfaceSett();

    void showLastWdgt();


    void onRequest2pollThese(QStringList nis, quint8 metertype);




    void gimmeMyLastSettingsPrint();

    void setLastPageSettPrint(QVariantHash pagesett);
    void gimmeMyLastSettingsPrintMain();

    void gimmeMyLastSettingsPrintHtmlProfiles();


#ifdef ALLOW_HUMAN_STOPS

    void showMessageIneedHuman(QString message, QString deviceni);

#endif

private slots:
    void initPage();

    void onActClicked();

    void onShowThisWdiget(const int &tag);





    void on_actionGlobal_triggered();


//    void createFireflyTaskManager();
    void createParametryzatorTaskManager();



    void on_actionPrint_triggered();

    void on_actionPoll_triggered();




private: //functions


    bool createThisWdiget(const int &tag);

    void createActions();

    void createAppOutLog();

    void createSqliteServer();


      QWidget *createSTACK_PAGE_CONFIG();//operations sw
      QWidget *createSTACK_PAGE_IFACE();//iface settings
      QWidget *createSTACK_PAGE_PROFILES(); // parametryzation profiles
      QWidget *createSTACK_PAGE_HISTORY(); //history of parametryzation

      QWidget *createSTACK_PAGE_PRINTING(); //history of parametryzation


      QWidget *createSTACK_PAGE_LOG(); //iface exhange


      QWidget *createSTACK_PAGE_NISRCHR(); //history of ni's reports


    QAction *addAction2bar(const QString &text, const QString &icon, const int &wdgtTag);


private:
    Ui::MainWindow *ui;

    QMap<int, QWidget*> readyWdgts;

    quint8 lastWdgtTag;

};
#endif // MAINWINDOW_H

