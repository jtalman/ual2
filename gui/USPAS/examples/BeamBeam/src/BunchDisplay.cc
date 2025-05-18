#include "BunchDisplay.hh"

BunchDisplay::BunchDisplay()
{
  nbins1 = 100;
  nbins2 = 100;

  m_updateCounter = 1;

  initPhspCanvas();
  initXy3dCanvas();
  initLongCanvas();

}

BunchDisplay::~BunchDisplay()
{
  deleteA();
  deleteB();
}

void BunchDisplay::setMaxPosition(PAC::Position& maxP)
{
  m_maxP = maxP;
}

void BunchDisplay::update(PAC::Bunch& bunch)
{
  updatePhspA(bunch);
  updateXy3dA(bunch);
  updateLongA(bunch);

  m_updateCounter++;
}

void BunchDisplay::updatePhspA(PAC::Bunch& bunch){

  bunchAnalyzer.updateTH1F(bunch, xA,  0);
  bunchAnalyzer.updateTH1F(bunch, yA,  2);

  bunchAnalyzer.updateTH2F(bunch, xpxA,  0, 1);
  bunchAnalyzer.updateTH2F(bunch, ypyA,  2, 3);
}

void BunchDisplay::updateXy3dA(PAC::Bunch& bunch){

  bunchAnalyzer.updateTH2F(bunch, xy3DA, 0, 2);
}

void BunchDisplay::updateLongA(PAC::Bunch& bunch){

    bunchAnalyzer.updateTH1F(bunch, ctA,  4);
    bunchAnalyzer.updateTH1F(bunch, deA,  5);
    bunchAnalyzer.updateTH2F(bunch, ctdeA, 4, 5);
}


void BunchDisplay::show(int it)
{

  if(xpxA  == 0) createPhspA(0);
  if(xy3DA == 0) createXy3dA(0);
  if(ctdeA == 0) createLongA(0);

  deletePhspB();
  deleteXy3dB();
  deleteLongB();

  createPhspB(it);
  createXy3dB(it);
  createLongB(it);

  copyA2B();

  drawPhspCanvas();
  drawXy3dCanvas();
  drawLongCanvas();

  cleanA();

}

void BunchDisplay::initPhspCanvas(){

  phspCanvas  = new TCanvas("phspCanvas","Phase space");
  phspCanvas->Divide(2,2);

  xpxA = 0;
  ypyA = 0;
  xA   = 0;
  yA   = 0;

  xpxB = 0;
  ypyB = 0;
  xB   = 0;
  yB   = 0;
}

void BunchDisplay::initXy3dCanvas(){

  xy3DCanvas  = new TCanvas("xy3DCanvas","xy 3D");

  xy3DA = 0;
  xy3DB = 0;

}

void BunchDisplay::initLongCanvas(){

  longCanvas  = new TCanvas("longCanvas", "Long. phase space");
  longCanvas->Divide(2,2);

  ctdeA = 0;
  ctA   = 0;
  deA   = 0;

  ctdeB = 0;
  ctB   = 0;
  deB   = 0;
}

void BunchDisplay::drawPhspCanvas(){

  phspCanvas->cd(1);
  xpxB->SetOption("colz");
  xpxB->Draw();

  phspCanvas->cd(2);
  ypyB->SetOption("colz");
  ypyB->Draw();

  phspCanvas->cd(3);
  xB->Draw();
  phspCanvas->cd(4);
  yB->Draw();

  phspCanvas->Modified();
  phspCanvas->Update();
}

void BunchDisplay::drawXy3dCanvas(){

  xy3DCanvas->cd();
  xy3DB->SetOption("lego");
  xy3DB->Draw();

  xy3DCanvas->Modified();
  xy3DCanvas->Update();

}

void BunchDisplay::drawLongCanvas(){

  longCanvas->cd(1);
  ctdeB->SetOption("colz");
  ctdeB->Draw();
      
  longCanvas->cd(3);
  ctB->Draw();  // ctH->SetFillColor(kGreen); ctH->Draw("C");
   
  longCanvas->cd(4);
  deB->Draw(); // deH->SetFillColor(kRed); deH->Draw("C");

  longCanvas->Modified();
  longCanvas->Update();

}

