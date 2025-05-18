/****************************************************************************
** UAL::USPAS::BasicPlayer meta object code from reading C++ file 'BasicPlayer.hh'
**
** Created: Thu Jun 2 06:57:16 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.2.0b2   edited Jun 18 16:20 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BasicPlayer.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0b2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UAL::USPAS::BasicPlayer::className() const
{
    return "UAL::USPAS::BasicPlayer";
}

QMetaObject *UAL::USPAS::BasicPlayer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UAL__USPAS__BasicPlayer( "UAL::USPAS::BasicPlayer", &UAL::USPAS::BasicPlayer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UAL::USPAS::BasicPlayer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::USPAS::BasicPlayer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UAL::USPAS::BasicPlayer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::USPAS::BasicPlayer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UAL::USPAS::BasicPlayer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = UAL::QT::BasicPlayer::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"UAL::USPAS::BasicPlayer", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UAL__USPAS__BasicPlayer.setMetaObject( metaObj );
    return metaObj;
}

void* UAL::USPAS::BasicPlayer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UAL::USPAS::BasicPlayer" ) )
	return this;
    return UAL::QT::BasicPlayer::qt_cast( clname );
}

bool UAL::USPAS::BasicPlayer::qt_invoke( int _id, QUObject* _o )
{
    return UAL::QT::BasicPlayer::qt_invoke(_id,_o);
}

bool UAL::USPAS::BasicPlayer::qt_emit( int _id, QUObject* _o )
{
    return UAL::QT::BasicPlayer::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UAL::USPAS::BasicPlayer::qt_property( int id, int f, QVariant* v)
{
    return UAL::QT::BasicPlayer::qt_property( id, f, v);
}

bool UAL::USPAS::BasicPlayer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
