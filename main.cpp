#include <QApplication>
#include "window.h"

int main(int argc, char *argv[])
{
    // create an application with commandline arguments
    QApplication a(argc, argv);
    // turn off quit the application when all windows are closed
    a.setQuitOnLastWindowClosed(false);
    Window w;
    // show the window
    w.show();

    return a.exec();
}
