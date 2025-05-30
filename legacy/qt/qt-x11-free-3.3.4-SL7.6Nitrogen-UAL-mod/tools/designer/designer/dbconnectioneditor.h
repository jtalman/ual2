/****************************************************************************
** Form interface generated from reading ui file 'dbconnectioneditor.ui'
**
** Created: Sun May 25 09:21:27 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef DATABASECONNECTIONEDITORBASE_H
#define DATABASECONNECTIONEDITORBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QGroupBox;

class DatabaseConnectionEditorBase : public QDialog
{
    Q_OBJECT

public:
    DatabaseConnectionEditorBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DatabaseConnectionEditorBase();

    QPushButton* PushButton2;
    QPushButton* PushButton1;
    QGroupBox* grp;

protected:
    QGridLayout* DatabaseConnectionEditorBaseLayout;
    QSpacerItem* Spacer1;
    QGridLayout* grpLayout;
    QSpacerItem* Spacer2;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();


};

#endif // DATABASECONNECTIONEDITORBASE_H
