/****************************************************************************
** TopicChooser meta object code from reading C++ file 'topicchooserimpl.h'
**
** Created: Sun May 25 09:25:09 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../topicchooserimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TopicChooser::className() const
{
    return "TopicChooser";
}

QMetaObject *TopicChooser::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TopicChooser( "TopicChooser", &TopicChooser::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TopicChooser::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TopicChooser", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TopicChooser::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TopicChooser", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TopicChooser::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = TopicChooserBase::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"TopicChooser", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TopicChooser.setMetaObject( metaObj );
    return metaObj;
}

void* TopicChooser::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TopicChooser" ) )
	return this;
    return TopicChooserBase::qt_cast( clname );
}

bool TopicChooser::qt_invoke( int _id, QUObject* _o )
{
    return TopicChooserBase::qt_invoke(_id,_o);
}

bool TopicChooser::qt_emit( int _id, QUObject* _o )
{
    return TopicChooserBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TopicChooser::qt_property( int id, int f, QVariant* v)
{
    return TopicChooserBase::qt_property( id, f, v);
}

bool TopicChooser::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
