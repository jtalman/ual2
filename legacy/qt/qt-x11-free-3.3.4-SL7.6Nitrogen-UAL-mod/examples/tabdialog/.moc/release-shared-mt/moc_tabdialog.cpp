/****************************************************************************
** TabDialog meta object code from reading C++ file 'tabdialog.h'
**
** Created: Sun May 25 09:28:25 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../tabdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TabDialog::className() const
{
    return "TabDialog";
}

QMetaObject *TabDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TabDialog( "TabDialog", &TabDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TabDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TabDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TabDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TabDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TabDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"TabDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TabDialog.setMetaObject( metaObj );
    return metaObj;
}

void* TabDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TabDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool TabDialog::qt_invoke( int _id, QUObject* _o )
{
    return QTabDialog::qt_invoke(_id,_o);
}

bool TabDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TabDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool TabDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
