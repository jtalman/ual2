/****************************************************************************
** Form interface generated from reading ui file 'actioneditor.ui'
**
** Created: Sun May 25 09:21:27 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ACTIONEDITORBASE_H
#define ACTIONEDITORBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class ActionListView;
class QToolButton;
class QListViewItem;

class ActionEditorBase : public QWidget
{
    Q_OBJECT

public:
    ActionEditorBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ActionEditorBase();

    QToolButton* buttonNewAction;
    QToolButton* buttonDeleteAction;
    QToolButton* buttonConnect;
    ActionListView* listActions;

protected:
    QVBoxLayout* ActionEditorBaseLayout;
    QHBoxLayout* Layout2;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();
    virtual void connectionsClicked();
    virtual void currentActionChanged( QListViewItem * );
    virtual void deleteAction();
    virtual void newAction();


private:
    QPixmap image0;

};

#endif // ACTIONEDITORBASE_H
