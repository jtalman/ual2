/****************************************************************************
** Form interface generated from reading ui file 'mainfilesettings.ui'
**
** Created: Sun May 25 09:24:28 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CPPMAINFILE_H
#define CPPMAINFILE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QListBox;
class QListBoxItem;
class QPushButton;
struct QUnknownInterface;

class CppMainFile : public QDialog
{
    Q_OBJECT

public:
    CppMainFile( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CppMainFile();

    QLabel* TextLabel1;
    QLineEdit* editFileName;
    QLabel* TextLabel2;
    QListBox* listForms;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void init();
    virtual void setup( QUnknownInterface * appIface );
    virtual void updateOkButton();

protected:
    QGridLayout* CppMainFileLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // CPPMAINFILE_H
