#ifndef EDITEURTEXTE_H
#define EDITEURTEXTE_H
#include "includes.h"
#include "transaction.h"


class EditeurTexte : public QMainWindow
{
private:
    Q_OBJECT
    QWidget *m_parent; //on stock dans un pointeur l'adresse du widget parent.
    QTextEdit * zone_centrale;

    //Menu Fichier + Actions ("sous-menus")
    QMenu * file_menu;
    QAction * quit;
    QAction * save;
    QAction * new_file;
    QMenu * edition_menu;
    QAction * undo;
    QAction * redo;
    QAction * cut;
    QAction * copy;
    QAction * paste;
    EvolutionTransaction m_tab_tr;

    void closeEvent(QCloseEvent* x_button); //polymorphisque (overriding) afin de retourner sur le widget parent



public:
    EditeurTexte(QWidget * parent = nullptr, EvolutionTransaction tab = {});




public slots:
    void close_notepad();
    void new_notepad();
    void copy_texte();
    void paste_texte();
    void cut_texte();
    void s_redo();
    void s_undo();
    void save_to_file();
};

#endif // EDITEURTEXTE_H
