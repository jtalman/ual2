/****************************************************************************
** Form interface generated from reading ui file 'serverbase.ui'
**
** Created: Sun May 25 09:29:33 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SERVERINFOBASE_H
#define SERVERINFOBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTextEdit;
class QPushButton;

class ServerInfoBase : public QWidget
{
    Q_OBJECT

public:
    ServerInfoBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ServerInfoBase();

    QLabel* TextLabel1;
    QTextEdit* infoText;
    QPushButton* btnQuit;

protected:
    QVBoxLayout* ServerInfoBaseLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Spacer2;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // SERVERINFOBASE_H
