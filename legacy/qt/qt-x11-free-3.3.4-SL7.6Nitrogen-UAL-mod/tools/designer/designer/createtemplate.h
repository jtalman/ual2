/****************************************************************************
** Form interface generated from reading ui file 'createtemplate.ui'
**
** Created: Sun May 25 09:21:27 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CREATETEMPLATE_H
#define CREATETEMPLATE_H

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

class CreateTemplate : public QDialog
{
    Q_OBJECT

public:
    CreateTemplate( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CreateTemplate();

    QLabel* TextLabel1;
    QLineEdit* editName;
    QListBox* listClass;
    QPushButton* buttonCreate;
    QPushButton* PushButton1;
    QLabel* TextLabel2;

protected:
    QGridLayout* CreateTemplateLayout;
    QSpacerItem* Spacer2;
    QHBoxLayout* Layout1;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // CREATETEMPLATE_H
