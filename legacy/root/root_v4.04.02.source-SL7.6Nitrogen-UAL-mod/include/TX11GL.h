// @(#)root/x11:$Name:  $:$Id: TX11GL.h,v 1.2 2004/08/16 10:00:45 brun Exp $
// Author: Timur Pocheptsov 09/08/2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TX11GL
#define ROOT_TX11GL


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TX11GL                                                               //
//                                                                      //
// The TX11GL is X11 implementation of TVirtualGLImp class.             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TVirtualGL
#include "TVirtualGL.h"
#endif

#if !defined(__CINT__)
#include <GL/glx.h>
#else
struct Display;
struct XVisualInfo;
#endif


class TX11GL : public TVirtualGLImp {

private:
   Display     *fDpy;
   XVisualInfo *fVisInfo;

public:
   TX11GL();

   Window_t CreateGLWindow(Window_t wind);
   ULong_t  CreateContext(Window_t wind);
   void     DeleteContext(ULong_t ctx);
   void     MakeCurrent(Window_t wind, ULong_t ctx);
   void     SwapBuffers(Window_t wind);

   ClassDef(TX11GL, 0);
};

#endif
