// @(#)root/tree:$Name:  $:$Id: TCut.cxx,v 1.4 2000/10/27 16:31:27 rdm Exp $
// Author: Rene Brun   14/04/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TCut                                                                 //
//                                                                      //
//  A specialized string object used for TTree selections.              //
//  A TCut object has a name and a title. It does not add any data      //
//  members compared to a TNamed. It only add a set of operators to     //
//  facilitate logical string concatenation. For example, assume        //
//     cut1 = "x<1"  and cut2 = "y>2"                                   //
//  then                                                                //
//     cut1 && cut2 will be the string "(x<1)&&(y>2)"                   //
//                                                                      //
//  Operators =, +=, +, *, !, &&, || overloaded.                        //
//                                                                      //
//   Examples of use:                                                   //
//     Root > TCut c1 = "x<1"                                           //
//     Root > TCut c2 = "y<0"                                           //
//     Root > TCut c3 = c1&&c2                                          //
//     Root > ntuple.Draw("x", c1)                                      //
//     Root > ntuple.Draw("x", c1||"x>0")                               //
//     Root > ntuple.Draw("x", c1&&c2)                                  //
//     Root > ntuple.Draw("x", "(x+y)"*(c1&&c2))                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TCut.h"

ClassImp(TCut)

//______________________________________________________________________________
TCut::TCut() : TNamed()
{

}

//______________________________________________________________________________
TCut::TCut(const char *title) : TNamed("CUT",title)
{

}

//______________________________________________________________________________
TCut::TCut(const char *name, const char *title) : TNamed(name,title)
{

}

//______________________________________________________________________________
TCut::TCut(const TCut &cut) : TNamed(cut)
{
   //fName  = cut.fName;
   //fTitle = cut.fTitle;
}

//______________________________________________________________________________
TCut::~TCut()
{

}

//______________________________________________________________________________
TCut& TCut::operator=(const char *rhs)
{
   fTitle = rhs;
   return *this;
}

//______________________________________________________________________________
TCut& TCut::operator=(const TCut& rhs)
{
   if (this != &rhs)
      TNamed::operator=(rhs);

   return *this;
}

//______________________________________________________________________________
TCut& TCut::operator+=(const char *rhs)
{
   if (!rhs || strlen(rhs) == 0) return *this;
   if (fTitle.Length() == 0)
      fTitle = rhs;
   else
      fTitle = "(" + fTitle + ")&&(" + TString(rhs) + ")";
   return *this;
}

//______________________________________________________________________________
TCut& TCut::operator+=(const TCut& rhs)
{
   if (rhs.fTitle.Length() == 0) return *this;
   if (fTitle.Length() == 0)
      fTitle = rhs;
   else
      fTitle = "(" + fTitle + ")&&(" + rhs.fTitle + ")";
   return *this;
}

//______________________________________________________________________________
TCut& TCut::operator*=(const char *rhs)
{
   if (!rhs || strlen(rhs) == 0) return *this;
   if (fTitle.Length() == 0)
      fTitle = rhs;
   else
      fTitle = "(" + fTitle + ")*(" + TString(rhs) + ")";
   return *this;
}

//______________________________________________________________________________
TCut& TCut::operator*=(const TCut& rhs)
{
   if (rhs.fTitle.Length() == 0) return *this;
   if (fTitle.Length() == 0)
      fTitle = rhs;
   else
      fTitle = "(" + fTitle + ")*(" + rhs.fTitle + ")";
   return *this;
}

//______________________________________________________________________________
TCut operator+(const TCut& lhs, const char *rhs)
{
   return TCut(lhs) += rhs;
}

//______________________________________________________________________________
TCut operator+(const char *lhs, const TCut& rhs)
{
   return TCut(lhs) += rhs;
}

//______________________________________________________________________________
TCut operator+(const TCut& lhs, const TCut& rhs)
{
   return TCut(lhs) += rhs;
}

//______________________________________________________________________________
TCut operator*(const TCut& lhs, const char *rhs)
{
   return TCut(lhs) *= rhs;
}

//______________________________________________________________________________
TCut operator*(const char *lhs, const TCut& rhs)
{
   return TCut(lhs) *= rhs;
}

//______________________________________________________________________________
TCut operator*(const TCut& lhs, const TCut& rhs)
{
   return TCut(lhs) *= rhs;
}

//______________________________________________________________________________
TCut operator&&(const TCut& lhs, const char *rhs)
{
   return TCut(lhs) += rhs;
}

//______________________________________________________________________________
TCut operator&&(const char *lhs, const TCut& rhs)
{
   return TCut(lhs) += rhs;
}

//______________________________________________________________________________
TCut operator&&(const TCut& lhs, const TCut& rhs)
{
   return TCut(lhs) += rhs;
}

//______________________________________________________________________________
TCut operator||(const TCut& lhs, const char *rhs)
{
   if (lhs.fTitle.Length() == 0 && (!rhs || strlen(rhs) == 0)) return TCut();
   if (lhs.fTitle.Length() == 0) return TCut(rhs);
   if (!rhs || strlen(rhs) == 0) return TCut(lhs);
   TString s = "(" + lhs.fTitle + ")||(" + TString(rhs) + ")";
   return TCut(s.Data());
}

//______________________________________________________________________________
TCut operator||(const char *lhs, const TCut& rhs)
{
   if ((!lhs || strlen(lhs) == 0) && rhs.fTitle.Length() == 0) return TCut();
   if (!lhs || strlen(lhs) == 0) return TCut(rhs);
   if (rhs.fTitle.Length() == 0) return TCut(lhs);
   TString s = "(" + TString(lhs) + ")||(" + rhs.fTitle + ")";
   return TCut(s.Data());
}

//______________________________________________________________________________
TCut operator||(const TCut& lhs, const TCut& rhs)
{
   if (lhs.fTitle.Length() == 0 && rhs.fTitle.Length() == 0) return TCut();
   if (lhs.fTitle.Length() == 0) return TCut(rhs);
   if (rhs.fTitle.Length() == 0) return TCut(lhs);
   TString s = "(" + lhs.fTitle + ")||(" + rhs.fTitle + ")";
   return TCut(s.Data());
}

//______________________________________________________________________________
TCut operator!(const TCut &rhs)
{
   if (rhs.fTitle.Length() == 0) return TCut();
   TString s = "!(" + rhs.fTitle + ")";
   return TCut(s.Data());
}

