// @(#)root/geom:$Name:  $:$Id: TGeoPhysicalNode.cxx,v 1.3 2004/04/22 14:07:14 brun Exp $
// Author: Andrei Gheata   17/02/04

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// TGeoPhysicalNode
//_________

#include "TGeoManager.h"
#include "TGeoCache.h"
#include "TGeoMatrix.h"
#include "TGeoShape.h"
#include "TGeoVolume.h"
#include "TVirtualGeoPainter.h"

#include "TGeoPhysicalNode.h"

// statics and globals

ClassImp(TGeoPhysicalNode)

//_____________________________________________________________________________
TGeoPhysicalNode::TGeoPhysicalNode()
{
// Default constructor
   fLevel        = 0;
   fMatrices     = 0;
   fNodes        = 0;
   SetVisibility(kTRUE);
   SetVisibleFull(kFALSE);
   SetIsVolAtt(kTRUE);
   SetAligned(kFALSE);
}

//_____________________________________________________________________________
TGeoPhysicalNode::TGeoPhysicalNode(const char *path)
{
// Constructor
   if (!strlen(path)) {
      Error("ctor", "path not valid");
      return;
   }
   fLevel  = 0;
   fMatrices = new TObjArray(30);
   fNodes    = new TObjArray(30);
   SetPath(path);   
   SetVisibility(kTRUE);
   SetVisibleFull(kFALSE);
   SetIsVolAtt(kTRUE);
   SetAligned(kFALSE);
}

//_____________________________________________________________________________
TGeoPhysicalNode::~TGeoPhysicalNode()
{
// Destructor
   if (fMatrices) {
      fMatrices->Delete();
      delete fMatrices;
   }   
   if (fNodes) delete fNodes;
}

//_____________________________________________________________________________
void TGeoPhysicalNode::Align(TGeoMatrix *newmat, TGeoShape *newshape, Bool_t check)
{
   // Align a physical node with a new relative matrix/shape.
   // Example: /TOP_1/A_1/B_1/C_1
   //    node->Align(transl_1, box) will perform:
   //    - change RELATIVE translation of C_1 node (with respect to its
   //      container volume B) to transl_1
   //    - change the shape of the C volume
   // *NOTE* The operations will affect ONLY the LAST node in the branch. All
   //   volumes/nodes in the branch represented by this physical node are
   //   CLONED so the operation does not affect other possible replicas.
   if (!newmat && !newshape) return;
   TGeoNode *node = GetNode();
   if (node->IsOffset()) {
      Error("Align", "Cannot align division nodes: %s\n",node->GetName());
      return;
   }   
   TGeoNode *nnode = 0;
   TGeoVolume *vm = GetVolume(0);
   TGeoVolume *vd = 0;   
   Int_t i,id;
   if (!IsAligned()) {
      for (i=0; i<fLevel; i++) {
         // Get daughter node and its id inside vm
         node = GetNode(i+1);
         id = vm->GetIndex(node);
         if (id < 0) {
            Error("Align","cannot align node %s",GetNode(i+1)->GetName());
            return;
         }
         // Clone daughter volume and node
         vd = node->GetVolume()->CloneVolume();
         nnode = node->MakeCopyNode();
         // Correct pointers to mother and volume
         nnode->SetVolume(vd);
         nnode->SetMotherVolume(vm);
         // Decouple old node from mother volume and connect new one
         vm->GetNodes()->RemoveAt(id);
         vm->GetNodes()->AddAt(nnode,id);
         fNodes->RemoveAt(i+1);
         fNodes->AddAt(nnode,i+1);
         // Consider new cloned volume as mother and continue
         vm = vd;
      }
   } else {
      nnode = GetNode();
   }         
   // Now nnode is a cloned node of the one that need to be aligned
   TGeoNodeMatrix *aligned = (TGeoNodeMatrix*)nnode;
   vm = nnode->GetMotherVolume();
   vd = nnode->GetVolume();
   if (newmat) {
      // Register matrix and make it the active one
      if (!newmat->IsRegistered()) newmat->RegisterYourself();    
      aligned->SetMatrix(newmat);
      // Update the global matrix for the aligned node
      TGeoHMatrix *global = GetMatrix();
      TGeoHMatrix *up = GetMatrix(fLevel-1);
      *global = up;
      global->Multiply(newmat);
   }
   // Change the shape for the aligned node
   if (newshape) vd->SetShape(newshape);
   // Now we have to re-voxelize the mother volume
   vm->SetVoxelFinder(0);
   vm->Voxelize("ALL");
   vm->FindOverlaps(); 
   // Eventually check for overlaps
   if (check) vm->CheckOverlaps();
   SetAligned(kTRUE);
}   

