// @(#)root/table:$Name:  $:$Id: TIndexTable.cxx,v 1.3 2004/02/13 16:32:23 rdm Exp $
// Author: Valery Fine(fine@bnl.gov)   01/03/2001

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2001 [BNL] Brookhaven National Laboratory.              *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TIndexTable.h"

//////////////////////////////////////////////////////////////////////////////
// TIndexTable class is helper class to keep the list of the referencs to the
// TTable rows and iterate over it.
// TIndexTable is a persistent class.
// The pointer to the TIndexTable object may be used as an element
// of the TTable row and saved with the table all together.
//
// For example, the track table may contain a member to the "map" of the hits
//  struct {
//    float helix;
//    TIndexTable *hits;
//  } tracks_t;
//
//   // Create track table:
//   LArTrackTable *tracks = new LArTrackTable(...);
//
//   // Get pointer to the hit table
//   LArHitTable *hits = GiveMeHits();
//   // Loop over all tracks
//   LArTrackTable::iterator track = tracks->begin();
//   LArTrackTable::iterator last = tracks->end();
//   for (;track != last;track++) {
//     // Find all hits of this track
//      LArHitTable::iterator hit     = hits->begin();
//      LArHitTable::iterator lastHit = hits->end();
//      Long_t hitIndx = 0;
//      // Create an empty list of this track hits
//      (*track).hits = new TIndexTable(hits);
//      for(;hit != lastHit;hit++,hitIndx) {
//        if (IsMyHit(*hit)) {  // add this hit index to the current track
//           (*track).hits->push_back(hitIndx);
//        }
//      }
//   }
//___________________________________________________________________

// TableClassImpl(TIndexTable,int);
  TTableDescriptor *TIndexTable::fgColDescriptors = TIndexTable::CreateDescriptor();
  ClassImp(TIndexTable)
#if 0
  void TIndexTable::Dictionary()
   {
      TClass *c = CreateClass(_QUOTE_(className), Class_Version(),
                              DeclFileName(), ImplFileName(),
                              DeclFileLine(), ImplFileLine());

      char *structBuf = new char[strlen(_QUOTE2_(structName,.h))+2];
      strcpy(structBuf,_QUOTE2_(structName,.h));
      char *s = strstr(structBuf,"_st.h");
      if (s) { *s = 0;  strcat(structBuf,".h"); }
      TClass *r = CreateClass(_QUOTE_(structName), Class_Version(),
                              structBuf, structBuf, 1,  1 );
      fgIsA = c;
      fgColDescriptors = new TTableDescriptor(r);
   }
   _TableClassImp_(TIndexTable,int)
#endif
  TableClassStreamerImp(TIndexTable)

//___________________________________________________________________
     TIndexTable::TIndexTable(const TTable *table):TTable("Index",-1), fRefTable(table)
{
   if (!fgColDescriptors)    CreateDescriptor();
   fSize = fgColDescriptors->Sizeof();
  // Add refered table to this index.
   // yf  if (table) Add((TDataSet *)table);
}
//___________________________________________________________________
TTableDescriptor *TIndexTable::CreateDescriptor()
{
  if (!fgColDescriptors) {
     // Set an empty descriptor
     fgColDescriptors= new TTableDescriptor("int");
      // Create one
     if (fgColDescriptors) {
       TTableDescriptor  &dsc = *fgColDescriptors;
       tableDescriptor_st row;

       memset(&row,0,sizeof(row));
       strncpy(row.fColumnName,"index",sizeof(row.fColumnName));

       row.fType = kInt;
       row.fTypeSize = sizeof(Int_t);
       row.fSize = row.fTypeSize;
       dsc.AddAt(&row);
     }
  }
  return fgColDescriptors;
}

//___________________________________________________________________
const TTable *TIndexTable::Table() const
{
  return fRefTable;
}
