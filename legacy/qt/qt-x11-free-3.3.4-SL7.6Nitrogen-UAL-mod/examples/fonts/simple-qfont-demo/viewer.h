/* $Id: qt/viewer.h   3.3.4   edited May 27 2003 $ */

#ifndef VIEWER_H
#define VIEWER_H     


#include <qwidget.h>
#include <qfont.h>

class QTextView;
class QPushButton;

class Viewer : public QWidget
{
Q_OBJECT

public:   
    Viewer();

private slots:
    void setDefault();
    void setSansSerif();
    void setItalics();

private:
    void setFontSubstitutions();
    void layout();
    void showFontInfo( QFont & );

    QTextView * greetings; 
    QTextView * fontInfo;

    QPushButton * defaultButton;
    QPushButton * sansSerifButton;
    QPushButton * italicsButton;
};

#endif
