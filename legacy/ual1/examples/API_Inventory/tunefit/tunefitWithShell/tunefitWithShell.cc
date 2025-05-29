#include<stdlib.h>   //   exit()
#include<iostream>
#include "Main/Teapot.h"
#include "UAL/UI/Shell.hh"

using namespace UAL;

int main(int argc, char*argv[]){
 if(argc!=3){
  std::cout << "Usage:\n./tunefitWithShell 28.685 29.675\n";
  std::cout << "argv[0] is this executable - ./get_twiss\n";
  std::cout << "argv[1] is tuneX           - 28.685\n";
  std::cout << "argv[2] is tuneY           - 29.675\n";
  exit(0);
 }
 double tuneX = atof(argv[1]);
 double tuneY = atof(argv[2]);

 UAL::Shell shell;

 std::string sxfFile   = "../working.sxf";

 const UAL::Arguments bootStrap = Arguments();
 bootStrap << Arg("order", 5);                                   //   ???
 shell.setMapAttributes( bootStrap );

 double mass   = UAL::pmass;
 double gamma  = 1;
 double energy = gamma*mass;
 double charge = 1;

 UAL::Arguments forRunParams = Arguments();
 forRunParams << Arg("lattice", "ring");
 forRunParams << Arg("types",   "Sbend")      << Arg("ir", 2);
 forRunParams << Arg("types",   "Quadrupole") << Arg("ir", 2);
 forRunParams << Arg("order",   1);
 forRunParams << Arg("energy",  energy);
 forRunParams << Arg("mass",    mass);
 forRunParams << Arg("charge",  charge);
 forRunParams << Arg("tunex",   tuneX);
 forRunParams << Arg("tuney",   tuneY);
 forRunParams << Arg("b1f",     "^qf$");
 forRunParams << Arg("b1d",     "^qd$");
 const UAL::Arguments forRun = forRunParams;

// shell.setBeamAttributes(Args() << Arg("energy", energy) << Arg("mass", mass)
//			  << Arg("charge",charge));

std::cerr << "File: " << __FILE__ << " line: " << __LINE__ << "\n";
 shell.readSXF(Args()  << Arg("file", sxfFile.c_str()));
 shell.addSplit( forRun );
 shell.use( forRun );

 shell.tunefit( forRun );
// shell.twiss( forRun );

 return 1;
}
