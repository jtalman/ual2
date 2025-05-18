#include<stdlib.h>   //   exit()
#include<iostream>
#include "Main/Teapot.h"
#include "UAL/UI/Shell.hh"

using namespace UAL;

int main(int argc, char*argv[]){
 if(argc!=1){
  std::cout << "usage: ./get_twiss)\n";
  std::cout << "argv[0] is this executable - ./get_twiss\n";
  exit(0);
 }

 UAL::Shell shell;

 std::string sxfFile   = "./working.sxf";
 std::string twissFile = "./working.twiss";

// shell.readSXF(Args()  << Arg("file", sxfFile.c_str()));

 const UAL::Arguments bootStrap = Arguments();
 bootStrap << Arg("order", 5);                                   //   ???
 shell.setMapAttributes( bootStrap );

// shell.addSplit(Args() << Arg("lattice", "ring") << Arg("types", "Sbend")
//	 << Arg("ir", 2));

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
 forRunParams << Arg("print",   twissFile.c_str());
 const UAL::Arguments forRun = forRunParams;

// shell.setBeamAttributes(Args() << Arg("energy", energy) << Arg("mass", mass)
//			  << Arg("charge",charge));

 shell.readSXF(Args()  << Arg("file", sxfFile.c_str()));
 shell.addSplit( forRun );
 shell.use( forRun );
 shell.twiss( forRun );

 return 1;
}
