/****************************************************************************
** TQtWidget meta object code from reading C++ file 'TQtWidget.h'
**
** Created: Thu Jun 25 13:23:45 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../inc/TQtWidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TQtWidget::className() const
{
    return "TQtWidget";
}

QMetaObject *TQtWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TQtWidget( "TQtWidget", &TQtWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TQtWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TQtWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TQtWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TQtWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"cd", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "subpadnumber", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"cd", 1, param_slot_1 };
    static const QUMethod slot_2 = {"Disconnect", 0, 0 };
    static const QUMethod slot_3 = {"Refresh", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "fileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"Save", 2, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "fileName", &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"Save", 2, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "fileName", &static_QUType_QString, 0, QUParameter::In },
	{ "format", &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"Save", 3, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "fileName", &static_QUType_QString, 0, QUParameter::In },
	{ "format", &static_QUType_charstar, 0, QUParameter::In },
	{ "quality", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"Save", 4, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "fileName", &static_QUType_charstar, 0, QUParameter::In },
	{ "format", &static_QUType_charstar, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"Save", 3, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "fileName", &static_QUType_charstar, 0, QUParameter::In },
	{ "format", &static_QUType_charstar, 0, QUParameter::In },
	{ "quality", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"Save", 4, param_slot_9 };
    static const QMetaData slot_tbl[] = {
	{ "cd()", &slot_0, QMetaData::Public },
	{ "cd(int)", &slot_1, QMetaData::Public },
	{ "Disconnect()", &slot_2, QMetaData::Public },
	{ "Refresh()", &slot_3, QMetaData::Public },
	{ "Save(const QString&)", &slot_4, QMetaData::Public },
	{ "Save(const char*)", &slot_5, QMetaData::Public },
	{ "Save(const QString&,const char*)", &slot_6, QMetaData::Public },
	{ "Save(const QString&,const char*,int)", &slot_7, QMetaData::Public },
	{ "Save(const char*,const char*)", &slot_8, QMetaData::Public },
	{ "Save(const char*,const char*,int)", &slot_9, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"CanvasPainted", 0, 0 };
    static const QUParameter param_signal_1[] = {
	{ "ok", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"Saved", 1, param_signal_1 };
    static const QUMethod signal_2 = {"polish", 0, 0 };
    static const QUParameter param_signal_3[] = {
	{ "selected", &static_QUType_ptr, "TObject", QUParameter::In },
	{ "event", &static_QUType_ptr, "unsigned int", QUParameter::In },
	{ "c", &static_QUType_ptr, "TCanvas", QUParameter::In }
    };
    static const QUMethod signal_3 = {"RootEventProcessed", 3, param_signal_3 };
    static const QMetaData signal_tbl[] = {
	{ "CanvasPainted()", &signal_0, QMetaData::Public },
	{ "Saved(bool)", &signal_1, QMetaData::Public },
	{ "polish()", &signal_2, QMetaData::Public },
	{ "RootEventProcessed(TObject*,unsigned int,TCanvas*)", &signal_3, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"TQtWidget", parentObject,
	slot_tbl, 10,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TQtWidget.setMetaObject( metaObj );
    return metaObj;
}

void* TQtWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TQtWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL CanvasPainted
void TQtWidget::CanvasPainted()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL Saved
void TQtWidget::Saved( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL polish
void TQtWidget::polish()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL RootEventProcessed
void TQtWidget::RootEventProcessed( TObject* t0, unsigned int t1, TCanvas* t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_ptr.set(o+2,&t1);
    static_QUType_ptr.set(o+3,t2);
    activate_signal( clist, o );
}

bool TQtWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: cd(); break;
    case 1: cd((int)static_QUType_int.get(_o+1)); break;
    case 2: Disconnect(); break;
    case 3: Refresh(); break;
    case 4: static_QUType_bool.set(_o,Save((const QString&)static_QUType_QString.get(_o+1))); break;
    case 5: static_QUType_bool.set(_o,Save((const char*)static_QUType_charstar.get(_o+1))); break;
    case 6: static_QUType_bool.set(_o,Save((const QString&)static_QUType_QString.get(_o+1),(const char*)static_QUType_charstar.get(_o+2))); break;
    case 7: static_QUType_bool.set(_o,Save((const QString&)static_QUType_QString.get(_o+1),(const char*)static_QUType_charstar.get(_o+2),(int)static_QUType_int.get(_o+3))); break;
    case 8: static_QUType_bool.set(_o,Save((const char*)static_QUType_charstar.get(_o+1),(const char*)static_QUType_charstar.get(_o+2))); break;
    case 9: static_QUType_bool.set(_o,Save((const char*)static_QUType_charstar.get(_o+1),(const char*)static_QUType_charstar.get(_o+2),(int)static_QUType_int.get(_o+3))); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TQtWidget::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: CanvasPainted(); break;
    case 1: Saved((bool)static_QUType_bool.get(_o+1)); break;
    case 2: polish(); break;
    case 3: RootEventProcessed((TObject*)static_QUType_ptr.get(_o+1),(unsigned int)(*((unsigned int*)static_QUType_ptr.get(_o+2))),(TCanvas*)static_QUType_ptr.get(_o+3)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool TQtWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool TQtWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
