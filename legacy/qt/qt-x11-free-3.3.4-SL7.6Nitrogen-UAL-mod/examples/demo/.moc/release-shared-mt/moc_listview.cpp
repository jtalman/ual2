/****************************************************************************
** ListView meta object code from reading C++ file 'listview.h'
**
** Created: Sun May 25 09:27:10 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../dnd/listview.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ListView::className() const
{
    return "ListView";
}

QMetaObject *ListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ListView( "ListView", &ListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"ListView", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ListView.setMetaObject( metaObj );
    return metaObj;
}

void* ListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ListView" ) )
	return this;
    return QListView::qt_cast( clname );
}

bool ListView::qt_invoke( int _id, QUObject* _o )
{
    return QListView::qt_invoke(_id,_o);
}

bool ListView::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool ListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
