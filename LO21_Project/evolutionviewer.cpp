#include "evolutionviewer.h"

// Attnetion: toujours app le constructeur de la classe mere

EvolutionViewer::EvolutionViewer(EvolutionCours &e, MainWindow * q) : QWidget(),evolution(e), MW_parent(q)
{
    
    setAttribute(Qt::WA_DeleteOnClose);

    this->setFixedSize(1100,700);
    bar_chart = new QChart();

    // Série de bougies (verte/rouge)
    series = new QCandlestickSeries(this);
    series ->setIncreasingColor(QColor(Qt::green));
    series -> setDecreasingColor(QColor(Qt::red));



    QStringList categories; //Contient l'info a afficher sur l'axe des X pour chaque bougie creee
    QStringList bar_categories; //info pour l'axe


    //Parcourir la liste des CoursOHLC
    for (EvolutionCours::iterator it= evolution.begin();it != evolution.end(); ++it) {
        // Pour chaque CoursOHLC
        CoursOHLC & cours = *it;
        //Creer une bougie => QCandleStickSet
        Bougie * bougie = new Bougie(cours.getOpen(), cours.getHigh(),cours.getLow(),cours.getClose(),&cours,this);
        //Ajouter la bougie à la série de bougie
        series->append(bougie);
        bar_set = new QBarSet(cours.getDate().toString("dd.MM.yy")); //creer un QBarSet avec un QString, la date
        bar_set->append(cours.getVolume()); //rajoute l'information (ici volume)
        bar_series = new QBarSeries(this); //creer la serie de barre (ici il y aura une barre par serie) au widget parent
        bar_series->append(bar_set); //rajoute le barset a la serie
        bar_chart -> addSeries(bar_series); //rajoute la serie (ici une serie = 1 barre)
        bar_series = nullptr; //refais pointer bar_series sur nullptr
        categories << cours.getDate().toString("dd.MM.yy");
        bar_categories << cours.getDate().toString("dd.MM.yy");
        connect(bougie, SIGNAL(clickBougie(Bougie*)),this,SLOT(viewCoursOHLC(Bougie*)));
    }

    //Fait la correspondance entre une bougie et une information "categorie" affichee sur l'axe des x
    QBarCategoryAxis *axisX = new QBarCategoryAxis;
    QBarCategoryAxis *bar_axisX = new QBarCategoryAxis;
    bar_axisX->append(bar_categories);
    axisX->append(categories);

    // Un graphe peut contenir plusieurs series de données
    chart = new QChart();
    chart -> addSeries(series);
    chart -> setTitle("Cours OHLC");
    chart -> createDefaultAxes();
    chart -> setAxisX(axisX, series);
    chart -> legend() -> setAlignment(Qt::AlignBottom);
    // Widget d'affichage
    chartView = new QChartView(chart,this);
    //    chartView -> setFixedSize(800,300);

    //chart bar


    bar_chart -> setTitle("Graphique en barre (volume)");
    bar_chart -> createDefaultAxes();
    bar_chart -> setAxisX(bar_axisX,bar_series);
    bar_chart -> legend() -> setAlignment(Qt::AlignBottom);
    //widget d'affichage
    bar_chart_view = new QChartView(bar_chart,this);
    //    bar_chart_view -> setFixedSize(800,300);

    open = new QLineEdit(this);
    high = new QLineEdit(this);
    low = new QLineEdit(this);
    close = new QLineEdit(this);
    forme = new QLineEdit(this);
    openl = new QLabel("Open",this);
    highl = new QLabel("High",this);
    lowl = new QLabel("Low",this);
    closel = new QLabel("Close",this);
    formel = new QLabel("Forme", this);
    tab_tr = new QPushButton("Transactions effectuées",this);
    editeur = new QPushButton("Editeur de texte",this);
    retour = new QPushButton("Retour",this);
    //Création des différents composants graphique
    // Positionnement à l'aide de Hbox/Vbox

    copen = new QHBoxLayout;
    copen -> addWidget(openl);
    copen -> addWidget(open);
    chigh = new QHBoxLayout;
    chigh -> addWidget(highl);
    chigh -> addWidget(high);
    clow = new QHBoxLayout;
    clow -> addWidget(lowl);
    clow-> addWidget(low);
    cclose = new QHBoxLayout;
    cclose -> addWidget(closel);
    cclose ->addWidget(close);
    cforme = new QHBoxLayout;
    cforme->addWidget(formel);
    cforme->addWidget(forme);
    coucheCours = new QVBoxLayout;
    coucheCours -> addLayout(copen);
    coucheCours -> addLayout(chigh);
    coucheCours -> addLayout(clow);
    coucheCours -> addLayout(cclose);
    coucheCours->addLayout(cforme);
    coucheCours -> addWidget(tab_tr);
    coucheCours -> addWidget(editeur);
    coucheCours -> addWidget(retour);


    QGridLayout * disp = new QGridLayout();
    disp->setColumnMinimumWidth(0,1400);
    disp->setRowMinimumHeight(0,400);
    disp->setRowMinimumHeight(1,400);
    disp->addWidget(chartView,0,0);
    disp->addWidget(bar_chart_view,1,0);
    QWidget * widget = new QWidget();
    widget->setLayout(disp);
    double ratio = evolution.getNbCours() * 70;
    widget->resize(static_cast<int>(ratio), 800);
    QScrollArea * scroll_area = new QScrollArea();
    scroll_area->setWidget(widget);



    fenetre = new QVBoxLayout();
    fenetre -> addWidget(scroll_area);
    fenetre -> addLayout(coucheCours);


    setLayout(fenetre);
    connect(retour,SIGNAL(clicked()),this,SLOT(e_retour()));
    connect(tab_tr,SIGNAL(clicked()),this,SLOT(show_trans()));
    connect(editeur,SIGNAL(clicked()),this,SLOT(editeur_t()));

}

