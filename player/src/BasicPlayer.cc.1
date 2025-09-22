#include <iostream>

#include <qaction.h>
#include <qapplication.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qsettings.h>
#include <qstatusbar.h>
#include <qstringlist.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qslider.h>
#include <qwidgetstack.h>

#include "UAL/UI/OpticsCalculator.hh"

#include "UAL/QT/Player/BeamEditor.hh"
#include "UAL/QT/Player/ParameterViewer.hh"
#include "UAL/QT/Player/PropagatorViewer.hh"
#include "UAL/QT/Player/TunefitEditor.hh"
#include "UAL/QT/Player/ChromfitEditor.hh"

// #include "UAL/COIN3D/Viewers/BunchViewer.hh"
#include "UAL/ROOT/Viewers/PoincareXPxViewer.hh"
#include "UAL/ROOT/Viewers/PoincareYPyViewer.hh"
#include "UAL/ROOT/Viewers/PoincareCtDeViewer.hh"
#include "UAL/ROOT/Viewers/BunchXPxViewer.hh"
#include "UAL/ROOT/Viewers/BunchYPyViewer.hh"
#include "UAL/ROOT/Viewers/BunchCtDeViewer.hh"
#include "UAL/ROOT/Viewers/MountainRangeViewer.hh"
#include "UAL/ROOT/Viewers/BunchTBTViewer.hh"
#include "UAL/ROOT/Viewers/MonitorFFTViewer.hh"
#include "UAL/ROOT/Viewers/OrbitViewer.hh"
#include "UAL/ROOT/Viewers/TwissViewer.hh"
#include "UAL/ROOT/Viewers/BpmSvd1DViewer.hh"

#include "BasicPlayer.hh"

UAL::USPAS::BasicPlayer::BasicPlayer() // QWidget *parent, const char *name)
  : UAL::QT::BasicPlayer() // parent, name)
{
}

void UAL::USPAS::BasicPlayer::setShell(UAL::QT::PlayerShell* shell)
{
std::cerr << __FILE__ << ": " << __LINE__ << "enter void UAL::USPAS::BasicPlayer::setShell\n";
  UAL::QT::BasicPlayer::setShell(shell);
  initPlayer();
std::cerr << __FILE__ << ": " << __LINE__ << "leave void UAL::USPAS::BasicPlayer::setShell\n";
}

void UAL::USPAS::BasicPlayer::initPlayer()
{
std::cerr << __FILE__ << ": " << __LINE__ << "enter void UAL::USPAS::BasicPlayer::initPlayer\n";
  QStringList strList;
  strList.push_back("Bunch 3D");

  listView->setRootIsDecorated(true);
  listView->setSortColumn(-1);

  QListViewItem* infoDir            = new QListViewItem(listView, tr("Info"));
  infoDir->setOpen(true);
  QListViewItem* adxfParameterItem  = new QListViewItem(infoDir, tr("Parameters"));
  QListViewItem* propagatorsItem    = new QListViewItem(infoDir, tr("Propagators"));

  QListViewItem* inputDir           = new QListViewItem(listView, infoDir, tr("Input"));
  inputDir->setOpen(true);
  QListViewItem* beamEditorItem     = new QListViewItem(inputDir, tr("Beam"));
  QListViewItem* tunefitEditorItem  = new QListViewItem(inputDir, tr("Tune Fitting"));
  QListViewItem* chromfitEditorItem = new QListViewItem(inputDir, tr("Chrom Fitting"));

  QListViewItem* preDir             = new QListViewItem(listView, inputDir, tr("Preprocessing"));
  preDir->setOpen(true);
  QListViewItem* orbitItem          = new QListViewItem(preDir, tr("Orbit"));
  QListViewItem* twissItem          = new QListViewItem(preDir, tr("Twiss"));

  QListViewItem* graphicsDir        = new QListViewItem(listView, preDir, tr("Animated Graphics"));
  graphicsDir->setOpen(true);
  // QListViewItem* bunch3DItem       = new QListViewItem(graphicsDir, tr("Bunch 3D"));
  QListViewItem* poincareXPxItem   = new QListViewItem(graphicsDir, tr("Poincare X-PX"));
  QListViewItem* poincareYPyItem   = new QListViewItem(graphicsDir, tr("Poincare Y-PY"));
  QListViewItem* poincareCtDeItem  = new QListViewItem(graphicsDir, tr("Poincare CT-DE"));
  QListViewItem* bunchXPxItem      = new QListViewItem(graphicsDir, tr("Bunch X-PX"));
  QListViewItem* bunchYPyItem      = new QListViewItem(graphicsDir, tr("Bunch Y-PY"));
  QListViewItem* bunchCtDeItem     = new QListViewItem(graphicsDir, tr("Bunch CT-DE"));
  QListViewItem* mountainRangeItem = new QListViewItem(graphicsDir, tr("Mountain Range"));
  QListViewItem* bmpTBTItem        = new QListViewItem(graphicsDir, tr("BPM TBT"));

  QListViewItem* outputDir         = new QListViewItem(listView, graphicsDir, tr("Post-processing"));
  outputDir->setOpen(true);
  QListViewItem* fftItem           = new QListViewItem(outputDir, tr("FFT"));
  QListViewItem* svdXItem          = new QListViewItem(outputDir, tr("PCA X"));
 

  UAL::QT::BeamEditor* beamEditor = new UAL::QT::BeamEditor(widgetStack);
  beamEditor->setShell(p_shell);
  beamEditor->setPlayer(this);
  addEditor("UAL::QT::BeamEditor", beamEditor);
  widgetStack->addWidget(beamEditor, 1 );

  UAL::QT::ParameterViewer* parameterViewer = new UAL::QT::ParameterViewer(widgetStack);
  addEditor("UAL::QT::ParameterViewer", parameterViewer);
  widgetStack->addWidget(parameterViewer, 2 );

  UAL::QT::ChromfitEditor* chromfitEditor = new UAL::QT::ChromfitEditor(widgetStack);
  chromfitEditor->setShell(p_shell);
  chromfitEditor->setPlayer(this);
  addEditor("UAL::QT::ChromfitEditor", chromfitEditor);
  widgetStack->addWidget(chromfitEditor, 3 );

  UAL::QT::PropagatorViewer* propagatorViewer = new UAL::QT::PropagatorViewer(widgetStack);
  addEditor("UAL::QT::PropagatorViewer", propagatorViewer);
  widgetStack->addWidget(propagatorViewer, 4 );

  UAL::QT::TunefitEditor* tunefitEditor = new UAL::QT::TunefitEditor(widgetStack);
  tunefitEditor->setShell(p_shell);
  tunefitEditor->setPlayer(this);
  addEditor("UAL::QT::TunefitEditor", tunefitEditor);
  widgetStack->addWidget(tunefitEditor, 5 );

  listView->setSelected(beamEditorItem, true);
  widgetStack->raiseWidget(1);

std::cerr << __FILE__ << ": " << __LINE__ << "leave void UAL::USPAS::BasicPlayer::initPlayer\n";
}

