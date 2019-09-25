#include "editeurtexte.h"


EditeurTexte::EditeurTexte(QWidget * q,EvolutionTransaction tab) : QMainWindow(), m_parent(q),m_tab_tr(tab)

{
    menuBar()->setNativeMenuBar(false);

    std::cout<<"Creation d'un editeur de texte" << std::endl;
    //window settings
    setWindowTitle("Editeur de texte");
    setFixedSize(450,450);

    //zone milieu de l'editeur
    zone_centrale = new QTextEdit(); //creation de la zone de texte
    this->setCentralWidget(zone_centrale); //dire que la zone occupe l'espace central du widget

    //barre du menu
    file_menu = new QMenu();
    file_menu = menuBar()->addMenu("&Fichier");
    new_file = new QAction("&Nouveau fichier",this);
    save = new QAction("&Sauvegarder",this);
    quit = new QAction("&Fermer",this);
    file_menu->addAction(new_file);
    file_menu->addAction(save);
    file_menu->addAction(quit);

    edition_menu = new QMenu();
    edition_menu = menuBar()->addMenu("&Edition");
    undo = new QAction("&Undo",this);
    redo = new QAction("&Redo",this);
    cut = new QAction("&Couper",this);
    copy = new QAction("&Copier",this);
    paste = new QAction("&Coller",this);
    edition_menu->addAction(undo);
    edition_menu->addAction(redo);
    edition_menu->addAction(cut);
    edition_menu->addAction(copy);
    edition_menu->addAction(paste);

    //raccourcis
    new_file->setShortcut(QKeySequence("Ctrl+N"));
    quit->setShortcut(QKeySequence("Ctrl+W"));
    copy->setShortcut(QKeySequence("Ctrl+C"));
    paste->setShortcut(QKeySequence("Ctrl+V"));
    cut->setShortcut(QKeySequence("Ctrl+X"));
    undo->setShortcut(QKeySequence("Ctrl+Z"));
    redo->setShortcut(QKeySequence("Ctrl+Shift+Z"));
    save->setShortcut(QKeySequence("Ctrl+S"));

    //signals&slots
    connect(quit,SIGNAL(triggered()),this,SLOT(close_notepad()));
    connect(new_file,SIGNAL(triggered()),this,SLOT(new_notepad()));
    connect(copy,SIGNAL(triggered()),this,SLOT(copy_texte()));
    connect(paste,SIGNAL(triggered()),this,SLOT(paste_texte()));
    connect(cut,SIGNAL(triggered()),this,SLOT(cut_texte()));
    connect(undo,SIGNAL(triggered()),this,SLOT(s_undo()));
    connect(redo,SIGNAL(triggered()),this,SLOT(s_redo()));
    connect(save,SIGNAL(triggered()),this,SLOT(save_to_file()));


}

void EditeurTexte::close_notepad(){
    this->close();
    m_parent->show();
}
void EditeurTexte::new_notepad(){
    EditeurTexte * new_notepad = new EditeurTexte(m_parent);
    new_notepad->show();
}

void EditeurTexte::copy_texte(){
    zone_centrale->copy();
}

void EditeurTexte::paste_texte(){
    zone_centrale->paste();
}

void EditeurTexte::cut_texte(){
    zone_centrale->cut();
}

void EditeurTexte::s_redo(){
    zone_centrale->redo();
}

void EditeurTexte::s_undo(){
    zone_centrale->undo();
}

void EditeurTexte::save_to_file(){


    //save to file
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer un fichier","notes", "Fichier Texte(*.txt *.ol)");
    QFile file(fichier);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    file.write("NOTES \n\n");
    file.write(zone_centrale->toPlainText().toLatin1());
    file.write("\n\n########################\n\n");
    file.write("RESUME DES TRANSACTIONS \n\n");

    QFile file1(fichier);
     QTextStream stream(&file1);
    if (file1.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        for (int i = 0; i < m_tab_tr.getTabTr().size() ; ++i){
            stream << "Date : " <<
                      m_tab_tr.getTabTr()[i].getDateTransaction().toString("yyyy-MM-dd") <<
                      ", solde de contrepartie : " << m_tab_tr.getTabTr()[i].getTotalContrepartie() <<
                   ", solde de base : " <<m_tab_tr.getTabTr()[i].getTotalBase() <<", ROI : "<< m_tab_tr.getTabTr()[i].getROI()<<", type de la transaction : " <<
                 m_tab_tr.getTabTr()[i].getTypeTransaction()<<endl;
        }
    }




}
void EditeurTexte::closeEvent(QCloseEvent *x_button){
    m_parent->show();
    x_button->accept();
}
