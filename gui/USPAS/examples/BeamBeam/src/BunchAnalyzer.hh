#ifndef UAL_BUNCH_ANALYZER_HH
#define UAL_BUNCH_ANALYZER_HH

#ifndef __CINT__

#include <TH2D.h>

#endif

#include "PAC/Beam/Bunch.hh"
#include "ACCSIM/Bunch/BunchAnalyzer.hh"


namespace UAL {


  class BunchAnalyzer
  {

  public:

    /** Constructor */
    BunchAnalyzer();  

    void updateEmit(PAC::Bunch& bunch, PAC::Position& emit);
    void updateTH1F(PAC::Bunch& bunch, TH1F* th2f, int i1);
    void updateTH2F(PAC::Bunch& bunch, TH2F* th2f, int i1, int i2);
  

  private:

    ACCSIM::BunchAnalyzer m_accsimAnalyzer;

  };
};

#endif

