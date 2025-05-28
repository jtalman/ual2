/****************************************************************************
** TQtClientWidget meta object code from reading C++ file 'TQtClientWidget.h'
**
** Created: Thu Jun 25 13:23:47 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtClientWidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtClientWidget::className() const
{
    return "TQtClientWidget";
}

QMetaObject *TQtClientWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtClientWidget( "TQtClientWidget", &TQtClientWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtClientWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtClientWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtClientWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtClientWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtClientWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUMethod slot_0 = {"Disconnect", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"Accelerate", 1, param_slot_1 };
    static const QUMethod slot_2 = {"polish", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Disconnect()", &slot_0, QMetaData::Protected },
	{ "Accelerate(int)", &slot_1, QMetaData::Public },
	{ "polish()", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtClientWidget", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtClientWidget.setMetaObject( metaObj );
    return metaObj;
}

void* TQtClientWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtClientWidget" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool TQtClientWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Disconnect(); break;
    case 1: Accelerate((int)static_QUType_int.get(_o+1)); break;
    case 2: polish(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtClientWidget::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TQtClientWidget::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool TQtClientWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
