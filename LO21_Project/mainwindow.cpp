#include "mainwindow.h"
#include "evolutionviewer.h"
#include "modepas.h"
#include "automaticmode.h"
#include <QCalendarWidget>



MainWindow::MainWindow(Import * p):QWidget(),parent(p)
{
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Trading Simulator");

    //**BUTTONS**//
    //group
    butt_horizgroup = new QGroupBox(tr("Simulation"));

    retour = new QPushButton("Retour",this);
    retour->setMaximumSize(100,30);

    simul = new QPushButton("Simulation",this);
    simul->setMaximumSize(150,30);
    menu = new QMenu(simul);
    mode_man = new QAction("&Mode Manuel",this);
    mode_auto = new QAction("&Mode Automatique",this);
    mode_pas_pas = new QAction("&Mode Pas-à-Pas",this);
    menu->addAction(mode_man);
    menu->addAction(mode_pas_pas);
    menu->addAction(mode_auto);
    simul->setMenu(menu);

    //**BUTTONS LAYOUTING**//
    h_butt_lay = new QHBoxLayout();
    h_butt_lay->addWidget(retour, Qt::AlignLeft);
    h_butt_lay->addWidget(simul, Qt::AlignRight);
    butt_horizgroup->setLayout(h_butt_lay);

    //**CURRENCIES LAYOUTING**//

    QRegExp dev("[A-Z][A-Z][A-Z]");
    QValidator * validator = new QRegExpValidator(dev, this);

    //form
    curr_form_group = new QGroupBox(tr("Porte-feuille"));
    curr_form_lay = new QFormLayout;

    //horizontal layouting
    base_l = new QLabel("Devise de base",this);

    Embase = new QDoubleSpinBox();
    Embase->setMaximum(9999999999);
    Embase->setValue(0.0);
    h2_curr = new QHBoxLayout();

    Ebase = new QLineEdit();
    Ebase->setFixedSize(75,20);
    Ebase->setPlaceholderText("BAS");
    Ebase->setValidator(validator);
    h1_curr = new QHBoxLayout();

    h1_curr->addWidget(base_l);
    h2_curr->addWidget(Embase);
    h1_curr->addWidget(Ebase);

    contr_l = new QLabel("Devise de contrepartie",this);

    Emctr = new QDoubleSpinBox();
    Emctr->setMaximum(9999999999);
    Emctr->setValue(1000000.00);
    h4_curr = new QHBoxLayout();

    Ectr = new QLineEdit();
    Ectr->setFixedSize(75,20);
    Ectr->setPlaceholderText("CTR");
    Ectr->setValidator(validator);
    h3_curr = new QHBoxLayout();

    h3_curr->addWidget(contr_l);
    h4_curr->addWidget(Emctr);
    h3_curr->addWidget(Ectr);

    curr_form_lay->addRow(h1_curr);
    curr_form_lay->addRow(h2_curr);
    curr_form_lay->addRow(h3_curr);
    curr_form_lay->addRow(h4_curr);
    curr_form_group->setLayout(curr_form_lay);

    //**TAXE**//
    taxe_form_group = new QGroupBox(tr("Transactions"));
    taxe_form_lay = new QFormLayout;

    broker = new QLabel("Taxe",this);
    Ebroker = new QDoubleSpinBox();
    Ebroker->setValue(0.1);
    Ebroker->setMaximum(100);
    Ebroker->setSuffix("%");
    h5_curr = new QHBoxLayout();
    h5_curr->addWidget(broker);
    h5_curr->addWidget(Ebroker);

    taxe_form_lay->addRow(h5_curr);
    taxe_form_group->setLayout(taxe_form_lay);

    //**SETTINGS LAYOUTING**//
    date_form_group = new QGroupBox(tr("Calendrier"));
    date_form_lay = new QGridLayout();

    //horizontal layouting

    QDate dt_db = parent->getEvolutionCours()->getCours()[1].getDate(); //premiere date du fichier
    unsigned int nbcours = parent->getEvolutionCours()->getNbCours();
    QDate dt_fn = parent->getEvolutionCours()->getCours()[nbcours-1].getDate(); //derniere date du fichier (-1 car la case 1 contient les titres du fichier)

    bg_date = new QLabel("Début de simulation",this);
    Dbg_date = new QCalendarWidget();
    Dbg_date->setSelectedDate(dt_db);
    Dbg_date->setMinimumDate(dt_db);
    Dbg_date->setMaximumDate(dt_fn);
    date_form_lay->addWidget(bg_date,0,0);
    date_form_lay->addWidget(Dbg_date,1,0);

    end_date = new QLabel("Fin de simulation",this);
    Dend_date = new QCalendarWidget();
    Dend_date->setSelectedDate(dt_fn);
    Dend_date->setMinimumDate(dt_db);
    Dend_date->setMaximumDate(dt_fn);
    date_form_lay->addWidget(end_date,0,1);
    date_form_lay->addWidget(Dend_date,1,1);

    date_form_group->setLayout(date_form_lay);

    //**MAIN LAYOUT**//
    main_lay = new QVBoxLayout();
    main_lay->addWidget(date_form_group);
    main_lay->addWidget(curr_form_group);
    main_lay->addWidget(taxe_form_group);
    main_lay->addWidget(butt_horizgroup);
    setLayout(main_lay);

    //**CONNECTS**//
    connect(mode_man,SIGNAL(triggered()),this,SLOT(launch_manual()));
    connect(mode_pas_pas,SIGNAL(triggered()),this,SLOT(launch_pas()));
    connect(mode_auto,SIGNAL(triggered()),this,SLOT(launch_auto()));
    connect(retour,SIGNAL(clicked()),this,SLOT(go_back()));

}

