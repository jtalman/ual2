/****************************************************************************
** UAL::QT::BasicPlayer meta object code from reading C++ file 'BasicPlayer.hh'
**
** Created: Fri May 1 17:28:36 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../BasicPlayer.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UAL::QT::BasicPlayer::className() const
{
    return "UAL::QT::BasicPlayer";
}

QMetaObject *UAL::QT::BasicPlayer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UAL__QT__BasicPlayer( "UAL::QT::BasicPlayer", &UAL::QT::BasicPlayer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UAL::QT::BasicPlayer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::QT::BasicPlayer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UAL::QT::BasicPlayer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::QT::BasicPlayer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UAL::QT::BasicPlayer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MainPlayerUI::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_0 = {"openApdf", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "openApdf()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"UAL::QT::BasicPlayer", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UAL__QT__BasicPlayer.setMetaObject( metaObj );
    return metaObj;
}

void* UAL::QT::BasicPlayer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UAL::QT::BasicPlayer" ) )
	return this;
    return MainPlayerUI::qt_cast( clname );
}

bool UAL::QT::BasicPlayer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: static_QUType_bool.set(_o,openApdf()); break;
    default:
	return MainPlayerUI::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UAL::QT::BasicPlayer::qt_emit( int _id, QUObject* _o )
{
    return MainPlayerUI::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UAL::QT::BasicPlayer::qt_property( int id, int f, QVariant* v)
{
    return MainPlayerUI::qt_property( id, f, v);
}

bool UAL::QT::BasicPlayer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
