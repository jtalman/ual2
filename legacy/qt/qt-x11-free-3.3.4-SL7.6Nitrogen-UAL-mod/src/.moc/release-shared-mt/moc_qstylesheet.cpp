/****************************************************************************
** QStyleSheet meta object code from reading C++ file 'qstylesheet.h'
**
** Created: Sun May 25 09:19:46 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../kernel/qstylesheet.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QStyleSheet::className() const
{
    return "QStyleSheet";
}

QMetaObject *QStyleSheet::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QStyleSheet( "QStyleSheet", &QStyleSheet::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QStyleSheet::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QStyleSheet", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QStyleSheet::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QStyleSheet", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QStyleSheet::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"QStyleSheet", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QStyleSheet.setMetaObject( metaObj );
    return metaObj;
}

void* QStyleSheet::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QStyleSheet" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool QStyleSheet::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool QStyleSheet::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool QStyleSheet::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool QStyleSheet::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
