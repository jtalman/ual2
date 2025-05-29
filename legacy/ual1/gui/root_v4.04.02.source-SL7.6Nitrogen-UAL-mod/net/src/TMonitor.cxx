// @(#)root/net:$Name:  $:$Id: TMonitor.cxx,v 1.7 2004/12/15 17:48:03 rdm Exp $
// Author: Fons Rademakers   09/01/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMonitor                                                             //
//                                                                      //
// This class monitors activity on a number of network sockets.         //
// The actual monitoring is done by TSystem::DispatchOneEvent().        //
// Typical usage: create a TMonitor object. Register a number of        //
// TSocket objects and call TMonitor::Select(). Select() returns the    //
// socket object which has data waiting. TSocket objects can be added,  //
// removed, (temporary) enabled or disabled.                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMonitor.h"
#include "TSocket.h"
#include "TList.h"
#include "TSystem.h"
#include "TSysEvtHandler.h"
#include "TTimer.h"
#include "TError.h"


//---- Socket event handler ----------------------------------------------------
//
// This utility class is only used by TMonitor.
//

class TSocketHandler : public TFileHandler {
private:
   TMonitor  *fMonitor;   //monitor to which this handler belongs
   TSocket   *fSocket;    //socket being handled

public:
   TSocketHandler(TMonitor *m, TSocket *s, Int_t interest, Bool_t mainloop = kTRUE);
   Bool_t   Notify();
   Bool_t   ReadNotify() { return Notify(); }
   Bool_t   WriteNotify() { return Notify(); }
   TSocket *GetSocket() const { return fSocket; }
};

TSocketHandler::TSocketHandler(TMonitor *m, TSocket *s,
                               Int_t interest, Bool_t mainloop)
               : TFileHandler(s->GetDescriptor(), interest)
{
   fMonitor = m;
   fSocket  = s;

   if (mainloop)
      Add();
}

Bool_t TSocketHandler::Notify()
{
   fMonitor->SetReady(fSocket);
   return kTRUE;
}

//---- Timeout timer -----------------------------------------------------------
//
// This utility class is only used via TMonitor::Select(Int_t timeout)
//

class TTimeOutTimer : public TTimer {
private:
   TMonitor   *fMonitor;   //monitor to which this timer belongs

public:
   TTimeOutTimer(TMonitor *m, Long_t ms);
   Bool_t  Notify();
};

TTimeOutTimer::TTimeOutTimer(TMonitor *m, Long_t ms)
              : TTimer(ms, kTRUE)
{
   fMonitor = m;
   gSystem->AddTimer(this);
}

Bool_t TTimeOutTimer::Notify()
{
   fMonitor->SetReady((TSocket *)-1);
   Remove();       // one shot only
   return kTRUE;
}
//------------------------------------------------------------------------------


ClassImp(TMonitor)

//______________________________________________________________________________
TMonitor::TMonitor(Bool_t mainloop)
{
   // Create a monitor object. If mainloop is true the monitoring will be
   // done in the main event loop.

   Assert(gSystem);

   fActive   = new TList;
   fDeActive = new TList;
   fMainLoop = mainloop;
}

//______________________________________________________________________________
TMonitor::~TMonitor()
{
   // Cleanup the monitor object. Does not delete socket being monitored.

   fActive->Delete();
   SafeDelete(fActive);

   fDeActive->Delete();
   SafeDelete(fDeActive);
}

//______________________________________________________________________________
void TMonitor::Add(TSocket *sock, Int_t interest)
{
   // Add socket to the monitor's active list. If interest=kRead then we
   // want to monitor the socket for read readiness, if interest=kWrite
   // then we monitor the socket for write readiness, if interest=kRead|kWrite
   // then we monitor both read and write readiness.

   fActive->Add(new TSocketHandler(this, sock, interest, fMainLoop));
}
//______________________________________________________________________________
void TMonitor::SetInterest(TSocket *sock, Int_t interest)
{
   // Set interest mask for socket sock to interest. If the socket is not
   // in the active list move it or add it there.
   // If interest=kRead then we want to monitor the socket for read readiness,
   // if interest=kWrite then we monitor the socket for write readiness,
   // if interest=kRead|kWrite then we monitor both read and write readiness.

   TSocketHandler *s = 0;

   if (!interest)
      interest = kRead;

   // Check first the activated list ...
   TIter next(fActive);
   while ((s = (TSocketHandler *) next())) {
      if (sock == s->GetSocket()) {
         s->SetInterest(interest);
         return;
      }
   }

   // Check now the deactivated list ...
   TIter next1(fDeActive);
   while ((s = (TSocketHandler *) next1())) {
      if (sock == s->GetSocket()) {
         fDeActive->Remove(s);
         fActive->Add(s);
         s->SetInterest(interest);
         return;
      }
   }

   // The socket is not in our lists: just add it
   fActive->Add(new TSocketHandler(this, sock, interest, fMainLoop));
}

//______________________________________________________________________________
void TMonitor::Remove(TSocket *sock)
{
   // Remove a socket from the monitor.

   TIter next(fActive);
   TSocketHandler *s;

   while ((s = (TSocketHandler *) next())) {
      if (sock == s->GetSocket()) {
         fActive->Remove(s);
         delete s;
         return;
      }
   }

   TIter next1(fDeActive);

   while ((s = (TSocketHandler *) next1())) {
      if (sock == s->GetSocket()) {
         fDeActive->Remove(s);
         delete s;
         return;
      }
   }
}

