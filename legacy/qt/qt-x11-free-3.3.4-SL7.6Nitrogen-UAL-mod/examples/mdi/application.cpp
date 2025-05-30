/****************************************************************************
** $Id: qt/application.cpp   3.3.4   edited Jan 4 17:55 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "application.h"
#include <qworkspace.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmovie.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qtextedit.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>
#include <qobjectlist.h>
#include <qvbox.h>
#include <qsimplerichtext.h>

#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"


const char * fileOpenText = "Click this button to open a <em>new file</em>. <br><br>"
"You can also select the <b>Open command</b> from the File menu.";
const char * fileSaveText = "Click this button to save the file you are "
"editing.  You will be prompted for a file name.\n\n"
"You can also select the Save command from the File menu.\n\n"
"Note that implementing this function is left as an exercise for the reader.";
const char * filePrintText = "Click this button to print the file you "
"are editing.\n\n"
"You can also select the Print command from the File menu.";

ApplicationWindow::ApplicationWindow()
    : QMainWindow( 0, "example application main window", WDestructiveClose )
{
    int id;

    QPixmap openIcon, saveIcon;

    fileTools = new QToolBar( this, "file operations" );
    addToolBar( fileTools, tr( "File Operations" ), DockTop, TRUE );

    openIcon = QPixmap( fileopen );
    QToolButton * fileOpen
	= new QToolButton( openIcon, "Open File", QString::null,
			   this, SLOT(load()), fileTools, "open file" );

    saveIcon = QPixmap( filesave );
    QToolButton * fileSave
	= new QToolButton( saveIcon, "Save File", QString::null,
			   this, SLOT(save()), fileTools, "save file" );

#ifndef QT_NO_PRINTER
    printer = new QPrinter( QPrinter::HighResolution );
    QPixmap printIcon;

    printIcon = QPixmap( fileprint );
    QToolButton * filePrint
	= new QToolButton( printIcon, "Print File", QString::null,
			   this, SLOT(print()), fileTools, "print file" );
    QWhatsThis::add( filePrint, filePrintText );
#endif

    (void)QWhatsThis::whatsThisButton( fileTools );

    QWhatsThis::add( fileOpen, fileOpenText );
    QWhatsThis::add( fileSave, fileSaveText );

    QPopupMenu * file = new QPopupMenu( this );
    menuBar()->insertItem( "&File", file );

    file->insertItem( "&New", this, SLOT(newDoc()), CTRL+Key_N );

    id = file->insertItem( openIcon, "&Open...",
			   this, SLOT(load()), CTRL+Key_O );
    file->setWhatsThis( id, fileOpenText );

    id = file->insertItem( saveIcon, "&Save",
			   this, SLOT(save()), CTRL+Key_S );
    file->setWhatsThis( id, fileSaveText );
    id = file->insertItem( "Save &As...", this, SLOT(saveAs()) );
    file->setWhatsThis( id, fileSaveText );
#ifndef QT_NO_PRINTER
    file->insertSeparator();
    id = file->insertItem( printIcon, "&Print...",
			   this, SLOT(print()), CTRL+Key_P );
    file->setWhatsThis( id, filePrintText );
#endif
    file->insertSeparator();
    file->insertItem( "&Close", this, SLOT(closeWindow()), CTRL+Key_W );
    file->insertItem( "&Quit", qApp, SLOT( closeAllWindows() ), CTRL+Key_Q );

    windowsMenu = new QPopupMenu( this );
    windowsMenu->setCheckable( TRUE );
    connect( windowsMenu, SIGNAL( aboutToShow() ),
	     this, SLOT( windowsMenuAboutToShow() ) );
    menuBar()->insertItem( "&Windows", windowsMenu );

    menuBar()->insertSeparator();
    QPopupMenu * help = new QPopupMenu( this );
    menuBar()->insertItem( "&Help", help );

    help->insertItem( "&About", this, SLOT(about()), Key_F1);
    help->insertItem( "About &Qt", this, SLOT(aboutQt()));
    help->insertSeparator();
    help->insertItem( "What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1);

    QVBox* vb = new QVBox( this );
    vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    ws = new QWorkspace( vb );
    ws->setScrollBarsEnabled( TRUE );
    setCentralWidget( vb );

    statusBar()->message( "Ready", 2000 );
}


ApplicationWindow::~ApplicationWindow()
{
#ifndef QT_NO_PRINTER
    delete printer;
#endif
}



MDIWindow* ApplicationWindow::newDoc()
{
    MDIWindow* w = new MDIWindow( ws, 0, WDestructiveClose );
    connect( w, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );
    w->setCaption("unnamed document");
    w->setIcon( QPixmap("document.xpm") );
    // show the very first window in maximized mode
    if ( ws->windowList().isEmpty() )
	w->showMaximized();
    else
	w->show();
    return w;
}

void ApplicationWindow::load()
{
    QString fn = QFileDialog::getOpenFileName( QString::null, QString::null, this );
    if ( !fn.isEmpty() ) {
	MDIWindow* w = newDoc();
	w->load( fn );
    }  else {
	statusBar()->message( "Loading aborted", 2000 );
    }
}

void ApplicationWindow::save()
{
    MDIWindow* m = (MDIWindow*)ws->activeWindow();
    if ( m )
	m->save();
}


void ApplicationWindow::saveAs()
{
    MDIWindow* m = (MDIWindow*)ws->activeWindow();
    if ( m )
	m->saveAs();
}


void ApplicationWindow::print()
{
#ifndef QT_NO_PRINTER
    MDIWindow* m = (MDIWindow*)ws->activeWindow();
    if ( m )
	m->print( printer );
#endif
}


void ApplicationWindow::closeWindow()
{
    MDIWindow* m = (MDIWindow*)ws->activeWindow();
    if ( m )
	m->close();
}

void ApplicationWindow::about()
{
    QMessageBox::about( this, "Qt Application Example",
			"This example demonstrates simple use of\n "
			"Qt's Multiple Document Interface (MDI).");
}


void ApplicationWindow::aboutQt()
{
    QMessageBox::aboutQt( this, "Qt Application Example" );
}


void ApplicationWindow::windowsMenuAboutToShow()
{
    windowsMenu->clear();
    int cascadeId = windowsMenu->insertItem("&Cascade", ws, SLOT(cascade() ) );
    int tileId = windowsMenu->insertItem("&Tile", ws, SLOT(tile() ) );
    int horTileId = windowsMenu->insertItem("Tile &Horizontally", this, SLOT(tileHorizontal() ) );
    if ( ws->windowList().isEmpty() ) {
	windowsMenu->setItemEnabled( cascadeId, FALSE );
	windowsMenu->setItemEnabled( tileId, FALSE );
	windowsMenu->setItemEnabled( horTileId, FALSE );
    }
    windowsMenu->insertSeparator();
    QWidgetList windows = ws->windowList();
    for ( int i = 0; i < int(windows.count()); ++i ) {
	int id = windowsMenu->insertItem(windows.at(i)->caption(),
					 this, SLOT( windowsMenuActivated( int ) ) );
	windowsMenu->setItemParameter( id, i );
	windowsMenu->setItemChecked( id, ws->activeWindow() == windows.at(i) );
    }
}

void ApplicationWindow::windowsMenuActivated( int id )
{
    QWidget* w = ws->windowList().at( id );
    if ( w )
	w->showNormal();
    w->setFocus();
}

void ApplicationWindow::tileHorizontal()
{
    // primitive horizontal tiling
    QWidgetList windows = ws->windowList();
    if ( !windows.count() )
	return;

    int heightForEach = ws->height() / windows.count();
    int y = 0;
    for ( int i = 0; i < int(windows.count()); ++i ) {
	QWidget *window = windows.at(i);
	if ( window->testWState( WState_Maximized ) ) {
	    // prevent flicker
	    window->hide();
	    window->showNormal();
	}
	int preferredHeight = window->minimumHeight()+window->parentWidget()->baseSize().height();
	int actHeight = QMAX(heightForEach, preferredHeight);

	window->parentWidget()->setGeometry( 0, y, ws->width(), actHeight );
	y += actHeight;
    }
}

void ApplicationWindow::closeEvent( QCloseEvent *e )
{
    QWidgetList windows = ws->windowList();
    if ( windows.count() ) {
	for ( int i = 0; i < int(windows.count()); ++i ) {
	    QWidget *window = windows.at( i );
	    if ( !window->close() ) {
		e->ignore();
		return;
	    }
	}
    }

    QMainWindow::closeEvent( e );
}

MDIWindow::MDIWindow( QWidget* parent, const char* name, int wflags )
    : QMainWindow( parent, name, wflags )
{
    mmovie = 0;
    medit = new QTextEdit( this );
    setFocusProxy( medit );
    setCentralWidget( medit );
}

MDIWindow::~MDIWindow()
{
    delete mmovie;
}

void MDIWindow::closeEvent( QCloseEvent *e )
{
    if ( medit->isModified() ) {
	switch( QMessageBox::warning( this, "Save Changes",
	    tr("Save changes to %1?").arg( caption() ),
	    tr("Yes"), tr("No"), tr("Cancel") ) ) {
	case 0:
	    {
		save();
		if ( !filename.isEmpty() )
		    e->accept();
		else
		    e->ignore();
	    }
	    break;
	case 1:
	    e->accept();
	    break;
	default:
	    e->ignore();
	    break;
	}
    } else {
	e->accept();
    }
}

void MDIWindow::load( const QString& fn )
{
    filename  = fn;
    QFile f( filename );
    if ( !f.open( IO_ReadOnly ) )
	return;

    if(fn.contains(".gif")) {
	QWidget * tmp=new QWidget(this);
	setFocusProxy(tmp);
	setCentralWidget(tmp);
	medit->hide();
	delete medit;
	QMovie * qm=new QMovie(fn);
#ifdef Q_WS_QWS // temporary speed-test hack
	qm->setDisplayWidget(tmp);
#endif
	tmp->setBackgroundMode(QWidget::NoBackground);
	tmp->show();
	mmovie=qm;
    } else {
	mmovie = 0;

	QTextStream t(&f);
	QString s = t.read();
	medit->setText( s );
	f.close();


    }
    setCaption( filename );
    emit message( QString("Loaded document %1").arg(filename), 2000 );
}

void MDIWindow::save()
{
    if ( filename.isEmpty() ) {
        saveAs();
        return;
    }

    QString text = medit->text();
    QFile f( filename );
    if ( !f.open( IO_WriteOnly ) ) {
        emit message( QString("Could not write to %1").arg(filename),
		      2000 );
        return;
    }

    QTextStream t( &f );
    t << text;
    f.close();

    setCaption( filename );

    emit message( QString( "File %1 saved" ).arg( filename ), 2000 );
}

void MDIWindow::saveAs()
{
    QString fn = QFileDialog::getSaveFileName( filename, QString::null, this );
    if ( !fn.isEmpty() ) {
        filename = fn;
        save();
    } else {
        emit message( "Saving aborted", 2000 );
    }
}

void MDIWindow::print( QPrinter* printer)
{
#ifndef QT_NO_PRINTER
    int pageNo = 1;

    if ( printer->setup(this) ) {		// printer dialog
        printer->setFullPage( TRUE );
	emit message( "Printing...", 0 );
	QPainter p;
	if ( !p.begin( printer ) )
	    return;				// paint on printer
	QPaintDeviceMetrics metrics( p.device() );
	int dpiy = metrics.logicalDpiY();
	int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins
	QRect view( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
	QSimpleRichText richText( QStyleSheet::convertFromPlainText(medit->text()),
				  QFont(),
				  medit->context(),
				  medit->styleSheet(),
				  medit->mimeSourceFactory(),
				  view.height() );
	richText.setWidth( &p, view.width() );
	int page = 1;
	do {
	    richText.draw( &p, margin, margin, view, colorGroup() );
	    view.moveBy( 0, view.height() );
	    p.translate( 0 , -view.height() );
	    p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),
			view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
	    if ( view.top() - margin >= richText.height() )
		break;
	    QString msg( "Printing (page " );
	    msg += QString::number( ++pageNo );
	    msg += ")...";
	    emit message( msg, 0 );
	    printer->newPage();
	    page++;
	} while (TRUE);
    }
#endif
}
