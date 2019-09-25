#ifndef indicateurs_H
#define indicateurs_H

#include "includes.h"
#include "trading.h"
#include "mainwindow.h"

class Indicateurs
{


    double macd = 0;
    double rsi = 0;
    double ema = 0;

    MainWindow * m_parent;

public:

    Indicateurs(MainWindow * p = nullptr);
    double getRSI() const {return rsi;}
    double getMACD() const {return macd;}
    double getEMA() const {return ema;}
    double calculerRSI(int db, int fn);
    double calculerEMA(int db,int fn);
    double calculerMACD(int d);
};

#endif // indicateurs_H
