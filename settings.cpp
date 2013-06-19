#include "settings.h"
#include "ui_settings.h"
#include <QFileDialog>
#include <QMessageBox>

Ustawienia::Ustawienia(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Ustawienia)
{
    ui->setupUi(this);
   WczytajUstawienia();

   ui->buttonBox->button(QDialogButtonBox::SaveAll)->setText(tr("Zapisz wszystko"));
   ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Anuluj"));


   //->setText(tr("Zapisz"));

}

Ustawienia::~Ustawienia()
{

   // emit glowne_okno_pokaz(true);
    delete ui;
}
void Ustawienia::WczytajUstawienia()
{
    QSettings settings("Qszukacz", "config");

     settings.beginGroup("configuration");

    ui->szuk_w_podk->setChecked(settings.value("szuk_w_podk").toBool());

    if (settings.value("lista_plikow")==true && settings.value("sciezka_zapisu_listy").toString().isEmpty()==false)
    {
        ui->line_sciezk_zapis->setText(settings.value("sciezka_zapisu_listy").toString());
        ui->lista_plikow->setChecked(settings.value("lista_plikow").toBool());
     }
    else ui->lista_plikow->setChecked(false);
     ui->rozmiar->setChecked( settings.value("rozmiar").toBool());
     ui->nazwa->setChecked( settings.value("nazwa").toBool());
     if (settings.value("sciezka_szukania").toString().isEmpty()==false)
        ui->line_sciezk_szuk->setText(settings.value("sciezka_szukania").toString());

     settings.endGroup();
}
void Ustawienia::on_buttonBox_accepted()
{
    Ustawienia::writeSettings();
   // emit glowne_okno_pokaz(true);
}
void Ustawienia::on_buttonBox_rejected()
{
  //  emit glowne_okno_pokaz(true);
}
void Ustawienia::PrzywrocDomyslne()
{

    ui->lista_plikow->setChecked(false);
    ui->nazwa->setChecked(true);
    ui->rozmiar->setChecked(true);
    ui->szuk_w_podk->setChecked(true);
    ui->line_sciezk_szuk->setText("");
    ui->line_sciezk_zapis->setText("");
    QMessageBox::information(this,QString::fromUtf8("QSzukacz - Ustawienia"),QString::fromUtf8("Przywrócono Ustawienia Domyślne"));
     //settings.endGroup();
}

void Ustawienia::on_lista_plikow_toggled(bool checked)
{
    ui->line_sciezk_zapis->setEnabled(checked);
    ui->l_sciezk_zapis->setEnabled(checked);
    ui->sciezka_zapisz_przycisk->setEnabled(checked);
}
void Ustawienia::on_pushButton_clicked()
{
    PrzywrocDomyslne();
    writeSettings();
   // emit glowne_okno_pokaz(true);
    Ustawienia::close();
  //  WczytajUstawienia();
}
void Ustawienia::on_sciezka_zapisz_przycisk_clicked()
{
   /* QString katalog = QFileDialog::getExistingDirectory(this, tr("Otwórz katalog"),
                                                      "/home",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);*/
    QString katalog = QFileDialog::getSaveFileName(
            this,
            QString::fromUtf8("Zapisz listę plików"),
                QString::fromUtf8("lista plików"),
            tr("Dokument tekstowy (*.txt)") );
    ui->line_sciezk_zapis->setText(katalog);
}


void Ustawienia::on_sciezka_szuk_przycisk_clicked()
{

        QString katalog = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Otwórz katalog"),
                                                          "/home",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);
        ui->line_sciezk_szuk->setText(katalog);

}
void Ustawienia::writeSettings()
 {
    if (ui->rozmiar->isChecked()==false && ui->nazwa->isChecked()==false)
    {

        QMessageBox::warning(this,QString::fromUtf8("Błąd - Zapis nie udany"),QString::fromUtf8("Wybierz jakieś kryterium wyszukiwania!"));
        return;
    }
    else
    {
        QSettings settings("Qszukacz", "config");

         settings.beginGroup("configuration");



         if (ui->lista_plikow->isChecked())
          {
             if (ui->line_sciezk_zapis->text().isEmpty()==false)
             {
                settings.setValue("lista_plikow",ui->lista_plikow->isChecked());
                settings.setValue("sciezka_zapisu_listy",ui->line_sciezk_zapis->text());
             }
             else
             {
                 settings.setValue("lista_plikow",false);
                 ui->lista_plikow->setChecked(false);
                 QMessageBox::warning(this,QString::fromUtf8("Błąd"),QString::fromUtf8("Nie wybrano ścieżki zapisu"));

             }
          }
         settings.setValue("szuk_w_podk",ui->szuk_w_podk->isChecked());
         settings.setValue("sciezka_szukania",ui->line_sciezk_szuk->text());
         settings.setValue("rozmiar",ui->rozmiar->isChecked());
         settings.setValue("nazwa",ui->nazwa->isChecked());

         settings.endGroup();
    }

     //settings.setValue("size", size());
     //settings.setValue("pos", pos());
     //settings.setValue("");

 }
