#include <math.h>

#include "UAL/APF/PropagatorFactory.hh"
#include "UAL/QT/Player/TurnCounter.hh"

#include "UAL/QT/Player/RightAlignedTableItem.hh"
#include "UAL/QT/Player/TunefitEditor.hh"
#include "TuneRamper.hh"

PacLattice UAL::USPAS::TuneRamper::s_lattice;
Teapot     UAL::USPAS::TuneRamper::s_teapot;

UAL::USPAS::TuneRamper::TuneRamper()
  : TEAPOT::BasicTracker()
{
  m_q0 = 0.0;
  m_q1 = 0.0;
}

UAL::USPAS::TuneRamper::TuneRamper(const UAL::USPAS::TuneRamper& st)
  : TEAPOT::BasicTracker(st)
{
}

UAL::USPAS::TuneRamper::~TuneRamper()
{
}

UAL::PropagatorNode* UAL::USPAS::TuneRamper::clone()
{
  return new UAL::USPAS::TuneRamper(*this);
}

void UAL::USPAS::TuneRamper::setLatticeElements(const UAL::AcceleratorNode& sequence, 
						int is0, int is1,
						const UAL::AttributeSet& attSet)
{
  TEAPOT::BasicTracker::setLatticeElements(sequence, is0, is1, attSet);

  const PacLattice& lattice     = (PacLattice&) sequence;
  PAC::BeamAttributes ba = (PAC::BeamAttributes&) attSet;

  // Initialize lattice 
  if(s_lattice.name() != lattice.name()){
    s_lattice = lattice;
    s_teapot.use(lattice);
  }

}


void UAL::USPAS::TuneRamper::propagate(UAL::Probe& probe)
{
  std::string b1f="quadhf";
  std::string b1d="quadvf";

  double tunex = 2.247;
  double tuney = 1.25;

  int turn = UAL::QT::TurnCounter::getInstance()->getTurn();
  if((turn/10)*10 == turn) {
    tunex += turn*0.000006;
      std::cout << "TuneRamper turn = " << turn << ", tunex=" << tunex << std::endl;
    UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance();
    optics.tunefit(tunex, tuney, b1f, b1d);
    optics.calculate();

  }
}


UAL::USPAS::TuneRamperRegister::TuneRamperRegister()
  : space(6, 5)
{
  UAL::PropagatorNodePtr nodePtr(new UAL::USPAS::TuneRamper());
  UAL::PropagatorFactory::getInstance().add("UAL::USPAS::TuneRamper", nodePtr);
}

static UAL::USPAS::TuneRamperRegister the_UAL_USPAS_TuneRamper_Register; 




