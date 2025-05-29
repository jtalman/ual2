#include<stdlib.h>
#include <regex.h>

#include"UAL/UI/ShellImp.hh"
#include"Optics/PacTMap.h"

#include"UAL/SXF/Parser.hh"
#include"UAL/UI/OpticsCalculator.hh"

Teapot* m_teapot;

#include"selectElementsByNames"

int main(int argc, char*argv[]){
 UAL::ShellImp::getInstance().m_space = new ZLIB::Space(6, 1);  //   6
 PacTMap map(6);                                                //   6
 PAC::BeamAttributes ba;
//#include"setba"
//#include"printba"
 std::string b1f="^qf$";
 std::string b1d="^qd$";
 m_teapot = new Teapot();
 PacLattices::iterator latIterator;
 std::string elname;
 PAC::Position orbit;

 UAL::SXFParser parser;
 parser.read("../working.sxf","");                              //   sxf file
 latIterator = PacLattices::instance()->find("ring");
 m_teapot->use(*latIterator);
 for(int i = 0; i < m_teapot->size(); i++){
  TeapotElement& anode = m_teapot->element(i);
  elname = anode.getDesignName();
//std::cerr << i << " " << elname << "\n";
 }
 std::vector<int> b1fVector;
 selectElementsByNames(b1f, b1fVector);
 for(int i = 0; i < b1fVector.size(); i++){
  std::cerr << i << " " << b1fVector[i] << "\n";
 }
 std::vector<int> b1dVector;
 selectElementsByNames(b1d, b1dVector);
 for(int i = 0; i < b1dVector.size(); i++){
  std::cerr << i << " " << b1dVector[i] << "\n";
 }

 UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
 optics.use("ring");                                            //   lattice name

 if(argc!=3){
  std::cout << "Usage:\n./tunefitWithTeapot 28.685 29.675\n";
  std::cout << "argv[0] is this executable - ./tunefitWithTeapot\n";
  std::cout << "argv[1] is tuneX           - 28.685\n";
  std::cout << "argv[2] is tuneY           - 29.675\n";
  exit(0);
 }
 double tuneX = atof(argv[1]);
 double tuneY = atof(argv[2]);
// m_teapot->clorbit(orbit, ba);
   m_teapot->tunethin(ba, orbit, b1fVector, b1dVector, tuneX, tuneY);
}
