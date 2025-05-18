#include <fstream>

#include "math.h"
#include "timer.h"

#ifndef __CINT__

#include <TSystem.h>
#include <TObject.h>
#include <TNtupleD.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TApplication.h>

#endif

#include "UAL/UI/Shell.hh"
#include "Optics/PacTMap.h"
#include "ACCSIM/Base/GaussianGenerator.hh"
#include "ACCSIM/Bunch/BunchGenerator.hh"

#include "BunchGenerator.hh"
#include "BunchAnalyzer.hh"
#include "BunchDisplay.hh"
#include "MatrixTracker.hh"
#include "CMTracker.hh"
#include "BBTracker.hh"
#include "BBDisplay.hh"

using namespace UAL;

int main(int argc, char** argv)
{

#ifndef __CINT__
  TApplication theApp("BeamBeam", &argc, argv);
#endif

  UAL::Shell shell;
  std::ofstream logFile ("./log.out");

  // ************************************************************************
  logFile << "\nParameters." << std::endl;
  // ************************************************************************

  int nturns         = 3000000; 

  double charge      = 1.0;
  double mass        = UAL::pmass;
  double energy      = 250; 
  double gamma       = energy/mass;

  int    np1         = 1; 
  int    np2         = 1000; 

  double nppb1       = 1.4e+11;
  double nppb2       = 0.6e+11;

  double em1         = 15.0e-6/6.0/gamma; // m*rad
  double em2         = 9*em1; // m*rad

  double qx0         = 28.215;
  double qy0         = 28.230; 

  double n_c = 1.0e4;
  double dQ_x = 3e-5; //tune noise amplitude
  double dQ_y = 3e-5;
  double Qm_x = 0e-4; //tune modulation amplitude
  double Qm_y = 0e-4;

  // ************************************************************************
  logFile << "\nDefine the space of Taylor maps." << std::endl;
  // ************************************************************************
 
  shell.setMapAttributes(Args() << Arg("order", 7)); 

  // ************************************************************************
  logFile << "\nSet Lattice ." << std::endl;
  // ************************************************************************

  double betax  = 0.27;
  double betay  = 0.27;
  double alphax = 0.0;
  double alphay = 0.0;
  double gammax = (1. + alphax*alphax)/betax;
  double gammay = (1. + alphay*alphay)/betay;

  PacTwissData twissIn;

  twissIn.beta(0, betax); 
  twissIn.beta(1, betay);
  twissIn.alpha(0, alphax); 
  twissIn.alpha(1, alphay);
  twissIn.mu(0, 2.0*UAL::pi*qx0); 
  twissIn.mu(1, 2.0*UAL::pi*qy0); 

  double chx         = 2.0;
  double chy         = 2.0;

  double V           = 0.002; // [GeV]
  double lag         = 0.5;
  double harmon      = 7*360; // 2520

  double suml        = 3834.03; // 
  double alpha       = 0.00174248; // gt = 23.96

  double eta0        = alpha - 1./gamma/gamma;

  // ************************************************************************
  logFile << "\nBeam attributes . " << std::endl;
  // ************************************************************************ 

  PAC::BeamAttributes ba;

  ba.setMass(mass);
  ba.setEnergy(energy);

  // ************************************************************************
  logFile << "\nGenerate Transverse distribution\n . " << std::endl;
  // ************************************************************************

  UAL::BunchGenerator bunchGenerator;
  PAC::Position emIn;

  int    seed = -100; 

  PAC::Bunch bunch1(np1);
  bunch1.setBeamAttributes(ba);

  emIn.set(em1, 0.0, em1, 0.0, 0.0, 0.0);
  bunchGenerator.addGaussianEllipses(bunch1, twissIn, emIn, seed);

  PAC::Bunch bunch2(np2);
  bunch2.setBeamAttributes(ba);

  emIn.set(em2, 0.0, em2, 0.0, 0.0, 0.0);
  bunchGenerator.addGaussianEllipses(bunch2, twissIn, emIn, seed);

  logFile << "em1 = " << em1 << ", em2 = " << em2 << std::endl;
  logFile << "sigma1 = "   << sqrt(em1*betax) 
	  << ", sigma2 = " << sqrt(em2*betax) 
	  << std::endl;

  // ************************************************************************
  logFile << "\nGenerate Longitudinal distribution\n . " << std::endl;
  // ************************************************************************

  ACCSIM::BunchGenerator accsimGenerator;

  double p2      = energy*energy - mass*mass;
  double v0byc2  = p2/(energy*energy);
  double v0byc   = sqrt(v0byc2);

  double phi2dep = (charge*V*energy*v0byc2)/(2.0*UAL::pi*harmon*eta0*p2);
  phi2dep        = sqrt(fabs(phi2dep));
  double ct2phi  = (2.0*UAL::pi*harmon)*(v0byc/suml);

  double ct2dep  = phi2dep*ct2phi;

  double ctMax  = suml/harmon/v0byc/2.;
  double deMax  = ctMax*ct2dep;

  double ctHalfWidth = ctMax*0.6; 
  double deHalfWidth = ctHalfWidth*ct2dep;

  logFile << "ctHalfWidth  = " << ctHalfWidth 
	  << ", deHalfWidth  = " << deHalfWidth << std::endl;


  accsimGenerator.addBinomialEllipse1D(bunch1, 
				      3,             // m, ~gauss
				      4,             // ct index
				      ctHalfWidth,   // ct half width
				      5,             // de index
				      deHalfWidth,   // de half width
				      0.0,           // alpha
				      seed);         // seed

  accsimGenerator.addBinomialEllipse1D(bunch2, 
				      3,             // m, ~gauss
				      4,             // ct index
				      ctHalfWidth,   // ct half width
				      5,             // de index
				      deHalfWidth,   // de half width
				      0.0,           // alpha
				      seed);         // seed



  // ************************************************************************
  logFile << "\nCalculate rms\n . " << std::endl;
  // ************************************************************************ 

  UAL::BunchAnalyzer bunchAnalyzer;

  PAC::Position emOut;
  PacTwissData twissOut;

  bunchAnalyzer.updateEmit(bunch1, emOut);
  logFile << "ex1 = " << emOut.getX() << ", ey1 = " << emOut.getY() << std::endl;

  bunchAnalyzer.updateEmit(bunch2, emOut);
  logFile << "ex2 = " << emOut.getX() << ", ey2 = " << emOut.getY() << std::endl;

  // ************************************************************************
  logFile << "\nEstimate beam-beam parameters\n . " << std::endl;
  // ************************************************************************ 

  double pradius = 1.54e-18;

  double sigma1 = sqrt(em1*betax);
  double sigma2 = sqrt(em2*betax);

  double ksi1 = nppb1*pradius*betax/(4.*UAL::pi*gamma*sigma1*sigma1);
  double ksi2 = nppb2*pradius*betay/(4.*UAL::pi*gamma*sigma2*sigma2);

  logFile << "ksi1 = " << ksi1 << ", ksi2 = " << ksi2 << std::endl; 


  // ************************************************************************
  logFile << "\nMatrix Tracker . " << std::endl;
  // ************************************************************************ 

  UAL::MatrixTracker matrixTracker;
  matrixTracker.setTwiss(twissIn);

  CMTracker cmTracker;
  cmTracker.setLattice(suml, alpha);
  cmTracker.setTwiss(twissIn);
  cmTracker.setChromaticities(chx, chy);
  cmTracker.setRF(V, lag, harmon);

  // ************************************************************************
  logFile << "\nMatrix Tracker . " << std::endl;
  // ************************************************************************ 

  UAL::BBTracker bbTracker;
  bbTracker.setCoreBunch(nppb1, sqrt(em1*betax));
  bbTracker.setTailBunch(nppb2, sqrt(em2*betax));

  BBDisplay bbDisplay;
  bbDisplay.showBB(7.0, 7.0, nppb1, sqrt(em1*betax), gamma);

  // ************************************************************************
  logFile << "\nNoise . " << std::endl;
  // ************************************************************************ 

  ACCSIM::GaussianGenerator gaussGenerator;
  

  double d_nx, d_ny, xi_n;

  xi_n = gaussGenerator.getNumber(seed);
  d_nx = xi_n/sqrt(1 + n_c);

  xi_n = gaussGenerator.getNumber(seed);
  d_ny = xi_n/sqrt(1 + n_c);

  double qx = qx0;
  double qy = qy0;

  // ************************************************************************
  logFile << "\nROOT graphics . " << std::endl;
  // ************************************************************************

  bool plotFlag = true;

  PAC::Position maxP;
  maxP.set(5*sqrt(6*em2*betax), 
	   5*sqrt(6*em2*gammax), 
	   5*sqrt(6*em2*betay), 
	   5*sqrt(6*em2*gammay), 
	   ctMax, 
	   deMax);

  logFile << "xMax  = " << maxP.getX()  << ", yMax  = " << maxP.getY() << std::endl;
  logFile << "ctMax = " << maxP.getCT()  << ", deMax = " << maxP.getDE() << std::endl;

  BunchDisplay bunchDisplay; 

  if(plotFlag){
    bunchDisplay.setMaxPosition(maxP);
    bunchDisplay.show(0);
    if(gSystem->ProcessEvents());
  }

  // ************************************************************************
  logFile << "\nPropagate bunch . " << std::endl;
  // ************************************************************************
 
  double t1,t2; // time variable
  PAC::Position em1Out, em2Out;

  int updateCounter = 1;

  start_ms();
  t1 = (end_ms());
  for(int it=0; it < nturns; it++){

    // noise

    xi_n = gaussGenerator.getNumber(seed);
    d_nx = (n_c/(1.0 + n_c))*d_nx + (1.0/(1.0 + n_c))*xi_n;
    qx = qx0 + Qm_x*sin(2.0*UAL::pi*it/997.0) + dQ_x*d_nx*sqrt(1 + n_c);

    xi_n = gaussGenerator.getNumber(seed);
    d_ny = (n_c/(1.0 + n_c))*d_ny + (1.0/(1.0 + n_c))*xi_n;
    qy = qy0 + Qm_y*sin(2.0*UAL::pi*it/997.0) + dQ_y*d_ny*sqrt(1 + n_c);

    // bb

    bbTracker.propagate(bunch1);
    bbTracker.propagate(bunch2);

    // map.propagate(bunch);
    // matrixTracker.setTunes(qx, qy);
    // matrixTracker.propagate(bunch1);
    // matrixTracker.propagate(bunch2);

    cmTracker.setTunes(qx, qy);
    cmTracker.propagate(bunch1);
    cmTracker.propagate(bunch2);

    // calculate rms

    bunchAnalyzer.updateEmit(bunch1, em1Out);
    bunchAnalyzer.updateEmit(bunch2, em2Out);
    updateCounter++;

    if(plotFlag) {
      bunchDisplay.update(bunch1);
      bunchDisplay.update(bunch2); 

      if(gSystem->ProcessEvents());
    }   

    if(it%100 == 0){
      t2 = (end_ms());
      std::cout << "turn = " << it + 1 << ", time  = " << t2 - t1 << " ms" << endl;
      t1 = (end_ms());
    }

    if(it%997 == 0) {
      logFile << "turn: " << it << std::endl;
      
      em1Out /= updateCounter;
      logFile << "ex1 = " << em1Out.getX() << ", ey1 = " << em1Out.getY() << std::endl;
      logFile << "sx1 = "   << sqrt(em1Out.getX()*betax) 
	      << ", sy1 = " << sqrt(em1Out.getY()*betay) 
	      << std::endl;      
      em1Out.set(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

      em2Out /= updateCounter;
      logFile << "ex2 = " << em2Out.getX() << ", ey2 = " << em2Out.getY() << std::endl;
      logFile << "sx2 = "   << sqrt(em2Out.getX()*betax) 
	      << ", sy2 = " << sqrt(em2Out.getY()*betay) 
	      << std::endl;  
      em2Out.set(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

      updateCounter = 1;
    }

    if(it%10000 == 0){
      if(plotFlag) {
	bunchDisplay.show(it);
	// sprintf(hName, "phspH%.4d.gif", it);
        // phspCanvas->SaveAs(hName);
      }
    }

  }

#ifndef __CINT__
   theApp.Run();
#endif

}
