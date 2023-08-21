QT       += core gui network printsupport serialport sql charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APPLCTN_NAME=\\\"parametryzatorv2\\\"
#DEFINES += SMPLCOVERMODE
DEFINES += HASGUI4USR
DEFINES += HASSETTLOADEREMUL
#DEFINES += HASMAPS I don't need this

DEFINES += ENABLE_EXTSUPPORT_OF_IFACES
DEFINES += DISABLE_PROC_MANAGER
DEFINES += ITISPARAMETRYZATOR


VERSION = 1.0.1

DEFINES += "MYAPPNAME=\"\\\"Parametryzator V2\\\"\""
DEFINES += "MYAPPOWNER=\"\\\"KTS-Intek Ltd\\\"\""
DEFINES += "MYAPPOWNERSITE=\"\\\"http://kts-intek.com\\\"\""


DEFINES += DISABLE_METERSCHEDULER


DEFINES += DISABLEQCOMPRESSOR
DEFINES += DISABLE_UART_PRIORITY
DEFINES += DONOTINCLUDEFIREFLY
DEFINES += DISABLE_WATCHDOG_TIMER
DEFINES += DISABLE_PROC_MANAGER

#defines that disable some modules, that I don't want to use
DEFINES += DISABLEVPNMANAGER
DEFINES += DISABLECOMMANDONUPDATE
DEFINES += DISABLE_RUN_PROCESS
DEFINES += DONOTINCLUDEM2MSETTINGS


DEFINES += IS_ZBYRATOR
DEFINES += IS_ZBYRATORWDGT
DEFINES += ENABLE_SETTLOADER4MATILDA

#it tell to zbyrator-base to allow stops
DEFINES += ALLOW_HUMAN_STOPS

# I need some functionality
#DEFINES += IS_USESTARTDEV

#tells sqlite-server to use IPC for configuration
DEFINES += ENBL_IPC_4_CONFIG

# it disables modbus-bbb client
DEFINES += DISABLE_DATA_HOLDER_CLIENT


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


include(../../../Matilda-units/ifaces/ifaces/ifaces.pri)

include(../../../Matilda-units/qtwidgetonly/matilda-conf-shared-widgets/matilda-conf-shared-widgets.pri)

include(../../../Matilda-units/matilda-base/type-converter/type-converter.pri)


include(../../../Matilda-units/device-poll/zbyrator-base/zbyrator-base.pri)


include(../../../Matilda-units/matilda-bbb/matilda-bbb-settings/matilda-bbb-settings.pri)
include(../../../Matilda-units/matilda-base/MatildaIO/MatildaIO.pri)

include(../../../Matilda-units/gui/image-and-font/image-and-font.pri)

#include(../../../Matilda-units/charts/widget-charts/widget-charts.pri)

include(../../../Matilda-units/printer/printer-widgets/printer-widgets.pri)

include(../../../Matilda-units/qtwidgetonly/widgets-shared/widgets-shared.pri)

include(../../../Matilda-units/qtwidgetonly/lamp-conf-shared/lamp-conf-shared.pri)

include(../../../Matilda-units/device-poll/parametryzator-base/parametryzator-base.pri)



include(../../../Matilda-units/database/sqliteserver/sqliteserver.pri)

include(../../../Matilda-units/database/sqliteshared/sqliteshared.pri)

include(../../../Matilda-units/meter-plugin-shared/plugin-types/plugin-types.pri)


include(../../../Matilda-units/firefly/firefly-shared/firefly-shared.pri)

include(../../../Matilda-units/qtwidgetonly/widgets-meters/widgets-meters.pri)

include(../../../Matilda-units/matilda-bbb/matilda-bbb-serverside-shared/matilda-bbb-serverside-shared.pri)

include(../../../Matilda-units/charts/widget-charts/widget-charts.pri)

include(../../../Matilda-units/gui/guisett-shared-ucon/guisett-shared-ucon.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    parametryzator-wdgt-src/parametryzatorguimedium.cpp \
    parametryzator-wdgt-src/parametryzatorregisteredkeyswdgt.cpp \
    parametryzator-wdgts/getparametryzatorhistory.cpp \
    parametryzator-wdgts/humanaccessdialog.cpp \
    parametryzator-wdgts/paramertyzatorconfigtabwdgt.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/displaysettviewer.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/dstsettviewer.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzationprofileswdgttopper.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzatorprintsettingswdgt.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/paramnisearcher.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/printtvpreview.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaylimits.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaysettviewer.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaytestsettviewer.cpp \
    parametryzator-wdgts/parametryzation-viewer-wdgts/tariffsettviewer.cpp \
    parametryzator-wdgts/parametryzationprofileeditor.cpp \
    parametryzator-wdgts/parametryzationprofileswdgt.cpp \
    parametryzator-wdgts/parametryzationprofileviewer.cpp \
    parametryzator-wdgts/parametryzatorstartpage.cpp \
    parametryzator-wdgts/paramnisearcherwdgt.cpp \
    parametryzator-wdgts/parampollwdgtdad.cpp \
    parametryzator-wdgts/printparametryzator.cpp \
    parametryzator-wdgts/switchrelaywdgt.cpp

HEADERS += \
    mainwindow.h \
    parametryzator-wdgt-src/parametryzatorguimedium.h \
    parametryzator-wdgt-src/parametryzatorregisteredkeyswdgt.h \
    parametryzator-wdgts/getparametryzatorhistory.h \
    parametryzator-wdgts/humanaccessdialog.h \
    parametryzator-wdgts/paramertyzatorconfigtabwdgt.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/displaysettviewer.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/dstsettviewer.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzationprofileswdgttopper.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzatorprintsettingswdgt.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/paramnisearcher.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/printtvpreview.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaylimits.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaysettviewer.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaytestsettviewer.h \
    parametryzator-wdgts/parametryzation-viewer-wdgts/tariffsettviewer.h \
    parametryzator-wdgts/parametryzationprofileeditor.h \
    parametryzator-wdgts/parametryzationprofileswdgt.h \
    parametryzator-wdgts/parametryzationprofileviewer.h \
    parametryzator-wdgts/parametryzatorstartpage.h \
    parametryzator-wdgts/paramnisearcherwdgt.h \
    parametryzator-wdgts/parampollwdgtdad.h \
    parametryzator-wdgts/printparametryzator.h \
    parametryzator-wdgts/switchrelaywdgt.h

FORMS += \
    mainwindow.ui \
    parametryzator-wdgts/humanaccessdialog.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/displaysettviewer.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/dstsettviewer.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzationprofileswdgttopper.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/parametryzatorprintsettingswdgt.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaylimits.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaysettviewer.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/relaytestsettviewer.ui \
    parametryzator-wdgts/parametryzation-viewer-wdgts/tariffsettviewer.ui \
    parametryzator-wdgts/parametryzationprofileswdgt.ui \
    parametryzator-wdgts/parametryzationprofileviewer.ui \
    parametryzator-wdgts/paramnisearcherwdgt.ui \
    parametryzator-wdgts/parampollwdgtdad.ui \
    parametryzator-wdgts/printparametryzator.ui \
    parametryzator-wdgts/switchrelaywdgt.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resdef.qrc
