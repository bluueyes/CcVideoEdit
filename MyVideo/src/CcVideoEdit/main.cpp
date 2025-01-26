#include "CcVideoUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CcVideoUI w;
    w.show();
    return a.exec();
}
