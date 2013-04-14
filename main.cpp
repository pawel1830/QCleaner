#include "mojaklasa.h"
#include <QApplication>
#include "thread1.h"
//QList<QString> listaSciezek;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   //thread1 watek;

    mojaklasa w;
    w.show();
  //  watek.start();
    
    return a.exec();
}
