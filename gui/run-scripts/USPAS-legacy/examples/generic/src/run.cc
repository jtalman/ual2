#include <qapplication.h>

#include<TRint.h>
#include<TSystem.h>
#include<TStyle.h>

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
  std::string latticeFile  = argv[2];   // "../../lattices/general_fodo.sxf"; 
  std::string apdfFile     = argv[3];   // "$UAL/gui/USPAS/examples/project/apdf/eteapot.apdf";

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

  playerShell.setMapAttributes(Args() << Arg("order", 5));

  if(latticeFile.find(".adxf") != std::string::npos) {
    playerShell.readADXF(Args() << Arg("file", latticeFile )); 
  }else {
    playerShell.readSXF(Args() << Arg("file",  latticeFile )); 
  }
  std::cout << __LINE__ << "\n";
  playerShell.addSplit(Args() << Arg("lattice", "ring") << Arg("types", "Sbend") << Arg("ir", 1));
  std::cout << __LINE__ << "\n";
  playerShell.addSplit(Args() << Arg("lattice", "ring") << Arg("types", "Quadrupole") << Arg("ir", 1));
  std::cout << __LINE__ << "\n";
  playerShell.use(Args() << Arg("lattice", lattice));
  playerShell.map( Args() << Arg( "order", 1 ) << Arg( "print", "map" ));
//playerShell.tunefit(Args() << Arg("tunex", tuneX) << Arg("tuney", tuneY) << Arg("b1f", "^qf$") << Arg("b1d", "^qd$"));
//playerShell.chromfit(Args() << Arg("chromx", chromX) << Arg("chromy", chromY)<< Arg("b2f", "^sf") << Arg("b2d", "^sd"));
  std::cout << __LINE__ << "\n";
  playerShell.twiss( Args() << Arg( "print", "twiss" ) );
  std::cout << __LINE__ << "\n";
//shell.analysis(Args());

  double charge =  79;
  double mass   = 183.43261;
  // double energy =  23.75*mass; // 109*mass
  double energy =  8.7*mass;

  playerShell.setBeamAttributes(Args() 
 			  << Arg("charge", charge)
 			  << Arg("mass",   mass)
 			  << Arg("energy", energy));

  // ************************************************************************
  std::cout << "Set RF." << std::endl;
  // ************************************************************************ 

  double V      = 300.0e-6; // GeV
  double dgamma =   5.0e-5; // gamma gain per turn 
  double lag    =   asin(dgamma*mass/(charge*V))/2./UAL::pi;

  playerShell.setRF(Args() 
	      << Arg("V", V) 
	      << Arg("harmon", 360)
	      << Arg("lag", lag));

  // std::cout << "phase = " << 360*lag << " degrees" << std::endl;

  // ************************************************************************
  std::cout << "Read the APDF file (propagator description)." << std::endl;
  // ************************************************************************

  playerShell.readAPDF(Args() << Arg("file", apdfFile )); 

  // ************************************************************************
  std::cout << "Generate a bunch distribution." << std::endl;
  // ************************************************************************
  
  playerShell.setBunch(Args() 
//		 << Arg("np",    1000) 
		 << Arg("enx",   1.5e-7)  
		 << Arg("eny",   1.5e-7) 
		 << Arg("ctMax", 0.567) 
		 << Arg("deMax", 0.004321) 
		 << Arg("seed", -100)); 
  
  playerShell.setBunch( Args() << Arg("type", "grid") );
//playerShell.turns=1000;
//playerShell.m_turns=1000;
//unsigned int turns=1000;
//int turns=500;

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
//player->setFprint(100);
//player->JDT();

  // SoQt::init(gtPlayer);

  qtApp.setMainWidget(player);
  player->show();

  qtApp.exec();

}
