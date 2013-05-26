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


private:
    Ui::mojaklasa *ui;
    QAction *rightClickMenu;
    QMenu *menu;

    
private slots:
    void on_pushButton_clicked();

    void on_skanuj_clicked();


    void on_usun_clicked();

    void on_Anuluj_clicked();
    void usunItem();
    //void Menu(const QPoint &pos);

    void przejdzFolder();

    void on_usunWszystko_clicked();

public slots:
   void listowanie(QTreeWidgetItem* item,int znaleziono);
   void onProgress(int p, int max);
   void zakonczSzukanie(QString koniec);
signals:
   void wyslijParametry(bool podKatalog, QList<QString> sciezkiPierwotne);
};

#endif // MOJAKLASA_H
