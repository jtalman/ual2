/****************************************************************************
** Form interface generated from reading ui file 'remotectrl.ui'
**
** Created: Sun May 25 09:29:46 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef REMOTECTRL_H
#define REMOTECTRL_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QPushButton;

class RemoteCtrl : public QDialog
{
    Q_OBJECT

public:
    RemoteCtrl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~RemoteCtrl();

    QLineEdit* textToSend;
    QPushButton* sText;
    QPushButton* sImage;
    QPushButton* sPalette;
    QPushButton* quitButton;

protected:
    QHBoxLayout* RemoteCtrlLayout;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer5_2;
    QVBoxLayout* Layout6;
    QSpacerItem* Spacer6;

protected slots:
    virtual void languageChange();

};

#endif // REMOTECTRL_H
