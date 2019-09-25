#ifndef EVOLUTIONVIEWER_H
#define EVOLUTIONVIEWER_H


#include "includes.h"
#include "trading.h"
#include "transaction.h"
#include "transactionviewer.h"
#include "mainwindow.h"
#include "modepas.h"
#include "editeurtexte.h"


class Bougie;

class EvolutionViewer : public QWidget{
    Q_OBJECT
    EvolutionCours& evolution;
    EvolutionTransaction tab_transaction;
    friend class Achat_Vente;
    friend class Bougie;

    QCandlestickSeries* series; // un ensemble de bougies
    QChart *chart; // un graphique sur un ensemble de bougies
    QChartView *chartView; // un viewer de graphique

    QBarSeries * bar_series;
    QBarSet * bar_set;
    QChart * bar_chart;
    QChartView * bar_chart_view;

    QLineEdit* open; // barres d’édition
    QLineEdit* high;
    QLineEdit* low;
    QLineEdit* close;
    QLineEdit* forme;
    QLabel* openl; // labels
    QLabel* highl;
    QLabel* lowl;
    QLabel* closel;
    QLabel* formel;
    QPushButton * tab_tr;
    QPushButton * editeur;
    QPushButton * retour;
    QHBoxLayout* copen; // couches de placement
    QHBoxLayout* chigh;
    QHBoxLayout* clow;
    QHBoxLayout* cclose;
    QHBoxLayout* cforme;
    QVBoxLayout* coucheCours;
    QVBoxLayout* fenetre;
    QHBoxLayout* fenetre_pas;
    QVBoxLayout * fenetre_finale;



    MainWindow *MW_parent = nullptr;
    modepas * pas_parent = nullptr;

    void closeEvent(QCloseEvent* x_button); //polymorphisque (overriding) afin de retourner sur l'interface principale


public:
    explicit EvolutionViewer(EvolutionCours& e, MainWindow * parent = nullptr);
    /*Surcharge du constructeur pour le mode pas a pas*/
    explicit EvolutionViewer(EvolutionCours &ev, modepas * mdp = nullptr);
signals:
private slots:
    //Creation dun slot lors de la reception d'un signal clickBougie => MAJ des champs de la partie droite
    void viewCoursOHLC(Bougie * b);
public slots:
    void e_retour();
    void show_trans();
    void editeur_t();
};

//**CLASSE BOUGIE**//

class Bougie : public QCandlestickSet {
    Q_OBJECT
    friend class Achat_Vente;

    CoursOHLC* cours;
    bool double_clicked = 0;
    EvolutionViewer *EV_parent = nullptr;

public:
    Bougie(qreal open, qreal high, qreal low, qreal close,CoursOHLC* c, EvolutionViewer *p = nullptr, qreal
           timestamp = 0.0): QCandlestickSet(open, high, low, close, timestamp, p),cours(c),EV_parent(p){
        connect(this,SIGNAL(clicked()),this, SLOT( viewCoursOHLC()));
        connect(this,SIGNAL(doubleClicked()),this, SLOT(achat_vente()));

    }


    bool getDClicked() const {return double_clicked;}
    void setDClicked(bool value) {double_clicked = value;}
    CoursOHLC& getCoursOHLC() { return *cours; }
    const CoursOHLC& getCoursOHLC() const { return *cours; }
    QString getForme();
    void mouseDoubleClickEvent(QMouseEvent *){ //Double click event
        emit doubleClicked();
    }
signals:
    void clickBougie(Bougie* c);
private slots:
    void viewCoursOHLC(){ emit clickBougie(this); }

public slots:
    void achat_vente();
};

//**CLASSE ACHAT VENTE**//

class Achat_Vente : public QWidget{
    Q_OBJECT


    QLabel * label;
    QDoubleSpinBox * montant;
    QPushButton * achat;
    QPushButton * vente;
    QHBoxLayout * h_lay;
    QVBoxLayout * v_lay;
    Bougie * BG_parent;
    MainWindow * mw_parent;
public:
    Achat_Vente(Bougie * b = nullptr);
public slots:
    void acheter();
    void vendre();

};



#endif // EVOLUTIONVIEWER_H
