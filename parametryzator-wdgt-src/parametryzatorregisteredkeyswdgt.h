#ifndef PARAMETRYZATORREGISTEREDKEYSWDGT_H
#define PARAMETRYZATORREGISTEREDKEYSWDGT_H


///[!] widgets-shared
#include "gui-src/wdgt/referencewidgetclassgui.h"


class ParametryzatorRegisteredKeysWdgt : public ReferenceWidgetClassGui
{
    Q_OBJECT
public:
    explicit ParametryzatorRegisteredKeysWdgt(GuiHelper *gHelper, QWidget *parent = nullptr);

signals:

public slots:
    void initPage();

    void showRegisteredKeys();

};

#endif // PARAMETRYZATORREGISTEREDKEYSWDGT_H
