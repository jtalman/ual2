/****************************************************************************
** TQtClientGuard meta object code from reading C++ file 'TQtClientGuard.h'
**
** Created: Thu Jun 25 13:23:43 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtClientGuard.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtClientGuard::className() const
{
    return "TQtClientGuard";
}

QMetaObject *TQtClientGuard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtClientGuard( "TQtClientGuard", &TQtClientGuard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtClientGuard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtClientGuard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtClientGuard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtClientGuard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtClientGuard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"Disconnect", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Disconnect()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtClientGuard", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtClientGuard.setMetaObject( metaObj );
    return metaObj;
}

void* TQtClientGuard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtClientGuard" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool TQtClientGuard::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Disconnect(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtClientGuard::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtClientGuard::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool TQtClientGuard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *TQtPixmapGuard::className() const
{
    return "TQtPixmapGuard";
}

QMetaObject *TQtPixmapGuard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtPixmapGuard( "TQtPixmapGuard", &TQtPixmapGuard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtPixmapGuard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtPixmapGuard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtPixmapGuard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtPixmapGuard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtPixmapGuard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"Disconnect", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Disconnect()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtPixmapGuard", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtPixmapGuard.setMetaObject( metaObj );
    return metaObj;
}

void* TQtPixmapGuard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtPixmapGuard" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool TQtPixmapGuard::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Disconnect(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtPixmapGuard::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtPixmapGuard::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool TQtPixmapGuard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
