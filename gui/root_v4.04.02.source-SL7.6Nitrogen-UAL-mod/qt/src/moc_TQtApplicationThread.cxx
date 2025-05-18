/****************************************************************************
** TQtApplicationThread meta object code from reading C++ file 'TQtApplicationThread.h'
**
** Created: Thu Jun 25 13:23:42 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtApplicationThread.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtApplicationThread::className() const
{
    return "TQtApplicationThread";
}

QMetaObject *TQtApplicationThread::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtApplicationThread( "TQtApplicationThread", &TQtApplicationThread::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtApplicationThread::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtApplicationThread", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtApplicationThread::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtApplicationThread", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtApplicationThread::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"AboutToQuit", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "AboutToQuit()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtApplicationThread", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtApplicationThread.setMetaObject( metaObj );
    return metaObj;
}

void* TQtApplicationThread::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtApplicationThread" ) )
	return this;
    if ( !qstrcmp( clname, "TQtRootThread" ) )
	return (TQtRootThread*)this;
    return QObject::qt_cast( clname );
}

bool TQtApplicationThread::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AboutToQuit(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtApplicationThread::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtApplicationThread::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool TQtApplicationThread::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
