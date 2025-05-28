// @(#)root/qt:$Name:  $:$Id: TQMimeTypes.h,v 1.3 2005/03/01 07:24:01 brun Exp $
// Author: Valeri Fine   21/01/2003

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2002 by Valeri Fine.                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TQMimeTypes
#define ROOT_TQMimeTypes


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQMimeTypes and TQMime                                               //
//                                                                      //
// This class handles mime types, used by browsers to map file types    //
// to applications and icons. TQMime is internally used by TQMimeTypes. //
//                                                                      //
// This classes are based on TGMimeTypes and TGMime class from          //
// ROOT "gui"  package                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TString.h"
#ifndef __CINT__
#  include <qfiledialog.h>
#else
  class QFileIconProvider;
#endif

class TOrdCollection;
class TRegexp;

class QIconSet;
class TSystemFile;

class TQMime : public TObject {

friend class TQMimeTypes;

private:
   TString   fType;      // mime type
   TString   fPattern;   // filename pattern
   TString   fAction;    // associated action
   QIconSet  *fIcon;     // associated icon set
   TRegexp   *fReg;      // pattern regular expression

public:
  ~TQMime();
};


class TQMimeTypes : public TObject {
private:
      void operator=(const TQMimeTypes&) const {}
      void operator=(const TQMimeTypes&)  {}
      TQMimeTypes(const TQMimeTypes&) : TObject()  {}

protected:
   TString          fIconPath;   // the path to the icon directory
   TString          fFilename;   // file name of mime type file
   Bool_t           fChanged;    // true if file has changed
   TOrdCollection  *fList;       // list of mime types
   QFileIconProvider fDefaultProvider; // Default provider of the system icons;

   TQMime    *Find(const char *filename) const;
   const QIconSet *AddType(const TSystemFile *filename);

public:
   TQMimeTypes(const char *iconPath, const char *file);
   virtual ~TQMimeTypes();
   void   SaveMimes();
   Bool_t HasChanged() const { return fChanged; }
   void   AddType(const char *type, const char *pat, const char *icon, const char *sicon, const char *action);
   void   Print(Option_t *option="") const;
   Bool_t GetAction(const char *filename, char *action) const;
   Bool_t GetType(const char *filename, char *type) const;
   const  QIconSet *GetIcon(const char *filename) const;
   const  QIconSet *GetIcon(const TSystemFile *filename);


//MOC_SKIP_BEGIN 
   ClassDef(TQMimeTypes,0)  // Pool of mime type objects
//MOC_SKIP_END 
};

#endif
