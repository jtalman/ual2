/****************************************************************************
** Form interface generated from reading ui file 'gotolinedialog.ui'
**
** Created: Sun May 25 09:21:27 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QSpinBox;
class QPushButton;
struct EditorInterface;

class GotoLineDialog : public QDialog
{
    Q_OBJECT

public:
    GotoLineDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~GotoLineDialog();

    QLabel* TextLabel1;
    QSpinBox* spinLine;
    QPushButton* PushButton2;
    QPushButton* PushButton1;

public slots:
    virtual void gotoLine();
    virtual void setEditor( EditorInterface * e );

protected:
    EditorInterface *editor;

    QGridLayout* GotoLineDialogLayout;
    QSpacerItem* Spacer3;
    QHBoxLayout* Layout1;
    QSpacerItem* Spacer2;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();


};

#endif // GOTOLINEDIALOG_H
