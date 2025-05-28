// @(#)root/win32gdk:$Name:  $:$Id: TGWin32VirtualGLProxy.h,v 1.8 2005/01/27 14:51:49 brun Exp $
// Author: Valeriy Onuchin   05/08/04

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGWin32VirtualGLProxy
#define ROOT_TGWin32VirtualGLProxy


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGWin32VirtualGLProxy                                                //
//                                                                      //
// The TGWin32VirtualGLProxy proxy class to TVirtualGL                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TVirtualGL
#include "TVirtualGL.h"
#endif

#ifndef ROOT_TGWin32ProxyBase
#include "TGWin32ProxyBase.h"
#endif


class TGWin32VirtualGLProxy : public TVirtualGL, public TGWin32ProxyBase {

public:
   TGWin32VirtualGLProxy();
   virtual ~TGWin32VirtualGLProxy() {}

   Window_t CreateGLWindow(Window_t wind);
   ULong_t CreateContext(Window_t wind);
   void DeleteContext(ULong_t ctx);
   void MakeCurrent(Window_t wind, ULong_t ctx);
   void SwapBuffers(Window_t wind);
   void AddRotation(Double_t *rotmatrix, Double_t *extraangles);
   void BeginGLCmd(EG3D2GLmode mode);
   void ClearColor(Int_t color);
   void ClearGLColor(Float_t red, Float_t green, Float_t blue, Float_t alpha);
   void ClearGLDepth(Float_t val);
   void MatrixModeGL(EG3D2GLmode matrix);
   void NewMVGL();
   void NewPRGL();
   void FrustumGL(Double_t xmin, Double_t xmax, Double_t ymin,
                  Double_t ymax, Double_t znear, Double_t zfar);
   void GLLight(EG3D2GLmode name, EG3D2GLmode prop, const Float_t * lig_prop);
   void LightModel(EG3D2GLmode name, const Float_t * lig_prop);
   void LightModel(EG3D2GLmode name, Int_t prop);
   void CullFaceGL(EG3D2GLmode);
   void ViewportGL(Int_t xmin, Int_t ymin, Int_t width, Int_t height);
   void MaterialGL(EG3D2GLmode face, const Float_t * mat_prop);
   void MaterialGL(EG3D2GLmode face, Float_t mat_prop);
   void BeginGL(EG3D2GLmode);
   void EndGL();
   void SetGLVertex(const Double_t * vert);
   void SetGLNormal(const Double_t * norm);
   void ClearGL(UInt_t bufbits );
   Int_t CreateGLLists(Int_t range);
   void DeleteGLLists(Int_t ilist, Int_t range);
   void DisableGL(EG3D2GLmode mode);
   void EnableGL(EG3D2GLmode mode);
   void EndGLList();
   void EndGLCmd();
   void FlushGL();
   void FrontGLFace(EG3D2GLmode faceflag);
   void GetGL(EG3D2GLmode mode, UChar_t *params);
   void GetGL(EG3D2GLmode mode, Double_t *params);
   void GetGL(EG3D2GLmode mode, Float_t *params);
   void GetGL(EG3D2GLmode mode, Int_t *params);
   Int_t GetGLError();
   Bool_t GetRootLight();
   Bool_t GetTrueColorMode();
   void MultGLMatrix(Double_t *mat);
   void NewGLList(UInt_t ilist=1, EG3D2GLmode mode=kCOMPILE);
   void NewGLModelView(Int_t ilist);
   void PaintGLPoints(Int_t n, Float_t *p, Option_t *option);
   void PolygonGLMode(EG3D2GLmode face, EG3D2GLmode mode);
   void PushGLMatrix();
   void PopGLMatrix();
   void RotateGL(Double_t angle, Double_t x, Double_t y, Double_t z);
   void RotateGL(Double_t Theta, Double_t Phi, Double_t Psi);
   void SetGLVertex(Float_t *vertex);
   void SetGLColor(Float_t *rgb);
   void SetGLColorIndex(Int_t color);
   void SetGLLineWidth(Float_t width);
   void SetGLPointSize(Float_t size);
   void SetStack(Double_t *matrix=0);
   void ShadeGLModel(EG3D2GLmode mode);
   void TranslateGL(Double_t x, Double_t y, Double_t z);
   void RunGLList(Int_t list);
   void NewProjectionView(Double_t viewboxmin[], Double_t viewboxmax[], Bool_t perspective=kTRUE);
   void NewModelView(Double_t *angles, Double_t *delta );
   void PaintCone(Float_t *vertex, Int_t ndiv, Int_t nstacks);
   void PaintPolyLine(Int_t n, Float_t *p, Option_t *option);
   void PaintPolyLine(Int_t n, Double_t *p, Option_t *option);
   void PaintGLPointsObject(const TPoints3DABC *points, Option_t *option="");
   void PaintBrik(Float_t vertex[24]);
   void PaintXtru(Float_t *vertex, Int_t nxy, Int_t nz);
   void SetRootLight(Bool_t flag=kTRUE);
   void SetLineAttr(Color_t color, Int_t width=1);
   void UpdateMatrix(Double_t *translate=0, Double_t *rotate=0, Bool_t isreflection=kFALSE);
   void SetTrueColorMode(Bool_t flag=kTRUE);
   void PaintPolyMarker(const Double_t * vertices, Style_t marker_style, UInt_t size);
   void DrawSelectionBox(
                         Double_t xmin, Double_t xmax, 
						       Double_t ymin, Double_t ymax,
						       Double_t zmin, Double_t zmax
						      );
   void EnterSelectionMode(UInt_t * buff, Int_t size, Event_t *, Int_t * viewport);
   Int_t ExitSelectionMode();
   void GLLoadName(UInt_t name);
   void DrawFaceSet(const Double_t * pnts, const Int_t * pols,
                    const Double_t * normals, const Float_t * mat,
                    UInt_t size);
   void TraverseGraph(TGLRender *render);
   TGLSceneObject *SelectObject(TGLRender *render, Int_t x, Int_t y, Int_t camera);
   void MoveSelected(TGLRender *render, Double_t x, Double_t y, Double_t z);
   void EndMovement(TGLRender *render);
   void Invalidate(TGLRender *render);
   void DrawSphere(const Float_t *color);
   void PrintObjects(Int_t format, Int_t sort, TGLRender *render, TGLWindow *glWin, 
                     Float_t rad, Float_t yc, Float_t zc);

   static TVirtualGL *ProxyObject();
   static TVirtualGL *RealObject();
};

#endif
