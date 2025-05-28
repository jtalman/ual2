// @(#)root/base:$Name:  $:$Id: TKeyMapFile.h,v 1.1.1.1 2000/05/16 17:00:39 rdm Exp $
// Author: Rene Brun   23/07/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TKeyMapFile
#define ROOT_TKeyMapFile


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TKeyMapFile                                                          //
//                                                                      //
// Utility class for browsing TMapFile objects.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

class TBrowser;
class TMapFile;

class TKeyMapFile : public TNamed {

private:

   TMapFile      *fMapFile;       //pointer to map file

public:
    TKeyMapFile();
    TKeyMapFile(const char *name, const char *classname, TMapFile *mapfile);
    virtual ~TKeyMapFile() {;}
    virtual void      Browse(TBrowser *b);

    ClassDef(TKeyMapFile,0)  //Utility class for browsing TMapFile objects.
};

#endif
