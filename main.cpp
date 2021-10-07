#include "mainwindow.h"

#include <QApplication>
#include "guisett-shared-src/appfontloader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const AppFontLoader::LoadFontMemo m = AppFontLoader::loadFontAndSett();

    MainWindow w(m.font4log, m.defFontPointSize);
    w.showSplash();

    return a.exec();
}
