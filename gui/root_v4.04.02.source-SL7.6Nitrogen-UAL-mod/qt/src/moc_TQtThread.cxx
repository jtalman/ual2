/****************************************************************************
** TQtThread meta object code from reading C++ file 'TQtThread.h'
**
** Created: Thu Jun 25 13:23:46 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtThread.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtThread::className() const
{
    return "TQtThread";
}

QMetaObject *TQtThread::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtThread( "TQtThread", &TQtThread::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtThread::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtThread", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtThread::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtThread", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtThread::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"TQtThread", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtThread.setMetaObject( metaObj );
    return metaObj;
}

void* TQtThread::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtThread" ) )
	return this;
    if ( !qstrcmp( clname, "TGQt" ) )
	return (TGQt*)this;
    return QObject::qt_cast( clname );
}

bool TQtThread::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool TQtThread::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtThread::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool TQtThread::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
