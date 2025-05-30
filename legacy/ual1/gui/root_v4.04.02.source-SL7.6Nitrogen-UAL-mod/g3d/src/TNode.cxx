// @(#)root/g3d:$Name:  $:$Id: TNode.cxx,v 1.25 2005/04/21 08:13:25 brun Exp $
// Author: Rene Brun   14/09/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "Riostream.h"
#include "TROOT.h"
#include "TClass.h"
#include "TVirtualPad.h"
#include "TView.h"
#include "TGeometry.h"
#include "TRotMatrix.h"
#include "TShape.h"
#include "TNode.h"
#include "TBrowser.h"
#include "X3DBuffer.h"
#include "TVirtualViewer3D.h"
#include "TBuffer3D.h"

#if 0
const Int_t kMAXLEVELS = 20;
const Int_t kVectorSize = 3;
const Int_t kMatrixSize = kVectorSize*kVectorSize;
#endif

static Double_t gTranslation[kMAXLEVELS][kVectorSize];
static Double_t gRotMatrix[kMAXLEVELS][kMatrixSize];
static Int_t gGeomLevel = 0;

TNode *gNode;


ClassImp(TNode)

//______________________________________________________________________________
//*-*-*-*-*-*-*-*-*-*-*-* T N O D E  description *-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                    =======================
//*-*
//*-*    A TNode object is used to build the geometry hierarchy (see TGeometry).
//*-*    A node may contain other nodes.
//*-*
//*-*    A geometry node has attributes:
//*-*      - name and title
//*-*      - pointer to the referenced shape (see TShape).
//*-*      - x,y,z offset with respect to the mother node.
//*-*      - pointer to the rotation matrix (see TRotMatrix).
//*-*
//*-*    A node can be drawn.
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


//______________________________________________________________________________
TNode::TNode()
{
//*-*-*-*-*-*-*-*-*-*-*Node default constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ========================

   fMatrix = 0;
   fParent = 0;
   fShape  = 0;
   fNodes  = 0;
   fVisibility = 1;
}

//______________________________________________________________________________
TNode::TNode(const char *name, const char *title, const char *shapename, Double_t x, Double_t y, Double_t z, const char *matrixname, Option_t *option)
       :TNamed(name,title),TAttLine(), TAttFill()
{
//*-*-*-*-*-*-*-*-*-*-*Node normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ======================
//*-*
//*-*    name    is the name of the node
//*-*    title   is title
//*-*    shapename is the name of the referenced shape
//*-*    x,y,z   are the offsets of the volume with respect to his mother
//*-*    matrixname  is the name of the rotation matrix
//*-*
//*-*    This new node is added into the list of sons of the current node
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#ifdef WIN32
//*-* The color "1" - default produces a very bad 3D image with OpenGL
   Color_t lcolor = 16;
   SetLineColor(lcolor);
#endif
   static Int_t counter = 0;
   counter++;
//   if(!(counter%1000))cout<<"TNode count="<<counter<<" name="<<name<<endl;
   fX      = x;
   fY      = y;
   fZ      = z;
   fNodes  = 0;
   fShape  = gGeometry->GetShape(shapename);
   fParent = gGeometry->GetCurrentNode();
   fOption = option;
   fVisibility = 1;

   if (strlen(matrixname)) fMatrix = gGeometry->GetRotMatrix(matrixname);
   else {
     fMatrix = gGeometry->GetRotMatrix("Identity");
     if (!fMatrix)
        fMatrix  = new TRotMatrix("Identity","Identity matrix",90,0,90,90,0,0);
   }

   if (!fShape) {
      Printf("Error Referenced shape does not exist: %s",shapename);
      return;
   }

   ImportShapeAttributes();
   if (fParent) {
      fParent->BuildListOfNodes();
      fParent->GetListOfNodes()->Add(this);
   } else {
      gGeometry->GetListOfNodes()->Add(this);
      cd();
   }
}


