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
#include "ACCSIM/Bunch/BunchAnalyzer.hh"

#include "BunchGenerator.hh"
#include "MatrixTracker.hh"
#include "BBTracker.hh"

using namespace UAL;

int main(int argc, char** argv)
{

#ifndef __CINT__
  // TApplication theApp("BeamBeam", &argc, argv);
#endif

  UAL::Shell shell;
  std::ofstream logFile ("./log.out");

  // ************************************************************************
  logFile << "\nParameters." << std::endl;
  // ************************************************************************

  double mass        = UAL::pmass;
  double energy      = 191.5; // 250; 
  double gamma       = energy/mass;

  int    np          = 10000; 
  int    np1         = np; 
  int    np2         = np - np1;

  double nppb        = 1.4e+11;

  double em1         = 15.0e-6/6.0/gamma; // m*rad
  double em2         = 15.0e-6/6.0/gamma; // m*rad

  double qx0         = 28.228; // 28.69
  double qy0         = 28.243; // 28.68

  double sigma_N_e = 0.03;
  double n_c = 1.0e4;
  double dQ_x = 3e-5; //tune noise amplitude
  double dQ_y = 3e-5;
  double Qm_x = 0e-4; //tune modulation amplitude
  double Qm_y = 0e-4;

  int nturns         = 100; 

  // ************************************************************************
  logFile << "\nDefine the space of Taylor maps." << std::endl;
  // ************************************************************************
 
  shell.setMapAttributes(Args() << Arg("order", 7)); 

  // ************************************************************************
  logFile << "\nSet Lattice ." << std::endl;
  // ************************************************************************

  double betax  = 1.0;
  double betay  = 1.0;
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

  // ************************************************************************
  logFile << "\nBeam attributes . " << std::endl;
  // ************************************************************************ 

  PAC::BeamAttributes ba;

  ba.setMass(mass);
  ba.setEnergy(energy);
  ba.setMacrosize(nppb/np);

  // ************************************************************************
  logFile << "\nGenerate distribution\n . " << std::endl;
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

  logFile << "em1 = " << em1 << "em2 = " << em2 << std::endl;

  // ************************************************************************
  logFile << "\nCalculate rms\n . " << std::endl;
  // ************************************************************************ 

  ACCSIM::BunchAnalyzer bunchAnalyzer;

  PAC::Position orbitOut, emOut;
  PacTwissData twissOut;

  if(bunch1.size() > 0) {
    bunchAnalyzer.getRMS(bunch1, orbitOut, twissOut, emOut);
    logFile << "ex1 = " << emOut.getX() << ", ey1 = " << emOut.getY() << std::endl;
  }

  if(bunch2.size() > 0) {
    bunchAnalyzer.getRMS(bunch2, orbitOut, twissOut, emOut);
    logFile << "ex2 = " << emOut.getX() << ", ey2 = " << emOut.getY() << std::endl;
  }

  // ************************************************************************
  logFile << "\nEstimate beam-beam parameters\n . " << std::endl;
  // ************************************************************************ 

  double pradius = 1.54e-18;

  double sigma1 = sqrt(em1*betax);
  double sigma2 = sqrt(em2*betax);

  double ksi1 = np1*(nppb/np)*pradius*betax/(4.*UAL::pi*gamma*sigma1*sigma1);
  double ksi2 = np2*(nppb/np)*pradius*betay/(4.*UAL::pi*gamma*sigma2*sigma2);

  logFile << "ksi1 = " << ksi1 << ", ksi2 = " << ksi2 << std::endl; 

  // ************************************************************************
  logFile << "\nMatrix Tracker . " << std::endl;
  // ************************************************************************ 

  UAL::MatrixTracker matrixTracker;
  matrixTracker.setTwiss(twissIn);

  // ************************************************************************
  logFile << "\nMatrix Tracker . " << std::endl;
  // ************************************************************************ 

  UAL::BBTracker bbTracker;
  bbTracker.setCoreBunch(np1*(nppb/np), sqrt(em1*betax));
  bbTracker.setTailBunch(np2*(nppb/np), sqrt(em2*betax));

  // ************************************************************************
  logFile << "\nNoise . " << std::endl;
  // ************************************************************************ 

  double xi_n;

  /*
  xi_n = gauss(ran1, ran2, 1, 0);
  double d_nx = xi_n/sqrt(1 + n_c);

  xi_n = gauss(ran1, ran2, 1, 0);
  double d_ny = xi_n/sqrt(1 + n_c);
  */

  double qx = qx0;
  double qy = qy0;

  // ************************************************************************
  logFile << "\nROOT graphics . " << std::endl;
  // ************************************************************************

  double xMax = sqrt(6*em2*betax), pxMax = sqrt(6*em2*gammax);
  double yMax = sqrt(6*em2*betay), pyMax = sqrt(6*em2*gammay);

  logFile << "xMax  = " << xMax << ", pxMax  = " << pxMax << std::endl;
  logFile << "yMax  = " << yMax << ", pyMax  = " << pyMax << std::endl;

  double ctMax = 1.0, deMax = 1.0;
 
  TCanvas* phspCanvas  = new TCanvas("phspCanvas","Phase space");
  phspCanvas->Divide(2,2);

  TH2F *xpxH = 0, *ypyH = 0;
  TH1F *xH   = 0,   *yH = 0;
  char hName[24];

  TCanvas* xy3DCanvas  = new TCanvas("xy3DCanvas","xy 3D");
 
  TH2F *xy3DH = 0;

  // ************************************************************************
  logFile << "\nPropagate bunch . " << std::endl;
  // ************************************************************************
 
  double t1,t2; // time variable
  start_ms();

  bool plotFlag = false;

  for(int it=0; it < nturns; it++){

    t1 = (end_ms());

    // noise
    /*
    xi_n = gauss(ran1, ran2, 1, 0);
    d_nx = (n_c/(1.0 + n_c))*d_nx + (1.0/(1.0 + n_c))*xi_n;
    qx = qx0 + Qm_x*sin(2.0*UAL::pi*k/997.0) + dQ_x*d_nx*sqrt(1 + n_c);

    xi_n = gauss(ran1, ran2, 1, 0);
    d_ny = (n_c/(1.0 + n_c))*d_ny + (1.0/(1.0 + n_c))*xi_n;
    qy = qy0 + Qm_y*sin(2.0*UAL::pi*k/997.0) + dQ_y*d_ny*sqrt(1 + n_c);
    */

    matrixTracker.setTunes(qx, qy);

    bbTracker.propagate(bunch1);
    bbTracker.propagate(bunch2);

    // map.propagate(bunch);
    matrixTracker.propagate(bunch1);
    matrixTracker.propagate(bunch2);


    if(plotFlag) {

      if(xpxH)     delete xpxH;
      if(ypyH)     delete ypyH;
      if(xH)       delete xH;
      if(yH)       delete yH;   
      if(xy3DH)    delete xy3DH;  

      sprintf(hName, "xpxH%.3d", it);
      xpxH = new TH2F(hName, "x-px ", 
		     100, -1.5*xMax, 1.5*xMax, 
		     100, -1.5*pxMax, 1.5*pxMax);

      sprintf(hName, "ypyH%.3d", it);
      ypyH = new TH2F(hName, "y-py ", 
		     100, -1.5*yMax, 1.5*yMax, 
		     100, -1.5*pyMax, 1.5*pyMax);

      sprintf(hName, "xH%.3d", it);
      xH = new TH1F(hName, "x ", 
		    100, -1.5*xMax, 1.5*xMax);

      sprintf(hName, "yH%.3d", it);
      yH = new TH1F(hName, "y ", 
		    100, -1.5*xMax, 1.5*xMax);

      sprintf(hName, "xy3DH%.3d", it);
      xy3DH = new TH2F(hName, "x-y ", 
		     100, -1.5*xMax, 1.5*xMax, 
		     100, -1.5*yMax, 1.5*yMax);

      for(int ip =0; ip < bunch1.size(); ip++){
	if(bunch1[ip].isLost()) continue;
	PAC::Position& pos = bunch1[ip].getPosition();
	xpxH->Fill(pos.getX(), pos.getPX());
	ypyH->Fill(pos.getY(), pos.getPY());
	xH->Fill(pos.getX());
	yH->Fill(pos.getY());
	xy3DH->Fill(pos.getX(), pos.getY());
      }

      phspCanvas->cd(1);
      xpxH->SetOption("colz");
      xpxH->Draw();
      phspCanvas->cd(2);
      ypyH->SetOption("colz");
      ypyH->Draw();
      phspCanvas->cd(3);
      xH->Draw();
      phspCanvas->cd(4);
      yH->Draw();

      phspCanvas->Modified();
      phspCanvas->Update();

      xy3DCanvas->cd();
      xy3DH->SetOption("lego");
      xy3DH->Draw();

      xy3DCanvas->Modified();
      xy3DCanvas->Update();

      if(gSystem->ProcessEvents());

      sprintf(hName, "phspH%.4d.gif", it);
      phspCanvas->SaveAs(hName);

    }

    t2 = (end_ms());
    std::cout << "turn = " << it + 1 << ", time  = " << t2 - t1 << " ms" << endl;

    if(bunch1.size() > 0) {
      bunchAnalyzer.getRMS(bunch1, orbitOut, twissOut, emOut);
      logFile << "ex1 = " << emOut.getX() << ", ey1 = " << emOut.getY() << std::endl;
    }
    if(bunch2.size() > 0) {
      bunchAnalyzer.getRMS(bunch2, orbitOut, twissOut, emOut);
      logFile << "ex2 = " << emOut.getX() << ", ey2 = " << emOut.getY() << std::endl;
    }

  }

#ifndef __CINT__
   // theApp.Run();
#endif

}
