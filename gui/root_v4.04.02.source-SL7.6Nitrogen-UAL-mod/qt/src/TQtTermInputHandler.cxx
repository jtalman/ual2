// @(#)root/qt:$Name:  $:$Id: TQtTermInputHandler.cxx,v 1.2 2005/03/01 07:24:01 brun Exp $
// Author: Valeri Fine   02/03/2005
/****************************************************************************
** $Id: TQtTermInputHandler.cxx,v 1.2 2005/03/01 07:24:01 brun Exp $
**
** Copyright (C) 2004 by Valeri Fine.  All rights reserved.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
*****************************************************************************/


#include "TQtTermInputHandler.h"
#include "TApplication.h"
#include <qsocketnotifier.h> 

//
// TQtTermInputHandler is to process the stdin ROOT events from within Qt event loop
// It would be nice to derive it from the TTermInputHandler class.
// Unfortunately this class is defined locally within TRint.cxx file scope
//

//______________________________________________________________________________
TQtTermInputHandler::TQtTermInputHandler (Int_t fd) : TFileHandler(fd, 1) 
{ 
   QSocketNotifier *sn;
   sn = new QSocketNotifier(fd, QSocketNotifier::Read, this,"QtTermInputHandler");
   QObject::connect( sn, SIGNAL(activated(int)),this, SLOT(Activate (int)) );
}
//______________________________________________________________________________
Bool_t TQtTermInputHandler::Notify()
{
   return gApplication->HandleTermInput();
}
//______________________________________________________________________________
void TQtTermInputHandler::Activate(int /*fd*/){
  //  Qt slot to activate ROOT TFileHandler from the Qt event loop
  //QSocketNotifier *sn = sender();
  //if (sn->Type() == QSocketNotifier::Read)
#if ROOT_VERSION_CODE > ROOT_VERSION(4,00,8)
     SetReadReady();
#else     
     fprintf(stderr,"Your ROOT version is too OLD !!!\n"
     "You have to update your ROOT version to access ROOT prompt"
     "from the Qt event loop. Sorry !!!\n");
#endif   
  //else if (sn->Type() == QSocketNotifier::Write)
  //   SetWriteReady();
}
