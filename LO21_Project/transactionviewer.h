#ifndef TRANSACTIONVIEWER_H
#define TRANSACTIONVIEWER_H

#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "transaction.h"
#include <QWidget>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QLabel>

class tableau_t : public QWidget {

    Q_OBJECT

    QStandardItemModel *model;

    QTableView* table;

    EvolutionTransaction tab;

public:
    tableau_t();
    void setVecteur(EvolutionTransaction vect){ tab = vect; }
    EvolutionTransaction getVecteur(){ return tab; }
    void affichage();

};






#endif // TRANSACTIONVIEWER_H
