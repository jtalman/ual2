/****************************************************************************
** KALedMeter meta object code from reading C++ file 'ledmeter.h'
**
** Created: Sun May 25 09:27:13 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../qasteroids/ledmeter.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KALedMeter::className() const
{
    return "KALedMeter";
}

QMetaObject *KALedMeter::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KALedMeter( "KALedMeter", &KALedMeter::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KALedMeter::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KALedMeter", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KALedMeter::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KALedMeter", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KALedMeter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "v", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setValue", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "setValue(int)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"KALedMeter", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KALedMeter.setMetaObject( metaObj );
    return metaObj;
}

void* KALedMeter::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KALedMeter" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool KALedMeter::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setValue((int)static_QUType_int.get(_o+1)); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KALedMeter::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KALedMeter::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool KALedMeter::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
