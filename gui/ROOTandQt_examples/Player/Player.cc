#include<qapplication.h>
#include"MainPlayerUI.h"
#include"TGraph.h"
#include"TQtWidget.h"
#include"TCanvas.h"

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
    TQtWidget *MyWidget= new TQtWidget(0,"MyWidget");

    MainPlayerUI *mw = new MainPlayerUI();

    mw->setCaption( "Qt Example - Application" );
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
