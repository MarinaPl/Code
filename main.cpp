#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


//обратное отображение эталонной и кодируемой строк
//подумать над выбором директории
//можно добавить очистку форм
