/****************************************************************************
** QTimer meta object code from reading C++ file 'qtimer.h'
**
** Created: Sun May 25 09:19:46 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../kernel/qtimer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QTimer::className() const
{
    return "QTimer";
}

QMetaObject *QTimer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QTimer( "QTimer", &QTimer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QTimer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QTimer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QTimer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QTimer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QTimer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod signal_0 = {"timeout", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "timeout()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QTimer", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QTimer.setMetaObject( metaObj );
    return metaObj;
}

void* QTimer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QTimer" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL timeout
void QTimer::timeout()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool QTimer::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool QTimer::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: timeout(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QTimer::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool QTimer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
