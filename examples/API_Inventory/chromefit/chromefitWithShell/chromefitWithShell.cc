#include<stdlib.h>   //   exit()
#include<iostream>
#include "Main/Teapot.h"
#include "UAL/UI/Shell.hh"

using namespace UAL;

int main(int argc, char*argv[]){
 if(argc!=3){
  std::cout << "Usage:\n./chromefitWithShell 2. 2.\n";
  std::cout << "argv[0] is this executable - ./chromefitWithShell\n";
  std::cout << "argv[1] is chromeX           - 2.\n";
  std::cout << "argv[2] is chromeY           - 2.\n";
  exit(0);
 }
 double chromeX = atof(argv[1]);
 double chromeY = atof(argv[2]);

 UAL::Shell shell;

 std::string sxfFile   = "../working.sxf";

 const UAL::Arguments bootStrap = Arguments();
 bootStrap << Arg("order", 2);                                   //   ???
 shell.setMapAttributes( bootStrap );

 double mass   = UAL::pmass;
 double gamma  = 120.315;
 double energy = gamma*mass;
 double charge = 1;

 UAL::Arguments forRunParams = Arguments();
 forRunParams << Arg("lattice", "ring");
 forRunParams << Arg("types",   "Sbend")      << Arg("ir", 1);
 forRunParams << Arg("types",   "Quadrupole") << Arg("ir", 32);
 forRunParams << Arg("order",   1);
 forRunParams << Arg("energy",  energy);
 forRunParams << Arg("mass",    mass);
 forRunParams << Arg("charge",  charge);
 forRunParams << Arg("chromx",   chromeX);
 forRunParams << Arg("chromy",   chromeY);
// forRunParams << Arg("b1f",     "^qf$");
// forRunParams << Arg("b1d",     "^qd$");
 forRunParams << Arg("b2f",     "^sf");
 forRunParams << Arg("b2d",     "^sd");
 const UAL::Arguments forRun = forRunParams;

// shell.setBeamAttributes(Args() << Arg("energy", energy) << Arg("mass", mass)
//			  << Arg("charge",charge));

std::cerr << "File: " << __FILE__ << " line: " << __LINE__ << "\n";
 shell.readSXF(Args()  << Arg("file", sxfFile.c_str()));
 shell.addSplit( forRun );
 shell.use( forRun );

 shell.chromfit( forRun );
// shell.twiss( forRun );

 return 1;
}
