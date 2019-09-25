#include "indicateurs.h"
#include "sstream"
#include <iomanip>

using namespace std;

Indicateurs::Indicateurs(MainWindow * p):m_parent(p)
{

}

double Indicateurs::calculerRSI(int db, int fn){

    double rsi;
    int nbgain = 0;
    int nbloss = 0;
    int jours;
    double averagegain = 0;
    double averageloss = 0;
    double o, c;

    for (int it = db; it != fn; ++it) {

        o = m_parent->getParent()->getEvolutionCours()->getCours()[it].getOpen();
        c = m_parent->getParent()->getEvolutionCours()->getCours()[it].getClose();



        if (o>c) {
            nbloss++;
            averageloss+=((o-c)/o)*100;
        }
        else {
            nbgain++;
            averagegain+=((c-o)/o)*100;
        }
    }


    if (nbgain>0) {
        averagegain = averagegain/nbgain;
    }
    if (nbloss>0) {
        averageloss = averageloss/nbloss;
    }

    jours = nbloss+nbgain;

    if (nbloss>0) {
        rsi = 100 - (100/(1+((averagegain/jours)/(averageloss/jours))));
    }
    else {
        rsi = 100 - (100/(1+((averagegain/jours))));
    }

    return rsi;
}

double Indicateurs::calculerEMA(int db, int fn) {

    double c = 0;
    double ema = 0, ancien_ema = 0;
    double sma = 0;

    int db2, fn2;

    db2 = db-(fn-db);
    fn2 = fn-(fn-db);

    for (int it = db2; it != fn2; ++it) {
        sma += m_parent->getParent()->getEvolutionCours()->getCours()[it].getClose();
    }

    sma = sma/(fn-db);

    ancien_ema = sma;


    for (int it = db; it != fn; ++it) {

        c = m_parent->getParent()->getEvolutionCours()->getCours()[it].getClose();
        ema = (c - ancien_ema)*(2/(1+fn-db))+ancien_ema*(1-(2/(1+fn-db)));
        ancien_ema = ema;
    }

    return ema;
}

double Indicateurs::calculerMACD(int d) {

    int d9 = d-9;

    double ancien_signal = calculerEMA(d9-12,d9)-calculerEMA(d9-26,d9);
    double signal = 0;
    double macd;

    //On calcule l'EMA de 9 périodes des MACD

    for (int it = d9+1; it != d; ++it) {
        signal = (calculerEMA(it-12,it)-calculerEMA(it-26,it))*(2/(1+9))+ancien_signal*(1-(2/(1+9)));
        ancien_signal = signal;
    }

    //On calcule le MACD du jour d

    macd = (calculerEMA(d-12,d)-calculerEMA(d-26,d));

    //Puis on retourne la différence entre la courbe signal et celle MACD

    return macd-signal;

}
