#ifndef UAL_QT_PLAYER_SHELL_HH
#define UAL_QT_PLAYER_SHELL_HH

#include "UAL/UI/Shell.hh"
#include "TIBETAN/Propagator/RFCavityTracker.hh"

// #include "UAL/QT/Player/SectorTracker.hh"

namespace UAL
{
 namespace QT {

   /** Non-GUI user-oriented interface to  UAL modules */
  class PlayerShell : public UAL::Shell {

  public:

    /** Constructor */
    PlayerShell();

    /** Destructor */
    virtual ~PlayerShell();

    void setBunchSize(int b){m_bunchSize=b;}
    int getBunchSize(){return m_bunchSize;}

    void setTurns(int t){m_turns=t;}
    int getTurns(){return m_turns;}

    void setPlotUpdates(int p){m_plotUpdates=p;}
    int getPlotUpdates(){return m_plotUpdates;}

    void setNbins(int nb){m_nbins=nb;}
    int getNbins(){return m_nbins;}

    void setNsteps(int ns){m_nsteps=ns;}
    int getNsteps(){return m_nsteps;}

    /** Set beam attributes */
    bool setBeamAttributes(const UAL::Arguments& args);

    /** Set RF */
    bool setRF(const UAL::Arguments& args);

    /** Returns a RF cavity */
    TIBETAN::RFCavityTracker& getRF() {return m_rfTracker;}

    /** Returns lomgitudinal parameters of a bunch 
     * (temporary methods)*/
    double getCtHalfWidth() { return m_bunchGenerator.ctHalfWidth; }
    double getDeHalfWidth() { return m_bunchGenerator.deHalfWidth; }

    /** Sets longitudinal parameters 
     * (temporary method) 
     */
    void setLongEmittance(double ctHalfWidth, double deHalfWidth);

    // void getTwiss(PacTwissData& twiss);

    /** Sets the application parameters */
    virtual bool initRun(const UAL::Arguments& args = UAL::Args());

    /** Run application */
    virtual bool run(int turn);

  protected:

    /** Chnage a phase of the RF cavity */
    void changeLag(UAL::OpticsCalculator& optics);

  protected:

    /**  RF parameters */
    double m_V, m_harmon, m_lag;
    /** RF tracker */
    TIBETAN::RFCavityTracker m_rfTracker;

  protected:

    /** SectorTracker */
    // UAL_RHIC::SectorTracker m_sectorTracker;
    //
 
  private:
    int m_turns;
    int m_bunchSize;
    int m_plotUpdates;

    int m_nbins;
    int m_nsteps;

  };
 }
}

#endif
