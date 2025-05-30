/****************************************************************************
** $Id: qt/application.h   3.3.4   edited Dec 10 2003 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <qmainwindow.h>
#include <qptrlist.h>

class QTextEdit;
class QToolBar;
class QPopupMenu;
class QWorkspace;
class QPopupMenu;
class QMovie;

class MDIWindow: public QMainWindow
{
    Q_OBJECT
public:
    MDIWindow( QWidget* parent, const char* name, int wflags );
    ~MDIWindow();

    void load( const QString& fn );
    void save();
    void saveAs();
    void print( QPrinter* );

protected:
    void closeEvent( QCloseEvent * );

signals:
    void message(const QString&, int );

private:
    QTextEdit* medit;
    QMovie * mmovie;
    QString filename;
};


class ApplicationWindow: public QMainWindow
{
    Q_OBJECT
public:
    ApplicationWindow();
    ~ApplicationWindow();

protected:
    void closeEvent( QCloseEvent * );

private slots:
    MDIWindow* newDoc();
    void load();
    void save();
    void saveAs();
    void print();
    void closeWindow();
    void tileHorizontal();

    void about();
    void aboutQt();

    void windowsMenuAboutToShow();
    void windowsMenuActivated( int id );

private:
    QPrinter *printer;
    QWorkspace* ws;
    QToolBar *fileTools;
    QPopupMenu* windowsMenu;
};


#endif
