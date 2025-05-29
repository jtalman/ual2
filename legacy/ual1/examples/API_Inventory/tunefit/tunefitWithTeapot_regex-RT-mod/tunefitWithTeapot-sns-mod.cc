#include<stdlib.h>
#include<regex.h>
#include<string>

#include"UAL/UI/ShellImp.hh"
#include"Optics/PacTMap.h"

#include"UAL/SXF/Parser.hh"
#include"UAL/UI/OpticsCalculator.hh"

Teapot* m_teapot;

#include"selectElementsByNames"
//#include"selectElementsByNamesIntoNames"

int main(int argc, char*argv[]){
 UAL::ShellImp::getInstance().m_space = new ZLIB::Space(6, 1);  //   6
 PacTMap map(6);                                                //   6
 PAC::BeamAttributes ba;
//#include"setba"
//#include"printba"

//   const char* b1f="^qf\\.*\\d*$";
//   const char* b1d="^qd\\.*\\d*$";

//const char* b1f="^QFH.2$|^QFH.3$|^QFH.4$|^QFH.5$|^QFH.6$|^QFH.7$|^QFH.8$|^QFH.9$|^QFH.10$|^QFH.11$";
//const char* b1d="^QFD.1$|^QFD.2$|^QFD.3$|^QFD.4$|^QFD.5$|^QFD.6$|^QFD.7$|^QFD.8$|^QFD.9$|^QFD.10$";
#include"snsRegex"

 m_teapot = new Teapot();
 PacLattices::iterator latIterator;
 std::string elname;
 PAC::Position orbit;

 UAL::SXFParser parser;
 parser.read("./sns-mod.sxf","");                              //   sxf file
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
 regcomp(&r, regexFor_sxf, REG_EXTENDED);
 if(regexec(&r, stringsxfName, (size_t)0, NULL, 0) == 0){
  printf("valid lattice element name\n");
 }else{
  printf("not valid lattice element name\n");
 }

 std::vector<int> b1fVector;
 std::vector<std::string> b1fVectorName;
 std::cerr << "focusing\n";
 selectElementsByNames(b1f, b1fVector);
// selectElementsByNamesIntoNames(b1f, b1fVectorName);
 for(int i = 0; i < b1fVector.size(); i++){
//std::cerr << i << " " << b1fVector[i] << "\n";
 }
 for(int i = 0; i < b1fVectorName.size(); i++){
//std::cerr << i << " " << b1fVectorName[i] << "\n";
 }

 std::vector<int> b1dVector;
 std::cerr << "defocusing\n";
 selectElementsByNames(b1d, b1dVector);
 for(int i = 0; i < b1dVector.size(); i++){
//std::cerr << i << " " << b1dVector[i] << "\n";
 }

 UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
 optics.use("ring");                                            //   lattice name

 if(argc!=3){
  std::cout << "Usage:\n./tunefitWithTeapot-sns-mod 3.6 3.4\n";
  std::cout << "argv[0] is this executable - ./tunefitWithTeapot-sns-mod\n";
  std::cout << "argv[1] is tuneX           -3.6\n";
  std::cout << "argv[2] is tuneY           -3.4\n";
  std::cout << "(sns-mod.sxf is the lattice file, line 34)\n";
  exit(0);
 }
 double tuneX = atof(argv[1]);
 double tuneY = atof(argv[2]);
// m_teapot->clorbit(orbit, ba);

std::cerr << __FILE__ << ": " << __LINE__ << "\n";
   m_teapot->tunethin(ba, orbit, b1fVector, b1dVector, tuneX, tuneY);
}