void UAL::USPAS::BasicPlayer::showPage(QListViewItem* item)
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void UAL::USPAS::BasicPlayer::showPage(QListViewItem* item)\n";
  if(item->text(0) == "Beam") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Beam\n";
      widgetStack->raiseWidget(1);
std::cerr << __FILE__ << " " << __LINE__ << " leave Beam\n";
  }


  if(item->text(0) == "Parameters") {
      widgetStack->raiseWidget(2);
  }

  if(item->text(0) == "Chrom Fitting") {
      widgetStack->raiseWidget(3);
  }

  if(item->text(0) == "Propagators") {
      widgetStack->raiseWidget(4);
  }

  if(item->text(0) == "Tune Fitting") {
      widgetStack->raiseWidget(5);
  }

  /*
  if(item->text(0) == "Bunch 3D") {
    if(m_viewers.find("UAL::COIN3D::BunchViewer") != m_viewers.end()) return;
    UAL::COIN3D::BunchViewer* viewer = 
      new UAL::COIN3D::BunchViewer(this, &p_shell->getBunch());

    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::COUN3D::BunchViewer", viewer);
    viewer->show();
  }
  */

  if(item->text(0) == "Bunch X-PX") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Bunch X-PX\n";

    if(m_viewers.find("UAL::ROOT::BunchXPxViewer") != m_viewers.end()) return;
    UAL::ROOT::BunchXPxViewer* viewer = 
      new UAL::ROOT::BunchXPxViewer(this, &p_shell->getBunch());

    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::ROOT::BunchXPxViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave Bunch X-PX\n";
  }

  if(item->text(0) == "Bunch Y-PY") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Bunch Y-PY\n";

    if(m_viewers.find("UAL::ROOT::BunchYPyViewer") != m_viewers.end()) return;
    UAL::ROOT::BunchYPyViewer* viewer = 
      new UAL::ROOT::BunchYPyViewer(this, &p_shell->getBunch());

    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::BunchYPyViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave Bunch Y-PY\n";
  }

  if(item->text(0) == "Bunch CT-DE") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Bunch CT-DE\n";

    if(m_viewers.find("UAL::ROOT::BunchCtDeViewer") != m_viewers.end()) return;
    UAL::ROOT::BunchCtDeViewer* viewer = 
      new UAL::ROOT::BunchCtDeViewer(this,  &p_shell->getBunch());
    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::ROOT::BunchCtDeViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave Bunch CT-DE\n";
  }

  if(item->text(0) == "Mountain Range") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Mountain Range\n";

    if(m_viewers.find("UAL::MountainRangeViewer") != m_viewers.end()) return;
    UAL::ROOT::MountainRangeViewer* viewer = 
      new UAL::ROOT::MountainRangeViewer(this, &p_shell->getBunch());
    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::ROOT::MountainRangeViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave Mountain Range\n";
  }

  if(item->text(0) == "Twiss") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Twiss\n";
