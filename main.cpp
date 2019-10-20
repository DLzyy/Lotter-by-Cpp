#include "QWMainWindow.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    freopen("err.txt", "w", stderr);
    QApplication a(argc, argv);
    QWMainWindow w;
    w.show();
    return a.exec();
}
