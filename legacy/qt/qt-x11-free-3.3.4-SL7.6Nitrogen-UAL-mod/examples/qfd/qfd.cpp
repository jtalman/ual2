/****************************************************************************
** $Id: qt/qfd.cpp   3.3.4   edited May 27 2003 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "fontdisplayer.h"

#include <qapplication.h>
#include <qslider.h>
#include <qpainter.h>
#include <qstatusbar.h>



int main(int argc, char** argv)
{
    QApplication app(argc,argv);

    FontDisplayer m;
    QSize sh = m.centralWidget()->sizeHint();
    m.resize(sh.width(),
             sh.height()+3*m.statusBar()->height());
    app.setMainWidget(&m);
    m.setCaption("Qt Example - QFD");
    m.show();

    return app.exec();
}
