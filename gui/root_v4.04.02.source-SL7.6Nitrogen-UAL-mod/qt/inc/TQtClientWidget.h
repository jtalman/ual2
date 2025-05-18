// @(#)root/qt:$Name:  $:$Id: TQtClientWidget.h,v 1.4 2005/04/15 07:19:50 brun Exp $
/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2002 by Valeri Fine.                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TQtClientWidget
#define ROOT_TQtClientWidget

#ifndef __CINT__
#  include <qframe.h>
#  include <qcursor.h>
#else
  class QFrame;
  class QCursor;
#endif
#include "GuiTypes.h"

//
// TQtClientWidget  is a QFrame implemantation backing  ROOT TGWindow objects
// It tries to mimic the X11 Widget behaviour, that kind the ROOT Gui relies on heavily.
//
class QAccel;
class QCursor;
class QCloseEvent;
class TQtClientGuard;
class TQtWidget;

class TQtClientWidget: public QFrame {
#ifndef __CINT__
       Q_OBJECT
#endif
private:
         void  operator=(const TQtClientWidget&)  {}
         void  operator=(const TQtClientWidget&) const {}
         TQtClientWidget(const TQtClientWidget&) : QFrame() {}

protected:

       UInt_t fGrabButtonMask;
       UInt_t fGrabPointerMask;
       UInt_t fEventMask;
       UInt_t fSelectEventMask;
       EMouseButton fButton;
       QAccel *fGrabbedKey;
       Bool_t fPointerOwner;
       QCursor *fPointerCursor;
       bool   fIsClosing;
       bool   fDeleteNotify;
       TQtClientGuard  *fGuard;
       TQtWidget       *fCanvasWidget;
       friend class TQtClientGuard;
       friend class TGQt;

       TQtClientWidget(TQtClientGuard *guard, QWidget* parent=0, const char* name=0, WFlags f=0 ):
          QFrame(parent,name,f)
         ,fGrabButtonMask(kAnyModifier),fGrabPointerMask(kAnyModifier)
         ,fEventMask(kNoEventMask),fSelectEventMask(0) // ,fAttributeEventMask(0)
         ,fButton(kAnyButton),fGrabbedKey(0),fPointerOwner(kFALSE),fPointerCursor(0),fIsClosing(false)
         ,fDeleteNotify(false),fGuard(guard), fCanvasWidget(0)
          { }
       void SetCanvasWidget(TQtWidget *widget);
public:
    enum {kRemove = -1, kTestKey = 0, kInsert = 1};
    virtual ~TQtClientWidget();
    virtual void closeEvent(QCloseEvent *ev);
    bool   DeleteNotify();
    TQtWidget *GetCanvasWidget() const;
    void   GrabEvent(Event_t &ev,bool own=TRUE);
    QAccel *HasAccel() const ;
    bool   IsClosing();
    bool   IsGrabbed       (Event_t &ev);
    TQtClientWidget *IsKeyGrabbed(const Event_t &ev);
    bool   IsPointerGrabbed(Event_t &ev);
    UInt_t IsEventSelected (UInt_t evmask);
    bool   IsGrabOwner()   { return fPointerOwner;}
    void   SetAttributeEventMask(UInt_t evmask);
    void   SetButtonMask   (UInt_t modifier=kAnyModifier,EMouseButton button=kAnyButton);
    void   SetClosing(bool flag=kTRUE);
    void   SetCursor();
    void   SetCursor(Cursor_t cursor);
    void   SetDeleteNotify(bool flag=true);
    void   SetEventMask    (UInt_t evmask);
    void   SelectInput     (UInt_t evmask);
    void   SetPointerMask  (UInt_t modifier, Cursor_t cursor, Bool_t owner_events);
    Bool_t SetKeyMask      (Int_t keycode = 0, UInt_t modifier=kAnyModifier,int insert=kInsert);
    void   UnSetButtonMask (bool dtor=false);
    void   UnSetPointerMask(bool dtor=false);
    void   UnSetKeyMask(Int_t keycode = 0, UInt_t modifier=kAnyModifier);
    UInt_t ButtonMask  () const;
    EMouseButton Button() const ;
    UInt_t PointerMask () const;
protected slots:
      void Disconnect();
public slots:
    virtual void Accelerate(int id);
    virtual void polish();
//MOC_SKIP_BEGIN
    ClassDef(TQtClientWidget,0) // QFrame implementation backing  ROOT TGWindow objects
//MOC_SKIP_END
};

