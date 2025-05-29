// @(#)root/qt:$Name:  $:$Id: TQtWidget.cxx,v 1.16 2005/04/06 09:32:11 brun Exp $
// Author: Valeri Fine   23/01/2003

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2003 by Valeri Fine.                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

// Definition of TQtWidget class
// "double-buffere widget

#include <iostream>
#include <qapplication.h>
#include "TQtWidget.h"
#include "TQtTimer.h"

#include "TROOT.h"
#include "TRint.h"
#include "TSystem.h"
#include "Getline.h"
#include "TGQt.h"
#include "TCanvas.h"
#include "Buttons.h"
#include <qevent.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qfileinfo.h>

#ifdef R__QTWIN32
// #include "Windows4Root.h"
#include "TWinNTSystem.h"
#include "Win32Constants.h"
#endif

ClassImp(TQtWidget)

////////////////////////////////////////////////////////////////////////////////
//
//  TQtWidget is a QWidget with the QPixmap double buffer
//  It is designed to back the ROOT TCanvasImp class interface  and it can be used
//  as a regular Qt Widget to create Qt-based GUI with the embedded TCanvas objects
//
//           This widget can be used as a Qt "custom widget" 
//         to build a custom GUI interfaces with  Qt Designer
//
// class emits Qt signals and has a Qt public slots
//
//  Public slots:  (Qt)
//
//   virtual void cd();  // make the associated TCanvas the current one (shortcut to TCanvas::cd()) 
//   virtual void cd(int subpadnumber); // as above - shortcut to Canvas::cd(int subpadnumber)
//   void Disconnect(); // disconnect the Qidget from the ROOT TCanvas (used in the class dtor)
//   void Refresh();    // forece the aasociated TCanvas::Update to be called
//   virtual bool Save(const QString &fileName) const;  // Save the widget image with some ppixmap file 
//   virtual bool Save(const char    *fileName) const;
//   virtual bool Save(const QString &fileName,const char *format,int quality=60) const;
//   virtual bool Save(const char    *fileName,const char *format,int quality=60) const;
//
//  signals        (Qt)
//
//    CanvasPainted();  // Signal the TCanvas has been painted onto the screen
//    Saved(bool ok);   // Signal the TCanvas has been saved into the file
//    RootEventProcessed(TObject *selected, unsigned int event, TCanvas *c);
//                     // Signal the Qt mouse/keybord events have been process by ROOT
// 
//
//  For example to create the custom responce to the mouse crossing TCanvas
//  connect the RootEventProsecced signal witrh your qt slot:
//
// connect(tQtWidget,SIGNAL(RootEventProcessed(TObject *, unsigned int, TCanvas *))
//          ,this,SLOT(CanvasEvent(TObject *, unsigned int, TCanvas *)));
//  . . . 
//
//void qtrootexample1::CanvasEvent(TObject *obj, unsigned int event, TCanvas *)
//{
//  TQtWidget *tipped = (TQtWidget *)sender();
//  const char *objectInfo = 
//        obj->GetObjectInfo(tipped->GetEventX(),tipped->GetEventY());
//  QString tipText ="You have ";
//  if  (tipped == tQtWidget1)
//     tipText +="clicked";
//  else
//     tipText +="passed";
//  tipText += " the object <";
//  tipText += obj->GetName();
//  tipText += "> of class "; 
//  tipText += obj->ClassName();
//  tipText += " : ";
//  tipText += objectInfo;
//  
//  QToolTip::remove(tipped);
//  QToolTip::add(tipped,tipText);
// }
//
////////////////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________
TCanvas  *TQtWidget::Canvas()
{
#ifdef R__QTGUITHREAD
   if (qApp->tryLock() ) {
      TCanvas  *c = 0;
      if (fCanvas)
         c = gROOT->IsLineProcessing() ? 0 : GetCanvas();
      qApp->unlock();
      return c;
   }
   return 0;
#else
   return GetCanvas();
#endif
};

