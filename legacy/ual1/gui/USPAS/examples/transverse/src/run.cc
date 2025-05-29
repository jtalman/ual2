// Qt
#include <qapplication.h>
// Coin 3D
// #include <Inventor/Qt/SoQt.h>
// Root
#include "TRint.h"
#include <TSystem.h>
#include <TStyle.h>
// DA
#include "ZLIB/Tps/Space.hh"
#include "Optics/PacChromData.h"

#include "UAL/QT/Player/PlayerShell.hh"

#include "UAL/ADXF/Reader.hh"
#include "UAL/ADXF/Writer.hh"

#include "UAL/ADXF/ConstantManager.hh"
#include "BasicPlayer.hh"

using namespace UAL;

int main(int argc, char *argv[])
{
  if(argc < 4) {
    std::cout << "Usage: ./run ringName latticeFile apdfFile" << std::endl;
    return 0;
  }
  std::cout << argc << std::endl;

  std::string lattice     = argv[1]; // "ring";
  std::string latticeFile = argv[2]; // "../../lattices/eq_tune_fodo.adxf"; 
  std::string apdfFile    = argv[3]; // "../apdf/teapot.apdf";

  // ************************************************************************
  std::cout << "Declare Qt, ROOT, DA, UAL environment." << std::endl;
  // ************************************************************************

  // Declare GUI Interface
  QApplication qtApp(argc, argv);

  // Declare ROOT application
  TRint *rint = new TRint("Rint", &argc, qApp->argv());
  gStyle->SetPalette(1, 0);

  // Declare DA space 
  ZLIB::Space space(6, 5);

  // Declare UI Interface
  UAL::QT::PlayerShell shell; 

  // ************************************************************************
  std::cout << "Read the ADXF file (lattice description)." << std::endl;
  // ************************************************************************

  if(latticeFile.find(".adxf") != std::string::npos) {
    shell.readADXF(Args() << Arg("file", latticeFile )); 
  }else {
    shell.readSXF(Args() << Arg("file",  latticeFile )); 
  }

  // ************************************************************************
  std::cout << "Split quadrupoles." << std::endl;
  // ************************************************************************ 

  shell.addSplit(Args() 
  	 << Arg("lattice", lattice) 
  	 << Arg("types", "Quadrupole") 
  	 << Arg("ir", 2));

  // ************************************************************************
  std::cout << "Select a lattice." << std::endl;
  // ************************************************************************ 

  shell.use(Args() << Arg("lattice", lattice));

  // ************************************************************************
  std::cout << "Set beam attributes." << std::endl;
  // ************************************************************************

  double charge =  1;
  double mass   = 0.511e-3;
  double energy =  5.0;

  shell.setBeamAttributes(Args() 
 			  << Arg("charge", charge)
 			  << Arg("mass",   mass)
 			  << Arg("energy", energy));

  // ************************************************************************
  std::cout << "Read the APDF file (propagator description)." << std::endl;
  // ************************************************************************

  shell.readAPDF(Args() << Arg("file", apdfFile )); 

  // ************************************************************************
  std::cout << "Generate a bunch distribution." << std::endl;
  // ************************************************************************

  shell.setBunch(Args() 
		 << Arg("type", "grid") 
		 << Arg("np",    2) 
		 << Arg("enx",  1.0e-7)  
		 << Arg("eny",  1.0e-8) 
		 << Arg("ctMax", 0.01) 
		 << Arg("deMax", 0.0006)); 

  // ************************************************************************
  std::cout << "Open the Player application." << std::endl;
  // ************************************************************************  

  UAL::USPAS::BasicPlayer* player = new UAL::USPAS::BasicPlayer();
  player->setShell(&shell);

  player->setTurns(1000);
  player->setFprint(25);

  // SoQt::init(twissPlayer);

  qtApp.setMainWidget(player);
  player->show();

  qtApp.exec();

}
