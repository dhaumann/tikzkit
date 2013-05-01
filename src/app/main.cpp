#include <QApplication>
#include "DemoWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DemoWidget w;
    w.resize(640, 480);
    w.show();

    return app.exec();
}

// kate: indent-width 4; replace-tabs on;