//_____________________________________________________________________________
TQtWidget::TQtWidget(QWidget* parent, const char* name, WFlags f,bool embedded):QWidget(parent,name,f)
          ,fBits(0),fCanvas(0),fPixmapID(this),fPaint(TRUE),fSizeChanged(FALSE)
          ,fDoubleBufferOn(FALSE),fEmbedded(embedded),fWrapper(0),fSaveFormat("PNG")
{
std::cerr << __FILE__ << " " << __LINE__ << " enter TQtWidget::TQtWidget(QWidget* parent, const char* name, WFlags\n";
  setFocusPolicy(QWidget::WheelFocus);
  setWFlags(getWFlags () | Qt::WRepaintNoErase | Qt:: WResizeNoErase );
  setBackgroundMode(Qt::NoBackground);
std::cerr << __LINE__ << "\n";
  if (fEmbedded) {
std::cerr << __LINE__ << "\n";
    static int argc;
    if (!gApplication) {
std::cerr << __LINE__ << "\n";
        argc = qApp->argc();
std::cerr << __LINE__ << "\n";
        TRint *rint = new TRint("Rint", &argc ,qApp->argv());
std::cerr << __LINE__ << "\n";
        // To mimic what TRint::Run(kTRUE) does.
        Getlinem(kInit, rint->GetPrompt());
std::cerr << __LINE__ << "\n";
        TQtTimer::Create()->start(0,TRUE);
std::cerr << __LINE__ << "\n";
    }
std::cerr << __LINE__ << "\n";
    Bool_t batch = gROOT->IsBatch();
std::cerr << __LINE__ << "\n";
    if (!batch) gROOT->SetBatch(kTRUE); // to avoid the recursion within TCanvas ctor
std::cerr << __LINE__ << "\n";
//  TGQt::RegisterWid(this);
std::cerr << __LINE__ << "\n";
//  fCanvas = new TCanvas(name, 4, 4, TGQt::RegisterWid(this));
    fCanvas = new TCanvas(name, 4, 4, TGQt::RegisterWid(this));
std::cerr << __LINE__ << "\n";
    // fprintf(stderr,"TQtWidget::TQtWidget fEditable %d\n", fCanvas->IsEditable());
    gROOT->SetBatch(batch);
std::cerr << __LINE__ << "\n";
    connect(this, SIGNAL(destroyed()),SLOT(Disconnect()));
std::cerr << __LINE__ << "\n";
  }
std::cerr << __LINE__ << "\n";
  fSizeHint = QWidget::sizeHint();
  setSizePolicy (QSizePolicy::Expanding ,QSizePolicy::Expanding );
#ifdef R__QTWIN32
   // Set the application icon for all ROOT widgets
   static HICON rootIcon = 0;
std::cerr << __LINE__ << "\n";
   if (!rootIcon) {
std::cerr << __LINE__ << "\n";
      HICON hIcon = ((TWinNTSystem *)gSystem)->GetSmallIcon(kMainROOTIcon);
      if (!hIcon) hIcon = LoadIcon(NULL, IDI_APPLICATION);
      rootIcon = hIcon;
      SetClassLong(winId(),        // handle to window
                   GCL_HICON,      // index of value to change
                   LONG(rootIcon)  // new value
      );
    }
#endif
std::cerr << __FILE__ << " " << __LINE__ << " leave TQtWidget::TQtWidget(QWidget* parent, const char* name, WFlags\n";
}
//______________________________________________________________________________
TQtWidget::~TQtWidget()
{
  qApp->lock();
  TCanvas *savCanvas = fCanvas; fCanvas = 0;
  qApp->unlock();

  TGQt::UnRegisterWid(this);
  if (fEmbedded) {
      TGQt::UnRegisterWid(this); // to block the double deleting from the 
                                 // TQt::Delete
      if (savCanvas  && savCanvas->GetCanvasID()) 
             delete savCanvas;
   }
}

