#include<stdlib.h>
#include<regex.h>

#include"UAL/UI/ShellImp.hh"
#include"Optics/PacTMap.h"

#include"UAL/SXF/Parser.hh"
#include"UAL/UI/OpticsCalculator.hh"

Teapot* m_teapot;

#include"selectElementsByNames"

int main(int argc, char*argv[]){
//UAL::ShellImp::getInstance().m_space = new ZLIB::Space(6, 1);  //   1 for tune
  UAL::ShellImp::getInstance().m_space = new ZLIB::Space(6, 2);  //   2 for sext
 PacTMap map(6);                                                
 PAC::BeamAttributes ba;

 // std::string b1f="^qf$";
 // std::string b1d="^qd$";
   const char* b1f="^qu2$|^qu3$|^qu10$|^qu11$|^qu14$|^qu15$|^qu22$|^qu23$|^qu6$|^qu7$|^qu18$|^qu19$ ";
    const char* b1d="^qu1$|^qu4$|^qu5$|^qu8$|^qu9$|^qu12$|^qu13$|^qu16$|^qu17$|^qu20$|^qu21$|^qu24$";

  // std::string b2f="^sf";
  //std::string b2d="^sd";

    // std::string b2f="^mx15$";
    // std::string b2d="^mx13$";

    std::string b2f="^mx10$";               // N-straight
    std::string b2d="^mx06$";              // MXL

    // std::string b2f="^mx06$|^mx08$|^mx15$|^mx17$";                                    //   MXL
    // std::string b2d="^mx05$|^mx07$|^mx09$|^mx14$|^mx16$|^mx18$";       //   MXG, MXS   

 m_teapot = new Teapot();
 PacLattices::iterator latIterator;
 std::string elname;
 PAC::Position orbit;

 UAL::SXFParser parser;
 parser.read("./ring.sxf","");
 latIterator = PacLattices::instance()->find("ring");
 m_teapot->use(*latIterator);
 for(int i = 0; i < m_teapot->size(); i++){
  TeapotElement& anode = m_teapot->element(i);
  elname = anode.getDesignName();
//std::cerr << i << " " << elname << "\n";
 }
 std::vector<int> b2fVector;
 selectElementsByNames(b2f, b2fVector);
 for(int i = 0; i < b2fVector.size(); i++){
  std::cerr << i << " " << b2fVector[i] << "\n";
 }
 std::vector<int> b2dVector;
 selectElementsByNames(b2d, b2dVector);
 for(int i = 0; i < b2dVector.size(); i++){
  std::cerr << i << " " << b2dVector[i] << "\n";
 }

 UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
 optics.use("ring");

 if(argc!=3){
  std::cout << "Usage:\n./chromefitWithTeapot 0  0 \n";
  std::cout << "argv[0] is this executable - ./chromefitWithTeapot\n";
  std::cout << "argv[1] is chromeX           -   0 \n";
  std::cout << "argv[2] is chromeY           -    0 \n";
  exit(0);
 }
 double chromeX = atof(argv[1]);
 double chromeY = atof(argv[2]);
// m_teapot->clorbit(orbit, ba);                                            // clorbit
// m_teapot->tunethin(ba, orbit, b1fVector, b1dVector, tuneX, tuneY);       // tune
   m_teapot->chromfit(ba, orbit, b2fVector, b2dVector, chromeX, chromeY);   // chrome
}