void BunchDisplay::createPhspA(int it){

  char hName[24];

  double xMax  = m_maxP.getX();
  double pxMax = m_maxP.getPX();
  double yMax  = m_maxP.getY();
  double pyMax = m_maxP.getPY();


  sprintf(hName, "xpxA%.3d", it);
  xpxA = new TH2F(hName, "x-px ", 
		  100, -1.5*xMax, 1.5*xMax, 
		  100, -1.5*pxMax, 1.5*pxMax);

  sprintf(hName, "ypyA%.3d", it);
  ypyA = new TH2F(hName, "y-py ", 
		  100, -1.5*yMax, 1.5*yMax, 
		  100, -1.5*pyMax, 1.5*pyMax);

  sprintf(hName, "xA%.3d", it);
  xA = new TH1F(hName, "x ", 
		100, -1.5*xMax, 1.5*xMax);

  sprintf(hName, "yA%.3d", it);
  yA = new TH1F(hName, "y ", 
		100, -1.5*xMax, 1.5*xMax);
}

void BunchDisplay::createXy3dA(int it){

  char hName[24];

  double xMax  = m_maxP.getX();
  double yMax  = m_maxP.getY();

  sprintf(hName, "xy3DA%.3d", it);
  xy3DA = new TH2F(hName, "x-y ", 
		   100, -1.5*xMax, 1.5*xMax, 
		   100, -1.5*yMax, 1.5*yMax);

}

void BunchDisplay::createLongA(int it){

  char hName[24];

  double ctMax = m_maxP.getCT();
  double deMax = m_maxP.getDE();

  sprintf(hName, "ctdeA%.3d", it);
  ctdeA = new TH2F(hName, "x-px ", 
		   100, -1.5*ctMax, 1.5*ctMax, 
		   100, -1.5*deMax, 1.5*deMax);

  sprintf(hName, "ctA%.3d", it);
  ctA = new TH1F(hName, "ct ", 
		100, -1.5*ctMax, 1.5*ctMax);

  sprintf(hName, "deA%.3d", it);
  deA = new TH1F(hName, "de ", 
		100, -1.5*deMax, 1.5*deMax);

}

void BunchDisplay::createPhspB(int it){

  char hName[24];

  double xMax  = m_maxP.getX();
  double pxMax = m_maxP.getPX();
  double yMax  = m_maxP.getY();
  double pyMax = m_maxP.getPY();


  sprintf(hName, "xpxB%.3d", it);
  xpxB = new TH2F(hName, "x-px ", 
		  100, -1.5*xMax, 1.5*xMax, 
		  100, -1.5*pxMax, 1.5*pxMax);

  sprintf(hName, "ypyB%.3d", it);
  ypyB = new TH2F(hName, "y-py ", 
		  100, -1.5*yMax, 1.5*yMax, 
		  100, -1.5*pyMax, 1.5*pyMax);

  sprintf(hName, "xB%.3d", it);
  xB = new TH1F(hName, "x ", 
		100, -1.5*xMax, 1.5*xMax);

  sprintf(hName, "yB%.3d", it);
  yB = new TH1F(hName, "y ", 
		100, -1.5*xMax, 1.5*xMax);
}

void BunchDisplay::createXy3dB(int it){

  char hName[24];

  double xMax  = m_maxP.getX();
  double yMax  = m_maxP.getY();

  sprintf(hName, "xy3DB%.3d", it);
  xy3DB = new TH2F(hName, "x-y ", 
		   100, -1.5*xMax, 1.5*xMax, 
		   100, -1.5*yMax, 1.5*yMax);

}

void BunchDisplay::createLongB(int it){

  char hName[24];

  double ctMax = m_maxP.getCT();
  double deMax = m_maxP.getDE();

  sprintf(hName, "ctdeB%.3d", it);
  ctdeB = new TH2F(hName, "x-px ", 
		   100, -1.5*ctMax, 1.5*ctMax, 
		   100, -1.5*deMax, 1.5*deMax);

  sprintf(hName, "ctB%.3d", it);
  ctB = new TH1F(hName, "ct ", 
		100, -1.5*ctMax, 1.5*ctMax);

  sprintf(hName, "deB%.3d", it);
  deB = new TH1F(hName, "de ", 
		100, -1.5*deMax, 1.5*deMax);

}

void BunchDisplay::cleanA()
{
  cleanPhspA();
  cleanXy3dA();
  cleanLongA();
  m_updateCounter = 1;
}

