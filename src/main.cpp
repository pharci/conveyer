#include "app/app.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/res/icon.ico"));
    App w;
    w.setWindowTitle("Conveyer");
    w.show();
    return a.exec();
}