/****************************************************************************
** UAL::QT::ParameterViewer meta object code from reading C++ file 'ParameterViewer.hh'
**
** Created: Fri May 1 17:28:40 2020
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../ParameterViewer.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UAL::QT::ParameterViewer::className() const
{
    return "UAL::QT::ParameterViewer";
}

QMetaObject *UAL::QT::ParameterViewer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UAL__QT__ParameterViewer( "UAL::QT::ParameterViewer", &UAL::QT::ParameterViewer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UAL::QT::ParameterViewer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::QT::ParameterViewer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UAL::QT::ParameterViewer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UAL::QT::ParameterViewer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UAL::QT::ParameterViewer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = BasicEditor::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setValue", 2, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "setValue(int,int)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"UAL::QT::ParameterViewer", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UAL__QT__ParameterViewer.setMetaObject( metaObj );
    return metaObj;
}

void* UAL::QT::ParameterViewer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UAL::QT::ParameterViewer" ) )
	return this;
    return BasicEditor::qt_cast( clname );
}

bool UAL::QT::ParameterViewer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setValue((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return BasicEditor::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UAL::QT::ParameterViewer::qt_emit( int _id, QUObject* _o )
{
    return BasicEditor::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UAL::QT::ParameterViewer::qt_property( int id, int f, QVariant* v)
{
    return BasicEditor::qt_property( id, f, v);
}

bool UAL::QT::ParameterViewer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
