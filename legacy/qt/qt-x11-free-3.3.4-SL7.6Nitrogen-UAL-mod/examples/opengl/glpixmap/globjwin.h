/****************************************************************************
** $Id: qt/globjwin.h   3.3.4   edited May 27 2003 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

/****************************************************************************
**
** The GLObjectWindow contains a GLBox and three sliders connected to
** the GLBox's rotation slots.
**
****************************************************************************/

#ifndef GLOBJWIN_H
#define GLOBJWIN_H

#include <qwidget.h>

class GLBox;
class QLabel;
class QPopupMenu;

class GLObjectWindow : public QWidget
{
    Q_OBJECT
public:
    GLObjectWindow( QWidget* parent = 0, const char* name = 0 );

protected slots:

    void		grabFrameBuffer();
    void		makePixmap();
    void		makePixmapHidden();
    void		makePixmapForMenu();
    void		useFixedPixmapSize();

private:
    void		drawOnPixmap( QPixmap* pm );
    GLBox* c1;
    QLabel* lb;
    int fixMenuItemId;
    int insertMenuItemId;
    QSize pmSz;
    QPopupMenu* file;
};


#endif // GLOBJWIN_H
