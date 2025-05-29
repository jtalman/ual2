#ifndef __ODC_FINDER__
#define __ODC_FINDER__
/******************************************************************************/
/*                                                                            */
/*                       X r d O d c F i n d e r . h h                        */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

//          $Id: XrdOdcFinder.hh,v 1.4 2005/01/03 07:48:58 abh Exp $

#include "XrdOdc/XrdOdcConfDefs.hh"
#include "XrdOuc/XrdOucPthread.hh"

class  XrdOdcManager;
class  XrdOucError;
class  XrdOucErrInfo;
class  XrdOucLogger;
class  XrdOucTList;
struct XrdOdcData;
struct XrdSfsPrep;

// The following return conventions are use by Forward(), Locate(), & Prepare()
//
// Return Val   Resp.errcode          Resp.errtext
// ---------    -------------------   --------
// -EREMOTE     port (0 for default)  Host name
// > 0          Wait time (= retval)  Reason for wait
// < 0          Error number          Error message
// = 0          Not applicable        Not applicable (see below)
//                                    Forward() -> Request forwarded
//                                    Locate()  -> Redirection does not apply
//                                    Prepare() -> Request submitted

class XrdOdcFinder
{
public:
virtual int    Configure(char *cfn) = 0;

virtual int    Forward(XrdOucErrInfo &Resp, const char *cmd, 
                       char *arg1=0, char *arg2=0) = 0;

virtual int    isRemote() {return myPersona == XrdOdcFinder::amRemote;}

virtual int    Locate(XrdOucErrInfo &Resp, const char *path, int flags) = 0;

virtual int    Prepare(XrdOucErrInfo &Resp, XrdSfsPrep &pargs) = 0;

virtual void   UpdateFD(int num) = 0;

        enum   Persona {amLocal, amProxy, amRemote, amTarget};

               XrdOdcFinder(XrdOucLogger *lp, Persona acting);
virtual       ~XrdOdcFinder() {}

protected:

Persona myPersona;
static char *OLBPath;
};

/******************************************************************************/
/*                          L o c a l   F i n d e r                           */
/******************************************************************************/
  
class XrdOdcFinderLCL : public XrdOdcFinder
{
public:
        int    Configure(char *cfn);

        int    Forward(XrdOucErrInfo &Resp, const char *cmd, 
                       char *arg1=0, char *arg2=0) {return 0;}

        int    Locate(XrdOucErrInfo &Resp, const char *path, int flags);

        int    Prepare(XrdOucErrInfo &Resp, XrdSfsPrep &pargs) {return 0;}

        void   UpdateFD(int numfd)
                       {myData.Lock(); myFDcnt += numfd; myData.UnLock();}

        void  *InspectLoad(void);

        int    isRedirector() {return nument;}

        void  *ReportLoad(void);

               XrdOdcFinderLCL(XrdOucLogger *lp, int lclport=0);
              ~XrdOdcFinderLCL();

private:

void               calcLoad(int &load, int &cputime);
int                LocbyFD();
int                LocbyLD();
int                LocbyRR();
void               probLoad(int probnum, int sent, int sport, pid_t spid);
int                StartMonitor(int Inspect, int tint);

XrdOucMutex        myData;
XrdOdcPselT        pselPort;
char              *myHost;
int                myPort;
int                myFDcnt;
int                repint;
struct XrdOdcData *repsad;
struct XrdOdcData *repdata;

int              (XrdOdcFinderLCL::*doLocate)();

struct    OdcData {         int   port;
                            pid_t pid;
                            int   ok;
                            int   numfd;
                            int   load;
                   unsigned int   tics;
                  } altserv[maxPORTS];
int       nument;
int       nextent;
};

/******************************************************************************/
/*                         R e m o t e   F i n d e r                          */
/******************************************************************************/

#define XRDODCMAXMAN 16
  
class XrdOdcFinderRMT : public XrdOdcFinder
{
public:
        int    Configure(char *cfn);

        int    Forward(XrdOucErrInfo &Resp, const char *cmd, 
                       char *arg1=0, char *arg2=0);

        int    Locate(XrdOucErrInfo &Resp, const char *path, int flags);

        int    Prepare(XrdOucErrInfo &Resp, XrdSfsPrep &pargs);

        void   UpdateFD(int numfd) {}

               XrdOdcFinderRMT(XrdOucLogger *lp, int isProxy=0);
              ~XrdOdcFinderRMT();

private:
int            Decode(char **resp);
XrdOdcManager *SelectManager(XrdOucErrInfo &Resp, char *path);
void           SelectManFail(XrdOucErrInfo &Resp);
int            StartManagers(XrdOucTList *);

XrdOdcManager *myManTable[XRDODCMAXMAN];
XrdOdcManager *myManagers;
int            myManCount;
XrdOucMutex    myData;
int            ConWait;
int            RepDelay;
int            RepNone;
int            RepWait;
unsigned char  SMode;
};

/******************************************************************************/
/*                         T a r g e t   F i n d e r                          */
/******************************************************************************/

class XrdOucStream;
  
class XrdOdcFinderTRG : public XrdOdcFinder
{
public:
        void   Added(const char *path);

        int    Configure(char *cfn);

        int    Forward(XrdOucErrInfo &Resp, const char *cmd,
                       char *arg1=0, char *arg2=0) {return 0;}

        int    Locate(XrdOucErrInfo &Resp, const char *path, int flags)
               {return 0;}

        int    Prepare(XrdOucErrInfo &Resp, XrdSfsPrep &pargs) {return 0;}

        void   Removed(const char *path);

        void  *Start();

        void   UpdateFD(int num) {}

               XrdOdcFinderTRG(XrdOucLogger *lp, int isprime, int port);
              ~XrdOdcFinderTRG();

private:

void  Hookup();

XrdOucStream  *OLBp;
XrdOucMutex    myData;
int            myPort;
char          *OLBPath;
char          *Login;
int            Primary;
int            Active;
};
#endif