void BunchDisplay::cleanPhspA()
{
  for(int i1 =0; i1 < nbins1; i1++){
      xA->SetBinContent(i1, 0.0);
      yA->SetBinContent(i1, 0.0);    
    for(int i2=0; i2 < nbins2; i2++) {
      xpxA->SetBinContent(i1, i2, 0.0);
      ypyA->SetBinContent(i1, i2, 0.0);
    }
  }
}

void BunchDisplay::cleanXy3dA()
{
  for(int i1 =0; i1 < nbins1; i1++){
    for(int i2=0; i2 < nbins2; i2++) {
      xy3DA->SetBinContent(i1, i2, 0.0);
    }
  }
}

void BunchDisplay::cleanLongA()
{
  for(int i1 =0; i1 < nbins1; i1++){
      ctA->SetBinContent(i1, 0.0);
      deA->SetBinContent(i1, 0.0);    
    for(int i2=0; i2 < nbins2; i2++) {
      ctdeA->SetBinContent(i1, i2, 0.0);
    }
  }
}

void BunchDisplay::copyA2B()
{
  copyPhspA2B();
  copyXy3dA2B();
  copyLongA2B();
}

void BunchDisplay::copyPhspA2B()
{
  double counterW = 1./m_updateCounter;

  for(int i1 =0; i1 < nbins1; i1++){
      xB->SetBinContent(i1, xA->GetBinContent(i1)*counterW);
      yB->SetBinContent(i1, yA->GetBinContent(i1)*counterW);    
    for(int i2=0; i2 < nbins2; i2++) {
      xpxB->SetBinContent(i1, i2,  xpxA->GetBinContent(i1, i2)*counterW);
      ypyB->SetBinContent(i1, i2,  ypyA->GetBinContent(i1, i2)*counterW);
    }
  }
}

void BunchDisplay::copyXy3dA2B()
{
  double counterW = 1./m_updateCounter;

  for(int i1 =0; i1 < nbins1; i1++){
    for(int i2=0; i2 < nbins2; i2++) {
      xy3DB->SetBinContent(i1, i2, xy3DA->GetBinContent(i1, i2)*counterW);
    }
  }
}

void BunchDisplay::copyLongA2B()
{
  double counterW = 1./m_updateCounter;

  for(int i1 =0; i1 < nbins1; i1++){
      ctB->SetBinContent(i1, ctA->GetBinContent(i1)*counterW);
      deB->SetBinContent(i1, deA->GetBinContent(i1)*counterW);    
    for(int i2=0; i2 < nbins2; i2++) {
      ctdeB->SetBinContent(i1, i2,  ctdeA->GetBinContent(i1, i2)*counterW);
    }
  }
}


void BunchDisplay::deleteA()
{
  deletePhspA();
  deleteXy3dA();
  deleteLongA();
}

void BunchDisplay::deletePhspA()
{
  if(xpxA) delete xpxA;    xpxA  = 0;
  if(ypyA) delete ypyA;    ypyA  = 0;
  if(xA)   delete xA;      xA    = 0;
  if(yA)   delete yA;      yA    = 0;
}

void BunchDisplay::deleteXy3dA()
{
  if(xy3DA) delete xy3DA;  xy3DA = 0; 
}

void BunchDisplay::deleteLongA()
{
  if(ctdeA) delete ctdeA;    ctdeA  = 0;
  if(ctA)   delete ctA;      ctA    = 0;
  if(deA)   delete deA;      deA    = 0;

}

void BunchDisplay::deleteB()
{
  deletePhspB();
  deleteXy3dB();
  deleteLongB();
}

void BunchDisplay::deletePhspB()
{
  if(xpxB) delete xpxB;    xpxB  = 0;
  if(ypyB) delete ypyB;    ypyB  = 0;
  if(xB)   delete xB;      xB    = 0;
  if(yB)   delete yB;      yB    = 0;
}

void BunchDisplay::deleteXy3dB()
{
  if(xy3DB) delete xy3DB;  xy3DB = 0; 
}

void BunchDisplay::deleteLongB()
{
  if(ctdeB) delete ctdeB;    ctdeB  = 0;
  if(ctB)   delete ctB;      ctB    = 0;
  if(deB)   delete deB;      deB    = 0;
}
