#include<qapplication.h>
#include<qpushbutton.h>
#include<qwidget.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget *window = new QWidget;
    QPushButton *button = new QPushButton("Click me", window);
    button->show();
    window->show();
    return a.exec();
}