void MainWindow::launch_manual(){
    QVector<QStringList> info = parent->getParser()->getInfo();
    bool date = getBeginDate() < getEndDate();
    if(date){
        QString debut = this->getBeginDate().toString("yyyy-MM-dd");
        QString fin = this->getEndDate().toString("yyyy-MM-dd");

        int db = 0; int fn = 0;

        for (auto it = info.begin()+1; it != info.end(); it++){ //la case 0 contient les titres
            if(it->at(0) == debut){
                db = info.indexOf(*it);
            }
            if(it->at(0) == fin){
                fn = info.indexOf(*it)+1;
            }

        }



        EvolutionCours * evol = new EvolutionCours();


        for (int i = db; i < fn; i++){
            evol->addCours(parent->getEvolutionCours()->getCours()[i].getOpen(),parent->getEvolutionCours()->getCours()[i].getHigh(),
                           parent->getEvolutionCours()->getCours()[i].getLow(),parent->getEvolutionCours()->getCours()[i].getClose(),
                           parent->getEvolutionCours()->getCours()[i].getVolume(),
                           parent->getEvolutionCours()->getCours()[i].getDate());
        }

        //tester si ca passe la boucle ou pas

        EvolutionViewer * v =  new EvolutionViewer(*evol, this);
        v->show();

    }else{
        QMessageBox * msg = new QMessageBox();
        msg->setIcon(QMessageBox::Warning);
        msg->setText("La date de fin est antérieur à celle de début.");
        msg->exec();
    }

}

void MainWindow::launch_pas(){
    bool date = getBeginDate() < getEndDate();

    if(date){
        modepas * pas = new modepas(this);
        pas->show();
    }else{
        QMessageBox * msg = new QMessageBox();
        msg->setIcon(QMessageBox::Warning);
        msg->setText("La date de fin est antérieur à celle de début.");
        msg->exec();
    }
}


void MainWindow::launch_auto(){
    bool date = getBeginDate() < getEndDate();

    if(date){
        Automaticmode * a_mod = new Automaticmode(this);
        a_mod->show();
    }else{
        QMessageBox * msg = new QMessageBox();
        msg->setIcon(QMessageBox::Warning);
        msg->setText("La date de fin est antérieur à celle de début.");
        msg->exec();
    }
}

void MainWindow::go_back(){
    this->close();
    parent->show();
}