//______________________________________________________________________________
inline  bool TQtClientWidget::IsPointerGrabbed(Event_t &ev)
{
   //
   //    grab     ( -owner_event && id == current id  -)         *
   //  o------>o---------------------------------->o--------------->o-->
   //          |             *                     |  grab pointer  |
   //          |                                   |                |
   //          |           evmask                  |                |
   //          |---------------------------------->|                |
   //          |             *                                      |
   //          |                      *                             |
   //          |--------------------------------------------------->|
   //                             discard event
   //

   bool isGrabbed = ev.fState & fGrabPointerMask;
   //fprintf(stderr," TQtClientWidget::IsPointerGrabbed %p grabbed=%d\n", this, isGrabbed);
   //fprintf(stderr,"                                 wid= %p mask=0x%x\n", (TQtClientWidget *)TGQt::wid(ev.fWindow), fGrabPointerMask);
   //fprintf(stderr,"                                 fPointerOwner= %d ev.fState=0x%x\n", fPointerOwner, ev.fState);

   return isGrabbed;
}
//______________________________________________________________________________
inline bool TQtClientWidget::DeleteNotify(){return fDeleteNotify; }

//______________________________________________________________________________
inline TQtWidget *TQtClientWidget::GetCanvasWidget() const
{ return fCanvasWidget;}
 //______________________________________________________________________________
inline QAccel *TQtClientWidget::HasAccel() const 
{  return fGrabbedKey; }

//______________________________________________________________________________
inline bool  TQtClientWidget::IsClosing(){ return fIsClosing; }

//______________________________________________________________________________
inline UInt_t TQtClientWidget::IsEventSelected (UInt_t evmask)
{
   //if (evmask & (kButtonPressMask | kButtonMotionMask) )
   //   fprintf(stderr,"TQtClientWidget::IsEventSelected event %x, mask %x. match %x\n"
   //   , evmask, fSelectEventtMask, evmask & (kButtonPressMask | kButtonMotionMask));
   return  (evmask & fSelectEventMask);
}

//______________________________________________________________________________
inline void TQtClientWidget::SetCursor()
{ // Set this widget pre-defined cursor
   if (fPointerCursor) setCursor(*fPointerCursor);
}
//______________________________________________________________________________
inline void TQtClientWidget::SetCursor(Cursor_t cursor)
{
   // Change the pre-define curos shape and set it
   fPointerCursor = (QCursor *)cursor;
   SetCursor();
}

//______________________________________________________________________________
inline void  TQtClientWidget::SetClosing(bool flag) { fIsClosing = flag;}
//______________________________________________________________________________
inline void  TQtClientWidget::SetDeleteNotify(bool flag){fDeleteNotify = flag;}

//______________________________________________________________________________
inline void TQtClientWidget::SetAttributeEventMask(UInt_t evmask) { SelectInput (evmask);}

//______________________________________________________________________________
inline void TQtClientWidget::SetEventMask(UInt_t evmask) { fEventMask = evmask;}

//______________________________________________________________________________
inline void TQtClientWidget::SelectInput (UInt_t evmask) {fSelectEventMask=evmask;}

//______________________________________________________________________________
inline EMouseButton TQtClientWidget::Button()const { return fButton;           }

//______________________________________________________________________________
inline UInt_t TQtClientWidget::ButtonMask()  const { return fGrabButtonMask;   }

//______________________________________________________________________________
inline UInt_t TQtClientWidget::PointerMask() const { return fGrabPointerMask;  }

#endif

