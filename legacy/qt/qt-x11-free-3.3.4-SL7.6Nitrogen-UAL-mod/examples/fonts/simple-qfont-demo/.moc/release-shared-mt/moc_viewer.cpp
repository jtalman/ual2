/****************************************************************************
** Viewer meta object code from reading C++ file 'viewer.h'
**
** Created: Sun May 25 09:27:30 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../viewer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Viewer::className() const
{
    return "Viewer";
}

QMetaObject *Viewer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Viewer( "Viewer", &Viewer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Viewer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Viewer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Viewer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Viewer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Viewer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"setDefault", 0, 0 };
    static const QUMethod slot_1 = {"setSansSerif", 0, 0 };
    static const QUMethod slot_2 = {"setItalics", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "setDefault()", &slot_0, QMetaData::Private },
	{ "setSansSerif()", &slot_1, QMetaData::Private },
	{ "setItalics()", &slot_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Viewer", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Viewer.setMetaObject( metaObj );
    return metaObj;
}

void* Viewer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Viewer" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool Viewer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setDefault(); break;
    case 1: setSansSerif(); break;
    case 2: setItalics(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Viewer::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Viewer::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool Viewer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
