#include<stdlib.h>
#include<regex.h>

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
// std::string b1f="[^qf$|^qf\.[1-9]$]";
// std::string b1f="^qf?[0-9]*$";
// std::string b1f="^qf\.[0-9]|qf\.[1-9][0-9]$";
// std::string b1f="^qf\.?\d*$";
// std::string b1f="( ^qf\.2$ | ^qf\.3$ )";
// std::string b1f="^qf\d*\.?\d+$";
// std::string b1f="^qf\.?\d+$";
// std::string b1f="^qf\.?$";
// std::string b1f="^qf*$";                               //  <<<--- WORKS
// std::string b1f="\sqf*\s$";
// std::string b1f="^qf ( (\d+ (\.\d*)?)  |  (\.\d+) ) $";
// std::string b1f="(?<![^qf])\d+(?:\.\d+)?(?![^d])";
// std::string b1f="^qf$";
// std::string b1f="qf\.*\d*";

// const char* b1f="qf\\.*\\d*";
   const char* b1f="^qf\\.*\\d*$";
   const char* b1d="^qd\\.*\\d*$";
// const char* b1d="qd\\.*\\d*";

// std::string b1d="^qd$";
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

 const char* stringsxfName="xxxqf.2YYYz";
 const char* regexFor_sxf="qf\.*\d*";
 regex_t r;
//int r_sxf = regcomp(&r, "^qf(\.?)$", 0);
//int r_sxf = regcomp(&r, "^qf?\d?$", 0);
//int r_sxf = regcomp(&r, "^qf\.?$", 0);
//int r_sxf = regcomp(&r, "^qf\.?\d*", REG_EXTENDED);
//int r_sxf = regcomp(&r, "qf.2", REG_EXTENDED);
//            regcomp(&r, "qf.2", REG_EXTENDED);

//            regcomp(&r, "qf\.", REG_EXTENDED);
//            regcomp(&r, "qf\.*", REG_EXTENDED);
//            regcomp(&r, "qf\.*\d*", REG_EXTENDED);
//            regcomp(&r, "qf\\.*\\d*", REG_EXTENDED);
              regcomp(&r, regexFor_sxf, REG_EXTENDED);
 if(regexec(&r, stringsxfName, (size_t)0, NULL, 0) == 0){
  printf("valid lattice element name\n");
 }else{
  printf("not valid lattice element name\n");
 }

 std::vector<int> b1fVector;
 selectElementsByNames(b1f, b1fVector);
 std::cerr << "focusing\n";
 for(int i = 0; i < b1fVector.size(); i++){
  std::cerr << i << " " << b1fVector[i] << "\n";
 }

 std::vector<int> b1dVector;
 selectElementsByNames(b1d, b1dVector);
 std::cerr << "defocusing\n";
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
