#include "mojaklasa.h"
#include "ui_mojaklasa.h"
#include <QString>
#include <QFileDialog>
#include <QList>
#include "thread1.h"
#include <QTreeWidgetItem>


mojaklasa::mojaklasa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mojaklasa)
{
    ui->setupUi(this);
    watek = new thread1;
    connect(watek,SIGNAL(wyszukiwanie(QTreeWidgetItem*)),this,SLOT(listowanie(QTreeWidgetItem*)));
    connect(watek,SIGNAL(progress(int,int)),this,SLOT(onProgress(int,int)));
    connect(this,SIGNAL(wyslijParametry(bool,QList<QString>)),watek,SLOT(ustawParametry(bool,QList<QString>)));
    ui->progressBar->setHidden(true);
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
    listaSciezek.push_back(katalog);
    //ui->lineEdit->setText(katalog);
   // listuj_kat(katalog);
    ui->listWidget->addItems(listaSciezek);
}


void mojaklasa::listowanie(QTreeWidgetItem *item)
{
    ui->listaDuplikatow->addTopLevelItem(item);
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
*/
    ui->progressBar->setHidden(false);
    QList<QString>listaaa;
    for (int i=0; i<ui->listWidget->count();i++)
    {
        listaaa.operator +=(ui->listWidget->item(i)->text());
        //qDebug() << ui->listWidget->item(i)->text();
    }
    mojaklasa::wyslijParametry(ui->checkBox->isChecked(),listaaa);
    watek->start();
    if(watek->isFinished()) qDebug() << "koniec";

}