//______________________________________________________________________________
TNode::TNode(const char *name, const char *title, TShape *shape, Double_t x, Double_t y, Double_t z, TRotMatrix *matrix, Option_t *option)
                :TNamed(name,title),TAttLine(),TAttFill()
{
//*-*-*-*-*-*-*-*-*-*-*Node normal constructor*-*-*-*-*-*-*-*-*-*-*
//*-*                  ================================
//*-*
//*-*    name    is the name of the node
//*-*    title   is title
//*-*    shape   is the pointer to the shape definition
//*-*    x,y,z   are the offsets of the volume with respect to his mother
//*-*    matrix  is the pointer to the rotation matrix
//*-*
//*-*    This new node is added into the list of sons of the current node
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#ifdef WIN32
//*-* The color "1" - default produces a very bad 3D image with OpenGL
   Color_t lcolor = 16;
   SetLineColor(lcolor);
#endif

   fX      = x;
   fY      = y;
   fZ      = z;
   fNodes  = 0;
   fShape  = shape;
   fMatrix = matrix;
   fOption = option;
   fVisibility = 1;
   fParent = gGeometry->GetCurrentNode();
   if(!fMatrix) {
     fMatrix =gGeometry->GetRotMatrix("Identity");
     if (!fMatrix)
        fMatrix  = new TRotMatrix("Identity","Identity matrix",90,0,90,90,0,0);
   }

   if(!shape) {Printf("Illegal referenced shape"); return;}

   if (fParent) {
      fParent->BuildListOfNodes();
      fParent->GetListOfNodes()->Add(this);
      ImportShapeAttributes();
   } else {
      gGeometry->GetListOfNodes()->Add(this);
      cd();
   }

}

//______________________________________________________________________________
TNode::~TNode()
{
//*-*-*-*-*-*-*-*-*-*-*Node default destructor*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ======================

   //if (!gGeometry) return;
   if (fParent)     fParent->GetListOfNodes()->Remove(this);
   else    {if (gGeometry) gGeometry->GetListOfNodes()->Remove(this);}
   if (fNodes) fNodes->Delete();
   if (gGeometry && gGeometry->GetCurrentNode() == this) gGeometry->SetCurrentNode(0);
   delete fNodes;
   fNodes = 0;
}

//______________________________________________________________________________
void TNode::Browse(TBrowser *b)
{
    if( fNodes ) {
       fNodes->Browse( b );
    } else {
       Draw();
       gPad->Update();
    }
}

//______________________________________________________________________________
void TNode::BuildListOfNodes()
{
//*-*-*-*-*-*Create the list to support sons of this node*-*-*-*-*-*-*-*-*-*-*
//*-*        ============================================

   if (!fNodes) fNodes   = new TList;
}

//______________________________________________________________________________
void TNode::cd(const char *)
{
//*-*-*-*-*-*Change Current Reference node to this*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*        =====================================

   gGeometry->SetCurrentNode(this);
}

