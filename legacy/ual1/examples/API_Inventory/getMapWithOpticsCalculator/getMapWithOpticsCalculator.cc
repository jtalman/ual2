#include"UAL/UI/ShellImp.hh"
#include"Optics/PacTMap.h"

#include"UAL/SXF/Parser.hh"
#include"UAL/UI/OpticsCalculator.hh"

int main(int argc, char*argv[]){
 UAL::ShellImp::getInstance().m_space = new ZLIB::Space(6, 1);   //   6
 PacTMap map(6);                                                 //   6

 UAL::SXFParser parser;
 parser.read("./working.sxf","");                           //   sxf file
 UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
 optics.use("ring");                                             //   lattice name

 if(argc!=1){
  std::cout << "usage: ./get_map)\n";
  std::cout << "argv[0] is this executable - ./get_map\n";
  goto end;
 }

 optics.m_teapot->map(map, PAC::BeamAttributes(), 1);            //   order 1
 map.write("./working.map1");                                    //   order 1
end:;}                                                           //   !!!
