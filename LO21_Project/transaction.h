#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "includes.h"

class Transaction
{
    QDate date_transaction;
    double total_contrepartie;
    double total_base;
    double ROI;
    QString type_transaction;
    double positif = 0;
    double negatif = 0;

public:
    Transaction(){}
    Transaction(const QDate & d, double t_c, double t_b, double roi, QString ty):date_transaction(d),total_contrepartie(t_c),
        total_base(t_b),ROI(roi),type_transaction(ty)
    {

    }


    QDate getDateTransaction()  {return date_transaction;}

    double getTotalContrepartie()  {return total_contrepartie;}

    double getROI()  {return ROI;}

    double getTotalBase()  {return total_base;}

    double getPositif()  {return abs(positif);}

    double getNegatif()  {return abs(negatif);}

    QString getTypeTransaction()  {return type_transaction;}

    void calcul_tr(double montant, double broker, double open_price,
                   double close_price, double montant_base, double montant_contrepartie); //calcul les totaux de la transactions


};




class EvolutionTransaction
{
    QVector<Transaction> tab_transac;
    //    Transaction * tab_transac;
    //    unsigned int nb_transac = 0;
    //    unsigned int nb_max_transac = 0;
public:
    EvolutionTransaction(){}

    QVector<Transaction> getTabTr() const {return tab_transac;}
    void addTransaction(const Transaction & tr);
    void deleteLast();
    void print_tab();

};

#endif // TRANSACTION_H
