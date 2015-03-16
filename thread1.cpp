#include "thread1.h"
#include <QTreeWidgetItem>
#include "mojaklasa.h"
#include <QMessageBox>



thread1::thread1(QObject *parent) :QThread(parent)
{

}
void thread1::ustawParametry(QList<QString> sciezkiPierwotne)
{
    QSettings settings("Qszukacz", "config");

    settings.beginGroup("configuration");

    szukaj_w_podk = settings.value("szuk_w_podk").toBool();
    czy_zapisac_liste = settings.value("lista_plikow").toBool();
     if (czy_zapisac_liste==true)
        zapis_listy = settings.value("sciezka_zapisu_listy").toString();
    szukaj_po_rozmiar = settings.value("rozmiar").toBool();
    szukaj_po_nazwa = settings.value("nazwa").toBool();

    settings.endGroup();
    sciezki.operator =(sciezkiPierwotne);

}


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

//void zapisListy()
void thread1::szukaj_duplikatow(QList<szukanie> listaPlikow)
{
    int liczbaZnalezionych = 0;

    int i=0;
    for ( i=0; i<listaPlikow.size();i++)
    {
        emit progress(i,listaPlikow.size());
        bool czyJestDuplikat=false;
        szukanie wzor = listaPlikow.at(i);
        QFile file(zapis_listy);
        if (zapis_listy.isEmpty()==false)
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                emit SendMessage("Błąd","Nie można zapisać listy plików!");
                emit watekStop();
            }
        for ( int a=i+1; a<listaPlikow.size() ;a++)
        {
            if (zapis_listy.isEmpty()==false)

                {
                    QTextStream out(&file);
                    out << listaPlikow[a].sciezka << "\n";
                    out.flush();
                }

            if (szukaj_po_nazwa==true && szukaj_po_rozmiar==true)
            {
                if (wzor.nazwa == listaPlikow.at(a).nazwa && wzor.rozmiar == listaPlikow.at(a).rozmiar)
                {
                    liczbaZnalezionych++;

                    QTreeWidgetItem* item = new QTreeWidgetItem();
                    item->setText(0,listaPlikow[a].nazwa);
                    item->setText(1,listaPlikow[a].sciezka);
                    item->setText(2,convertSize(listaPlikow[a].rozmiar));

                    listaPlikow.removeAt(a);

                    emit wyszukiwanie(item,liczbaZnalezionych);

                    czyJestDuplikat=true;
                }
            }
            else
                if(szukaj_po_nazwa==true)
                {
                    if (wzor.nazwa == listaPlikow.at(a).nazwa)
                    {
                        liczbaZnalezionych++;

                        QTreeWidgetItem* item = new QTreeWidgetItem();
                        item->setText(0,listaPlikow[a].nazwa);
                        item->setText(1,listaPlikow[a].sciezka);
                        item->setText(2,convertSize(listaPlikow[a].rozmiar));


                        listaPlikow.removeAt(a);

                        emit wyszukiwanie(item,liczbaZnalezionych);

                        czyJestDuplikat=true;
                    }
                }
            else
                    if (szukaj_po_rozmiar==true)
                        if (wzor.rozmiar == listaPlikow.at(a).rozmiar)
                        {
                            liczbaZnalezionych++;

                            QTreeWidgetItem* item = new QTreeWidgetItem();
                            item->setText(0,listaPlikow[a].nazwa);
                            item->setText(1,listaPlikow[a].sciezka);
                            item->setText(2,convertSize(listaPlikow[a].rozmiar));


                            listaPlikow.removeAt(a);

                            emit wyszukiwanie(item,liczbaZnalezionych);

                            czyJestDuplikat=true;
                        }
        }
        if (czyJestDuplikat==true)
        {
            QTreeWidgetItem* item2 = new QTreeWidgetItem();
            item2->setText(0,listaPlikow[i].nazwa);
            item2->setText(1,listaPlikow[i].sciezka);
            item2->setText(2,convertSize(listaPlikow[i].rozmiar));
            liczbaZnalezionych++;
            emit wyszukiwanie(item2,liczbaZnalezionych);
      }

    }
    emit progress(i,i);
    emit zakonczSzukanie(QString::fromUtf8("Zakończono.."));

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
