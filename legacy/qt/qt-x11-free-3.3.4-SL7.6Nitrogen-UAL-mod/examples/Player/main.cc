#include <qapplication.h>
#include "MainPlayerUI.h"

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );

    UAL::QT::MainPlayerUI *mw = new UAL::QT::MainPlayerUI();

    mw->setCaption( "Qt Example - Application" );
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
