#ifndef UAL_BB_DISPLAY_HH
#define UAL_BB_DISPLAY_HH

#ifndef __CINT__

#include <TSystem.h>
#include <TObject.h>
#include <TNtupleD.h>
#include <TH2D.h>
#include <TCanvas.h>

#endif

#include "PAC/Beam/Position.hh"
#include "BunchAnalyzer.hh"

  class BBDisplay
  {

  public:

    /** Constructor */
    BBDisplay();  

    /** Destructor */
    virtual ~BBDisplay();

    void showBB(double xMax, double yMax, double N, double sigma, double gamma);
 

  private:

    int nbins1, nbins2;

    TCanvas* bbCanvas;
    TH2F *bbH;

    TCanvas* sextCanvas;
    TH2F *sextH;

  private:

  };





#endif
