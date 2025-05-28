// @(#)root/base:$Name:  $:$Id: TFile.h,v 1.36 2004/08/24 10:41:58 brun Exp $
// Author: Rene Brun   28/11/94

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TFile
#define ROOT_TFile


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFile                                                                //
//                                                                      //
// ROOT file.                                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TDirectory
#include "TDirectory.h"
#endif
#ifndef ROOT_TCache
#include "TCache.h"
#endif

class TFree;
class TArrayC;
class TArchiveFile;

class TFile : public TDirectory {

protected:
   Double_t      fSumBuffer;      //Sum of buffer sizes of objects written so far
   Double_t      fSum2Buffer;     //Sum of squares of buffer sizes of objects written so far
   Double_t      fBytesWrite;     //Number of bytes written to this file
   Double_t      fBytesRead;      //Number of bytes read from this file
   Long64_t      fBEGIN;          //First used byte in file
   Long64_t      fEND;            //Last used byte in file
   Long64_t      fSeekFree;       //Location on disk of free segments structure
   Long64_t      fSeekInfo;       //Location on disk of StreamerInfo record
   Int_t         fD;              //File descriptor
   Int_t         fVersion;        //File format version
   Int_t         fCompress;       //Compression level from 0(not compressed) to 9 (max compression)
   Int_t         fNbytesFree;     //Number of bytes for free segments structure
   Int_t         fNbytesInfo;     //Number of bytes for StreamerInfo record
   Int_t         fWritten;        //Number of objects written so far
   Int_t         fNProcessIDs;    //Number of TProcessID written to this file
   TString       fRealName;       //Effective real file name (not original url)
   TString       fOption;         //File options
   Char_t        fUnits;          //Number of bytes for file pointers
   TList        *fFree;           //Free segments linked list table
   TArrayC      *fClassIndex;     //!Index of TStreamerInfo classes written to this file
   TCache       *fCache;          //!Page cache used to reduce number of small I/O's
   TObjArray    *fProcessIDs;     //!Array of pointers to TProcessIDs
   Long64_t      fOffset;         //!Seek offset used by remote file classes
   TArchiveFile *fArchive;        //!Archive file from which we read this file
   Long64_t      fArchiveOffset;  //!Offset at which file starts in archive
   Bool_t        fIsArchive;      //!True if this is a pure archive file
   
   static Double_t fgBytesWrite;  //Number of bytes written by all TFile objects
   static Double_t fgBytesRead;   //Number of bytes read by all TFile objects

   void     Init(Bool_t create);
   Long64_t GetRelOffset() const { return fOffset - fArchiveOffset; }
   Int_t    ReadBufferViaCache(char *buf, Int_t len);
   Int_t    WriteBufferViaCache(const char *buf, Int_t len);

   // Interface to basic system I/O routines
   virtual Int_t    SysOpen(const char *pathname, Int_t flags, UInt_t mode);
   virtual Int_t    SysClose(Int_t fd);
   virtual Int_t    SysRead(Int_t fd, void *buf, Int_t len);
   virtual Int_t    SysWrite(Int_t fd, const void *buf, Int_t len);
   virtual Long64_t SysSeek(Int_t fd, Long64_t offset, Int_t whence);
   virtual Int_t    SysStat(Int_t fd, Long_t *id, Long64_t *size, Long_t *flags, Long_t *modtime);
   virtual Int_t    SysSync(Int_t fd);

private:
   TFile(const TFile &);            //Files cannot be copied
   void operator=(const TFile &);

public:
   // TFile status bits
   enum {
      kStartBigFile  = 2000000000,
      kRecovered     = BIT(10),
      kHasReferences = BIT(11),
      kDevNull       = BIT(12),
      kWriteError    = BIT(14) // BIT(13) is taken up by TObject
   };
   enum ERelativeTo { kBeg = 0, kCur = 1, kEnd = 2 };

