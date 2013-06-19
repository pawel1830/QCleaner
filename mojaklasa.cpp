#include "mojaklasa.h"
#include "ui_mojaklasa.h"
#include <QString>
#include <QFileDialog>
#include <QList>
#include "thread1.h"
#include <QTreeWidgetItem>
#include <QMessageBox>
 #include <QDesktopServices>
#include "settings.h"



Ustawienia *settings_window;
mojaklasa::mojaklasa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mojaklasa)
{
    ui->setupUi(this);
    watek = new thread1;

    settings_window = new Ustawienia;
  //  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8") );
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("ISO-8859-2") );
    connect(watek,SIGNAL(wyszukiwanie(QTreeWidgetItem*,int)),this,SLOT(listowanie(QTreeWidgetItem*,int)));
    connect(watek,SIGNAL(progress(int,int)),this,SLOT(onProgress(int,int)));
    connect(watek,SIGNAL(zakonczSzukanie(QString)),this,SLOT(zakonczSzukanie(QString)));
    connect(this,SIGNAL(wyslijParametry(QList<QString>)),watek,SLOT(ustawParametry(QList<QString>)));
    connect(ui->pushButton_2,SIGNAL(clicked()),ui->listaDuplikatow,SLOT(selectAll()));
 //   connect(settings_window,SIGNAL(glowne_okno_pokaz(bool)),this,SLOT(setEnabled(bool)));
    connect(watek,SIGNAL(SendMessage(QString,QString)),this,SLOT(displayMessage(QString,QString)));
    connect(watek,SIGNAL(watekStop()),this,SLOT(on_Anuluj_clicked()));



    ui->usunWszystko->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
 //   ui->listaDuplikatow->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->listaDuplikatow->addAction(ui->actionOtworz);
    ui->listaDuplikatow->addAction(ui->actionPrzejdz);
    ui->listaDuplikatow->addAction(ui->actionUsun);


    connect(ui->actionUsun,SIGNAL(triggered()),SLOT(menuListyPlikow()));
    connect(ui->actionPrzejdz,SIGNAL(triggered()),this,SLOT(przejdzFolder()));
    connect(ui->actionOtworz,SIGNAL(triggered()),this,SLOT(openFile()));

    ui->progressBar->setHidden(true);
    ui->Anuluj->setHidden(true);
    ui->statusBar->showMessage(tr("Uruchomiono"),1500);
    //mojaklasa.moveToThread(watek);
    //wczytanie ustawień
    QSettings settings("Qszukacz", "config");

     settings.beginGroup("configuration");
     if (settings.value("sciezka_szukania").toString().isEmpty()==false)
     {
        QString katalog = settings.value("sciezka_szukania").toString();
        ui->listWidget->addItem(katalog);
     }
}

mojaklasa::~mojaklasa()
{
    delete ui;
}
void mojaklasa::resizeEvent(QResizeEvent *)
{
    //zmiana rozmiaru okna
    int x = mojaklasa::size().width();
    int y = mojaklasa::size().height();
    ui->listaDuplikatow->resize(x-309,y-83);
   // ui->listWidget->move(x-307,ui->listWidget->pos().y());
    ui->label->move(x-300,50);
    ui->listWidget->resize(307,y-150);
    ui->listWidget->move(x-310,80);
    ui->pushButton->move(x-300,y-65);
    ui->usun->move(x-255,y-65);
    ui->settings->move(x-40,10);
    ui->progressBar->resize(x-565,23);
   // ui->czescKatalogow->setGeometry(QRect(x_g,y_g,g_w,g_h));

}

