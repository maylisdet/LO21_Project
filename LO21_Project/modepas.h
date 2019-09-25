#ifndef MODEPAS_H
#define MODEPAS_H

#include "includes.h"
#include "csvparser.h"
#include "mainwindow.h"
#include "transaction.h"
#include "transactionviewer.h"

class memento;

class modepas : public QWidget
{
    Q_OBJECT
private:

    QLabel *date;
    QLabel *date_a_afficher;
    QLabel * open;
    QLabel * open_a_afficher;

    QHBoxLayout * datec;
    QHBoxLayout * openc;

    QDoubleSpinBox * montant;
    QLabel * symbole;

    QHBoxLayout * montantc;

    QPushButton *annuler;
    QLabel *derniere_trans;
    QLabel *derniere_date;
    QHBoxLayout * derniere_layout;

    QPushButton * acheterb;
    QPushButton * vendreb;
    QPushButton * afficher;
    QPushButton * joursuivant;
    QPushButton *retour;

    QHBoxLayout * boutons;


    MainWindow *mw_parent;

    memento * etat;

    QVBoxLayout * fenetre;

    EvolutionTransaction * evol;
    int index; //correspond au jour courant

    double total_base;
    friend class Transaction;
    friend class MainWindow;

public:
    modepas(MainWindow * parent=nullptr);
    //QDate getDateDebut() const {return debut->date();}
    double getMontant()const{return montant->value();}
    EvolutionTransaction * getEvolTr() const {return evol;}


public slots:
    void acheter();
    void vendre();
    void afficher_preced();
    void jour_suivant();
    void annuler_trans();
};

class memento{
private:
    double base;
    double ctr;
    double ROI;
    QDate date;
    int index;
public:
    memento(double b, double c, double r, QDate d, int i) : base(b), ctr(c), ROI(r), date(d), index(i){}
    double getMBase() const {return base;}
    double getMCtr() const {return ctr;}
    double getROI() const {return ROI;}
    QDate getDate() const {return date;}
    int getIndex() const {return index;}
};
#endif // MODEPAS_H