//______________________________________________________________________________
Int_t TNode::DistancetoPrimitive(Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*-*Compute distance from point px,py to a Node*-*-*-*-*-*
//*-*                  ===========================================
//*-*  Compute the closest distance of approach from point px,py to this node.
//*-*  The distance is computed in pixels units.
//*-*
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

   const Int_t big = 9999;
   const Int_t inaxis = 7;
   const Int_t maxdist = 5;

   Int_t puxmin = gPad->XtoAbsPixel(gPad->GetUxmin());
   Int_t puymin = gPad->YtoAbsPixel(gPad->GetUymin());
   Int_t puxmax = gPad->XtoAbsPixel(gPad->GetUxmax());
   Int_t puymax = gPad->YtoAbsPixel(gPad->GetUymax());

//*-*- return if point is not in the user area
   if (px < puxmin - inaxis) return big;
   if (py > puymin + inaxis) return big;
   if (px > puxmax + inaxis) return big;
   if (py < puymax - inaxis) return big;

   TView *view =gPad->GetView();
   if (!view) return big;

//*-*- Update translation vector and rotation matrix for new level
   if (fMatrix && gGeometry) {
      gGeometry->UpdateTempMatrix(fX,fY,fZ,fMatrix->GetMatrix(),fMatrix->IsReflection());
   }
//*-*- Paint Referenced shape
   Int_t dist = big;
   if (fVisibility && fShape->GetVisibility()) {
      gNode = this;
      dist = fShape->DistancetoPrimitive(px,py);
      if (dist < maxdist) {
         gPad->SetSelected(this);
         return 0;
      }
   }
   if ( TestBit(kSonsInvisible) ) return dist;
   if (!gGeometry) return dist;

//*-*- Loop on all sons
   Int_t nsons = 0;
   if (fNodes) nsons = fNodes->GetSize();
   Int_t dnode = dist;
   if (nsons) {

      gGeometry->PushLevel();
      TNode *node;
      TObject *obj;
      TIter  next(fNodes);
      while ((obj = next())) {
         node = (TNode*)obj;
         dnode = node->DistancetoPrimitive(px,py);
         if (dnode <= 0) break;
         if (dnode < dist) dist = dnode;
      }
      gGeometry->PopLevel();
   }

   return dnode;
}

//______________________________________________________________________________
void TNode::Draw(Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*-*Draw Referenced node with current parameters*-*-*-*
//*-*                   =============================================

   TString opt = option;
   opt.ToLower();
//*-*- Clear pad if option "same" not given
   if (!gPad) {
      if (!gROOT->GetMakeDefCanvas()) return;
      (gROOT->GetMakeDefCanvas())();
   }
   if (!opt.Contains("same")) gPad->Clear();

//*-*- Draw Referenced node
   if (!gGeometry) new TGeometry;
   gGeometry->SetGeomLevel();
   gGeometry->UpdateTempMatrix();

   AppendPad(option);

   // Create a 3-D view
   TView *view = gPad->GetView();
   if (!view) {
      view = new TView(11);
      // Set the view to perform a first autorange (frame) draw. 
      // TViewer3DPad will revert view to normal painting after this
      view->SetAutoRange(kTRUE);
   }
   
   // Create a 3D viewer to draw us
   gPad->GetViewer3D(option);
}

//______________________________________________________________________________
void TNode::DrawOnly(Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*Draw only Sons of this node*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                ===========================

   SetVisibility(2);
   Draw(option);
}


//______________________________________________________________________________
void TNode::ExecuteEvent(Int_t, Int_t, Int_t)
{
//*-*-*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*
//*-*                  =========================================
//*-*  This member function must be implemented to realize the action
//*-*  corresponding to the mouse click on the object in the window
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//   if (gPad->GetView())
//             gPad->GetView()->ExecuteRotateView(event, px, py);

//   if (!gPad->GetListOfPrimitives()->FindObject(this)) gPad->SetCursor(kCross);
   gPad->SetCursor(kHand);
}

//______________________________________________________________________________
TNode *TNode::GetNode(const char *name) const
{
//*-*-*-*-*-*-*Return pointer to node with name in the node tree*-*-*-*-*
//*-*          =================================================

   if (!strcmp(name, GetName())) return (TNode*)this;
   TNode *node, *nodefound;
   if (!fNodes) return 0;
   TObjLink *lnk = fNodes->FirstLink();
   while (lnk) {
      node = (TNode *)lnk->GetObject();
      if (node->TestBit(kNotDeleted)) {
         nodefound = node->GetNode(name);
         if (nodefound) return nodefound;
      }
      lnk = lnk->Next();
   }
   return 0;
}

//______________________________________________________________________________
char *TNode::GetObjectInfo(Int_t, Int_t) const
{
   const char *snull = "";
   if (!gPad) return (char*)snull;
   static char info[64];
   sprintf(info,"%s/%s, shape=%s/%s",GetName(),GetTitle(),fShape->GetName(),fShape->ClassName());
   return info;
}

