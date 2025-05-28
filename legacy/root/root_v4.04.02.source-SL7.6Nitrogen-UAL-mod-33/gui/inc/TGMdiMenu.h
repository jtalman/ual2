// @(#)root/gui:$Name:  $:$Id: TGMdiMenu.h,v 1.3 2004/09/10 14:00:40 brun Exp $
// Author: Bertrand Bellenot   20/08/2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

/**************************************************************************

    This file is part of TGMdi, an extension to the xclass toolkit.
    Copyright (C) 1998-2002 by Harald Radke, Hector Peraza.

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This application is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

**************************************************************************/

#ifndef ROOT_TGMdiMenu
#define ROOT_TGMdiMenu

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGMdiMenu.                                                           //
//                                                                      //
// This file contains the TGMdiMenuBar class.                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif


class TGMdiMainFrame;
class TGMdiTitleIcon;
class TGMdiButtons;


class TGMdiMenuBar : public TGCompositeFrame {

friend class TGMdiMainFrame;

protected:
   TGCompositeFrame   *fLeft, *fRight;
   TGMenuBar          *fBar;
   TGLayoutHints      *fLHint, *fLeftHint;
   TGLayoutHints      *fRightHint, *fBarHint;

   void AddFrames(TGMdiTitleIcon *icon, TGMdiButtons *buttons);
   void RemoveFrames(TGMdiTitleIcon *icon, TGMdiButtons *buttons);

public:
   TGMdiMenuBar(const TGWindow *p, Int_t w = 1, Int_t h = 20);
   virtual ~TGMdiMenuBar();

   void AddPopup(TGHotString *s, TGPopupMenu *menu, TGLayoutHints *l);
   TGMenuBar *GetMenuBar() const { return fBar;}
   virtual void SavePrimitive(ofstream &out, Option_t *option);

   ClassDef(TGMdiMenuBar, 0)
};

#endif
