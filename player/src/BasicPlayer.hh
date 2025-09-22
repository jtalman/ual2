#ifndef UAL_USPAS_BASIC_PLAYER_HH
#define UAL_USPAS_BASIC_PLAYER_HH

#include <qmainwindow.h>

#include "UAL/QT/Player/BasicPlayer.hh"

class QAction;

namespace UAL
{
 namespace USPAS {

  class BasicPlayer : public UAL::QT::BasicPlayer {

   Q_OBJECT

  public:

    /** Constructor */
   BasicPlayer(); // QWidget *parent = 0, const char *name = 0);

    /** Set a pointer to shell */
    void setShell(UAL::QT::PlayerShell* shell);

    /** Showes page selected in the listView */
    virtual void showPage(QListViewItem* item);

  private:

    void initPlayer();
    
   };
  }
}

#endif
