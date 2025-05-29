/******************************************************************************/
/*                                                                            */
/*                      X r d O d c M a n a g e r . c c                       */
/*                                                                            */
/* (c) 2003 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/
  
//          $Id: XrdOdcManager.cc,v 1.11 2005/01/16 04:33:08 abh Exp $

const char *XrdOdcManagerCVSID = "$Id: XrdOdcManager.cc,v 1.11 2005/01/16 04:33:08 abh Exp $";

#include "XrdOdc/XrdOdcManager.hh"
#include "XrdOdc/XrdOdcMsg.hh"
#include "XrdOdc/XrdOdcTrace.hh"

#include "XrdOuc/XrdOucError.hh"
#include "XrdOuc/XrdOucPthread.hh"
#include "XrdNet/XrdNetLink.hh"
#include "XrdNet/XrdNetWork.hh"
 
/******************************************************************************/
/*                               G l o b a l s                                */
/******************************************************************************/
  
extern XrdOucTrace OdcTrace;

/******************************************************************************/
/*                           C o n s t r u c t o r                            */
/******************************************************************************/
  
XrdOdcManager::XrdOdcManager(XrdOucError *erp, char *host, int port, 
                             int cw, int nr)
{
   char *dot;

// Set error object
//
   eDest   = erp;
   Host    = strdup(host);
   if ((dot = index(Host, '.')))
      {*dot = '\0'; HPfx = strdup(Host); *dot = '.';}
      else HPfx = strdup(Host);
   Port    = port;
   Link    = 0;
   Active  = 0;
   mytid   = 0;
   Silent  = 0;
   nrMax   = nr;
   Network = new XrdNetWork(eDest, 0);

// Compute dally value
//
   dally = cw / 2 - 1;
   if (dally < 3) dally = 3;
      else if (dally > 10) dally = 10;
}

/******************************************************************************/
/*                            D e s t r u c t o r                             */
/******************************************************************************/

XrdOdcManager::~XrdOdcManager()
{
  if (Network) delete Network;
  if (Link)    Link->Recycle();
  if (Host)    free(Host);
  if (HPfx)    free(HPfx);
  if (mytid)   XrdOucThread::Kill(mytid);
}
  
/******************************************************************************/
/*                                  S e n d                                   */
/******************************************************************************/
  
int XrdOdcManager::Send(char *msg, int mlen)
{
   int allok = 0;

// Determine message length
//
   if (!mlen) mlen = strlen(msg);

// Send the request
//
   if (Active)
      {myData.Lock();
       if (Link)
          if (!(allok = (Link && Link->Send(msg, mlen, 0) == 0)))
             {Active = 0;
              Link->Close(1);
             }
       myData.UnLock();
      }

// All done
//
   return allok;
}
  
int XrdOdcManager::Send(const struct iovec *iov, int iovcnt)
{
   int allok = 0;

// Send the request
//
   if (Active)
      {myData.Lock();
       if (Link)
          if (!(allok = (Link && Link->Send(iov, iovcnt, 0) == 0)))
             {Active = 0;
              Link->Close(1);
             }
       myData.UnLock();
      }

// All done
//
   return allok;
}

/******************************************************************************/
/*                                 S t a r t                                  */
/******************************************************************************/
  
void *XrdOdcManager::Start()
{
   char *msg;
   int   msgid, retc;

// First step is to connect to the manager
//
   do {Hookup();

       // Now simply start receiving messages on the stream
       //
       while((msg = Receive(msgid))) XrdOdcMsg::Reply(msgid, msg);

       // Tear down the connection
       //
       myData.Lock();
       if (Link)
          {if (((retc = Link->LastError()) == EBADF) && !Active) retc = 0;
           Link->Recycle(); 
           Link = 0;
          } else retc = 0;
       Active = 0;
       myData.UnLock();

       // Indicate the problem
       //
       if (retc) eDest->Emsg("Manager", retc, "receive msg from", Host);
          else   eDest->Emsg("Manager", "Disconnected from", Host);
       Sleep(dally);
      } while(1);

// We should never get here
//
   return (void *)0;
}

/******************************************************************************/
/*                               w h a t s U p                                */
/******************************************************************************/
  
void XrdOdcManager::whatsUp()
{

// The olb did not respond. Increase silent count and see if restart is needed
//
   myData.Lock();
   if (Active)
      {Silent++;
       if (Silent > nrMax)
          {Active = 0; Silent = 0;
           if (Link) Link->Close(1);
          }
      }
   myData.UnLock();
}

/******************************************************************************/
/*                       P r i v a t e   M e t h o d s                        */
/******************************************************************************/
/******************************************************************************/
/*                                H o o k u p                                 */
/******************************************************************************/
  
void XrdOdcManager::Hookup()
{
   XrdNetLink *lp;
   int tries = 12, opts = 0;

// Keep trying to connect to the manager
//
   do {while(!(lp = Network->Connect(Host, Port, opts)))
            {Sleep(dally);
             if (tries--) opts = XRDNET_NOEMSG;
                else     {opts = 0; tries = 12;}
            }
       if (lp->Send((char *)"login director\n") == 0) break;
       lp->Recycle();
      } while(1);

// All went well, finally
//
   myData.Lock();
   Link   = lp;
   Active = 1;
   Silent = 0;
   myData.UnLock();

// Tell the world
//
   eDest->Emsg("Manager", "Connected to", Host);
}

/******************************************************************************/
/*                                 S l e e p                                  */
/******************************************************************************/
  
void XrdOdcManager::Sleep(int slpsec)
{
   int retc;
   struct timespec lftp, rqtp = {slpsec, 0};

   while ((retc = nanosleep(&rqtp, &lftp)) < 0 && errno == EINTR)
         {rqtp.tv_sec  = lftp.tv_sec;
          rqtp.tv_nsec = lftp.tv_nsec;
         }

   if (retc < 0) eDest->Emsg("Manager", errno, "sleep");
}

/******************************************************************************/
/*                               R e c e i v e                                */
/******************************************************************************/
  
char *XrdOdcManager::Receive(int &msgid)
{
// This method is always run out of the manager's object thread. Other threads
// may call methods that initiate a link reset via a deferred close. We will
// notice that here because the file descriptor will be closed. This will
// cause us to return an error and precipitate a connection teardown.
//
   EPNAME("Receive")
   char *lp, *tp, *rest;
   if ((lp=Link->GetLine()) && *lp)
      {DEBUG("Server: Received from " <<Link->Name() <<": " <<lp);
       Silent = 0;  // Setting Silent w/o lock may cause rare connection bounce
       if ((tp=Link->GetToken(&rest)))
          {errno = 0;
           msgid  = (int)strtol(tp, (char **)NULL, 10);
           if (!errno) return rest;
           eDest->Emsg("Manager", "Invalid msgid from", Host);
          }
      }
   return 0;
}
