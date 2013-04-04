#ifndef MOJAKLASA_H
#define MOJAKLASA_H

#include <QMainWindow>
#include <QDir>
#include <QTreeWidgetItem>
#include <QList>

namespace Ui {
class mojaklasa;
}

class mojaklasa : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mojaklasa(QWidget *parent = 0);
    ~mojaklasa();


    
private slots:
    void on_pushButton_clicked();

    void on_skanuj_clicked();

    void listuj(QString nazwa);
    void listuj_kat(QString nazwa);
    void szukaj_duplikatow();


   // void addChildren(QTreeWidgetItem* ,QString);

private:
    Ui::mojaklasa *ui;

    QList<QString> listaSciezek;
    struct szukanie
    {
        QString sciezka;
        QString nazwa;
        int rozmiar;
    };
    QList<szukanie> lista_plikow;

};

#endif // MOJAKLASA_H
