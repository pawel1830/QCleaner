#include "thread1.h"
#include <QtCore>
#include <QDebug>
#include <QTreeWidgetItem>
#include "mojaklasa.h"



thread1::thread1(QObject *parent) :QThread(parent)
{

}
void thread1::ustawParametry(bool podKatalog, QList<QString> sciezkiPierwotne)
{
    sciezki.operator =(sciezkiPierwotne);
    szukaj_w_podk = podKatalog;
}

/*
    QDir dir;

        dir.setPath(nazwa);
        dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs | QDir::AllEntries);
        dir.setSorting(QDir::Size | QDir::Reversed);

   */

void thread1::tworzListePlikow(QDir dir)
{

    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
            for(int i=0; i<list.size();++i)
            {
                    //qDebug() <<  indent + finfo.fileName() << endl;
                QFileInfo fInfo = list.at(i);
                    if (fInfo.isDir() && szukaj_w_podk==true) {
                            tworzListePlikow(QDir(fInfo.absoluteFilePath()));
                    }
                    if (fInfo.isFile() && fInfo.isHidden()==false)
                    {
                        szukanie dane;
                        dane.nazwa=fInfo.fileName();
                        dane.sciezka=fInfo.absoluteFilePath();
                        dane.rozmiar=fInfo.size();
                        lista_plikow.push_back(dane);

                    }
                    //emit progress(i,list.size());
            }
}
QString convertSize(double size)
{
    QString wynik;
   double typ = 1000;
    int kilobyte = typ;


        double megabyte = kilobyte * typ;
        double gigabyte = megabyte * typ;
        double terabyte = gigabyte * typ;


        if ((size >= 0) && (size < kilobyte)) {
            wynik = QString::number(size) + 'B';

        } else if ((size >= kilobyte) && (size < megabyte)) {
            wynik = (QString::number(size/kilobyte,'f',2))+"KB";


        } else if ((size >= megabyte) && (size < gigabyte)) {
            wynik = (QString::number(size/megabyte,'f',2))+"MB";

        } else if ((size >= gigabyte) && (size < terabyte)) {

            wynik = (QString::number(size/gigabyte,'f',2))+"GB";

        } else if (size >= terabyte) {

             wynik = (QString::number(size/terabyte,'f',2))+"TB";

        } else {
            wynik = QString::number(size) + 'B';
}
        return wynik;
}
void thread1::szukaj_duplikatow(QList<szukanie> listaPlikow)
{
    int liczbaZnalezionych = 0;

    int i=0;
    //qDebug() << listaPlikow.size();
    for ( i=0; i<listaPlikow.size();i++)
    {
        emit progress(i,listaPlikow.size());


        bool czyJestDuplikat=false;
        szukanie wzor = listaPlikow.at(i);
        for ( int a=i+1; a<listaPlikow.size() ;a++)
        {
            //szukanie porownanie = listaPlikow.at(a);
            //qDebug() << a << "początek";

            //qDebug() << i << " & " << a << " & " << pierwszyRaz;

            //if (listaPlikow.operator [](i).nazwa == listaPlikow.operator [](a).nazwa)
            if (wzor.nazwa == listaPlikow.at(a).nazwa)
            {
                //QFileInfo fileInfo;

                liczbaZnalezionych++;
               QTreeWidgetItem* item = new QTreeWidgetItem();

                item->setText(0,listaPlikow[a].nazwa);
               //item->setText(2,lista_plikow.value(a).rozmiar);
              item->setText(1,listaPlikow[a].sciezka);
              item->setText(2,convertSize(listaPlikow[a].rozmiar));
              //item->setText(2,QString::number(listaPlikow[a].rozmiar));
              listaPlikow.removeAt(a);
              emit wyszukiwanie(item,liczbaZnalezionych);
              //qDebug() << "znaleziono";
              //qDebug() << "znaleziono " << a;
              czyJestDuplikat=true;


            }
            //listaPlikow.removeAt(i);
        }
        if (czyJestDuplikat==true)
        {
        QTreeWidgetItem* item2 = new QTreeWidgetItem();
          item2->setText(0,listaPlikow[i].nazwa);
          item2->setText(1,listaPlikow[i].sciezka);
          item2->setText(2,convertSize(listaPlikow[i].rozmiar));
         // ui->listaplikow->addTopLevelItem(item2);
          liczbaZnalezionych++;
          emit wyszukiwanie(item2,liczbaZnalezionych);
          //qDebug() << i << pierwszyRaz;
        //  msleep(1000);

      }

    }
    emit progress(i,i);
    emit zakonczSzukanie("Zakończono..");

}
void thread1::run()
{

    lista_plikow.clear();

    for (int i=0; i<sciezki.count(); i++)
    {
        tworzListePlikow(QDir(sciezki.at(i)));
    }


  thread1::szukaj_duplikatow(lista_plikow);
    //((mojaklasa*)parent())->skanuj(sciezki);
}