//_____________________________________________________________________________
void TGeoPhysicalNode::cd() const
{

}

//_____________________________________________________________________________
void TGeoPhysicalNode::Draw(Option_t * /*option*/)
{

}

//_____________________________________________________________________________
TGeoNode *TGeoPhysicalNode::GetMother(Int_t levup) const
{
// Return parent at LEVUP generation
   Int_t ind = fLevel-levup;
   if (ind<0) return 0;
   return (TGeoNode*)fNodes->UncheckedAt(ind);
}   

//_____________________________________________________________________________
TGeoHMatrix *TGeoPhysicalNode::GetMatrix(Int_t level) const
{
// Return global matrix for node at LEVEL.
   if (level<0) return (TGeoHMatrix*)fMatrices->UncheckedAt(fLevel);
   if (level>fLevel) return 0;
   return (TGeoHMatrix*)fMatrices->UncheckedAt(level);
}

//_____________________________________________________________________________
const char *TGeoPhysicalNode::GetName() const
{
// Retreive the name of this physical node
   static TString name;
   name = "";
   for (Int_t level=0;level<fLevel+1; level++) {
      name += "/";
      name += GetNode(level)->GetName();
   }    
   return name.Data();  
}

//_____________________________________________________________________________
TGeoNode *TGeoPhysicalNode::GetNode(Int_t level) const
{
// Return node in branch at LEVEL. If not specified, return last leaf.
   if (level<0) return (TGeoNode*)fNodes->UncheckedAt(fLevel);
   if (level>fLevel) return 0;
   return (TGeoNode*)fNodes->UncheckedAt(level);
}   

//_____________________________________________________________________________
TGeoVolume *TGeoPhysicalNode::GetVolume(Int_t level) const
{
// Return volume associated with node at LEVEL in the branch
   TGeoNode *node = GetNode(level);
   if (node) return node->GetVolume();
   return 0;
}

//_____________________________________________________________________________
TGeoShape *TGeoPhysicalNode::GetShape(Int_t level) const
{
// Return shape associated with volume.
   TGeoVolume *vol = GetVolume(level);
   if (vol) return vol->GetShape();
   return 0;
}   

//_____________________________________________________________________________
void TGeoPhysicalNode::Paint(Option_t * /*option*/)
{
// Paint this node and its content according to visualization settings.
   TVirtualGeoPainter *painter = gGeoManager->GetGeomPainter();
   if (!painter) return;
//   painter->PaintNode(this, option);
}

//_____________________________________________________________________________
void TGeoPhysicalNode::SetBranchAsState()
{
// Set node branch according to current state
   TGeoNodeCache *cache = gGeoManager->GetCache();
   if (!cache) {
      Error("SetBranchAsState","no state available");
      return;
   }
   if (!cache->IsDummy()) {
      Error("SetBranchAsState", "not implemented for full cache");
      return;
   }   
   
   if (!fNodes)    fNodes = new TObjArray(30);
   if (!fMatrices) fMatrices = new TObjArray(30);
   fLevel = gGeoManager->GetLevel();   
   TGeoHMatrix **matrices = (TGeoHMatrix **) cache->GetMatrices();
   TGeoNode **branch = (TGeoNode **) cache->GetBranch();
   for (Int_t i=0; i<=fLevel; i++) {
      fNodes->AddAt(branch[i],i);
      fMatrices->AddAt(new TGeoHMatrix(*matrices[i]),i);
   }   
}

//_____________________________________________________________________________
Bool_t TGeoPhysicalNode::SetPath(const char *path)
{
// Specify the path for this node.
   if (!gGeoManager->cd(path)) {
      Error("SetPath","wrong path -> maybe RestoreMasterVolume");
      return kFALSE;
   }
   SetBranchAsState();
   return kTRUE;
}

