/****************************************************************************
** Form interface generated from reading ui file 'about.ui'
**
** Created: Sun May 25 09:21:27 2025
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AboutDialog();

    QLabel* aboutPixmap;
    QLabel* aboutVersion;
    QLabel* aboutCopyright;
    QLabel* aboutLicense;
    QPushButton* PushButton1;

protected:
    QVBoxLayout* AboutDialogLayout;
    QSpacerItem* Spacer3;
    QHBoxLayout* Layout1;
    QSpacerItem* Spacer2;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // ABOUTDIALOG_H
