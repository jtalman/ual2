/********************************************************************************/
/*                     X T N e t A d m i n _ c i n t f . c c                    */
/*                                    2004                                      */
/*     Produced by Alvise Dorigo & Fabrizio Furano for INFN padova              */
/*                 A C wrapper for XTNetAdmin functionalities                   */
/********************************************************************************/
//
//   $Id: XrdClientAdmin_c.cc,v 1.15 2005/02/09 01:58:37 abh Exp $
//
// Author: Alvise Dorigo, Fabrizio Furano

#include "XrdClient/XrdClientAdmin.hh"
#include "XrdClient/XrdClientDebug.hh"
#include "XrdClient/XrdClientString.hh"
#include "XrdClient/XrdClientVector.hh"

#include <rpc/types.h>
#include <stdlib.h>

// We need a reasonable buffer to hold strings to be passed to/from Perl in some cases
char *sharedbuf;

void SharedBufRealloc(long size) {
   sharedbuf = (char *)realloc(sharedbuf, size);
   memset(sharedbuf, 0, size);
}
void SharedBufFree() {
   if (sharedbuf) free(sharedbuf);
   sharedbuf = 0;
}


// Useful to otkenize an input char * into a vector of strings
vecString *Tokenize(const char *str, char sep) {
   XrdClientString s(str);

   int it = 0;
   int itTokenEnd = 0;

   vecString *res = new vecString;

   XrdClientString sl;

   while( (res) && (it < s.GetSize()) )
      {
	 //Eat separators
	 while((it < s.GetSize()) && (s[it] == sep))
	    it++;

	 //Find next token
	 itTokenEnd = it;
	 if (itTokenEnd < s.GetSize())
	    do {
	       itTokenEnd++;
	    } while((itTokenEnd < s.GetSize()) && (s[itTokenEnd] != sep));

	 //Append token to result
	 if(it < itTokenEnd) {
	    sl = s.Substr(it, itTokenEnd);
	    res->Push_back(sl);
	 }

	 it = itTokenEnd;
      }

   return res;
}


void BuildBoolAnswer(vecBool &vb) {
   SharedBufRealloc(vb.GetSize());

   for (int i = 0; i < vb.GetSize(); i++) {
      sharedbuf[i] = '0';
      if (vb[i]) sharedbuf[i] = '1';
   }
   sharedbuf[vb.GetSize()] = '\0';

}



// In this version we support only one instance to be handled
// by this wrapper
XrdClientAdmin *adminst = NULL;

