/****************************************************************************
** TQtClientFilter meta object code from reading C++ file 'TQtClientFilter.h'
**
** Created: Thu Jun 25 13:23:44 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtClientFilter.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtClientFilter::className() const
{
    return "TQtClientFilter";
}

QMetaObject *TQtClientFilter::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtClientFilter( "TQtClientFilter", &TQtClientFilter::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtClientFilter::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtClientFilter", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtClientFilter::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtClientFilter", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtClientFilter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "TQtClientWidget", QUParameter::In }
    };
    static const QUMethod slot_0 = {"AppendButtonGrab", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "TQtClientWidget", QUParameter::In }
    };
    static const QUMethod slot_1 = {"AppendPointerGrab", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_ptr, "QObject", QUParameter::In }
    };
    static const QUMethod slot_2 = {"RemoveButtonGrab", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_ptr, "QObject", QUParameter::In }
    };
    static const QUMethod slot_3 = {"RemovePointerGrab", 1, param_slot_3 };
    static const QMetaData slot_tbl[] = {
	{ "AppendButtonGrab(TQtClientWidget*)", &slot_0, QMetaData::Public },
	{ "AppendPointerGrab(TQtClientWidget*)", &slot_1, QMetaData::Public },
	{ "RemoveButtonGrab(QObject*)", &slot_2, QMetaData::Public },
	{ "RemovePointerGrab(QObject*)", &slot_3, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtClientFilter", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtClientFilter.setMetaObject( metaObj );
    return metaObj;
}

void* TQtClientFilter::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtClientFilter" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool TQtClientFilter::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AppendButtonGrab((TQtClientWidget*)static_QUType_ptr.get(_o+1)); break;
    case 1: AppendPointerGrab((TQtClientWidget*)static_QUType_ptr.get(_o+1)); break;
    case 2: RemoveButtonGrab((QObject*)static_QUType_ptr.get(_o+1)); break;
    case 3: RemovePointerGrab((QObject*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtClientFilter::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtClientFilter::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool TQtClientFilter::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
