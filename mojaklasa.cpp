#include "mojaklasa.h"
#include "ui_mojaklasa.h"
#include <QString>
#include <QFileDialog>
#include <QList>


mojaklasa::mojaklasa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mojaklasa)
{
    ui->setupUi(this);
}

mojaklasa::~mojaklasa()
{
    delete ui;
}

void mojaklasa::on_pushButton_clicked()
{

    QString katalog = QFileDialog::getExistingDirectory(this, tr("Otwórz katalog"),
                                                      "/home",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    listaSciezek.push_back(katalog);
    ui->lineEdit->setText(katalog);
    listuj_kat(katalog);
    ui->listWidget->addItems(listaSciezek);
}

void mojaklasa::listuj_kat(QString nazwa)
{


    QDir dir;

        dir.setPath(nazwa);
        dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs | QDir::AllEntries);
        dir.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = dir.entryInfoList();
        foreach(QFileInfo fileInfo, list)
        {
            if(fileInfo.isDir() && fileInfo.fileName()!="." && fileInfo.fileName()!="..")
          {



            listaSciezek.push_back(fileInfo.filePath());
            listuj_kat(fileInfo.filePath());



          }


        }
}



void mojaklasa::listuj(QString nazwa)
{
    QDir dir;

        dir.setPath(nazwa);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = dir.entryInfoList();
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

        //ewentualne zrzucenie listy do pliku
}
void mojaklasa::szukaj_duplikatow()
{

    //int i=0;
    for (int i=0; i<lista_plikow.size();i++)
    {
        for (int a=i+1; a<lista_plikow.size() ;a++)
        {
            if (lista_plikow[i].nazwa == lista_plikow[a].nazwa)
            {
                //QFileInfo fileInfo;
                QTreeWidgetItem* item = new QTreeWidgetItem();

                item->setText(0,lista_plikow[a].nazwa);
               //item->setText(2,lista_plikow.value(a).rozmiar);
              item->setText(1,lista_plikow[a].sciezka);
                ui->listaplikow->addTopLevelItem(item);


              QTreeWidgetItem* item2 = new QTreeWidgetItem();
                item2->setText(0,lista_plikow[i].nazwa);
                item2->setText(1,lista_plikow[i].sciezka);
                ui->listaplikow->addTopLevelItem(item2);


            }
        }
    }

}
void mojaklasa::on_skanuj_clicked()
{

    for (int i=0; i<=listaSciezek.size();i++)
    {

   listuj(listaSciezek.value(i));
    }
    szukaj_duplikatow();

}
