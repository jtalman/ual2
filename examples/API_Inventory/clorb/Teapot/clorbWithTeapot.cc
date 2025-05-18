#include<stdlib.h>
#include <regex.h>

#include"UAL/UI/ShellImp.hh"
#include"Optics/PacTMap.h"

#include"UAL/SXF/Parser.hh"
#include"UAL/UI/OpticsCalculator.hh"

#include"Main/TeapotClOrbitService.h"
#include "Main/TeapotMatrixService.h"

Teapot* m_teapot;
TeapotClOrbitService* m_teapot_clorbit_service;

static int     TeapotClOrbitService_Dim  = 4;
static double  Delta = 1.e-8;
static int     TeapotClOrbitService_Nit  = 150;
static double  TeapotClOrbitService_Tiny = 1.e-10;

#include"selectElementsByNames"
#include"closedMatrix"
#include"closedOrbit"
#include"propagate"

int main(int argc, char*argv[]){
 UAL::ShellImp::getInstance().m_space = new ZLIB::Space(6, 1);  //   6
// PacTMap map(6);                                              //   6
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
 m_teapot_clorbit_service = new TeapotClOrbitService(*m_teapot);
 for(int i = 0; i < m_teapot->size(); i++){
  TeapotElement& anode = m_teapot->element(i);
  elname = anode.getDesignName();
//std::cerr << i << " " << elname << "\n";
 }
 std::vector<int> b1fVector;
 selectElementsByNames(b1f, b1fVector);
 for(int i = 0; i < b1fVector.size(); i++){
//std::cerr << i << " " << b1fVector[i] << "\n";
 }
 std::vector<int> b1dVector;
 selectElementsByNames(b1d, b1dVector);
 for(int i = 0; i < b1dVector.size(); i++){
//std::cerr << i << " " << b1dVector[i] << "\n";
 }

 UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
 optics.use("ring");                                            //   lattice name

 if(argc!=3){
  std::cout << "Usage:\n./clorbWithTeapot 28.685 29.675\n";
  std::cout << "argv[0] is this executable - ./clorbWithTeapot\n";
  std::cout << "argv[1] is tuneX           - 28.685\n";
  std::cout << "argv[2] is tuneY           - 29.675\n";
  exit(0);
 }
 double tuneX = atof(argv[1]);
 double tuneY = atof(argv[2]);

 PAC::Position delta;
 delta.set(Delta, Delta, Delta, Delta, 0.0, Delta);
 PacTMap map(TEAPOT_DIMENSION);
 map.refOrbit(orbit);
  TeapotMatrixService service(*m_teapot);
//TeapotMatrixService service(code_);
#include"print_map"
  service.define(map, ba,  delta, 1); 
//service.define(map, att, delta, 1); 
#include"print_map"
 PAC::Position before, after;
 closedOrbit(before, map, 1);
 before += orbit;
 int i, nit, flag = 0;
 double d;
 after = before;
 propagate(after, ba, 0, m_teapot->size());
 for(nit = 0; nit < TeapotClOrbitService_Nit; nit++) {
  before += after;
  before /= 2.;
  before[4] = orbit[4];
  before[5] = orbit[5];
  after = before;
  flag = propagate(after, ba, 0, m_teapot->size());
  if(flag) break;
  d = 0;
  for(i=0; i < 4; i++) d += fabs(after[i] - before[i]);
  if(d < TeapotClOrbitService_Tiny) break;
 }
 if(flag) {
  cerr << "\nparticle lost, closed orbit did not converge in " << nit << " turns\n";
  string msg = " ";
  PacDomainError(msg).raise();
 }
 if(nit == TeapotClOrbitService_Nit) {
  cerr << "\nclosed orbit did not converge in " <<  nit << " turns \n";
 }
 for(i=0; i < TeapotClOrbitService_Dim; i++) { orbit[i] = after[i]; }
 std::cerr << __LINE__ << " orbit ";
 for(int i=0;i<6;i++){
  std::cerr << orbit[i] << " ";
 }
 std::cerr << "\n";
 std::cerr << __LINE__ << " leave void TeapotClOrbitService::define\n";

// m_teapot->clorbit(orbit, ba);
// m_teapot->tunethin(ba, orbit, b1fVector, b1dVector, tuneX, tuneY);

// m_teapot_clorbit_service
}
