#ifndef MOJAKLASA_H
#define MOJAKLASA_H

#include <QMainWindow>
#include <QDir>
#include <QTreeWidgetItem>
#include <QList>
#include "thread1.h"

namespace Ui {
class mojaklasa;
}

class mojaklasa : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mojaklasa(QWidget *parent = 0);
    ~mojaklasa();
   thread1 *watek;
    //xtern QList<QString> listaSciezek;
    //void skanuj(QList<QString> listaSciezek);
QList<QString> listaSciezek;


private:
    Ui::mojaklasa *ui;

  //  QList<QString> listaSciezek;
/*    struct szukanie
    {
        QString sciezka;
        QString nazwa;
        int rozmiar;
    };*/
   // QList<szukanie> lista_plikow;


    
private slots:
    void on_pushButton_clicked();

    void on_skanuj_clicked();

 //   void listuj(QString nazwa);
//   void listuj_kat(QString nazwa);
//    void szukaj_duplikatow(QList<szukanie> listaPlikow);


   // void addChildren(QTreeWidgetItem* ,QString);
public slots:
   void listowanie(QTreeWidgetItem* item);
   void onProgress(int p, int max);
signals:
   void wyslijParametry(bool podKatalog, QList<QString> sciezkiPierwotne);
};

#endif // MOJAKLASA_H
