#include<iostream>
#include<string>

#include<stdio.h>
#include<stdlib.h>

#include"UAL/APDF/APDF_Builder.hh"
#include"PAC/Beam/Bunch.hh"
#include"UAL/UI/Shell.hh"

#include"ETEAPOT2/Integrator/genMethods/Vectors.h"
#include"ETEAPOT2/Integrator/genMethods/designExtern"
#include"ETEAPOT2/Integrator/genMethods/designDef"
#include"ETEAPOT2/Integrator/genMethods/bunchParticleExtern"
#include"ETEAPOT2/Integrator/genMethods/bunchParticleDef"

double __SPIN__[1][3];

using namespace UAL;

int main(int argc,char * argv[]){
 if(argc!=2){
//std::cout << "usage: ./E2_Track ./sxf/E_pEDM-rtr1-preMobius.RF.sxf 646617.830 -1.2 0 1000 0.02 10 >! /dev/null (>&! OUT)\n";
  std::cout << "usage: ./E2_Survey 1.25\n";
  std::cout << "argv[0] is this executable         - ./E2_Survey                         \n";
  std::cout << "argv[1] is design gamma            - 1.25                                \n";
  exit(0);
 }

 double gammaD=atof(argv[1]);
 double betaD =sqrt(1.-1./gammaD/gammaD);
// double vD    =betaD;
 vD    =betaD*UAL::clight;
// double vDMKS =betaD*UAL::clight;

 UAL::Shell shell;

 std::string apdfFile = "./working.apdf";
 PAC::Bunch bunch(1);
 bunch[0].getPosition().set(0,0,0,0,0,0);

 shell.readSXF( UAL::Args() << UAL::Arg( "file",  "./working.sxf" ) );
 shell.use(UAL::Args() << UAL::Arg("lattice", "ring"));
 UAL::APDF_Builder apBuilder;
 UAL::AcceleratorPropagator* ap = apBuilder.parse(apdfFile);

 if(ap == 0){
  std::cout << "Accelerator Propagator has not been created " << std::endl;
  return 1;
 }

 ap->propagate(bunch);

 std::cerr << "\n";
 std::cerr << "++===========================================================================================++\n";
 std::cerr << "++===========================================================================================++\n";
 std::cerr << "++===========================================================================================++\n";
 std::cerr << "                                                                                               \n";
 std::cerr << "    gamma design   " << gammaD << "                                                            \n";
 std::cerr << "    beta design    " << betaD << "                                                             \n";
 std::cerr << "    v design MKS   " << betaD*UAL::clight << "                                                 \n";
 std::cerr << "                                                                                               \n";
 std::cerr << "    s measured     " << sDT << "                                                               \n";
 std::cerr << "    theta measured " << thetaDT << "                                                           \n";
 std::cerr << "                                                                                               \n";
 std::cerr << "    t measured     " << tofDT << "                                                             \n";
 std::cerr << "    f measured     " << 1./tofDT << "                                                          \n";
 std::cerr << "                                                                                               \n";
 std::cerr << "++===========================================================================================++\n";
 std::cerr << "++===========================================================================================++\n";
 std::cerr << "++===========================================================================================++\n";
 std::cerr << "\n";

 return 0;
}