//______________________________________________________________________________
void TNode::ImportShapeAttributes()
{
//*-*-*-*-*-*-*Copy shape attributes as node attributes*-*-*-*-*--*-*-*-*-*-*
//*-*          ========================================

   SetLineColor(fShape->GetLineColor());
   SetLineStyle(fShape->GetLineStyle());
   SetLineWidth(fShape->GetLineWidth());
   SetFillColor(fShape->GetFillColor());
   SetFillStyle(fShape->GetFillStyle());

   if (!fNodes) return;
   TNode *node;

   TObjLink *lnk = fNodes->FirstLink();
   while (lnk) {
      node = (TNode *)lnk->GetObject();
      node->ImportShapeAttributes();
      lnk = lnk->Next();
   }

}

//______________________________________________________________________________
Bool_t TNode::IsFolder() const
{
//*-*-*-*-*Return TRUE if node contains nodes, FALSE otherwise*-*
//*-*      ======================================================

   if (fNodes) return kTRUE;
   else        return kFALSE;
}

//______________________________________________________________________________
void TNode::Local2Master(const Double_t *local, Double_t *master)
{
//*-*-*-*-*Convert one point from local system to master reference system*-*-*
//*-*      ==============================================================
//
//  Note that before invoking this function, the global rotation matrix
//  and translation vector for this node must have been computed.
//  This is automatically done by the Paint functions.
//  Otherwise TNode::UpdateMatrix should be called before.

   Double_t x,y,z;
   Float_t bomb = gGeometry->GetBomb();

   Double_t *matrix      = &gRotMatrix[gGeomLevel][0];
   Double_t *translation = &gTranslation[gGeomLevel][0];

   x = bomb*translation[0]
     + local[0]*matrix[0]
     + local[1]*matrix[3]
     + local[2]*matrix[6];

   y = bomb*translation[1]
     + local[0]*matrix[1]
     + local[1]*matrix[4]
     + local[2]*matrix[7];

   z = bomb*translation[2]
     + local[0]*matrix[2]
     + local[1]*matrix[5]
     + local[2]*matrix[8];

   master[0] = x; master[1] = y; master[2] = z;
}

//______________________________________________________________________________
void TNode::Local2Master(const Float_t *local, Float_t *master)
{
//*-*-*-*-*Convert one point from local system to master reference system*-*-*
//*-*      ==============================================================
//
//  Note that before invoking this function, the global rotation matrix
//  and translation vector for this node must have been computed.
//  This is automatically done by the Paint functions.
//  Otherwise TNode::UpdateMatrix should be called before.

   Float_t x,y,z;
   Float_t bomb = gGeometry->GetBomb();

   Double_t *matrix      = &gRotMatrix[gGeomLevel][0];
   Double_t *translation = &gTranslation[gGeomLevel][0];

   x = bomb*translation[0]
     + local[0]*matrix[0]
     + local[1]*matrix[3]
     + local[2]*matrix[6];

   y = bomb*translation[1]
     + local[0]*matrix[1]
     + local[1]*matrix[4]
     + local[2]*matrix[7];

   z = bomb*translation[2]
     + local[0]*matrix[2]
     + local[1]*matrix[5]
     + local[2]*matrix[8];

   master[0] = x; master[1] = y; master[2] = z;

}

