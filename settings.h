#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mojaklasa.h"

namespace Ui {
class Ustawienia;
}

class Ustawienia : public QDialog
{
    Q_OBJECT
    
public:
    explicit Ustawienia(QDialog *parent = 0);
    ~Ustawienia();
    
private:
    Ui::Ustawienia *ui;
    void writeSettings();

signals:
   // void glowne_okno_pokaz(bool);
private slots:

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_lista_plikow_toggled(bool checked);
    void on_sciezka_zapisz_przycisk_clicked();
    void on_sciezka_szuk_przycisk_clicked();
    void on_pushButton_clicked();
    void WczytajUstawienia();

public slots:
    void PrzywrocDomyslne();
};

#endif // DIALOG_H








