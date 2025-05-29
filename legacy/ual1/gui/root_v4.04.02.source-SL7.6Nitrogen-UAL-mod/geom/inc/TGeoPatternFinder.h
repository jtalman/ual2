// @(#)root/geom:$Name:  $:$Id: TGeoPatternFinder.h,v 1.10 2005/02/09 13:30:27 brun Exp $
// Author: Andrei Gheata   30/10/01

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGeoPatternFinder
#define ROOT_TGeoPatternFinder

#ifndef ROOT_TObject
#include "TObject.h"
#endif

#ifndef ROOT_TGeoMatrix
#include "TGeoMatrix.h"
#endif

#ifndef ROOT_TGeoVolume
#include "TGeoVolume.h"
#endif


/*************************************************************************
 * TGeoPatternFinder - base finder class for patterns. A pattern is specifying 
 *   a division type
 *************************************************************************/


class TGeoPatternFinder : public TObject
{
protected :
   Double_t            fStep;           // division step length
   Double_t            fStart;          // starting point on divided axis
   Double_t            fEnd;            // ending point
   Int_t               fCurrent;        // current division element
   Int_t               fNdivisions;     // number of divisions
   Int_t               fDivIndex;       // index of first div. node
   TGeoMatrix         *fMatrix;         // generic matrix
   TGeoVolume         *fVolume;         // volume to which applies

public:
   // constructors
   TGeoPatternFinder();
   TGeoPatternFinder(TGeoVolume *vol, Int_t ndiv);
   // destructor
   virtual ~TGeoPatternFinder();
   // methods
   virtual void        cd(Int_t /*idiv*/) {}
   virtual Int_t       GetByteCount() const {return 36;}
   Int_t               GetCurrent()      {return fCurrent;}
   Int_t               GetDivIndex()     {return fDivIndex;}
   virtual TGeoMatrix *GetMatrix()       {return fMatrix;}
   Int_t               GetNdiv() const   {return fNdivisions;}
   TGeoNode           *GetNodeOffset(Int_t idiv) {return fVolume->GetNode(fDivIndex+idiv);}  
   Double_t            GetStart() const  {return fStart;}
   Double_t            GetStep() const   {return fStep;}
   Double_t            GetEnd() const    {return fEnd;}
   TGeoVolume         *GetVolume() const {return fVolume;}
   virtual TGeoNode   *FindNode(Double_t * /*point*/) {return 0;} 
   void                SetDivIndex(Int_t index) {fDivIndex = index;}
   void                SetVolume(TGeoVolume *vol) {fVolume = vol;}

  ClassDef(TGeoPatternFinder, 3)              // patterns to divide volumes
};


/*************************************************************************
 * TGeoPatternX - a X axis divison pattern
 *   
 *************************************************************************/
class TGeoTranslation;

class TGeoPatternX : public TGeoPatternFinder 
{
public:
   // constructors
   TGeoPatternX();
   TGeoPatternX(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternX(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternX(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);

   // destructor
   virtual ~TGeoPatternX();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv; 
                           ((TGeoTranslation*)fMatrix)->SetDx(fStart+idiv*fStep+fStep/2);}
   virtual TGeoNode   *FindNode(Double_t *point);
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternX, 1)              // X division pattern
};

/*************************************************************************
 * TGeoPatternY - a Y axis divison pattern
 *   
 *************************************************************************/

class TGeoPatternY : public TGeoPatternFinder
{
public:
   // constructors
   TGeoPatternY();
   TGeoPatternY(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternY(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternY(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);
   // destructor
   virtual ~TGeoPatternY();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv; 
                          ((TGeoTranslation*)fMatrix)->SetDy(fStart+idiv*fStep+fStep/2);}
   virtual TGeoNode   *FindNode(Double_t *point); 
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternY, 1)              // Y division pattern
};

/*************************************************************************
 * TGeoPatternZ - a Z axis divison pattern
 *   
 *************************************************************************/

class TGeoPatternZ : public TGeoPatternFinder
{
public:
   // constructors
   TGeoPatternZ();
   TGeoPatternZ(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternZ(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternZ(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);
   // destructor
   virtual ~TGeoPatternZ();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv; 
                          ((TGeoTranslation*)fMatrix)->SetDz(fStart+idiv*fStep+fStep/2);}
   virtual TGeoNode   *FindNode(Double_t *point); 
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternZ, 1)              // Z division pattern
};

/*************************************************************************
 * TGeoPatternParaX - a X axis divison pattern for PARA shapes
 *   
 *************************************************************************/

class TGeoPatternParaX : public TGeoPatternFinder 
{
public:
   // constructors
   TGeoPatternParaX();
   TGeoPatternParaX(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternParaX(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternParaX(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);

   // destructor
   virtual ~TGeoPatternParaX();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv; 
                           ((TGeoTranslation*)fMatrix)->SetDx(fStart+idiv*fStep+fStep/2);}
   virtual TGeoNode   *FindNode(Double_t *point);
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternParaX, 1)              // Para X division pattern
};

/*************************************************************************
 * TGeoPatternParaY - a Y axis divison pattern for PARA shapes
 *   
 *************************************************************************/

