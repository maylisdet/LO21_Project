#include "transaction.h"
#include "transactionviewer.h"
#include "includes.h"





tableau_t::tableau_t():QWidget(){

    // Propriétés fenêtre

    this->setWindowTitle("Transactions effectuées");


    // Attributs
    model = new QStandardItemModel(1,5,this);
    table = new QTableView(this);
    table->setFixedSize(720,300);
    table->setModel(model);


    // Propriétés tableau
    model->setHeaderData( 0, Qt::Horizontal, "Date" );
    model->setHeaderData( 1, Qt::Horizontal, "Montant devise de cotation" );
    model->setHeaderData( 2, Qt::Horizontal, "Montant devise de base" );
    model->setHeaderData( 3, Qt::Horizontal, "Ratio" );
    model->setHeaderData( 4, Qt::Horizontal, "Type" );

    table->setColumnWidth(0,80);
    table->setColumnWidth(1,200);
    table->setColumnWidth(2,180);
    table->setColumnWidth(3,80);
    table->setColumnWidth(4,180);

}


void tableau_t::affichage(){

    for(int i=0;i<tab.getTabTr().size();i++){

        // Récupération des valeurs des attributs de la transaction actuelle
        QDate date = tab.getTabTr()[i].getDateTransaction();
        QString cotation;
        QString base;

        if(tab.getTabTr()[i].getTypeTransaction() == "Acheter"){
            cotation = QString::number(tab.getTabTr()[i].getTotalContrepartie()) + " (+" + QString::number(tab.getTabTr()[i].getPositif()) + ")";
            base = QString::number(tab.getTabTr()[i].getTotalBase()) + " (-" + QString::number(tab.getTabTr()[i].getNegatif()) + ")";
        }
        else if(tab.getTabTr()[i].getTypeTransaction() == "Vendre"){
            cotation = QString::number(tab.getTabTr()[i].getTotalContrepartie()) + " (-" + QString::number(tab.getTabTr()[i].getNegatif()) + ")";
            base = QString::number(tab.getTabTr()[i].getTotalBase()) + " (+" + QString::number(tab.getTabTr()[i].getPositif()) + ")";
        }

        QString ratio = QString::number(tab.getTabTr()[i].getROI());
        QString type = tab.getTabTr()[i].getTypeTransaction();

        // Placement des valeurs dans la ligne du tableau
        QStandardItem *d = new QStandardItem;
        QString dt = date.toString("dd.MM.yyyy");
        d->setText(dt);
        model->setItem(static_cast<int>(i), 0, d);

        QStandardItem *c = new QStandardItem;
        c->setText(cotation);
        model->setItem(static_cast<int>(i), 1, c);

        QStandardItem *b = new QStandardItem;
        b->setText(base);
        model->setItem(static_cast<int>(i), 2, b);

        QStandardItem *r = new QStandardItem;
        r->setText(ratio);
        model->setItem(static_cast<int>(i), 3, r);

        QStandardItem *z = new QStandardItem;
        z->setText(type);
        model->setItem(static_cast<int>(i), 4, z);

        this->show();

    }
}
