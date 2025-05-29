/****************************************************************************
** TQtEmitter meta object code from reading C++ file 'TGQt.h'
**
** Created: Thu Jun 25 13:23:46 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TGQt.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtEmitter::className() const
{
    return "TQtEmitter";
}

QMetaObject *TQtEmitter::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtEmitter( "TQtEmitter", &TQtEmitter::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtEmitter::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtEmitter", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtEmitter::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtEmitter", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtEmitter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ "p", &static_QUType_varptr, "\x06", QUParameter::In }
    };
    static const QUMethod signal_0 = {"padPainted", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "padPainted(QPixmap*)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtEmitter", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtEmitter.setMetaObject( metaObj );
    return metaObj;
}

void* TQtEmitter::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtEmitter" ) )
	return this;
    return QObject::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL padPainted
void TQtEmitter::padPainted( QPixmap* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool TQtEmitter::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool TQtEmitter::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: padPainted((QPixmap*)static_QUType_varptr.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool TQtEmitter::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool TQtEmitter::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
