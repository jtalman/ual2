/****************************************************************************
** LCDRange meta object code from reading C++ file 'lcdrange.h'
**
** Created: Sun May 25 09:26:15 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../lcdrange.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *LCDRange::className() const
{
    return "LCDRange";
}

QMetaObject *LCDRange::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LCDRange( "LCDRange", &LCDRange::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LCDRange::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LCDRange", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LCDRange::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LCDRange", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LCDRange::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setValue", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "minVal", &static_QUType_int, 0, QUParameter::In },
	{ "maxVal", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"setRange", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "setValue(int)", &slot_0, QMetaData::Public },
	{ "setRange(int,int)", &slot_1, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"valueChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "valueChanged(int)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"LCDRange", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LCDRange.setMetaObject( metaObj );
    return metaObj;
}

void* LCDRange::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LCDRange" ) )
	return this;
    return QVBox::qt_cast( clname );
}

// SIGNAL valueChanged
void LCDRange::valueChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool LCDRange::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setValue((int)static_QUType_int.get(_o+1)); break;
    case 1: setRange((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool LCDRange::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: valueChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return QVBox::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool LCDRange::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool LCDRange::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