   TFile();
   TFile(const char *fname, Option_t *option="", const char *ftitle="", Int_t compress=1);
   virtual ~TFile();
   virtual void      Close(Option_t *option=""); // *MENU*
   virtual void      Copy(TObject &) const { MayNotUse("Copy(TObject &)"); }
   virtual void      Delete(const char *namecycle="");
   virtual void      Draw(Option_t *option="");
   virtual void      DrawMap(const char *keys="*",Option_t *option=""); // *MENU*
   virtual void      FillBuffer(char *&buffer);
   virtual void      Flush();
   TArchiveFile     *GetArchive() const { return fArchive; }
   Int_t             GetBestBuffer() const;
   TArrayC          *GetClassIndex() const { return fClassIndex; }
   Int_t             GetCompressionLevel() const { return fCompress; }
   Float_t           GetCompressionFactor();
   virtual Long64_t  GetEND() const { return fEND; }
   virtual Int_t     GetErrno() const;
   virtual void      ResetErrno() const;
   Int_t             GetFd() const { return fD; }
   TObjArray        *GetListOfProcessIDs() const {return fProcessIDs;}
   TList            *GetListOfFree() const { return fFree; }
   virtual Int_t     GetNfree() const { return fFree->GetSize(); }
   virtual Int_t     GetNProcessIDs() const { return fNProcessIDs; }
   Option_t         *GetOption() const { return fOption.Data(); }
   Double_t          GetBytesRead() const { return fBytesRead; }
   Double_t          GetBytesWritten() const { return fBytesWrite; }
   Int_t             GetVersion() const { return fVersion; }
   Int_t             GetRecordHeader(char *buf, Long64_t first, Int_t maxbytes, Int_t &nbytes, Int_t &objlen, Int_t &keylen);
   virtual Int_t     GetNbytesInfo() const {return fNbytesInfo;}
   virtual Int_t     GetNbytesFree() const {return fNbytesFree;}
   virtual Long64_t  GetSeekFree() const {return fSeekFree;}
   virtual Long64_t  GetSeekInfo() const {return fSeekInfo;}
   virtual Long64_t  GetSize() const;
   virtual TList    *GetStreamerInfoList();
   virtual void      IncrementProcessIDs() {fNProcessIDs++;}
   virtual Bool_t    IsArchive() const { return fIsArchive; }
   virtual Bool_t    IsOpen() const;
   virtual void      ls(Option_t *option="") const;
   virtual void      MakeFree(Long64_t first, Long64_t last);
   virtual void      MakeProject(const char *dirname, const char *classes="*", Option_t *option="new"); // *MENU*
   virtual void      Map(); // *MENU*
   virtual void      Paint(Option_t *option="");
   virtual void      Print(Option_t *option="") const;
   virtual Bool_t    ReadBuffer(char *buf, Int_t len);
   virtual void      ReadFree();
   virtual void      ReadStreamerInfo();
   virtual Int_t     Recover();
   virtual Int_t     ReOpen(Option_t *mode);
   virtual void      Seek(Long64_t offset, ERelativeTo pos = kBeg);
   virtual void      SetCompressionLevel(Int_t level=1);
   virtual void      SetEND(Long64_t last) { fEND = last; }
   virtual void      SetOption(Option_t *option=">") { fOption = option; }
   virtual void      ShowStreamerInfo();
   virtual Int_t     Sizeof() const;
   void              SumBuffer(Int_t bufsize);
   virtual void      UseCache(Int_t maxCacheSize = 10, Int_t pageSize = TCache::kDfltPageSize);
   virtual Bool_t    WriteBuffer(const char *buf, Int_t len);
   virtual Int_t     Write(const char *name=0, Int_t opt=0, Int_t bufsiz=0);
   virtual Int_t     Write(const char *name=0, Int_t opt=0, Int_t bufsiz=0) const;
   virtual void      WriteFree();
   virtual void      WriteHeader();
   virtual void      WriteStreamerInfo();

   static TFile     *Open(const char *name, Option_t *option = "",
                          const char *ftitle = "", Int_t compress = 1,
                          Int_t netopt = 0);

   static Double_t   GetFileBytesRead();
   static Double_t   GetFileBytesWritten();

   static void       SetFileBytesRead(Double_t bytes=0);
   static void       SetFileBytesWritten(Double_t bytes=0);

   ClassDef(TFile,6)  //ROOT file
};

R__EXTERN TFile   *gFile;

#endif
