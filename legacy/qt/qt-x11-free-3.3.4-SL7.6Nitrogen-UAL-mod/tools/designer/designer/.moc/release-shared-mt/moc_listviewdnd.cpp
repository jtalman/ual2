/****************************************************************************
** ListViewDnd meta object code from reading C++ file 'listviewdnd.h'
**
** Created: Sun May 25 09:23:42 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../listviewdnd.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ListViewDnd::className() const
{
    return "ListViewDnd";
}

QMetaObject *ListViewDnd::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ListViewDnd( "ListViewDnd", &ListViewDnd::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ListViewDnd::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ListViewDnd", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ListViewDnd::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ListViewDnd", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ListViewDnd::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ListDnd::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"confirmDrop", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "confirmDrop(QListViewItem*)", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod signal_0 = {"dropped", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "dropped(QListViewItem*)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ListViewDnd", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ListViewDnd.setMetaObject( metaObj );
    return metaObj;
}

void* ListViewDnd::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ListViewDnd" ) )
	return this;
    return ListDnd::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL dropped
void ListViewDnd::dropped( QListViewItem* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool ListViewDnd::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: confirmDrop((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return ListDnd::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ListViewDnd::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: dropped((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return ListDnd::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ListViewDnd::qt_property( int id, int f, QVariant* v)
{
    return ListDnd::qt_property( id, f, v);
}

bool ListViewDnd::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
