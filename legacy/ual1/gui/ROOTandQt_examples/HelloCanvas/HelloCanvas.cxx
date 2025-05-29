#include<qapplication.h>
#include"TGraph.h"
#include"TQtWidget.h"
#include"TCanvas.h"

int main( int argc, char **argv ) {
 QApplication *app = new QApplication(argc, argv);
 TQtWidget *MyWidget= new TQtWidget(0,"MyWidget");
// Create any other Qt-widget here
 MyWidget->show();
 MyWidget->GetCanvas()->cd();
 TGraph *mygraph;
 float x[3] = {1,2,3};
 float y[3] = {1.5, 3.0, 4.5};
 mygraph  = new TGraph(3,x,y);
 mygraph->SetMarkerStyle(20);
 mygraph->Draw("AP");
 MyWidget->GetCanvas()->Update();
 app->exec();
 return 0;
}
