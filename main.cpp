#include "mainwindow.h"
#include <QApplication>
//#include "main.h"


int main(int argc, char *argv[])
{
    char * id []= {"null","4950","127.0.0.1"};
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //test(3,id);
    return a.exec();
}
