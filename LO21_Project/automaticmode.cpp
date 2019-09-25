#include "automaticmode.h"

Automaticmode::Automaticmode(MainWindow * p):QWidget(),parent(p)
{
    setAttribute(Qt::WA_DeleteOnClose);
    
    setFixedSize(300,260);
    setWindowTitle("Automatic Mode");
    choix_strat = new QLabel("Choix de stratégie");
    mont = new QLabel("Rentrez le montant voulu",this);
    mt_spin = new QDoubleSpinBox();
    mt_spin->setMaximum(10000000);
    strat_box = new QComboBox(this);
    QStringList list;
    list << "RSI" << "MACD";
    strat_box->addItems(list);
    launch_sim = new QPushButton("Lancer Simulation",this);
    show_log = new QPushButton("Afficher le tableau des transactions",this);
    notes = new QPushButton("Editeur de texte",this);

    v_lay = new QVBoxLayout();
    v_lay->addWidget(choix_strat);
    v_lay->addWidget(strat_box);
    v_lay->addWidget(mont);
    v_lay->addWidget(mt_spin);
    v_lay->addWidget(launch_sim);
    v_lay->addWidget(show_log);
    v_lay->addWidget(notes);
    v_lay->setAlignment(Qt::AlignCenter);
    setLayout(v_lay);

    connect(launch_sim,SIGNAL(clicked()),this,SLOT(strat()));
    connect(show_log,SIGNAL(clicked()),this,SLOT(log()));
    connect(notes,SIGNAL(clicked()),this,SLOT(edit_t()));


}

/*En fonction du choix de l'utilisateur une strategie sera appliquée concernant les transactions
à effectuer*/

void Automaticmode::strat(){
    if(mt_spin->value() == 0.00){ //On choisit d'obligatoirement renseigner un montant
        QMessageBox * msg = new QMessageBox();
        msg->setIcon(QMessageBox::Warning);
        msg->setText("Veuillez indiquer un motant");
        msg->exec();
    }else{
        Indicateurs *id = new Indicateurs(parent); //un objet indicateur est cree
        QVector<QStringList> info = parent->getParent()->getParser()->getInfo();



        QString debut = parent->getBeginDate().toString("yyyy-MM-dd");
        QString fin = parent->getEndDate().toString("yyyy-MM-dd");

        int db = 0; int fn = 0;

        for (auto it = info.begin()+1; it != info.end(); it++){ //la case 0 contient les titres
            if(it->at(0) == debut){
                db = info.indexOf(*it);
            }
            if(it->at(0) == fin){
                fn = info.indexOf(*it)+1;
            }
        }


        if(strat_box->currentText() == "RSI"){
            for (int i = db; i < fn; ++i){
                double rsi = id->calculerRSI(i-14,i);

                if(rsi <= 30){

                    if (tab_tr.getTabTr().isEmpty()){ //premiere transaction
                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          parent->getSoldeContrepartie(),parent->getSoldeBase(),0,"Acheter");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);
                    }else{

                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          tab_tr.getTabTr().last().getTotalContrepartie(),
                                                          tab_tr.getTabTr().last().getTotalBase(),0,"Acheter");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);
                    }



                }else if (rsi >= 70){

                    if (tab_tr.getTabTr().isEmpty()){ //premiere transaction
                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          parent->getSoldeContrepartie(),parent->getSoldeBase(),0,"Vendre");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);


                    }else{

                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          tab_tr.getTabTr().last().getTotalContrepartie(),
                                                          tab_tr.getTabTr().last().getTotalBase(),0,"Vendre");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);
                    }

                }
            }
        }else if (strat_box->currentText() == "MACD"){
            for (int i = db; i < fn; ++i){
                double macd = id->calculerMACD(i);
                if (macd > 0){
                    if (tab_tr.getTabTr().isEmpty()){ //premiere transaction
                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          parent->getSoldeContrepartie(),parent->getSoldeBase(),0,"Acheter");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);
                    }else{

                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          tab_tr.getTabTr().last().getTotalContrepartie(),
                                                          tab_tr.getTabTr().last().getTotalBase(),0,"Acheter");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);
                    }
                }else{
                    if (tab_tr.getTabTr().isEmpty()){ //premiere transaction
                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          parent->getSoldeContrepartie(),parent->getSoldeBase(),0,"Vendre");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);
                    }else{

                        Transaction * t = new Transaction(parent->getParent()->getEvolutionCours()->getCours()[i].getDate(),
                                                          tab_tr.getTabTr().last().getTotalContrepartie(),
                                                          tab_tr.getTabTr().last().getTotalBase(),0,"Vendre");
                        t->calcul_tr(mt_spin->value(),parent->getBroker(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getOpen(),
                                     parent->getParent()->getEvolutionCours()->getCours()[i].getClose(),
                                     parent->getSoldeBase(),parent->getSoldeContrepartie());
                        tab_tr.addTransaction(*t);
                    }

                }
            }


        }

    }


}



void Automaticmode::log(){
    tableau_t * t = new tableau_t();
    if (tab_tr.getTabTr().isEmpty()){
        QMessageBox * msg = new QMessageBox();
        msg->setIcon(QMessageBox::Information);
        msg->setText("Aucune transaction n'a été faite");
        msg->exec();
    }else{
        t->setVecteur(tab_tr);
        t->affichage();
    }
}

void Automaticmode::edit_t(){
    EditeurTexte * edit = new EditeurTexte(this,tab_tr);
    edit->show();
}
