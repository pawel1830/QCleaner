#include "mojaklasa.h"
#include "ui_mojaklasa.h"
#include <QString>
#include <QFileDialog>
#include <QList>
#include "thread1.h"
#include <QTreeWidgetItem>
#include <QMessageBox>


mojaklasa::mojaklasa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mojaklasa)
{
    ui->setupUi(this);
    watek = new thread1;
    connect(watek,SIGNAL(wyszukiwanie(QTreeWidgetItem*,int)),this,SLOT(listowanie(QTreeWidgetItem*,int)));
    connect(watek,SIGNAL(progress(int,int)),this,SLOT(onProgress(int,int)));
    connect(this,SIGNAL(wyslijParametry(bool,QList<QString>)),watek,SLOT(ustawParametry(bool,QList<QString>)));
    ui->progressBar->setHidden(true);
    ui->Anuluj->setHidden(true);
    ui->statusBar->showMessage(tr("Uruchomiono"),1500);
    //mojaklasa.moveToThread(watek);
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
   // listaSciezek.push_back(katalog);
    //ui->lineEdit->setText(katalog);
   // listuj_kat(katalog);
    ui->listWidget->addItem(katalog);
}


void mojaklasa::listowanie(QTreeWidgetItem *item, int znaleziono)
{
    ui->listaDuplikatow->addTopLevelItem(item);
    ui->statusBar->showMessage(tr("Znaleziono ")+QString::number(znaleziono)+tr(" plików"),0);
    \
}
void mojaklasa::onProgress(int p,int max)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(p);
}

void mojaklasa::on_skanuj_clicked()
{

   /* for (int i=0; i<=listaSciezek.size();i++)
    {

   listuj(listaSciezek.value(i));
    }
    szukaj_duplikatow();
*/ui->listaDuplikatow->clear();
    if (ui->listWidget->count()>=1)
    {
        ui->statusBar->showMessage(tr("Skanowanie.."),0);
        ui->progressBar->setHidden(false);
        ui->Anuluj->setHidden(false);
        QList<QString>listaaa;
        for (int i=0; i<ui->listWidget->count();i++)
        {
            listaaa.operator +=(ui->listWidget->item(i)->text());
            //qDebug() << ui->listWidget->item(i)->text();
        }

        mojaklasa::wyslijParametry(ui->checkBox->isChecked(),listaaa);
        watek->start();
        ui->listaDuplikatow->setSortingEnabled(true);
        ui->listaDuplikatow->sortByColumn(1,Qt::AscendingOrder);
        ui->Anuluj->setHidden(true);

    }
    else
    {
        QMessageBox::warning(this,"Błąd","Lista jest pusta! Dodaj katalog do listy i spróbuj ponownie");
    }

}

void mojaklasa::on_usun_clicked()
{
   /* QList<QListWidgetItem*> listaDoUsuniecia = ui->listWidget->selectedItems();
    for (int i=0; i<listaDoUsuniecia.size(); i++)
    {*/
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
       /* ui->listWidget->removeItemWidget(listaDoUsuniecia[i]);
        qDebug() << listaDoUsuniecia.at(i);
    }*/
}

void mojaklasa::on_Anuluj_clicked()
{


    while (watek->isRunning())
   { watek->terminate(); }
    ui->progressBar->setHidden(true);
    ui->Anuluj->setHidden(true);

    if (watek->isRunning()==false)
        QMessageBox::information(this,"koniec roboty","Przeszukiwanie Przerwane przez Użytkownika");
}
