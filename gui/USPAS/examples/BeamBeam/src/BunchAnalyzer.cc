#include "UAL/Common/Def.hh"
#include "Optics/PacTwissData.h"
#include "ACCSIM/Base/Def.hh"
#include "ACCSIM/Base/UniformGenerator.hh"

#include "BunchAnalyzer.hh"

UAL::BunchAnalyzer::BunchAnalyzer()
{
}

void UAL::BunchAnalyzer::updateEmit(PAC::Bunch& bunch, PAC::Position& emit)
{

  if(bunch.size() == 0) return;

  PAC::Position orbitOut, emOut;
  PacTwissData twissOut;

  m_accsimAnalyzer.getRMS(bunch, orbitOut, twissOut, emOut);
  emit += emOut;
}

void UAL::BunchAnalyzer::updateTH1F(PAC::Bunch& bunch, TH1F* th1f, int i1)
{
  if(th1f == 0) return;

  for(int ip =0; ip < bunch.size(); ip++){
    if(bunch[ip].isLost()) continue;
    PAC::Position& pos = bunch[ip].getPosition();
    th1f->Fill(pos[i1]);
  }
}

void UAL::BunchAnalyzer::updateTH2F(PAC::Bunch& bunch, TH2F* th2f, int i1, int i2)
{
  if(th2f == 0) return;

  for(int ip =0; ip < bunch.size(); ip++){
    if(bunch[ip].isLost()) continue;
    PAC::Position& pos = bunch[ip].getPosition();
    th2f->Fill(pos[i1], pos[i2]);
  }

}
