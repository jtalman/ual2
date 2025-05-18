#include "FieldDisplay.hh"

FieldDisplay::FieldDisplay()
{
  nbins1 = 100;
  nbins2 = 100;

  bbCanvas   = 0;
  bbH        = 0;

  sextCanvas = 0;
  sextH      = 0;

}

FieldDisplay::~FieldDisplay()
{
  if(bbH)   delete bbH;    bbH   = 0; 
  if(sextH) delete sextH;  sextH = 0; 
}




void FieldDisplay::showBB(double xMax, double yMax, double N, double sigma, double gamma){

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

void FieldDisplay::showSextupole(double xMax, double yMax, double kl2, double sigma, double gamma){

  sextCanvas  = new TCanvas("sextCanvas","Sextupole x kick");

  sextH = new TH2F("sextH", "sext x kick  ", 
		   100, -xMax, xMax, 
		   100, -yMax, yMax);

  double dx = xMax/nbins1;
  double dy = yMax/nbins2;

  double r_p = 1.54e-18;
  double factor = sqrt(betax)*kl2/2.0*sigma*sigma;
  double x, y, r2, force;

  for(int i1 =0; i1 < nbins1; i1++){
    for(int i2=0; i2 < nbins2; i2++) {
      x = -xMax + 2*i1*dx;
      r2 = x*x;
      if(r2 == 0.0) force = 0.0;
      else          force = factor*r2;
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

