EvolutionViewer::EvolutionViewer(EvolutionCours &ev, modepas * mdp):QWidget(),evolution(ev),pas_parent(mdp){

    setAttribute(Qt::WA_DeleteOnClose);
    
    this->setFixedSize(1100,700);
    bar_chart = new QChart();

    // Série de bougies (verte/rouge)
    series = new QCandlestickSeries(this);
    series ->setIncreasingColor(QColor(Qt::green));
    series -> setDecreasingColor(QColor(Qt::red));



    QStringList categories; //Contient l'info a afficher sur l'axe des X pour chaque bougie creee
    QStringList bar_categories; //info pour l'axe


    //Parcourir la liste des CoursOHLC
    for (EvolutionCours::iterator it= evolution.begin();it != evolution.end(); ++it) {
        // Pour chaque CoursOHLC
        CoursOHLC & cours = *it;
        //Creer une bougie => QCandleStickSet
        Bougie * bougie = new Bougie(cours.getOpen(), cours.getHigh(),cours.getLow(),cours.getClose(),&cours,this);
        //Ajouter la bougie à la série de bougie
        series->append(bougie);
        bar_set = new QBarSet(cours.getDate().toString("dd.MM.yy")); //creer un QBarSet avec un QString, la date
        bar_set->append(cours.getVolume()); //rajoute l'information (ici volume)
        bar_series = new QBarSeries(this); //creer la serie de barre (ici il y aura une barre par serie) au widget parent
        bar_series->append(bar_set); //rajoute le barset a la serie
        bar_chart -> addSeries(bar_series); //rajoute la serie (ici une serie = 1 barre)
        bar_series = nullptr; //refais pointer bar_series sur nullptr
        categories << cours.getDate().toString("dd.MM.yy");
        bar_categories << cours.getDate().toString("dd.MM.yy");
        connect(bougie, SIGNAL(clickBougie(Bougie*)),this,SLOT(viewCoursOHLC(Bougie*)));
    }

    //Fait la correspondance entre une bougie et une information "categorie" affichee sur l'axe des x
    QBarCategoryAxis *axisX = new QBarCategoryAxis;
    QBarCategoryAxis *bar_axisX = new QBarCategoryAxis;
    bar_axisX->append(bar_categories);
    axisX->append(categories);

    // Un graphe peut contenir plusieurs series de données
    chart = new QChart();
    chart -> addSeries(series);
    chart -> setTitle("Cours OHLC");
    chart -> createDefaultAxes();
    chart -> setAxisX(axisX, series);
    chart -> legend() -> setAlignment(Qt::AlignBottom);
    // Widget d'affichage
    chartView = new QChartView(chart,this);
    chartView -> setFixedSize(800,300);

    //chart bar


    bar_chart -> setTitle("Graphique en barre (volume)");
    bar_chart -> createDefaultAxes();
    bar_chart -> setAxisX(bar_axisX,bar_series);
    bar_chart -> legend() -> setAlignment(Qt::AlignBottom);
    //widget d'affichage
    bar_chart_view = new QChartView(bar_chart,this);
    bar_chart_view -> setFixedSize(800,300);

    open = new QLineEdit(this);
    high = new QLineEdit(this);
    low = new QLineEdit(this);
    close = new QLineEdit(this);
    forme = new QLineEdit(this);
    openl = new QLabel("Open",this);
    highl = new QLabel("High",this);
    lowl = new QLabel("Low",this);
    closel = new QLabel("Close",this);
    formel = new QLabel("Forme", this);
    tab_tr = new QPushButton("Transactions effectuées",this);
    editeur = new QPushButton("Editeur de texte",this);
    retour = new QPushButton("Retour",this);
    //Création des différents composants graphique
    // Positionnement à l'aide de Hbox/Vbox

    copen = new QHBoxLayout;
    copen -> addWidget(openl);
    copen -> addWidget(open);
    chigh = new QHBoxLayout;
    chigh -> addWidget(highl);
    chigh -> addWidget(high);
    clow = new QHBoxLayout;
    clow -> addWidget(lowl);
    clow-> addWidget(low);
    cclose = new QHBoxLayout;
    cclose -> addWidget(closel);
    cclose ->addWidget(close);
    cforme = new QHBoxLayout;
    cforme->addWidget(formel);
    cforme->addWidget(forme);
    coucheCours = new QVBoxLayout;
    coucheCours -> addLayout(copen);
    coucheCours -> addLayout(chigh);
    coucheCours -> addLayout(clow);
    coucheCours -> addLayout(cclose);
    coucheCours->addLayout(cforme);
    coucheCours -> addWidget(tab_tr);
    coucheCours -> addWidget(editeur);
    coucheCours -> addWidget(retour);


    fenetre_pas = new QHBoxLayout;

    fenetre_pas -> addWidget(chartView);
    fenetre_pas -> addLayout(coucheCours);

    fenetre_finale = new QVBoxLayout; //layout final
    fenetre_finale -> addLayout(fenetre_pas);
    fenetre_finale ->addWidget(bar_chart_view);


    setLayout(fenetre_finale);
    connect(retour,SIGNAL(clicked()),this,SLOT(e_retour()));
    connect(tab_tr,SIGNAL(clicked()),this,SLOT(show_trans()));
    connect(editeur,SIGNAL(clicked()),this,SLOT(editeur_t()));
}

