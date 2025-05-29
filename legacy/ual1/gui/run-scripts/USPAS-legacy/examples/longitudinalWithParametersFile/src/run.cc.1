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
  std::string latticeFile  = argv[2];   // "../../lattices/AGS_Analogue.sxf"; 
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
  UAL::QT::PlayerShell playerShell; 

  // ************************************************************************
  std::cout << "Read the lattice file." << std::endl;
  // ************************************************************************

  if(latticeFile.find(".adxf") != std::string::npos) {
    playerShell.readADXF(Args() << Arg("file", latticeFile )); 
  }else {
    playerShell.readSXF(Args() << Arg("file",  latticeFile )); 
  }

  // ************************************************************************
  std::cout << "Split elements." << std::endl;
  // ************************************************************************ 

  // playerShell.addSplit(Args() 
  //		 << Arg("lattice", lattice) 
  //		 << Arg("types", "Quadrupole") 
  //		 << Arg("ir", 2));

  // ************************************************************************
  std::cout << "Select a lattice." << std::endl;
  // ************************************************************************ 

  playerShell.use(Args() << Arg("lattice", lattice));

  // ************************************************************************
  std::cout << "Set beam attributes." << std::endl;
  // ************************************************************************

  double charge =  1;
  double mass   = 0.000511;
  // double energy =  23.75*mass; // 109*mass
  double energy =  29.382*mass;

  playerShell.setBeamAttributes(Args() << Arg("charge", charge));
  playerShell.setBeamAttributes(Args() << Arg("mass", mass));
  playerShell.setBeamAttributes(Args() << Arg("energy", energy));

  // ************************************************************************
  std::cout << "Set RF." << std::endl;
  // ************************************************************************ 

  double V      = 1.0e-6; // GeV
  double dgamma =   20.0e-5; // gamma gain per turn 
  //  double lag    =   asin(dgamma*mass/(charge*V))/2./UAL::pi;
  double lag    = 0.001626;


  playerShell.setRF(Args() 
	      << Arg("V", V) 
	      << Arg("harmon", 360)
	      << Arg("lag", lag));

  std::cout << "phase = " << 360*lag << " degrees" << std::endl;

  // ************************************************************************
  std::cout << "Read the APDF file (propagator description)." << std::endl;
  // ************************************************************************

  playerShell.readAPDF(Args() << Arg("file", apdfFile )); 

  // ************************************************************************
  std::cout << "Generate a bunch distribution." << std::endl;
  // ************************************************************************
  
  playerShell.setBunch(Args() 
		 << Arg("np", 10000) 
		 << Arg("enx",  1.0e-6)  
		 << Arg("eny",  1.0e-6) 
		 << Arg("ctMax", 0.5) 
		 << Arg("deMax", 1.0e-6) 
		 << Arg("seed", -100)); 
  playerShell.setBunch( Args() << Arg("type", "gauss") );

  // ************************************************************************
  std::cout << "Open the Player application." << std::endl;
  // ************************************************************************  

//UAL::USPAS::BasicPlayer* player = new UAL::USPAS::BasicPlayer();
//player->setShell(&playerShell);
//player->setTurns(10000);
//player->setFprint(100);

#include"parameters"
  std::cout << __LINE__ << "\n";
//int turns=atoi(argv[4]);
//int nbinsX=atoi(argv[5]);
  std::cout << __LINE__ << " turns " << turns << "\n";
//std::cout << __LINE__ << " nbinsX " << nbinsX << "\n";
  playerShell.setBunchSize(bunchSize);                     // NOT ACTIVE
  playerShell.setBunch(Args() << Arg("np", bunchSize));

  playerShell.setTurns(turns);
  playerShell.setPlotUpdates(plotUpdates);
  std::cout << __LINE__ << "\n";
  playerShell.setNbins(nbins);
  playerShell.setNsteps(nsteps);
  std::cout << __LINE__ << "\n";

  // ************************************************************************
  std::cout << "Open the Player application." << std::endl;
  // ************************************************************************  

  UAL::USPAS::BasicPlayer* player = new UAL::USPAS::BasicPlayer();
  player->setShell(&playerShell);

  player->setTurns(turns);
//player->setTurns(500);

  player->setFprint(plotUpdates);

  // SoQt::init(gtPlayer);

  qtApp.setMainWidget(player);
  player->show();

  qtApp.exec();

}
