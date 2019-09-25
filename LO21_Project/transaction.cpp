#include "transaction.h"
#include "includes.h"

//verif expection

void Transaction::calcul_tr(double montant, double broker, double open_price,
                            double close_price, double montant_base, double montant_contrepartie){
    if (type_transaction == "Acheter"){ //ACHAT CONTREPARTIE
        double tmp_contrepartie = total_contrepartie;
        total_contrepartie += montant;
        double tmp_base = total_base; //on retient le montant initiale;
        total_base -= 1/open_price + (1/open_price * broker); // on perd plus que prevu a cause de la taxe
        ROI  = (total_base + total_contrepartie*close_price ) / ((montant_base + montant_contrepartie/montant_base));
        positif = total_contrepartie - tmp_contrepartie;
        negatif = tmp_base - total_base;

    }else if (type_transaction == "Vendre"){ //VENTRE CONTREPARTIE (GAIN BASE)
        double tmp_contrepartie = total_contrepartie;
        total_contrepartie -= montant;
        double tmp_base = total_base; //on retient le montant initiale;
        total_base += montant*open_price - (montant * open_price * broker); // on gagne moins a cause la taxe
        ROI  = (total_base + total_contrepartie*close_price ) / ((montant_base + montant_contrepartie/montant_base));
        positif = tmp_base - total_base;
        negatif = tmp_contrepartie - total_contrepartie ;

    }
}

void EvolutionTransaction::addTransaction(const Transaction &tr){
    tab_transac.push_back(tr);
}

void EvolutionTransaction::deleteLast(){
    tab_transac.pop_back();
}

void EvolutionTransaction::print_tab(){
    for (auto it = tab_transac.begin(); it != tab_transac.end(); ++it){
        QString date = it->getDateTransaction().toString("yyyy-MM-d");
        cout << "Date de la transaction : " << date.toStdString() << ", total devise de contrepartie : " << it->getTotalContrepartie()
             << ", total devise de base : " << it->getTotalBase() << ", type de la transaction : " << it->getTypeTransaction().toStdString() <<
                ", le ROI est : " << it->getROI() << endl;
    }
}