extern "C" {

   bool XrdInitialize(const char *url, const char *EnvValues) {

      // The parameters are supplied as a string of tokens
      // in the following form:
      //  parm_name parm_value\n
      //
      // The code tries to guess if the value is an integer or
      // a string
      vecString *env = Tokenize(EnvValues, '\n');

      for (int it = 0; it < env->GetSize(); it++) {
	 char tok1[256], tok2[256];
	 long v;

	 if (sscanf((*env)[it].c_str(), "%256s %ld", tok1, &v) == 2) {
	    // It's an integer value
	    EnvPutInt(tok1, v);
	    //cout << "Env: " << tok1 << " Val=" << EnvGetLong(tok1) << endl;
	 }
	 else {
	    sscanf((*env)[it].c_str(), "%256s %256s", tok1, tok2);
	    EnvPutString(tok1, tok2);
	    //cout << "Env: " << tok1 << " Val=" << EnvGetString(tok1) << endl;
	 }
      }

      delete env;

      DebugSetLevel(EnvGetLong(NAME_DEBUG));

      if (!adminst)
	 adminst = new XrdClientAdmin(url);
      
      adminst->Connect();
      
      sharedbuf = 0;
      return (adminst != NULL);
   }
   
   bool XrdTerminate() {
      delete adminst;
      adminst = NULL;

      SharedBufFree();

      return TRUE;
   }

   // The other functions, slightly modified from the originals
   //  in order to deal more easily with the perl syntax.
   // Hey these are wrappers!

   char *XrdSysStatX(const char *paths_list) {
      
      if (!adminst) return NULL;

      vecString *vs = Tokenize(paths_list, '\n');
      SharedBufRealloc(vs->GetSize()+1);

      adminst->SysStatX(paths_list, (kXR_char*)sharedbuf);

      // Let's turn the binary output to something readable
      for (int i = 0; i < vs->GetSize(); i++)
	 sharedbuf[i] += '0';

      delete vs;
      return(sharedbuf);
   }


   char *XrdExistFiles(const char *filepaths) {
      if (!adminst) return NULL;
      bool res = FALSE;
      vecBool vb;
  
      vecString *vs = Tokenize(filepaths, '\n');

      if (res = adminst->ExistFiles(*vs, vb)) {
	 BuildBoolAnswer(vb);
      }
      else SharedBufRealloc(16);
    
      delete vs;
      return(sharedbuf);

   }

   char *XrdExistDirs(const char *filepaths) {
      if (!adminst) return NULL;
      bool res = FALSE;
      vecBool vb;

      vecString *vs = Tokenize(filepaths, '\n');

      if (res = adminst->ExistDirs(*vs, vb)) {
	 BuildBoolAnswer(vb);
      }
      else SharedBufRealloc(16);

      delete vs;
      return(sharedbuf);
 
   }

   char *XrdIsFileOnline(const char *filepaths) {
      if (!adminst) return NULL;
      bool res = FALSE;
      vecBool vb;

      vecString *vs = Tokenize(filepaths, '\n');

      if (res = adminst->IsFileOnline(*vs, vb)) {
	 BuildBoolAnswer(vb);
      }
      else SharedBufRealloc(16);
    
      delete vs;
      return(sharedbuf);

   }




   bool XrdMv(const char *fileSrc, const char *fileDest) {
      if (!adminst) return adminst;

      return(adminst->Mv(fileSrc, fileDest));
   }


   bool XrdMkdir(const char *dir, int user, int group, int other) {
      if (!adminst) return adminst;

      return(adminst->Mkdir(dir, user, group, other));
   }


   bool XrdChmod(const char *file, int user, int group, int other) {
      if (!adminst) return adminst;

      return(adminst->Chmod(file, user, group, other));
   }


   bool XrdRm(const char *file) {
      if (!adminst) return adminst;

      return(adminst->Rm(file));
   }


   bool XrdRmdir(const char *path) {
      if (!adminst) return adminst;

      return(adminst->Rmdir(path));
   }


   bool XrdPrepare(const char *filepaths, unsigned char opts, unsigned char prty) {
      if (!adminst) return adminst;

      bool res;

      vecString *vs = Tokenize(filepaths, '\n');

      res = adminst->Prepare(*vs, opts, prty);

      delete vs;

      return(res);
   }

   char *XrdDirList(const char *dir) {
      vecString entries;
      XrdClientString lst;

      if (!adminst) return 0;

      if (!adminst->DirList(dir, entries)) return 0;

      joinStrings(lst, entries);

      SharedBufRealloc(lst.GetSize()+1);
      strcpy(sharedbuf, lst.c_str());

      return sharedbuf;
   }


   char *XrdGetChecksum(const char *path) {
      if (!adminst) return 0;

      char *chksum = 0;
      long chksumlen;

      // chksum now is a memory block allocated by the client itself
      // containing the 0-term response data
      if ( (chksumlen = adminst->GetChecksum((kXR_char *)path, (kXR_char **)&chksum)) ) {

	 // The data has to be copied to the sharedbuf
	 // to deal with perl parameter passing

	 SharedBufRealloc(chksumlen);
	 strncpy(sharedbuf, chksum, chksumlen);
	 sharedbuf[chksumlen] = 0;

         free(chksum);

	 return sharedbuf;
      }
      else return 0;

   }

   bool XrdStat(const char *fname, long *id, long long *size, long *flags, long *modtime) {
      if (!adminst) return false;
      
      return (adminst->Stat(fname, *id, *size, *flags, *modtime));

   }

} // extern c

