/****************************************************************************
** $Id: qt/main.cpp   3.3.4   edited May 27 2003 $
**
** Copyright ( C ) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qapplication.h>

#include "mainwindow.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    ABMainWindow *mw = new ABMainWindow();
    mw->setCaption( "Qt Example - Addressbook" );
    a.setMainWidget( mw );
    mw->show();

    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    int result = a.exec();
    delete mw;
    return result;
}
