/****************************************************************************
** StatusPicker meta object code from reading C++ file 'main.h'
**
** Created: Sun May 25 09:30:38 2025
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../main.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *StatusPicker::className() const
{
    return "StatusPicker";
}

QMetaObject *StatusPicker::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StatusPicker( "StatusPicker", &StatusPicker::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StatusPicker::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StatusPicker", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StatusPicker::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StatusPicker", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StatusPicker::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QComboBox::staticMetaObject();
#ifndef QT_NO_PROPERTIES
    static const QMetaProperty props_tbl[1] = {
 	{ "int","statusid", 0x10000003, &StatusPicker::metaObj, 0, -1 }
    };
#endif // QT_NO_PROPERTIES
    metaObj = QMetaObject::new_metaobject(
	"StatusPicker", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	props_tbl, 1,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StatusPicker.setMetaObject( metaObj );
    return metaObj;
}

void* StatusPicker::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StatusPicker" ) )
	return this;
    return QComboBox::qt_cast( clname );
}

bool StatusPicker::qt_invoke( int _id, QUObject* _o )
{
    return QComboBox::qt_invoke(_id,_o);
}

bool StatusPicker::qt_emit( int _id, QUObject* _o )
{
    return QComboBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StatusPicker::qt_property( int id, int f, QVariant* v)
{
    switch ( id - staticMetaObject()->propertyOffset() ) {
    case 0: switch( f ) {
	case 0: setStatusId(v->asInt()); break;
	case 1: *v = QVariant( this->statusId() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    default:
	return QComboBox::qt_property( id, f, v );
    }
    return TRUE;
}

bool StatusPicker::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CustomTable::className() const
{
    return "CustomTable";
}

QMetaObject *CustomTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CustomTable( "CustomTable", &CustomTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CustomTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CustomTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CustomTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CustomTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CustomTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDataTable::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"CustomTable", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CustomTable.setMetaObject( metaObj );
    return metaObj;
}

void* CustomTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CustomTable" ) )
	return this;
    return QDataTable::qt_cast( clname );
}

bool CustomTable::qt_invoke( int _id, QUObject* _o )
{
    return QDataTable::qt_invoke(_id,_o);
}

bool CustomTable::qt_emit( int _id, QUObject* _o )
{
    return QDataTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CustomTable::qt_property( int id, int f, QVariant* v)
{
    return QDataTable::qt_property( id, f, v);
}

bool CustomTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CustomSqlEditorFactory::className() const
{
    return "CustomSqlEditorFactory";
}

QMetaObject *CustomSqlEditorFactory::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CustomSqlEditorFactory( "CustomSqlEditorFactory", &CustomSqlEditorFactory::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CustomSqlEditorFactory::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CustomSqlEditorFactory", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CustomSqlEditorFactory::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CustomSqlEditorFactory", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CustomSqlEditorFactory::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QSqlEditorFactory::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"CustomSqlEditorFactory", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CustomSqlEditorFactory.setMetaObject( metaObj );
    return metaObj;
}

void* CustomSqlEditorFactory::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CustomSqlEditorFactory" ) )
	return this;
    return QSqlEditorFactory::qt_cast( clname );
}

bool CustomSqlEditorFactory::qt_invoke( int _id, QUObject* _o )
{
    return QSqlEditorFactory::qt_invoke(_id,_o);
}

bool CustomSqlEditorFactory::qt_emit( int _id, QUObject* _o )
{
    return QSqlEditorFactory::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CustomSqlEditorFactory::qt_property( int id, int f, QVariant* v)
{
    return QSqlEditorFactory::qt_property( id, f, v);
}

bool CustomSqlEditorFactory::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
