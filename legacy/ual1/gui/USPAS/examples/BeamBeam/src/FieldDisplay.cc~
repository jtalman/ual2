#include "BBDisplay.hh"

BBDisplay::BBDisplay()
{
  nbins1 = 100;
  nbins2 = 100;

  bbCanvas   = 0;
  bbH        = 0;

  sextCanvas = 0;
  sextH      = 0;

}

BBDisplay::~BBDisplay()
{
  if(bbH)   delete bbH;    bbH   = 0; 
  if(sextH) delete sextH;  sextH = 0; 
}




void BBDisplay::showBB(double xMax, double yMax, double N, double sigma, double gamma){

  bbCanvas  = new TCanvas("bbCanvas","Beam-Beam x kick");

  bbH = new TH2F("bbH", "beam-beam x kick  ", 
		   100, -xMax, xMax, 
		   100, -yMax, yMax);

  double dx = xMax/nbins1;
  double dy = yMax/nbins2;

  double r_p = 1.54e-18;
  double factor = N*r_p/gamma/sigma;
  double x, y, r2, force;

  for(int i1 =0; i1 < nbins1; i1++){
    for(int i2=0; i2 < nbins2; i2++) {
      x = -xMax + 2*i1*dx;
      y = -yMax + 2*i2*dy;
      r2 = x*x + y*y;
      if(r2 == 0.0) force = 0.0;
      else          force = x*2.0*factor*(1.0 - exp(-r2/2.0))/r2;
      bbH->SetBinContent(i1, i2,  force);
      // bbH->Fill(x, y, force*1000);
    }
  }


  bbCanvas->cd();
  bbH->SetOption("surf1");
  bbH->Draw();

  bbCanvas->Modified();
  bbCanvas->Update();

}

















