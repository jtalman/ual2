#include "UAL/APF/PropagatorFactory.hh"
#include "PAC/Beam/Bunch.hh"
#include "SMF/PacLattice.h"

#include "NoisyMonitor.hh"

int UAL::USPAS::NoisyMonitor::m_seed = -100;

UAL::USPAS::NoisyMonitor::NoisyMonitor() 
{
  //  m_seed = -100;
}

UAL::USPAS::NoisyMonitor::NoisyMonitor(const UAL::USPAS::NoisyMonitor& t)
{
  //  m_seed = t.m_seed;
}

UAL::USPAS::NoisyMonitor::~NoisyMonitor()
{
}

UAL::PropagatorNode* UAL::USPAS::NoisyMonitor::clone()
{
  return new UAL::USPAS::NoisyMonitor(*this);
}

void UAL::USPAS::NoisyMonitor::propagate(UAL::Probe& probe)
{

  PAC::Bunch& bunch = static_cast<PAC::Bunch&>(probe);
  
  int counter = 0;
  PAC::Position pos;
  for(int i=0; i < bunch.size(); i++){
    if(bunch[i].isLost()) continue;
    pos += bunch[i].getPosition();
    counter++;
  }

  pos /= counter; 
  double x = pos.getX();

  pos.setX(x + 1.0e-3*(-1 + 2*ran1(m_seed)));

  m_data.push_back(pos);
}

double UAL::USPAS::NoisyMonitor::ran1(int& idum)
{
  int j;
  long k; 
  static long iy=0; 
  static long iv[NTAB];
  float temp;
 
  if(idum <= 0 || !iy)
  {
    if (-(idum) < 1) idum=1;
    else idum = -(idum);
    for (j=NTAB+7; j>=0; j--) {
      k=(idum)/IQ;
      idum=IA*(idum-k*IQ) - IR*k;
      if (idum < 0) idum += IM;
      if (j< NTAB) iv[j] = idum;
    }
    iy=iv[0];
  }
 
  k=(idum)/IQ;
  idum=IA*(idum-k*IQ)-IR*k;
  if (idum < 0) idum += IM;
  j=iy/NDIV;
  iy=iv[j];
  iv[j]=idum;
  if ((temp=AM*iy) > RNMX) return RNMX; 
  else return temp;  
}

UAL::USPAS::NoisyMonitorRegister::NoisyMonitorRegister()
{
  UAL::PropagatorNodePtr nodePtr(new UAL::USPAS::NoisyMonitor());
  UAL::PropagatorFactory::getInstance().add("UAL::USPAS::NoisyMonitor", nodePtr);
}

static UAL::USPAS::NoisyMonitorRegister the_UAL_USPAS_NoisyMonitor_Register; 