class TGeoPatternParaY : public TGeoPatternFinder 
{
private :
// data members
   Double_t         fTxy;      // tangent of alpha
public:
   // constructors
   TGeoPatternParaY();
   TGeoPatternParaY(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternParaY(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternParaY(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);

   // destructor
   virtual ~TGeoPatternParaY();
   // methods
   virtual void        cd(Int_t idiv);
   virtual TGeoNode   *FindNode(Double_t *point);
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternParaY, 1)              // Para Y division pattern
};

/*************************************************************************
 * TGeoPatternParaZ - a Z axis divison pattern for PARA shapes
 *   
 *************************************************************************/

class TGeoPatternParaZ : public TGeoPatternFinder 
{
private :
// data members
   Double_t            fTxz;  // tangent of alpha xz
   Double_t            fTyz;  // tangent of alpha yz
public:
   // constructors
   TGeoPatternParaZ();
   TGeoPatternParaZ(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternParaZ(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternParaZ(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);

   // destructor
   virtual ~TGeoPatternParaZ();
   // methods
   virtual void        cd(Int_t idiv);
   virtual TGeoNode   *FindNode(Double_t *point);
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternParaZ, 1)              // Para Z division pattern
};

/*************************************************************************
 * TGeoPatternTrapZ - a Z axis divison pattern for TRAP or GTRA shapes
 *   
 *************************************************************************/

class TGeoPatternTrapZ : public TGeoPatternFinder 
{
private :
// data members
   Double_t            fTxz;  // tangent of alpha xz
   Double_t            fTyz;  // tangent of alpha yz
public:
   // constructors
   TGeoPatternTrapZ();
   TGeoPatternTrapZ(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternTrapZ(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternTrapZ(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);

   // destructor
   virtual ~TGeoPatternTrapZ();
   // methods
   Double_t            GetTxz() const {return fTxz;}
   Double_t            GetTyz() const {return fTyz;}
   virtual void        cd(Int_t idiv);
   virtual TGeoNode   *FindNode(Double_t *point);
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternTrapZ, 1)              // Trap od Gtra Z division pattern
};


/*************************************************************************
 * TGeoPatternCylR - a cylindrical R divison pattern
 *   
 *************************************************************************/

class TGeoPatternCylR : public TGeoPatternFinder
{
public:
   // constructors
   TGeoPatternCylR();
   TGeoPatternCylR(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternCylR(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternCylR(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);
   // destructor
   virtual ~TGeoPatternCylR();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv;}
   virtual TGeoNode   *FindNode(Double_t *point); 
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternCylR, 1)              // Cylindrical R division pattern
};

/*************************************************************************
 * TGeoPatternCylPhi - a cylindrical phi divison pattern
 *   
 *************************************************************************/

class TGeoPatternCylPhi : public TGeoPatternFinder
{
private :
// data members
   Double_t           *fSinCos;          //![2*fNdivisions] table of sines/cosines
public:
   // constructors
   TGeoPatternCylPhi();
   TGeoPatternCylPhi(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternCylPhi(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternCylPhi(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);
   // destructor
   virtual ~TGeoPatternCylPhi();
   // methods
   virtual void        cd(Int_t idiv);
   virtual TGeoNode   *FindNode(Double_t *point); 
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternCylPhi, 1)              // Cylindrical phi division pattern
};

/*************************************************************************
 * TGeoPatternSphR - a spherical R divison pattern
 *   
 *************************************************************************/

class TGeoPatternSphR : public TGeoPatternFinder
{
public:
   // constructors
   TGeoPatternSphR();
   TGeoPatternSphR(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternSphR(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternSphR(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);
   // destructor
   virtual ~TGeoPatternSphR();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv;}
   virtual TGeoNode   *FindNode(Double_t *point); 
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternSphR, 1)              // spherical R division pattern
};

/*************************************************************************
 * TGeoPatternSphTheta - a spherical theta divison pattern
 *   
 *************************************************************************/

class TGeoPatternSphTheta : public TGeoPatternFinder
{
private :
// data members

public:
   // constructors
   TGeoPatternSphTheta();
   TGeoPatternSphTheta(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternSphTheta(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternSphTheta(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);
   // destructor
   virtual ~TGeoPatternSphTheta();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv;}
   virtual TGeoNode   *FindNode(Double_t *point); 
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternSphTheta, 1)              // spherical theta division pattern

//***** Need to add class and globals to LinkDef.h *****
};

/*************************************************************************
 * TGeoPatternSphPhi - a spherical phi divison pattern
 *   
 *************************************************************************/

class TGeoPatternSphPhi : public TGeoPatternFinder
{
public:
   // constructors
   TGeoPatternSphPhi();
   TGeoPatternSphPhi(TGeoVolume *vol, Int_t ndivisions);
   TGeoPatternSphPhi(TGeoVolume *vol, Int_t ndivisions, Double_t step);
   TGeoPatternSphPhi(TGeoVolume *vol, Int_t ndivisions, Double_t start, Double_t end);
   // destructor
   virtual ~TGeoPatternSphPhi();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv;}
   virtual TGeoNode   *FindNode(Double_t *point); 
   virtual void        SavePrimitive(ofstream &out, Option_t *option);

  ClassDef(TGeoPatternSphPhi, 1)              // Spherical phi division pattern
};

/*************************************************************************
 * TGeoPatternHoneycomb - a divison pattern specialized for honeycombs
 *   
 *************************************************************************/

class TGeoPatternHoneycomb : public TGeoPatternFinder
{
private :
// data members
   Int_t               fNrows;                  // number of rows
   Int_t               fAxisOnRows;             // axis along each row
   Int_t              *fNdivisions;             // [fNrows] number of divisions for each row
   Double_t           *fStart;                  // [fNrows] starting points for each row

public:
   // constructors
   TGeoPatternHoneycomb();
   TGeoPatternHoneycomb(TGeoVolume *vol, Int_t nrows);
   // destructor
   virtual ~TGeoPatternHoneycomb();
   // methods
   virtual void        cd(Int_t idiv) {fCurrent=idiv;}
   virtual TGeoNode   *FindNode(Double_t *point); 

  ClassDef(TGeoPatternHoneycomb, 1)             // pattern for honeycomb divisions
};

#endif