//_____________________________________________________________________________
void TQtWidget::adjustSize()
{
  // Adjusts the size of the widget to fit the contents.
  // Adjust the size of the double buffer to the
  // current Widget size
  QWidget::adjustSize ();
  AdjustBufferSize();
  update();
}
//_____________________________________________________________________________
void TQtWidget::erase ()
{
  // Erases the specified area (x, y, w, h) in the widget
  // without generating a paint event.
  QWidget::erase();
  fPixmapID.fill();
}
//_____________________________________________________________________________
void TQtWidget::cd()
{
 // [slot] to make this embedded canvas the current one
  cd(0);
}
 //______________________________________________________________________________
void TQtWidget::cd(int subpadnumber)
{
 // [slot] to make this embedded canvas / pad the current one
  qApp->lock();
  TCanvas *c = fCanvas;
  if (c) c->cd(subpadnumber);
  qApp->unlock();
}
//______________________________________________________________________________
void TQtWidget::Disconnect()
{
   // [slot] Disconnect the Qt widget from CTanvas object before deleting
   // to avoid the dead lock
  qApp->lock();
 // one has to set CanvasID = 0 to disconnect things properly.
  TCanvas *c = fCanvas; fCanvas = 0; delete c;
  qApp->unlock();
}
//_____________________________________________________________________________
void TQtWidget::Refresh()
{
   // [slot]  to allow Qt signal refreshing TOOT TCanvas if needed

   TCanvas *c = Canvas();
   if (!fPixmapID.paintingActive())  AdjustBufferSize();
   if (c) {
      c->Resize();
      c->Update();
   }
}

//_____________________________________________________________________________
void TQtWidget::resize (int w, int h)
{
   // resize the widget and its double buffer
   // fprintf(stderr,"TQtWidget::resize (int w=%d, int h=%d)\n",w,h);
   QWidget::resize(w,h);
   AdjustBufferSize();
   // fPixmapID.fill();
}

//_____________________________________________________________________________
void TQtWidget::customEvent(QCustomEvent *e)
{
   // The custom responce to the special WIN32 events
   // These events are not present with X11 systems
   switch (e->type() - QEvent::User) {
   case kEXITSIZEMOVE:
      { // WM_EXITSIZEMOVE
         fPaint = TRUE;
         setUpdatesEnabled( TRUE );
         exitSizeEvent();
         break;
      }
   case kENTERSIZEMOVE:
      {
         //  WM_ENTERSIZEMOVE
         fSizeChanged=FALSE;
         fPaint = FALSE;
         setUpdatesEnabled( FALSE );
      }
   case kFORCESIZE:
   default:
      {
         // Force resize
         fPaint       = TRUE;
         fSizeChanged = TRUE;
         setUpdatesEnabled( TRUE );
         exitSizeEvent();
         break;
      }
   };
}
//_____________________________________________________________________________
void TQtWidget::focusInEvent ( QFocusEvent *e )
{
   // The custom responce to the Qt QFocusEvent "in"
   // this imposes an extra protection to avoid TObject interaction with
   // mouse event accidently
   if (!fWrapper && e->gotFocus()) {
      setMouseTracking(TRUE);
   }
   if ( autoMask() ) updateMask();
}
//_____________________________________________________________________________
void TQtWidget::focusOutEvent ( QFocusEvent *e )
{
   // The custom responce to the Qt QFocusEvent "out"
   // this imposes an extra protection to avoid TObject interaction with
   // mouse event accidently
   if (!fWrapper && e->lostFocus()) {
      setMouseTracking(FALSE);
   }
   if ( autoMask() ) updateMask();
}

//_____________________________________________________________________________
void TQtWidget::mousePressEvent (QMouseEvent *e)
{
   // Map the Qt mouse press button event to the ROOT TCanvas events
   // Mouse events occur when a mouse button is pressed or released inside
   // a widget or when the mouse cursor is moved.

   //    kButton1Down   =  1, kButton2Down   =  2, kButton3Down   =  3,
   EEventType rootButton = kNoEvent;
   TCanvas *c = Canvas();
   if (c){
      switch (e->button ())
      {
      case Qt::LeftButton:  rootButton = kButton1Down; break;
      case Qt::RightButton: rootButton = kButton3Down; break;
      case Qt::MidButton:   rootButton = kButton2Down; break;
      default: break;
      };
      if (rootButton != kNoEvent) {
         c->HandleInput(rootButton, e->x(), e->y());
         e->accept(); EmitSignal(kMousePressEvent); return;
      }
   }
   QWidget::mousePressEvent(e);
}

