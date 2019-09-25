#include "csvparser.h"

CSVParser::CSVParser(const QString & chemin)

/* On lit le fichier ligne par ligne et on
  rempli un QVector de QStringList :
  - la premiere case contient les titres des colonnes
  - les autres cases contiennent les infos OHLCV */
{
    QFile * csv_file = new QFile(chemin);
    if(!csv_file->open(QIODevice::ReadOnly | QIODevice::Text))
        throw CSVParserException("Erreur lors de l'ouverture");

    while(!csv_file->atEnd()){
        QString line = csv_file->readLine();
        QStringList info = line.split(",");
        liste_donnee.push_back(info);
    }
    csv_file->close();

}


QVector<QStringList> CSVParser::getInfo() const{
    return liste_donnee;
}


