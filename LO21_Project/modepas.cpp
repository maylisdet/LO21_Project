#include "modepas.h"
#include "evolutionviewer.h"

modepas::modepas(MainWindow *q) : QWidget (), mw_parent(q)
{
    setAttribute(Qt::WA_DeleteOnClose);
    
    total_base = q->getSoldeBase();
    this->setFixedSize(800,500);
    setWindowTitle("Mode pas à pas");

    etat=nullptr;
    evol = new EvolutionTransaction();
    QDate db=q->getBeginDate();

    date = new QLabel("Date actuelle :", this);
    date_a_afficher = new QLabel(db.toString("dd/MM/yyyy"),this);
    open = new QLabel("Prix d'ouverture :",this);

    QVector<QStringList> info = q->parent->getParser()->getInfo();
    auto it = info.begin()+1;
    index= info.indexOf(*it);

    //Attention : Il faut entrer des dates qui existent dans le csv dans la fenêtre de choix de date de début
    while(q->parent->getEvolutionCours()->getCours()[index].getDate().toString("dd/MM/yyyy")!=db.toString("dd/MM/yyyy")){ //on atteint la date choisie
        index++;
    }

    open_a_afficher = new QLabel(QString::number(q->parent->getEvolutionCours()->getCours()[index].getOpen()),this);


    datec = new QHBoxLayout;
    datec->addWidget(date,0, Qt::AlignRight);
    datec->addWidget(date_a_afficher,0,Qt::AlignLeft);

    openc = new QHBoxLayout;
    openc->addWidget(open, 0, Qt::AlignRight);
    openc->addWidget(open_a_afficher, 0, Qt::AlignLeft);

    annuler = new QPushButton("Annuler dernière transaction", this);
    derniere_trans = new QLabel("Derniere transaction : ");
    derniere_date = new QLabel("Aucune dernière transaction", this);
    derniere_layout = new QHBoxLayout;

    derniere_layout->addWidget(derniere_trans,0, Qt::AlignRight);
    derniere_layout->addWidget(derniere_date,0, Qt::AlignLeft);
    derniere_layout->addWidget(annuler,0, Qt::AlignCenter);

    montant = new QDoubleSpinBox(this);
    montant->setRange(0,1000000);
    symbole = new QLabel(q->getCodeBase(),this);

    montantc = new QHBoxLayout;
    montantc->addWidget(montant,0, Qt::AlignRight);
    montantc->addWidget(symbole,0,Qt::AlignLeft);



    fenetre = new QVBoxLayout;
    fenetre->addLayout(datec);
    fenetre->addLayout(openc);
    fenetre->addLayout(derniere_layout);
    fenetre->addLayout(montantc);



    acheterb = new QPushButton("Acheter", this);
    vendreb = new QPushButton("Vendre", this);
    afficher = new QPushButton("Afficher jours précédents", this);
    joursuivant = new QPushButton("Passer au jour suivant", this);
    retour = new QPushButton("Retour",this);
    boutons = new QHBoxLayout;

    boutons->addWidget(acheterb);
    boutons->addWidget(vendreb);
    boutons->addWidget(afficher);
    boutons->addWidget(joursuivant);
    boutons->addWidget(retour);

    fenetre->addLayout(boutons);

    setLayout(fenetre);


    connect(retour,SIGNAL(clicked()),this,SLOT(close()));
    connect(acheterb,SIGNAL(clicked()),this,SLOT(acheter()));
    connect(vendreb,SIGNAL(clicked()),this,SLOT(vendre()));
    connect(joursuivant,SIGNAL(clicked()),this,SLOT(jour_suivant()));
    connect(afficher,SIGNAL(clicked()),this,SLOT(afficher_preced()));
    connect(annuler,SIGNAL(clicked()),this,SLOT(annuler_trans()));
}