//_____________________________________________________________________________
void TQtWidget::mouseMoveEvent (QMouseEvent * e)
{
   //  Map the Qt mouse move pointer event to the ROOT TCanvas events
   //  kMouseMotion   = 51,
   //  kButton1Motion = 21, kButton2Motion = 22, kButton3Motion = 23, kKeyPress = 24
   EEventType rootButton = kMouseMotion;
   TCanvas *c = Canvas();
   if (c){
      if (e->state() & LeftButton) { rootButton = kButton1Motion; }
      c->HandleInput(rootButton, e->x(), e->y());
      e->accept();  EmitSignal(kMouseMoveEvent); return;
   }
   QWidget::mouseMoveEvent(e);
}

//_____________________________________________________________________________
void TQtWidget::mouseReleaseEvent(QMouseEvent * e)
{
   //  Map the Qt mouse button release event to the ROOT TCanvas events
   //   kButton1Up     = 11, kButton2Up     = 12, kButton3Up     = 13
   EEventType rootButton = kNoEvent;
   TCanvas *c = Canvas();
   if (c){
      switch (e->button())
      {
      case Qt::LeftButton:  rootButton = kButton1Up; break;
      case Qt::RightButton: rootButton = kButton3Up; break;
      case Qt::MidButton:   rootButton = kButton2Up; break;
      default: break;
      };
      if (rootButton != kNoEvent) {
         c->HandleInput(rootButton, e->x(), e->y());
         gPad->Modified();
         e->accept(); EmitSignal(kMouseReleaseEvent);return;
      }
   }
   QWidget::mouseReleaseEvent(e);
}

