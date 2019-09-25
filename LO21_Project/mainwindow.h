#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "import.h"
#include "trading.h"



class MainWindow : public QWidget
{
    Q_OBJECT
    //**BUTTON**//
    QPushButton * retour;
    QAction * mode_man;
    QAction * mode_auto;
    QAction * mode_pas_pas;
    QPushButton * simul;
    QMenu * menu;

    //**LABEL&EDITS**//

    //for dates
    QLabel * bg_date;
    QLabel * end_date;
    QCalendarWidget * Dbg_date;
    QCalendarWidget * Dend_date;

    //for currencies
    QValidator * validator;
    QLabel * base_l;
    QLabel * contr_l;
    QLabel * broker;
    QLabel * montant_b;
    QLabel * montant_c;
    QLineEdit * Ebase;
    QLineEdit * Ectr;
    QDoubleSpinBox * Ebroker;
    QDoubleSpinBox * Embase;
    QDoubleSpinBox * Emctr;

    //**GROUPBOXS**//
    QGroupBox * date_form_group;
    QGroupBox * curr_form_group;
    QGroupBox * taxe_form_group;
    QGroupBox * butt_horizgroup;


    //**LAYOUT**//
    //button layout
    QHBoxLayout * h_butt_lay;

    //currencies layout
    QHBoxLayout * h1_curr;
    QHBoxLayout * h2_curr;
    QHBoxLayout * h3_curr;
    QHBoxLayout * h4_curr;
    QHBoxLayout * h5_curr;

    //dates
    QHBoxLayout * h1_date;
    QHBoxLayout * h2_date;

    //forms
    QFormLayout * curr_form_lay;
    QFormLayout * taxe_form_lay;

    //grids
    QGridLayout * date_form_lay;

    //main layout
    QVBoxLayout * main_lay;


    Import * parent = nullptr; //widget parent

    friend class modepas;
    friend class Transaction;


public:
    MainWindow(Import * p);
    QDate getBeginDate() {return Dbg_date->selectedDate();}
    QDate getEndDate() {return Dend_date->selectedDate();}
    double getSoldeContrepartie() const {return Emctr->value();}
    double getSoldeBase() const {return Embase->value();}
    double getBroker()const {return Ebroker->value();}
    Import * getParent() const {return parent;}
    QString getCodeBase(){return Ebase->text();}


public slots:
    void launch_manual();
    void launch_pas();
    void launch_auto();
    void go_back();
};

#endif // MAINWINDOW_H