//______________________________________________________________________________
void TNode::ls(Option_t *option) const
{
//*-*-*-*-*-*-*-*-*-*-*-*List Referenced object with current parameters*-*-*-*
//*-*                   ===============================================

   Int_t sizeX3D = 0;
   TString opt = option;
   opt.ToLower();

   if (!gGeometry) new TGeometry;

   Int_t maxlevel = 15;
   if (opt.Contains("1")) maxlevel = 1;
   if (opt.Contains("2")) maxlevel = 2;
   if (opt.Contains("3")) maxlevel = 3;
   if (opt.Contains("4")) maxlevel = 4;
   if (opt.Contains("5")) maxlevel = 5;
   if (opt.Contains("x")) sizeX3D  = 1;

   TROOT::IndentLevel();

   Int_t nsons = 0;
   if (fNodes) nsons = fNodes->GetSize();
   const char *shapename, *matrixname;
   if (fShape) shapename = fShape->IsA()->GetName();
   else        shapename = "????";
   cout<<GetName()<<":"<<GetTitle()<<" is a "<<shapename;
   if (sizeX3D) {
      gSize3D.numPoints = 0;
      gSize3D.numSegs   = 0;
      gSize3D.numPolys  = 0;
      Sizeof3D();
      cout<<" NumPoints="<<gSize3D.numPoints;
      cout<<" NumSegs  ="<<gSize3D.numSegs;
      cout<<" NumPolys ="<<gSize3D.numPolys;
   } else {
      cout<<" X="<<fX<<" Y="<<fY<<" Z="<<fZ;
      if (nsons) cout<<" Sons="<<nsons;
      if (fMatrix) matrixname   = fMatrix->GetName();
      else         matrixname   = "Identity";
      if(strcmp(matrixname,"Identity")) cout<<" Rot="<<matrixname;
   }
   cout<<endl;
   if(!nsons) return;
   if (gGeomLevel >= maxlevel) return;

   TROOT::IncreaseDirLevel();
   gGeomLevel++;
   fNodes->ls(option);
   gGeomLevel--;
   TROOT::DecreaseDirLevel();

}

//______________________________________________________________________________
void TNode::Master2Local(const Double_t *master, Double_t *local)
{
//*-*-*-*-*Convert one point from master system to local reference system*-*-*
//*-*      ==============================================================
//
//  Note that before invoking this function, the global rotation matrix
//  and translation vector for this node must have been computed.
//  This is automatically done by the Paint functions.
//  Otherwise TNode::UpdateMatrix should be called before.

   Double_t x,y,z;
   Float_t bomb = gGeometry->GetBomb();

   Double_t *matrix      = &gRotMatrix[gGeomLevel][0];
   Double_t *translation = &gTranslation[gGeomLevel][0];

   Double_t xms = master[0] - bomb*translation[0];
   Double_t yms = master[1] - bomb*translation[1];
   Double_t zms = master[2] - bomb*translation[2];

   x = xms*matrix[0] + yms*matrix[1] + zms*matrix[2];
   y = xms*matrix[3] + yms*matrix[4] + zms*matrix[5];
   z = xms*matrix[6] + yms*matrix[7] + zms*matrix[8];

   local[0] = x; local[1] = y; local[2] = z;
}

//______________________________________________________________________________
void TNode::Master2Local(const Float_t *master, Float_t *local)
{
//*-*-*-*-*Convert one point from master system to local reference system*-*-*
//*-*      ==============================================================
//
//  Note that before invoking this function, the global rotation matrix
//  and translation vector for this node must have been computed.
//  This is automatically done by the Paint functions.
//  Otherwise TNode::UpdateMatrix should be called before.

   Float_t x,y,z;
   Float_t bomb = gGeometry->GetBomb();

   Double_t *matrix      = &gRotMatrix[gGeomLevel][0];
   Double_t *translation = &gTranslation[gGeomLevel][0];

   Double_t xms = master[0] - bomb*translation[0];
   Double_t yms = master[1] - bomb*translation[1];
   Double_t zms = master[2] - bomb*translation[2];

   x = xms*matrix[0] + yms*matrix[1] + zms*matrix[2];
   y = xms*matrix[3] + yms*matrix[4] + zms*matrix[5];
   z = xms*matrix[6] + yms*matrix[7] + zms*matrix[8];

   local[0] = x; local[1] = y; local[2] = z;
}