//_____________________________________________________________________________
void TQtWidget::mouseDoubleClickEvent(QMouseEvent * e)
{
std::cerr << __FILE__ << " " << __LINE__ << " enter void TQtWidget::mouseDoubleClickEvent(QMouseEvent * e)\n";
   //  Map the Qt mouse double click button event to the ROOT TCanvas events
   //  kButton1Double = 61, kButton2Double = 62, kButton3Double = 63
   EEventType rootButton = kNoEvent;
   TCanvas *c = Canvas();
   if (c){
      switch (e->button())
      {
      case Qt::LeftButton:  rootButton = kButton1Double; break;
      case Qt::RightButton: rootButton = kButton3Double; break;
      case Qt::MidButton:   rootButton = kButton2Double; break;
      default: break;
      };
      if (rootButton != kNoEvent) {
         c->HandleInput(rootButton, e->x(), e->y());
         e->accept(); EmitSignal(kMouseDoubleClickEvent);return;
      }
   }
   QWidget::mouseDoubleClickEvent(e);
std::cerr << __FILE__ << " " << __LINE__ << " leave void TQtWidget::mouseDoubleClickEvent(QMouseEvent * e)\n";
}
//_____________________________________________________________________________
void TQtWidget::keyPressEvent(QKeyEvent * e)
{
   //  Map the Qt key press event to the ROOT TCanvas events
   // kKeyDown  =  4
   TCanvas *c = Canvas();
   if (c){
      c->HandleInput(kKeyPress, e->ascii(), e->key());
      EmitSignal(kKeyPressEvent);
   }
   QWidget::keyPressEvent(e);
}
//_____________________________________________________________________________
void TQtWidget::keyReleaseEvent(QKeyEvent * e)
{
   // Map the Qt key release event to the ROOT TCanvas events
   // kKeyUp    = 14
   QWidget::keyReleaseEvent(e);
}
//_____________________________________________________________________________
void TQtWidget::enterEvent(QEvent *e)
{
   // Map the Qt mouse enters widget event to the ROOT TCanvas events
   // kMouseEnter    = 52
   TCanvas *c = Canvas();
   if (c){
      c->HandleInput(kMouseEnter, 0, 0);
      EmitSignal(kEnterEvent);
  }
   QWidget::enterEvent(e);
}
//_____________________________________________________________________________
void TQtWidget::leaveEvent (QEvent *e)
{
   //  Map the Qt mouse leaves widget event to the ROOT TCanvas events
   // kMouseLeave    = 53
   TCanvas *c = Canvas();
   if (c){
      c->HandleInput(kMouseLeave, 0, 0);
      EmitSignal(kLeaveEvent);
   }
   QWidget::leaveEvent(e);
}
//_____________________________________________________________________________
void TQtWidget::resizeEvent(QResizeEvent *e)
{
   // The widget will be erased and receive a paint event immediately after
   // processing the resize event.
   // No drawing need be (or should be) done inside this handler.
   if (!e) return;
   if (topLevelWidget()->isMinimized())      { fSizeChanged=FALSE; }
   else if (topLevelWidget()->isMaximized ()){
      fSizeChanged=TRUE;
      exitSizeEvent();
      fSizeChanged=TRUE;
   } else {
#ifdef R__QTWIN32
      if (!fPaint)  {
         // real resize event
         fSizeChanged=TRUE;
         stretchWidget(e);
      }
#else
      fSizeChanged=TRUE;
      fPaint = kTRUE;
      exitSizeEvent();
#endif

   }
   if ( autoMask() )
      updateMask();
}
//____________________________________________________________________________
void TQtWidget::SetSaveFormat(const char *format)
{
     // Set the default save format for the widget
   fSaveFormat = TGQt::QtFileFormat(format);
}
//____________________________________________________________________________
bool TQtWidget::Save(const char *fileName) const
{
   //
   //  TQtWidget::Save(const QString &fileName) is a public Qt slot.
   //  it saves the double buffer of this object using the default save
   //  format  defined the file extension
   //  If the "fileName" has no extension the "default" format is to be used instead
   //  The deafult format is "PNG".
   //  It can be changed with the TQtWidget::SetSaveFormat method
   //
    return Save(QString(fileName));
}
//____________________________________________________________________________
bool TQtWidget::Save(const QString &fileName) const
{
   //
   //  TQtWidget::Save(const QString &fileName) is a public Qt slot.
   //  it saves the double buffer of this object using the default save
   //  format  defined the file extension
   //  If the "fileName" has no extension the "default" format is to be used instead
   //  The deafult format is "PNG".
   //  It can be changed with the TQtWidget::SetSaveFormat method
   //
   QString fileNameExtension = QFileInfo(fileName).extension(FALSE).upper();
   QString saveFormat;
   if (fileNameExtension.isEmpty() ) {
      saveFormat = fSaveFormat; // this is default
   } else {
      saveFormat = TGQt::QtFileFormat(fileNameExtension);
   }
   return Save(fileName,saveFormat);
}

