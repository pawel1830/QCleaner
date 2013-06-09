#include "settings.h"
#include "ui_settings.h"
#include <QFileDialog>
#include <QMessageBox>

Ustawienia::Ustawienia(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Ustawienia)
{
    ui->setupUi(this);
    QSettings settings("Qszukacz", "config");

     settings.beginGroup("configuration");

    ui->szuk_w_podk->setChecked(settings.value("szuk_w_podk").toBool());
    ui->lista_plikow->setChecked(settings.value("lista_plikow").toBool());
     if (settings.value("lista_plikow")==true)
        ui->line_sciezk_zapis->setText(settings.value("sciezka_zapisu_listy").toString());
     ui->rozmiar->setChecked( settings.value("rozmiar").toBool());
     ui->nazwa->setChecked( settings.value("nazwa").toBool());
     ui->line_sciezk_szuk->setText(settings.value("sciezka_szukania").toString());

     settings.endGroup();
    //sciezki.operator =(sciezkiPierwotne);

}

Ustawienia::~Ustawienia()
{

    emit glowne_okno_pokaz(true);
    delete ui;
}
void Ustawienia::on_buttonBox_accepted()
{
    Ustawienia::writeSettings();
    emit glowne_okno_pokaz(true);
}
void Ustawienia::on_buttonBox_rejected()
{
    emit glowne_okno_pokaz(true);
}

void Ustawienia::on_lista_plikow_toggled(bool checked)
{
    ui->line_sciezk_zapis->setEnabled(checked);
    ui->l_sciezk_zapis->setEnabled(checked);
    ui->sciezka_zapisz_przycisk->setEnabled(checked);
}
void Ustawienia::on_sciezka_zapisz_przycisk_clicked()
{
   /* QString katalog = QFileDialog::getExistingDirectory(this, tr("Otwórz katalog"),
                                                      "/home",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);*/
    QString katalog = QFileDialog::getSaveFileName(
            this,
            tr("Zapisz listę plików"),
                "/home",
            tr("Dokument tekstowy (*.txt)") );
    ui->line_sciezk_zapis->setText(katalog);
}


void Ustawienia::on_sciezka_szuk_przycisk_clicked()
{

        QString katalog = QFileDialog::getExistingDirectory(this, tr("Otwórz katalog"),
                                                          "/home",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);
        ui->line_sciezk_szuk->setText(katalog);

}
void Ustawienia::writeSettings()
 {
    if (ui->rozmiar->isChecked()==false && ui->nazwa->isChecked()==false)
    {

        QMessageBox::warning(this,"Błąd - Zapis nie udany","Wybierz jakieś kryterium wyszukiwania!");
        return;
    }
    else
    {
        QSettings settings("Qszukacz", "config");

         settings.beginGroup("configuration");

         settings.setValue("szuk_w_podk",ui->szuk_w_podk->isChecked());
         settings.setValue("lista_plikow",ui->lista_plikow->isChecked());
         settings.setValue("sciezka_zapisu_listy",ui->line_sciezk_zapis->text());
         settings.setValue("sciezka_szukania",ui->line_sciezk_szuk->text());
         settings.setValue("rozmiar",ui->rozmiar->isChecked());
         settings.setValue("nazwa",ui->nazwa->isChecked());

         settings.endGroup();
    }

     //settings.setValue("size", size());
     //settings.setValue("pos", pos());
     //settings.setValue("");

 }
