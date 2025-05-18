/* Configurations file for linuxx8664gcc */
#ifdef HAVE_CONFIG
#define ROOTPREFIX    "$(ROOTSYS)"
#define ROOTBINDIR    "$(ROOTSYS)/bin"
#define ROOTLIBDIR    "$(ROOTSYS)/lib"
#define ROOTINCDIR    "$(ROOTSYS)/include"
#define ROOTETCDIR    "$(ROOTSYS)/etc"
#define ROOTDATADIR   "$(ROOTSYS)"
#define ROOTDOCDIR    "$(ROOTSYS)"
#define ROOTMACRODIR  "$(ROOTSYS)/macros"
#define ROOTSRCDIR    "$(ROOTSYS)/src"
#define ROOTICONPATH  "$(ROOTSYS)/icons"
#define EXTRAICONPATH ""
#define TTFFONTDIR    "$(ROOTSYS)/fonts"
#define CINTINCDIR    "$(ROOTSYS)/cint"
#endif

#define HAS_SETRESUID  /**/

#define R__KRB5INIT   "/usr/bin/kinit"

#define R__CONFIGUREOPTIONS   "QTDIR=/usr/lib64/qt-3.3"
