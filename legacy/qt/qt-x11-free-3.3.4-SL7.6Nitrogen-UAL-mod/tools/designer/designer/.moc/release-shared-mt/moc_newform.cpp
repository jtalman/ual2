/****************************************************************************
** NewFormBase meta object code from reading C++ file 'newform.h'
**
** Created: Sun May 25 09:23:49 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../newform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *NewFormBase::className() const
{
    return "NewFormBase";
}

QMetaObject *NewFormBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_NewFormBase( "NewFormBase", &NewFormBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString NewFormBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NewFormBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString NewFormBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NewFormBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* NewFormBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "QIconViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"itemChanged", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"projectChanged", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected },
	{ "itemChanged(QIconViewItem*)", &slot_1, QMetaData::Protected },
	{ "projectChanged(const QString&)", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"NewFormBase", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_NewFormBase.setMetaObject( metaObj );
    return metaObj;
}

void* NewFormBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "NewFormBase" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool NewFormBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    case 1: itemChanged((QIconViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: projectChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool NewFormBase::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool NewFormBase::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool NewFormBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
