#ifndef THREAD1_H
#define THREAD1_H
#include <QtCore>
#include <QTreeWidgetItem>
//#include "ui_mojaklasa.h"
//#include "mojaklasa.h"


class thread1 : public
    QThread
{
    Q_OBJECT
public:
    //thread1();
    explicit thread1(QObject *parent = 0);
    struct szukanie
    {
        QString sciezka;
        QString nazwa;
        int rozmiar;
    };
    QList<szukanie> szukanieFaza1(
            //*QList<QString> sciezkiKatalogow,
             bool podkatalog);

    void run();
    bool Stop;
    void listuj_kat(QString nazwa);
    QList<szukanie> listuj(QString nazwa);

    void szukaj_duplikatow(QList<szukanie> listaPlikow);

    QList<QString>sciezki;
    bool szukaj_w_podk;

signals:
    void wyszukiwanie(QTreeWidgetItem*);
    void progress(int p,int max);
public slots:
    void ustawParametry(bool podKatalog, QList<QString> sciezkiPierwotne);
    //QList<szukanie> szukanieFaza1(QList<QString> sciezkiKatalogow, bool podkatalog);
};
#endif // THREAD1_H

