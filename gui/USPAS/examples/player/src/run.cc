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

  std::string lattice      = argv[1];  // "blue";
  std::string latticeFile  = argv[2]; // "../data/eq_tune_fodo.adxf"; 
  std::string apdfFile     = argv[3];  // "../data/tibetan.apdf";

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
  std::cout << "Read the lattice file." << std::endl;
  // ************************************************************************

    shell.readSXF(Args() << Arg("file", latticeFile )); 

    /*
      if(latticeFile.find(".adxf") != std::string::npos) {
      shell.readADXF(Args() << Arg("file", latticeFile )); 
      }else {
      shell.readSXF(Args() << Arg("file",  latticeFile )); 
      }
    */

  // ************************************************************************
  std::cout << "Split elements." << std::endl;
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
  std::cout << "Fit chromaticity." << std::endl;
  // ************************************************************************ 

  // shell.chromfit(Args() 
  //		 << Arg("b2f", "sext1")
  //		 << Arg("b2d", "sext2"));

  // ************************************************************************
  std::cout << "Set beam attributes." << std::endl;
  // ************************************************************************

  double charge =  1; // 79;
  double mass   =  UAL::pmass; // 183.43261;
  double energy =  106.8*mass; // 23.75*mass

  shell.setBeamAttributes(Args() 
 			  << Arg("charge", charge)
 			  << Arg("mass",   mass)
 			  << Arg("energy", energy));

  // ************************************************************************
  std::cout << "Set RF." << std::endl;
  // ************************************************************************ 

  double V      = 300.0e-6; // GeV

  shell.setRF(Args() << Arg("V", V) << Arg("harmon", 360));

  // ************************************************************************
  std::cout << "Read the APDF file (propagator description)." << std::endl;
  // ************************************************************************

  shell.readAPDF(Args() << Arg("file", apdfFile )); 

  // ************************************************************************
  std::cout << "Generate a bunch distribution." << std::endl;
  // ************************************************************************
  
  shell.setBunch(Args() 
		 << Arg("type", "gauss") 
		 << Arg("np",     10000) 
		 << Arg("enx",  15.0e-6)  
		 << Arg("eny",  15.0e-6)
		 << Arg("ctMax", -1.0) 
		 << Arg("deMax", -1.0)); 

  // ************************************************************************
  std::cout << "Open the Player application." << std::endl;
  // ************************************************************************  

  UAL::USPAS::BasicPlayer* player = new UAL::USPAS::BasicPlayer();
  player->setShell(&shell);
  player->setTurns(1000);
  player->setFprint(10);

  // SoQt::init(player);

  qtApp.setMainWidget(player);
  player->show();

  qtApp.exec();

}
