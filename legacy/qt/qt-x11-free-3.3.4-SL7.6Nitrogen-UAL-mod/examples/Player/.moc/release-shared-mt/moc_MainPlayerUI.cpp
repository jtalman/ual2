/****************************************************************************
** UAL::QT::MainPlayerUI meta object code from reading C++ file 'MainPlayerUI.h'
**
** Created: Fri May 1 17:28:45 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../MainPlayerUI.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UAL::QT::MainPlayerUI::className() const
{
    return "UAL::QT::MainPlayerUI";
}

QMetaObject *UAL::QT::MainPlayerUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UAL__QT__MainPlayerUI( "UAL::QT::MainPlayerUI", &UAL::QT::MainPlayerUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UAL::QT::MainPlayerUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::QT::MainPlayerUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UAL::QT::MainPlayerUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::QT::MainPlayerUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UAL::QT::MainPlayerUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"startRun", 0, 0 };
    static const QUMethod slot_1 = {"pauseRun", 0, 0 };
    static const QUMethod slot_2 = {"continueRun", 0, 0 };
    static const QUMethod slot_3 = {"stopRun", 0, 0 };
    static const QUMethod slot_4 = {"initRun", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_5 = {"showPage", 1, param_slot_5 };
    static const QUMethod slot_6 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "startRun()", &slot_0, QMetaData::Public },
	{ "pauseRun()", &slot_1, QMetaData::Public },
	{ "continueRun()", &slot_2, QMetaData::Public },
	{ "stopRun()", &slot_3, QMetaData::Public },
	{ "initRun()", &slot_4, QMetaData::Public },
	{ "showPage(QListViewItem*)", &slot_5, QMetaData::Public },
	{ "languageChange()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"UAL::QT::MainPlayerUI", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UAL__QT__MainPlayerUI.setMetaObject( metaObj );
    return metaObj;
}

void* UAL::QT::MainPlayerUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UAL::QT::MainPlayerUI" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool UAL::QT::MainPlayerUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: startRun(); break;
    case 1: pauseRun(); break;
    case 2: continueRun(); break;
    case 3: stopRun(); break;
    case 4: initRun(); break;
    case 5: showPage((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 6: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UAL::QT::MainPlayerUI::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UAL::QT::MainPlayerUI::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool UAL::QT::MainPlayerUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