//______________________________________________________________________________
void TNode::Paint(Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*-*Paint Referenced node with current parameters*-*-*-*
//*-*                   ==============================================
//*-*
//*-*  vis = 1  (default) shape is drawn
//*-*  vis = 0  shape is not drawn but its sons may be not drawn
//*-*  vis = -1 shape is not drawn. Its sons are not drawn
//*-*  vis = -2 shape is drawn. Its sons are not drawn
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

   Int_t level = 0;
   if (gGeometry) level = gGeometry->GeomLevel();
//*-*- Update translation vector and rotation matrix for new level
   if (level) {
      gGeometry->UpdateTempMatrix(fX,fY,fZ,fMatrix->GetMatrix(),fMatrix->IsReflection());
   }

//*-*- Paint Referenced shape
//   Int_t vis = fShape->GetVisibility();
//   if ( vis == -1) return;
   Int_t nsons = 0;
   if (fNodes) nsons = fNodes->GetSize();
//   if (vis == -3) {
//     if (nsons == 0) vis = 1;
//     else            vis = 0;
//   }

   TAttLine::Modify();
   TAttFill::Modify();

   Bool_t viewerWantsSons = kTRUE;

   if (fVisibility && fShape->GetVisibility()) {
      gNode = this;
      fShape->SetLineColor(GetLineColor());
      fShape->SetLineStyle(GetLineStyle());
      fShape->SetLineWidth(GetLineWidth());
      fShape->SetFillColor(GetFillColor());
      fShape->SetFillStyle(GetFillStyle());

      TVirtualViewer3D * viewer3D = gPad->GetViewer3D();
      if (viewer3D) {
         // We only provide master frame positions in these shapes
         // so don't ask viewer preference

         // Ask all shapes for kCore/kBoundingBox/kShapeSpecific
         // Not all will support the last two - which is fine
         const TBuffer3D & buffer = 
            fShape->GetBuffer3D(TBuffer3D::kCore|TBuffer3D::kBoundingBox|TBuffer3D::kShapeSpecific);
         Int_t reqSections = viewer3D->AddObject(buffer, &viewerWantsSons);
         if (reqSections != TBuffer3D::kNone)
         {
            fShape->GetBuffer3D(reqSections);
            viewer3D->AddObject(buffer);
         }
      }
   }
   if ( TestBit(kSonsInvisible) ) return;

//*-*- Paint all sons
   if(!nsons || !viewerWantsSons) return;

   gGeometry->PushLevel();
   TNode *node;
   TObject *obj;
   TIter  next(fNodes);
   while ((obj = next())) {
      node = (TNode*)obj;
      node->Paint(option);
   }
   gGeometry->PopLevel();
}

//______________________________________________________________________________
void TNode::RecursiveRemove(TObject *obj)
{
//*-*-*-*-*Recursively remove object from the list of nodes of this node*-*-*-*
//*-*      =============================================================

   if (fNodes && dynamic_cast<TNode*>(obj) ) fNodes->RecursiveRemove(obj);
}

//______________________________________________________________________________
void TNode::SetName(const char *name)
{
// Change the name of this Node
//
   if (gPad) gPad->Modified();

//  Nodes are named objects in a THashList.
//  We must update the hashlist if we change the name
   if (fParent) fParent->GetListOfNodes()->Remove(this);
   fName = name;
   if (fParent) fParent->GetListOfNodes()->Add(this);
}

//______________________________________________________________________________
void TNode::SetNameTitle(const char *name, const char *title)
{
// Change the name and title of this Node
//
   if (gPad) gPad->Modified();

//  Nodes are named objects in a THashList.
//  We must update the hashlist if we change the name
   if (fParent) fParent->GetListOfNodes()->Remove(this);
   fName  = name;
   fTitle = title;
   if (fParent) fParent->GetListOfNodes()->Add(this);
}

