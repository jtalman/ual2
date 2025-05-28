// @(#)root/gui:$Name:  $:$Id: TGFSContainer.h,v 1.12 2004/10/15 17:44:20 rdm Exp $
// Author: Fons Rademakers   19/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGFSContainer
#define ROOT_TGFSContainer


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGFileIcon, TGFileEntry, TGFSContainer                               //
//                                                                      //
// Utility classes used by the file selection dialog (TGFileDialog).    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGIcon
#include "TGIcon.h"
#endif
#ifndef ROOT_TTimer
#include "TTimer.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif


//----- file sort mode
enum EFSSortMode {
   kSortByName,
   kSortByType,
   kSortBySize,
   kSortByDate
};


class TRegexp;
class TGPicture;
class TGFileContainer;
class TViewUpdateTimer;
class TGFileIcon;
class TGFileItem;


class TGFileItem : public TGLVEntry {

protected:
   const TGPicture  *fBlpic;        // big icon
   const TGPicture  *fSlpic;        // small icon
   const TGPicture  *fLcurrent;     // current icon
   Int_t             fType;         // file type
   Int_t             fUid, fGid;    // file uid and gid
   Bool_t            fIsLink;       // true if symbolic link
   Long64_t          fSize;         // file size

   virtual void DoRedraw();

public:
   TGFileItem(const TGWindow *p = 0,
              const TGPicture *bpic = 0, const TGPicture *blpic = 0,
              const TGPicture *spic = 0, const TGPicture *slpic = 0,
              TGString *name = 0, Int_t type = 0, Long64_t size = 1,
              Int_t uid = 0, Int_t gid = 0,
              EListViewMode viewMode = kLVList, UInt_t options = kVerticalFrame,
              Pixel_t back = GetWhitePixel());

   virtual void SetViewMode(EListViewMode viewMode);

   Bool_t  IsActive() const { return fActive; }
   Bool_t  IsSymLink() const { return fIsLink; }
   Int_t   GetType() const { return fType; }
   ULong_t GetSize() const { return fSize; }

   ClassDef(TGFileItem,0)   // Class representing file system object
};


class TGFileContainer : public TGLVContainer {

friend class TGFSFrameElement;

protected:
   EFSSortMode       fSortType;       // sorting mode of contents
   TRegexp          *fFilter;         // file filter
   TViewUpdateTimer *fRefresh;        // refresh timer
   ULong_t           fMtime;          // directory modification time
   TString           fDirectory;      // current directory
   const TGPicture  *fFolder_t;       // small folder icon
   const TGPicture  *fFolder_s;       // big folder icon
   const TGPicture  *fApp_t;          // small application icon
   const TGPicture  *fApp_s;          // big application icon
   const TGPicture  *fDoc_t;          // small document icon
   const TGPicture  *fDoc_s;          // big document icon
   const TGPicture  *fSlink_t;        // small symbolic link icon
   const TGPicture  *fSlink_s;        // big symbolic link icon

   void CreateFileList();

public:
   TGFileContainer(const TGWindow *p = 0, UInt_t w = 1, UInt_t h = 1,
                   UInt_t options = kSunkenFrame,
                   Pixel_t back = GetDefaultFrameBackground());
   TGFileContainer(TGCanvas *p, UInt_t options = kSunkenFrame,
                   Pixel_t back = GetDefaultFrameBackground());

   virtual ~TGFileContainer();

   virtual Bool_t HandleTimer(TTimer *t);
   void StopRefreshTimer();
   void StartRefreshTimer(ULong_t msec=1000);

   virtual TGFileItem *AddFile(const char *name);
   virtual void AddFrame(TGFrame *f, TGLayoutHints *l = 0);
   virtual void Sort(EFSSortMode sortType);
   virtual void SetFilter(const char *filter);
   virtual void ChangeDirectory(const char *path);
   virtual void DisplayDirectory();

   const char *GetDirectory() const { return fDirectory.Data(); }

   virtual void GetFilePictures(const TGPicture **pic, const TGPicture **lpic,
                                Int_t file_type, Bool_t is_link, const char *ext,
                                Bool_t small);

   virtual void SavePrimitive(ofstream &out, Option_t *option);

   ClassDef(TGFileContainer,0)  // Container containing file system objects
};

#endif
