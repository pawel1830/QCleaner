#include "mojaklasa.h"
#include "ui_mojaklasa.h"
#include <QString>
#include <QFileDialog>
#include <QList>
#include "thread1.h"
#include <QTreeWidgetItem>
#include <QMessageBox>
 #include <QDesktopServices>


mojaklasa::mojaklasa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mojaklasa)
{
    ui->setupUi(this);
    watek = new thread1;
    connect(watek,SIGNAL(wyszukiwanie(QTreeWidgetItem*,int)),this,SLOT(listowanie(QTreeWidgetItem*,int)));
    connect(watek,SIGNAL(progress(int,int)),this,SLOT(onProgress(int,int)));
    connect(watek,SIGNAL(zakonczSzukanie(QString)),this,SLOT(zakonczSzukanie(QString)));
    connect(this,SIGNAL(wyslijParametry(bool,QList<QString>)),watek,SLOT(ustawParametry(bool,QList<QString>)));
    connect(ui->pushButton_2,SIGNAL(clicked()),ui->listaDuplikatow,SLOT(selectAll()));


    ui->usunWszystko->setHidden(true);
    ui->pushButton_2->setHidden(true);
    ui->listaDuplikatow->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->listaDuplikatow->addAction(ui->actionUsun);
    ui->listaDuplikatow->addAction(ui->actionPrzejdz);
    connect(ui->actionUsun,SIGNAL(triggered()),SLOT(usunItem()));
    connect(ui->actionPrzejdz,SIGNAL(triggered()),this,SLOT(przejdzFolder()));


    ui->progressBar->setHidden(true);
    ui->Anuluj->setHidden(true);
    ui->statusBar->showMessage(tr("Uruchomiono"),1500);
    //mojaklasa.moveToThread(watek);
}

mojaklasa::~mojaklasa()
{
    delete ui;
}

void mojaklasa::usunItem()
{
    int zaznDoUsun = ui->listaDuplikatow->selectedItems().size();


    if (zaznDoUsun>0)
    {
        QString trescKomunikatu;
        if(zaznDoUsun==1)
            trescKomunikatu = "Usunąć "+ui->listaDuplikatow->selectedItems().first()->text(1)+" ?";
        else
            trescKomunikatu = "Usunąć wszystkie("+QString::number(zaznDoUsun)+") pliki?";

        //odpowiedz = QMessageBox::question(this,"Usuwanie",trescKomunikatu,QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
        //odpowiedz = QMessageBox::question(this,"Usuwanie",trescKomunikatu,QMessageBox:|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
       QMessageBox msgBox;
       QAbstractButton *myNoButton = msgBox.addButton(trUtf8("Nie"), QMessageBox::NoRole);
       QAbstractButton *myYesButton = msgBox.addButton(trUtf8("Tak"), QMessageBox::YesRole);
       msgBox.setText("Usuwanie");
       msgBox.setInformativeText(trescKomunikatu);
       msgBox.setIcon(QMessageBox::Question);
       msgBox.exec();

        if (msgBox.clickedButton() == myYesButton)
        {
            //int zm = ui->listaDuplikatow->selectedItems().size();

            for(int i=0; i<zaznDoUsun;i++)
            {
                QTreeWidgetItem *item = ui->listaDuplikatow->selectedItems().first();
                QFile file;
                file.setFileName(item->text(1));
                if (file.remove()== true)
                {

                    int index = ui->listaDuplikatow->indexOfTopLevelItem(item);
                    ui->listaDuplikatow->takeTopLevelItem(index);
                }
                else
                    if (file.RemoveError)
                        QMessageBox::warning(this,"Błąd","Nie można usunąć"+file.fileName());

            }
        }


    }
}
void mojaklasa::przejdzFolder()
{

    QString sciezka( ui->listaDuplikatow->selectedItems().first()->text(1));
    QStringList list1 = sciezka.split("/");
    QString path;
    for (int i=0; i < list1.count()-1;i++ )
        path.append(list1.at(i)+"/");
    //QString run= program+path;
    QDesktopServices::openUrl(QUrl("file:///" + path));

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
        ui->Anuluj->setHidden(false);
        ui->pushButton_2->setHidden(true);
        ui->usunWszystko->setHidden(true);


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
    ui->statusBar->showMessage(tr("Przerwane.."),1500);
}
void mojaklasa::zakonczSzukanie(QString koniec)
{
   QString cos = koniec.number(ui->listaDuplikatow->topLevelItemCount());
    ui->statusBar->showMessage(koniec,3000);
    ui->Anuluj->setHidden(true);

    if(ui->listaDuplikatow->topLevelItemCount() > 0)
    {

        ui->pushButton_2->setHidden(false);
        ui->usunWszystko->setHidden(false);

    }

}



void mojaklasa::on_usunWszystko_clicked()
{
    ui->listaDuplikatow->selectAll();
    usunItem();
}