void EvolutionViewer::viewCoursOHLC(Bougie * b){
    QString str;
    str.setNum(b->getCoursOHLC().getOpen());
    open->setText(str);
    str.setNum(b->getCoursOHLC().getHigh());
    high->setText(str);
    str.setNum(b->getCoursOHLC().getLow());
    low->setText(str);
    str.setNum(b->getCoursOHLC().getClose());
    close->setText(str);
    forme->setText(b->getForme());
}

void EvolutionViewer::e_retour(){
    this->hide();
    if(MW_parent !=nullptr)
        MW_parent->show();
    else if(pas_parent != nullptr){
        pas_parent->show();
    }
}
void EvolutionViewer::show_trans(){
    if (tab_transaction.getTabTr().isEmpty() && MW_parent != nullptr){

        QMessageBox * msg = new QMessageBox(this);
        msg->setIcon(QMessageBox::Warning);
        msg->setText("Veuillez effectuer une 1ère transaction");
        msg->exec();
    }else if(!tab_transaction.getTabTr().isEmpty() && MW_parent != nullptr){ //si on charge evolutionviewer depuis le mode manuel

        tableau_t * table_tr = new tableau_t();
        table_tr->setVecteur(tab_transaction);
        table_tr->affichage();
    }else if(!pas_parent->getEvolTr()->getTabTr().isEmpty() && pas_parent != nullptr){//si on charge evolutionviewer depuis le mode pas a pas

        tableau_t * table_tr = new tableau_t();
        table_tr->setVecteur(*pas_parent->getEvolTr());
        table_tr->affichage();
    }
}

void EvolutionViewer::closeEvent(QCloseEvent* x_button){ //surcharge de l'event close
    if(pas_parent == nullptr && MW_parent != nullptr){
        MW_parent->show();
        x_button->accept();
    }else{
        pas_parent->show();
        x_button->accept();
    }
}

void EvolutionViewer::editeur_t(){
    EditeurTexte * edit = new EditeurTexte(this,tab_transaction);
    edit->show();
}
QString Bougie::getForme(){
    CoursOHLC * cours = &getCoursOHLC();
    if(cours->getClose()==cours->getOpen()){
        return "Doji";
    }
    else if(cours->isIncreasing()){
        double corps = cours->getClose()-cours->getOpen();
        if(corps< cours->MecheSup() && cours->MecheInf()==0){
            return "Etoile Filante";
        }
        if(corps< cours->MecheInf() && cours->MecheSup()==0){
            return "Pendu";
        }
        if(corps< cours->MecheInf() && corps < cours->MecheSup()){
            return "Toupie";
        }
    }
    else if(!cours->isIncreasing()){
        double corps = cours->getOpen()-cours->getClose();
        if(corps < cours->MecheInf() && cours->MecheSup()==0){
            return "Marteau";
        }
        if(corps< cours->MecheInf() && corps < cours->MecheSup()){
            return "Toupie";
        }
    }
    return "Bougie normale";
}


