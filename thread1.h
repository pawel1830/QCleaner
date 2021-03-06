#ifndef THREAD1_H
#define THREAD1_H
#include <QtCore>
#include <QTreeWidgetItem>



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
        double rozmiar;
    };
    void run();
    bool Stop;
    void tworzListePlikow(QDir dir);
    void szukaj_duplikatow(QList<szukanie> listaPlikow);

    QList<QString>sciezki;
    bool szukaj_w_podk;
    bool szukaj_po_rozmiar;
    bool szukaj_po_nazwa;
    bool czy_zapisac_liste;
    QString zapis_listy;
    QList<szukanie> lista_plikow;

signals:
    void wyszukiwanie(QTreeWidgetItem*,int);
    void progress(int p,int max);
    void zakonczSzukanie(QString);
    void SendMessage(QString title,QString message);
    void watekStop();

public slots:
    void ustawParametry(QList<QString> sciezkiPierwotne);

};
#endif // THREAD1_H

