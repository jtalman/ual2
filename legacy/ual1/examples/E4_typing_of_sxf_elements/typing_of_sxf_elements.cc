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
 if(argc!=1){
  std::cout << "usage: ./typing\n";
  std::cout << "argv[0] is this executable         - ./typing                         \n";
  exit(0);
 }

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

std::cout << "size : " << ap->getRootNode().size() << " propagators " << endl;
UAL::PropagatorSequence& apSeq = ap->getRootNode();

int counter = 0;
std::list<UAL::PropagatorNodePtr>::iterator it;
for(it = apSeq.begin(); it != apSeq.end(); it++){
 std::cout << counter++ << " (*it)->getType() " << (*it)->getType() << std::endl;
}

 return 0;
}