void mojaklasa::menuListyPlikow()
{
    int zaznDoUsun = ui->listaDuplikatow->selectedItems().size();
   /* QFile file;

    file.setFileName(ui->listaDuplikatow->selectedItems().first()->text(1));
    if (file.remove()== true)
    {

        QTreeWidgetItem *item = ui->listaDuplikatow->currentItem();
        int i = ui->listaDuplikatow->indexOfTopLevelItem(item);
        ui->listaDuplikatow->takeTopLevelItem(i);
           QMessageBox::information(this,"Usuwanie",QString::number(zaznDoUsun));

    }*/


    if (zaznDoUsun>0)
    {
        QString trescKomunikatu;
        if(zaznDoUsun==1)
            trescKomunikatu = QString::fromUtf8("Usunąć ")+ui->listaDuplikatow->selectedItems().first()->text(1)+" ?";
        else
            trescKomunikatu = QString::fromUtf8("Usunąć wszystkie(")+QString::number(zaznDoUsun)+") pliki?";

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
                    //QMessageBox::information(this,"Usuwanie",QString::number(index));
                    ui->listaDuplikatow->takeTopLevelItem(index);
                    QString cos = QString::number(ui->listaDuplikatow->topLevelItemCount());
                    ui->statusBar->showMessage(cos+QString::fromUtf8(" elementów")) ;

                }
                else
                    if (file.RemoveError)

                        QMessageBox::warning(this,tr("Błąd"),"Nie można usunąć"+file.fileName());

            }
        }


    }
}
void mojaklasa::openFile()
{
    if (ui->listaDuplikatow->selectedItems().count()>0)
    {
           QString sciezka( ui->listaDuplikatow->currentItem()->text(1));

        if (!sciezka.isEmpty())
        {
            QUrl url = QUrl::fromLocalFile(sciezka);
            QDesktopServices::openUrl(url);
        }
    }
}

void mojaklasa::przejdzFolder()
{

    //if (ui->listaDuplikatow->SelectedClicked)
    //QString sciezka( ui->listaDuplikatow->selectedItems().first()->text(1));
    if (ui->listaDuplikatow->selectedItems().count()>0)
    {
        QString sciezka( ui->listaDuplikatow->currentItem()->text(1));
        QStringList list1 = sciezka.split("/");
        QString path;
        for (int i=0; i < list1.count()-1;i++ )
            path.append(list1.at(i)+"/");
        //QString run= program+path;
        if (!path.isEmpty())
        {
            QDesktopServices::openUrl(QUrl("file:///" + path));

        }
    }
}

void mojaklasa::on_pushButton_clicked()
{

    QString katalog = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Otwórz katalog"),
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
*/  ui->listaDuplikatow->clear();
    ui->listaDuplikatow->setContextMenuPolicy(Qt::NoContextMenu);
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

        mojaklasa::wyslijParametry(listaaa);
        watek->start();
        ui->listaDuplikatow->setSortingEnabled(true);
        ui->listaDuplikatow->sortByColumn(1,Qt::AscendingOrder);
        ui->Anuluj->setHidden(false);
        ui->pushButton_2->setDisabled(true);
        ui->usunWszystko->setDisabled(true);
        ui->settings->setDisabled(true);


    }
    else
    {
        QMessageBox::warning(this,QString::fromUtf8("Błąd"),QString::fromUtf8("Lista jest pusta! Dodaj katalog do listy i spróbuj ponownie"));
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
        QMessageBox::information(this,"koniec roboty","Przeszukiwanie Przerwane..");
    ui->statusBar->showMessage(tr("Przerwane.."),1500);
}
void mojaklasa::zakonczSzukanie(QString koniec)
{
   QString cos = koniec.number(ui->listaDuplikatow->topLevelItemCount());
    ui->statusBar->showMessage(koniec,3000);
    ui->Anuluj->setHidden(true);
    ui->settings->setEnabled(true);
    if(ui->listaDuplikatow->topLevelItemCount() > 0)
    {

        ui->pushButton_2->setDisabled(false);
        ui->usunWszystko->setDisabled(false);

        ui->statusBar->showMessage(cos+QString::fromUtf8(" elementów")) ;
        ui->listaDuplikatow->setContextMenuPolicy(Qt::ActionsContextMenu);


    }

}


void mojaklasa::on_settings_clicked()
{
    //Ustawienia *settings_window = new Ustawienia;
    mojaklasa::setEnabled(false);
   // settings_window->show();
    settings_window->exec();
    mojaklasa::setEnabled(true);


}
void mojaklasa::displayMessage(QString title,QString message)

{

    QMessageBox::warning(this,title,message);

}
