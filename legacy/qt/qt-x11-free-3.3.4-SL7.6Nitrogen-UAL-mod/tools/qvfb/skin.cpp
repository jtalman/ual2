/**********************************************************************
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of Qt/Embedded virtual framebuffer.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "skin.h"
#include "qvfb.h"
#include "qvfbview.h"

#include <qbitmap.h>
#include <qpixmap.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qpainter.h>
#ifdef Q_WS_MAC
# include <qt_mac.h>
#endif

Skin::Skin( QVFb *p, const QString &skinFile, int &viewW, int &viewH ) : QWidget(p)
{
    QFile f( skinFile );
    f.open( IO_ReadOnly );
    QTextStream ts( &f );
    ts >> skinImageUpFileName;
    ts >> skinImageDownFileName;
    ts >> viewX1;
    ts >> viewY1;
    ts >> viewW;
    ts >> viewH;
    ts >> transparancy;
    ts >> numberOfAreas;
//  Debug the skin file parsing
//  printf("read: -%s- -%i- -%i- -%i-\n", skinImage.latin1(), viewX1, viewY1, numberOfAreas );
    areas = new ButtonAreas[numberOfAreas];

    for (int i = 0; i < numberOfAreas; i++) {
	ts >> areas[i].name;
	ts >> areas[i].keyCode;
	ts >> areas[i].x1;
	ts >> areas[i].y1;
	ts >> areas[i].x2;
	ts >> areas[i].y2;
//	Debug the skin file parsing
//	printf("read: -%s- -%i- -%i- -%i- -%i- -%i-\n", areas[i].name.latin1(),
//	    areas[i].keyCode, areas[i].x1, areas[i].y1, areas[i].x2, areas[i].y2 );
    }

    parent = p;
    skinImageUp = new QPixmap( skinImageUpFileName );
    skinImageDown = new QPixmap( skinImageDownFileName );
//  setPixmap( ipaq );
    setFixedSize( skinImageUp->size() );
    QBitmap mask = skinImageUp->createHeuristicMask();
    int wf = WStyle_Customize | WType_TopLevel | WStyle_NoBorder;
    parent->reparent( 0, wf, pos(), TRUE );
    parent->setMask( mask );
    parent->setFixedSize( skinImageUp->size() );
    buttonPressed = FALSE;
    buttonIndex = 0;
}


Skin::~Skin( )
{
}


void Skin::setView( QVFbView *v )
{
    view = v;
    view->move( viewX1, viewY1 );
#ifdef Q_WS_MAC
    QMacSavedPortInfo::setAlphaTransparancy(view, transparancy);
#endif
}


void Skin::paintEvent( QPaintEvent * )
{
    QPainter p( this );
//  printf("read: -%s-\n", skinImageUp.latin1());
    if (skinImageUp)
	p.drawPixmap( 0, 0, *skinImageUp );
    if (buttonPressed == TRUE) {
	ButtonAreas *ba = &areas[buttonIndex];
	if (skinImageDown)
	    p.drawPixmap( ba->x1, ba->y1, *skinImageDown, ba->x1, ba->y1, ba->x2 - ba->x1, ba->y2 - ba->y1 );
    }
}


void Skin::mousePressEvent( QMouseEvent *e )
{
    if (e->button() == RightButton) {
	parent->popupMenu();
    } else {
	buttonPressed = FALSE;

	for (int i = 0; i < numberOfAreas; i++) {
	    QPoint p1( areas[i].x1, areas[i].y1 );
	    QPoint p2( areas[i].x2, areas[i].y2 );
	    QRect r( p1, p2 );
	    if ( r.contains( e->pos() ) ) {
		buttonPressed = TRUE;
		buttonIndex = i;
		buttonCode = areas[buttonIndex].keyCode;
		QKeyEvent keyEvent( QEvent::KeyPress, buttonCode, 0, 0 );
		if (view)
		    view->skinKeyPressEvent( &keyEvent );
//		Debug message to be sure we are clicking the right areas
//		printf("%s clicked\n", areas[i].name);
		ButtonAreas *ba = &areas[buttonIndex];
		repaint( ba->x1, ba->y1, ba->x2 - ba->x1, ba->y2 - ba->y1, FALSE );
		continue;
	    }
	}
	
//	This is handy for finding the areas to define rectangles for new skins
//	printf("Clicked in %i,%i\n",  e->pos().x(),  e->pos().y());
	clickPos = e->pos();
    }
}


void Skin::mouseMoveEvent( QMouseEvent *e )
{
    if ( buttonPressed == FALSE ) {
	QPoint newpos =  e->globalPos() - clickPos;
	parent->move( newpos );
    }
}


void Skin::mouseReleaseEvent( QMouseEvent * )
{
    if ( buttonPressed ) {
	QKeyEvent keyEvent( QEvent::KeyRelease, buttonCode, 0, 0 );
	if (view)
	    view->skinKeyReleaseEvent( &keyEvent );
	buttonPressed = FALSE;
	ButtonAreas *ba = &areas[buttonIndex];
	repaint( ba->x1, ba->y1, ba->x2 - ba->x1, ba->y2 - ba->y1, FALSE );
    }
}