void Bougie::achat_vente(){
    if(double_clicked == 0 && EV_parent->MW_parent != nullptr){
        Achat_Vente * a_v = new Achat_Vente(this);
    }else if(double_clicked == 1 && EV_parent->MW_parent != nullptr){
        QMessageBox * msg = new QMessageBox();
        msg->setIcon(QMessageBox::Warning);
        msg->setText("Vous avez déjà clické sur cette bougie");
        msg->exec();
    }
    else{
        QMessageBox * msg = new QMessageBox();
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Cette manipulation n'est pas possible en mode pas à pas");
        msg->exec();
    }
}

Achat_Vente::Achat_Vente(Bougie * b):QWidget(),BG_parent(b),mw_parent(b->EV_parent->MW_parent){
    b->setDClicked(1);
    this->setFixedSize(250,150);
    this->setWindowTitle("Transaction");
    label = new QLabel("Que voulez vous faire (veuilez indiquez le montant)",this);
    montant = new QDoubleSpinBox();
    achat = new QPushButton("Acheter",this);
    vente = new QPushButton("Vendre",this);
    h_lay = new QHBoxLayout();
    h_lay->addWidget(achat);
    h_lay->addWidget(vente);
    v_lay = new QVBoxLayout();
    v_lay->addWidget(label);
    v_lay->addWidget(montant);
    v_lay->addLayout(h_lay);
    this->setLayout(v_lay);
    this->show();
    connect(achat,SIGNAL(clicked()),this,SLOT(acheter()));
    connect(vente,SIGNAL(clicked()),this,SLOT(vendre()));
}

void Achat_Vente::acheter(){
    if( BG_parent->EV_parent->tab_transaction.getTabTr().isEmpty()==true){
        Transaction * tr = new Transaction(BG_parent->cours->getDate(),mw_parent->getSoldeContrepartie(),mw_parent->getSoldeBase(),0,achat->text()); //A MODIF LES MONTANT DE D ET C QUAND NACIM FINI
        tr->calcul_tr(montant->value(),mw_parent->getBroker(),BG_parent->cours->getOpen(),BG_parent->cours->getClose(),
                      mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
        BG_parent->EV_parent->tab_transaction.addTransaction(*tr);
        this->hide();
    }else{
        Transaction * tr = new Transaction(BG_parent->cours->getDate(),BG_parent->EV_parent->tab_transaction.getTabTr().last().getTotalContrepartie()
                                           ,BG_parent->EV_parent->tab_transaction.getTabTr().last().getTotalBase(),0,achat->text()); //A MODIF LES MONTANT DE D ET C QUAND NACIM FINI
        tr->calcul_tr(montant->value(),mw_parent->getBroker(),BG_parent->cours->getOpen(),BG_parent->cours->getClose(),
                      mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
        BG_parent->EV_parent->tab_transaction.addTransaction(*tr);
        this->hide();
    }
}

void Achat_Vente::vendre(){
    if( BG_parent->EV_parent->tab_transaction.getTabTr().isEmpty()==true){
        Transaction * tr = new Transaction(BG_parent->cours->getDate(),mw_parent->getSoldeContrepartie(),mw_parent->getSoldeBase(),0,achat->text()); //A MODIF LES MONTANT DE D ET C QUAND NACIM FINI
        tr->calcul_tr(montant->value(),mw_parent->getBroker(),BG_parent->cours->getOpen(),BG_parent->cours->getClose(),
                      mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
        BG_parent->EV_parent->tab_transaction.addTransaction(*tr);
        this->hide();
    }else{
        Transaction * tr = new Transaction(BG_parent->cours->getDate(),BG_parent->EV_parent->tab_transaction.getTabTr().last().getTotalContrepartie()
                                           ,BG_parent->EV_parent->tab_transaction.getTabTr().last().getTotalBase(),0,vente->text()); //A MODIF LES MONTANT DE D ET C QUAND NACIM FINI
        tr->calcul_tr(montant->value(),mw_parent->getBroker(),BG_parent->cours->getOpen(),BG_parent->cours->getClose(),
                      mw_parent->getSoldeBase(),mw_parent->getSoldeContrepartie());
        BG_parent->EV_parent->tab_transaction.addTransaction(*tr);
        this->hide();
    }
}



//REGARDER LE CALCUL DES VALEURS DE BASE (ELLES PARAISSENT BIZARRE).


