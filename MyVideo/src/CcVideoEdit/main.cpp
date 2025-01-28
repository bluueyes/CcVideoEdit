#include "CcVideoUI.h"
#include <QtWidgets/QApplication>
#include "CcAudio.h"

int main(int argc, char *argv[])
{
    //CcAudio::GetInstance()->ExPortA("cc.mp4", "out.mp3");
    //CcAudio::GetInstance()->Merge("test1.mp4", "out.mp3", "out.mp4");
    QApplication a(argc, argv);
    CcVideoUI w;
    w.show();
    return a.exec();
}
