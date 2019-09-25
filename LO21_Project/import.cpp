#include "import.h"
#include "mainwindow.h"

Import::Import():QWidget()
{
    setWindowIcon(QIcon("./icons.icns"));
    this->setWindowTitle("Trading Simulator");

    //**LABEL**//
    welcome1 = new QLabel("BIENVENUE SUR TRADING SIMULATOR",this);
    welcome2 = new QLabel("Veuillez charger des données :",this);

    //**CSV**//
    //form
    csv_form_group = new QGroupBox(tr("Importation CSV"));
    csv_form_lay = new QFormLayout;
    csv_form_lay->setSpacing(10);
    load_csv = new QPushButton("Charger un CSV",this);
    csv_form_lay->addWidget(load_csv);
    csv_form_group->setLayout(csv_form_lay);

    //**XML**//
    //form
    xml_form_group = new QGroupBox(tr("Importation XML"));
    xml_form_lay = new QFormLayout;
    xml_form_lay->setSpacing(10);
    load_xml = new QPushButton("Charger un XML",this);
    load_xml->setEnabled(false);
    xml_form_lay->addWidget(load_xml);
    xml_form_group->setLayout(xml_form_lay);

    //**MANUEL**//
    //form
    man_form_group = new QGroupBox(tr("Importation manuelle"));
    man_form_lay = new QFormLayout;
    man_form_lay->setSpacing(10);
    load_man = new QPushButton("Charger manuellement",this);
    load_man->setEnabled(false);
    man_form_lay->addWidget(load_man);
    man_form_group->setLayout(man_form_lay);

    main_lay = new QVBoxLayout();
    main_lay->addSpacing(10);
    main_lay->addWidget(welcome1,0,Qt::AlignCenter);
    main_lay->addSpacing(15);
    main_lay->addWidget(welcome2,0,Qt::AlignCenter);
    main_lay->addSpacing(30);
    main_lay->addWidget(csv_form_group);
    main_lay->addWidget(xml_form_group);
    main_lay->addWidget(man_form_group);
    setLayout(main_lay);

    //**SLOTS&SIGNALS**//
    connect(load_csv,SIGNAL(clicked()),this,SLOT(load_csv_file()));
    connect(load_xml,SIGNAL(clicked()),this,SLOT(load_xml_file()));
    connect(load_man,SIGNAL(clicked()),this,SLOT(load_man_file()));
}

void Import::load_csv_file(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open CSV file"), "", tr("Text Files (*.csv)"));

    QFileInfo file_info(fileName);

    parser = new CSVParser(file_info.filePath());
    QVector<QStringList> liste_donnee = parser->getInfo(); //vecteur qui contient les infos


    evolution = new EvolutionCours();

    qreal open = 0; qreal high = 0; qreal low = 0; qreal close = 0; qreal volume = 0; QDate date;

    for (auto it = liste_donnee.begin(); it != liste_donnee.end(); ++it){ //la case 0 contient les titres
        for (int i = 0; i < it->size(); ++i){
            switch(i){
            case 0 : date = QDate::fromString(it->at(i),"yyyy-MM-d");
                break;
            case 1 : open = it->at(i).toDouble();
                break;
            case 2 : high = it->at(i).toDouble();
                break;
            case 3 : low = it->at(i).toDouble();
                break;
            case 4 : close = it->at(i).toDouble();
                break;
            case 6 : volume = it->at(i).toDouble();
                //a l'index 5 se trouve l'info ajd, on en a pas besoin
            }
        }

        evolution->addCours(open,high,low,close,volume,date); //pour chaque ligne du fichier on creer un coursOHLC
    }

    MainWindow * win = new MainWindow(this);
    win->show();
    this->hide();
}

void Import::load_xml_file(){
    MainWindow * win = new MainWindow(this);
    win->show();
    this->hide();
}

void Import::load_man_file(){
    MainWindow * win = new MainWindow(this);
    win->show();
    this->hide();
}
