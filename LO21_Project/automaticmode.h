#ifndef AUTOMATICMODE_H
#define AUTOMATICMODE_H

#include "indicateurs.h"
#include "mainwindow.h"
#include "includes.h"
#include "transaction.h"
#include "transactionviewer.h"
#include "editeurtexte.h"



class Automaticmode : public QWidget
{
    Q_OBJECT
    MainWindow * parent = nullptr;
    QLabel * choix_strat;
    QLabel * mont;
    QDoubleSpinBox *mt_spin;
    QComboBox *strat_box;
    EvolutionTransaction tab_tr;
    QVBoxLayout * v_lay;
    QPushButton *launch_sim;
    QPushButton * show_log;
    QPushButton * notes;


public slots:
    void strat();
    void log();
    void edit_t();



public:
    Automaticmode(MainWindow * p = nullptr);

};

#endif // AUTOMATICMODE_H
