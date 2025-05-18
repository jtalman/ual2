#include<stdlib.h>
#include<regex.h>

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

    const char* b1f="^qu2$|^qu3$|^qu10$|^qu11$|^qu14$|^qu15$|^qu22$|^qu23$|^qu6$|^qu7$|^qu18$|^qu19$";
    const char* b1d="^qu1$|^qu4$|^qu5$|^qu8$|^qu9$|^qu12$|^qu13$|^qu16$|^qu17$|^qu20$|^qu21$|^qu24$";

 m_teapot = new Teapot();
 PacLattices::iterator latIterator;
 std::string elname;
 PAC::Position orbit;

 UAL::SXFParser parser;
 parser.read("./cosy.sxf","");                              //   sxf file
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
 selectElementsByNames(b1f, b1fVector);
// selectElementsByNamesIntoNames(b1f, b1fVectorName);
 std::cerr << "focusing\n";
 for(int i = 0; i < b1fVector.size(); i++){
//std::cerr << i << " " << b1fVector[i] << "\n";
 }
 for(int i = 0; i < b1fVectorName.size(); i++){
//std::cerr << i << " " << b1fVectorName[i] << "\n";
 }

 std::vector<int> b1dVector;
 selectElementsByNames(b1d, b1dVector);
 std::cerr << "defocusing\n";
 for(int i = 0; i < b1dVector.size(); i++){
//std::cerr << i << " " << b1dVector[i] << "\n";
 }

 UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
 optics.use("ring");                                            //   lattice name

 if(argc!=3){
  std::cout << "Usage:\n./tunefitWithTeapot-cosy 3.6 3.4\n";
  std::cout << "argv[0] is this executable - ./tunefitWithTeapot-cosy\n";
  std::cout << "argv[1] is tuneX           -3.6\n";
  std::cout << "argv[2] is tuneY           -3.4\n";
  std::cout << "(cosy.sxf is the lattice file, line 34)\n";
  exit(0);
 }
 double tuneX = atof(argv[1]);
 double tuneY = atof(argv[2]);
// m_teapot->clorbit(orbit, ba);
   m_teapot->tunethin(ba, orbit, b1fVector, b1dVector, tuneX, tuneY);
}