std::cerr << __LINE__ << "\n";
    if(m_viewers.find("UAL::ROOT::TwissViewer") != m_viewers.end()) return;

std::cerr << __LINE__ << "\n";
    std::vector<double> atVector;
    std::vector<PacTwissData> twissVector;

std::cerr << __LINE__ << "\n";
    UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance(); 
    optics.getTwiss(atVector, twissVector);

std::cerr << __LINE__ << "\n";
    UAL::ROOT::TwissViewer* viewer = new UAL::ROOT::TwissViewer(this, atVector, twissVector);
std::cerr << __LINE__ << "\n";
    addViewer("UAL::ROOT::TwissViewer", viewer);
std::cerr << __LINE__ << "\n";
    viewer->show();
std::cerr << __FILE__ << " " << __LINE__ << " leave Twiss\n";
  }

  if(item->text(0) == "Orbit") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Orbit\n";
std::cerr << __LINE__ << "\n";
    if(m_viewers.find("UAL::ROOT::OrbitViewer") != m_viewers.end()) return;

    std::vector<double> atVector;
    std::vector<PAC::Position> orbitVector;
std::cerr << __LINE__ << "\n";

    UAL::OpticsCalculator& optics = UAL::OpticsCalculator::getInstance(); 
    optics.getOrbit(atVector, orbitVector);

std::cerr << __LINE__ << "\n";
    UAL::ROOT::OrbitViewer* viewer = new UAL::ROOT::OrbitViewer(this, atVector, orbitVector);
std::cerr << __LINE__ << "\n";
    addViewer("UAL::ROOT::OrbitViewer", viewer);
std::cerr << __LINE__ << "\n";
    viewer->show();
std::cerr << __FILE__ << " " << __LINE__ << " leave Orbit\n";
  }

  if(item->text(0) == "BPM TBT") {
std::cerr << __FILE__ << " " << __LINE__ << " enter BPM TBT\n";

    if(m_viewers.find("UAL::ROOT::BunchTBTViewer") != m_viewers.end()) return;

    UAL::ROOT::BunchTBTViewer* viewer = 
      new UAL::ROOT::BunchTBTViewer(this);

    addViewer("UAL::ROOT::BunchTBTViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave BPM TBT\n";
  }

  if(item->text(0) == "FFT") {
std::cerr << __FILE__ << " " << __LINE__ << " enter FFT\n";

    if(m_viewers.find("UAL::ROOT::MonitorFFTViewer") != m_viewers.end()) return;

    UAL::ROOT::MonitorFFTViewer* viewer = new UAL::ROOT::MonitorFFTViewer(this);
    addViewer("UAL::ROOT::MonitorFFTViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave FFT\n";
  }

  if(item->text(0) == "PCA X") {
std::cerr << __FILE__ << " " << __LINE__ << " enter PCA X\n";

    if(m_viewers.find("UAL::ROOT::BpmSvdXViewer") != m_viewers.end()) return;

    UAL::ROOT::BpmSvd1DViewer* viewer = new UAL::ROOT::BpmSvd1DViewer(this, 0);
    addViewer("UAL::ROOT::BpmSvdXViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave PCA X\n";
  }

  if(item->text(0) == "Poincare X-PX") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Poincare X-PX\n";

    if(m_viewers.find("UAL::ROOT::PoincareXPxViewer") != m_viewers.end()) return;

    UAL::ROOT::PoincareXPxViewer* viewer = 
      new UAL::ROOT::PoincareXPxViewer(this);

    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::ROOT::PoincareXPxViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave Poincare X-PX\n";
  }

  if(item->text(0) == "Poincare Y-PY") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Poincare Y-PY\n";

    if(m_viewers.find("UAL::ROOT::PoincareYPyViewer") != m_viewers.end()) return;

    UAL::ROOT::PoincareYPyViewer* viewer = 
      new UAL::ROOT::PoincareYPyViewer(this);

    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::ROOT::PoincareYPyViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave Poincare Y-PY\n";
  }

  if(item->text(0) == "Poincare CT-DE") {
std::cerr << __FILE__ << " " << __LINE__ << " enter Poincare CT-DE\n";

    if(m_viewers.find("UAL::ROOT::PoincareCtDeViewer") != m_viewers.end()) return;

    UAL::ROOT::PoincareCtDeViewer* viewer = 
      new UAL::ROOT::PoincareCtDeViewer(this);

    viewer->setTurns(m_turns);
    viewer->setFprint(m_fprint);
    addViewer("UAL::ROOT::PoincareCtDeViewer", viewer);
    viewer->show();

std::cerr << __FILE__ << " " << __LINE__ << " leave Poincare CT-DE\n";
  }

std::cerr << __FILE__ << " " << __LINE__ << " leave void UAL::USPAS::BasicPlayer::showPage(QListViewItem* item)\n";
}
