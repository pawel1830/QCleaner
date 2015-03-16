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
    connect(watek,SIGNAL(wyszukiwanie(QTreeWidgetItem*,int)),this,SLOT(listowanie(QTreeWidgetItem*,int)));
    connect(watek,SIGNAL(progress(int,int)),this,SLOT(onProgress(int,int)));
    connect(watek,SIGNAL(zakonczSzukanie(QString)),this,SLOT(zakonczSzukanie(QString)));
    connect(this,SIGNAL(wyslijParametry(QList<QString>)),watek,SLOT(ustawParametry(QList<QString>)));
    connect(ui->zaznaczWszystko,SIGNAL(clicked()),ui->listaDuplikatow,SLOT(selectAll()));
    connect(watek,SIGNAL(SendMessage(QString,QString)),this,SLOT(displayMessage(QString,QString)));
    connect(watek,SIGNAL(watekStop()),this,SLOT(on_Anuluj_clicked()));

    ui->usunWszystko->setDisabled(true);
    ui->zaznaczWszystko->setDisabled(true);
    ui->listaDuplikatow->addAction(ui->actionOtworz);
    ui->listaDuplikatow->addAction(ui->actionPrzejdz);
    ui->listaDuplikatow->addAction(ui->actionUsun);


    connect(ui->actionUsun,SIGNAL(triggered()),SLOT(menuListyPlikow()));
    connect(ui->actionPrzejdz,SIGNAL(triggered()),this,SLOT(przejdzFolder()));
    connect(ui->actionOtworz,SIGNAL(triggered()),this,SLOT(openFile()));

    ui->progressBar->setHidden(true);
    ui->Anuluj->setHidden(true);
    ui->statusBar->showMessage(tr("Uruchomiono"),1500);

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


void mojaklasa::menuListyPlikow()
{
    int zaznDoUsun = ui->listaDuplikatow->selectedItems().size();



    if (zaznDoUsun>0)
    {
        QString trescKomunikatu;
        if(zaznDoUsun==1)
            trescKomunikatu = QString::fromUtf8("Usunąć ")+ui->listaDuplikatow->selectedItems().first()->text(1)+" ?";
        else
            trescKomunikatu = QString::fromUtf8("Usunąć wszystkie(")+QString::number(zaznDoUsun)+") pliki?";

       QMessageBox msgBox;
       QAbstractButton *myYesButton = msgBox.addButton(trUtf8("Tak"), QMessageBox::YesRole);
       msgBox.setText("Usuwanie");
       msgBox.setInformativeText(trescKomunikatu);
       msgBox.setIcon(QMessageBox::Question);
       msgBox.exec();

        if (msgBox.clickedButton() == myYesButton)
        {
            for(int i=0; i<zaznDoUsun;i++)
            {
                QTreeWidgetItem *item = ui->listaDuplikatow->selectedItems().first();
                QFile file;
                file.setFileName(item->text(1));
                if (file.remove()== true)
                {
                    int index = ui->listaDuplikatow->indexOfTopLevelItem(item);
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
    if (ui->listaDuplikatow->selectedItems().count()>0)
    {
        QString sciezka( ui->listaDuplikatow->currentItem()->text(1));
        QStringList list1 = sciezka.split("/");
        QString path;
        for (int i=0; i < list1.count()-1;i++ )
            path.append(list1.at(i)+"/");
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
    ui->listaDuplikatow->clear();
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
        }

        mojaklasa::wyslijParametry(listaaa);
        watek->start();
        ui->listaDuplikatow->setSortingEnabled(true);
        ui->listaDuplikatow->sortByColumn(1,Qt::AscendingOrder);
        ui->Anuluj->setHidden(false);
        ui->zaznaczWszystko->setDisabled(true);
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
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
}

void mojaklasa::on_Anuluj_clicked()
{


    while (watek->isRunning())
   { watek->terminate(); }
    ui->progressBar->setHidden(true);
    ui->Anuluj->setHidden(true);

    if (watek->isRunning()==false)
        QMessageBox::information(this,"Koniec roboty","Przeszukiwanie przerwane \nprzez użytkownika.");
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
        ui->zaznaczWszystko->setDisabled(false);
        ui->usunWszystko->setDisabled(false);

        ui->statusBar->showMessage(cos+QString::fromUtf8(" elementów")) ;
        ui->listaDuplikatow->setContextMenuPolicy(Qt::ActionsContextMenu);
    }

}


void mojaklasa::on_settings_clicked() //wywołanie okna konfiguracyjnego
{
    mojaklasa::setEnabled(false);
    settings_window->exec();
    mojaklasa::setEnabled(true);


}
void mojaklasa::displayMessage(QString title,QString message)

{
    QMessageBox::warning(this,title,message);
}
