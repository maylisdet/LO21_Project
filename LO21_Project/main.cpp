#include <QApplication>
#include <QWidget>
#include <iostream>
#include "csvparser.h"
#include "includes.h"
#include "transaction.h"
#include "editeurtexte.h"
#include "import.h"
#include "mainwindow.h"
#include "transactionviewer.h"
#include "indicateurs.h"
#include "automaticmode.h"



using namespace std;


int main(int argc, char *argv[]) {
        QApplication app(argc, argv);

        Import * imp = new Import();
        imp->show();



  return app.exec();


}
