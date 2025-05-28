// @(#)root/qt:$Name:  $:$Id: TQtRootApplication.h,v 1.2 2004/07/28 00:12:40 rdm Exp $
// Author: Valeri Fine   21/01/2002

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2002 by Valeri Fine.                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TQtRootApplication
#define ROOT_TQtRootApplication

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtRootApplication                                                   //
//                                                                      //
// This class create the Qt version of the ROOT                         //
// application environment.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TApplicationImp
#include "TApplicationImp.h"
#endif



class TQtRootApplication : public TApplicationImp {

private:
   char        *fDisplay;       // display server to conntect to

   TQtRootApplication() { fDisplay = 0; }
   void GetOptions(Int_t *argc, char **argv);

public:
   TQtRootApplication(const char *appClassName, Int_t *argc, char **argv);
   virtual ~TQtRootApplication();

   void    Show() { }
   void    Hide() { }
   void    Iconify() { }
   Bool_t  IsCmdThread() { return kTRUE; } // by default (for UNIX) ROOT is a single thread application
   void    Init() { }
   void    Open() { }
   void    Raise() { }
   void    Lower() { }

//   ClassDef(TQtRootApplication,0)  // ROOT native GUI application environment
};

#endif
