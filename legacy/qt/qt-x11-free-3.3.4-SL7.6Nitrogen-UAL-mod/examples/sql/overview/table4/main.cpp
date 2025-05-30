/****************************************************************************
** $Id: qt/main.cpp   3.3.4   edited May 27 2003 $
**
** Copyright (C) 1992-2002 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "main.h"


StatusPicker::StatusPicker( QWidget *parent, const char *name )
    : QComboBox( parent, name )
{
    QSqlCursor cur( "status" );
    cur.select( cur.index( "name" ) );

    int i = 0;
    while ( cur.next() ) {
	insertItem( cur.value( "name" ).toString(), i );
	index2id[i] = cur.value( "id" ).toInt();
	i++;
    }
}


int StatusPicker::statusId() const
{
    return index2id[ currentItem() ];
}


void StatusPicker::setStatusId( int statusid )
{
    QMap<int,int>::Iterator it;
    for ( it = index2id.begin(); it != index2id.end(); ++it ) {
	if ( it.data() == statusid ) {
	    setCurrentItem( it.key() );
	    break;
	}
    }
}


void CustomTable::paintField( QPainter * p, const QSqlField* field,
			      const QRect & cr, bool b)
{
    if ( !field )
	return;
    if ( field->name() == "statusid" ) {
	QSqlQuery query( "SELECT name FROM status WHERE id=" +
		     field->value().toString() );
	QString text;
	if ( query.next() ) {
	    text = query.value( 0 ).toString();
	}
	p->drawText( 2,2, cr.width()-4, cr.height()-4, fieldAlignment( field ), text );
    }
    else {
	QDataTable::paintField( p, field, cr, b) ;
    }
}


QWidget *CustomSqlEditorFactory::createEditor(
    QWidget *parent, const QSqlField *field )
{
    if ( field->name() == "statusid" ) {
	QWidget *editor = new StatusPicker( parent );
	return editor;
    }

    return QSqlEditorFactory::createEditor( parent, field );
}


int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    if ( createConnections() ) {
	QSqlCursor staffCursor( "staff" );

	CustomTable		*staffTable	= new CustomTable( &staffCursor );
	QSqlPropertyMap		*propMap	= new QSqlPropertyMap();
	CustomSqlEditorFactory	*editorFactory	= new CustomSqlEditorFactory();
	propMap->insert( "StatusPicker", "statusid" );
	staffTable->installPropertyMap( propMap );
	staffTable->installEditorFactory( editorFactory );

	app.setMainWidget( staffTable );

	staffTable->addColumn( "forename", "Forename" );
	staffTable->addColumn( "surname",  "Surname" );
	staffTable->addColumn( "salary",   "Annual Salary" );
	staffTable->addColumn( "statusid", "Status" );

	QStringList order = QStringList() << "surname" << "forename";
	staffTable->setSort( order );

	staffTable->refresh();
	staffTable->show();

	return app.exec();
    }

    return 1;
}
