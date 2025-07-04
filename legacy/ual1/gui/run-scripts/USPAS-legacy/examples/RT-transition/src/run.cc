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
#include "BasicPlayer.hh"

#include "UAL/ADXF/Reader.hh"
#include "UAL/ADXF/Writer.hh"

#include "UAL/ADXF/ConstantManager.hh"

using namespace UAL;

int main(int argc, char *argv[])
{

  if(argc < 4) {
    std::cout << "Usage: ./run ringName latticeFile apdfFile" << std::endl;
    return 0;
  }
  std::cout << argc << std::endl;

  std::string lattice      = argv[1];   // "ring";
  std::string latticeFile  = argv[2];   // "../../lattices/eq_tune_fodo.adxf"; 
  std::string apdfFile     = argv[3];   // "../apdf/tibetan.apdf";

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

  if(latticeFile.find(".adxf") != std::string::npos) {
    shell.readADXF(Args() << Arg("file", latticeFile )); 
  }else {
    shell.readSXF(Args() << Arg("file",  latticeFile )); 
  }

  // ************************************************************************
  std::cout << "Split elements." << std::endl;
  // ************************************************************************ 

  // shell.addSplit(Args() 
  //		 << Arg("lattice", lattice) 
  //		 << Arg("types", "Quadrupole") 
  //		 << Arg("ir", 2));

  // ************************************************************************
  std::cout << "Select a lattice." << std::endl;
  // ************************************************************************ 

  shell.use(Args() << Arg("lattice", lattice));

  // ************************************************************************
  std::cout << "Set beam attributes." << std::endl;
  // ************************************************************************

  double charge =  79;
  double mass   = 183.43261;
  // double energy =  23.75*mass; // 109*mass
  double energy =  8.7*mass;

  shell.setBeamAttributes(Args() 
 			  << Arg("charge", charge)
 			  << Arg("mass",   mass)
 			  << Arg("energy", energy));

  // ************************************************************************
  std::cout << "Set RF." << std::endl;
  // ************************************************************************ 

  double V      = 300.0e-6; // GeV
  double dgamma =   5.0e-5; // gamma gain per turn 
  double lag    =   asin(dgamma*mass/(charge*V))/2./UAL::pi;

  shell.setRF(Args() 
	      << Arg("V", V) 
	      << Arg("harmon", 360)
	      << Arg("lag", lag));

  // std::cout << "phase = " << 360*lag << " degrees" << std::endl;

  // ************************************************************************
  std::cout << "Read the APDF file (propagator description)." << std::endl;
  // ************************************************************************

  shell.readAPDF(Args() << Arg("file", apdfFile )); 

  // ************************************************************************
  std::cout << "Generate a bunch distribution." << std::endl;
  // ************************************************************************
  
  shell.setBunch(Args() 
		 << Arg("np", 10000) 
		 << Arg("enx",  15.0e-6)  
		 << Arg("eny",  15.0e-6) 
		 << Arg("ctMax", 0.5) 
		 << Arg("deMax", 0.004) 
		 << Arg("seed", -100)); 

  // ************************************************************************
  std::cout << "Open the Player application." << std::endl;
  // ************************************************************************  

#include"parameters"
  shell.setBunchSize(bunchSize);                     // NOT ACTIVE
  shell.setBunch(Args() << Arg("np", bunchSize));

  shell.setTurns(turns);
  shell.setPlotUpdates(plotUpdates);
  std::cout << __LINE__ << "\n";
  shell.setNbins(nbins);
  shell.setNsteps(nsteps);

  UAL::USPAS::BasicPlayer* player = new UAL::USPAS::BasicPlayer();
  player->setShell(&shell);
//player->setTurns(10000);
  player->setTurns(turns);
  player->setFprint(100);

  // SoQt::init(gtPlayer);

  qtApp.setMainWidget(player);
//int w = QApplication::desktop()->width();
//int h = QApplication::desktop()->height();
//player->resize(w,h);
  player->show();

  qtApp.exec();

}