//____________________________________________________________________________
bool TQtWidget::Save(const char *fileName,const char *format,int quality)const
{
   return Save(QString(fileName),format,quality);
}
//____________________________________________________________________________
bool TQtWidget::Save(const QString &fileName,const char *format,int quality)const
{
   //  TQtWidget::save is a public Qt slot.
   //  it saves the double buffer of this object using QPixmap facility
   bool Ok = false;
   bool rootFormatFound=kTRUE;
   QString saveType =  TGQt::RootFileFormat(format);
   if (saveType.isEmpty() )  {
      rootFormatFound = false;
      saveType = TGQt::QtFileFormat(format);
   }
   TCanvas *c = GetCanvas();
   if (rootFormatFound && c) {
      c->Print((const char *)fileName,(const char *)saveType);
      Ok = true;
   } else {
      Ok = GetBuffer().save(fileName,saveType,quality);
   }
   emit ((TQtWidget *)this)->Saved(Ok);
   return Ok;
}
//_____________________________________________________________________________
void TQtWidget::stretchWidget(QResizeEvent * /*s*/)
{
   // Stretch the widget during sizing

   if  (!paintingActive()) {
#ifdef R__QTWIN32
      QPainter painter( this );
      if (!StretchBlt(
         painter.handle(),    // handle of destination device context
         0,           // x-coordinate of upper-left corner of dest. rect.
         0,           // y-coordinate of upper-left corner of dest. rect.
         width(),     // width of destination rectangle
         height(),    // height of destination rectangle
         GetBuffer().handle(), // handle of source device context
         0,           // x-coordinate of upper-left corner of source rectangle
         0,           // y-coordinate of upper-left corner of source rectangle
         GetBuffer().width(),  // width of source rectangle
         GetBuffer().height(), // height of source rectangle
         SRCCOPY      // raster operation code
         )) {
            qSystemWarning("StretchBlt failed!" );
            printf("last error %d\n",GetLastError());
         }
#else
      QPainter pnt(this);
      pnt.drawPixmap(rect(),GetBuffer());
#endif
   }
}
//_____________________________________________________________________________
void TQtWidget::exitSizeEvent ()
{
   // Responce to the "exit size event"

   if (!fSizeChanged) return;
   Refresh();
}

//____________________________________________________________________________
bool TQtWidget::paintFlag(bool mode)
{
   //  Set new fPaint flag
   //  Returns: the previous version of the flag
   bool flag = fPaint;
   fPaint = mode;
   return flag;
}
//____________________________________________________________________________
void TQtWidget::showEvent ( QShowEvent *)
{
   // Custom handler of the Qt show event
   // Non-spontaneous show events are sent to widgets immediately before
   // they are shown.
   // The spontaneous show events of top-level widgets are delivered afterwards.

   if ( fPixmapID.size() != size() )
   {
      fSizeChanged = kTRUE;
      exitSizeEvent();
   }
}

//____________________________________________________________________________
void TQtWidget::paintEvent (QPaintEvent *e)
{
   // Custom handler of the Qt paint event
   // A paint event is a request to repaint all or part of the widget.
   // It can happen as a result of repaint() or update(), or because the widget
   // was obscured and has now been uncovered, or for many other reasons.

#ifdef R__QTWIN32
   if ( fEmbedded && fPixmapID.size() != size() )
   {
      fSizeChanged = kTRUE;
      exitSizeEvent();
      update();
      return;
   }
#endif
   QRect rect = e->rect();
   if ( fPaint && !rect.isEmpty() )
   {
      // fprintf(stderr,"TQtWidget::paintEvent: window = %p; buffer =  %p\n",
      //  (QPaintDevice *)this, (QPaintDevice *)&GetBuffer());
      bitBlt(this, rect.x(),rect.y(),&GetBuffer(),rect.x(), rect.y(), rect.width(), rect.height());
   }
}
//  Layout methods:
//____________________________________________________________________________
void TQtWidget::SetSizeHint (const QSize &size) {
   //  sets the preferred size of the widget.
   fSizeHint = size;
}
//____________________________________________________________________________
QSize TQtWidget::sizeHint () const{
   //  returns the preferred size of the widget.
   return QWidget::sizeHint();
}
//____________________________________________________________________________
QSize TQtWidget::minimumSizeHint () const{
   // returns the smallest size the widget can have.
   return QWidget::minimumSizeHint ();
}
//____________________________________________________________________________
QSizePolicy TQtWidget::sizePolicy () const{
   //  returns a QSizePolicy; a value describing the space requirements
   return QWidget::sizePolicy ();
}
//____________________________________________________________________________
void  TQtWidget::EmitTestedSignal() 
{
   TCanvas *c        = GetCanvas();
   TObject *selected = GetSelected();
   UInt_t event      = GetEvent();
   emit RootEventProcessed(selected, event, c);
}
//____________________________________________________________________________
void  TQtWidget::SetBit(UInt_t f, Bool_t set)
{
   // Set or unset the user status bits as specified in f.

   if (set)
      SetBit(f);
   else
      ResetBit(f);
}
