#ifndef CSVPARSER_H
#define CSVPARSER_H
#include "includes.h"

class CSVParserException{
    QString m_info;

public:
    CSVParserException(const QString & info ):m_info(info){}
    QString get_info()const {return m_info;}
};

class CSVParser{

    QVector<QStringList> liste_donnee;


public:
    CSVParser(const QString & chemin);
    QVector<QStringList> getInfo() const;



};

#endif // CSVPARSER_H
