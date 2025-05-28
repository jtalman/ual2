/****************************************************************************
** TQtTermInputHandler meta object code from reading C++ file 'TQtTermInputHandler.h'
**
** Created: Thu Jun 25 13:23:44 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtTermInputHandler.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtTermInputHandler::className() const
{
    return "TQtTermInputHandler";
}

QMetaObject *TQtTermInputHandler::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtTermInputHandler( "TQtTermInputHandler", &TQtTermInputHandler::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtTermInputHandler::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtTermInputHandler", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtTermInputHandler::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtTermInputHandler", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtTermInputHandler::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "fd", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"Activate", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "Activate(int)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtTermInputHandler", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtTermInputHandler.setMetaObject( metaObj );
    return metaObj;
}

void* TQtTermInputHandler::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtTermInputHandler" ) )
	return this;
    if ( !qstrcmp( clname, "TFileHandler" ) )
	return (TFileHandler*)this;
    return QObject::qt_cast( clname );
}

bool TQtTermInputHandler::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Activate((int)static_QUType_int.get(_o+1)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtTermInputHandler::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtTermInputHandler::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool TQtTermInputHandler::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
