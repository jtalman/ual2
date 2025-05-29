#include"stdlib.h"
#include"UAL/UI/ShellImp.hh"
#include"Optics/PacTMap.h"

#include"UAL/SXF/Parser.hh"
#include"UAL/UI/OpticsCalculator.hh"

int main(int argc, char*argv[]){
 UAL::ShellImp::getInstance().m_space = new ZLIB::Space(6, 1);   //   6
 PacTMap map(6);                                                 //   6
#include"setba"
#include"printba"
 std::string b1f="^qf$";
 std::string b1d="^qd$";

 UAL::SXFParser parser;
 parser.read("../working.sxf","");                           //   sxf file
 UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
 optics.use("ring");                                             //   lattice name

 if(argc!=3){
  std::cout << "Usage:\n./tunefitWithOpticsCalculator 28.685 29.675\n";
  std::cout << "argv[0] is this executable - ./tunefitWithOpticsCalculator\n";
  std::cout << "argv[1] is tuneX           - 28.685\n";
  std::cout << "argv[2] is tuneY           - 29.675\n";
  exit(0);
 }
 double tuneX = atof(argv[1]);
 double tuneY = atof(argv[2]);

 optics.tunefit(tuneX,tuneY,b1d,b1f);
// optics.m_teapot->map(map, PAC::BeamAttributes(), 1);            //   order 1
// map.write("./working.map1");                                    //   order 1
}
