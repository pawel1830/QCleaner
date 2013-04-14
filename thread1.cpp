#include "thread1.h"
#include <QtCore>
#include <QDebug>
#include <QTreeWidgetItem>
#include "mojaklasa.h"


/*void mojaklasa::skanuj(QList<QString> Sciezki)
{
    for (int i=0; i<=Sciezki.size();i++)
    {

  listuj(Sciezki.value(i));
    }
    szukaj_duplikatow();
}*/

thread1::thread1(QObject *parent) :QThread(parent)
{

}
void thread1::ustawParametry(bool podKatalog, QList<QString> sciezkiPierwotne)
{
    sciezki.operator =(sciezkiPierwotne);
    szukaj_w_podk = podKatalog;
}

void thread1::listuj_kat(QString nazwa)
{


    QDir dir;

        dir.setPath(nazwa);
        dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs | QDir::AllEntries);
        dir.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = dir.entryInfoList();
        foreach(QFileInfo fileInfo, list)
        {
            if(fileInfo.isDir() && fileInfo.isHidden()==false)
          {



            sciezki.push_back(fileInfo.filePath());
            listuj_kat(fileInfo.filePath());



          }


        }
}
QList<thread1::szukanie> thread1::listuj(QString nazwa)
{
    QDir dir;

        dir.setPath(nazwa);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = dir.entryInfoList();
        QList<szukanie> lista_plikow;
        foreach(QFileInfo fileInfo, list)
            {


              if(fileInfo.isFile())
              {
             //   QTreeWidgetItem* item = new QTreeWidgetItem();
            //  item->setText(0,fileInfo.fileName());
            //   item->setText(2,QString::number(fileInfo.size()/1000000));
             //  item->setText(1,fileInfo.filePath());
                szukanie dane;
                dane.nazwa=fileInfo.fileName();
                dane.sciezka=fileInfo.absoluteFilePath();
                dane.rozmiar=fileInfo.size();

                lista_plikow.push_back(dane);
             //   ui->listaplikow->addTopLevelItem(item);
               // item->setIcon(0,*(new QIcon("file.jpg")));
              }





            }
        return lista_plikow;

        //ewentualne zrzucenie listy do pliku
}
QList<thread1::szukanie> thread1::szukanieFaza1(/*QList<QString> sciezkiKatalogow,*/ bool podkatalog)
{
    QList<thread1::szukanie> calaLista;
    if (podkatalog==true)
    {
        for (int i=0; i<sciezki.count(); i++)
        {
            emit progress(i,sciezki.count());
            //QString zmienna=sciezkiKatalogow.at(i);
            thread1::listuj_kat(sciezki.at(i));
        }
    }
    for (int i=0; i<sciezki.count(); i++)
    {
        calaLista.operator +=(thread1::listuj(sciezki.at(i)));
    }
    return calaLista;
}


void thread1::szukaj_duplikatow(QList<szukanie> listaPlikow)
{

    int i=0;
    qDebug() << listaPlikow.size();
    for ( i=0; i<listaPlikow.size();i++)
    {
        emit progress(i,listaPlikow.size());
        for (int a=i+1; a<listaPlikow.size() ;a++)
        {
            qDebug() << "szukanie";
            if (listaPlikow[i].nazwa == listaPlikow[a].nazwa)
            {
                //QFileInfo fileInfo;
                QTreeWidgetItem* item = new QTreeWidgetItem();

                item->setText(0,listaPlikow[a].nazwa);
               //item->setText(2,lista_plikow.value(a).rozmiar);
              item->setText(1,listaPlikow[a].sciezka);
              emit wyszukiwanie(item);
              qDebug() << "znaleziono";


              QTreeWidgetItem* item2 = new QTreeWidgetItem();
                item2->setText(0,listaPlikow[i].nazwa);
                item2->setText(1,listaPlikow[i].sciezka);
               // ui->listaplikow->addTopLevelItem(item2);
                emit wyszukiwanie(item2);


            }
        }
    }
    emit progress(i,i);

}
void thread1::run()
{
    qDebug() << "running";
  /*  for (int i=0; i<=mojaklasa::listaSciezek.size();i++)
    {

   mojaklasa::listuj(mojaklasa::listaSciezek.value(i));
    }
    mojaklasa::szukaj_duplikatow();*/
   // QList<QString>sciezki;
//    sciezki = (((mojaklasa*)parent())->listaSciezek);
   // sciezki.push_back(tr("/home/pawel/"));
    //bool szukaj_w_podk = true;//((mojaklasa::ui*)->checkBox->isChecked());
  // thread1::szukanieFaza1(sciezki,szukaj_w_podk);
    QList<szukanie> lista_do_szukania;
    lista_do_szukania.operator =(szukanieFaza1(szukaj_w_podk));
    qDebug() << "lista do szukania";
   thread1::szukaj_duplikatow(lista_do_szukania);
    //((mojaklasa*)parent())->skanuj(sciezki);
}