void modepas::acheter(){
    if(getMontant()<=0){
        QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Le montant entré est invalide");
        msg->exec();
    }
    else
    {
        if(mw_parent->getSoldeContrepartie() < getMontant()){
            QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
            msg->setIcon(QMessageBox::Critical);
            msg->setText("Vous ne disposez pas assez de devises de contrepartie pour effectuer cet achat.");
            msg->exec();
        }
        else
        {
            if(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate()> mw_parent->getEndDate()){
                QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
                msg->setIcon(QMessageBox::Critical);
                msg->setText("La date limite de fin a été atteinte");
                msg->exec();
            }
            else
            {
                if(etat!=nullptr){
                    delete etat;
                }
                if(evol->getTabTr().isEmpty()){
                    Transaction * tr = new Transaction(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate(), mw_parent->getSoldeContrepartie(), mw_parent->getSoldeBase(),0, acheterb->text());
                    tr->calcul_tr(montant->value(),mw_parent->getBroker(),mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen(),mw_parent->parent->getEvolutionCours()->getCours()[index].getClose(),
                                  mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
                    evol->addTransaction(*tr);
                    etat = new memento(mw_parent->getSoldeBase(), mw_parent->getSoldeContrepartie(),tr->getROI(), tr->getDateTransaction(), index);       //on stocke cette transaction comme étant la dernière pour pouvoir l'annuler si besoin
                }else{
                    Transaction * tr = new Transaction(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate(),evol->getTabTr().last().getTotalContrepartie(),evol->getTabTr().last().getTotalBase(),0,acheterb->text());
                    tr->calcul_tr(montant->value(),mw_parent->getBroker(),mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen(),mw_parent->parent->getEvolutionCours()->getCours()[index].getClose(),
                                  mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
                    evol->addTransaction(*tr);
                    etat = new memento(mw_parent->getSoldeBase(), mw_parent->getSoldeContrepartie(),tr->getROI(), tr->getDateTransaction(), index);       //on stocke cette transaction comme étant la dernière pour pouvoir l'annuler si besoin
                }

                QString last_trans = mw_parent->parent->getEvolutionCours()->getCours()[index].getDate().toString("dd/MM/yyyy") + " Achat de " + QString::number(getMontant()) + " " +mw_parent->getCodeBase();
                derniere_date->setText(last_trans); //update de la dernière transaction
                index++; //on passe au jour suivant

                mw_parent->Embase->setValue(mw_parent->getSoldeBase() + getMontant());
                mw_parent->Emctr->setValue(mw_parent->getSoldeContrepartie() - getMontant());
                date_a_afficher->setText(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate().toString("dd/MM/yyyy"));
                open_a_afficher->setText(QString::number(mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen()));
            }
        }
    }
}

void modepas::vendre(){
    if(getMontant()<=0){
        QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Le montant entré est invalide");
        msg->exec();
    }
    else
    {
        if(this->mw_parent->getSoldeBase() < this->getMontant()){
            QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
            msg->setIcon(QMessageBox::Critical);
            msg->setText("Vous ne disposez pas assez de devises de base pour effectuer cette vente.");
            msg->exec();
        }
        else
        {
            if(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate()> mw_parent->getEndDate()){
                QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
                msg->setIcon(QMessageBox::Critical);
                msg->setText("La date limite de fin a été atteinte");
                msg->exec();
            }
            else
            {

                if(etat!=nullptr){
                    delete etat;
                }
                if(evol->getTabTr().isEmpty()){
                    Transaction * tr = new Transaction(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate(), mw_parent->getSoldeContrepartie(), mw_parent->getSoldeBase(),0, vendreb->text());
                    tr->calcul_tr(montant->value(),mw_parent->getBroker(),mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen(),mw_parent->parent->getEvolutionCours()->getCours()[index].getClose(),
                                  mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
                    evol->addTransaction(*tr);
                    etat = new memento(mw_parent->getSoldeBase(), mw_parent->getSoldeContrepartie(),tr->getROI(), tr->getDateTransaction(), index);       //on stocke cette transaction comme étant la dernière pour pouvoir l'annuler si besoin
                }else{
                    Transaction * tr = new Transaction(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate(),evol->getTabTr().last().getTotalContrepartie(),evol->getTabTr().last().getTotalBase(),0,vendreb->text());
                    tr->calcul_tr(montant->value(),mw_parent->getBroker(),mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen(),mw_parent->parent->getEvolutionCours()->getCours()[index].getClose(),
                                  mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
                    evol->addTransaction(*tr);
                    etat = new memento(mw_parent->getSoldeBase(), mw_parent->getSoldeContrepartie(),tr->getROI(), tr->getDateTransaction(), index);       //on stocke cette transaction comme étant la dernière pour pouvoir l'annuler si besoin
                }

                QString last_trans =etat->getDate().toString("dd/MM/yyyy") + " Vente de " + QString::number(getMontant()) + " " +mw_parent->getCodeBase();
                derniere_date->setText(last_trans);//update de la dernière transaction
                index++; //on passe au jour suivant

                mw_parent->Embase->setValue(mw_parent->getSoldeBase()-getMontant());
                mw_parent->Emctr->setValue(mw_parent->getSoldeContrepartie()+getMontant());
                date_a_afficher->setText(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate().toString("dd/MM/yyyy"));
                open_a_afficher->setText(QString::number(mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen()));
            }
        }
    }
}

void modepas::afficher_preced(){
    EvolutionCours * evol = new EvolutionCours();

    for(int j = index-7;j<index;j++){ // on affiche sur une semaine
        if(mw_parent->parent->getEvolutionCours()->getCours()[j].getDate() >= mw_parent->parent->getEvolutionCours()->getCours()[0].getDate()) //on vérifie qu'on n'affiche pas les données antérieures à celles du CSV
        {
            evol->addCours(mw_parent->parent->getEvolutionCours()->getCours()[j].getOpen(),
                           mw_parent->parent->getEvolutionCours()->getCours()[j].getHigh(),
                           mw_parent->parent->getEvolutionCours()->getCours()[j].getLow(),
                           mw_parent->parent->getEvolutionCours()->getCours()[j].getClose(),
                           mw_parent->parent->getEvolutionCours()->getCours()[j].getVolume(),
                           mw_parent->parent->getEvolutionCours()->getCours()[j].getDate());
        }
    }
    EvolutionViewer * v = new EvolutionViewer(*evol, this);
    v->show();
}

void modepas::jour_suivant(){
    if(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate()==mw_parent->getEndDate()){
        QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
        msg->setIcon(QMessageBox::Critical);
        msg->setText("La date limite de fin a été atteinte");
        msg->exec();
    }
    else
    {
        index++;
        date_a_afficher->setText(mw_parent->parent->getEvolutionCours()->getCours()[index].getDate().toString("dd/MM/yyyy"));
        open_a_afficher->setText(QString::number(mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen()));
    }
}

void modepas::annuler_trans(){
    if(etat==nullptr){
        QMessageBox * msg = new QMessageBox(); //msg d'erreur avec bouton OK par default
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Il n'y a pas de transaction à annuler.");
        msg->exec();
    }
    else
    {
        evol->deleteLast();
        index=etat->getIndex();
        date_a_afficher->setText(etat->getDate().toString("dd/MM/yyyy"));
        open_a_afficher->setText(QString::number(mw_parent->parent->getEvolutionCours()->getCours()[index].getOpen()));
        mw_parent->Embase->setValue(etat->getMBase());
        mw_parent->Emctr->setValue(etat->getMCtr());
        derniere_date->setText("Aucune dernière transaction");
        delete etat;
        etat = nullptr;
    }
}
