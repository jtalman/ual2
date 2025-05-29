/****************************************************************************
** TQtTimer meta object code from reading C++ file 'TQtTimer.h'
**
** Created: Thu Jun 25 13:23:45 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtTimer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtTimer::className() const
{
    return "TQtTimer";
}

QMetaObject *TQtTimer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtTimer( "TQtTimer", &TQtTimer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtTimer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtTimer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtTimer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtTimer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtTimer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTimer::staticMetaObject();
    static const QUMethod slot_0 = {"AwakeRootEvent", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "AwakeRootEvent()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtTimer", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtTimer.setMetaObject( metaObj );
    return metaObj;
}

void* TQtTimer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtTimer" ) )
	return this;
    return QTimer::qt_cast( clname );
}

bool TQtTimer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AwakeRootEvent(); break;
    default:
	return QTimer::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtTimer::qt_emit( int _id, QUObject* _o )
{
    return QTimer::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtTimer::qt_property( int id, int f, QVariant* v)
{
    return QTimer::qt_property( id, f, v);
}

bool TQtTimer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
