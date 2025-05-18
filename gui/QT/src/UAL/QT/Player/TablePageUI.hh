/****************************************************************************
** Form interface generated from reading ui file 'TablePageUI.ui'
**
** Created: Tue Feb 25 09:21:06 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TABLEPAGEUI_H
#define TABLEPAGEUI_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QTable;

class TablePageUI : public QWidget
{
    Q_OBJECT

public:
    TablePageUI( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~TablePageUI();

    QFrame* frame3;
    QFrame* frame23;
    QLabel* label;
    QTable* table;

public slots:
    virtual void setValue( int row, int col );
    virtual void activateChanges();

protected:
    QHBoxLayout* TablePageUILayout;
    QVBoxLayout* frame3Layout;
    QSpacerItem* spacer5;
    QVBoxLayout* frame23Layout;
    QHBoxLayout* layout3;
    QSpacerItem* spacer4;

protected slots:
    virtual void languageChange();

};

#endif // TABLEPAGEUI_H
