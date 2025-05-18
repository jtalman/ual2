// @(#)root/qt:$Name:  $:$Id: TQtApplication.h,v 1.4 2005/04/15 07:19:49 brun Exp $
// Author: Valeri Fine   21/01/2002

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2002 by Valeri Fine.                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TQtApplication
#define ROOT_TQtApplication

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtApplication -  Instantiate the Qt system within ROOT environment  //
//                                                                      //
// Instantiate the Qt package by creating Qapplication object if any   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TQtRConfig.h"
#include "Rtypes.h"

class TQtApplicationThread;

class TQtApplication { // : public TApplicationImp
  
private:
  friend class TQtApplicationThread;
  TQtApplicationThread  *fGUIThread;

  void    CreateGUIThread(int argc, char **argv);

  static void CreateQApplication(int argc, char ** argv, bool GUIenabled);

protected:
   static TQtApplication *fgQtApplication;

public:

   TQtApplication() {};
   TQtApplication(const char *appClassName, int argc, char **argv);
   virtual ~TQtApplication();
   static bool Terminate();

   static TQtApplication *GetQtApplication();
   static bool IsThisGuiThread();
   static Int_t QtVersion();
   ClassDef(TQtApplication,0) // Instantiate the Qt system within ROOT environment

};
#endif
