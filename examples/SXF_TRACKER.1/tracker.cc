#include <iostream>
#include <fstream>
#include <iomanip>

#include <stdio.h>

#include "UAL/APDF/APDF_Builder.hh"
#include "PAC/Beam/Position.hh"
#include "SMF/PacSmf.h"
#include "PAC/Beam/Bunch.hh"
#include "Main/Teapot.h"
#include "UAL/UI/Shell.hh"

#include "PAC/Beam/Particle.hh"
#include "PAC/Beam/Spin.hh"

#include "UAL/SMF/AcceleratorNodeFinder.hh"

#include "timer.h"
#include "positionPrinter.hh"

using namespace UAL;

int main(int argc,char * argv[]){

  UAL::Shell shell;

  std::string variantName = "ring";

  // ************************************************************************
  std::cout << "\nDefine the space of Taylor maps." << std::endl;
  // ************************************************************************

  shell.setMapAttributes(UAL::Args() << UAL::Arg("order", 5));

  // ************************************************************************
  std::cout << "\nBuild lattice." << std::endl;
  // ************************************************************************

  std::string sxfFile = "./data/";
  sxfFile += variantName;
  sxfFile += ".sxf";

  shell.readSXF(UAL::Args() << UAL::Arg("file",  sxfFile.c_str()));

  // ************************************************************************
  std::cout << "\nAdd split ." << std::endl;
  // ************************************************************************

  
  shell.addSplit(UAL::Args() << UAL::Arg("lattice", "ring") << UAL::Arg("types", "Sbend") << UAL::Arg("ir", 4));

  shell.addSplit(UAL::Args() << UAL::Arg("lattice", "ring") << UAL::Arg("types", "Quadrupole") << UAL::Arg("ir", 4));
  

  // ************************************************************************
  std::cout << "Select lattice." << std::endl;
  // ************************************************************************

  shell.use(UAL::Args() << UAL::Arg("lattice", "ring"));

  // ************************************************************************
  std::cout << "\nWrite SXF file ." << std::endl;
  // ************************************************************************

  std::string outputFile = "./out/cpp/";
  outputFile += variantName;
  outputFile += ".sxf";

  shell.writeSXF(UAL::Args() << UAL::Arg("file",  outputFile.c_str()));

  // ************************************************************************
  std::cout << "\nDefine beam parameters." << std::endl;
  // ************************************************************************

  double mass   = 1.8756; // proton rest mass
  double pc = 0.97;
  double energy = sqrt(mass*mass + pc*pc);
  double vByc = pc/energy;

  shell.setBeamAttributes(UAL::Args() << UAL::Arg("mass", mass));
  shell.setBeamAttributes(UAL::Args() << UAL::Arg("energy", energy));
//shell.setBeamAttributes(UAL::Args() << UAL::Arg("energy", energy) << UAL::Arg("mass", mass));

  PAC::BeamAttributes& ba = shell.getBeamAttributes();

  // ************************************************************************
  std::cout << "\nLinear analysis." << std::endl;
  // ************************************************************************
  
  // Make linear matrix

  std::string mapFile = "./out/cpp/";
  mapFile += variantName;
  mapFile += ".map2";

  std::cout << " matrix" << std::endl;
  shell.map(UAL::Args() << UAL::Arg("order", 2) << UAL::Arg("print", mapFile.c_str()));

  // Calculate twiss
  
  std::string twissFile = "./out/cpp/";
  twissFile += variantName;
  twissFile += ".twiss";

  std::cout << " twiss (ring )" << std::endl;

  shell.twiss(UAL::Args() << UAL::Arg("print", twissFile.c_str()));

  std::cout << " calculate suml" << std::endl;
  shell.analysis(UAL::Args());

  return 1;
}