//______________________________________________________________________________
void TMonitor::RemoveAll()
{
   // Remove all sockets from the monitor.

   fActive->Delete();
   fDeActive->Delete();
}

//______________________________________________________________________________
void TMonitor::Activate(TSocket *sock)
{
   // Activate a de-activated socket.

   TIter next(fDeActive);
   TSocketHandler *s;

   while ((s = (TSocketHandler *) next())) {
      if (sock == s->GetSocket()) {
         fDeActive->Remove(s);
         fActive->Add(s);
         s->Add();
         return;
      }
   }
}

//______________________________________________________________________________
void TMonitor::ActivateAll()
{
   // Activate all de-activated sockets.

   TIter next(fDeActive);
   TSocketHandler *s;

   while ((s = (TSocketHandler *) next())) {
      fActive->Add(s);
      s->Add();
   }
   fDeActive->Clear();
}

//______________________________________________________________________________
void TMonitor::DeActivate(TSocket *sock)
{
   // De-activate a socket.

   TIter next(fActive);
   TSocketHandler *s;

   while ((s = (TSocketHandler *) next())) {
      if (sock == s->GetSocket()) {
         fActive->Remove(s);
         fDeActive->Add(s);
         s->Remove();
         return;
      }
   }
}

//______________________________________________________________________________
void TMonitor::DeActivateAll()
{
   // De-activate all activated sockets.

   TIter next(fActive);
   TSocketHandler *s;

   while ((s = (TSocketHandler *) next())) {
      fDeActive->Add(s);
      s->Remove();
   }
   fActive->Clear();
}

//______________________________________________________________________________
TSocket *TMonitor::Select()
{
   // Return pointer to socket for which an event is waiting.
   // Return 0 in case of error.

   fReady = 0;

   while (!fReady)
      gSystem->InnerLoop();

   return fReady;
}

//______________________________________________________________________________
TSocket *TMonitor::Select(Long_t timeout)
{
   // Return pointer to socket for which an event is waiting.
   // Wait a maximum of timeout milliseconds.
   // If return is due to timeout it returns (TSocket *)-1.
   // Return 0 in case of any other error situation.

   fReady = 0;

   TTimeOutTimer t(this, timeout);

   while (!fReady)
      gSystem->InnerLoop();

   return fReady;
}

//______________________________________________________________________________
Int_t TMonitor::Select(TList *rdready, TList *wrready, Long_t timeout)
{
   // Return numbers of sockets that are ready for reading or writing.
   // Wait a maximum of timeout milliseconds.
   // Return 0 if timed-out. Return < 0 in case of error.
   // If rdready and/or wrready are not 0, the lists of sockets with
   // something to read and/or write are also returned.

   Int_t nr = -2;

   TSocketHandler *h = 0;
   Int_t ns = fActive->GetSize();
   if (ns == 1) {
      // Avoid additional loops inside
      h = (TSocketHandler *)fActive->First();
      nr = gSystem->Select((TFileHandler *)h, timeout);
   } else if (ns > 1) {
      nr = gSystem->Select(fActive, timeout);
   }

   if (nr > 0 && (rdready || wrready)) {
      // Clear the lists
      if (rdready)
         rdready->Clear();
      if (wrready)
         wrready->Clear();
      // Got a file descriptor
      if (!h) {
         TIter next(fActive);
         while ((h = (TSocketHandler *)next())) {
            if (rdready && h->IsReadReady())
               rdready->Add(h->GetSocket());
            if (wrready && h->IsWriteReady())
               wrready->Add(h->GetSocket());
         }
      } else {
         if (rdready && h->IsReadReady())
            rdready->Add(h->GetSocket());
         if (wrready && h->IsWriteReady())
            wrready->Add(h->GetSocket());
      }
   }

   return nr;
}

//______________________________________________________________________________
void TMonitor::SetReady(TSocket *sock)
{
   // Called by TSocketHandler::Notify() to signal which socket is ready
   // to be read or written. User should not call this routine. The ready
   // socket will be returned via the Select() user function.
   // Ready(TSocket *sock) signal is emitted.

   fReady = sock;
   Ready(fReady);
}

//______________________________________________________________________________
Int_t TMonitor::GetActive() const
{
   // Return number of sockets in the active list.

   return fActive->GetSize();
}

//______________________________________________________________________________
Int_t TMonitor::GetDeActive() const
{
   // Return number of sockets in the de-active list.

   return fDeActive->GetSize();
}

//______________________________________________________________________________
TList *TMonitor::GetListOfActives() const
{
   // Returns a list with all active sockets. This list must be deleted
   // by the user. DO NOT call Delete() on this list as it will delete
   // the sockets that are still being used by the monitor.

   TList *list = new TList;

   TIter next(fActive);

   while (TSocketHandler *h = (TSocketHandler*) next())
      list->Add(h->GetSocket());

   return list;
}

//______________________________________________________________________________
TList *TMonitor::GetListOfDeActives() const
{
   // Returns a list with all de-active sockets. This list must be deleted
   // by the user. DO NOT call Delete() on this list as it will delete
   // the sockets that are still being used by the monitor.

   TList *list = new TList;

   TIter next(fDeActive);

   while (TSocketHandler *h = (TSocketHandler*) next())
      list->Add(h->GetSocket());

   return list;
}

//______________________________________________________________________________
void TMonitor::Ready(TSocket *sock)
{
   // Emit signal when some socket is ready

   Emit("Ready(TSocket*)", (Long_t)sock);
}
