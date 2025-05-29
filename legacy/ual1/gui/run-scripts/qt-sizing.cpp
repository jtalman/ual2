#include<qapplication.h>
//#include<qguiapplication.h>
#include<qpushbutton.h>
#include<qwidget.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int w = 500;
    int h = 300;
    int x = QApplication::desktop()->width()/2 - w/2;
    int y = QApplication::desktop()->height()/2 - h/2;

    int width = QApplication::desktop()->width();
    int height = QApplication::desktop()->height();
 
//    setGeometry(x, y, w, h);

    QWidget *window = new QWidget;

    window->resize(w    , h     );
//  window->resize(width, height);

    QPushButton *button = new QPushButton("Click me", window);
    button->show();
    window->show();
    return a.exec();
}