//______________________________________________________________________________
void TNode::SetParent(TNode *parent)
{
   // set the pointer to the parent, keep parents informed about who they have

   TNode *pp = parent;
   while(pp) {
     if (pp == this) {
       printf("Error: Cannot set parent node to be a child node:%s\n",GetName());
       printf("       Operation not performed!\n");
       return;
     }
     pp = pp->GetParent();
   }

   if (fParent)   fParent->GetListOfNodes()->Remove(this);
   else         gGeometry->GetListOfNodes()->Remove(this);

   fParent = parent;

   if (fParent) {
      fParent->BuildListOfNodes(); // new parent might not have list
      fParent->GetListOfNodes()->Add(this);
   }
   else gGeometry->GetListOfNodes()->Add(this);
}

//______________________________________________________________________________
void TNode::SetVisibility(Int_t vis)
{
//*-*-*-*-*-*-*Set visibility for this node and its sons*-*-*-*-*--*-*-*-*-*-*
//*-*          =========================================
//*-*  vis = 3  node is drawn and its sons are drawn
//*-*  vis = 2  node is not drawn but its sons are drawn
//*-*  vis = 1  (default) node is drawn
//*-*  vis = 0  node is not drawn
//*-*  vis = -1 node is not drawn. Its sons are not drawn
//*-*  vis = -2 node is drawn. Its sons are not drawn
//*-*  vis = -3 Only node leaves are drawn
//*-*  vis = -4 Node is not drawn. Its immediate sons are drawn
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

   ResetBit(kSonsInvisible);
   TNode *node;
   if (vis == -4 ) {         //Node is not drawn. Its immediate sons are drawn
      fVisibility = 0;
      if (!fNodes) { fVisibility = 1; return;}
      TIter  next(fNodes); while ((node = (TNode*)next())) { node->SetVisibility(-2); }
   } else if (vis == -3 ) {  //Only node leaves are drawn
      fVisibility = 0;
      if (!fNodes) { fVisibility = 1; return;}
      TIter  next(fNodes); while ((node = (TNode*)next())) { node->SetVisibility(-3); }

   } else if (vis == -2) {  //node is drawn. Its sons are not drawn
      fVisibility = 1; SetBit(kSonsInvisible); if (!fNodes) return;
      TIter  next(fNodes); while ((node = (TNode*)next())) { node->SetVisibility(-1); }

   } else if (vis == -1) {  //node is not drawn. Its sons are not drawn
      fVisibility = 0; SetBit(kSonsInvisible); if (!fNodes) return;
      TIter  next(fNodes); while ((node = (TNode*)next())) { node->SetVisibility(-1); }

   } else if (vis ==  0) {  //node is not drawn
      fVisibility = 0;

   } else if (vis ==  1) {  //node is drawn
      fVisibility = 1;

   } else if (vis ==  2) {  //node is not drawn but its sons are drawn
      fVisibility = 0; if (!fNodes) return;
      TIter  next(fNodes); while ((node = (TNode*)next())) { node->SetVisibility(3); }

   } else if (vis ==  3) {  //node is drawn and its sons are drawn
      fVisibility = 1; if (!fNodes) return;
      TIter  next(fNodes); while ((node = (TNode*)next())) { node->SetVisibility(3); }
   }
}

//______________________________________________________________________________
void TNode::Sizeof3D() const
{
//*-*-*-*-*-*-*Return total size of this 3-D Node with its attributes*-*-*
//*-*          ==========================================================

   if (fVisibility && fShape->GetVisibility()) {
      fShape->Sizeof3D();
   }
   if ( TestBit(kSonsInvisible) ) return;

   if (!fNodes) return;
   TNode *node;
   TObject *obj;
   TIter  next(fNodes);
   while ((obj = next())) {
      node = (TNode*)obj;
      node->Sizeof3D();
   }
}

