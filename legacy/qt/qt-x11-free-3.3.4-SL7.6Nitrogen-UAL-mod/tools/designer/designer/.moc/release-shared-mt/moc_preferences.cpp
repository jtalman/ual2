/****************************************************************************
** Preferences meta object code from reading C++ file 'preferences.h'
**
** Created: Sun May 25 09:23:52 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../preferences.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Preferences::className() const
{
    return "Preferences";
}

QMetaObject *Preferences::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Preferences( "Preferences", &Preferences::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Preferences::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Preferences", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Preferences::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Preferences", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Preferences::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QUMethod slot_1 = {"init", 0, 0 };
    static const QUMethod slot_2 = {"destroy", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected },
	{ "init()", &slot_1, QMetaData::Protected },
	{ "destroy()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Preferences", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Preferences.setMetaObject( metaObj );
    return metaObj;
}

void* Preferences::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Preferences" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool Preferences::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    case 1: init(); break;
    case 2: destroy(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Preferences::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Preferences::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool Preferences::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
