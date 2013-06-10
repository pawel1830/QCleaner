#ifndef MOJAKLASA_H
#define MOJAKLASA_H

#include "settings.h"
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
   //Ustawienia *settings_window = new Ustawienia;


private:
    Ui::mojaklasa *ui;
    QAction *rightClickMenu;
    QMenu *menu;


    
private slots:
    void on_pushButton_clicked();

    void on_skanuj_clicked();


    void on_usun_clicked();

   // void on_Anuluj_clicked();
    void menuListyPlikow();
    //void Menu(const QPoint &pos);

    //void on_pushButton_2_clicked();
    void przejdzFolder();

    void on_settings_clicked();

public slots:
    void on_Anuluj_clicked();
   void listowanie(QTreeWidgetItem* item,int znaleziono);
   void onProgress(int p, int max);
   void zakonczSzukanie(QString koniec);
   void displayMessage(QString title,QString message);

signals:
   void wyslijParametry( QList<QString> sciezkiPierwotne);
};

#endif // MOJAKLASA_H
