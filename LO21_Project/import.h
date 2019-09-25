#ifndef IMPORT_H
#define IMPORT_H

#include <QDebug>
#include "includes.h"
#include "trading.h"
#include "csvparser.h"



class Import : public QWidget
{
private:
    Q_OBJECT

    QVBoxLayout * main_lay;

    /**BOUTON**/
    QPushButton * load_csv;
    QPushButton * load_xml;
    QPushButton * load_man;

    /**LABEL**/
    QLabel * welcome1;
    QLabel * welcome2;

    /**GROUPS**/
    QGroupBox * man_form_group;
    QGroupBox * csv_form_group;
    QGroupBox * xml_form_group;
    QFormLayout * csv_form_lay;
    QFormLayout * xml_form_lay;
    QFormLayout * man_form_lay;

    /**STOCKAGE DONNEE**/
    EvolutionCours * evolution = nullptr;
    CSVParser * parser;

    /**LAYOUT**/
    QHBoxLayout * h_lay;
    QVBoxLayout * v_lay;
    QVBoxLayout * f_lay;

public:
    Import();
    CSVParser * getParser() const {return parser;}
    EvolutionCours * getEvolutionCours() const {return evolution;}

public slots:
    void load_csv_file();
    void load_xml_file();
    void load_man_file();

};

#endif // IMPORT_H