//_______________________________________________________________________
void TNode::Streamer(TBuffer &b)
{
//*-*-*-*-*-*-*-*-*Stream a class object*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              =========================================
   if (b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = b.ReadVersion(&R__s, &R__c);
      if (R__v > 2) {
         TNode::Class()->ReadBuffer(b, this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(b);
      TAttLine::Streamer(b);
      TAttFill::Streamer(b);
      b >> fX;
      b >> fY;
      b >> fZ;
      b >> fMatrix;
      b >> fShape;
      b >> fParent;
      b >> fNodes;
      fOption.Streamer(b);
      if (R__v > 1) b >> fVisibility;
      else  fVisibility = fShape->GetVisibility();
      b.CheckByteCount(R__s, R__c, TNode::IsA());
      //====end of old versions

   } else {
      TNode::Class()->WriteBuffer(b,this);
   }
}


//______________________________________________________________________________
void TNode::UpdateMatrix()
{
//    Update global rotation matrix/translation vector for this node
//   this function must be called before invoking Local2Master

   TNode *nodes[kMAXLEVELS], *node;
   Int_t i;
   for (i=0;i<kVectorSize;i++) gTranslation[0][i] = 0;
   for (i=0;i<kMatrixSize;i++) gRotMatrix[0][i] = 0;
   gRotMatrix[0][0] = 1;   gRotMatrix[0][4] = 1;   gRotMatrix[0][8] = 1;

   node     = this;
   gGeomLevel  = 0;
   //build array of parent nodes
   while (node) {
      nodes[gGeomLevel] = node;
      node = node->GetParent();
      gGeomLevel++;
   }
   gGeomLevel--;
   //Update matrices in the hierarchy
   for (i=1;i<=gGeomLevel;i++) {
      node = nodes[gGeomLevel-i];
      UpdateTempMatrix(&(gTranslation[i-1][0]),&gRotMatrix[i-1][0]
                      ,node->GetX(),node->GetY(),node->GetZ(),node->GetMatrix()->GetMatrix()
                      ,&gTranslation[i][0],&gRotMatrix[i][0]);
   }
}

//______________________________________________________________________________
void TNode::UpdateTempMatrix(const Double_t *dx,const Double_t *rmat
                         , Double_t x, Double_t y, Double_t z, Double_t *matrix
                         , Double_t *dxnew, Double_t *rmatnew)
{
//*-*-*-*-*-*-*Compute new translation vector and global matrix*-*-*-*-*-*-*-*
//*-*          ================================================
//*-*
//*-*  dx      old translation vector
//*-*  rmat    old global matrix
//*-*  x,y,z   offset of new local system with respect to mother
//*-*  dxnew   new translation vector
//*-*  rmatnew new global rotation matrix
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-


   dxnew[0] = dx[0] + x*rmat[0] + y*rmat[3] + z*rmat[6];
   dxnew[1] = dx[1] + x*rmat[1] + y*rmat[4] + z*rmat[7];
   dxnew[2] = dx[2] + x*rmat[2] + y*rmat[5] + z*rmat[8];


   rmatnew[0] = rmat[0]*matrix[0] + rmat[3]*matrix[1] + rmat[6]*matrix[2];
   rmatnew[1] = rmat[1]*matrix[0] + rmat[4]*matrix[1] + rmat[7]*matrix[2];
   rmatnew[2] = rmat[2]*matrix[0] + rmat[5]*matrix[1] + rmat[8]*matrix[2];
   rmatnew[3] = rmat[0]*matrix[3] + rmat[3]*matrix[4] + rmat[6]*matrix[5];
   rmatnew[4] = rmat[1]*matrix[3] + rmat[4]*matrix[4] + rmat[7]*matrix[5];
   rmatnew[5] = rmat[2]*matrix[3] + rmat[5]*matrix[4] + rmat[8]*matrix[5];
   rmatnew[6] = rmat[0]*matrix[6] + rmat[3]*matrix[7] + rmat[6]*matrix[8];
   rmatnew[7] = rmat[1]*matrix[6] + rmat[4]*matrix[7] + rmat[7]*matrix[8];
   rmatnew[8] = rmat[2]*matrix[6] + rmat[5]*matrix[7] + rmat[8]*matrix[8];
}
