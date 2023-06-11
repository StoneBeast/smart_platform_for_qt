#include "smarthome.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
    qputenv("QT_IM_MODULE", QByteArray("softkeyboard"));
#endif

    QApplication a(argc, argv);
   //SmartHome w;
   //w.show();
    VisibleNetwork v;
    v.show();
    return a.exec();
}
