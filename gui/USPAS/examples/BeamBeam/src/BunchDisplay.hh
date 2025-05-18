#ifndef UAL_BUNCH_DISPLAY_HH
#define UAL_BUNCH_DISPLAY_HH

#ifndef __CINT__

#include <TSystem.h>
#include <TObject.h>
#include <TNtupleD.h>
#include <TH2D.h>
#include <TCanvas.h>

#endif

#include "PAC/Beam/Position.hh"
#include "BunchAnalyzer.hh"

  class BunchDisplay
  {

  public:

    /** Constructor */
    BunchDisplay();  

    /** Destructor */
    virtual ~BunchDisplay();

    void setMaxPosition(PAC::Position& maxP);
    void update(PAC::Bunch& bunch);
    void show(int iturn);
 

  private:

    int m_updateCounter;
    UAL::BunchAnalyzer bunchAnalyzer;

    PAC::Position m_maxP;

    TCanvas* phspCanvas;

    int nbins1, nbins2;

    TH2F *xpxA, *ypyA, *xpxB, *ypyB;
    TH1F *xA, *yA, *xB, *yB; 

    TCanvas* xy3DCanvas;
    TH2F *xy3DA, *xy3DB;

    TCanvas* longCanvas;

    TH2F *ctdeA, *ctdeB;
    TH1F *ctA, *deA, *ctB, *deB; 

  private:

    void cleanA();
    void copyA2B();
    void deleteA(); 
    void deleteB();

    void initPhspCanvas();
    void updatePhspA(PAC::Bunch& bunch);
    void drawPhspCanvas();
    void createPhspA(int it);
    void cleanPhspA();
    void deletePhspA();
    void createPhspB(int it);
    void deletePhspB();
    void copyPhspA2B();

    void initXy3dCanvas();
    void updateXy3dA(PAC::Bunch& bunch);
    void drawXy3dCanvas();
    void createXy3dA(int it);
    void cleanXy3dA();
    void deleteXy3dA();
    void createXy3dB(int it);
    void deleteXy3dB(); 
    void copyXy3dA2B();

    void initLongCanvas();
    void updateLongA(PAC::Bunch& bunch);
    void drawLongCanvas();
    void createLongA(int it);
    void cleanLongA();
    void deleteLongA();
    void createLongB(int it);
    void deleteLongB();
    void copyLongA2B();
  };





#endif
